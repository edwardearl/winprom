// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// BasinSadlListView.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "BasinSadlListView.h"
#include "CycleListView.h"
#include "ListFrame.h"
#include <algorithm>
#include <functional>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;


/////////////////////////////////////////////////////////////////////////////
// CBasinSadlListView

IMPLEMENT_DYNCREATE(CBasinSadlListView, CFeatureListView)

CBasinSadlListView::CBasinSadlListView()
{
}

CBasinSadlListView::~CBasinSadlListView()
{
}

bool CBasinSadlListView::is_initial;
SadlListFilter CBasinSadlListView::init_lf;

BEGIN_MESSAGE_MAP(CBasinSadlListView, CFeatureListView)
  //{{AFX_MSG_MAP(CBasinSadlListView)
  ON_WM_CREATE()
  ON_COMMAND(ID_LIST_SADL, OnListSadl)
  ON_UPDATE_COMMAND_UI(ID_LIST_SADL, OnUpdateListSadl)
  //}}AFX_MSG_MAP
  ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnHeaderClick)
  ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnUnlistItem)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBasinSadlListView diagnostics

#ifdef _DEBUG
void CBasinSadlListView::AssertValid() const
{
  CFeatureListView::AssertValid();
}

void CBasinSadlListView::Dump(CDumpContext& dc) const
{
  CFeatureListView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CBasinSadlListView message handlers

void CBasinSadlListView::init_list(CPromDoc *document)
{
  CListSadl_dlg dlg(document->data,init_lf,
    document->is_drainage?IDI_SADDLE:IDI_BASINSADL);
  is_initial=dlg.DoModal()==IDOK;
  if (!is_initial) return;

  CListFrame::client_width=init_lf.offmap?328:272;
  CSingleDocTemplate& BasinSadlListViewTpl=CPromApp::getApp()->BasinSadlListViewTpl;
  CListFrame *newlist=(CListFrame *)BasinSadlListViewTpl.CreateNewFrame(document,NULL);
  BasinSadlListViewTpl.InitialUpdateFrame(newlist,document);
}

int CBasinSadlListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFeatureListView::OnCreate(lpCreateStruct) == -1)
    return -1;

  InsertColumn(CWR_COL,"CWR",LVCFMT_RIGHT,48);
  InsertColumn(CWRO_COL,"CWRO",LVCFMT_RIGHT,48);
  InsertColumn(CYCLE_COL,"Cycle",LVCFMT_CENTER,40);
  InsertColumn(STAT_COL,"Stat",LVCFMT_CENTER,48);
  InsertColumn(PROM_COL,"Prominence",LVCFMT_CENTER,88);
  if (init_lf.offmap)
    InsertColumn(OMP_COL,"OM Prom",LVCFMT_RIGHT,56);

  return 0;
}

void CBasinSadlListView::UpdateItem(int nItem)
{
  CListCtrl& listCtrl=GetListCtrl();
  CFeatureListView::UpdateItem(nItem);
  bs_list[nItem].prom.sprint(buf);
  listCtrl.SetItemText(nItem,PROM_COL,buf);
  if (list_filter.offmap) {
    Elevation omp=bs_list[nItem].offmap_prom*(GetDocument()->drain_const());
    if (omp==elev_infty) strcpy(buf,"unlimited");
    else if (omp>-elev_infty)
      sprintf(buf,Elev_intvl::elem_format,bs_list[nItem].offmap_prom);
    else buf[0]='\0';
    listCtrl.SetItemText(nItem,OMP_COL,buf);
  }

  short int dc=GetDocument()->drain_const();
  Basin_saddle& bs=(Basin_saddle &)GetFeature(nItem);
  Basin_saddle::Cycle_walk_info cwinfo;
  bs.get_walk_info(cwinfo,dc);
  Elevation walk_risk=dc*bs.walk_risk(cwinfo),wro=dc*bs.WR_offense(cwinfo);
  if (walk_risk==-elev_infty)
    sprintf(buf,"%s",Basin_saddle::WS_string);
  else sprintf(buf,Elev_intvl::elem_format,walk_risk);
  listCtrl.SetItemText(nItem,CWR_COL,buf);
  if (wro==-elev_infty)
    sprintf(buf,"%s",Basin_saddle::WS_string);
  else sprintf(buf,Elev_intvl::elem_format,wro);
  listCtrl.SetItemText(nItem,CWRO_COL,buf);
  sprintf(buf,"%d",bs.cycle);
  listCtrl.SetItemText(nItem,CYCLE_COL,buf);
  listCtrl.SetItemText(nItem,STAT_COL,Saddle::status_name[bs.status]);
}

