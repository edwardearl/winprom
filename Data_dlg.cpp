// Data_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "Data_dlg.h"
#include "DataListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_FIELD_BASE 128

static char (&buf)[sizeof CDataListView::buf]=CDataListView::buf;


/////////////////////////////////////////////////////////////////////////////
// CData_dlg dialog

CData_dlg::CData_dlg(CPromDoc& doc, const GridPoint& gp, CWnd *pParent)
  : CDialog(CData_dlg::IDD, pParent),document(doc),
    target(gp),record(doc.data.get_record(gp))
{
  //{{AFX_DATA_INIT(CData_dlg)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

CData_dlg::CData_dlg(CPromDoc& doc, Database::RecordID rec, CWnd *pParent)
  : CDialog(CData_dlg::IDD, pParent),document(doc),
    target(doc.data.get_location(rec)),record(rec)
{
}


void CData_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CData_dlg)
  DDX_Control(pDX, IDC_VIEW, m_view_butn);
  DDX_Control(pDX, IDC_DELETE, m_delete_butn);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CData_dlg, CDialog)
  //{{AFX_MSG_MAP(CData_dlg)
  ON_BN_CLICKED(IDC_FIELDS, OnFields)
  ON_BN_CLICKED(IDC_DELETE, OnDelete)
  ON_BN_CLICKED(IDC_VIEW, OnView)
  ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
  //}}AFX_MSG_MAP
  ON_EN_CHANGE(IDC_FIELD_BASE+0,OnChangeField)
  ON_EN_CHANGE(IDC_FIELD_BASE+1,OnChangeField)
  ON_EN_CHANGE(IDC_FIELD_BASE+2,OnChangeField)
  ON_EN_CHANGE(IDC_FIELD_BASE+3,OnChangeField)
  ON_EN_CHANGE(IDC_FIELD_BASE+4,OnChangeField)
  ON_EN_CHANGE(IDC_FIELD_BASE+5,OnChangeField)
  ON_EN_CHANGE(IDC_FIELD_BASE+6,OnChangeField)
  ON_EN_CHANGE(IDC_FIELD_BASE+7,OnChangeField)
  ON_EN_CHANGE(IDC_FIELD_BASE+8,OnChangeField)
  ON_EN_CHANGE(IDC_FIELD_BASE+9,OnChangeField)
  ON_EN_CHANGE(IDC_FIELD_BASE+10,OnChangeField)
  ON_EN_CHANGE(IDC_FIELD_BASE+11,OnChangeField)
  ON_EN_CHANGE(IDC_FIELD_BASE+12,OnChangeField)
  ON_EN_CHANGE(IDC_FIELD_BASE+13,OnChangeField)
  ON_EN_CHANGE(IDC_FIELD_BASE+14,OnChangeField)
  ON_EN_CHANGE(IDC_FIELD_BASE+15,OnChangeField)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CData_dlg message handlers

