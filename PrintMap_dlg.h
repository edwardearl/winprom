// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_PRINTMAP_DLG_H__E8C82CE0_CD32_11D6_A23A_00A447C10404__INCLUDED_)
#define AFX_PRINTMAP_DLG_H__E8C82CE0_CD32_11D6_A23A_00A447C10404__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrintMap_dlg.h : header file
//

#include "MapView.h"


/////////////////////////////////////////////////////////////////////////////
// CPrintMap_dlg dialog

class CPrintMap_dlg : public CDialog
{
  CPrintInfo *pInfo;
  const CMapView& view;
  const char *get_limit_text(CEdit *&);
  void set_limit_text();
  void set_limit_text(const Rectangl& lim) {limits=lim; set_limit_text();}
  void set_window_limits();
  void set_pages();

// Construction
public:
  CPrintMap_dlg(const CMapView&, CPrintInfo *, CWnd* pParent = NULL);   // standard constructor

  Rectangl limits;

// Dialog Data
  //{{AFX_DATA(CPrintMap_dlg)
  enum { IDD = IDD_PRINT_MAP };
  CButton	m_elev_map_ctl;
  CButton	m_domap_ctl;
  CButton	m_div_tree_ctl;
  CEdit	m_pages_edit;
  CEdit		m_edit_w;
  CEdit		m_edit_s;
  CEdit		m_edit_n;
  CEdit		m_edit_e;
  CButton	m_last_print_butn;
  CString	m_print_e;
  CString	m_print_n;
  CString	m_print_s;
  CString	m_print_w;
  BOOL		m_div_tree;
  BOOL		m_domap;
  BOOL		m_elev_map;
  int		m_unit;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CPrintMap_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CPrintMap_dlg)
  afx_msg void OnPrintSetup();
  virtual void OnOK();
  afx_msg void OnWindow();
  virtual BOOL OnInitDialog();
  afx_msg void OnLastPrint();
  afx_msg void OnWorkspace();
  afx_msg void OnUpdatePages();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTMAP_DLG_H__E8C82CE0_CD32_11D6_A23A_00A447C10404__INCLUDED_)
