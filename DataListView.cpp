// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// DataListView.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "DataListView.h"
#include "DataFrame.h"
#include "LocationFmt_dlg.h"
#include "DataLocationFilter_dlg.h"
#include "DataListSummary_dlg.h"
#include "RQFinfo_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char CDataListView::buf[256];


/////////////////////////////////////////////////////////////////////////////
// CDataListView

IMPLEMENT_DYNCREATE(CDataListView, CListView)

CDataListView::CDataListView()
{
}

CDataListView::~CDataListView()
{
}

const char *CDataListView::init_title=0;


BEGIN_MESSAGE_MAP(CDataListView, CListView)
  //{{AFX_MSG_MAP(CDataListView)
  ON_WM_CREATE()
  ON_COMMAND(ID_LIST_FORMAT, OnListFormat)
  ON_UPDATE_COMMAND_UI(ID_LIST_FORMAT, OnUpdateHaveItems)
  ON_COMMAND(ID_EDIT_UNLIST, OnEditUnlist)
  ON_UPDATE_COMMAND_UI(ID_EDIT_UNLIST, OnUpdateEditAnySelect)
  ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
  ON_COMMAND(ID_EDIT_SELECT_INVERT, OnEditSelectInvert)
  ON_COMMAND(ID_EDIT_RECORD, OnEditRecord)
  ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
  ON_COMMAND(ID_EDIT_CENTER, OnEditCenter)
  ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnHeaderClick)
  ON_COMMAND(ID_VIEW_LISTX, OnViewList)
  ON_UPDATE_COMMAND_UI(ID_VIEW_LISTX, OnUpdateViewList)
  ON_COMMAND(ID_VIEW_REPORT, OnViewReport)
  ON_UPDATE_COMMAND_UI(ID_VIEW_REPORT, OnUpdateViewReport)
  ON_COMMAND(ID_LIST_RELIST, OnListRelist)
  ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
  ON_COMMAND(ID_LIST_SUMMARY, OnListSummary)
  ON_UPDATE_COMMAND_UI(ID_EDIT_CENTER, OnUpdateSingleSelect)
  ON_UPDATE_COMMAND_UI(ID_EDIT_RECORD, OnUpdateEditRecord)
  ON_COMMAND(ID_EDIT_SEARCH, OnEditSearch)
  ON_UPDATE_COMMAND_UI(ID_EDIT_SEARCH, OnUpdateEditSearch)
  ON_COMMAND(ID_EDIT_SELECT_FILTER_SET, OnEditSelectFilterSet)
  ON_COMMAND(ID_EDIT_SELECT_FILTER_PRECISE, OnEditSelectFilterPrecise)
  ON_COMMAND(ID_EDIT_SELECT_FILTER_CLEAR, OnEditSelectFilterClear)
  ON_COMMAND(ID_FILE_TEXT, OnFileText)
  ON_COMMAND(ID_FILE_HTML, OnFileHTML)
  ON_COMMAND(ID_FILE_CSV, OnFileCSV)
  ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
  ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
  ON_WM_LBUTTONUP()
  ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
  ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
  ON_WM_MOUSEMOVE()
  ON_COMMAND(ID_EDIT_BROWSE, OnEditBrowse)
  ON_WM_RBUTTONDOWN()
  ON_COMMAND(ID_RECORD_BROWSE, OnRecordBrowse)
  ON_COMMAND(ID_RECORD_CENTER, OnRecordCenter)
  ON_COMMAND(ID_RECORD_DELETE, OnRecordDelete)
  ON_COMMAND(ID_RECORD_EDIT, OnRecordEdit)
  ON_COMMAND(ID_RECORD_UNLIST, OnRecordUnlist)
  ON_COMMAND(ID_RECORD_RQF, OnRQFinfo)
  ON_COMMAND(ID_EDIT_RQF, OnEditRQF)
  ON_UPDATE_COMMAND_UI(ID_EDIT_RQF, OnUpdateEditRQF)
  ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateSingleSelect)
  ON_UPDATE_COMMAND_UI(ID_LIST_SUMMARY, OnUpdateHaveItems)
  ON_UPDATE_COMMAND_UI(ID_FILE_TEXT, OnUpdateHaveItems)
  ON_UPDATE_COMMAND_UI(ID_FILE_HTML, OnUpdateHaveItems)
  ON_UPDATE_COMMAND_UI(ID_FILE_CSV, OnUpdateHaveItems)
  ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditAnySelect)
  ON_UPDATE_COMMAND_UI(ID_EDIT_BROWSE, OnUpdateSingleSelect)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDataListView diagnostics

