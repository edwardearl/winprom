// FeatureListView.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "ListFrame.h"
#include "PromSadlListView.h"
#include "CycleListView.h"
#include "PeakNbrListView.h"
#include "RunoffNbrListView.h"
#include "ParentListView.h"
#include "WalkListView.h"
#include "LPRtreeView.h"
#include "FeatureEdit_dlg.h"
#include "FeatureFmt_dlg.h"
#include "FeatureTypeFilter_dlg.h"
#include "FeatureListSummary_dlg.h"
#include "RQFinfo_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const char *get_delimiter(Format_style style, Col_align align)
{
  switch (style) {
  case FMS_FLAG: return " ";
  case FMS_HTML: 
    switch (align) {
    case CA_LEFT: return "<TD>";
    case CA_CENTER: return "<TD align=center>";
    case CA_RIGHT: return "<TD align=right>";
    default: assert(0);
    }
  case FMS_CSV: return ",";
  default: assert(0);
  }
  return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CFeatureListView

IMPLEMENT_DYNCREATE(CFeatureListView, CListView)

CFeatureListView::CFeatureListView()
{
}

CFeatureListView::~CFeatureListView()
{
}

BEGIN_MESSAGE_MAP(CFeatureListView, CListView)
  //{{AFX_MSG_MAP(CFeatureListView)
  ON_WM_CREATE()
  ON_COMMAND(ID_VIEW_REPORT, OnViewReport)
  ON_UPDATE_COMMAND_UI(ID_VIEW_REPORT, OnUpdateViewReport)
  ON_COMMAND(ID_VIEW_SMALL, OnViewSmall)
  ON_UPDATE_COMMAND_UI(ID_VIEW_SMALL, OnUpdateViewSmall)
  ON_COMMAND(ID_VIEW_LARGE, OnViewLarge)
  ON_UPDATE_COMMAND_UI(ID_VIEW_LARGE, OnUpdateViewLarge)
  ON_COMMAND(ID_VIEW_ALIGN, OnViewAlign)
  ON_UPDATE_COMMAND_UI(ID_VIEW_ALIGN, OnUpdateViewAlign)
  ON_COMMAND(ID_VIEW_ARRANGE, OnViewArrange)
  ON_UPDATE_COMMAND_UI(ID_VIEW_ARRANGE, OnUpdateViewArrange)
  ON_COMMAND(ID_FILE_TEXT, OnFileText)
  ON_COMMAND(ID_FILE_HTML, OnFileHtml)
  ON_COMMAND(ID_LIST_RELIST, OnListRelist)
  ON_UPDATE_COMMAND_UI(ID_LIST_RELIST, OnUpdateListRelist)
  ON_COMMAND(ID_LIST_FORMAT, OnListFormat)
  ON_COMMAND(ID_LIST_SUMMARY, OnListSummary)
  ON_COMMAND(ID_EDIT_UNLIST, OnEditUnlist)
  ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditAnySelect)
  ON_COMMAND(ID_EDIT_FEATURE, OnEditFeature)
  ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
  ON_COMMAND(ID_EDIT_SELECT_INVERT, OnEditSelectInvert)
  ON_COMMAND(ID_EDIT_SELECT_FILTER_CLEAR, OnEditSelectFilterClear)
  ON_COMMAND(ID_EDIT_SELECT_FILTER_PRECISE, OnEditSelectFilterPrecise)
  ON_COMMAND(ID_EDIT_SELECT_FILTER_SET, OnEditSelectFilterSet)
  ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
  ON_COMMAND(ID_LIST_SADL, OnListSadl)
  ON_UPDATE_COMMAND_UI(ID_LIST_SADL, OnUpdateListSadl)
  ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
  ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
  ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
  ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
  ON_COMMAND(ID_LIST_NBRS, OnListNbrs)
  ON_UPDATE_COMMAND_UI(ID_LIST_NBRS, OnUpdateListNbrs)
  ON_COMMAND(ID_EDIT_ROTATE, OnEditRotate)
  ON_UPDATE_COMMAND_UI(ID_EDIT_ROTATE, OnUpdateEditRotate)
  ON_COMMAND(ID_EDIT_CENTER, OnEditCenter)
  ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
  ON_UPDATE_COMMAND_UI(ID_EDIT_SPLICE, OnUpdateEditSplice)
  ON_COMMAND(ID_EDIT_SPLICE, OnEditSplice)
  ON_UPDATE_COMMAND_UI(ID_FILE_HTML, OnUpdateHaveItems)
  ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
  ON_COMMAND(ID_VIEW_LISTX, OnViewList)
  ON_UPDATE_COMMAND_UI(ID_VIEW_LISTX, OnUpdateViewList)
  ON_COMMAND(ID_LIST_TREE, OnListTree)
  ON_UPDATE_COMMAND_UI(ID_LIST_TREE, OnUpdateListTree)
  ON_COMMAND(ID_EDIT_SEARCH, OnEditSearch)
  ON_UPDATE_COMMAND_UI(ID_EDIT_SEARCH, OnUpdateEditSearch)
  ON_COMMAND(ID_FEATURE_CENTER, OnFeatureCenter)
  ON_COMMAND(ID_FEATURE_DELETE, OnFeatureDelete)
  ON_COMMAND(ID_FEATURE_EDIT, OnFeatureEdit)
  ON_COMMAND(ID_FEATURE_LIST_NBRS, OnFeatureListNbrs)
  ON_COMMAND(ID_FEATURE_LIST_SADDLES, OnFeatureListSaddles)
  ON_COMMAND(ID_FEATURE_LIST_TREE, OnFeatureListTree)
  ON_COMMAND(ID_FEATURE_UNLIST, OnFeatureUnlist)
  ON_COMMAND(ID_BASINSADL_ROTATE, OnBasinsadlRotate)
  ON_COMMAND(ID_RUNOFF_SPLICE, OnRunoffSplice)
  ON_WM_RBUTTONDOWN()
  ON_UPDATE_COMMAND_UI(ID_LIST_WALK, OnUpdateListWalk)
  ON_COMMAND(ID_LIST_WALK, OnListWalk)
  ON_UPDATE_COMMAND_UI(ID_EDIT_CENTER, OnUpdateEditSingleSelect)
  ON_UPDATE_COMMAND_UI(ID_EDIT_FEATURE, OnUpdateEditFeature)
  ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
  ON_WM_KEYUP()
  ON_WM_LBUTTONUP()
  ON_WM_MOUSEMOVE()
  ON_COMMAND(ID_EDIT_SWI, OnEditSWI)
  ON_UPDATE_COMMAND_UI(ID_EDIT_SWI, OnUpdateEditSWI)
  ON_COMMAND(ID_FILE_CSV, OnFileCSV)
  ON_COMMAND(ID_EDIT_BROWSE, OnEditBrowse)
  ON_COMMAND(ID_FEATURE_BROWSE, OnFeatureBrowse)
  ON_UPDATE_COMMAND_UI(ID_SAVE_DLPR_TREE, OnUpdateHaveDLPRtree)
  ON_COMMAND(ID_FEATURE_RQF, OnRQFinfo)
  ON_COMMAND(ID_EDIT_RQF, OnEditRQF)
  ON_UPDATE_COMMAND_UI(ID_EDIT_RQF, OnUpdateEditRQF)
  ON_COMMAND(ID_LIST_SAVE, OnListSave)
  ON_COMMAND(ID_LIST_LOAD, OnListLoad)
  ON_COMMAND(ID_LIST_APPEND, OnListAppend)
  ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnHeaderClick)
  ON_COMMAND(ID_LIST_PARENTS, OnListParents)
  ON_UPDATE_COMMAND_UI(ID_LIST_PARENTS, OnUpdateListParents)
  ON_COMMAND(ID_EDIT_MOVEUP, OnEditMoveUp)
  ON_UPDATE_COMMAND_UI(ID_EDIT_MOVEUP, OnUpdateEditMoveUp)
  ON_COMMAND(ID_EDIT_MOVEDOWN, OnEditMoveDown)
  ON_UPDATE_COMMAND_UI(ID_EDIT_MOVEDOWN, OnUpdateEditMoveDown)
  ON_UPDATE_COMMAND_UI(ID_EDIT_UNLIST, OnUpdateEditAnySelect)
  ON_UPDATE_COMMAND_UI(ID_FILE_TEXT, OnUpdateHaveItems)
  ON_UPDATE_COMMAND_UI(ID_LIST_FORMAT, OnUpdateHaveItems)
  ON_UPDATE_COMMAND_UI(ID_LIST_SUMMARY, OnUpdateHaveItems)
  ON_UPDATE_COMMAND_UI(ID_FILE_CSV, OnUpdateHaveItems)
  ON_UPDATE_COMMAND_UI(ID_EDIT_BROWSE, OnUpdateEditSingleSelect)
  ON_UPDATE_COMMAND_UI(ID_SAVEAS_DLPR_TREE, OnUpdateHaveDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_INFO_DLPR_TREE, OnUpdateHaveDLPRtree)
  ON_COMMAND(ID_FEATURE_LIST_PARENTS, OnFeatureListParents)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


