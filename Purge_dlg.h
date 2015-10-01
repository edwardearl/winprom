#if !defined(AFX_PURGE_DLG_H__4155BC21_EB5D_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_PURGE_DLG_H__4155BC21_EB5D_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Purge_dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPurge_dlg dialog

class CPurge_dlg : public CDialog
{
  UINT feature_icon_id;
  void init_data();

// Construction
protected:
  CPurge_dlg(UINT, UINT);
public:
  CPurge_dlg(UINT);  // standard constructor

// Dialog Data
  //{{AFX_DATA(CPurge_dlg)
  enum { IDD = IDD_PURGE };
  int	m_prom;
  BOOL	m_keep_name;
  BOOL	m_keep_edit;
  BOOL	m_ridge;
  BOOL	m_cell;
  //}}AFX_DATA


// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CPurge_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CPurge_dlg)
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PURGE_DLG_H__4155BC21_EB5D_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
