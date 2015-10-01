#if !defined(AFX_DOMAPINFO_DLG_H__C93CFFD8_1F03_46E5_A0B2_EE8F3F72076B__INCLUDED_)
#define AFX_DOMAPINFO_DLG_H__C93CFFD8_1F03_46E5_A0B2_EE8F3F72076B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DomapInfo_dlg.h : header file
//

#include "MapInfo_dlg.h"


/////////////////////////////////////////////////////////////////////////////
// CDomapInfo_dlg dialog

class CDomapInfo_dlg : public CMapInfo_dlg
{
// Construction
public:
  CDomapInfo_dlg(const Area_info&, const Area_info&, const CString&, bool);

// Dialog Data
  //{{AFX_DATA(CDomapInfo_dlg)
  enum { IDD = IDD_DOMAP_INFO };
  int	m_ndom_0peak;
  int	m_ndom_1peak;
  int	m_ndom_peaks;
  int	m_ndom_total;
  int	m_ndom_area;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDomapInfo_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDomapInfo_dlg)
    // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOMAPINFO_DLG_H__C93CFFD8_1F03_46E5_A0B2_EE8F3F72076B__INCLUDED_)
