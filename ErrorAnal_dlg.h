// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_ERRORANAL_DLG_H__0CD72A01_C994_11D8_A23B_30F753C10000__INCLUDED_)
#define AFX_ERRORANAL_DLG_H__0CD72A01_C994_11D8_A23B_30F753C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ErrorAnal_dlg.h : header file
//


struct Error_stats {
  float sigma_cal,sigma_app;
  bool separam;
  AnalData peak_data,sadl_data;
  Error_stats() : sigma_cal(sqrt(3.0)),sigma_app(2),separam(false) {}
  AnalData *get_sadl_data() {return separam?&sadl_data:&peak_data;}
  const AnalData *get_data(FT_index ft) const
    {return separam && ft!=FT_PEAK ? &sadl_data : &peak_data;}
  void summarize();
};

class CPromDoc;

/////////////////////////////////////////////////////////////////////////////
// CErrorAnal_dlg dialog

class CErrorAnal_dlg : public CDialog
{
  Error_stats& error_info;
  const CPromDoc& document;
  void DoExchangeFixedOptions(bool);
  void DoExchangeStatOptions(bool);
  void UpdateStatParams();
  void set_tabs();
  void update_option_ctls();
  void update_stat_param_ctls();
  void get_stat_options();
  AnalData *active_data();
  void list_stats();
  void compute_stats();
  void compute_stats(AnalData *);

// Construction
public:
  CErrorAnal_dlg(Error_stats&, const CPromDoc&, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CErrorAnal_dlg)
  enum { IDD = IDD_ERROR_ANAL };
  CButton	m_more_params;
  CTabCtrl	m_featr_type_tab;
  float	m_bias;
  BOOL	m_enable_fixed;
  float	m_fixed_error;
  float	m_mahal;
  int	m_num_featr;
  float	m_raw_error;
  float	m_resid_error;
  float	m_sigma_cal;
  float	m_sigma_app;
  int	m_stat_model;
  BOOL	m_separam;
  //}}AFX_DATA
  AnalData m_peak_data,m_sadl_data;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CErrorAnal_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CErrorAnal_dlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnChangeStatOptions();
  afx_msg void OnChangeFeatrType(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnChangeFixedOptions();
  afx_msg void OnFilter();
  afx_msg void OnMoreParams();
  afx_msg void OnOptions();
  virtual void OnOK();
  afx_msg void OnSeparam();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ERRORANAL_DLG_H__0CD72A01_C994_11D8_A23B_30F753C10000__INCLUDED_)
