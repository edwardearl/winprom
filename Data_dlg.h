// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_DATA_DLG_H__FCD2A2A1_C487_11D6_A23A_20A452C10000__INCLUDED_)
#define AFX_DATA_DLG_H__FCD2A2A1_C487_11D6_A23A_20A452C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Data_dlg.h : header file
//

#define MAX_FIELDS 20


/////////////////////////////////////////////////////////////////////////////
// CData_dlg dialog

class CData_dlg : public CDialog
{
  CPromDoc& document;
  const GridPoint& target;
  Database::RecordID record;
  CStatic name_label[MAX_FIELDS],type_label[MAX_FIELDS];
  CEdit value_label[MAX_FIELDS];

  bool save_records();

// Construction
public:
  //CData_dlg(Database&, const GridPoint&, CWnd *pParent);
  //CData_dlg(Database&, Database::RecordID, CWnd *pParent);
  CData_dlg(CPromDoc&, const GridPoint&, CWnd *pParent);
  CData_dlg(CPromDoc&, Database::RecordID, CWnd *pParent);

// Dialog Data
  //{{AFX_DATA(CData_dlg)
  enum { IDD = IDD_DATA };
  CButton	m_view_butn;
  CButton	m_delete_butn;
  //}}AFX_DATA
  bool changed;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CData_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CData_dlg)
  afx_msg void OnFields();
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  afx_msg void OnDelete();
  afx_msg void OnView();
  afx_msg void OnBrowse();
  //}}AFX_MSG
  afx_msg void OnChangeField();
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATA_DLG_H__FCD2A2A1_C487_11D6_A23A_20A452C10000__INCLUDED_)
