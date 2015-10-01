// PromCutoff_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "PromCutoff_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPromCutoff_dlg dialog

CPromCutoff_dlg::CPromCutoff_dlg(CWnd* pParent /*=NULL*/)
  : CCutoffOnly_dlg(CPromCutoff_dlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CPromCutoff_dlg)
  m_offmap = FALSE;
  m_cell_rot = FALSE;
  m_ridge_rot = FALSE;
  //}}AFX_DATA_INIT
}

void CPromCutoff_dlg::DoDataExchange(CDataExchange* pDX)
{
  CCutoffOnly_dlg::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPromCutoff_dlg)
  DDX_Check(pDX, IDC_OFFMAP, m_offmap);
  DDX_Check(pDX, IDC_CELL, m_cell_rot);
  DDX_Check(pDX, IDC_RIDGE, m_ridge_rot);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPromCutoff_dlg, CCutoffOnly_dlg)
  //{{AFX_MSG_MAP(CPromCutoff_dlg)
    // NOTE: the ClassWizard will add message map macros here
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPromCutoff_dlg message handlers
