// ErrorAllow_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "ErrorAllow_dlg.h"
#include "ErrorParam_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CErrorAllow_dlg dialog

CErrorAllow_dlg::CErrorAllow_dlg(const CPromDoc& doc, CWnd *pParent /*=NULL*/)
  : CDialog(CErrorAllow_dlg::IDD,pParent),document(doc),m_app_filter(true)
{
  //{{AFX_DATA_INIT(CErrorAllow_dlg)
  m_far = 0;
  m_near = 0;
  m_error = 0;
  m_separam = FALSE;
  m_num_featr = 0;
  m_raw_err = 0.0f;
  m_model_err = 0.0f;
  m_parvar = FALSE;
  m_sigma_in = 0.0f;
  m_sigma_out = 0.0f;
  m_mahal_val = 0.0f;
  m_bias = 0.0f;
  m_elev_data = FALSE;
  m_range_fixed = FALSE;
  m_RQF_gp = FALSE;
  m_RQF_stat = -1;
  m_RQF_badhess = -1;
  m_regress = FALSE;
  //}}AFX_DATA_INIT
  m_stat_filter.rec_filter.align(doc.data);
  m_app_filter.rec_filter.align(doc.data);
}


void CErrorAllow_dlg::DoDataExchange(CDataExchange *pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CErrorAllow_dlg)
  DDX_Control(pDX, IDC_PARAMS, m_params_butn);
  DDX_Control(pDX, IDC_FEATR_TYPE, m_type_tab);
  DDX_Control(pDX, IDC_NEW_ERROR, m_error_ctl);
  DDX_Text(pDX, IDC_FAR, m_far);
  DDX_Text(pDX, IDC_NEAR, m_near);
  DDX_Text(pDX, IDC_NEW_ERROR, m_error);
  DDX_Check(pDX, IDC_SEPARAM, m_separam);
  DDX_Text(pDX, IDC_NUM_FEATR, m_num_featr);
  DDX_Text(pDX, IDC_RAW_ERR, m_raw_err);
  DDX_Text(pDX, IDC_MODEL_ERR, m_model_err);
  DDX_Check(pDX, IDC_PARVAR, m_parvar);
  DDX_Text(pDX, IDC_SIGMA_IN, m_sigma_in);
  DDX_Text(pDX, IDC_SIGMA_OUT, m_sigma_out);
  DDX_Text(pDX, IDC_MAHAL_VAL, m_mahal_val);
  DDX_Text(pDX, IDC_BIAS, m_bias);
  DDX_Check(pDX, IDC_ELEV_DATA, m_elev_data);
  DDX_Check(pDX, IDC_RANGE_FIXED, m_range_fixed);
  DDX_Check(pDX, IDC_RQF_GP, m_RQF_gp);
  DDX_CBIndex(pDX, IDC_RQF_STAT, m_RQF_stat);
  DDX_CBIndex(pDX, IDC_BADHESS, m_RQF_badhess);
  DDX_Check(pDX, IDC_REGRESS, m_regress);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CErrorAllow_dlg, CDialog)
  //{{AFX_MSG_MAP(CErrorAllow_dlg)
  ON_BN_CLICKED(IDC_APP_FILTER, OnAppFilter)
  ON_BN_CLICKED(IDC_SEPARAM, OnSeparam)
  ON_BN_CLICKED(IDC_STAT_FILTER, OnStatFilter)
  ON_NOTIFY(TCN_SELCHANGE, IDC_FEATR_TYPE, OnChangeFeatrType)
  ON_BN_CLICKED(IDC_PARAMS, OnParams)
  ON_BN_CLICKED(IDC_HESSIGN, OnUpdateParams)
  ON_BN_CLICKED(IDC_REGRESS, OnJustUpdate)
  ON_BN_CLICKED(IDC_PARVAR, OnUpdateParams)
  ON_BN_CLICKED(IDC_ELEV_DATA, OnUpdateParams)
  ON_BN_CLICKED(IDC_RQF_GP, OnUpdateParams)
  ON_CBN_SELCHANGE(IDC_RQF_STAT, OnUpdateParams)
  ON_CBN_SELCHANGE(IDC_BADHESS, OnUpdateParams)
  ON_BN_CLICKED(IDC_RANGE_FIXED, OnJustUpdate)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CErrorAllow_dlg message handlers

void CErrorAllow_dlg::set_params()
{
  const Regress *data = m_type_tab.GetCurSel()==0 ? &peak_data : &sadl_data;
  m_num_featr=data->points.size();
  if (m_num_featr>0) {
    m_raw_err=sqrt(data->sigma2_raw);
    if (m_num_featr>2) m_model_err=sqrt(data->sigma2_res);
    else m_model_err=-1;
    m_bias=data->bias;
  }
  else {
    m_raw_err=m_model_err=-1;
    m_bias=0;
  }
  m_mahal_val=data->mahal;
  m_params_butn.EnableWindow(m_num_featr>=2);
  UpdateData(FALSE);
}

