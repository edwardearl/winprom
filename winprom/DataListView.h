// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_DATALISTVIEW_H__F5E7B5E0_C68D_11D6_A23A_10A452C10000__INCLUDED_)
#define AFX_DATALISTVIEW_H__F5E7B5E0_C68D_11D6_A23A_10A452C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataListView.h : header file
//

#include "Format.h"


/////////////////////////////////////////////////////////////////////////////
// CDataListView view

class CDataListView : public CListView
{
  static const char *init_title;
  Record_location_filter list_filter;
  Database::RecordID r_click_rec;
  int r_click_item;
  vector<GridPoint> fixed_list;
  const char *title;
  void copyClip(Clip& clp) const;
  void printHeader(FILE *, const Database&, Format_style) const;
  void printRecord(int, FILE *, const Database&, Format_style) const;
  static void set_client_width(const Database&);

protected:
  CDataListView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CDataListView)
  Location_fmt list_fmt;
  void setItemIndicators();

// Attributes
public:
  static char buf[256];

// Operations
public:
  CPromDoc *GetDocument() const {return (CPromDoc *)CListView::GetDocument();}
  void setViewStyle(DWORD);
  DWORD GWL() const
    {return GetWindowLong(GetListCtrl().GetSafeHwnd(),GWL_STYLE);}

  static void init_list(CPromDoc *);
  static void init_list(CPromDoc *, const vector<GridPoint>&, const char *t);
  void create_columns();
  virtual void SetViewTitle();
  void AddItem(Database::RecordID);
  void UpdateRecord(Database::RecordID);
  void UpdateItem(int);
  bool CanPaste(const Clip&) const;
  void pasteClip(const Clip&);

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDataListView)
  public:
  virtual void OnInitialUpdate();
  protected:
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  //}}AFX_VIRTUAL

// Implementation
protected:
  virtual ~CDataListView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CDataListView)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnListFormat();
  afx_msg void OnUpdateHaveItems(CCmdUI* pCmdUI);
  afx_msg void OnEditUnlist();
  afx_msg void OnUpdateEditAnySelect(CCmdUI* pCmdUI);
  afx_msg void OnEditSelectAll();
  afx_msg void OnEditSelectInvert();
  afx_msg void OnEditRecord();
  afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnEditCenter();
  afx_msg void OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnViewList();
  afx_msg void OnUpdateViewList(CCmdUI* pCmdUI);
  afx_msg void OnViewReport();
  afx_msg void OnUpdateViewReport(CCmdUI* pCmdUI);
  afx_msg void OnListRelist();
  afx_msg void OnEditDelete();
  afx_msg void OnListSummary();
  afx_msg void OnUpdateSingleSelect(CCmdUI* pCmdUI);
  afx_msg void OnUpdateEditRecord(CCmdUI* pCmdUI);
  afx_msg void OnEditSearch();
  afx_msg void OnUpdateEditSearch(CCmdUI* pCmdUI);
  afx_msg void OnEditSelectFilterSet();
  afx_msg void OnEditSelectFilterPrecise();
  afx_msg void OnEditSelectFilterClear();
  afx_msg void OnFileText();
  afx_msg void OnFileHTML();
  afx_msg void OnFileCSV();
  afx_msg void OnEditPaste();
  afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnEditCopy();
  afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnEditBrowse();
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnRecordBrowse();
  afx_msg void OnRecordCenter();
  afx_msg void OnRecordDelete();
  afx_msg void OnRecordEdit();
  afx_msg void OnRecordUnlist();
  afx_msg void OnRQFinfo();
  afx_msg void OnEditRQF();
  afx_msg void OnUpdateEditRQF(CCmdUI* pCmdUI);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATALISTVIEW_H__F5E7B5E0_C68D_11D6_A23A_10A452C10000__INCLUDED_)
