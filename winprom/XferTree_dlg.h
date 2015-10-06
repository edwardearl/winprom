// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_XFERTREE_DLG_H__5792FFA0_0BA1_11D6_A238_D09C52C10000__INCLUDED_)
#define AFX_XFERTREE_DLG_H__5792FFA0_0BA1_11D6_A238_D09C52C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XferTree_dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CXferTree_dlg dialog

class CXferTree_dlg : public CDialog
{
  bool have_elev,have_dom;

// Construction
public:
  CXferTree_dlg(bool, bool, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CXferTree_dlg)
  enum { IDD = IDD_TRANSFER };
  int	m_sadl_source;
  //}}AFX_DATA
  Assoc_info m_assoc;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CXferTree_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CXferTree_dlg)
  afx_msg void OnAssoc();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XFERTREE_DLG_H__5792FFA0_0BA1_11D6_A238_D09C52C10000__INCLUDED_)
