// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// FeatureTypeFilter_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "FeatureTypeFilter_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFeatureTypeFilter_dlg dialog

CFeatureTypeFilter_dlg::CFeatureTypeFilter_dlg(FeatureTypeFilter& ff, CWnd* pParent /*=NULL*/)
  : CFeatureFilter_dlg(CFeatureTypeFilter_dlg::IDD,ff,pParent),filter(ff)
{
  //{{AFX_DATA_INIT(CFeatureTypeFilter_dlg)
  m_basin_saddles = FALSE;
  m_peaks = FALSE;
  m_runoffs = FALSE;
  m_saddles = FALSE;
  //}}AFX_DATA_INIT
}

void CFeatureTypeFilter_dlg::DoDataExchange(CDataExchange *pDX)
{
  CFeatureFilter_dlg::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CFeatureTypeFilter_dlg)
  DDX_Check(pDX, IDC_BSNSDLS, m_basin_saddles);
  DDX_Check(pDX, IDC_PEAKS, m_peaks);
  DDX_Check(pDX, IDC_RUNOFFS, m_runoffs);
  DDX_Check(pDX, IDC_SADDLES, m_saddles);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeatureTypeFilter_dlg, CFeatureFilter_dlg)
  //{{AFX_MSG_MAP(CFeatureTypeFilter_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFeatureTypeFilter_dlg message handlers

BOOL CFeatureTypeFilter_dlg::OnInitDialog() 
{
  m_peaks=filter.peaks;
  m_saddles=filter.saddles;
  m_runoffs=filter.runoffs;
  m_basin_saddles=filter.bsnsdls;

  CFeatureFilter_dlg::OnInitDialog();

  return TRUE;	// return TRUE unless you set the focus to a control
}

void CFeatureTypeFilter_dlg::OnOK() 
{
  if (!UpdateData()) return;

  if (!m_peaks && !m_saddles && !m_runoffs && !m_basin_saddles) {
    if (MessageBox("You have excluded all feature types.\n"
		   "Is that really what you want?",
		   "Winprom Entry Warning",
		   MB_YESNO|MB_DEFBUTTON2|MB_ICONWARNING)!=IDYES)
      return;
  }

  if (!close()) return;

  filter.peaks=m_peaks!=FALSE;
  filter.saddles=m_saddles!=FALSE;
  filter.runoffs=m_runoffs!=FALSE;
  filter.bsnsdls=m_basin_saddles!=FALSE;

  EndDialog(IDOK);
}
