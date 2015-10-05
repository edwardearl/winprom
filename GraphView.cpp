// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// GraphView.cpp : implementation of the CGraphView class
//

#include "stdafx.h"
#include "winprom.h"
#include "PromFrame.h"

#include "PromDoc.h"
#include "GraphView.h"
#include "PromSadlListView.h"
#include "CycleListView.h"
#include "PeakNbrListView.h"
#include "ParentListView.h"
#include "WalkListView.h"
#include "RunoffNbrListView.h"
#include "LPRtreeView.h"
#include "FeatureFmt_dlg.h"
#include "IdentifyFeature_dlg.h"
#include "TreeSelectFilter_dlg.h"
#include "RQFinfo_dlg.h"
#include "RidgeView_dlg.h"
#include "FeatureView_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char CGraphView::buf[256];


/////////////////////////////////////////////////////////////////////////////
// CGraphView

IMPLEMENT_DYNCREATE(CGraphView, CScrollView)

BEGIN_MESSAGE_MAP(CGraphView, CScrollView)
  //{{AFX_MSG_MAP(CGraphView)
  ON_COMMAND(ID_VIEW_ZOOM_WAYIN, OnViewZoomWayIn)
  ON_COMMAND(ID_VIEW_ZOOM_IN, OnViewZoomIn)
  ON_COMMAND(ID_VIEW_ZOOM_OUT, OnViewZoomOut)
  ON_COMMAND(ID_VIEW_ZOOM_WAYOUT, OnViewZoomWayOut)
  ON_COMMAND(ID_VIEW_RIDGE_BASINSADL, OnViewRidgeBasinSadl)
  ON_UPDATE_COMMAND_UI(ID_VIEW_RIDGE_BASINSADL, OnUpdateViewRidgeBasinSadl)
  ON_COMMAND(ID_VIEW_RIDGE_RUNOFF, OnViewRidgeRunoff)
  ON_UPDATE_COMMAND_UI(ID_VIEW_RIDGE_RUNOFF, OnUpdateViewRidgeRunoff)
  ON_WM_SIZE()
  ON_COMMAND(ID_VIEW_LABEL_BASINSADL, OnViewLabelBasinSadl)
  ON_COMMAND(ID_VIEW_LABEL_PEAK, OnViewLabelPeak)
  ON_COMMAND(ID_VIEW_LABEL_RUNOFF, OnViewLabelRunoff)
  ON_COMMAND(ID_VIEW_LABEL_SADDLE, OnViewLabelSaddle)
  ON_COMMAND(ID_VIEW_COLORS_BG, OnViewColorsBg)
  ON_COMMAND(ID_VIEW_FEATURES_PEAKS, OnViewFeaturesPeaks)
  ON_COMMAND(ID_VIEW_FEATURES_BASINSADL, OnViewFeaturesBasinsadl)
  ON_COMMAND(ID_VIEW_FEATURES_RUNOFFS, OnViewFeaturesRunoffs)
  ON_COMMAND(ID_VIEW_FEATURES_SADDLES, OnViewFeaturesSaddles)
  ON_UPDATE_COMMAND_UI(ID_VIEW_FEATURES_PEAKS, OnUpdateViewFeaturesPeaks)
  ON_UPDATE_COMMAND_UI(ID_VIEW_FEATURES_BASINSADL, OnUpdateViewFeaturesBasinsadl)
  ON_UPDATE_COMMAND_UI(ID_VIEW_FEATURES_RUNOFFS, OnUpdateViewFeaturesRunoffs)
  ON_UPDATE_COMMAND_UI(ID_VIEW_FEATURES_SADDLES, OnUpdateViewFeaturesSaddles)
  ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_WAYIN, OnUpdateViewZoomWayin)
  ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_IN, OnUpdateViewZoomIn)
  ON_COMMAND(ID_VIEW_FEATURES_ALL, OnViewFeaturesAll)
  ON_COMMAND(ID_VIEW_FEATURES_NONE, OnViewFeaturesNone)
  ON_COMMAND(ID_VIEW_LABEL_ALL, OnViewLabelAll)
  ON_COMMAND(ID_VIEW_RIDGE_ALL, OnViewRidgeAll)
  ON_COMMAND(ID_VIEW_RIDGE_NONE, OnViewRidgeNone)
  ON_UPDATE_COMMAND_UI(ID_VIEW_RIDGE_ALL, OnUpdateViewRidgeAll)
  ON_UPDATE_COMMAND_UI(ID_VIEW_RIDGE_NONE, OnUpdateViewRidgeNone)
  ON_UPDATE_COMMAND_UI(ID_VIEW_LABEL_PEAK, OnUpdateViewLabelPeak)
  ON_UPDATE_COMMAND_UI(ID_VIEW_LABEL_BASINSADL, OnUpdateViewLabelBasinsadl)
  ON_UPDATE_COMMAND_UI(ID_VIEW_LABEL_RUNOFF, OnUpdateViewLabelRunoff)
  ON_UPDATE_COMMAND_UI(ID_VIEW_LABEL_SADDLE, OnUpdateViewLabelSaddle)
  ON_UPDATE_COMMAND_UI(ID_VIEW_FEATURES_NONE, OnUpdateViewFeaturesNone)
  ON_UPDATE_COMMAND_UI(ID_VIEW_FEATURES_ALL, OnUpdateViewFeaturesAll)
  ON_WM_LBUTTONDOWN()
  ON_COMMAND(ID_EDIT_UNVIEW, OnEditUnview)
  ON_UPDATE_COMMAND_UI(ID_EDIT_UNVIEW, OnUpdateEditAnySelect)
  ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
  ON_COMMAND(ID_EDIT_FEATURE, OnEditFeature)
  ON_COMMAND(ID_EDIT_CENTER, OnEditCenter)
  ON_WM_LBUTTONDBLCLK()
  ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
  ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
  ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
  ON_COMMAND(ID_EDIT_ROTATE, OnEditRotate)
  ON_UPDATE_COMMAND_UI(ID_EDIT_ROTATE, OnUpdateEditRotate)
  ON_WM_RBUTTONDOWN()
  ON_COMMAND(ID_FEATURE_CENTER, OnFeatureCenter)
  ON_COMMAND(ID_FEATURE_DELETE, OnFeatureDelete)
  ON_COMMAND(ID_FEATURE_EDIT, OnFeatureEdit)
  ON_COMMAND(ID_FEATURE_UNVIEW, OnFeatureUnview)
  ON_COMMAND(ID_BASINSADL_ROTATE, OnBasinsadlRotate)
  ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
  ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
  ON_COMMAND(ID_EDIT_SELECT_INVERT, OnEditSelectInvert)
  ON_UPDATE_COMMAND_UI(ID_VIEW_LABEL_ALL, OnUpdateAnyLabelVisible)
  ON_COMMAND(ID_EDIT_SELECT_FILTER_SET, OnEditSelectFilterSet)
  ON_COMMAND(ID_EDIT_SELECT_FILTER_PRECISE, OnEditSelectFilterPrecise)
  ON_COMMAND(ID_EDIT_SELECT_FILTER_CLEAR, OnEditSelectFilterClear)
  ON_COMMAND(ID_RUNOFF_SPLICE, OnRunoffSplice)
  ON_COMMAND(ID_FEATURE_LIST_SADDLES, OnFeatureListSaddles)
  ON_COMMAND(ID_FEATURE_LIST_NBRS, OnFeatureListNbrs)
  ON_COMMAND(ID_LIST_PROMSADL, OnListPromSadl)
  ON_UPDATE_COMMAND_UI(ID_LIST_PROMSADL, OnUpdateListPromSadl)
  ON_COMMAND(ID_LIST_CYCLE, OnListCycle)
  ON_UPDATE_COMMAND_UI(ID_LIST_CYCLE, OnUpdateListCycle)
  ON_UPDATE_COMMAND_UI(ID_EDIT_SPLICE, OnUpdateEditSplice)
  ON_COMMAND(ID_EDIT_SPLICE, OnEditSplice)
  ON_UPDATE_COMMAND_UI(ID_LIST_NBRS, OnUpdateListNbrs)
  ON_COMMAND(ID_LIST_NBRS, OnListNbrs)
  ON_COMMAND(ID_IDENTIFY_SADDLE, OnIdentifySaddle)
  ON_COMMAND(ID_VIEW_COLORS_LABELS, OnViewColorsLabels)
  ON_COMMAND(ID_VIEW_IDENTIFY, OnViewIdentify)
  ON_WM_ERASEBKGND()
  ON_COMMAND(ID_LIST_TREE, OnListTree)
  ON_UPDATE_COMMAND_UI(ID_LIST_TREE, OnUpdateListTree)
  ON_COMMAND(ID_FEATURE_LIST_TREE, OnFeatureListTree)
  ON_COMMAND(ID_VIEW_RIDGE_SADDLE, OnViewRidgeSaddle)
  ON_UPDATE_COMMAND_UI(ID_VIEW_RIDGE_SADDLE, OnUpdateViewRidgeSaddle)
  ON_COMMAND(ID_VIEW_RIDGE_PEAK, OnViewRidgePeak)
  ON_UPDATE_COMMAND_UI(ID_VIEW_RIDGE_PEAK, OnUpdateViewRidgePeak)
  ON_COMMAND(ID_EDIT_SEARCH, OnEditSearch)
  ON_UPDATE_COMMAND_UI(ID_EDIT_SEARCH, OnUpdateEditSearch)
  ON_UPDATE_COMMAND_UI(ID_LIST_WALK, OnUpdateListWalk)
  ON_COMMAND(ID_LIST_WALK, OnListWalk)
  ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_OUT, OnUpdateViewable)
  ON_UPDATE_COMMAND_UI(ID_EDIT_CENTER, OnUpdateEditSingleSelect)
  ON_UPDATE_COMMAND_UI(ID_EDIT_FEATURE, OnUpdateEditFeature)
  ON_WM_LBUTTONUP()
  ON_WM_MOUSEMOVE()
  ON_COMMAND(ID_EDIT_SWI, OnEditSWI)
  ON_UPDATE_COMMAND_UI(ID_EDIT_SWI, OnUpdateEditSWI)
  ON_COMMAND(ID_GRAPH_SELECT, OnGraphSelect)
  ON_WM_KEYDOWN()
  ON_WM_CREATE()
  ON_COMMAND(ID_EDIT_BROWSE, OnEditBrowse)
  ON_COMMAND(ID_FEATURE_BROWSE, OnFeatureBrowse)
  ON_UPDATE_COMMAND_UI(ID_VIEW_MAP_SET, OnUpdateViewGraphKind)
  ON_COMMAND(ID_FEATURE_RQF, OnRQFinfo)
  ON_COMMAND(ID_EDIT_RQF, OnEditRQF)
  ON_UPDATE_COMMAND_UI(ID_EDIT_RQF, OnUpdateEditRQF)
  ON_COMMAND(ID_VIEW_RIDGE_OPTIONS, OnViewRidgeOptions)
  ON_COMMAND(ID_VIEW_FEATURES_OPTIONS, OnViewFeaturesOptions)
  ON_COMMAND(ID_LIST_PARENTS, OnListParents)
  ON_UPDATE_COMMAND_UI(ID_LIST_PARENTS, OnUpdateListParents)
  ON_COMMAND(ID_FEATURE_LIST_PARENTS, OnFeatureListParents)
  ON_COMMAND(ID_IDENTIFY_PARENT_LINE, OnIdentifyParentLine)
  ON_COMMAND(ID_IDENTIFY_PARENT_CELL, OnIdentifyParentCell)
  ON_COMMAND(ID_IDENTIFY_PARENT_ISLAND, OnIdentifyParentIsland)
  ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditAnySelect)
  ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateAnyLabelVisible)
  ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_INVERT, OnUpdateAnyLabelVisible)
  ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_FILTER_SET, OnUpdateAnyLabelVisible)
  ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_FILTER_PRECISE, OnUpdateAnyLabelVisible)
  ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_FILTER_CLEAR, OnUpdateAnyLabelVisible)
  ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_WAYOUT, OnUpdateViewable)
  ON_UPDATE_COMMAND_UI(ID_EDIT_BROWSE, OnUpdateEditSingleSelect)
  ON_UPDATE_COMMAND_UI(ID_VIEW_PROFILE_SET, OnUpdateViewGraphKind)
  ON_COMMAND(ID_IDENTIFY_NBR_1, OnIdentifyNbr1)
  ON_COMMAND(ID_IDENTIFY_NBR_2, OnIdentifyNbr2)
  ON_COMMAND(ID_IDENTIFY_NBR_BOTH, OnIdentifyNbrBoth)
  //}}AFX_MSG_MAP
  ON_WM_NCMOUSEMOVE()
  // Standard printing commands
  ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
  //ON_NOTIFY_EX(TTN_NEEDTEXT,0,DisplayTip)
END_MESSAGE_MAP()


const double CGraphView::zoom_factor=1.2589,CGraphView::big_zoom_factor=3.1623;

CGraphView::Settings CGraphView::initial;
bool CGraphView::center_target=true,CGraphView::select_target=true;

CGraphView::Settings::Settings() :
  view_peak(true),view_saddle(true),view_runoff(true),view_bsnsadl(true),
  peak_ridge(false),saddle_ridge(true),runoff_ridge(true),bsnsadl_ridge(true),
  color_bg(RGB(0,0,0)),color_ridge(RGB(255,255,0)),color_label(RGB(255,255,255))
{
  peak_fmt.name_f=Feature_fmt::ALT_ELEV;
  peak_fmt.show_loc=peak_fmt.show_elev=false;
  peak_fmt.elev_f=Feature_fmt::RANGE;
  saddle_fmt=runoff_fmt=bsnsadl_fmt=peak_fmt;
}

void CGraphView::Settings::load_registry()
{
  Settings factory;
  CWinApp *app=AfxGetApp();
  view_peak=app->GetProfileInt("Graph","Peak",factory.view_peak)!=0;
  view_saddle=app->GetProfileInt("Graph","Saddle",factory.view_saddle)!=0;
  view_runoff=app->GetProfileInt("Graph","Runoff",factory.view_runoff)!=0;
  view_bsnsadl=app->GetProfileInt("Graph","Basin saddle",factory.view_bsnsadl)!=0;
  peak_ridge=app->GetProfileInt("Graph","Peak ridge",factory.peak_ridge)!=0;
  saddle_ridge=app->GetProfileInt("Graph","Saddle ridge",factory.saddle_ridge)!=0;
  runoff_ridge=app->GetProfileInt("Graph","Runoff ridge",factory.runoff_ridge)!=0;
  bsnsadl_ridge=app->GetProfileInt("Graph","Basin saddle ridge",factory.bsnsadl_ridge)!=0;
  peak_fmt.load_registry("Peak format",factory.peak_fmt);
  saddle_fmt.load_registry("Saddle format",factory.saddle_fmt);
  runoff_fmt.load_registry("Runoff format",factory.runoff_fmt);
  bsnsadl_fmt.load_registry("Basin saddle format",factory.bsnsadl_fmt);
  color_bg=app->GetProfileInt("Color","Background",factory.color_bg);
  color_ridge=app->GetProfileInt("Color","Ridge",factory.color_ridge);
  color_label=app->GetProfileInt("Color","Label",factory.color_label);
}

void CGraphView::Settings::save_registry() const
{
  CWinApp *app=AfxGetApp();
  app->WriteProfileInt("Graph","Peak",view_peak);
  app->WriteProfileInt("Graph","Saddle",view_saddle);
  app->WriteProfileInt("Graph","Runoff",view_runoff);
  app->WriteProfileInt("Graph","Basin saddle",view_bsnsadl);
  app->WriteProfileInt("Graph","Peak ridge",peak_ridge);
  app->WriteProfileInt("Graph","Saddle ridge",saddle_ridge);
  app->WriteProfileInt("Graph","Runoff ridge",runoff_ridge);
  app->WriteProfileInt("Graph","Basin saddle ridge",bsnsadl_ridge);
  peak_fmt.save_registry("Peak format");
  saddle_fmt.save_registry("Saddle format");
  runoff_fmt.save_registry("Runoff format");
  bsnsadl_fmt.save_registry("Basin saddle format");
  app->WriteProfileInt("Color","Background",color_bg);
  app->WriteProfileInt("Color","Ridge",color_ridge);
  app->WriteProfileInt("Color","Label",color_label);
}