#ifdef _DEBUG
void CDataListView::AssertValid() const
{
  CListView::AssertValid();
}

void CDataListView::Dump(CDumpContext& dc) const
{
  CListView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CDataListView message handlers

static unsigned get_column_width(Field_type ft)
{
  switch (ft) {
  case FLD_STRING: case FLD_LOC: return 160;
  case FLD_INTEGER: case FLD_REAL: return 72;
  }
  assert(0);
  return 0;
}

void CDataListView::set_client_width(const Database& data)
{
  CDataFrame::client_width=0;
  for (Database::const_FieldID fid=data.begin_field();
       fid!=data.end_field(); ++fid) {
    CDataFrame::client_width+=get_column_width((*fid).get_type());
  }
}

void CDataListView::init_list(CPromDoc *document)
{
  document->init_data_lf.align(document->data);
  if (CDataLocationFilter_dlg(document->init_data_lf).DoModal()!=IDOK) return;

  init_title="Data";
  set_client_width(document->data);
  CSingleDocTemplate& DataViewTpl=CPromApp::getApp()->DataListViewTpl;
  CDataFrame *newlist=(CDataFrame *)DataViewTpl.CreateNewFrame(document,NULL);
  DataViewTpl.InitialUpdateFrame(newlist,document);
}

static const vector<GridPoint> *init_fixed_list=0;

void CDataListView::init_list(CPromDoc *doc, const vector<GridPoint>& rlist,
			      const char *t)
{
  set_client_width(doc->data);
  CSingleDocTemplate& DataViewTpl=CPromApp::getApp()->DataListViewTpl;
  CDataFrame *newlist=(CDataFrame *)DataViewTpl.CreateNewFrame(doc,NULL);
  init_fixed_list=&rlist;
  init_title=t;
  DataViewTpl.InitialUpdateFrame(newlist,doc);
  init_fixed_list=0;
}

void CDataListView::setViewStyle(DWORD newStyle)
{
  HWND hListCtrl=GetListCtrl().GetSafeHwnd();
  DWORD oldStyle=GetWindowLong(hListCtrl,GWL_STYLE);
  if ((oldStyle&LVS_TYPEMASK)!=newStyle) {
    oldStyle&=~LVS_TYPEMASK;
    SetWindowLong(hListCtrl,GWL_STYLE,newStyle|oldStyle);
  }
}

void CDataListView::create_columns()
{
  CListCtrl& listCtrl=GetListCtrl();
  listCtrl.InsertColumn(0,"Location",LVCFMT_LEFT,160);
  Database& data=GetDocument()->data;
  unsigned col_ndx=1;
  for (Database::const_FieldID fid=data.begin_field();
       fid!=data.end_field(); ++fid,++col_ndx) {
    Field_type ft=(*fid).get_type();
    listCtrl.InsertColumn(col_ndx,(*fid).name,
      ft==FLD_STRING||ft==FLD_LOC?LVCFMT_LEFT:LVCFMT_RIGHT,
      get_column_width(ft));
  }
}

int CDataListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CListView::OnCreate(lpCreateStruct) == -1)
    return -1;

  setViewStyle(LVS_REPORT);
  create_columns();

  return 0;
}

void CDataListView::AddItem(Database::RecordID rid)
{
  if (rid==REC_NONE) return;
  CListCtrl& listCtrl=GetListCtrl();
  const GridPoint& loc=GetDocument()->data.get_location(rid);
  int item=listCtrl.GetItemCount();
  loc.sprint(buf);
  listCtrl.InsertItem(item,buf);
  listCtrl.SetItemData(item,rid);
  UpdateItem(item);
}

void CDataListView::UpdateRecord(Database::RecordID rec)
{
  const Database& data=GetDocument()->data;
  CListCtrl& listCtrl=GetListCtrl();

  int nitem=listCtrl.GetItemCount();
  for (int item=0; item<nitem; ++item)
    if (listCtrl.GetItemData(item)==rec)
      UpdateItem(item);
}

