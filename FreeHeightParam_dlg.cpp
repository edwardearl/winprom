// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// FreeHeightParam_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "FreeHeightParam_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFreeHeightParam_dlg dialog

CFreeHeightParam_dlg::CFreeHeightParam_dlg(CWnd* pParent /*=NULL*/)
  : CDialog(CFreeHeightParam_dlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CFreeHeightParam_dlg)
  m_ref_elev = 0;
  m_rad_vert = _T("");
  //}}AFX_DATA_INIT
}


void CFreeHeightParam_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CFreeHeightParam_dlg)
  DDX_Text(pDX, IDC_REF_ELEV, m_ref_elev);
  DDX_CBString(pDX, IDC_RAD_VERT, m_rad_vert);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFreeHeightParam_dlg, CDialog)
  //{{AFX_MSG_MAP(CFreeHeightParam_dlg)
    // NOTE: the ClassWizard will add message map macros here
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFreeHeightParam_dlg message handlers
