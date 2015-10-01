// IdentifyFeature_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "IdentifyFeature_dlg.h"
#include "GraphView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CIdentifyFeature_dlg dialog

CIdentifyFeature_dlg::CIdentifyFeature_dlg()
  : CDialog(CIdentifyFeature_dlg::IDD,NULL)
{
  //{{AFX_DATA_INIT(CIdentifyFeature_dlg)
  m_center_target = FALSE;
  m_select_target = FALSE;
  //}}AFX_DATA_INIT
}


void CIdentifyFeature_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CIdentifyFeature_dlg)
  DDX_Check(pDX, IDC_CENTER_TARGET, m_center_target);
  DDX_Check(pDX, IDC_SELECT_TARGET, m_select_target);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIdentifyFeature_dlg, CDialog)
  //{{AFX_MSG_MAP(CIdentifyFeature_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Message handlers

BOOL CIdentifyFeature_dlg::OnInitDialog() 
{
  m_center_target=CGraphView::center_target;
  m_select_target=CGraphView::select_target;

  CDialog::OnInitDialog();

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}
