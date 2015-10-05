// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_IMPOUT_DLG_H__745ED562_31FC_11D6_A239_608552C10000__INCLUDED_)
#define AFX_IMPOUT_DLG_H__745ED562_31FC_11D6_A239_608552C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImpOut_dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CImpOut_dlg dialog

class CImpOut_dlg : public CDialog
{
  const GridPoint location,*max_loc;
  const double *max_val,(*avg_y_ring)[2],(*avg_y_inf)[2],(*int_dist)[8],
    dist_intvl,nearedge,conv_fac;
  unsigned ndist;
  CMapView *view;

  void set_type();

// Construction
public:
  CImpOut_dlg(const GridPoint&, double, double, const double *, const GridPoint *,
	      const double (*)[8], const double (*)[2], const double (*)[2],
	      unsigned, double, CMapView *, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CImpOut_dlg)
  enum { IDD = IDD_IMPOUT };
  CListBox	m_dist;
  CTabCtrl	m_type;
  float		m_spdir;
  float		m_sp_dist;
  CString	m_sploc;
  int		m_spval;
  int		m_intval;
  CString	m_location;
  float		m_nearedge;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CImpOut_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CImpOut_dlg)
  afx_msg void OnSelchangeType(NMHDR* pNMHDR, LRESULT* pResult);
  virtual BOOL OnInitDialog();
  afx_msg void OnToggle();
  afx_msg void OnCenter();
  afx_msg void OnCenterSP();
  afx_msg void OnToggleSP();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPOUT_DLG_H__745ED562_31FC_11D6_A239_608552C10000__INCLUDED_)
