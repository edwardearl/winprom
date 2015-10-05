// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// ProfileView.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "ProfileView.h"
#include "PromFrame.h"
#include "ProfileFrame.h"
#include "SetProfile_dlg.h"
#include "ProfileParam_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CProfileView

IMPLEMENT_DYNCREATE(CProfileView, CGraphView)

CProfileView::Settings CProfileView::initial;

CProfileView::CProfileView()
{
  current=initial;
  prom_range.low=current.min_prom;
  PromFunc_min=PromScaleFunc(prom_range.low);
  topology=Domain::TOPO_CELL;
}

CProfileView::~CProfileView()
{
}

BEGIN_MESSAGE_MAP(CProfileView, CGraphView)
  //{{AFX_MSG_MAP(CProfileView)
  ON_COMMAND(ID_VIEW_PROFILE_ZOOM_NARROWER, OnViewProfileZoomNarrower)
  ON_COMMAND(ID_VIEW_PROFILE_ZOOM_SHORTER, OnViewProfileZoomShorter)
  ON_COMMAND(ID_VIEW_PROFILE_ZOOM_TALLER, OnViewProfileZoomTaller)
  ON_COMMAND(ID_VIEW_PROFILE_ZOOM_WIDER, OnViewProfileZoomWider)
  ON_WM_MOUSEMOVE()
  ON_COMMAND(ID_VIEW_PROFILE_PARAMS, OnViewProfileParams)
  ON_UPDATE_COMMAND_UI(ID_VIEW_PROFILE_ZOOM_TALLER, OnUpdateViewProfileZoomTaller)
  ON_UPDATE_COMMAND_UI(ID_VIEW_PROFILE_ZOOM_WIDER, OnUpdateViewProfileZoomWider)
  ON_WM_CREATE()
  ON_WM_HSCROLL()
  ON_COMMAND(ID_VIEW_PROFILE_SET, OnViewProfileSet)
  ON_WM_KEYDOWN()
  ON_UPDATE_COMMAND_UI(ID_VIEW_PROFILE_SET, OnUpdateViewProfileSet)
  ON_COMMAND(ID_VIEW_SET, OnViewSet)
  ON_COMMAND(ID_VIEW_APPLY, OnViewApply)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


CProfileView::Settings::Settings() :
  elev_scale(0.1),prom_scale(500000),min_prom(500),
  scale_type(ST_INVERSE),sadl_pos(0),
  ue_runoff(CPromDoc::UE_MIDPT)
{
}

static double get_psm(CProfileView::Scale_type scale_type)
{
  switch (scale_type) {
  case CProfileView::ST_LINEAR: return 10000;
  case CProfileView::ST_LOG: return 100;
  case CProfileView::ST_INVERSE: return 1;
  case CProfileView::ST_INVSQ: return 1;
  }
  assert(0);
  return 0;
}

void CProfileView::Settings::load_registry()
{
  Settings factory;
  CWinApp *app=AfxGetApp();
  scale_type=(Scale_type)app->GetProfileInt("Profile","Scale type",factory.scale_type);
  elev_scale=app->GetProfileInt("Profile","Elev scale",factory.elev_scale*1000)/1000.0;
  prom_scale=app->GetProfileInt("Profile","Prom scale",factory.prom_scale)/
    get_psm(scale_type);
  min_prom=app->GetProfileInt("Profile","Min prom",factory.min_prom);
  sadl_pos=app->GetProfileInt("Profile","Saddle prom pos",factory.sadl_pos);
  ue_runoff=(CPromDoc::Use_elev)app->GetProfileInt("Profile","Runoff elev",factory.ue_runoff);
}

void CProfileView::Settings::save_registry() const
{
  CWinApp *app=AfxGetApp();
  app->WriteProfileInt("Profile","Scale type",scale_type);
  app->WriteProfileInt("Profile","Elev scale",elev_scale*1000);
  app->WriteProfileInt("Profile","Prom scale",prom_scale*get_psm(scale_type));
  app->WriteProfileInt("Profile","Min prom",min_prom);
  app->WriteProfileInt("Profile","Saddle prom pos",sadl_pos);
  app->WriteProfileInt("Profile","Runoff elev",ue_runoff);
}


