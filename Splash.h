#ifndef _SPLASH_
#define _SPLASH_

// Splash.h : header file
//


/////////////////////////////////////////////////////////////////////////////
//   Splash Screen class

class CSplashWnd : public CWnd
{
// Attributes:
public:
  CBitmap m_bitmap;

// Operations
public:
  CSplashWnd(); // Construction
  bool Show(CWnd *parentWnd=NULL);
  void Hide();

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CSplashWnd)
  //}}AFX_VIRTUAL

// Implementation
public:
  ~CSplashWnd();

protected:
  BOOL Create(CWnd *pParentWnd = NULL);

  // Generated message map functions
protected:
  //{{AFX_MSG(CSplashWnd)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnPaint();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

#endif
