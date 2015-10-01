#if !defined(AFX_FEATURELISTVIEW_H__01A5A941_C6C4_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_FEATURELISTVIEW_H__01A5A941_C6C4_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FeatureListView.h : header file
//

#include "PromDoc.h"
#include "Format.h"
#include "FeatureTypeFilter_dlg.h"


struct Clip;


/////////////////////////////////////////////////////////////////////////////
// CFeatureListView view

class CFeatureListView : public CListView
{
  void ListNbrs(Feature&, FT_index) const;
  void load_list(bool);
  bool get_select_filter();
  const char *title;

protected:
  CFeatureListView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CFeatureListView)

  short name_col,elev_col,loc_col;
  Feature_fmt list_fmt;
  int InsertColumn(int, const char *, int, int);
  int DeleteColumn(int);
  virtual void swap_items(int);
  void setItemIndicators();
  bool tree_writeable()
    {return GetDocument()->tree_writeable && !GetDocument()->working;}
  void select_extreme_saddles(bool);

  Feature *r_click_featr;
  FT_index r_click_type;
  int r_click_item;

  friend class CFeatureListSummary_dlg;

// Attributes
public:
  static char buf[256];
  FeatureTypeFilter select_filter;
  vector<const Feature *> features; // used only for scratch in sorting lists with extra info

// Operations
public:
  CPromDoc *GetDocument() const {return (CPromDoc *)CListView::GetDocument();}
  void setViewStyle(DWORD);
  void setStyle(DWORD);
  void clearStyle(DWORD);
  void toggleStyle(DWORD);
  virtual bool CanRelist() const {return false;}
  DWORD GWL() const
    {return GetWindowLong(GetListCtrl().GetSafeHwnd(),GWL_STYLE);}

  static CFeatureListView *init_list(CPromDoc *, const char *t="Features");
  void AddItem(Feature&, FT_index);
  int AddBlankItem(Feature&, FT_index);
  virtual void UpdateItem(int);
  virtual bool CanPaste(const Clip&) const {return true;}
  virtual bool CanSort() const {return true;}
  virtual bool has_extra_info() const {return false;}
  void build_features();
  void UpdateAllItems();
  virtual bool prelist();
  virtual Elevation default_min_prom() const {return 0;}
  virtual bool default_offmap() const {return false;}
  virtual bool default_ridge() const {return false;}
  virtual bool default_cell() const {return false;}
  Feature& GetFeature(int nItem) const
    {return *(Feature *)GetListCtrl().GetItemData(nItem);}
  FT_index GetFeatureType(int) const;
  FT_index GetFeatureImage(int) const;
  virtual void printFeature(int, FILE *, Format_style);
  virtual void printHeader(FILE *, Format_style);
  virtual void SetViewTitle();
  void copyClip(Clip&) const;
  virtual void pasteClip(const Clip&);

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFeatureListView)
  public:
  virtual void OnInitialUpdate();
  protected:
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  //}}AFX_VIRTUAL

// Implementation
protected:
  virtual ~CFeatureListView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CFeatureListView)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnViewReport();
  afx_msg void OnUpdateViewReport(CCmdUI* pCmdUI);
  afx_msg void OnViewSmall();
  afx_msg void OnUpdateViewSmall(CCmdUI* pCmdUI);
  afx_msg void OnViewLarge();
  afx_msg void OnUpdateViewLarge(CCmdUI* pCmdUI);
  afx_msg void OnViewAlign();
  afx_msg void OnUpdateViewAlign(CCmdUI* pCmdUI);
  afx_msg void OnViewArrange();
  afx_msg void OnUpdateViewArrange(CCmdUI* pCmdUI);
  afx_msg void OnFileText();
  afx_msg void OnFileHtml();
  afx_msg void OnListRelist();
  afx_msg void OnUpdateListRelist(CCmdUI* pCmdUI);
  afx_msg void OnListFormat();
  afx_msg void OnListSummary();
  afx_msg void OnEditUnlist();
  afx_msg void OnUpdateEditAnySelect(CCmdUI* pCmdUI);
  afx_msg void OnEditFeature();
  afx_msg void OnEditSelectAll();
  afx_msg void OnEditSelectInvert();
  afx_msg void OnEditSelectFilterClear();
  afx_msg void OnEditSelectFilterPrecise();
  afx_msg void OnEditSelectFilterSet();
  afx_msg void OnEditDelete();
  afx_msg void OnListSadl();
  afx_msg void OnUpdateListSadl(CCmdUI* pCmdUI);
  afx_msg void OnEditCopy();
  afx_msg void OnEditPaste();
  afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
  afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnListNbrs();
  afx_msg void OnUpdateListNbrs(CCmdUI* pCmdUI);
  afx_msg void OnEditRotate();
  afx_msg void OnUpdateEditRotate(CCmdUI* pCmdUI);
  afx_msg void OnEditCenter();
  afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
  afx_msg void OnUpdateEditSplice(CCmdUI* pCmdUI);
  afx_msg void OnEditSplice();
  afx_msg void OnUpdateHaveItems(CCmdUI* pCmdUI);
  afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnViewList();
  afx_msg void OnUpdateViewList(CCmdUI* pCmdUI);
  afx_msg void OnListTree();
  afx_msg void OnUpdateListTree(CCmdUI* pCmdUI);
  afx_msg void OnEditSearch();
  afx_msg void OnUpdateEditSearch(CCmdUI* pCmdUI);
  afx_msg void OnFeatureCenter();
  afx_msg void OnFeatureDelete();
  afx_msg void OnFeatureEdit();
  afx_msg void OnFeatureListNbrs();
  afx_msg void OnFeatureListSaddles();
  afx_msg void OnFeatureListTree();
  afx_msg void OnFeatureUnlist();
  afx_msg void OnBasinsadlRotate();
  afx_msg void OnRunoffSplice();
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnUpdateListWalk(CCmdUI* pCmdUI);
  afx_msg void OnListWalk();
  afx_msg void OnUpdateEditSingleSelect(CCmdUI* pCmdUI);
  afx_msg void OnUpdateEditFeature(CCmdUI* pCmdUI);
  afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnEditSWI();
  afx_msg void OnUpdateEditSWI(CCmdUI* pCmdUI);
  afx_msg void OnFileCSV();
  afx_msg void OnEditBrowse();
  afx_msg void OnFeatureBrowse();
  afx_msg void OnUpdateHaveDLPRtree(CCmdUI* pCmdUI);
  afx_msg void OnRQFinfo();
  afx_msg void OnEditRQF();
  afx_msg void OnUpdateEditRQF(CCmdUI* pCmdUI);
  afx_msg void OnListSave();
  afx_msg void OnListLoad();
  afx_msg void OnListAppend();
  afx_msg void OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnListParents();
  afx_msg void OnUpdateListParents(CCmdUI* pCmdUI);
  afx_msg void OnEditMoveUp();
  afx_msg void OnUpdateEditMoveUp(CCmdUI* pCmdUI);
  afx_msg void OnEditMoveDown();
  afx_msg void OnUpdateEditMoveDown(CCmdUI* pCmdUI);
  afx_msg void OnFeatureListParents();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FEATURELISTVIEW_H__01A5A941_C6C4_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