void CDataListView::UpdateItem(int item)
{
  int ivalue;
  float rvalue;
  const char *svalue;
  GridPoint lvalue;
  unsigned col_ndx=1;
  const Database& data=GetDocument()->data;
  Database::RecordID rec=GetListCtrl().GetItemData(item);

  for (Database::const_FieldID fid=data.begin_field();
       fid!=data.end_field(); ++fid,++col_ndx) {
    switch ((*fid).get_type()) {
    case FLD_INTEGER:
      data.get_value(rec,fid,ivalue);
      sprintf(buf,"%d",ivalue);
      break;
    case FLD_REAL:
      data.get_value(rec,fid,rvalue);
      sprintf(buf,"%g",rvalue);
      break;
    case FLD_STRING:
      data.get_value(rec,fid,svalue);
      sprintf(buf,"%s",svalue);
      break;
    case FLD_LOC:
      data.get_value(rec,fid,lvalue);
      if (lvalue) lvalue.sprint(buf);
      else buf[0]='\0';
      break;
    default:
      assert(0);
    }
    GetListCtrl().SetItemText(item,col_ndx,buf);
  }
}

void CDataListView::OnInitialUpdate()
{
  CPromDoc *doc=GetDocument();
  Database& data=doc->data;
  Database::RecordID rid;

  if (init_title) {
    if (init_fixed_list) fixed_list=*init_fixed_list;
    else list_filter=doc->init_data_lf;
    title=init_title;
    init_title=0;
  }

  if (!fixed_list.empty()) {
    for (vector<GridPoint>::iterator fli=fixed_list.begin();
	 fli!=fixed_list.end(); ++fli) {
      rid=data.get_record(*fli);
      if (rid!=REC_NONE) AddItem(rid);
    }
  }
  else {
    list_fmt.set();

    Database::RecordID nrec=data.get_nrec();
    bool *nonorphans=0;
    if (list_filter.orphan!=list_filter.nonorph)
      nonorphans=doc->nonorphans();
    list_filter.align(data);
    for (rid=0; rid<nrec; ++rid)
      if (list_filter.test(data,rid,!nonorphans || nonorphans[rid]))
	AddItem(rid);
    delete[] nonorphans;
  }

  setItemIndicators();
}

void CDataListView::OnUpdate(CView *, LPARAM lHint, CObject *pHint)
{
  if (lHint!=(LPARAM)&CPromDoc::dummyModifyData) return;
  CPromDoc *doc=GetDocument();
  CListCtrl& listCtrl=GetListCtrl();
  if (doc->record_change==REC_NONE) {
    listCtrl.DeleteAllItems();
    while (listCtrl.DeleteColumn(0)); // delete all columns
    create_columns();
    OnInitialUpdate();
  }
  else {
    UpdateRecord(doc->record_change);
  }
}

void CDataListView::OnListFormat()
{
  CListCtrl& listCtrl=GetListCtrl();
  CLocationFmt_dlg dlg(GetParent(),list_fmt);
  if (dlg.DoModal()!=IDOK) return;

  list_fmt.set();
  int i,nItems=listCtrl.GetItemCount();
  Database::RecordID rec;
  for (i=0; i<nItems; ++i) {
    rec=listCtrl.GetItemData(i);
    const GridPoint& loc=GetDocument()->data.get_location(rec);
    loc.sprint(buf);
    listCtrl.SetItemText(i,0,buf);
  }
}

void CDataListView::OnUpdateHaveItems(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetListCtrl().GetItemCount()>0);
}

void CDataListView::OnEditUnlist()
{
  CListCtrl& listCtrl=GetListCtrl();
  int selItem;
  while (true) {
    selItem=listCtrl.GetNextItem(-1,LVNI_SELECTED);
    if (selItem<0) break;
    listCtrl.DeleteItem(selItem);
  }
  setItemIndicators();
}

void CDataListView::OnUpdateEditAnySelect(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetListCtrl().GetSelectedCount()>0);
}

void CDataListView::OnEditSelectAll()
{
  CListCtrl& listCtrl=GetListCtrl();
  int i,n=listCtrl.GetItemCount();
  for (i=0; i<n; ++i)
    listCtrl.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
}

