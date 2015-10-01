// Relief_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "Relief_dlg.h"
#include "TransPal_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int CColorIntervalBox::CompareItem(LPCOMPAREITEMSTRUCT lpCI)
{
  int elev_diff=
    ((const Elev_color_intv *)(lpCI->itemData1))->elev-
    ((const Elev_color_intv *)(lpCI->itemData2))->elev;
  if (elev_diff>0) return 1;
  else if (elev_diff<0) return -1;
  return 0;
}

void CColorIntervalBox::DrawItem(LPDRAWITEMSTRUCT lpDI)
{
  assert(lpDI->CtlType==ODT_LISTBOX);
  if (lpDI->itemID>GetCount()) return;
  // background
  HBRUSH hbr=CreateSolidBrush(GetSysColor(
    lpDI->itemState&ODS_SELECTED?COLOR_HIGHLIGHT:COLOR_WINDOW));
  FillRect(lpDI->hDC,&lpDI->rcItem,hbr);
  DeleteObject(hbr);
  if (lpDI->itemAction&ODA_FOCUS)
    DrawFocusRect(lpDI->hDC,&lpDI->rcItem);
  // text
  char text[8];
  const Elev_color_intv& eci=get_eci(lpDI->itemID);
  sprintf(text,"%d ",eci.elev);
  SetBkMode(lpDI->hDC,TRANSPARENT);
  DrawText(lpDI->hDC,text,-1,&lpDI->rcItem,DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
  // color box
  hbr=CreateSolidBrush(eci.color);
  HGDIOBJ hbr_old=SelectObject(lpDI->hDC,hbr);
  int rect_right=lpDI->rcItem.left+lpDI->rcItem.bottom-lpDI->rcItem.top-2;
  Rectangle(lpDI->hDC,
	    lpDI->rcItem.left+2,lpDI->rcItem.top+2, // left,top
	    rect_right,lpDI->rcItem.bottom-2); // right,bottom
  if (eci.interp) {
    MoveToEx(lpDI->hDC,lpDI->rcItem.left+2,lpDI->rcItem.top+2,NULL); // left,top
    LineTo(lpDI->hDC,rect_right,lpDI->rcItem.bottom-2); // right,bottom
  }
  SelectObject(lpDI->hDC,hbr_old);
  DeleteObject(hbr);
}

void CColorIntervalBox::DeleteItem(LPDELETEITEMSTRUCT lpDI)
{
  delete (Elev_color_intv *)(lpDI->itemData);
}


/////////////////////////////////////////////////////////////////////////////
// CRelief_dlg dialog

void CRelief_dlg::interpolate_all(bool b)
{
  int nitem=m_colorintv_box.GetCount(),item;
  for (item=0; item<nitem; ++item)
    m_colorintv_box.get_eci(item).interp=b;
  m_colorintv_box.Invalidate();
}

void CRelief_dlg::set_interp_butn()
{
  int nitem=m_colorintv_box.GetCount(),item;
  if (nitem==0) return;
  bool b=m_colorintv_box.get_eci(0).interp;
  for (item=1; item<nitem; ++item) {
    if (m_colorintv_box.get_eci(item).interp!=b) {
      m_interpolate_butn.SetCheck(2);
      return;
    }
  }
  m_interpolate_butn.SetCheck(b);
}

CRelief_dlg::CRelief_dlg(vector<Elev_color_intv>& ec, CWnd* pParent /*=NULL*/)
  : CDialog(CRelief_dlg::IDD, pParent),elev_colors(ec)
{
  //{{AFX_DATA_INIT(CRelief_dlg)
  m_intensity = 0.0f;
  m_direction = 0.0f;
  m_color_enable = FALSE;
  //}}AFX_DATA_INIT
}

void CRelief_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CRelief_dlg)
  DDX_Control(pDX, IDC_INTERP, m_interpolate_butn);
  DDX_Text(pDX, IDC_INTENSITY, m_intensity);
  DDX_Text(pDX, IDC_DIRECTION, m_direction);
  DDV_MinMaxFloat(pDX, m_direction, 0.f, 360.f);
  DDX_Check(pDX, IDC_COLOR_ENABLE, m_color_enable);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRelief_dlg, CDialog)
  //{{AFX_MSG_MAP(CRelief_dlg)
  ON_BN_CLICKED(IDC_ADD, OnAdd)
  ON_BN_CLICKED(IDC_DELETE, OnDelete)
  ON_BN_CLICKED(IDC_MODIFY, OnModify)
  ON_BN_CLICKED(IDC_TRANSFORM, OnTransform)
  ON_BN_CLICKED(IDC_INTERP, OnInterpolate)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRelief_dlg message handlers

