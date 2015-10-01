// TransPal_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "TransPal_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTransPal_dlg dialog

CTransPal_dlg::CTransPal_dlg(CWnd* pParent /*=NULL*/)
  : CDialog(CTransPal_dlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CTransPal_dlg)
  m_high_old = _T("");
  m_high_new = 0;
  m_low_new = 0;
  m_low_old = _T("");
  //}}AFX_DATA_INIT
}

void CTransPal_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CTransPal_dlg)
  DDX_Text(pDX, IDC_HIGH_OLD, m_high_old);
  DDX_Text(pDX, IDC_HIGH_NEW, m_high_new);
  DDX_Text(pDX, IDC_LOW_NEW, m_low_new);
  DDX_Text(pDX, IDC_LOW_OLD, m_low_old);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransPal_dlg, CDialog)
  //{{AFX_MSG_MAP(CTransPal_dlg)
    // NOTE: the ClassWizard will add message map macros here
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTransPal_dlg message handlers
