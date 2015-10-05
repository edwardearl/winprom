#if !defined(AFX_DATAIMPORT_DLG_H__E1E7C097_830E_4B5D_88B3_F1A27098CEF1__INCLUDED_)
#define AFX_DATAIMPORT_DLG_H__E1E7C097_830E_4B5D_88B3_F1A27098CEF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataImport_dlg.h : header file
//

#include "Database.h"


/////////////////////////////////////////////////////////////////////////////
// CDataImport_dlg dialog

class CDataImport_dlg : public CDialog
{
  const Database& data;

// Construction
public:
  CDataImport_dlg(const Database&, CWnd* pParent = NULL);   // standard constructor
  Database::const_FieldID name_fld,elev_fld;

// Dialog Data
  //{{AFX_DATA(CDataImport_dlg)
  enum { IDD = IDD_DATA_IMPORT };
  CComboBox	m_name_cb;
  CComboBox	m_elev_cb;
  BOOL	m_edit;
  int	m_bs_status;
  int	m_ps_status;
  //}}AFX_DATA
  FeatureTypeFilter m_filter;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDataImport_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDataImport_dlg)
  afx_msg void OnFilter();
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAIMPORT_DLG_H__E1E7C097_830E_4B5D_88B3_F1A27098CEF1__INCLUDED_)