char CFeatureListView::buf[256];


/////////////////////////////////////////////////////////////////////////////
// CFeatureListView diagnostics

#ifdef _DEBUG
void CFeatureListView::AssertValid() const
{
  CListView::AssertValid();
}

void CFeatureListView::Dump(CDumpContext& dc) const
{
  CListView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CFeatureListView message handlers

static const char *init_title=0;

CFeatureListView *CFeatureListView::init_list(CPromDoc *document, const char *t)
{
  CListFrame::client_width=0;
  CSingleDocTemplate& ListViewTpl=CPromApp::getApp()->ListViewTpl;
  CListFrame *newlist=(CListFrame *)ListViewTpl.CreateNewFrame(document,NULL);
  init_title=t;
  ListViewTpl.InitialUpdateFrame(newlist,document);

  return (CFeatureListView *)newlist->GetActiveView();
}

void CFeatureListView::setStyle(DWORD newStyle)
{
  HWND hListCtrl=GetListCtrl().GetSafeHwnd();
  DWORD oldStyle=GetWindowLong(hListCtrl,GWL_STYLE);
  SetWindowLong(hListCtrl,GWL_STYLE,oldStyle|newStyle);
}

void CFeatureListView::clearStyle(DWORD newStyle)
{
  HWND hListCtrl=GetListCtrl().GetSafeHwnd();
  DWORD oldStyle=GetWindowLong(hListCtrl,GWL_STYLE);
  SetWindowLong(hListCtrl,GWL_STYLE,oldStyle&~newStyle);
}

void CFeatureListView::toggleStyle(DWORD newStyle)
{
  HWND hListCtrl=GetListCtrl().GetSafeHwnd();
  DWORD oldStyle=GetWindowLong(hListCtrl,GWL_STYLE);
  SetWindowLong(hListCtrl,GWL_STYLE,oldStyle^newStyle);
}

void CFeatureListView::setViewStyle(DWORD newStyle)
{
  HWND hListCtrl=GetListCtrl().GetSafeHwnd();
  DWORD oldStyle=GetWindowLong(hListCtrl,GWL_STYLE);
  if ((oldStyle&LVS_TYPEMASK)!=newStyle) {
    oldStyle&=~LVS_TYPEMASK;
    SetWindowLong(hListCtrl,GWL_STYLE,newStyle|oldStyle);
  }
}

BOOL CFeatureListView::PreCreateWindow(CREATESTRUCT& cs)
{
  m_dwDefaultStyle|=LVS_SHAREIMAGELISTS|LVS_SHOWSELALWAYS;
  if (!CanSort()) m_dwDefaultStyle|=LVS_NOSORTHEADER;
  m_dwDefaultStyle&=~LVS_AUTOARRANGE;
  return CListView::PreCreateWindow(cs);
}

int CFeatureListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CListView::OnCreate(lpCreateStruct) == -1)
    return -1;

  CListCtrl& listCtrl=GetListCtrl();
  setViewStyle(LVS_REPORT);

  listCtrl.InsertColumn(name_col=0,"Name",LVCFMT_LEFT,192);
  listCtrl.InsertColumn(elev_col=1,"Elevation",LVCFMT_CENTER,88);
  listCtrl.InsertColumn(loc_col=2,"Location",LVCFMT_LEFT,160);

  CPromApp *app=CPromApp::getApp();
  listCtrl.SetImageList(&app->largeImageList,LVSIL_NORMAL);
  listCtrl.SetImageList(&app->smallImageList,LVSIL_SMALL);
  listCtrl.SetImageList(&app->flagImageList,LVSIL_STATE);

  return 0;
}

int CFeatureListView::InsertColumn(int icol, const char *text,
				   int align, int width)
{
  if (icol<=name_col) ++name_col;
  if (icol<=elev_col) ++elev_col;
  if (icol<=loc_col) ++loc_col;
  return GetListCtrl().InsertColumn(icol,text,align,width);
}

int CFeatureListView::DeleteColumn(int icol)
{
  assert(icol!=name_col && icol!=elev_col && icol!=loc_col);
  if (icol<name_col) --name_col;
  if (icol<elev_col) --elev_col;
  if (icol<loc_col) --loc_col;
  return GetListCtrl().DeleteColumn(icol);
}

void CFeatureListView::OnInitialUpdate() 
{
  title=init_title;
  list_fmt.set(GetDocument()->data);
  CToolBar& fb=((CListFrame *)GetParent())->fileBar;
  fb.SetWindowText(GetDocument()->is_drainage?"Drainage Tree":"Divide Tree");
  setItemIndicators();
}

int CFeatureListView::AddBlankItem(Feature& feature, FT_index type)
{
  if (!feature.location) return -1;
  CListCtrl& listCtrl=GetListCtrl();
  int nItem=listCtrl.GetItemCount();
  GetDocument()->type2image(type);
  listCtrl.InsertItem(LVIF_STATE|LVIF_PARAM|LVIF_IMAGE,nItem,0,0,LVIS_FOCUSED|LVIS_SELECTED,
    type,(DWORD)(&feature));
  return nItem;
}

void CFeatureListView::AddItem(Feature& feature, FT_index type)
{
  int nItem=AddBlankItem(feature,type);
  if (nItem>=0) UpdateItem(nItem);
}

void CFeatureListView::UpdateItem(int nItem)
{
  CListCtrl& listCtrl=GetListCtrl();
  Feature& feature=GetFeature(nItem);

  if (feature.name) listCtrl.SetItemText(nItem,name_col,feature.name);
  else if (list_fmt.name_f==Feature_fmt::ALT_ELEV) {
    feature.elev.sprint(buf);
    // trim leading & trailing blanks
    char *lbuf=buf;
    while (isspace(*lbuf)) ++lbuf;
    if (lbuf[0]!='\0') {
      char *rbuf=lbuf+strlen(lbuf);
      while (isspace(rbuf[-1])) --rbuf;
      *rbuf='\0';
    }
    listCtrl.SetItemText(nItem,name_col,lbuf);
  }
  else if (list_fmt.name_f==Feature_fmt::ALT_LOC) {
    feature.location.sprint(buf);
    listCtrl.SetItemText(nItem,name_col,buf);
  }
  else listCtrl.SetItemText(nItem,name_col,"");

  feature.elev.sprint(buf);
  listCtrl.SetItemText(nItem,elev_col,buf);

  listCtrl.SetItemState(nItem,
    INDEXTOSTATEIMAGEMASK(feature.edge_effect*2+!feature.edited),
    LVIS_STATEIMAGEMASK);

  feature.location.sprint(buf);
  listCtrl.SetItemText(nItem,loc_col,buf);
}

void CFeatureListView::UpdateAllItems()
{
  list_fmt.set(GetDocument()->data);
  int i,nItems=GetListCtrl().GetItemCount();
  for (i=0; i<nItems; ++i)
    UpdateItem(i);
}

bool CFeatureListView::prelist()
{
  return true;
}

FT_index CFeatureListView::GetFeatureImage(int item) const
{
  static LV_ITEM item_info;
  item_info.mask=LVIF_IMAGE;
  item_info.iItem=item;
  item_info.iSubItem=0;
  GetListCtrl().GetItem(&item_info);
  return (FT_index)item_info.iImage;
}

FT_index CFeatureListView::GetFeatureType(int item) const
{
  FT_index type=GetFeatureImage(item);
  GetDocument()->image2type(type);
  return type;
}

