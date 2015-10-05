// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// TreePaste_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "TreePaste_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTreePaste_dlg dialog

CTreePaste_dlg::CTreePaste_dlg(CWnd *pParent, CPromDoc *doc,
			       Feature& featr, FT_index ft, bool paste)
  : CDialog(CTreePaste_dlg::IDD,pParent),document(doc),
    feature(featr),type(ft),canPaste(paste)
{
  //{{AFX_DATA_INIT(CTreePaste_dlg)
  m_show_next = TRUE;
  //}}AFX_DATA_INIT
}


void CTreePaste_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CTreePaste_dlg)
  DDX_Control(pDX, IDC_PASTE, m_paste_ctl);
  DDX_Check(pDX, IDC_SHOW_NEXT, m_show_next);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTreePaste_dlg, CDialog)
  //{{AFX_MSG_MAP(CTreePaste_dlg)
  ON_BN_CLICKED(IDC_CENTER, OnCenter)
  ON_BN_CLICKED(IDC_ABORT, OnAbort)
  ON_BN_CLICKED(IDC_PASTE, OnPaste)
  ON_BN_CLICKED(IDC_SKIP, OnSkip)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTreePaste_dlg message handlers

void CTreePaste_dlg::OnCenter()
{
  CPromApp::getApp()->viewFeature(document,feature,type);
}

void CTreePaste_dlg::OnAbort()
{
  EndDialog(IDC_ABORT);
}

void CTreePaste_dlg::OnPaste()
{
  UpdateData(TRUE);
  EndDialog(IDC_PASTE);
}

void CTreePaste_dlg::OnSkip()
{
  UpdateData(TRUE);
  EndDialog(IDC_SKIP);
}

BOOL CTreePaste_dlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_paste_ctl.EnableWindow(canPaste);

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}
