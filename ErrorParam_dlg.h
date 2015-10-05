// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_ERRORPARAM_DLG_H__C67A0320_2926_11D7_A23A_00F553C10000__INCLUDED_)
#define AFX_ERRORPARAM_DLG_H__C67A0320_2926_11D7_A23A_00F553C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ErrorParam_dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CErrorParam_dlg dialog

class CErrorParam_dlg : public CDialog
{
  const MultiRegress& data;

// Construction
public:
  CErrorParam_dlg(const MultiRegress&, CWnd *pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CErrorParam_dlg)
  enum { IDD = IDD_ERROR_PARAMS };
  float	m_lambda;
  float	m_gamma;
  float	m_bias;
  float	m_bias_std;
  CString	m_diag_coeff;
  CString	m_diag_std;
  CString	m_knight_coeff;
  CString	m_knight_std;
  CString	m_ortho_coeff;
  CString	m_ortho_std;
  CString	m_ortho2_coeff;
  CString	m_ortho2_std;
  CString	m_diag_slope;
  CString	m_knight_slope;
  CString	m_ortho_slope;
  CString	m_ortho2_slope;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CErrorParam_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CErrorParam_dlg)
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ERRORPARAM_DLG_H__C67A0320_2926_11D7_A23A_00F553C10000__INCLUDED_)
