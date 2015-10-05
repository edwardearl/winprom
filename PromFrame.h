// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// PromFrame.h : interface of the CPromFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROMFRAME_H__9927AF4A_C393_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_PROMFRAME_H__9927AF4A_C393_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CPromFrame : public CMDIFrameWnd
{
  DECLARE_DYNAMIC(CPromFrame)

  bool CreateFileBar(CToolBar&, UINT, const char *);
  void TB_toggle(CToolBar&);
  void TB_displayAll(bool);

public:
  CPromFrame();

// Attributes
public:

// Operations
public:

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CPromFrame)
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  //}}AFX_VIRTUAL

// Implementation
public:
  virtual ~CPromFrame();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
  CStatusBar statusBar;
  enum {SB_PANE_SEP=0,SB_PANE_HT,SB_PANE_WD,
    SB_PANE_LOC,SB_PANE_ELEV,SB_PANE_CAPS}; // must match indicators[]
  CToolBar mainToolBar,em_fileBar,dm_fileBar,dt_fileBar,db_fileBar;

// Generated message map functions
protected:
  //{{AFX_MSG(CPromFrame)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnViewTbAll();
  afx_msg void OnViewTbDLPRmap();
  afx_msg void OnViewTbDLPRtree();
  afx_msg void OnViewTbElevMap();
  afx_msg void OnViewTbNone();
  afx_msg void OnUpdateViewTbElevMap(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewTbDLPRtree(CCmdUI* pCmdUI);
  afx_msg void OnUpdateViewTbDLPRmap(CCmdUI* pCmdUI);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnViewTbDatabase();
  afx_msg void OnUpdateViewTbDatabase(CCmdUI* pCmdUI);
  //}}AFX_MSG
  afx_msg void OnNcMouseMove(UINT nFlags, CPoint point);
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROMFRAME_H__9927AF4A_C393_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
