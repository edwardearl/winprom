// MapView.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "MapView.h"
#include "GridView_dlg.h"
#include "Contour_dlg.h"
#include "DomainView_dlg.h"
#include "PromCutoff_dlg.h"
#include "SetLocation_dlg.h"
#include "ElevRect_dlg.h"
#include "GridPointFilter_dlg.h"
#include "Imparam_dlg.h"
#include "ImpOut_dlg.h"
#include "FreeHeightParam_dlg.h"
#include "FreeHeight_dlg.h"
#include "PrintMap_dlg.h"
#include "Relief_dlg.h"
#include "RQFinfo_dlg.h"
#include "PromFrame.h"
#include "LocationFmt_dlg.h"
#include "region.h"
#include "Scale_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GV_POINT_WIDTH 6
#define GV_ELEV_WIDTH 24

// need this for map mark region iterator
class GridSet_mark {
  const CPromDoc& document;
public:
  typedef GridPoint GridPoint;
  GridSet_mark(const CPromDoc& doc) : document(doc) {}
  bool contains(const GridPoint&) const;
};

bool GridSet_mark::contains(const GridPoint& gp) const
{
  return
    (document.em_state!=NOT_LOADED && document.elev.contains(gp) &&
     document.elev[gp]!=elev_undef) ||
    (document.dm_state!=NOT_LOADED && document.domain.contains(gp) &&
     document.domain[gp]!=Domain::undef);
}

typedef Grid_tpl<GridSet_mark,CPromDoc,RawNbr_iter> Grid_mark;

// descriptor of grid point filtered region
class Filter_region {
protected:
  const GPfilter& filter;
  Bool_map& mark_map;
  bool mark;
public:
  Filter_region(const GPfilter& f, Bool_map& ma, bool m)
    : filter(f),mark_map(ma),mark(m) {}
  bool path(const RawNbr_iter&) const {return true;}
  bool region(const GridPoint& c) const {return filter.test(c);}
  bool visited(const GridPoint& c) const {return mark_map[c]==mark;}
  bool step(const RawNbr_iter& ni) const
    {return filter.test(*ni) && mark_map[*ni]!=mark;}
  void visit(const GridPoint& c) const {mark_map[c]=mark;}
  void clear() const {}
};

#define BAD_COLOR 0xFFFFFFFF


/////////////////////////////////////////////////////////////////////////////
// CMapView

IMPLEMENT_DYNCREATE(CMapView, CGraphView)

CMapView::CMapView()
{
  current=initial;
  global_offset.x=global_offset.y=GRAPH_VIEW_MARGIN;
  view_em=view_dm=view_dt=view_mark=true;
  grid_view=GV_NONE;
  cell_major=elev_infty;
  cell_std=cell_minor=0;
  cell_cr=false;
  cell_edge_sadl=true;
  cell_utd=false;
  ncell=0;
  cells=0;
}

CMapView::~CMapView()
{
  delete[] cells;
}


BEGIN_MESSAGE_MAP(CMapView, CGraphView)
  //{{AFX_MSG_MAP(CMapView)
  ON_COMMAND(ID_VIEW_MAP_DLPRTREE, OnViewMapDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_VIEW_MAP_DLPRTREE, OnUpdateViewMapDLPRtree)
  ON_WM_MOUSEMOVE()
  ON_COMMAND(ID_VIEW_MAP_CONTOURS, OnViewMapContours)
  ON_COMMAND(ID_VIEW_MAP_SET, OnViewMapSet)
  ON_COMMAND(ID_GP_CENTER, OnGpCenter)
  ON_COMMAND(ID_VIEW_MAP_MARK_CLEAR, OnViewMapMarkClear)
  ON_COMMAND(ID_VIEW_MAP_MARK_CLEAR_ALL, OnViewMapMarkClearAll)
  ON_COMMAND(ID_VIEW_MAP_MARK_PRECISE, OnViewMapMarkPrecise)
  ON_COMMAND(ID_VIEW_MAP_MARK_SET, OnViewMapMarkSet)
  ON_UPDATE_COMMAND_UI(ID_VIEW_MAP_MARK_CLEAR, OnUpdateViewMapMark)
  ON_COMMAND(ID_GP_MARKAREA, OnGpMarkArea)
  ON_COMMAND(ID_VIEW_MAP_MARK_VIEW, OnViewMapMarkView)
  ON_UPDATE_COMMAND_UI(ID_VIEW_MAP_MARK_VIEW, OnUpdateViewMapMarkView)
  ON_COMMAND(ID_GP_DLPR_PEAK, OnIdentifyDLPRpeak)
  ON_WM_ERASEBKGND()
  ON_COMMAND(ID_VIEW_COLORS_MARK, OnViewColorsMark)
  ON_COMMAND(ID_VIEW_SCROLL_CENTER, OnViewScrollCenter)
  ON_UPDATE_COMMAND_UI(ID_VIEW_SCROLL_CENTER, OnUpdateViewScrollCenter)
  ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_IN, OnUpdateViewable)
  ON_COMMAND(ID_IMPRESS, OnImpress)
  ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
  ON_COMMAND(ID_VIEW_MAP_RELIEF, OnViewMapRelief)
  ON_WM_CREATE()
  ON_COMMAND(ID_OP_DLPR_CHECK, OnOpDLPRcheck)
  ON_COMMAND(ID_GP_BROWSE, OnBrowse)
  ON_COMMAND(ID_GP_RQF, OnGP_RQF)
  ON_COMMAND(ID_VIEW_LABEL_STATUS, OnViewLabelStatus)
  ON_COMMAND(ID_GP_MARK_POINT, OnGpMarkPoint)
  ON_COMMAND(ID_VIEW_SET, OnViewSet)
  ON_COMMAND(ID_OPEN_MARK_MAP, OnOpenMarkMap)
  ON_UPDATE_COMMAND_UI(ID_OPEN_MARK_MAP, OnUpdateHaveMarkMap)
  ON_COMMAND(ID_SAVEAS_MARK_MAP, OnSaveMarkMap)
  ON_COMMAND(ID_VIEW_MAP_DOMAINS, OnViewMapDomains)
  ON_UPDATE_COMMAND_UI(ID_VIEW_MAP_DOMAINS, OnUpdateViewMapDomains)
  ON_UPDATE_COMMAND_UI(ID_VIEW_MAP_CONTOURS, OnUpdateViewMapContours)
  ON_UPDATE_COMMAND_UI(ID_VIEW_MAP_RELIEF, OnUpdateViewMapRelief)
  ON_COMMAND(ID_VIEW_MAP_GRID, OnViewMapGrid)
  ON_COMMAND(ID_VIEW_APPLY, OnViewApply)
  ON_COMMAND(ID_VIEW_MAP_CUTOFFS, OnViewMapCutoffs)
  ON_COMMAND(ID_GP_NEW_PEAK, OnGpNewPeak)
  ON_COMMAND(ID_GP_NEW_SADDLE, OnGpNewSaddle)
  ON_COMMAND(ID_VIEW_ZOOM_SCALE, OnViewZoomScale)
  ON_UPDATE_COMMAND_UI(ID_VIEW_MAP_MARK_CLEAR_ALL, OnUpdateViewMapMark)
  ON_UPDATE_COMMAND_UI(ID_VIEW_MAP_MARK_PRECISE, OnUpdateViewMapMark)
  ON_UPDATE_COMMAND_UI(ID_VIEW_MAP_MARK_SET, OnUpdateViewMapMark)
  ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_WAYIN, OnUpdateViewable)
  ON_UPDATE_COMMAND_UI(ID_VIEW_MAP_SET, OnUpdateViewable)
  ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrint)
  ON_UPDATE_COMMAND_UI(ID_SAVEAS_MARK_MAP, OnUpdateHaveMarkMap)
  ON_COMMAND(ID_FREE_HEIGHT, OnFreeHeight)
  //}}AFX_MSG_MAP
  ON_COMMAND(ID_GP_NEW_RUNOFF, OnGpNewRunoff)
END_MESSAGE_MAP()


CMapView::Settings CMapView::initial;

CMapView::Settings::Settings() :
  color_contour(RGB(160,255,160)),color_elev_void(RGB(0,255,255)),
  color_domain(RGB(255,0,0)),color_dom_void(RGB(255,128,0)),
  color_grid(RGB(255,0,255)),color_mark(RGB(0,0,128)),
  scale(50),
  contour_major(5),contour_std(5),contour_minor(80),contour_datum(0),
  autozoom(true),GPontop(true),
  light_int(0),light_dir(315),elev_color_enable(false)
{
}

