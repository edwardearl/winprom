#if !defined(AFX_ERRORALLOW_DLG_H__4155BC22_EB5D_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_ERRORALLOW_DLG_H__4155BC22_EB5D_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ErrorAllow_dlg.h : header file
//

#include "FeatureFilter_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CErrorAllow_dlg dialog

class CErrorAllow_dlg : public CDialog
{
  const CPromDoc& document;
  void set_tabs();
  void update_stats();
  void set_params();
  bool getRQF(const Feature&, double&, double&, double&, FT_index);
  void add_stat(const Feature&, FT_index type);

// Construction
public:
  CErrorAllow_dlg(const CPromDoc&, CWnd *pParent = NULL);   // standard constructor
  Regress peak_data,sadl_data;

// Dialog Data
  //{{AFX_DATA(CErrorAllow_dlg)
  enum { IDD = IDD_ERROR_ALLOWANCE };
  CButton	m_params_butn;
  CTabCtrl	m_type_tab;
  CEdit	m_error_ctl;
  int	m_far;
  int	m_near;
  int	m_error;
  BOOL	m_separam;
  int	m_num_featr;
  float	m_raw_err;
  float	m_model_err;
  BOOL	m_parvar;
  float	m_sigma_in;
  float	m_sigma_out;
  float	m_mahal_val;
  float	m_bias;
  BOOL	m_elev_data;
  BOOL	m_range_fixed;
  BOOL	m_RQF_gp;
  int	m_RQF_stat;
  int	m_RQF_badhess;
  BOOL	m_regress;
  //}}AFX_DATA
  FeatureFilter m_app_filter,m_stat_filter;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CErrorAllow_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CErrorAllow_dlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnAppFilter();
  afx_msg void OnSeparam();
  afx_msg void OnStatFilter();
  virtual void OnOK();
  afx_msg void OnChangeFeatrType(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnParams();
  afx_msg void OnUpdateParams();
  afx_msg void OnJustUpdate();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ERRORALLOW_DLG_H__4155BC22_EB5D_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
