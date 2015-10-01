// RunoffListView.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "RunoffListView.h"
#include "ListFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CRunoffListView

IMPLEMENT_DYNCREATE(CRunoffListView, CFeatureListView)

CRunoffListView::CRunoffListView()
{
}

CRunoffListView::~CRunoffListView()
{
}

bool CRunoffListView::is_initial;
RunoffFilter CRunoffListView::init_rf;

BEGIN_MESSAGE_MAP(CRunoffListView, CFeatureListView)
  //{{AFX_MSG_MAP(CRunoffListView)
  ON_WM_CREATE()
  //}}AFX_MSG_MAP
  ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnHeaderClick)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRunoffListView diagnostics

#ifdef _DEBUG
void CRunoffListView::AssertValid() const
{
  CFeatureListView::AssertValid();
}

void CRunoffListView::Dump(CDumpContext& dc) const
{
  CFeatureListView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CRunoffListView message handlers

void CRunoffListView::init_list(CPromDoc *document)
{
  is_initial=CRunoffList_dlg(document->data,init_rf,document->is_drainage).DoModal()==IDOK;
  if (!is_initial) return;

  CListFrame::client_width=32;
  CSingleDocTemplate& RunoffListViewTpl=CPromApp::getApp()->RunoffListViewTpl;
  CListFrame *newlist=(CListFrame *)RunoffListViewTpl.CreateNewFrame(document,NULL);
  RunoffListViewTpl.InitialUpdateFrame(newlist,document);
}

int CRunoffListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CFeatureListView::OnCreate(lpCreateStruct) == -1)
    return -1;

  InsertColumn(IQ_COL,"IQs",LVCFMT_CENTER,32);

  return 0;
}

void CRunoffListView::UpdateItem(int nItem)
{
  CFeatureListView::UpdateItem(nItem);
  sprintf(buf,"%d",((Runoff &)GetFeature(nItem)).interior_quads);
  GetListCtrl().SetItemText(nItem,IQ_COL,buf);
}

void CRunoffListView::printFeature(int item, FILE *out_file, Format_style style)
{
  switch (style) {
  case FMS_FLAG: Runoff::format="%Q R %F"; break;
  case FMS_HTML: Runoff::format="<TD align=right>%Q<TD align=center>R%F"; break;
  case FMS_CSV: Runoff::format="%Q,R,%F"; break;
  default: assert(0);
  }
  ((Runoff&)GetFeature(item)).print(out_file);
}

void CRunoffListView::printHeader(FILE *out_file, Format_style style)
{
  switch (style) {
  case FMS_FLAG: Runoff::format="%Q T %F"; break;
  case FMS_HTML: Runoff::format="<TD align=center>%Q<TD>Type%F"; break;
  case FMS_CSV: Runoff::format="%Q,Type,%F"; break;
  default: assert(0);
  }
  Runoff::print_header(out_file);
}

bool CRunoffListView::CanPaste(const Clip& clp) const
{
  unsigned i,n=clp.ftr_count();
  for (i=0; i<n; ++i)
    if (clp.type[i]!=FT_RUNOFF) return false;
  return true;
}

bool CRunoffListView::prelist()
{
  CPromDoc *doc=GetDocument();
  return CRunoffList_dlg(doc->data,runoff_filter,doc->is_drainage,GetParent()).DoModal()==IDOK;
}

void CRunoffListView::OnInitialUpdate()
{
  BeginWaitCursor();

  CFeatureListView::OnInitialUpdate();

  if (is_initial) {
    runoff_filter=init_rf;
    is_initial=false;
  }

  Divide_tree& tree=GetDocument()->tree;
  const Database& data=GetDocument()->data;
  Domain::Index n=tree.n_runoff();
  CListCtrl& listCtrl=GetListCtrl();
  listCtrl.SetItemCount(n);

  int nitem=0;
  for (Domain::Index i=0; i<n; ++i)
    if (tree.runoff(i).location && runoff_filter.test(tree.runoff(i),data)) {
      AddItem(tree.runoff(i),FT_RUNOFF);
      sprintf(buf,"%d",tree.runoff(i).interior_quads);
      listCtrl.SetItemText(nitem,IQ_COL,buf);
      ++nitem;
    }

  setItemIndicators();

  EndWaitCursor();
}

void CRunoffListView::SetViewTitle()
{
  CPromDoc *document=GetDocument();
  sprintf(buf,"%s%s Runoffs",
	  document->dt_name.IsEmpty()?"":document->dt_name+" - ",
	  document->is_drainage?"Streambed":"");
  GetParent()->SetWindowText(buf);
  GetParent()->SetIcon(AfxGetApp()->
    LoadIcon(document->is_drainage?IDI_SBRO:IDI_RUNOFF),TRUE);
}

static int compare_dir;

static int CALLBACK compare_IQ(LPARAM p1, LPARAM p2, LPARAM)
{
  const Runoff &ro1=*(Runoff *)p1,&ro2=*(Runoff *)p2;
  return ro1.interior_quads-ro2.interior_quads;
}

void CRunoffListView::OnHeaderClick(NMHDR *pNMHDR, LRESULT *pResult)
{
  NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
  CListCtrl& listCtrl=GetListCtrl();

  if (pNMListView->iSubItem==IQ_COL) {
    // sort by IQs
    compare_dir=GetKeyState(VK_SHIFT)<0 ? -1 : 1;
    listCtrl.SortItems(compare_IQ,(LPARAM)this);
    *pResult = 0;
  }
  else {
    CFeatureListView::OnHeaderClick(pNMHDR,pResult);
  }
}
