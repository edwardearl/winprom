// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_FILEERROR_DLG_H__73E088A3_1114_11D4_8FA1_0040056FBD08__INCLUDED_)
#define AFX_FILEERROR_DLG_H__73E088A3_1114_11D4_8FA1_0040056FBD08__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FileError_dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileError_dlg dialog

class CFileError_dlg : public CDialog
{
// Construction
public:
  CFileError_dlg();   // standard constructor

// Dialog Data
  //{{AFX_DATA(CFileError_dlg)
  enum { IDD = IDD_FILE_ERROR };
  BOOL		m_future;
  CString	m_message;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFileError_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CFileError_dlg)
  afx_msg void OnAbort();
  afx_msg void OnRetry();
  afx_msg void OnSkip();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEERROR_DLG_H__73E088A3_1114_11D4_8FA1_0040056FBD08__INCLUDED_)
