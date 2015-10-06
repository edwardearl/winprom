// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// ElevMapInfo_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "ElevMapInfo_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CElevMapInfo_dlg dialog

CElevMapInfo_dlg::CElevMapInfo_dlg(const Area_info& a, const Area_info& d,
				   const CString& n, bool m)
  : CMapInfo_dlg(a,d,n,m,CElevMapInfo_dlg::IDD)
{
  //{{AFX_DATA_INIT(CElevMapInfo_dlg)
  m_elev_high = 0;
  m_elev_low = 0;
  //}}AFX_DATA_INIT
}

void CElevMapInfo_dlg::DoDataExchange(CDataExchange* pDX)
{
  CMapInfo_dlg::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CElevMapInfo_dlg)
  DDX_Text(pDX, IDC_ELEV_HIGH, m_elev_high);
  DDX_Text(pDX, IDC_ELEV_LOW, m_elev_low);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CElevMapInfo_dlg, CMapInfo_dlg)
  //{{AFX_MSG_MAP(CElevMapInfo_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CElevMapInfo_dlg message handlers
