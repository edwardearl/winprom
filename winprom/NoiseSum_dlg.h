// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_NOISESUM_DLG_H__D74CAC45_3553_4F6E_9EAC_BCBADCA5F9C3__INCLUDED_)
#define AFX_NOISESUM_DLG_H__D74CAC45_3553_4F6E_9EAC_BCBADCA5F9C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NoiseSum_dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CNoiseSum_dlg dialog

class CNoiseSum_dlg : public CDialog
{
// Construction
public:
  CNoiseSum_dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CNoiseSum_dlg)
  enum { IDD = IDD_NOISE_SUM };
  int		m_big_noise;
  int		m_big_prom_noise;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CNoiseSum_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CNoiseSum_dlg)
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOISESUM_DLG_H__D74CAC45_3553_4F6E_9EAC_BCBADCA5F9C3__INCLUDED_)
