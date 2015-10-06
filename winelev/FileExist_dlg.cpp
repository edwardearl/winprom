// FileExist_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winelev.h"
#include "FileExist_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFileExist_dlg dialog

CFileExist_dlg::CFileExist_dlg()
  : CDialog(CFileExist_dlg::IDD,NULL)
{
  //{{AFX_DATA_INIT(CFileExist_dlg)
  m_future = FALSE;
  m_filename = _T("");
  //}}AFX_DATA_INIT
  m_future=TRUE;
}

void CFileExist_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CFileExist_dlg)
  DDX_Check(pDX, IDC_FUTURE, m_future);
  DDX_Text(pDX, IDC_FILENAME, m_filename);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFileExist_dlg, CDialog)
  //{{AFX_MSG_MAP(CFileExist_dlg)
  ON_BN_CLICKED(IDC_ABORT, OnAbort)
  ON_BN_CLICKED(IDC_OVERWRITE, OnOverwrite)
  ON_BN_CLICKED(IDC_SKIP, OnSkip)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFileExist_dlg message handlers

void CFileExist_dlg::OnAbort()
{
  EndDialog(IDC_ABORT);
}

void CFileExist_dlg::OnOverwrite()
{
  UpdateData(TRUE);
  EndDialog(IDC_OVERWRITE);
}

void CFileExist_dlg::OnSkip()
{
  UpdateData(TRUE);
  EndDialog(IDC_SKIP);
}
