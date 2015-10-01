#if !defined(AFX_CONTOUR_DLG_H__725A6A01_EE87_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_CONTOUR_DLG_H__725A6A01_EE87_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Contour_dlg.h : header file
//

#include "ColorBox.h"


/////////////////////////////////////////////////////////////////////////////
// CContour_dlg dialog

class CContour_dlg : public CDialog
{
  CColorBox m_box_contour,m_box_void;

// Construction
public:
  CContour_dlg();   // standard constructor

// Dialog Data
  //{{AFX_DATA(CContour_dlg)
  enum { IDD = IDD_CONTOUR };
  CEdit	m_standard_ctl;
  CEdit	m_minor_ctl;
  CEdit	m_major_ctl;
  int	m_major;
  int	m_minor;
  int	m_standard;
  BOOL	m_autozoom;
  int	m_GPontop;
  int	m_datum;
  BOOL	m_enable;
  //}}AFX_DATA
  COLORREF m_color_contour,m_color_void;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CContour_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CContour_dlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  afx_msg void OnContourColor();
  afx_msg void OnVoidColor();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTOUR_DLG_H__725A6A01_EE87_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
