// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// Imparam_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "Imparam_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CImparam_dlg dialog

CImparam_dlg::CImparam_dlg(CWnd* pParent /*=NULL*/)
  : CDialog(CImparam_dlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CImparam_dlg)
  m_conv_fac = _T("");
  m_distint = 0.0f;
  //}}AFX_DATA_INIT
}

void CImparam_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CImparam_dlg)
  DDX_Control(pDX, IDC_DISTINT, m_distint_ctl);
  DDX_Control(pDX, IDC_CONV_FAC, m_conv_fac_ctl);
  DDX_CBString(pDX, IDC_CONV_FAC, m_conv_fac);
  DDX_Text(pDX, IDC_DISTINT, m_distint);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CImparam_dlg, CDialog)
  //{{AFX_MSG_MAP(CImparam_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CImparam_dlg message handlers

BOOL CImparam_dlg::OnInitDialog()
{
  m_conv_fac="3280.840";
  m_distint=0.5;

  CDialog::OnInitDialog();

  return TRUE;
}

void CImparam_dlg::OnOK()
{
  if (!UpdateData()) return;

  if (m_distint<=0) {
    MessageBox("Distribution interval must be positive",
	       "Impressiveness Parameter Error");
    m_distint_ctl.SetSel(0,-1);
    m_distint_ctl.SetFocus();
    return;
  }

  char *endp;
  conv_fac=strtod(m_conv_fac,&endp);
  if (conv_fac<0 || *endp!='\0') {
    MessageBox("Conversion factor must be a positive number",
	       "Impressiveness Parameter Error");
    m_conv_fac_ctl.SetEditSel(0,-1);
    m_distint_ctl.SetFocus();
    return;
  }

  EndDialog(IDOK);
}
