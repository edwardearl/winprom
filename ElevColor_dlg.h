#if !defined(AFX_ELEVCOLOR_DLG_H__995454D8_91C9_4BD1_A7F1_7AB338B27BDA__INCLUDED_)
#define AFX_ELEVCOLOR_DLG_H__995454D8_91C9_4BD1_A7F1_7AB338B27BDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElevColor_dlg.h : header file
//

#include "ColorBox.h"

struct Elev_color_intv {
  COLORREF color;
  Elevation elev;
  bool interp;
};


/////////////////////////////////////////////////////////////////////////////
// CElevColor_dlg dialog

class CElevColor_dlg : public CDialog
{
  Elev_color_intv& elev_color_intv;
  CColorBox m_color_box;

// Construction
public:
  CElevColor_dlg(Elev_color_intv&, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CElevColor_dlg)
  enum { IDD = IDD_ELEV_COLOR };
  int	m_elev;
  BOOL	m_interp;
  //}}AFX_DATA
  COLORREF m_color;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CElevColor_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CElevColor_dlg)
  afx_msg void OnColor();
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEVCOLOR_DLG_H__995454D8_91C9_4BD1_A7F1_7AB338B27BDA__INCLUDED_)
