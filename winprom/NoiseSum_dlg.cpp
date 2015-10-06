// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// NoiseSum_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "NoiseSum_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CNoiseSum_dlg dialog

CNoiseSum_dlg::CNoiseSum_dlg(CWnd* pParent /*=NULL*/)
  : CDialog(CNoiseSum_dlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CNoiseSum_dlg)
  m_big_noise = 0;
  m_big_prom_noise = 0;
  //}}AFX_DATA_INIT
}

void CNoiseSum_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CNoiseSum_dlg)
  DDX_Text(pDX, IDC_BIG_NOISE, m_big_noise);
  DDX_Text(pDX, IDC_BIG_NOISE_PROM, m_big_prom_noise);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNoiseSum_dlg, CDialog)
  //{{AFX_MSG_MAP(CNoiseSum_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNoiseSum_dlg message handlers

void CNoiseSum_dlg::OnOK() 
{
  EndDialog(IDOK);
}
