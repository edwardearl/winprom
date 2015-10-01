// WSinfo_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "WSinfo_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


Location_fmt CWSinfo_dlg::info_fmt;


/////////////////////////////////////////////////////////////////////////////
// CWSinfo_dlg dialog

CWSinfo_dlg::CWSinfo_dlg(const Area_info& a, const Area_info& d)
  : CDialog(CWSinfo_dlg::IDD, NULL),alloc(a),defined(d)
{
  //{{AFX_DATA_INIT(CWSinfo_dlg)
  m_alloc_e = _T("");
  m_alloc_n = _T("");
  m_alloc_s = _T("");
  m_alloc_w = _T("");
  m_defined_e = _T("");
  m_defined_n = _T("");
  m_defined_s = _T("");
  m_defined_w = _T("");
  m_quad_lat = _T("");
  m_quad_lon = _T("");
  m_step_lat = _T("");
  m_step_lon = _T("");
  m_quad_coord1 = _T("");
  m_quad_coord2 = _T("");
  m_step_coord1 = _T("");
  m_step_coord2 = _T("");
  //}}AFX_DATA_INIT
}


void CWSinfo_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CWSinfo_dlg)
  DDX_Text(pDX, IDC_ALLOC_E, m_alloc_e);
  DDX_Text(pDX, IDC_ALLOC_N, m_alloc_n);
  DDX_Text(pDX, IDC_ALLOC_S, m_alloc_s);
  DDX_Text(pDX, IDC_ALLOC_W, m_alloc_w);
  DDX_Text(pDX, IDC_DEFINED_E, m_defined_e);
  DDX_Text(pDX, IDC_DEFINED_N, m_defined_n);
  DDX_Text(pDX, IDC_DEFINED_S, m_defined_s);
  DDX_Text(pDX, IDC_DEFINED_W, m_defined_w);
  DDX_Text(pDX, IDC_QUAD_LAT, m_quad_lat);
  DDX_Text(pDX, IDC_QUAD_LON, m_quad_lon);
  DDX_Text(pDX, IDC_STEP_LAT, m_step_lat);
  DDX_Text(pDX, IDC_STEP_LON, m_step_lon);
  DDX_Text(pDX, IDC_QUAD_COORD1, m_quad_coord1);
  DDX_Text(pDX, IDC_QUAD_COORD2, m_quad_coord2);
  DDX_Text(pDX, IDC_STEP_COORD1, m_step_coord1);
  DDX_Text(pDX, IDC_STEP_COORD2, m_step_coord2);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWSinfo_dlg, CDialog)
  //{{AFX_MSG_MAP(CWSinfo_dlg)
  ON_BN_CLICKED(IDC_FORMAT, OnFormat)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWSinfo_dlg message handlers

void CWSinfo_dlg::reformat()
{
  char (&buf)[sizeof CPromDoc::buf]=CPromDoc::buf;

  ((GridPoint)alloc.rect.corner2()).sprint1(buf);
  m_alloc_n=buf;
  ((GridPoint)alloc.rect.corner1()).sprint1(buf);
  m_alloc_s=buf;
  ((GridPoint)alloc.rect.corner2()).sprint2(buf);
  m_alloc_e=buf;
  ((GridPoint)alloc.rect.corner1()).sprint2(buf);
  m_alloc_w=buf;

  ((GridPoint)defined.rect.corner2()).sprint1(buf);
  m_defined_n=buf;
  ((GridPoint)defined.rect.corner1()).sprint1(buf);
  m_defined_s=buf;
  ((GridPoint)defined.rect.corner2()).sprint2(buf);
  m_defined_e=buf;
  ((GridPoint)defined.rect.corner1()).sprint2(buf);
  m_defined_w=buf;

  GridPoint::sprint_quadx(buf);
  m_quad_lat=buf;
  GridPoint::sprint_quady(buf);
  m_quad_lon=buf;

  GridPoint::sprint_xstep(buf);
  m_step_lat=buf;
  GridPoint::sprint_ystep(buf);
  m_step_lon=buf;
}

BOOL CWSinfo_dlg::OnInitDialog()
{
  m_quad_coord1=m_step_coord1=GridPoint::coord1_label;
  m_quad_coord2=m_step_coord2=GridPoint::coord2_label;
  info_fmt.set();
  reformat();

  CDialog::OnInitDialog();

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}

void CWSinfo_dlg::OnFormat()
{
  CLocationFmt_dlg dlg(this,info_fmt);
  if (dlg.DoModal()==IDOK) {
    info_fmt.set();
    reformat();
    UpdateData(FALSE);
  }
}
