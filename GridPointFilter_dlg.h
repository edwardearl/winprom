#if !defined(AFX_GRIDPOINTFILTER_DLG_H__7F3F19E0_37E4_11D4_8FA2_0040056FBD08__INCLUDED_)
#define AFX_GRIDPOINTFILTER_DLG_H__7F3F19E0_37E4_11D4_8FA2_0040056FBD08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridPointFilter_dlg.h : header file
//

#include "PromDoc.h"


struct GPfilter {
  CPromDoc& document;
  Elev_intvl elev;
  Domain *dom;
  Rectangl limits;
  bool dom_ancestors,dom_descendants;
  Domain::Topology topology;
  GPfilter(CPromDoc&, GridPoint&);
  bool test(const GridPoint&) const;
};


/////////////////////////////////////////////////////////////////////////////
// CGridPointFilter_dlg dialog

class CGridPointFilter_dlg : public CDialog
{
  GPfilter& target;
  GridPoint start;

// Construction
public:
  CGridPointFilter_dlg(GPfilter&, const GridPoint&);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CGridPointFilter_dlg)
  enum { IDD = IDD_GP_FILTER };
  CEdit	m_limit_w_ctl;
  CEdit	m_limit_s_ctl;
  CEdit	m_limit_n_ctl;
  CEdit	m_limit_e_ctl;
  CButton	m_desc_ctl;
  CButton	m_anc_ctl;
  CButton	m_all_ctl;
  CEdit		m_low_ctl;
  CEdit		m_high_ctl;
  BOOL		m_dlpr_all;
  BOOL		m_dlpr_ancestors;
  BOOL		m_dlpr_descendants;
  CString	m_elev_high;
  CString	m_elev_low;
  CString	m_limit_e;
  CString	m_limit_n;
  CString	m_limit_s;
  CString	m_limit_w;
  int		m_topology;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CGridPointFilter_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CGridPointFilter_dlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDPOINTFILTER_DLG_H__7F3F19E0_37E4_11D4_8FA2_0040056FBD08__INCLUDED_)
