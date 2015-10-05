// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_TREEFRAME_H__D6E36D80_28A9_11D4_8FA1_0040056FBD08__INCLUDED_)
#define AFX_TREEFRAME_H__D6E36D80_28A9_11D4_8FA1_0040056FBD08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeFrame.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CTreeFrame frame

class CTreeFrame : public CFrameWnd
{
  DECLARE_DYNCREATE(CTreeFrame)
protected:
  CTreeFrame();           // protected constructor used by dynamic creation

// Attributes
public:
  CStatusBar treeStatus;
  CToolBar fileBar;

// Operations
public:

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTreeFrame)
  protected:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  //}}AFX_VIRTUAL
  virtual void OnUpdateFrameTitle(BOOL);

// Implementation
protected:
  virtual ~CTreeFrame();
  CToolBar treeTools;

  // Generated message map functions
  //{{AFX_MSG(CTreeFrame)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnViewTbDLPRtree();
  afx_msg void OnUpdateViewTbDLPRtree(CCmdUI* pCmdUI);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEFRAME_H__D6E36D80_28A9_11D4_8FA1_0040056FBD08__INCLUDED_)
