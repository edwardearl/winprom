// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_IDENTIFYFEATURE_DLG_H__D0878923_0A28_11D4_8FA1_0040056FBD08__INCLUDED_)
#define AFX_IDENTIFYFEATURE_DLG_H__D0878923_0A28_11D4_8FA1_0040056FBD08__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// IdentifyFeature_dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIdentifyFeature_dlg dialog

class CIdentifyFeature_dlg : public CDialog
{
// Construction
public:
  CIdentifyFeature_dlg();   // standard constructor

// Dialog Data
  //{{AFX_DATA(CIdentifyFeature_dlg)
  enum { IDD = IDD_IDENTIFY_FEATURE };
  BOOL	m_center_target;
  BOOL	m_select_target;
  //}}AFX_DATA


// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CIdentifyFeature_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CIdentifyFeature_dlg)
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IDENTIFYFEATURE_DLG_H__D0878923_0A28_11D4_8FA1_0040056FBD08__INCLUDED_)
