// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_DATAFILTER_DLG_H__5D25DB00_D6F2_11D6_A23A_708052C10000__INCLUDED_)
#define AFX_DATAFILTER_DLG_H__5D25DB00_D6F2_11D6_A23A_708052C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataFilter_dlg.h : header file
//

#define MAX_FIELDS 20


/////////////////////////////////////////////////////////////////////////////
// CDataFilter_dlg dialog

class CDataFilter_dlg : public CDialog
{
  Record_filter& rec_filter;
  CStatic name_label[MAX_FIELDS],type_label[MAX_FIELDS];
  CEdit min_value[MAX_FIELDS],max_value[MAX_FIELDS],
    north_value[MAX_FIELDS],south_value[MAX_FIELDS];
  CComboBox string_rule[MAX_FIELDS];
  void create_minmax_text(const CRect&, const CRect&, unsigned);
  void init_data();

// Construction
protected:
  CDataFilter_dlg(Record_filter&, UINT, CWnd *pParent=NULL);
public:
  CDataFilter_dlg(Record_filter&, CWnd *pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CDataFilter_dlg)
  enum { IDD = IDD_DATA_FILTER };
  BOOL	m_accept_nodata;
  BOOL	m_case;
  BOOL	m_latin1;
  //}}AFX_DATA


// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDataFilter_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDataFilter_dlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAFILTER_DLG_H__5D25DB00_D6F2_11D6_A23A_708052C10000__INCLUDED_)