/////////////////////////////////////////////////////////////////////////////
// CProfileView drawing

// nearest integer (helper function)
inline static Elevation elev_nint(double x)
{
  return (Elevation)(x + (x>0 ? 0.5 : -0.5));
}

Elevation CProfileView::GetProm(const Domain& dom) const
{
  CPromDoc *doc=GetDocument();
  bool is_drg=GetDocument()->is_drainage;
  Elevation sadl_elev = dom.parent.cell ?
    dom.parent.saddle->saddle.elev.*doc->inner_endpt() :
    elev_range.*doc->inner_endpt();
  return doc->drain_const()*(dom.peak.elev.*doc->outer_endpt()-sadl_elev);
}

double CProfileView::PromScaleFunc(Elevation prom) const
{
  switch (current.scale_type) {
  case ST_LINEAR: return prom;
  case ST_LOG: return log((float)prom);
  case ST_INVERSE: return -1.0/prom;
  case ST_INVSQ: return -1.0/(prom*prom);
  }
  assert(0);
  return 0;
}

Elevation CProfileView::PromScaleInvFunc(double prom_func) const
{
  switch (current.scale_type) {
  case ST_LINEAR: return elev_nint(prom_func);
  case ST_LOG: return elev_nint(exp(prom_func));
  case ST_INVERSE: return elev_nint(-1.0/prom_func);
  case ST_INVSQ: return elev_nint(sqrt(-1.0/prom_func));
  }
  assert(0);
  return 0;
}

int CProfileView::PromCoord(Elevation prom) const
{
  return prom<prom_range.low ? POINT_UNDEF :
    (int)((PromScaleFunc(prom)-PromFunc_min)*current.prom_scale+0.5)+GRAPH_VIEW_MARGIN;
}

Elevation CProfileView::PromFromCoord(int prom_pos) const
{
  return PromScaleInvFunc(PromFunc_min+(prom_pos-GRAPH_VIEW_MARGIN)/current.prom_scale);
}

Elevation CProfileView::ElevFromCoord(int elev_pos) const
{
  return (Elevation)(elev_range.high+(GRAPH_VIEW_MARGIN-elev_pos)/current.elev_scale);
}

bool CProfileView::posHasProm(int prom_pos) const
{
  return prom_pos>=GRAPH_VIEW_MARGIN &&
    prom_pos<GetTotalSize().cx-GRAPH_VIEW_MARGIN;
}

int CProfileView::saddle_EA_coord(Elevation se, Domain *peak1, Domain *peak2) const
{
  int coord1=PromCoord(GetProm(*peak1)),coord2=PromCoord(GetProm(*peak2));
  if (coord1>=0 && coord2>=0) {
    Elevation rise1=peak1->peak.elev.high-se,
      rise2=peak2->peak.elev.high-se;
    if (rise1<0) rise1*=-1;
    if (rise2<0) rise2*=-1;
    return (rise1*coord2+rise2*coord1)/(rise1+rise2);
  }

  return POINT_UNDEF;
}