void CDataListView::OnEditSelectInvert()
{
  CListCtrl& listCtrl=GetListCtrl();
  int i,n=listCtrl.GetItemCount();
  for (i=0; i<n; ++i)
    listCtrl.SetItemState(i,listCtrl.GetItemState(i,LVIS_SELECTED)^LVIS_SELECTED,
      LVIS_SELECTED);
}

void CDataListView::OnEditRecord()
{
  int item=GetListCtrl().GetNextItem(-1,LVNI_SELECTED);
  assert(item>=0);
  GetDocument()->EditRecord(GetListCtrl().GetItemData(item),this);
}

void CDataListView::OnUpdateEditRecord(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetDocument()->tree_writeable &&
		 GetListCtrl().GetSelectedCount()==1);
}

void CDataListView::OnUpdateSingleSelect(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(GetListCtrl().GetSelectedCount()==1);
}

void CDataListView::OnDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
  OnEditRecord();
  *pResult=0;
}

void CDataListView::OnEditCenter()
{
  CListCtrl& listCtrl=GetListCtrl();
  int i=listCtrl.GetNextItem(-1,LVNI_SELECTED);
  CPromDoc *doc=GetDocument();
  const GridPoint& gp=doc->data.get_location(listCtrl.GetItemData(i));
  CPromApp::getApp()->viewLocation(gp);
}

void CDataListView::SetViewTitle()
{
  CPromDoc *document=GetDocument();
  CString t;
  if (!document->db_name.IsEmpty()) t=document->db_name+" - ";
  GetParent()->SetWindowText(t+title);
}

struct SortInfo {
	const Database &data;
	unsigned int index;

	SortInfo(const Database &database, unsigned int index) 
		: data(database), index(index) {
	}
};

static int compare_dir;

static int CALLBACK compare_data_key(LPARAM data1param, LPARAM data2param,
				     LPARAM datafnparam)
{
  SortInfo *info = (SortInfo *) datafnparam;
  const vector<GridPoint>& locations = info->data.get_locations();
  if (locations[data1param]<locations[data2param]) return -compare_dir;
  if (locations[data1param]>locations[data2param]) return compare_dir;
  return 0;
}

static int CALLBACK compare_data_int(LPARAM data1param, LPARAM data2param,
				     LPARAM datafnparam)
{
  SortInfo *info = (SortInfo *)datafnparam;
  const vector<int>& ints= (info->data.begin_field() + info->index)->get_ints();
  return (ints[data1param]-ints[data2param])*compare_dir;
}

static int CALLBACK compare_data_real(LPARAM data1param, LPARAM data2param,
				      LPARAM datafnparam)
{
  SortInfo *info = (SortInfo *)datafnparam;
  const vector<float>& reals= (info->data.begin_field() + info->index)->get_reals();
  if (reals[data1param]<reals[data2param]) return -compare_dir;
  if (reals[data1param]>reals[data2param]) return compare_dir;
  return 0;
}

static int CALLBACK compare_data_string(LPARAM data1param, LPARAM data2param,
					LPARAM datafnparam)
{
  SortInfo *info = (SortInfo *)datafnparam;
  const vector<CString>& strings= (info->data.begin_field() + info->index)->get_strings();
  if (strings[data1param]<strings[data2param]) return -compare_dir;
  if (strings[data1param]>strings[data2param]) return compare_dir;
  return 0;
}

static int CALLBACK compare_data_loc(LPARAM data1param, LPARAM data2param,
				     LPARAM datafnparam)
{
  SortInfo *info = (SortInfo *)datafnparam;
  const vector<GridPoint>& locations= (info->data.begin_field() + info->index)->get_locs();
  if (locations[data1param]<locations[data2param]) return -compare_dir;
  if (locations[data1param]>locations[data2param]) return compare_dir;
  return 0;
}

void CDataListView::OnHeaderClick(NMHDR *pNMHDR, LRESULT *pResult)
{
  NM_LISTVIEW *pNMListView = (NM_LISTVIEW *)pNMHDR;
  CListCtrl& listCtrl=GetListCtrl();
  const Database& data=GetDocument()->data;
  compare_dir=GetKeyState(VK_SHIFT)<0 ? -1 : 1;
  if (pNMListView->iSubItem>0) {
    // data field
    unsigned fld_ndx=pNMListView->iSubItem-1;

	SortInfo info(data, fld_ndx);

    switch ((*(data.begin_field()+fld_ndx)).get_type()) {
    case FLD_INTEGER:
      listCtrl.SortItems(compare_data_int, (LPARAM) &info);
      break;
    case FLD_REAL:
		listCtrl.SortItems(compare_data_real, (LPARAM) &info);
      break;
    case FLD_STRING:
		listCtrl.SortItems(compare_data_string, (LPARAM) &info);
      break;
    case FLD_LOC:
		listCtrl.SortItems(compare_data_loc, (LPARAM) &info);
    }
  }
  else {
    // location
    listCtrl.SortItems(compare_data_key, 
      (LPARAM)(&data.get_locations()));
  }

  *pResult = 0;
}

