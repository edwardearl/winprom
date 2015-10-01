#if !defined(AFX_STATOPTIONS_DLG_H__15070200_CBF0_11D8_A23B_10F753C10000__INCLUDED_)
#define AFX_STATOPTIONS_DLG_H__15070200_CBF0_11D8_A23B_10F753C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatOptions_dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CStatOptions_dlg dialog

class CStatOptions_dlg : public CDialog
{
// Construction
public:
  CStatOptions_dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CStatOptions_dlg)
  enum { IDD = IDD_STAT_OPTIONS };
  int	m_symmetry;
  int	m_min_nbr;
  BOOL	m_measr_error;
  BOOL	m_convolutn;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CStatOptions_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CStatOptions_dlg)
    // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATOPTIONS_DLG_H__15070200_CBF0_11D8_A23B_10F753C10000__INCLUDED_)