void CBasinSadlListView::printFeature(int item, FILE *out_file,
				      Format_style style)
{
  if (style==FMS_HTML) fprintf(out_file,"<TD align=center>");
  bs_list[item].prom.print(out_file);
  if (list_filter.offmap) {
    fprintf(out_file,get_delimiter(style,CA_RIGHTx));
    if (bs_list[item].offmap_prom==elev_infty)
      fprintf(out_file,"unltd");
    else if (bs_list[item].offmap_prom==-elev_infty)
      fprintf(out_file,"     ");
    else fprintf(out_file,Elev_intvl::elem_format,bs_list[item].offmap_prom);
  }
  if (style!=FMS_HTML) fprintf(out_file,get_delimiter(style,CA_RIGHTx));
  switch (style) {
  case FMS_FLAG: Basin_saddle::format="%W %O %C B %F"; break;
  case FMS_HTML: Basin_saddle::format=
	"<TD align=right>%W<TD align=right>%O<TD align=right>%C<TD align=center>B%F"; break;
  case FMS_CSV: Basin_saddle::format="%W,%O,%C,B,%F"; break;
  }
  Basin_saddle& bs=(Basin_saddle&)GetFeature(item);
  Basin_saddle::Cycle_walk_info cwinfo;
  short int dc=GetDocument()->drain_const();
  bs.get_walk_info(cwinfo,dc);
  bs.print(cwinfo,dc,out_file);
}

void CBasinSadlListView::printHeader(FILE *out_file, Format_style style)
{
  if (style==FMS_HTML) fprintf(out_file,"<TD align=center>");
  fprintf(out_file,Elev_intvl::field_format,Domain::prom_hdr);
  if (list_filter.offmap) {
    fprintf(out_file,get_delimiter(style,CA_CENTERx));
    fprintf(out_file,"  OMP");
  }
  if (style!=FMS_HTML) fprintf(out_file,get_delimiter(style,CA_CENTERx));
  switch (style) {
  case FMS_FLAG: Basin_saddle::format="%W %O %C T %F"; break;
  case FMS_HTML: Basin_saddle::format=
	"<TD align=center>%W<TD align=center>%O<TD align=center>%C<TD>Type%F"; break;
  case FMS_CSV: Basin_saddle::format="%W,%O,%C,Type,%F"; break;
  }
  Basin_saddle::print_header(out_file);
}

bool CBasinSadlListView::CanPaste(const Clip& clp) const
{
  unsigned n=clp.ftr_count();
  for (unsigned i=0; i<n; ++i)
    if (clp.type[i]!=FT_BASINSADL) return false;
  return true;
}

bool CBasinSadlListView::prelist()
{
  bool have_omp_col=list_filter.offmap; // save old value
  CListSadl_dlg dlg(GetDocument()->data,list_filter,
    GetDocument()->is_drainage?IDI_SADDLE:IDI_BASINSADL,GetParent());
  if (dlg.DoModal()!=IDOK) return false;

  // Did we have an "OM Prom" column?
  if (have_omp_col) {
    // yes. Get rid of it if desired.
    if (!list_filter.offmap)
      DeleteColumn(OMP_COL);
  }
  else {
    // Nope. Add it if desired.
    if (list_filter.offmap)
      InsertColumn(OMP_COL,"OM Prom",LVCFMT_CENTER,56);
  }

  return true;
}

