// winelev.h : main header file for the WINELEV application
//

#if !defined(AFX_WINELEV_H__DDC810A7_D24C_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_WINELEV_H__DDC810A7_D24C_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
  #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#define ID_CVT_FILE WM_USER


/////////////////////////////////////////////////////////////////////////////
// CElevApp:
// See winelev.cpp for the implementation of this class
//

class CConvertCtl_dlg;

class CElevApp : public CWinApp
{

public:
  CElevApp();

  CConvertCtl_dlg *control_dlg;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CElevApp)
  public:
  virtual BOOL InitInstance();
  //}}AFX_VIRTUAL

// Implementation

  //{{AFX_MSG(CElevApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
  //}}AFX_MSG
  afx_msg void OnCvtFile(UINT, LONG);
  afx_msg void OnBackgdTaskDone(WPARAM,LPARAM);
  DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINELEV_H__DDC810A7_D24C_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
