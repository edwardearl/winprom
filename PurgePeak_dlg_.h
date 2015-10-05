// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_PURGEPEAK_DLG_H__3B3B542A_8FE9_4366_AEF2_B64864BB6606__INCLUDED_)
#define AFX_PURGEPEAK_DLG_H__3B3B542A_8FE9_4366_AEF2_B64864BB6606__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PurgePeak_dlg.h : header file
//

#include "Purge_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CPurgePeak_dlg dialog

class CPurgePeak_dlg : public CPurge_dlg
{
// Construction
public:
  CPurgePeak_dlg(bool, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CPurgePeak_dlg)
  enum { IDD = IDD_PURGE_PEAK };
  BOOL	m_nonterm_pk;
  BOOL	m_nonterm_ro;
  BOOL	m_nonterm_bs;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CPurgePeak_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CPurgePeak_dlg)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PURGEPEAK_DLG_H__3B3B542A_8FE9_4366_AEF2_B64864BB6606__INCLUDED_)
