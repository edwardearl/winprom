// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// ProfileFrame.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "ProfileFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CProfileFrame

IMPLEMENT_DYNCREATE(CProfileFrame, CGraphFrame)

CProfileFrame::CProfileFrame()
{
}

CProfileFrame::~CProfileFrame()
{
}


BEGIN_MESSAGE_MAP(CProfileFrame, CGraphFrame)
  //{{AFX_MSG_MAP(CProfileFrame)
  ON_WM_CREATE()
  ON_COMMAND(ID_VIEW_HRULER, OnViewHruler)
  ON_UPDATE_COMMAND_UI(ID_VIEW_HRULER, OnUpdateViewHruler)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CProfileFrame message handlers

int CProfileFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CGraphFrame::OnCreate(lpCreateStruct) == -1)
    return -1;

  if (!prom_ruler.Create(this,WS_CHILD|WS_VISIBLE|CBRS_TOP))
    return -1;

  return 0;
}

void CProfileFrame::OnViewHruler()
{
  bool displayed=!(prom_ruler.GetStyle()&WS_VISIBLE);
  ShowControlBar(&prom_ruler,displayed,FALSE);
}

void CProfileFrame::OnUpdateViewHruler(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck((prom_ruler.GetStyle() & WS_VISIBLE) != 0);
}