void CDataListView::OnViewList()
{
  setViewStyle(LVS_LIST);
}

void CDataListView::OnUpdateViewList(CCmdUI *pCmdUI)
{
  pCmdUI->SetRadio((GWL()&LVS_TYPEMASK)==LVS_LIST);
}

void CDataListView::OnViewReport()
{
  setViewStyle(LVS_REPORT);
}

void CDataListView::OnUpdateViewReport(CCmdUI *pCmdUI)
{
  pCmdUI->SetRadio((GWL()&LVS_TYPEMASK)==LVS_REPORT);
}

void CDataListView::setItemIndicators()
{
  CStatusBar& dataStatus=((CDataFrame *)GetParent())->dataStatus;
  CListCtrl& listCtrl=GetListCtrl();
  sprintf(buf,"%d items",listCtrl.GetItemCount());
  dataStatus.SetPaneText(1,buf);
}

void CDataListView::OnListRelist()
{
  if (fixed_list.empty() && CDataLocationFilter_dlg(list_filter).DoModal()!=IDOK) return;

  GetListCtrl().DeleteAllItems();
  OnInitialUpdate();
}

void CDataListView::OnEditDelete()
{
  GetDocument()->DeleteRecord(GetListCtrl().GetNextItem(-1,LVNI_SELECTED));
}

void CDataListView::OnListSummary()
{
  CDataListSummary_dlg(this).DoModal();
}

void CDataListView::OnEditSearch()
{
  bool all_found=true,none_found=true,item_found;
  CPromDoc *doc=GetDocument();
  CListCtrl& listCtrl=GetListCtrl();

  int j,n=listCtrl.GetItemCount();
  unsigned i,nc=doc->copyPaste.ftr_count();
  for (i=0; i<nc; ++i) {
    const GridPoint& target=doc->copyPaste.list[i]->location;
    item_found=false;
    for (j=0; j<n; ++j) {
      if (target==doc->data.get_location(listCtrl.GetItemData(j))) {
	item_found=true;
	listCtrl.SetItemState(j,LVIS_SELECTED,LVIS_SELECTED);
      }
    }
    if (item_found) none_found=false;
    else all_found=false;
  }
  nc=doc->copyPaste.rec_count();
  for (i=0; i<nc; ++i) {
    const GridPoint& target=doc->data.get_location(doc->copyPaste.orph_list[i]);
    for (j=0; j<n; ++j) {
      if (target==doc->data.get_location(listCtrl.GetItemData(j))) {
	item_found=true;
	listCtrl.SetItemState(j,LVIS_SELECTED,LVIS_SELECTED);
      }
    }
    if (item_found) none_found=false;
    else all_found=false;
  }

  if (none_found) {
    MessageBox("No feature on the clipboard is in this list.",
	       "Winprom Search");
  }
  else if (!all_found) {
    MessageBox("Some feature(s) on the clipboard are not in this list.",
	       "Winprom Search");
  }
}

void CDataListView::OnUpdateEditSearch(CCmdUI *pCmdUI)
{
  const Clip& clip=GetDocument()->copyPaste;
  pCmdUI->Enable(!clip.empty());
}

void CDataListView::OnEditSelectFilterSet()
{
  const Database& data=GetDocument()->data;
  Record_location_filter filter;
  filter.align(data);
  CDataLocationFilter_dlg dlg(filter);
  if (dlg.DoModal()!=IDOK) return;
  CListCtrl& listCtrl=GetListCtrl();
  int i,n=listCtrl.GetItemCount();
  bool *nonorphans=0;
  if (filter.orphan!=filter.nonorph)
    nonorphans=GetDocument()->nonorphans();
  Database::RecordID rid;
  for (i=0; i<n; ++i) {
    rid=listCtrl.GetItemData(i);
    if (filter.test(data,rid,!nonorphans || nonorphans[rid]))
      listCtrl.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
  }
  delete[] nonorphans;
}

