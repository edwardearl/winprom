// PromSadlListView.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "PromSadlListView.h"
#include "WalkListView.h"
#include "ListFrame.h"
#include "PromSadl_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CPromSadlListView::List_info::List_info(Domain& dom)
  : doms(0),UPE_col(false)
{
  store(1);
  doms[0]=&dom;
}

void CPromSadlListView::List_info::store(Domain::Index count)
{
  delete[] doms;
  doms=new Domain *[ndom=count];
}

void CPromSadlListView::List_info::transfer(List_info& a)
{
  delete[] doms;
  *this=a;
  a.doms=0;
  a.ndom=0;
}


/////////////////////////////////////////////////////////////////////////////
// CPromSadlListView

IMPLEMENT_DYNCREATE(CPromSadlListView, CGroupListView)

CPromSadlListView::CPromSadlListView()
{
}

CPromSadlListView::~CPromSadlListView()
{
}

CPromSadlListView::List_info CPromSadlListView::init_info;

BEGIN_MESSAGE_MAP(CPromSadlListView, CGroupListView)
  //{{AFX_MSG_MAP(CPromSadlListView)
  ON_WM_CREATE()
  ON_COMMAND(ID_LIST_WALK, OnListWalk)
  ON_UPDATE_COMMAND_UI(ID_LIST_WALK, OnUpdateListWalk)
  //}}AFX_MSG_MAP
  ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnUnlistItem)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPromSadlListView diagnostics

#ifdef _DEBUG
void CPromSadlListView::AssertValid() const
{
  CGroupListView::AssertValid();
}

void CPromSadlListView::Dump(CDumpContext& dc) const
{
  CGroupListView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CPromSadlListView message handlers

void CPromSadlListView::init_list(CPromDoc *doc, Domain& peak, CWnd *parent,
				  Elevation min_prom)
{
  List_info peak1(peak);
  peak1.min_prom=min_prom;
  init_list(doc,peak1,parent);
}

void CPromSadlListView::init_list(CPromDoc *doc, List_info& peaks, CWnd *parent)
{
  assert(peaks.min_prom>=0);
  peaks.UPE_col=init_info.UPE_col;
  peaks.saddle_filter=init_info.saddle_filter;
  peaks.runoff_filter=init_info.runoff_filter;
  peaks.off_sadls=init_info.off_sadls;
  peaks.runoffs=init_info.runoffs;
  peaks.basinsadls=init_info.basinsadls;
  CPromSadl_dlg dlg(*doc,peaks,parent);
  dlg.m_UPE=peaks.UPE_col;
  if (dlg.DoModal()!=IDOK) return;
  peaks.UPE_col=dlg.m_UPE!=FALSE;
  init_info.transfer(peaks);

  CListFrame::client_width=176;
  if (init_info.UPE_col) CListFrame::client_width+=88;
  CSingleDocTemplate& PromSadlListViewTpl=CPromApp::getApp()->PromSadlListViewTpl;
  CListFrame *newlist=(CListFrame *)PromSadlListViewTpl.CreateNewFrame(doc,NULL);
  PromSadlListViewTpl.InitialUpdateFrame(newlist,doc);
  newlist->SetIcon(AfxGetApp()->
    LoadIcon(doc->is_drainage?IDI_BASINSADL_LIST:IDI_PROMSADL_LIST),TRUE);
}

int CPromSadlListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CGroupListView::OnCreate(lpCreateStruct) == -1)
    return -1;

  InsertColumn(PROM_COL,"Prominence",LVCFMT_CENTER,88);
  if (init_info.UPE_col) {
    InsertColumn(UPE_col=PROM_COL+1,"UPE",LVCFMT_CENTER,88);
    USE_col=UPE_col+1;
  }
  else {
    UPE_col=0;
    USE_col=PROM_COL+1;
  }
  InsertColumn(USE_col,"USE",LVCFMT_CENTER,88);

  return 0;
}

void CPromSadlListView::UpdateItem(int nItem)
{
  CGroupListView::UpdateItem(nItem);
  CListCtrl& listCtrl=GetListCtrl();
  saddle_list[nItem].prom.sprint(buf);
  listCtrl.SetItemText(nItem,PROM_COL,buf);
  if (UPE_col!=0) {
    saddle_list[nItem].usable_peak_elev.sprint(buf);
    listCtrl.SetItemText(nItem,UPE_col,buf);
  }
  saddle_list[nItem].usable_sadl_elev.sprint(buf);
  listCtrl.SetItemText(nItem,USE_col,buf);
}

