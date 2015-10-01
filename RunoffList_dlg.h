#if !defined(AFX_RUNOFFLIST_DLG_H__19A8E6E3_CB53_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_RUNOFFLIST_DLG_H__19A8E6E3_CB53_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RunoffList_dlg.h : header file
//

#include "FeatureFilter_dlg.h"
#include "domain.h"


struct RunoffFilter : FeatureFilter {
  bool IQ_0,IQ_1,IQ_2,IQ_3,edge,corner,interior;
  RunoffFilter(bool ej=true) : FeatureFilter(ej),
    IQ_0(true),IQ_1(true),IQ_2(true),IQ_3(true),
    edge(true),corner(true),interior(true) {}
  bool test(const Runoff&, const Database&) const;
};


/////////////////////////////////////////////////////////////////////////////
// CRunoffList_dlg dialog

class CRunoffList_dlg : public CDialog
{
  RunoffFilter& filter;
  FeatureFilter temp_ff;
  const Database& data;
  bool drainage;

// Construction
public:
  CRunoffList_dlg(const Database&, RunoffFilter&, bool,
		  CWnd *parent=NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CRunoffList_dlg)
  enum { IDD = IDD_RUNOFF_LIST };
  BOOL	m_edge;
  BOOL	m_interior;
  BOOL	m_IQ_1;
  BOOL	m_IQ_2;
  BOOL	m_IQ_3;
  BOOL	m_corner;
  BOOL	m_IQ_0;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CRunoffList_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CRunoffList_dlg)
  afx_msg void OnFilter();
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNOFFLIST_DLG_H__19A8E6E3_CB53_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
