#if !defined(AFX_SADDLELISTVIEW_H__19A8E6E1_CB53_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_SADDLELISTVIEW_H__19A8E6E1_CB53_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SaddleListView.h : header file
//

#include "FeatureListView.h"
#include "ListSadl_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CSaddleListView view

class CSaddleListView : public CFeatureListView
{
  friend int CALLBACK compare_stat(LPARAM, LPARAM, LPARAM);
  friend int CALLBACK compare_prom(LPARAM, LPARAM, LPARAM);
  friend int CALLBACK compare_omp(LPARAM, LPARAM, LPARAM);
  static bool is_initial;
  vector<PromListItem> saddle_list;
  SadlListFilter list_filter;

protected:
  CSaddleListView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CSaddleListView)

  virtual void swap_items(int);

  enum {STAT_COL=2,PROM_COL=3,OMP_COL=4};

// Attributes
public:
  static SadlListFilter init_lf;

// Operations
public:
  static void init_list(CPromDoc *);
  virtual void UpdateItem(int);
  virtual void printFeature(int, FILE *, Format_style);
  virtual void printHeader(FILE *, Format_style);
  virtual bool CanPaste(const Clip&) const;
  virtual bool prelist();
  virtual bool CanRelist() const {return true;}
  virtual bool has_extra_info() const {return true;}
  virtual Elevation default_min_prom() const {return list_filter.min_prom;}
  virtual bool default_offmap() const {return list_filter.offmap;}
  virtual bool default_ridge() const {return list_filter.ridge;}
  virtual bool default_cell() const {return list_filter.cell;}
  virtual void SetViewTitle();
  virtual void pasteClip(const Clip&);

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CSaddleListView)
  public:
  virtual void OnInitialUpdate();
  protected:
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  //}}AFX_VIRTUAL

// Implementation
protected:
  virtual ~CSaddleListView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CSaddleListView)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  //}}AFX_MSG
  afx_msg void OnHeaderClick(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnUnlistItem(NMHDR *pNMHDR, LRESULT *pResult);
  DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SADDLELISTVIEW_H__19A8E6E1_CB53_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
