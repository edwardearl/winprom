// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// SaddleWalkInvView.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "SaddleWalkInvView.h"
#include "ListFrame.h"
#include "region.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSaddleWalkInvView

IMPLEMENT_DYNCREATE(CSaddleWalkInvView, CGroupListView)

CSaddleWalkInvView::CSaddleWalkInvView()
{
}

CSaddleWalkInvView::~CSaddleWalkInvView()
{
}


bool CSaddleWalkInvView::is_initial;
SWI_filter CSaddleWalkInvView::init_filter;

BEGIN_MESSAGE_MAP(CSaddleWalkInvView, CGroupListView)
  //{{AFX_MSG_MAP(CSaddleWalkInvView)
  ON_WM_CREATE()
  ON_COMMAND(ID_EDIT_SWI, OnEditSWI)
  ON_UPDATE_COMMAND_UI(ID_EDIT_SWI, OnUpdateEditSWI)
  ON_UPDATE_COMMAND_UI(ID_LIST_LOAD, OnUpdateListLoad)
  ON_UPDATE_COMMAND_UI(ID_LIST_APPEND, OnUpdateListLoad)
  //}}AFX_MSG_MAP
  ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnUnlistItem)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSaddleWalkInvView diagnostics

#ifdef _DEBUG
void CSaddleWalkInvView::AssertValid() const
{
  CGroupListView::AssertValid();
}

void CSaddleWalkInvView::Dump(CDumpContext& dc) const
{
  CGroupListView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CSaddleWalkInvView message handlers

void CSaddleWalkInvView::init_list(CPromDoc *document)
{
  is_initial=CSWI_dlg(*document,init_filter).DoModal()==IDOK;
  if (!is_initial) return;

  CListFrame::client_width=144;
  CSingleDocTemplate& SadlWalkInvViewTpl=CPromApp::getApp()->SadlWalkInvViewTpl;
  CListFrame *newlist=(CListFrame *)SadlWalkInvViewTpl.CreateNewFrame(document,NULL);
  SadlWalkInvViewTpl.InitialUpdateFrame(newlist,document);
  newlist->SetIcon(AfxGetApp()->LoadIcon(IDI_SADDLE),TRUE);
}

int CSaddleWalkInvView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CGroupListView::OnCreate(lpCreateStruct) == -1)
    return -1;

  InsertColumn(ELEV_COL,"Map Elev",LVCFMT_RIGHT,56);
  InsertColumn(SWIR_COL,"SWIR",LVCFMT_CENTER,88);

  return 0;
}

Elevation CSaddleWalkInvView::getElev(const Feature& featr) const
{
  Elev_map& elev=GetDocument()->elev;
  return elev.contains(featr.location)?elev[featr.location]:elev_undef;
}

void CSaddleWalkInvView::UpdateItem(int nItem)
{
  CGroupListView::UpdateItem(nItem);
  Feature& featr=GetFeature(nItem);
  Elevation elev=getElev(featr);
  if (elev==elev_undef) buf[0]='\0';
  else sprintf(buf,Elev_intvl::elem_format,elev);
  GetListCtrl().SetItemText(nItem,ELEV_COL,buf);
  if (saddle_list[nItem].saddle) {
    Elev_intvl swir=featr.elev-saddle_list[nItem].saddle->elev;
    if (GetDocument()->is_drainage) swir.transform(-1,0);
    swir.sprint(buf);
    GetListCtrl().SetItemText(nItem,SWIR_COL,buf);
  }
}

void CSaddleWalkInvView::printFeature(int item, FILE *out_file,
				      Format_style style)
{
  if (style==FMS_HTML) fprintf(out_file,"<TD align=right>");
  Feature& featr=GetFeature(item);
  Elevation elev=getElev(item);
  if (elev==elev_undef) fprintf(out_file,"     ");
  else fprintf(out_file,Elev_intvl::elem_format,elev);
  fprintf(out_file,get_delimiter(style,CA_RIGHTx));
  if (saddle_list[item].saddle) {
    Elev_intvl swir=featr.elev-saddle_list[item].saddle->elev;
    if (GetDocument()->is_drainage) swir.transform(-1,0);
    swir.print(out_file);
  }
  else fprintf(out_file,Elev_intvl::field_format,"");
  if (style!=FMS_HTML) fprintf(out_file,get_delimiter(style,CA_RIGHTx));
  CGroupListView::printFeature(item,out_file,style);
}

