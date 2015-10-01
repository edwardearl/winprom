#if !defined(AFX_FIELDS_DLG_H__FCD2A2A2_C487_11D6_A23A_20A452C10000__INCLUDED_)
#define AFX_FIELDS_DLG_H__FCD2A2A2_C487_11D6_A23A_20A452C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Fields_dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CFields_dlg dialog

class CFields_dlg : public CDialog
{
  Database& data;
  bool changed;
  void swap(int);

// Construction
public:
  CFields_dlg(Database&, CWnd *pParent);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CFields_dlg)
  enum { IDD = IDD_FIELDS };
  CListBox	m_field_list;
  CString	m_fld_name;
  int		m_type;
  //}}AFX_DATA


// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFields_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CFields_dlg)
  afx_msg void OnAdd();
  afx_msg void OnRemove();
  afx_msg void OnRename();
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  afx_msg void OnUp();
  afx_msg void OnDown();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIELDS_DLG_H__FCD2A2A2_C487_11D6_A23A_20A452C10000__INCLUDED_)
