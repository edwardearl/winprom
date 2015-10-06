#if !defined(AFX_FILEEXIST_DLG_H__73E088A6_1114_11D4_8FA1_0040056FBD08__INCLUDED_)
#define AFX_FILEEXIST_DLG_H__73E088A6_1114_11D4_8FA1_0040056FBD08__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FileExist_dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileExist_dlg dialog

class CFileExist_dlg : public CDialog
{
// Construction
public:
  CFileExist_dlg();   // standard constructor

// Dialog Data
  //{{AFX_DATA(CFileExist_dlg)
  enum { IDD = IDD_FILE_EXISTS };
  BOOL	m_future;
  CString	m_filename;
  //}}AFX_DATA


// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFileExist_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CFileExist_dlg)
  afx_msg void OnAbort();
  afx_msg void OnOverwrite();
  afx_msg void OnSkip();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEEXIST_DLG_H__73E088A6_1114_11D4_8FA1_0040056FBD08__INCLUDED_)
