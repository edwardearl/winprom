// SetProfile_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "SetProfile_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSetProfile_dlg dialog

CSetProfile_dlg::CSetProfile_dlg()
  : CDialog(CSetProfile_dlg::IDD,NULL)
{
  //{{AFX_DATA_INIT(CSetProfile_dlg)
  m_elev = 0;
  m_prom = 0;
  //}}AFX_DATA_INIT
}


void CSetProfile_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CSetProfile_dlg)
  DDX_Text(pDX, IDC_ELEVATION, m_elev);
  DDX_Text(pDX, IDC_PROMINENCE, m_prom);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetProfile_dlg, CDialog)
  //{{AFX_MSG_MAP(CSetProfile_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSetProfile_dlg message handlers