/////////////////////////////////////////////////////////////////////////////
// CGraphView construction/destruction

CGraphView::CGraphView()
  : tree_peak_xing(true),tree_sadl_xing(true),
    tree_peak_select(true),tree_sadl_select(true),
    bs_xing(false)
{
  current=initial;
  zero_select();
  label_state[FT_PEAK]=label_state[FT_SADDLE]=
    label_state[FT_RUNOFF]=label_state[FT_BASINSADL]=0;
  ridge_major=elev_infty;
  ridge_std=ridge_minor=0;
  ftr_large=ftr_medium=1000;
  ftr_small=0;
  ftr_label=0;
  peak_elev_cap=true;
  offmap_prom=ftr_rr=ftr_cr=ridge_rr=false;
  topology=Domain::TOPO_MS;
  //tool_info[FT_PEAK]=tool_info[FT_SADDLE]=
    //tool_info[FT_RUNOFF]=tool_info[FT_BASINSADL]=0;
  old_center.x=old_center.y=-1;
  tree_filter_rel=0;
  tree_filter_action=0;
}

CGraphView::~CGraphView()
{
  for (unsigned i=0; i<FT_NUM_TYPES; ++i) {
    delete[] label_state[i];
    //delete[] tool_info[i];
  }
}

BOOL CGraphView::PreCreateWindow(CREATESTRUCT& cs)
{
  return CScrollView::PreCreateWindow(cs);
}

int CGraphView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CScrollView::OnCreate(lpCreateStruct) == -1)
    return -1;

  //tool_tip.Create(this,TTS_ALWAYSTIP|TTS_NOPREFIX);
  //tool_tip.Activate(TRUE);
  //EnableToolTips();
  //set_tools();

  return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CGraphView drawing

CSize CGraphView::getWinSize() const
{
  RECT winRect;
  GetClientRect(&winRect);
  return CSize(winRect.right,winRect.bottom);
}

void CGraphView::setGraphSizes(const CSize& sizeTotal)
{
  CSize sizePage=getWinSize();
  sizePage.cx=(int)(sizePage.cx/zoom_factor);
  sizePage.cy=(int)(sizePage.cy/zoom_factor);
  CScrollView::SetScrollSizes(MM_TEXT,sizeTotal,sizePage,
    CSize(GRAPH_SCROLL_ARROW_BUTN,GRAPH_SCROLL_ARROW_BUTN));
  setSizeIndicators();
  Invalidate();
}

void CGraphView::setGraphScroll(CPoint scrollPos)
{
  // the input scroll position is the CENTER position
  CSize winSize=getWinSize();
  scrollPos.x-=winSize.cx/2;
  scrollPos.y-=winSize.cy/2;
  setLimitedScroll(scrollPos);
}

void CGraphView::setLimitedScroll(CPoint scrollPos)
{
  CSize viewSize=GetTotalSize(),winSize=getWinSize();

  if (scrollPos.x+winSize.cx>viewSize.cx)
    scrollPos.x=viewSize.cx-winSize.cx;
  if (scrollPos.y+winSize.cy>viewSize.cy)
    scrollPos.y=viewSize.cy-winSize.cy;
  ScrollToPosition(scrollPos);
}

CPoint CGraphView::getGraphScroll()
{
  // the returned position is the CENTER position
  CSize viewSize=GetTotalSize(),winSize=getWinSize();
  CPoint scrollPos=GetScrollPosition();
  scrollPos.x+=winSize.cx/2;
  scrollPos.y+=winSize.cy/2;
  if (scrollPos.x>viewSize.cx) scrollPos.x=viewSize.cx;
  if (scrollPos.y>viewSize.cy) scrollPos.y=viewSize.cy;
  return scrollPos;
}

void CGraphView::zoom(double zf)
{
  CPoint scrollPos=getGraphScroll();
  scrollPos.x-=GRAPH_VIEW_MARGIN;
  scrollPos.y-=GRAPH_VIEW_MARGIN;
  scrollPos.x=(int)(scrollPos.x*zf);
  scrollPos.y=(int)(scrollPos.y*zf);
  scrollPos.x+=GRAPH_VIEW_MARGIN;
  scrollPos.y+=GRAPH_VIEW_MARGIN;
  resize(zf);
  setGraphScroll(scrollPos);
  if (!tree_writeable()) AfxGetApp()->OnIdle(0);
  Invalidate();
}

BOOL CGraphView::OnEraseBkgnd(CDC *pDC)
{
  if (GetDocument()->dt_state==NOT_LOADED)
    return CScrollView::OnEraseBkgnd(pDC);
  clearBG(pDC);
  return true;
}

void CGraphView::clearBG(CDC *pDC)
{
  CRect visibleRect;
  pDC->GetClipBox(&visibleRect);
  pDC->FillSolidRect(visibleRect,current.color_bg);
}

static CRect winVisible; // used only to accelerate drawing
static CPoint center;
static double radius2;
static CDC *static_pDC;  // avoids overhead of passing CDC* argument
			 // into DrawRidge() and DrawFeature()
static CPromApp *static_app; // avoids CPromApp* argument passing overhead

Domain *CGraphView::vis_peak(Domain *d, Elevation min_prom) const
{
  switch (topology) {
  case Domain::TOPO_MS:
    return d;
  case Domain::TOPO_LINE:
    return d->parent_with_prom(min_view_prom());
  case Domain::TOPO_CELL: case Domain::TOPO_ISLAND:
    assert(min_prom>=min_view_prom());
    return d->parent_with_prom(min_prom);
  }
  assert(0);
  return 0;
}

void CGraphView::DrawRidge(const CPoint& point, Feature *featr, FT_index type)
{
  if (point.x==POINT_UNDEF || !featr->location) return;

  DrawRidge(point,GetFeaturePoint(*featr,type));
}

void CGraphView::DrawRidge(const CPoint& point, const CPoint& point2)
{
  if (point.x==POINT_UNDEF || point2.x==POINT_UNDEF) return;

  if (point.x<winVisible.left) {
    if (point2.x<winVisible.left) return;
  }
  else if (point.x>winVisible.right) {
    if (point2.x>winVisible.right) return;
  }

  if (point.y<winVisible.top) {
    if (point2.y<winVisible.top) return;
  }
  else if (point.y>winVisible.bottom) {
    if (point2.y>winVisible.bottom) return;
  }

  int x1=point.x-center.x,x2=point2.x-center.x,
    y1=point.y-center.y,y2=point2.y-center.y,
    dx=point.x-point2.x,dy=point.y-point2.y;
  if (abs(x1*y2-y1*x2)>sqrt((float)(dx*dx+dy*dy))*radius2) return;

  static_pDC->MoveTo(point);
  static_pDC->LineTo(point2);
}

void CGraphView::DrawRidges(CDC *pDC)
{
  COLORREF minor_color_ridge=
	((current.color_ridge&0xFEFEFE)+
	 (pDC->IsPrinting()?0xFEFEFE:(get_bg_color()&0xFEFEFE)) )>>1;
  CPen majorPen(PS_SOLID,2,current.color_ridge),
    stdPen(PS_SOLID,0,current.color_ridge),
    minorPen(PS_SOLID,0,minor_color_ridge),
    *offmapPen=ridge_major<elev_infty?&majorPen:
	       ridge_std<elev_infty?&stdPen:&minorPen;

  CPromDoc *doc=GetDocument();
  Divide_tree& tree=doc->tree;
  doc->update_ridge_info_rq();
  Domain::Index npeak=tree.n_dom(),ipeak;
  CPoint peakPoint,sadlPoint;
  Domain *far_dom;
  pDC->GetClipBox(&winVisible);
  center.x=(winVisible.left+winVisible.right)/2;
  center.y=(winVisible.top+winVisible.bottom)/2;
  int dx=winVisible.right-winVisible.left,
    dy=winVisible.bottom-winVisible.top;
  radius2=sqrt((float)(dx*dx+dy*dy))/2;
  static_pDC=pDC;
  Elevation ridge_prom,cp,mvp=min_view_prom();
  bool show_rr = ridge_rr && current.bsnsadl_ridge;
  for (ipeak=0; ipeak<npeak; ++ipeak) {
    Domain& dom=tree[ipeak];
    if (dom.peak.location) {
      assert((dom.parent.line==0) == (dom.parent.cell==0));
      peakPoint=GetFeaturePoint(dom.peak,FT_PEAK);
      if (peakPoint.x!=POINT_UNDEF) {
	if (current.runoff_ridge &&
	    (topology==Domain::TOPO_MS || topology==Domain::TOPO_LINE)) {
	  pDC->SelectObject(topology==Domain::TOPO_MS?offmapPen:&stdPen);
	  for (ROiter roi=dom.runoffs.begin();
	       roi!=dom.runoffs.end(); ++roi)
	    DrawRidge(peakPoint,*roi,FT_RUNOFF);
	}
	if ((topology==Domain::TOPO_MS ? dom.primary_nbr : dom.parent.line) &&
	    (current.peak_ridge || current.saddle_ridge)) {
	  switch (topology) {
	  case Domain::TOPO_MS:
	    if (current.runoff_ridge &&
		(show_rr ? dom.ridge.rot_offmap : dom.ridge.offmap))
	      ridge_prom=elev_infty-1;
	    else ridge_prom = show_rr ?
	      dom.ridge.rot_prom : dom.ridge.prom;
	    if (current.bsnsadl_ridge) {
	      cp=current.runoff_ridge?dom.ridge.cycle_offmap:dom.ridge.cycle_prom;
	      if (cp>ridge_prom) ridge_prom=cp;
	    }
	    break;
	  case Domain::TOPO_LINE:
	    ridge_prom=current.bsnsadl_ridge?
	      dom.ridge.cycle_line_child:dom.ridge.line_child;
	    break;
	  case Domain::TOPO_CELL: case Domain::TOPO_ISLAND:
	    ridge_prom=dom.prom.peak;
	    break;
	  default:
	    assert(0);
	  }
	  if (ridge_prom>=ridge_minor) {
	    if (ridge_prom<ridge_std) pDC->SelectObject(&minorPen);
	    else if (ridge_prom<ridge_major) pDC->SelectObject(&stdPen);
	    else pDC->SelectObject(&majorPen);
	    far_dom=dom.get_parent(topology,mvp);
	    if (current.saddle_ridge) {
	      sadlPoint = topology==Domain::TOPO_MS ?
		GetFeaturePoint(dom.saddle,FT_SADDLE) :
		GetFeaturePoint(dom.parent.saddle->saddle,FT_SADDLE);
	      DrawRidge(peakPoint,sadlPoint);
	      DrawRidge(sadlPoint,&far_dom->peak,FT_PEAK);
	    }
	    else // current.peak_ridge assumed
	      DrawRidge(peakPoint,&far_dom->peak,FT_PEAK);
	  }
	} // if (saddle_ridge)
      }
    }
  }

  if (current.bsnsadl_ridge) {
    Domain::Index nbs=tree.n_bsnsdl(),ibs;
    CPoint peak2point;
    for (ibs=0; ibs<nbs; ++ibs) {
      Basin_saddle& bs=tree.bsnsdl(ibs);
      sadlPoint=GetFeaturePoint(bs,FT_BASINSADL);
      if (sadlPoint.x<0) continue;
      peakPoint=GetFeaturePoint(vis_peak(bs.peak1,bs.prom.onmap)->peak,FT_PEAK);
      peak2point=GetFeaturePoint(vis_peak(bs.peak2,bs.prom.onmap)->peak,FT_PEAK);
      if (topology==Domain::TOPO_MS) {
	if (current.runoff_ridge &&
	    (show_rr ? bs.prom.rr_offmap :
		       bs.prom.offmap==elev_infty))
	  ridge_prom=elev_infty-1;
	else {
	  ridge_prom = show_rr ?
	    bs.prom.rr_onmap : bs.prom.onmap;
	}
	cp=current.runoff_ridge?bs.prom.offmap:bs.prom.onmap;
	if (cp>ridge_prom) ridge_prom=cp;
      }
      else ridge_prom=bs.prom.onmap;
      if (ridge_prom<0) ridge_prom=0;
      if (ridge_prom>=ridge_minor) {
	if (ridge_prom<ridge_std) pDC->SelectObject(&minorPen);
	else if (ridge_prom<ridge_major) pDC->SelectObject(&stdPen);
	else pDC->SelectObject(&majorPen);
	DrawRidge(peakPoint,sadlPoint);
	DrawRidge(peak2point,sadlPoint);
      }
    }
  } // if (bsnsadl_ridge)

  pDC->SelectStockObject(NULL_PEN);
}

static COLORREF color_select;

Elevation CGraphView::get_feature_prom(const Feature *featr, FT_index type) const
{
  Elevation prom;
  switch (type) {
  case FT_PEAK:
    prom=get_peak_prom(*Domain::FromPeak(featr));
    break;
  case FT_SADDLE:
    prom=get_saddle_prom(*Domain::FromSadl(featr));
    break;
  case FT_RUNOFF:
    prom=get_prom(*(Runoff *)featr);
    break;
  case FT_BASINSADL:
    prom=get_prom(*(Basin_saddle *)featr);
    break;
  default: assert(0);
  }
  return prom;
}

Elevation CGraphView::get_peak_prom(const Domain& d) const
{
  Elevation prom=d.prom.peak;
  if (peak_elev_cap && d.peak.elev.high<prom) prom=d.peak.elev.high;
  if (prom<0) prom=0;
  return prom;
}

Elevation CGraphView::get_saddle_prom(const Domain::Sadl_prom& sp) const
{
  Elevation prom=sp.*sadl_prom_types[offmap_prom][ftr_cr];
  if (ftr_rr) {
    if (offmap_prom && sp.rr_offmap) prom=elev_infty;
    else if (sp.rr_onmap>prom) prom=sp.rr_onmap;
  }
  if (prom==elev_infty) --prom;
  else if (prom<0) prom=0;
  return prom;
}

Elevation CGraphView::get_saddle_prom(const Domain& d) const
{
  return get_saddle_prom(d.prom.saddle);
}

Elevation CGraphView::get_prom(const Runoff& ro) const
{
  Elevation ro_prom=0,sadl_elev=ro.elev.high,pk_prom,pk_omp;
  for (const Domain *d=ro.island; d;
       sadl_elev=d->prom.sadl_elev,d=d->prom.island_parent) {
    pk_prom=d->prom.peak;
    pk_omp=d->peak.elev.high-sadl_elev;
    if (pk_prom>=ftr_large) {
      if (pk_omp<ftr_large && pk_prom>ro_prom) ro_prom=pk_prom;
    }
    else if (pk_prom>=ftr_medium) {
      if (pk_omp<ftr_medium && pk_prom>ro_prom) ro_prom=pk_prom;
    }
    else if (pk_prom>=ftr_small) {
      if (pk_omp<ftr_small && pk_prom>ro_prom) ro_prom=pk_prom;
    }
  }
  return ro_prom;
}

Elevation CGraphView::get_prom(const Basin_saddle& bs) const
{
  return get_saddle_prom(bs.prom);
}