void CMapView::Settings::load_registry()
{
  Settings factory;
  CWinApp *app=AfxGetApp();
  color_contour=app->GetProfileInt("Color","Contour",factory.color_contour);
  color_elev_void=app->GetProfileInt("Color","Defined elevation",factory.color_elev_void);
  color_grid=app->GetProfileInt("Color","Grid",factory.color_grid);
  color_domain=app->GetProfileInt("Color","Domain",factory.color_domain);
  color_dom_void=app->GetProfileInt("Color","Defined domain",factory.color_dom_void);
  color_mark=app->GetProfileInt("Color","Mark",factory.color_mark);
  scale=app->GetProfileInt("Scale","Scale",factory.scale*1000)/1000.0;
  contour_major=app->GetProfileInt("Contour","Major",factory.contour_major);
  contour_std=app->GetProfileInt("Contour","Standard",factory.contour_std);
  contour_minor=app->GetProfileInt("Contour","Minor",factory.contour_minor);
  contour_datum=app->GetProfileInt("Contour","Datum",factory.contour_datum);
  autozoom=app->GetProfileInt("Contour","Autozoom",factory.autozoom)!=0;
  GPontop=app->GetProfileInt("Contour","GP_on_top",factory.GPontop)!=0;
  light_int=app->GetProfileInt("Relief","Light",factory.light_int*1000000)*0.000001;
  light_dir=app->GetProfileInt("Relief","Direction",factory.light_dir);
  elev_color_enable=app->GetProfileInt("Relief","Enable color",factory.elev_color_enable)!=0;

  int palette_size=app->GetProfileInt("Palette","size",0),i,b;
  Elev_color_intv eci;
  for (i=0; i<palette_size; ++i) {
    sprintf(CPromDoc::buf,"Palette_%d",i);
    eci.elev=app->GetProfileInt(CPromDoc::buf,"Elevation",elev_undef);
    if (eci.elev==elev_undef ||
	(i>0 && eci.elev<=elev_colors.back().elev))
      break;
    eci.color=app->GetProfileInt(CPromDoc::buf,"Color",BAD_COLOR);
    if (eci.color==BAD_COLOR) break;
    b=app->GetProfileInt(CPromDoc::buf,"Interp",2);
    if (b!=0 && b!=1) break;
    eci.interp=b!=0;
    elev_colors.push_back(eci);
  }
}

void CMapView::Settings::save_registry() const
{
  CWinApp *app=AfxGetApp();
  app->WriteProfileInt("Color","Contour",color_contour);
  app->WriteProfileInt("Color","Defined elevation",color_elev_void);
  app->WriteProfileInt("Color","Grid",color_grid);
  app->WriteProfileInt("Color","Domain",color_domain);
  app->WriteProfileInt("Color","Defined domain",color_dom_void);
  app->WriteProfileInt("Color","Mark",color_mark);
  app->WriteProfileInt("Scale","Scale",scale*1000);
  app->WriteProfileInt("Contour","Major",contour_major);
  app->WriteProfileInt("Contour","Standard",contour_std);
  app->WriteProfileInt("Contour","Minor",contour_minor);
  app->WriteProfileInt("Contour","Datum",contour_datum);
  app->WriteProfileInt("Contour","Autozoom",autozoom);
  app->WriteProfileInt("Contour","GP_on_top",GPontop);
  app->WriteProfileInt("Relief","Light",light_int*1000000);
  app->WriteProfileInt("Relief","Direction",light_dir);
  app->WriteProfileInt("Relief","Enable color",elev_color_enable);

  const vector<Elev_color_intv>& palette=elev_colors;
  app->WriteProfileInt("Palette","size",palette.size());
  for (unsigned i=0; i<palette.size(); ++i) {
    sprintf(CPromDoc::buf,"Palette_%d",i);
    app->WriteProfileInt(CPromDoc::buf,"Elevation",palette[i].elev);
    app->WriteProfileInt(CPromDoc::buf,"Color",palette[i].color);
    app->WriteProfileInt(CPromDoc::buf,"Interp",palette[i].interp);
  }
}


/////////////////////////////////////////////////////////////////////////////
// CMapView drawing

BOOL CMapView::OnEraseBkgnd(CDC *pDC)
{
  if (!CMapView::viewable())
    return CScrollView::OnEraseBkgnd(pDC);
  clearBG(pDC);
  return true;
}

CPoint CMapView::GetFeaturePoint(const Feature& f, FT_index t)
{
  return gp2point(f.location);
}

CPoint CMapView::gp2point(const GridPoint& c) const
{
  return CPoint((int)((c.y-scrollRect.limit[1].low)*hstep_pix),
		(int)((scrollRect.limit[0].high-c.x)*vstep_pix))
	+global_offset;
}

GridPoint CMapView::point2gp(const CPoint& point) const
{
  CPromDoc *doc=GetDocument();
  GridPoint::Coord
    v=nint((global_offset.y-point.y)/vstep_pix+scrollRect.limit[0].high),
    h=nint((point.x-global_offset.x)/hstep_pix+scrollRect.limit[1].low);
  GridPoint c;
  c.set_cart(v,h);
  return c;
}

void CMapView::setMapScale()
{
  GridPoint ref;
  ref.set_cart(scrollRect.limit[0].midpt(),scrollRect.limit[1].midpt());
  setMapScale(ref);
}

void CMapView::setMapScale(const GridPoint& ref)
{
  vstep_pix=ref.xstep()*current.scale;
  hstep_pix=ref.ystep()*current.scale;
}

void CMapView::setMapSize()
{
  CPromDoc *document=GetDocument();
  CSize sizeTotal;
  sizeTotal.cx=(long)(scrollRect.limit[1].size()*hstep_pix)+2*GRAPH_VIEW_MARGIN;
  sizeTotal.cy=(long)(scrollRect.limit[0].size()*vstep_pix)+2*GRAPH_VIEW_MARGIN;
  setGraphSizes(sizeTotal);
}

void CMapView::zoom(double zf)
{
  if (current.autozoom && GetDocument()->em_state!=NOT_LOADED && view_em) {
    if (zf>=2 && current.contour_minor>=2) current.contour_minor/=2;
    else if (zf<=0.5) current.contour_minor*=2;
  }
  current.scale*=zf;
  OnViewScrollCenter();
}

void CMapView::OnViewZoomScale()
{
  CScale_dlg dlg;
  dlg.m_scale=current.scale;
  if (dlg.DoModal()!=IDOK) return;
  current.scale=dlg.m_scale;
  OnViewScrollCenter();
}

bool CMapView::viewable()
{
  return !GetDocument()->total_alloc.rect.is_empty();
}

static int bracket(Elevation elev1, Elevation elev2, Elevation contour, bool GPontop,
		   const CPoint& pt1, const CPoint& pt2, CPoint& pierce)
{
  if (elev1==elev2) return 0;

  if (contour==elev1) {
    if ((elev1<elev2) == GPontop) return 0;
    pierce=pt1;
    return 1;
  }
  if (contour==elev2) {
    if ((elev2<elev1) == GPontop) return 0;
    pierce=pt2;
    return 1;
  }

  float prop=(contour-elev1)/(float)(elev2-elev1);
  if (prop<=0 || prop>=1) return 0;
  pierce.x=(long)(prop*(pt2.x-pt1.x)+pt1.x+0.5);
  pierce.y=(long)(prop*(pt2.y-pt1.y)+pt1.y+0.5);
  return 1;
}

static CPoint pierce[4]; // scratch for building endpoint lists

static void bracket3(CDC *pDC, Elevation elev1, Elevation elev2, Elevation elev3,
		     Elevation c_elev, bool GPontop,
		     const CPoint& pt1, const CPoint& pt2, const CPoint& pt3)
{
  int nbrac=0;
  nbrac+=bracket(elev1,elev2,c_elev,GPontop,pt1,pt2,pierce[nbrac]);
  nbrac+=bracket(elev2,elev3,c_elev,GPontop,pt2,pt3,pierce[nbrac]);
  nbrac+=bracket(elev3,elev1,c_elev,GPontop,pt3,pt1,pierce[nbrac]);
  assert(nbrac==2);
  pDC->MoveTo(pierce[0]);
  pDC->LineTo(pierce[1]);
}

