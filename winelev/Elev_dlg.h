// Elev_dlg.h : header file
//

#if !defined(AFX_ELEV_DLG_H__DDC810A9_D24C_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_ELEV_DLG_H__DDC810A9_D24C_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "../winprom/Grid.h"

// this list must match the radio buttons in the "from type" box
enum FromType {
  FROM_SRTM,FROM_NED,FROM_UDEM,FROM_GTOPO30,FROM_GLOBE,
  FROM_ETOPO2,FROM_RAMP,FROM_ELEV,FROM_DELEV,FROM_AUTO
};

enum ConvertStatus {CF_SUCCESS,CF_FAIL,CF_ABORT};


/////////////////////////////////////////////////////////////////////////////
// CElev_dlg dialog

class CElev_dlg : public CDialog
{
  friend class CConvertCtl_dlg;
  float conv_fac;
  CFileDialog *files_dlg;
  CConvertCtl_dlg *control_dlg;
  static void set_suffix(CString&, const char *);
  FromType get_auto_type(const char *) const;
  FromType get_from_type(const char *) const;
  bool check_exist(const char *) throw(file_error_name);
  ConvertStatus convert_file(const CString&, void (CElev_dlg::*)(const CString&));
  void process_hgt_bil(const char *, FILE *, unsigned short) throw(file_error_name);
  void srtm_port(const CString&) throw(file_error_name);
  void ned_port(const CString&) throw(file_error_name);
  void dem_port(const CString&) throw(file_error_name);
  void gtopo30_port(const CString&) throw(file_error_name);
  void globe_port(const CString&) throw(file_error_name);
  void etopo2_port(const CString&) throw(file_error_name);
  void ramp_port(const CString&) throw(file_error_name);
  void process_elev(Elev_map&, CString) throw(file_error_name);
  void process_elev(const CString&) throw(file_error_name);
  void process_delev(const CString&) throw(file_error_name);
  bool breakup_bin_equat(FILE *, bool,
			 GridPoint_cart::Coord, GridPoint_cart::Coord,
			 GridPoint_cart::Coord, GridPoint_cart::Coord,
			 Elev_map[], GridPoint_equat[], Elevation[],
			 Elevation, Elevation);
  bool breakup_bin_polar(FILE *, bool, const char *, int,
			 GridPoint_cart::Coord, GridPoint_cart::Coord,
			 Elev_map[], GridPoint_polar[], Elevation[],
			 Elevation, Elevation);

// Construction
public:
  CElev_dlg();	// standard constructor

  UINT convert_files();

// Dialog Data
  //{{AFX_DATA(CElev_dlg)
  enum { IDD = IDD_WINELEV_DIALOG };
  CComboBox	m_conv_fac_ctl;
  int	m_exist_action;
  BOOL	m_error_prompt;
  BOOL	m_blank;
  CString	m_conv_fac;
  int	m_from_type;
  BOOL	m_delsrc;
  BOOL	m_to_domap;
  BOOL	m_to_elevmap;
  BOOL	m_to_divtree;
  BOOL	m_ocean_def;
  BOOL	m_to_diffelev;
  //}}AFX_DATA

  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CElev_dlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:
  HICON m_hIcon;

  // Generated message map functions
  //{{AFX_MSG(CElev_dlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnDestroy();
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void OnConvert();
  afx_msg void OnAbout();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEV_DLG_H__DDC810A9_D24C_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
