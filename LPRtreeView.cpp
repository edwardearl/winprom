// LPRtreeView.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "LPRtreeView.h"
#include "PromSadlListView.h"
#include "ParentListView.h"
#include "CycleListView.h"
#include "RunoffNbrListView.h"
#include "PeakNbrListView.h"
#include "TreeFrame.h"
#include "FeatureFmt_dlg.h"
#include "FeatureListSummary_dlg.h"
#include "TreePaste_dlg.h"
#include "RQFinfo_dlg.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static char buf[256];


CLPRtreeView::HPinfo *CLPRtreeView::getHPinfo(Domain *dom)
{
  for (unsigned i=0; i<MAX_HPINFO; ++i)
    if (lastHPs[i].HP==dom) return lastHPs+i;
  return 0;
}

void CLPRtreeView::useHP(Domain *dom, Elevation inner, Elevation outer)
{
  HPinfo *lastHP=getHPinfo(dom);
  if (lastHP==0) lastHP=lastHPs+(MAX_HPINFO-1);
  for (; lastHP>lastHPs; --lastHP)
    lastHP[0]=lastHP[-1];
  lastHPs[0].HP=dom;
  lastHPs[0].inner_prom=inner;
  lastHPs[0].outer_prom=outer;
}


/////////////////////////////////////////////////////////////////////////////
// CLPRtreeView

static void set_fact_fmt(Feature_fmt& fmt)
{
  fmt.show_loc=false;
  fmt.name_f=Feature_fmt::ALT_ELEV;
}

void CLPRtreeView::factory_initial_settings()
{
  set_fact_fmt(init_fmt);
}

void CLPRtreeView::load_initial_settings()
{
  Feature_fmt fact_init_fmt;
  set_fact_fmt(fact_init_fmt);
  init_fmt.load_registry("LPR format",fact_init_fmt);
}

void CLPRtreeView::save_initial_settings()
{
  init_fmt.save_registry("LPR format");
}

IMPLEMENT_DYNCREATE(CLPRtreeView, CTreeView)

CLPRtreeView::CLPRtreeView()
  : tree_item(0)
{
}

CLPRtreeView::~CLPRtreeView()
{
  delete[] tree_item;
}

bool CLPRtreeView::is_initial;
TreeFilter CLPRtreeView::init_tf;
Domain *CLPRtreeView::init_patriarch;
CLPRtreeView::HPinfo CLPRtreeView::lastHPs[MAX_HPINFO];
Feature_fmt CLPRtreeView::init_fmt;

BEGIN_MESSAGE_MAP(CLPRtreeView, CTreeView)
  //{{AFX_MSG_MAP(CLPRtreeView)
  ON_WM_CREATE()
  ON_COMMAND(ID_EDIT_FEATURE, OnEditFeature)
  ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
  ON_COMMAND(ID_EDIT_SPLICE, OnEditSplice)
  ON_UPDATE_COMMAND_UI(ID_EDIT_SPLICE, OnUpdateEditSplice)
  ON_COMMAND(ID_EDIT_CENTER, OnEditCenter)
  ON_COMMAND(ID_LIST_FORMAT, OnListFormat)
  ON_COMMAND(ID_LIST_RELIST, OnListRelist)
  ON_COMMAND(ID_EDIT_ROTATE, OnEditRotate)
  ON_UPDATE_COMMAND_UI(ID_EDIT_ROTATE, OnUpdateEditRotate)
  ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
  ON_COMMAND(ID_EDIT_UNLIST, OnEditUnlist)
  ON_COMMAND(ID_LIST_SADL, OnListSadl)
  ON_UPDATE_COMMAND_UI(ID_LIST_SADL, OnUpdateListSadl)
  ON_COMMAND(ID_LIST_NBRS, OnListNbrs)
  ON_UPDATE_COMMAND_UI(ID_LIST_NBRS, OnUpdateListNbrs)
  ON_COMMAND(ID_FILE_TEXT, OnFileText)
  ON_COMMAND(ID_FILE_HTML, OnFileHtml)
  ON_UPDATE_COMMAND_UI(ID_FILE_TEXT, OnUpdateHaveItems)
  ON_UPDATE_COMMAND_UI(ID_EDIT_UNLIST, OnUpdateEditUnlist)
  ON_COMMAND(ID_LIST_SUMMARY, OnListSummary)
  ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
  ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
  ON_COMMAND(ID_EDIT_SEARCH, OnEditSearch)
  ON_UPDATE_COMMAND_UI(ID_EDIT_SEARCH, OnUpdateEditSearch)
  ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
  ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
  ON_COMMAND(ID_LIST_TREE, OnListTree)
  ON_UPDATE_COMMAND_UI(ID_LIST_TREE, OnUpdateListTree)
  ON_WM_RBUTTONDOWN()
  ON_COMMAND(ID_FEATURE_CENTER, OnFeatureCenter)
  ON_COMMAND(ID_FEATURE_DELETE, OnFeatureDelete)
  ON_COMMAND(ID_FEATURE_EDIT, OnFeatureEdit)
  ON_COMMAND(ID_FEATURE_LIST_NBRS, OnFeatureListNbrs)
  ON_COMMAND(ID_FEATURE_LIST_SADDLES, OnFeatureListSaddles)
  ON_COMMAND(ID_FEATURE_LIST_TREE, OnFeatureListTree)
  ON_COMMAND(ID_FEATURE_UNLIST, OnFeatureUnlist)
  ON_COMMAND(ID_RUNOFF_SPLICE, OnRunoffSplice)
  ON_UPDATE_COMMAND_UI(ID_EDIT_CENTER, OnUpdateRealFeature)
  ON_UPDATE_COMMAND_UI(ID_EDIT_FEATURE, OnUpdateEditFeature)
  ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
  ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONUP()
  ON_COMMAND(ID_EDIT_BROWSE, OnEditBrowse)
  ON_COMMAND(ID_FEATURE_BROWSE, OnFeatureBrowse)
  ON_UPDATE_COMMAND_UI(ID_SAVE_DLPR_TREE, OnUpdateHaveDLPRtree)
  ON_COMMAND(ID_FEATURE_RQF, OnRQFinfo)
  ON_COMMAND(ID_EDIT_RQF, OnEditRQF)
  ON_UPDATE_COMMAND_UI(ID_EDIT_RQF, OnUpdateEditRQF)
  ON_COMMAND(ID_FEATURE_LIST_PARENTS, OnFeatureListParents)
  ON_UPDATE_COMMAND_UI(ID_FILE_HTML, OnUpdateHaveItems)
  ON_UPDATE_COMMAND_UI(ID_LIST_SUMMARY, OnUpdateHaveItems)
  ON_UPDATE_COMMAND_UI(ID_EDIT_BROWSE, OnUpdateRealFeature)
  ON_UPDATE_COMMAND_UI(ID_SAVEAS_DLPR_TREE, OnUpdateHaveDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_INFO_DLPR_TREE, OnUpdateHaveDLPRtree)
  ON_COMMAND(ID_LIST_PARENTS, OnListParents)
  ON_UPDATE_COMMAND_UI(ID_LIST_PARENTS, OnUpdateListParents)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLPRtreeView diagnostics

