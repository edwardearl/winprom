#if !defined(AFX_LISTBS_DLG_H__8E7F36BF_FCE5_402F_A3A6_DEDBCCD516EB__INCLUDED_)
#define AFX_LISTBS_DLG_H__8E7F36BF_FCE5_402F_A3A6_DEDBCCD516EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListSadl_dlg.h : header file
//

#include "ListFilter_dlg.h"


struct SadlListFilter : ListFilter {
  bool ridge,cell;
  SadlListFilter() : ridge(false),cell(false) {}
  void get_prom(const Domain::Sadl_prom&, Elevation&, Elevation&) const;
};


/////////////////////////////////////////////////////////////////////////////
// CListSadl_dlg dialog

class CListSadl_dlg : public CListFilter_dlg
{
  SadlListFilter& filter;
  void data_init();

// Construction
protected:
  CListSadl_dlg(UINT, const Database&, SadlListFilter&, UINT, CWnd *pParent = NULL);   // standard constructor
public:
  CListSadl_dlg(const Database&, SadlListFilter&, UINT, CWnd *pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CListSadl_dlg)
  enum { IDD = IDD_LIST_SADDLE };
  BOOL	m_cell;
  BOOL	m_ridge;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CListSadl_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CListSadl_dlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTBS_DLG_H__8E7F36BF_FCE5_402F_A3A6_DEDBCCD516EB__INCLUDED_)
