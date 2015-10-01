// FeatureListSummary_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "FeatureListSummary_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static char (&buf)[sizeof CFeatureListView::buf]=CFeatureListView::buf;


/////////////////////////////////////////////////////////////////////////////
// CFeatureListSummary_dlg dialog

void CFeatureListSummary_dlg::init()
{
  //{{AFX_DATA_INIT(CFeatureListSummary_dlg)
  m_area_e = _T("");
  m_area_n = _T("");
  m_area_s = _T("");
  m_area_w = _T("");
  m_count = 0;
  m_high = 0;
  m_low = 0;
  //}}AFX_DATA_INIT
}

CFeatureListSummary_dlg::CFeatureListSummary_dlg(CFeatureListView *parent,
						 UINT resTpl)
  : CDialog(resTpl,parent)
{
  init();
  m_count=parent->GetListCtrl().GetItemCount();
  for (int item=0; item<m_count; ++item) {
    const Feature& feature=parent->GetFeature(item);
    elev|=feature.elev;
    area|=feature.location;
  }
  summary_fmt=parent->list_fmt.loc_f;
}

CFeatureListSummary_dlg::CFeatureListSummary_dlg(CLPRtreeView *parent)
  : CDialog(CFeatureListSummary_dlg::IDD,parent)
{
  init();
  parent->BranchSummary(parent->GetTreeCtrl().GetRootItem(),m_count,elev,area);
  summary_fmt=parent->tree_fmt.loc_f;
}


void CFeatureListSummary_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CFeatureListSummary_dlg)
  DDX_Text(pDX, IDC_AREA_E, m_area_e);
  DDX_Text(pDX, IDC_AREA_N, m_area_n);
  DDX_Text(pDX, IDC_AREA_S, m_area_s);
  DDX_Text(pDX, IDC_AREA_W, m_area_w);
  DDX_Text(pDX, IDC_COUNT, m_count);
  DDX_Text(pDX, IDC_HIGH, m_high);
  DDX_Text(pDX, IDC_LOW, m_low);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeatureListSummary_dlg, CDialog)
  //{{AFX_MSG_MAP(CFeatureListSummary_dlg)
  ON_BN_CLICKED(IDC_FORMAT, OnFormat)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFeatureListSummary_dlg message handlers

void CFeatureListSummary_dlg::reformat()
{
  ((GridPoint)area.corner2()).sprint1(buf);
  m_area_n=buf;
  ((GridPoint)area.corner1()).sprint1(buf);
  m_area_s=buf;
  ((GridPoint)area.corner2()).sprint2(buf);
  m_area_e=buf;
  ((GridPoint)area.corner1()).sprint2(buf);
  m_area_w=buf;
}

BOOL CFeatureListSummary_dlg::OnInitDialog() 
{
  m_high=elev.high;
  m_low=elev.low;

  summary_fmt.set();
  reformat();

  CDialog::OnInitDialog();

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}

void CFeatureListSummary_dlg::OnFormat() 
{
  CLocationFmt_dlg dlg(this,summary_fmt);
  if (dlg.DoModal()==IDOK) {
    summary_fmt.set();
    reformat();
    UpdateData(FALSE);
  }
}