void CSaddleWalkInvView::printHeader(FILE *out_file, Format_style style)
{
  if (style==FMS_HTML) fprintf(out_file,"<TD align=right>");
  fprintf(out_file,"  ME ");
  fprintf(out_file,get_delimiter(style,CA_CENTERx));
  fprintf(out_file,Elev_intvl::field_format,"SWIR");
  if (style!=FMS_HTML) fprintf(out_file,get_delimiter(style,CA_RIGHTx));
  CGroupListView::printHeader(out_file,style);
}

void CSaddleWalkInvView::OnUnlistItem(NMHDR *pNMHDR, LRESULT *pResult)
{
  CGroupListView::OnUnlistItem(pNMHDR,pResult);
  int item=((NM_LISTVIEW *)pNMHDR)->iItem;
  assert(GetListCtrl().GetItemCount()==saddle_list.GetSize());
  saddle_list.RemoveAt(item);
}

bool CSaddleWalkInvView::prelist()
{
  return CSWI_dlg(*GetDocument(),relist_filter,GetParent()).DoModal()==IDOK;
}

void CSaddleWalkInvView::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  if ((Feature *)lHint==&CPromDoc::dummyModifyMap)
    UpdateAllItems();
  CGroupListView::OnUpdate(pSender,lHint,pHint);
}

// descriptor for downhill walk from start
class Descent {
  const Elev_map& elev;
  Bool_map& marker;
  Elevation floor;
public:
  bool visit_mark;
  Descent(const Elev_map& e, Elevation f, Bool_map& m) :
    elev(e),floor(f),marker(m),visit_mark(true) {}
  bool path(const Neighbor_iter& ni) const
    {return elev[*ni]<=elev[ni.center()];}
  bool region(const GridPoint& c) const {return elev[c]>=floor;}
  bool visited(const GridPoint& c) const {return marker[c]==visit_mark;}
  bool step(const Neighbor_iter& ni) const
    {return elev[*ni]>=floor && marker[*ni]!=visit_mark && elev[*ni]<=elev[ni.center()];}
  void visit(const GridPoint& c) const {marker[c]=visit_mark;}
  void clear() const {}
};

// check for possible SWI between /hisadl/ and /losadl/,
// where /hisadl/ is higher than /losadl/ on the elevation map,
// vice versa on the divide tree.
void CSaddleWalkInvView::check_swi_lo(bool& list_hi,
	Elevation low_start, const Elev_map& elev, const Bool_map& mark,
	Feature& hisadl, Feature& losadl, FT_index hitype, FT_index lotype)
{
  const GridPoint& loc=losadl.location;
  Domain *ca;
  CPromDoc *doc=GetDocument();
  if (loc && losadl.edited && losadl.elev.high>=low_start &&
      mark.contains(loc) && mark[loc] &&
      relist_filter.test(hisadl,losadl,elev,doc->data) &&
      // TODO: basin saddles
      (hitype!=FT_SADDLE || lotype!=FT_SADDLE ||
       Domain::SWI_switch_cxn(Domain::FromSadl(&hisadl),Domain::FromSadl(&losadl),
			      ca,doc->drain_const(),
			      doc->inner_endpt(),doc->outer_endpt())==Domain::GOOD_CXN)) {
    if (!list_hi) {
      saddle_list.Add(SWI_companion(0,FT_NUM_TYPES));
      AddBlankItem(hisadl,hitype);
      list_hi=true;
    }
    saddle_list.Add(SWI_companion(&hisadl,hitype));
    AddBlankItem(losadl,lotype);
  }
}

