#if !defined(AFX_RIDGEVIEW_DLG_H__DDF68495_BEC1_4E36_A726_29E97D93EDA3__INCLUDED_)
#define AFX_RIDGEVIEW_DLG_H__DDF68495_BEC1_4E36_A726_29E97D93EDA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RidgeView_dlg.h : header file
//

#include "ColorBox.h"
#include "CutoffOnly_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CRidgeView_dlg dialog

class CRidgeView_dlg : public CCutoffOnly_dlg
{
  CColorBox m_color_box;
  bool local_topo;

// Construction
public:
  CRidgeView_dlg(bool, CWnd *pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CRidgeView_dlg)
  enum { IDD = IDD_RIDGE_VIEW };
  CButton	m_sadl_butn;
  CButton	m_peak_butn;
  BOOL	m_runoff;
  BOOL	m_basin_sadl;
  BOOL	m_peak;
  BOOL	m_saddle;
  BOOL	m_ridge_rot;
  int	m_topology;
  //}}AFX_DATA
  COLORREF m_color;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CRidgeView_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CRidgeView_dlg)
  afx_msg void OnPeak();
  afx_msg void OnSaddle();
  afx_msg void OnColor();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RIDGEVIEW_DLG_H__DDF68495_BEC1_4E36_A726_29E97D93EDA3__INCLUDED_)
