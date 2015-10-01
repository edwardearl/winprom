#if !defined(AFX_FEATUREFMT_DLG_H__4CE75941_C808_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_FEATUREFMT_DLG_H__4CE75941_C808_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FeatureFmt_dlg.h : header file
//

#include "Format.h"


/////////////////////////////////////////////////////////////////////////////
// CFeatureFmt_dlg dialog

class CFeatureFmt_dlg : public CDialog
{
  Feature_fmt& target;
  Location_fmt temp_loc_fmt;
  CBitmap text_bmp,html_bmp,csv_bmp;
  const Database& data;
  void set_target(Feature_fmt&) const;
  void reset_custom(Format_style);

// Construction
public:
  CFeatureFmt_dlg(const Database&, CWnd *, Feature_fmt&);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CFeatureFmt_dlg)
  enum { IDD = IDD_FEATURE_FMT };
  CButton	m_reset_text_butn;
  CButton	m_reset_html_butn;
  CButton	m_reset_csv_butn;
  CEdit		m_custom_ctl;
  int		m_name_fmt;
  BOOL		m_precise;
  BOOL		m_show_loc;
  BOOL		m_show_elev;
  int		m_elev_fmt;
  CString	m_custom;
  //}}AFX_DATA


// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFeatureFmt_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CFeatureFmt_dlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  afx_msg void OnLocation();
  afx_msg void OnReset();
  afx_msg void OnResetText();
  afx_msg void OnResetHTML();
  afx_msg void OnResetCSV();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FEATUREFMT_DLG_H__4CE75941_C808_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
