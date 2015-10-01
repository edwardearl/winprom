#if !defined(AFX_FEATUREEDIT_DLG_H__E69B0B81_C778_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_FEATUREEDIT_DLG_H__E69B0B81_C778_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FeatureEdit_dlg.h : header file
//

#include "point.h"
#include "Format.h"


/////////////////////////////////////////////////////////////////////////////
// CFeatureEdit_dlg dialog

class CFeatureEdit_dlg : public CDialog
{
  Feature& target;
  FT_index tgt_type;
  CPromDoc *document;
  bool edit_click;
  //static Location_fmt loc_fmt;

// Construction
public:
  CFeatureEdit_dlg(CWnd *, Feature&, FT_index, CPromDoc *);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CFeatureEdit_dlg)
  enum { IDD = IDD_FEATURE };
  CComboBox	m_sadl_stat_ctl;
  CEdit		m_loc_field;
  CButton	m_editCtl;
  BOOL		m_edge;
  BOOL		m_edited;
  CString	m_location;
  int		m_elev_high;
  int		m_elev_low;
  CString	m_name;
  CString	m_flat_sep;
  CString	m_prom;
  CString	m_omp;
  CString	m_bind_type;
  int		m_sadl_stat;
  CString	m_ss_label;
  CString	m_prom_label;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFeatureEdit_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CFeatureEdit_dlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnChangeElev();
  virtual void OnOK();
  afx_msg void OnEdited();
  afx_msg void OnFormat();
  afx_msg void OnDelete();
  afx_msg void OnView();
  afx_msg void OnData();
  afx_msg void OnBrowse();
  afx_msg void OnRevert();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FEATUREEDIT_DLG_H__E69B0B81_C778_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
