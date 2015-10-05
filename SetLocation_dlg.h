// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_SETLOCATION_DLG_H__ED89D085_F26F_11D3_8FA0_0040056FBD08__INCLUDED_)
#define AFX_SETLOCATION_DLG_H__ED89D085_F26F_11D3_8FA0_0040056FBD08__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SetLocation_dlg.h : header file
//

#include "Format.h"


/////////////////////////////////////////////////////////////////////////////
// CSetLocation_dlg dialog

class CSetLocation_dlg : public CDialog
{
  GridPoint& location;
  const Location_fmt& format;
  Rectangl bound;

// Construction
public:
  CSetLocation_dlg(GridPoint&, const Rectangl&, const Location_fmt&);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CSetLocation_dlg)
  enum { IDD = IDD_SET_LOCATION };
  CEdit		m_lon_ctl;
  CEdit		m_lat_ctl;
  CString	m_lat;
  CString	m_lon;
  int		m_unit;
  //}}AFX_DATA


// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CSetLocation_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CSetLocation_dlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETLOCATION_DLG_H__ED89D085_F26F_11D3_8FA0_0040056FBD08__INCLUDED_)
