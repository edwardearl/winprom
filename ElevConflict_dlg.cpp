// ElevConflict_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "ElevConflict_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CElevConflict_dlg dialog

CElevConflict_dlg::CElevConflict_dlg(CWnd* pParent /*=NULL*/)
  : CDialog(CElevConflict_dlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CElevConflict_dlg)
  m_location = _T("");
  m_warn = FALSE;
  m_workspace = 0;
  m_file = 0;
  m_use = -1;
  //}}AFX_DATA_INIT
}


void CElevConflict_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CElevConflict_dlg)
  DDX_Text(pDX, IDC_LOCATION, m_location);
  DDX_Check(pDX, IDC_WARN, m_warn);
  DDX_Text(pDX, IDC_WORKSPACE, m_workspace);
  DDX_Text(pDX, IDC_FILE, m_file);
  DDX_Radio(pDX, IDC_USE_WORKSPACE, m_use);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CElevConflict_dlg, CDialog)
  //{{AFX_MSG_MAP(CElevConflict_dlg)
  ON_BN_CLICKED(IDC_ABORT, OnAbort)
  ON_BN_CLICKED(IDC_CONTINUE, OnContinue)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CElevConflict_dlg message handlers

void CElevConflict_dlg::OnAbort() 
{
  EndDialog(IDC_ABORT);
}

void CElevConflict_dlg::OnContinue() 
{
  UpdateData();
  EndDialog(IDC_CONTINUE);
}
