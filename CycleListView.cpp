// CycleListView.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "CycleListView.h"
#include "ListFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCycleListView

IMPLEMENT_DYNCREATE(CCycleListView, CGroupListView)

CCycleListView::CCycleListView()
{
}

CCycleListView::~CCycleListView()
{
}

CycleFilter CCycleListView::init_filter(0);

BEGIN_MESSAGE_MAP(CCycleListView, CGroupListView)
  //{{AFX_MSG_MAP(CCycleListView)
  ON_WM_CREATE()
  ON_COMMAND(ID_EDIT_ROTATE, OnEditRotate)
  ON_UPDATE_COMMAND_UI(ID_EDIT_ROTATE, OnUpdateEditRotate)
  //}}AFX_MSG_MAP
  ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnUnlistItem)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCycleListView diagnostics

#ifdef _DEBUG
void CCycleListView::AssertValid() const
{
  CGroupListView::AssertValid();
}

void CCycleListView::Dump(CDumpContext& dc) const
{
  CGroupListView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CCycleListView message handlers

void CCycleListView::init_list(CPromDoc *doc, Basin_saddle& bs, CWnd *parent,
			       Elevation min_prom, bool offmap, bool ridge, bool cell)
{
  CycleFilter bs1(bs);
  bs1.min_prom=min_prom;
  bs1.offmap=offmap;
  bs1.ridge=ridge;
  bs1.cell=cell;
  init_list(doc,bs1,parent);
}

void CCycleListView::init_list(CPromDoc *doc, CycleFilter& bs, CWnd *parent)
{
  if (bs.min_prom<0) {
    bs.min_prom=init_filter.min_prom;
    bs.offmap=init_filter.offmap;
    bs.ridge=init_filter.ridge;
    bs.cell=init_filter.cell;
  }
  (FeatureFilter&)bs=init_filter;
  bs.nonoffend=init_filter.nonoffend;
  CCycleSadl_dlg dlg(doc->data,bs,parent);
  if (dlg.DoModal()!=IDOK) return;
  init_filter.transfer(bs);

  CListFrame::client_width=init_filter.offmap?288:232;
  CSingleDocTemplate& CycleListViewTpl=CPromApp::getApp()->CycleListViewTpl;
  CListFrame *newlist=(CListFrame *)CycleListViewTpl.CreateNewFrame(doc,NULL);
  CycleListViewTpl.InitialUpdateFrame(newlist,doc);
  newlist->SetIcon(AfxGetApp()->LoadIcon(IDI_CYCLE),TRUE);
}

int CCycleListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CGroupListView::OnCreate(lpCreateStruct) == -1)
    return -1;

  InsertColumn(CWR_COL,"CWR",LVCFMT_RIGHT,48);
  InsertColumn(CWRO_COL,"CWRO",LVCFMT_RIGHT,48);
  InsertColumn(STAT_COL,"Stat",LVCFMT_CENTER,48);
  InsertColumn(PROM_COL,"Prominence",LVCFMT_CENTER,88);
  if (init_filter.offmap) InsertColumn(OMP_COL,"OM Prom",LVCFMT_RIGHT,56);

  return 0;
}

static const Basin_saddle *cwinfo_bs=0;
static Basin_saddle::Cycle_walk_info cwinfo;