void CPromSadlListView::printFeature(int item, FILE *out_file,
				     Format_style style)
{
  const char *style_col=get_delimiter(style,CA_CENTERx);
  if (style==FMS_HTML) fprintf(out_file,style_col);
  saddle_list[item].prom.print(out_file);
  if (UPE_col) {
    fprintf(out_file,style_col);
    saddle_list[item].usable_peak_elev.print(out_file);
  }
  fprintf(out_file,style_col);
  saddle_list[item].usable_sadl_elev.print(out_file);
  if (style!=FMS_HTML) fprintf(out_file,style_col);
  CGroupListView::printFeature(item,out_file,style);
}

void CPromSadlListView::printHeader(FILE *out_file, Format_style style)
{
  const char *style_col=get_delimiter(style,CA_CENTERx);
  if (style==FMS_HTML) fprintf(out_file,style_col);
  fprintf(out_file,Elev_intvl::field_format,Domain::prom_hdr);
  if (UPE_col) {
    fprintf(out_file,style_col);
    fprintf(out_file,Elev_intvl::field_format,Domain::usable_peak_hdr);
  }
  fprintf(out_file,style_col);
  fprintf(out_file,Elev_intvl::field_format,Domain::usable_saddle_hdr);
  if (style!=FMS_HTML) fprintf(out_file,style_col);
  CGroupListView::printHeader(out_file,style);
}

void CPromSadlListView::OnUnlistItem(NMHDR *pNMHDR, LRESULT *pResult)
{
  CGroupListView::OnUnlistItem(pNMHDR,pResult);
  int item=((NM_LISTVIEW *)pNMHDR)->iItem;
  assert(GetListCtrl().GetItemCount()==saddle_list.GetSize());
  saddle_list.RemoveAt(item);
}

bool CPromSadlListView::prelist()
{
  CPromSadl_dlg dlg(*GetDocument(),relist_info,GetParent());
  dlg.m_UPE=relist_info.UPE_col;
  if (dlg.DoModal()!=IDOK) return false;

  // Did we have a UPE column?
  if (relist_info.UPE_col) {
    // yes. Get rid of it if desired
    if (!dlg.m_UPE) {
      DeleteColumn(UPE_col);
      --USE_col;
      UPE_col=0;
    }
  }
  else {
    // Nope. Add it if desired.
    if (dlg.m_UPE) {
      InsertColumn(UPE_col=PROM_COL+1,"UPE",LVCFMT_CENTER,88);
      ++USE_col;
    }
  }
  relist_info.UPE_col=dlg.m_UPE!=FALSE;

  return true;
}

void CPromSadlListView::compress_master_list()
{
  // Remove deleted peaks from the master peak list.
  int i,j;
  for (i=j=0; i<relist_info.ndom; ++i) {
    if (relist_info.doms[i]->peak.location) {
      if (i>j) relist_info.doms[j]=relist_info.doms[i];
      ++j;
    }
  }
  relist_info.ndom=j;
}

void CPromSadlListView::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint) 
{
  if (lHint==0) {
    delete[] relist_info.doms;
    relist_info.doms=0;
    relist_info.ndom=0;
  }
  else {
    Feature *changed_feature=(Feature *)lHint;
    if (changed_feature==&CPromDoc::dummyModifyData) return;
    if (changed_feature==&CPromDoc::dummyModifyTopo ||
	changed_feature==&CPromDoc::dummyModifySplice) {
      if (changed_feature==&CPromDoc::dummyModifySplice)
	compress_master_list();
      GetListCtrl().DeleteAllItems();
      OnInitialUpdate();
      return;
    }
    else if (changed_feature==&CPromDoc::dummyModifyTrans) {
      CPromDoc *doc=GetDocument();
      const float &m=doc->trans_m,&b=doc->trans_b;
      unsigned i,n=saddle_list.GetSize();
      for (i=0; i<n; ++i) {
	if (GetFeatureType(i)==FT_SADDLE) {
	  saddle_list[i].usable_peak_elev.transform(m,b);
	  saddle_list[i].usable_sadl_elev.transform(m,b);
	  saddle_list[i].prom=
	    saddle_list[i].usable_peak_elev-saddle_list[i].usable_sadl_elev;
	}
	else {
	  saddle_list[i].prom.transform(m,0);
	}
      }
    }
    else if (changed_feature==&CPromDoc::dummyModifyAll ||
	     changed_feature==&CPromDoc::dummyModifyBounds ||
	     changed_feature==&CPromDoc::dummyModifyMap ||
	     changed_feature==&CPromDoc::dummyModifyRObase ||
	     changed_feature==&CPromDoc::dummyModifyBSbase ||
	     changed_feature==&CPromDoc::dummySaveCenter)
    {
      // do nothing
    }
    else if (!changed_feature->location) {
      // a feature was deleted from the tree.
      compress_master_list();
    }
  }
  CGroupListView::OnUpdate(pSender,lHint,pHint);
}

