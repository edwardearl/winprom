// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_BACKGDTASK_DLG_H__99468940_7622_11D5_A238_509152C10000__INCLUDED_)
#define AFX_BACKGDTASK_DLG_H__99468940_7622_11D5_A238_509152C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BackgroundTask_dlg.h : header file
//

#define IDD_BACKGROUND -1 // dummy
#define IDT_BACKGD_TASK_DONE 257


/////////////////////////////////////////////////////////////////////////////
// CBackgdTask_dlg dialog

class CBackgdTask_dlg : public CDialog
{
  bool running;
  CWinThread *pThread;
  friend UINT thread_entry(CBackgdTask_dlg *);

// Construction
public:
  CBackgdTask_dlg(UINT);

  void OnDone(WPARAM);

// Dialog Data
  //{{AFX_DATA(CBackgdTask_dlg)
  enum { IDD = IDD_BACKGROUND };
  CButton	m_suspend;
  CButton	m_cancel;
  //}}AFX_DATA

  bool stop_request;
  static CBackgdTask_dlg *current_backgd_task;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CBackgdTask_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL
  virtual int proc() throw() = 0;

// Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CBackgdTask_dlg)
  virtual void OnCancel();
  afx_msg void OnSuspend();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BACKGDTASK_DLG_H__99468940_7622_11D5_A238_509152C10000__INCLUDED_)
