// WalkListView.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "WalkListView.h"
#include "ListFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CWalkListView

IMPLEMENT_DYNCREATE(CWalkListView, CFeatureListView)

CWalkListView::CWalkListView()
{
}

CWalkListView::~CWalkListView()
{
}

WalkInfo CWalkListView::init_walk_info;


BEGIN_MESSAGE_MAP(CWalkListView, CFeatureListView)
  //{{AFX_MSG_MAP(CWalkListView)
  ON_COMMAND(ID_EDIT_SELECT_OVERLAPS, OnEditSelectOverlaps)
  ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_OVERLAPS, OnUpdateEditSelectOverlaps)
  ON_UPDATE_COMMAND_UI(ID_LIST_LOAD, OnUpdateListLoad)
  ON_UPDATE_COMMAND_UI(ID_LIST_APPEND, OnUpdateListLoad)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWalkListView diagnostics

#ifdef _DEBUG
void CWalkListView::AssertValid() const
{
  CFeatureListView::AssertValid();
}

void CWalkListView::Dump(CDumpContext& dc) const
{
  CFeatureListView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CWalkListView message handlers

static Domain *getDomain(Feature *featr, FT_index ft)
{
  switch (ft) {
  case FT_PEAK: return Domain::FromPeak(featr);
  case FT_SADDLE: return Domain::FromSadl(featr);
  case FT_RUNOFF: return ((Runoff *)featr)->peak;
  }
  return 0;
}

void CWalkListView::init_list(CPromDoc *document, const WalkInfo& info)
{
  Domain *begin_dom=getDomain(info.begin,info.begin_type),
    *end_dom=getDomain(info.end,info.end_type);
  if (Domain::common_ancestor(begin_dom,end_dom)==0) {
    ::MessageBox(NULL,"There is no possible walk\n"
		 "between the selected features.",
		 "Winprom Walk",MB_OK|MB_ICONERROR);
    return;
  }

  CListFrame::client_width=0;
  CSingleDocTemplate& WalkListViewTpl=CPromApp::getApp()->WalkListViewTpl;
  CListFrame *newlist=(CListFrame *)WalkListViewTpl.CreateNewFrame(document,NULL);

  init_walk_info=info;

  WalkListViewTpl.InitialUpdateFrame(newlist,document);
  newlist->SetIcon(AfxGetApp()->LoadIcon(IDR_GRAPHTYPE),TRUE);
}

void CWalkListView::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  if (lHint==0) {
    walk_info.begin=walk_info.end=0;
  }
  else {
    Feature *changed_feature=(Feature *)lHint;
    if (!changed_feature->location) {
      if (changed_feature==walk_info.begin) walk_info.begin=0;
      else if (changed_feature==walk_info.end) walk_info.end=0;
    }
  }
  CFeatureListView::OnUpdate(pSender,lHint,pHint);
}

void CWalkListView::OnInitialUpdate()
{
  CFeatureListView::OnInitialUpdate();

  if (init_walk_info) {
    walk_info=init_walk_info;
    init_walk_info.begin=init_walk_info.end=0;
  }

  if (!walk_info) return;

  CListCtrl& listCtrl=GetListCtrl();

  Domain *begin_dom=getDomain(walk_info.begin,walk_info.begin_type),
    *end_dom=getDomain(walk_info.end,walk_info.end_type),
    *ca=Domain::common_ancestor(begin_dom,end_dom);
  if (ca==0) {
    MessageBox("There is no longer any possible walk between\n"
	       "the two terminal features of this list.",
	       "Winprom Walk",MB_OK|MB_ICONERROR);
    return;
  }
  if (walk_info.begin_type==FT_SADDLE && ca!=begin_dom)
    begin_dom=begin_dom->primary_nbr;
  if (walk_info.end_type==FT_SADDLE && ca!=end_dom)
    end_dom=end_dom->primary_nbr;

  if (walk_info.begin_type!=FT_PEAK)
    AddItem(*walk_info.begin,walk_info.begin_type);

  for (; begin_dom!=ca; begin_dom=begin_dom->primary_nbr) {
    AddItem(begin_dom->peak,FT_PEAK);
    AddItem(begin_dom->saddle,FT_SADDLE);
  }

  AddItem(ca->peak,FT_PEAK);

  if (ca!=end_dom) {
    int leg2len=Domain::length(end_dom,ca),i;
    Domain **leg2=new Domain *[leg2len];
    for (i=0; end_dom!=ca; end_dom=end_dom->primary_nbr,++i)
      leg2[i]=end_dom;
    for (i=leg2len-1; i>=0; --i) {
      AddItem(leg2[i]->saddle,FT_SADDLE);
      AddItem(leg2[i]->peak,FT_PEAK);
    }
    delete[] leg2;
  }

  if (walk_info.end_type!=FT_PEAK)
    AddItem(*walk_info.end,walk_info.end_type);

  setItemIndicators();
}

void CWalkListView::SetViewTitle()
{
  CPromDoc *document=GetDocument();
  GetParent()->SetWindowText(document->dt_name.IsEmpty()?
    "Walk":document->dt_name+" - Walk");
}

void CWalkListView::OnEditSelectOverlaps()
{
  select_extreme_saddles(GetDocument()->is_drainage);
}

void CWalkListView::OnUpdateEditSelectOverlaps(CCmdUI *pCmdUI)
{
  pCmdUI->SetText(GetDocument()->is_drainage?
    "Highest &saddles":"Lowest &saddles");
}

void CWalkListView::OnUpdateListLoad(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(FALSE);
}