BOOL CData_dlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  if (record==REC_NONE) m_delete_butn.EnableWindow(FALSE);

  CRect win_rect,name_rect,type_rect,value_rect;
  GetWindowRect(&win_rect);
  unsigned fld_ndx=0;
  int ivalue;
  float rvalue;
  const char *svalue;
  GridPoint lvalue;

  win_rect.bottom+=4;
  value_rect.top=win_rect.bottom-win_rect.top-28;
  type_rect.top=name_rect.top=value_rect.top+2;
  name_rect.left=10;
  type_rect.bottom=name_rect.bottom=value_rect.bottom=value_rect.top+18;
  name_rect.right=name_rect.left+152;
  type_rect.left=name_rect.right+8;
  type_rect.right=type_rect.left+8;
  value_rect.left=type_rect.right+8;
  value_rect.right=value_rect.left+120;
  for (Database::const_FieldID fid=document.data.begin_field();
       fid!=document.data.end_field(); ++fid,++fld_ndx) {
    name_label[fld_ndx].Create((*fid).name,WS_CHILD|WS_VISIBLE,name_rect,this);
    name_label[fld_ndx].SetFont(GetFont(),FALSE);
    buf[0]=Field::ft_char[(*fid).get_type()-1];
    buf[1]='\0';
    type_label[fld_ndx].Create(buf,WS_CHILD|WS_VISIBLE,type_rect,this);
    type_label[fld_ndx].SetFont(GetFont(),FALSE);
    value_label[fld_ndx].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
      value_rect,this,IDC_FIELD_BASE+fld_ndx);
    value_label[fld_ndx].ModifyStyleEx(0,WS_EX_CLIENTEDGE,SWP_DRAWFRAME);
    value_label[fld_ndx].SetFont(GetFont(),FALSE);
    switch ((*fid).get_type()) {
    case FLD_INTEGER:
      if (record==REC_NONE) ivalue=0;
      else document.data.get_value(record,fid,ivalue);
      sprintf(buf,"%d",ivalue);
      value_label[fld_ndx].SetWindowText(buf);
      break;
    case FLD_REAL:
      if (record==REC_NONE) rvalue=0.0;
      else document.data.get_value(record,fid,rvalue);
      sprintf(buf,"%g",rvalue);
      value_label[fld_ndx].SetWindowText(buf);
      break;
    case FLD_STRING:
      if (record==REC_NONE) svalue="";
      else document.data.get_value(record,fid,svalue);
      value_label[fld_ndx].SetWindowText(svalue);
      break;
    case FLD_LOC:
      if (record!=REC_NONE) {
	document.data.get_value(record,fid,lvalue);
	if (lvalue) lvalue.sprint(buf);
	else buf[0]='\0';
	value_label[fld_ndx].SetWindowText(buf);
      }
      break;
    default:
      assert(0);
    }
    value_rect.top+=24;
    name_rect.top+=24;
    type_rect.top+=24;
    value_rect.bottom+=24;
    name_rect.bottom+=24;
    type_rect.bottom+=24;
    win_rect.bottom+=24;
  }
  MoveWindow(win_rect);

  changed=false;

  return TRUE;
}

void CData_dlg::OnChangeField()
{
  changed=true;
}

void CData_dlg::OnFields()
{
  if (changed) {
    switch (MessageBox("Accept changes made so far?","Winprom Data",
		       MB_YESNOCANCEL|MB_DEFBUTTON1)) {
    case IDYES:
      if (!save_records()) return;
      break;
    case IDNO:
      changed=false;
      break;
    case IDCANCEL:
      return;
    }
  }
  EndDialog(IDC_FIELDS);
}

void CData_dlg::OnOK()
{
  if (save_records()) CDialog::OnOK();
}

bool CData_dlg::save_records()
{
  CString value_text;
  unsigned fld_ndx=0;
  int ivalue;
  float rvalue;
  char *endp;
  Database::FieldID fid;

  // first pass over fields -- validate them
  for (fid=document.data.begin_field();
       fid!=document.data.end_field(); ++fid,++fld_ndx) {
    value_label[fld_ndx].GetWindowText(value_text);
    switch ((*fid).get_type()) {
    case FLD_INTEGER:
      ivalue=strtol(value_text,&endp,10);
      if (*endp!='\0') {
	sprintf(buf,"Integer format error for field \"%s\"",
		std::string((*fid).name).c_str());
	MessageBox(buf);
	value_label[fld_ndx].SetFocus();
	value_label[fld_ndx].SetSel(0,-1);
	return false;
      }
      break;
    case FLD_REAL:
      rvalue=strtod(value_text,&endp);
      if (*endp!='\0') {
	sprintf(buf,"Real format error for field \"%s\"",
		std::string((*fid).name).c_str());
	MessageBox(buf);
	value_label[fld_ndx].SetFocus();
	value_label[fld_ndx].SetSel(0,-1);
	return false;
      }
      break;
    }
  }

  if (record==REC_NONE)
    record=document.data.add_record(target);

  // second pass over fields -- save their new values
  for (fld_ndx=0,fid=document.data.begin_field();
       fid!=document.data.end_field(); ++fid,++fld_ndx) {
    value_label[fld_ndx].GetWindowText(value_text);
    switch ((*fid).get_type()) {
    case FLD_INTEGER:
      document.data.set_value(record,fid,atoi(value_text));
      break;
    case FLD_REAL:
      document.data.set_value(record,fid,(float)atof(value_text));
      break;
    case FLD_STRING:
      document.data.set_value(record,fid,value_text);
      break;
    }
  }

  return true;
}

void CData_dlg::OnDelete()
{
  EndDialog(IDC_DELETE);
}

void CData_dlg::OnView()
{
  CPromApp::getApp()->viewLocation(target);
}

void CData_dlg::OnBrowse()
{
  CPromApp::getApp()->Navigate(target);
}