void CMapView::OnDraw(CDC *pDC)
{
  CPromDoc *doc=GetDocument();
  if (doc->working || !viewable()) return;

  saveCenter();
  CRect viewRectVbl;
  pDC->GetClipBox(viewRectVbl);
  Rectangl visible(
    point2gp(CPoint(viewRectVbl.left-GRAPH_VIEW_MARGIN,
		    viewRectVbl.bottom+GRAPH_VIEW_MARGIN)),
    point2gp(CPoint(viewRectVbl.right+GRAPH_VIEW_MARGIN,
		    viewRectVbl.top-GRAPH_VIEW_MARGIN))
  );
  visible&=doc->total_alloc.rect; // just make sure things don't get out of bounds

  // draw shaded relief
  if (doc->em_state!=NOT_LOADED &&
      (current.light_int!=0 || current.elev_color_enable)) {
    const Elev_map& elev=doc->elev;
    const vector<Elev_color_intv>& elev_colors=current.elev_colors;
    Rectangl elev_rect(visible);
    elev_rect&=elev;
    GridPoint NW,NE,SE,SW;
    Elevation NW_elev,NE_elev,SE_elev,SW_elev,c_elev,e0;
    double dot;
    float p,q;
    unsigned light;
    int hi,lo,mid,rv,gv,bv,mv;
    CPoint NW_point,SE_point;
    COLORREF color,color1;
    for (Rect_iter_all gi(elev_rect); gi; ++gi) {
      // find the four corners of the cell
      NW=(*gi);
      NE.x=NW.x;
      SE.x=SW.x=NW.x-1;
      SW.y=NW.y;
      NE.y=SE.y=NW.y+1;
      if (!elev_rect.contains(SE)) continue;
      // find the elevation of each corner of the cell
      NW_elev=elev[NW];
      NE_elev=elev[NE];
      SE_elev=elev[SE];
      SW_elev=elev[SW];
      if (NW_elev!=elev_undef && NE_elev!=elev_undef &&
	  SE_elev!=elev_undef && SW_elev!=elev_undef) {
	if (current.light_int>0) {
	  // find the light level of the cell
	  dot=(light_vec_x*(SW_elev-NW_elev+SE_elev-NE_elev)+
	       light_vec_y*(NW_elev-NE_elev+SW_elev-SE_elev));
	  light=(dot/sqrt(dot*dot+1)+1)*128;
	}
	if (current.elev_color_enable) {
	  // find the color in the palette
	  c_elev=NW_elev+(SW_elev-NW_elev)/4+(NE_elev-NW_elev)/4+(SE_elev-NW_elev)/4;
	  lo=-1;
	  hi=elev_colors.size();
	  while (hi>lo+1) {
	    mid=(hi+lo)/2;
	    if (c_elev<elev_colors[mid].elev) hi=mid;
	    else lo=mid;
	  }
	  if (hi>0) {
	    // elevation is above the level where the palette starts; use it
	    color=elev_colors[hi-1].color;
	    if (hi<elev_colors.size() && elev_colors[hi-1].interp) {
	      // interpolate color to next higher interval
	      e0=elev_colors[hi-1].elev;
	      p=((float)c_elev-e0)/(elev_colors[hi].elev-e0);
	      q=1-p;
	      color1=elev_colors[hi].color;
	      color=RGB(q*GetRValue(color)+p*GetRValue(color1),
			q*GetGValue(color)+p*GetGValue(color1),
			q*GetBValue(color)+p*GetBValue(color1));
	    }
	    if (current.light_int>0) {
	      // we have both color and shading; normalize the color to the light level
	      mv=rv=GetRValue(color);
	      gv=GetGValue(color);
	      bv=GetBValue(color);
	      if (gv>mv) mv=gv;
	      if (bv>mv) mv=bv;
	      rv=light*rv/mv;
	      gv=light*gv/mv;
	      bv=light*bv/mv;
	      color=RGB(rv,gv,bv);
	    }
	  }
	}
	else hi=0;
	if (hi==0) {
	  if (current.light_int>0) color=RGB(light,light,light);
	  else continue;
	}
	NW_point=gp2point(NW);
	SE_point=gp2point(SE);
	pDC->FillSolidRect(NW_point.x,NW_point.y,SE_point.x-NW_point.x,
			   SE_point.y-NW_point.y,color);
      }
    }
  }

  // draw marked grid points
  Rectangl mark_rect(visible);
  mark_rect&=grid_mark;
  if (!mark_rect.is_empty() && view_mark) {
    int jy1,jy2;
    GridPoint gp;
    CBrush selectBrush(current.color_mark);
    pDC->SelectObject(&selectBrush);
    pDC->SelectStockObject(NULL_PEN);
    jy1=semi_gp2coord_x(mark_rect.limit[0].low);
    for (gp.x=mark_rect.limit[0].low; gp.x<=mark_rect.limit[0].high; ++gp.x) {
      jy2=semi_gp2coord_x(gp.x+1);
      for (gp.y=mark_rect.limit[1].low; gp.y<=mark_rect.limit[1].high; ++gp.y) {
	if (grid_mark[gp]) {
	  pDC->Rectangle(semi_gp2coord_y(gp.y-1),jy2,
			 semi_gp2coord_y(gp.y)+2,jy1+2);
	}
      }
      jy1=jy2;
    }
    pDC->SelectStockObject(NULL_BRUSH);
  }

  // draw contour map
  if (doc->em_state!=NOT_LOADED && view_em) {
    const Elev_map& elev=doc->elev;
    Rectangl elev_rect(visible);
    elev_rect&=elev;
    GridPoint NW,NE,SE,SW;
    Elevation NW_elev,NE_elev,SE_elev,SW_elev;
    CPoint NW_point,NE_point,SE_point,SW_point;
    COLORREF interval_color,minor_contour_color=
	((current.color_contour&0xFEFEFE)+
	 (pDC->IsPrinting()?0xFEFEFE:(get_bg_color()&0xFEFEFE)) )>>1;
    // minor contour color is halfway between regular contour and background
    CPen voidPen(PS_SOLID,0,current.color_elev_void),
      majorPen(PS_SOLID,2,current.color_contour),
      stdPen(PS_SOLID,0,current.color_contour),
      minorPen(PS_SOLID,0,minor_contour_color);
    Elev_intvl elev_range,contour_index;
    double contour_space;
    Elevation ci,ci_step,c_elev,minor_interval,saddle_elev,
      major_minor=current.contour_std*current.contour_major,
      equality=current.contour_minor+(current.GPontop?0:-1);
    unsigned short ndefined,nbrac;

    // trim down the elevation map rectangle right and bottom sides
    // since we're interested in the "cells" between grid points
    ++elev_rect.limit[0].low;
    --elev_rect.limit[1].high;

    for (Rect_iter_all gi(elev_rect); gi; ++gi) {
      NW=(*gi);
      NE.x=NW.x;
      SE.x=SW.x=NW.x-1;
      SW.y=NW.y;
      NE.y=SE.y=NW.y+1;
      NW_elev=elev[NW];
      NE_elev=elev[NE];
      SE_elev=elev[SE];
      SW_elev=elev[SW];
      ndefined=0;
      elev_range.empty();
      if (NW_elev!=elev_undef) {
	++ndefined;
	NW_point=gp2point(NW);
	NW_elev-=current.contour_datum;
	elev_range|=NW_elev;
      }
      if (NE_elev!=elev_undef) {
	++ndefined;
	NE_point=gp2point(NE);
	NE_elev-=current.contour_datum;
	elev_range|=NE_elev;
      }
      if (SE_elev!=elev_undef) {
	++ndefined;
	SE_point=gp2point(SE);
	SE_elev-=current.contour_datum;
	elev_range|=SE_elev;
      }
      if (SW_elev!=elev_undef) {
	++ndefined;
	SW_point=gp2point(SW);
	SW_elev-=current.contour_datum;
	elev_range|=SW_elev;
      }
      contour_space=current.contour_minor*sqrt(vstep_pix*vstep_pix+
	hstep_pix*hstep_pix)/elev_range.size();
      if (elev_range.low<0) elev_range.low=0;
      contour_index.low=(elev_range.low+equality)/current.contour_minor;
      contour_index.high=(elev_range.high+equality)/current.contour_minor;
      if (contour_space>=2) ci_step=1;
      else if (contour_space*current.contour_std>=2) {
	ci_step=current.contour_std;
	contour_index.low=(contour_index.low+ci_step-1)/ci_step*ci_step;
      }
      else {
	ci_step=major_minor;
	contour_index.low=(contour_index.low+ci_step-1)/ci_step*ci_step;
      }
      switch (ndefined) {
      case 0: case 1:
	break;
      case 2:
	pDC->SelectObject(&voidPen);
	if (NW_elev!=elev_undef) {
	  pDC->MoveTo(NW_point);
	  if (NE_elev!=elev_undef) {
	    pDC->LineTo(NE_point);
	    pDC->SetPixelV(NE_point,current.color_elev_void);
	  }
	  else if (SW_elev!=elev_undef) {
	    pDC->LineTo(SW_point);
	    pDC->SetPixelV(SW_point,current.color_elev_void);
	  }
	}
	if (SE_elev!=elev_undef) {
	  pDC->MoveTo(SE_point);
	  if (NE_elev!=elev_undef) {
	    pDC->LineTo(NE_point);
	    pDC->SetPixelV(NE_point,current.color_elev_void);
	  }
	  else if (SW_elev!=elev_undef) {
	    pDC->LineTo(SW_point);
	    pDC->SetPixelV(SW_point,current.color_elev_void);
	  }
	}
	break;
      case 3:
	for (ci=contour_index.low; ci<contour_index.high; ci+=ci_step) {
	  c_elev=ci*current.contour_minor;
	  if (ci%current.contour_std!=0) pDC->SelectObject(&minorPen);
	  else if (ci%major_minor!=0) pDC->SelectObject(&stdPen);
	  else pDC->SelectObject(&majorPen);
	  if (NW_elev==elev_undef)
	    bracket3(pDC,NE_elev,SE_elev,SW_elev,c_elev,current.GPontop,
		     NE_point,SE_point,SW_point);
	  else if (NE_elev==elev_undef)
	    bracket3(pDC,SE_elev,SW_elev,NW_elev,c_elev,current.GPontop,
		     SE_point,SW_point,NW_point);
	  else if (SE_elev==elev_undef)
	    bracket3(pDC,SW_elev,NW_elev,NE_elev,c_elev,current.GPontop,
		     SW_point,NW_point,NE_point);
	  else if (SW_elev==elev_undef)
	    bracket3(pDC,NW_elev,NE_elev,SE_elev,c_elev,current.GPontop,
		     NW_point,NE_point,SE_point);
	  else assert(0);
	}
	pDC->SelectObject(&voidPen);
	if (NW_elev==elev_undef || SE_elev==elev_undef) {
	  pDC->MoveTo(NE_point);
	  pDC->LineTo(SW_point);
	  pDC->SetPixelV(SW_point,current.color_elev_void);
	}
	else if (NE_elev==elev_undef || SW_elev==elev_undef) {
	  pDC->MoveTo(NW_point);
	  pDC->LineTo(SE_point);
	  pDC->SetPixelV(SE_point,current.color_elev_void);
	}
	break;
      case 4:
	for (ci=contour_index.low; ci<contour_index.high; ci+=ci_step) {
	  nbrac=0;
	  c_elev=ci*current.contour_minor;
	  minor_interval=ci%current.contour_std;
	  interval_color=(minor_interval==0)?
			  current.color_contour:minor_contour_color;
	  nbrac+=bracket(NW_elev,NE_elev,c_elev,current.GPontop,
			 NW_point,NE_point,pierce[nbrac]);
	  nbrac+=bracket(NE_elev,SE_elev,c_elev,current.GPontop,
			 NE_point,SE_point,pierce[nbrac]);
	  nbrac+=bracket(SE_elev,SW_elev,c_elev,current.GPontop,
			 SE_point,SW_point,pierce[nbrac]);
	  nbrac+=bracket(SW_elev,NW_elev,c_elev,current.GPontop,
			 SW_point,NW_point,pierce[nbrac]);
	  if (minor_interval!=0) pDC->SelectObject(&minorPen);
	  else if (ci%major_minor!=0) pDC->SelectObject(&stdPen);
	  else pDC->SelectObject(&majorPen);
	  if (nbrac==2) {
	    pDC->MoveTo(pierce[0]);
	    pDC->LineTo(pierce[1]);
	    pDC->SetPixelV(pierce[1],interval_color);
	  }
	  else if (nbrac==4) {
	    saddle_elev=NW_elev-((SW_elev-NW_elev)*(NE_elev-NW_elev))/
				 (NW_elev-NE_elev+SE_elev-SW_elev);
	    pDC->MoveTo(pierce[0]);
	    if ((NW_elev>saddle_elev)==(c_elev>saddle_elev)) {
	      pDC->LineTo(pierce[3]);
	      pDC->MoveTo(pierce[2]);
	      pDC->LineTo(pierce[1]);
	    }
	    else {
	      pDC->LineTo(pierce[1]);
	      pDC->MoveTo(pierce[2]);
	      pDC->LineTo(pierce[3]);
	    }
	    pDC->SetPixelV(pierce[1],interval_color);
	    pDC->SetPixelV(pierce[3],interval_color);
	  }
	  else assert(0);
	}
	break;
      default:
	assert(0);
      }

    }

    // be sure OnDraw() doesn't return with a local pen selected
    pDC->SelectStockObject(NULL_PEN);
  }

  // draw domain map
  if (doc->dm_state!=NOT_LOADED && view_dm) {
    update_cell();
    const Divide_tree& tree=doc->tree;
    const Index_map& domain=doc->domain;
    Domain::Index dom_ctr,dom_E,dom_S,n=tree.n_dom();
    Rectangl domain_rect(visible);
    COLORREF domap_color=doc->is_drainage?
	CGraphView::get_ridge_color():current.color_domain,
      minor_domap_color=((domap_color&0xFEFEFE)+
	(pDC->IsPrinting()?0xFEFEFE:(get_bg_color()&0xFEFEFE)) )>>1;
    // minor contour color is halfway between regular contour and background
    CPen voidPen(PS_SOLID,0,current.color_dom_void),
      majorPen(PS_SOLID,2,domap_color),
      stdPen(PS_SOLID,0,domap_color),
      minorPen(PS_SOLID,0,minor_domap_color),
      *eastPen,*southPen;
    // augment the domain map rectangle
    // since an off-window grid point can have an on-window domain boundary
    ++domain_rect.limit[0].high;
    ++domain_rect.limit[1].high;
    --domain_rect.limit[0].low;
    --domain_rect.limit[1].low;
    domain_rect&=domain;
    GridPoint center,east,south;
    CPoint SE_point;
    int n_coord,w_coord;

    for (Rect_iter_all gi(domain_rect); gi; ++gi) {
      center=(*gi);
      east.x=center.x;
      east.y=center.y+1;
      south.x=center.x-1;
      south.y=center.y;
      eastPen=southPen=0;
      dom_ctr=domain[center];
      if (center.y+1<=domain_rect.limit[1].high) {
	dom_E=domain[east];
	if ((dom_ctr==Domain::undef) != (dom_E==Domain::undef))
	  eastPen=&voidPen;
	else if (dom_ctr!=Domain::undef) {
	  if (dom_ctr>=n || dom_E>=n) {
	    if (dom_ctr!=dom_E) eastPen=&stdPen;
	  }
	  else if (cells[dom_ctr].minor!=cells[dom_E].minor) {
	    if (cells[dom_ctr].standard==cells[dom_E].standard)
	      eastPen=&minorPen;
	    else if (cells[dom_ctr].major==cells[dom_E].major)
	      eastPen=&stdPen;
	    else eastPen=&majorPen;
	  }
	}
      }
      if (south.x>=domain_rect.limit[0].low) {
	dom_S=domain[south];
	if ((dom_ctr==Domain::undef) != (dom_S==Domain::undef))
	  southPen=&voidPen;
	else if (dom_ctr!=Domain::undef) {
	  if (dom_ctr>=n || dom_S>=n) {
	    if (dom_ctr!=dom_S) southPen=&stdPen;
	  }
	  else if (cells[dom_ctr].minor!=cells[dom_S].minor) {
	    if (cells[dom_ctr].standard==cells[dom_S].standard)
	      southPen=&minorPen;
	    else if (cells[dom_ctr].major==cells[dom_S].major)
	      southPen=&stdPen;
	    else southPen=&majorPen;
	  }
	}
      }
      if (eastPen || southPen) {
	SE_point.x=semi_gp2coord_y(center.y);
	SE_point.y=semi_gp2coord_x(center.x);
	if (eastPen) {
	  pDC->SelectObject(eastPen);
	  n_coord=semi_gp2coord_x(center.x+1);
	  pDC->MoveTo(SE_point.x,n_coord);
	  pDC->LineTo(SE_point);
	}
	if (southPen) {
	  pDC->SelectObject(southPen);
	  w_coord=semi_gp2coord_y(center.y-1);
	  pDC->MoveTo(w_coord,SE_point.y);
	  pDC->LineTo(SE_point);
	}
	if (eastPen==&voidPen || southPen==&voidPen)
	  pDC->SetPixelV(SE_point,current.color_dom_void);
	else if (eastPen==&majorPen || eastPen==&stdPen ||
		 southPen==&majorPen || southPen==&stdPen)
	  pDC->SetPixelV(SE_point,domap_color);
	else pDC->SetPixelV(SE_point,minor_domap_color);
      }
    }

    // be sure OnDraw() doesn't return with a local pen selected
    pDC->SelectStockObject(NULL_PEN);
  }

  if (CMapView::divideTreeVisible()) DrawRidges(pDC);

  // draw grid points
  if (grid_view!=GV_NONE && hstep_pix>=GV_POINT_WIDTH) {
    CBrush brush(current.color_grid),*oldbrush=pDC->SelectObject(&brush);
    pDC->SetTextColor(current.color_grid);
    pDC->SelectStockObject(NULL_PEN);
    pDC->SelectStockObject(DEFAULT_GUI_FONT);
    pDC->SetBkMode(TRANSPARENT);
    CPoint grid_center;
    Rect_iter_all gi(visible);
    CSize elevSize;
    int len;
    for (gi.reset(); gi; ++gi) {
      grid_center=gp2point(*gi);
      if (grid_view==GV_POINT || doc->em_state==NOT_LOADED ||
	  hstep_pix<GV_ELEV_WIDTH || !doc->elev.contains(*gi) ||
	  doc->elev[*gi]==elev_undef)
	pDC->Rectangle(grid_center.x-1,grid_center.y-1,
		       grid_center.x+3,grid_center.y+3);
      else {
	sprintf(buf,"%d",doc->elev[*gi]);
	len=strlen(buf);
	elevSize=pDC->GetTextExtent(buf,len);
	grid_center.x-=elevSize.cx/2;
	grid_center.y-=elevSize.cy/2;
	pDC->TextOut(grid_center.x,grid_center.y,buf,len);
      }
    }
    pDC->SelectObject(oldbrush);
  }

  if (CMapView::divideTreeVisible())
    DrawFeatures(pDC,doc->tree);

}


