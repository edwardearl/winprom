// ErrorAnal_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "ErrorAnal_dlg.h"
#include "FeatureFilter_dlg.h"
#include "StatOptions_dlg.h"
#include "ErrorParam_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void Error_stats::summarize()
{
  peak_data.regress.summarize();
  if (separam) sadl_data.regress.summarize();
}

/////////////////////////////////////////////////////////////////////////////
// CErrorAnal_dlg dialog

CErrorAnal_dlg::CErrorAnal_dlg(Error_stats& ei, const CPromDoc& doc,
			       CWnd* pParent /*=NULL*/)
  : CDialog(CErrorAnal_dlg::IDD, pParent),error_info(ei),document(doc)
{
  //{{AFX_DATA_INIT(CErrorAnal_dlg)
  m_bias = 0.0f;
  m_enable_fixed = FALSE;
  m_fixed_error = 0.0f;
  m_mahal = 0.0f;
  m_num_featr = 0;
  m_raw_error = 0.0f;
  m_resid_error = 0.0f;
  m_sigma_cal = 0.0f;
  m_sigma_app = 0.0f;
  m_stat_model = -1;
  m_separam = FALSE;
  //}}AFX_DATA_INIT
}

void CErrorAnal_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CErrorAnal_dlg)
  DDX_Control(pDX, IDC_MORE_PARAMS, m_more_params);
  DDX_Control(pDX, IDC_FEATR_TYPE, m_featr_type_tab);
  DDX_Text(pDX, IDC_BIAS, m_bias);
  DDX_Check(pDX, IDC_ENABLE_FIXED, m_enable_fixed);
  DDX_Text(pDX, IDC_FIXED_ERROR, m_fixed_error);
  DDX_Text(pDX, IDC_MAHAL, m_mahal);
  DDX_Text(pDX, IDC_NUM_FEATR, m_num_featr);
  DDX_Text(pDX, IDC_RAW_ERR, m_raw_error);
  DDX_Text(pDX, IDC_RESID_ERR, m_resid_error);
  DDX_Text(pDX, IDC_SIGMA_IN, m_sigma_cal);
  DDX_Text(pDX, IDC_SIGMA_OUT, m_sigma_app);
  DDX_CBIndex(pDX, IDC_STAT_MODEL, m_stat_model);
  DDX_Check(pDX, IDC_SEPARAM, m_separam);
  //}}AFX_DATA_MAP
}

void CErrorAnal_dlg::DoExchangeFixedOptions(bool save)
{
  CDataExchange DX(this,save);
  DDX_Check(&DX, IDC_ENABLE_FIXED, m_enable_fixed);
  DDX_Text(&DX, IDC_FIXED_ERROR, m_fixed_error);
}

void CErrorAnal_dlg::DoExchangeStatOptions(bool save)
{
  DDX_CBIndex(&CDataExchange(this,save), IDC_STAT_MODEL, m_stat_model);
}

void CErrorAnal_dlg::UpdateStatParams()
{
  CDataExchange DX(this,FALSE);
  DDX_Text(&DX, IDC_NUM_FEATR, m_num_featr);
  DDX_Text(&DX, IDC_RAW_ERR, m_raw_error);
  DDX_Text(&DX, IDC_RESID_ERR, m_resid_error);
  DDX_Text(&DX, IDC_BIAS, m_bias);
  if (m_separam && m_mahal>=0) DDX_Text(&DX, IDC_MAHAL, m_mahal);
  else GetDlgItem(IDC_MAHAL)->SetWindowText("");
}

BEGIN_MESSAGE_MAP(CErrorAnal_dlg, CDialog)
  //{{AFX_MSG_MAP(CErrorAnal_dlg)
  ON_CBN_SELCHANGE(IDC_STAT_MODEL, OnChangeStatOptions)
  ON_NOTIFY(TCN_SELCHANGE, IDC_FEATR_TYPE, OnChangeFeatrType)
  ON_BN_CLICKED(IDC_ENABLE_FIXED, OnChangeFixedOptions)
  ON_BN_CLICKED(IDC_FILTER, OnFilter)
  ON_BN_CLICKED(IDC_MORE_PARAMS, OnMoreParams)
  ON_BN_CLICKED(IDC_OPTIONS, OnOptions)
  ON_BN_CLICKED(IDC_CONVLUTN, OnChangeStatOptions)
  ON_BN_CLICKED(IDC_MEASURE_ERROR, OnChangeStatOptions)
  ON_CBN_SELCHANGE(IDC_MIN_NBR, OnChangeStatOptions)
  ON_CBN_SELCHANGE(IDC_SYMMETRY, OnChangeStatOptions)
  ON_EN_KILLFOCUS(IDC_FIXED_ERROR, OnChangeFixedOptions)
  ON_BN_CLICKED(IDC_SEPARAM, OnSeparam)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CErrorAnal_dlg message handlers

AnalData *CErrorAnal_dlg::active_data()
{
  return m_featr_type_tab.GetCurSel()?&m_sadl_data:&m_peak_data;
}

void CErrorAnal_dlg::list_stats()
{
  if (document.em_state==NOT_LOADED) {
    m_num_featr=m_bias=0;
    m_raw_error=m_resid_error=-1;
    return;
  }
  document.list_stats(&m_peak_data,m_separam?&m_sadl_data:&m_peak_data);
}