void CGraphView::DrawFeature(const Feature& featr, FT_index type,
			     Label_state ls, Elevation prom)
{
  static CPoint ftrPoint;
  static CRect lblRect;
  assert(prom>=0);
  if (featr.location && ls!=LS_HIDDEN) {
    FT_index t=type;
    GetDocument()->image2type(t);
    ftrPoint=GetFeaturePoint(featr,t);
    if (ftrPoint.x!=POINT_UNDEF && winVisible.PtInRect(ftrPoint)) {
      featr.sprint(buf);
      // clip leading space from text label
      char *lbuf=buf;
      while (isspace(*lbuf)) ++lbuf;
      // draw icon
      if (prom>=ftr_large) {
	static_app->largeImageList.Draw(static_pDC,type,
	  CPoint(ftrPoint.x-LARGE_ICON_SIZE/2,ftrPoint.y-LARGE_ICON_Y_OFFSET),
	  ls==LS_SELECTED?ILD_SELECTED:ILD_TRANSPARENT);
      }
      else if (prom>=ftr_medium) {
	static_app->mediumImageList.Draw(static_pDC,type,
	  CPoint(ftrPoint.x-MEDIUM_ICON_SIZE/2,ftrPoint.y-MEDIUM_ICON_Y_OFFSET),
	  ls==LS_SELECTED?ILD_SELECTED:ILD_TRANSPARENT);
      }
      else if (prom>=ftr_small) {
	static_app->smallImageList.Draw(static_pDC,type,
	  CPoint(ftrPoint.x-SMALL_ICON_SIZE/2,ftrPoint.y-SMALL_ICON_Y_OFFSET),
	  ls==LS_SELECTED?ILD_SELECTED:ILD_TRANSPARENT);
      }
      if (prom>=ftr_label && lbuf[0]!='\0') { // label is not empty
	// clip trailing space
	char *rbuf=lbuf+strlen(lbuf);
	while (isspace(rbuf[-1])) --rbuf;
	*rbuf='\0';
	if (!static_pDC->IsPrinting())
	  static_pDC->SetBkColor(ls==LS_SELECTED?color_select:current.color_bg);
	if (prom>=ftr_medium) {
	  lblRect.top=ftrPoint.y+(prom>=ftr_large ?
	    LARGE_ICON_SIZE-LARGE_ICON_Y_OFFSET :
	    MEDIUM_ICON_SIZE-MEDIUM_ICON_Y_OFFSET);
	  lblRect.bottom=lblRect.top+24;
	  lblRect.left=ftrPoint.x-LARGE_LABEL_RECT_SIDE;
	  lblRect.right=ftrPoint.x+LARGE_LABEL_RECT_SIDE;
	  static_pDC->DrawText(lbuf,-1,lblRect,DT_CALCRECT|DT_NOPREFIX|DT_WORDBREAK|DT_CENTER);
	  int h_offset=ftrPoint.x-(lblRect.left+lblRect.right)/2;
	  lblRect.left+=h_offset;
	  lblRect.right+=h_offset;
	  static_pDC->DrawText(lbuf,-1,lblRect,DT_NOCLIP|DT_NOPREFIX|DT_WORDBREAK|DT_CENTER);
	}
	else if (prom>=ftr_small) {
	  static CSize lblSize;
	  lblSize=static_pDC->GetTextExtent(lbuf,rbuf-lbuf);
	  static_pDC->TextOut(ftrPoint.x+SMALL_ICON_SIZE/2,
			      ftrPoint.y-lblSize.cy/2,lbuf,rbuf-lbuf);
	}
      }
    }
  }
}

void CGraphView::DrawPeaks(const Divide_tree& tree, FT_index type)
{
  if (current.view_peak && label_state[FT_PEAK]) {
    GetDocument()->update_peak_prom_info_rq();
    current.peak_fmt.set(GetDocument()->data,FMS_NOFLAG);
    for (int i=tree.n_dom()-1; i>=0; --i)
      DrawFeature(tree[i].peak,type,label_state[FT_PEAK][i],
		  get_peak_prom(tree[i]));
  }
}

void CGraphView::DrawSaddles(const Divide_tree& tree, FT_index type)
{
  if (current.view_saddle && label_state[FT_SADDLE]) {
    GetDocument()->update_sadl_prom_info_rq();
    current.saddle_fmt.set(GetDocument()->data,FMS_NOFLAG);
    for (int i=tree.n_dom()-1; i>=0; --i)
      DrawFeature(tree[i].saddle,type,label_state[FT_SADDLE][i],
		  get_saddle_prom(tree[i]));
  }
}

void CGraphView::DrawRunoffs(const Divide_tree& tree, FT_index type)
{
  if (current.view_runoff && label_state[FT_RUNOFF]) {
    GetDocument()->update_peak_prom_info_rq();
    current.runoff_fmt.set(GetDocument()->data,FMS_NOFLAG);
    for (Domain::Index i=0; i<tree.n_runoff(); ++i)
      DrawFeature(tree.runoff(i),type,label_state[FT_RUNOFF][i],
		  get_prom(tree.runoff(i)));
  }
}

void CGraphView::DrawBasnsadls(const Divide_tree& tree, FT_index type)
{
  if (current.view_bsnsadl && label_state[FT_BASINSADL]) {
    GetDocument()->update_sadl_prom_info_rq();
    current.bsnsadl_fmt.set(GetDocument()->data,FMS_NOFLAG);
    for (int i=tree.n_bsnsdl()-1; i>=0; --i) {
      DrawFeature(tree.bsnsdl(i),type,label_state[FT_BASINSADL][i],
		  get_prom(tree.bsnsdl(i)));
    }
  }
}

void CGraphView::DrawFeatures(CDC *pDC, Divide_tree& tree)
{
  pDC->GetClipBox(&winVisible);
  // expand the window to accomodate partially visible features
  winVisible.top-=LARGE_LABEL_RECT_BOTM;
  winVisible.bottom+=LARGE_ICON_Y_OFFSET;
  winVisible.left-=SMALL_LABEL_RECT_WIDTH;
  winVisible.right+=LARGE_LABEL_RECT_SIDE;
  // prepare the DC
  static_pDC=pDC;
  static_app=CPromApp::getApp();
  pDC->SetTextColor(pDC->IsPrinting()?RGB(0,0,0):current.color_label);
  pDC->SetBkMode(OPAQUE);
  color_select=GetSysColor(COLOR_HIGHLIGHT);
  CFont *oldfont=(CFont *)pDC->SelectStockObject(DEFAULT_GUI_FONT);

  // draw all features in appropriate order
  if (GetDocument()->is_drainage) {
    DrawPeaks(tree,FT_BASIN);
    DrawRunoffs(tree,FT_SBRO);
    DrawSaddles(tree,FT_BASINSADL);
    DrawBasnsadls(tree,FT_SADDLE);
  }
  else {
    DrawBasnsadls(tree,FT_BASINSADL);
    DrawSaddles(tree,FT_SADDLE);
    DrawRunoffs(tree,FT_RUNOFF);
    DrawPeaks(tree,FT_PEAK);
  }

  pDC->SelectObject(oldfont);
}

void CGraphView::OnDraw(CDC *pDC)
{
  CPromDoc *doc=GetDocument();
  if (doc->working) return;

  saveCenter();
  DrawRidges(pDC);
  DrawFeatures(pDC,doc->tree);
}

void CGraphView::viewFeature(const CPromDoc *doc, const Feature& featr, FT_index type)
{
  if (GetDocument()!=doc) {
    ::MessageBox(NULL,"The active graphical view does not represent the\n"
		 "same workspace as the active feature list view.",
		 "Winprom View",MB_OK|MB_ICONERROR);
    return;
  }
  viewFeature(featr,type);
}

void CGraphView::viewFeature(const Feature& featr, FT_index type)
{
  CPoint new_center=GetFeaturePoint(featr,type);
  if (new_center.x==POINT_UNDEF) {
    ::MessageBox(NULL,"The feature you requested is outside\n"
		 "the range of the active view.",
		 "Winprom Feature Unviewable",MB_OK|MB_ICONERROR);
    return;
  }
  setGraphScroll(new_center);
}

void CGraphView::viewLocation(const GridPoint&)
{
  ::MessageBox(NULL,"The active graphical view is not location-based.",
	       "Winprom View",MB_OK|MB_ICONERROR);
}

#ifdef tool_tips

BOOL CGraphView::DisplayTip(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
  TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
  UINT nID =pNMHDR->idFrom;
  if (pTTT->uFlags & TTF_IDISHWND) {
    // idFrom is actually the HWND of the tool
    nID = ::GetDlgCtrlID((HWND)nID);
    if(nID) {
      pTTT->lpszText = MAKEINTRESOURCE(nID);
      pTTT->hinst = AfxGetResourceHandle();
      return TRUE;
    }
  }
  return FALSE;
}


static CRect clientRect;

void CGraphView::set_tool(const Feature& feature, FT_index ft, Domain::Index i)
{
  Tool_info& info=tool_info[ft][i];
  CPoint point=GetFeaturePoint(feature,ft);
  point-=GetScrollPosition();
  if (clientRect.PtInRect(point)) {
    // we need a tool tip
    CRect toolRect(point.x-LARGE_CLK_TOL,point.y-LARGE_CLK_TOL,
		   point.x+LARGE_CLK_TOL,point.y+LARGE_CLK_TOL);
    if (info.have_tool) {
      // move it
      tool_tip.SetToolRect(this,i,&toolRect);
    }
    else {
      // create it
      feature.sprint(buf);
      tool_tip.AddTool(this,buf,toolRect,i);
    }
  }
  else if (info.have_tool)
    tool_tip.DelTool(this,i);
}

void CGraphView::set_tools()
{
  Feature::format="%N\n%L to %H";
  GetClientRect(&clientRect);
  const Divide_tree& tree=GetDocument()->tree;
  //for (int i=tree.n_dom()-1; i>=0; --i)
    //set_tool(tree[i].peak,FT_PEAK,i);
  tool_tip.AddTool(this,"Now is the time",CRect(0,0,50,50),1);
}

#endif // def tool_tips

/////////////////////////////////////////////////////////////////////////////
// CGraphView printing

BOOL CGraphView::OnPreparePrinting(CPrintInfo *pInfo)
{
  CPromApp *pApp = CPromApp::getApp();
  pInfo->m_pPD->m_pd.Flags|=PD_PRINTSETUP;
  print_params.job_size=GetTotalSize();
  print_params.job_size.cx+=2*GRAPH_VIEW_MARGIN;
  print_params.job_size.cy+=2*GRAPH_VIEW_MARGIN;
  if (pApp->DoPrintDialog(pInfo->m_pPD) != IDOK) return FALSE;
  print_params.update(*pInfo->m_pPD);
  return print_params.final_update(pInfo,10);
}

#define MAX_PIXIN 100

static bool DCscaled;

void CGraphView::OnBeginPrinting(CDC *, CPrintInfo* /*pInfo*/)
{
  DCscaled=false;
}

void CGraphView::scale_print(CDC *pDC, CPrintInfo *pInfo) const
{
  if (pInfo->m_bPreview || !DCscaled) {
    int pixin=pDC->GetDeviceCaps(LOGPIXELSY);
    if (pixin>MAX_PIXIN) {
      int fac=(pixin-1)/MAX_PIXIN+1;
      pDC->SetMapMode(MM_ANISOTROPIC);
      pDC->ScaleViewportExt(fac,1,fac,1);
      DCscaled=true;
      //TRACE("scaled page %d DC\n",pInfo->m_nCurPage);
    }
  }
}

void CGraphView::OnPrepareDC(CDC *pDC, CPrintInfo *pInfo)
{
  if (!pInfo) {
    CScrollView::OnPrepareDC(pDC, pInfo);
    return;
  }
  scale_print(pDC,pInfo);
  const int& nxpag=print_params.nxpag;
  const CSize& page_size=print_params.page_size;
  int ixpag=(pInfo->m_nCurPage-1)%nxpag,iypag=(pInfo->m_nCurPage-1)/nxpag;
  pDC->SetWindowOrg(ixpag*page_size.cx-print_params.extra_margin.cx,
		    iypag*page_size.cy-print_params.extra_margin.cy);
}

void CGraphView::OnPrint(CDC *pDC, CPrintInfo *pInfo)
{
  //TRACE("printing page %d\n",pInfo->m_nCurPage);
  CScrollView::OnPrint(pDC, pInfo);
}

void CGraphView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CGraphView::PrintParams::update(CPrintDialog& pd)
{
  if (pd.m_pd.hDC==0)
    pd.m_pd.hDC=pd.CreatePrinterDC();
  page_size.cx=::GetDeviceCaps(pd.m_pd.hDC,HORZRES);
  page_size.cy=::GetDeviceCaps(pd.m_pd.hDC,VERTRES);
  int pixin=::GetDeviceCaps(pd.m_pd.hDC,LOGPIXELSY);
  if (pixin>MAX_PIXIN) {
    int fac=(pixin-1)/MAX_PIXIN+1;
    page_size.cx/=fac;
    page_size.cy/=fac;
  }
  nxpag=(job_size.cx-1)/page_size.cx+1;
  nypag=(job_size.cy-1)/page_size.cy+1;
}

