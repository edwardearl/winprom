// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_DATALISTSUMMARY_DLG_H__7E146AE0_C804_11D6_A23A_108452C10000__INCLUDED_)
#define AFX_DATALISTSUMMARY_DLG_H__7E146AE0_C804_11D6_A23A_108452C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataListSummary_dlg.h : header file
//

#include "LocationFmt_dlg.h"
#include "DataListView.h"


/////////////////////////////////////////////////////////////////////////////
// CDataListSummary_dlg dialog

class CDataListSummary_dlg : public CDialog
{
  Rectangl area;
  Location_fmt summary_fmt;
  void reformat();

// Construction
public:
  CDataListSummary_dlg(CDataListView *);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CDataListSummary_dlg)
  enum { IDD = IDD_DATA_SUMMARY };
  CString	m_area_e;
  CString	m_area_n;
  CString	m_area_s;
  CString	m_area_w;
  int		m_count;
  //}}AFX_DATA


// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDataListSummary_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDataListSummary_dlg)
  afx_msg void OnFormat();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATALISTSUMMARY_DLG_H__7E146AE0_C804_11D6_A23A_108452C10000__INCLUDED_)
