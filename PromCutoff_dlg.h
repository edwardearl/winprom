// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_PROMCUTOFF_DLG_H__51145A4F_A0A2_4B0D_BDBF_8AF4FA9CDF48__INCLUDED_)
#define AFX_PROMCUTOFF_DLG_H__51145A4F_A0A2_4B0D_BDBF_8AF4FA9CDF48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PromCutoff_dlg.h : header file
//

#include "CutoffOnly_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CPromCutoff_dlg dialog

class CPromCutoff_dlg : public CCutoffOnly_dlg
{
// Construction
public:
  CPromCutoff_dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CPromCutoff_dlg)
  enum { IDD = IDD_PROM_CUTOFF };
  int	m_offmap;
  int	m_cell_rot;
  int	m_ridge_rot;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CPromCutoff_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CPromCutoff_dlg)
    // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROMCUTOFF_DLG_H__51145A4F_A0A2_4B0D_BDBF_8AF4FA9CDF48__INCLUDED_)
