#if !defined(AFX_PROMRULERBAR_H__A9FEC600_36D4_11D4_8FA1_0040056FBD08__INCLUDED_)
#define AFX_PROMRULERBAR_H__A9FEC600_36D4_11D4_8FA1_0040056FBD08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PromRulerBar.h : header file
//

#include "RulerBar.h"
#include "ProfileView.h"


/////////////////////////////////////////////////////////////////////////////
// CPromRulerBar window

class CPromRulerBar : public CRulerBar
{
// Construction
public:
  CPromRulerBar();

// Attributes
public:
  CProfileView *view;

// Operations
public:
  virtual bool hasValue(int a) const
    {return view->posHasProm(a+view->GetScrollPosition().x);}
  virtual double highValue() const {return view->prom_range.high;}
  virtual double GetValue(int) const;
  virtual int GetTickPos(double);
  virtual int GetTickBase() const {return -view->GetScrollPosition().x;}

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CPromRulerBar)
  //}}AFX_VIRTUAL

// Implementation
public:
  virtual ~CPromRulerBar();

  // Generated message map functions
protected:
  //{{AFX_MSG(CPromRulerBar)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROMRULERBAR_H__A9FEC600_36D4_11D4_8FA1_0040056FBD08__INCLUDED_)