void CBasinSadlListView::OnUnlistItem(NMHDR *pNMHDR, LRESULT *pResult)
{
  assert(GetListCtrl().GetItemCount()==bs_list.size());
  int item=((NM_LISTVIEW *)pNMHDR)->iItem;
  bs_list.erase(bs_list.begin()+item);
	
  *pResult = 0;
}

void CBasinSadlListView::OnInitialUpdate()
{
  BeginWaitCursor();

  CFeatureListView::OnInitialUpdate();

  if (is_initial) {
    list_filter=init_lf;
    is_initial=false;
  }

  CPromDoc *doc=GetDocument();
  Divide_tree& tree=doc->tree;
  const Database& data=doc->data;
  Domain::Index i,nbs=tree.n_bsnsdl(),ndom=tree.n_dom();
  Elev_intvl bs_prom;
  Elevation bs_omp=-elev_infty;
  bs_list.clear();
  doc->tree_flip_drain();
  if (doc->is_drainage) list_filter.elev.transform(-1,0);
  doc->update_sadl_prom_info();
  if (list_filter.ridge) doc->update_ridge_info();
  const Domain *dom_base=&tree[0];
#ifndef DEBUG
  TRACE("           --- ON ---      --- OFF ---\n");
  TRACE(" Index      IC     PE       IC     PE\n");
#endif

  for (i=0; i<nbs; ++i) {
    Basin_saddle& bs=tree.bsnsdl(i);
#ifndef NDEBUG // include this to test global prom against PE prom
    if (bs.location) {
      const Domain::Sadl_prom& bsp=bs.prom;
      bs_prom.high=bs.extreme_prom(HI_END);
      bs_omp=bs.offmap_prom();
      if (bs_omp==-elev_infty) bs_omp=bs_prom.high;
      if (bsp.onmap!=bs_prom.high || bsp.offmap!=bs_omp) {
	Sleep(4);
	TRACE("%6d    %5d  %5d    %5d  %5d    ",
	      i,bsp.onmap,bs_prom.high,bsp.offmap,bs_omp);
	bs.trace();
	TRACE("\n");
      }
      bs_omp=-elev_infty;
    }
#endif
    if (!bs.location || !list_filter.test(bs,data) ||
	list_filter.min_prom>0 && !bs.can_rotate()) continue;
    list_filter.get_prom(bs.prom,bs_prom.high,bs_omp);
    if (bs_prom.high>=list_filter.min_prom || bs_omp>=list_filter.min_prom ||
	list_filter.min_prom==0) {
      bs_prom.low=bs.extreme_prom(LO_END);
      bs_list.push_back(PromListItem(bs,bs_prom));
      if (list_filter.offmap) {
	bs_list.back().offmap_prom=bs_omp;
      }
    }
  }

  sort(bs_list.begin(),bs_list.end(),greater<PromListItem>());
  GetListCtrl().SetItemCount(bs_list.size());
  doc->tree_flip_drain();
  if (doc->is_drainage) list_filter.elev.transform(-1,0);

  for (vector<PromListItem>::iterator vi=bs_list.begin();
       vi!=bs_list.end(); ++vi) {
    if (doc->is_drainage)
      (*vi).transform(-1);
    AddItem(*(*vi).feature,FT_BASINSADL);
  }

  setItemIndicators();

  EndWaitCursor();
}

void CBasinSadlListView::SetViewTitle()
{
  CPromDoc *document=GetDocument();
  sprintf(buf,"%s%s Saddles",
	  document->dt_name.IsEmpty()?"":document->dt_name+" - ",
	  document->is_drainage?"Prominence":"Basin");
  GetParent()->SetWindowText(buf);
  GetParent()->SetIcon(AfxGetApp()->
    LoadIcon(document->is_drainage?IDI_SADDLE:IDI_BASINSADL),TRUE);
}

