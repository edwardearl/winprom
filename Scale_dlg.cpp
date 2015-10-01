// Scale_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "Scale_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScale_dlg dialog

CScale_dlg::CScale_dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScale_dlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CScale_dlg)
  m_scale = 0.0f;
  //}}AFX_DATA_INIT
}

void CScale_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CScale_dlg)
  DDX_Text(pDX, IDC_SCALE, m_scale);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScale_dlg, CDialog)
  //{{AFX_MSG_MAP(CScale_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CScale_dlg message handlers

void CScale_dlg::OnOK()
{
  if (!UpdateData()) return;
  if (m_scale<=0) {
    MessageBox("Scale must be greater than 0.","Winprom Scale Error",MB_OK|MB_ICONERROR);
    return;
  }
  CDialog::OnOK();
}