BOOL CRelief_dlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_colorintv_box.SubclassDlgItem(IDC_COLOR_INTV,this);
  for (vector<Elev_color_intv>::iterator eci=elev_colors.begin();
       eci!=elev_colors.end(); ++eci)
    m_colorintv_box.AddString((const char *)new Elev_color_intv(*eci));
  set_interp_butn();

  return TRUE;  // return TRUE unless you set the focus to a control
}

void CRelief_dlg::OnAdd()
{
  Elev_color_intv eci;
  int nitem=m_colorintv_box.GetCount();
  if (nitem==0) {
    eci.elev=0;
    eci.color=RGB(0,255,0);
    eci.interp=false;
  }
  else {
    const Elev_color_intv& eci1=m_colorintv_box.get_eci(nitem-1);
    eci.interp=eci1.interp;
    if (nitem==1) {
      eci.elev=eci1.elev+1000;
      eci.color=eci1.color;
    }
    else {
      const Elev_color_intv& eci2=m_colorintv_box.get_eci(nitem-2);
      eci.elev=eci1.elev-eci2.elev+eci1.elev; // two separate eci1.elev terms avoids overflow
      short int mv,
	rv=2*GetRValue(eci1.color)-GetRValue(eci2.color),
	gv=2*GetGValue(eci1.color)-GetGValue(eci2.color),
	bv=2*GetBValue(eci1.color)-GetBValue(eci2.color);
      if (rv<0) rv=0;
      if (gv<0) gv=0;
      if (bv<0) bv=0;
      mv=rv;
      if (gv>mv) mv=gv;
      if (bv>mv) mv=bv;
      if (mv>255) {
	rv=255*rv/mv;
	gv=255*gv/mv;
	bv=255*bv/mv;
      }
      eci.color=RGB(rv,gv,bv);
    }
  }
  CElevColor_dlg dlg(eci);
  if (dlg.DoModal()!=IDOK) return;

  int new_item=m_colorintv_box.AddString((const char *)new Elev_color_intv(eci)),
    topvis=m_colorintv_box.GetTopIndex();
  if (new_item<topvis) m_colorintv_box.SetTopIndex(new_item);
  else if (new_item>topvis+5) m_colorintv_box.SetTopIndex(new_item-5);
  m_colorintv_box.SetCurSel(new_item);
  set_interp_butn();
}

void CRelief_dlg::OnDelete()
{
  int item=m_colorintv_box.GetCurSel();
  if (item==LB_ERR) {
    MessageBox("Select an item to delete.",NULL,MB_OK|MB_ICONERROR);
    return;
  }
  m_colorintv_box.DeleteString(item);
  set_interp_butn();
}

void CRelief_dlg::OnModify()
{
  int item=m_colorintv_box.GetCurSel();
  if (item==LB_ERR) {
    MessageBox("Select an item to modify.",NULL,MB_OK|MB_ICONERROR);
    return;
  }
  Elev_color_intv eci=m_colorintv_box.get_eci(item);
  CElevColor_dlg dlg(eci);
  if (dlg.DoModal()!=IDOK) return;

  m_colorintv_box.DeleteString(item);
  m_colorintv_box.AddString((const char *)new Elev_color_intv(eci));
  set_interp_butn();
}

void CRelief_dlg::OnOK()
{
  CDialog::OnOK();
  int nitem=m_colorintv_box.GetCount(),item;
  elev_colors.resize(nitem);
  for (item=0; item<nitem; ++item)
    elev_colors[item]=m_colorintv_box.get_eci(item);
}

void CRelief_dlg::OnTransform()
{
  int nitem=m_colorintv_box.GetCount(),item;
  if (nitem<2) {
    MessageBox("The elevation color palette must have at least 2 items.",
	       NULL,MB_OK|MB_ICONERROR);
    return;
  }
  Elevation low_old=m_colorintv_box.get_eci(0).elev,
    high_old=m_colorintv_box.get_eci(nitem-1).elev;
  if (low_old>=high_old) {
    MessageBox("The elevation color palette must represent a nonzero elevation range.",
	       NULL,MB_OK|MB_ICONERROR);
    return;
  }
  CTransPal_dlg dlg;
  dlg.m_low_old.Format("%d -->",low_old);
  dlg.m_low_new=low_old;
  dlg.m_high_old.Format("%d -->",high_old);
  dlg.m_high_new=high_old;
  if (dlg.DoModal()!=IDOK) return;
  float m=(dlg.m_high_new-dlg.m_low_new)/(float)(high_old-low_old);
  for (item=0; item<nitem; ++item) {
    Elev_color_intv& eci=m_colorintv_box.get_eci(item);
    eci.elev=m*(eci.elev-low_old)+dlg.m_low_new;
  }
  m_colorintv_box.Invalidate();
}

void CRelief_dlg::OnInterpolate()
{
  int bstate=m_interpolate_butn.GetCheck();
  if (bstate<2) interpolate_all(bstate!=0);
}
