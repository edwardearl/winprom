// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_ERRORFILTER_DLG_H__ECD6E801_88CC_11D4_8FA2_0040056FBD08__INCLUDED_)
#define AFX_ERRORFILTER_DLG_H__ECD6E801_88CC_11D4_8FA2_0040056FBD08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ErrorFilter_dlg.h : header file
//

#include "FeatureTypeFilter_dlg.h"
#include "domain.h"


struct ErrorFilter : FeatureTypeFilter {
  Elevation min_error;
  short reference;
  ErrorFilter() : reference(0),min_error(1) {unedit=false;}
  bool test(const Feature&, FT_index, Elevation, const Database&) const;
};


/////////////////////////////////////////////////////////////////////////////
// CErrorFilter_dlg dialog

class CErrorFilter_dlg : public CDialog
{
  ErrorFilter& filter;
  CPromDoc& document;
  FeatureTypeFilter m_filter;
  Error_stats m_error_stats;

// Construction
public:
  CErrorFilter_dlg(ErrorFilter&, CPromDoc&, CWnd *parent=0);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CErrorFilter_dlg)
  enum { IDD = IDD_ERROR_FILTER };
  int	m_min_error;
  int		m_reference;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CErrorFilter_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CErrorFilter_dlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  afx_msg void OnListFilter();
  afx_msg void OnStat();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ERRORFILTER_DLG_H__ECD6E801_88CC_11D4_8FA2_0040056FBD08__INCLUDED_)
