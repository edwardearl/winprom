// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// Shoreline_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "Shoreline_dlg.h"
#include "FeatureTypeFilter_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CShoreline_dlg dialog

CShoreline_dlg::CShoreline_dlg(CWnd* pParent /*=NULL*/)
  : CDialog(CShoreline_dlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CShoreline_dlg)
  m_sadl_elev = 0;
  m_set_edit = FALSE;
  m_mark_edit = FALSE;
  m_exterme_label = _T("");
  m_basin_elev = 0;
  //}}AFX_DATA_INIT
}

void CShoreline_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CShoreline_dlg)
  DDX_Text(pDX, IDC_SADL_ELEV, m_sadl_elev);
  DDX_Check(pDX, IDC_SET_EDIT, m_set_edit);
  DDX_Check(pDX, IDC_MARK_EDIT, m_mark_edit);
  DDX_Text(pDX, IDC_EXTREME_LABEL, m_exterme_label);
  DDX_Text(pDX, IDC_BASIN_ELEV, m_basin_elev);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShoreline_dlg, CDialog)
  //{{AFX_MSG_MAP(CShoreline_dlg)
  ON_BN_CLICKED(IDC_FILTER, OnFilter)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CShoreline_dlg message handlers

void CShoreline_dlg::OnFilter()
{
  CFeatureTypeFilter_dlg(m_filter,this).DoModal();
}
