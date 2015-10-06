// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// FeatureView_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "FeatureView_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFeatureView_dlg dialog

CFeatureView_dlg::CFeatureView_dlg(CWnd* pParent /*=NULL*/)
  : CCutoffOnly_dlg(CFeatureView_dlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CFeatureView_dlg)
  m_basin_sadl = FALSE;
  m_peak = FALSE;
  m_runoff = FALSE;
  m_saddle = FALSE;
  m_offmap = FALSE;
  m_label = _T("");
  m_elev_cap = FALSE;
  m_cell_rot = FALSE;
  m_ridge_rot = FALSE;
  //}}AFX_DATA_INIT
}

void CFeatureView_dlg::DoDataExchange(CDataExchange* pDX)
{
  CCutoffOnly_dlg::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CFeatureView_dlg)
  DDX_Check(pDX, IDC_BASIN_SADL, m_basin_sadl);
  DDX_Check(pDX, IDC_PEAK, m_peak);
  DDX_Check(pDX, IDC_RUNOFF, m_runoff);
  DDX_Check(pDX, IDC_SADDLE, m_saddle);
  DDX_Check(pDX, IDC_OFFMAP, m_offmap);
  DDX_Text(pDX, IDC_LABEL, m_label);
  DDX_Check(pDX, IDC_ELEV_CAP, m_elev_cap);
  DDX_Check(pDX, IDC_CELL, m_cell_rot);
  DDX_Check(pDX, IDC_RIDGE, m_ridge_rot);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeatureView_dlg, CCutoffOnly_dlg)
  //{{AFX_MSG_MAP(CFeatureView_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFeatureView_dlg message handlers

BOOL CFeatureView_dlg::OnInitDialog()
{
  if (label<elev_infty) m_label.Format("%d",label);

  CCutoffOnly_dlg::OnInitDialog();

  return TRUE;  // return TRUE unless you set the focus to a control
}

void CFeatureView_dlg::OnOK() 
{
  if (!UpdateData() || !close()) return;

  if (strlen(m_label)==0) label=elev_infty;
  else {
    label=atoi(m_label);
    if (label>=elev_infty) {
      MessageBox("Icon label prominence must be <= 32511",
		 "Winprom Feature View Error",MB_OK|MB_ICONERROR);
      return;
    }
  }

  EndDialog(IDOK);
}