void CCycleListView::UpdateItem(int nItem)
{
  CListCtrl& listCtrl=GetListCtrl();
  CGroupListView::UpdateItem(nItem);
  Elevation walk_risk,wro;
  short int dc=GetDocument()->drain_const();
  Saddle& featr=(Saddle&)GetFeature(nItem);
  switch (GetFeatureType(nItem)) {
  case FT_BASINSADL:
    {
      Basin_saddle& bs=(Basin_saddle&)featr;
      if (cwinfo_bs!=&bs) bs.get_walk_info(cwinfo,dc);
      walk_risk=bs.walk_risk(cwinfo);
      wro=bs.WR_offense(cwinfo);
    }
    break;
  case FT_SADDLE:
    {
      Basin_saddle *bs=saddle_list[nItem].basinsadl;
      Domain *sadl_dom=Domain::FromSadl(&featr);
      if (cwinfo_bs!=bs) bs->get_walk_info(cwinfo,dc);
      walk_risk=bs->walk_risk(cwinfo,sadl_dom);
      wro=bs->WR_offense(cwinfo,sadl_dom);
    }
    break;
  default:
    assert(0);
  }
  if (dc<0) {
    Elevation temp=walk_risk;
    walk_risk=wro;
    wro=temp;
  }
  if (walk_risk==-elev_infty)
    sprintf(buf,"%s",Basin_saddle::WS_string);
  else sprintf(buf,Elev_intvl::elem_format,walk_risk);
  listCtrl.SetItemText(nItem,CWR_COL,buf);
  if (wro==-elev_infty)
    sprintf(buf,"%s",Basin_saddle::WS_string);
  else sprintf(buf,Elev_intvl::elem_format,wro);
  listCtrl.SetItemText(nItem,CWRO_COL,buf);
  saddle_list[nItem].prom.sprint(buf);
  listCtrl.SetItemText(nItem,PROM_COL,buf);
  if (relist_filter.offmap) {
    if (saddle_list[nItem].offmap_prom==elev_infty)
      strcpy(buf,"unlimited");
    else if (saddle_list[nItem].offmap_prom>-elev_infty)
      sprintf(buf,Elev_intvl::elem_format,saddle_list[nItem].offmap_prom);
    else buf[0]='\0';
    listCtrl.SetItemText(nItem,OMP_COL,buf);
  }
  listCtrl.SetItemText(nItem,STAT_COL,Saddle::status_name[featr.status]);
}

void CCycleListView::printFeature(int item, FILE *out_file, Format_style style)
{
  const char *style_col=get_delimiter(style,CA_CENTER);
  if (style==FMS_HTML) fprintf(out_file,style_col);
  Feature& featr=GetFeature(item);
  short int dc=GetDocument()->drain_const();
  Elevation walk_risk,wro;
  switch (GetFeatureType(item)) {
  case FT_BASINSADL:
    {
      Basin_saddle& bs=(Basin_saddle&)featr;
      if (&bs!=cwinfo_bs) bs.get_walk_info(cwinfo,dc);
      walk_risk=bs.walk_risk(cwinfo);
      wro=bs.WR_offense(cwinfo);
    }
    break;
  case FT_SADDLE:
    {
      Basin_saddle *bs=saddle_list[item].basinsadl;
      if (bs!=cwinfo_bs) bs->get_walk_info(cwinfo,dc);
      Domain *sadl_dom=Domain::FromSadl(&featr);
      walk_risk=bs->walk_risk(cwinfo,sadl_dom);
      wro=bs->WR_offense(cwinfo,sadl_dom);
    }
    break;
  default:
    assert(0);
  }
  if (dc<0) {
    Elevation temp=walk_risk;
    walk_risk=wro;
    wro=temp;
  }
  fprintf(out_file,Elev_intvl::elem_format,walk_risk);
  fprintf(out_file,style_col);
  fprintf(out_file,Elev_intvl::elem_format,wro);
  fprintf(out_file,style_col);
  saddle_list[item].prom.print(out_file);
  if (relist_filter.offmap) {
    fprintf(out_file,style_col);
    if (saddle_list[item].offmap_prom==elev_infty)
      fprintf(out_file,"unltd");
    else if (saddle_list[item].offmap_prom==-elev_infty)
      fprintf(out_file,"     ");
    else fprintf(out_file,Elev_intvl::elem_format,
		 saddle_list[item].offmap_prom);
  }
  if (style!=FMS_HTML) fprintf(out_file,style_col);
  CGroupListView::printFeature(item,out_file,style);
}

