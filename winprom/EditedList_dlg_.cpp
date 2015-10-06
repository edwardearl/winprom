// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// EditedList_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "EditedList_dlg.h"
#include "FeatureFilter_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CEditedList_dlg dialog

CEditedList_dlg::CEditedList_dlg(const Database& db, EditFilter &ef, CWnd* pParent /*=NULL*/)
  : CDialog(CEditedList_dlg::IDD, pParent),filter(ef)
{
  //{{AFX_DATA_INIT(CEditedList_dlg)
  m_elev = FALSE;
  m_name = FALSE;
  //}}AFX_DATA_INIT
  filter.rec_filter.align(db);
}

void CEditedList_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CEditedList_dlg)
  DDX_Check(pDX, IDC_ELEV, m_elev);
  DDX_Check(pDX, IDC_NAME, m_name);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditedList_dlg, CDialog)
  //{{AFX_MSG_MAP(CEditedList_dlg)
  ON_BN_CLICKED(IDC_FILTER, OnFilter)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CEditedList_dlg message handlers

void CEditedList_dlg::OnFilter() 
{
  CFeatureFilter_dlg dlg(m_filter,this);
  dlg.DoModal();
}

void CEditedList_dlg::OnOK() 
{
  filter.edge=m_elev!=FALSE;
  filter.name=m_name!=FALSE;
  (FeatureFilter&)filter=m_filter;

  CDialog::OnOK();
}

BOOL CEditedList_dlg::OnInitDialog()
{
  m_elev=filter.edge;
  m_name=filter.name;
  m_filter=filter;

  CDialog::OnInitDialog();

  return TRUE;  // return TRUE unless you set the focus to a control
}
