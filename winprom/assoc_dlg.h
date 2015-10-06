// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_ALIGN_DLG_H__5F842D27_28BD_46AD_8233_6F60DCA95319__INCLUDED_)
#define AFX_ALIGN_DLG_H__5F842D27_28BD_46AD_8233_6F60DCA95319__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Assoc_dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CAssoc_dlg dialog

class CAssoc_dlg : public CDialog
{
  Assoc_info& target;
// Construction
public:
  CAssoc_dlg(Assoc_info&, bool, bool, bool, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CAssoc_dlg)
	enum { IDD = IDD_ASSOC };
	CEdit	m_cpdm_edit;
  CEdit	m_radius_edit;
  CButton	m_path_ctl;
  CButton	m_wkup_ctl;
  CButton	m_dom_ctl;
  BOOL	m_domain;
  BOOL	m_walkup;
  float	m_radius;
  BOOL	m_peak_dest;
  BOOL	m_saddle_dest;
  BOOL	m_elev_prom;
  BOOL	m_bs_dest;
  BOOL	m_ro_dest;
  BOOL	m_path;
  int	m_bs_prom_type;
  float	m_cpdm;
  //}}AFX_DATA
  bool have_elev,have_dom,have_topo;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CAssoc_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CAssoc_dlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALIGN_DLG_H__5F842D27_28BD_46AD_8233_6F60DCA95319__INCLUDED_)
