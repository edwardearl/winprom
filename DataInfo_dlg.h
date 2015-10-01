#if !defined(AFX_DATAINFO_DLG_H__B59BAE60_C615_11D6_A23A_40C843C10000__INCLUDED_)
#define AFX_DATAINFO_DLG_H__B59BAE60_C615_11D6_A23A_40C843C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataInfo_dlg.h : header file
//

#include "LocationFmt_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CDataInfo_dlg dialog

class CDataInfo_dlg : public CDialog
{
  const CPromDoc& doc;
  Rectangl area;
  static Location_fmt info_fmt;
  void reformat();

// Construction
public:
  CDataInfo_dlg(const CPromDoc&);

// Dialog Data
  //{{AFX_DATA(CDataInfo_dlg)
  enum { IDD = IDD_DATA_INFO };
  CListBox	m_field_list;
  CString	m_file;
  CString	m_area_n;
  CString	m_area_s;
  CString	m_area_w;
  int		m_records;
  CString	m_area_e;
  //}}AFX_DATA


// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDataInfo_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDataInfo_dlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnFormat();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAINFO_DLG_H__B59BAE60_C615_11D6_A23A_40C843C10000__INCLUDED_)