void CDataListView::OnEditSelectFilterPrecise()
{
  const Database& data=GetDocument()->data;
  Record_location_filter filter;
  filter.align(data);
  CDataLocationFilter_dlg dlg(filter);
  if (dlg.DoModal()!=IDOK) return;
  CListCtrl& listCtrl=GetListCtrl();
  int i,n=listCtrl.GetItemCount();
  bool *nonorphans=0;
  if (filter.orphan!=filter.nonorph)
    nonorphans=GetDocument()->nonorphans();
  Database::RecordID rid;
  for (i=0; i<n; ++i) {
    rid=listCtrl.GetItemData(i);
    listCtrl.SetItemState(i,filter.test(data,rid,
	!nonorphans || nonorphans[rid])?LVIS_SELECTED:0,
	LVIS_SELECTED);
  }
  delete[] nonorphans;
}

void CDataListView::OnEditSelectFilterClear()
{
  const Database& data=GetDocument()->data;
  Record_location_filter filter;
  filter.align(data);
  CDataLocationFilter_dlg dlg(filter);
  if (dlg.DoModal()!=IDOK) return;
  CListCtrl& listCtrl=GetListCtrl();
  int selItem=-1;
  bool *nonorphans=0;
  if (filter.orphan!=filter.nonorph)
    nonorphans=GetDocument()->nonorphans();
  Database::RecordID rid;
  while (true) {
    selItem=listCtrl.GetNextItem(selItem,LVNI_SELECTED);
    if (selItem<0) break;
    rid=listCtrl.GetItemData(selItem);
    if (filter.test(data,rid,!nonorphans || nonorphans[rid]))
      listCtrl.SetItemState(selItem,0,LVIS_SELECTED);
  }
  delete[] nonorphans;
}

static unsigned *width;

void CDataListView::printHeader(FILE *out_file, const Database& data,
				Format_style style) const
{
  const char *style_del=get_delimiter(style,CA_CENTERx);
  Field_type ft;
  list_fmt.set(style==FMS_CSV);
  width=new unsigned[data.get_nrec()];
  if (style==FMS_HTML) fprintf(out_file,style_del);
  fprintf(out_file,GridPoint::field_format,"Location");
  const Database::const_FieldID bf=data.begin_field();
  for (Database::const_FieldID fid=bf; fid!=data.end_field(); ++fid) {
    width[fid-bf]=(*fid).get_width();
    ft=(*fid).get_type();
    fprintf(out_file,ft==FLD_STRING||ft==FLD_LOC?"%s%-*s":"%s%*s",
	    style_del,width[fid-bf],(*fid).name);
  }
}

void CDataListView::printRecord(int item, FILE *out_file, const Database& data,
				Format_style style) const
{
  static int int_val;
  static float real_val;
  static CString string_val;
  static GridPoint loc_val;
  const char *style_del_left=get_delimiter(style,CA_LEFTx),
    *style_del_right=get_delimiter(style,CA_RIGHTx);
  list_fmt.set(style==FMS_CSV);
  if (style==FMS_HTML) fprintf(out_file,style_del_right);
  Database::RecordID rid=GetListCtrl().GetItemData(item);
  data.get_location(rid).print(out_file);
  const Database::const_FieldID bf=data.begin_field();
  for (Database::const_FieldID fid=bf; fid!=data.end_field(); ++fid) {
    switch ((*fid).get_type()) {
    case FLD_INTEGER:
      data.get_value(rid,fid,int_val);
      fprintf(out_file,"%s%*d",style_del_right,width[fid-bf],int_val);
      break;
    case FLD_REAL:
      data.get_value(rid,fid,real_val);
      fprintf(out_file,"%s%*g",style_del_right,width[fid-bf],real_val);
      break;
    case FLD_STRING:
      data.get_value(rid,fid,string_val);
      fprintf(out_file,"%s%-*s",style_del_left,width[fid-bf], std::string(string_val).c_str());
      break;
    case FLD_LOC:
      data.get_value(rid,fid,loc_val);
      loc_val.sprint(buf);
      fprintf(out_file,"%s%-*s",style_del_left,width[fid-bf],buf);
      break;
    }
  }
}

