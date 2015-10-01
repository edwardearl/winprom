// MapInfo_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "MapInfo_dlg.h"
#include "PromDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


Location_fmt CMapInfo_dlg::info_fmt;


/////////////////////////////////////////////////////////////////////////////
// CMapInfo_dlg dialog

void CMapInfo_dlg::init()
{
  //{{AFX_DATA_INIT(CMapInfo_dlg)
  m_alloc_area = 0.0f;
  m_defined_area = 0.0f;
  m_alloc_e = _T("");
  m_alloc_gp = 0;
  m_alloc_n = _T("");
  m_alloc_s = _T("");
  m_alloc_w = _T("");
  m_defined_e = _T("");
  m_defined_gp = 0;
  m_defined_n = _T("");
  m_defined_s = _T("");
  m_defined_w = _T("");
  m_file = _T("");
  //}}AFX_DATA_INIT
}

CMapInfo_dlg::CMapInfo_dlg(const Area_info& a, const Area_info& d,
			   const CString& n, bool m)
  : CDialog(CMapInfo_dlg::IDD, NULL),alloc(a),defined(d),
    name(n),modified(m)
{
  init();
}

CMapInfo_dlg::CMapInfo_dlg(const Area_info& a, const Area_info& d,
			   const CString& n, bool m, UINT nID)
  : CDialog(nID, NULL),alloc(a),defined(d),
    name(n),modified(m)
{
  init();
}

void CMapInfo_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CMapInfo_dlg)
  DDX_Text(pDX, IDC_ALLOC_AREA, m_alloc_area);
  DDX_Text(pDX, IDC_DEFINED_AREA, m_defined_area);
  DDX_Text(pDX, IDC_ALLOC_E, m_alloc_e);
  DDX_Text(pDX, IDC_ALLOC_GP, m_alloc_gp);
  DDX_Text(pDX, IDC_ALLOC_N, m_alloc_n);
  DDX_Text(pDX, IDC_ALLOC_S, m_alloc_s);
  DDX_Text(pDX, IDC_ALLOC_W, m_alloc_w);
  DDX_Text(pDX, IDC_DEFINED_E, m_defined_e);
  DDX_Text(pDX, IDC_DEFINED_GP, m_defined_gp);
  DDX_Text(pDX, IDC_DEFINED_N, m_defined_n);
  DDX_Text(pDX, IDC_DEFINED_S, m_defined_s);
  DDX_Text(pDX, IDC_DEFINED_W, m_defined_w);
  DDX_Text(pDX, IDC_FILE, m_file);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapInfo_dlg, CDialog)
  //{{AFX_MSG_MAP(CMapInfo_dlg)
  ON_BN_CLICKED(IDC_FORMAT, OnFormat)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapInfo_dlg message handlers

void CMapInfo_dlg::reformat()
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

  if (!defined.rect.is_empty()) {
    ((GridPoint)defined.rect.corner2()).sprint1(buf);
    m_defined_n=buf;
    ((GridPoint)defined.rect.corner1()).sprint1(buf);
    m_defined_s=buf;
    ((GridPoint)defined.rect.corner2()).sprint2(buf);
    m_defined_e=buf;
    ((GridPoint)defined.rect.corner1()).sprint2(buf);
    m_defined_w=buf;
  }
  else {
    m_defined_n=m_defined_s=m_defined_e=m_defined_w="";
  }
}

BOOL CMapInfo_dlg::OnInitDialog()
{
  m_file=name;
  if (modified) m_file+=" *";

  m_alloc_area=alloc.area;
  m_alloc_gp=alloc.ngp;
  m_defined_area=defined.area;
  m_defined_gp=defined.ngp;

  info_fmt.set();
  reformat();

  CDialog::OnInitDialog();

  return TRUE;  // return TRUE unless you set the focus to a control
	        // EXCEPTION: OCX Property Pages should return FALSE
}

void CMapInfo_dlg::OnFormat()
{
  CLocationFmt_dlg dlg(this,info_fmt);
  if (dlg.DoModal()==IDOK) {
    info_fmt.set();
    reformat();
    UpdateData(FALSE);
  }
}