BOOL CGraphView::PrintParams::final_update(CPrintInfo *pInfo, unsigned npag_warn)
{
  int npag=nxpag*nypag;
  if (!pInfo->m_bPreview && npag_warn>0 && npag>=npag_warn) {
    sprintf(buf,"Your print job has %d pages.\nDo you want to continue?",npag);
    if (AfxMessageBox(buf,MB_YESNO|MB_ICONWARNING|
		      (npag<5*npag_warn?MB_DEFBUTTON1:MB_DEFBUTTON2))!=IDYES)
      return FALSE;
  }
  extra_margin.cx=(page_size.cx*nxpag-job_size.cx)/2;
  extra_margin.cy=(page_size.cy*nypag-job_size.cy)/2;
  pInfo->m_pPD->m_pd.nFromPage=1;
  pInfo->m_pPD->m_pd.nToPage=npag;
  pInfo->SetMaxPage(npag);
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGraphView diagnostics

#ifdef _DEBUG
void CGraphView::AssertValid() const
{
  CScrollView::AssertValid();
}

void CGraphView::Dump(CDumpContext& dc) const
{
  CScrollView::Dump(dc);
}

CPromDoc *CGraphView::GetDocument() const // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPromDoc)));
  return (CPromDoc *)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CGraphView message handlers

void CGraphView::saveCenter()
{
  if (viewable()) old_center=getGraphScroll();
  else old_center.x=old_center.y=-1;
}

void CGraphView::restoreCenter()
{
  if (old_center.x>=0 && old_center.y>=0)
    setGraphScroll(old_center);
}

void CGraphView::InvalidateFeature(Feature& featr, FT_index type)
{
  CSize winSize=getWinSize();
  CPoint featurePt=GetFeaturePoint(featr,type),
	 viewUL=GetScrollPosition(),
	 viewLR(viewUL.x+winSize.cx,viewUL.y+winSize.cy);
  if (featurePt.x==POINT_UNDEF) return;
  CRect featureRect;
  Elevation prom=get_feature_prom(&featr,type);
  if (prom>=ftr_medium) {
    featureRect.left=featurePt.x-LARGE_LABEL_RECT_SIDE;
    featureRect.top=featurePt.y-(prom>=ftr_large ?
      LARGE_ICON_Y_OFFSET : MEDIUM_ICON_Y_OFFSET);
    featureRect.right=featurePt.x+LARGE_LABEL_RECT_SIDE;
    featureRect.bottom=featurePt.y+LARGE_LABEL_RECT_BOTM;
  }
  else if (prom>=ftr_small) {
    featureRect.left=featurePt.x-SMALL_ICON_SIZE/2;
    featureRect.top=featurePt.y-SMALL_ICON_SIZE/2;
    featureRect.right=featurePt.x+SMALL_ICON_SIZE/2+SMALL_LABEL_RECT_WIDTH;
    featureRect.bottom=featurePt.y+SMALL_ICON_SIZE/2;
  }
  if (CRect().IntersectRect(CRect(viewUL,viewLR),featureRect)) {
    viewUL.x*=-1; viewUL.y*=-1;
    featureRect.OffsetRect(viewUL);
    InvalidateRect(featureRect);
  }
}

void CGraphView::OnUpdate(CView *, LPARAM lHint, CObject *pHint)
{
  Feature *changed_feature=(Feature *)lHint;

  CPromDoc *doc=GetDocument();
  Divide_tree& tree=doc->tree;

  if (changed_feature==0) {
    for (unsigned ift=0; ift<FT_NUM_TYPES; ++ift) {
      delete[] label_state[ift];
      //delete[] tool_info[i];
    }
    doc->update_ridge_info();
    doc->update_sadl_prom_info();
    Domain::Index i,npeak=tree.n_dom(),nro=tree.n_runoff(),nbs=tree.n_bsnsdl();
    label_state[FT_PEAK]=npeak ? new Label_state[npeak] : 0;
    for (i=0; i<npeak; ++i) label_state[FT_PEAK][i]=LS_VISIBLE;
    label_state[FT_SADDLE]=npeak ? new Label_state[npeak] : 0;
    for (i=0; i<npeak; ++i) label_state[FT_SADDLE][i]=LS_VISIBLE;
    label_state[FT_RUNOFF]=nro ? new Label_state[nro] : 0;
    for (i=0; i<nro; ++i) label_state[FT_RUNOFF][i]=LS_VISIBLE;
    label_state[FT_BASINSADL]=nbs ? new Label_state[nbs] : 0;
    for (i=0; i<nbs; ++i) label_state[FT_BASINSADL][i]=LS_VISIBLE;
    zero_select();

#ifdef tool_tips
    tool_info[FT_PEAK]=npeak ? new Tool_info[npeak] : 0;
    tool_info[FT_SADDLE]=npeak ? new Tool_info[npeak] : 0;
    tool_info[FT_RUNOFF]=nro ? new Tool_info[nro] : 0;
    tool_info[FT_BASINSADL]=nbs ? new Tool_info[nbs] : 0;
#endif
  }
  else if (changed_feature==&CPromDoc::dummyModifyRObase) {
    Domain::Index i,nro=tree.n_runoff();
    Label_state *new_ro_sel=new Label_state[nro];
    //for (i=0; i<doc->old_n; ++i) new_ro_sel[i]=label_state[FT_RUNOFF][i];
    //for (; i<nro; ++i) new_ro_sel[i]=LS_VISIBLE;
    for (i=0; i<doc->new_iro; ++i) new_ro_sel[i]=label_state[FT_RUNOFF][i];
    new_ro_sel[doc->new_iro]=LS_VISIBLE;
    for (; i<doc->old_n; ++i) new_ro_sel[i+1]=label_state[FT_RUNOFF][i];
    delete[] label_state[FT_RUNOFF];
    label_state[FT_RUNOFF]=new_ro_sel;
  }
  else if (changed_feature==&CPromDoc::dummyModifyBSbase ||
	   changed_feature==&CPromDoc::dummyModifySplice) {
    Domain::Index i,nbs=tree.n_bsnsdl();
    Label_state *new_bs_sel=new Label_state[nbs];
    for (i=0; i<doc->old_n; ++i) new_bs_sel[i]=label_state[FT_BASINSADL][i];
    for (; i<nbs; ++i) new_bs_sel[i]=LS_VISIBLE;
    delete[] label_state[FT_BASINSADL];
    label_state[FT_BASINSADL]=new_bs_sel;
  }

  if (changed_feature==0 ||
      changed_feature==&CPromDoc::dummyModifyTopo ||
      changed_feature==&CPromDoc::dummyModifyAll ||
      changed_feature==&CPromDoc::dummyModifyTrans ||
      changed_feature==&CPromDoc::dummyModifyRObase ||
      changed_feature==&CPromDoc::dummyModifyBSbase ||
      changed_feature==&CPromDoc::dummyModifySplice ||
      changed_feature==&CPromDoc::dummyModifyBounds ||
      changed_feature==&CPromDoc::dummyModifyData)
    Invalidate();
  else if (changed_feature==&CPromDoc::dummyModifyMap ||
	   changed_feature==&CPromDoc::dummySaveCenter) {
    // do nothing; CGraphView doesn't know what a map is
  }
  else if (!changed_feature->location) {
    // a feature was deleted
    Invalidate();
    switch (doc->changed_type) {
    case FT_PEAK:
      if (label_state[FT_PEAK][tree.index(Domain::FromPeak(changed_feature))]==LS_SELECTED)
	--nsel_feature[FT_PEAK];
      break;
    case FT_SADDLE:
      if (label_state[FT_SADDLE][tree.saddle_index(*changed_feature)]==LS_SELECTED)
	--nsel_feature[FT_SADDLE];
      break;
    case FT_RUNOFF:
      if (label_state[FT_RUNOFF][tree.index(*(Runoff *)changed_feature)]==LS_SELECTED)
	--nsel_feature[FT_RUNOFF];
      break;
    case FT_BASINSADL:
      if (label_state[FT_BASINSADL][tree.index(*(Basin_saddle *)changed_feature)]==LS_SELECTED)
	--nsel_feature[FT_BASINSADL];
      break;
    }
  }
  else {
    // a feature was modified.
    /* we must invalidate the entire view, in case prominences
       (and hence icon sizes and line weights) changed */
    Invalidate();
#if 0
    // if showing features or ridges that might have changed, invalidate the whole view.
    // otherwise, invalidate only the changed feature.
    if ((current.peak_ridge || current.saddle_ridge ||
	 current.runoff_ridge || current.bsnsadl_ridge) &&
	(ridge_major>0 && ridge_major<elev_infty ||
	 ridge_std>0   && ridge_std<elev_infty   ||
	 ridge_minor>0 && ridge_minor<elev_infty)	||
	(current.view_peak || current.view_saddle ||
	 current.view_runoff || current.view_bsnsadl) &&
	(ftr_large>0  && ftr_large<elev_infty	||
	 ftr_medium>0 && ftr_medium<elev_infty	||
	 ftr_small>0  && ftr_small<elev_infty))
      Invalidate();
    else InvalidateFeature(*changed_feature,doc->changed_type);
#endif
  }
}

void CGraphView::OnViewZoomWayIn()
{
  zoom(big_zoom_factor);
}

void CGraphView::OnViewZoomIn()
{
  zoom(zoom_factor);
}

void CGraphView::OnViewZoomOut()
{
  zoom(1/zoom_factor);
}

void CGraphView::OnViewZoomWayOut()
{
  zoom(1/big_zoom_factor);
}

void CGraphView::OnUpdateViewZoomWayin(CCmdUI *pCmdUI)
{
  CSize viewSize=GetTotalSize();
  pCmdUI->Enable(viewSize.cx*big_zoom_factor<=GRAPH_MAX_DIM &&
		 viewSize.cy*big_zoom_factor<=GRAPH_MAX_DIM &&
		 viewable());
}

void CGraphView::OnUpdateViewZoomIn(CCmdUI *pCmdUI)
{
  CSize viewSize=GetTotalSize();
  pCmdUI->Enable(viewSize.cx*zoom_factor<=GRAPH_MAX_DIM &&
		 viewSize.cy*zoom_factor<=GRAPH_MAX_DIM &&
		 viewable());
}

void CGraphView::OnUpdateViewable(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(viewable());
}

void CGraphView::OnViewRidgeBasinSadl()
{
  current.bsnsadl_ridge=!current.bsnsadl_ridge;
  Invalidate();
}

void CGraphView::OnUpdateViewRidgeBasinSadl(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(divideTreeVisible());
  pCmdUI->SetCheck(current.bsnsadl_ridge);
  pCmdUI->SetText(GetDocument()->is_drainage?
		  "Basin - &prom saddle\tShift+F12":
		  "Peak - &basin saddle\tShift+F12");
}

void CGraphView::OnViewRidgeRunoff()
{
  current.runoff_ridge=!current.runoff_ridge;
  Invalidate();
}

void CGraphView::OnUpdateViewRidgeRunoff(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(divideTreeVisible());
  pCmdUI->SetCheck(current.runoff_ridge);
  pCmdUI->SetText(GetDocument()->is_drainage?
		  "Basin - &runoff\tShift+F11":
		  "Peak - &runoff\tShift+F11");
}

void CGraphView::OnViewRidgeSaddle()
{
  current.saddle_ridge=!current.saddle_ridge;
  if (current.saddle_ridge) current.peak_ridge=false;
  Invalidate();
}

void CGraphView::OnUpdateViewRidgeSaddle(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(divideTreeVisible());
  pCmdUI->SetCheck(current.saddle_ridge);
  pCmdUI->SetText(GetDocument()->is_drainage?
		  "Basin - &saddle - basin\tShift+F10":
		  "Peak - &saddle - peak\tShift+F10");
}

void CGraphView::OnViewRidgePeak()
{
  current.peak_ridge=!current.peak_ridge;
  if (current.peak_ridge) current.saddle_ridge=false;
  Invalidate();
}

void CGraphView::OnUpdateViewRidgePeak(CCmdUI *pCmdUI)
{
  if (pCmdUI->m_pSubMenu) {
    CPromDoc::enable_submenu(pCmdUI,divideTreeVisible());
  }
  else {
    pCmdUI->Enable(divideTreeVisible());
    pCmdUI->SetCheck(current.peak_ridge);
    pCmdUI->SetText(GetDocument()->is_drainage?
		    "&Basin - to - basin\tShift+F9":
		    "&Peak - to - peak\tShift+F9");
  }
}

void CGraphView::OnViewRidgeAll()
{
  current.peak_ridge=false;
  current.saddle_ridge=current.runoff_ridge=current.bsnsadl_ridge=true;
  Invalidate();
}

void CGraphView::OnUpdateViewRidgeAll(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(divideTreeVisible());
}

void CGraphView::OnViewRidgeNone()
{
  current.peak_ridge=current.saddle_ridge=current.runoff_ridge=current.bsnsadl_ridge=false;
  Invalidate();
}

void CGraphView::OnUpdateViewRidgeNone(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(divideTreeVisible());
}

void CGraphView::OnSize(UINT nType, int cx, int cy)
{
  CScrollView::SetScrollSizes(MM_TEXT,GetTotalSize(),
    CSize((int)(cx/zoom_factor),(int)(cy/zoom_factor)),
    CSize(GRAPH_SCROLL_ARROW_BUTN,GRAPH_SCROLL_ARROW_BUTN));
  restoreCenter();
  //set_tools();
  setSizeIndicators();
}

void CGraphView::OnViewLabelBasinSadl()
{
  CFeatureFmt_dlg dlg(GetDocument()->data,GetParent(),current.bsnsadl_fmt);
  if (dlg.DoModal()==IDOK) Invalidate();
}

void CGraphView::OnUpdateViewLabelBasinsadl(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(divideTreeVisible() && current.view_bsnsadl);
  pCmdUI->SetText(GetDocument()->is_drainage?
		  "&Prom saddle...":"&Basin saddle...");
}

void CGraphView::OnViewLabelPeak()
{
  CFeatureFmt_dlg dlg(GetDocument()->data,GetParent(),current.peak_fmt);
  if (dlg.DoModal()==IDOK) Invalidate();
}

void CGraphView::OnUpdateViewLabelPeak(CCmdUI *pCmdUI)
{
  if (pCmdUI->m_pSubMenu) {
    CPromDoc::enable_submenu(pCmdUI,divideTreeVisible());
  }
  else {
    pCmdUI->Enable(divideTreeVisible() && current.view_peak);
    pCmdUI->SetText(GetDocument()->is_drainage?"&Basin...":"&Peak...");
  }
}

void CGraphView::OnViewLabelRunoff()
{
  CFeatureFmt_dlg dlg(GetDocument()->data,GetParent(),current.runoff_fmt);
  if (dlg.DoModal()==IDOK) Invalidate();
}

void CGraphView::OnUpdateViewLabelRunoff(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(divideTreeVisible() && current.view_runoff);
}

void CGraphView::OnViewLabelSaddle()
{
  CFeatureFmt_dlg dlg(GetDocument()->data,GetParent(),current.saddle_fmt);
  if (dlg.DoModal()==IDOK) Invalidate();
}

void CGraphView::OnUpdateViewLabelSaddle(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(divideTreeVisible() && current.view_saddle);
}

void CGraphView::OnViewLabelAll()
{
  CFeatureFmt_dlg dlg(GetDocument()->data,GetParent(),current.peak_fmt);
  if (dlg.DoModal()!=IDOK) return;
  current.saddle_fmt=current.runoff_fmt=current.bsnsadl_fmt=current.peak_fmt;
  Invalidate();
}

void CGraphView::OnUpdateAnyLabelVisible(CCmdUI *pCmdUI)
{
  Divide_tree& tree=GetDocument()->tree;
  int i,n;

  if (!divideTreeVisible()) {
    pCmdUI->Enable(FALSE);
    return;
  }

  if (current.view_peak) {
    n=tree.n_dom();
    for (i=0; i<n; ++i)
      if (tree[i].peak.location && label_state[FT_PEAK][i]!=LS_HIDDEN) {
	pCmdUI->Enable(TRUE);
	return;
      }
  }

  if (current.view_saddle) {
    n=tree.n_dom();
    for (i=0; i<n; ++i)
      if (tree[i].primary_nbr && label_state[FT_SADDLE][i]!=LS_HIDDEN) {
	pCmdUI->Enable(TRUE);
	return;
      }
  }

  if (current.view_runoff) {
    n=tree.n_runoff();
    for (i=0; i<n; ++i)
      if (tree.runoff(i).location && label_state[FT_RUNOFF][i]!=LS_HIDDEN) {
	pCmdUI->Enable(TRUE);
	return;
      }
  }

  if (current.view_bsnsadl) {
    n=tree.n_bsnsdl();
    for (i=0; i<n; ++i)
      if (tree.bsnsdl(i).location && label_state[FT_BASINSADL][i]!=LS_HIDDEN) {
	pCmdUI->Enable(TRUE);
	return;
      }
  }

  pCmdUI->Enable(FALSE);
}

void CGraphView::OnViewColorsBg()
{
  CViewColorDialog dlg(current.color_bg,"Background Color");
  if (dlg.DoModal()!=IDOK) return;
  current.color_bg=dlg.GetColor();
  Invalidate();
}

void CGraphView::OnViewColorsLabels()
{
  CViewColorDialog dlg(current.color_label,"Label Color");
  if (dlg.DoModal()!=IDOK) return;
  current.color_label=dlg.GetColor();
  Invalidate();
}

void CGraphView::OnViewFeaturesPeaks()
{
  current.view_peak=!current.view_peak;
  Invalidate();
}

void CGraphView::OnViewFeaturesBasinsadl()
{
  current.view_bsnsadl=!current.view_bsnsadl;
  Invalidate();
}

void CGraphView::OnViewFeaturesRunoffs()
{
  current.view_runoff=!current.view_runoff;
  Invalidate();
}

void CGraphView::OnViewFeaturesSaddles()
{
  current.view_saddle=!current.view_saddle;
  Invalidate();
}

void CGraphView::OnViewFeaturesAll()
{
  current.view_peak=current.view_saddle=current.view_runoff=current.view_bsnsadl=true;
  translateStates(LS_HIDDEN,LS_VISIBLE,0);
  Invalidate();
}

void CGraphView::OnViewFeaturesNone()
{
  current.view_peak=current.view_saddle=current.view_runoff=current.view_bsnsadl=false;
  Invalidate();
}

void CGraphView::OnUpdateViewFeaturesPeaks(CCmdUI *pCmdUI)
{
  if (pCmdUI->m_pSubMenu) {
    CPromDoc::enable_submenu(pCmdUI,divideTreeVisible());
  }
  else {
    pCmdUI->Enable(divideTreeVisible());
    pCmdUI->SetCheck(current.view_peak);
    pCmdUI->SetText(GetDocument()->is_drainage?"&Basins\tF9":"&Peaks\tF9");
  }
}

void CGraphView::OnUpdateViewFeaturesBasinsadl(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(divideTreeVisible());
  pCmdUI->SetCheck(current.view_bsnsadl);
  pCmdUI->SetText(GetDocument()->is_drainage?
		  "&Prom saddles\tF12":"&Basin saddles\tF12");
}

void CGraphView::OnUpdateViewFeaturesRunoffs(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(divideTreeVisible());
  pCmdUI->SetCheck(current.view_runoff);
}

void CGraphView::OnUpdateViewFeaturesSaddles(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(divideTreeVisible());
  pCmdUI->SetCheck(current.view_saddle);
}

void CGraphView::OnUpdateViewFeaturesNone(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(divideTreeVisible());
}

void CGraphView::OnUpdateViewFeaturesAll(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(divideTreeVisible());
}

void CGraphView::OnActivateView(BOOL bActivate, CView *pActivateView, CView *pDeactiveView)
{
  if (bActivate) {
    setSizeIndicators();
    GetDocument()->set_tb_bmp();
  }
  CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

bool CGraphView::mouseHit(CPoint point, Feature& featr,
			  FT_index type, Elevation prom)
{
  if (!featr.location) return false;
  CPoint featurePt=GetFeaturePoint(featr,type);
  if (featurePt.x==POINT_UNDEF) return false;
  point-=featurePt;
  int click_tol;
  if (prom>=ftr_large) click_tol=LARGE_CLK_TOL;
  else if (prom>=ftr_medium) click_tol=MEDIUM_CLK_TOL;
  else if (prom>=ftr_small) click_tol=SMALL_CLK_TOL;
  else return false;
  return point.x<=click_tol && point.x>=-click_tol &&
    point.y<=click_tol && point.y>=-click_tol;
}

Feature *CGraphView::HitPeak(CPoint point, bool visible,
			     Domain::Index& i, FT_index& type)
{
  if (current.view_peak) {
    Divide_tree& tree=GetDocument()->tree;
    Domain::Index npeak=tree.n_dom();
    for (i=0; i<npeak; ++i) {
      Domain& dom=tree[i];
      if (visible==(label_state[FT_PEAK][i]!=LS_HIDDEN) &&
	  mouseHit(point,dom.peak,FT_PEAK,get_peak_prom(dom))) {
	type=FT_PEAK;
	return &dom.peak;
      }
    }
  }
  return 0;
}

Feature *CGraphView::HitSaddle(CPoint point, bool visible,
			       Domain::Index& i, FT_index& type)
{
  if (current.view_saddle) {
    Divide_tree& tree=GetDocument()->tree;
    Domain::Index nsadl=tree.n_dom();
    for (i=0; i<nsadl; ++i) {
      Domain& dom=tree[i];
      if (visible==(label_state[FT_SADDLE][i]!=LS_HIDDEN) &&
	  mouseHit(point,dom.saddle,FT_SADDLE,get_saddle_prom(dom))) {
	type=FT_SADDLE;
	return &dom.saddle;
      }
    }
  }
  return 0;
}

Feature *CGraphView::HitRunoff(CPoint point, bool visible,
			       Domain::Index& i, FT_index& type)
{
  if (current.view_runoff) {
    Divide_tree& tree=GetDocument()->tree;
    Domain::Index nro=tree.n_runoff();
    for (i=0; i<nro; ++i) {
      Runoff& ro=tree.runoff(i);
      if (visible==(label_state[FT_RUNOFF][i]!=LS_HIDDEN) &&
	  mouseHit(point,ro,FT_RUNOFF,get_prom(ro))) {
	type=FT_RUNOFF;
	return &ro;
      }
    }
  }
  return 0;
}

Feature *CGraphView::HitBasnsadl(CPoint point, bool visible,
				 Domain::Index& i, FT_index& type)
{
  if (current.view_bsnsadl) {
    Divide_tree& tree=GetDocument()->tree;
    Domain::Index nbs=tree.n_bsnsdl();
    for (i=0; i<nbs; ++i) {
      Basin_saddle& bs=tree.bsnsdl(i);
      if (visible==(label_state[FT_BASINSADL][i]!=LS_HIDDEN) &&
	  mouseHit(point,bs,FT_BASINSADL,get_prom(bs))) {
	type=FT_BASINSADL;
	return &bs;
      }
    }
  }
  return 0;
}

Feature *CGraphView::HitTest(CPoint point, bool visible,
			     Domain::Index& i, FT_index& type)
{
  point+=GetScrollPosition();
  Feature *hit;
  if (GetDocument()->is_drainage) {
    hit=HitBasnsadl(point,visible,i,type);
    if (hit) return hit;
    hit=HitSaddle(point,visible,i,type);
    if (hit) return hit;
    hit=HitRunoff(point,visible,i,type);
    if (hit) return hit;
    hit=HitPeak(point,visible,i,type);
    if (hit) return hit;
  }
  else {
    hit=HitPeak(point,visible,i,type);
    if (hit) return hit;
    hit=HitRunoff(point,visible,i,type);
    if (hit) return hit;
    hit=HitSaddle(point,visible,i,type);
    if (hit) return hit;
    hit=HitBasnsadl(point,visible,i,type);
    if (hit) return hit;
  }

  return 0;
}

static CGraphView::Label_state state1,state2;

void CGraphView::translateState(FT_index type, Domain::Index i, Feature& featr,
				Feature *&oneFeatr, FT_index& oneType)
{
  if (!featr.location) return;
  if (label_state[type][i]==state1) {
    label_state[type][i]=state2;
    oneFeatr = oneFeatr ? &CPromDoc::dummyModifyAll : &featr;
    oneType=type;
  }
  nsel_feature[type]+=label_state[type][i]==LS_SELECTED;
}

void CGraphView::translateStates(Label_state ls1, Label_state ls2,
				 Elevation prom)
{
  Divide_tree& tree=GetDocument()->tree;
  Domain::Index i,npeak=tree.n_dom(),nro=tree.n_runoff(),nbs=tree.n_bsnsdl();
  Feature *oneFeature=0;
  FT_index oneType;
  state1=ls1;
  state2=ls2;
  if (current.view_peak) {
    nsel_feature[FT_PEAK]=0;
    for (i=0; i<npeak; ++i)
      if (get_peak_prom(tree[i])>=prom)
	translateState(FT_PEAK,i,tree[i].peak,oneFeature,oneType);
  }
  if (current.view_saddle) {
    nsel_feature[FT_SADDLE]=0;
    for (i=0; i<npeak; ++i)
      if (get_saddle_prom(tree[i])>=prom)
	translateState(FT_SADDLE,i,tree[i].saddle,oneFeature,oneType);
  }
  if (current.view_runoff) {
    nsel_feature[FT_RUNOFF]=0;
    for (i=0; i<nro; ++i)
      if (get_prom(tree.runoff(i))>=prom)
	translateState(FT_RUNOFF,i,tree.runoff(i),oneFeature,oneType);
  }
  if (current.view_bsnsadl) {
    nsel_feature[FT_BASINSADL]=0;
    for (i=0; i<nbs; ++i)
      if (get_prom(tree.bsnsdl(i))>=prom)
	translateState(FT_BASINSADL,i,tree.bsnsdl(i),oneFeature,oneType);
  }

  if (oneFeature==&CPromDoc::dummyModifyAll) Invalidate();
  else if (oneFeature!=0) InvalidateFeature(*oneFeature,oneType);
}

void CGraphView::SelectOne(Feature *featr, Domain::Index i, FT_index type)
{
  // fool translateState() into thinking this label state wasn't changed
  // this is so the flicker can be avoided if it was the only one already selected
  bool already_selected=label_state[type][i]==LS_SELECTED;
  label_state[type][i]=LS_VISIBLE;
  translateStates(LS_SELECTED,LS_VISIBLE,0); // clear selection
  label_state[type][i]=LS_SELECTED;
  nsel_feature[type]=1;
  if (!already_selected) InvalidateFeature(*featr,type);
}

void CGraphView::OnLButtonDown(UINT nFlags, CPoint point)
{
  if (!divideTreeVisible()) return;
  Domain::Index i;
  FT_index type;
  CPromDoc *doc=GetDocument();
  doc->dragDrop.clear();
  downPt=point;
  Feature *clkf=HitTest(point,(nFlags&MK_SHIFT)==0,i,type);
  if (clkf) {
    if (nFlags&MK_CONTROL) {
      label_state[type][i]=label_state[type][i]==LS_SELECTED?
	LS_VISIBLE:LS_SELECTED;
      nsel_feature[type]+=label_state[type][i]==LS_SELECTED?1:-1;
      InvalidateFeature(*clkf,type);
    }
    else {
      if (nFlags&MK_SHIFT) {
	label_state[type][i]=LS_VISIBLE;
	InvalidateFeature(*clkf,type);
      }
      else if (label_state[type][i]!=LS_SELECTED)
	SelectOne(clkf,i,type);
    }
  }
  else if ((nFlags&MK_CONTROL)==0 && (nFlags&MK_SHIFT)==0)
    translateStates(LS_SELECTED,LS_VISIBLE,0); // clear selection
  if (!tree_writeable()) AfxGetApp()->OnIdle(0);
}

#define MOVE_DELTA 3

void CGraphView::OnMouseMove(UINT nFlags, CPoint point)
{
  CPromDoc *doc=GetDocument();
  CPromApp *app=CPromApp::getApp();
  if (nFlags&MK_LBUTTON) {
    CPoint ptRoot(point);
    MapWindowPoints(NULL,&ptRoot,1);
    if (doc->dragDrop.ftr_count()>0) {
      // dragging
      CImageList::DragMove(ptRoot);
      SetCursor(app->LoadStandardCursor(doc->
	canTakeDrop(this,point) ? IDC_ARROW : IDC_NO));
    }
    else if (visibleSelect()>0 &&
	     (abs(point.x-downPt.x)>=MOVE_DELTA || abs(point.y-downPt.y)>=MOVE_DELTA)) {
      // begin drag
      copyClip(doc->dragDrop);
      app->smallImageList.BeginDrag(doc->dragDrop.iconType(doc->is_drainage),
				    CPoint(SMALL_ICON_SIZE/2,SMALL_ICON_Y_OFFSET));
      CImageList::DragEnter(NULL,ptRoot);
      SetCapture();
    }
  }

  CScrollView::OnMouseMove(nFlags, point);
}

void CGraphView::OnLButtonUp(UINT nFlags, CPoint point)
{
  CPromDoc *document=GetDocument();
  if (document->dragDrop.ftr_count()==0) {
    Domain::Index i;
    FT_index type;
    Feature *clkf=HitTest(point,true,i,type);
    if (clkf && (nFlags&MK_CONTROL)==0 && (nFlags&MK_SHIFT)==0)
      SelectOne(clkf,i,type);
  }
  else document->takeDrop(this,point);

  CScrollView::OnLButtonUp(nFlags, point);
}

Domain::Index CGraphView::visibleSelect()
{
  return divideTreeVisible() ?
    (current.view_peak   ?nsel_feature[FT_PEAK]     :0) +
    (current.view_saddle ?nsel_feature[FT_SADDLE]   :0) +
    (current.view_runoff ?nsel_feature[FT_RUNOFF]   :0) +
    (current.view_bsnsadl?nsel_feature[FT_BASINSADL]:0) :
    0;
}

void CGraphView::OnUpdateEditAnySelect(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(visibleSelect()>0);
}

void CGraphView::OnUpdateEditSingleSelect(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(visibleSelect()==1);
}

Feature& CGraphView::FindSelectedFeature(Domain::Index& i, FT_index& type)
{
  Divide_tree& tree=GetDocument()->tree;
  Domain::Index n;
  if (current.view_peak && nsel_feature[FT_PEAK]==1) {
    n=tree.n_dom();
    for (i=0; i<n; ++i)
      if (tree[i].peak.location && label_state[FT_PEAK][i]==LS_SELECTED) {
	type=FT_PEAK;
	return tree[i].peak;
      }
    assert(0);
  }

  if (current.view_saddle && nsel_feature[FT_SADDLE]==1) {
    n=tree.n_dom();
    for (i=0; i<n; ++i)
      if (tree[i].primary_nbr && label_state[FT_SADDLE][i]==LS_SELECTED) {
	type=FT_SADDLE;
	return tree[i].saddle;
      }
    assert(0);
  }

  if (current.view_runoff && nsel_feature[FT_RUNOFF]==1) {
    n=tree.n_runoff();
    for (i=0; i<n; ++i)
      if (tree.runoff(i).location && label_state[FT_RUNOFF][i]==LS_SELECTED) {
	type=FT_RUNOFF;
	return tree.runoff(i);
      }
    assert(0);
  }

  if (current.view_bsnsadl && nsel_feature[FT_BASINSADL]==1) {
    n=tree.n_bsnsdl();
    for (i=0; i<n; ++i)
      if (tree.bsnsdl(i).location && label_state[FT_BASINSADL][i]==LS_SELECTED) {
	type=FT_BASINSADL;
	return tree.bsnsdl(i);
      }
    assert(0);
  }

  assert(0);
  return CPromDoc::dummyModifyAll; // keep compiler happy

}

Feature& CGraphView::FindSelectedPeak()
{
  Divide_tree& tree=GetDocument()->tree;
  Domain::Index n=tree.n_dom(),i;
  for (i=0; i<n; ++i)
    if (tree[i].peak.location && label_state[FT_PEAK][i]==LS_SELECTED)
      return tree[i].peak;
  assert(0);
  return CPromDoc::dummyModifyAll; // keep compiler happy
}

Feature& CGraphView::FindSelectedSaddle()
{
  Divide_tree& tree=GetDocument()->tree;
  Domain::Index n=tree.n_dom(),i;
  for (i=0; i<n; ++i)
    if (tree[i].primary_nbr && label_state[FT_SADDLE][i]==LS_SELECTED)
      return tree[i].saddle;
  assert(0);
  return CPromDoc::dummyModifyAll; // keep compiler happy
}

Runoff& CGraphView::FindSelectedRunoff()
{
  Divide_tree& tree=GetDocument()->tree;
  Domain::Index n=tree.n_runoff(),i;
  for (i=0; i<n; ++i)
    if (tree.runoff(i).location && label_state[FT_RUNOFF][i]==LS_SELECTED)
      return tree.runoff(i);
  assert(0);
  return (Runoff&)CPromDoc::dummyModifyAll; // keep compiler happy
}

Basin_saddle& CGraphView::FindSelectedBsnsadl()
{
  Divide_tree& tree=GetDocument()->tree;
  Domain::Index n=tree.n_bsnsdl(),i;
  for (i=0; i<n; ++i)
    if (tree.bsnsdl(i).location && label_state[FT_BASINSADL][i]==LS_SELECTED)
      return tree.bsnsdl(i);
  assert(0);
  return (Basin_saddle&)CPromDoc::dummyModifyAll; // keep compiler happy
}

void CGraphView::copyClip(Clip& clp)
{
  Divide_tree& tree=GetDocument()->tree;
  Domain::Index i,n;

  clp.clear();

  n=tree.n_dom();
  if (current.view_peak) {
    for (i=0; i<n; ++i) {
      if (tree[i].peak.location && label_state[FT_PEAK][i]==LS_SELECTED) {
	clp.add_featr(tree[i].peak,FT_PEAK);
      }
    }
  }

  if (current.view_saddle) {
    for (i=0; i<n; ++i) {
      if (tree[i].primary_nbr && label_state[FT_SADDLE][i]==LS_SELECTED) {
	clp.add_featr(tree[i].saddle,FT_SADDLE);
      }
    }
  }

  if (current.view_runoff) {
    n=tree.n_runoff();
    for (i=0; i<n; ++i) {
      if (tree.runoff(i).location && label_state[FT_RUNOFF][i]==LS_SELECTED) {
	clp.add_featr(tree.runoff(i),FT_RUNOFF);
      }
    }
  }

  if (current.view_bsnsadl) {
    n=tree.n_bsnsdl();
    for (i=0; i<n; ++i) {
      if (tree.bsnsdl(i).location && label_state[FT_BASINSADL][i]==LS_SELECTED) {
	clp.add_featr(tree.bsnsdl(i),FT_BASINSADL);
      }
    }
  }

}

void CGraphView::OnEditCopy()
{
  copyClip(GetDocument()->copyPaste);
}

void CGraphView::pasteClip(const Clip& clp)
{
  Divide_tree& tree=GetDocument()->tree;
  int iv;
  unsigned i,n=clp.ftr_count();
  for (i=0; i<n; ++i) {
    if (clp.list[i]->location) {
      switch (clp.type[i]) {
      case FT_PEAK:
	iv=tree.index(Domain::FromPeak(clp.list[i]));
	break;
      case FT_SADDLE:
	iv=tree.saddle_index(*clp.list[i]);
	break;
      case FT_RUNOFF:
	iv=tree.index(*(Runoff *)clp.list[i]);
	break;
      case FT_BASINSADL:
	iv=tree.index(*(Basin_saddle *)clp.list[i]);
	break;
      default:
	assert(0);
      }
      if (label_state[clp.type[i]][iv]==LS_HIDDEN)
	label_state[clp.type[i]][iv]=LS_VISIBLE;
    }
  }

  Invalidate();
}

void CGraphView::OnEditPaste()
{
  pasteClip(GetDocument()->copyPaste);
}

void CGraphView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
  Clip& clp=GetDocument()->copyPaste;
  unsigned i,n=clp.ftr_count();
  for (i=0; i<n; ++i) {
    switch (clp.type[i]) {
    case FT_PEAK:
      if (current.view_peak) break;
      continue;
    case FT_SADDLE:
      if (current.view_saddle) break;
      continue;
    case FT_RUNOFF:
      if (current.view_runoff) break;
      continue;
    case FT_BASINSADL:
      if (current.view_bsnsadl) break;
      continue;
    default:
      assert(0);
    }
    pCmdUI->Enable(TRUE);
    return;
  }
  pCmdUI->Enable(FALSE);
}

void CGraphView::OnEditSelectAll()
{
  translateStates(LS_VISIBLE,LS_SELECTED,ftr_small);
}

void CGraphView::invertState(FT_index type, Domain::Index i, Feature& featr)
{
  if (!featr.location) return;
  switch (label_state[type][i]) {
  case LS_VISIBLE:
    label_state[type][i]=LS_SELECTED;
    ++nsel_feature[type];
    break;
  case LS_SELECTED:
    label_state[type][i]=LS_VISIBLE;
    --nsel_feature[type];
    break;
  }
}

void CGraphView::OnEditSelectInvert()
{
  Divide_tree& tree=GetDocument()->tree;
  Domain::Index i,npeak=tree.n_dom(),nro=tree.n_runoff(),nbs=tree.n_bsnsdl();
  if (current.view_peak)
    for (i=0; i<npeak; ++i)
      invertState(FT_PEAK,i,tree[i].peak);
  if (current.view_saddle)
    for (i=0; i<npeak; ++i)
      invertState(FT_SADDLE,i,tree[i].saddle);
  if (current.view_runoff)
    for (i=0; i<nro; ++i)
      invertState(FT_RUNOFF,i,tree.runoff(i));
  if (current.view_bsnsadl)
    for (i=0; i<nbs; ++i)
      invertState(FT_BASINSADL,i,tree.bsnsdl(i));

  Invalidate();
}

void CGraphView::filterTranslateState(FT_index type, Domain::Index i,
				      const Feature& featr, const FeatureFilter& filter)
{
  if (featr.location && label_state[type][i]==state1 &&
      filter.test(featr,GetDocument()->data)) {
    label_state[type][i]=state2;
    if (state2==LS_SELECTED) ++nsel_feature[type];
    else if (state1==LS_SELECTED) --nsel_feature[type];
  }
}

void CGraphView::filterTranslateStates(const FeatureTypeFilter& filter,
				       Label_state ls1, Label_state ls2)
{
  state1=ls1;
  state2=ls2;
  filterModifyStates(filter, &CGraphView::filterTranslateState);
}

void CGraphView::OnEditSelectFilterSet()
{
  FeatureTypeFilter filter;
  filter.rec_filter.align(GetDocument()->data);
  CFeatureTypeFilter_dlg dlg(filter,GetParent());
  if (dlg.DoModal()==IDOK)
    filterTranslateStates(filter,LS_VISIBLE,LS_SELECTED);
}

void CGraphView::filterModifyStates(const FeatureTypeFilter& filter,
	void (CGraphView::*modify_fn)(FT_index, Domain::Index,
				      const Feature&, const FeatureFilter&))
{
  Divide_tree& tree=GetDocument()->tree;
  Domain::Index i,npeak=tree.n_dom(),nro=tree.n_runoff(),nbs=tree.n_bsnsdl();
  if (current.view_peak && filter.test(FT_PEAK)) {
    for (i=0; i<npeak; ++i)
      (this->*modify_fn)(FT_PEAK,i,tree[i].peak,filter);
  }
  if (current.view_saddle && filter.test(FT_SADDLE)) {
    for (i=0; i<npeak; ++i)
      (this->*modify_fn)(FT_SADDLE,i,tree[i].saddle,filter);
  }
  if (current.view_runoff && filter.test(FT_RUNOFF)) {
    for (i=0; i<nro; ++i)
      (this->*modify_fn)(FT_RUNOFF,i,tree.runoff(i),filter);
  }
  if (current.view_bsnsadl && filter.test(FT_BASINSADL)) {
    for (i=0; i<nbs; ++i)
      (this->*modify_fn)(FT_BASINSADL,i,tree.bsnsdl(i),filter);
  }

  Invalidate();
}

void CGraphView::filterPreciseState(FT_index type, Domain::Index i,
				    const Feature& featr, const FeatureFilter& filter)
{
  if (!featr.location || label_state[type][i]==LS_HIDDEN) return;
  if ((label_state[type][i]=filter.test(featr,GetDocument()->data)?LS_SELECTED:LS_VISIBLE)==LS_SELECTED)
    ++nsel_feature[type];
}

void CGraphView::OnEditSelectFilterPrecise()
{
  FeatureTypeFilter filter;
  filter.rec_filter.align(GetDocument()->data);
  CFeatureTypeFilter_dlg dlg(filter,GetParent());
  if (dlg.DoModal()==IDOK) {
    zero_select();
    filterModifyStates(filter, &CGraphView::filterPreciseState);
  }
}

void CGraphView::filterInvertState(FT_index type, Domain::Index i,
				   const Feature& featr, const FeatureFilter& filter)
{
  if (!featr.location || !filter.test(featr,GetDocument()->data)) return;
  switch (label_state[type][i]) {
  case LS_VISIBLE:
    label_state[type][i]=LS_SELECTED;
    ++nsel_feature[type];
    break;
  case LS_SELECTED:
    label_state[type][i]=LS_VISIBLE;
    --nsel_feature[type];
    break;
  }
}

void CGraphView::OnEditSelectFilterClear()
{
  FeatureTypeFilter filter;
  filter.rec_filter.align(GetDocument()->data);
  CFeatureTypeFilter_dlg dlg(filter,GetParent());
  if (dlg.DoModal()==IDOK)
    filterTranslateStates(filter,LS_SELECTED,LS_VISIBLE);
}

void CGraphView::OnEditSearch()
{
  const CPromDoc *doc=GetDocument();
  const Clip& clip=doc->copyPaste;
  const Divide_tree& tree=doc->tree;
  Feature *oneFeature=0;
  FT_index oneType;
  state1=LS_VISIBLE;
  state2=LS_SELECTED;
  unsigned i,n=clip.ftr_count();
  for (i=0; i<n; ++i) {
    Feature& featr=*clip.list[i];
    switch (clip.type[i]) {
    case FT_PEAK:
      if (current.view_peak)
	translateState(FT_PEAK,tree.index(Domain::FromPeak(&featr)),
		       featr,oneFeature,oneType);
      break;
    case FT_SADDLE:
      if (current.view_saddle)
	translateState(FT_SADDLE,tree.index(Domain::FromSadl(&featr)),
		       featr,oneFeature,oneType);
      break;
    case FT_RUNOFF:
      if (current.view_runoff)
	translateState(FT_RUNOFF,tree.index((Runoff&)featr),
		       featr,oneFeature,oneType);
      break;
    case FT_BASINSADL:
      if (current.view_bsnsadl)
	translateState(FT_BASINSADL,tree.index((Basin_saddle&)featr),
		       featr,oneFeature,oneType);
      break;
    default:
      assert(0);
    }
  }

  if (oneFeature==&CPromDoc::dummyModifyAll) Invalidate();
  else if (oneFeature!=0) InvalidateFeature(*oneFeature,oneType);
  else {
    MessageBox("No feature on the clipboard is visible.",
	       "Winprom Search");
  }
}

void CGraphView::OnUpdateEditSearch(CCmdUI *pCmdUI)
{
  const Clip& clip=GetDocument()->copyPaste;
  unsigned i,n=clip.ftr_count();
  for (i=0; i<n; ++i) {
    switch (clip.type[i]) {
    case FT_PEAK:
      if (current.view_peak) {
	pCmdUI->Enable(TRUE);
	return;
      }
      break;
    case FT_SADDLE:
      if (current.view_saddle) {
	pCmdUI->Enable(TRUE);
	return;
      }
      break;
    case FT_RUNOFF:
      if (current.view_runoff) {
	pCmdUI->Enable(TRUE);
	return;
      }
      break;
    case FT_BASINSADL:
      if (current.view_bsnsadl) {
	pCmdUI->Enable(TRUE);
	return;
      }
      break;
    default:
      assert(0);
    }
  }
  pCmdUI->Enable(FALSE);
}

void CGraphView::OnEditCenter()
{
  FT_index ft;
  Domain::Index i;
  Feature& f=FindSelectedFeature(i,ft);
  viewFeature(f,ft);
}

void CGraphView::OnEditBrowse() 
{
  FT_index ft;
  Domain::Index i;
  Feature& f=FindSelectedFeature(i,ft);
  CPromApp::getApp()->Navigate(GetDocument(),f,ft);
}

void CGraphView::OnEditUnview()
{
  translateStates(LS_SELECTED,LS_HIDDEN,0);
}

void CGraphView::OnEditFeature()
{
  FT_index ft;
  Domain::Index i;
  Feature& feature=FindSelectedFeature(i,ft);
  GetDocument()->EditFeature(feature,ft,GetParent());
}

void CGraphView::OnUpdateEditFeature(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(tree_writeable() && visibleSelect()==1);
}

void CGraphView::OnEditDelete()
{
  if (visibleSelect()==1) {
    FT_index ft;
    Domain::Index i;
    Feature& feature=FindSelectedFeature(i,ft);
    GetDocument()->DeleteFeature(feature,ft,GetParent());
  }
  else {
    Feature *peak=&FindSelectedPeak(),*sadl=&FindSelectedSaddle();
#ifdef administrator
    if (CPromDoc::peak_sadl_nbr(peak,sadl))
      GetDocument()->DeletePeak(*peak,sadl,GetParent(),true);
    else if (GetDocument()->dt_CanClose(true)) {
      GetDocument()->DeleteRiver(peak,sadl);
    }
#else
    GetDocument()->DeletePeak(*peak,sadl,GetParent(),true);
#endif
  }
}

void CGraphView::OnUpdateEditDelete(CCmdUI *pCmdUI)
{
  if (!tree_writeable()) {
    pCmdUI->Enable(FALSE);
    return;
  }
  if (visibleSelect()!=1) {
    bool opos=current.view_peak && current.view_saddle &&
      nsel_feature[FT_PEAK]==1 && nsel_feature[FT_SADDLE]==1;
#ifdef administrator
    pCmdUI->Enable(opos);
    if (!opos ||
	CPromDoc::peak_sadl_nbr(&FindSelectedPeak(),&FindSelectedSaddle()))
      pCmdUI->SetText("Delete");
    else pCmdUI->SetText("Delete riverbed");
#else
    pCmdUI->Enable(opos &&
      CPromDoc::peak_sadl_nbr(&FindSelectedPeak(),&FindSelectedSaddle()));
#endif
  }
}

void CGraphView::OnEditRotate()
{
  if (visibleSelect()==1) {
    GetDocument()->Rotate(FindSelectedBsnsadl(),0,GetParent());
  }
  else {
    GetDocument()->Rotate(FindSelectedBsnsadl(),
			  &FindSelectedSaddle(),GetParent());
  }
}

void CGraphView::OnUpdateEditRotate(CCmdUI *pCmdUI)
{
  if (!tree_writeable())
    pCmdUI->Enable(FALSE);
  else if (visibleSelect()==1)
    pCmdUI->Enable(current.view_bsnsadl && nsel_feature[FT_BASINSADL]==1);
  else
    pCmdUI->Enable(current.view_bsnsadl && current.view_saddle &&
		   nsel_feature[FT_BASINSADL]==1 && nsel_feature[FT_SADDLE]==1 &&
		   ((Basin_saddle&)FindSelectedBsnsadl()).
		   on_cycle(Domain::FromSadl(&FindSelectedSaddle())));
}

void CGraphView::OnEditSplice()
{
  if (nsel_feature[FT_RUNOFF]==1) {
    GetDocument()->Splice(FindSelectedRunoff(),GetParent());
  }
  else {
    Runoff **ros=new Runoff*[nsel_feature[FT_RUNOFF]];
    Divide_tree& tree=GetDocument()->tree;
    Domain::Index iro,nro=tree.n_runoff(),i=0;
    for (iro=0; iro<nro; ++iro) {
      Runoff& ro=tree.runoff(iro);
      if (ro.location && label_state[FT_RUNOFF][iro]==LS_SELECTED) {
	ros[i]=&ro;
	++i;
      }
    }
    assert(i==nsel_feature[FT_RUNOFF]);
    GetDocument()->Splice(ros,nsel_feature[FT_RUNOFF],GetParent());
    delete ros;
  }
}

void CGraphView::OnUpdateEditSplice(CCmdUI *pCmdUI)
{
  if (!tree_writeable() ||
      (current.view_peak && nsel_feature[FT_PEAK]>0) ||
      (current.view_saddle && nsel_feature[FT_SADDLE]>0) ||
      (current.view_bsnsadl && nsel_feature[FT_BASINSADL]>0) ||
      !current.view_runoff || nsel_feature[FT_RUNOFF]==0) {
    pCmdUI->Enable(FALSE);
    return;
  }
  unsigned total_iq=0;
  Divide_tree& tree=GetDocument()->tree;
  Domain::Index i,n=tree.n_runoff();
  for (i=0; i<n; ++i) {
    if (tree.runoff(i).location && label_state[FT_RUNOFF][i]==LS_SELECTED) {
      total_iq+=tree.runoff(i).interior_quads;
      if (total_iq>4) {
	pCmdUI->Enable(FALSE);
	return;
      }
    }
  }
  pCmdUI->Enable(TRUE);
}

void CGraphView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  if (!tree_writeable()) {
    MessageBox("You cannot edit a feature here because a feature\n"
	       "is already open for editing in the same workspace.",
	       NULL,MB_OK|MB_ICONERROR);
    return;
  }
  if (divideTreeVisible() && visibleSelect()==1 &&
      (nFlags&MK_SHIFT)==0 && (nFlags&MK_CONTROL)==0) {
    FT_index ft;
    Domain::Index i;
    Feature& feature=FindSelectedFeature(i,ft);
    point+=GetScrollPosition();
    if (mouseHit(point,feature,ft,get_feature_prom(&feature,ft)))
      GetDocument()->EditFeature(feature,ft,GetParent());
  }
}

