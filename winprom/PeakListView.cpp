// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// PeakListView.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "PeakListView.h"
#include "PromSadlListView.h"
#include "ListFrame.h"
#include <algorithm>
#include <functional>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPeakListView

IMPLEMENT_DYNCREATE(CPeakListView, CFeatureListView)

CPeakListView::CPeakListView()
{
}

CPeakListView::~CPeakListView()
{
}

bool CPeakListView::is_initial;
ListFilter CPeakListView::init_lf(true);

BEGIN_MESSAGE_MAP(CPeakListView, CFeatureListView)
  //{{AFX_MSG_MAP(CPeakListView)
  ON_WM_CREATE()
  ON_UPDATE_COMMAND_UI(ID_LIST_SADL, OnUpdateListSadl)
  ON_COMMAND(ID_LIST_SADL, OnListSadl)
  //}}AFX_MSG_MAP
  ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnHeaderClick)
  ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnUnlistItem)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPeakListView diagnostics

#ifdef _DEBUG
void CPeakListView::AssertValid() const
{
  CFeatureListView::AssertValid();
}

void CPeakListView::Dump(CDumpContext& dc) const
{
  CFeatureListView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CPeakListView message handlers

void CPeakListView::init_list(CPromDoc *document)
{
  is_initial=CListFilter_dlg(document->data,init_lf,document->is_drainage?IDI_BASIN:IDI_PEAK).DoModal()==IDOK;
  if (!is_initial) return;

  CListFrame::client_width=init_lf.offmap?144:88;
  CSingleDocTemplate& PeakListViewTpl=CPromApp::getApp()->PeakListViewTpl;
  CListFrame *newlist=(CListFrame *)PeakListViewTpl.CreateNewFrame(document,NULL);
  PeakListViewTpl.InitialUpdateFrame(newlist,document);
}

int CPeakListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CFeatureListView::OnCreate(lpCreateStruct) == -1)
    return -1;

  InsertColumn(PROM_COL,"Prominence",LVCFMT_CENTER,88);
  if (init_lf.offmap)
    InsertColumn(OMP_COL,"OM Prom",LVCFMT_RIGHT,56);

  return 0;
}

void CPeakListView::UpdateItem(int nItem)
{
  CListCtrl& listCtrl=GetListCtrl();
  CFeatureListView::UpdateItem(nItem);
  peak_list[nItem].prom.sprint(buf);
  listCtrl.SetItemText(nItem,PROM_COL,buf);
  if (list_filter.offmap) {
    if (peak_list[nItem].offmap_prom*(GetDocument()->drain_const()) < elev_infty)
      sprintf(buf,Elev_intvl::elem_format,peak_list[nItem].offmap_prom);
    else buf[0]='\0';
    listCtrl.SetItemText(nItem,OMP_COL,buf);
  }
}

void CPeakListView::printFeature(int item, FILE *out_file, Format_style style)
{
  if (style==FMS_HTML) fprintf(out_file,"<TD align=center>");
  peak_list[item].prom.print(out_file);
  if (list_filter.offmap) {
    fprintf(out_file,get_delimiter(style,CA_RIGHTx));
    if (peak_list[item].offmap_prom==elev_infty)
      fprintf(out_file,"     ");
    else fprintf(out_file,Elev_intvl::elem_format,peak_list[item].offmap_prom);
  }
  if (style!=FMS_HTML) fprintf(out_file,get_delimiter(style,CA_CENTERx));
  CFeatureListView::printFeature(item,out_file,style);
}

void CPeakListView::printHeader(FILE *out_file, Format_style style)
{
  if (style==FMS_HTML) fprintf(out_file,"<TD align=center>");
  fprintf(out_file,Elev_intvl::field_format,Domain::prom_hdr);
  if (list_filter.offmap) {
    fprintf(out_file,get_delimiter(style,CA_CENTERx));
    fprintf(out_file,"  OMP");
  }
  if (style!=FMS_HTML) fprintf(out_file,get_delimiter(style,CA_CENTERx));
  CFeatureListView::printHeader(out_file,style);
}

