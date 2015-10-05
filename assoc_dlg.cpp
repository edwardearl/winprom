// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// Assoc_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "Assoc_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// Cssoc_dlg dialog

CAssoc_dlg::CAssoc_dlg(Assoc_info& tgt, bool e, bool d, bool p, CWnd* pParent /*=NULL*/)
  : CDialog(CAssoc_dlg::IDD, pParent),target(tgt),
    have_elev(e),have_dom(d),have_topo(p)
{
  //{{AFX_DATA_INIT(CAssoc_dlg)
  m_domain = FALSE;
  m_walkup = FALSE;
  m_radius = 0.0f;
  m_peak_dest = FALSE;
  m_saddle_dest = FALSE;
  m_elev_prom = FALSE;
  m_bs_dest = FALSE;
  m_ro_dest = FALSE;
  m_path = FALSE;
  m_bs_prom_type = -1;
  m_cpdm = 0.0f;
  //}}AFX_DATA_INIT
}

void CAssoc_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAssoc_dlg)
  DDX_Control(pDX, IDC_CPDM, m_cpdm_edit);
  DDX_Control(pDX, IDC_RADIUS, m_radius_edit);
  DDX_Control(pDX, IDC_PATH, m_path_ctl);
  DDX_Control(pDX, IDC_WALKUP, m_wkup_ctl);
  DDX_Control(pDX, IDC_DOMAIN, m_dom_ctl);
  DDX_Check(pDX, IDC_DOMAIN, m_domain);
  DDX_Check(pDX, IDC_WALKUP, m_walkup);
  DDX_Text(pDX, IDC_RADIUS, m_radius);
  DDX_Check(pDX, IDC_PEAKS, m_peak_dest);
  DDX_Check(pDX, IDC_SADDLES, m_saddle_dest);
  DDX_Check(pDX, IDC_ELEV_PROM, m_elev_prom);
  DDX_Check(pDX, IDC_BSNSDLS, m_bs_dest);
  DDX_Check(pDX, IDC_RUNOFFS, m_ro_dest);
  DDX_Check(pDX, IDC_PATH, m_path);
  DDX_Radio(pDX, IDC_BSPT_STANDARD, m_bs_prom_type);
  DDX_Text(pDX, IDC_CPDM, m_cpdm);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAssoc_dlg, CDialog)
  //{{AFX_MSG_MAP(CAssoc_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAssoc_dlg message handlers

BOOL CAssoc_dlg::OnInitDialog()
{
  m_radius=target.radius;
  m_peak_dest=target.pk_dest;
  m_saddle_dest=target.ps_dest;
  m_ro_dest=target.ro_dest;
  m_bs_dest=target.bs_dest;
  m_domain=target.domain && have_dom;
  m_walkup=target.walkup && have_elev;
  m_path=target.path && have_topo;
  m_cpdm=target.cpdm;
  m_elev_prom=target.elev_prom;
  m_bs_prom_type=target.bspt_assoc;

  CDialog::OnInitDialog();

  if (!have_dom) m_dom_ctl.EnableWindow(FALSE);
  if (!have_elev) m_wkup_ctl.EnableWindow(FALSE);
  if (!have_topo) {
    m_path_ctl.EnableWindow(FALSE);
    m_cpdm_edit.EnableWindow(FALSE);
  }

  return TRUE;  // return TRUE unless you set the focus to a control
}

void CAssoc_dlg::OnOK()
{
  if (!UpdateData()) return;
  if (m_radius<0) {
    MessageBox("The tolerance radius is less than 0.",
	       "Winprom Error",MB_OK|MB_ICONERROR);
    m_radius_edit.SetFocus();
    m_radius_edit.SetSel(0,1);
    return;
  }
  if (!m_peak_dest && !m_saddle_dest && !m_ro_dest && !m_bs_dest) {
    MessageBox("You must enable at least one feature type destination.",
	       "Winprom Error",MB_OK|MB_ICONERROR);
    return;
  }

  target.radius=m_radius;
  target.pk_dest=m_peak_dest!=FALSE;
  target.ps_dest=m_saddle_dest!=FALSE;
  target.ro_dest=m_ro_dest!=FALSE;
  target.bs_dest=m_bs_dest!=FALSE;
  target.domain=m_domain!=FALSE;
  target.walkup=m_walkup!=FALSE;
  target.path=m_path!=FALSE;
  target.cpdm=m_cpdm;
  target.elev_prom=m_elev_prom!=FALSE;
  target.bspt_assoc=m_bs_prom_type!=FALSE;

  EndDialog(IDOK);
}