/////////////////////////////////////////////////////////////////////////////
// CMapView diagnostics

#ifdef _DEBUG
void CMapView::AssertValid() const
{
  CGraphView::AssertValid();
}

void CMapView::Dump(CDumpContext& dc) const
{
  CGraphView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CMapView message handlers

bool CMapView::divideTreeVisible()
{
  return GetDocument()->dt_state!=NOT_LOADED && view_dt;
}

void CMapView::OnUpdate(CView *pSender, LPARAM lHint, CObject* pHint) 
{
  Feature *changed_feature=(Feature *)lHint;

  CPromDoc *doc=GetDocument();
  if (changed_feature==0 || changed_feature==&CPromDoc::dummyModifyBounds) {
    if (CMapView::viewable()) {
      if (!old_center_loc) {
	// put it in the NW corner
	old_center_loc.x=doc->total_alloc.rect.limit[0].high;
	old_center_loc.y=doc->total_alloc.rect.limit[1].low;
      }
      scrollCenter(old_center_loc);
    }
    else setGraphSizes();
    if (!(doc->mark_bound==(Rectangl)grid_mark)) {
      grid_mark.set_bound(doc->mark_bound);
      grid_mark.fill(false);
    }
    if (changed_feature==0) cell_utd=0;
  }
  else if (changed_feature==&CPromDoc::dummyModifyMap)
    Invalidate();
  else if (changed_feature==&CPromDoc::dummySaveCenter) {
    if (old_center.x>=0 && old_center.y>=0)
      old_center_loc=point2gp(old_center);
    else old_center_loc.undefine();
  }
  else if (changed_feature==&CPromDoc::dummyModifyData ||
	   changed_feature==&CPromDoc::dummyModifyRObase ||
	   changed_feature==&CPromDoc::dummyModifyBSbase ||
	   changed_feature==&CPromDoc::dummyModifySplice ||
	   changed_feature==&CPromDoc::dummyModifyTopo ||
	   changed_feature==&CPromDoc::dummyModifyTrans) {
    // do nothing
  }
  else {
    // a feature(s) was modified. we might have to redraw the cell map.
    cell_utd=false;
    if (view_dm &&
	(cell_major>0 && cell_major<elev_infty ||
	 cell_std>0   && cell_std<elev_infty   ||
	 cell_minor>0 && cell_minor<elev_infty))
      Invalidate();
  }
  CGraphView::OnUpdate(pSender,lHint,pHint);
}

void CMapView::InvalidatePoint(const GridPoint& gp)
{
  CPoint pt=gp2point(gp)-GetScrollPosition();
  CRect pointRect;
  int halfsize_x=hstep_pix/2+2,halfsize_y=vstep_pix/2+2;
  pointRect.top=pt.y-halfsize_y;
  pointRect.bottom=pt.y+halfsize_y;
  pointRect.left=pt.x-halfsize_x;
  pointRect.right=pt.x+halfsize_x;
  InvalidateRect(pointRect);
}

void CMapView::OnViewMapSet()
{
  GridPoint location=point2gp(getGraphScroll());
  CSetLocation_dlg
    dlg(location,GetDocument()->total_alloc.rect,CPromDoc::status_fmt);
  if (dlg.DoModal()==IDOK) scrollCenter(location);
}

void CMapView::OnUpdateViewable(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(CMapView::viewable());
}

void CMapView::OnViewMapGrid()
{
  CGridView_dlg dlg((hstep_pix>=GV_POINT_WIDTH) + (hstep_pix>=GV_ELEV_WIDTH));
  dlg.m_color=current.color_grid;
  dlg.m_grid_view=grid_view;

  if (dlg.DoModal()!=IDOK) return;

  current.color_grid=dlg.m_color;
  switch (dlg.m_grid_view) {
  case 0: grid_view=GV_NONE; break;
  case 1: grid_view=GV_POINT; break;
  case 2: grid_view=GV_ELEV; break;
  }

  Invalidate();
}

void CMapView::OnViewMapContours()
{
  CContour_dlg dlg;
  dlg.m_enable=view_em;
  dlg.m_major=current.contour_major;
  dlg.m_standard=current.contour_std;
  dlg.m_minor=current.contour_minor;
  dlg.m_datum=current.contour_datum;
  dlg.m_autozoom=current.autozoom;
  dlg.m_GPontop=current.GPontop?0:1;
  dlg.m_color_contour=current.color_contour;
  dlg.m_color_void=current.color_elev_void;

  if (dlg.DoModal()!=IDOK) return;

  view_em=dlg.m_enable!=FALSE;
  current.contour_major=dlg.m_major;
  current.contour_std=dlg.m_standard;
  current.contour_minor=dlg.m_minor;
  current.contour_datum=dlg.m_datum;
  current.autozoom=dlg.m_autozoom!=FALSE;
  current.GPontop=dlg.m_GPontop==0;
  current.color_contour=dlg.m_color_contour;
  current.color_elev_void=dlg.m_color_void;

  CMapView::SetViewTitle();
  Invalidate();
}

void CMapView::OnUpdateViewMapContours(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetDocument()->em_state!=NOT_LOADED);
  pCmdUI->SetCheck(view_em);
}