#ifdef _DEBUG
void CLPRtreeView::AssertValid() const
{
  CTreeView::AssertValid();
}

void CLPRtreeView::Dump(CDumpContext& dc) const
{
  CTreeView::Dump(dc);
}
#endif //_DEBUG


Feature& CLPRtreeView::GetFeature(HTREEITEM hItem) const
{
  return *(Feature *)GetTreeCtrl().GetItemData(hItem);
}

FT_index CLPRtreeView::GetFeatureImage(HTREEITEM hItem) const
{
  int img,dummy;
  if (GetTreeCtrl().GetItemImage(hItem,img,dummy))
    return (FT_index)img;
  return (FT_index)(-1);
}

FT_index CLPRtreeView::GetFeatureType(HTREEITEM hItem) const
{
  FT_index type=GetFeatureImage(hItem);
  GetDocument()->image2type(type);
  return type;
}

Feature& CLPRtreeView::SelectedFeature() const
{
  return GetFeature(GetTreeCtrl().GetSelectedItem());
}

FT_index CLPRtreeView::SelectedFeatureType() const
{
  return GetFeatureType(GetTreeCtrl().GetSelectedItem());
}

inline static float dampen(const Domain *dom, Elev_endpt which)
{
  float se=dom->parent.saddle->saddle.elev.*which;
  return (dom->peak.elev.*which-se)/(dom->parent.island->peak.elev.*which-se);
}

static float dampen(const Domain *dom, const Domain *end, Elev_endpt which)
{
  float answer=1;
  for (; dom!=end; dom=dom->parent.island)
    answer*=dampen(dom,which);
  return answer;
}

static Elev_intvl prop_prom(const Domain& dom, bool drainage,
			   float bcp, float wcp)
{
  if (drainage) {
    float t=bcp;
    bcp=wcp;
    wcp=t;
  }
  Elev_intvl pp;
  Elevation se=dom.parent.saddle->saddle.elev.low;
  pp.high=nint((dom.peak.elev.high-se)*bcp/(dom.parent.island->peak.elev.low-se));
  se=dom.parent.saddle->saddle.elev.high;
  pp.low=nint((dom.peak.elev.low-se)*wcp/(dom.parent.island->peak.elev.high-se));
  return pp;
}


/////////////////////////////////////////////////////////////////////////////
// CLPRtreeView message handlers

void CLPRtreeView::init_tree(CPromDoc *document, Domain& patrch, CWnd *parentWnd)
{
  document->update_sadl_prom_info();
  short int dc=document->drain_const();
  Elev_endpt inner=document->inner_endpt(),outer=document->outer_endpt();
  Domain *HP=patrch.parent_with_prom(elev_infty);
  float bcd,wcd;
  HPinfo *lastHP=getHPinfo(HP);
  init_tf.pp_child_best=dampen(&patrch,HP,inner);
  init_tf.pp_child_worst=dampen(&patrch,HP,outer);
  // initially, pp_child_best and pp_child_worst represent the tree HP
  if (lastHP!=0) {
    init_tf.pp_child_best*=lastHP->inner_prom;
    init_tf.pp_child_worst*=lastHP->outer_prom;
  }
  else {
    // (assuming tree patriarch is oceanic island HP)
    init_tf.pp_child_best*=HP->peak.elev.*inner;
    init_tf.pp_child_worst*=HP->peak.elev.*outer;
  }
  if (HP==&patrch) {
    // TODO: reevaluate
    bcd=wcd=1;
    float ratio=(float)patrch.peak.elev.high/patrch.peak.elev.low;
    init_tf.pp_patrch_prom.high=nint(init_tf.pp_child_best*ratio);
    init_tf.pp_patrch_prom.low=nint(init_tf.pp_child_worst/ratio);
  }
  else {
    bcd=dampen(patrch.parent.island,HP,inner);
    wcd=dampen(patrch.parent.island,HP,outer);
    float
      bcp=bcd*init_tf.pp_child_best,
      wcp=wcd*init_tf.pp_child_worst;
    bcd*=dampen(&patrch,inner);
    wcd*=dampen(&patrch,outer);
    // pp_child_best and pp_child_worst are modified to represent the view patriarch
    init_tf.pp_child_best=nint(bcd*init_tf.pp_child_best);
    init_tf.pp_child_worst=nint(wcd*init_tf.pp_child_worst);
    init_tf.pp_patrch_prom=prop_prom(patrch,document->is_drainage,bcp,wcp);
  }
  is_initial=CTreeFilter_dlg(init_tf,*document).DoModal()==IDOK;
  if (!is_initial) return;

  // update HP bcp & wcp
  if (init_tf.pp_enable)
    useHP(HP,nint(init_tf.pp_child_best/bcd),nint(init_tf.pp_child_worst/wcd));

  init_patriarch=&patrch;
  CSingleDocTemplate& LPRtreeViewTpl=CPromApp::getApp()->LPRtreeViewTpl;
  CTreeFrame *newtree=(CTreeFrame *)LPRtreeViewTpl.CreateNewFrame(document,NULL);
  LPRtreeViewTpl.InitialUpdateFrame(newtree,document);
}

BOOL CLPRtreeView::PreCreateWindow(CREATESTRUCT& cs) 
{
  cs.style|=TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT;
  return CTreeView::PreCreateWindow(cs);
}

int CLPRtreeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CTreeView::OnCreate(lpCreateStruct) == -1)
    return -1;

  CTreeCtrl& treeCtrl=GetTreeCtrl();

  CPromApp *app=CPromApp::getApp();
  treeCtrl.SetImageList(&app->smallImageList,TVSIL_NORMAL);
  treeCtrl.SetImageList(&app->flagImageList,TVSIL_STATE);

  return 0;
}