CPoint CProfileView::GetFeaturePoint(const Feature& f, FT_index type)
{
  Elevation elev;
  int prom_coord;
  Domain *dom1,*dom2;
  switch (type) {
  case FT_PEAK:
    elev=f.elev.high;
    prom_coord=PromCoord(GetProm(*Domain::FromPeak(&f)));
    break;
  case FT_SADDLE:
    elev=f.elev.low;
    if (current.sadl_pos==0) {
      dom1=Domain::FromSadl(&f)->parent.child;
      dom2=dom1->get_parent(topology,prom_range.low);
      prom_coord=saddle_EA_coord(elev,dom1,dom2);
    }
    else {
      prom_coord=PromCoord(GetProm(*Domain::FromSadl(&f)->parent.child));
    }
    break;
  case FT_BASINSADL:
    elev=f.elev.low;
    {
      const Basin_saddle& bs=(Basin_saddle&)f;
      if (current.sadl_pos==0) {
	if (bs.prom.onmap>=prom_range.low) {
	  dom1=bs.peak1->parent_with_prom(prom_range.low);
	  dom2=bs.peak2->parent_with_prom(prom_range.low);
	  prom_coord=saddle_EA_coord(elev,dom1,dom2);
	}
	else prom_coord=POINT_UNDEF;
      }
      else {
	prom_coord=PromCoord(bs.prom.onmap);
      }
    }
    break;
  case FT_RUNOFF:
    if (((Runoff&)f).peak!=0 && GetProm(*((Runoff&)f).peak)>=prom_range.low) {
      elev=CPromDoc::getUseElev(f.elev,current.ue_runoff);
      prom_coord=PromCoord(((Runoff&)f).peak->peak.elev.high-elev);
    }
    else prom_coord=POINT_UNDEF;
    // TODO: runoff lower than saddle?
    break;
  default:
    assert(0);
  }
  return CPoint(prom_coord,ElevCoord(elev));
}

void CProfileView::setProfileSize()
{
  CSize sizeTotal;
  sizeTotal.cx=
    (int)((PromScaleFunc(prom_range.high)-PromScaleFunc(prom_range.low))*current.prom_scale)+
      2*GRAPH_VIEW_MARGIN;
  sizeTotal.cy=
    (int)((elev_range.high-elev_range.low)*current.elev_scale)+
      2*GRAPH_VIEW_MARGIN;
  setGraphSizes(sizeTotal);
  prom_ruler->Invalidate();
}

void CProfileView::setProfileRange()
{
  CPromDoc *document=GetDocument();
  if (document->dt_state!=NOT_LOADED) {
    Divide_tree& tree=document->tree;
    Domain::Index i,ndom=tree.n_dom(),nrunoff=tree.n_runoff();
    Elevation elev,prom;
    elev_range.empty();
    prom_range.high=prom_range.low;
    for (i=0; i<ndom; ++i) {
      if (tree[i].peak.location) {
	elev_range|=tree[i].peak.elev.high;
	prom=GetProm(tree[i]);
	if (prom>prom_range.high) prom_range.high=prom;
      }
      if (tree[i].primary_nbr) {
	elev_range|=tree[i].saddle.elev.low;
      }
    }
    for (i=0; i<nrunoff; ++i) {
      if (tree.runoff(i).location && tree.runoff(i).peak!=0 &&
	  GetProm(*tree.runoff(i).peak)>=prom_range.low) {
	elev=CPromDoc::getUseElev(tree.runoff(i).elev,current.ue_runoff);
	elev_range|=elev;
	prom=tree.runoff(i).peak->peak.elev.high-elev;
	if (prom>prom_range.high) prom_range.high=prom;
      }
    }
    // recalculate patriarch prominence since the lowest elevation
    // probably changed
    for (i=0; i<ndom; ++i) {
      if (tree[i].peak.location && tree[i].parent.line==0) {
	prom=GetProm(tree[i]);
	if (prom>prom_range.high) prom_range.high=prom;
      }
    }
    setProfileSize();
  }
  else setGraphSizes();
}

void CProfileView::resize(double zf)
{
  current.elev_scale*=zf;
  current.prom_scale*=zf;
  setProfileSize();
}

void CProfileView::OnDraw(CDC *pDC)
{
  CGraphView::OnDraw(pDC);
}


/////////////////////////////////////////////////////////////////////////////
// CProfileView diagnostics

#ifdef _DEBUG
void CProfileView::AssertValid() const
{
  CGraphView::AssertValid();
}

void CProfileView::Dump(CDumpContext& dc) const
{
  CGraphView::Dump(dc);
}
#endif //_DEBUG

