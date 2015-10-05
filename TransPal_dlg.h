// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_TRANSPAL_DLG_H__EB2BB477_70D4_45F8_85CB_5DF5CB63DC43__INCLUDED_)
#define AFX_TRANSPAL_DLG_H__EB2BB477_70D4_45F8_85CB_5DF5CB63DC43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TransPal_dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CTransPal_dlg dialog

class CTransPal_dlg : public CDialog
{
// Construction
public:
  CTransPal_dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CTransPal_dlg)
  enum { IDD = IDD_TRANSPAL };
  CString	m_high_old;
  int		m_high_new;
  int		m_low_new;
  CString	m_low_old;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTransPal_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CTransPal_dlg)
    // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSPAL_DLG_H__EB2BB477_70D4_45F8_85CB_5DF5CB63DC43__INCLUDED_)
