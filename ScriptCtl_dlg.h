#if !defined(AFX_SCRIPTCTL_DLG_H__122C0E61_36D8_11D4_8FA2_0040056FBD08__INCLUDED_)
#define AFX_SCRIPTCTL_DLG_H__122C0E61_36D8_11D4_8FA2_0040056FBD08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScriptCtl_dlg.h : header file
//

#include "BackgdTask_dlg.h"

#define IDT_SCRIPT_CMD 258

class CPromDoc;


/////////////////////////////////////////////////////////////////////////////
// CScriptCtl_dlg dialog

class CScriptCtl_dlg : public CBackgdTask_dlg
{
  CPromDoc& document;
  const CString& name;

// Construction
public:
  CScriptCtl_dlg(CPromDoc&, const CString&);   // standard constructor

  void OnScriptCmd(const char *);

// Dialog Data
  //{{AFX_DATA(CScriptCtl_dlg)
  enum { IDD = IDD_SCRIPT_CTL };
  CEdit		m_command;
  //}}AFX_DATA

  static CScriptCtl_dlg *current_script_ctl;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CScriptCtl_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL
  virtual int proc() throw();

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CScriptCtl_dlg)
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIPTCTL_DLG_H__122C0E61_36D8_11D4_8FA2_0040056FBD08__INCLUDED_)