void CLPRtreeView::add_bs_leg(Basin_saddle& bs, Domain *start,
			      Divide_tree& tree)
{
  CTreeCtrl& treeCtrl=GetTreeCtrl();
  HTREEITEM hParent,hSaddle;
  while (start!=bs.common_ancestor) {
    hParent=tree_item[tree.index(start->parent.child)];
    if (hParent!=NULL && bs.can_rotate(start->saddle)) {
      hSaddle=treeCtrl.GetNextSiblingItem(treeCtrl.GetChildItem(hParent));
      AddItem(hParent,bs,FT_BASINSADL,hSaddle);
    }
    start=start->primary_nbr;
  }
}

struct Hiro_info {
  Elevation elev;
  Domain::Index iro;
  Hiro_info() : elev(-elev_infty),iro(Domain::undef) {}
};

void CLPRtreeView::OnInitialUpdate()
{
  BeginWaitCursor();

  CPromDoc *doc=GetDocument();
  bcp_field=doc->inner_endpt();
  wcp_field=doc->outer_endpt();
  drg_const=doc->drain_const();
  CToolBar& fb=((CTreeFrame *)GetParent())->fileBar;
  fb.SetWindowText(doc->is_drainage?"Drainage Tree":"Divide Tree");

  if (is_initial) {
    tree_filter=init_tf;
    tree_fmt=init_fmt;
    patriarch=init_patriarch;
    is_initial=false;
  }

  delete[] tree_item;
  tree_fmt.set(GetDocument()->data,FMS_NOFLAG);
  if (patriarch) {
    CTreeCtrl& treeCtrl=GetTreeCtrl();
    // Add domains to tree
    Divide_tree& tree=doc->tree;
    Domain::Index i,n=tree.n_dom();
    tree_item=new HTREEITEM[n];
    for (i=0; i<n; ++i) tree_item[i]=NULL;
    for (i=0; i<n; ++i) {
      Domain& dom=tree[i];
      if (dom.peak.location) AddDomain(i,true);
    }
    // Add peaks & saddles to tree
    AdjustBranch(TVI_ROOT);
    // Add basin saddles to tree
    n=tree.n_bsnsdl();
    for (i=0; i<n; ++i) {
      Basin_saddle& bs=tree.bsnsdl(i);
      add_bs_leg(bs,bs.peak1,tree);
      add_bs_leg(bs,bs.peak2,tree);
    }
    if (tree_filter.runoffs>TreeFilter::RO_NONE) {
      // Add runoffs to tree
      n=tree.n_runoff();
      Domain *ro_parent;
      HTREEITEM hParent;
      bool *is_hiro=0;
      if (tree_filter.runoffs==TreeFilter::RO_HIGH) {
	// determine the highest runoff for each domain
	Domain::Index ndom=tree.n_dom(),idom;
	Hiro_info *hiro_info=new Hiro_info[ndom];
	Elevation ro_eff_elev;
	for (i=0; i<n; ++i) {
	  Runoff& ro=tree.runoff(i);
	  ro_parent=ro.peak;
	  ro_eff_elev=ro.elev.high;
	  while (ro_parent) {
	    idom=tree.index(ro_parent);
	    if (ro_eff_elev<=hiro_info[idom].elev) break;
	    hiro_info[idom].elev=ro_eff_elev;
	    hiro_info[idom].iro=i;
	    if (!ro_parent->parent.line) break;
	    if (ro_parent->parent.saddle->saddle.elev.high<ro_eff_elev)
	      ro_eff_elev=ro_parent->parent.saddle->saddle.elev.high;
	    ro_parent=tree_parent(ro_parent);
	  }
	}
	// make a list of runoffs that are the highest runoff of some domain
	is_hiro=new bool[n];
	for (i=0; i<n; ++i) is_hiro[i]=false;
	for (idom=0; idom<ndom; ++idom) {
	  if (tree_item[idom]==NULL) continue;
	  i=hiro_info[idom].iro;
	  if (i!=Domain::undef) is_hiro[i]=true;
	}
	delete[] hiro_info;
      }
      // finally, add them to the tree
      for (i=0; i<n; ++i) {
	if (tree_filter.runoffs==TreeFilter::RO_HIGH && !is_hiro[i]) continue;
	Runoff& ro=tree.runoff(i);
	ro_parent=ro.peak;
	if (!ro_parent) continue;
	while (ro_parent->parent.island &&
	       ro.elev<ro_parent->parent.saddle->saddle.elev &&
	       tree_item[tree.index(ro_parent)]==NULL) {
	  ro_parent=tree_parent(ro_parent);
	}
	hParent=pasteLineage(ro_parent);
	AddItem(hParent,ro,FT_RUNOFF);
	// mark ancestors with runoff overlay
	for (; hParent!=NULL; hParent=treeCtrl.GetParentItem(hParent)) {
	  treeCtrl.SetItemState(hParent,INDEXTOOVERLAYMASK(1),TVIS_OVERLAYMASK);
	}
      }
      delete[] is_hiro;
    }
    treeCtrl.Expand(treeCtrl.GetRootItem(),TVE_EXPAND);
  }
  else tree_item=0;

  EndWaitCursor();
}

void CLPRtreeView::SetBranchText(const Domain& branch) const
{
  char *lbuf;
  CPromDoc *doc=GetDocument();
  CTreeCtrl& treeCtrl=GetTreeCtrl();
  if (branch.peak.name) sprintf(buf,"%s,  ",branch.peak.name);
  else if (tree_fmt.name_f==Feature_fmt::ALT_ELEV) {
    branch.peak.elev.sprint(buf);
    // trim leading & trailing blanks
    lbuf=buf;
    while (isspace(*lbuf)) ++lbuf;
    if (lbuf[0]!='\0') {
      char *rbuf=lbuf+strlen(lbuf);
      while (isspace(rbuf[-1])) --rbuf;
      *rbuf='\0';
    }
    if (lbuf>buf) strcpy(buf,lbuf);
    strcat(buf,",  ");
  }
  else if (tree_fmt.name_f==Feature_fmt::ALT_LOC) {
    branch.peak.location.sprint(buf);
    strcat(buf,",  ");
  }
  else buf[0]='\0';

  if (tree_filter.pp_enable) {
    strcat(buf,"PP =");
    lbuf=buf+strlen(buf);
    if (&branch==patriarch) tree_filter.pp_patrch_prom.sprint(lbuf);
    else {
      float
	bcp=dampen(branch.parent.island,patriarch,bcp_field)*tree_filter.pp_child_best,
	wcp=dampen(branch.parent.island,patriarch,wcp_field)*tree_filter.pp_child_worst;
      lbuf=prop_prom(branch,doc->is_drainage,bcp,wcp).sprint(lbuf);
      // trim trailing blanks
      while (isspace(lbuf[-1])) --lbuf;
      *lbuf='\0';
      sprintf(lbuf,", M = %.2f to %.2f",
	      wcp/(branch.parent.island->peak.elev.*wcp_field-
		   branch.parent.saddle->saddle.elev.*bcp_field),
	      bcp/(branch.parent.island->peak.elev.*bcp_field-
		   branch.parent.saddle->saddle.elev.*wcp_field));
    }
  }
  else {
    Elev_intvl std_prom=branch.parent.line ?
      branch.peak.elev-branch.parent.saddle->saddle.elev :
      elev_infty*(doc->drain_const());
    strcat(buf,"Prom =");
    lbuf=buf+strlen(buf);
    std_prom.sprint(lbuf);
  }
}