void CMapView::set_light_vec()
{
  const Rectangl bound=GetDocument()->elev;
  light_vec_x=current.light_int*cos(current.light_dir*0.0174532925199433);
  light_vec_y=current.light_int*sin(current.light_dir*0.0174532925199433)/
    cos(bound.limit[0].midpt()*GridPoint_equat::lat_step*sec2rad);
}

void CMapView::OnViewMapRelief()
{
  CRelief_dlg dlg(current.elev_colors);
  dlg.m_direction=current.light_dir;
  dlg.m_intensity=current.light_int;
  dlg.m_color_enable=current.elev_color_enable;

  if (dlg.DoModal()!=IDOK) return;

  current.light_dir=dlg.m_direction;
  current.light_int=dlg.m_intensity;
  current.elev_color_enable=dlg.m_color_enable!=FALSE;

  set_light_vec();
  Invalidate();
}

void CMapView::OnUpdateViewMapRelief(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetDocument()->em_state!=NOT_LOADED);
  pCmdUI->SetCheck(current.elev_color_enable || current.light_int>0);
}

void CMapView::OnViewMapDomains()
{
  CDomainView_dlg dlg;
  CPromDoc *doc=GetDocument();

  dlg.major=cell_major;
  dlg.standard=cell_std;
  dlg.minor=cell_minor;
  dlg.m_enable=view_dm;
  dlg.m_color_domain=current.color_domain;
  dlg.m_color_void=current.color_dom_void;
  dlg.m_offmap=offmap_prom;
  dlg.m_rotation=cell_cr;
  dlg.m_edge_sadl=cell_edge_sadl;

  if (dlg.DoModal()!=IDOK) return;

  cell_major=dlg.major;
  cell_std=dlg.standard;
  cell_minor=dlg.minor;
  view_dm=dlg.m_enable!=FALSE;
  current.color_domain=dlg.m_color_domain;
  current.color_dom_void=dlg.m_color_void;
  offmap_prom=dlg.m_offmap!=FALSE;
  cell_cr=dlg.m_rotation!=FALSE;
  cell_edge_sadl=dlg.m_edge_sadl!=FALSE;
  cell_utd=false;

  Invalidate();
}

void CMapView::OnUpdateViewMapDomains(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetDocument()->dm_state!=NOT_LOADED);
  pCmdUI->SetCheck(view_dm);
}

void CMapView::OnViewMapDLPRtree()
{
  view_dt=!view_dt;
  CMapView::SetViewTitle();
  Invalidate();
}

void CMapView::OnUpdateViewMapDLPRtree(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetDocument()->dt_state!=NOT_LOADED);
  pCmdUI->SetCheck(view_dt);
  pCmdUI->SetText(GetDocument()->is_drainage?"Drainage &tree":"Divide &tree");
}

void CMapView::OnViewMapMarkView()
{
  view_mark=!view_mark;
  Invalidate();
}

void CMapView::OnUpdateViewMapMarkView(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetDocument()->em_state!=NOT_LOADED ||
		 GetDocument()->dm_state!=NOT_LOADED);
  pCmdUI->SetCheck(view_mark);
}

void CMapView::OnViewColorsMark()
{
  CViewColorDialog dlg(current.color_mark,"Mark Color");
  if (dlg.DoModal()!=IDOK) return;
  current.color_mark=dlg.GetColor();
  if (view_mark) Invalidate();
}

void CMapView::OnMouseMove(UINT nFlags, CPoint point)
{
  CGraphView::OnMouseMove(nFlags,point);

  CPromDoc *doc=GetDocument();
  if (!CMapView::viewable()) return;
  point+=GetScrollPosition();
  CStatusBar& sb=((CPromFrame *)AfxGetMainWnd())->statusBar;
  GridPoint c=point2gp(point);
  CPromDoc::status_fmt.set();
  c.sprint(buf);
  sb.SetPaneText(CPromFrame::SB_PANE_LOC,buf);
  if (doc->em_state!=NOT_LOADED && doc->elev.contains(c) &&
      doc->elev[c]!=elev_undef) {
    sprintf(buf,"%d",doc->elev[c]);
    sb.SetPaneText(CPromFrame::SB_PANE_ELEV,buf);
  }
  else sb.SetPaneText(CPromFrame::SB_PANE_ELEV,"");
}

