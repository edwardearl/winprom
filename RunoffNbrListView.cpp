// RunoffNbrListView.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "RunoffNbrListView.h"
#include "ListFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CRunoffNbrListView

IMPLEMENT_DYNCREATE(CRunoffNbrListView, CRunoffListView)

CRunoffNbrListView::CRunoffNbrListView()
{
}

CRunoffNbrListView::~CRunoffNbrListView()
{
}

Runoff *CRunoffNbrListView::init_center_runoff;
Runoff_nbr_type CRunoffNbrListView::init_ront=RO_NBR_SAME_GP;
float CRunoffNbrListView::init_radius=0.1f;


BEGIN_MESSAGE_MAP(CRunoffNbrListView, CRunoffListView)
  //{{AFX_MSG_MAP(CRunoffNbrListView)
  ON_WM_CREATE()
  //}}AFX_MSG_MAP
  ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnHeaderClick)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRunoffNbrListView diagnostics

#ifdef _DEBUG
void CRunoffNbrListView::AssertValid() const
{
  CRunoffListView::AssertValid();
}

void CRunoffNbrListView::Dump(CDumpContext& dc) const
{
  CRunoffListView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CRunoffNbrListView message handlers

void CRunoffNbrListView::init_list(CPromDoc *document, Runoff& ro)
{
  CRunoffNbr_dlg dlg(NULL,init_ront,init_radius);
  if (dlg.DoModal()!=IDOK) return;

  CListFrame::client_width=96;
  CSingleDocTemplate& RunoffNbrListViewTpl=CPromApp::getApp()->RunoffNbrListViewTpl;
  CListFrame *newlist=(CListFrame *)RunoffNbrListViewTpl.CreateNewFrame(document,NULL);

  init_center_runoff=&ro;
  init_ront=(Runoff_nbr_type)dlg.m_grid;
  init_radius=dlg.m_radius;

  RunoffNbrListViewTpl.InitialUpdateFrame(newlist,document);
}

int CRunoffNbrListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CRunoffListView::OnCreate(lpCreateStruct) == -1)
    return -1;

  InsertColumn(DIST_COL,"Distance",LVCFMT_RIGHT,64);

  return 0;
}

void CRunoffNbrListView::UpdateItem(int nItem)
{
  CRunoffListView::UpdateItem(nItem);
  if (center_runoff) {
    sprintf(buf,"%5.2g",(GetFeature(nItem).location-center_runoff->location));
    GetListCtrl().SetItemText(nItem,DIST_COL,buf);
  }
  else GetListCtrl().SetItemText(nItem,DIST_COL,"");
}

void CRunoffNbrListView::printFeature(int item, FILE *out_file, Format_style style)
{
  Distance dist=GetFeature(item).location-center_runoff->location;
  switch (style) {
  case FMS_FLAG:
    fprintf(out_file,"%6.2g ",dist);
    break;
  case FMS_HTML:
    fprintf(out_file,"<TD align=right>%6.2g",dist);
    break;
  case FMS_CSV:
    fprintf(out_file,"%6.2g,",dist);
    break;
  default: assert(0);
  }
  CRunoffListView::printFeature(item,out_file,style);
}

void CRunoffNbrListView::printHeader(FILE *out_file, Format_style style)
{
  switch (style) {
  case FMS_FLAG:
    fprintf(out_file,"  Dist ");
    break;
  case FMS_HTML:
    fprintf(out_file,"<TD align=right>Distance");
    break;
  case FMS_CSV:
    fprintf(out_file,"  Dist,");
    break;
  default: assert(0);
  }
  CRunoffListView::printHeader(out_file,style);
}

bool CRunoffNbrListView::prelist()
{
  CRunoffNbr_dlg dlg(GetParent(),ront,radius);
  if (dlg.DoModal()==IDOK) {
    ront=(Runoff_nbr_type)dlg.m_grid;
    radius=dlg.m_radius;
    return true;
  }
  return false;
}

void CRunoffNbrListView::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  if (lHint==0) {
    center_runoff=0;
  }
  else {
    Feature *changed_feature=(Feature *)lHint;
    if (changed_feature==center_runoff && !changed_feature->location)
      center_runoff=0;
  }
  CRunoffListView::OnUpdate(pSender,lHint,pHint);
}

