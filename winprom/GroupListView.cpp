// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// GroupListView.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "GroupListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGroupListView

IMPLEMENT_DYNCREATE(CGroupListView, CFeatureListView)

CGroupListView::CGroupListView()
{
}

CGroupListView::~CGroupListView()
{
}


BEGIN_MESSAGE_MAP(CGroupListView, CFeatureListView)
  //{{AFX_MSG_MAP(CGroupListView)
  ON_UPDATE_COMMAND_UI(ID_LIST_LOAD, OnUpdateListLoad)
  ON_UPDATE_COMMAND_UI(ID_LIST_APPEND, OnUpdateListLoad)
  //}}AFX_MSG_MAP
  ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnUnlistItem)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGroupListView diagnostics

#ifdef _DEBUG
void CGroupListView::AssertValid() const
{
  CFeatureListView::AssertValid();
}

void CGroupListView::Dump(CDumpContext& dc) const
{
  CFeatureListView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CGroupListView message handlers

void CGroupListView::OnUpdateListLoad(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(FALSE);
}

void CGroupListView::OnUnlistItem(NMHDR *pNMHDR, LRESULT *pResult)
{
  int item=((NM_LISTVIEW *)pNMHDR)->iItem;
  if (begins_group(item)) {
    CListCtrl& listCtrl=GetListCtrl();
    while (item+1<listCtrl.GetItemCount() && !begins_group(item+1))
      listCtrl.DeleteItem(item+1);
  }

  *pResult = 0;
}
