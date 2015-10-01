#if !defined(AFX_FEATURELISTSUMMARY_DLG_H__19A8E6E2_CB53_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_FEATURELISTSUMMARY_DLG_H__19A8E6E2_CB53_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FeatureListSummary_dlg.h : header file
//

#include "FeatureListView.h"
#include "LPRtreeView.h"
#include "LocationFmt_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CFeatureListSummary_dlg dialog

class CFeatureListSummary_dlg : public CDialog
{
  Elev_intvl elev;
  Rectangl area;
  Location_fmt summary_fmt;
  void reformat();
  void init();

// Construction
public:
  CFeatureListSummary_dlg(CFeatureListView *, UINT=CFeatureListSummary_dlg::IDD);
  CFeatureListSummary_dlg(CLPRtreeView *);

// Dialog Data
  //{{AFX_DATA(CFeatureListSummary_dlg)
  enum { IDD = IDD_FEATURE_SUMMARY };
  CString	m_area_e;
  CString	m_area_n;
  CString	m_area_s;
  CString	m_area_w;
  int		m_count;
  int		m_high;
  int		m_low;
  //}}AFX_DATA


// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFeatureListSummary_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CFeatureListSummary_dlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnFormat();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FEATURELISTSUMMARY_DLG_H__19A8E6E2_CB53_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