HTREEITEM CLPRtreeView::AddDomain(Domain::Index i, bool use_filter)
{
  if (tree_item[i]!=NULL) return tree_item[i];
  Divide_tree& tree=GetDocument()->tree;
  Domain *dom=&tree[i];
  CPromDoc *doc=GetDocument();
  if (use_filter && !tree_filter.test(*dom,doc->data,doc))
    return NULL;
  HTREEITEM hParent;
  if (dom==patriarch) {
    hParent=TVI_ROOT;
  }
  else {
    Domain *parnt=tree_parent(dom);
    if (!parnt) return NULL;
    hParent=AddDomain(tree.index(parnt),use_filter);
    if (hParent==NULL) return NULL;
  }
  SetBranchText(*dom);
  CTreeCtrl& treeCtrl=GetTreeCtrl();
  tree_item[i]=treeCtrl.InsertItem(buf,FT_TREE,FT_TREE,hParent);
  treeCtrl.SetItemData(tree_item[i],(DWORD)dom);
  return tree_item[i];
}

HTREEITEM CLPRtreeView::NewDom(Domain *dom, HTREEITEM hParent)
{
  CTreeCtrl& treeCtrl=GetTreeCtrl();

  SetBranchText(*dom);
  HTREEITEM hBranch=treeCtrl.InsertItem(buf,FT_TREE,FT_TREE,hParent);
  treeCtrl.SetItemData(hBranch,(DWORD)dom);
  AddItem(hBranch,dom->peak,FT_PEAK);
  if (dom->parent.line) AddItem(hBranch,dom->parent.saddle->saddle,FT_SADDLE);
  return hBranch;
}

HTREEITEM CLPRtreeView::AddItem(HTREEITEM hParent,
				Feature& featr, FT_index type,
				HTREEITEM after)
{
  CTreeCtrl& tree=GetTreeCtrl();
  GetDocument()->type2image(type);
  HTREEITEM hItem=tree.InsertItem(0,type,type,hParent,after);
  tree.SetItemData(hItem,(DWORD)&featr);
  UpdateItem(hItem);
  return hItem;
}


int CALLBACK cmp_peak_elev(LPARAM lParam1, LPARAM lParam2, LPARAM)
{
  Domain *dom1=(Domain *)lParam1,*dom2=(Domain *)lParam2;
  return dom2->peak.elev.high-dom1->peak.elev.high;
}

int CALLBACK cmp_sadl_elev(LPARAM lParam1, LPARAM lParam2, LPARAM)
{
  Domain *dom1=(Domain *)lParam1,*dom2=(Domain *)lParam2;
  assert(dom1->parent.line && dom2->parent.line);
  Elevation
    se1=dom1->parent.saddle->saddle.elev.low,
    se2=dom2->parent.saddle->saddle.elev.low;
  return se2-se1;
}

int CALLBACK cmp_prom(LPARAM lParam1, LPARAM lParam2, LPARAM)
{
  Domain *dom1=(Domain *)lParam1,*dom2=(Domain *)lParam2;
  assert(dom1->parent.line && dom2->parent.line);
  return dom2->peak_topo_prom()-dom1->peak_topo_prom();
}

// Sort branches and add peaks & saddles.
void CLPRtreeView::AdjustBranch(HTREEITEM hBranch)
{
  CTreeCtrl& treeCtrl=GetTreeCtrl();
  HTREEITEM hChild;
  TVSORTCB sort;
  sort.hParent=hBranch;
  switch (tree_filter.child_sorting) {
  case TreeFilter::CS_SADL_ELEV: sort.lpfnCompare=cmp_sadl_elev; break;
  case TreeFilter::CS_PEAK_ELEV: sort.lpfnCompare=cmp_peak_elev; break;
  case TreeFilter::CS_PROM: sort.lpfnCompare=cmp_prom; break;
  default: assert(0);
  }
  treeCtrl.SortChildrenCB(&sort);
  for (hChild=treeCtrl.GetChildItem(hBranch); hChild!=NULL;
       hChild=treeCtrl.GetNextSiblingItem(hChild)) {
    AdjustBranch(hChild);
  }

  if (hBranch!=TVI_ROOT) {
    Domain *dom=(Domain *)treeCtrl.GetItemData(hBranch);
    hChild=AddItem(hBranch,dom->peak,FT_PEAK,TVI_FIRST);
    if (dom->parent.line)
      AddItem(hBranch,dom->parent.saddle->saddle,FT_SADDLE,hChild);
  }

}

HTREEITEM CLPRtreeView::GetFeatureHandle(HTREEITEM hBranch,
					 const Feature *target) const
{
  CTreeCtrl& treeCtrl=GetTreeCtrl();
  HTREEITEM hChild,result;
  for (hChild=treeCtrl.GetChildItem(hBranch); hChild!=NULL;
       hChild=treeCtrl.GetNextSiblingItem(hChild)) {
    if (GetFeatureType(hChild)<FT_NUM_TYPES && &GetFeature(hChild)==target)
      return hChild;
    if (treeCtrl.ItemHasChildren(hChild)) {
      result=GetFeatureHandle(hChild,target);
      if (result!=NULL) return result;
    }
  }
  return NULL;
}

