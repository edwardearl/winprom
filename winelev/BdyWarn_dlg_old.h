// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_BDYWARN_DLG_H__9EEB3381_BB63_4B91_8F03_BA5F2F8672FF__INCLUDED_)
#define AFX_BDYWARN_DLG_H__9EEB3381_BB63_4B91_8F03_BA5F2F8672FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BdyWarn_dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CBdyWarn_dlg dialog

class CBdyWarn_dlg : public CDialog
{
// Construction
public:
  CBdyWarn_dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CBdyWarn_dlg)
  enum { IDD = IDD_BDY_WARNING };
  CString	m_file1;
  CString	m_file2;
  BOOL		m_reprompt;
  CString	m_warn_type;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CBdyWarn_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CBdyWarn_dlg)
    // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BDYWARN_DLG_H__9EEB3381_BB63_4B91_8F03_BA5F2F8672FF__INCLUDED_)
