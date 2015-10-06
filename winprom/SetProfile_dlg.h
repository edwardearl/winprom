// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_SETPROFILE_DLG_H__A9FEC603_36D4_11D4_8FA1_0040056FBD08__INCLUDED_)
#define AFX_SETPROFILE_DLG_H__A9FEC603_36D4_11D4_8FA1_0040056FBD08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetProfile_dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetProfile_dlg dialog

class CSetProfile_dlg : public CDialog
{
// Construction
public:
  CSetProfile_dlg();   // standard constructor

// Dialog Data
  //{{AFX_DATA(CSetProfile_dlg)
  enum { IDD = IDD_PROFILE_SET };
  int		m_elev;
  int		m_prom;
  //}}AFX_DATA


// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CSetProfile_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CSetProfile_dlg)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETPROFILE_DLG_H__A9FEC603_36D4_11D4_8FA1_0040056FBD08__INCLUDED_)
