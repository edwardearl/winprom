// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// TreeInfo_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "TreeInfo_dlg.h"
#include "PromDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Location_fmt CTreeInfo_dlg::info_fmt;


/////////////////////////////////////////////////////////////////////////////
// CTreeInfo_dlg dialog

CTreeInfo_dlg::CTreeInfo_dlg(const CPromDoc& doc)
	: CDialog(CTreeInfo_dlg::IDD, NULL),document(doc),tree(document.tree)
{
  //{{AFX_DATA_INIT(CTreeInfo_dlg)
  m_peak_count = 0;
  m_peak_high = 0;
  m_peak_low = 0;
  m_runoff_count = 0;
  m_runoff_high = 0;
  m_runoff_low = 0;
  m_saddle_count = 0;
  m_saddle_high = 0;
  m_saddle_low = 0;
  m_alloc_e = _T("");
  m_alloc_n = _T("");
  m_alloc_s = _T("");
  m_alloc_w = _T("");
  m_defined_e = _T("");
  m_defined_n = _T("");
  m_defined_s = _T("");
  m_defined_w = _T("");
  m_bsnsadl_count = 0;
  m_bsnsadl_high = 0;
  m_bsnsadl_low = 0;
  m_file = _T("");
  m_peak_store = 0;
  m_runoff_store = 0;
  m_bsnsadl_store = 0;
  m_peaks = _T("");
  m_basn_sadls = _T("");
  m_saddles = _T("");
  //}}AFX_DATA_INIT
}


void CTreeInfo_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CTreeInfo_dlg)
  DDX_Text(pDX, IDC_PEAK_COUNT, m_peak_count);
  DDX_Text(pDX, IDC_PEAK_HIGH, m_peak_high);
  DDX_Text(pDX, IDC_PEAK_LOW, m_peak_low);
  DDX_Text(pDX, IDC_RUNOFF_COUNT, m_runoff_count);
  DDX_Text(pDX, IDC_RUNOFF_HIGH, m_runoff_high);
  DDX_Text(pDX, IDC_RUNOFF_LOW, m_runoff_low);
  DDX_Text(pDX, IDC_SADDLE_COUNT, m_saddle_count);
  DDX_Text(pDX, IDC_SADDLE_HIGH, m_saddle_high);
  DDX_Text(pDX, IDC_SADDLE_LOW, m_saddle_low);
  DDX_Text(pDX, IDC_ALLOC_E, m_alloc_e);
  DDX_Text(pDX, IDC_ALLOC_N, m_alloc_n);
  DDX_Text(pDX, IDC_ALLOC_S, m_alloc_s);
  DDX_Text(pDX, IDC_ALLOC_W, m_alloc_w);
  DDX_Text(pDX, IDC_DEFINED_E, m_defined_e);
  DDX_Text(pDX, IDC_DEFINED_N, m_defined_n);
  DDX_Text(pDX, IDC_DEFINED_S, m_defined_s);
  DDX_Text(pDX, IDC_DEFINED_W, m_defined_w);
  DDX_Text(pDX, IDC_BASINSADL_COUNT, m_bsnsadl_count);
  DDX_Text(pDX, IDC_BASINSADL_HIGH, m_bsnsadl_high);
  DDX_Text(pDX, IDC_BASINSADL_LOW, m_bsnsadl_low);
  DDX_Text(pDX, IDC_FILE, m_file);
  DDX_Text(pDX, IDC_PEAK_STORE, m_peak_store);
  DDX_Text(pDX, IDC_RUNOFF_STORE, m_runoff_store);
  DDX_Text(pDX, IDC_BASINSADL_STORE, m_bsnsadl_store);
  DDX_Text(pDX, IDC_PEAKS, m_peaks);
  DDX_Text(pDX, IDC_BASIN_SADLS, m_basn_sadls);
  DDX_Text(pDX, IDC_SADDLES, m_saddles);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTreeInfo_dlg, CDialog)
  //{{AFX_MSG_MAP(CTreeInfo_dlg)
  ON_BN_CLICKED(IDC_FORMAT, OnFormat)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTreeInfo_dlg message handlers

