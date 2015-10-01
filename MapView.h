#if !defined(AFX_MAPVIEW_H__AECC1161_EC29_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_MAPVIEW_H__AECC1161_EC29_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MapView.h : header file
//

#include "GraphView.h"
#include "ElevColor_dlg.h"


struct Cell_xlate {
  Domain::Index major,standard,minor;
};

/////////////////////////////////////////////////////////////////////////////
// CMapView view

class CMapView : public CGraphView
{
  friend class CImpOut_dlg;
  friend class CRQFinfo_dlg;
  friend class CPrintMap_dlg;

  struct Settings {
    COLORREF color_contour,color_elev_void,color_domain,color_dom_void,
      color_grid,color_mark;
    double scale; // in pixels per grid point unit (e.g. km)
    Elevation contour_major,contour_std,contour_minor,contour_datum;
    bool autozoom,GPontop,elev_color_enable;
    double light_int,light_dir;
    vector<Elev_color_intv> elev_colors;
    Settings();
    void load_registry();
    void save_registry() const;
  } current;
  Elevation cell_major,cell_std,cell_minor;
  bool cell_cr,cell_edge_sadl;
  double hstep_pix,vstep_pix;
  enum {GV_NONE,GV_POINT,GV_ELEV} grid_view;
  bool view_em,view_dm,view_dt,view_mark;
  Bool_map grid_mark;
  double light_vec_x,light_vec_y;
  GridPoint r_click_gp,old_center_loc;
  CPoint global_offset;
  Rectangl scrollRect,save_scrollRect;
  Rectangl print_limits;
  bool cell_utd;
  Domain::Index ncell;
  Cell_xlate *cells;

  void setMapSize();
  void setMapScale();
  void setMapScale(const GridPoint&);
  void MarkElevRange(bool,bool=false);
  void setMarkMenuItemState(CMenu *, UINT);
  void scrollCenterCoord(int, GridPoint::Coord, double);
  void scrollCenter(GridPoint);
  void set_light_vec();
  void InvalidatePoint(const GridPoint&);
  void update_cell();
  static Settings initial;

protected:
  CMapView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CMapView)
  virtual bool viewable();
  virtual void setSizeIndicators();
  virtual CMenu *featureMenu(CMenu&, Feature&);
  virtual CMenu *backgdMenu(CMenu&, CPoint&);
  virtual CMenu *idMenu(CMenu& menu) {return menu.GetSubMenu(6);}
  virtual COLORREF get_ridge_color() const
    {return GetDocument()->is_drainage?
     current.color_domain:CGraphView::get_ridge_color();}

// Attributes
public:

// Operations
public:
  virtual CPoint GetFeaturePoint(const Feature&, FT_index);
  CPoint gp2point(const GridPoint&) const;
  int semi_gp2coord_x(int x) const
    {return (int)((scrollRect.limit[0].high-x+0.5)*vstep_pix)+global_offset.y;}
  int semi_gp2coord_y(int y) const
    {return (int)((y-scrollRect.limit[1].low+0.5)*hstep_pix)+global_offset.x;}
  GridPoint point2gp(const CPoint&) const;
  virtual void zoom(double);
  virtual void SetViewTitle();
  virtual void viewFeature(const CPromDoc *, const Feature&, FT_index);
  virtual void viewLocation(const GridPoint&);
  static void factory_initial_settings() {initial=Settings();}
  static void load_initial_settings() {initial.load_registry();}
  static void save_initial_settings() {initial.save_registry();}

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMapView)
  public:
  virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
  protected:
  virtual void OnDraw(CDC* pDC);
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
  virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
  //}}AFX_VIRTUAL

  virtual bool divideTreeVisible();

// Implementation
protected:
  virtual ~CMapView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
  //{{AFX_MSG(CMapView)
  afx_msg void OnViewMapDLPRtree();
  afx_msg void OnUpdateViewMapDLPRtree(CCmdUI* pCmdUI);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnViewMapContours();
  afx_msg void OnViewMapSet();
  afx_msg void OnGpCenter();
  afx_msg void OnViewMapMarkClear();
  afx_msg void OnViewMapMarkClearAll();
  afx_msg void OnViewMapMarkPrecise();
  afx_msg void OnViewMapMarkSet();
  afx_msg void OnUpdateViewMapMark(CCmdUI* pCmdUI);
  afx_msg void OnGpMarkArea();
  afx_msg void OnViewMapMarkView();
  afx_msg void OnUpdateViewMapMarkView(CCmdUI* pCmdUI);
  afx_msg void OnIdentifyDLPRpeak();
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnViewColorsMark();
  afx_msg void OnViewScrollCenter();
  afx_msg void OnUpdateViewScrollCenter(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewable(CCmdUI* pCmdUI);
  afx_msg void OnImpress();
  afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
  afx_msg void OnViewMapRelief();
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnOpDLPRcheck();
  afx_msg void OnBrowse();
  afx_msg void OnGP_RQF();
  afx_msg void OnViewLabelStatus();
  afx_msg void OnGpMarkPoint();
  afx_msg void OnViewSet();
  afx_msg void OnOpenMarkMap();
  afx_msg void OnUpdateHaveMarkMap(CCmdUI* pCmdUI);
  afx_msg void OnSaveMarkMap();
  afx_msg void OnViewMapDomains();
  afx_msg void OnUpdateViewMapDomains(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewMapContours(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewMapRelief(CCmdUI* pCmdUI);
  afx_msg void OnViewMapGrid();
  afx_msg void OnViewApply();
  afx_msg void OnViewMapCutoffs();
  afx_msg void OnGpNewPeak();
  afx_msg void OnGpNewSaddle();
  afx_msg void OnViewZoomScale();
  afx_msg void OnFreeHeight();
  //}}AFX_MSG
  afx_msg void OnGpNewRunoff();
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPVIEW_H__AECC1161_EC29_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
