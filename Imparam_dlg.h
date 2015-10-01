#if !defined(AFX_IMPARAM_DLG_H__745ED560_31FC_11D6_A239_608552C10000__INCLUDED_)
#define AFX_IMPARAM_DLG_H__745ED560_31FC_11D6_A239_608552C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Imparam_dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CImparam_dlg dialog

class CImparam_dlg : public CDialog
{
// Construction
public:
  CImparam_dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CImparam_dlg)
  enum { IDD = IDD_IMPARAM };
  CEdit	m_distint_ctl;
  CComboBox	m_conv_fac_ctl;
  CString	m_conv_fac;
  float		m_distint;
  //}}AFX_DATA
  float conv_fac;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CImparam_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CImparam_dlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPARAM_DLG_H__745ED560_31FC_11D6_A239_608552C10000__INCLUDED_)
