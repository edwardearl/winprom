#if !defined(AFX_PURGEDOMAIN_DLG_H__57171093_EAE1_4B5E_B50B_06CFF1209544__INCLUDED_)
#define AFX_PURGEDOMAIN_DLG_H__57171093_EAE1_4B5E_B50B_06CFF1209544__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PurgeDomain_dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CPurgeDomain_dlg dialog

class CPurgeDomain_dlg : public CDialog
{
// Construction
public:
  CPurgeDomain_dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CPurgeDomain_dlg)
  enum { IDD = IDD_PURGE_DOMAIN };
  BOOL	m_offmap;
  int		m_prom;
  BOOL	m_rotate;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CPurgeDomain_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CPurgeDomain_dlg)
    // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PURGEDOMAIN_DLG_H__57171093_EAE1_4B5E_B50B_06CFF1209544__INCLUDED_)
