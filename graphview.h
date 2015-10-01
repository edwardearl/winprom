// GraphView.h : interface of the CGraphView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHVIEW_H__9927AF50_C393_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_GRAPHVIEW_H__9927AF50_C393_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "FeatureFmt_dlg.h"
#include "FeatureFilter_dlg.h"

#define GRAPH_VIEW_MARGIN 48
#define GRAPH_MAX_DIM 32000
#define GRAPH_SCROLL_ARROW_KEY 32
#define GRAPH_SCROLL_ARROW_BUTN 16

#define LARGE_CLK_TOL 12
#define MEDIUM_CLK_TOL 9
#define SMALL_CLK_TOL 6
#define NO_CLK_TOL 3
#define LARGE_LABEL_RECT_SIDE 40
#define LARGE_LABEL_RECT_BOTM 65
#define SMALL_LABEL_RECT_WIDTH 192

#define POINT_UNDEF (-32768)

struct Tool_info {
  bool have_tool;
  Tool_info() : have_tool(false) {}
};


class CGraphView : public CScrollView
{
public:
  enum Label_state {LS_HIDDEN,LS_VISIBLE,LS_SELECTED};
private:
  bool peak_elev_cap;
  Domain::Index r_click_index;
  CPoint downPt;
  FeatureFilter tree_peak_xing,tree_sadl_xing,tree_peak_select,tree_sadl_select;
  int tree_filter_rel,tree_filter_action;
  bool bs_xing;
  //Tool_info *tool_info[FT_NUM_TYPES];
  //CToolTipCtrl tool_tip;

  Label_state *label_state[FT_NUM_TYPES];
  Domain::Index nsel_feature[FT_NUM_TYPES];
  void InvalidateFeature(Feature&, FT_index);
  void DrawRidge(const CPoint&, Feature *, FT_index);
  void DrawRidge(const CPoint&, const CPoint&);
  void DrawFeature(const Feature&, FT_index, Label_state, Elevation);
  Elevation get_saddle_prom(const Domain::Sadl_prom&) const;
  Elevation get_feature_prom(const Feature *, FT_index) const;
  Elevation get_peak_prom(const Domain&) const;
  Elevation get_saddle_prom(const Domain&) const;
  Elevation get_prom(const Runoff&) const;
  Elevation get_prom(const Basin_saddle&) const;
  bool mouseHit(CPoint, Feature&, FT_index, Elevation);
  Feature *HitPeak(CPoint, bool, Domain::Index&, FT_index&);
  Feature *HitSaddle(CPoint, bool, Domain::Index&, FT_index&);
  Feature *HitRunoff(CPoint, bool, Domain::Index&, FT_index&);
  Feature *HitBasnsadl(CPoint, bool, Domain::Index&, FT_index&);
  Feature *HitTest(CPoint, bool, Domain::Index&, FT_index&);
  Feature &FindSelectedFeature(Domain::Index&, FT_index&);
  Feature &FindSelectedSaddle();
  Basin_saddle &FindSelectedBsnsadl();
  Runoff &FindSelectedRunoff();
  void translateState(FT_index, Domain::Index, Feature&, Feature *&, FT_index&);
  void invertState(FT_index, Domain::Index, Feature&);
  void filterModifyStates(const FeatureTypeFilter&,
    void (CGraphView::*)(FT_index, Domain::Index,
			 const Feature&, const FeatureFilter&));
  void filterTranslateStates(const FeatureTypeFilter&, Label_state, Label_state);
  void filterTranslateState(FT_index, Domain::Index, const Feature&, const FeatureFilter&);
  void filterPreciseState(FT_index, Domain::Index, const Feature&, const FeatureFilter&);
  void filterInvertState(FT_index, Domain::Index, const Feature&, const FeatureFilter&);
  void filterModifyState(FT_index, Domain::Index, const Feature&, const FeatureFilter&);
  void SelectOne(Feature *, Domain::Index, FT_index);
  Domain::Index visibleSelect();
  void zero_select()
    {nsel_feature[FT_PEAK]=nsel_feature[FT_SADDLE]=
     nsel_feature[FT_RUNOFF]=nsel_feature[FT_BASINSADL]=0;}
  void viewFeature(const Feature&, FT_index);
  bool tree_writeable()
    {return GetDocument()->tree_writeable && !GetDocument()->working;}
  void select_subtree(Domain&, const Domain *, bool *,
		      const Domain *, Domain::Topology);
  Domain *get_child();
  //void set_tools();
  //void set_tool(const Feature&, FT_index, Domain::Index);

protected:
  // create from serialization only
  CGraphView();
  DECLARE_DYNCREATE(CGraphView)

