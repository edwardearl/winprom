// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_RUNOFFLISTVIEW_H__9C93FD82_C999_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_RUNOFFLISTVIEW_H__9C93FD82_C999_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RunoffListView.h : header file
//

#include "FeatureListView.h"
#include "RunoffList_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CRunoffListView view

class CRunoffListView : public CFeatureListView
{
  static bool is_initial;
  RunoffFilter runoff_filter;
  static RunoffFilter init_rf;

protected:
  CRunoffListView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CRunoffListView)

  enum {IQ_COL=2};

// Attributes
public:

// Operations
public:
  static void init_list(CPromDoc *);
  virtual void UpdateItem(int);
  virtual void printFeature(int, FILE *, Format_style);
  virtual void printHeader(FILE *, Format_style);
  virtual bool CanPaste(const Clip&) const;
  virtual bool prelist();
  virtual bool CanRelist() const {return true;}
  virtual void SetViewTitle();

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CRunoffListView)
  public:
  virtual void OnInitialUpdate();
  //}}AFX_VIRTUAL

// Implementation
protected:
  virtual ~CRunoffListView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CRunoffListView)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  //}}AFX_MSG
  afx_msg void OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult);
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNOFFLISTVIEW_H__9C93FD82_C999_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
