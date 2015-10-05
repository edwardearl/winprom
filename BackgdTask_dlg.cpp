// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// BackgroundTask_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BackgdTask_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static UINT thread_entry(CBackgdTask_dlg *myself)
{
  int result=myself->proc();
  CBackgdTask_dlg::current_backgd_task=myself;
  AfxGetApp()->PostThreadMessage(IDT_BACKGD_TASK_DONE,result,0);
  return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CBackgdTask_dlg dialog

CBackgdTask_dlg::CBackgdTask_dlg(UINT dlgResource)
  : CDialog(dlgResource)
{
  //{{AFX_DATA_INIT(CBackgdTask_dlg)
  //}}AFX_DATA_INIT
}

void CBackgdTask_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CBackgdTask_dlg)
  DDX_Control(pDX, IDSUSPEND, m_suspend);
  DDX_Control(pDX, IDCANCEL, m_cancel);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBackgdTask_dlg, CDialog)
  //{{AFX_MSG_MAP(CBackgdTask_dlg)
  ON_BN_CLICKED(IDSUSPEND, OnSuspend)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CBackgdTask_dlg *CBackgdTask_dlg::current_backgd_task;


/////////////////////////////////////////////////////////////////////////////
// CBackgdTask_dlg message handlers

BOOL CBackgdTask_dlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  running=true;
  stop_request=false;
  pThread=AfxBeginThread((AFX_THREADPROC)thread_entry,this);

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}

void CBackgdTask_dlg::OnSuspend()
{
  if (pThread==0) CDialog::OnOK();
  else if (running) {
    pThread->SuspendThread();
    m_suspend.SetWindowText("&Resume");
    running=false;
  }
  else {
    pThread->ResumeThread();
    m_suspend.SetWindowText("&Suspend");
    running=true;
  }
}

void CBackgdTask_dlg::OnCancel()
{
  if (pThread!=0) stop_request=true;
  else CDialog::OnCancel();
}

void CBackgdTask_dlg::OnDone(WPARAM result)
{
  if (result!=0) EndDialog(result);
  else {
    m_suspend.SetWindowText("OK");
    pThread=0;
  }
}
