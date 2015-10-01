#if !defined(AFX_LISTFILTER_DLG_H__81DA1A06_CA5D_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_LISTFILTER_DLG_H__81DA1A06_CA5D_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ListFilter_dlg.h : header file
//

#include "FeatureFilter_dlg.h"


struct ListFilter : FeatureFilter {
  Elevation min_prom;
  bool offmap;
  ListFilter(bool e=false) : min_prom(1000),offmap(false)
    {edge=e;}
};


struct PromListItem {
  Feature *feature;
  Elev_intvl prom;
  Elevation offmap_prom;
  PromListItem() : feature(0) {}
  PromListItem(Feature& f, const Elev_intvl& p) : feature(&f),prom(p) {}
  bool operator>(const PromListItem& a) const {return prom.high>a.prom.high;}
  void transform(float);
};


/////////////////////////////////////////////////////////////////////////////
// CListFilter_dlg dialog

class CListFilter_dlg : public CDialog
{
  ListFilter& filter;
  FeatureFilter temp_ff;
  UINT feature_icon_id;
  const Database& data;
  void data_init();

// Construction
protected:
  CListFilter_dlg(UINT, const Database&, ListFilter&, UINT, CWnd *parent=NULL);  // standard constructor
public:
  CListFilter_dlg(const Database&, ListFilter&, UINT, CWnd *parent=NULL);  // standard constructor

// Dialog Data
  //{{AFX_DATA(CListFilter_dlg)
  enum { IDD = IDD_LIST };
  int	m_prom;
  BOOL	m_offmap;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CListFilter_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CListFilter_dlg)
  afx_msg void OnFilter();
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTFILTER_DLG_H__81DA1A06_CA5D_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
