// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// PurgePeak_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "PurgePeak_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPurgePeak_dlg dialog

CPurgePeak_dlg::CPurgePeak_dlg(bool drainage, CWnd* pParent /*=NULL*/)
  : CPurge_dlg(CPurgePeak_dlg::IDD,drainage?IDI_BASIN:IDI_PEAK)
{
  //{{AFX_DATA_INIT(CPurgePeak_dlg)
  m_nonterm_pk = FALSE;
  m_nonterm_ro = FALSE;
  m_nonterm_bs = FALSE;
  //}}AFX_DATA_INIT
}

void CPurgePeak_dlg::DoDataExchange(CDataExchange* pDX)
{
  CPurge_dlg::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPurgePeak_dlg)
  DDX_Check(pDX, IDC_NONTERM_PK, m_nonterm_pk);
  DDX_Check(pDX, IDC_NONTERM_RO, m_nonterm_ro);
  DDX_Check(pDX, IDC_NONTERM_BS, m_nonterm_bs);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPurgePeak_dlg, CPurge_dlg)
  //{{AFX_MSG_MAP(CPurgePeak_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPurgePeak_dlg message handlers
