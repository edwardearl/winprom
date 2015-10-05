// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// Contour_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "Contour_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CContour_dlg dialog

CContour_dlg::CContour_dlg()
  : CDialog(CContour_dlg::IDD, NULL)
{
  //{{AFX_DATA_INIT(CContour_dlg)
  m_major = 0;
  m_minor = 0;
  m_standard = 0;
  m_autozoom = FALSE;
  m_GPontop = -1;
  m_datum = 0;
  m_enable = FALSE;
  //}}AFX_DATA_INIT
}


void CContour_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CContour_dlg)
  DDX_Control(pDX, IDC_STANDARD, m_standard_ctl);
  DDX_Control(pDX, IDC_MINOR, m_minor_ctl);
  DDX_Control(pDX, IDC_MAJOR, m_major_ctl);
  DDX_Text(pDX, IDC_MAJOR, m_major);
  DDX_Text(pDX, IDC_MINOR, m_minor);
  DDX_Text(pDX, IDC_STANDARD, m_standard);
  DDX_Check(pDX, IDC_AUTOZOOM, m_autozoom);
  DDX_Radio(pDX, IDC_GRID_ONTOP, m_GPontop);
  DDX_Text(pDX, IDC_DATUM, m_datum);
  DDX_Check(pDX, IDC_ENABLE, m_enable);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContour_dlg, CDialog)
  //{{AFX_MSG_MAP(CContour_dlg)
  ON_BN_CLICKED(IDC_CONTOUR_COLOR, OnContourColor)
  ON_BN_CLICKED(IDC_VOID_COLOR, OnVoidColor)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CContour_dlg message handlers

BOOL CContour_dlg::OnInitDialog() 
{
  CDialog::OnInitDialog();

  m_box_contour.SubclassDlgItem(IDC_BOX_CONTOUR,this);
  m_box_contour.set_color(m_color_contour);
  m_box_void.SubclassDlgItem(IDC_BOX_VOID,this);
  m_box_void.set_color(m_color_void);

  return TRUE;  // return TRUE unless you set the focus to a control
}

void CContour_dlg::OnOK()
{
  if (!UpdateData()) return;

  if (m_major<=0) {
    MessageBox("Major contour interval must be a\n"
	       "positive number of standard intervals",
	       "Winprom Entry Error",MB_OK|MB_ICONERROR);
    m_major_ctl.SetSel(0,-1);
    m_major_ctl.SetFocus();
    return;
  }

  if (m_standard<=0) {
    MessageBox("Standard contour interval must be a\n"
	       "positive number of minor intervals",
	       "Winprom Entry Error",MB_OK|MB_ICONERROR);
    m_standard_ctl.SetSel(0,-1);
    m_standard_ctl.SetFocus();
    return;
  }

  if (m_minor<=0) {
    MessageBox("Minor contour interval must be positive",
	       "Winprom Entry Error",MB_OK|MB_ICONERROR);
    m_minor_ctl.SetSel(0,-1);
    m_minor_ctl.SetFocus();
    return;
  }

  EndDialog(IDOK);
}

void CContour_dlg::OnContourColor()
{
  CViewColorDialog dlg(m_color_contour,"Contour Color");
  if (dlg.DoModal()!=IDOK) return;

  m_color_contour=dlg.GetColor();
  m_box_contour.set_color(m_color_contour);
}

void CContour_dlg::OnVoidColor()
{
  CViewColorDialog dlg(m_color_void,"Void Elevation Color");
  if (dlg.DoModal()!=IDOK) return;

  m_color_void=dlg.GetColor();
  m_box_void.set_color(m_color_void);
}