#define PARENT_POS 0
#define NBR_POS 2

void CGraphView::OnRButtonDown(UINT nFlags, CPoint point)
{
  Domain::Index i;
  if (divideTreeVisible() && (nFlags&MK_SHIFT)==0) {
    r_click_featr=HitTest(point,true,i,r_click_type);
    if (r_click_featr)
      point=GetFeaturePoint(*r_click_featr,r_click_type)-GetScrollPosition();
    assert(point.x!=POINT_UNDEF);
  }
  else r_click_featr=0;
  CMenu graphMenu,*popup,*id_menu;
  graphMenu.LoadMenu(IDR_MENU_POPUP);
  if (r_click_featr) {
    r_click_index=i;
    popup=featureMenu(graphMenu,*r_click_featr);
    GetDocument()->setup_popup(popup,*r_click_featr,r_click_type);
    GetDocument()->update_sadl_prom_info();
    id_menu=idMenu(*popup);
    switch (r_click_type) {
    case FT_PEAK:
      if (!Domain::FromPeak(r_click_featr)->parent.cell) {
	// Disable the parent and saddle submenus, if this is the patriarch
	id_menu->EnableMenuItem(PARENT_POS,MF_BYPOSITION|MF_GRAYED);
	id_menu->EnableMenuItem(ID_IDENTIFY_SADDLE,MF_BYCOMMAND|MF_GRAYED);
      }
      id_menu->EnableMenuItem(NBR_POS,MF_BYPOSITION|MF_GRAYED);
      break;
    case FT_SADDLE:
      id_menu->ModifyMenu(ID_IDENTIFY_SADDLE,MF_BYCOMMAND|MF_STRING,
			  ID_IDENTIFY_SADDLE,"&Child");
      break;
    case FT_RUNOFF:
      id_menu->ModifyMenu(ID_IDENTIFY_SADDLE,MF_BYCOMMAND|MF_STRING,
			  ID_IDENTIFY_SADDLE,"&Peak");
      if (!((Runoff *)r_click_featr)->peak) {
	id_menu->EnableMenuItem(ID_IDENTIFY_SADDLE,MF_BYCOMMAND|MF_GRAYED);
      }
      id_menu->EnableMenuItem(PARENT_POS,MF_BYPOSITION|MF_GRAYED);
      id_menu->EnableMenuItem(NBR_POS,MF_BYPOSITION|MF_GRAYED);
      break;
    case FT_BASINSADL:
      id_menu->EnableMenuItem(PARENT_POS,MF_BYPOSITION|MF_GRAYED);
      id_menu->EnableMenuItem(ID_IDENTIFY_SADDLE,MF_BYCOMMAND|MF_GRAYED);
      break;
    default:
      assert(0);
    }
  }
  else {
    popup=backgdMenu(graphMenu,point);
  }
  ClientToScreen(&point);
  if (popup) popup->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,point.x,point.y,this);
}

