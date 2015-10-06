// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_FREEHEIGHTPARAM_DLG_H__CBDCE52E_6D02_49D7_86F7_ABAC1FF6F07A__INCLUDED_)
#define AFX_FREEHEIGHTPARAM_DLG_H__CBDCE52E_6D02_49D7_86F7_ABAC1FF6F07A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FreeHeightParam_dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CFreeHeightParam_dlg dialog

class CFreeHeightParam_dlg : public CDialog
{
// Construction
public:
  CFreeHeightParam_dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CFreeHeightParam_dlg)
  enum { IDD = IDD_FH_PARAM };
  int		m_ref_elev;
  CString	m_rad_vert;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFreeHeightParam_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CFreeHeightParam_dlg)
    // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FREEHEIGHTPARAM_DLG_H__CBDCE52E_6D02_49D7_86F7_ABAC1FF6F07A__INCLUDED_)
