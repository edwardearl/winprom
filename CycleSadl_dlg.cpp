// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// CycleSadl_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "CycleSadl_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CycleFilter::CycleFilter(Elevation mp)
  : bs(0),nonoffend(false)
{
  min_prom=mp;
  store(0);
}

CycleFilter::CycleFilter(Basin_saddle& bs_)
  : bs(0),nonoffend(false)
{
  min_prom=-1;
  store(1);
  bs[0]=&bs_;
}

void CycleFilter::store(Domain::Index count)
{
  delete[] bs;
  bs=new Basin_saddle *[nbs=count];
}

void CycleFilter::transfer(CycleFilter& a)
{
  delete[] bs;
  *this=a;
  a.nbs=0;
  a.bs=0;
}


/////////////////////////////////////////////////////////////////////////////
// CCycleSadl_dlg dialog

CCycleSadl_dlg::CCycleSadl_dlg(const Database& db, CycleFilter& cf, CWnd *pParent)
  : CListSadl_dlg(CCycleSadl_dlg::IDD,db,cf,IDI_CYCLE,pParent),filter(cf)
{
  //{{AFX_DATA_INIT(CCycleSadl_dlg)
  m_nonoffend = FALSE;
  //}}AFX_DATA_INIT
}

void CCycleSadl_dlg::DoDataExchange(CDataExchange* pDX)
{
  CListSadl_dlg::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CCycleSadl_dlg)
  DDX_Check(pDX, IDC_CWR_NONOFFEND, m_nonoffend);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCycleSadl_dlg, CListSadl_dlg)
  //{{AFX_MSG_MAP(CCycleSadl_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCycleSadl_dlg message handlers

BOOL CCycleSadl_dlg::OnInitDialog() 
{
  m_nonoffend=filter.nonoffend;

  CListSadl_dlg::OnInitDialog();

  return TRUE;
}

void CCycleSadl_dlg::OnOK() 
{
  CListSadl_dlg::OnOK();

  filter.nonoffend=m_nonoffend!=FALSE;
}
