// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_BASINSADLLISTVIEW_H__9C93FD83_C999_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_BASINSADLLISTVIEW_H__9C93FD83_C999_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// BasinSadlListView.h : header file
//

#include "FeatureListView.h"
#include "ListSadl_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CBasinSadlListView view

class CBasinSadlListView : public CFeatureListView
{
  friend int CALLBACK compare_stat(LPARAM, LPARAM, LPARAM);
  friend int CALLBACK compare_prom(LPARAM, LPARAM, LPARAM);
  friend int CALLBACK compare_omp(LPARAM, LPARAM, LPARAM);
  static bool is_initial;
  vector<PromListItem> bs_list;
  SadlListFilter list_filter;

protected:
  CBasinSadlListView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CBasinSadlListView)

  virtual void swap_items(int);

  enum {CWR_COL=2,CWRO_COL=3,CYCLE_COL=4,STAT_COL=5,PROM_COL=6,OMP_COL=7};

// Attributes
public:
  static SadlListFilter init_lf;

// Operations
public:
  static void init_list(CPromDoc *);
  virtual void UpdateItem(int);
  virtual void printFeature(int, FILE *, Format_style);
  virtual void printHeader(FILE *, Format_style);
  virtual bool CanPaste(const Clip&) const;
  virtual bool has_extra_info() const {return true;}
  virtual bool prelist();
  virtual bool CanRelist() const {return true;}
  virtual Elevation default_min_prom() const {return list_filter.min_prom;}
  virtual bool default_offmap() const {return list_filter.offmap;}
  virtual bool default_ridge() const {return list_filter.ridge;}
  virtual bool default_cell() const {return list_filter.cell;}
  virtual void SetViewTitle();
  virtual void pasteClip(const Clip&);

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CBasinSadlListView)
  public:
  virtual void OnInitialUpdate();
  protected:
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  //}}AFX_VIRTUAL

// Implementation
protected:
  virtual ~CBasinSadlListView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CBasinSadlListView)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnListSadl();
  afx_msg void OnUpdateListSadl(CCmdUI* pCmdUI);
  //}}AFX_MSG
  afx_msg void OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnUnlistItem(NMHDR *pNMHDR, LRESULT *pResult);
  DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASINSADLLISTVIEW_H__9C93FD83_C999_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
