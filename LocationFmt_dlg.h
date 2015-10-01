#if !defined(AFX_LOCATIONFMT_DLG_H__F2C5AF43_C4DB_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_LOCATIONFMT_DLG_H__F2C5AF43_C4DB_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// LocationFmt_dlg.h : header file
//

#include "Format.h"


/////////////////////////////////////////////////////////////////////////////
// CLocationFmt_dlg dialog

class CLocationFmt_dlg : public CDialog
{
  Location_fmt& target;
  void set_target(Location_fmt&) const;

// Construction
public:
  CLocationFmt_dlg(CWnd *, Location_fmt&);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CLocationFmt_dlg)
  enum { IDD = IDD_LOCATION_FMT };
  CEdit	m_custom_edit;
  BOOL	m_news;
  CString	m_custom;
  int		m_angle_fmt;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CLocationFmt_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CLocationFmt_dlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  afx_msg void OnReset();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOCATIONFMT_DLG_H__F2C5AF43_C4DB_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