void CLPRtreeView::UpdateItem(HTREEITEM hFeatr)
{
  CTreeCtrl& treeCtrl=GetTreeCtrl();
  Feature& featr=GetFeature(hFeatr);

  UINT stateMask;
  if (featr.edge_effect) {
    if (featr.edited) stateMask=INDEXTOSTATEIMAGEMASK(1);
    else stateMask=INDEXTOSTATEIMAGEMASK(2);
  }
  else {
    if (featr.edited) stateMask=INDEXTOSTATEIMAGEMASK(15);
    else stateMask=INDEXTOSTATEIMAGEMASK(3);
  }
  treeCtrl.SetItemState(hFeatr,stateMask,TVIS_STATEIMAGEMASK);

  featr.sprint(buf);
  treeCtrl.SetItemText(hFeatr,buf);
}

void CLPRtreeView::UpdateBranch(HTREEITEM hParent, Feature *target)
{
  CTreeCtrl& treeCtrl=GetTreeCtrl();
  CPromDoc *doc=GetDocument();
  HTREEITEM hChild;
  Feature *tree_feature;
  FT_index image;

  Domain *dom=(Domain *)treeCtrl.GetItemData(hParent);
  if (target==&CPromDoc::dummyModifyAll ||
      target==&CPromDoc::dummyModifyTrans ||
      target==&dom->peak || target==&dom->parent.saddle->saddle) {
    SetBranchText(*dom);
    treeCtrl.SetItemText(hParent,buf);
  }
  for (hChild=treeCtrl.GetChildItem(hParent); hChild!=NULL;
       hChild=treeCtrl.GetNextSiblingItem(hChild)) {
    image=GetFeatureImage(hChild);
    if (image==FT_TREE) UpdateBranch(hChild,target);
    else {
      tree_feature=&GetFeature(hChild);
      if (target==&CPromDoc::dummyModifyRObase) {
	doc->image2type(image);
	if (image==FT_RUNOFF) {
	  Domain::Index iro=(Runoff *)tree_feature-doc->old_ro;
	  treeCtrl.SetItemData(hChild,
	    (DWORD)&doc->tree.bsnsdl(iro+(iro>=doc->new_iro)));
	}
      }
      else if (target==&CPromDoc::dummyModifyBSbase) {
	doc->image2type(image);
	if (image==FT_BASINSADL)
	  treeCtrl.SetItemData(hChild,
	    (DWORD)&doc->tree.bsnsdl((Basin_saddle *)tree_feature-doc->old_bs));
      }
      else if (target==&CPromDoc::dummyModifyAll ||
	       target==&CPromDoc::dummyModifyTrans || target==tree_feature) {
	if (tree_feature->location) {
	  UpdateItem(hChild);
	  if (target==&CPromDoc::dummyModifyTrans && doc->trans_m<0) {
	    CPromDoc::imageFlip(image);
	    treeCtrl.SetItemImage(hChild,image,image);
	  }
	}
	else treeCtrl.DeleteItem(hChild);
      }
    }
  }
}

void CLPRtreeView::CopyBranch(HTREEITEM hBranch, Clip& clp) const
{
  CTreeCtrl& treeCtrl=GetTreeCtrl();
  HTREEITEM hChild;
  FT_index type;
  for (hChild=treeCtrl.GetChildItem(hBranch); hChild!=NULL;
       hChild=treeCtrl.GetNextSiblingItem(hChild)) {
    type=GetFeatureType(hChild);
    if (type<FT_NUM_TYPES) {
      clp.add_featr(GetFeature(hChild),type);
    }
    if (treeCtrl.ItemHasChildren(hChild))
      CopyBranch(hChild,clp);
  }
}

void CLPRtreeView::PrintBranch(FILE *file, HTREEITEM hBranch, bool html) const
{
  static unsigned indent=0;
  CTreeCtrl& treeCtrl=GetTreeCtrl();
  HTREEITEM hChild;
  FT_index type;
  SetBranchText(*(Domain *)treeCtrl.GetItemData(hBranch));

  if (html) {
    fprintf(file,"<li>%s</li>\n",buf);
    fprintf(file,"<ul>\n");
  }
  else {
    fprintf(file,"%*s%s\n",indent*2,"",buf);
    ++indent;
  }

  for (hChild=treeCtrl.GetChildItem(hBranch); hChild!=NULL;
       hChild=treeCtrl.GetNextSiblingItem(hChild)) {
    type=GetFeatureType(hChild);
    if (type<FT_NUM_TYPES) {
      if (html) fprintf(file,"<li>");
      else fprintf(file,"%*s",indent*2,"");
      switch (type) {
      case FT_PEAK: fprintf(file,"Peak   "); break;
      case FT_SADDLE: fprintf(file,"Saddle "); break;
      case FT_RUNOFF: fprintf(file,"Runoff "); break;
      case FT_BASINSADL: fprintf(file,"BS     "); break;
      default: assert(0);
      }
      GetFeature(hChild).print(file);
      if (html) fprintf(file,"</li>");
      putc('\n',file);
    }
    else {
      PrintBranch(file,hChild,html);
    }
  }
  if (html) fprintf(file,"</ul>\n");
  else --indent;
}

void CLPRtreeView::BranchSummary(HTREEITEM hBranch, int& count,
				 Elev_intvl& elev, Rectangl& area) const
{
  CTreeCtrl& treeCtrl=GetTreeCtrl();
  FT_index type;
  for (HTREEITEM hChild=treeCtrl.GetChildItem(hBranch); hChild!=NULL;
       hChild=treeCtrl.GetNextSiblingItem(hChild)) {
    Feature& feature=GetFeature(hChild);
    type=GetFeatureType(hChild);
    if (type<FT_NUM_TYPES) {
      ++count;
      elev|=feature.elev;
      area|=feature.location;
    }
    if (treeCtrl.ItemHasChildren(hChild))
      BranchSummary(hChild,count,elev,area);
  }
}

