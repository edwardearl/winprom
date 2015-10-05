// ErrorListView.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "ErrorListView.h"
#include "ErrorAnal_dlg.h"
#include "ListFrame.h"
#include <algorithm>
#include <functional>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CErrorListView

IMPLEMENT_DYNCREATE(CErrorListView, CFeatureListView)

CErrorListView::CErrorListView()
{
}

CErrorListView::~CErrorListView()
{
}

bool CErrorListView::is_initial;
ErrorFilter CErrorListView::init_ef;


BEGIN_MESSAGE_MAP(CErrorListView, CFeatureListView)
  //{{AFX_MSG_MAP(CErrorListView)
  ON_WM_CREATE()
  ON_UPDATE_COMMAND_UI(ID_LIST_LOAD, OnUpdateListLoad)
  ON_UPDATE_COMMAND_UI(ID_LIST_APPEND, OnUpdateListLoad)
  //}}AFX_MSG_MAP
  ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnHeaderClick)
  ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnUnlistItem)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CErrorListView diagnostics

#ifdef _DEBUG
void CErrorListView::AssertValid() const
{
  CFeatureListView::AssertValid();
}

void CErrorListView::Dump(CDumpContext& dc) const
{
  CFeatureListView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CErrorListView message handlers

void CErrorListView::init_list(CPromDoc *document)
{
  is_initial=CErrorFilter_dlg(init_ef,*document).DoModal()==IDOK;
  if (!is_initial) return;

  CListFrame::client_width=96;
  CSingleDocTemplate& ErrorListViewTpl=CPromApp::getApp()->ErrorListViewTpl;
  CListFrame *newlist=(CListFrame *)ErrorListViewTpl.CreateNewFrame(document,NULL);
  ErrorListViewTpl.InitialUpdateFrame(newlist,document);
}

int CErrorListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CFeatureListView::OnCreate(lpCreateStruct) == -1)
    return -1;

  InsertColumn(ELEV_COL,"Map Elev",LVCFMT_RIGHT,56);
  InsertColumn(ERROR_COL,"Error",LVCFMT_RIGHT,40);

  return 0;
}

Elevation CErrorListView::getError(const Feature& featr, const AnalData& data,
				   Elevation& map_elev) const
{
  Elev_map& elev=GetDocument()->elev;
  if (featr.location && elev.contains(featr.location)) {
    map_elev=elev[featr.location];
    if (map_elev!=elev_undef) {
      double nbr_avgs[MAX_REGRESS_PARAM];
      if (data.options.model!=Stat_options::NONE &&
	  data.options.get_nbr_avg(elev,featr.location,nbr_avgs)) {
	double x=elev[featr.location]-data.regress.bias,ni=1/data.regress.n;
	unsigned i,n=data.regress.nparam;
	for (i=0; i<n; ++i) {
	  nbr_avgs[i]-=data.regress.Sx[i]*ni;
	  x-=nbr_avgs[i]*data.regress.alpha[i];
	}
	map_elev=nint(x);
      }
      if (error_filter.reference==0) return map_elev-featr.elev.midpt();
      if (map_elev<featr.elev.low) return map_elev-featr.elev.low;
      if (map_elev>featr.elev.high) return map_elev-featr.elev.high;
      return 0;
    }
  }
  return elev_undef;
}

void CErrorListView::UpdateItem(int nItem)
{
  CListCtrl& listCtrl=GetListCtrl();

  CFeatureListView::UpdateItem(nItem);
  Elevation error=error_list[nItem].error;
  if (error==elev_undef) {
    buf[0]='\0';
    listCtrl.SetItemText(nItem,ELEV_COL,buf);
  }
  else {
    sprintf(buf,Elev_intvl::elem_format,error_list[nItem].map_elev);
    listCtrl.SetItemText(nItem,ELEV_COL,buf);
    sprintf(buf,Elev_intvl::elem_format,error);
  }
  listCtrl.SetItemText(nItem,ERROR_COL,buf);
}

void CErrorListView::printFeature(int item, FILE *out_file,
				  Format_style style)
{
  if (style==FMS_HTML) fprintf(out_file,"<TD align=right>");
  Elevation error=error_list[item].error;
  if (error==elev_undef) fprintf(out_file,"     ");
  else fprintf(out_file,Elev_intvl::elem_format,error);
  fprintf(out_file,get_delimiter(style,CA_RIGHTx));
  error=error_list[item].map_elev;
  if (error==elev_undef) fprintf(out_file,"     ");
  else fprintf(out_file,Elev_intvl::elem_format,error);
  if (style!=FMS_HTML) fprintf(out_file,get_delimiter(style,CA_CENTERx));
  CFeatureListView::printFeature(item,out_file,style);
}

void CErrorListView::printHeader(FILE *out_file, Format_style style)
{
  if (style==FMS_HTML) fprintf(out_file,"<TD align=center>");
  fprintf(out_file,"Error");
  fprintf(out_file,get_delimiter(style,CA_CENTERx));
  fprintf(out_file," Map ");
  if (style!=FMS_HTML) fprintf(out_file,get_delimiter(style,CA_CENTERx));
  CFeatureListView::printHeader(out_file,style);
}

bool CErrorListView::prelist()
{
  return CErrorFilter_dlg(error_filter,*GetDocument(),GetParent()).DoModal()==IDOK;
}

void CErrorListView::OnUnlistItem(NMHDR *pNMHDR, LRESULT *pResult)
{
  assert(GetListCtrl().GetItemCount()==error_list.size());
  int item=((NM_LISTVIEW *)pNMHDR)->iItem;
  error_list.erase(error_list.begin()+item);
	
  *pResult = 0;
}

