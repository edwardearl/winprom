// ProfileParam_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "ProfileParam_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CProfileParam_dlg dialog

CProfileParam_dlg::CProfileParam_dlg(CProfileView *view)
  : CDialog(CProfileParam_dlg::IDD,NULL),pView(view)
{
  //{{AFX_DATA_INIT(CProfileParam_dlg)
  m_min_prom = 0;
  m_runoff_ue = -1;
  m_scale_type = -1;
  m_sadl_pos = -1;
  //}}AFX_DATA_INIT
}

void CProfileParam_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CProfileParam_dlg)
  DDX_Text(pDX, IDC_MIN_PROM, m_min_prom);
  DDX_Radio(pDX, IDC_RUNOFF_HIGH, m_runoff_ue);
  DDX_Radio(pDX, IDC_SCALE_LINEAR, m_scale_type);
  DDX_Radio(pDX, IDC_EQUAL_ANGLE, m_sadl_pos);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProfileParam_dlg, CDialog)
  //{{AFX_MSG_MAP(CProfileParam_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CProfileParam_dlg message handlers

BOOL CProfileParam_dlg::OnInitDialog()
{
  m_min_prom=pView->prom_range.low;
  m_scale_type=pView->current.scale_type;
  m_sadl_pos=pView->current.sadl_pos;
  m_runoff_ue=pView->current.ue_runoff;

  CDialog::OnInitDialog();

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}

void CProfileParam_dlg::OnOK()
{
  if (!UpdateData(TRUE)) return;
  if (m_scale_type!=0 && (m_min_prom<=0)) {
    MessageBox("With a nonlonear prominence scale, minimum\n"
	       "prominence must be greater than zero.",
	       "Winprom Profile Parameter Error",MB_OK|MB_ICONERROR);
    return;
  }
  if (m_min_prom>pView->prom_range.high) {
    sprintf(CPromDoc::buf,"Minimum prominence must be less\n"
	    "than maximum prominence of %d.",pView->prom_range.high);
    MessageBox(CPromDoc::buf,"Winprom Profile Parameter Error",
	       MB_OK|MB_ICONERROR);
    return;
  }

  pView->prom_range.low=pView->current.min_prom=m_min_prom;
  pView->current.scale_type=(CProfileView::Scale_type)m_scale_type;
  pView->current.sadl_pos=m_sadl_pos;
  pView->current.ue_runoff=(CPromDoc::Use_elev)m_runoff_ue;

  CDialog::EndDialog(IDOK);
}
