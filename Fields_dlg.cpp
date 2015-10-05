// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// Fields_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "Fields_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static char (&buf)[sizeof CPromDoc::buf]=CPromDoc::buf;


/////////////////////////////////////////////////////////////////////////////
// CFields_dlg dialog

CFields_dlg::CFields_dlg(Database& d, CWnd *pParent)
	: CDialog(CFields_dlg::IDD, pParent),data(d),changed(false)
{
  //{{AFX_DATA_INIT(CFields_dlg)
  m_fld_name = _T("");
  m_type = -1;
  //}}AFX_DATA_INIT
}


void CFields_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CFields_dlg)
  DDX_Control(pDX, IDC_FIELD_LIST, m_field_list);
  DDX_Text(pDX, IDC_FLD_NAME, m_fld_name);
  DDX_Radio(pDX, IDC_NFT_INTEGER, m_type);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFields_dlg, CDialog)
  //{{AFX_MSG_MAP(CFields_dlg)
  ON_BN_CLICKED(IDC_ADD, OnAdd)
  ON_BN_CLICKED(IDC_REMOVE, OnRemove)
  ON_BN_CLICKED(IDC_RENAME, OnRename)
  ON_BN_CLICKED(IDC_UP, OnUp)
  ON_BN_CLICKED(IDC_DOWN, OnDown)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFields_dlg message handlers

void CFields_dlg::OnAdd()
{
  UpdateData();
  if (m_fld_name.IsEmpty()) {
    MessageBox("Field name must be nonempty.");
    return;
  }
  if (data.valid_field_name(m_fld_name)) {
    MessageBox("There is already a field by the name you specified.");
    return;
  }
  switch (m_type) {
  case 0: data.add_field(m_fld_name,FLD_INTEGER); break;
  case 1: data.add_field(m_fld_name,FLD_REAL); break;
  case 2: data.add_field(m_fld_name,FLD_STRING); break;
  case 3: data.add_field(m_fld_name,FLD_LOC); break;
  default:
    MessageBox("Please specify a field type.");
    return;
  }
  sprintf(buf,"%s\t%c",(const char *)m_fld_name,Field::ft_char[m_type]);
  m_field_list.AddString(buf);
  changed=true;
}

void CFields_dlg::OnRename()
{
  UpdateData();
  if (m_fld_name.IsEmpty()) {
    MessageBox("Field name must be nonempty.");
    return;
  }
  if (data.valid_field_name(m_fld_name)) {
    MessageBox("There is already a field by the name you specified.");
    return;
  }
  int sel_fld=m_field_list.GetCurSel();
  if (sel_fld<0) {
    MessageBox("Select a field to rename.");
    return;
  }
  Database::FieldID fid=data.begin_field()+sel_fld;
  (*fid).name=m_fld_name;
  m_field_list.DeleteString(sel_fld);
  sprintf(buf,"%s\t%c",(const char *)(*fid).name,
	  Field::ft_char[(*fid).get_type()-1]);
  m_field_list.InsertString(sel_fld,buf);
  changed=true;
}

void CFields_dlg::swap(int up_fld)
{
  Database::FieldID fid=data.begin_field()+up_fld;
  data.swap_field(fid);
  m_field_list.DeleteString(up_fld);
  sprintf(buf,"%s\t%c",(const char *)(*(fid-1)).name,
	  Field::ft_char[(*(fid-1)).get_type()-1]);
  m_field_list.InsertString(up_fld-1,buf);
  changed=true;
}

void CFields_dlg::OnUp()
{
  int sel_fld=m_field_list.GetCurSel();
  if (sel_fld<=0) {
    MessageBox("Select a field other than the top one.");
    return;
  }
  swap(sel_fld);
}

void CFields_dlg::OnDown()
{
  int sel_fld=m_field_list.GetCurSel();
  if (sel_fld<0 || sel_fld+1>=data.get_nfield()) {
    MessageBox("Select a field other than the bottom one.");
    return;
  }
  swap(sel_fld+1);
}

void CFields_dlg::OnRemove()
{
  int sel_fld=m_field_list.GetCurSel();
  if (sel_fld<0) {
    MessageBox("No field selected.");
    return;
  }
  Database::FieldID fid=data.begin_field()+sel_fld;
  if (!(*fid).is_virgin())
    if (MessageBox("Are you sure you want to remove that field?",NULL,
		   MB_YESNO|MB_DEFBUTTON2)!=IDYES) return;
  m_field_list.DeleteString(sel_fld);
  data.remove_field(fid);
  changed=true;
}

BOOL CFields_dlg::OnInitDialog() 
{
  CDialog::OnInitDialog();

  m_field_list.SetTabStops(80);
  for (Database::const_FieldID fid=data.begin_field();
       fid!=data.end_field(); ++fid) {
    sprintf(buf,"%s\t%c",(const char *)(*fid).name,
	    Field::ft_char[(*fid).get_type()-1]);
    m_field_list.AddString(buf);
  }

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}

void CFields_dlg::OnOK() 
{
  EndDialog(changed?IDOK:IDCANCEL);
}
