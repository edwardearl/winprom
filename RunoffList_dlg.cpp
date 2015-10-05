// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// RunoffList_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "RunoffList_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// RunoffFilter implementation

bool RunoffFilter::test(const Runoff& ro, const Database& data) const
{
  if (!FeatureFilter::test(ro,data)) return false;
  switch (ro.interior_quads) {
  case 0: if (!IQ_0) return false; break;
  case 1: if (!IQ_1) return false; break;
  case 2: if (!IQ_2) return false; break;
  case 3: if (!IQ_3) return false; break;
  }
  if (ro.location.quad_corner()) {
    if (!corner) return false;
  }
  else if (ro.location.quad_edge()) {
    if (!edge) return false;
  }
  else {
    if (!interior) return false;
  }

  return true;
}


/////////////////////////////////////////////////////////////////////////////
// CRunoffList_dlg dialog

CRunoffList_dlg::CRunoffList_dlg(const Database& db, RunoffFilter& rf,
				 bool drg, CWnd *parent)
	: CDialog(CRunoffList_dlg::IDD,parent),data(db),
	  filter(rf),temp_ff(rf),drainage(drg)
{
  //{{AFX_DATA_INIT(CRunoffList_dlg)
  m_edge = FALSE;
  m_interior = FALSE;
  m_IQ_1 = FALSE;
  m_IQ_2 = FALSE;
  m_IQ_3 = FALSE;
  m_corner = FALSE;
  m_IQ_0 = FALSE;
  //}}AFX_DATA_INIT
}


void CRunoffList_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CRunoffList_dlg)
  DDX_Check(pDX, IDC_EDGE, m_edge);
  DDX_Check(pDX, IDC_INTERIOR, m_interior);
  DDX_Check(pDX, IDC_IQ_1, m_IQ_1);
  DDX_Check(pDX, IDC_IQ_2, m_IQ_2);
  DDX_Check(pDX, IDC_IQ_3, m_IQ_3);
  DDX_Check(pDX, IDC_CORNERS, m_corner);
  DDX_Check(pDX, IDC_IQ_0, m_IQ_0);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRunoffList_dlg, CDialog)
  //{{AFX_MSG_MAP(CRunoffList_dlg)
  ON_BN_CLICKED(IDC_FILTER, OnFilter)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRunoffList_dlg message handlers

BOOL CRunoffList_dlg::OnInitDialog()
{
  temp_ff=filter;
  temp_ff.rec_filter.align(data);
  m_IQ_0=filter.IQ_0;
  m_IQ_1=filter.IQ_1;
  m_IQ_2=filter.IQ_2;
  m_IQ_3=filter.IQ_3;
  m_corner=filter.corner;
  m_edge=filter.edge;
  m_interior=filter.interior;

  CDialog::OnInitDialog();

  SetIcon(AfxGetApp()->LoadIcon(drainage?IDI_SBRO:IDI_RUNOFF),TRUE);

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}

void CRunoffList_dlg::OnFilter()
{
  CFeatureFilter_dlg dlg(temp_ff,this);
  dlg.DoModal();
}

void CRunoffList_dlg::OnOK()
{
  CDialog::OnOK();

  (FeatureFilter&)filter=temp_ff;
  filter.IQ_0=m_IQ_0!=FALSE;
  filter.IQ_1=m_IQ_1!=FALSE;
  filter.IQ_2=m_IQ_2!=FALSE;
  filter.IQ_3=m_IQ_3!=FALSE;
  filter.corner=m_corner!=FALSE;
  filter.edge=m_edge!=FALSE;
  filter.interior=m_interior!=FALSE;
}
