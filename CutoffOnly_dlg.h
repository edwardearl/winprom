// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_CUTOFFONLY_DLG_H__A04BD0AB_AB46_40F6_B6F5_512DE7F5041B__INCLUDED_)
#define AFX_CUTOFFONLY_DLG_H__A04BD0AB_AB46_40F6_B6F5_512DE7F5041B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CutoffOnly_dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CCutoffOnly_dlg dialog

class CCutoffOnly_dlg : public CDialog
{
  void init_data();
// Construction
public:
  CCutoffOnly_dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CCutoffOnly_dlg)
  enum { IDD = IDD_CUTOFF_ONLY };
  CString	m_major;
  CString	m_minor;
  CString	m_standard;
  //}}AFX_DATA
  unsigned major,standard,minor;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CCutoffOnly_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:
  CCutoffOnly_dlg(UINT, CWnd* pParent = NULL);   // standard constructor
  bool close();

  // Generated message map functions
  //{{AFX_MSG(CCutoffOnly_dlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUTOFFONLY_DLG_H__A04BD0AB_AB46_40F6_B6F5_512DE7F5041B__INCLUDED_)
