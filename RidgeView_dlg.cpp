// RidgeView_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "RidgeView_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CRidgeView_dlg dialog

CRidgeView_dlg::CRidgeView_dlg(bool lt, CWnd *pParent /*=NULL*/)
  : CCutoffOnly_dlg(CRidgeView_dlg::IDD,pParent)
{
  //{{AFX_DATA_INIT(CRidgeView_dlg)
  m_runoff = FALSE;
  m_basin_sadl = FALSE;
  m_peak = FALSE;
  m_saddle = FALSE;
  m_ridge_rot = FALSE;
  m_topology = -1;
  //}}AFX_DATA_INIT
  local_topo=lt;
}

void CRidgeView_dlg::DoDataExchange(CDataExchange* pDX)
{
  CCutoffOnly_dlg::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CRidgeView_dlg)
  DDX_Control(pDX, IDC_SADDLE, m_sadl_butn);
  DDX_Control(pDX, IDC_PEAK, m_peak_butn);
  DDX_Check(pDX, IDC_RUNOFF, m_runoff);
  DDX_Check(pDX, IDC_BASIN_SADL, m_basin_sadl);
  DDX_Check(pDX, IDC_PEAK, m_peak);
  DDX_Check(pDX, IDC_SADDLE, m_saddle);
  DDX_Check(pDX, IDC_RIDGE, m_ridge_rot);
  DDX_Radio(pDX, IDC_MORSE_SMALE, m_topology);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRidgeView_dlg, CCutoffOnly_dlg)
  //{{AFX_MSG_MAP(CRidgeView_dlg)
  ON_BN_CLICKED(IDC_PEAK, OnPeak)
  ON_BN_CLICKED(IDC_SADDLE, OnSaddle)
  ON_BN_CLICKED(IDC_COLOR, OnColor)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRidgeView_dlg message handlers

BOOL CRidgeView_dlg::OnInitDialog()
{
  CCutoffOnly_dlg::OnInitDialog();

  if (!local_topo) {
    GetDlgItem(IDC_MORSE_SMALE)->EnableWindow(FALSE);
  }

  m_color_box.SubclassDlgItem(IDC_COLOR_BOX,this);
  m_color_box.set_color(m_color);

  return TRUE;  // return TRUE unless you set the focus to a control
}

void CRidgeView_dlg::OnPeak() 
{
  if (m_peak_butn.GetCheck()) m_sadl_butn.SetCheck(0);
}

void CRidgeView_dlg::OnSaddle() 
{
  if (m_sadl_butn.GetCheck()) m_peak_butn.SetCheck(0);
}

void CRidgeView_dlg::OnColor()
{
  CViewColorDialog dlg(m_color,"Ridge Color");
  if (dlg.DoModal()!=IDOK) return;

  m_color=dlg.GetColor();
  m_color_box.set_color(m_color);
}