void CErrorListView::OnInitialUpdate() 
{
  CFeatureListView::OnInitialUpdate();

  if (is_initial) {
    error_filter=init_ef;
    is_initial=false;
  }

  CPromDoc& doc=*GetDocument();
  Divide_tree& tree=doc.tree;
  Elev_map& elev=doc.elev;
  const Database& data=doc.data;
  Domain::Index i,n;
  Elevation map_elev,err;
  error_list.clear();
  AnalData *sadl_data=doc.error_stats.get_sadl_data();
  doc.list_stats(&doc.error_stats.peak_data,sadl_data);
  doc.error_stats.summarize();
  n=tree.n_dom();
  for (i=0; i<n; ++i) {
    Feature &p=tree[i].peak,&s=tree[i].saddle;
    err=getError(p,doc.error_stats.peak_data,map_elev);
    if (error_filter.test(p,FT_PEAK,err,data)) {
      error_list.push_back(Error(&p,FT_PEAK,map_elev,err));
    }
    err=getError(s,*sadl_data,map_elev);
    if (error_filter.test(s,FT_SADDLE,err,data)) {
      error_list.push_back(Error(&s,FT_SADDLE,map_elev,err));
    }
  }
  n=tree.n_runoff();
  for (i=0; i<n; ++i) {
    Feature &f=tree.runoff(i);
    err=getError(f,*sadl_data,map_elev);
    if (error_filter.test(f,FT_RUNOFF,err,data)) {
      error_list.push_back(Error(&f,FT_RUNOFF,map_elev,err));
    }
  }
  n=tree.n_bsnsdl();
  for (i=0; i<n; ++i) {
    Feature &f=tree.bsnsdl(i);
    err=getError(f,*sadl_data,map_elev);
    if (error_filter.test(f,FT_BASINSADL,err,data)) {
      error_list.push_back(Error(&f,FT_BASINSADL,map_elev,err));
    }
  }

  sort(error_list.begin(),error_list.end(),greater<Error>());
  GetListCtrl().SetItemCount(error_list.size());
  for (vector<Error>::iterator vi=error_list.begin(); vi!=error_list.end(); ++vi)
    AddItem(*(*vi).featr,(*vi).ft);

  setItemIndicators();
}

void CErrorListView::SetViewTitle()
{
  CPromDoc *document=GetDocument();
  GetParent()->SetWindowText(document->dt_name.IsEmpty()?
    "Elevation Errors":document->dt_name+" - Elevation Errors");
}

void CErrorListView::pasteClip(const Clip& clp)
{
  Feature *new_ftr;
  const AnalData *new_ftr_data;
  const CPromDoc& doc=*GetDocument();
  list_fmt.set(doc.data);
  Elevation err,map_elev;
  unsigned n=clp.ftr_count();
  for (unsigned i=0; i<n; ++i) {
    new_ftr=clp.list[i];
    new_ftr_data=doc.error_stats.get_data(clp.type[i]);
    err=getError(*new_ftr,*new_ftr_data,map_elev);
    error_list.push_back(Error(new_ftr,clp.type[i],map_elev,err));
    AddItem(*new_ftr,clp.type[i]);
  }
  setItemIndicators();
}

void CErrorListView::OnUpdateListLoad(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetDocument()->em_state!=NOT_LOADED);
}

static int compare_dir;

static int CALLBACK compare_map_elev(LPARAM p1, LPARAM p2, LPARAM plist)
{
  const CErrorListView *current_elv=(CErrorListView *)plist;
  const vector<Error>& error_list=((CErrorListView *)plist)->error_list;
  return (error_list[p2].map_elev-error_list[p1].map_elev)*compare_dir;
}

static int CALLBACK compare_error(LPARAM p1, LPARAM p2, LPARAM plist)
{
  const CErrorListView *current_elv=(CErrorListView *)plist;
  const vector<Error>& error_list=((CErrorListView *)plist)->error_list;
  return (abs(error_list[p2].error)-abs(error_list[p1].error))*compare_dir;
}

void CErrorListView::OnHeaderClick(NMHDR *pNMHDR, LRESULT *pResult)
{
  NM_LISTVIEW *pNMListView = (NM_LISTVIEW *)pNMHDR;
  CListCtrl& listCtrl=GetListCtrl();

  build_features();
  if (pNMListView->iSubItem==ELEV_COL) {
    // sort by map elevation
    compare_dir=GetKeyState(VK_SHIFT)<0 ? -1 : 1;
    listCtrl.SortItems(compare_map_elev,(LPARAM)this);
    *pResult = 0;
  }
  else if (pNMListView->iSubItem==ERROR_COL) {
    // sort by error
    compare_dir=GetKeyState(VK_SHIFT)<0 ? -1 : 1;
    listCtrl.SortItems(compare_error,(LPARAM)this);
    *pResult = 0;
  }
  else {
    CFeatureListView::OnHeaderClick(pNMHDR,pResult);
  }

  vector<Error> temp_list;
  int i,j,n=listCtrl.GetItemCount();
  temp_list.resize(n);
  for (i=0; i<n; ++i) {
    j=listCtrl.GetItemData(i);
    temp_list[i]=error_list[j];
    listCtrl.SetItemData(i,(LPARAM)features[j]);
  }
  error_list=temp_list;
}

void CErrorListView::swap_items(int nItem)
{
  Error temp=error_list[nItem];
  error_list[nItem]=error_list[nItem+1];
  error_list[nItem+1]=temp;
  CFeatureListView::swap_items(nItem);
}