void CCycleListView::printHeader(FILE *out_file, Format_style style)
{
  const char *style_fmt;
  switch (style) {
  case FMS_FLAG:
    style_fmt="%s %s ";
    break;
  case FMS_HTML:
    style_fmt="<TD align=center>%s<TD align=center>%s<TD align=center>";
    break;
  case FMS_CSV:
    style_fmt="%s,%s,";
    break;
  default:
    assert(0);
  }
  fprintf(out_file,style_fmt,
	  Basin_saddle::walk_risk_hdr,Basin_saddle::WRO_hdr);
  fprintf(out_file,Elev_intvl::field_format,Domain::prom_hdr);
  if (relist_filter.offmap) {
    fprintf(out_file,get_delimiter(style,CA_CENTER));
    fprintf(out_file,"  OMP");
  }
  if (style!=FMS_HTML) fprintf(out_file,get_delimiter(style,CA_CENTER));
  CGroupListView::printHeader(out_file,style);
}

void CCycleListView::OnUnlistItem(NMHDR *pNMHDR, LRESULT *pResult)
{
  CGroupListView::OnUnlistItem(pNMHDR,pResult);
  int item=((NM_LISTVIEW *)pNMHDR)->iItem;
  assert(GetListCtrl().GetItemCount()==saddle_list.GetSize());
  saddle_list.RemoveAt(item);
}

bool CCycleListView::prelist()
{
  bool have_omp_col=relist_filter.offmap; // save old value
  CCycleSadl_dlg dlg(GetDocument()->data,relist_filter,GetParent());
  if (dlg.DoModal()!=IDOK) return false;

  // Did we have an "OM Prom" column?
  if (have_omp_col) {
    // yes.  Get rid of it if desired.
    if (!relist_filter.offmap)
      DeleteColumn(OMP_COL);
  }
  else {
    // Nope.  Add it if desired.
    if (relist_filter.offmap)
      InsertColumn(OMP_COL,"OM Prom",LVCFMT_CENTER,56);
  }

  return true;
}

void CCycleListView::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint) 
{
  if (lHint==0) {
    delete[] relist_filter.bs;
    relist_filter.bs=0;
    relist_filter.nbs=0;
  }
  else {
    Feature *changed_feature=(Feature *)lHint;
    if (changed_feature==&CPromDoc::dummyModifyData) return;
    if (changed_feature==&CPromDoc::dummyModifyTopo ||
	changed_feature==&CPromDoc::dummyModifySplice) {
      GetListCtrl().DeleteAllItems();
      OnInitialUpdate();
      return;
    }
    if (changed_feature==&CPromDoc::dummyModifyTrans) {
      CPromDoc *doc=GetDocument();
      const float &m=doc->trans_m;
      unsigned i,n=saddle_list.GetSize();
      for (i=0; i<n; ++i) {
	saddle_list[i].prom.transform(m,0);
	saddle_list[i].offmap_prom=nint(saddle_list[i].offmap_prom*m);
      }
    }
    else if (changed_feature!=&CPromDoc::dummyModifyAll &&
	     changed_feature!=&CPromDoc::dummyModifyBounds &&
	     changed_feature!=&CPromDoc::dummyModifyMap &&
	     changed_feature!=&CPromDoc::dummyModifyRObase &&
	     changed_feature!=&CPromDoc::dummyModifyBSbase &&
	     changed_feature!=&CPromDoc::dummySaveCenter &&
	     !changed_feature->location) {
      // a feature was deleted from the tree.
      // Delete it from the master peak list, if it's there.
      int i,j;
      for (i=j=0; i<relist_filter.nbs; ++i) {
	if (changed_feature!=relist_filter.bs[i]) {
	  if (i>j) relist_filter.bs[j]=relist_filter.bs[i];
	  ++j;
	}
      }
      relist_filter.nbs=j;
    }
  }
  CGroupListView::OnUpdate(pSender,lHint,pHint);
}

