// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// DataFrame.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "DataFrame.h"
#include "DataListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDataFrame

IMPLEMENT_DYNCREATE(CDataFrame, CFrameWnd)

CDataFrame::CDataFrame()
{
}

CDataFrame::~CDataFrame()
{
}

BEGIN_MESSAGE_MAP(CDataFrame, CFrameWnd)
  //{{AFX_MSG_MAP(CDataFrame)
  ON_WM_CREATE()
  ON_COMMAND(ID_VIEW_TB_DATABASE, OnViewTbDatabase)
  ON_UPDATE_COMMAND_UI(ID_VIEW_TB_DATABASE, OnUpdateViewTbDatabase)
  //}}AFX_MSG_MAP
  ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
END_MESSAGE_MAP()

unsigned CDataFrame::client_width;

static UINT indicators[] =
{
  ID_SEPARATOR,           // status line indicator
  ID_INDICATOR_NITEM,
  //ID_INDICATOR_NSELECT,
  ID_INDICATOR_CAPS,
};


/////////////////////////////////////////////////////////////////////////////
// CDataFrame message handlers

BOOL CDataFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
  cs.cx=192+client_width;
  if (cs.cx<464) cs.cx=464;
  cs.cy=400;

  return CFrameWnd::PreCreateWindow(cs);
}

void CDataFrame::OnUpdateFrameTitle(BOOL)
{
  ((CDataListView *)GetActiveView())->SetViewTitle();
}

int CDataFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  if (!dataTools.Create(this) || !dataTools.LoadToolBar(IDR_DATA_LIST)) {
    TRACE0("Failed to create toolbar\n");
    return -1;  // fail to create
  }
  dataTools.SetBarStyle(dataTools.GetBarStyle() |
    CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

  if (!fileBar.Create(this) || !fileBar.LoadToolBar(IDR_FILE_DATABASE)) {
    TRACE0("Failed to create file bar\n");
    return false;  // fail to create
  }
  fileBar.SetBarStyle(fileBar.GetBarStyle() |
    CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
  fileBar.EnableDocking(CBRS_ALIGN_ANY);
  EnableDocking(CBRS_ALIGN_ANY);
  fileBar.SetWindowText("Database");
  ShowControlBar(&fileBar,FALSE,FALSE);
  DockControlBar(&fileBar);

  if (!dataStatus.Create(this) ||
      !dataStatus.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT))) {
    TRACE0("Failed to create status bar\n");
    return -1;  // fail to create
  }

  return 0;
}

void CDataFrame::OnViewTbDatabase()
{
  bool displayed=!(fileBar.GetStyle()&WS_VISIBLE);
  ShowControlBar(&fileBar,displayed,FALSE);
}

void CDataFrame::OnUpdateViewTbDatabase(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck((fileBar.GetStyle() & WS_VISIBLE) != 0);
}
