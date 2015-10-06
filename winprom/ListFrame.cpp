// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// ListFrame.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "ListFrame.h"
#include "FeatureListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CListFrame

IMPLEMENT_DYNCREATE(CListFrame, CFrameWnd)

CListFrame::CListFrame()
{
}

CListFrame::~CListFrame()
{
}

BEGIN_MESSAGE_MAP(CListFrame, CFrameWnd)
  //{{AFX_MSG_MAP(CListFrame)
  ON_WM_CREATE()
  ON_COMMAND(ID_VIEW_TB_DLPR_TREE, OnViewTbDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_VIEW_TB_DLPR_TREE, OnUpdateViewTbDLPRtree)
  //}}AFX_MSG_MAP
  ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
END_MESSAGE_MAP()

int CListFrame::client_width;

static UINT indicators[] =
{
  ID_SEPARATOR,           // status line indicator
  ID_INDICATOR_NITEM,
  //ID_INDICATOR_NSELECT,
  ID_INDICATOR_CAPS,
};


/////////////////////////////////////////////////////////////////////////////
// CListFrame message handlers

BOOL CListFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  cs.cx=client_width+472;
  if (cs.cx<572) cs.cx=572;
  cs.cy=400;

  return CFrameWnd::PreCreateWindow(cs);
}

int CListFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  if (!listTools.Create(this) || !listTools.LoadToolBar(IDR_FEATURE_LIST)) {
    TRACE0("Failed to create toolbar\n");
    return -1;  // fail to create
  }
  listTools.SetBarStyle(listTools.GetBarStyle() |
    CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

  if (!fileBar.Create(this) || !fileBar.LoadToolBar(IDR_FILE_DLPR_TREE)) {
    TRACE0("Failed to create file bar\n");
    return false;  // fail to create
  }
  fileBar.SetBarStyle(fileBar.GetBarStyle() |
    CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
  fileBar.EnableDocking(CBRS_ALIGN_ANY);
  EnableDocking(CBRS_ALIGN_ANY);
  ShowControlBar(&fileBar,FALSE,FALSE);
  DockControlBar(&fileBar);
  fileBar.SetWindowText("Spruce Tree");

  if (!listStatus.Create(this) ||
      !listStatus.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT))) {
    TRACE0("Failed to create status bar\n");
    return -1;  // fail to create
  }
  //DWORD w=listStatus.GetWindowContextHelpId();
  //listStatus.SetWindowContextHelpId(IDW_LIST_STATUS_BAR);
  //w=listStatus.GetWindowContextHelpId();

  return 0;
}

void CListFrame::OnUpdateFrameTitle(BOOL)
{
  ((CFeatureListView *)GetActiveView())->SetViewTitle();
}

void CListFrame::OnViewTbDLPRtree()
{
  bool displayed=!(fileBar.GetStyle()&WS_VISIBLE);
  ShowControlBar(&fileBar,displayed,FALSE);
}

void CListFrame::OnUpdateViewTbDLPRtree(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck((fileBar.GetStyle() & WS_VISIBLE) != 0);
}
