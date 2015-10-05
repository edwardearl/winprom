// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_PARENTTYPE_DLG_H__0428F4B4_A3A1_4228_BCCB_B13657AAED0F__INCLUDED_)
#define AFX_PARENTTYPE_DLG_H__0428F4B4_A3A1_4228_BCCB_B13657AAED0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParentType_dlg.h : header file
//

#include "ParentListView.h"


/////////////////////////////////////////////////////////////////////////////
// CParentType_dlg dialog

class CParentType_dlg : public CDialog
{
  CParentListView::List_info& list_info;
  const CPromDoc& doc;

// Construction
public:
  CParentType_dlg(const CPromDoc&, CParentListView::List_info&, CWnd *pParent = NULL);

// Dialog Data
  //{{AFX_DATA(CParentType_dlg)
  enum { IDD = IDD_PARENTS };
  CButton	m_filter_butn;
  BOOL	m_runoffs;
  int		m_parent_type;
  int		m_prom;
  //}}AFX_DATA
  Prom_sadl_filter m_filter;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CParentType_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CParentType_dlg)
  afx_msg void OnFilter();
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARENTTYPE_DLG_H__0428F4B4_A3A1_4228_BCCB_B13657AAED0F__INCLUDED_)
