// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// PrintMap_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "PrintMap_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPrintMap_dlg dialog

CPrintMap_dlg::CPrintMap_dlg(const CMapView& v, CPrintInfo *pi, CWnd* pParent /*=NULL*/)
  : CDialog(CPrintMap_dlg::IDD, pParent),view(v),pInfo(pi)
{
  //{{AFX_DATA_INIT(CPrintMap_dlg)
  m_print_e = _T("");
  m_print_n = _T("");
  m_print_s = _T("");
  m_print_w = _T("");
  m_div_tree = FALSE;
  m_domap = FALSE;
  m_elev_map = FALSE;
  m_unit = -1;
  //}}AFX_DATA_INIT
}

void CPrintMap_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPrintMap_dlg)
  DDX_Control(pDX, IDC_ELEV_MAP, m_elev_map_ctl);
  DDX_Control(pDX, IDC_DOMAP, m_domap_ctl);
  DDX_Control(pDX, IDC_DIV_TREE, m_div_tree_ctl);
  DDX_Control(pDX, IDC_PAGES, m_pages_edit);
  DDX_Control(pDX, IDC_PRINT_W, m_edit_w);
  DDX_Control(pDX, IDC_PRINT_S, m_edit_s);
  DDX_Control(pDX, IDC_PRINT_N, m_edit_n);
  DDX_Control(pDX, IDC_PRINT_E, m_edit_e);
  DDX_Control(pDX, IDC_LAST_PRINT, m_last_print_butn);
  DDX_Text(pDX, IDC_PRINT_E, m_print_e);
  DDX_Text(pDX, IDC_PRINT_N, m_print_n);
  DDX_Text(pDX, IDC_PRINT_S, m_print_s);
  DDX_Text(pDX, IDC_PRINT_W, m_print_w);
  DDX_Check(pDX, IDC_DIV_TREE, m_div_tree);
  DDX_Check(pDX, IDC_DOMAP, m_domap);
  DDX_Check(pDX, IDC_ELEV_MAP, m_elev_map);
  DDX_Radio(pDX, IDC_UNIT_DEGREES, m_unit);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPrintMap_dlg, CDialog)
  //{{AFX_MSG_MAP(CPrintMap_dlg)
  ON_BN_CLICKED(IDC_PRINT_SETUP, OnPrintSetup)
  ON_BN_CLICKED(IDC_WINDOW, OnWindow)
  ON_BN_CLICKED(IDC_LAST_PRINT, OnLastPrint)
  ON_BN_CLICKED(IDC_WORKSPACE, OnWorkspace)
  ON_EN_KILLFOCUS(IDC_PRINT_N, OnUpdatePages)
  ON_EN_KILLFOCUS(IDC_PRINT_E, OnUpdatePages)
  ON_EN_KILLFOCUS(IDC_PRINT_S, OnUpdatePages)
  ON_EN_KILLFOCUS(IDC_PRINT_W, OnUpdatePages)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPrintMap_dlg message handlers

void CPrintMap_dlg::set_window_limits()
{
  CRect clientRect;
  view.GetClientRect(&clientRect);
  CPoint point_nw=view.GetScrollPosition(),
    point_sw=point_nw+CPoint(0,clientRect.bottom),
    point_ne=point_nw+CPoint(clientRect.right,0);
  limits.set(view.point2gp(point_sw),view.point2gp(point_ne));
}

void CPrintMap_dlg::set_pages()
{
  if (limits.is_empty()) {
    m_pages_edit.SetWindowText("");
    return;
  }
  CPoint sw=view.gp2point(limits.corner1()),ne=view.gp2point(limits.corner2());
  view.print_params.job_size.cx=ne.x-sw.x+2*GRAPH_VIEW_MARGIN;
  view.print_params.job_size.cy=sw.y-ne.y+2*GRAPH_VIEW_MARGIN;
  view.print_params.update(*pInfo->m_pPD);
  sprintf(CPromDoc::buf,"%d",view.print_params.nxpag*view.print_params.nypag);
  m_pages_edit.SetWindowText(CPromDoc::buf);
}

BOOL CPrintMap_dlg::OnInitDialog()
{
  if (!view.print_limits.is_empty())
    limits=view.print_limits;
  else set_window_limits();
  m_unit=0;
  set_limit_text();
  CPromDoc *doc=view.GetDocument();
  m_elev_map=(view.view_em || view.current.light_int>0 || view.current.elev_color_enable) &&
    doc->em_state!=NOT_LOADED;
  m_domap=view.view_dm && doc->dm_state!=NOT_LOADED;
  m_div_tree=view.view_dt && doc->dt_state!=NOT_LOADED;

  CDialog::OnInitDialog();

  if (!m_elev_map) m_elev_map_ctl.EnableWindow(FALSE);
  if (!m_domap) m_domap_ctl.EnableWindow(FALSE);
  if (!m_div_tree) m_div_tree_ctl.EnableWindow(FALSE);
  if (view.print_limits.is_empty())
    m_last_print_butn.EnableWindow(FALSE);
  set_pages();

  return TRUE;  // return TRUE unless you set the focus to a control
}