void CGraphView::scroll_by(int x, int y)
{
  setLimitedScroll(GetScrollPosition()+CPoint(x,y));
}

void CGraphView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  bool shift=GetKeyState(VK_SHIFT)<0;
  switch (nChar) {
  case VK_UP: case VK_NUMPAD8:
    scroll_by(0,-(shift?getWinSize().cy/zoom_factor:
		        GRAPH_SCROLL_ARROW_KEY)*nRepCnt);
    break;
  case VK_DOWN: case VK_NUMPAD2:
    scroll_by(0,(shift?getWinSize().cy/zoom_factor:
		       GRAPH_SCROLL_ARROW_KEY)*nRepCnt);
    break;
  case VK_LEFT: case VK_NUMPAD4:
    scroll_by(-(shift?getWinSize().cx/zoom_factor:
		      GRAPH_SCROLL_ARROW_KEY)*nRepCnt,0);
    break;
  case VK_RIGHT: case VK_NUMPAD6:
    scroll_by((shift?getWinSize().cx/zoom_factor:
		     GRAPH_SCROLL_ARROW_KEY)*nRepCnt,0);
    break;
  }

  CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGraphView::OnFeatureCenter()
{
  viewFeature(*r_click_featr,r_click_type);
}

void CGraphView::OnRQFinfo()
{
  CRQFinfo_dlg(GetDocument(),r_click_featr,r_click_type).DoModal();
}