void CProfileView::SetViewTitle()
{
  CPromDoc *document=GetDocument();
  CWnd *parent=GetParent();
  if (!document->dt_name.IsEmpty()) {
    parent->SetWindowText(document->dt_name+
      (document->is_drainage ? " - Drainage Tree Profile" : " - Divide Tree Profile"));
    parent->SetIcon(AfxGetApp()->LoadIcon(IDR_GRAPHTYPE),TRUE);
  }
  else if (CProfileView::divideTreeVisible()) {
    parent->SetWindowText(document->is_drainage ?
      "Drainage Tree Profile" : "Divide Tree Profile");
    parent->SetIcon(AfxGetApp()->LoadIcon(IDR_GRAPHTYPE),TRUE);
  }
  else parent->SetWindowText("Profile View");
}


/////////////////////////////////////////////////////////////////////////////
// CProfileView message handlers

int CProfileView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CGraphView::OnCreate(lpCreateStruct) == -1)
    return -1;

  prom_ruler=&((CProfileFrame *)GetParent())->prom_ruler;
  prom_ruler->view=this;

  return 0;
}

void CProfileView::OnUpdate(CView *pSender, LPARAM lHint, CObject* pHint) 
{
  Feature *changed_feature=(Feature *)lHint;

  if (changed_feature==0 || changed_feature==&CPromDoc::dummyModifyAll ||
      changed_feature==&CPromDoc::dummyModifyTrans ||
      changed_feature==&CPromDoc::dummyModifyBounds ||
      changed_feature==&CPromDoc::dummyModifyTopo ||
      changed_feature==&CPromDoc::dummyModifySplice)
    setProfileRange();
  else if (changed_feature==&CPromDoc::dummyModifyMap ||
	   changed_feature==&CPromDoc::dummySaveCenter) {
    // do nothing; CProfileView doesn't know what a map is
  }
  else if (changed_feature==&CPromDoc::dummyModifyData ||
	   changed_feature==&CPromDoc::dummyModifyRObase ||
	   changed_feature==&CPromDoc::dummyModifyBSbase) {
    // let the base class do it
  }
  else {
    Elevation elev,prom;
    CPromDoc *doc=GetDocument();
    switch (doc->changed_type) {
    case FT_PEAK:
      elev=changed_feature->elev.high;
      prom=GetProm(*Domain::FromPeak(changed_feature));
      break;
    case FT_SADDLE:
      elev=changed_feature->elev.low;
      prom=Domain::FromSadl(changed_feature)->parent.child->peak.elev.high-elev;
    case FT_RUNOFF:
      elev=CPromDoc::getUseElev(changed_feature->elev,current.ue_runoff);
      prom=((Runoff *)changed_feature)->peak->peak.elev.high-elev;
      break;
    case FT_BASINSADL:
      prom=-1;
      break;
    default: assert(0);
    }
    if (prom>=0) elev_range|=elev;
    if (prom>prom_range.high) prom_range.high=prom;
    setProfileSize();
  }
  CGraphView::OnUpdate(pSender,lHint,pHint);
}

void CProfileView::OnMouseMove(UINT nFlags, CPoint point) 
{
  CGraphView::OnMouseMove(nFlags,point);

  if (!CProfileView::viewable()) return;
  point+=GetScrollPosition();
  CStatusBar& sb=((CPromFrame *)AfxGetMainWnd())->statusBar;

  if (posHasProm(point.x)) {
    sprintf(buf,"Prominence = %3d",PromFromCoord(point.x));
    sb.SetPaneText(CPromFrame::SB_PANE_LOC,buf);
  }
  else sb.SetPaneText(CPromFrame::SB_PANE_LOC,"");

  sprintf(buf,Elev_intvl::elem_format,ElevFromCoord(point.y));
  sb.SetPaneText(CPromFrame::SB_PANE_ELEV,buf);
}

