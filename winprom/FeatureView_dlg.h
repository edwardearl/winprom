// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_FEATUREVIEW_DLG_H__288A5EC8_F2C6_4392_A374_3956DC4B2038__INCLUDED_)
#define AFX_FEATUREVIEW_DLG_H__288A5EC8_F2C6_4392_A374_3956DC4B2038__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FeatureView_dlg.h : header file
//

#include "CutoffOnly_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CFeatureView_dlg dialog

class CFeatureView_dlg : public CCutoffOnly_dlg
{
// Construction
public:
  CFeatureView_dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CFeatureView_dlg)
  enum { IDD = IDD_FEATURE_VIEW };
  BOOL	m_basin_sadl;
  BOOL	m_peak;
  BOOL	m_runoff;
  BOOL	m_saddle;
  BOOL	m_offmap;
  CString	m_label;
  BOOL	m_elev_cap;
  BOOL	m_cell_rot;
  BOOL	m_ridge_rot;
  //}}AFX_DATA
  unsigned label;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFeatureView_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CFeatureView_dlg)
  virtual void OnOK();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FEATUREVIEW_DLG_H__288A5EC8_F2C6_4392_A374_3956DC4B2038__INCLUDED_)
