// DomainView_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "DomainView_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDomainView_dlg dialog

CDomainView_dlg::CDomainView_dlg()
  : CCutoffOnly_dlg(CDomainView_dlg::IDD,NULL)
{
  //{{AFX_DATA_INIT(CDomainView_dlg)
  m_enable = FALSE;
  m_rotation = FALSE;
  m_offmap = FALSE;
  m_edge_sadl = FALSE;
  //}}AFX_DATA_INIT
}

void CDomainView_dlg::DoDataExchange(CDataExchange* pDX)
{
  CCutoffOnly_dlg::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDomainView_dlg)
  DDX_Check(pDX, IDC_ENABLE, m_enable);
  DDX_Check(pDX, IDC_ROTATION, m_rotation);
  DDX_Check(pDX, IDC_OFFMAP, m_offmap);
  DDX_Check(pDX, IDC_EDGE_SADL, m_edge_sadl);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDomainView_dlg, CCutoffOnly_dlg)
  //{{AFX_MSG_MAP(CDomainView_dlg)
  ON_BN_CLICKED(IDC_DOMAIN_COLOR, OnDomainColor)
  ON_BN_CLICKED(IDC_VOID_COLOR, OnVoidColor)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDomainView_dlg message handlers

BOOL CDomainView_dlg::OnInitDialog()
{
  CCutoffOnly_dlg::OnInitDialog();

  m_box_domain.SubclassDlgItem(IDC_BOX_DOMAIN,this);
  m_box_domain.set_color(m_color_domain);
  m_box_void.SubclassDlgItem(IDC_BOX_VOID,this);
  m_box_void.set_color(m_color_void);

  return TRUE;  // return TRUE unless you set the focus to a control
}

void CDomainView_dlg::OnDomainColor()
{
  CViewColorDialog dlg(m_color_domain,"Domain Boundary Color");
  if (dlg.DoModal()!=IDOK) return;

  m_color_domain=dlg.GetColor();
  m_box_domain.set_color(m_color_domain);
}

void CDomainView_dlg::OnVoidColor()
{
  CViewColorDialog dlg(m_color_void,"Void Domain Color");
  if (dlg.DoModal()!=IDOK) return;

  m_color_void=dlg.GetColor();
  m_box_void.set_color(m_color_void);
}
