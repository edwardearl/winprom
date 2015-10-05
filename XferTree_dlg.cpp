// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// XferTree_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "XferTree_dlg.h"
#include "Assoc_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXferTree_dlg dialog

CXferTree_dlg::CXferTree_dlg(bool e, bool d, CWnd* pParent /*=NULL*/)
  : CDialog(CXferTree_dlg::IDD, pParent),have_elev(e),have_dom(d),m_assoc(0,true)
{
  //{{AFX_DATA_INIT(CXferTree_dlg)
  m_sadl_source = -1;
  //}}AFX_DATA_INIT
}

void CXferTree_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CXferTree_dlg)
  DDX_Radio(pDX, IDC_SS_PROM, m_sadl_source);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXferTree_dlg, CDialog)
  //{{AFX_MSG_MAP(CXferTree_dlg)
  ON_BN_CLICKED(IDC_ASSOC, OnAssoc)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXferTree_dlg message handlers

void CXferTree_dlg::OnAssoc()
{
  CAssoc_dlg dlg(m_assoc,have_elev,have_dom,true);
  dlg.DoModal();
}
