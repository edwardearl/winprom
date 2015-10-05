// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_CROP_DLG_H__690A2540_51FC_11D7_A23A_00F553C10000__INCLUDED_)
#define AFX_CROP_DLG_H__690A2540_51FC_11D7_A23A_00F553C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Crop_dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CCrop_dlg dialog

class CCrop_dlg : public CDialog
{
  Rectangl& limits;
  const char *title;

// Construction
public:
  CCrop_dlg(const char *, Rectangl&, UINT=IDD, CWnd *pParent = NULL);   // standard constructor

  void check_ok();

// Dialog Data
  //{{AFX_DATA(CCrop_dlg)
  enum { IDD = IDD_CROP };
  CEdit	m_w_ctl;
  CEdit	m_s_ctl;
  CEdit	m_n_ctl;
  CEdit	m_e_ctl;
  CString	m_limit_e;
  CString	m_limit_n;
  CString	m_limit_s;
  CString	m_limit_w;
  //}}AFX_DATA
  bool check_interval;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CCrop_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CCrop_dlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CROP_DLG_H__690A2540_51FC_11D7_A23A_00F553C10000__INCLUDED_)