// Find all possible cases of SWI in which /sadl/ is higher on the map,
// but lower on the divide tree.
void CSaddleWalkInvView::check_swi(const Elev_map& elev, Bool_map& mark,
				   Divide_tree& tree, Feature& sadl, FT_index type)
{
  const GridPoint& loc=sadl.location;
  if (!(loc && sadl.edited && elev.contains(loc) &&
	relist_filter.test(sadl,GetDocument()->data)))
    return;
  Grid grid(elev);
  Domain::Index j,n=tree.n_dom(),nb=tree.n_bsnsdl();
  Elevation low_start=sadl.elev.low,swi_floor=elev_infty;
  for (j=0; j<n; ++j) {
    const Feature& losadl=tree[j].saddle;
    const GridPoint& low_loc=losadl.location;
    if (low_loc && losadl.edited && losadl.elev.high>=low_start &&
	elev.contains(low_loc) && elev[low_loc]<swi_floor)
      swi_floor=elev[low_loc];
  }
  if (swi_floor>elev[loc] ||
      (swi_floor==elev[loc] && !relist_filter.flat)) return;
  bool list_hi=false;
  Descent downhill(elev,swi_floor,mark);
  Region_iter<Descent,Grid> ri1(loc,downhill,grid);
  for (; ri1; ++ri1); // mark the downhill accessible region
  for (j=0; j<n; ++j) {
    check_swi_lo(list_hi,low_start,elev,mark,sadl,
		 tree[j].saddle,type,FT_SADDLE);
  }
  if (relist_filter.basin) {
    for (j=0; j<nb; ++j) {
      check_swi_lo(list_hi,low_start,elev,mark,sadl,
		   tree.bsnsdl(j),type,FT_BASINSADL);
    }
  }
  downhill.visit_mark=false;
  Region_iter<Descent,Grid> ri2(loc,downhill,grid);
  for (; ri2; ++ri2); // clear the marks
}

void CSaddleWalkInvView::OnInitialUpdate() 
{
  BeginWaitCursor();
  saddle_list.SetSize(0);

  CGroupListView::OnInitialUpdate();

  if (is_initial) {
    relist_filter=init_filter;
    is_initial=false;
  }

  CPromDoc *doc=GetDocument();
  Divide_tree& tree=doc->tree;
  Elev_map& elev=doc->elev;
  Bool_map mark(elev);
  mark.fill(false);
  doc->tree_flip_drain();
  doc->elev_flip_drain();
  Domain::Index i,n=tree.n_dom(),nb=tree.n_bsnsdl();

  for (i=0; i<n; ++i) {
    check_swi(elev,mark,tree,tree[i].saddle,FT_SADDLE);
  }
  if (relist_filter.basin) {
    for (i=0; i<nb; ++i) {
      check_swi(elev,mark,tree,tree.bsnsdl(i),FT_BASINSADL);
    }
  }
  doc->tree_flip_drain();
  doc->elev_flip_drain();

  UpdateAllItems();

  setItemIndicators();

  EndWaitCursor();
}

void CSaddleWalkInvView::SetViewTitle()
{
  CPromDoc *document=GetDocument();
  GetParent()->SetWindowText(document->dt_name.IsEmpty()?
    "Saddle Walk Inversions":document->dt_name+" - Saddle Walk Inversions");
}

void CSaddleWalkInvView::OnEditSWI()
{
  CListCtrl& listCtrl=GetListCtrl();
  if (listCtrl.GetSelectedCount()==1) {
    int item=listCtrl.GetNextItem(-1,LVNI_SELECTED);
    FT_index type1=GetFeatureType(item),type2=saddle_list[item].type;
    Feature &sadl1=GetFeature(item),&sadl2=*saddle_list[item].saddle;
    CPromDoc *doc=GetDocument();
    CWnd *pw=GetParent();
    if (type1==FT_SADDLE && type2==FT_SADDLE)
      doc->SWI_switch(sadl1,sadl2,pw);
    else if (type1==FT_SADDLE && type2==FT_BASINSADL)
      doc->SWI_switch(sadl1,(Basin_saddle&)sadl2,pw);
    else if (type1==FT_BASINSADL && type2==FT_SADDLE)
      doc->SWI_switch((Basin_saddle&)sadl1,sadl2,pw);
    else if (type1==FT_BASINSADL && type2==FT_BASINSADL)
      doc->SWI_switch((Basin_saddle&)sadl1,(Basin_saddle&)sadl2,pw);
    else assert(0);
  }
  else CGroupListView::OnEditSWI();
}

void CSaddleWalkInvView::OnUpdateEditSWI(CCmdUI* pCmdUI) 
{
  CListCtrl& listCtrl=GetListCtrl();
  if (listCtrl.GetSelectedCount()==1) {
    int item=listCtrl.GetNextItem(-1,LVNI_SELECTED);
    pCmdUI->Enable(saddle_list[item].saddle!=0);
  }
  else CGroupListView::OnUpdateEditSWI(pCmdUI);
}

void CSaddleWalkInvView::OnUpdateListLoad(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(FALSE);
}
