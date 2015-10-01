#if !defined(AFX_DATAFRAME_H__96C275A0_C73C_11D6_A23A_708552C10000__INCLUDED_)
#define AFX_DATAFRAME_H__96C275A0_C73C_11D6_A23A_708552C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataFrame.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CDataFrame frame

class CDataFrame : public CFrameWnd
{
  DECLARE_DYNCREATE(CDataFrame)
protected:
  CDataFrame();           // protected constructor used by dynamic creation

// Attributes
public:
  static unsigned client_width;
  CStatusBar dataStatus;

// Operations
public:

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDataFrame)
  protected:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  //}}AFX_VIRTUAL
  virtual void OnUpdateFrameTitle(BOOL);

// Implementation
protected:
  virtual ~CDataFrame();
  CToolBar dataTools,fileBar;

  // Generated message map functions
  //{{AFX_MSG(CDataFrame)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnViewTbDatabase();
  afx_msg void OnUpdateViewTbDatabase(CCmdUI* pCmdUI);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAFRAME_H__96C275A0_C73C_11D6_A23A_708552C10000__INCLUDED_)
