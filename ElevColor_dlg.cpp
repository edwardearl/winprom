// ElevColor_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "ElevColor_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CElevColor_dlg dialog

CElevColor_dlg::CElevColor_dlg(Elev_color_intv& eci, CWnd* pParent /*=NULL*/)
  : CDialog(CElevColor_dlg::IDD, pParent),elev_color_intv(eci)
{
  //{{AFX_DATA_INIT(CElevColor_dlg)
  m_elev = 0;
  m_interp = FALSE;
  //}}AFX_DATA_INIT
}

void CElevColor_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CElevColor_dlg)
  DDX_Text(pDX, IDC_ELEV, m_elev);
  DDV_MinMaxInt(pDX, m_elev, -16383, 32767);
  DDX_Check(pDX, IDC_INTERP, m_interp);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CElevColor_dlg, CDialog)
  //{{AFX_MSG_MAP(CElevColor_dlg)
  ON_BN_CLICKED(IDC_COLOR, OnColor)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CElevColor_dlg message handlers

BOOL CElevColor_dlg::OnInitDialog()
{
  m_interp=elev_color_intv.interp;
  m_elev=elev_color_intv.elev;
  m_color=elev_color_intv.color;

  CDialog::OnInitDialog();

  m_color_box.SubclassDlgItem(IDC_COLOR_BOX,this);
  m_color_box.set_color(m_color);

  return TRUE;  // return TRUE unless you set the focus to a control
}

void CElevColor_dlg::OnColor()
{
  CColorDialog dlg(m_color);
  if (dlg.DoModal()!=IDOK) return;

  m_color=dlg.GetColor();
  m_color_box.set_color(m_color);
}

void CElevColor_dlg::OnOK()
{
  CDialog::OnOK();
  elev_color_intv.interp=m_interp!=FALSE;
  elev_color_intv.elev=m_elev;
  elev_color_intv.color=m_color;
}
