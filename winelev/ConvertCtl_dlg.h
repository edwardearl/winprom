// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_CONVERTCTL_DLG_H__E998E5C5_1375_11D4_8FA1_0040056FBD08__INCLUDED_)
#define AFX_CONVERTCTL_DLG_H__E998E5C5_1375_11D4_8FA1_0040056FBD08__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ConvertCtl_dlg.h : header file
//

#include "../winprom/BackgdTask_dlg.h"

class CElev_dlg;


/////////////////////////////////////////////////////////////////////////////
// CConvertCtl_dlg dialog

class CConvertCtl_dlg : public CBackgdTask_dlg
{
  CElev_dlg& elev_win;

// Construction
public:
  CConvertCtl_dlg(CElev_dlg&);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CConvertCtl_dlg)
  enum { IDD = IDD_CONVERT_CTL };
  CProgressCtrl	m_progress;
  CString	m_fromfile;
  CString	m_stage;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CConvertCtl_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL
  virtual int proc() throw();

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CConvertCtl_dlg)
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONVERTCTL_DLG_H__E998E5C5_1375_11D4_8FA1_0040056FBD08__INCLUDED_)
