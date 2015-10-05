// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// ElevRect_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "ElevRect_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CElevRect_dlg dialog

CElevRect_dlg::CElevRect_dlg(Elev_intvl& elev, Rectangl& rect)
  : CDialog(CElevRect_dlg::IDD,AfxGetMainWnd()),
    target(elev),limits(rect)
{
  //{{AFX_DATA_INIT(CElevRect_dlg)
  m_elev_high = _T("");
  m_elev_low = _T("");
  m_limit_e = _T("");
  m_limit_n = _T("");
  m_limit_s = _T("");
  m_limit_w = _T("");
  //}}AFX_DATA_INIT
}

void CElevRect_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CElevRect_dlg)
  DDX_Control(pDX, IDC_LIMIT_W, m_limit_w_ctl);
  DDX_Control(pDX, IDC_LIMIT_S, m_limit_s_ctl);
  DDX_Control(pDX, IDC_LIMIT_N, m_limit_n_ctl);
  DDX_Control(pDX, IDC_LIMIT_E, m_limit_e_ctl);
  DDX_Control(pDX, IDC_ELEV_LOW, m_low_ctl);
  DDX_Control(pDX, IDC_ELEV_HIGH, m_high_ctl);
  DDX_Text(pDX, IDC_ELEV_HIGH, m_elev_high);
  DDX_Text(pDX, IDC_ELEV_LOW, m_elev_low);
  DDX_Text(pDX, IDC_LIMIT_E, m_limit_e);
  DDX_Text(pDX, IDC_LIMIT_N, m_limit_n);
  DDX_Text(pDX, IDC_LIMIT_S, m_limit_s);
  DDX_Text(pDX, IDC_LIMIT_W, m_limit_w);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CElevRect_dlg, CDialog)
  //{{AFX_MSG_MAP(CElevRect_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CElevRect_dlg message handlers

void CElevRect_dlg::OnOK() 
{
  char *end;
  long low,high;

  if (!UpdateData(TRUE)) return;

  if (m_elev_low.IsEmpty()) low=-elev_infty;
  else {
    low=strtol(m_elev_low,&end,10);
    if (*end!='\0') {
      MessageBox("Enter a number for the low elevation.",
		 "Winprom Entry Error",MB_OK|MB_ICONERROR);
      m_low_ctl.SetFocus();
      m_low_ctl.SetSel(0,-1);
      return;
    }
  }

  if (m_elev_high.IsEmpty()) high=elev_infty;
  else {
    high=strtol(m_elev_high,&end,10);
    if (*end!='\0') {
      MessageBox("Enter a number for the high elevation.",
		 "Winprom Entry Error",MB_OK|MB_ICONERROR);
      m_high_ctl.SetFocus();
      m_high_ctl.SetSel(0,-1);
      return;
    }
  }

  if (low>high) {
    MessageBox("High elevation must be >= low elevation.",
	       "Winprom Entry Error",MB_OK|MB_ICONERROR);
    return;
  }

  // Check location limits

  if (!CPromDoc::get_rect(limits,this,m_limit_n,m_limit_s,m_limit_e,m_limit_w,
			  m_limit_n_ctl,m_limit_s_ctl,m_limit_e_ctl,m_limit_w_ctl))
    return;

  if (low<-elev_infty) low=-elev_infty;
  if (high>elev_infty) high=elev_infty;
  target.set((Elevation)low,(Elevation)high);

  EndDialog(IDOK);
}

BOOL CElevRect_dlg::OnInitDialog()
{
  if (target.low==-elev_infty) m_elev_low.Empty();
  else m_elev_low.Format("%d",target.low);
  if (target.high==elev_infty) m_elev_high.Empty();
  else m_elev_high.Format("%d",target.high);

  CDialog::OnInitDialog();

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}
