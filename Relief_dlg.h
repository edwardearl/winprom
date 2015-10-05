// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_RELIEF_DLG_H__3C92ABA0_18A3_11D7_A23A_30F753C10000__INCLUDED_)
#define AFX_RELIEF_DLG_H__3C92ABA0_18A3_11D7_A23A_30F753C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Relief_dlg.h : header file
//

#include "ElevColor_dlg.h"


class CColorIntervalBox : public CListBox {
  virtual void DrawItem(LPDRAWITEMSTRUCT);
  virtual int CompareItem(LPCOMPAREITEMSTRUCT);
  virtual void DeleteItem(LPDELETEITEMSTRUCT);
public:
  Elev_color_intv& get_eci(int item) const
    {return *(Elev_color_intv *)GetItemData(item);}
};


/////////////////////////////////////////////////////////////////////////////
// CRelief_dlg dialog

class CRelief_dlg : public CDialog
{
  CColorIntervalBox m_colorintv_box;
  vector<Elev_color_intv>& elev_colors;
  void interpolate_all(bool);
  void set_interp_butn();

// Construction
public:
  CRelief_dlg(vector<Elev_color_intv>&, CWnd* pParent = NULL);

// Dialog Data
  //{{AFX_DATA(CRelief_dlg)
  enum { IDD = IDD_RELIEF };
  CButton	m_interpolate_butn;
  float	m_intensity;
  float	m_direction;
  BOOL	m_color_enable;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CRelief_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CRelief_dlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnAdd();
  afx_msg void OnDelete();
  afx_msg void OnModify();
  virtual void OnOK();
  afx_msg void OnTransform();
  afx_msg void OnInterpolate();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RELIEF_DLG_H__3C92ABA0_18A3_11D7_A23A_30F753C10000__INCLUDED_)
