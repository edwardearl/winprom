// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// ErrorParam_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "ErrorParam_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CErrorParam_dlg dialog

CErrorParam_dlg::CErrorParam_dlg(const MultiRegress& r, CWnd *pParent /*=NULL*/)
  : CDialog(CErrorParam_dlg::IDD, pParent),data(r)
{
  //{{AFX_DATA_INIT(CErrorParam_dlg)
  m_lambda = 0.0f;
  m_gamma = 0.0f;
  m_bias = 0.0f;
  m_bias_std = 0.0f;
  m_diag_coeff = _T("");
  m_diag_std = _T("");
  m_knight_coeff = _T("");
  m_knight_std = _T("");
  m_ortho_coeff = _T("");
  m_ortho_std = _T("");
  m_ortho2_coeff = _T("");
  m_ortho2_std = _T("");
  m_diag_slope = _T("");
  m_knight_slope = _T("");
  m_ortho_slope = _T("");
  m_ortho2_slope = _T("");
  //}}AFX_DATA_INIT
}

void CErrorParam_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CErrorParam_dlg)
  DDX_Text(pDX, IDC_LAMBDA, m_lambda);
  DDX_Text(pDX, IDC_GAMMA, m_gamma);
  DDX_Text(pDX, IDC_BIAS, m_bias);
  DDX_Text(pDX, IDC_BIAS_STD, m_bias_std);
  DDX_Text(pDX, IDC_DIAG_COEFF, m_diag_coeff);
  DDX_Text(pDX, IDC_DIAG_STD, m_diag_std);
  DDX_Text(pDX, IDC_KNIGHT_COEFF, m_knight_coeff);
  DDX_Text(pDX, IDC_KNIGHT_STD, m_knight_std);
  DDX_Text(pDX, IDC_ORTHO_COEFF, m_ortho_coeff);
  DDX_Text(pDX, IDC_ORTHO_STD, m_ortho_std);
  DDX_Text(pDX, IDC_ORTHO2_COEFF, m_ortho2_coeff);
  DDX_Text(pDX, IDC_ORTHO2_STD, m_ortho2_std);
  DDX_Text(pDX, IDC_DIAG_SLOPE, m_diag_slope);
  DDX_Text(pDX, IDC_KNIGHT_SLOPE, m_knight_slope);
  DDX_Text(pDX, IDC_ORTHO_SLOPE, m_ortho_slope);
  DDX_Text(pDX, IDC_ORTHO2_SLOPE, m_ortho2_slope);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CErrorParam_dlg, CDialog)
  //{{AFX_MSG_MAP(CErrorParam_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CErrorParam_dlg message handlers

BOOL CErrorParam_dlg::OnInitDialog()
{
  m_bias=data.bias;
  m_bias_std=data.sigma2_bias;
  if (m_bias_std>0) m_bias_std=sqrt(m_bias_std);
  switch (data.nparam) {
  case 4:
    m_knight_coeff.Format("%.3f",data.alpha[3]);
    m_knight_slope.Format("%.3f",data.slope(3));
    if (data.sigma2_res>=0)
      m_knight_std.Format("%.3f",sqrt(data.A[3][3]*data.sigma2_res));
    else m_knight_std="-1";
  case 3:
    m_ortho2_coeff.Format("%.3f",data.alpha[2]);
    m_ortho2_slope.Format("%.3f",data.slope(2));
    if (data.sigma2_res>=0)
      m_ortho2_std.Format("%.3f",sqrt(data.A[2][2]*data.sigma2_res));
    else m_ortho2_std="-1";
  case 2:
    m_diag_coeff.Format("%.3f",data.alpha[1]);
    m_diag_slope.Format("%.3f",data.slope(1));
    if (data.sigma2_res>=0)
      m_diag_std.Format("%.3f",sqrt(data.A[1][1]*data.sigma2_res));
    else m_diag_std="-1";
  case 1:
    m_ortho_coeff.Format("%.3f",data.alpha[0]);
    m_ortho_slope.Format("%.3f",data.slope(0));
    if (data.sigma2_res>=0)
      m_ortho_std.Format("%.3f",sqrt(data.A[0][0]*data.sigma2_res));
    else m_ortho_std="-1";
  case 0:
    break;
  }
  //m_lambda=data.lambda[0];
  //m_gamma=data.gamma;
  //if (m_gamma>0) m_gamma=sqrt(m_gamma);

  CDialog::OnInitDialog();

  return TRUE;  // return TRUE unless you set the focus to a control
}
