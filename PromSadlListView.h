// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_PROMSADLLISTVIEW_H__A808BBA1_CD87_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_PROMSADLLISTVIEW_H__A808BBA1_CD87_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PromSadlListView.h : header file
//

#include "GroupListView.h"


struct PromSadl {
  Elev_intvl prom,usable_peak_elev,usable_sadl_elev;
  Domain *peak;
  PromSadl() {}
  PromSadl(const Elev_intvl& p) : prom(p),peak(0) {}
  PromSadl(const Elev_intvl& upe, const Elev_intvl& use, Domain *pk) :
    prom(upe-use),usable_peak_elev(upe),usable_sadl_elev(use),peak(pk) {}
};


/////////////////////////////////////////////////////////////////////////////
// CPromSadlListView view

class CPromSadlListView : public CGroupListView
{
  CArray<PromSadl,PromSadl&> saddle_list;
  void add_item(Feature&, FT_index, Domain&, Elev_intvl, Elev_intvl);
  static void add_sec_item(Feature&, FT_index,
    const Elev_intvl&, const Elev_intvl&, bool);
  void compress_master_list();

protected:
  CPromSadlListView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CPromSadlListView)

  short UPE_col,USE_col;
  enum {PROM_COL=2};
  virtual bool begins_group(int i) {return GetFeatureType(i)==FT_PEAK;}

// Attributes
public:
  struct List_info : Prom_sadl_filter {
    Domain::Index ndom;
    Domain **doms;
    bool UPE_col;
    List_info() : doms(0),UPE_col(false) {}
    List_info(Domain&);
    List_info(List_info& a) : doms(0) {(*this)=a;}
    ~List_info() {delete[] doms;}
    void store(Domain::Index);
    void transfer(List_info&);
  };

private:
  static List_info init_info;
  List_info relist_info;

// Operations
public:
  static void init_list(CPromDoc *, Domain&, CWnd *, Elevation=0);
  static void init_list(CPromDoc *, List_info&, CWnd *);
  virtual void UpdateItem(int);
  virtual void printFeature(int, FILE *, Format_style);
  virtual void printHeader(FILE *, Format_style);
  virtual bool prelist();
  virtual Elevation default_min_prom() const {return relist_info.min_prom;}
  virtual bool default_offmap() const
    {return relist_info.off_sadls || relist_info.runoffs;}
  virtual void SetViewTitle();

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CPromSadlListView)
  public:
  virtual void OnInitialUpdate();
  protected:
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  //}}AFX_VIRTUAL

// Implementation
protected:
  virtual ~CPromSadlListView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CPromSadlListView)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnListWalk();
  afx_msg void OnUpdateListWalk(CCmdUI* pCmdUI);
  //}}AFX_MSG
  afx_msg void OnUnlistItem(NMHDR *pNMHDR, LRESULT *pResult);
  DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROMSADLLISTVIEW_H__A808BBA1_CD87_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
