#include "stdafx.h"
#include "ColorBox.h"


BEGIN_MESSAGE_MAP(CDrawBox,CStatic)
  ON_WM_PAINT()
END_MESSAGE_MAP()

//IMPLEMENT_DYNCREATE(CDrawBox,CStatic)

BOOL CDrawBox::Create(DWORD dwStyle, const RECT& rect, CWnd *parent, UINT nID)
{
  return CStatic::Create(NULL,dwStyle,rect,parent,nID);
}

void CDrawBox::OnPaint()
{
  CPaintDC dc(this);
  OnDraw(&dc);
}


//IMPLEMENT_DYNCREATE(CColorBox,CDrawBox)

void CColorBox::set_color(COLORREF cr)
{
  color=cr;
  Invalidate();
}

void CColorBox::OnDraw(CDC *pDC)
{
  RECT rc;
  GetClientRect(&rc);
  pDC->FillSolidRect(&rc,color);
}
