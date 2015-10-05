// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// StatOptions_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "StatOptions_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CStatOptions_dlg dialog

CStatOptions_dlg::CStatOptions_dlg(CWnd* pParent /*=NULL*/)
  : CDialog(CStatOptions_dlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CStatOptions_dlg)
  m_symmetry = -1;
  m_min_nbr = -1;
  m_measr_error = FALSE;
  m_convolutn = FALSE;
  //}}AFX_DATA_INIT
}

void CStatOptions_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CStatOptions_dlg)
  DDX_CBIndex(pDX, IDC_SYMMETRY, m_symmetry);
  DDX_CBIndex(pDX, IDC_MIN_NBR, m_min_nbr);
  DDX_Check(pDX, IDC_MEASURE_ERROR, m_measr_error);
  DDX_Check(pDX, IDC_CONVLUTN, m_convolutn);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatOptions_dlg, CDialog)
  //{{AFX_MSG_MAP(CStatOptions_dlg)
    // NOTE: the ClassWizard will add message map macros here
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStatOptions_dlg message handlers
