#if !defined(AFX_PURGENOISE_DLG_H__B0C31460_08C4_46E1_89C4_4C2426A5CA20__INCLUDED_)
#define AFX_PURGENOISE_DLG_H__B0C31460_08C4_46E1_89C4_4C2426A5CA20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PurgeNoise_dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CPurgeNoise_dlg dialog

class CPurgeNoise_dlg : public CDialog
{
  bool drainage;

// Construction
public:
  CPurgeNoise_dlg(bool, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CPurgeNoise_dlg)
  enum { IDD = IDD_PURGE_NOISE };
  float	m_noise;
  CString	m_label;
  BOOL	m_resid_nc;
  BOOL	m_hess_nc;
  BOOL	m_keep_edit;
  BOOL	m_keep_name;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CPurgeNoise_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CPurgeNoise_dlg)
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PURGENOISE_DLG_H__B0C31460_08C4_46E1_89C4_4C2426A5CA20__INCLUDED_)
