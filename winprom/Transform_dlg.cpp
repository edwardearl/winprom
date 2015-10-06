// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// Transform_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "Transform_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTransform_dlg dialog

CTransform_dlg::CTransform_dlg(UINT nID, CWnd *parent)
  : CDialog(nID,parent)
{
  m_add=0;
  m_mult=1;
}

CTransform_dlg::CTransform_dlg(CWnd *pParent /*=NULL*/)
  : CDialog(CTransform_dlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CTransform_dlg)
  m_add = 0.0f;
  m_mult = 1.0f;
  //}}AFX_DATA_INIT
}

void CTransform_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CTransform_dlg)
  DDX_Text(pDX, IDC_ADD, m_add);
  DDX_Text(pDX, IDC_MULT, m_mult);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransform_dlg, CDialog)
  //{{AFX_MSG_MAP(CTransform_dlg)
    // NOTE: the ClassWizard will add message map macros here
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTransform_dlg message handlers
