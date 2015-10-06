// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// CG: This file was added by the Splash Screen component.
// Splash.cpp : implementation file
//

#include "stdafx.h"  // e. g. stdafx.h
#include "resource.h"  // e.g. resource.h
#include "Splash.h"  // e.g. splash.h

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
//   Splash Screen class

CSplashWnd::CSplashWnd()
{
}

CSplashWnd::~CSplashWnd()
{
}

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
  //{{AFX_MSG_MAP(CSplashWnd)
  ON_WM_CREATE()
  ON_WM_PAINT()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

bool CSplashWnd::Show(CWnd *pParentWnd /*= NULL*/)
{
  if (!Create(pParentWnd)) return false;
  UpdateWindow();
  return true;
}

void CSplashWnd::Hide()
{
  // Destroy the window, and update the mainframe.
  DestroyWindow();
  AfxGetMainWnd()->UpdateWindow();
}

BOOL CSplashWnd::Create(CWnd *pParentWnd /*= NULL*/)
{
  if (!m_bitmap.LoadBitmap(IDB_SPLASH))
    return FALSE;

  BITMAP bm;
  m_bitmap.GetBitmap(&bm);

  return CreateEx(0,
    AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
    NULL, WS_POPUP | WS_VISIBLE, 0, 0, bm.bmWidth, bm.bmHeight, pParentWnd->GetSafeHwnd(), NULL);
}

int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  // Center the window.
  CenterWindow();

  return 0;
}

void CSplashWnd::OnPaint()
{
  CPaintDC dc(this);

  CDC dcImage;
  if (!dcImage.CreateCompatibleDC(&dc))
    return;

  BITMAP bm;
  m_bitmap.GetBitmap(&bm);

  // Paint the image.
  CBitmap *pOldBitmap=dcImage.SelectObject(&m_bitmap);
  dc.BitBlt(0,0,bm.bmWidth,bm.bmHeight,&dcImage,0,0,SRCCOPY);
  dcImage.SelectObject(pOldBitmap);
}