void CDataListView::OnFileText()
{
  CListCtrl& listCtrl=GetListCtrl();
  CString name=CPromDoc::GetFileName(CPromDoc::FO_SAVE,0,"Text","txt",GetParent());
  if (name.IsEmpty()) return;
  FILE *out_file=fopen(name,"w");
  if (out_file==0) {
    CPromDoc::FileError("opening output file",name);
    return;
  }
  const Database& data=GetDocument()->data;
  printHeader(out_file,data,FMS_FLAG);
  putc('\n',out_file);
  int item,itemCount=listCtrl.GetItemCount();
  for (item=0; item<itemCount; ++item) {
    printRecord(item,out_file,data,FMS_FLAG);
    putc('\n',out_file);
  }
  fclose(out_file);
  delete[] width;
}

void CDataListView::OnFileHTML()
{
  CListCtrl& listCtrl=GetListCtrl();
  CString name=CPromDoc::GetFileName(CPromDoc::FO_SAVE,0,"HTML","htm",GetParent());
  if (name.IsEmpty()) return;
  FILE *out_file=fopen(name,"w");
  if (out_file==0) {
    CPromDoc::FileError("opening output HTML file",name);
    return;
  }
  const Database& data=GetDocument()->data;
  fprintf(out_file,"<HTML> <TABLE BORDER>\n");
  fprintf(out_file,"<TR>");
  printHeader(out_file,data,FMS_HTML);
  fprintf(out_file,"</TR>\n");
  int item,itemCount=listCtrl.GetItemCount();
  for (item=0; item<itemCount; ++item) {
    fprintf(out_file,"<TR>");
    printRecord(item,out_file,data,FMS_HTML);
    fprintf(out_file,"</TR>\n");
  }
  fprintf(out_file,"</TABLE></HTML>\n");
  fclose(out_file);
  delete[] width;
}

void CDataListView::OnFileCSV()
{
  CListCtrl& listCtrl=GetListCtrl();
  CString name=CPromDoc::GetFileName(CPromDoc::FO_SAVE,0,"CSV","csv",GetParent());
  if (name.IsEmpty()) return;
  FILE *out_file=fopen(name,"w");
  if (out_file==0) {
    CPromDoc::FileError("opening output CSV file",name);
    return;
  }
  const Database& data=GetDocument()->data;
  printHeader(out_file,data,FMS_CSV);
  putc('\n',out_file);
  int item,itemCount=listCtrl.GetItemCount();
  for (item=0; item<itemCount; ++item) {
    printRecord(item,out_file,data,FMS_CSV);
    putc('\n',out_file);
  }
  fclose(out_file);
  delete[] width;
}

void CDataListView::pasteClip(const Clip& clp)
{
  const Database& data=GetDocument()->data;
  Database::RecordID rid;
  unsigned i,n=clp.ftr_count();
  for (i=0; i<n; ++i) {
    rid=data.get_record(clp.list[i]->location);
    if (rid==REC_NONE) continue;
    AddItem(rid);
  }
  for (vector<Database::RecordID>::const_iterator oi=clp.orph_list.begin();
       oi!=clp.orph_list.end(); ++oi)
    AddItem(*oi);
  setItemIndicators();
}

void CDataListView::OnEditPaste() 
{
  pasteClip(GetDocument()->copyPaste);
}

bool CDataListView::CanPaste(const Clip& clip) const
{
  if (!clip.orph_list.empty()) return true;
  const Database& data=GetDocument()->data;
  unsigned i,n=clip.ftr_count();
  for (i=0; i<n; ++i)
    if (data.get_record(clip.list[i]->location)!=REC_NONE) return true;
  return false;
}

void CDataListView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(CanPaste(GetDocument()->copyPaste));
}

void CDataListView::OnLButtonUp(UINT nFlags, CPoint point)
{
  GetDocument()->takeDrop(this,point);

  CListView::OnLButtonUp(nFlags, point);
}

