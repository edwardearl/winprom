// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_PARENTLISTVIEW_H__9342270D_DE62_4A4D_96BB_78A8CCC0FD42__INCLUDED_)
#define AFX_PARENTLISTVIEW_H__9342270D_DE62_4A4D_96BB_78A8CCC0FD42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParentListView.h : header file
//

#include "GroupListView.h"


/////////////////////////////////////////////////////////////////////////////
// CParentListView view

class CParentListView : public CGroupListView
{
  struct Prom_sadl {
    Feature *sadl;
    Elev_intvl usable_peak_elev,usable_sadl_elev;
    bool offmap;
    Prom_sadl(Feature *s, const Elev_intvl& upe, const Elev_intvl& use, bool om) :
      sadl(s),usable_peak_elev(upe),usable_sadl_elev(use),offmap(om) {}
    Elev_intvl prom() const {return usable_peak_elev-usable_sadl_elev;}
  };
  vector<Prom_sadl> prom_sadls; // the actual prominence saddles
  static void add_prom_sadl(Feature&, FT_index,
		const Elev_intvl&, const Elev_intvl& use, bool);
  static void add_parent(Feature&, FT_index);
  int get_prom_sadl(Feature *) const;
  void list_parents(const Prom_sadl&);

protected:
  CParentListView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CParentListView)

  virtual bool begins_group(int i) {return GetFeatureType(i)==FT_SADDLE;}

  enum {PROM_COL=2};

// Attributes
public:
  struct List_info {
    Domain::Topology topology;
    Domain *peak;
    vector<Feature *> sadls; // the saddles provided when the list was generated
    Elevation min_prom;
    Prom_sadl_filter filter;
    bool runoffs;
    List_info() : peak(0),topology(Domain::TOPO_ISLAND),runoffs(false) {}
  };

private:
  static bool is_initial;
  static List_info init_info;
  List_info relist_info;

// Operations
public:
  static void init_list(CPromDoc *, Domain&, CWnd *, Elevation=0);
  static void init_list(CPromDoc *, List_info&, CWnd *, Elevation=0);
  virtual void UpdateItem(int);
  virtual bool prelist();
  virtual Elevation default_min_prom() const {return relist_info.min_prom;}
  virtual void SetViewTitle();

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CParentListView)
  public:
  virtual void OnInitialUpdate();
  //}}AFX_VIRTUAL

// Implementation
protected:
  virtual ~CParentListView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CParentListView)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARENTLISTVIEW_H__9342270D_DE62_4A4D_96BB_78A8CCC0FD42__INCLUDED_)
