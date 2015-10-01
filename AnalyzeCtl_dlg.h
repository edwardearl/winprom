#if !defined(AFX_ANALYZECTL_DLG_H__E42686E0_0524_11D6_A238_609152C10000__INCLUDED_)
#define AFX_ANALYZECTL_DLG_H__E42686E0_0524_11D6_A238_609152C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnalyzeCtl_dlg.h : header file
//

#include "BackgdTask_dlg.h"

#define IDT_ANALYZE_LINE 259
#define IDT_ANALYZE_WALKUP 260

class user_stop : public exception {
public:
  virtual const char *what() const throw() {return "User abort";}
};


/////////////////////////////////////////////////////////////////////////////
// CAnalyzeCtl_dlg dialog

class CAnalyzeCtl_dlg : public CBackgdTask_dlg
{
  CPromDoc& document;

// Construction
public:
  CAnalyzeCtl_dlg(CPromDoc&);   // standard constructor

  void update_line(int);
  void update_walkup(int);

// Dialog Data
  //{{AFX_DATA(CAnalyzeCtl_dlg)
  enum { IDD = IDD_ANALYZE_CTL };
  CEdit	m_walkup_count_edit;
  CStatic	m_found;
  CStatic	m_search;
  CProgressCtrl	m_walkup_prog;
  CProgressCtrl	m_search_prog;
  int		m_peak_count;
  int		m_sadl_count;
  CString	m_search_pc;
  //}}AFX_DATA
  const char *error;
  unsigned walkup_count;

  static CAnalyzeCtl_dlg *current_anal_ctl;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CAnalyzeCtl_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL
  virtual int proc() throw();

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CAnalyzeCtl_dlg)
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANALYZECTL_DLG_H__E42686E0_0524_11D6_A238_609152C10000__INCLUDED_)