void CPrintMap_dlg::OnPrintSetup()
{
  if (AfxGetApp()->DoPrintDialog(pInfo->m_pPD) != IDOK) return;
  set_pages();
}

void CPrintMap_dlg::OnWorkspace()
{
  UpdateData();
  Area_info alloc,defined; // alloc just keeps document calls happy
  Elev_intvl elev_range;
  CPromDoc *doc=view.GetDocument();
  if (m_elev_map) doc->GetElevMapArea(alloc,defined,elev_range);
  if (m_domap) doc->GetDomapArea(alloc,defined);
  if (m_div_tree) doc->tree.area(alloc.rect,defined.rect);
  if (defined.rect.is_empty()) {
    AfxGetMainWnd()->MessageBox("You\'ve turned off all workspace elements.");
    return;
  }
  UpdateData();
  set_limit_text(defined.rect);
  UpdateData(FALSE);
  set_pages();
}

void CPrintMap_dlg::OnWindow()
{
  set_window_limits();
  UpdateData();
  set_limit_text();
  UpdateData(FALSE);
  set_pages();
}

void CPrintMap_dlg::OnLastPrint()
{
  UpdateData();
  set_limit_text(view.print_limits);
  UpdateData(FALSE);
  set_pages();
}

void CPrintMap_dlg::OnUpdatePages()
{
  CEdit *bad_field;
  get_limit_text(bad_field);
  set_pages();
}

void CPrintMap_dlg::OnOK()
{
  CEdit *bad_field;
  const char *status=get_limit_text(bad_field);
  if (status) {
    MessageBox(status,"Winprom Entry Error",MB_OK|MB_ICONERROR);
    if (bad_field) {
      bad_field->SetFocus();
      bad_field->SetSel(0,-1);
    }
    return;
  }
  EndDialog(IDOK);
}

const char *CPrintMap_dlg::get_limit_text(CEdit *&bad_field)
{
  static char buf[64];
  if (!UpdateData(TRUE)) return 0;

  GridPoint::Coord limit_n,limit_s,limit_e,limit_w;

  switch (m_unit) {
  case 0:
    GridPoint_equat::angle_format=AF_DEGREE;
    break;
  case 1:
    GridPoint_equat::angle_format=AF_RADIAN;
    break;
  case 2:
    GridPoint_equat::angle_format=AF_GRAD;
    break;
  default:
    assert(0);
  }
  limit_n=GridPoint::sscanx(m_print_n);
  limit_e=GridPoint::sscany(m_print_e);
  limit_s=GridPoint::sscanx(m_print_s);
  limit_w=GridPoint::sscany(m_print_w);
  bad_field=0;
  limits.empty();
  if (limit_n==coord_infty+1) {
    bad_field=&m_edit_n;
    sprintf(buf,"Format error in %s limit",GridPoint::bound1_high);
    return buf;
  }
  if (limit_e==coord_infty+1) {
    bad_field=&m_edit_e;
    sprintf(buf,"Format error in %s limit",GridPoint::bound2_high);
    return buf;
  }
  if (limit_s==coord_infty+1) {
    bad_field=&m_edit_s;
    sprintf(buf,"Format error in %s limit",GridPoint::bound1_low);
    return buf;
  }
  if (limit_w==coord_infty+1) {
    bad_field=&m_edit_w;
    sprintf(buf,"Format error in %s limit",GridPoint::bound2_low);
    return buf;
  }

  if (limit_n<=limit_s) {
    sprintf(buf,"%s limits are out of order.",GridPoint::coord1_label);
    return buf;
  }
  if (limit_e<=limit_w) {
    sprintf(buf,"%s limits are out of order.",GridPoint::coord2_label);
    return buf;
  }

  GridPoint sw,ne;
  sw.set_cart(limit_s,limit_w);
  ne.set_cart(limit_n,limit_e);
  limits.set(sw,ne);

  return 0;
}

void CPrintMap_dlg::set_limit_text()
{
  char (&buf)[sizeof CPromDoc::buf]=CPromDoc::buf;
  Location_fmt loc_fmt;
  switch (m_unit) {
  case 0:
    loc_fmt.angle_f=AF_DMS;
    break;
  case 1:
    loc_fmt.angle_f=AF_RADIAN;
    break;
  case 2:
    loc_fmt.angle_f=AF_GRAD;
    break;
  }
  loc_fmt.set();
  const GridPoint sw=limits.corner1(),ne=limits.corner2();
  ne.sprint1(buf);
  m_print_n=buf;
  sw.sprint1(buf);
  m_print_s=buf;
  ne.sprint2(buf);
  m_print_e=buf;
  sw.sprint2(buf);
  m_print_w=buf;
}
