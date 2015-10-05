// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_ELEVMAPINFO_DLG_H__E2B9D152_E373_4276_93FE_073E2E3CD229__INCLUDED_)
#define AFX_ELEVMAPINFO_DLG_H__E2B9D152_E373_4276_93FE_073E2E3CD229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElevMapInfo_dlg.h : header file
//

#include "MapInfo_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CElevMapInfo_dlg dialog

class CElevMapInfo_dlg : public CMapInfo_dlg
{
// Construction
public:
  CElevMapInfo_dlg(const Area_info&, const Area_info&, const CString&, bool);

// Dialog Data
  //{{AFX_DATA(CElevMapInfo_dlg)
  enum { IDD = IDD_ELEV_MAP_INFO };
  int		m_elev_high;
  int		m_elev_low;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CElevMapInfo_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CElevMapInfo_dlg)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEVMAPINFO_DLG_H__E2B9D152_E373_4276_93FE_073E2E3CD229__INCLUDED_)
