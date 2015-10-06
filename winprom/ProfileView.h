// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_PROFILEVIEW_H__2BDD1300_183C_11D4_8FA1_0040056FBD08__INCLUDED_)
#define AFX_PROFILEVIEW_H__2BDD1300_183C_11D4_8FA1_0040056FBD08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProfileView.h : header file
//

#include "GraphView.h"
class CPromRulerBar;


/////////////////////////////////////////////////////////////////////////////
// CProfileView view

class CProfileView : public CGraphView
{
public:
  enum Scale_type {ST_LINEAR,ST_LOG,ST_INVERSE,ST_INVSQ};
private:
  friend class CProfileParam_dlg;
  friend class CPromRulerBar;
  struct Settings {
    enum Scale_type scale_type;
    double elev_scale,prom_scale;
    Elevation min_prom;
    short int sadl_pos;
    CPromDoc::Use_elev ue_runoff;
    Settings();
    void load_registry();
    void save_registry() const;
  } current;
  double PromFunc_min;
  Elev_intvl prom_range,elev_range;
  CPromRulerBar *prom_ruler;

  Elevation GetProm(const Domain&) const;
  int saddle_EA_coord(Elevation, Domain *, Domain *) const;
  int ElevCoord(Elevation elev) const
    {return (int)((elev_range.high-elev)*current.elev_scale)+GRAPH_VIEW_MARGIN;}
  double PromScaleFunc(Elevation) const;
  Elevation PromScaleInvFunc(double) const;
  void setProfileSize();
  void setProfileRange();
  static Settings initial;

protected:
  CProfileView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CProfileView)
  virtual bool local_topo() const {return false;}
  virtual Elevation min_view_prom() const {return prom_range.low;}
  virtual void setSizeIndicators();

// Attributes
public:

// Operations
public:
  virtual CPoint GetFeaturePoint(const Feature&, FT_index);
  int PromCoord(Elevation) const;
  bool posHasProm(int) const;
  Elevation PromFromCoord(int) const;
  Elevation ElevFromCoord(int) const;
  virtual void resize(double);
  void hzoom(double);
  void vzoom(double);
  virtual void SetViewTitle();
  static void factory_initial_settings() {initial=Settings();}
  static void load_initial_settings() {initial.load_registry();}
  static void save_initial_settings() {initial.save_registry();}

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CProfileView)
  protected:
  virtual void OnDraw(CDC* pDC);      // overridden to draw this view
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  //}}AFX_VIRTUAL

  virtual bool divideTreeVisible() {return GetDocument()->dt_state!=NOT_LOADED;}

// Implementation
protected:
  virtual ~CProfileView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
  //{{AFX_MSG(CProfileView)
  afx_msg void OnViewProfileZoomNarrower();
  afx_msg void OnViewProfileZoomShorter();
  afx_msg void OnViewProfileZoomTaller();
  afx_msg void OnViewProfileZoomWider();
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnViewProfileParams();
  afx_msg void OnUpdateViewProfileZoomTaller(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewProfileZoomWider(CCmdUI* pCmdUI);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnViewProfileSet();
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnUpdateViewProfileSet(CCmdUI* pCmdUI);
  afx_msg void OnViewSet();
  afx_msg void OnViewApply();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROFILEVIEW_H__2BDD1300_183C_11D4_8FA1_0040056FBD08__INCLUDED_)
