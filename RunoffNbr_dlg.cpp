// RunoffNbr_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "RunoffNbr_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CRunoffNbr_dlg dialog

CRunoffNbr_dlg::CRunoffNbr_dlg(CWnd *parent, Runoff_nbr_type ront, float radius)
  : CDialog(CRunoffNbr_dlg::IDD,parent)
{
  //{{AFX_DATA_INIT(CRunoffNbr_dlg)
  m_radius = 0.0f;
  m_grid = -1;
  //}}AFX_DATA_INIT
  m_radius=radius;
  m_grid=ront;
}


void CRunoffNbr_dlg::DoDataExchange(CDataExchange *pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CRunoffNbr_dlg)
  DDX_Text(pDX, IDC_RADIUS_TEXT, m_radius);
  DDX_Radio(pDX, IDC_RN_SAME_GP, m_grid);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRunoffNbr_dlg, CDialog)
  //{{AFX_MSG_MAP(CRunoffNbr_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRunoffNbr_dlg message handlers
