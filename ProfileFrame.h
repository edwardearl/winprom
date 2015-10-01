#if !defined(AFX_PROFILEFRAME_H__A9FEC602_36D4_11D4_8FA1_0040056FBD08__INCLUDED_)
#define AFX_PROFILEFRAME_H__A9FEC602_36D4_11D4_8FA1_0040056FBD08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProfileFrame.h : header file
//

#include "GraphFrame.h"
#include "PromRulerBar.h"

/////////////////////////////////////////////////////////////////////////////
// CProfileFrame frame

class CProfileFrame : public CGraphFrame
{
  friend class CProfileView;
  CPromRulerBar prom_ruler;
  DECLARE_DYNCREATE(CProfileFrame)
protected:
  CProfileFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CProfileFrame)
  //}}AFX_VIRTUAL

// Implementation
protected:
  virtual ~CProfileFrame();

  // Generated message map functions
  //{{AFX_MSG(CProfileFrame)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnViewHruler();
  afx_msg void OnUpdateViewHruler(CCmdUI* pCmdUI);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROFILEFRAME_H__A9FEC602_36D4_11D4_8FA1_0040056FBD08__INCLUDED_)