bool CPeakListView::CanPaste(const Clip& clp) const
{
  unsigned n=clp.ftr_count();
  for (unsigned i=0; i<n; ++i)
    if (clp.type[i]!=FT_PEAK) return false;
  return true;
}

bool CPeakListView::prelist()
{
  bool have_omp_col=list_filter.offmap; // save old value
  CListFilter_dlg dlg(GetDocument()->data,list_filter,
    GetDocument()->is_drainage?IDI_BASIN:IDI_PEAK,GetParent());
  if (dlg.DoModal()!=IDOK) return false;

  // Did we have an "OM Prom" column?
  if (have_omp_col) {
    // yes.  Get rid of it if desired.
    if (!list_filter.offmap)
      DeleteColumn(OMP_COL);
  }
  else {
    // Nope.  Add it if desired.
    if (list_filter.offmap)
      InsertColumn(OMP_COL,"OM Prom",LVCFMT_RIGHT,56);
  }

  return true;
}

void CPeakListView::OnUnlistItem(NMHDR *pNMHDR, LRESULT *pResult)
{
  assert(GetListCtrl().GetItemCount()==peak_list.size());
  int item=((NM_LISTVIEW *)pNMHDR)->iItem;
  peak_list.erase(peak_list.begin()+item);
	
  *pResult = 0;
}

void CPeakListView::OnInitialUpdate() 
{
  BeginWaitCursor();

  CFeatureListView::OnInitialUpdate();

  if (is_initial) {
    list_filter=init_lf;
    is_initial=false;
  }

  CPromDoc *doc=GetDocument();
  Divide_tree& tree=doc->tree;
  doc->tree_flip_drain();
  doc->update_peak_prom_info();
  Topo_info *min_prom_island=tree.get_island_tree(HI_END);
  Elevation *min_pk_prom=tree.min_peak_prom(min_prom_island);
  delete[] min_prom_island;
  const Database& data=doc->data;
  if (doc->is_drainage) list_filter.elev.transform(-1,0);
  Domain::Index i,n=tree.n_dom();
  Elev_intvl prom;
  peak_list.clear();

  for (i=0; i<n; ++i) {
    Domain& dom=tree[i];
#ifndef NDEBUG // include this to test global prom against PE prom
    if (dom.peak.location) {
      if (dom.prom.peak!=dom.extreme_pk_prom(LO_END) ||
	  min_pk_prom[i]!=dom.extreme_pk_prom(HI_END)) {
	Sleep(4);
	TRACE("%6d    %5d  %5d    %5d  %5d    ",i,
	      dom.prom.peak,dom.extreme_pk_prom(LO_END),
	      min_pk_prom[i],dom.extreme_pk_prom(HI_END));
	dom.peak.trace();
	TRACE("\n");
      }
    }
#endif
    if (dom.peak.location && list_filter.test(dom.peak,data)) {
      if (dom.prom.peak>=list_filter.min_prom) {
	prom.set(min_pk_prom[i],dom.prom.peak);
	peak_list.push_back(PromListItem(dom.peak,prom));
	if (list_filter.offmap) {
	  peak_list.back().offmap_prom=dom.prom.pk_omp;
	}
      }
    }
  }
  delete[] min_pk_prom;

  sort(peak_list.begin(),peak_list.end(),greater<PromListItem>());
  GetListCtrl().SetItemCount(peak_list.size());
  doc->tree_flip_drain();
  if (doc->is_drainage) list_filter.elev.transform(-1,0);

  for (vector<PromListItem>::iterator vi=peak_list.begin();
       vi!=peak_list.end(); ++vi) {
    if (doc->is_drainage)
      (*vi).transform(-1);
    AddItem(*(*vi).feature,FT_PEAK);
  }

  setItemIndicators();

  EndWaitCursor();
}

void CPeakListView::SetViewTitle()
{
  CPromDoc *document=GetDocument();
  sprintf(buf,"%s%s",
	  document->dt_name.IsEmpty()?"":document->dt_name+" - ",
	  document->is_drainage?"Basins":"Peaks");
  GetParent()->SetWindowText(buf);
  GetParent()->SetIcon(AfxGetApp()->
    LoadIcon(document->is_drainage?IDI_BASIN:IDI_PEAK),TRUE);
}