void CLPRtreeView::OnUpdate(CView *, LPARAM lHint, CObject *pHint)
{
  Feature *changed_feature=(Feature *)lHint;
  if (changed_feature==&CPromDoc::dummyModifyData ||
      changed_feature==&CPromDoc::dummyModifyBounds ||
      changed_feature==&CPromDoc::dummyModifyMap ||
      changed_feature==&CPromDoc::dummySaveCenter) return;

  CTreeCtrl& treeCtrl=GetTreeCtrl();
  CPromDoc *doc=GetDocument();
  bcp_field=doc->inner_endpt();
  wcp_field=doc->outer_endpt();
  drg_const=doc->drain_const();

  Divide_tree& tree=doc->tree;
  if (lHint==0) {
    treeCtrl.DeleteAllItems();
    patriarch=0;
    return;
  }

  if (changed_feature==&CPromDoc::dummyModifyTopo ||
      changed_feature==&CPromDoc::dummyModifySplice) {
    GetTreeCtrl().DeleteAllItems();
    OnInitialUpdate();
    return;
  }
  if (changed_feature==&CPromDoc::dummyModifyTrans && doc->trans_m<0) {
    CToolBar& fb=((CTreeFrame *)GetParent())->fileBar;
    fb.SetWindowText(doc->is_drainage?"Drainage Tree":"Divide Tree");
  }
  tree_fmt.set(doc->data,FMS_NOFLAG);
  UpdateBranch(treeCtrl.GetRootItem(),changed_feature);
}

void CLPRtreeView::OnEditFeature()
{
  GetDocument()->EditFeature(SelectedFeature(),SelectedFeatureType(),this);
}

void CLPRtreeView::OnUpdateEditFeature(CCmdUI *pCmdUI)
{
  FT_index type=SelectedFeatureType();
  pCmdUI->Enable(tree_writeable() && type>=0 && type<FT_NUM_TYPES);
}

void CLPRtreeView::OnDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
  if (!tree_writeable()) {
    MessageBox("You cannot edit a feature here because\n"
	       "some feature is already open for editing.",
	       NULL,MB_OK|MB_ICONERROR);
    return;
  }
  FT_index type=SelectedFeatureType();
  if (type>=0 && type<FT_NUM_TYPES)
    GetDocument()->EditFeature(SelectedFeature(),type,this);

  *pResult = 0;
}

void CLPRtreeView::OnUpdateRealFeature(CCmdUI *pCmdUI)
{
  FT_index type=SelectedFeatureType();
  pCmdUI->Enable(type>=0 && type<FT_NUM_TYPES);
}

void CLPRtreeView::copyClip(Clip& clp) const
{
  FT_index type=SelectedFeatureType();
  clp.clear();
  if (type<FT_TREE) {
    clp.clear();
    clp.add_featr(SelectedFeature(),type);
  }
  else if (type==FT_TREE) {
    HTREEITEM hBranch=GetTreeCtrl().GetSelectedItem();
    CopyBranch(hBranch,clp);
  }
  else assert(0);
}

void CLPRtreeView::OnEditCopy()
{
  copyClip(GetDocument()->copyPaste);
}

HTREEITEM CLPRtreeView::pasteLineage(Domain *dom)
{
  assert(dom!=0);
  CTreeCtrl& treeCtrl=GetTreeCtrl();
  HTREEITEM hBranch=tree_item[GetDocument()->tree.index(dom)];
  if (hBranch!=NULL) return hBranch;
  if (dom==patriarch) return NewDom(dom);
  return NewDom(dom,pasteLineage(tree_parent(dom)));
}

bool CLPRtreeView::checkPaste(Domain *dom, HTREEITEM& hParent,
			      Feature& featr, FT_index type)
{
  CTreeCtrl& treeCtrl=GetTreeCtrl();
  if (inquirePaste) {
    Domain *domx;
    for (domx=dom; domx!=NULL && domx!=patriarch;
	 domx=tree_parent(domx));
    CTreePaste_dlg dlg(this,GetDocument(),featr,type,domx!=NULL);
    actPaste=dlg.DoModal();
    switch (actPaste) {
    case IDC_PASTE:
      hParent=pasteLineage(dom);
    case IDC_SKIP:
      break;
    case IDC_ABORT: return false;
    default: assert(0);
    }
    inquirePaste=dlg.m_show_next!=FALSE;
  }
  return true;
}

void CLPRtreeView::pasteClip(const Clip& clp)
{
  CTreeCtrl& treeCtrl=GetTreeCtrl();
  Divide_tree& tree=GetDocument()->tree;
  Feature *featr;
  Domain *dom;
  HTREEITEM hParent,firstItem;
  inquirePaste=true;
  for (int i=0; i<clp.ftr_count(); ++i) {
    featr=clp.list[i];
    switch (clp.type[i]) {
    case FT_PEAK:
      hParent=tree_item[tree.index(Domain::FromPeak(featr))];
      if (hParent!=NULL) {
	// the peak's domain is there
	if (GetFeatureType(treeCtrl.GetChildItem(hParent))!=FT_PEAK)
	  // but the peak isn't. Add it.
	  AddItem(hParent,*featr,FT_PEAK,TVI_FIRST);
      }
      else {
	// the peak's domain is not there.
	dom=tree_parent(Domain::FromPeak(featr));
	hParent=tree_item[tree.index(dom)];
	if (hParent==NULL)
	  // the peak's parent's domain is not there.
	  // Inquire about pasting lineage.
	  if (!checkPaste(dom,hParent,*featr,FT_PEAK)) return;
	if (hParent!=NULL)
	  NewDom(Domain::FromPeak(featr),hParent);
      }
      break;
    case FT_SADDLE:
      dom=Domain::FromSadl(featr);
      hParent=tree_item[tree.index(dom)];
      if (hParent==NULL)
	if (!checkPaste(dom,hParent,*featr,FT_SADDLE)) return;
      if (hParent!=NULL) {
	firstItem=treeCtrl.GetChildItem(hParent);
	switch (GetFeatureType(firstItem)) {
	case FT_PEAK:
	  if (GetFeatureType(treeCtrl.GetNextSiblingItem(firstItem))!=FT_SADDLE)
	    AddItem(hParent,*featr,FT_SADDLE,firstItem);
	  break;
	case FT_SADDLE:
	  break;
	default:
	  AddItem(hParent,*featr,FT_SADDLE,TVI_FIRST);
	  break;
	}
      }
      break;
    case FT_RUNOFF:
      dom=((Runoff *)featr)->peak;
      hParent=tree_item[tree.index(dom)];
      if (hParent==NULL)
	if (!checkPaste(dom,hParent,*featr,FT_RUNOFF)) return;
      if (hParent!=NULL) {
	HTREEITEM child;
	for (child=treeCtrl.GetChildItem(hParent); child!=NULL;
	     child=treeCtrl.GetNextSiblingItem(child))
	  if (GetFeatureType(child)==FT_RUNOFF &&
	      featr==&GetFeature(child)) break;
	if (child==NULL) AddItem(hParent,*featr,FT_RUNOFF);
      }
      break;
    case FT_BASINSADL:
      break;
    default:
      assert(0);
    }
  }
}