void CFeatureListView::setItemIndicators()
{
  CStatusBar& listStatus=((CListFrame *)GetParent())->listStatus;
  CListCtrl& listCtrl=GetListCtrl();
  sprintf(buf,"%d items",listCtrl.GetItemCount());
  listStatus.SetPaneText(1,buf);
}

void CFeatureListView::OnUpdate(CView *, LPARAM lHint, CObject *)
{
  Feature *changed_feature=(Feature *)lHint;
  if (changed_feature==&CPromDoc::dummyModifyData ||
      changed_feature==&CPromDoc::dummyModifyBounds ||
      changed_feature==&CPromDoc::dummyModifyTopo ||
      changed_feature==&CPromDoc::dummyModifyMap ||
      changed_feature==&CPromDoc::dummySaveCenter) return;

  CListCtrl& listCtrl=GetListCtrl();
  if (lHint==0) {
    listCtrl.DeleteAllItems();
    return;
  }

  CPromDoc *doc=GetDocument();
  Divide_tree& tree=doc->tree;
  Feature *list_feature;
  if (changed_feature==&CPromDoc::dummyModifyTrans && doc->trans_m<0) {
    SetViewTitle();
    CToolBar& fb=((CListFrame *)GetParent())->fileBar;
    fb.SetWindowText(doc->is_drainage?"Drainage Tree":"Divide Tree");
    CString wart;
    fb.GetWindowText(wart);
    TRACE("file bar label: %s\n",(const char *)wart);
  }
  int item;
  FT_index new_img;
  list_fmt.set(GetDocument()->data);
  for (item=0; item<listCtrl.GetItemCount(); ++item) {
    list_feature=&GetFeature(item);
    if (changed_feature==&CPromDoc::dummyModifyAll ||
	changed_feature==&CPromDoc::dummyModifyTrans ||
	changed_feature==list_feature) {
      if (list_feature->location) {
	if (changed_feature==&CPromDoc::dummyModifyTrans && doc->trans_m<0) {
	  new_img=GetFeatureImage(item);
	  CPromDoc::imageFlip(new_img);
	  listCtrl.SetItem(item,0,LVIF_IMAGE,0,new_img,0,0,0);
	}
	UpdateItem(item);
      }
      else {
	listCtrl.DeleteItem(item);
	--item;
      }
    }
    else if (changed_feature==&CPromDoc::dummyModifyRObase) {
      if (GetFeatureType(item)==FT_RUNOFF) {
	Domain::Index iro=(Runoff *)list_feature-doc->old_ro;
	listCtrl.SetItemData(item,
	  (DWORD)&tree.runoff(iro+(iro>=doc->new_iro)));
      }
    }
    else if (changed_feature==&CPromDoc::dummyModifyBSbase) {
      if (GetFeatureType(item)==FT_BASINSADL)
	listCtrl.SetItemData(item,
	  (DWORD)&tree.bsnsdl((Basin_saddle *)list_feature-doc->old_bs));
    }
    else if (changed_feature==&CPromDoc::dummyModifySplice) {
      switch (GetFeatureType(item)) {
      case FT_BASINSADL:
	// translate basin saddle pointers
	// since basin saddle array may have grown due to splice
	listCtrl.SetItemData(item,
	  (DWORD)&tree.bsnsdl((Basin_saddle *)list_feature-doc->old_bs));
	break;
      case FT_RUNOFF:
	// update runoffs affected by splice
	if (list_feature->location)
	  UpdateItem(item);
	else {
	  listCtrl.DeleteItem(item);
	  --item;
	}
	break;
      }
    }
  }
  setItemIndicators();
}

void CFeatureListView::OnFileText() 
{
  CListCtrl& listCtrl=GetListCtrl();
  CString name=CPromDoc::GetFileName(CPromDoc::FO_SAVE,0,"Text","txt",GetParent());
  if (name.IsEmpty()) return;
  FILE *out_file=fopen(name,"w");
  if (out_file==0) {
    CPromDoc::FileError("opening output file",name);
    return;
  }
  list_fmt.set(GetDocument()->data,FMS_FLAG);
  printHeader(out_file,FMS_FLAG);
  putc('\n',out_file);
  int item,itemCount=listCtrl.GetItemCount();
  for (item=0; item<itemCount; ++item) {
    printFeature(item,out_file,FMS_FLAG);
    putc('\n',out_file);
  }
  fclose(out_file);
}

static const char
  *no_center="<COL span=1>",
  *one_center="<COL span=1 align=center>\n",
  *two_center="<COL span=2 align=center>\n",
  *first_center="<COL span=1 align=center>\n<COL span=1>\n",
  *first_two_center="<COL span=2 align=center>\n<COL span=1>\n";
static const char *html_col_align[][2][2]={ // [name_fmt][show_loc][show_elev]
  "",one_center,one_center,two_center,
  no_center,no_center,first_center,two_center,
  no_center,first_center,no_center,first_center,
  no_center,first_center,first_center,first_two_center
};

void CFeatureListView::OnFileHtml() 
{
  CListCtrl& listCtrl=GetListCtrl();
  CString name=CPromDoc::GetFileName(CPromDoc::FO_SAVE,0,"HTML","htm",GetParent());
  if (name.IsEmpty()) return;
  FILE *out_file=fopen(name,"w");
  if (out_file==0) {
    CPromDoc::FileError("opening output HTML file",name);
    return;
  }
  fprintf(out_file,"<HTML>\n<TABLE BORDER>\n%s",
    html_col_align[list_fmt.name_f][list_fmt.show_loc][list_fmt.show_elev]);
  fprintf(out_file,"<TR>");
  list_fmt.set(GetDocument()->data,FMS_HTML);
  printHeader(out_file,FMS_HTML);
  fprintf(out_file,"</TR>\n");
  int item,itemCount=listCtrl.GetItemCount();
  for (item=0; item<itemCount; ++item) {
    fprintf(out_file,"<TR>");
    printFeature(item,out_file,FMS_HTML);
    fprintf(out_file,"</TR>\n");
  }
  fprintf(out_file,"</TABLE>\n</HTML>\n");
  fclose(out_file);
}

void CFeatureListView::OnFileCSV()
{
  CListCtrl& listCtrl=GetListCtrl();
  CString name=CPromDoc::GetFileName(CPromDoc::FO_SAVE,0,"CSV","csv",GetParent());
  if (name.IsEmpty()) return;
  FILE *out_file=fopen(name,"w");
  if (out_file==0) {
    CPromDoc::FileError("opening output CSV file",name);
    return;
  }
  list_fmt.set(GetDocument()->data,FMS_CSV);
  printHeader(out_file,FMS_CSV);
  putc('\n',out_file);
  int item,itemCount=listCtrl.GetItemCount();
  for (item=0; item<itemCount; ++item) {
    printFeature(item,out_file,FMS_CSV);
    putc('\n',out_file);
  }
  fclose(out_file);
}

void CFeatureListView::printFeature(int item, FILE *out_file,
				    Format_style style)
{
  bool drain=GetDocument()->is_drainage;
  char type;
  switch (GetFeatureType(item)) {
  case FT_PEAK: type=drain?'B':'P'; break;
  case FT_SADDLE: type='S'; break;
  case FT_RUNOFF: type='R'; break;
  case FT_BASINSADL: type=drain?'P':'B'; break;
  default: assert(0);
  }
  switch (style) {
  case FMS_FLAG:
    fprintf(out_file,"%c ",type);
    break;
  case FMS_HTML:
    fprintf(out_file,"<TD>%c",type);
    break;
  case FMS_CSV:
    fprintf(out_file,"%c,",type);
    break;
  default:
    assert(0);
  }
  GetFeature(item).print(out_file);
}

void CFeatureListView::printHeader(FILE *out_file, Format_style style)
{
  switch (style) {
  case FMS_FLAG:
    fprintf(out_file,"T ");
    break;
  case FMS_HTML:
    fprintf(out_file,"<TD>Type");
    break;
  case FMS_CSV:
    fprintf(out_file,"Type,");
    break;
  default:
    assert(0);
  }
  Feature::print_header(out_file);
}

void CFeatureListView::OnViewLarge()
{
  setViewStyle(LVS_ICON);
}

void CFeatureListView::OnUpdateViewLarge(CCmdUI *pCmdUI)
{
  pCmdUI->SetRadio((GWL()&LVS_TYPEMASK)==LVS_ICON);
}

void CFeatureListView::OnViewList()
{
  setViewStyle(LVS_LIST);
}

