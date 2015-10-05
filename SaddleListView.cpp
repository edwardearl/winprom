// SaddleListView.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "SaddleListView.h"
#include "ListFrame.h"
#include <algorithm>
#include <functional>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSaddleListView

IMPLEMENT_DYNCREATE(CSaddleListView, CFeatureListView)

CSaddleListView::CSaddleListView()
{
}

CSaddleListView::~CSaddleListView()
{
}

bool CSaddleListView::is_initial;
SadlListFilter CSaddleListView::init_lf;

BEGIN_MESSAGE_MAP(CSaddleListView, CFeatureListView)
  //{{AFX_MSG_MAP(CSaddleListView)
  ON_WM_CREATE()
  //}}AFX_MSG_MAP
  ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnHeaderClick)
  ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnUnlistItem)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSaddleListView diagnostics

#ifdef _DEBUG
void CSaddleListView::AssertValid() const
{
  CFeatureListView::AssertValid();
}

void CSaddleListView::Dump(CDumpContext& dc) const
{
  CFeatureListView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CSaddleListView message handlers

void CSaddleListView::init_list(CPromDoc *document)
{
  is_initial=CListSadl_dlg(document->data,init_lf,
    document->is_drainage?IDI_BASINSADL:IDI_SADDLE).DoModal()==IDOK;
  if (!is_initial) return;

  CListFrame::client_width=init_lf.offmap?192:136;
  CSingleDocTemplate& SaddleListViewTpl=CPromApp::getApp()->SaddleListViewTpl;
  CListFrame *newlist=(CListFrame *)SaddleListViewTpl.CreateNewFrame(document,NULL);
  SaddleListViewTpl.InitialUpdateFrame(newlist,document);
}

int CSaddleListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CFeatureListView::OnCreate(lpCreateStruct) == -1)
    return -1;

  InsertColumn(STAT_COL,"Status",LVCFMT_CENTER,48);
  InsertColumn(PROM_COL,"Prominence",LVCFMT_CENTER,88);
  if (init_lf.offmap)
    InsertColumn(OMP_COL,"OM Prom",LVCFMT_RIGHT,56);

  return 0;
}

void CSaddleListView::UpdateItem(int nItem)
{
  CListCtrl& listCtrl=GetListCtrl();
  CFeatureListView::UpdateItem(nItem);
  saddle_list[nItem].prom.sprint(buf);
  listCtrl.SetItemText(nItem,PROM_COL,buf);
  if (list_filter.offmap) {
    Elevation omp=saddle_list[nItem].offmap_prom*(GetDocument()->drain_const());
    if (omp==elev_infty) strcpy(buf,"unlimited");
    else if (omp>-elev_infty)
      sprintf(buf,Elev_intvl::elem_format,saddle_list[nItem].offmap_prom);
    else buf[0]='\0';
    listCtrl.SetItemText(nItem,OMP_COL,buf);
  }
  listCtrl.SetItemText(nItem,STAT_COL,
    Saddle::status_name[((Saddle&)GetFeature(nItem)).status]);
}

void CSaddleListView::printFeature(int item, FILE *out_file,
				   Format_style style)
{
  if (style==FMS_HTML) fprintf(out_file,"<TD align=center>");
  saddle_list[item].prom.print(out_file);
  if (list_filter.offmap) {
    fprintf(out_file,get_delimiter(style,CA_RIGHTx));
    if (saddle_list[item].offmap_prom==elev_infty)
      fprintf(out_file,"unltd");
    else if (saddle_list[item].offmap_prom==-elev_infty)
      fprintf(out_file,"     ");
    else fprintf(out_file,Elev_intvl::elem_format,
		 saddle_list[item].offmap_prom);
  }
  if (style!=FMS_HTML) fprintf(out_file,get_delimiter(style,CA_CENTERx));
  CFeatureListView::printFeature(item,out_file,style);
}

void CSaddleListView::printHeader(FILE *out_file, Format_style style)
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

bool CSaddleListView::CanPaste(const Clip& clp) const
{
  static unsigned nx=0;
  unsigned i,n=clp.ftr_count();
  for (i=0; i<n; ++i)
    if (clp.type[i]!=FT_SADDLE) return false;
  return true;
}

