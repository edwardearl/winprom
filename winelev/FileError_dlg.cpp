// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// FileError_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winelev.h"
#include "FileError_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFileError_dlg dialog

CFileError_dlg::CFileError_dlg()
  : CDialog(CFileError_dlg::IDD,NULL)
{
  //{{AFX_DATA_INIT(CFileError_dlg)
  m_future = FALSE;
  m_message = _T("");
  //}}AFX_DATA_INIT
  m_future=TRUE;
}

void CFileError_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CFileError_dlg)
  DDX_Check(pDX, IDC_FUTURE, m_future);
  DDX_Text(pDX, IDC_MESSAGE, m_message);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFileError_dlg, CDialog)
  //{{AFX_MSG_MAP(CFileError_dlg)
  ON_BN_CLICKED(IDC_ABORT, OnAbort)
  ON_BN_CLICKED(IDC_RETRY, OnRetry)
  ON_BN_CLICKED(IDC_SKIP, OnSkip)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFileError_dlg message handlers

void CFileError_dlg::OnAbort() 
{
  EndDialog(IDC_ABORT);
}

void CFileError_dlg::OnRetry() 
{
  EndDialog(IDC_RETRY);
}

void CFileError_dlg::OnSkip() 
{
  UpdateData(TRUE);
  EndDialog(IDC_SKIP);
}