void CFeatureListView::OnUpdateViewList(CCmdUI *pCmdUI)
{
  pCmdUI->SetRadio((GWL()&LVS_TYPEMASK)==LVS_LIST);
}

void CFeatureListView::OnViewReport()
{
  setViewStyle(LVS_REPORT);
}

void CFeatureListView::OnUpdateViewReport(CCmdUI *pCmdUI)
{
  pCmdUI->SetRadio((GWL()&LVS_TYPEMASK)==LVS_REPORT);
}

void CFeatureListView::OnViewSmall()
{
  setViewStyle(LVS_SMALLICON);
}

void CFeatureListView::OnUpdateViewSmall(CCmdUI *pCmdUI)
{
  pCmdUI->SetRadio((GWL()&LVS_TYPEMASK)==LVS_SMALLICON);
}

void CFeatureListView::OnViewAlign()
{
  GetListCtrl().Arrange(LVA_DEFAULT);
}

void CFeatureListView::OnUpdateViewAlign(CCmdUI *pCmdUI)
{
  DWORD currentStyle=GWL()&LVS_TYPEMASK;
  pCmdUI->Enable(currentStyle==LVS_ICON || currentStyle==LVS_SMALLICON);
}

void CFeatureListView::OnViewArrange()
{
  toggleStyle(LVS_AUTOARRANGE);
}

void CFeatureListView::OnEditUnlist()
{
  CListCtrl& listCtrl=GetListCtrl();
  int selItem;
  SetRedraw(FALSE);
  while (true) {
    selItem=listCtrl.GetNextItem(-1,LVNI_SELECTED);
    if (selItem<0) break;
    listCtrl.DeleteItem(selItem);
  }
  SetRedraw(TRUE);
  setItemIndicators();
}

void CFeatureListView::OnEditSearch()
{
  bool found=false,not_found=false,item_found;
  CPromDoc *doc=GetDocument();
  CListCtrl& listCtrl=GetListCtrl();
  Feature *target;

  int j,n=listCtrl.GetItemCount();
  unsigned i,nc=doc->copyPaste.ftr_count();
  for (i=0; i<nc; ++i) {
    target=doc->copyPaste.list[i];
    item_found=false;
    for (j=0; j<n; ++j) {
      if (target==&GetFeature(j)) {
	item_found=true;
	listCtrl.SetItemState(j,LVIS_SELECTED,LVIS_SELECTED);
      }
    }
    if (item_found) found=true;
    else not_found=true;
  }

  if (!found) {
    MessageBox("No feature on the clipboard is in this list.",
	       "Winprom Search");
  }
  else if (not_found) {
    MessageBox("Some feature(s) on the clipboard are not in this list.",
	       "Winprom Search");
  }
}

void CFeatureListView::OnUpdateEditSearch(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetDocument()->copyPaste.ftr_count()>0);
}

void CFeatureListView::OnUpdateHaveItems(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetListCtrl().GetItemCount()>0);
}

void CFeatureListView::OnUpdateEditAnySelect(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetListCtrl().GetSelectedCount()>0);
}

void CFeatureListView::OnUpdateEditSingleSelect(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetListCtrl().GetSelectedCount()==1);
}

void CFeatureListView::OnEditFeature()
{
  int item=GetListCtrl().GetNextItem(-1,LVNI_SELECTED);
  if (item<0) return;
  Feature& feature=GetFeature(item);
  FT_index ft=GetFeatureType(item);

  GetDocument()->EditFeature(feature,ft,GetParent());
}

void CFeatureListView::OnUpdateEditFeature(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(tree_writeable() && GetListCtrl().GetSelectedCount()==1);
}

void CFeatureListView::OnDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
  if (!tree_writeable()) {
    MessageBox("You cannot edit a feature here because\n"
	       "some feature is already open for editing.",
	       NULL,MB_OK|MB_ICONERROR);
    return;
  }
  OnEditFeature();
  *pResult=0;
}

void CFeatureListView::OnEditSelectAll()
{
  CListCtrl& listCtrl=GetListCtrl();
  int i,n=listCtrl.GetItemCount();
  for (i=0; i<n; ++i)
    listCtrl.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
}

void CFeatureListView::OnEditSelectInvert()
{
  CListCtrl& listCtrl=GetListCtrl();
  int i,n=listCtrl.GetItemCount();
  for (i=0; i<n; ++i)
    listCtrl.SetItemState(i,listCtrl.GetItemState(i,LVIS_SELECTED)^LVIS_SELECTED,
      LVIS_SELECTED);
}

bool CFeatureListView::get_select_filter()
{
  FeatureTypeFilter temp_ff=select_filter;
  temp_ff.rec_filter.align(GetDocument()->data);
  CFeatureTypeFilter_dlg dlg(temp_ff,this);
  if (dlg.DoModal()!=IDOK) return false;
  select_filter=temp_ff;
  return true;
}

void CFeatureListView::OnEditSelectFilterClear()
{
  const Database& data=GetDocument()->data;
  if (!get_select_filter()) return;
  CListCtrl& listCtrl=GetListCtrl();
  int selItem=-1;
  while (true) {
    selItem=listCtrl.GetNextItem(selItem,LVNI_SELECTED);
    if (selItem<0) break;
    if (select_filter.test(GetFeature(selItem),GetFeatureType(selItem),data))
      listCtrl.SetItemState(selItem,0,LVIS_SELECTED);
  }
}

void CFeatureListView::OnEditSelectFilterPrecise()
{
  const Database& data=GetDocument()->data;
  if (!get_select_filter()) return;
  CListCtrl& listCtrl=GetListCtrl();
  int i,n=listCtrl.GetItemCount();
  for (i=0; i<n; ++i) {
    listCtrl.SetItemState(i,select_filter.test(GetFeature(i),GetFeatureType(i),
					       data) ? LVIS_SELECTED : 0,
			  LVIS_SELECTED);
  }
}

void CFeatureListView::OnEditSelectFilterSet()
{
  const Database& data=GetDocument()->data;
  if (!get_select_filter()) return;
  CListCtrl& listCtrl=GetListCtrl();
  int i,n=listCtrl.GetItemCount();
  for (i=0; i<n; ++i) {
    if (select_filter.test(GetFeature(i),GetFeatureType(i),data))
      listCtrl.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
  }
}

void CFeatureListView::OnEditDelete() 
{
  CListCtrl& listCtrl=GetListCtrl();
  int item=listCtrl.GetNextItem(-1,LVNI_SELECTED);
  if (listCtrl.GetSelectedCount()==1)
    GetDocument()->
      DeleteFeature(GetFeature(item),GetFeatureType(item),GetParent());
  else {
    assert(listCtrl.GetSelectedCount()==2);
    int item2=listCtrl.GetNextItem(item,LVNI_SELECTED);
    FT_index ft=GetFeatureType(item),ft2=GetFeatureType(item2);
    if (ft==FT_PEAK && ft2==FT_SADDLE)
      GetDocument()->DeletePeak(GetFeature(item),&GetFeature(item2),GetParent(),true);
    else if (ft2==FT_PEAK && ft==FT_SADDLE)
      GetDocument()->DeletePeak(GetFeature(item2),&GetFeature(item),GetParent(),true);
    else assert(0);
  }
}

void CFeatureListView::OnUpdateEditDelete(CCmdUI *pCmdUI)
{
  if (!tree_writeable()) {
    pCmdUI->Enable(FALSE);
    return;
  }
  CListCtrl& listCtrl=GetListCtrl();
  int item=listCtrl.GetNextItem(-1,LVNI_SELECTED);
  if (listCtrl.GetSelectedCount()==2) {
    int item2=listCtrl.GetNextItem(item,LVNI_SELECTED);
    FT_index ft=GetFeatureType(item),ft2=GetFeatureType(item2);
    if ((ft==FT_PEAK && ft2==FT_SADDLE) ||
	(ft2==FT_PEAK && ft==FT_SADDLE)) {
      // one is a peak and one is a saddle.  Make sure they're neighbors.
      Feature *peak,*sadl;
      if (ft==FT_PEAK) {
	peak=&GetFeature(item);
	sadl=&GetFeature(item2);
      }
      else {
	peak=&GetFeature(item2);
	sadl=&GetFeature(item);
      }
      pCmdUI->Enable(CPromDoc::peak_sadl_nbr(peak,sadl));
    }
    else pCmdUI->Enable(FALSE);
  }
  else pCmdUI->Enable(listCtrl.GetSelectedCount()==1);
}

