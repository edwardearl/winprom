// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// GridPointFilter_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "GridPointFilter_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


GPfilter::GPfilter(CPromDoc& doc, GridPoint& c)
  : document(doc)
{
  if (doc.em_state!=NOT_LOADED && doc.elev.contains(c) &&
      doc.elev[c]!=elev_undef)
    elev=doc.elev[c];
  else {
    elev.low=-elev_infty;
    elev.high=elev_infty;
  }
  if (doc.dm_state!=NOT_LOADED && doc.domain.contains(c) &&
      doc.domain[c]!=Domain::undef) {
    dom=&document.tree[doc.domain[c]];
  }
  else dom=0;
  dom_ancestors=false;
  dom_descendants=false;
  topology=Domain::TOPO_CELL;
}

bool GPfilter::test(const GridPoint& gp) const
{
  if (elev.low!=-elev_infty || elev.high!=elev_infty) {
    if (!document.elev.contains(gp) || document.elev[gp]==elev_undef ||
	!elev.contains(document.elev[gp]))
      return false;
  }
  if (!limits.contains(gp)) return false;
  if (!dom) return true;
  const Index_map& domap=document.domain;
  if (!domap.contains(gp) || domap[gp]==Domain::undef) return false;
  if (dom_ancestors) {
    Divide_tree& tree=document.tree;
    Domain::Index ipdom=domap[gp];
    if (ipdom<tree.n_dom() && tree[ipdom].is_ancestor(dom,topology))
      return true;
  }
  if (dom_descendants) {
    Divide_tree& tree=document.tree;
    Domain::Index ipdom=domap[gp];
    if (ipdom<tree.n_dom() && dom->is_ancestor(&tree[ipdom],topology))
      return true;
  }

  return false;
}


/////////////////////////////////////////////////////////////////////////////
// CGridPointFilter_dlg dialog

CGridPointFilter_dlg::CGridPointFilter_dlg(GPfilter& filter, const GridPoint& s)
  : CDialog(CGridPointFilter_dlg::IDD,AfxGetMainWnd()),target(filter),start(s)
{
  //{{AFX_DATA_INIT(CGridPointFilter_dlg)
  m_dlpr_all = FALSE;
  m_dlpr_ancestors = FALSE;
  m_dlpr_descendants = FALSE;
  m_elev_high = _T("");
  m_elev_low = _T("");
  m_limit_e = _T("");
  m_limit_n = _T("");
  m_limit_s = _T("");
  m_limit_w = _T("");
  m_topology = -1;
  //}}AFX_DATA_INIT
}

void CGridPointFilter_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CGridPointFilter_dlg)
  DDX_Control(pDX, IDC_LIMIT_W, m_limit_w_ctl);
  DDX_Control(pDX, IDC_LIMIT_S, m_limit_s_ctl);
  DDX_Control(pDX, IDC_LIMIT_N, m_limit_n_ctl);
  DDX_Control(pDX, IDC_LIMIT_E, m_limit_e_ctl);
  DDX_Control(pDX, IDC_DLPR_DESCENDANTS, m_desc_ctl);
  DDX_Control(pDX, IDC_DLPR_ANCESTORS, m_anc_ctl);
  DDX_Control(pDX, IDC_DLPR_ALL, m_all_ctl);
  DDX_Control(pDX, IDC_ELEV_LOW, m_low_ctl);
  DDX_Control(pDX, IDC_ELEV_HIGH, m_high_ctl);
  DDX_Check(pDX, IDC_DLPR_ALL, m_dlpr_all);
  DDX_Check(pDX, IDC_DLPR_ANCESTORS, m_dlpr_ancestors);
  DDX_Check(pDX, IDC_DLPR_DESCENDANTS, m_dlpr_descendants);
  DDX_Text(pDX, IDC_ELEV_HIGH, m_elev_high);
  DDX_Text(pDX, IDC_ELEV_LOW, m_elev_low);
  DDX_Text(pDX, IDC_LIMIT_E, m_limit_e);
  DDX_Text(pDX, IDC_LIMIT_N, m_limit_n);
  DDX_Text(pDX, IDC_LIMIT_S, m_limit_s);
  DDX_Text(pDX, IDC_LIMIT_W, m_limit_w);
  DDX_Radio(pDX, IDC_TOPO_LINE, m_topology);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGridPointFilter_dlg, CDialog)
  //{{AFX_MSG_MAP(CGridPointFilter_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGridPointFilter_dlg message handlers

BOOL CGridPointFilter_dlg::OnInitDialog() 
{
  if (target.elev.low==-elev_infty) m_elev_low.Empty();
  else m_elev_low.Format("%d",target.elev.low);
  if (target.elev.high==elev_infty) m_elev_high.Empty();
  else m_elev_high.Format("%d",target.elev.high);

  m_dlpr_all=!target.dom;
  m_dlpr_ancestors=target.dom_ancestors;
  m_dlpr_descendants=target.dom_descendants;
  m_topology=target.topology-1;

  CDialog::OnInitDialog();

  if (target.elev.low==-elev_infty && target.elev.high==elev_infty) {
    m_low_ctl.EnableWindow(FALSE);
    m_high_ctl.EnableWindow(FALSE);
  }
  if (!target.dom) {
    m_all_ctl.EnableWindow(FALSE);
    m_anc_ctl.EnableWindow(FALSE);
    m_desc_ctl.EnableWindow(FALSE);
  }
  else if (target.document.dt_state==NOT_LOADED) {
    m_anc_ctl.EnableWindow(FALSE);
    m_desc_ctl.EnableWindow(FALSE);
  }

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}

void CGridPointFilter_dlg::OnOK()
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

  if (low>target.elev.high || high<target.elev.low) {
    MessageBox("The elevation range does not contain\n"
	       "the elevation of your starting point.",
	       "Winprom Range Error",MB_OK|MB_ICONERROR);
    return;
  }

  // Check location limits

  if (!CPromDoc::get_rect(target.limits,this,m_limit_n,m_limit_s,m_limit_e,m_limit_w,
			  m_limit_n_ctl,m_limit_s_ctl,m_limit_e_ctl,m_limit_w_ctl))
    return;

  if (!target.limits.contains(start)) {
    MessageBox("The horizontal limit does not contain your starting point.",
	       "Winprom Range Error",MB_OK|MB_ICONERROR);
    return;
  }

  if (low<-elev_infty) low=-elev_infty;
  if (high>elev_infty) high=elev_infty;
  target.elev.set((Elevation)low,(Elevation)high);

  if (m_dlpr_all) target.dom=0;
  target.dom_ancestors=m_dlpr_ancestors!=FALSE;
  target.dom_descendants=m_dlpr_descendants!=FALSE;
  target.topology=(Domain::Topology)(m_topology+1);

  EndDialog(IDOK);
}
