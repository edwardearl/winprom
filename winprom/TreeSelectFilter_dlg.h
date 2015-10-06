// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_TREESELECTFILTER_DLG_H__9FE453E1_9BE7_11D6_A239_708552C10000__INCLUDED_)
#define AFX_TREESELECTFILTER_DLG_H__9FE453E1_9BE7_11D6_A239_708552C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeSelectFilter_dlg.h : header file
//

#include "FeatureFilter_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CTreeSelectFilter_dlg dialog

class CTreeSelectFilter_dlg : public CDialog
{
  const Database& data;

// Construction
public:
  CTreeSelectFilter_dlg(const Database&, CWnd *pParent = NULL);   // standard constructor

// Dialog Data
  FeatureFilter peak_xing,sadl_xing,peak_select,sadl_select;
  bool bs_root;
  //{{AFX_DATA(CTreeSelectFilter_dlg)
  enum { IDD = IDD_TREE_SELECT };
  CButton	m_bs_xing_butn;
  int	m_relation;
  int	m_action;
  BOOL	m_bs_xing;
  int	m_topology;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTreeSelectFilter_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CTreeSelectFilter_dlg)
  afx_msg void OnPkSelect();
  afx_msg void OnPkXing();
  afx_msg void OnSadlSelect();
  afx_msg void OnSadlXing();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREESELECTFILTER_DLG_H__9FE453E1_9BE7_11D6_A239_708552C10000__INCLUDED_)