void CFeatureListView::OnEditSplice()
{
  CListCtrl& listCtrl=GetListCtrl();
  if (listCtrl.GetSelectedCount()==1) {
    GetDocument()->Splice((Runoff&)GetFeature(listCtrl.GetNextItem(-1,LVNI_SELECTED)),GetParent());
  }
  else {
    int nsel=listCtrl.GetSelectedCount();
    Runoff **ros=new Runoff*[nsel];
    Divide_tree& tree=GetDocument()->tree;
    Domain::Index i=0;
    int selItem=-1;
    while (true) {
      selItem=listCtrl.GetNextItem(selItem,LVNI_SELECTED);
      if (selItem<0) break;
      ros[i]=(Runoff *)&GetFeature(selItem);
      ++i;
    }
    assert((int)i==nsel);
    GetDocument()->Splice(ros,nsel,GetParent());
    delete ros;
  }
}

void CFeatureListView::OnUpdateEditSplice(CCmdUI *pCmdUI)
{
  if (!tree_writeable()) {
    pCmdUI->Enable(FALSE);
    return;
  }
  CListCtrl& listCtrl=GetListCtrl();
  unsigned total_iq=0;
  int selItem=-1;
  while (true) {
    selItem=listCtrl.GetNextItem(selItem,LVNI_SELECTED);
    if (selItem<0) break;
    if (GetFeatureType(selItem)==FT_RUNOFF) {
      total_iq+=((Runoff&)GetFeature(selItem)).interior_quads;
      if (total_iq>4) {
	pCmdUI->Enable(FALSE);
	return;
      }
    }
    else {
      pCmdUI->Enable(FALSE);
      return;
    }
  }
  pCmdUI->Enable(total_iq>0);
}

void CFeatureListView::OnEditRotate()
{
  CListCtrl& listCtrl=GetListCtrl();
  int item=listCtrl.GetNextItem(-1,LVNI_SELECTED);
  if (listCtrl.GetSelectedCount()==1)
    GetDocument()->Rotate((Basin_saddle&)GetFeature(item),0,GetParent());
  else {
    assert(listCtrl.GetSelectedCount()==2);
    int item2=listCtrl.GetNextItem(item,LVNI_SELECTED);
    FT_index ft=GetFeatureType(item),ft2=GetFeatureType(item2);
    if (ft==FT_BASINSADL && ft2==FT_SADDLE)
      GetDocument()->Rotate((Basin_saddle&)GetFeature(item),&GetFeature(item2),
			    GetParent());
    else if (ft2==FT_BASINSADL && ft==FT_SADDLE)
      GetDocument()->Rotate((Basin_saddle&)GetFeature(item2),&GetFeature(item),
			    GetParent());
    else assert(0);
  }
}

void CFeatureListView::OnUpdateEditRotate(CCmdUI *pCmdUI)
{
  if (!tree_writeable()) {
    pCmdUI->Enable(FALSE);
    return;
  }
  CListCtrl& listCtrl=GetListCtrl();
  int item=listCtrl.GetNextItem(-1,LVNI_SELECTED);
  if (listCtrl.GetSelectedCount()==1)
    pCmdUI->Enable(GetFeatureType(item)==FT_BASINSADL);
  else if (listCtrl.GetSelectedCount()==2) {
    int item2=listCtrl.GetNextItem(item,LVNI_SELECTED);
    FT_index ft=GetFeatureType(item),ft2=GetFeatureType(item2);
    if ((ft==FT_BASINSADL && ft2==FT_SADDLE) ||
	(ft2==FT_BASINSADL && ft==FT_SADDLE)) {
      Feature *bs,*sadl;
      if (ft==FT_BASINSADL) {
	bs=&GetFeature(item);
	sadl=&GetFeature(item2);
      }
      else {
	bs=&GetFeature(item2);
	sadl=&GetFeature(item);
      }
      pCmdUI->Enable(((Basin_saddle *)bs)->on_cycle(Domain::FromSadl(sadl)));
    }
    else pCmdUI->Enable(FALSE);
  }
  else pCmdUI->Enable(FALSE);
}

void CFeatureListView::copyClip(Clip& clp) const
{
  CListCtrl& listCtrl=GetListCtrl();
  clp.clear();
  int selItem=-1;
  while (true) {
    selItem=listCtrl.GetNextItem(selItem,LVNI_SELECTED);
    if (selItem<0) break;
    clp.add_featr(GetFeature(selItem),GetFeatureType(selItem));
  }
}

void CFeatureListView::OnEditCopy()
{
  copyClip(GetDocument()->copyPaste);
}

void CFeatureListView::pasteClip(const Clip& clp)
{
  list_fmt.set(GetDocument()->data);
  unsigned i,n=clp.ftr_count();
  for (i=0; i<n; ++i)
    AddItem(*clp.list[i],clp.type[i]);
  setItemIndicators();
}

void CFeatureListView::OnEditPaste()
{
  pasteClip(GetDocument()->copyPaste);
}

void CFeatureListView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
  CPromDoc *document=GetDocument();
  pCmdUI->Enable(document->copyPaste.ftr_count()>0 && CanPaste(document->copyPaste));
}

void CFeatureListView::OnEditCenter()
{
  int i=GetListCtrl().GetNextItem(-1,LVNI_SELECTED);
  Feature& featr=GetFeature(i);
  CPromApp::getApp()->viewFeature(GetDocument(),featr,GetFeatureType(i));
}

void CFeatureListView::OnEditBrowse()
{
  int item=GetListCtrl().GetNextItem(-1,LVNI_SELECTED);
  CPromApp::getApp()->Navigate(GetDocument(),GetFeature(item),GetFeatureType(item));
}

void CFeatureListView::OnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
  NM_LISTVIEW *pNMListView = (NM_LISTVIEW *)pNMHDR;
  CPromDoc *document=GetDocument();
  copyClip(document->dragDrop);
  CPromApp::getApp()->smallImageList.
    BeginDrag(document->dragDrop.iconType(document->is_drainage),
	      CPoint(SMALL_ICON_SIZE/2,SMALL_ICON_Y_OFFSET));
  CImageList::DragEnter(NULL,pNMListView->ptAction);
  SetCapture();

  *pResult = 0;
}

void CFeatureListView::OnMouseMove(UINT nFlags, CPoint point)
{
  if (nFlags&MK_LBUTTON) {
    // dragging
    CPoint ptRoot(point);
    MapWindowPoints(NULL,&ptRoot,1);
    CImageList::DragMove(ptRoot);
    SetCursor(AfxGetApp()->LoadStandardCursor(GetDocument()->
      canTakeDrop(this,point) ? IDC_ARROW : IDC_NO));
  }

  CListView::OnMouseMove(nFlags,point);
}

void CFeatureListView::OnLButtonUp(UINT nFlags, CPoint point)
{
  GetDocument()->takeDrop(this,point);

  CListView::OnLButtonUp(nFlags,point);
}

void CFeatureListView::ListNbrs(Feature& featr, FT_index ft) const
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

void CFeatureListView::OnListNbrs()
{
  int selItem=GetListCtrl().GetNextItem(-1,LVNI_SELECTED);
  ListNbrs(GetFeature(selItem),GetFeatureType(selItem));
}

void CFeatureListView::OnUpdateListNbrs(CCmdUI *pCmdUI)
{
  CListCtrl& listCtrl=GetListCtrl();
  FT_index selType=GetFeatureType(listCtrl.GetNextItem(-1,LVNI_SELECTED));
  pCmdUI->Enable(listCtrl.GetSelectedCount()==1 &&
		 (selType==FT_PEAK || selType==FT_RUNOFF));
}

void CFeatureListView::OnListWalk()
{
  CListCtrl& listCtrl=GetListCtrl();
  int selItem1=listCtrl.GetNextItem(-1,LVNI_SELECTED),selItem2;
  const Basin_saddle *bs;
  switch (listCtrl.GetSelectedCount()) {
  case 2:
    selItem2=listCtrl.GetNextItem(selItem1,LVNI_SELECTED);
    CWalkListView::init_list(GetDocument(),
      WalkInfo(&GetFeature(selItem1),GetFeatureType(selItem1),
	       &GetFeature(selItem2),GetFeatureType(selItem2)));
    break;
  case 1:
    bs=(Basin_saddle *)&GetFeature(selItem1);
    CWalkListView::init_list(GetDocument(),
      WalkInfo(&bs->peak1->peak,FT_PEAK,&bs->peak2->peak,FT_PEAK));
    break;
  }
}

