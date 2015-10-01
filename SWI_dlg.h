#if !defined(AFX_SWI_DLG_H__73143E40_FC99_11D5_A238_908352C10000__INCLUDED_)
#define AFX_SWI_DLG_H__73143E40_FC99_11D5_A238_908352C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SWI_dlg.h : header file
//

#include "FeatureFilter_dlg.h"


struct SWI_filter {
  bool nondef,flat,basin;
  bool filter_both;
  FeatureFilter sadl_filter;
  SWI_filter() : nondef(true),flat(false),basin(true),filter_both(false) {}
  bool test(const Feature&, const Database&) const;
  bool test(const Feature&, const Feature&,
	    const Elev_map&, const Database&) const;
};


/////////////////////////////////////////////////////////////////////////////
// CSWI_dlg dialog

class CSWI_dlg : public CDialog
{
  SWI_filter& target;
  FeatureFilter temp_ff;
  const Database& data;
  bool drainage;

// Construction
public:
  CSWI_dlg(const CPromDoc&, SWI_filter&, CWnd *pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CSWI_dlg)
  enum { IDD = IDD_SWI };
  CButton	m_basin_ctl;
  int	m_filter_both;
  BOOL	m_nondef;
  BOOL	m_flat;
  BOOL	m_basin;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CSWI_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CSWI_dlg)
  afx_msg void OnFilter();
  virtual void OnOK();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SWI_DLG_H__73143E40_FC99_11D5_A238_908352C10000__INCLUDED_)
