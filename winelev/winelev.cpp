// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// winelev.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxpriv.h"
#include "winelev.h"
#include "Elev_dlg.h"
#include "ConvertCtl_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CElevApp

BEGIN_MESSAGE_MAP(CElevApp, CWinApp)
  //{{AFX_MSG_MAP(CElevApp)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
  //}}AFX_MSG
  ON_COMMAND(ID_HELP, CWinApp::OnHelp)
  ON_THREAD_MESSAGE(ID_CVT_FILE, CElevApp::OnCvtFile)
  ON_THREAD_MESSAGE(IDT_BACKGD_TASK_DONE,CElevApp::OnBackgdTaskDone)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CElevApp construction

CElevApp::CElevApp()
{
}


/////////////////////////////////////////////////////////////////////////////
// The one and only CElevApp object

CElevApp theApp;


/////////////////////////////////////////////////////////////////////////////
// CElevApp initialization

BOOL CElevApp::InitInstance()
{
  AfxEnableControlContainer();

  CElev_dlg dlg;
  m_pMainWnd = &dlg;
  dlg.DoModal();

  // Since the dialog has been closed, return FALSE so that we exit the
  //  application, rather than start the application's message pump.
  return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// File conversion progress

void CElevApp::OnCvtFile(WPARAM, LPARAM)
{
  control_dlg->UpdateData(FALSE);
}

void CElevApp::OnBackgdTaskDone(WPARAM result, LPARAM)
{
  CBackgdTask_dlg::current_backgd_task->OnDone(result);
}