  static const double zoom_factor,big_zoom_factor;
  CSize getWinSize() const;
  void clearBG(CDC *);
  virtual bool viewable() {return GetDocument()->dt_state!=NOT_LOADED;}
  void DrawRidges(CDC *);
  void DrawPeaks(const Divide_tree&, FT_index);
  void DrawSaddles(const Divide_tree&, FT_index);
  void DrawRunoffs(const Divide_tree&, FT_index);
  void DrawBasnsadls(const Divide_tree&, FT_index);
  void DrawFeatures(CDC *, Divide_tree&);
  virtual void setSizeIndicators() {assert(0);}
  void setGraphSizes() {setGraphSizes(CSize(1000,1000));}
  void setGraphSizes(const CSize&);
  void setGraphScroll(CPoint);
  void setLimitedScroll(CPoint);
  void scroll_by(int, int);
  CPoint getGraphScroll();
  virtual CMenu *featureMenu(CMenu& menu, Feature&) {return menu.GetSubMenu(1);}
  virtual CMenu *backgdMenu(CMenu&, CPoint&) {return 0;}
  virtual CMenu *idMenu(CMenu& menu) {return menu.GetSubMenu(5);}
  Feature &FindSelectedPeak();
  void translateStates(Label_state, Label_state, Elevation);
  void IdentifyFeature(Feature&, FT_index);
  void scale_print(CDC *, CPrintInfo *) const;
  virtual COLORREF get_ridge_color() const {return current.color_ridge;}
  COLORREF get_bg_color() const {return current.color_bg;}
  Domain::Index get_nsel(FT_index ft) const {return nsel_feature[ft];}
  Domain *vis_peak(Domain *, Elevation) const;
  virtual bool local_topo() const {return true;} // whether or not MS topo is permitted
  virtual Elevation min_view_prom() const {return -elev_infty;}

  Feature *r_click_featr;
  FT_index r_click_type;
  struct Settings {
    bool view_peak,view_saddle,view_runoff,view_bsnsadl;
    bool peak_ridge,saddle_ridge,runoff_ridge,bsnsadl_ridge;
    Feature_fmt peak_fmt,saddle_fmt,runoff_fmt,bsnsadl_fmt;
    COLORREF color_label,color_bg,color_ridge;
    Settings();
    void load_registry();
    void save_registry() const;
  } current;
  Elevation ridge_major,ridge_std,ridge_minor,
    ftr_large,ftr_medium,ftr_small,ftr_label;
  Domain::Topology topology;
  bool offmap_prom,ftr_rr,ftr_cr,ridge_rr;
  CPoint old_center;
  void saveCenter();
  void restoreCenter();
  mutable struct PrintParams {
    int nxpag,nypag;
    CSize job_size,page_size,extra_margin;
    void update(CPrintDialog&);
    BOOL final_update(CPrintInfo *, unsigned=0);
  } print_params;

private:
  static Settings initial;

// Attributes
public:
  CPromDoc *GetDocument() const;
  // CIdentifyFeature_dlg needs access to these
  static bool center_target,select_target;
  static char buf[256];

// Operations
public:
  virtual CPoint GetFeaturePoint(const Feature&, FT_index)
    {assert(0); return CPoint(0,0);}
  virtual void zoom(double);
  virtual void resize(double) {assert(0);}
  virtual void SetViewTitle() {assert(0);}
  virtual void viewFeature(const CPromDoc *, const Feature&, FT_index);
  virtual void viewLocation(const GridPoint&);
  void copyClip(Clip&);
  void pasteClip(const Clip&);
  Label_state get_label_state(FT_index ft, Domain::Index i)
    {return label_state[ft][i];}
  void set_initial_settings() {initial=current;}
  void get_initial_settings() {current=initial;}
  static void factory_initial_settings() {initial=Settings();}
  static void load_initial_settings() {initial.load_registry();}
  static void save_initial_settings() {initial.save_registry();}

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CGraphView)
  public:
  virtual void OnDraw(CDC* pDC);  // overridden to draw this view
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
  protected:
  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
  virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
  //}}AFX_VIRTUAL

  virtual bool divideTreeVisible() {assert(0); return false;}

