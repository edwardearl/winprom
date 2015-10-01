#if !defined(AFX_GENERICLISTVIEW_H__84F9529E_81C6_4077_9420_15E2EE47BD57__INCLUDED_)
#define AFX_GENERICLISTVIEW_H__84F9529E_81C6_4077_9420_15E2EE47BD57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GenericListView.h : header file
//

#include "FeatureListView.h"


/////////////////////////////////////////////////////////////////////////////
// CGenericListView view

class CGenericListView : public CFeatureListView
{
  static bool is_initial;
  FeatureTypeFilter filter;
  void add_item(Feature&, FT_index);

protected:
  CGenericListView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CGenericListView)

// Attributes
public:
  static FeatureTypeFilter init_ff;

// Operations
public:
  static void init_list(CPromDoc *);
  virtual bool prelist();
  virtual bool CanRelist() const {return true;}

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CGenericListView)
  public:
  virtual void OnInitialUpdate();
  //}}AFX_VIRTUAL

// Implementation
protected:
  virtual ~CGenericListView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CGenericListView)
    // NOTE - the ClassWizard will add and remove member functions here.
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERICLISTVIEW_H__84F9529E_81C6_4077_9420_15E2EE47BD57__INCLUDED_)
