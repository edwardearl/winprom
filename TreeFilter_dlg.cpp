// TreeFilter_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "TreeFilter_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


bool TreeFilter::test(const Domain& dom, const Database& data,
		      const CPromDoc *doc) const
{
  short int dc=doc->drain_const();
  return peak_ff.test(dom.peak,data) &&
    (!dom.parent.line || dom.peak_topo_prom()>min_prom &&
			 sadl_ff.test(dom.parent.saddle->saddle,data));
}


/////////////////////////////////////////////////////////////////////////////
// CTreeFilter_dlg dialog

CTreeFilter_dlg::CTreeFilter_dlg(TreeFilter& tf, const CPromDoc& d, CWnd *pParent)
  : CDialog(CTreeFilter_dlg::IDD,pParent),filter(tf),doc(d)
{
  //{{AFX_DATA_INIT(CTreeFilter_dlg)
  m_min_prom = 0;
  m_pp_hp_max = 0;
  m_pp_hp_min = 0;
  m_pp_filter = FALSE;
  m_pp_sort = FALSE;
  m_pp_enable = FALSE;
  m_pp_child_worst = 0;
  m_pp_child_best = 0;
  m_topology = -1;
  m_runoffs = -1;
  m_sort_children = -1;
  //}}AFX_DATA_INIT
}

void CTreeFilter_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CTreeFilter_dlg)
  DDX_Control(pDX, IDC_PP_CHILD_WORST, m_pp_child_worst_ctl);
  DDX_Control(pDX, IDC_PP_CHILD_BEST, m_pp_child_best_ctl);
  DDX_Text(pDX, IDC_MIN_PROM, m_min_prom);
  DDX_Text(pDX, IDC_PP_HP_MAX, m_pp_hp_max);
  DDX_Text(pDX, IDC_PP_HP_MIN, m_pp_hp_min);
  DDX_Check(pDX, IDC_PP_FILTER, m_pp_filter);
  DDX_Check(pDX, IDC_PP_SORT, m_pp_sort);
  DDX_Check(pDX, IDC_PP_ENABLE, m_pp_enable);
  DDX_Text(pDX, IDC_PP_CHILD_WORST, m_pp_child_worst);
  DDX_Text(pDX, IDC_PP_CHILD_BEST, m_pp_child_best);
  DDX_Radio(pDX, IDC_LINE, m_topology);
  DDX_Radio(pDX, IDC_LRO_NONE, m_runoffs);
  DDX_Radio(pDX, IDC_SC_PROM, m_sort_children);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_SC_PEAK, m_peak_elev);
}


BEGIN_MESSAGE_MAP(CTreeFilter_dlg, CDialog)
  //{{AFX_MSG_MAP(CTreeFilter_dlg)
  ON_BN_CLICKED(IDC_PEAK_FILTER, OnPeakFilter)
  ON_BN_CLICKED(IDC_SADL_FILTER, OnSadlFilter)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTreeFilter_dlg message handlers

BOOL CTreeFilter_dlg::OnInitDialog()
{
  m_min_prom=filter.min_prom;
  m_peak_ff=filter.peak_ff;
  m_sadl_ff=filter.sadl_ff;
  m_peak_ff.rec_filter.align(doc.data);
  m_sadl_ff.rec_filter.align(doc.data);
  m_topology=filter.topology-1;
  m_runoffs=filter.runoffs;
  m_pp_enable=filter.pp_enable;
  m_pp_filter=filter.pp_filter;
  m_pp_sort=filter.pp_sort;
  m_pp_hp_min=filter.pp_patrch_prom.low;
  m_pp_hp_max=filter.pp_patrch_prom.high;
  m_pp_child_best=filter.pp_child_best;
  m_pp_child_worst=filter.pp_child_worst;
  m_sort_children=filter.child_sorting;

  CDialog::OnInitDialog();

  SetIcon(AfxGetApp()->LoadIcon(IDR_LPR_TREE),TRUE);
  m_peak_elev.SetWindowText(doc.is_drainage?"Basin elevation":"Peak elevation");

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}

void CTreeFilter_dlg::OnOK()
{
  if (!UpdateData(TRUE)) return;

  if (m_pp_hp_min>m_pp_hp_max) {
    MessageBox("For patriarch proportional prominence,\n"
	       "the high value must be >= the low value.",
	       "Winprom Entry Error",MB_OK|MB_ICONERROR);
    return;
  }
  if (m_pp_child_best>m_pp_hp_max || m_pp_child_best<m_pp_hp_min) {
    MessageBox("The \"best for child\" proportional prominence of the patriarch\n"
	       "must be within the patriarch's proportional prominence range.",
	       "Winprom Entry Error",MB_OK|MB_ICONERROR);
    m_pp_child_best_ctl.SetFocus();
    m_pp_child_best_ctl.SetSel(0,-1);
    return;
  }
  if (m_pp_child_worst>m_pp_hp_max || m_pp_child_worst<m_pp_hp_min) {
    MessageBox("The \"worst for child\" proportional prominence of the patriarch\n"
	       "must be within the patriarch's proportional prominence range.",
	       "Winprom Entry Error",MB_OK|MB_ICONERROR);
    m_pp_child_worst_ctl.SetFocus();
    m_pp_child_worst_ctl.SetSel(0,-1);
    return;
  }

  filter.min_prom=m_min_prom;
  filter.peak_ff=m_peak_ff;
  filter.sadl_ff=m_sadl_ff;
  filter.topology=(Domain::Topology)(m_topology+1);
  filter.runoffs=(TreeFilter::List_runoffs)m_runoffs;
  filter.pp_enable=m_pp_enable!=FALSE;
  filter.pp_filter=m_pp_filter!=FALSE;
  filter.pp_sort=m_pp_sort!=FALSE;
  filter.pp_patrch_prom.low=m_pp_hp_min;
  filter.pp_patrch_prom.high=m_pp_hp_max;
  filter.pp_child_best=m_pp_child_best;
  filter.pp_child_worst=m_pp_child_worst;
  filter.child_sorting=(TreeFilter::Child_sorting)m_sort_children;

  CDialog::EndDialog(IDOK);
}

void CTreeFilter_dlg::OnPeakFilter()
{
  CFeatureFilter_dlg(m_peak_ff,this).DoModal();
}

void CTreeFilter_dlg::OnSadlFilter()
{
  CFeatureFilter_dlg(m_sadl_ff,this).DoModal();
}