void CBasinSadlListView::pasteClip(const Clip& clp)
{
  Elev_intvl prom;
  Elevation omp;
  CPromDoc *doc=GetDocument();
  Basin_saddle *new_bs;
  list_fmt.set(GetDocument()->data);
  doc->tree_flip_drain();
  int old_size=GetListCtrl().GetItemCount();
  unsigned i,n=clp.ftr_count();
  doc->update_sadl_prom_info();
  if (list_filter.ridge) doc->update_ridge_info();
  for (i=0; i<n; ++i) {
    new_bs=(Basin_saddle *)clp.list[i];
    list_filter.get_prom(new_bs->prom,prom.high,omp);
    prom.low=new_bs->extreme_prom(LO_END);
    bs_list.push_back(PromListItem(*new_bs,prom));
    if (list_filter.offmap)
      bs_list.back().offmap_prom=omp;
    if (doc->is_drainage)
      bs_list.back().transform(-1);
    AddBlankItem(*new_bs,clp.type[i]);
  }
  doc->tree_flip_drain();
  for (i=0; i<n; ++i)
    UpdateItem(i+old_size);
  setItemIndicators();
}

void CBasinSadlListView::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  if ((Feature *)lHint==&CPromDoc::dummyModifyTrans) {
    CPromDoc *doc=GetDocument();
    const float& m=doc->trans_m;
    for (vector<PromListItem>::iterator vi=bs_list.begin();
	 vi!=bs_list.end(); ++vi)
      (*vi).transform(m);
  }

  CFeatureListView::OnUpdate(pSender,lHint,pHint);
}

static int compare_dir;

static int CALLBACK compare_stat(LPARAM p1, LPARAM p2, LPARAM plist)
{
  CBasinSadlListView *list=(CBasinSadlListView *)plist;
  int stat1=((const Saddle *)(list->bs_list[p1].feature))->status,
    stat2=((const Saddle *)(list->bs_list[p2].feature))->status;
  return (stat2-stat1)*compare_dir;
}

static int CALLBACK compare_prom(LPARAM p1, LPARAM p2, LPARAM plist)
{
  CBasinSadlListView *list=(CBasinSadlListView *)plist;
  Elev_endpt cmp_endpt=list->GetDocument()->outer_endpt();
  return (list->bs_list[p2].prom.*cmp_endpt-list->bs_list[p1].prom.*cmp_endpt)
    *(list->GetDocument()->drain_const())*compare_dir;
}

static int CALLBACK compare_omp(LPARAM p1, LPARAM p2, LPARAM plist)
{
  CBasinSadlListView *list=(CBasinSadlListView *)plist;
  Elevation omp1=list->bs_list[p1].offmap_prom,
    omp2=list->bs_list[p2].offmap_prom;
  return (omp2-omp1)*(list->GetDocument()->drain_const())*compare_dir;
}

static int CALLBACK compare_cycle(LPARAM p1, LPARAM p2, LPARAM plist)
{
  CBasinSadlListView *list=(CBasinSadlListView *)plist;
  return (((Basin_saddle *)list->features[p1])->cycle-
    ((Basin_saddle *)list->features[p2])->cycle)*compare_dir;
}

static int CALLBACK compare_cwr(LPARAM p1, LPARAM p2, LPARAM plist)
{
  CBasinSadlListView *list=(CBasinSadlListView *)plist;
  short int dc=list->GetDocument()->drain_const();
  Basin_saddle::Cycle_walk_info cw1,cw2;
  const Basin_saddle &bs1=(Basin_saddle&)list->features[p1],
    &bs2=(Basin_saddle&)list->features[p2];
  return (bs1.walk_risk(cw1)-bs2.walk_risk(cw2))*compare_dir;
}

