#if !defined(AFX_FEATUREFILTER_DLG_H__86E5ECE1_CAD0_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_FEATUREFILTER_DLG_H__86E5ECE1_CAD0_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FeatureFilter_dlg.h : header file
//

#include "interval.h"
#include "point.h"
#include "Database.h"


/////////////////////////////////////////////////////////////////////////////
// CFeatureFilter_dlg dialog

class CFeatureFilter_dlg : public CDialog
{
  FeatureFilter& filter;
  void init();

// Construction/destruction
protected:
  CFeatureFilter_dlg(UINT, FeatureFilter&, CWnd *);
  bool close();
public:
  CFeatureFilter_dlg(FeatureFilter&, CWnd *);

// Dialog Data
  //{{AFX_DATA(CFeatureFilter_dlg)
  enum { IDD = IDD_FEATURE_FILTER };
  CEdit		m_limit_w_ctl;
  CEdit		m_limit_s_ctl;
  CEdit		m_limit_n_ctl;
  CEdit		m_limit_e_ctl;
  CEdit		m_elev_low_ctl;
  CEdit		m_elev_high_ctl;
  BOOL		m_case;
  BOOL		m_edge;
  int		m_name_filter;
  CString	m_name;
  CString	m_elev_high;
  CString	m_elev_low;
  CString	m_limit_e;
  CString	m_limit_n;
  CString	m_limit_s;
  CString	m_limit_w;
  BOOL		m_nonedge;
  BOOL		m_strict;
  BOOL		m_isolatin1;
  BOOL		m_edit;
  BOOL		m_unedit;
  //}}AFX_DATA
  Record_filter m_data_filter;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFeatureFilter_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CFeatureFilter_dlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  afx_msg void OnData();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FEATUREFILTER_DLG_H__86E5ECE1_CAD0_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
