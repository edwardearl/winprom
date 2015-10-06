// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_TREEINFO_DLG_H__405E29A2_C435_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_TREEINFO_DLG_H__405E29A2_C435_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TreeInfo_dlg.h : header file
//

#include "domain.h"
#include "LocationFmt_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CTreeInfo_dlg dialog

class CTreeInfo_dlg : public CDialog
{
  const CPromDoc& document;
  const Divide_tree& tree;
  Rectangl alloc,defined;
  static Location_fmt info_fmt;
  void reformat();

// Construction
public:
  CTreeInfo_dlg(const CPromDoc&); // standard constructor

// Dialog Data
  //{{AFX_DATA(CTreeInfo_dlg)
  enum { IDD = IDD_TREE_INFO };
  int		m_peak_count;
  int		m_peak_high;
  int		m_peak_low;
  int		m_runoff_count;
  int		m_runoff_high;
  int		m_runoff_low;
  int		m_saddle_count;
  int		m_saddle_high;
  int		m_saddle_low;
  CString	m_alloc_e;
  CString	m_alloc_n;
  CString	m_alloc_s;
  CString	m_alloc_w;
  CString	m_defined_e;
  CString	m_defined_n;
  CString	m_defined_s;
  CString	m_defined_w;
  int		m_bsnsadl_count;
  int		m_bsnsadl_high;
  int		m_bsnsadl_low;
  CString	m_file;
  int		m_peak_store;
  int		m_runoff_store;
  int		m_bsnsadl_store;
  CString	m_peaks;
  CString	m_basn_sadls;
  CString	m_saddles;
  //}}AFX_DATA


// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTreeInfo_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CTreeInfo_dlg)
  afx_msg void OnFormat();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEINFO_DLG_H__405E29A2_C435_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
