// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// FeatureEdit_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "FeatureEdit_dlg.h"
#include "LocationFmt_dlg.h"
#include "PeakListView.h"
#include "SaddleListView.h"
#include "BasinSadlListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static char buf[256];


/////////////////////////////////////////////////////////////////////////////
// CFeatureEdit_dlg dialog

CFeatureEdit_dlg::CFeatureEdit_dlg(CWnd *parent, Feature& feature,
				   FT_index type, CPromDoc *doc)
  : CDialog(CFeatureEdit_dlg::IDD,parent),target(feature),
    tgt_type(type),document(doc),edit_click(false)
{
  //{{AFX_DATA_INIT(CFeatureEdit_dlg)
  m_edge = FALSE;
  m_edited = FALSE;
  m_location = _T("");
  m_elev_high = 0;
  m_elev_low = 0;
  m_name = _T("");
  m_flat_sep = _T("");
  m_prom = _T("");
  m_omp = _T("");
  m_bind_type = _T("");
  m_sadl_stat = -1;
  m_ss_label = _T("");
  m_prom_label = _T("");
  //}}AFX_DATA_INIT
}


void CFeatureEdit_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CFeatureEdit_dlg)
  DDX_Control(pDX, IDC_SADL_STATUS, m_sadl_stat_ctl);
  DDX_Control(pDX, IDC_LOCATION, m_loc_field);
  DDX_Control(pDX, IDC_EDITED, m_editCtl);
  DDX_Check(pDX, IDC_EDGE_EFFECT, m_edge);
  DDX_Check(pDX, IDC_EDITED, m_edited);
  DDX_Text(pDX, IDC_LOCATION, m_location);
  DDX_Text(pDX, IDC_HIGH, m_elev_high);
  DDX_Text(pDX, IDC_LOW, m_elev_low);
  DDX_Text(pDX, IDC_NAME, m_name);
  DDV_MaxChars(pDX, m_name, 255);
  DDX_Text(pDX, IDC_FLAT_SEP, m_flat_sep);
  DDX_Text(pDX, IDC_PROM, m_prom);
  DDX_Text(pDX, IDC_OMP, m_omp);
  DDX_Text(pDX, IDC_BIND_TYPE, m_bind_type);
  DDX_CBIndex(pDX, IDC_SADL_STATUS, m_sadl_stat);
  DDX_Text(pDX, IDC_SS_LABEL, m_ss_label);
  DDX_Text(pDX, IDC_PROM_LABEL, m_prom_label);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeatureEdit_dlg, CDialog)
  //{{AFX_MSG_MAP(CFeatureEdit_dlg)
  ON_EN_CHANGE(IDC_HIGH, OnChangeElev)
  ON_BN_CLICKED(IDC_EDITED, OnEdited)
  ON_BN_CLICKED(IDC_FORMAT, OnFormat)
  ON_BN_CLICKED(IDC_DELETE, OnDelete)
  ON_BN_CLICKED(IDC_VIEW, OnView)
  ON_BN_CLICKED(IDC_DATA, OnData)
  ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
  ON_EN_CHANGE(IDC_LOW, OnChangeElev)
  ON_BN_CLICKED(IDC_REVERT, OnRevert)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//Location_fmt CFeatureEdit_dlg::loc_fmt;


/////////////////////////////////////////////////////////////////////////////
// CFeatureEdit_dlg message handlers

