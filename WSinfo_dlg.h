#if !defined(AFX_WSINFO_DLG_H__2D9EF961_D192_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_WSINFO_DLG_H__2D9EF961_D192_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// WSinfo_dlg.h : header file
//

#include "Area_info.h"
#include "LocationFmt_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CWSinfo_dlg dialog

class CWSinfo_dlg : public CDialog
{
  const Area_info& alloc;
  const Area_info& defined;
  static Location_fmt info_fmt;
  void reformat();

// Construction
public:
  CWSinfo_dlg(const Area_info&, const Area_info&);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CWSinfo_dlg)
  enum { IDD = IDD_ALL_INFO };
  CString	m_alloc_e;
  CString	m_alloc_n;
  CString	m_alloc_s;
  CString	m_alloc_w;
  CString	m_defined_e;
  CString	m_defined_n;
  CString	m_defined_s;
  CString	m_defined_w;
  CString	m_quad_lat;
  CString	m_quad_lon;
  CString	m_step_lat;
  CString	m_step_lon;
  CString	m_quad_coord1;
  CString	m_quad_coord2;
  CString	m_step_coord1;
  CString	m_step_coord2;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CWSinfo_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CWSinfo_dlg)
  afx_msg void OnFormat();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WSINFO_DLG_H__2D9EF961_D192_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
