// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// ErrorFilter_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "ErrorFilter_dlg.h"
#include "ErrorAnal_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// ErrorFilter implementation

bool ErrorFilter::test(const Feature& featr, FT_index ft,
		       Elevation err, const Database& data) const
{
  if (!FeatureTypeFilter::test(featr,ft,data) || err==elev_undef)
    return false;
  return abs(err)>=min_error;
}


/////////////////////////////////////////////////////////////////////////////
// CErrorFilter_dlg dialog

CErrorFilter_dlg::CErrorFilter_dlg(ErrorFilter& ef, CPromDoc& doc, CWnd *pParent)
  : CDialog(CErrorFilter_dlg::IDD,pParent),filter(ef),document(doc)
{
  //{{AFX_DATA_INIT(CErrorFilter_dlg)
  m_min_error = 0;
  m_reference = -1;
  //}}AFX_DATA_INIT
  doc.error_stats.peak_data.filter.rec_filter.align(doc.data);
  doc.error_stats.sadl_data.filter.rec_filter.align(doc.data);
}

void CErrorFilter_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CErrorFilter_dlg)
  DDX_Text(pDX, IDC_MIN_ERROR, m_min_error);
  DDX_Radio(pDX, IDC_REF_MIDPT, m_reference);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CErrorFilter_dlg, CDialog)
  //{{AFX_MSG_MAP(CErrorFilter_dlg)
  ON_BN_CLICKED(IDC_LIST_FILTER, OnListFilter)
  ON_BN_CLICKED(IDC_STAT, OnStat)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CErrorFilter_dlg message handlers

BOOL CErrorFilter_dlg::OnInitDialog()
{
  m_min_error=filter.min_error;
  m_reference=filter.reference;
  m_filter=filter;
  m_filter.rec_filter.align(document.data);
  m_error_stats=document.error_stats;

  CDialog::OnInitDialog();

  return TRUE;
}

void CErrorFilter_dlg::OnOK()
{
  CDialog::OnOK();

  (FeatureTypeFilter&)filter=m_filter;
  filter.min_error=m_min_error;
  filter.reference=m_reference;
  document.error_stats=m_error_stats;
}

void CErrorFilter_dlg::OnListFilter()
{
  CFeatureTypeFilter_dlg dlg(m_filter,this);
  dlg.DoModal();
}

void CErrorFilter_dlg::OnStat()
{
  CErrorAnal_dlg dlg(m_error_stats,document);
  dlg.DoModal();
}