// Implementation
public:
  virtual ~CGraphView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
  //{{AFX_MSG(CGraphView)
  afx_msg void OnViewZoomWayIn();
  afx_msg void OnViewZoomIn();
  afx_msg void OnViewZoomOut();
  afx_msg void OnViewZoomWayOut();
  afx_msg void OnViewRidgeBasinSadl();
  afx_msg void OnUpdateViewRidgeBasinSadl(CCmdUI* pCmdUI);
  afx_msg void OnViewRidgeRunoff();
  afx_msg void OnUpdateViewRidgeRunoff(CCmdUI* pCmdUI);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnViewLabelBasinSadl();
  afx_msg void OnViewLabelPeak();
  afx_msg void OnViewLabelRunoff();
  afx_msg void OnViewLabelSaddle();
  afx_msg void OnViewColorsBg();
  afx_msg void OnViewFeaturesPeaks();
  afx_msg void OnViewFeaturesBasinsadl();
  afx_msg void OnViewFeaturesRunoffs();
  afx_msg void OnViewFeaturesSaddles();
  afx_msg void OnUpdateViewFeaturesPeaks(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewFeaturesBasinsadl(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewFeaturesRunoffs(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewFeaturesSaddles(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewZoomWayin(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewZoomIn(CCmdUI* pCmdUI);
  afx_msg void OnViewFeaturesAll();
  afx_msg void OnViewFeaturesNone();
  afx_msg void OnViewLabelAll();
  afx_msg void OnViewRidgeAll();
  afx_msg void OnViewRidgeNone();
  afx_msg void OnUpdateViewRidgeAll(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewRidgeNone(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewLabelPeak(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewLabelBasinsadl(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewLabelRunoff(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewLabelSaddle(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewFeaturesNone(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewFeaturesAll(CCmdUI* pCmdUI);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnEditUnview();
  afx_msg void OnUpdateEditAnySelect(CCmdUI* pCmdUI);
  afx_msg void OnEditCopy();
  afx_msg void OnEditFeature();
  afx_msg void OnEditCenter();
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnEditSelectAll();
  afx_msg void OnEditDelete();
  afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
  afx_msg void OnEditRotate();
  afx_msg void OnUpdateEditRotate(CCmdUI* pCmdUI);
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnFeatureCenter();
  afx_msg void OnFeatureDelete();
  afx_msg void OnFeatureEdit();
  afx_msg void OnFeatureUnview();
  afx_msg void OnBasinsadlRotate();
  afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
  afx_msg void OnEditPaste();
  afx_msg void OnEditSelectInvert();
  afx_msg void OnUpdateAnyLabelVisible(CCmdUI* pCmdUI);
  afx_msg void OnEditSelectFilterSet();
  afx_msg void OnEditSelectFilterPrecise();
  afx_msg void OnEditSelectFilterClear();
  afx_msg void OnRunoffSplice();
  afx_msg void OnFeatureListSaddles();
  afx_msg void OnFeatureListNbrs();
  afx_msg void OnListPromSadl();
  afx_msg void OnUpdateListPromSadl(CCmdUI* pCmdUI);
  afx_msg void OnListCycle();
  afx_msg void OnUpdateListCycle(CCmdUI* pCmdUI);
  afx_msg void OnUpdateEditSplice(CCmdUI* pCmdUI);
  afx_msg void OnEditSplice();
  afx_msg void OnUpdateListNbrs(CCmdUI* pCmdUI);
  afx_msg void OnListNbrs();
  afx_msg void OnIdentifySaddle();
  afx_msg void OnViewColorsLabels();
  afx_msg void OnViewIdentify();
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnListTree();
  afx_msg void OnUpdateListTree(CCmdUI* pCmdUI);
  afx_msg void OnFeatureListTree();
  afx_msg void OnViewRidgeSaddle();
  afx_msg void OnUpdateViewRidgeSaddle(CCmdUI* pCmdUI);
  afx_msg void OnViewRidgePeak();
  afx_msg void OnUpdateViewRidgePeak(CCmdUI* pCmdUI);
  afx_msg void OnEditSearch();
  afx_msg void OnUpdateEditSearch(CCmdUI* pCmdUI);
  afx_msg void OnUpdateListWalk(CCmdUI* pCmdUI);
  afx_msg void OnListWalk();
  afx_msg void OnUpdateViewable(CCmdUI* pCmdUI);
  afx_msg void OnUpdateEditSingleSelect(CCmdUI* pCmdUI);
  afx_msg void OnUpdateEditFeature(CCmdUI* pCmdUI);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnEditSWI();
  afx_msg void OnUpdateEditSWI(CCmdUI* pCmdUI);
  afx_msg void OnGraphSelect();
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnEditBrowse();
  afx_msg void OnFeatureBrowse();
  afx_msg void OnUpdateViewGraphKind(CCmdUI* pCmdUI);
  afx_msg void OnRQFinfo();
  afx_msg void OnEditRQF();
  afx_msg void OnUpdateEditRQF(CCmdUI* pCmdUI);
  afx_msg void OnViewRidgeOptions();
  afx_msg void OnViewFeaturesOptions();
  afx_msg void OnListParents();
  afx_msg void OnUpdateListParents(CCmdUI* pCmdUI);
  afx_msg void OnFeatureListParents();
  afx_msg void OnIdentifyParentLine();
  afx_msg void OnIdentifyParentCell();
  afx_msg void OnIdentifyParentIsland();
  afx_msg void OnIdentifyNbr1();
  afx_msg void OnIdentifyNbr2();
  afx_msg void OnIdentifyNbrBoth();
  //}}AFX_MSG
  afx_msg void OnNcMouseMove(UINT nFlags, CPoint point);
  //afx_msg BOOL DisplayTip(UINT, NMHDR *, LRESULT *);
  DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GraphView.cpp
inline CPromDoc* CGraphView::GetDocument() const
  {return (CPromDoc *)m_pDocument;}
#endif

/////////////////////////////////////////////////////////////////////////////


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_MAPVIEW_H__9927AF50_C393_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