void CGraphView::OnFeatureBrowse()
{
  CPromApp::getApp()->Navigate(GetDocument(),*r_click_featr,r_click_type);
}

void CGraphView::OnFeatureDelete()
{
  GetDocument()->DeleteFeature(*r_click_featr,r_click_type,GetParent());
}

void CGraphView::OnFeatureEdit()
{
  GetDocument()->EditFeature(*r_click_featr,r_click_type,GetParent());
}

void CGraphView::OnFeatureUnview()
{
  Divide_tree& tree=GetDocument()->tree;
  if (label_state[r_click_type][r_click_index]==LS_SELECTED)
    --nsel_feature[r_click_type];
  label_state[r_click_type][r_click_index]=LS_HIDDEN;
  switch (r_click_type) {
  case FT_PEAK:
    InvalidateFeature(tree[r_click_index].peak,FT_PEAK);
    break;
  case FT_SADDLE:
    InvalidateFeature(tree[r_click_index].saddle,FT_SADDLE);
    break;
  case FT_RUNOFF:
    InvalidateFeature(tree.runoff(r_click_index),FT_RUNOFF);
    break;
  case FT_BASINSADL:
    InvalidateFeature(tree.bsnsdl(r_click_index),FT_BASINSADL);
    break;
  default:
    assert(0);
    break;
  }
}

void CGraphView::OnBasinsadlRotate()
{
  GetDocument()->Rotate(*(Basin_saddle *)r_click_featr,0,GetParent());
}

void CGraphView::OnRunoffSplice()
{
  GetDocument()->Splice(*(Runoff *)r_click_featr,GetParent());
}

void CGraphView::OnFeatureListSaddles()
{
  switch (r_click_type) {
  case FT_PEAK:
    CPromSadlListView::init_list(GetDocument(),
      *Domain::FromPeak(r_click_featr),GetParent());
    break;
  case FT_BASINSADL:
    CCycleListView::init_list(GetDocument(),
      *(Basin_saddle *)r_click_featr,GetParent());
    break;
  default:
    assert(0);
  }
}

void CGraphView::OnFeatureListParents()
{
  CParentListView::init_list(GetDocument(),
    *Domain::FromPeak(r_click_featr),GetParent());
}

void CGraphView::OnFeatureListNbrs()
{
  switch (r_click_type) {
  case FT_PEAK:
    CPeakNbrListView::init_list(GetDocument(),*Domain::FromPeak(r_click_featr));
    break;
  case FT_RUNOFF:
    CRunoffNbrListView::init_list(GetDocument(),*(Runoff *)r_click_featr);
    break;
  default:
    assert(0);
  }
}

void CGraphView::OnFeatureListTree()
{
  CLPRtreeView::init_tree(GetDocument(),
    *Domain::FromPeak(r_click_featr),GetParent());
}

void CGraphView::OnListPromSadl()
{
  Divide_tree& tree=GetDocument()->tree;
  CPromSadlListView::List_info peaks;
  peaks.store(nsel_feature[FT_PEAK]);
  int selItem=-1,i=0;
  Domain::Index npeak=tree.n_dom(),ipeak;
  for (ipeak=0; ipeak<npeak; ++ipeak) {
    if (label_state[FT_PEAK][ipeak]==LS_SELECTED) {
      peaks.doms[i]=&tree[ipeak];
      ++i;
    }
  }
  CPromSadlListView::init_list(GetDocument(),peaks,GetParent());
}

void CGraphView::OnUpdateListPromSadl(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(divideTreeVisible() &&
    current.view_peak && nsel_feature[FT_PEAK]>0 &&
    (!current.view_saddle  || nsel_feature[FT_SADDLE ]==0) &&
    (!current.view_runoff  || nsel_feature[FT_RUNOFF ]==0) &&
    (!current.view_bsnsadl || nsel_feature[FT_BASINSADL]==0));
  pCmdUI->SetText(GetDocument()->is_drainage?
		  "B&asin saddles...":"Pr&ominence saddles...");
}

void CGraphView::OnListCycle()
{
  Divide_tree& tree=GetDocument()->tree;
  CycleFilter bs;
  bs.store(nsel_feature[FT_BASINSADL]);
  int selItem=-1,i=0;
  Domain::Index nbs=tree.n_bsnsdl();
  for (Domain::Index ibs=0; ibs<nbs; ++ibs) {
    if (label_state[FT_BASINSADL][ibs]==LS_SELECTED) {
      bs.bs[i]=&tree.bsnsdl(ibs);
      ++i;
    }
  }
  CCycleListView::init_list(GetDocument(),bs,GetParent());
}

void CGraphView::OnUpdateListCycle(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(divideTreeVisible() &&
    current.view_bsnsadl && nsel_feature[FT_BASINSADL]>0 &&
    (!current.view_peak   || nsel_feature[FT_PEAK  ]==0) &&
    (!current.view_saddle || nsel_feature[FT_SADDLE]==0) &&
    (!current.view_runoff || nsel_feature[FT_RUNOFF]==0));
}

void CGraphView::OnListNbrs()
{
  if (current.view_peak && nsel_feature[FT_PEAK]==1) {
    CPeakNbrListView::init_list(GetDocument(),
			    *Domain::FromPeak(&FindSelectedPeak()));
  }
  else if (current.view_runoff && nsel_feature[FT_RUNOFF]==1) {
    CRunoffNbrListView::init_list(GetDocument(),FindSelectedRunoff());
  }
  else assert(0);
}

void CGraphView::OnUpdateListNbrs(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(divideTreeVisible() && visibleSelect()==1 &&
    (!current.view_saddle  || nsel_feature[FT_SADDLE ]==0) &&
    (!current.view_bsnsadl || nsel_feature[FT_BASINSADL]==0));
}

void CGraphView::OnListWalk()
{
  Feature *featr[2];
  FT_index ft[2];
  int ifeatr=0;
  Divide_tree& tree=GetDocument()->tree;
  Domain::Index i,n=tree.n_dom();
  if (current.view_peak) {
    for (i=0; i<n; ++i)
      if (tree[i].peak.location && label_state[FT_PEAK][i]==LS_SELECTED) {
	featr[ifeatr]=&tree[i].peak;
	ft[ifeatr]=FT_PEAK;
	++ifeatr;
      }
  }
  if (current.view_saddle) {
    for (i=0; i<n; ++i)
      if (tree[i].primary_nbr && label_state[FT_SADDLE][i]==LS_SELECTED) {
	featr[ifeatr]=&tree[i].saddle;
	ft[ifeatr]=FT_SADDLE;
	++ifeatr;
      }
  }
  if (current.view_runoff) {
    n=tree.n_runoff();
    for (i=0; i<n; ++i)
      if (tree.runoff(i).location && label_state[FT_RUNOFF][i]==LS_SELECTED) {
	featr[ifeatr]=&tree.runoff(i);
	ft[ifeatr]=FT_RUNOFF;
	++ifeatr;
      }
  }
  if (current.view_bsnsadl) {
    n=tree.n_bsnsdl();
    for (i=0; i<n; ++i)
      if (tree.bsnsdl(i).location && label_state[FT_BASINSADL][i]==LS_SELECTED) {
	featr[ifeatr]=&tree.bsnsdl(i);
	ft[ifeatr]=FT_BASINSADL;
	++ifeatr;
      }
  }
  if (ifeatr==2) {
    CWalkListView::init_list(GetDocument(),WalkInfo(featr[0],ft[0],featr[1],ft[1]));
  }
  else {
    assert(ifeatr==1);
    const Basin_saddle& bs=*(Basin_saddle *)featr[0];
    CWalkListView::init_list(GetDocument(),
      WalkInfo(&bs.peak1->peak,FT_PEAK,&bs.peak2->peak,FT_PEAK));
  }
}

void CGraphView::OnUpdateListWalk(CCmdUI *pCmdUI)
{
  if (!divideTreeVisible()) {
    pCmdUI->Enable(FALSE);
    return;
  }
  switch (visibleSelect()) {
  case 2: // both selected features must not be a basin saddle
    pCmdUI->Enable(!current.view_bsnsadl || nsel_feature[FT_BASINSADL]==0);
    break;
  case 1: // selected feature must be a basin saddle
    pCmdUI->Enable(current.view_bsnsadl && nsel_feature[FT_BASINSADL]==1);
    break;
  default:
    pCmdUI->Enable(FALSE);
    break;
  }
  return;
}

void CGraphView::OnListTree()
{
  CLPRtreeView::init_tree(GetDocument(),
    *Domain::FromPeak(&FindSelectedPeak()),GetParent());
}

void CGraphView::OnUpdateListTree(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(current.view_peak && nsel_feature[FT_PEAK]==1);
}

void CGraphView::IdentifyFeature(Feature& target, FT_index type)
{
  if (center_target) viewFeature(target,type);
  if (select_target) {
    Divide_tree& tree=GetDocument()->tree;
    Domain::Index i;
    switch (type) {
    case FT_PEAK:
      i=tree.index(Domain::FromPeak(&target));
      break;
    case FT_SADDLE:
      i=tree.saddle_index(target);
      break;
    default:
      assert(0);
    }
    if (label_state[type][i]!=LS_SELECTED) {
      label_state[type][i]=LS_SELECTED;
      ++nsel_feature[type];
      InvalidateFeature(target,type);
    }
  }
}

Domain *CGraphView::get_child()
{
  switch (r_click_type) {
  case FT_PEAK: return Domain::FromPeak(r_click_featr);
  case FT_SADDLE: return Domain::FromSadl(r_click_featr)->parent.child;
  }
  assert(0);
  return 0;
}

void CGraphView::OnIdentifyParentLine()
{
  IdentifyFeature(get_child()->parent.line->
		  parent_with_prom(min_view_prom())->peak,FT_PEAK);
}

void CGraphView::OnIdentifyParentCell()
{
  IdentifyFeature(get_child()->prom_parent()->peak,FT_PEAK);
}

void CGraphView::OnIdentifyParentIsland()
{
  IdentifyFeature(get_child()->parent.island->peak,FT_PEAK);
}

void CGraphView::OnIdentifySaddle()
{
  switch (r_click_type) {
  case FT_PEAK:
    IdentifyFeature(Domain::FromPeak(r_click_featr)->parent.saddle->saddle,FT_SADDLE);
    break;
  case FT_SADDLE:
    IdentifyFeature(Domain::FromSadl(r_click_featr)->parent.child->peak,FT_PEAK);
    break;
  case FT_RUNOFF:
    IdentifyFeature(((Runoff *)r_click_featr)->peak->peak,FT_PEAK);
    break;
  default:
    assert(0);
  }
}

void CGraphView::OnIdentifyNbr1()
{
  switch (r_click_type) {
  case FT_SADDLE:
    IdentifyFeature(Domain::FromSadl(r_click_featr)->peak,FT_PEAK);
    break;
  case FT_BASINSADL:
    {
      Basin_saddle *bs=(Basin_saddle *)r_click_featr;
      IdentifyFeature(vis_peak(bs->peak1,bs->prom.onmap)->peak,FT_PEAK);
    }
    break;
  default:
    assert(0);
  }
}

void CGraphView::OnIdentifyNbr2()
{
  switch (r_click_type) {
  case FT_SADDLE:
    IdentifyFeature(Domain::FromSadl(r_click_featr)->primary_nbr->peak,FT_PEAK);
    break;
  case FT_BASINSADL:
    {
      Basin_saddle *bs=(Basin_saddle *)r_click_featr;
      IdentifyFeature(vis_peak(bs->peak2,bs->prom.onmap)->peak,FT_PEAK);
    }
    break;
  default:
    assert(0);
  }
}

void CGraphView::OnIdentifyNbrBoth()
{
  bool ct=center_target; // save the value so we can temporarily turn it off
  center_target=false;
  switch (r_click_type) {
  case FT_SADDLE:
    IdentifyFeature(Domain::FromSadl(r_click_featr)->peak,FT_PEAK);
    IdentifyFeature(Domain::FromSadl(r_click_featr)->primary_nbr->peak,FT_PEAK);
    break;
  case FT_BASINSADL:
    {
      Basin_saddle *bs=(Basin_saddle *)r_click_featr;
      IdentifyFeature(vis_peak(bs->peak1,bs->prom.onmap)->peak,FT_PEAK);
      IdentifyFeature(vis_peak(bs->peak2,bs->prom.onmap)->peak,FT_PEAK);
    }
    break;
  default:
    assert(0);
  }
  center_target=ct; // restore the old value of center_target
}

void CGraphView::OnViewIdentify()
{
  CIdentifyFeature_dlg dlg;
  if (dlg.DoModal()==IDOK) {
    center_target=dlg.m_center_target!=FALSE;
    select_target=dlg.m_select_target!=FALSE;
  }
}

void CGraphView::OnNcMouseMove(UINT, CPoint)
{
  CStatusBar& sb=((CPromFrame *)AfxGetMainWnd())->statusBar;
  sb.SetPaneText(CPromFrame::SB_PANE_LOC,"");
  sb.SetPaneText(CPromFrame::SB_PANE_ELEV,"");
}


