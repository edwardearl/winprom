// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// BdyWarn_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winelev.h"
#include "BdyWarn_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CBdyWarn_dlg dialog

CBdyWarn_dlg::CBdyWarn_dlg(CWnd* pParent /*=NULL*/)
  : CDialog(CBdyWarn_dlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CBdyWarn_dlg)
  m_file1 = _T("");
  m_file2 = _T("");
  m_reprompt = FALSE;
  m_warn_type = _T("");
  //}}AFX_DATA_INIT
}

void CBdyWarn_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CBdyWarn_dlg)
  DDX_Text(pDX, IDC_FILE1, m_file1);
  DDX_Text(pDX, IDC_FILE2, m_file2);
  DDX_Check(pDX, IDC_REPROMPT, m_reprompt);
  DDX_Text(pDX, IDC_WARN_TYPE, m_warn_type);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBdyWarn_dlg, CDialog)
  //{{AFX_MSG_MAP(CBdyWarn_dlg)
    // NOTE: the ClassWizard will add message map macros here
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBdyWarn_dlg message handlers
