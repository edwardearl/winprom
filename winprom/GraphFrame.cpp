// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// GraphFrame.cpp : implementation of the CGraphFrame class
//

#include "stdafx.h"
#include "winprom.h"

#include "GraphFrame.h"
#include "GraphView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGraphFrame

IMPLEMENT_DYNCREATE(CGraphFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CGraphFrame, CMDIChildWnd)
  //{{AFX_MSG_MAP(CGraphFrame)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGraphFrame construction/destruction

CGraphFrame::CGraphFrame()
{
}

CGraphFrame::~CGraphFrame()
{
}

BOOL CGraphFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  if (!CMDIChildWnd::PreCreateWindow(cs)) return FALSE;
  cs.style|=WS_MAXIMIZE;
  return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CGraphFrame diagnostics

#ifdef _DEBUG
void CGraphFrame::AssertValid() const
{
  CMDIChildWnd::AssertValid();
}

void CGraphFrame::Dump(CDumpContext& dc) const
{
  CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

void CGraphFrame::ActivateFrame(int nCmdShow)
{
  nCmdShow = SW_SHOWMAXIMIZED;
  CMDIChildWnd::ActivateFrame(nCmdShow);
}


/////////////////////////////////////////////////////////////////////////////
// CGraphFrame message handlers

void CGraphFrame::OnUpdateFrameTitle(BOOL)
{
  ((CGraphView *)GetActiveView())->SetViewTitle();
}

// This handler prevents the ID_VIEW_STATUS_BAR message from affecting the ruler
BOOL CGraphFrame::OnCmdMsg(UINT nID, int nCode, void *pExtra, AFX_CMDHANDLERINFO *pHandlerInfo)
{
  return nID==ID_VIEW_STATUS_BAR ? FALSE :
    CMDIChildWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
