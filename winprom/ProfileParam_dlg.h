// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_PROFILEPARAM_DLG_H__2BDD1301_183C_11D4_8FA1_0040056FBD08__INCLUDED_)
#define AFX_PROFILEPARAM_DLG_H__2BDD1301_183C_11D4_8FA1_0040056FBD08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProfileParam_dlg.h : header file
//

#include "ProfileView.h"


/////////////////////////////////////////////////////////////////////////////
// CProfileParam_dlg dialog

class CProfileParam_dlg : public CDialog
{
  CProfileView *pView;

// Construction
public:
  CProfileParam_dlg(CProfileView *);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CProfileParam_dlg)
  enum { IDD = IDD_PROFILE_PARAM };
  int	m_min_prom;
  int	m_runoff_ue;
  int	m_scale_type;
  int	m_sadl_pos;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CProfileParam_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CProfileParam_dlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROFILEPARAM_DLG_H__2BDD1301_183C_11D4_8FA1_0040056FBD08__INCLUDED_)
