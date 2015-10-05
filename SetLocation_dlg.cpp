// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// SetLocation_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "SetLocation_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSetLocation_dlg dialog

CSetLocation_dlg::CSetLocation_dlg(GridPoint& c, const Rectangl& b, const Location_fmt& fmt)
  : CDialog(CSetLocation_dlg::IDD,NULL),location(c),bound(b),format(fmt)
{
  //{{AFX_DATA_INIT(CSetLocation_dlg)
  m_lat = _T("");
  m_lon = _T("");
  m_unit = -1;
  //}}AFX_DATA_INIT
}

void CSetLocation_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CSetLocation_dlg)
  DDX_Control(pDX, IDC_LONGITUDE, m_lon_ctl);
  DDX_Control(pDX, IDC_LATITUDE, m_lat_ctl);
  DDX_Text(pDX, IDC_LATITUDE, m_lat);
  DDX_Text(pDX, IDC_LONGITUDE, m_lon);
  DDX_Radio(pDX, IDC_UNIT_DEGREES, m_unit);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetLocation_dlg, CDialog)
  //{{AFX_MSG_MAP(CSetLocation_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSetLocation_dlg message handlers

BOOL CSetLocation_dlg::OnInitDialog() 
{
  char (&buf)[sizeof CPromDoc::buf]=CPromDoc::buf;
  format.set();
  if (format.angle_f==AF_UTM) {
    int zone;
    double ew,ns;
    location.get_UTM(zone,ew,ns);
    sprintf(buf,"%+07.0lf",ns);
    m_lat=buf;
    sprintf(buf,"%2d %06.0lf",zone,ew);
    m_lon=buf;
  }
  else {
    sprint_angle(buf,location.lat(),'N','S');
    m_lat=buf;
    sprint_angle(buf,location.lon(),'E','W');
    m_lon=buf;
  }
  switch (format.angle_f) {
  case AF_DEGREE: case AF_DM: case AF_DMS:
    m_unit=0;
    break;
  case AF_RADIAN:
    m_unit=1;
    break;
  case AF_GRAD:
    m_unit=2;
    break;
  case AF_UTM:
    m_unit=3;
    break;
  default:
    assert(0);
  }

  CDialog::OnInitDialog();

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}

void CSetLocation_dlg::OnOK() 
{
  GridPoint::Coord lat,lon;
  int zone,ew,ns;

  if (!UpdateData(TRUE)) return;

  if (m_unit!=3) {
    switch (m_unit) {
    case 0:
      GridPoint_equat::angle_format=AF_DEGREE;
      break;
    case 1:
      GridPoint_equat::angle_format=AF_RADIAN;
      break;
    case 2:
      GridPoint_equat::angle_format=AF_GRAD;
      break;
    default:
      assert(0);
    }
    lat=sscansec(m_lat,'N','S');
    lon=sscansec(m_lon,'E','W');
  }
  else {
    if (sscanf(m_lon,"%d%d%s",&zone,&ew,&CPromDoc::buf)!=2) zone=0;
    if (sscanf(m_lat,"%d%s",&ns,&CPromDoc::buf)!=1) ns=11000000;
  }

  if (m_unit==3) {
    if (zone<1 || zone>60 || ew<=0 || ew>=1000000) {
      MessageBox("Error in zone and/or east-west coordinate",
		 "Winprom Entry Error",MB_OK|MB_ICONERROR);
      m_lon_ctl.SetFocus();
      m_lon_ctl.SetSel(0,-1);
      return;
    }
    if (ns<-10000000 || ns>10000000) {
      MessageBox("Error in north-south coordinate",
		 "Winprom Entry Error",MB_OK|MB_ICONERROR);
      m_lat_ctl.SetFocus();
      m_lat_ctl.SetSel(0,-1);
      return;
    }
    if (!location.set_UTM(zone,ew,ns)) {
      if (MessageBox("East-west coordinate is outside zone.\n"
		     "Use it anyway?","Winprom Entry Error",
		     MB_YESNO|MB_DEFBUTTON2|MB_ICONWARNING)!=IDYES) {
	m_lon_ctl.SetFocus();
	m_lon_ctl.SetSel(0,-1);
	return;
      }
    }
  }
  else {
    if (lat==coord_infty+1) {
      MessageBox("Format error- reenter latitude",
		 "Winprom Entry Error",MB_OK|MB_ICONERROR);
      m_lat_ctl.SetFocus();
      m_lat_ctl.SetSel(0,-1);
      return;
    }
    if (lon==coord_infty+1) {
      MessageBox("Format error- reenter longitude",
		 "Winprom Entry Error",MB_OK|MB_ICONERROR);
      m_lon_ctl.SetFocus();
      m_lon_ctl.SetSel(0,-1);
      return;
    }
    location.set_sec(lat,lon);
  }
  if (!bound.contains(location)) {
    MessageBox("That point lies outside the area of the map.",
	       "Winprom Set Location",MB_OK|MB_ICONERROR);
    return;
  }

  EndDialog(IDOK);
}
