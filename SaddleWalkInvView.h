// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_SADDLEWALKINVVIEW_H__0C39D2A0_D6AD_11D5_A238_308352C10000__INCLUDED_)
#define AFX_SADDLEWALKINVVIEW_H__0C39D2A0_D6AD_11D5_A238_308352C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SaddleWalkInvView.h : header file
//

#include "GroupListView.h"
#include "SWI_dlg.h"


struct SWI_companion {
  Feature *saddle;
  FT_index type;
  SWI_companion() {}
  SWI_companion(Feature *s, FT_index t) : saddle(s),type(t) {}
};


/////////////////////////////////////////////////////////////////////////////
// CSaddleWalkInvView view

class CSaddleWalkInvView : public CGroupListView
{
  static bool is_initial;
  static SWI_filter init_filter;
  SWI_filter relist_filter;
  CArray<SWI_companion, SWI_companion&> saddle_list;
  Elevation getElev(const Feature&) const;
  Elevation getElev(int nItem) const {return getElev(GetFeature(nItem));}
  void check_swi(const Elev_map&, Bool_map&, Divide_tree&, Feature&, FT_index);
  void check_swi_lo(bool&, Elevation, const Elev_map&, const Bool_map&,
		    Feature&, Feature&, FT_index, FT_index);

protected:
  CSaddleWalkInvView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CSaddleWalkInvView)

  enum {ELEV_COL=2,SWIR_COL=3};
  virtual bool begins_group(int i) {return !saddle_list[i].saddle;}

// Attributes
public:

// Operations
public:
  static void init_list(CPromDoc *);
  virtual void UpdateItem(int);
  virtual void printFeature(int, FILE *, Format_style);
  virtual void printHeader(FILE *, Format_style);
  virtual bool prelist();
  virtual void SetViewTitle();

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CSaddleWalkInvView)
  public:
  virtual void OnInitialUpdate();
  protected:
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  //}}AFX_VIRTUAL

// Implementation
protected:
  virtual ~CSaddleWalkInvView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CSaddleWalkInvView)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnEditSWI();
  afx_msg void OnUpdateEditSWI(CCmdUI* pCmdUI);
  afx_msg void OnUpdateListLoad(CCmdUI* pCmdUI);
  //}}AFX_MSG
  afx_msg void OnUnlistItem(NMHDR *pNMHDR, LRESULT *pResult);
  DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SADDLEWALKINVVIEW_H__0C39D2A0_D6AD_11D5_A238_308352C10000__INCLUDED_)
