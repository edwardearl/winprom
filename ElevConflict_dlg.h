#if !defined(AFX_ELEVCONFLICT_DLG_H__E7263802_EAB8_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_ELEVCONFLICT_DLG_H__E7263802_EAB8_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ElevConflict_dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CElevConflict_dlg dialog

class CElevConflict_dlg : public CDialog
{
// Construction
public:
  CElevConflict_dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CElevConflict_dlg)
  enum { IDD = IDD_ELEV_CONFLICT };
  CString	m_location;
  BOOL		m_warn;
  int		m_workspace;
  int		m_file;
  int		m_use;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CElevConflict_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CElevConflict_dlg)
  afx_msg void OnAbort();
  afx_msg void OnContinue();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEVCONFLICT_DLG_H__E7263802_EAB8_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
