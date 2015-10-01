#if !defined(AFX_TREEPASTE_DLG_H__6D393EA1_2CC3_11D4_8FA1_0040056FBD08__INCLUDED_)
#define AFX_TREEPASTE_DLG_H__6D393EA1_2CC3_11D4_8FA1_0040056FBD08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreePaste_dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CTreePaste_dlg dialog

class CTreePaste_dlg : public CDialog
{
  CPromDoc *document;
  Feature& feature;
  FT_index type;
  bool canPaste;

// Construction
public:
  CTreePaste_dlg(CWnd *, CPromDoc *, Feature&, FT_index, bool);

// Dialog Data
  //{{AFX_DATA(CTreePaste_dlg)
  enum { IDD = IDD_TREE_PASTE };
  CButton	m_paste_ctl;
  BOOL		m_show_next;
  //}}AFX_DATA


// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTreePaste_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CTreePaste_dlg)
  afx_msg void OnCenter();
  afx_msg void OnAbort();
  afx_msg void OnPaste();
  afx_msg void OnSkip();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEPASTE_DLG_H__6D393EA1_2CC3_11D4_8FA1_0040056FBD08__INCLUDED_)
