// ParentType_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "ParentType_dlg.h"
#include "PromSadl_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CParentType_dlg dialog

CParentType_dlg::CParentType_dlg(const CPromDoc& d,
				 CParentListView::List_info& li,
				 CWnd *pParent /*=NULL*/)
  : CDialog(CParentType_dlg::IDD, pParent),doc(d),list_info(li)
{
  //{{AFX_DATA_INIT(CParentType_dlg)
  m_runoffs = FALSE;
  m_parent_type = -1;
  m_prom = 0;
  //}}AFX_DATA_INIT
}

void CParentType_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CParentType_dlg)
  DDX_Control(pDX, IDC_PS_FILTER, m_filter_butn);
  DDX_Check(pDX, IDC_RUNOFFS, m_runoffs);
  DDX_Radio(pDX, IDC_PT_HEIGHT, m_parent_type);
  DDX_Text(pDX, IDC_PROM, m_prom);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParentType_dlg, CDialog)
  //{{AFX_MSG_MAP(CParentType_dlg)
  ON_BN_CLICKED(IDC_PS_FILTER, OnFilter)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CParentType_dlg message handlers

BOOL CParentType_dlg::OnInitDialog() 
{
  m_parent_type=list_info.topology-1;
  m_prom=list_info.min_prom;
  m_filter=list_info.filter;
  m_runoffs=list_info.runoffs;

  CDialog::OnInitDialog();

  m_filter_butn.EnableWindow(list_info.sadls.empty());

  return TRUE;  // return TRUE unless you set the focus to a control
}

void CParentType_dlg::OnFilter()
{
  CPromSadl_dlg dlg(doc,m_filter,this);
  dlg.parent=true;
  dlg.DoModal();
}

void CParentType_dlg::OnOK()
{
  CDialog::OnOK();

  list_info.topology=(Domain::Topology)(m_parent_type+1);
  list_info.min_prom=m_prom;
  list_info.filter=m_filter;
  list_info.runoffs=m_runoffs!=FALSE;
}
