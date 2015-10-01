#if !defined(AFX_DATAALIGN_DLG_H__9E1214A3_09B3_11D7_A23A_B08452C10000__INCLUDED_)
#define AFX_DATAALIGN_DLG_H__9E1214A3_09B3_11D7_A23A_B08452C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataAlign_dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CDataAlign_dlg dialog

class CDataAlign_dlg : public CDialog
{
  bool have_elev,have_dom;
  const Database& data;

// Construction
public:
  CDataAlign_dlg(bool, bool, const Database&, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CDataAlign_dlg)
  enum { IDD = IDD_DATA_ALIGN };
  CComboBox	m_mult_rec_sadl_list;
  CComboBox	m_mult_rec_peak_list;
  BOOL	m_orphan;
  int	m_mult_rec_peak;
  int	m_mult_rec_sadl;
  BOOL	m_distance;
  int	m_pritype;
  BOOL	m_excise;
  BOOL	m_prom;
  //}}AFX_DATA
  Assoc_info m_assoc;
  Database::FieldID mrf_peak,mrf_sadl;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDataAlign_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDataAlign_dlg)
  afx_msg void OnAssoc();
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAALIGN_DLG_H__9E1214A3_09B3_11D7_A23A_B08452C10000__INCLUDED_)