void CMapView::setSizeIndicators()
{
  CPromDoc *doc=GetDocument();
  CStatusBar& sb=((CPromFrame *)AfxGetMainWnd())->statusBar;
  if (CMapView::viewable()) {
    CSize winSize=getWinSize();
    sprintf(buf,winSize.cx/current.scale<1000?"%5.3g":"%5.0f",
	    winSize.cx/current.scale);
    sb.SetPaneText(CPromFrame::SB_PANE_WD,buf);
    sprintf(buf,winSize.cy/current.scale<1000?"%5.3g":"%5.0f",
	    winSize.cy/current.scale);
    sb.SetPaneText(CPromFrame::SB_PANE_HT,buf);
  }
  else {
    sb.SetPaneText(CPromFrame::SB_PANE_WD,"");
    sb.SetPaneText(CPromFrame::SB_PANE_HT,"");
  }
}

void CMapView::setMarkMenuItemState(CMenu *popup, UINT mark_item)
{
  CPromDoc *doc=GetDocument();
  if (!grid_mark.contains(r_click_gp) || !view_mark) {
    popup->EnableMenuItem(mark_item,MF_BYPOSITION|MF_GRAYED);
    return;
  }
  if (grid_mark[r_click_gp])
    popup->ModifyMenu(mark_item,MF_BYPOSITION|MF_STRING,
		      ID_GP_MARKAREA,"Un&mark");
  if (!(doc->em_state!=NOT_LOADED && doc->elev.contains(r_click_gp) &&
	doc->elev[r_click_gp]!=elev_undef) &&
      !(doc->dm_state!=NOT_LOADED && doc->domain.contains(r_click_gp) &&
	doc->domain[r_click_gp]!=Domain::undef)) {
    popup->EnableMenuItem(ID_GP_MARKAREA,MF_BYCOMMAND|MF_GRAYED);
  }
}

CMenu *CMapView::backgdMenu(CMenu& parentMenu, CPoint &point)
{
  point+=GetScrollPosition();
  r_click_gp=point2gp(point);
  point=gp2point(r_click_gp);
  CMenu *popup=parentMenu.GetSubMenu(3);
  setMarkMenuItemState(popup,2);
  CPromDoc *doc=GetDocument();
  if (doc->is_drainage)
    popup->ModifyMenu(ID_GP_DLPR_PEAK,MF_BYCOMMAND|MF_STRING,
		      ID_GP_DLPR_PEAK,"&Domain basin");
  if (doc->dm_state!=NOT_LOADED && CMapView::divideTreeVisible() &&
      ((Rectangl)doc->domain).contains(r_click_gp) &&
      doc->domain[r_click_gp]!=Domain::undef &&
      doc->domain[r_click_gp]<doc->tree.n_dom() &&
      doc->tree[doc->domain[r_click_gp]].peak.location) {
    popup->EnableMenuItem(ID_GP_DLPR_PEAK,MF_BYCOMMAND|MF_ENABLED);
  }
  else if (doc->is_drainage)
    popup->EnableMenuItem(ID_GP_DLPR_PEAK,MF_BYCOMMAND|MF_GRAYED);
  if (doc->em_state==NOT_LOADED || !doc->elev.contains(r_click_gp)) {
    popup->EnableMenuItem(ID_GP_RQF,MF_BYCOMMAND|MF_GRAYED);
    popup->EnableMenuItem(ID_IMPRESS,MF_BYCOMMAND|MF_GRAYED);
    popup->EnableMenuItem(ID_FREE_HEIGHT,MF_BYCOMMAND|MF_GRAYED);
  }
  if (doc->dt_state!=NOT_LOADED) {
    if (CGraphView::current.view_peak) {
      switch (get_nsel(FT_PEAK)) {
      case 1:
	popup->EnableMenuItem(ID_GP_NEW_SADDLE,MF_BYCOMMAND|MF_ENABLED);
	popup->ModifyMenu(ID_GP_NEW_SADDLE,MF_BYCOMMAND|MF_STRING,
			  ID_GP_NEW_RUNOFF,"&Runoff");
	break;
      case 2:
	popup->EnableMenuItem(ID_GP_NEW_SADDLE,MF_BYCOMMAND|MF_ENABLED);
	break;
      }
    }
  }
  point-=GetScrollPosition();
  return popup;
}

CMenu *CMapView::featureMenu(CMenu& parentMenu, Feature& featr)
{
  r_click_gp=featr.location;
  CMenu *popup=parentMenu.GetSubMenu(2);
  setMarkMenuItemState(popup,4);
  return popup;
}

void CMapView::OnGpCenter()
{
  setGraphScroll(gp2point(r_click_gp));
}

void CMapView::OnViewMapMarkClear()
{
  MarkElevRange(false);
}

void CMapView::OnViewMapMarkClearAll()
{
  grid_mark.fill(false);
  Invalidate();
}

void CMapView::OnViewMapMarkPrecise()
{
  MarkElevRange(true,true);
}

void CMapView::MarkElevRange(bool value, bool clear)
{
  Elev_intvl target(-elev_infty,elev_infty);
  Rectangl
    limits(-coord_infty,coord_infty,-coord_infty,coord_infty);
  CElevRect_dlg dlg(target,limits);
  if (dlg.DoModal()!=IDOK) return;

  BeginWaitCursor();
  if (clear) grid_mark.fill(false);
  Elev_map& elev=GetDocument()->elev;
  Elevation elv;
  for (Rect_iter_all gi(limits&elev); gi; ++gi) {
    elv=elev[*gi];
    if (elv!=elev_undef && target.contains(elv))
      grid_mark[*gi]=value;
  }
  Invalidate();
  EndWaitCursor();
}

void CMapView::OnViewMapMarkSet()
{
  MarkElevRange(true);
}


void CMapView::OnUpdateViewMapMark(CCmdUI *pCmdUI)
{
  pCmdUI->Enable((GetDocument()->em_state!=NOT_LOADED ||
		  GetDocument()->dm_state!=NOT_LOADED) && view_mark);
}

void CMapView::OnGpMarkArea()
{
  if (r_click_featr) r_click_gp=r_click_featr->location;
  GPfilter filter(*GetDocument(),r_click_gp);
  CGridPointFilter_dlg dlg(filter,r_click_gp);
  if (dlg.DoModal()!=IDOK) return;
  BeginWaitCursor();
  bool prev_mark=grid_mark[r_click_gp];
  Grid_mark markable(*GetDocument());
  /* TODO(akirmse): This doesn't compile:

  1>c:\github\winprom\region.h(116): error C2664: 'Conditional_nbr_iter<Nbr_iter,GridSet>::Conditional_nbr_iter(const Conditional_nbr_iter<Nbr_iter,GridSet> &)': cannot convert argument 1 from 'const Grid_mark' to 'const GridSet &'
  1>          with
  1>          [
  1>              Nbr_iter=RawNbr_iter,
  1>              GridSet=GridSet
  1>          ]
  1>  c:\github\winprom\region.h(116): note: Reason: cannot convert from 'const Grid_mark' to 'const GridSet'
  1>  c:\github\winprom\region.h(116): note: No user-defined-conversion operator available that can perform this conversion, or the operator cannot be called
  1>  c:\github\winprom\region.h(115): note: while compiling class template member function 'void Region_iter<Filter_region,Grid_mark>::operator ++(void)'
  1>  c:\github\winprom\mapview.cpp(1238): note: see reference to function template instantiation 'void Region_iter<Filter_region,Grid_mark>::operator ++(void)' being compiled
  1>  c:\github\winprom\mapview.cpp(1236): note: see reference to class template instantiation 'Region_iter<Filter_region,Grid_mark>' being compiled

  for (Region_iter<Filter_region,Grid_mark>	
       ri(r_click_gp,Filter_region(filter,grid_mark,!prev_mark),markable);
       ri; ++ri);
  */
  Invalidate();
  EndWaitCursor();
}

void CMapView::OnGpMarkPoint()
{
  if (r_click_featr) r_click_gp=r_click_featr->location;
  bool& mark=grid_mark[r_click_gp];
  mark=!mark;
  InvalidatePoint(r_click_gp);
}

void CMapView::OnIdentifyDLPRpeak()
{
  CPromDoc *doc=GetDocument();
  IdentifyFeature(doc->tree[doc->domain[r_click_gp]].peak,FT_PEAK);
}

void CMapView::OnGP_RQF()
{
  CRQFinfo_dlg(GetDocument(),r_click_gp,this).DoModal();
}

void CMapView::scrollCenterCoord(int ic, GridPoint::Coord spot, double step_pix)
{
  const Rectangl& total_rect=GetDocument()->total_alloc.rect;
  if (step_pix*total_rect.limit[ic].size()>GRAPH_MAX_DIM) {
    int half_size=(int)(GRAPH_MAX_DIM/step_pix/2);
    if (spot-total_rect.limit[ic].low<half_size) {
      scrollRect.limit[ic].set(total_rect.limit[ic].low,
			       total_rect.limit[ic].low+half_size*2);
    }
    else if (total_rect.limit[ic].high-spot<half_size) {
      scrollRect.limit[ic].set(total_rect.limit[ic].high-half_size*2,
			       total_rect.limit[ic].high);
    }
    else {
      scrollRect.limit[ic].set(spot-half_size,spot+half_size);
    }
  }
  else scrollRect.limit[ic]=total_rect.limit[ic];
}

