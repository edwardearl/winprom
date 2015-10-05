// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_GRIDVIEW_DLG_H__306C823D_2DDC_42B7_A775_EB56509BCF15__INCLUDED_)
#define AFX_GRIDVIEW_DLG_H__306C823D_2DDC_42B7_A775_EB56509BCF15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridView_dlg.h : header file
//

#include "ColorBox.h"


/////////////////////////////////////////////////////////////////////////////
// CGridView_dlg dialog

class CGridView_dlg : public CDialog
{
  unsigned short mug;
  CColorBox m_color_box;

// Construction
public:
  CGridView_dlg(unsigned short, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CGridView_dlg)
  enum { IDD = IDD_GRID_VIEW };
  CString	m_note;
  int		m_grid_view;
  //}}AFX_DATA
  COLORREF m_color;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CGridView_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CGridView_dlg)
  afx_msg void OnColor();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDVIEW_DLG_H__306C823D_2DDC_42B7_A775_EB56509BCF15__INCLUDED_)