void CLPRtreeView::OnEditPaste()
{
  pasteClip(GetDocument()->copyPaste);
}

void CLPRtreeView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetDocument()->copyPaste.ftr_count()>0);
}

void CLPRtreeView::OnEditUnlist()
{
  CTreeCtrl& treeCtrl=GetTreeCtrl();
  treeCtrl.DeleteItem(treeCtrl.GetSelectedItem());
}

void CLPRtreeView::OnUpdateEditUnlist(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetTreeCtrl().GetSelectedItem()!=NULL);
}

void CLPRtreeView::OnEditCenter()
{
  CPromApp::getApp()->viewFeature(GetDocument(),SelectedFeature(),SelectedFeatureType());
}

void CLPRtreeView::OnEditBrowse()
{
  CPromApp::getApp()->Navigate(GetDocument(),SelectedFeature(),SelectedFeatureType());
}

void CLPRtreeView::OnEditSearch()
{
  CTreeCtrl& treeCtrl=GetTreeCtrl();
  HTREEITEM hItem=
    GetFeatureHandle(treeCtrl.GetRootItem(),GetDocument()->copyPaste.list[0]);
  if (hItem!=NULL) {
    treeCtrl.SelectItem(hItem);
    treeCtrl.EnsureVisible(hItem);
  }
  else {
    MessageBox("The feature on the clipboard is not in this tree.",
	       "Winprom Search");
  }
}

void CLPRtreeView::OnUpdateEditSearch(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetDocument()->copyPaste.ftr_count()==1);
}

void CLPRtreeView::OnEditSplice()
{
  GetDocument()->Splice((Runoff&)SelectedFeature(),this);
}

void CLPRtreeView::OnUpdateEditSplice(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(tree_writeable() && SelectedFeatureType()==FT_RUNOFF);
}

void CLPRtreeView::OnEditRotate()
{
  GetDocument()->Rotate((Basin_saddle&)SelectedFeature(),0,this);
}

void CLPRtreeView::OnUpdateEditRotate(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(tree_writeable() && SelectedFeatureType()==FT_BASINSADL);
}

void CLPRtreeView::OnEditDelete()
{
  GetDocument()->DeleteFeature(SelectedFeature(),SelectedFeatureType(),GetParent());
}

void CLPRtreeView::OnUpdateEditDelete(CCmdUI *pCmdUI)
{
  FT_index type=SelectedFeatureType();
  pCmdUI->Enable(tree_writeable() &&
		 (type==FT_PEAK || type==FT_SADDLE ||
		  type==FT_RUNOFF || type==FT_BASINSADL));
}

void CLPRtreeView::OnListFormat()
{
  const Database& data=GetDocument()->data;
  if (CFeatureFmt_dlg(data,this,tree_fmt).DoModal()==IDOK) {
    tree_fmt.set(data,FMS_NOFLAG);
    init_fmt=tree_fmt;
    UpdateBranch(GetTreeCtrl().GetRootItem(),&CPromDoc::dummyModifyAll);
  }
}

void CLPRtreeView::OnListRelist() 
{
  CPromDoc *doc=GetDocument();
  if (CTreeFilter_dlg(tree_filter,*doc,this).DoModal()!=IDOK) return;
  if (tree_filter.pp_enable) {
    Domain *HP=patriarch->patriarch();
    useHP(HP,
	  nint(tree_filter.pp_child_best/dampen(patriarch,HP,bcp_field)),
	  nint(tree_filter.pp_child_worst/dampen(patriarch,HP,wcp_field)));
  }
  GetTreeCtrl().DeleteAllItems();
  OnInitialUpdate();
}

void CLPRtreeView::SetViewTitle()
{
  CPromDoc *document=GetDocument();
  GetParent()->SetWindowText(document->dt_name.IsEmpty()?
    "Hierarchy":document->dt_name+" - Hierarchy");
}

void CLPRtreeView::ListSadl(Feature& featr) const
{
  CPromSadlListView::init_list(GetDocument(),
    *Domain::FromPeak(&featr),GetParent());
}

void CLPRtreeView::ListCycle(Feature& featr) const
{
  CCycleListView::init_list(GetDocument(),
    *(Basin_saddle *)&featr,GetParent());
}

void CLPRtreeView::OnListSadl()
{
  switch (SelectedFeatureType()) {
  case FT_PEAK:
    ListSadl(SelectedFeature());
    break;
  case FT_BASINSADL:
    ListCycle(SelectedFeature());
    break;
  default:
    assert(0);
  }
}

void CLPRtreeView::OnUpdateListSadl(CCmdUI *pCmdUI)
{
  FT_index selType=SelectedFeatureType();
  pCmdUI->Enable(selType==FT_PEAK || selType==FT_BASINSADL);
}

void CLPRtreeView::ListParents(Feature& peak) const
{
  CParentListView::init_list(GetDocument(),
    *Domain::FromPeak(&peak),GetParent());
}

void CLPRtreeView::OnListParents()
{
  ListParents(SelectedFeature());
}

void CLPRtreeView::OnUpdateListParents(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(SelectedFeatureType()==FT_PEAK);
}

void CLPRtreeView::ListNbrs(Feature& featr, FT_index ft) const
{
  switch (ft) {
  case FT_PEAK:
    CPeakNbrListView::init_list(GetDocument(),*Domain::FromPeak(&featr));
    break;
  case FT_RUNOFF:
    CRunoffNbrListView::init_list(GetDocument(),(Runoff&)featr);
    break;
  default:
    assert(0);
  }
}

void CLPRtreeView::OnListNbrs()
{
  ListNbrs(SelectedFeature(),SelectedFeatureType());
}

void CLPRtreeView::OnUpdateListNbrs(CCmdUI *pCmdUI)
{
  FT_index selType=SelectedFeatureType();
  pCmdUI->Enable(selType==FT_PEAK || selType==FT_RUNOFF);
}

void CLPRtreeView::OnListSummary()
{
  CFeatureListSummary_dlg(this).DoModal();
}