void CMapView::scrollCenter(GridPoint center_loc)
{
  const Rectangl& total_rect=GetDocument()->total_alloc.rect;
  if (center_loc.x<total_rect.limit[0].low)
    center_loc.x=total_rect.limit[0].low;
  else if (center_loc.x>total_rect.limit[0].high)
    center_loc.x=total_rect.limit[0].high;
  if (center_loc.y<total_rect.limit[1].low)
    center_loc.y=total_rect.limit[1].low;
  else if (center_loc.y>total_rect.limit[1].high)
    center_loc.y=total_rect.limit[1].high;
  setMapScale(center_loc);
  scrollCenterCoord(0,center_loc.x,vstep_pix);
  scrollCenterCoord(1,center_loc.y,hstep_pix);
  setMapSize();
  Invalidate();
  setGraphScroll(gp2point(center_loc));
}

void CMapView::OnViewScrollCenter()
{
  scrollCenter(point2gp(old_center));
}

void CMapView::OnUpdateViewScrollCenter(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(viewable() && !(scrollRect==GetDocument()->total_alloc.rect));
}

void CMapView::viewFeature(const CPromDoc *doc, const Feature& featr, FT_index)
{
  if (!doc->total_alloc.rect.contains(featr.location)) {
    ::MessageBox(NULL,"The feature you are trying to locate\n"
		 "lies outside the active map view.",
		 "Winprom View",MB_OK|MB_ICONERROR);
  }
  else scrollCenter(featr.location);
}

void CMapView::viewLocation(const GridPoint& gp)
{
  if (!GetDocument()->total_alloc.rect.contains(gp)) {
    ::MessageBox(NULL,"The point you are trying to locate\n"
		 "lies outside the active map view.",
		 "Winprom View",MB_OK|MB_ICONERROR);
  }
  else scrollCenter(gp);
}

void CMapView::SetViewTitle()
{
  CPromDoc *document=GetDocument();
  CWnd *parent=GetParent();
  if (!document->dt_name.IsEmpty() && view_dt) {
    parent->SetWindowText(document->dt_name+
      (document->is_drainage ? " - Drainage Tree Map" : " - Divide Tree Map"));
    parent->SetIcon(AfxGetApp()->LoadIcon(IDR_GRAPHTYPE),TRUE);
  }
  else if (!document->em_name.IsEmpty() && view_em) {
    parent->SetWindowText(document->em_name+" - Elevation Map");
    parent->SetIcon(AfxGetApp()->LoadIcon(IDR_ELEV),TRUE);
  }
  else if (!document->dm_name.IsEmpty() && view_dm) {
    parent->SetWindowText(document->dm_name+" - Domain Map");
    parent->SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),TRUE);
  }
  else if (CMapView::divideTreeVisible()) {
    parent->SetWindowText(document->is_drainage ?
		"Drainage Tree Map" : "Divide Tree Map");
    parent->SetIcon(AfxGetApp()->LoadIcon(IDR_GRAPHTYPE),TRUE);
  }
  else if (document->em_state!=NOT_LOADED && view_em) {
    parent->SetWindowText("Elevation Map");
    parent->SetIcon(AfxGetApp()->LoadIcon(IDR_ELEV),TRUE);
  }
  else if (document->dm_state!=NOT_LOADED && view_dm) {
    parent->SetWindowText("Domain Map");
    parent->SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),TRUE);
  }
  else parent->SetWindowText("Map View");
}

#define N_DIST 100

void CMapView::OnImpress()
{
  CImparam_dlg dlg;
  if (dlg.DoModal()!=IDOK) return;

  BeginWaitCursor();
  enum {Rat_AB,Rat_EA,Angle_AB,Angle_EA,Type_N};
  int type,ir,i;
  const Elev_map& elev=GetDocument()->elev;
  Elevation r_click_elev=elev[r_click_gp];
  double imp[N_DIST+1][2*Type_N],max_val[2*Type_N],y,s,ri,r,dA,value,
    kdA=dlg.conv_fac*dlg.conv_fac*0.1591549430918953;
  double avg_y_ring[N_DIST+1][2],avg_y_inf[N_DIST+1][2],wt[N_DIST+1][2],s1,as;
  GridPoint max_loc[2*Type_N];
  // initialize sums to 0
  for (i=0; i<N_DIST+1; ++i) {
    avg_y_ring[i][0]=avg_y_ring[i][1]=0;
    wt[i][0]=wt[i][1]=0;
    for (type=0; type<2*Type_N; ++type)
      imp[i][type]=0;
  }
  for (type=0; type<2*Type_N; ++type)
    max_val[type]=0;
  // leter rip
  for (Grid_iter gi(elev); gi; ++gi) {
    assert(elev[*gi]!=elev_undef);
    if ((*gi)!=r_click_gp) {
      dA=(*gi).area()*kdA;
      y=r_click_elev-elev[*gi];
      // determine bin
      r=r_click_gp-(*gi);
      ri=1/(r*dlg.conv_fac);
      ir=(unsigned)(r/dlg.m_distint);
      if (ir>N_DIST) ir=N_DIST;
      // compute weight & slope
      s=fabs(y)*ri;
      wt[ir][y<0]+=ri;
      avg_y_ring[ir][y<0]+=s;
      // rational sum
      type=y>=0?Rat_AB:Rat_EA;
      imp[ir][type]+=(log(1+s)-s/(s+1))*ri*dA; // p=1
      s1=s+1;
      imp[ir][type+4]+=(2*s*(6+9*s+2*s*s)/(s1*s1)-12*log(s1))*dA; // p=2
      value=fabs(y)*s/s1;
      if (value>max_val[type]) {
	max_val[type]=value;
	max_loc[type]=(*gi);
      }
      // angle sum
      type=y>=0?Angle_AB:Angle_EA;
      imp[ir][type]+=0.3183098861837907*log(1+s*s)*ri*dA; // p=1
      as=atan(s);
      imp[ir][type+4]+=0.8105694691387022*(-as*as+2*s*as-log(1+s*s))*dA; // p=2
      value=fabs(y)*as*0.6366197723675813;
      if (value>max_val[type]) {
	max_val[type]=value;
	max_loc[type]=(*gi);
      }
    }
  }
  // compute sum over bins
  for (i=0; i<N_DIST; ++i)
    for (type=0; type<2*Type_N; ++type)
      imp[i+1][type]+=imp[i][type];

  // compute cumulative integrals from r to infinity
  avg_y_inf[N_DIST][0]=avg_y_ring[N_DIST][0];
  avg_y_inf[N_DIST][1]=avg_y_ring[N_DIST][1];
  avg_y_ring[N_DIST][0]/=wt[N_DIST][0];
  avg_y_ring[N_DIST][1]/=wt[N_DIST][1];
  for (i=N_DIST-1; i>=0; --i) {
    for (type=0; type<=1; ++type) {
      avg_y_inf[i][type]=avg_y_inf[i+1][type]+avg_y_ring[i][type];
      avg_y_ring[i][type]/=wt[i][type];
      wt[i][type]+=wt[i+1][type];
      avg_y_inf[i+1][type]/=wt[i+1][type];
    }
  }
  avg_y_inf[0][0]/=wt[0][0];
  avg_y_inf[0][1]/=wt[0][1];

  // adjust the p=2 answers
  for (type=Type_N; type<2*Type_N; ++type)
    imp[N_DIST][type]=sqrt(imp[N_DIST][type]);
  for (i=0; i<N_DIST; ++i)
    for (type=Type_N; type<2*Type_N; ++type)
      imp[i][type]/=imp[N_DIST][type];

  EndWaitCursor();

  CImpOut_dlg out_dlg(r_click_gp,RawGridSet(Rectangl(elev)).nearest_edge(r_click_gp),
		      dlg.conv_fac,max_val,max_loc,imp,avg_y_ring,avg_y_inf,
		      N_DIST,dlg.m_distint,this);
  out_dlg.DoModal();

  return;
}

void CMapView::OnFreeHeight()
{
  CFreeHeightParam_dlg dlg;
  const Elev_map& elev=GetDocument()->elev;
  if (r_click_featr) dlg.m_ref_elev=r_click_featr->elev.midpt();
  else dlg.m_ref_elev=elev[r_click_gp];
  dlg.m_rad_vert="20903520";
  if (dlg.DoModal()!=IDOK) return;

  CFreeHeight_dlg out_dlg;
  out_dlg.DoModal();
}