void CCycleListView::AddSaddle(Basin_saddle *bs, Domain *dom)
{
  Elevation omp=-elev_infty;
  Elev_intvl prom;
  if ((relist_filter.nonoffend || bs->can_rotate(dom->saddle)) &&
      relist_filter.test(dom->saddle,GetDocument()->data)) {
    relist_filter.get_prom(dom->prom.saddle,prom.high,omp);
    if (prom.high>=relist_filter.min_prom || omp>=relist_filter.min_prom) {
      prom.low=dom->extreme_sadl_prom(
	bs->status>dom->saddle.status?elev_infty:bs->elev.high,LO_END);
      saddle_list.Add(CycleSadl(prom,omp,bs));
      if (relist_filter.offmap)
	saddle_list[saddle_list.GetUpperBound()].offmap_prom=omp;
      AddBlankItem(dom->saddle,FT_SADDLE);
    }
  }
}

void CCycleListView::OnInitialUpdate() 
{
  CGroupListView::OnInitialUpdate();

  if (init_filter.bs)
    relist_filter.transfer(init_filter);

  if (relist_filter.bs==0) return;

  BeginWaitCursor();
  saddle_list.SetSize(0);

  CListCtrl& listCtrl=GetListCtrl();
  Domain *dom;
  Elevation omp;
  Elev_intvl prom;
  CPromDoc *doc=GetDocument();
  Divide_tree& tree=doc->tree;
  doc->tree_flip_drain();
  doc->update_sadl_prom_info();
  if (relist_filter.ridge) doc->update_ridge_info();

  for (Domain::Index i=0; i<relist_filter.nbs; ++i) {
    Basin_saddle& bs=*relist_filter.bs[i];
    relist_filter.get_prom(bs.prom,prom.high,omp);
    prom.low=bs.extreme_prom(LO_END);
    // add the basin saddle to the list
    saddle_list.Add(CycleSadl(prom,omp));
    AddBlankItem(bs,FT_BASINSADL);
    if (bs.common_ancestor==0) continue;

    // add leg 1 (peak 1 to common ancestor) saddles to the list
    for (dom=bs.peak1; dom!=bs.common_ancestor; dom=dom->primary_nbr) {
      AddSaddle(&bs,dom);
    }
    // add leg 2 (peak 2 to common ancestor) saddles to the list in reverse order
    if (bs.common_ancestor!=bs.peak2) {
      int leg2len=Domain::length(bs.peak2,bs.common_ancestor),i;
      Domain **leg2=new Domain *[leg2len];
      for (dom=bs.peak2,i=0; dom!=bs.common_ancestor; dom=dom->primary_nbr,++i)
	leg2[i]=dom;
      for (i=leg2len-1; i>=0; --i)
	AddSaddle(&bs,leg2[i]);
      delete[] leg2;
    }
  }

  //delete[] sadl_hi_prom;
  //delete[] sadl_lo_prom;
  //delete[] sadl_omp;

  doc->tree_flip_drain();

  UpdateAllItems();

  setItemIndicators();

  EndWaitCursor();
}

void CCycleListView::SetViewTitle()
{
  CPromDoc *document=GetDocument();
  sprintf(buf,"%s%s Saddle Cycle",
	  document->dt_name.IsEmpty()?"":document->dt_name+" - ",
	  document->is_drainage?"Prominence":"Basin");
  GetParent()->SetWindowText(buf);
}

void CCycleListView::OnEditRotate()
{
  CListCtrl& listCtrl=GetListCtrl();
  if (listCtrl.GetSelectedCount()==1) {
    int item=listCtrl.GetNextItem(-1,LVNI_SELECTED);
    Feature& featr=GetFeature(item);
    if (GetFeatureType(item)==FT_SADDLE)
      GetDocument()->Rotate(*saddle_list[item].basinsadl,&featr,GetParent());
    else GetDocument()->Rotate((Basin_saddle&)featr,0,GetParent());
  }
  else CGroupListView::OnEditRotate();
}

void CCycleListView::OnUpdateEditRotate(CCmdUI *pCmdUI)
{

  CListCtrl& listCtrl=GetListCtrl();
  if (listCtrl.GetSelectedCount()==1) pCmdUI->Enable(TRUE);
  else CGroupListView::OnUpdateEditRotate(pCmdUI);
}