void CTreeInfo_dlg::reformat()
{
  char (&buf)[sizeof CPromDoc::buf]=CPromDoc::buf;

  ((GridPoint)alloc.corner2()).sprint1(buf);
  m_alloc_n=buf;
  ((GridPoint)alloc.corner1()).sprint1(buf);
  m_alloc_s=buf;
  ((GridPoint)alloc.corner2()).sprint2(buf);
  m_alloc_e=buf;
  ((GridPoint)alloc.corner1()).sprint2(buf);
  m_alloc_w=buf;

  ((GridPoint)defined.corner2()).sprint1(buf);
  m_defined_n=buf;
  ((GridPoint)defined.corner1()).sprint1(buf);
  m_defined_s=buf;
  ((GridPoint)defined.corner2()).sprint2(buf);
  m_defined_e=buf;
  ((GridPoint)defined.corner1()).sprint2(buf);
  m_defined_w=buf;
}

BOOL CTreeInfo_dlg::OnInitDialog() 
{
  long i;
  Elev_intvl peak_elev,sadl_elev,runoff_elev,bsnsadl_elev;

  m_file=document.dt_name;
  if (document.dt_state==CHANGED) m_file+=" *";

  m_peak_store=m_peak_count=tree.n_dom();
  m_saddle_count=m_peak_count;
  m_runoff_store=m_runoff_count=tree.n_runoff();
  m_bsnsadl_store=m_bsnsadl_count=tree.n_bsnsdl();

  for (i=0; i<m_peak_store; ++i) {
    if (tree[i].peak.location) {
      defined|=tree[i].peak.location;
      peak_elev|=tree[i].peak.elev;
      if (tree[i].primary_nbr) {
	defined|=tree[i].saddle.location;
	sadl_elev|=tree[i].saddle.elev;
      }
      else --m_saddle_count;
    }
    else {
      --m_peak_count;
      --m_saddle_count;
    }
  }

  for (i=0; i<m_runoff_store; ++i) {
    if (tree.runoff(i).location) {
      if (tree.runoff(i).peak) defined|=tree.runoff(i).location;
      else alloc|=tree.runoff(i).location;
      runoff_elev|=tree.runoff(i).elev;
    }
    else --m_runoff_count;
  }

  for (i=0; i<m_bsnsadl_store; ++i) {
    if (tree.bsnsdl(i).location) {
      defined|=tree.bsnsdl(i).location;
      bsnsadl_elev|=tree.bsnsdl(i).elev;
    }
    else --m_bsnsadl_count;
  }

  alloc|=defined;

  info_fmt.set();
  reformat();

  m_peaks=document.is_drainage?"Basins":"Peaks";
  m_saddles=document.is_drainage?"Basin saddles":"Prom saddles";
  m_basn_sadls=document.is_drainage?"Prom saddles":"Basin saddles";

  m_peak_high=peak_elev.high;
  m_peak_low=peak_elev.low;
  m_saddle_high=sadl_elev.high;
  m_saddle_low=sadl_elev.low;
  m_runoff_high=runoff_elev.high;
  m_runoff_low=runoff_elev.low;
  m_bsnsadl_high=bsnsadl_elev.high;
  m_bsnsadl_low=bsnsadl_elev.low;

  CDialog::OnInitDialog();

  SetWindowText(document.is_drainage?
    "Drainage Tree Information":"Divide Tree Information");

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}

void CTreeInfo_dlg::OnFormat() 
{
  CLocationFmt_dlg dlg(this,info_fmt);
  if (dlg.DoModal()==IDOK) {
    info_fmt.set();
    reformat();
    UpdateData(FALSE);
  }
}
