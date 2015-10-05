// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// DataLocationFilter_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "DataLocationFilter_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDataLocationFilter_dlg dialog

CDataLocationFilter_dlg::CDataLocationFilter_dlg(
	Record_location_filter& rf, CWnd* pParent /*=NULL*/)
  : CDataFilter_dlg(rf,CDataLocationFilter_dlg::IDD,pParent),filter(rf)
{
  //{{AFX_DATA_INIT(CDataLocationFilter_dlg)
  m_east = _T("");
  m_north = _T("");
  m_south = _T("");
  m_west = _T("");
  m_nonorph = FALSE;
  m_orphan = FALSE;
  //}}AFX_DATA_INIT
}

void CDataLocationFilter_dlg::DoDataExchange(CDataExchange* pDX)
{
  // We do not call CDataFilter_dlg::DoDataExchange() because we do not have
  // the IDC_ACCEPT_NODATA control.
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDataLocationFilter_dlg)
  DDX_Control(pDX, IDC_LIMIT_W, m_west_ctl);
  DDX_Control(pDX, IDC_LIMIT_S, m_south_ctl);
  DDX_Control(pDX, IDC_LIMIT_E, m_east_ctl);
  DDX_Control(pDX, IDC_LIMIT_N, m_north_ctl);
  DDX_Text(pDX, IDC_LIMIT_E, m_east);
  DDX_Text(pDX, IDC_LIMIT_N, m_north);
  DDX_Text(pDX, IDC_LIMIT_S, m_south);
  DDX_Text(pDX, IDC_LIMIT_W, m_west);
  DDX_Check(pDX, IDC_NONORPH, m_nonorph);
  DDX_Check(pDX, IDC_ORPHAN, m_orphan);
  //}}AFX_DATA_MAP
  DDX_Check(pDX, IDC_CASE, m_case);
  DDX_Check(pDX, IDC_LATIN1, m_latin1);
}

BEGIN_MESSAGE_MAP(CDataLocationFilter_dlg, CDataFilter_dlg)
  //{{AFX_MSG_MAP(CDataLocationFilter_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDataLocationFilter_dlg message handlers

BOOL CDataLocationFilter_dlg::OnInitDialog()
{
  char buf[64]; // used for formatting geographic coordinates
  const GridPoint sw=filter.limits.corner1(),ne=filter.limits.corner2();
  if (filter.limits.limit[0].high<coord_infty) {
    ne.sprint1(buf);
    m_north=buf;
  }
  else m_north.Empty();
  if (filter.limits.limit[0].low>-coord_infty) {
    sw.sprint1(buf);
    m_south=buf;
  }
  else m_south.Empty();
  if (filter.limits.limit[1].high<coord_infty) {
    ne.sprint2(buf);
    m_east=buf;
  }
  else m_east.Empty();
  if (filter.limits.limit[1].low>-coord_infty) {
    sw.sprint2(buf);
    m_west=buf;
  }
  else m_west.Empty();

  m_nonorph=filter.nonorph;
  m_orphan=filter.orphan;

  return CDataFilter_dlg::OnInitDialog();
}

void CDataLocationFilter_dlg::OnOK()
{
  if (!UpdateData(TRUE)) return;

  if (!CPromDoc::get_rect(filter.limits,this,m_north,m_south,m_east,m_west,
			  m_north_ctl,m_south_ctl,m_east_ctl,m_west_ctl))
    return;

  filter.nonorph=m_nonorph!=FALSE;
  filter.orphan=m_orphan!=FALSE;

  CDataFilter_dlg::OnOK();
}
