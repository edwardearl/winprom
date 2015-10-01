// TreeFrame.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "TreeFrame.h"
#include "LPRtreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTreeFrame

IMPLEMENT_DYNCREATE(CTreeFrame, CFrameWnd)

CTreeFrame::CTreeFrame()
{
}

CTreeFrame::~CTreeFrame()
{
}

BEGIN_MESSAGE_MAP(CTreeFrame, CFrameWnd)
  //{{AFX_MSG_MAP(CTreeFrame)
  ON_WM_CREATE()
  ON_COMMAND(ID_VIEW_TB_DLPR_TREE, OnViewTbDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_VIEW_TB_DLPR_TREE, OnUpdateViewTbDLPRtree)
  //}}AFX_MSG_MAP
  ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
END_MESSAGE_MAP()

static UINT indicators[] =
{
  ID_SEPARATOR,           // status line indicator
  ID_INDICATOR_CAPS,
};


/////////////////////////////////////////////////////////////////////////////
// CTreeFrame message handlers

BOOL CTreeFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
  cs.cx=cs.cy=432;

  return CFrameWnd::PreCreateWindow(cs);
}

int CTreeFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  if (!treeTools.Create(this) || !treeTools.LoadToolBar(IDR_LPR_TREE)) {
    TRACE0("Failed to create toolbar\n");
    return -1;  // fail to create
  }
  treeTools.SetBarStyle(treeTools.GetBarStyle() |
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

  if (!treeStatus.Create(this) ||
      !treeStatus.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT))) {
    TRACE0("Failed to create status bar\n");
    return -1;  // fail to create
  }
	
  return 0;
}

void CTreeFrame::OnUpdateFrameTitle(BOOL)
{
  ((CLPRtreeView *)GetActiveView())->SetViewTitle();
}

void CTreeFrame::OnViewTbDLPRtree()
{
  bool displayed=!(fileBar.GetStyle()&WS_VISIBLE);
  ShowControlBar(&fileBar,displayed,FALSE);
}

void CTreeFrame::OnUpdateViewTbDLPRtree(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck((fileBar.GetStyle() & WS_VISIBLE) != 0);
}
