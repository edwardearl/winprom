// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// FreeHeight_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "FreeHeight_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFreeHeight_dlg dialog

CFreeHeight_dlg::CFreeHeight_dlg(CWnd* pParent /*=NULL*/)
  : CDialog(CFreeHeight_dlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CFreeHeight_dlg)
  m_fvh = 0;
  m_azim_type = -1;
  //}}AFX_DATA_INIT
}

void CFreeHeight_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CFreeHeight_dlg)
  DDX_Control(pDX, IDC_MFH_AZIM, mfh_azim_list);
  DDX_Text(pDX, IDC_FVH, m_fvh);
  DDX_Radio(pDX, IDC_FH, m_azim_type);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFreeHeight_dlg, CDialog)
  //{{AFX_MSG_MAP(CFreeHeight_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFreeHeight_dlg message handlers

BOOL CFreeHeight_dlg::OnInitDialog()
{
  m_azim_type=0;

  CDialog::OnInitDialog();

  mfh_azim_list.InsertColumn(0,"Azimuth",LVCFMT_LEFT,56);
  mfh_azim_list.InsertColumn(1,"Value",LVCFMT_LEFT,56);
  mfh_azim_list.InsertColumn(2,"Range",LVCFMT_LEFT,56);
  mfh_azim_list.InsertColumn(3,"Location",LVCFMT_LEFT,160);

  return TRUE;
}