BOOL CMapView::OnPreparePrinting(CPrintInfo *pInfo)
{
  CPrintMap_dlg dlg(*this,pInfo);
  if (!AfxGetApp()->GetPrinterDeviceDefaults(&pInfo->m_pPD->m_pd)) {
    MessageBox("Can\'t find default printer.\nHave you installed one?",
	       "Winprom Map Print",MB_OK|MB_ICONERROR);
    return FALSE;
  }
  pInfo->m_pPD->m_pd.Flags|=PD_PRINTSETUP;
  save_scrollRect=scrollRect;
  if (dlg.DoModal()!=IDOK) return FALSE;
  if (pInfo->m_pPD->m_pd.hDC==0)
    pInfo->m_pPD->m_pd.hDC=pInfo->m_pPD->CreatePrinterDC();
  print_limits=dlg.limits;
  return print_params.final_update(pInfo);
}

void CMapView::OnPrepareDC(CDC *pDC, CPrintInfo *pInfo)
{
  if (!pInfo) {
    CScrollView::OnPrepareDC(pDC,pInfo);
    global_offset.x=global_offset.y=GRAPH_VIEW_MARGIN;
    return;
  }
  scale_print(pDC,pInfo);
  const int& nxpag=print_params.nxpag;
  const CSize& page_size=print_params.page_size;
  int ixpag=(pInfo->m_nCurPage-1)%nxpag,iypag=(pInfo->m_nCurPage-1)/nxpag;
  global_offset.x=-ixpag*page_size.cx+GRAPH_VIEW_MARGIN+print_params.extra_margin.cx;
  global_offset.y=-iypag*page_size.cy+GRAPH_VIEW_MARGIN+print_params.extra_margin.cy;
}

void CMapView::OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo)
{
  save_scrollRect=scrollRect;
  scrollRect=print_limits;
  CGraphView::OnBeginPrinting(pDC,pInfo);
}

void CMapView::OnEndPrinting(CDC *pDC, CPrintInfo *pInfo)
{
  scrollRect=save_scrollRect;
  CGraphView::OnEndPrinting(pDC,pInfo);
}

void CMapView::OnUpdateFilePrint(CCmdUI *pCmdUI)
{
  CPromDoc *doc=GetDocument();
  pCmdUI->Enable(doc->em_state!=NOT_LOADED && view_em ||
		 doc->dm_state!=NOT_LOADED && view_dm ||
		 doc->dt_state!=NOT_LOADED && view_dt);
}


int CMapView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CGraphView::OnCreate(lpCreateStruct) == -1)
    return -1;

  set_light_vec();

  return 0;
}

void CMapView::OnOpDLPRcheck()
{
  BeginWaitCursor();
  CPromDoc *doc=GetDocument();
  const Elev_map& elev=doc->elev;
  doc->elev_flip_drain();
  const Index_map& domap=doc->domain;
  Grid grid(elev);
  GridPoint steep_nbr;
  Rectangl bound=(Rectangl)elev&(Rectangl)domap;
  RawGridSet grid_set(bound);
  bool mismatch=false,marked=false;
  for (RawGrid_iter gi(grid_set); gi; ++gi) {
    const Domain::Index& dom=domap[*gi];
    if (domap.contains(*gi) && dom!=Domain::undef) {
      steep_nbr=steepest_nbr(*gi,elev,grid);
      if (steep_nbr) {
	if (domap[steep_nbr]!=dom) {
	  if (!mismatch) {
	    mismatch=true;
	    if (MessageBox("The domain of a grid point does not match that of its steepest-climb neighbor.\n"
			   "Would you like to mark any offending-domain grid points?","Winprom Domain Map Check",
			   MB_YESNO)!=IDYES) break;
	  }
	  grid_mark[*gi]=true;
	  marked=true;
	}
      }
    }
  }
  doc->elev_flip_drain();
  EndWaitCursor();

  if (!mismatch)
    MessageBox("The domain map is consistent with the elevation map.");
  if (marked) Invalidate();
}

void CMapView::OnBrowse()
{
  CPromApp::getApp()->Navigate(r_click_gp);
}

void CMapView::OnViewLabelStatus()
{
  CLocationFmt_dlg(this,CPromDoc::status_fmt).DoModal();
}

void CMapView::OnViewSet()
{
  CGraphView::set_initial_settings();
  initial=current;
}

void CMapView::OnViewApply()
{
  GridPoint location=point2gp(getGraphScroll());
  CGraphView::get_initial_settings();
  current=initial;
  scrollCenter(location);
  Invalidate();
}

void CMapView::OnOpenMarkMap()
{
  CString name=CPromDoc::GetFileName(CPromDoc::FO_OPEN,NULL,"Mark Map","wpm");
  if (name.IsEmpty()) return;

  try {
    Bool_map grid_mark_file;
    grid_mark_file.read(name);
    if (!grid_mark.transfer(grid_mark_file)) {
      CPromDoc::FileError("reading mark map",name,"File map does not overlap workspace");
    }
    Invalidate();
  }
  catch (file_error e) {
    CPromDoc::FileError("reading mark map",name,e.what());
  }
  catch (bad_alloc) {
    CPromDoc::FileError("reading mark map",name,"Memory exhaustion");
  }
}

void CMapView::OnSaveMarkMap()
{
  CString name=CPromDoc::GetFileName(CPromDoc::FO_SAVE,NULL,"Mark Map","wpm");
  if (name.IsEmpty()) return;

  try {
    grid_mark.write(name);
  }
  catch (file_error e) {
    CPromDoc::FileError("saving mark map",name,e.what());
  }
}

void CMapView::OnUpdateHaveMarkMap(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(!GetDocument()->mark_bound.is_empty());
}

static void promote(const Domain *&dom, Elevation prom, bool edge_sadl,
		    Elevation const Domain::Sadl_prom::*prom_type)
{
  while (dom->parent.cell &&
	 (dom->parent.saddle->saddle.edge_effect && !edge_sadl ?
	  dom->prom.pk_omp : dom->parent.saddle->prom.saddle.*prom_type) < prom)
    dom=dom->parent.cell;
}

// determine the cell for each peak
void CMapView::update_cell()
{
  CPromDoc *doc=GetDocument();
  if (doc->update_sadl_prom_info_rq() && cell_utd) return;
  const Divide_tree& tree=doc->tree;
  Domain::Index i,n=tree.n_dom();
  if (n>ncell || n<=ncell/2) {
    delete[] cells;
    cells=new Cell_xlate[n];
    ncell=n;
  }
  const Domain *dom;

  Elevation const Domain::Sadl_prom::*prom_type=
    sadl_prom_types[offmap_prom][cell_cr];
  for (i=0; i<n; ++i) {
    dom=&tree[i];
    if (!dom->peak.location) continue;
    promote(dom,cell_minor,cell_edge_sadl,prom_type);
    cells[i].minor=tree.index(dom);
    promote(dom,cell_std,cell_edge_sadl,prom_type);
    cells[i].standard=tree.index(dom);
    promote(dom,cell_major,cell_edge_sadl,prom_type);
    cells[i].major=tree.index(dom);
  }
  cell_utd=true;
}

void CMapView::OnViewMapCutoffs()
{
  CPromCutoff_dlg dlg;
  CPromDoc *doc=GetDocument();

  dlg.major=ftr_large;
  dlg.standard=ftr_medium;
  dlg.minor=ftr_small;
  if (CGraphView::current.runoff_ridge==offmap_prom) dlg.m_offmap=offmap_prom;
  else dlg.m_offmap=2;
  if (ftr_rr==ridge_rr) dlg.m_ridge_rot=ftr_rr;
  else dlg.m_ridge_rot=2;
  if (ftr_cr==cell_cr) dlg.m_cell_rot=ftr_cr;
  else dlg.m_cell_rot=2;

  if (dlg.DoModal()!=IDOK) return;

  ridge_major=ftr_large=cell_major=dlg.major;
  ridge_std=ftr_medium=cell_std=dlg.standard;
  ridge_minor=ftr_small=cell_minor=dlg.minor;
  if (dlg.m_offmap<2)
    CGraphView::current.runoff_ridge=offmap_prom=dlg.m_offmap!=FALSE;
  if (dlg.m_ridge_rot<2)
    ftr_rr=ridge_rr=dlg.m_ridge_rot!=FALSE;
  if (dlg.m_cell_rot<2)
    ftr_cr=cell_cr=dlg.m_cell_rot!=FALSE;
  cell_utd=false;

  translateStates(LS_SELECTED,LS_VISIBLE,0); // clear selection
  Invalidate();
}

void CMapView::OnGpNewPeak()
{
}

void CMapView::OnGpNewRunoff()
{
  CPromDoc *doc=GetDocument();
  Domain *peak=Domain::FromPeak(&FindSelectedPeak());
  doc->NewRunoff(r_click_gp,peak);
}

void CMapView::OnGpNewSaddle()
{
  CPromDoc *doc=GetDocument();
  Divide_tree& tree=doc->tree;
  Domain::Index i,n=tree.n_dom();
  Domain *peak1=0,*peak2=0;
  for (i=0; i<n; ++i) {
    if (get_label_state(FT_PEAK,i)==LS_SELECTED) {
      if (peak1==0) peak1=&tree[i];
      else {
	peak2=&tree[i];
	break;
      }
    }
  }
  assert(i<n);
  doc->NewSaddle(r_click_gp,peak1,peak2);
}
