#if !defined(AFX_RUNOFFNBRLISTVIEW_H__9B338F66_009D_11D4_8FA0_0040056FBD08__INCLUDED_)
#define AFX_RUNOFFNBRLISTVIEW_H__9B338F66_009D_11D4_8FA0_0040056FBD08__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RunoffNbrListView.h : header file
//

#include "RunoffListView.h"
#include "RunoffNbr_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CRunoffNbrListView view

class CRunoffNbrListView : public CRunoffListView
{
  friend int CALLBACK compare_dist(LPARAM, LPARAM, LPARAM);
  Runoff *center_runoff;
  static Runoff *init_center_runoff;
  Runoff_nbr_type ront;
  static Runoff_nbr_type init_ront;
  float radius;
  static float init_radius;
  enum Nbr_status {WITHIN,WITHOUT,FOREVER_WITHOUT};
  Nbr_status ro_nbr(const GridPoint&, const GridPoint&) const;

protected:
  CRunoffNbrListView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CRunoffNbrListView)

  enum {IQ_COL=2,DIST_COL=3};

// Attributes
public:

// Operations
public:
  static void init_list(CPromDoc *, Runoff&);
  virtual void UpdateItem(int);
  virtual void printFeature(int, FILE *, Format_style);
  virtual void printHeader(FILE *, Format_style);
  virtual bool prelist();
  virtual void SetViewTitle();

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CRunoffNbrListView)
  public:
  virtual void OnInitialUpdate();
  protected:
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  //}}AFX_VIRTUAL

// Implementation
protected:
  virtual ~CRunoffNbrListView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CRunoffNbrListView)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  //}}AFX_MSG
  afx_msg void OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult);
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNOFFNBRLISTVIEW_H__9B338F66_009D_11D4_8FA0_0040056FBD08__INCLUDED_)