void CFeatureListView::OnUpdateListWalk(CCmdUI *pCmdUI)
{
  CListCtrl& listCtrl=GetListCtrl();
  int selItem1=listCtrl.GetNextItem(-1,LVNI_SELECTED);
  FT_index selType1=GetFeatureType(selItem1),selType2;
  switch (listCtrl.GetSelectedCount()) {
  case 2:
    selType2=GetFeatureType(listCtrl.GetNextItem(selItem1,LVNI_SELECTED));
    pCmdUI->Enable(selType1!=FT_BASINSADL && selType2!=FT_BASINSADL);
    break;
  case 1:
    pCmdUI->Enable(selType1==FT_BASINSADL);
    break;
  default:
    pCmdUI->Enable(FALSE);
    break;
  }
}

void CFeatureListView::SetViewTitle()
{
  CPromDoc *document=GetDocument();
  GetParent()->SetWindowText(document->dt_name.IsEmpty()?
    title:document->dt_name+" - "+title);
  GetParent()->SetIcon(AfxGetApp()->
    LoadIcon(document->is_drainage?IDI_MIXED_DRAIN:IDI_MIXED),TRUE);
}

void CFeatureListView::OnListSadl() 
{
  CListCtrl& listCtrl=GetListCtrl();
  switch (GetFeatureType(listCtrl.GetNextItem(-1,LVNI_SELECTED))) {
  case FT_PEAK:
    {
      CPromSadlListView::List_info peaks;
      peaks.min_prom=default_min_prom();
      peaks.store(listCtrl.GetSelectedCount());
      int selItem=-1,i=0;
      while (true) {
	selItem=listCtrl.GetNextItem(selItem,LVNI_SELECTED);
	if (selItem<0) break;
	peaks.doms[i]=Domain::FromPeak(&GetFeature(selItem));
	++i;
      }
      CPromSadlListView::init_list(GetDocument(),peaks,GetParent());
    }
    break;
  case FT_BASINSADL:
    {
      CycleFilter bs;
      bs.store(listCtrl.GetSelectedCount());
      bs.min_prom=default_min_prom();
      bs.offmap=default_offmap();
      bs.ridge=default_ridge();
      bs.cell=default_cell();
      int selItem=-1,i=0;
      while (true) {
	selItem=listCtrl.GetNextItem(selItem,LVNI_SELECTED);
	if (selItem<0) break;
	bs.bs[i]=(Basin_saddle *)&GetFeature(selItem);
	++i;
      }
      CCycleListView::init_list(GetDocument(),bs,GetParent());
    }
    break;
  }
}

void CFeatureListView::OnUpdateListSadl(CCmdUI *pCmdUI)
{
  CListCtrl& listCtrl=GetListCtrl();
  if (listCtrl.GetSelectedCount()==0) {
    // nothing selected.  Disable control.
    pCmdUI->Enable(FALSE);
    return;
  }
  int selItem=-1;
  FT_index sel_FT=FT_NUM_TYPES; // type of selected features
  while (true) {
    selItem=listCtrl.GetNextItem(selItem,LVNI_SELECTED);
    if (selItem<0) break; // we're done checking the list of selected features.
    if (sel_FT!=FT_NUM_TYPES) {
      // there's a previous selected feature.
      // Make sure it's the same type as the current.
      if (GetFeatureType(selItem)!=sel_FT) {
	pCmdUI->Enable(FALSE);
	return;
      }
    }
    else {
      // no previous selected feature.  Record the type of this (the first) one.
      sel_FT=GetFeatureType(selItem);
      if (sel_FT!=FT_PEAK && sel_FT!=FT_BASINSADL) {
	// if it's not a peak or basin saddle, can't list saddles.
	pCmdUI->Enable(FALSE);
	return;
      }
    }
  }

  pCmdUI->Enable(TRUE);
  if (sel_FT==FT_PEAK) pCmdUI->SetText("Prominence &saddles...");
  else if (sel_FT==FT_BASINSADL) pCmdUI->SetText("&Cycle...");
}

void CFeatureListView::OnUpdateViewArrange(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck((GWL()&LVS_AUTOARRANGE)!=0);
}

void CFeatureListView::OnListRelist()
{
  if (!prelist()) return;
  GetListCtrl().DeleteAllItems();
  OnInitialUpdate();
}

void CFeatureListView::OnUpdateListRelist(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetDocument()->dt_state!=NOT_LOADED && CanRelist());
}

void CFeatureListView::OnListFormat()
{
  CFeatureFmt_dlg dlg(GetDocument()->data,GetParent(),list_fmt);
  if (dlg.DoModal()==IDOK)
    UpdateAllItems();
}

void CFeatureListView::OnListSummary()
{
  CFeatureListSummary_dlg(this).DoModal();
}

void CFeatureListView::OnListTree()
{
  CLPRtreeView::init_tree(GetDocument(),
    *Domain::FromPeak(&GetFeature(GetListCtrl().GetNextItem(-1,LVNI_SELECTED))),
    GetParent());
}

void CFeatureListView::OnUpdateListTree(CCmdUI *pCmdUI)
{
  CListCtrl& listCtrl=GetListCtrl();
  pCmdUI->Enable(listCtrl.GetSelectedCount()==1 &&
    GetFeatureType(listCtrl.GetNextItem(-1,LVNI_SELECTED))==FT_PEAK);
}

void CFeatureListView::OnFeatureCenter()
{
  CPromApp::getApp()->viewFeature(GetDocument(),*r_click_featr,r_click_type);
}

void CFeatureListView::OnRQFinfo()
{
  CRQFinfo_dlg(GetDocument(),r_click_featr,r_click_type,this).DoModal();
}

void CFeatureListView::OnFeatureBrowse()
{
  CPromApp::getApp()->Navigate(GetDocument(),*r_click_featr,r_click_type);
}

void CFeatureListView::OnFeatureDelete()
{
  GetDocument()->DeleteFeature(*r_click_featr,r_click_type,GetParent());
}

void CFeatureListView::OnFeatureEdit()
{
  GetDocument()->EditFeature(*r_click_featr,r_click_type,GetParent());
}

void CFeatureListView::OnFeatureListNbrs()
{
  ListNbrs(*r_click_featr,r_click_type);
}

void CFeatureListView::OnFeatureListSaddles()
{
  switch (r_click_type) {
  case FT_PEAK:
    CPromSadlListView::init_list(GetDocument(),*Domain::FromPeak(r_click_featr),
      GetParent(),default_min_prom());
    break;
  case FT_BASINSADL:
    CCycleListView::init_list(GetDocument(),*(Basin_saddle *)r_click_featr,
      GetParent(),default_min_prom(),default_offmap(),default_ridge(),default_cell());
    break;
  default:
    assert(0);
  }
}

void CFeatureListView::OnFeatureListParents()
{
  CParentListView::init_list(GetDocument(),*Domain::FromPeak(r_click_featr),
    GetParent(),default_min_prom());
}

void CFeatureListView::OnFeatureListTree()
{
  CLPRtreeView::init_tree(GetDocument(),
    *Domain::FromPeak(r_click_featr),GetParent());
}

void CFeatureListView::OnFeatureUnlist()
{
  GetListCtrl().DeleteItem(r_click_item);
}

void CFeatureListView::OnBasinsadlRotate()
{
  GetDocument()->Rotate(*(Basin_saddle *)r_click_featr,0,GetParent());
}

void CFeatureListView::OnRunoffSplice()
{
  GetDocument()->Splice(*(Runoff *)r_click_featr,GetParent());
}

void CFeatureListView::OnRButtonDown(UINT nFlags, CPoint point)
{
  CListCtrl& listCtrl=GetListCtrl();
  r_click_item=listCtrl.HitTest(point);
  if (r_click_item<0) return;
  listCtrl.SetItemState(r_click_item,LVIS_FOCUSED,LVIS_FOCUSED);

  r_click_featr=&GetFeature(r_click_item);
  r_click_type=GetFeatureType(r_click_item);
  CMenu graphMenu,*popup;
  graphMenu.LoadMenu(IDR_MENU_POPUP);
  popup=graphMenu.GetSubMenu(0);
  GetDocument()->setup_popup(popup,*r_click_featr,r_click_type);
  ClientToScreen(&point);
  popup->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,point.x,point.y,this);
}

