// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// Crop_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "Crop_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCrop_dlg dialog

CCrop_dlg::CCrop_dlg(const char *t, Rectangl& rect, UINT rid_tpl, CWnd* pParent /*=NULL*/)
  : CDialog(rid_tpl, pParent),title(t),limits(rect)
{
  //{{AFX_DATA_INIT(CCrop_dlg)
  m_limit_e = _T("");
  m_limit_n = _T("");
  m_limit_s = _T("");
  m_limit_w = _T("");
  //}}AFX_DATA_INIT
  check_interval=true;
}

void CCrop_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CCrop_dlg)
  DDX_Control(pDX, IDC_LIMIT_W, m_w_ctl);
  DDX_Control(pDX, IDC_LIMIT_S, m_s_ctl);
  DDX_Control(pDX, IDC_LIMIT_N, m_n_ctl);
  DDX_Control(pDX, IDC_LIMIT_E, m_e_ctl);
  DDX_Text(pDX, IDC_LIMIT_E, m_limit_e);
  DDX_Text(pDX, IDC_LIMIT_N, m_limit_n);
  DDX_Text(pDX, IDC_LIMIT_S, m_limit_s);
  DDX_Text(pDX, IDC_LIMIT_W, m_limit_w);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCrop_dlg, CDialog)
  //{{AFX_MSG_MAP(CCrop_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCrop_dlg message handlers

BOOL CCrop_dlg::OnInitDialog() 
{
  CPromDoc::set_rect(limits,m_limit_n,m_limit_s,m_limit_e,m_limit_w);

  CDialog::OnInitDialog();

  SetWindowText(title);

  return TRUE;  // return TRUE unless you set the focus to a control
}

static int gcd(int a, int b)
{
  a=abs(a);
  b=abs(b);
  int r;
  while (b!=0) {
    r=a%b;
    a=b;
    b=r;
  }
  return a;
}

void CCrop_dlg::check_ok()
{
  Rectangl new_limits;
  if (!CPromDoc::get_rect(new_limits,this,m_limit_n,m_limit_s,m_limit_e,m_limit_w,
			  m_n_ctl,m_s_ctl,m_e_ctl,m_w_ctl))
    return;

  new_limits&=limits;
  if (check_interval &&
      (new_limits.limit[0].high%GridPoint::quad_int_x ||
       new_limits.limit[0].low%GridPoint::quad_int_x ||
       new_limits.limit[1].high%GridPoint::quad_int_y ||
       new_limits.limit[1].low%GridPoint::quad_int_y)) {
    char quad_x[16],quad_y[16];
    GridPoint::Coord
      new_quad_x=gcd(GridPoint::quad_int_x,new_limits.limit[0].high),
      new_quad_y=gcd(GridPoint::quad_int_y,new_limits.limit[1].high);
    new_quad_x=gcd(new_quad_x,new_limits.limit[0].low);
    new_quad_y=gcd(new_quad_y,new_limits.limit[1].low);
    GridPoint::sprintx(quad_x,new_quad_x);
    GridPoint::sprinty(quad_y,new_quad_y);
    sprintf(CPromDoc::buf,"New limits are not multiples of the quad interval.\n"
	    "Reset quad interval to:\n%s=%s, %s=%s?",
	    GridPoint::coord1_label,quad_x,GridPoint::coord2_label,quad_y);
    switch (MessageBox(CPromDoc::buf,"Winprom Crop",
		       MB_YESNOCANCEL|MB_DEFBUTTON2|MB_ICONWARNING)) {
    case IDYES:
      GridPoint::quad_int_x=new_quad_x;
      GridPoint::quad_int_y=new_quad_y;
    case IDNO:
      break;
    case IDCANCEL:
      return;
    }
  }

  limits=new_limits;

  CDialog::OnOK();
}

void CCrop_dlg::OnOK()
{
  if (!UpdateData(TRUE)) return;
  check_ok();
}
