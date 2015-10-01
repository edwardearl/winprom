#if !defined(AFX_WALKLISTVIEW_H__724A7800_8A65_11D4_8FA2_0040056FBD08__INCLUDED_)
#define AFX_WALKLISTVIEW_H__724A7800_8A65_11D4_8FA2_0040056FBD08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WalkListView.h : header file
//

#include "FeatureListView.h"

struct WalkInfo {
  Feature *begin,*end;
  FT_index begin_type,end_type;
  WalkInfo() : begin(0),end(0) {}
  WalkInfo(Feature *b, FT_index bt, Feature *e, FT_index et) :
    begin(b),end(e),begin_type(bt),end_type(et) {}
  operator bool() {return begin!=0 && end!=0;}
};


/////////////////////////////////////////////////////////////////////////////
// CWalkListView view

class CWalkListView : public CFeatureListView
{
  WalkInfo walk_info;
  static WalkInfo init_walk_info;

protected:
  CWalkListView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CWalkListView)

// Attributes
public:

// Operations
public:
  static void init_list(CPromDoc *, const WalkInfo&);
  virtual bool CanPaste(const Clip&) const {return false;}
  virtual bool CanRelist() const {return true;}
  virtual void SetViewTitle();

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CWalkListView)
  public:
  virtual void OnInitialUpdate();
  protected:
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  //}}AFX_VIRTUAL

// Implementation
protected:
  virtual ~CWalkListView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CWalkListView)
  afx_msg void OnEditSelectOverlaps();
  afx_msg void OnUpdateEditSelectOverlaps(CCmdUI* pCmdUI);
  afx_msg void OnUpdateListLoad(CCmdUI* pCmdUI);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WALKLISTVIEW_H__724A7800_8A65_11D4_8FA2_0040056FBD08__INCLUDED_)