void CFeatureListView::OnClick(NMHDR *pNMHDR, LRESULT *pResult)
{
  if (!tree_writeable()) AfxGetApp()->OnIdle(0);
  *pResult = 0;
}

void CFeatureListView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  CListView::OnKeyUp(nChar, nRepCnt, nFlags);
  if (!tree_writeable()) AfxGetApp()->OnIdle(0);
}

void CFeatureListView::OnEditSWI() 
{
  CListCtrl& listCtrl=GetListCtrl();
  int item1=listCtrl.GetNextItem(-1,LVNI_SELECTED),
    item2=listCtrl.GetNextItem(item1,LVNI_SELECTED);
  FT_index type1=GetFeatureType(item1),type2=GetFeatureType(item2);
  Feature &f1=GetFeature(item1),&f2=GetFeature(item2);
  CPromDoc *doc=GetDocument();
  CWnd *pw=GetParent();
  if (type1==FT_SADDLE && type2==FT_SADDLE)
    doc->SWI_switch_sym(f1,f2,pw);
  else if (type1==FT_SADDLE && type2==FT_BASINSADL)
    doc->SWI_switch_sym(f1,(Basin_saddle&)f2,pw);
  else if (type1==FT_BASINSADL && type2==FT_SADDLE)
    doc->SWI_switch_sym(f2,(Basin_saddle&)f1,pw);
  else if (type1==FT_BASINSADL && type2==FT_BASINSADL)
    doc->SWI_switch_sym((Basin_saddle&)f1,(Basin_saddle&)f2,pw);
  else if (type1==FT_PEAK && type2==FT_SADDLE)
    doc->reconnect(f1,f2,pw);
  else if (type1==FT_SADDLE && type2==FT_PEAK)
    doc->reconnect(f2,f1,pw);
  else if (type1==FT_PEAK && type2==FT_BASINSADL)
    doc->reconnect(f1,(Basin_saddle&)f2,pw);
  else if (type1==FT_BASINSADL && type2==FT_PEAK)
    doc->reconnect(f2,(Basin_saddle&)f1,pw);
  else if (type1==FT_PEAK && type2==FT_RUNOFF)
    doc->reconnect(f1,(Runoff&)f2,pw);
  else if (type1==FT_RUNOFF && type2==FT_PEAK)
    doc->reconnect(f2,(Runoff&)f1,pw);
  else if (type1==FT_SADDLE && type2==FT_RUNOFF)
    doc->SWI_switch(f1,(Runoff&)f2);
  else if (type1==FT_RUNOFF && type2==FT_SADDLE)
    doc->SWI_switch(f2,(Runoff&)f1);
  else if (type1==FT_BASINSADL && type2==FT_RUNOFF)
    doc->SWI_switch((Basin_saddle&)f1,(Runoff&)f2,pw);
  else if (type1==FT_RUNOFF && type2==FT_BASINSADL)
    doc->SWI_switch((Basin_saddle&)f2,(Runoff&)f1,pw);
  else assert(0);
}

void CFeatureListView::OnUpdateEditSWI(CCmdUI *pCmdUI)
{
  if (!tree_writeable()) {
    pCmdUI->Enable(FALSE);
    return;
  }
  CListCtrl& listCtrl=GetListCtrl();
  if (listCtrl.GetSelectedCount()!=2) {
    pCmdUI->Enable(FALSE);
    return;
  }
  int item1=listCtrl.GetNextItem(-1,LVNI_SELECTED),
    item2=listCtrl.GetNextItem(item1,LVNI_SELECTED);
  FT_index type1=GetFeatureType(item1),type2=GetFeatureType(item2);
  pCmdUI->Enable((type1!=FT_RUNOFF || type2!=FT_RUNOFF) &&
		 (type1!=FT_PEAK || type2!=FT_PEAK));
}

void CFeatureListView::OnUpdateHaveDLPRtree(CCmdUI *pCmdUI)
{
  // override the CPromDoc version, which also changes the text!
  pCmdUI->Enable(GetDocument()->dt_state!=NOT_LOADED);
}

void CFeatureListView::select_extreme_saddles(bool high)
{
  CListCtrl& listCtrl=GetListCtrl();
  int i,n=listCtrl.GetItemCount();
  short int dc=high?1:-1;
  Elevation elev,ext_elev=-elev_infty;
  Elev_endpt srch_end = high ? LO_END : HI_END,
    ovlp_end = high ? HI_END : LO_END;
  for (i=0; i<n; ++i) {
    listCtrl.SetItemState(i,0,LVIS_SELECTED); // clear all selections
    if (GetFeatureType(i)!=FT_PEAK) {
      elev=GetFeature(i).elev.*srch_end*dc;
      if (elev>ext_elev) ext_elev=elev;
    }
  }
  for (i=0; i<n; ++i) {
    if (GetFeatureType(i)!=FT_PEAK && GetFeature(i).elev.*ovlp_end*dc>=ext_elev)
      listCtrl.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
  }
}

void CFeatureListView::OnEditRQF() 
{
  int item=GetListCtrl().GetNextItem(-1,LVNI_SELECTED);
  CRQFinfo_dlg(GetDocument(),&GetFeature(item),GetFeatureType(item)).DoModal();
}

void CFeatureListView::OnUpdateEditRQF(CCmdUI *pCmdUI)
{
  CPromDoc *doc=GetDocument();
  CListCtrl& listCtrl=GetListCtrl();
  pCmdUI->Enable(doc->em_state!=NOT_LOADED && listCtrl.GetSelectedCount()==1 &&
		 doc->elev.contains(GetFeature(listCtrl.GetNextItem(-1,LVNI_SELECTED)).location));
}

void CFeatureListView::OnListSave()
{
  CString name=
    CPromDoc::GetFileName(CPromDoc::FO_SAVE,NULL,"Feature List","wfl");
  if (name.IsEmpty()) return;

  FILE *file=fopen(name,"wb");
  if (file==0) {
    CPromDoc::FileError("writing feature list",name);
    return;
  }
  CListCtrl& listCtrl=GetListCtrl();
  int i,n=listCtrl.GetItemCount();
  try {
    fwrite(n,file);
    GridPoint::write_stamp(file);
    for (i=0; i<n; ++i) {
      fwrite(GetFeature(i).location,file);
      fwrite(GetFeatureType(i),file);
    }
  }
  catch (file_error e) {
    CPromDoc::FileError("writing feature list",name,e.what());
  }
  fclose(file);
}

