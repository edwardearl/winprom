#if !defined(AFX_DATALOCATIONFILTER_DLG_H__9E1214A5_09B3_11D7_A23A_B08452C10000__INCLUDED_)
#define AFX_DATALOCATIONFILTER_DLG_H__9E1214A5_09B3_11D7_A23A_B08452C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataLocationFilter_dlg.h : header file
//

#include "DataFilter_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CDataLocationFilter_dlg dialog

class CDataLocationFilter_dlg : public CDataFilter_dlg
{
  Record_location_filter& filter;

// Construction
public:
  CDataLocationFilter_dlg(Record_location_filter&, CWnd *pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CDataLocationFilter_dlg)
  enum { IDD = IDD_DATA_LOC_FILTER };
  CEdit	m_west_ctl;
  CEdit	m_south_ctl;
  CEdit	m_east_ctl;
  CEdit	m_north_ctl;
  CString	m_east;
  CString	m_north;
  CString	m_south;
  CString	m_west;
  BOOL	m_nonorph;
  BOOL	m_orphan;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDataLocationFilter_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDataLocationFilter_dlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATALOCATIONFILTER_DLG_H__9E1214A5_09B3_11D7_A23A_B08452C10000__INCLUDED_)