void CProfileView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
  prom_ruler->Invalidate();
  CGraphView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CProfileView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  prom_ruler->Invalidate();
  CGraphView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CProfileView::setSizeIndicators()
{
  CPromDoc *doc=GetDocument();
  CStatusBar& sb=((CPromFrame *)AfxGetMainWnd())->statusBar;
  if (CProfileView::viewable()) {
    CSize winSize=getWinSize();
    sprintf(buf,Elev_intvl::elem_format,(Elevation)(winSize.cy/current.elev_scale));
    sb.SetPaneText(CPromFrame::SB_PANE_HT,buf);
  }
  else {
    sb.SetPaneText(CPromFrame::SB_PANE_HT,"");
  }
  sb.SetPaneText(CPromFrame::SB_PANE_WD,"");
}

void CProfileView::hzoom(double zf)
{
  CPoint scrollPos=getGraphScroll();
  scrollPos.x-=GRAPH_VIEW_MARGIN;
  scrollPos.x=(long)(scrollPos.x*zf);
  scrollPos.x+=GRAPH_VIEW_MARGIN;
  current.prom_scale*=zf;
  setProfileSize();
  setGraphScroll(scrollPos);
  Invalidate();
}

void CProfileView::vzoom(double zf)
{
  CPoint scrollPos=getGraphScroll();
  scrollPos.y-=GRAPH_VIEW_MARGIN;
  scrollPos.y=(long)(scrollPos.y*zf);
  scrollPos.y+=GRAPH_VIEW_MARGIN;
  current.elev_scale*=zf;
  setProfileSize();
  setGraphScroll(scrollPos);
  Invalidate();
}

void CProfileView::OnViewProfileZoomNarrower()
{
  hzoom(1/zoom_factor);
}

void CProfileView::OnViewProfileZoomShorter()
{
  vzoom(1/zoom_factor);
}

void CProfileView::OnViewProfileZoomTaller()
{
  vzoom(zoom_factor);
}

void CProfileView::OnViewProfileZoomWider()
{
  hzoom(zoom_factor);
}

void CProfileView::OnUpdateViewProfileZoomTaller(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetTotalSize().cy*zoom_factor<=GRAPH_MAX_DIM &&
		 CProfileView::viewable());
}

void CProfileView::OnUpdateViewProfileZoomWider(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetTotalSize().cx*zoom_factor<=GRAPH_MAX_DIM &&
		 CProfileView::viewable());
}

void CProfileView::OnViewProfileParams()
{
  CProfileParam_dlg dlg(this);
  if (dlg.DoModal()!=IDOK) return;
  PromFunc_min=PromScaleFunc(prom_range.low);
  current.prom_scale=(GetTotalSize().cx-2*GRAPH_VIEW_MARGIN)/
    (PromScaleFunc(prom_range.high)-PromFunc_min);
  prom_ruler->tickWindow=current.scale_type==ST_LINEAR?16:48;
  Invalidate();
  prom_ruler->Invalidate();
}

void CProfileView::OnViewProfileSet()
{
  CSetProfile_dlg dlg;
  CPoint center=getGraphScroll();
  dlg.m_elev=ElevFromCoord(center.y);
  dlg.m_prom=PromFromCoord(center.x);
  if (dlg.DoModal()==IDOK) {
    setGraphScroll(CPoint(PromCoord(dlg.m_prom),ElevCoord(dlg.m_elev)));
  }
}

void CProfileView::OnUpdateViewProfileSet(CCmdUI *pCmdUI)
{
  // do nothing; this handler overrides the CGraphView default handler
  // that disables the popup menu title
}

void CProfileView::OnViewSet()
{
  CGraphView::set_initial_settings();
  initial=current;
}

void CProfileView::OnViewApply()
{
  CPoint scrollPos=getGraphScroll();
  Elevation elev=ElevFromCoord(scrollPos.y),prom=PromFromCoord(scrollPos.x);
  CGraphView::get_initial_settings();
  current=initial;
  setProfileSize();
  setGraphScroll(CPoint(PromCoord(prom),ElevCoord(elev)));
  Invalidate();
}
