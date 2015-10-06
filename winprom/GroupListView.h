// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_GROUPLISTVIEW_H__E900B9E9_EA8D_4BD5_B938_0A39B5FFA329__INCLUDED_)
#define AFX_GROUPLISTVIEW_H__E900B9E9_EA8D_4BD5_B938_0A39B5FFA329__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GroupListView.h : header file
//

#include "FeatureListView.h"


/////////////////////////////////////////////////////////////////////////////
// CGroupListView view

class CGroupListView : public CFeatureListView
{
protected:
  CGroupListView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CGroupListView)

  virtual bool begins_group(int) {assert(0); return false;}

// Attributes
public:

// Operations
public:
  virtual bool CanRelist() const {return true;}
  virtual bool CanSort() const {return false;}
  virtual bool CanPaste(const Clip&) const {return false;}

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CGroupListView)
  //}}AFX_VIRTUAL

// Implementation
protected:
  virtual ~CGroupListView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CGroupListView)
  afx_msg void OnUpdateListLoad(CCmdUI* pCmdUI);
  //}}AFX_MSG
  afx_msg void OnUnlistItem(NMHDR *pNMHDR, LRESULT *pResult);
  DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GROUPLISTVIEW_H__E900B9E9_EA8D_4BD5_B938_0A39B5FFA329__INCLUDED_)
