#if !defined(AFX_RUNOFFNBR_DLG_H__9B338F65_009D_11D4_8FA0_0040056FBD08__INCLUDED_)
#define AFX_RUNOFFNBR_DLG_H__9B338F65_009D_11D4_8FA0_0040056FBD08__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RunoffNbr_dlg.h : header file
//

enum Runoff_nbr_type {RO_NBR_SAME_GP,RO_NBR_NBR_GP,RO_NBR_RADIUS};


/////////////////////////////////////////////////////////////////////////////
// CRunoffNbr_dlg dialog

class CRunoffNbr_dlg : public CDialog
{
// Construction
public:
  CRunoffNbr_dlg(CWnd *, Runoff_nbr_type, float);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CRunoffNbr_dlg)
  enum { IDD = IDD_RUNOFF_NBR };
  float	m_radius;
  int		m_grid;
  //}}AFX_DATA


// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CRunoffNbr_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CRunoffNbr_dlg)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNOFFNBR_DLG_H__9B338F65_009D_11D4_8FA0_0040056FBD08__INCLUDED_)
