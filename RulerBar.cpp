// Ruler.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "RulerBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TEXT_MARGIN 2
#define TICK_LENGTH 6

int text_height;


/////////////////////////////////////////////////////////////////////////////
// CRulerBar

CRulerBar::CRulerBar()
{
  tickWindow=0;
  tickInterval=50;
  valueBase=0;
  minor_tick=2;
}

CRulerBar::~CRulerBar()
{
}


BEGIN_MESSAGE_MAP(CRulerBar, CStatusBar)
  //{{AFX_MSG_MAP(CRulerBar)
  ON_WM_PAINT()
  ON_WM_CREATE()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRulerBar message handlers

int CRulerBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CStatusBar::OnCreate(lpCreateStruct) == -1)
    return -1;

  GetStatusBarCtrl().SetMinHeight(24);

  return 0;
}

int CRulerBar::tickMarkLabel(CDC *pDC, double value)
{
  int tick_pos=GetTickPos(value);
  if (tick_pos>=0) tickMarkLabel(pDC,value,tick_pos);
  return tick_pos;
}

void CRulerBar::tickMarkLabel(CDC *pDC, double value, int tick_pos)
{
  static char buf[64];
  sprintf(buf,"%g",value);
  pDC->TextOut(tick_pos-pDC->GetTextExtent(buf).cx/2,TEXT_MARGIN,buf);
  tickMark(pDC,tick_pos);
}

void CRulerBar::tickMark(CDC *pDC, int tick_pos)
{
  pDC->MoveTo(tick_pos,2*TEXT_MARGIN+text_height);
  pDC->LineTo(tick_pos,2*TEXT_MARGIN+text_height+TICK_LENGTH);
}

void CRulerBar::OnPaint() 
{
  CPen pen(PS_SOLID,0,GetSysColor(COLOR_BTNTEXT));
  LOGFONT logFont;
  int tickwindow1,tickwindow2;
  unsigned iminor;
  double value1,value2,value,prev_value=-HUGE_VAL;
  CPaintDC dc(this); // device context for painting
  dc.SelectObject(&pen);
  dc.SetBkMode(TRANSPARENT);
  dc.SetTextColor(GetSysColor(COLOR_BTNTEXT));
  dc.SelectStockObject(DEFAULT_GUI_FONT);
  dc.GetCurrentFont()->GetLogFont(&logFont);
  text_height=abs(logFont.lfHeight);

  CRect clientRect;
  GetClientRect(&clientRect);

  if (tickInterval>0) {
    for (tickwindow1=GetTickBase(); tickwindow1<clientRect.right;
         tickwindow1+=tickInterval) {
      if (!hasValue(tickwindow1)) continue;
      value1=GetValue(tickwindow1);
      if (tickWindow==0) {
	tickMarkLabel(&dc,value1,tickwindow1);
      }
      else {
	tickwindow2=tickwindow1+tickWindow;
	value2=hasValue(tickwindow2) ? GetValue(tickwindow2) : highValue();
	if (value1*value2<=0) value=0;
	else if (value1>0) {
	  double scale=1.0;
	  while (value2>=10.0) {
	    value1*=0.1;
	    value2*=0.1;
	    scale*=10.0;
	  }
	  while (value2<1.0) {
	    value1*=10.0;
	    value2*=10.0;
	    scale*=0.1;
	  }
	  while (1) {
	    value=(int)value1+(value1==(int)value1?0:1);
	    if (value<=value2) break;
	    value1*=10.0;
	    value2*=10.0;
	    scale*=0.1;
	  }
	  value*=scale;
	}
	if (prev_value!=-HUGE_VAL) {
	  for (iminor=1; iminor<minor_tick; ++iminor) {
	    tickwindow2=GetTickPos((prev_value*iminor+value*(minor_tick-iminor))/minor_tick);
	    if (tickwindow2>=0) tickMark(&dc,tickwindow2);
	  }
	}
	prev_value=value;
	tickwindow1=tickMarkLabel(&dc,value);
      }
    }
  }
  else {
    value1=GetValue(clientRect.left);
    value1=((int)((value1-valueBase)/valueInterval))*valueInterval;
    value2=GetValue(clientRect.right);
    for (value=value1; value<=value2; value+=valueInterval) {
      tickMarkLabel(&dc,value);
    }
  }
  dc.DrawEdge(clientRect,EDGE_SUNKEN,BF_RECT);
}
