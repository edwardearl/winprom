// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_ELEVRANGE_DLG_H__2DA7A885_0301_11D4_8FA0_0040056FBD08__INCLUDED_)
#define AFX_ELEVRANGE_DLG_H__2DA7A885_0301_11D4_8FA0_0040056FBD08__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ElevRect_dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CElevRect_dlg dialog

class CElevRect_dlg : public CDialog
{
  Elev_intvl& target;
  Rectangl& limits;

// Construction
public:
  CElevRect_dlg(Elev_intvl&, Rectangl&);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CElevRect_dlg)
  enum { IDD = IDD_ELEV_RECT };
  CEdit	m_limit_w_ctl;
  CEdit	m_limit_s_ctl;
  CEdit	m_limit_n_ctl;
  CEdit	m_limit_e_ctl;
  CEdit	m_low_ctl;
  CEdit	m_high_ctl;
  CString	m_elev_high;
  CString	m_elev_low;
  CString	m_limit_e;
  CString	m_limit_n;
  CString	m_limit_s;
  CString	m_limit_w;
  //}}AFX_DATA


// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CElevRect_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CElevRect_dlg)
  virtual void OnOK();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEVRANGE_DLG_H__2DA7A885_0301_11D4_8FA0_0040056FBD08__INCLUDED_)