//typedef systm::Grid::Neighbor_iter Neighbor_iter;

CRunoffNbrListView::Nbr_status
CRunoffNbrListView::ro_nbr(const GridPoint& c1, const GridPoint& c2) const
{
  if (!c2) return WITHOUT;
  switch (ront) {
  case RO_NBR_SAME_GP:
    return c1==c2 ? WITHIN : FOREVER_WITHOUT;
  case RO_NBR_NBR_GP:
    if (c1==c2 || Neighbor_iter::is_neighbor(c1,c2)) return WITHIN;
    return Neighbor_iter::ever_neighbor(c1,c2) ? WITHOUT : FOREVER_WITHOUT;
  case RO_NBR_RADIUS:
    if (c1-c2<=radius) return WITHIN;
    return c1.ever_radius(c2)<=radius ? WITHOUT : FOREVER_WITHOUT;
  default:
    assert(0);
  }
  return FOREVER_WITHOUT;
}

void CRunoffNbrListView::OnInitialUpdate()
{
  // yes, it really is CFeatureListView::OnInitialUpdate()
  // you don't want to generate this list the same way you generate the
  // filtered all-runoff list
  CFeatureListView::OnInitialUpdate();

  if (init_center_runoff) {
    center_runoff=init_center_runoff;
    ront=init_ront;
    radius=init_radius;
    init_center_runoff=0;
  }

  if (center_runoff==0) return;

  CListCtrl& listCtrl=GetListCtrl();

  AddItem(*center_runoff,FT_RUNOFF);

  Divide_tree& tree=GetDocument()->tree;
  Domain::Index center_iro=tree.index(*center_runoff),
    iro=center_iro,nro=tree.n_runoff();
  while (iro>0) {
    --iro;
    switch (ro_nbr(center_runoff->location,tree.runoff(iro).location)) {
    case WITHIN:
      AddItem(tree.runoff(iro),FT_RUNOFF);
    case WITHOUT:
      break;
    case FOREVER_WITHOUT:
      iro=0; // force loop to quit
      break;
    default:
      assert(0);
    }
  }
  iro=center_iro;
  while (iro<nro-1) {
    ++iro;
    switch (ro_nbr(center_runoff->location,tree.runoff(iro).location)) {
    case WITHIN:
      AddItem(tree.runoff(iro),FT_RUNOFF);
    case WITHOUT:
      break;
    case FOREVER_WITHOUT:
      iro=nro; // force loop to quit
      break;
    default:
      assert(0);
    }
  }

  setItemIndicators();
}

void CRunoffNbrListView::SetViewTitle()
{
  CPromDoc *document=GetDocument();
  sprintf(buf,"%s%s Runoff Neighbors",
	  document->dt_name.IsEmpty()?"":document->dt_name+" - ",
	  document->is_drainage?"Streambed":"");
  GetParent()->SetWindowText(buf);
  GetParent()->SetIcon(AfxGetApp()->
    LoadIcon(document->is_drainage?IDI_SBRO:IDI_RUNOFF),TRUE);
}

static int compare_dir;

static int CALLBACK compare_dist(LPARAM p1, LPARAM p2, LPARAM plist)
{
  GridPoint ctr_ro_loc=((CRunoffNbrListView *)plist)->center_runoff->location;
  const Runoff &ro1=*(Runoff *)p1,&ro2=*(Runoff *)p2;
  return (ro1.location-ctr_ro_loc>ro2.location-ctr_ro_loc ? 1 : -1)*compare_dir;
}

void CRunoffNbrListView::OnHeaderClick(NMHDR *pNMHDR, LRESULT *pResult)
{
  NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
  CListCtrl& listCtrl=GetListCtrl();

  if (pNMListView->iSubItem==DIST_COL) {
    // sort by distance
    compare_dir=GetKeyState(VK_SHIFT)<0 ? -1 : 1;
    listCtrl.SortItems(compare_dist,(LPARAM)this);
    *pResult = 0;
  }
  else {
    CRunoffListView::OnHeaderClick(pNMHDR,pResult);
  }
}
