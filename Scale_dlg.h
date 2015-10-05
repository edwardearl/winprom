// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_SCALE_DLG_H__2F724BDA_4324_4201_9C76_F2F5028D6112__INCLUDED_)
#define AFX_SCALE_DLG_H__2F724BDA_4324_4201_9C76_F2F5028D6112__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Scale_dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScale_dlg dialog

class CScale_dlg : public CDialog
{
// Construction
public:
  CScale_dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CScale_dlg)
  enum { IDD = IDD_SCALE };
  float	m_scale;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CScale_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CScale_dlg)
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCALE_DLG_H__2F724BDA_4324_4201_9C76_F2F5028D6112__INCLUDED_)
