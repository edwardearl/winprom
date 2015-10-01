#if !defined(AFX_DEFINEFLAT_DLG_H__78B04200_52BE_11D7_A23A_D0F453C10000__INCLUDED_)
#define AFX_DEFINEFLAT_DLG_H__78B04200_52BE_11D7_A23A_D0F453C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DefineFlat_dlg.h : header file
//

#include "Crop_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CDefineFlat_dlg dialog

class CDefineFlat_dlg : public CCrop_dlg
{
// Construction
public:
  CDefineFlat_dlg(const char *, Rectangl&, CWnd *pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CDefineFlat_dlg)
  enum { IDD = IDD_DEFINE_FLAT };
  CEdit	m_low_ctl;
  CEdit	m_high_ctl;
  CString	m_high;
  CString	m_low;
  //}}AFX_DATA
  Elev_intvl elev_lmt;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDefineFlat_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDefineFlat_dlg)
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEFINEFLAT_DLG_H__78B04200_52BE_11D7_A23A_D0F453C10000__INCLUDED_)