void CFeatureListView::load_list(bool append)
{
  CString name=
    CPromDoc::GetFileName(CPromDoc::FO_OPEN,NULL,"Feature List","wfl");
  if (name.IsEmpty()) return;

  FILE *file=fopen(name,"rb");
  if (file==0) {
    CPromDoc::FileError("reading feature list",name);
    return;
  }
  CListCtrl& listCtrl=GetListCtrl();
  Divide_tree& tree=GetDocument()->tree;
  Clip temp_clip;
  try {
    GridPoint location;
    FT_index ftype;
    int i,j,n;
    Sorted_featr_lists sfl;
    tree.sort_doms(&Domain::peak,sfl.peaks);
    tree.sort_doms((Feature Domain::*)&Domain::saddle,sfl.sadls);
    tree.sort_ro(sfl.ro);
    tree.sort_bs(sfl.bs);
    fread(n,file);
    GridPoint::read_stamp(file);
    for (i=0; i<n; ++i) {
      fread(location,file);
      fread(ftype,file);
      switch (ftype) {
      case FT_PEAK:
	if (!sfl.peaks.empty()) {
	  j=Divide_tree::search_dom(location,sfl.peaks,&Domain::peak);
	  if (j<sfl.peaks.size() && sfl.peaks[j]->peak.location==location)
	    temp_clip.add_featr(sfl.peaks[j]->peak,FT_PEAK);
	}
	break;
      case FT_SADDLE:
	if (!sfl.sadls.empty()) {
	  j=Divide_tree::search_dom(location,sfl.sadls,(Feature Domain::*)&Domain::saddle);
	  if (j<sfl.sadls.size() && sfl.sadls[j]->saddle.location==location)
	    temp_clip.add_featr(sfl.sadls[j]->saddle,FT_SADDLE);
	}
	break;
      case FT_RUNOFF:
	if (!sfl.ro.empty()) {
	  j=Divide_tree::search_ro(location,sfl.ro);
	  if (j<sfl.ro.size() && sfl.ro[j]->location==location)
	    temp_clip.add_featr(*sfl.ro[j],FT_RUNOFF);
	}
	break;
      case FT_BASINSADL:
	if (!sfl.bs.empty()) {
	  j=Divide_tree::search_bs(location,sfl.bs);
	  if (j<sfl.bs.size() && sfl.bs[j]->location==location)
	    temp_clip.add_featr(*sfl.bs[j],FT_BASINSADL);
	}
      }
    }
    if (temp_clip.ftr_count()==0) {
      MessageBox("No features in the file matched any feature in the divide tree.",
		 "Winprom List Load Error",MB_ICONERROR);
    }
    else {
      if (temp_clip.ftr_count()<n) {
	sprintf(buf,"%d features in the file did not match any feature in the divide tree.",
		n-temp_clip.ftr_count());
	MessageBox(buf,"Winprom List Load Error",MB_ICONERROR);
      }
      if (!CanPaste(temp_clip)) {
        MessageBox("The file contains some features that cannot\n"
		   "be added to this kind of feature list.",
		   "Winprom List Load Error",MB_ICONERROR);
      }
      else {
	if (!append) listCtrl.DeleteAllItems();
	listCtrl.SetItemCount(n);
	pasteClip(temp_clip);
      }
    }
  }
  catch (file_error e) {
    CPromDoc::FileError("reading feature list",name,e.what());
  }
  fclose(file);
  setItemIndicators();
}

void CFeatureListView::OnListLoad()
{
  load_list(false);
}

void CFeatureListView::OnListAppend()
{
  load_list(true);
}

static int compare_dir;

static int CALLBACK compare_name(LPARAM p1, LPARAM p2, LPARAM plist)
{
  CFeatureListView *list=(CFeatureListView *)plist;
  bool extra_info=list->has_extra_info();
  const Feature &ftr1=extra_info ? *list->features[p1] : *(Feature *)p1,
    &ftr2=extra_info ? *list->features[p2] : *(Feature *)p2;
  if (!ftr1.name && !ftr2.name) return 0;
  if (!ftr1.name) return 1;
  if (!ftr2.name) return -1;
  return strcmp(ftr1.name,ftr2.name)*compare_dir;
}

static int CALLBACK compare_elev(LPARAM p1, LPARAM p2, LPARAM plist)
{
  CFeatureListView *list=(CFeatureListView *)plist;
  bool extra_info=list->has_extra_info();
  const Feature &ftr1=extra_info ? *list->features[p1] : *(Feature *)p1,
    &ftr2=extra_info ? *list->features[p2] : *(Feature *)p2;
  return (ftr2.elev.midpt()-ftr1.elev.midpt())*compare_dir;
}

static int CALLBACK compare_location(LPARAM p1, LPARAM p2, LPARAM plist)
{
  CFeatureListView *list=(CFeatureListView *)plist;
  bool extra_info=list->has_extra_info();
  const Feature &ftr1=extra_info ? *list->features[p1] : *(Feature *)p1,
    &ftr2=extra_info ? *list->features[p2] : *(Feature *)p2;
  if (ftr1.location==ftr2.location) return 0;
  return (ftr1.location<ftr2.location ? -1 : 1)*compare_dir;
}

void CFeatureListView::build_features()
{
  CListCtrl& listCtrl=GetListCtrl();
  int i,n=listCtrl.GetItemCount();
  features.resize(n);
  for (i=0; i<n; ++i) {
    features[i]=&GetFeature(i);
    listCtrl.SetItemData(i,i);
  }
}

void CFeatureListView::OnHeaderClick(NMHDR *pNMHDR, LRESULT *pResult)
{
  NM_LISTVIEW *pNMListView = (NM_LISTVIEW *)pNMHDR;
  CListCtrl& listCtrl=GetListCtrl();
  compare_dir=GetKeyState(VK_SHIFT)<0 ? -1 : 1;

  if (pNMListView->iSubItem==name_col) {
    // sort by name
    listCtrl.SortItems(compare_name,(LPARAM)this);
  }
  else if (pNMListView->iSubItem==elev_col) {
    // sort by elevation
    listCtrl.SortItems(compare_elev,(LPARAM)this);
  }
  else if (pNMListView->iSubItem==loc_col) {
    // sort by location
    listCtrl.SortItems(compare_location,(LPARAM)this);
  }
  else // trying to sort by nonexistent column!
    assert(0);

  *pResult = 0;
}

void CFeatureListView::OnListParents()
{
  CListCtrl& listCtrl=GetListCtrl();
  CParentListView::List_info pli;
  int selItem=-1;
  while (true) {
    selItem=listCtrl.GetNextItem(selItem,LVNI_SELECTED);
    if (selItem<0) break;
    Feature *f=&GetFeature(selItem);
    switch (GetFeatureType(selItem)) {
    case FT_SADDLE:
      pli.sadls.push_back(f);
      break;
    case FT_PEAK:
      pli.peak=Domain::FromPeak(f);
      break;
    default: assert(0);
    }
  }
  CParentListView::init_list(GetDocument(),pli,GetParent(),default_min_prom());
}

void CFeatureListView::OnUpdateListParents(CCmdUI *pCmdUI)
{
  CListCtrl& listCtrl=GetListCtrl();
  if (listCtrl.GetSelectedCount()==0) {
    // nothing selected.  Disable control.
    pCmdUI->Enable(FALSE);
    return;
  }
  int selItem=-1;
  bool selected_peak=false;
  while (true) {
    selItem=listCtrl.GetNextItem(selItem,LVNI_SELECTED);
    if (selItem<0) break; // we're done checking the list of selected features.
    switch (GetFeatureType(selItem)) {
    case FT_PEAK:
      if (selected_peak) {
	// we've got multiple selected peaks
	pCmdUI->Enable(FALSE);
	return;
      }
      selected_peak=true;
      break;
    case FT_RUNOFF: case FT_BASINSADL:
      pCmdUI->Enable(FALSE);
      return;
    }
  }
  pCmdUI->Enable(selected_peak);
}

void CFeatureListView::swap_items(int nItem)
{
  CListCtrl& listCtrl=GetListCtrl();
  LVITEM item1,item2;
  item1.mask=item2.mask=LVIF_IMAGE|LVIF_INDENT|LVIF_PARAM|LVIF_STATE;
  item1.stateMask=item2.stateMask=(UINT)-1;
  item1.iSubItem=item2.iSubItem=0;
  item1.iItem=nItem;
  item2.iItem=nItem+1;
  listCtrl.GetItem(&item1);
  listCtrl.GetItem(&item2);
  item1.iItem=nItem+1;
  item2.iItem=nItem;
  listCtrl.SetItem(&item1);
  listCtrl.SetItem(&item2);
  list_fmt.set(GetDocument()->data);
  UpdateItem(nItem);
  UpdateItem(nItem+1);
}

void CFeatureListView::OnEditMoveUp()
{
  swap_items(GetListCtrl().GetNextItem(-1,LVNI_SELECTED)-1);
}

void CFeatureListView::OnUpdateEditMoveUp(CCmdUI *pCmdUI)
{
  const CListCtrl& listCtrl=GetListCtrl();
  pCmdUI->Enable(CanSort() && listCtrl.GetSelectedCount()==1 &&
		 listCtrl.GetNextItem(-1,LVNI_SELECTED)>0);
}

void CFeatureListView::OnEditMoveDown()
{
  swap_items(GetListCtrl().GetNextItem(-1,LVNI_SELECTED));
}

void CFeatureListView::OnUpdateEditMoveDown(CCmdUI *pCmdUI)
{
  const CListCtrl& listCtrl=GetListCtrl();
  pCmdUI->Enable(CanSort() && listCtrl.GetSelectedCount()==1 &&
		 listCtrl.GetNextItem(-1,LVNI_SELECTED)<
		 listCtrl.GetItemCount()-1);
}
