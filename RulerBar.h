// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_RULER_H__F5C4A463_1C30_11D4_8FA1_0040056FBD08__INCLUDED_)
#define AFX_RULER_H__F5C4A463_1C30_11D4_8FA1_0040056FBD08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Ruler.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRulerBar window

class CRulerBar : public CStatusBar
{
  int tickMarkLabel(CDC *, double);
  void tickMarkLabel(CDC *, double, int);
  void tickMark(CDC *, int);
  unsigned minor_tick;

// Construction
public:
  CRulerBar();

// Attributes
public:
  int tickInterval,tickWindow;
  float valueBase,valueInterval;

// Operations
public:
  virtual void OnUpdateCmdUI(CFrameWnd *, BOOL) {}

  virtual bool hasValue(int) const {return true;}
  virtual double highValue() const {assert(0); return 0;}
  virtual double GetValue(int a) const {return a;}
  virtual int GetTickPos(double a) {return (int)(a+0.5);}
  virtual int GetTickBase() const {return 0;}

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CRulerBar)
  //}}AFX_VIRTUAL

// Implementation
public:
  virtual ~CRulerBar();

  // Generated message map functions
protected:
  //{{AFX_MSG(CRulerBar)
  afx_msg void OnPaint();
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RULER_H__F5C4A463_1C30_11D4_8FA1_0040056FBD08__INCLUDED_)
