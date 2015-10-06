// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_CYCLELISTVIEW_H__60AE8841_E61C_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_CYCLELISTVIEW_H__60AE8841_E61C_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CycleListView.h : header file
//

#include "GroupListView.h"
#include "CycleSadl_dlg.h"


struct CycleSadl {
  Elev_intvl prom;
  Elevation offmap_prom;
  Basin_saddle *basinsadl;
  CycleSadl() {}
  CycleSadl(const Elev_intvl& p, Elevation omp, Basin_saddle *bs=0) :
    prom(p),offmap_prom(omp),basinsadl(bs) {}
};


/////////////////////////////////////////////////////////////////////////////
// CCycleListView view

class CCycleListView : public CGroupListView
{
  CArray<CycleSadl, CycleSadl&> saddle_list;
  void AddSaddle(Basin_saddle *, Domain *);

protected:
  CCycleListView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CCycleListView)

  enum {CWR_COL=2,CWRO_COL=3,STAT_COL=4,PROM_COL=5,OMP_COL=6};
  virtual bool begins_group(int i) {return GetFeatureType(i)==FT_BASINSADL;}

// Attributes
public:

private:
  static CycleFilter init_filter;
  CycleFilter relist_filter;

// Operations
public:
  static void init_list(CPromDoc *, Basin_saddle&, CWnd *,
			Elevation=-1, bool=false, bool=false, bool=false);
  static void init_list(CPromDoc *, CycleFilter&, CWnd *);
  virtual void UpdateItem(int);
  virtual void printFeature(int, FILE *, Format_style);
  virtual void printHeader(FILE *, Format_style);
  virtual bool prelist();
  virtual Elevation default_min_prom() const {return relist_filter.min_prom;}
  virtual bool default_offmap() const {return relist_filter.offmap;}
  virtual bool default_ridge() const {return relist_filter.ridge;}
  virtual bool default_cell() const {return relist_filter.cell;}
  virtual void SetViewTitle();

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CCycleListView)
  public:
  virtual void OnInitialUpdate();
  protected:
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  //}}AFX_VIRTUAL

// Implementation
protected:
  virtual ~CCycleListView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CCycleListView)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnEditRotate();
  afx_msg void OnUpdateEditRotate(CCmdUI* pCmdUI);
  //}}AFX_MSG
  afx_msg void OnUnlistItem(NMHDR *pNMHDR, LRESULT *pResult);
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CYCLELISTVIEW_H__60AE8841_E61C_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
