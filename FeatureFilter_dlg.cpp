// FeatureFilter_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "FeatureFilter_dlg.h"
#include "DataFilter_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFeatureFilter_dlg dialog

void CFeatureFilter_dlg::init()
{
  //{{AFX_DATA_INIT(CFeatureFilter_dlg)
  m_case = FALSE;
  m_edge = FALSE;
  m_name_filter = -1;
  m_name = _T("");
  m_elev_high = _T("");
  m_elev_low = _T("");
  m_limit_e = _T("");
  m_limit_n = _T("");
  m_limit_s = _T("");
  m_limit_w = _T("");
  m_nonedge = FALSE;
  m_strict = FALSE;
  m_isolatin1 = FALSE;
  m_edit = FALSE;
  m_unedit = FALSE;
  //}}AFX_DATA_INIT
}

CFeatureFilter_dlg::CFeatureFilter_dlg(UINT nID, FeatureFilter& ff, CWnd *parent)
  : CDialog(nID),filter(ff)
{
  init();
}

CFeatureFilter_dlg::CFeatureFilter_dlg(FeatureFilter& ff, CWnd *parent)
  : CDialog(CFeatureFilter_dlg::IDD,parent),filter(ff)
{
  init();
}


void CFeatureFilter_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CFeatureFilter_dlg)
  DDX_Control(pDX, IDC_LIMIT_W, m_limit_w_ctl);
  DDX_Control(pDX, IDC_LIMIT_S, m_limit_s_ctl);
  DDX_Control(pDX, IDC_LIMIT_N, m_limit_n_ctl);
  DDX_Control(pDX, IDC_LIMIT_E, m_limit_e_ctl);
  DDX_Control(pDX, IDC_ELEV_LOW, m_elev_low_ctl);
  DDX_Control(pDX, IDC_ELEV_HIGH, m_elev_high_ctl);
  DDX_Check(pDX, IDC_CASE, m_case);
  DDX_Check(pDX, IDC_EDGE, m_edge);
  DDX_Radio(pDX, IDC_NAME_IS, m_name_filter);
  DDX_Text(pDX, IDC_NAME_TEXT, m_name);
  DDX_Text(pDX, IDC_ELEV_HIGH, m_elev_high);
  DDX_Text(pDX, IDC_ELEV_LOW, m_elev_low);
  DDX_Text(pDX, IDC_LIMIT_E, m_limit_e);
  DDX_Text(pDX, IDC_LIMIT_N, m_limit_n);
  DDX_Text(pDX, IDC_LIMIT_S, m_limit_s);
  DDX_Text(pDX, IDC_LIMIT_W, m_limit_w);
  DDX_Check(pDX, IDC_NONEDGE, m_nonedge);
  DDX_Check(pDX, IDC_STRICT, m_strict);
  DDX_Check(pDX, IDC_ISOLATIN, m_isolatin1);
  DDX_Check(pDX, IDC_EDIT, m_edit);
  DDX_Check(pDX, IDC_UNEDIT, m_unedit);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeatureFilter_dlg, CDialog)
  //{{AFX_MSG_MAP(CFeatureFilter_dlg)
  ON_BN_CLICKED(IDC_DATA, OnData)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFeatureFilter_dlg message handlers

BOOL CFeatureFilter_dlg::OnInitDialog() 
{
  if (filter.elev.high<elev_infty)
    m_elev_high.Format("%d",filter.elev.high);
  else m_elev_high.Empty();
  if (filter.elev.low>-elev_infty)
    m_elev_low.Format("%d",filter.elev.low);
  else m_elev_low.Empty();
  m_strict=filter.strict;

  CPromDoc::set_rect(filter.limits,m_limit_n,m_limit_s,m_limit_e,m_limit_w);

  m_name_filter=filter.name_filter_type;
  m_name=filter.name;
  m_case=filter.case_sens;
  m_isolatin1=filter.isolatin1_sens;
  m_edge=filter.edge;
  m_nonedge=filter.nonedge;
  m_edit=filter.edit;
  m_unedit=filter.unedit;
  m_data_filter=filter.rec_filter;

  CDialog::OnInitDialog();

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}

bool CFeatureFilter_dlg::close()
{
  if (!m_edge && !m_nonedge) {
    if (MessageBox("You have excluded both edge effects and non edge effects.\n"
		   "Is that really what you want?",
		   "Winprom Entry Warning",
		   MB_YESNO|MB_DEFBUTTON2|MB_ICONWARNING)!=IDYES)
      return false;
  }

  if (!m_edit && !m_unedit) {
    if (MessageBox("You have excluded both edited and unedited features.\n"
		   "Is that really what you want?",
		   "Winprom Entry Warning",
		   MB_YESNO|MB_DEFBUTTON2|MB_ICONWARNING)!=IDYES)
      return false;
  }

  const char *buf;
  Elevation elev_low,elev_high;
  // Check elevation limits

  if (m_elev_high.IsEmpty())
    elev_high=elev_infty;
  else {
    buf=m_elev_high;
    elev_high=(Elevation)strtol(buf,(char **)&buf,10);
    if (*buf) {
      MessageBox("Enter a number for the highest elevation\n"
		 "or leave blank if no restriction",
		 "Winprom Entry Error",MB_OK|MB_ICONERROR);
      m_elev_high_ctl.SetFocus();
      m_elev_high_ctl.SetSel(0,-1);
      return false;
    }
  }

  if (m_elev_low.IsEmpty())
    elev_low=-elev_infty;
  else {
    buf=m_elev_low;
    elev_low=(Elevation)strtol(buf,(char **)&buf,10);
    if (*buf) {
      MessageBox("Enter a number for the lowest elevation\n"
		 "or leave blank if no restriction",
		 "Winprom Entry Error",MB_OK|MB_ICONERROR);
      m_elev_low_ctl.SetFocus();
      m_elev_low_ctl.SetSel(0,-1);
      return false;
    }
  }

  if (elev_low>elev_high) {
    MessageBox("Highest elevation must be greater than\n"
	       "or equal to lowest elevation",
	       "Winprom Entry Error",MB_OK|MB_ICONERROR);
    return false;
  }

  // Check location limits

  if (!CPromDoc::get_rect(filter.limits,this,m_limit_n,m_limit_s,m_limit_e,m_limit_w,
			  m_limit_n_ctl,m_limit_s_ctl,m_limit_e_ctl,m_limit_w_ctl))
    return false;

  filter.elev.high=elev_high;
  filter.elev.low=elev_low;
  filter.strict=m_strict!=FALSE;
  filter.name_filter_type=(FeatureFilter::Name_ft)m_name_filter;
  filter.name=m_name;
  filter.case_sens=m_case!=FALSE;
  filter.isolatin1_sens=m_isolatin1!=FALSE;
  filter.edge=m_edge!=FALSE;
  filter.nonedge=m_nonedge!=FALSE;
  filter.edit=m_edit!=FALSE;
  filter.unedit=m_unedit!=FALSE;
  filter.rec_filter=m_data_filter;

  return true;
}

void CFeatureFilter_dlg::OnOK()
{
  if (!UpdateData(TRUE) || !close()) return;
  EndDialog(IDOK);
}

void CFeatureFilter_dlg::OnData()
{
  CDataFilter_dlg dlg(m_data_filter);
  dlg.DoModal();
}
