// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_ERRORLISTVIEW_H__ECD6E800_88CC_11D4_8FA2_0040056FBD08__INCLUDED_)
#define AFX_ERRORLISTVIEW_H__ECD6E800_88CC_11D4_8FA2_0040056FBD08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ErrorListView.h : header file
//

#include "FeatureListView.h"
#include "ErrorFilter_dlg.h"


struct Error {
  Feature* featr;
  FT_index ft;
  Elevation map_elev,error;
  Error() : featr(0) {}
  Error(Feature *f, FT_index type, Elevation me, Elevation e) :
    featr(f),ft(type),map_elev(me),error(e) {}
  bool operator>(const Error& a) const
    {return abs(error)>abs(a.error);}
};


/////////////////////////////////////////////////////////////////////////////
// CErrorListView view

class CErrorListView : public CFeatureListView
{
  friend int CALLBACK compare_error(LPARAM, LPARAM, LPARAM);
  friend int CALLBACK compare_map_elev(LPARAM, LPARAM, LPARAM);
  static bool is_initial;
  vector<Error> error_list;
  ErrorFilter error_filter;
  static ErrorFilter init_ef;
  Elevation getError(const Feature&, const AnalData& data, Elevation&) const;

protected:
  CErrorListView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CErrorListView)

  virtual void swap_items(int);

  enum {ELEV_COL=2,ERROR_COL=3};

// Attributes
public:

// Operations
public:
  static void init_list(CPromDoc *);
  virtual void UpdateItem(int);
  virtual void printFeature(int, FILE *, Format_style);
  virtual void printHeader(FILE *, Format_style);
  virtual bool has_extra_info() const {return true;}
  virtual bool prelist();
  virtual bool CanRelist() const {return true;}
  virtual void SetViewTitle();
  virtual void pasteClip(const Clip&);

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CErrorListView)
  public:
  virtual void OnInitialUpdate();
  //}}AFX_VIRTUAL

// Implementation
protected:
  virtual ~CErrorListView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CErrorListView)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnUpdateListLoad(CCmdUI* pCmdUI);
  //}}AFX_MSG
  afx_msg void OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnUnlistItem(NMHDR *pNMHDR, LRESULT *pResult);
  DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ERRORLISTVIEW_H__ECD6E800_88CC_11D4_8FA2_0040056FBD08__INCLUDED_)
