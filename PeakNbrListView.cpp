// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// PeakNbrListView.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "PeakNbrListView.h"
#include "ListFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPeakNbrListView

IMPLEMENT_DYNCREATE(CPeakNbrListView, CFeatureListView)

CPeakNbrListView::CPeakNbrListView()
{
}

CPeakNbrListView::~CPeakNbrListView()
{
}

Domain *CPeakNbrListView::init_center_peak;


BEGIN_MESSAGE_MAP(CPeakNbrListView, CFeatureListView)
  //{{AFX_MSG_MAP(CPeakNbrListView)
  ON_WM_CREATE()
  ON_COMMAND(ID_EDIT_SELECT_OVERLAPS, OnEditSelectOverlaps)
  ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_OVERLAPS, OnUpdateEditSelectOverlaps)
  ON_UPDATE_COMMAND_UI(ID_LIST_LOAD, OnUpdateListLoad)
  ON_UPDATE_COMMAND_UI(ID_LIST_APPEND, OnUpdateListLoad)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CpeakNbrListView diagnostics

#ifdef _DEBUG
void CPeakNbrListView::AssertValid() const
{
  CFeatureListView::AssertValid();
}

void CPeakNbrListView::Dump(CDumpContext& dc) const
{
  CFeatureListView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CPeakNbrListView message handlers

void CPeakNbrListView::init_list(CPromDoc *document, Domain& dom, bool select_ovl)
{
  CListFrame::client_width=112;
  CSingleDocTemplate& NbrListViewTpl=CPromApp::getApp()->NbrListViewTpl;
  CListFrame *newlist=(CListFrame *)NbrListViewTpl.CreateNewFrame(document,NULL);

  init_center_peak=&dom;

  NbrListViewTpl.InitialUpdateFrame(newlist,document);
  if (select_ovl) ((CPeakNbrListView *)newlist->GetActiveView())->OnEditSelectOverlaps();
  newlist->SetIcon(AfxGetApp()->LoadIcon(IDR_GRAPHTYPE),TRUE);
}

int CPeakNbrListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CFeatureListView::OnCreate(lpCreateStruct) == -1)
    return -1;

  InsertColumn(SWIR_COL,"SWIR",LVCFMT_RIGHT,56);
  InsertColumn(SWIRO_COL,"SWIRO",LVCFMT_RIGHT,56);
	
  return 0;
}

static void set_wr_col(CListCtrl& listCtrl, int nItem, int col, Elevation wr)
{
  char *const buf=CFeatureListView::buf;
  if (wr==elev_infty) buf[0]='\0';
  else if (wr==-elev_infty)
    sprintf(buf,"%s",Basin_saddle::WS_string);
  else sprintf(buf,Elev_intvl::elem_format,wr);
  listCtrl.SetItemText(nItem,col,buf);
}

static void nbr_WR(Domain *center, Feature *featr, FT_index ft,
		   Elevation& walk_risk, Elevation& wro, bool drainage)
{
  switch (ft) {
  case FT_SADDLE:
    {
      Domain *nbr_peak=CPromDoc::peak_beyond_sadl(&center->peak,featr);
      walk_risk=center->sadl_walk_inv_risk(nbr_peak,drainage);
      wro=center->SWIR_offense(nbr_peak,drainage);
    }
    break;
  case FT_PEAK:
    walk_risk=wro=elev_infty;
    break;
  case FT_RUNOFF:
    walk_risk=elev_infty;
    wro=center->SWIR_offense((Runoff *)featr,drainage);
    break;
  case FT_BASINSADL:
    walk_risk=elev_infty;
    wro=center->SWIR_offense((Basin_saddle *)featr,drainage);
    break;
  default:
    assert(0);
  }
}

void CPeakNbrListView::UpdateItem(int nItem)
{
  CFeatureListView::UpdateItem(nItem);
  CListCtrl& listCtrl=GetListCtrl();
  if (!center_peak) {
    listCtrl.SetItemText(nItem,SWIR_COL,"???");
    listCtrl.SetItemText(nItem,SWIRO_COL,"???");
    return;
  }
  Elevation walk_risk,wro;
  nbr_WR(center_peak,&GetFeature(nItem),GetFeatureType(nItem),
	 walk_risk,wro,GetDocument()->is_drainage);
  set_wr_col(listCtrl,nItem,SWIR_COL,walk_risk);
  set_wr_col(listCtrl,nItem,SWIRO_COL,wro);
}

