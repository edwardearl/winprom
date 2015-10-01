// TransformTree_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "TransformTree_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTransformTree_dlg dialog

CTransformTree_dlg::CTransformTree_dlg(CWnd* pParent /*=NULL*/)
  : CTransform_dlg(CTransformTree_dlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CTransformTree_dlg)
  m_edited = TRUE;
  m_peaks = FALSE;
  m_runoffs = FALSE;
  m_sadls = FALSE;
  m_basnsadls = FALSE;
  //}}AFX_DATA_INIT
}

void CTransformTree_dlg::DoDataExchange(CDataExchange* pDX)
{
  CTransform_dlg::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CTransformTree_dlg)
  DDX_Check(pDX, IDC_EDITED, m_edited);
  DDX_Check(pDX, IDC_PEAKS, m_peaks);
  DDX_Check(pDX, IDC_RUNOFFS, m_runoffs);
  DDX_Check(pDX, IDC_SADLS, m_sadls);
  DDX_Check(pDX, IDC_BASNSADLS, m_basnsadls);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransformTree_dlg, CTransform_dlg)
  //{{AFX_MSG_MAP(CTransformTree_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTransformTree_dlg message handlers

void CTransformTree_dlg::OnOK()
{
  UpdateData();
  if (m_mult!=1 && (m_peaks!=m_sadls || m_peaks!=m_runoffs || m_peaks!=m_basnsadls)) {
    if (MessageBox("You have a coefficient not equal to 1\n"
		   "and you apply the transformation to\n"
		   "some but not all kinds of features.\n"
		   "Is this really what you want?",
		   NULL,MB_ICONWARNING|MB_YESNO)!=IDYES) return;
  }

  EndDialog(IDOK);
}
