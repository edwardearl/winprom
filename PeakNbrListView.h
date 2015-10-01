#if !defined(AFX_NBRLISTVIEW_H__FFA22141_E866_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_NBRLISTVIEW_H__FFA22141_E866_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PeakNbrListView.h : header file
//

#include "FeatureListView.h"


/////////////////////////////////////////////////////////////////////////////
// CPeakNbrListView view

class CPeakNbrListView : public CFeatureListView
{
  Domain *center_peak;
  static Domain *init_center_peak;

protected:
  CPeakNbrListView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CPeakNbrListView)

  enum {SWIR_COL=2,SWIRO_COL=3};

// Attributes
public:

// Operations
public:
  static void init_list(CPromDoc *, Domain&, bool=false);
  virtual void UpdateItem(int);
  virtual void printFeature(int, FILE *, Format_style);
  virtual void printHeader(FILE *, Format_style);
  virtual bool CanPaste(const Clip&) const {return false;}
  virtual bool CanSort() const {return false;}
  virtual bool CanRelist() const {return true;}
  virtual void SetViewTitle();

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CPeakNbrListView)
  public:
  virtual void OnInitialUpdate();
  protected:
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  //}}AFX_VIRTUAL

// Implementation
protected:
  virtual ~CPeakNbrListView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CPeakNbrListView)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnEditSelectOverlaps();
  afx_msg void OnUpdateEditSelectOverlaps(CCmdUI* pCmdUI);
  afx_msg void OnUpdateListLoad(CCmdUI* pCmdUI);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NBRLISTVIEW_H__FFA22141_E866_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
