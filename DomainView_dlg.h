// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_DOMAINVIEW_DLG_H__BF78071D_9869_465C_90FE_D6C9EE5B3FEA__INCLUDED_)
#define AFX_DOMAINVIEW_DLG_H__BF78071D_9869_465C_90FE_D6C9EE5B3FEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DomainView_dlg.h : header file
//

#include "CutoffOnly_dlg.h"
#include "ColorBox.h"


/////////////////////////////////////////////////////////////////////////////
// CDomainView_dlg dialog

class CDomainView_dlg : public CCutoffOnly_dlg
{
  CColorBox m_box_domain,m_box_void;

  // Construction
public:
  CDomainView_dlg();   // standard constructor

// Dialog Data
  //{{AFX_DATA(CDomainView_dlg)
  enum { IDD = IDD_DOMAIN_VIEW };
  BOOL	m_enable;
  BOOL	m_rotation;
  BOOL	m_offmap;
  BOOL	m_edge_sadl;
  //}}AFX_DATA
  COLORREF m_color_domain,m_color_void;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDomainView_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDomainView_dlg)
  afx_msg void OnDomainColor();
  afx_msg void OnVoidColor();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOMAINVIEW_DLG_H__BF78071D_9869_465C_90FE_D6C9EE5B3FEA__INCLUDED_)
