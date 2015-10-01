#if !defined(AFX_TREEFILTER_DLG_H__D6E36D82_28A9_11D4_8FA1_0040056FBD08__INCLUDED_)
#define AFX_TREEFILTER_DLG_H__D6E36D82_28A9_11D4_8FA1_0040056FBD08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeFilter_dlg.h : header file
//

#include "FeatureFilter_dlg.h"

struct Domain;


struct TreeFilter {
  Elevation min_prom;
  FeatureFilter peak_ff,sadl_ff;
  bool ambig_sadl;
  Domain::Topology topology;
  enum List_runoffs {RO_NONE=0,RO_HIGH,RO_ALL} runoffs;
  bool pp_enable,pp_filter,pp_sort;
  Elev_intvl pp_patrch_prom;
  Elevation pp_child_best,pp_child_worst;
  enum Child_sorting {CS_PROM=0,CS_SADL_ELEV,CS_PEAK_ELEV} child_sorting;
  TreeFilter(Elevation hp=29035) : min_prom(1000),
    ambig_sadl(true),topology(Domain::TOPO_CELL),runoffs(RO_HIGH),
    pp_enable(false),pp_filter(true),pp_sort(true),
    pp_patrch_prom(hp),pp_child_best(hp),pp_child_worst(hp),
    child_sorting(CS_PROM) {}
  bool test(const Domain&, const Database&, const CPromDoc *) const;
  Elevation GetKey(const Domain&, float, const CPromDoc *) const;
};


/////////////////////////////////////////////////////////////////////////////
// CTreeFilter_dlg dialog

class CTreeFilter_dlg : public CDialog
{
  TreeFilter& filter;
  const CPromDoc& doc;

// Construction
public:
  CTreeFilter_dlg(TreeFilter&, const CPromDoc&, CWnd *parent=NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CTreeFilter_dlg)
  enum { IDD = IDD_TREE_FILTER };
  CEdit	m_pp_child_worst_ctl;
  CEdit	m_pp_child_best_ctl;
  int	m_min_prom;
  int	m_pp_hp_max;
  int	m_pp_hp_min;
  BOOL	m_pp_filter;
  BOOL	m_pp_sort;
  BOOL	m_pp_enable;
  int	m_pp_child_worst;
  int	m_pp_child_best;
  int	m_topology;
  int	m_runoffs;
  int	m_sort_children;
  //}}AFX_DATA
  CButton m_peak_elev;
  FeatureFilter m_peak_ff,m_sadl_ff;


// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTreeFilter_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CTreeFilter_dlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  afx_msg void OnPeakFilter();
  afx_msg void OnSadlFilter();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEFILTER_DLG_H__D6E36D82_28A9_11D4_8FA1_0040056FBD08__INCLUDED_)