void CPeakListView::pasteClip(const Clip& clp)
{
  Elev_intvl prom;
  Feature *new_peak;
  CPromDoc *doc=GetDocument();
  list_fmt.set(doc->data);
  Domain *dom;
  doc->tree_flip_drain();
  int old_size=GetListCtrl().GetItemCount();
  unsigned i,n=clp.ftr_count();
  for (i=0; i<n; ++i) {
    new_peak=clp.list[i];
    dom=Domain::FromPeak(new_peak);
    prom=dom->peak_prom();
    peak_list.push_back(PromListItem(*new_peak,prom));
    if (list_filter.offmap)
      peak_list.back().offmap_prom=dom->offmap_pk_prom();
    if (doc->is_drainage)
      peak_list.back().transform(-1);
    AddBlankItem(*new_peak,clp.type[i]);
  }
  doc->tree_flip_drain();
  for (i=0; i<n; ++i)
    UpdateItem(i+old_size);
  setItemIndicators();
}

void CPeakListView::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  if ((Feature *)lHint==&CPromDoc::dummyModifyTrans) {
    CPromDoc *doc=GetDocument();
    const float& m=doc->trans_m;
    for (vector<PromListItem>::iterator vi=peak_list.begin();
	 vi!=peak_list.end(); ++vi)
      (*vi).transform(m);
  }

  CFeatureListView::OnUpdate(pSender,lHint,pHint);
}

static int compare_dir;

static int CALLBACK compare_prom(LPARAM p1, LPARAM p2, LPARAM plist)
{
  CPeakListView *list=(CPeakListView *)plist;
  Elev_endpt cmp_endpt=list->GetDocument()->outer_endpt();
  return (list->peak_list[p2].prom.*cmp_endpt-list->peak_list[p1].prom.*cmp_endpt)
    *(list->GetDocument()->drain_const())*compare_dir;
}

static int CALLBACK compare_omp(LPARAM p1, LPARAM p2, LPARAM plist)
{
  CPeakListView *list=(CPeakListView *)plist;
  Elevation omp1=list->peak_list[p1].offmap_prom,
    omp2=list->peak_list[p2].offmap_prom;
  return (omp1-omp2)*(list->GetDocument()->drain_const())*compare_dir;
}

void CPeakListView::OnHeaderClick(NMHDR *pNMHDR, LRESULT *pResult)
{
  NM_LISTVIEW *pNMListView = (NM_LISTVIEW *)pNMHDR;
  CListCtrl& listCtrl=GetListCtrl();

  build_features();
  if (pNMListView->iSubItem==PROM_COL) {
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
  else {
    CFeatureListView::OnHeaderClick(pNMHDR,pResult);
  }

  vector<PromListItem> temp_list;
  int i,j,n=listCtrl.GetItemCount();
  temp_list.resize(n);
  for (i=0; i<n; ++i) {
    j=listCtrl.GetItemData(i);
    temp_list[i]=peak_list[j];
    listCtrl.SetItemData(i,(LPARAM)features[j]);
  }
  peak_list=temp_list;
}

void CPeakListView::OnListSadl()
{
  CListCtrl& listCtrl=GetListCtrl();
  if (listCtrl.GetSelectedCount()>0) {
    CFeatureListView::OnListSadl();
    return;
  }
  int n=listCtrl.GetItemCount(),i;
  CPromSadlListView::List_info peaks;
  peaks.min_prom=default_min_prom();
  peaks.store(n);
  for (i=0; i<n; ++i)
    peaks.doms[i]=Domain::FromPeak(&GetFeature(i));
  CPromSadlListView::init_list(GetDocument(),peaks,GetParent());
}

void CPeakListView::OnUpdateListSadl(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetListCtrl().GetItemCount()>0);
  pCmdUI->SetText("Prominence &saddles...");	
}

void CPeakListView::swap_items(int nItem)
{
  PromListItem temp=peak_list[nItem];
  peak_list[nItem]=peak_list[nItem+1];
  peak_list[nItem+1]=temp;
  CFeatureListView::swap_items(nItem);
}
