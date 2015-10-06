// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// GenericListView.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "GenericListView.h"
#include "ListFrame.h"
#include "FeatureTypeFilter_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGenericListView

IMPLEMENT_DYNCREATE(CGenericListView, CFeatureListView)

CGenericListView::CGenericListView()
{
}

CGenericListView::~CGenericListView()
{
}

bool CGenericListView::is_initial;
FeatureTypeFilter CGenericListView::init_ff;

BEGIN_MESSAGE_MAP(CGenericListView, CFeatureListView)
  //{{AFX_MSG_MAP(CGenericListView)
    // NOTE - the ClassWizard will add and remove mapping macros here.
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGenericListView diagnostics

#ifdef _DEBUG
void CGenericListView::AssertValid() const
{
  CFeatureListView::AssertValid();
}

void CGenericListView::Dump(CDumpContext& dc) const
{
  CFeatureListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGenericListView message handlers

void CGenericListView::init_list(CPromDoc *document)
{
  FeatureTypeFilter temp_ff=init_ff;
  temp_ff.rec_filter.align(document->data);
  is_initial=CFeatureTypeFilter_dlg(temp_ff).DoModal()==IDOK;
  if (!is_initial) return;

  init_ff=temp_ff;
  CListFrame::client_width=0;
  CSingleDocTemplate& GenericListViewTpl=CPromApp::getApp()->GenericListViewTpl;
  CListFrame *newlist=(CListFrame *)GenericListViewTpl.CreateNewFrame(document,NULL);
  GenericListViewTpl.InitialUpdateFrame(newlist,document);
}

void CGenericListView::add_item(Feature& f, FT_index ft)
{
  if (!f.location || !filter.test(f,ft,GetDocument()->data)) return;
  AddItem(f,ft);
}

void CGenericListView::OnInitialUpdate()
{
  CFeatureListView::OnInitialUpdate();

  if (is_initial) {
    filter=init_ff;
    is_initial=false;
  }

  Divide_tree& tree=GetDocument()->tree;
  Domain::Index i,n=tree.n_dom();
  for (i=0; i<n; ++i) {
    add_item(tree[i].peak,FT_PEAK);
    add_item(tree[i].saddle,FT_SADDLE);
  }
  n=tree.n_runoff();
  for (i=0; i<n; ++i)
    add_item(tree.runoff(i),FT_RUNOFF);
  n=tree.n_bsnsdl();
  for (i=0; i<n; ++i)
    add_item(tree.bsnsdl(i),FT_BASINSADL);

  setItemIndicators();
}

bool CGenericListView::prelist()
{
  FeatureTypeFilter temp_ff=filter;
  temp_ff.rec_filter.align(GetDocument()->data);
  CFeatureTypeFilter_dlg dlg(temp_ff,GetParent());
  if (dlg.DoModal()!=IDOK) return false;
  filter=temp_ff;
  return true;
}
