// ParentListView.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "ParentListView.h"
#include "ListFrame.h"
#include "ParentType_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CParentListView

IMPLEMENT_DYNCREATE(CParentListView, CGroupListView)

CParentListView::CParentListView()
{
}

CParentListView::~CParentListView()
{
}

bool CParentListView::is_initial;
CParentListView::List_info CParentListView::init_info;

BEGIN_MESSAGE_MAP(CParentListView, CGroupListView)
  //{{AFX_MSG_MAP(CParentListView)
  ON_WM_CREATE()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CParentListView diagnostics

#ifdef _DEBUG
void CParentListView::AssertValid() const
{
  CGroupListView::AssertValid();
}

void CParentListView::Dump(CDumpContext& dc) const
{
  CGroupListView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CParentListView message handlers

void CParentListView::init_list(CPromDoc *doc, Domain& peak, CWnd *parent,
				Elevation min_prom)
{
  List_info info;
  info.peak=&peak;
  init_list(doc,info,parent,min_prom);
}

void CParentListView::init_list(CPromDoc *doc, List_info& sadls, CWnd *parent,
				Elevation min_prom)
{
  sadls.topology=init_info.topology;
  sadls.filter.min_prom=sadls.min_prom=min_prom;
  sadls.runoffs=init_info.runoffs;
  CParentType_dlg dlg(*doc,sadls,parent);
  if (dlg.DoModal()!=IDOK) return;
  is_initial=true;
  init_info=sadls;

  CListFrame::client_width=88;
  CSingleDocTemplate& ParentListViewTpl=CPromApp::getApp()->ParentListViewTpl;
  CListFrame *newlist=(CListFrame *)ParentListViewTpl.CreateNewFrame(doc,NULL);
  ParentListViewTpl.InitialUpdateFrame(newlist,doc);
  newlist->SetIcon(AfxGetApp()->
    LoadIcon(doc->is_drainage?IDI_BASINSADL_LIST:IDI_PROMSADL_LIST),TRUE);
}

int CParentListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CGroupListView::OnCreate(lpCreateStruct) == -1)
    return -1;

  InsertColumn(PROM_COL,"Prominence",LVCFMT_CENTER,88);

  return 0;
}

int CParentListView::get_prom_sadl(Feature *f) const
{
  for (int i=0; i<prom_sadls.size(); ++i)
    if (prom_sadls[i].sadl==f) return i;
  return -1;
}

void CParentListView::UpdateItem(int nItem)
{
  CListCtrl& listCtrl=GetListCtrl();
  Feature *f=&GetFeature(nItem);
  int ips;
  Elevation pk_prom;
  CFeatureListView::UpdateItem(nItem);
  switch (GetFeatureType(nItem)) {
  case FT_SADDLE:
    ips=get_prom_sadl(f);
    if (ips>=0) {
      prom_sadls[ips].prom().sprint(buf);
      listCtrl.SetItemText(nItem,PROM_COL,buf);
    }
    break;
  case FT_PEAK:
    pk_prom=Domain::FromPeak(f)->prom.peak;
    if (pk_prom==elev_infty) {
      strcpy(buf,Elev_intvl::plus_infty_string);
    }
    else {
      sprintf(buf,"%d-",pk_prom);
    }
    listCtrl.SetItemText(nItem,PROM_COL,buf);
    break;
  case FT_RUNOFF:
    break;
  default: assert(0);
  }
}

bool CParentListView::prelist()
{
  CParentType_dlg dlg(*GetDocument(),relist_info,GetParent());
  return dlg.DoModal()==IDOK;
}

void CParentListView::SetViewTitle()
{
  CPromDoc *document=GetDocument();
  sprintf(buf,"%sParents",
	  document->dt_name.IsEmpty()?"":document->dt_name+" - ");
  GetParent()->SetWindowText(buf);
  GetParent()->SetIcon(AfxGetApp()->
    LoadIcon(document->is_drainage?IDI_BASINSADL_LIST:IDI_PROMSADL_LIST),TRUE);
}

static CParentListView *curnt_view;

void CParentListView::add_prom_sadl(Feature& featr, FT_index,
	const Elev_intvl& upe, const Elev_intvl& use, bool offmap)
{
  curnt_view->prom_sadls.push_back(Prom_sadl(&featr,upe,use,offmap));
}

void CParentListView::add_parent(Feature& f, FT_index ft)
{
  if (Domain::FromPeak(&f)->prom.peak>=curnt_view->relist_info.min_prom ||
      curnt_view->relist_info.min_prom==0)
    curnt_view->AddItem(f,ft);
}

void CParentListView::list_parents(const Prom_sadl& ps)
{
  AddItem(*ps.sadl,FT_SADDLE);
  if (ps.offmap) {
    CListCtrl& listCtrl=GetListCtrl();
    listCtrl.SetItemState(listCtrl.GetItemCount()-1,
			  INDEXTOOVERLAYMASK(1),LVIS_OVERLAYMASK);
  }
  relist_info.peak->list_parents(*Domain::FromSadl(ps.sadl),
	ps.usable_peak_elev,ps.usable_sadl_elev,
	relist_info.topology,add_parent,relist_info.runoffs);
}

void CParentListView::OnInitialUpdate()
{
  CGroupListView::OnInitialUpdate();

  if (is_initial) {
    relist_info=init_info;
    is_initial=false;
  }
  assert(relist_info.peak);

  // list the actual possible prom saddles
  prom_sadls.clear();
  curnt_view=this;
  relist_info.peak->list_prom_sadls(add_prom_sadl,relist_info.filter,GetDocument()->data);

  CPromDoc *doc=GetDocument();
  doc->tree_flip_drain();
  doc->update_peak_prom_info();
  zero_prob=doc->prom_zp;
  AddItem(relist_info.peak->peak,FT_PEAK);
  Domain::Index i;
  if (relist_info.sadls.empty())
    for (i=0; i<prom_sadls.size(); ++i)
      list_parents(prom_sadls[i]);
  else {
    int ips;
    for (i=0; i<relist_info.sadls.size(); ++i) {
      ips=get_prom_sadl(relist_info.sadls[i]);
      if (ips>=0) list_parents(prom_sadls[ips]);
      else AddItem(*relist_info.sadls[i],FT_SADDLE);
    }
  }

  doc->tree_flip_drain();

  setItemIndicators();
}
