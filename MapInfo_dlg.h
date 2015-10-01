#if !defined(AFX_MAPINFO_DLG_H__405E29A1_C435_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_MAPINFO_DLG_H__405E29A1_C435_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MapInfo_dlg.h : header file
//

#include "Area_info.h"
#include "LocationFmt_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CMapInfo_dlg dialog

class CMapInfo_dlg : public CDialog
{
  const Area_info& alloc;
  const Area_info& defined;
  const CString& name;
  const bool modified;
  static Location_fmt info_fmt;
  void init();
  void reformat();

// Construction
protected:
  CMapInfo_dlg(const Area_info&, const Area_info&, const CString&, bool, UINT);
public:
  CMapInfo_dlg(const Area_info&, const Area_info&, const CString&, bool);

// Dialog Data
  //{{AFX_DATA(CMapInfo_dlg)
  enum { IDD = IDD_MAP_INFO };
  float		m_alloc_area;
  float		m_defined_area;
  CString	m_alloc_e;
  int		m_alloc_gp;
  CString	m_alloc_n;
  CString	m_alloc_s;
  CString	m_alloc_w;
  CString	m_defined_e;
  int		m_defined_gp;
  CString	m_defined_n;
  CString	m_defined_s;
  CString	m_defined_w;
  CString	m_file;
  //}}AFX_DATA


// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMapInfo_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CMapInfo_dlg)
  afx_msg void OnFormat();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPINFO_DLG_H__405E29A1_C435_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
