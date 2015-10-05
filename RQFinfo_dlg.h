// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_RQFINFO_DLG_H__5F420560_0B6E_11D8_A23A_B0F453C10000__INCLUDED_)
#define AFX_RQFINFO_DLG_H__5F420560_0B6E_11D8_A23A_B0F453C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RQFinfo_dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CRQFinfo_dlg dialog

class CRQFinfo_dlg : public CDialog
{
  const CPromDoc *document;
  const GridPoint center;
  void data_init();
  const Feature *featr;
  FT_index ftype;

public:
  // Construction
  CRQFinfo_dlg(const CPromDoc *, const GridPoint&, CWnd* pParent = NULL);   // standard constructor
  CRQFinfo_dlg(const CPromDoc *, const Feature *, FT_index, CWnd* pParent = NULL);

// Dialog Data
  //{{AFX_DATA(CRQFinfo_dlg)
  enum { IDD = IDD_RQF_INFO };
  CString	m_convlutn;
  CString	m_eigen1;
  CString	m_eigen2;
  CString	m_elev_ctr;
  CString	m_elev_stat;
  CString	m_laplac;
  CString	m_nbr00;
  CString	m_nbr01;
  CString	m_nbr02;
  CString	m_nbr10;
  CString	m_nbr11;
  CString	m_nbr12;
  CString	m_nbr20;
  CString	m_nbr21;
  CString	m_nbr22;
  CString	m_statx;
  CString	m_staty;
  CString	m_location;
  CString	m_axis;
  CString	m_resid_asym1;
  CString	m_resid_asym2;
  CString	m_resid_sym;
  CString	m_hess1;
  CString	m_hess2;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CRQFinfo_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CRQFinfo_dlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnCenter();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RQFINFO_DLG_H__5F420560_0B6E_11D8_A23A_B0F453C10000__INCLUDED_)
