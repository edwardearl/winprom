// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_LPRTREEVIEW_H__D6E36D81_28A9_11D4_8FA1_0040056FBD08__INCLUDED_)
#define AFX_LPRTREEVIEW_H__D6E36D81_28A9_11D4_8FA1_0040056FBD08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LPRtreeView.h : header file
//

#include "Format.h"
#include "TreeFilter_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CLPRtreeView view

#define MAX_HPINFO 8

class CLPRtreeView : public CTreeView
{
  void ListSadl(Feature&) const;
  void ListCycle(Feature&) const;
  void ListParents(Feature&) const;
  void ListNbrs(Feature&, FT_index) const;

  struct HPinfo {
    Domain *HP;
    Elevation inner_prom,outer_prom;
    HPinfo() : HP(0) {}
  };

  static bool is_initial;
  static Domain *init_patriarch;
  static HPinfo lastHPs[MAX_HPINFO];
  static HPinfo *getHPinfo(Domain *);
  static void useHP(Domain *, Elevation, Elevation);
  static Feature_fmt init_fmt;
  Feature_fmt tree_fmt;
  Domain *patriarch;
  Elev_endpt bcp_field,wcp_field;
  int drg_const;
  Feature& GetFeature(HTREEITEM) const;
  FT_index GetFeatureType(HTREEITEM) const;
  FT_index GetFeatureImage(HTREEITEM) const;
  Feature& SelectedFeature() const;
  FT_index SelectedFeatureType() const;
  Domain *tree_parent(Domain *d) const
    {return d->get_parent(tree_filter.topology,tree_filter.min_prom);}
  void SetBranchText(const Domain&) const;
  HTREEITEM NewDom(Domain *, HTREEITEM=TVI_ROOT);
  Runoff *NewBranch(Domain *, HTREEITEM&, HTREEITEM=TVI_ROOT);
  HTREEITEM AddDomain(Domain::Index, bool);
  HTREEITEM AddItem(HTREEITEM, Feature&, FT_index, HTREEITEM=TVI_LAST);
  void AdjustBranch(HTREEITEM);
  HTREEITEM GetFeatureHandle(HTREEITEM, const Feature *) const;
  void add_bs_leg(Basin_saddle&, Domain *, Divide_tree&);
  void UpdateItem(HTREEITEM);
  void UpdateBranch(HTREEITEM, Feature *);
  void CopyBranch(HTREEITEM, Clip&) const;
  void PrintBranch(FILE *, HTREEITEM, bool=false) const;
  void BranchSummary(HTREEITEM, int&, Elev_intvl&, Rectangl&) const;
  bool checkPaste(Domain *, HTREEITEM&, Feature&, FT_index);
  HTREEITEM pasteLineage(Domain *);
  bool tree_writeable()
    {return GetDocument()->tree_writeable && !GetDocument()->working;}

  bool inquirePaste;
  DWORD actPaste;

  Feature *r_click_featr;
  FT_index r_click_type;
  HTREEITEM r_click_item;
  HTREEITEM *tree_item;
  static TreeFilter init_tf;

  friend class CFeatureListSummary_dlg;

protected:
  CLPRtreeView();  // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CLPRtreeView)

// Attributes
public:
  TreeFilter tree_filter;

// Operations
public:
  CPromDoc *GetDocument() const {return (CPromDoc *)CTreeView::GetDocument();}
  static void init_tree(CPromDoc *, Domain&, CWnd *);
  virtual void SetViewTitle();
  void copyClip(Clip&) const;
  void pasteClip(const Clip&);
  static void factory_initial_settings();
  static void load_initial_settings();
  static void save_initial_settings();

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CLPRtreeView)
  public:
  virtual void OnInitialUpdate();
  protected:
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  //}}AFX_VIRTUAL

// Implementation
protected:
  virtual ~CLPRtreeView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CLPRtreeView)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnEditFeature();
  afx_msg void OnEditCopy();
  afx_msg void OnEditSplice();
  afx_msg void OnUpdateEditSplice(CCmdUI* pCmdUI);
  afx_msg void OnEditCenter();
  afx_msg void OnListFormat();
  afx_msg void OnListRelist();
  afx_msg void OnEditRotate();
  afx_msg void OnUpdateEditRotate(CCmdUI* pCmdUI);
  afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnEditUnlist();
  afx_msg void OnListSadl();
  afx_msg void OnUpdateListSadl(CCmdUI* pCmdUI);
  afx_msg void OnListNbrs();
  afx_msg void OnUpdateListNbrs(CCmdUI* pCmdUI);
  afx_msg void OnFileText();
  afx_msg void OnFileHtml();
  afx_msg void OnUpdateHaveItems(CCmdUI* pCmdUI);
  afx_msg void OnUpdateEditUnlist(CCmdUI* pCmdUI);
  afx_msg void OnListSummary();
  afx_msg void OnEditDelete();
  afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
  afx_msg void OnEditSearch();
  afx_msg void OnUpdateEditSearch(CCmdUI* pCmdUI);
  afx_msg void OnEditPaste();
  afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
  afx_msg void OnListTree();
  afx_msg void OnUpdateListTree(CCmdUI* pCmdUI);
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnFeatureCenter();
  afx_msg void OnFeatureDelete();
  afx_msg void OnFeatureEdit();
  afx_msg void OnFeatureListNbrs();
  afx_msg void OnFeatureListSaddles();
  afx_msg void OnFeatureListTree();
  afx_msg void OnFeatureUnlist();
  afx_msg void OnRunoffSplice();
  afx_msg void OnUpdateRealFeature(CCmdUI* pCmdUI);
  afx_msg void OnUpdateEditFeature(CCmdUI* pCmdUI);
  afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnEditBrowse();
  afx_msg void OnFeatureBrowse();
  afx_msg void OnUpdateHaveDLPRtree(CCmdUI* pCmdUI);
  afx_msg void OnRQFinfo();
  afx_msg void OnEditRQF();
  afx_msg void OnUpdateEditRQF(CCmdUI* pCmdUI);
  afx_msg void OnFeatureListParents();
  afx_msg void OnListParents();
  afx_msg void OnUpdateListParents(CCmdUI* pCmdUI);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LPRTREEVIEW_H__D6E36D81_28A9_11D4_8FA1_0040056FBD08__INCLUDED_)