BOOL CFeatureEdit_dlg::OnInitDialog() 
{   
  static UINT divide_icon_id[]={IDI_PEAK,IDI_SADDLE,IDI_RUNOFF,IDI_BASINSADL};
  static UINT drainage_icon_id[]={IDI_BASIN,IDI_BASINSADL,IDI_SBRO,IDI_SADDLE};
  Domain *dom;
  Runoff *ro;
  Basin_saddle *bs;
  Elev_intvl prom,cycle_LIS;
  Elevation omp;
  m_edge=target.edge_effect;
  m_edited=target.edited;
  Elev_intvl::format="%L to %H";
  Elev_intvl::precise_format="%d";
  Elev_intvl::min_format="%d+";
  Elev_intvl::max_format="%d-";
  CPromDoc::status_fmt.set();
  target.location.sprint(buf);
  m_location=buf;
  m_elev_high=target.elev.high;
  m_elev_low=target.elev.low;
  m_name=target.name;
  if (tgt_type!=FT_PEAK) {
    Distance fs=((Saddle&)target).flat_sep;
    m_flat_sep.Format("%.3g",fabs(fs));
    if (fs>=0) m_bind_type="Flat Separation";
    else m_bind_type="Step slope";
  }
  switch (tgt_type) {
  case FT_PEAK:
    dom=Domain::FromPeak(&target);
    zero_prob=document->prom_zp;
    dom->peak_prom().sprint(CPromDoc::buf);
    m_prom=CPromDoc::buf;
    m_prom_label="Prominence";
    omp=dom->offmap_pk_prom();
    if (omp<elev_infty) m_omp.Format("%d",omp);
    break;
  case FT_SADDLE:
    dom=Domain::FromSadl(&target);
    zero_prob=document->prom_zp;
    dom->sadl_prom().sprint(CPromDoc::buf);
    omp=dom->offmap_sadl_prom();
    m_prom=CPromDoc::buf;
    m_prom_label="Prominence";
    if (omp>-elev_infty) {
      if (omp==elev_infty) m_omp="unlimited";
      else m_omp.Format("%d",omp);
    }
    m_sadl_stat=dom->saddle.status;
    m_ss_label="Saddle status";
    break;
  case FT_BASINSADL:
    bs=(Basin_saddle *)&target;
    zero_prob=document->prom_zp;
    Saddle::use_status=document->prom_ss;
    bs->get_prom().sprint(CPromDoc::buf);
    omp=bs->offmap_prom();
    m_prom=CPromDoc::buf;
    m_prom_label="Prominence";
    if (omp>-elev_infty) {
      if (omp==elev_infty) m_omp="unlimited";
      else m_omp.Format("%d",omp);
    }
    m_sadl_stat=bs->status;
    m_ss_label="Saddle status";
    break;
  case FT_RUNOFF:
    ro=(Runoff *)&target;
    m_prom.Format("%d",ro->interior_quads);
    m_prom_label="IQs";
    break;
  default: assert(0);
  }

  CDialog::OnInitDialog();

  if (tgt_type==FT_PEAK || tgt_type==FT_RUNOFF)
    m_sadl_stat_ctl.EnableWindow(FALSE);
  SetIcon(AfxGetApp()->LoadIcon((document->is_drainage?drainage_icon_id:divide_icon_id)[tgt_type]),TRUE);

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}

void CFeatureEdit_dlg::OnChangeElev() 
{
  if (!edit_click) m_editCtl.SetCheck(1);
}

void CFeatureEdit_dlg::OnOK()
{
  if (!UpdateData(TRUE)) return;

  if (!(m_elev_low==elev_infty && m_elev_high==-elev_infty) &&
      m_elev_low>m_elev_high) {
    MessageBox("Highest elevation must be greater than\n"
	       "or equal to lowest elevation",
	       "Winprom Entry Error",MB_OK|MB_ICONERROR);
    return;
  }
  target.elev.low=m_elev_low;
  target.elev.high=m_elev_high;
  target.edited=m_edited!=FALSE;
  target.edge_effect=m_edge!=FALSE;
  target=m_name;
  if (tgt_type==FT_SADDLE || tgt_type==FT_BASINSADL)
    ((Saddle&)target).status=m_sadl_stat;

  EndDialog(IDOK);
}

void CFeatureEdit_dlg::OnEdited() 
{
  edit_click=true;
}

void CFeatureEdit_dlg::OnFormat() 
{
  CLocationFmt_dlg dlg(this,CPromDoc::status_fmt);
  if (dlg.DoModal()==IDOK) {
    CPromDoc::status_fmt.set();
    target.location.sprint(buf);
    m_loc_field.SetWindowText(buf);
  }
}

void CFeatureEdit_dlg::OnDelete() 
{
  EndDialog(IDC_DELETE);
}

void CFeatureEdit_dlg::OnView()
{
  CPromApp::getApp()->viewFeature(document,target,tgt_type);
}

void CFeatureEdit_dlg::OnData()
{
  document->tree_writeable=true;
  document->EditRecord(target.location,this);
  document->tree_writeable=false;
}

void CFeatureEdit_dlg::OnBrowse()
{
  CPromApp::getApp()->Navigate(document,target,tgt_type);
}

void CFeatureEdit_dlg::OnRevert()
{
  if (document->em_state==NOT_LOADED) {
    MessageBox("No elevation map loaded",NULL,MB_OK|MB_ICONERROR);
    return;
  }
  Elev_map& elev=document->elev;
  if (!elev.contains(target.location)) {
    MessageBox("This feature is outside the elevation map.",
	       NULL,MB_OK|MB_ICONERROR);
    return;
  }
  if (elev[target.location]==elev_undef) {
    MessageBox("Elevation not defined at this feature\'s location",
	       NULL,MB_OK|MB_ICONERROR);
    return;
  }

  m_elev_high=m_elev_low=elev[target.location];
  CDataExchange DX(this,FALSE);
  DDX_Text(&DX, IDC_HIGH, m_elev_high);
  DDX_Text(&DX, IDC_LOW, m_elev_low);
  m_editCtl.SetCheck(0);
  if (tgt_type==FT_SADDLE || tgt_type==FT_BASINSADL)
    m_sadl_stat_ctl.SetCurSel(Saddle::STAT_UNKNOWN);
}