void CErrorAllow_dlg::update_stats()
{
  const Divide_tree& tree=document.tree;
  peak_data.zero();
  sadl_data.zero();
  Domain::Index i,n=tree.n_dom();
  for (i=0; i<n; ++i) {
    const Feature &p=tree[i].peak,&s=tree[i].saddle;
    if (p.location) add_stat(p,FT_PEAK);
    if (s.location) add_stat(s,FT_SADDLE);
  }
  n=tree.n_runoff();
  for (i=0; i<n; ++i) {
    const Feature &f=tree.runoff(i);
    if (f.location) add_stat(f,FT_RUNOFF);
  }
  n=tree.n_bsnsdl();
  for (i=0; i<n; ++i) {
    const Feature &f=tree.bsnsdl(i);
    if (f.location) add_stat(f,FT_BASINSADL);
  }

  //TRACE("Peak regression parameters:\n");
  peak_data.parvar=m_parvar!=FALSE;
  //peak_data.summarize();
  if (!peak_data.parvar && m_parvar) {
    AfxMessageBox("Cannot find meaningful variance parameters\n"
		  "for peaks.  Reverting to constant variance.",MB_ICONWARNING);
    m_parvar=FALSE;
  }
  peak_data.trace();
  if (m_separam) {
    //TRACE("Saddle regression parameters:\n");
    sadl_data.parvar=m_parvar!=FALSE;
    //sadl_data.summarize();
    if (!sadl_data.parvar && m_parvar) {
      AfxMessageBox("Cannot find meaningful variance parameters\n"
		    "for saddles.  Reverting to constant variance.",MB_ICONWARNING);
      // we previously computed peaks with parameterized variance,
      // so we have to recompute without.
      peak_data.parvar=false;
      //peak_data.summarize();
      m_parvar=FALSE;
    }
    sadl_data.trace();
  }

  set_params();
}

void CErrorAllow_dlg::set_tabs()
{
  m_type_tab.DeleteAllItems();
  if (m_separam) {
    m_type_tab.InsertItem(0,"Peak");
    m_type_tab.InsertItem(1,"Saddle");
  }
  else m_type_tab.InsertItem(0,"Peak && Saddle");
  update_stats();
}

BOOL CErrorAllow_dlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  set_tabs();
  m_error_ctl.SetFocus();
  m_error_ctl.SetSel(0,-1);

  return FALSE;  // return TRUE unless you set the focus to a control
}

void CErrorAllow_dlg::OnStatFilter()
{
  CFeatureFilter_dlg(m_stat_filter,this).DoModal();
  update_stats();
}

void CErrorAllow_dlg::OnAppFilter()
{
  CFeatureFilter_dlg(m_app_filter,this).DoModal();
}

void CErrorAllow_dlg::OnSeparam()
{
  if (!UpdateData()) return;
  set_tabs();
}

bool CErrorAllow_dlg::getRQF(const Feature& featr,
		double& lap, double& shev2, double& ftr_elev, FT_index type)
{
  int hessign;
  switch (type) {
  case FT_PEAK: case FT_RUNOFF: hessign=-2; break;
  case FT_SADDLE: case FT_BASINSADL: hessign=0; break;
  case FT_BASIN: case FT_SBRO: hessign=2; break;
  }
  RQF_methods rqfm;
  rqfm.statpt=(RQF_methods::statpt_t)m_RQF_stat;
  rqfm.grid_point=m_RQF_gp!=FALSE;
  rqfm.badhess=(RQF_methods::badhess_t)m_RQF_badhess;
  return ::getRQF(document.elev,featr,hessign,rqfm,lap,shev2,ftr_elev);
}

void CErrorAllow_dlg::add_stat(const Feature& featr, FT_index type)
{
  if (featr.edited && m_stat_filter.test(featr,document.data)) {
    double lap,shev2,ftr_elev;
    if (getRQF(featr,lap,shev2,ftr_elev,type)) {
      Regress *data =
	type==FT_PEAK || type==FT_BASIN || !m_separam ? &peak_data : &sadl_data;
      if (ftr_elev==elev_undef && m_elev_data &&
	  document.elev.contains(featr.location))
	ftr_elev=document.elev[featr.location];
      //if (ftr_elev!=elev_undef) {
	//data->add_point(lap,ftr_elev-featr.elev.midpt(),shev2,
			//0.5*(featr.elev.high-featr.elev.low)/m_sigma_in);
      //}
    }
  }
}

void CErrorAllow_dlg::OnUpdateParams()
{
  if (!UpdateData()) return;
  update_stats();
}

void CErrorAllow_dlg::OnOK()
{
  if (!UpdateData()) return;
  EndDialog(IDOK);
}

void CErrorAllow_dlg::OnChangeFeatrType(NMHDR *, LRESULT *pResult)
{
  set_params();
  *pResult = 0;
}

void CErrorAllow_dlg::OnParams()
{
  const Regress *data = m_type_tab.GetCurSel()==0 ? &peak_data : &sadl_data;
  //CErrorParam_dlg(*data).DoModal();
}

void CErrorAllow_dlg::OnJustUpdate()
{
  UpdateData();
}