static void print_wr(FILE *file, Elevation wr)
{
  if (wr==elev_infty) fprintf(file,"     ");
  else if (wr==-elev_infty) fprintf(file,"%s",Basin_saddle::WS_string);
  else fprintf(file,Elev_intvl::elem_format,wr);
}

void CPeakNbrListView::printFeature(int item, FILE *out_file,
				    Format_style style)
{
  Elevation walk_risk,wro;
  nbr_WR(center_peak,&GetFeature(item),GetFeatureType(item),
	 walk_risk,wro,GetDocument()->is_drainage);
  if (style==FMS_HTML) fprintf(out_file,"<TD align=right>");
  print_wr(out_file,walk_risk);
  fprintf(out_file,get_delimiter(style,CA_RIGHTx));
  print_wr(out_file,wro);
  if (style!=FMS_HTML) fprintf(out_file,get_delimiter(style,CA_RIGHTx));
  CFeatureListView::printFeature(item,out_file,style);
}

void CPeakNbrListView::printHeader(FILE *out_file, Format_style style)
{
  switch (style) {
  case FMS_FLAG:
    fprintf(out_file," SWIR SWIRO ");
    break;
  case FMS_HTML:
    fprintf(out_file,"<TD align=center>SWIR<TD align=center>SWIRO");
    break;
  case FMS_CSV:
    fprintf(out_file," SWIR,SWIRO,");
    break;
  default:
    assert(0);
  }
  CFeatureListView::printHeader(out_file,style);
}

void CPeakNbrListView::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  if (lHint==0) {
    center_peak=0;
  }
  else {
    Feature *changed_feature=(Feature *)lHint;
    if (changed_feature==&center_peak->peak && !changed_feature->location)
      center_peak=0;
  }
  CFeatureListView::OnUpdate(pSender,lHint,pHint);
}

void CPeakNbrListView::OnInitialUpdate()
{
  CFeatureListView::OnInitialUpdate();

  if (init_center_peak) {
    center_peak=init_center_peak;
    init_center_peak=0;
  }

  if (center_peak==0) return;

  CListCtrl& listCtrl=GetListCtrl();

  AddItem(center_peak->peak,FT_PEAK);

  for (Domain::Neighbor_iter ni(*center_peak); ni; ++ni) {
    AddItem(ni.saddle_owner()->saddle,FT_SADDLE);
    AddItem((*ni)->peak,FT_PEAK);
  }
  for (ROiter roi=center_peak->runoffs.begin();
       roi!=center_peak->runoffs.end(); ++roi)
    AddItem(**roi,FT_RUNOFF);
  for (BSiter bsi=center_peak->bsnsdls.begin();
       bsi!=center_peak->bsnsdls.end(); ++bsi) {
    Basin_saddle& bs=(**bsi);
    AddItem(bs,FT_BASINSADL);
    AddItem((bs.peak1==center_peak?bs.peak2:bs.peak1)->peak,FT_PEAK);
  }

  setItemIndicators();
}

void CPeakNbrListView::SetViewTitle()
{
  CPromDoc *document=GetDocument();
  sprintf(buf,"%s%s Neighbors",
	  document->dt_name.IsEmpty()?"":document->dt_name+" - ",
	  document->is_drainage?"Basin":"Peak");
  GetParent()->SetWindowText(buf);
}

void CPeakNbrListView::OnEditSelectOverlaps()
{
  select_extreme_saddles(!GetDocument()->is_drainage);
}

void CPeakNbrListView::OnUpdateEditSelectOverlaps(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(center_peak!=0);
  pCmdUI->SetText(GetDocument()->is_drainage?
    "Lowest &saddles":"Highest &saddles");
}

void CPeakNbrListView::OnUpdateListLoad(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(FALSE);
}