static int CALLBACK compare_cwro(LPARAM p1, LPARAM p2, LPARAM plist)
{
  CBasinSadlListView *list=(CBasinSadlListView *)plist;
  short int dc=list->GetDocument()->drain_const();
  Basin_saddle::Cycle_walk_info cw1,cw2;
  const Basin_saddle &bs1=(Basin_saddle&)list->features[p1],
    &bs2=(Basin_saddle&)list->features[p2];
  return (bs1.WR_offense(cw1)-bs2.WR_offense(cw2))*compare_dir;
}

void CBasinSadlListView::OnHeaderClick(NMHDR *pNMHDR, LRESULT *pResult)
{
  NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
  CListCtrl& listCtrl=GetListCtrl();

  build_features();
  if (pNMListView->iSubItem==STAT_COL) {
    // sort by status
    compare_dir=GetKeyState(VK_SHIFT)<0 ? -1 : 1;
    listCtrl.SortItems(compare_stat,(LPARAM)this);
    *pResult = 0;
  }
  else if (pNMListView->iSubItem==PROM_COL) {
    // sort by prominence
    compare_dir=GetKeyState(VK_SHIFT)<0 ? -1 : 1;
    listCtrl.SortItems(compare_prom,(LPARAM)this);
    *pResult = 0;
  }
  else if (list_filter.offmap && pNMListView->iSubItem==OMP_COL) {
    // sort by off-map prominence
    compare_dir=GetKeyState(VK_SHIFT)<0 ? -1 : 1;
    listCtrl.SortItems(compare_omp,(LPARAM)this);
    *pResult = 0;
  }
  else if (pNMListView->iSubItem==CYCLE_COL) {
    // sort by cycle length
    compare_dir=GetKeyState(VK_SHIFT)<0 ? -1 : 1;
    listCtrl.SortItems(compare_cycle,(LPARAM)this);
    *pResult = 0;
  }
  else if (pNMListView->iSubItem==CWR_COL) {
    // sort by cycle walk risk
    compare_dir=GetKeyState(VK_SHIFT)<0 ? -1 : 1;
    listCtrl.SortItems(compare_cwr,(LPARAM)this);
    *pResult = 0;
  }
  else if (pNMListView->iSubItem==CWRO_COL) {
    // sort by cycle walk risk offense
    compare_dir=GetKeyState(VK_SHIFT)<0 ? -1 : 1;
    listCtrl.SortItems(compare_cwro,(LPARAM)this);
    *pResult = 0;
  }
  else {
    CFeatureListView::OnHeaderClick(pNMHDR,pResult);
  }

  vector<PromListItem> temp_list;
  int i,j,n=listCtrl.GetItemCount();
  temp_list.resize(n);
  for (i=0; i<n; ++i) {
    j=listCtrl.GetItemData(i);
    temp_list[i]=bs_list[j];
    listCtrl.SetItemData(i,(LPARAM)features[j]);
  }
  bs_list=temp_list;
}

void CBasinSadlListView::OnListSadl() 
{
  CListCtrl& listCtrl=GetListCtrl();
  if (listCtrl.GetSelectedCount()>0) {
    CFeatureListView::OnListSadl();
    return;
  }
  int n=listCtrl.GetItemCount(),i;
  CycleFilter bs;
  bs.store(n);
  bs.min_prom=default_min_prom();
  bs.offmap=default_offmap();
  bs.ridge=default_ridge();
  bs.cell=default_cell();
  for (i=0; i<n; ++i)
    bs.bs[i]=(Basin_saddle *)&GetFeature(i);
  CCycleListView::init_list(GetDocument(),bs,GetParent());
}

void CBasinSadlListView::OnUpdateListSadl(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetListCtrl().GetItemCount()>0);
  pCmdUI->SetText("&Cycle...");
}

void CBasinSadlListView::swap_items(int nItem)
{
  PromListItem temp=bs_list[nItem];
  bs_list[nItem]=bs_list[nItem+1];
  bs_list[nItem+1]=temp;
  CFeatureListView::swap_items(nItem);
}
