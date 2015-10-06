// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_SHORELINE_DLG_H__BC1E56A0_37DB_11D8_A23A_E0C843C10000__INCLUDED_)
#define AFX_SHORELINE_DLG_H__BC1E56A0_37DB_11D8_A23A_E0C843C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Shoreline_dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CShoreline_dlg dialog

class CShoreline_dlg : public CDialog
{
// Construction
public:
  CShoreline_dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CShoreline_dlg)
  enum { IDD = IDD_SHORE };
  int	m_sadl_elev;
  BOOL	m_set_edit;
  BOOL	m_mark_edit;
  CString	m_exterme_label;
  int		m_basin_elev;
  //}}AFX_DATA
  FeatureTypeFilter m_filter;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CShoreline_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CShoreline_dlg)
  afx_msg void OnFilter();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHORELINE_DLG_H__BC1E56A0_37DB_11D8_A23A_E0C843C10000__INCLUDED_)
