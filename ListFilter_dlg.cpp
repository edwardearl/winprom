// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// ListFilter_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "ListFilter_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void PromListItem::transform(float m)
{
  prom.transform(m,0);
  if (offmap_prom!=elev_infty)
    offmap_prom=nint(offmap_prom*m);
}


/////////////////////////////////////////////////////////////////////////////
// CListFilter_dlg dialog

void CListFilter_dlg::data_init()
{
  //{{AFX_DATA_INIT(CListFilter_dlg)
  m_prom = 0;
  m_offmap = FALSE;
  //}}AFX_DATA_INIT
}

CListFilter_dlg::CListFilter_dlg(const Database& db, ListFilter& lf,
				 UINT icon_id, CWnd *pParent)
  : CDialog(CListFilter_dlg::IDD,pParent),feature_icon_id(icon_id),
    data(db),filter(lf),temp_ff(lf)
{
  data_init();
}

CListFilter_dlg::CListFilter_dlg(UINT nID, const Database& db, ListFilter& lf,
				 UINT icon_id, CWnd *pParent)
  : CDialog(nID,pParent),feature_icon_id(icon_id),
    data(db),filter(lf),temp_ff(lf)
{
  data_init();
}

void CListFilter_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CListFilter_dlg)
  DDX_Text(pDX, IDC_MIN_PROM, m_prom);
  DDX_Check(pDX, IDC_OFFMAP, m_offmap);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListFilter_dlg, CDialog)
  //{{AFX_MSG_MAP(CListFilter_dlg)
  ON_BN_CLICKED(IDC_FILTER, OnFilter)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CListFilter_dlg message handlers

BOOL CListFilter_dlg::OnInitDialog() 
{
  temp_ff=filter;
  temp_ff.rec_filter.align(data);
  m_prom=filter.min_prom;
  m_offmap=filter.offmap;

  CDialog::OnInitDialog();

  if (feature_icon_id) SetIcon(AfxGetApp()->LoadIcon(feature_icon_id),TRUE);

  return TRUE;
}

void CListFilter_dlg::OnFilter() 
{
  CFeatureFilter_dlg dlg(temp_ff,this);
  dlg.DoModal();
}

void CListFilter_dlg::OnOK() 
{
  CDialog::OnOK();

  (FeatureFilter&)filter=temp_ff;
  filter.min_prom=m_prom;
  filter.offmap=m_offmap!=FALSE;
}
