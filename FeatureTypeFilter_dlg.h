#if !defined(AFX_FEATURETYPEFILTER_DLG_H__2B8B6256_90E7_4AD8_82CE_1D2EB6F5A13F__INCLUDED_)
#define AFX_FEATURETYPEFILTER_DLG_H__2B8B6256_90E7_4AD8_82CE_1D2EB6F5A13F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FeatureTypeFilter_dlg.h : header file
//

#include "FeatureFilter_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CFeatureTypeFilter_dlg dialog

class CFeatureTypeFilter_dlg : public CFeatureFilter_dlg
{
  FeatureTypeFilter& filter;

// Construction
public:
  CFeatureTypeFilter_dlg(FeatureTypeFilter&, CWnd *pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CFeatureTypeFilter_dlg)
  enum { IDD = IDD_FEATURE_TYPE_FILTER };
  BOOL	m_basin_saddles;
  BOOL	m_peaks;
  BOOL	m_runoffs;
  BOOL	m_saddles;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFeatureTypeFilter_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CFeatureTypeFilter_dlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FEATURETYPEFILTER_DLG_H__2B8B6256_90E7_4AD8_82CE_1D2EB6F5A13F__INCLUDED_)
