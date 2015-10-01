// Purge_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "Purge_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPurge_dlg dialog

CPurge_dlg::CPurge_dlg(UINT rcTpl, UINT icon_id)
  : CDialog(rcTpl,NULL),feature_icon_id(icon_id)
{
  init_data();
}

CPurge_dlg::CPurge_dlg(UINT icon_id)
  : CDialog(CPurge_dlg::IDD,NULL),feature_icon_id(icon_id)
{
  init_data();
}

void CPurge_dlg::init_data()
{
  //{{AFX_DATA_INIT(CPurge_dlg)
  m_prom = 0;
  m_keep_name = FALSE;
  m_keep_edit = FALSE;
  m_ridge = FALSE;
  m_cell = FALSE;
  //}}AFX_DATA_INIT
}


void CPurge_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPurge_dlg)
  DDX_Text(pDX, IDC_PROM, m_prom);
  DDX_Check(pDX, IDC_NAME, m_keep_name);
  DDX_Check(pDX, IDC_EDITED, m_keep_edit);
  DDX_Check(pDX, IDC_RIDGE, m_ridge);
  DDX_Check(pDX, IDC_CELL, m_cell);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPurge_dlg, CDialog)
  //{{AFX_MSG_MAP(CPurge_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPurge_dlg message handlers

BOOL CPurge_dlg::OnInitDialog() 
{
  CDialog::OnInitDialog();

  if (feature_icon_id) SetIcon(AfxGetApp()->LoadIcon(feature_icon_id),TRUE);

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}
