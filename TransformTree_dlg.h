// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_TRANSFORMTREE_DLG_H__EA6E0FE0_7AE5_11D7_A23A_20FD53C10000__INCLUDED_)
#define AFX_TRANSFORMTREE_DLG_H__EA6E0FE0_7AE5_11D7_A23A_20FD53C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TransformTree_dlg.h : header file
//

#include "Transform_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CTransformTree_dlg dialog

class CTransformTree_dlg : public CTransform_dlg
{
// Construction
public:
  CTransformTree_dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CTransformTree_dlg)
  enum { IDD = IDD_TRANSFORM_TREE };
  BOOL	m_edited;
  BOOL	m_peaks;
  BOOL	m_runoffs;
  BOOL	m_sadls;
  BOOL	m_basnsadls;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTransformTree_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CTransformTree_dlg)
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFORMTREE_DLG_H__EA6E0FE0_7AE5_11D7_A23A_20FD53C10000__INCLUDED_)
