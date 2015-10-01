#if !defined(AFX_TRANSFORM_DLG_H__066973A0_EC50_11D4_A238_444553540000__INCLUDED_)
#define AFX_TRANSFORM_DLG_H__066973A0_EC50_11D4_A238_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Transform_dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTransform_dlg dialog

class CTransform_dlg : public CDialog
{
// Construction
protected:
  CTransform_dlg(UINT, CWnd *parent=NULL);
public:
  CTransform_dlg(CWnd *pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CTransform_dlg)
  enum { IDD = IDD_TRANSFORM };
  float	m_add;
  float	m_mult;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTransform_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CTransform_dlg)
    // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFORM_DLG_H__066973A0_EC50_11D4_A238_444553540000__INCLUDED_)