void CDataListView::copyClip(Clip& clp) const
{
  Sorted_featr_lists sfl;
  CPromDoc *doc=GetDocument();
  Divide_tree& tree=doc->tree;
  CListCtrl& listCtrl=GetListCtrl();
  const Database& data=doc->data;

  clp.clear();
  tree.sort_doms(&Domain::peak,sfl.peaks);
  tree.sort_doms((Feature Domain::*)&Domain::saddle,sfl.sadls);
  tree.sort_ro(sfl.ro);
  tree.sort_bs(sfl.bs);

  int selItem=-1;
  while (true) {
    selItem=listCtrl.GetNextItem(selItem,LVNI_SELECTED);
    if (selItem<0) break;
    const GridPoint& data_loc=data.get_location(listCtrl.GetItemData(selItem));
    // search
    if (!CPromDoc::clip_grid_point(clp,data_loc,sfl)) {
      // data record is orphaned.  Put it on orphan clipboard.
      clp.orph_list.push_back(listCtrl.GetItemData(selItem));
    }
  }
}

void CDataListView::OnEditCopy()
{
  copyClip(GetDocument()->copyPaste);
}

void CDataListView::OnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
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

void CDataListView::OnMouseMove(UINT nFlags, CPoint point)
{
  if (nFlags&MK_LBUTTON) {
    // dragging
    CPoint ptRoot(point);
    MapWindowPoints(NULL,&ptRoot,1);
    CImageList::DragMove(ptRoot);
    SetCursor(AfxGetApp()->LoadStandardCursor(GetDocument()->
      canTakeDrop(this,point) ? IDC_ARROW : IDC_NO));
  }

  CListView::OnMouseMove(nFlags, point);
}

void CDataListView::OnEditBrowse()
{
  int item=GetListCtrl().GetNextItem(-1,LVNI_SELECTED);
  assert(item>=0);
  CPromApp::getApp()->Navigate
    (GetDocument()->data.get_location(GetListCtrl().GetItemData(item)));
}

void CDataListView::OnRButtonDown(UINT nFlags, CPoint point)
{
  CListCtrl& listCtrl=GetListCtrl();
  r_click_item=listCtrl.HitTest(point);
  if (r_click_item<0) return;
  listCtrl.SetItemState(r_click_item,LVIS_FOCUSED,LVIS_FOCUSED);

  r_click_rec=GetListCtrl().GetItemData(r_click_item);
  CMenu graphMenu,*popup;
  graphMenu.LoadMenu(IDR_MENU_POPUP);
  popup=graphMenu.GetSubMenu(4);
  CPromDoc *doc=GetDocument();
  if (doc->em_state!=NOT_LOADED &&
      doc->elev.contains(doc->data.get_location(r_click_rec))) {
    popup->EnableMenuItem(ID_RECORD_RQF,MF_BYCOMMAND|MF_ENABLED);
  }
  ClientToScreen(&point);
  popup->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,point.x,point.y,this);
}

void CDataListView::OnRecordBrowse()
{
  CPromApp::getApp()->Navigate(GetDocument()->data.get_location(r_click_rec));
}

void CDataListView::OnRecordCenter()
{
  CPromApp::getApp()->viewLocation(GetDocument()->data.get_location(r_click_rec));
}

void CDataListView::OnRQFinfo()
{
  CPromDoc *doc=GetDocument();
  CRQFinfo_dlg(doc,doc->data.get_location(r_click_rec),this).DoModal();
}

void CDataListView::OnRecordDelete()
{
  GetDocument()->DeleteRecord(r_click_rec);
}

void CDataListView::OnRecordEdit()
{
  GetDocument()->EditRecord(r_click_rec,this);
}

void CDataListView::OnRecordUnlist()
{
  GetListCtrl().DeleteItem(r_click_item);
}

void CDataListView::OnEditRQF()
{
  GridPoint gp=GetDocument()->data.get_location(GetListCtrl().GetItemData(GetListCtrl().GetNextItem(-1,LVNI_SELECTED)));
  CRQFinfo_dlg(GetDocument(),gp).DoModal();
}

void CDataListView::OnUpdateEditRQF(CCmdUI *pCmdUI)
{
  CPromDoc *doc=GetDocument();
  CListCtrl& listCtrl=GetListCtrl();
  pCmdUI->Enable(doc->em_state!=NOT_LOADED && listCtrl.GetSelectedCount()==1 &&
		 doc->elev.contains(doc->data.get_location(listCtrl.GetItemData(listCtrl.GetNextItem(-1,LVNI_SELECTED)))));
}
