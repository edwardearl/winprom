// PromSadl_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "PromSadl_dlg.h"
#include "FeatureFilter_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPromSadl_dlg dialog

CPromSadl_dlg::CPromSadl_dlg(const CPromDoc& doc, Prom_sadl_filter& f, CWnd *pParent)
  : CDialog(CPromSadl_dlg::IDD,pParent),filter(f),document(doc)
{
  //{{AFX_DATA_INIT(CPromSadl_dlg)
  m_min_prom = 0;
  m_UPE = FALSE;
  m_runoffs = FALSE;
  m_offmap_saddles = FALSE;
  m_basnsadls = FALSE;
  //}}AFX_DATA_INIT
  parent=false;
}

void CPromSadl_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPromSadl_dlg)
  DDX_Control(pDX, IDC_RUNOFF_FILTER, m_ro_filter_butn);
  DDX_Control(pDX, IDC_UPE, m_upe_butn);
  DDX_Control(pDX, IDC_RUNOFFS, m_runoff_butn);
  DDX_Control(pDX, IDC_BASNSADL, m_bs_butn);
  DDX_Text(pDX, IDC_MIN_PROM, m_min_prom);
  DDX_Check(pDX, IDC_UPE, m_UPE);
  DDX_Check(pDX, IDC_RUNOFFS, m_runoffs);
  DDX_Check(pDX, IDC_OFFMAP_SADDLES, m_offmap_saddles);
  DDX_Check(pDX, IDC_BASNSADL, m_basnsadls);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPromSadl_dlg, CDialog)
  //{{AFX_MSG_MAP(CPromSadl_dlg)
  ON_BN_CLICKED(IDC_SADDLE_FILTER, OnSaddleFilter)
  ON_BN_CLICKED(IDC_RUNOFF_FILTER, OnRunoffFilter)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPromSadl_dlg message handlers

void CPromSadl_dlg::OnSaddleFilter()
{
  CFeatureFilter_dlg dlg(m_saddle_filter,this);
  dlg.DoModal();
}

void CPromSadl_dlg::OnRunoffFilter() 
{
  CFeatureFilter_dlg dlg(m_runoff_filter,this);
  dlg.DoModal();
}

BOOL CPromSadl_dlg::OnInitDialog()
{
  m_min_prom=filter.min_prom;
  m_saddle_filter=filter.saddle_filter;
  m_saddle_filter.rec_filter.align(document.data);
  m_runoff_filter=filter.runoff_filter;
  m_runoff_filter.rec_filter.align(document.data);
  m_offmap_saddles=filter.off_sadls;
  m_runoffs=filter.runoffs;
  m_basnsadls=filter.basinsadls;

  CDialog::OnInitDialog();

  if (parent) {
    m_upe_butn.EnableWindow(FALSE);
    m_ro_filter_butn.EnableWindow(FALSE);
    m_runoff_butn.EnableWindow(FALSE);
    m_bs_butn.EnableWindow(FALSE);
  }
  SetIcon(AfxGetApp()->
    LoadIcon(document.is_drainage?IDI_BASINSADL_LIST:IDI_PROMSADL_LIST),TRUE);
  SetWindowText(document.is_drainage?"Basin Saddles":"Prominence Saddles");

  return TRUE;  // return TRUE unless you set the focus to a control
}

void CPromSadl_dlg::OnOK()
{
  CDialog::OnOK();

  filter.min_prom=m_min_prom;
  filter.saddle_filter=m_saddle_filter;
  filter.runoff_filter=m_runoff_filter;
  filter.off_sadls=m_offmap_saddles!=FALSE;
  filter.runoffs=m_runoffs!=FALSE;
  filter.basinsadls=m_basnsadls!=FALSE;
}