bool CSaddleListView::prelist()
{
  bool have_omp_col=list_filter.offmap; // save old value
  CListSadl_dlg dlg(GetDocument()->data,list_filter,
    GetDocument()->is_drainage?IDI_BASINSADL:IDI_SADDLE,GetParent());
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

void CSaddleListView::OnUnlistItem(NMHDR *pNMHDR, LRESULT *pResult)
{
  assert(GetListCtrl().GetItemCount()==saddle_list.size());
  int item=((NM_LISTVIEW *)pNMHDR)->iItem;
  saddle_list.erase(saddle_list.begin()+item);
	
  *pResult = 0;
}

void CSaddleListView::OnInitialUpdate() 
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
  Domain::Index n=tree.n_dom();
  Elev_intvl prom;
  Elevation omp=-elev_infty;
  saddle_list.clear();
  doc->tree_flip_drain();
  if (doc->is_drainage) list_filter.elev.transform(-1,0);
  doc->update_sadl_prom_info();
  if (list_filter.ridge) doc->update_ridge_info();
#ifndef DEBUG
  TRACE("           --- ON ---      --- OFF ---\n");
  TRACE(" Index      IC     PE       IC     PE\n");
#endif

  for (Domain::Index i=0; i<n; ++i) {
    Domain& dom=tree[i];
#ifndef NDEBUG // include this to test global prom against PE prom
    if (dom.peak.location && dom.primary_nbr) {
      const Domain::Sadl_prom& sp=dom.prom.saddle;
      prom.high=dom.extreme_sadl_prom(HI_END);
      omp=dom.offmap_sadl_prom();
      if (omp==-elev_infty) omp=prom.high;
      if (sp.onmap!=prom.high || sp.offmap!=omp) {
	Sleep(4);
	TRACE("%6d    %5d  %5d    %5d  %5d    ",
	      i,sp.onmap,prom.high,sp.offmap,omp);
	dom.saddle.trace();
	TRACE("\n");
      }
      omp=-elev_infty;
    }
#endif
    if (dom.primary_nbr && list_filter.test(dom.saddle,data)) {
      list_filter.get_prom(dom.prom.saddle,prom.high,omp);
      if (prom.high>=list_filter.min_prom || omp>=list_filter.min_prom) {
	prom.low=dom.extreme_sadl_prom(LO_END);
	saddle_list.push_back(PromListItem(dom.saddle,prom));
	if (list_filter.offmap) {
	  saddle_list.back().offmap_prom=omp;
	}
      }
    }
  }

  sort(saddle_list.begin(),saddle_list.end(),greater<PromListItem>());
  GetListCtrl().SetItemCount(saddle_list.size());
  doc->tree_flip_drain();
  if (doc->is_drainage) list_filter.elev.transform(-1,0);

  for (vector<PromListItem>::iterator vi=saddle_list.begin();
       vi!=saddle_list.end(); ++vi) {
    if (doc->is_drainage)
      (*vi).transform(-1);
    AddItem(*(*vi).feature,FT_SADDLE);
  }

  setItemIndicators();

  EndWaitCursor();
}

void CSaddleListView::SetViewTitle()
{
  CPromDoc *document=GetDocument();
  GetParent()->SetWindowText(document->dt_name.IsEmpty()?
    "Saddles":document->dt_name+" - Saddles");
  GetParent()->SetIcon(AfxGetApp()->
    LoadIcon(document->is_drainage?IDI_BASINSADL:IDI_SADDLE),TRUE);
}

void CSaddleListView::pasteClip(const Clip& clp)
{
  Elev_intvl prom;
  Elevation omp;
  Feature *new_sadl;
  CPromDoc *doc=GetDocument();
  list_fmt.set(GetDocument()->data);
  Domain *dom;
  doc->tree_flip_drain();
  int old_size=GetListCtrl().GetItemCount();
  unsigned i,n=clp.ftr_count();
  doc->update_sadl_prom_info();
  if (list_filter.ridge) doc->update_ridge_info();
  for (i=0; i<n; ++i) {
    new_sadl=clp.list[i];
    dom=Domain::FromSadl(new_sadl);
    list_filter.get_prom(dom->prom.saddle,prom.high,omp);
    prom.low=dom->extreme_sadl_prom(LO_END);
    saddle_list.push_back(PromListItem(*new_sadl,prom));
    if (list_filter.offmap)
      saddle_list.back().offmap_prom=omp;
    if (doc->is_drainage)
      saddle_list.back().transform(-1);
    AddBlankItem(*new_sadl,clp.type[i]);
  }
  doc->tree_flip_drain();
  for (i=0; i<n; ++i)
    UpdateItem(i+old_size);
  setItemIndicators();
}

void CSaddleListView::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  if ((Feature *)lHint==&CPromDoc::dummyModifyTrans) {
    CPromDoc *doc=GetDocument();
    const float& m=doc->trans_m;
    for (vector<PromListItem>::iterator vi=saddle_list.begin();
	 vi!=saddle_list.end(); ++vi)
      (*vi).transform(m);
  }

  CFeatureListView::OnUpdate(pSender,lHint,pHint);
}

static int compare_dir;

static int CALLBACK compare_stat(LPARAM p1, LPARAM p2, LPARAM plist)
{
  CSaddleListView *list=(CSaddleListView *)plist;
  int stat1=((const Saddle *)(list->saddle_list[p1].feature))->status,
    stat2=((const Saddle *)(list->saddle_list[p2].feature))->status;
  return (stat2-stat1)*compare_dir;
}

static int CALLBACK compare_prom(LPARAM p1, LPARAM p2, LPARAM plist)
{
  CSaddleListView *list=(CSaddleListView *)plist;
  Elev_endpt cmp_endpt=list->GetDocument()->outer_endpt();
  return (list->saddle_list[p2].prom.*cmp_endpt-
	  list->saddle_list[p1].prom.*cmp_endpt)
    *(list->GetDocument()->drain_const())*compare_dir;
}

static int CALLBACK compare_omp(LPARAM p1, LPARAM p2, LPARAM plist)
{
  CSaddleListView *list=(CSaddleListView *)plist;
  Elevation omp1=list->saddle_list[p1].offmap_prom,
    omp2=list->saddle_list[p2].offmap_prom;
  return (omp2-omp1)*(list->GetDocument()->drain_const())*compare_dir;
}

void CSaddleListView::OnHeaderClick(NMHDR *pNMHDR, LRESULT *pResult)
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
  else {
    CFeatureListView::OnHeaderClick(pNMHDR,pResult);
  }

  vector<PromListItem> temp_list;
  int i,j,n=listCtrl.GetItemCount();
  temp_list.resize(n);
  for (i=0; i<n; ++i) {
    j=listCtrl.GetItemData(i);
    temp_list[i]=saddle_list[j];
    listCtrl.SetItemData(i,(LPARAM)features[j]);
  }
  saddle_list=temp_list;
}

void CSaddleListView::swap_items(int nItem)
{
  PromListItem temp=saddle_list[nItem];
  saddle_list[nItem]=saddle_list[nItem+1];
  saddle_list[nItem+1]=temp;
  CFeatureListView::swap_items(nItem);
}
