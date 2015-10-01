// DefineFlat_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "DefineFlat_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDefineFlat_dlg dialog

CDefineFlat_dlg::CDefineFlat_dlg(const char *t, Rectangl& rect, CWnd* pParent /*=NULL*/)
  : CCrop_dlg(t,rect,CDefineFlat_dlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CDefineFlat_dlg)
  m_high = _T("");
  m_low = _T("");
  //}}AFX_DATA_INIT
  check_interval=false;
}

void CDefineFlat_dlg::DoDataExchange(CDataExchange* pDX)
{
  CCrop_dlg::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDefineFlat_dlg)
  DDX_Control(pDX, IDC_ELEV_LOW, m_low_ctl);
  DDX_Control(pDX, IDC_ELEV_HIGH, m_high_ctl);
  DDX_Text(pDX, IDC_ELEV_HIGH, m_high);
  DDX_Text(pDX, IDC_ELEV_LOW, m_low);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDefineFlat_dlg, CCrop_dlg)
  //{{AFX_MSG_MAP(CDefineFlat_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDefineFlat_dlg message handlers

void CDefineFlat_dlg::OnOK()
{
  if (!UpdateData(TRUE)) return;

  const char *buf;
  // Check elevation limits

  if (m_high.IsEmpty())
    elev_lmt.high=elev_infty;
  else {
    buf=m_high;
    elev_lmt.high=(Elevation)strtol(buf,(char **)&buf,10);
    if (*buf) {
      MessageBox("Enter a number for the highest elevation\n"
		 "or leave blank if no restriction",
		 "Winprom Entry Error",MB_OK|MB_ICONERROR);
      m_high_ctl.SetFocus();
      m_high_ctl.SetSel(0,-1);
      return;
    }
  }

  if (m_low.IsEmpty())
    elev_lmt.low=-elev_infty;
  else {
    buf=m_low;
    elev_lmt.low=(Elevation)strtol(buf,(char **)&buf,10);
    if (*buf) {
      MessageBox("Enter a number for the lowest elevation\n"
		 "or leave blank if no restriction",
		 "Winprom Entry Error",MB_OK|MB_ICONERROR);
      m_low_ctl.SetFocus();
      m_low_ctl.SetSel(0,-1);
      return;
    }
  }

  if (elev_lmt.is_empty()) {
    MessageBox("Highest elevation must be greater than\n"
	       "or equal to lowest elevation",
	       "Winprom Entry Error",MB_OK|MB_ICONERROR);
    return;
  }

  check_ok();
}
