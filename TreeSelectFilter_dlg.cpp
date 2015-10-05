// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// TreeSelectFilter_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "TreeSelectFilter_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTreeSelectFilter_dlg dialog

CTreeSelectFilter_dlg::CTreeSelectFilter_dlg(const Database& db, CWnd *pParent /*=NULL*/)
  : CDialog(CTreeSelectFilter_dlg::IDD, pParent),data(db)
{
  //{{AFX_DATA_INIT(CTreeSelectFilter_dlg)
  m_relation = -1;
  m_action = -1;
  m_bs_xing = FALSE;
  m_topology = -1;
  //}}AFX_DATA_INIT
}

void CTreeSelectFilter_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CTreeSelectFilter_dlg)
  DDX_Control(pDX, IDC_BS_XING, m_bs_xing_butn);
  DDX_Radio(pDX, IDC_REL_ANY, m_relation);
  DDX_Radio(pDX, IDC_ACT_SET, m_action);
  DDX_Check(pDX, IDC_BS_XING, m_bs_xing);
  DDX_Radio(pDX, IDC_LINE, m_topology);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTreeSelectFilter_dlg, CDialog)
  //{{AFX_MSG_MAP(CTreeSelectFilter_dlg)
  ON_BN_CLICKED(IDC_PK_SELECT, OnPkSelect)
  ON_BN_CLICKED(IDC_PK_XING, OnPkXing)
  ON_BN_CLICKED(IDC_SADL_SELECT, OnSadlSelect)
  ON_BN_CLICKED(IDC_SADL_XING, OnSadlXing)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTreeSelectFilter_dlg message handlers

void CTreeSelectFilter_dlg::OnPkXing()
{
  CFeatureFilter_dlg(peak_xing,this).DoModal();
}

void CTreeSelectFilter_dlg::OnSadlXing()
{
  CFeatureFilter_dlg(sadl_xing,this).DoModal();
}

void CTreeSelectFilter_dlg::OnPkSelect()
{
  CFeatureFilter_dlg(peak_select,this).DoModal();
}

void CTreeSelectFilter_dlg::OnSadlSelect()
{
  CFeatureFilter_dlg(sadl_select,this).DoModal();
}

BOOL CTreeSelectFilter_dlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  if (bs_root) m_bs_xing_butn.EnableWindow(FALSE);

  return TRUE;
}