void CGraphView::OnEditSWI()
{
  CPromDoc *doc=GetDocument();
  Divide_tree& tree=doc->tree;
  Domain::Index n=tree.n_dom(),i,nps=0;
  Feature *sadl1=0,*sadl2=0,*peak=0;
  Basin_saddle *bs1=0,*bs2=0;
  Runoff *runoff=0;
  for (i=0; i<n; ++i) {
    if (current.view_saddle && tree[i].primary_nbr &&
	label_state[FT_SADDLE][i]==LS_SELECTED) {
      ++nps;
      if (sadl1==0) {
	sadl1=&tree[i].saddle;
	if (peak) break;
      }
      else {
	sadl2=&tree[i].saddle;
	break;
      }
    }
    if (current.view_peak && tree[i].peak.location &&
	label_state[FT_PEAK][i]==LS_SELECTED) {
      assert(peak==0);
      ++nps;
      peak=&tree[i].peak;
      if (sadl1) break;
    }
  }
  if (nps<2 && current.view_bsnsadl) {
    n=tree.n_bsnsdl();
    for (i=0; i<n; ++i) {
      Basin_saddle& bs=tree.bsnsdl(i);
      if (bs.location && label_state[FT_BASINSADL][i]==LS_SELECTED) {
	++nps;
	if (bs1==0) {
	  bs1=&bs;
	  if (nps==2) break;
	}
	else {
	  bs2=&bs;
	  break;
	}
      }
    }
  }
  if (nps<2 && current.view_runoff) {
    n=tree.n_runoff();
    for (i=0; i<n; ++i) {
      Runoff& ro=tree.runoff(i);
      if (ro.location && label_state[FT_RUNOFF][i]==LS_SELECTED) {
	assert(runoff==0);
	++nps;
	runoff=&ro;
	if (nps==2) break;
      }
    }
  }
  assert((peak!=0)+(sadl1!=0)+(sadl2!=0)+(bs1!=0)+(bs2!=0)+(runoff!=0)==nps);
  CWnd *pw=GetParent();
  if (peak && sadl1)
    doc->reconnect(*peak,*sadl1,pw);
  else if (peak && bs1)
    doc->reconnect(*peak,*bs1,pw);
  else if (sadl2)
    doc->SWI_switch_sym(*sadl1,*sadl2,pw);
  else if (sadl1 && bs1)
    doc->SWI_switch_sym(*sadl1,*bs1,pw);
  else if (bs2)
    doc->SWI_switch_sym(*bs1,*bs2,pw);
  else if (runoff && peak)
    doc->reconnect(*peak,*runoff,pw);
  else if (runoff && sadl1)
    doc->SWI_switch(*sadl1,*runoff);
  else if (runoff && bs1)
    doc->SWI_switch(*bs1,*runoff,pw);
  else assert(0);
}

void CGraphView::OnUpdateEditSWI(CCmdUI *pCmdUI)
{
  unsigned vis_peak=current.view_peak?nsel_feature[FT_PEAK]:0,
    vis_ps=current.view_saddle?nsel_feature[FT_SADDLE]:0,
    vis_ro=current.view_runoff?nsel_feature[FT_RUNOFF]:0,
    vis_bs=current.view_bsnsadl?nsel_feature[FT_BASINSADL]:0;
  pCmdUI->Enable(tree_writeable() && vis_ro<=1 && vis_peak<=1 &&
    vis_peak+vis_ps+vis_ro+vis_bs==2);
}

void CGraphView::filterModifyState(FT_index type, Domain::Index i,
				   const Feature& featr, const FeatureFilter& filter)
{
  if (tree_filter_action==2) filterInvertState(type,i,featr,filter);
  else filterTranslateState(type,i,featr,filter);
}

void CGraphView::select_subtree(Domain& domain, const Domain *root,
				bool *peak_visited, const Domain *ancestor,
				Domain::Topology topo)
{
  const CPromDoc *doc=GetDocument();
  const Database& data=doc->data;
  if (!tree_peak_xing.test(domain.peak,data)) return;
  const Divide_tree& tree=doc->tree;
  Domain::Index idom=tree.index(&domain);
  if (peak_visited) {
    if (peak_visited[idom]) return;
    peak_visited[idom]=true;
  }
  if (ancestor && !ancestor->is_ancestor(&domain,topo)) return;

  filterModifyState(FT_PEAK,tree.index(&domain),domain.peak,tree_peak_select);

  const vector<Runoff *>& ros=domain.runoffs;
  for (vector<Runoff *>::const_iterator roi=ros.begin();
       roi!=ros.end(); ++roi) {
    if (tree_sadl_xing.test(**roi,data))
      filterModifyState(FT_RUNOFF,tree.index(**roi),**roi,tree_sadl_select);
  }
  const Domain *so;
  for (Domain::Neighbor_iter ni(domain,root); ni; ++ni) {
    so=ni.saddle_owner();
    if (tree_sadl_xing.test(so->saddle,data)) {
      filterModifyState(FT_SADDLE,tree.index(so),so->saddle,tree_sadl_select);
      select_subtree(**ni,&domain,peak_visited,ancestor,topo);
    }
  }
  if (peak_visited) {
    const vector<Basin_saddle *>& bsn=domain.bsnsdls;
    for (vector<Basin_saddle *>::const_iterator bsi=bsn.begin();
	 bsi!=bsn.end(); ++bsi) {
      if (tree_sadl_xing.test(**bsi,data)) {
	filterModifyState(FT_BASINSADL,tree.index(**bsi),**bsi,tree_sadl_select);
	select_subtree(*(*bsi)->other_peak(&domain),0,peak_visited,ancestor,topo);
      }
    }
  }
}

void CGraphView::OnGraphSelect()
{
  CPromDoc *doc=GetDocument();
  const Database& data=doc->data;
  CTreeSelectFilter_dlg dlg(data);
  dlg.peak_xing=tree_peak_xing;
  dlg.peak_xing.rec_filter.align(data);
  dlg.sadl_xing=tree_sadl_xing;
  dlg.sadl_xing.rec_filter.align(data);
  dlg.peak_select=tree_peak_select;
  dlg.peak_select.rec_filter.align(data);
  dlg.sadl_select=tree_sadl_select;
  dlg.sadl_select.rec_filter.align(data);
  dlg.m_bs_xing=bs_xing || r_click_type==FT_BASINSADL;
  dlg.bs_root=r_click_type==FT_BASINSADL;
  dlg.m_relation=tree_filter_rel;
  dlg.m_topology=topology==Domain::TOPO_MS?0:topology-1;
  dlg.m_action=tree_filter_action;
  if (dlg.DoModal()!=IDOK) return;

  if (dlg.m_relation!=0) doc->update_sadl_prom_info();

  tree_peak_xing=dlg.peak_xing;
  tree_sadl_xing=dlg.sadl_xing;
  tree_peak_select=dlg.peak_select;
  tree_sadl_select=dlg.sadl_select;
  if (r_click_type!=FT_BASINSADL)
    bs_xing=dlg.m_bs_xing!=FALSE;
  tree_filter_rel=dlg.m_relation;
  Domain::Topology topo=(Domain::Topology)(dlg.m_topology+1);
  tree_filter_action=dlg.m_action;

  Domain *dom,*par_dom,*sadl_dom;
  Domain::Index i;
  const Divide_tree& tree=doc->tree;
  switch (tree_filter_action) {
  case 0: // set
    state1=LS_VISIBLE;
    state2=LS_SELECTED;
    break;
  case 1: // clear
    state1=LS_SELECTED;
    state2=LS_VISIBLE;
    break;
  }
  switch (r_click_type) {
  case FT_PEAK: 
    if (!tree_peak_xing.test(*r_click_featr,data)) {
      MessageBox("The clicked peak will not pass its crossing filter.",
		 "Winprom");
      return;
    }
    dom=Domain::FromPeak(r_click_featr);
    break;
  case FT_SADDLE:
    if (!tree_sadl_xing.test(*r_click_featr,data)) {
      MessageBox("The clicked saddle will not pass its crossing filter.",
		 "Winprom");
      return;
    }
    dom=Domain::FromSadl(r_click_featr);
    filterModifyState(FT_SADDLE,tree.index(dom),*r_click_featr,tree_sadl_select);
    break;
  case FT_RUNOFF:
    if (!tree_sadl_xing.test(*r_click_featr,data)) {
      MessageBox("The clicked runoff will not pass the saddle crossing filter.",
		 "Winprom");
      return;
    }
    {
      const Runoff& ro=*(Runoff *)r_click_featr;
      filterModifyState(FT_RUNOFF,tree.index(ro),*r_click_featr,tree_sadl_select);
      dom=ro.peak;
      if (dom==0) {
	Invalidate();
	return;
      }
    }
    break;
  case FT_BASINSADL:
    if (!tree_sadl_xing.test(*r_click_featr,data)) {
      MessageBox("The clicked basin saddle will not pass its crossing filter.",
		 "Winprom");
      return;
    }
    {
      const Basin_saddle& bs=*(Basin_saddle *)r_click_featr;
      dom=bs.peak1;
      filterModifyState(FT_BASINSADL,tree.index(bs),*r_click_featr,tree_sadl_select);
    }
    break;
  }

  Domain::Index n=tree.n_dom();
  bool *peak_visited;
  if (bs_xing) {
    peak_visited=new bool[n];
    for (i=0; i<n; ++i) peak_visited[i]=false;
  }
  else peak_visited=0;

  if (r_click_type==FT_BASINSADL && tree_filter_rel!=0) {
    MessageBox("Basin saddles don\'t have ancestors or descendants.","Winprom");
  }
  else if (tree_filter_rel==2) { // ancestors
    if (r_click_type==FT_SADDLE)
      dom=dom->parent.child->get_parent(topo);
    while (tree_peak_xing.test(dom->peak,data)) {
      i=tree.index(dom);
      filterModifyState(FT_PEAK,i,dom->peak,tree_peak_select);
      par_dom=dom->get_parent(topo);
      if (!par_dom) break;
      sadl_dom=dom->parent.saddle;
      if (!tree_sadl_xing.test(sadl_dom->saddle,data)) break;
      filterModifyState(FT_SADDLE,tree.index(sadl_dom),
			sadl_dom->saddle,tree_sadl_select);
      dom=par_dom;
    }
  }
  else if (r_click_type==FT_RUNOFF && tree_filter_rel==1) {
    MessageBox("Runoffs don\'t have any descendants.","Winprom");
  }
  else {
    switch (r_click_type) {
    case FT_PEAK: case FT_RUNOFF:
      select_subtree(*dom,0,peak_visited,tree_filter_rel==0?0:dom,topo);
      break;
    case FT_SADDLE:
      par_dom=tree_filter_rel==0?0:dom->parent.child;
      select_subtree(*dom,dom->primary_nbr,peak_visited,par_dom,topo);
      select_subtree(*dom->primary_nbr,dom,peak_visited,par_dom,topo);
      break;
    case FT_BASINSADL:
      {
	const Basin_saddle& bs=*(Basin_saddle *)r_click_featr;
	select_subtree(*bs.peak1,0,peak_visited,0,topo);
	select_subtree(*bs.peak2,0,peak_visited,0,topo);
      }
      break;
    }
  }

  delete[] peak_visited;

  Invalidate();
}

void CGraphView::OnUpdateViewGraphKind(CCmdUI *pCmdUI)
{
  // Disable item (this will be called if it's not the right kind of graph view)
  if (pCmdUI->m_pSubMenu) {
    pCmdUI->m_pMenu->EnableMenuItem(pCmdUI->m_nIndex,MF_BYPOSITION|MF_GRAYED);
  }
  CString title;
  pCmdUI->m_pMenu->GetMenuString(pCmdUI->m_nIndex,title,MF_BYPOSITION);
}

void CGraphView::OnEditRQF() 
{
  FT_index ft;
  Domain::Index i;
  const Feature *featr=&FindSelectedFeature(i,ft);
  CRQFinfo_dlg(GetDocument(),featr,ft).DoModal();
}

void CGraphView::OnUpdateEditRQF(CCmdUI *pCmdUI)
{
  FT_index ft;
  Domain::Index i;
  CPromDoc *doc=GetDocument();
  pCmdUI->Enable(visibleSelect()==1 && doc->em_state!=NOT_LOADED &&
		 doc->elev.contains(FindSelectedFeature(i,ft).location));
}

void CGraphView::OnViewRidgeOptions()
{
  CRidgeView_dlg dlg(local_topo());
  CPromDoc *doc=GetDocument();

  dlg.m_peak=current.peak_ridge;
  dlg.m_saddle=current.saddle_ridge;
  dlg.m_runoff=current.runoff_ridge;
  dlg.m_basin_sadl=current.bsnsadl_ridge;
  dlg.m_ridge_rot=ridge_rr;
  dlg.m_topology=topology;
  dlg.m_color=current.color_ridge;
  dlg.major=ridge_major;
  dlg.standard=ridge_std;
  dlg.minor=ridge_minor;

  if (dlg.DoModal()!=IDOK) return;

  current.peak_ridge=dlg.m_peak!=FALSE;
  current.saddle_ridge=dlg.m_saddle!=FALSE;
  current.runoff_ridge=dlg.m_runoff!=FALSE;
  current.bsnsadl_ridge=dlg.m_basin_sadl!=FALSE;
  ridge_rr=dlg.m_ridge_rot!=FALSE;
  topology=(Domain::Topology)dlg.m_topology;
  current.color_ridge=dlg.m_color;
  ridge_major=dlg.major;
  ridge_std=dlg.standard;
  ridge_minor=dlg.minor;

  Invalidate();
}

void CGraphView::OnViewFeaturesOptions()
{
  CFeatureView_dlg dlg;
  CPromDoc *doc=GetDocument();

  dlg.m_peak=current.view_peak;
  dlg.m_saddle=current.view_saddle;
  dlg.m_runoff=current.view_runoff;
  dlg.m_basin_sadl=current.view_bsnsadl;
  dlg.m_elev_cap=peak_elev_cap;
  dlg.m_offmap=offmap_prom;
  dlg.m_ridge_rot=ftr_rr;
  dlg.m_cell_rot=ftr_cr;
  dlg.major=ftr_large;
  dlg.standard=ftr_medium;
  dlg.minor=ftr_small;
  dlg.label=ftr_label;

  if (dlg.DoModal()!=IDOK) return;

  current.view_peak=dlg.m_peak!=FALSE;
  current.view_saddle=dlg.m_saddle!=FALSE;
  current.view_runoff=dlg.m_runoff!=FALSE;
  current.view_bsnsadl=dlg.m_basin_sadl!=FALSE;
  peak_elev_cap=dlg.m_elev_cap!=FALSE;
  offmap_prom=dlg.m_offmap!=FALSE;
  ftr_rr=dlg.m_ridge_rot!=FALSE;
  ftr_cr=dlg.m_cell_rot!=FALSE;
  ftr_large=dlg.major;
  ftr_medium=dlg.standard;
  ftr_small=dlg.minor;
  ftr_label=dlg.label;

  translateStates(LS_SELECTED,LS_VISIBLE,0); // clear selection
  Invalidate();
}

void CGraphView::OnListParents()
{
  Divide_tree& tree=GetDocument()->tree;
  CParentListView::List_info pli;
  int selItem=-1;
  Domain::Index ndom=tree.n_dom(),idom;
  for (idom=0; idom<ndom; ++idom) {
    if (label_state[FT_SADDLE][idom]==LS_SELECTED) {
      pli.sadls.push_back(&tree[idom].saddle);
    }
    if (label_state[FT_PEAK][idom]==LS_SELECTED)
      pli.peak=&tree[idom];
  }
  CParentListView::init_list(GetDocument(),pli,GetParent());
}

void CGraphView::OnUpdateListParents(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(nsel_feature[FT_PEAK]==1 &&
		 nsel_feature[FT_RUNOFF]==0 &&
		 nsel_feature[FT_BASINSADL]==0);
}
