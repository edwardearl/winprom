// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// DomapInfo_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "DomapInfo_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDomapInfo_dlg dialog

CDomapInfo_dlg::CDomapInfo_dlg(const Area_info& a, const Area_info& d,
			       const CString& n, bool m)
  : CMapInfo_dlg(a,d,n,m,CDomapInfo_dlg::IDD)
{
  //{{AFX_DATA_INIT(CDomapInfo_dlg)
  m_ndom_0peak = 0;
  m_ndom_1peak = 0;
  m_ndom_peaks = 0;
  m_ndom_total = 0;
  m_ndom_area = 0;
  //}}AFX_DATA_INIT
}

void CDomapInfo_dlg::DoDataExchange(CDataExchange* pDX)
{
  CMapInfo_dlg::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDomapInfo_dlg)
  DDX_Text(pDX, IDC_NDOM_0PEAK, m_ndom_0peak);
  DDX_Text(pDX, IDC_NDOM_1PEAK, m_ndom_1peak);
  DDX_Text(pDX, IDC_NDOM_PEAKS, m_ndom_peaks);
  DDX_Text(pDX, IDC_NDOM, m_ndom_total);
  DDX_Text(pDX, IDC_NDOM_AREA, m_ndom_area);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDomapInfo_dlg, CMapInfo_dlg)
  //{{AFX_MSG_MAP(CDomapInfo_dlg)
    // NOTE: the ClassWizard will add message map macros here
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDomapInfo_dlg message handlers
