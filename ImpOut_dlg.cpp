// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// ImpOut_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "MapView.h"
#include "ImpOut_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static char buf[64];


/////////////////////////////////////////////////////////////////////////////
// CImpOut_dlg dialog

CImpOut_dlg::CImpOut_dlg(const GridPoint& gp, double ne, double fac,
			 const double *mv, const GridPoint *ml,
			 const double (*dist)[8], const double (*ar)[2],
			 const double (*ai)[2], unsigned n, double di,
			 CMapView *v, CWnd *pParent /*=NULL*/)
  : CDialog(CImpOut_dlg::IDD, pParent),location(gp),nearedge(ne),conv_fac(fac),
    max_val(mv),max_loc(ml),int_dist(dist),avg_y_ring(ar),avg_y_inf(ai),
    ndist(n),dist_intvl(di),view(v)
{
  //{{AFX_DATA_INIT(CImpOut_dlg)
  m_spdir = 0.0f;
  m_sp_dist = 0.0f;
  m_sploc = _T("");
  m_spval = 0;
  m_intval = 0;
  m_location = _T("");
  m_nearedge = 0.0f;
  //}}AFX_DATA_INIT
}


void CImpOut_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CImpOut_dlg)
  DDX_Control(pDX, IDC_INTDIST, m_dist);
  DDX_Control(pDX, IDC_TYPE, m_type);
  DDX_Text(pDX, IDC_SPDIR, m_spdir);
  DDX_Text(pDX, IDC_SPDIST, m_sp_dist);
  DDX_Text(pDX, IDC_SPLOC, m_sploc);
  DDX_Text(pDX, IDC_SPVAL, m_spval);
  DDX_Text(pDX, IDC_INTVAL, m_intval);
  DDX_Text(pDX, IDC_LOCATION, m_location);
  DDX_Text(pDX, IDC_NEAREDGE, m_nearedge);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CImpOut_dlg, CDialog)
  //{{AFX_MSG_MAP(CImpOut_dlg)
  ON_NOTIFY(TCN_SELCHANGE, IDC_TYPE, OnSelchangeType)
  ON_BN_CLICKED(IDC_TOGGLE, OnToggle)
  ON_BN_CLICKED(IDC_CENTER, OnCenter)
  ON_BN_CLICKED(IDC_CENTER_SP, OnCenterSP)
  ON_BN_CLICKED(IDC_TOGGLE_SP, OnToggleSP)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CImpOut_dlg message handlers

void CImpOut_dlg::set_type()
{
  unsigned type=m_type.GetCurSel();
  double y,r,s,int_est;
  m_intval=int_dist[ndist][type];
  while (m_dist.GetCount()>0)
    m_dist.DeleteString(0);
  sprintf(buf," 0.00     0.0%%           %5.0f",avg_y_inf[0][type%2]);
  m_dist.AddString(buf);
  for (unsigned i=0; i<ndist; ++i) {
    y=avg_y_inf[i+1][type%2];
    r=(i+1)*dist_intvl*conv_fac;
    s=y/r;
    int_est=int_dist[i][type];
    if (type&0x02) int_est+=0.3183098861837907*(2*y*atan(s)-r*log(1+s*s)); // angle
    else int_est+=y-r*log(1+s); // rational
    sprintf(buf,"%5.2f    %4.1f%%   %5.0f   %5.0f   %5.0f",
	    (i+1)*dist_intvl,100*int_dist[i][type]/int_dist[ndist][type],
	    avg_y_ring[i][type%2],avg_y_inf[i+1][type%2],int_est);
    m_dist.AddString(buf);
  }
  sprintf(buf,"infty   100.0%%   %5.0f",avg_y_ring[ndist][type%2]);
  m_dist.AddString(buf);
  m_spval=max_val[type&0x03];
  m_sp_dist=max_loc[type&0x03]-location;
  m_spdir=location.direction(max_loc[type&0x03])*57.29577951;
  if (m_spdir<0) m_spdir+=360;
  max_loc[type&0x03].sprint(buf);
  m_sploc=buf;
  UpdateData(FALSE);
}

void CImpOut_dlg::OnSelchangeType(NMHDR *pNMHDR, LRESULT *pResult)
{
  set_type();
  *pResult = 0;
}

BOOL CImpOut_dlg::OnInitDialog()
{
  location.sprint(buf);
  m_location=buf;
  m_nearedge=nearedge;

  CDialog::OnInitDialog();

  m_type.InsertItem(0,"Rational AB 1");
  m_type.InsertItem(1,"Rational EA 1");
  m_type.InsertItem(2,"Angle AB 1");
  m_type.InsertItem(3,"Angle EA 1");
  m_type.InsertItem(4,"Rational AB 2");
  m_type.InsertItem(5,"Rational EA 2");
  m_type.InsertItem(6,"Angle AB 2");
  m_type.InsertItem(7,"Angle EA 2");
  m_type.SetCurSel(0);
  set_type();

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}

void CImpOut_dlg::OnToggle()
{
  view->grid_mark[location]=!view->grid_mark[location];
  view->InvalidatePoint(location);
}

void CImpOut_dlg::OnCenter()
{
  view->setGraphScroll(view->gp2point(location));
}

void CImpOut_dlg::OnCenterSP()
{
  view->setGraphScroll(view->gp2point(max_loc[m_type.GetCurSel()]));
}

void CImpOut_dlg::OnToggleSP()
{
  const GridPoint& loc=max_loc[m_type.GetCurSel()&0x03];
  if (loc) {
    view->grid_mark[loc]=!view->grid_mark[loc];
    view->InvalidatePoint(loc);
  }
}
