// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
#if !defined(AFX_EDITEDLIST_DLG_H__3C006F90_5A23_48E7_B54D_8143A57F793D__INCLUDED_)
#define AFX_EDITEDLIST_DLG_H__3C006F90_5A23_48E7_B54D_8143A57F793D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditedList_dlg.h : header file
//


struct EditFilter : public FeatureFilter {
  bool elev,name;
  EditFilter() : elev(true),name(false) {}
  bool test(const Feature& f, const Database& data) const
    {return ((elev && f.edited) || (name && f.name) &&
     FeatureFilter::test(f,data));}
};


/////////////////////////////////////////////////////////////////////////////
// CEditedList_dlg dialog

class CEditedList_dlg : public CDialog
{
  EditFilter& filter;

// Construction
public:
  CEditedList_dlg(const Database&, EditFilter&, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CEditedList_dlg)
  enum { IDD = IDD_EDITED_LIST };
  BOOL	m_elev;
  BOOL	m_name;
  //}}AFX_DATA
  FeatureFilter m_filter;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CEditedList_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CEditedList_dlg)
  afx_msg void OnFilter();
  virtual void OnOK();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITEDLIST_DLG_H__3C006F90_5A23_48E7_B54D_8143A57F793D__INCLUDED_)
