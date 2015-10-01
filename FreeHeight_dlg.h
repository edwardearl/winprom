#if !defined(AFX_FREEHEIGHT_DLG_H__BEC8E6A3_1013_4399_8796_9E06387C3459__INCLUDED_)
#define AFX_FREEHEIGHT_DLG_H__BEC8E6A3_1013_4399_8796_9E06387C3459__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FreeHeight_dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CFreeHeight_dlg dialog

class CFreeHeight_dlg : public CDialog
{
// Construction
public:
  CFreeHeight_dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CFreeHeight_dlg)
  enum { IDD = IDD_FH_OUT };
  CListCtrl	mfh_azim_list;
  int		m_fvh;
  int		m_azim_type;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFreeHeight_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CFreeHeight_dlg)
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FREEHEIGHT_DLG_H__BEC8E6A3_1013_4399_8796_9E06387C3459__INCLUDED_)
