// PromFrame.cpp : implementation of the CPromFrame class
//

#include "stdafx.h"
#include "winprom.h"

#include "GraphView.h"
#include "GraphFrame.h"
#include "PromFrame.h"
#include "PromDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPromFrame

IMPLEMENT_DYNAMIC(CPromFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CPromFrame, CMDIFrameWnd)
  //{{AFX_MSG_MAP(CPromFrame)
  ON_WM_CREATE()
  ON_COMMAND(ID_VIEW_TB_ALL, OnViewTbAll)
  ON_COMMAND(ID_VIEW_TB_DLPR_MAP, OnViewTbDLPRmap)
  ON_COMMAND(ID_VIEW_TB_DLPR_TREE, OnViewTbDLPRtree)
  ON_COMMAND(ID_VIEW_TB_ELEV_MAP, OnViewTbElevMap)
  ON_COMMAND(ID_VIEW_TB_NONE, OnViewTbNone)
  ON_UPDATE_COMMAND_UI(ID_VIEW_TB_ELEV_MAP, OnUpdateViewTbElevMap)
  ON_UPDATE_COMMAND_UI(ID_VIEW_TB_DLPR_TREE, OnUpdateViewTbDLPRtree)
  ON_UPDATE_COMMAND_UI(ID_VIEW_TB_DLPR_MAP, OnUpdateViewTbDLPRmap)
  ON_WM_MOUSEMOVE()
  ON_COMMAND(ID_VIEW_TB_DATABASE, OnViewTbDatabase)
  ON_UPDATE_COMMAND_UI(ID_VIEW_TB_DATABASE, OnUpdateViewTbDatabase)
  //}}AFX_MSG_MAP
  ON_WM_NCMOUSEMOVE()
  // Global help commands
END_MESSAGE_MAP()

static UINT indicators[] =
{
  ID_SEPARATOR,           // status line indicator
  ID_INDICATOR_HEIGHT,
  ID_INDICATOR_WIDTH,
  ID_INDICATOR_LOC,
  ID_INDICATOR_ELEV,
  ID_INDICATOR_CAPS,
  //ID_INDICATOR_NUM,
  //ID_INDICATOR_SCRL,
};


/////////////////////////////////////////////////////////////////////////////
// CPromFrame construction/destruction

CPromFrame::CPromFrame()
{
}

CPromFrame::~CPromFrame()
{
}

bool CPromFrame::CreateFileBar(CToolBar& tb, UINT resourceID, const char *title)
{
  if (!tb.Create(this) || !tb.LoadToolBar(resourceID)) {
    TRACE0("Failed to create toolbar\n");
    return false;  // fail to create
  }

  tb.SetBarStyle(tb.GetBarStyle() |
    CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

  if (title) {
    tb.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    ShowControlBar(&tb,FALSE,FALSE);
    DockControlBar(&tb);
    tb.SetWindowText(title);
  }

  return true;
}

int CPromFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  if (!statusBar.Create(this) ||
      !statusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT))) {
    TRACE0("Failed to create status bar\n");
    return -1;      // fail to create
  }

  statusBar.SetPaneText(SB_PANE_LOC,"");
  statusBar.SetPaneText(SB_PANE_ELEV,"");

  if (!CreateFileBar(mainToolBar,IDR_GRAPHTYPE,NULL)) return -1;
  if (!CreateFileBar(em_fileBar,IDR_FILE_ELEV_MAP,"Elevation Map")) return -1;
  if (!CreateFileBar(dm_fileBar,IDR_FILE_DLPR_MAP,"Domain Map")) return -1;
  if (!CreateFileBar(dt_fileBar,IDR_FILE_DLPR_TREE,"Spruce Tree")) return -1;
  if (!CreateFileBar(db_fileBar,IDR_FILE_DATABASE,"Database")) return -1;

  return 0;
}

BOOL CPromFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  return CMDIFrameWnd::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// CPromFrame diagnostics

#ifdef _DEBUG
void CPromFrame::AssertValid() const
{
  CMDIFrameWnd::AssertValid();
}

void CPromFrame::Dump(CDumpContext& dc) const
{
  CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CPromFrame message handlers

void CPromFrame::TB_toggle(CToolBar& whichbar)
{
  bool displayed=!(whichbar.GetStyle()&WS_VISIBLE);
  ShowControlBar(&whichbar,displayed,FALSE);
}

void CPromFrame::TB_displayAll(bool show)
{
  ShowControlBar(&em_fileBar,show,FALSE);
  ShowControlBar(&dm_fileBar,show,FALSE);
  ShowControlBar(&dt_fileBar,show,FALSE);
  ShowControlBar(&db_fileBar,show,FALSE);
}

void CPromFrame::OnViewTbAll() 
{
  TB_displayAll(true);
}

void CPromFrame::OnViewTbDLPRmap() 
{
  TB_toggle(dm_fileBar);
}

void CPromFrame::OnViewTbDLPRtree() 
{
  TB_toggle(dt_fileBar);
}

void CPromFrame::OnViewTbElevMap() 
{
  TB_toggle(em_fileBar);
}

void CPromFrame::OnViewTbDatabase() 
{
  TB_toggle(db_fileBar);
}

void CPromFrame::OnViewTbNone() 
{
  TB_displayAll(false);
}

void CPromFrame::OnUpdateViewTbElevMap(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck((em_fileBar.GetStyle() & WS_VISIBLE) != 0);
}

void CPromFrame::OnUpdateViewTbDLPRtree(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck((dt_fileBar.GetStyle() & WS_VISIBLE) != 0);
  CGraphFrame *frame=(CGraphFrame *)GetActiveFrame();
  pCmdUI->SetText(frame && ((CGraphView *)(frame->GetActiveView()))->GetDocument()->is_drainage?
		  "Drainage &tree":"Divide &tree");
}

void CPromFrame::OnUpdateViewTbDLPRmap(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck((dm_fileBar.GetStyle() & WS_VISIBLE) != 0);
}

void CPromFrame::OnUpdateViewTbDatabase(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck((db_fileBar.GetStyle() & WS_VISIBLE) != 0);
}

void CPromFrame::OnMouseMove(UINT, CPoint)
{
  statusBar.SetPaneText(SB_PANE_LOC,"");
  statusBar.SetPaneText(SB_PANE_ELEV,"");
}

void CPromFrame::OnNcMouseMove(UINT nFlags, CPoint point)
{
  OnMouseMove(nFlags, point);
}
