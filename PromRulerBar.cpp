// PromRulerBar.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "PromRulerBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPromRulerBar

CPromRulerBar::CPromRulerBar()
{
  tickInterval=48;
  tickWindow=48;
}

CPromRulerBar::~CPromRulerBar()
{
}


BEGIN_MESSAGE_MAP(CPromRulerBar, CRulerBar)
  //{{AFX_MSG_MAP(CPromRulerBar)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

double CPromRulerBar::GetValue(int a) const
{
  CPoint pt(a,0);
  MapWindowPoints(view,&pt,1);
  return view->PromFromCoord(pt.x+view->GetScrollPosition().x);
}

int CPromRulerBar::GetTickPos(double a)
{
  int pc=view->PromCoord((Elevation)(a+0.5));
  if (pc<0) return -1;
  CPoint pt(pc,0);
  view->MapWindowPoints(this,&pt,1);
  return pt.x-view->GetScrollPosition().x;
}

/////////////////////////////////////////////////////////////////////////////
// CPromRulerBar message handlers