void CPromSadlListView::add_item(Feature& featr, FT_index type, Domain& pk_dom,
				 Elev_intvl upe, Elev_intvl use)
{
  if (GetDocument()->is_drainage) {
    upe.transform(-1,0);
    use.transform(-1,0);
  }
  saddle_list.Add(PromSadl(upe,use,&pk_dom));
  AddBlankItem(featr,type);
}

static CPromSadlListView *curnt_view;
static Domain *curnt_pk;

void CPromSadlListView::add_sec_item(Feature& featr, FT_index type,
	const Elev_intvl& upe, const Elev_intvl& use, bool offmap)
{
  curnt_view->add_item(featr,type,*curnt_pk,upe,use);
  if (offmap) {
    CListCtrl& listCtrl=curnt_view->GetListCtrl();
    listCtrl.SetItemState(listCtrl.GetItemCount()-1,
			  INDEXTOOVERLAYMASK(1),LVIS_OVERLAYMASK);
  }
}

void CPromSadlListView::OnInitialUpdate() 
{
  CGroupListView::OnInitialUpdate();

  if (init_info.doms) // this is an initial list generation, not a regeneration
    relist_info.transfer(init_info);

  if (relist_info.doms==0) return;

  BeginWaitCursor();
  saddle_list.SetSize(0);

  CPromDoc *doc=GetDocument();
  const Database& data=doc->data;
  CListCtrl& listCtrl=GetListCtrl();
  Elev_intvl prom;
  doc->tree_flip_drain();
  zero_prob=doc->prom_zp;
  curnt_view=this;

  for (Domain::Index i=0; i<relist_info.ndom; ++i) {
    curnt_pk=relist_info.doms[i];
    prom=curnt_pk->peak_prom();
    if (doc->is_drainage) prom.transform(-1,0);
    saddle_list.Add(PromSadl(prom));
    AddBlankItem(curnt_pk->peak,FT_PEAK);
    curnt_pk->list_prom_sadls(add_sec_item,relist_info,data);
  }
  doc->tree_flip_drain();

  UpdateAllItems();

  setItemIndicators();

  EndWaitCursor();
}

void CPromSadlListView::SetViewTitle()
{
  CPromDoc *document=GetDocument();
  sprintf(buf,"%s%s Saddles",
	  document->dt_name.IsEmpty()?"":document->dt_name+" - ",
	  document->is_drainage?"Basin":"Prominence");
  GetParent()->SetWindowText(buf);
  GetParent()->SetIcon(AfxGetApp()->
    LoadIcon(document->is_drainage?IDI_BASINSADL_LIST:IDI_PROMSADL_LIST),TRUE);
}

void CPromSadlListView::OnListWalk()
{
  CListCtrl& listCtrl=GetListCtrl();
  if (listCtrl.GetSelectedCount()==1) {
    int selItem=listCtrl.GetNextItem(-1,LVNI_SELECTED);
    CWalkListView::init_list(GetDocument(),
      WalkInfo(&saddle_list[selItem].peak->peak,FT_PEAK,
	       &GetFeature(selItem),GetFeatureType(selItem)));
    return;
  }
  CGroupListView::OnListWalk();
}

void CPromSadlListView::OnUpdateListWalk(CCmdUI *pCmdUI)
{
  CListCtrl& listCtrl=GetListCtrl();
  if (listCtrl.GetSelectedCount()==1) {
    pCmdUI->Enable(saddle_list[listCtrl.GetNextItem(-1,LVNI_SELECTED)].peak!=0);
    return;
  }
  CGroupListView::OnUpdateListWalk(pCmdUI);
}
