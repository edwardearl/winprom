#if !defined(AFX_LISTFRAME_H__BF032481_C5D0_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_LISTFRAME_H__BF032481_C5D0_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ListFrame.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CListFrame frame

class CListFrame : public CFrameWnd
{
  DECLARE_DYNCREATE(CListFrame)
protected:
  CListFrame();           // protected constructor used by dynamic creation

// Attributes
public:
  static int client_width;
  CStatusBar listStatus;
  CToolBar fileBar;

// Operations
public:

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CListFrame)
  protected:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  //}}AFX_VIRTUAL
  virtual void OnUpdateFrameTitle(BOOL);

// Implementation
protected:
  virtual ~CListFrame();
  CToolBar listTools;

  // Generated message map functions
  //{{AFX_MSG(CListFrame)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnViewTbDLPRtree();
  afx_msg void OnUpdateViewTbDLPRtree(CCmdUI* pCmdUI);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTFRAME_H__BF032481_C5D0_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
