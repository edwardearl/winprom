#if !defined(AFX_BROWSEFORMAT_DLG_H__5D9F554A_2AFB_11D7_A23A_E0F653C10000__INCLUDED_)
#define AFX_BROWSEFORMAT_DLG_H__5D9F554A_2AFB_11D7_A23A_E0F653C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BrowseFormat_dlg.h : header file
//


struct Scale_info {
  float scale; // meters per pixel
  const char *text;
};

struct Option_info {
  const Scale_info *scales;
  const char *text;
  int nsize;
};

struct Provider_info {
  const Option_info *options;
  unsigned last_option;
};

/////////////////////////////////////////////////////////////////////////////
// CBrowseFormat_dlg dialog

class CBrowseFormat_dlg : public CDialog
{
  void update_close(UINT);
  void update_option(const Option_info&, const Option_info&);
  void set_best_size(int);
  void set_best_scale(const Scale_info[]);
  void set_option_list();
  void set_scale_size_list();
  float old_scale;
  int old_size; // 0=small, 1=medium, 2=large
  bool reload;

// Construction
public:
  CBrowseFormat_dlg(bool, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CBrowseFormat_dlg)
  enum { IDD = IDD_BROWSE_FMT };
  CButton	m_reload_butn;
  CComboBox	m_option_ctl;
  CComboBox	m_size_ctl;
  CComboBox	m_scale_ctl;
  CEdit		m_custom_edit;
  CString	m_custom;
  int		m_size;
  int		m_provider;
  int		m_scale;
  int		m_option;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CBrowseFormat_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CBrowseFormat_dlg)
  afx_msg void OnReset();
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  afx_msg void OnReload();
  afx_msg void OnChangeProvider();
  afx_msg void OnChangeOption();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BROWSEFORMAT_DLG_H__5D9F554A_2AFB_11D7_A23A_E0F653C10000__INCLUDED_)
