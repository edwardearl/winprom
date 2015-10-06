// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// PurgeDomain_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "PurgeDomain_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPurgeDomain_dlg dialog

CPurgeDomain_dlg::CPurgeDomain_dlg(CWnd* pParent /*=NULL*/)
  : CDialog(CPurgeDomain_dlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CPurgeDomain_dlg)
  m_offmap = TRUE;
  m_prom = 0;
  m_rotate = TRUE;
  //}}AFX_DATA_INIT
}

void CPurgeDomain_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPurgeDomain_dlg)
  DDX_Check(pDX, IDC_OFFMAP, m_offmap);
  DDX_Text(pDX, IDC_PROM, m_prom);
  DDX_Check(pDX, IDC_ROTATION, m_rotate);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPurgeDomain_dlg, CDialog)
  //{{AFX_MSG_MAP(CPurgeDomain_dlg)
    // NOTE: the ClassWizard will add message map macros here
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPurgeDomain_dlg message handlers
