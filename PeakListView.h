#if !defined(AFX_PEAKLISTVIEW_H__81DA1A01_CA5D_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_PEAKLISTVIEW_H__81DA1A01_CA5D_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PeakListView.h : header file
//

#include "FeatureListView.h"
#include "ListFilter_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CPeakListView view

class CPeakListView : public CFeatureListView
{
  friend int CALLBACK compare_prom(LPARAM, LPARAM, LPARAM);
  friend int CALLBACK compare_omp(LPARAM, LPARAM, LPARAM);
  static bool is_initial;
  vector<PromListItem> peak_list;
  ListFilter list_filter;

protected:
  CPeakListView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CPeakListView)

  virtual void swap_items(int);

  enum {PROM_COL=2,OMP_COL=3};

// Attributes
public:
  static ListFilter init_lf;

// Operations
public:
  static void init_list(CPromDoc *);
  virtual void UpdateItem(int);
  virtual void printFeature(int, FILE *, Format_style);
  virtual void printHeader(FILE *, Format_style);
  virtual bool CanPaste(const Clip&) const;
  virtual bool has_extra_info() const {return true;}
  virtual bool prelist();
  virtual bool CanRelist() const {return true;}
  virtual Elevation default_min_prom() const {return list_filter.min_prom;}
  virtual bool default_offmap() const {return list_filter.offmap;}
  virtual void SetViewTitle();
  virtual void pasteClip(const Clip&);

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CPeakListView)
  public:
  virtual void OnInitialUpdate();
  protected:
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  //}}AFX_VIRTUAL

// Implementation
protected:
  virtual ~CPeakListView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CPeakListView)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnUpdateListSadl(CCmdUI* pCmdUI);
  afx_msg void OnListSadl();
  //}}AFX_MSG
  afx_msg void OnHeaderClick(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnUnlistItem(NMHDR *pNMHDR, LRESULT *pResult);
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PEAKLISTVIEW_H__81DA1A01_CA5D_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
