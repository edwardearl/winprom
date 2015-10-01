#if !defined(AFX_PROMSADL_DLG_H__B95D9D01_CE4D_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_PROMSADL_DLG_H__B95D9D01_CE4D_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PromSadl_dlg.h : header file
//

#include "PromSadlListView.h"


/////////////////////////////////////////////////////////////////////////////
// CPromSadl_dlg dialog

class CPromSadl_dlg : public CDialog
{
  Prom_sadl_filter& filter;
  FeatureFilter m_saddle_filter,m_runoff_filter;
  const CPromDoc& document;

// Construction
public:
  CPromSadl_dlg(const CPromDoc&, Prom_sadl_filter&, CWnd *);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CPromSadl_dlg)
  enum { IDD = IDD_PROMSADL };
  CButton	m_ro_filter_butn;
  CButton	m_upe_butn;
  CButton	m_runoff_butn;
  CButton	m_bs_butn;
  int	m_min_prom;
  BOOL	m_UPE;
  BOOL	m_runoffs;
  BOOL	m_offmap_saddles;
  BOOL	m_basnsadls;
  //}}AFX_DATA
  bool parent;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CPromSadl_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CPromSadl_dlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  afx_msg void OnSaddleFilter();
  afx_msg void OnRunoffFilter();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROMSADL_DLG_H__B95D9D01_CE4D_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