void CErrorAnal_dlg::compute_stats()
{
  compute_stats(active_data());
  if (m_separam) m_mahal=m_peak_data.regress.mahal(m_sadl_data.regress);
}

void CErrorAnal_dlg::compute_stats(AnalData *data)
{
  if (!data) {
    compute_stats(&m_peak_data);
    if (m_separam) {
      compute_stats(&m_sadl_data);
      m_mahal=m_peak_data.regress.mahal(m_sadl_data.regress);
    }
    return;
  }
  data->regress.summarize();
  m_more_params.EnableWindow(data->regress.n>data->regress.nparam);
  //if (data->gamma<0) {
    //AfxMessageBox("Unaccounted model error is negative!\n"
		  //"Is your calibration range too low?",MB_ICONWARNING);
  //}
  //if (data==active_data()) update_stat_option_ctls();
}

void CErrorAnal_dlg::set_tabs()
{
  m_featr_type_tab.DeleteAllItems();
  if (m_separam) {
    m_featr_type_tab.InsertItem(0,"Peak");
    m_featr_type_tab.InsertItem(1,"Saddle");
  }
  else m_featr_type_tab.InsertItem(0,"Peak && Saddle");
  list_stats();
  compute_stats(0);
  update_option_ctls();
  update_stat_param_ctls();
}

BOOL CErrorAnal_dlg::OnInitDialog()
{
  m_separam=error_info.separam;
  m_peak_data=error_info.peak_data;
  m_peak_data.filter.rec_filter.align(document.data);
  m_sadl_data=error_info.sadl_data;
  m_sadl_data.filter.rec_filter.align(document.data);
  m_sigma_cal=error_info.sigma_cal;
  m_sigma_app=error_info.sigma_app;

  CDialog::OnInitDialog();

  set_tabs();

  return TRUE;  // return TRUE unless you set the focus to a control
}

void CErrorAnal_dlg::OnChangeFixedOptions()
{
  DoExchangeFixedOptions(true);
  AnalData *data=active_data();
  data->enable_fixed=m_enable_fixed!=FALSE;
  data->fixed_range=m_fixed_error;
}

void CErrorAnal_dlg::OnChangeStatOptions()
{
  get_stat_options(); // xfer stat options ctls to stat options of active data set
  list_stats();
  compute_stats();
  update_stat_param_ctls();
}

void CErrorAnal_dlg::update_option_ctls()
{
  const AnalData& data=*active_data();
  m_enable_fixed=data.enable_fixed;
  m_fixed_error=data.fixed_range;
  m_stat_model=data.options.model;
  DoExchangeFixedOptions(false);
  DoExchangeStatOptions(false);
}

void CErrorAnal_dlg::update_stat_param_ctls()
{
  const AnalData *data=active_data();
  m_num_featr=data->regress.n;
  m_raw_error=data->regress.sigma2_raw;
  if (m_raw_error>0) m_raw_error=sqrt(m_raw_error);
  m_resid_error=data->regress.sigma2_res;
  if (m_resid_error>0) m_resid_error=sqrt(m_resid_error);
  m_bias=data->regress.bias;
  UpdateStatParams();
}

void CErrorAnal_dlg::get_stat_options()
{
  DoExchangeStatOptions(true);
  AnalData *data=active_data();
  data->options.model=(Stat_options::Model)m_stat_model;
}

void CErrorAnal_dlg::OnSeparam()
{
  DDX_Check(&CDataExchange(this,TRUE),IDC_SEPARAM,m_separam);
  set_tabs();
}

void CErrorAnal_dlg::OnChangeFeatrType(NMHDR *pNMHDR, LRESULT *pResult)
{
  update_option_ctls();
  update_stat_param_ctls();

  *pResult = 0;
}

void CErrorAnal_dlg::OnFilter()
{
  FeatureFilter& filter=active_data()->filter;
  if (CFeatureFilter_dlg(filter,this).DoModal()!=IDOK) return;
  list_stats();
  compute_stats();
  update_stat_param_ctls(); // xfer stats of active data set to stat ctls
}

void CErrorAnal_dlg::OnMoreParams()
{
  const AnalData *data=active_data();
  CErrorParam_dlg(data->regress).DoModal();
}

void CErrorAnal_dlg::OnOptions()
{
  CStatOptions_dlg dlg;
  Stat_options& options=active_data()->options;
  dlg.m_symmetry=options.symmetry;
  dlg.m_min_nbr=options.mns;
  dlg.m_measr_error=options.measr_err;
  dlg.m_convolutn=options.nconv_inuse>0;
  if (dlg.DoModal()!=IDOK) return;
  options.symmetry=(Stat_options::Symmetry)dlg.m_symmetry;
  options.mns=(Stat_options::Min_nbr_set)dlg.m_min_nbr;
  options.measr_err=dlg.m_measr_error!=FALSE;
  options.nconv_inuse=dlg.m_convolutn?1:0;
  list_stats();
  compute_stats();
  update_stat_param_ctls();
}

void CErrorAnal_dlg::OnOK()
{
  OnChangeFixedOptions();
  DDX_Text(&CDataExchange(this,true), IDC_SIGMA_OUT, m_sigma_app);

  error_info.separam=m_separam!=FALSE;
  error_info.peak_data=m_peak_data;
  error_info.sadl_data=m_sadl_data;
  error_info.sigma_cal=m_sigma_cal;
  error_info.sigma_app=m_sigma_app;

  EndDialog(IDOK);
}