void CLPRtreeView::OnFileText()
{
  CString name=CPromDoc::GetFileName(CPromDoc::FO_SAVE,0,"Text","txt",GetParent());
  if (name.IsEmpty()) return;
  FILE *out_file=fopen(name,"w");
  if (out_file==0) {
    CPromDoc::FileError("opening output file",name);
    return;
  }
  tree_fmt.set(GetDocument()->data,FMS_NOFLAG);
  PrintBranch(out_file,GetTreeCtrl().GetRootItem());
  fclose(out_file);
}

void CLPRtreeView::OnFileHtml()
{
  CString name=CPromDoc::GetFileName(CPromDoc::FO_SAVE,0,"HTML","htm",GetParent());
  if (name.IsEmpty()) return;
  FILE *out_file=fopen(name,"w");
  if (out_file==0) {
    CPromDoc::FileError("opening output HTML file",name);
    return;
  }
  tree_fmt.set(GetDocument()->data,FMS_NOFLAG);
  fprintf(out_file,"<HTML> <UL>\n");
  PrintBranch(out_file,GetTreeCtrl().GetRootItem(),true);
  fprintf(out_file,"</UL></HTML>\n");
  fclose(out_file);
}

void CLPRtreeView::OnUpdateHaveItems(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetTreeCtrl().GetRootItem()!=NULL);
}

void CLPRtreeView::OnListTree()
{
  CLPRtreeView::init_tree(GetDocument(),
    *Domain::FromPeak(&SelectedFeature()),GetParent());
}

void CLPRtreeView::OnUpdateListTree(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(SelectedFeatureType()==FT_PEAK);
}

void CLPRtreeView::OnRButtonDown(UINT nFlags, CPoint point)
{
  CTreeView::OnRButtonDown(nFlags, point);

  CTreeCtrl& treeCtrl=GetTreeCtrl();
  r_click_item=treeCtrl.HitTest(point);
  if (r_click_item==NULL) return;

  r_click_featr=&GetFeature(r_click_item);
  r_click_type=GetFeatureType(r_click_item);
  if (r_click_type>=FT_NUM_TYPES) return;
  CMenu graphMenu,*popup;
  graphMenu.LoadMenu(IDR_MENU_POPUP);
  popup=graphMenu.GetSubMenu(0);
  GetDocument()->setup_popup(popup,*r_click_featr,r_click_type);
  ClientToScreen(&point);
  popup->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,point.x,point.y,this);
}

void CLPRtreeView::OnFeatureCenter()
{
  CPromApp::getApp()->viewFeature(GetDocument(),*r_click_featr,r_click_type);
}

void CLPRtreeView::OnRQFinfo()
{
  CRQFinfo_dlg(GetDocument(),r_click_featr,r_click_type,this).DoModal();
}

void CLPRtreeView::OnFeatureBrowse()
{
  CPromApp::getApp()->Navigate(GetDocument(),*r_click_featr,r_click_type);
}

void CLPRtreeView::OnFeatureDelete()
{
  GetDocument()->DeleteFeature(*r_click_featr,r_click_type,GetParent());
}

void CLPRtreeView::OnFeatureEdit()
{
  GetDocument()->EditFeature(*r_click_featr,r_click_type,this);
}

void CLPRtreeView::OnFeatureListNbrs()
{
  ListNbrs(*r_click_featr,r_click_type);
}

void CLPRtreeView::OnFeatureListSaddles()
{
  ListSadl(*r_click_featr);
}

void CLPRtreeView::OnFeatureListParents()
{
  ListParents(*r_click_featr);
}

void CLPRtreeView::OnFeatureListTree()
{
  CLPRtreeView::init_tree(GetDocument(),
    *Domain::FromPeak(r_click_featr),GetParent());
}

void CLPRtreeView::OnFeatureUnlist()
{
  GetTreeCtrl().DeleteItem(r_click_item);
}

void CLPRtreeView::OnRunoffSplice()
{
  GetDocument()->Splice(*(Runoff *)r_click_featr,this);
}

void CLPRtreeView::OnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
  NM_TREEVIEW *pNMTreeView = (NM_TREEVIEW *)pNMHDR;
  if (!tree_writeable()) AfxGetApp()->OnIdle(0);
  *pResult = 0;
}

void CLPRtreeView::OnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
  NM_TREEVIEW *pNMTreeView = (NM_TREEVIEW *)pNMHDR;
  GetTreeCtrl().SelectItem(pNMTreeView->itemNew.hItem);
  CPromDoc *document=GetDocument();
  copyClip(document->dragDrop);
  CPromApp::getApp()->smallImageList.
    BeginDrag(SelectedFeatureType()==FT_TREE ?
		FT_TREE : document->dragDrop.iconType(document->is_drainage),
	      CPoint(SMALL_ICON_SIZE/2,SMALL_ICON_Y_OFFSET));
  CImageList::DragEnter(NULL,pNMTreeView->ptDrag);
  SetCapture();

  *pResult = 0;
}

void CLPRtreeView::OnMouseMove(UINT nFlags, CPoint point)
{
  CPromDoc *doc=GetDocument();
  if ((nFlags&MK_LBUTTON) && doc->dragDrop.ftr_count()>0) {
    // dragging
    CPoint ptRoot(point);
    MapWindowPoints(NULL,&ptRoot,1);
    CImageList::DragMove(ptRoot);
    SetCursor(AfxGetApp()->LoadStandardCursor(doc->
      canTakeDrop(this,point) ? IDC_ARROW : IDC_NO));
  }

  CTreeView::OnMouseMove(nFlags,point);
}

void CLPRtreeView::OnLButtonUp(UINT nFlags, CPoint point)
{
  GetDocument()->takeDrop(this,point);

  CTreeView::OnLButtonUp(nFlags,point);
}

void CLPRtreeView::OnUpdateHaveDLPRtree(CCmdUI *pCmdUI)
{
  // override the CPromDoc version, which also changes the text!
  pCmdUI->Enable(GetDocument()->dt_state!=NOT_LOADED);
}

void CLPRtreeView::OnEditRQF()
{
  CRQFinfo_dlg(GetDocument(),&SelectedFeature(),SelectedFeatureType()).DoModal();
}

void CLPRtreeView::OnUpdateEditRQF(CCmdUI *pCmdUI)
{
  CPromDoc *doc=GetDocument();
  CTreeCtrl& treeCtrl=GetTreeCtrl();
  HTREEITEM selItem=treeCtrl.GetSelectedItem();
  pCmdUI->Enable(selItem!=NULL && doc->em_state!=NOT_LOADED &&
		 doc->elev.contains(SelectedFeature().location));
}
