// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_CYCLESADL_DLG_H__316DF760_9ED5_11D4_8FA2_0040056FBD08__INCLUDED_)
#define AFX_CYCLESADL_DLG_H__316DF760_9ED5_11D4_8FA2_0040056FBD08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CycleSadl_dlg.h : header file
//

#include "ListSadl_dlg.h"


struct CycleFilter : SadlListFilter {
  Domain::Index nbs;
  Basin_saddle **bs;
  bool nonoffend;
  CycleFilter(Elevation=-1);
  CycleFilter(Basin_saddle&);
  CycleFilter(CycleFilter& a) : bs(0) {(*this)=a;}
  ~CycleFilter() {delete[] bs;}
  void store(Domain::Index);
  void transfer(CycleFilter&);
};


/////////////////////////////////////////////////////////////////////////////
// CCycleSadl_dlg dialog

class CCycleSadl_dlg : public CListSadl_dlg
{
  CycleFilter& filter;

// Construction
public:
  CCycleSadl_dlg(const Database&, CycleFilter&, CWnd *);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CCycleSadl_dlg)
  enum { IDD = IDD_CYCLE };
  BOOL	m_nonoffend;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CCycleSadl_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CCycleSadl_dlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CYCLESADL_DLG_H__316DF760_9ED5_11D4_8FA2_0040056FBD08__INCLUDED_)
