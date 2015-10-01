// PurgeNoise_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "PurgeNoise_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPurgeNoise_dlg dialog

CPurgeNoise_dlg::CPurgeNoise_dlg(bool drain, CWnd* pParent /*=NULL*/)
  : CDialog(CPurgeNoise_dlg::IDD, pParent),drainage(drain)
{
  //{{AFX_DATA_INIT(CPurgeNoise_dlg)
  m_noise = 0.0f;
  m_label = _T("");
  m_resid_nc = FALSE;
  m_hess_nc = FALSE;
  m_keep_edit = FALSE;
  m_keep_name = FALSE;
  //}}AFX_DATA_INIT
}

void CPurgeNoise_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPurgeNoise_dlg)
  DDX_Text(pDX, IDC_NOISE, m_noise);
  DDX_Text(pDX, IDC_LABEL, m_label);
  DDX_Check(pDX, IDC_RESID_NC, m_resid_nc);
  DDX_Check(pDX, IDC_HESS_NC, m_hess_nc);
  DDX_Check(pDX, IDC_EDITED, m_keep_edit);
  DDX_Check(pDX, IDC_NAME, m_keep_name);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPurgeNoise_dlg, CDialog)
  //{{AFX_MSG_MAP(CPurgeNoise_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPurgeNoise_dlg message handlers

BOOL CPurgeNoise_dlg::OnInitDialog()
{
  m_label=drainage?"Keep basins for which":"Keep peaks for which";
  CDialog::OnInitDialog();
  SetIcon(AfxGetApp()->LoadIcon(drainage?IDI_BASIN:IDI_PEAK),TRUE);

  return TRUE;  // return TRUE unless you set the focus to a control
}
