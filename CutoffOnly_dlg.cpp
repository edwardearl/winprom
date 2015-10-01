// CutoffOnly_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "CutoffOnly_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCutoffOnly_dlg dialog

void CCutoffOnly_dlg::init_data()
{
  //{{AFX_DATA_INIT(CCutoffOnly_dlg)
  m_major = _T("");
  m_minor = _T("");
  m_standard = _T("");
  //}}AFX_DATA_INIT
}

CCutoffOnly_dlg::CCutoffOnly_dlg(CWnd* pParent /*=NULL*/)
  : CDialog(CCutoffOnly_dlg::IDD,pParent)
{
  init_data();
}

CCutoffOnly_dlg::CCutoffOnly_dlg(UINT nID, CWnd* pParent /*=NULL*/)
  : CDialog(nID,pParent)
{
  init_data();
}

void CCutoffOnly_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CCutoffOnly_dlg)
  DDX_Text(pDX, IDC_MAJOR, m_major);
  DDV_MaxChars(pDX, m_major, 6);
  DDX_Text(pDX, IDC_MINOR, m_minor);
  DDV_MaxChars(pDX, m_minor, 6);
  DDX_Text(pDX, IDC_STANDARD, m_standard);
  DDV_MaxChars(pDX, m_standard, 6);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCutoffOnly_dlg, CDialog)
  //{{AFX_MSG_MAP(CCutoffOnly_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCutoffOnly_dlg message handlers

BOOL CCutoffOnly_dlg::OnInitDialog() 
{
  if (major<elev_infty) m_major.Format("%d",major);
  if (standard<major) m_standard.Format("%d",standard);
  if (minor<standard) m_minor.Format("%d",minor);

  CDialog::OnInitDialog();

  return TRUE;  // return TRUE unless you set the focus to a control
}

bool CCutoffOnly_dlg::close()
{
  if (strlen(m_major)==0) major=elev_infty;
  else {
    major=atoi(m_major);
    if (major>=elev_infty) {
      MessageBox("Major prominence must be <= 32511",
		 "Winprom Feature View Error",MB_OK|MB_ICONERROR);
      return false;
    }
  }
  if (strlen(m_standard)==0) standard=major;
  else standard=atoi(m_standard);
  if (strlen(m_minor)==0) minor=standard;
  else minor=atoi(m_minor);

  if (major<standard || standard<minor) {
    MessageBox("Major prominence must be >= standard,\n"
	       "and standard must be >= minor",
	       "Winprom Feature View Error",MB_OK|MB_ICONERROR);
    return false;
  }

  return true;
}

void CCutoffOnly_dlg::OnOK() 
{
  if (!UpdateData() || !close()) return;

  EndDialog(IDOK);
}
