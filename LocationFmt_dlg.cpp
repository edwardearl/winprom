// LocationFmt_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "LocationFmt_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static const char *real_fmt[][2]={ // [GridPoint::angle_format][news_f]
  "%10.4f","%8.4f°%c",
  0,0,0,0,
  "%10.6f","%9.6f%c",
  "%9.4fG","%8.4fG%c",
};

static const char
  *loc_fld_fmt1="      %9s      ",
  *loc_fld_fmt2="       %9s       ";
static const char *loc_field_fmt[]={ // [GridPoint::angle_format]
  loc_fld_fmt1,loc_fld_fmt1,loc_fld_fmt2,loc_fld_fmt2,loc_fld_fmt1,loc_fld_fmt1
};

static char custom_fmt[256];

void Location_fmt::set(bool csv) const
{
  if (angle_f!=AF_UTM) {
    GridPoint::angle_format=angle_f;
    GridPoint::real_format=real_fmt[angle_f][news_f];
  }
  GridPoint::field_format = csv && angle_f!=AF_UTM ?
    "Latitude,Longitude" : loc_field_fmt[angle_f];
  if (custom.IsEmpty()) {
    GridPoint::format=angle_f==AF_UTM?"%Z %E %N":csv?"%A,%O":"%A %O";
  }
  else {
    strcpy(custom_fmt,custom);
    GridPoint::format=custom_fmt;
  }
}

/////////////////////////////////////////////////////////////////////////////
// CLocationFmt_dlg dialog

CLocationFmt_dlg::CLocationFmt_dlg(CWnd *parent, Location_fmt& lf)
	: CDialog(CLocationFmt_dlg::IDD,parent),target(lf)
{
  //{{AFX_DATA_INIT(CLocationFmt_dlg)
  m_news = FALSE;
  m_custom = _T("");
  m_angle_fmt = -1;
  //}}AFX_DATA_INIT
}


void CLocationFmt_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CLocationFmt_dlg)
  DDX_Control(pDX, IDC_CUSTOM, m_custom_edit);
  DDX_Check(pDX, IDC_NEWS, m_news);
  DDX_Text(pDX, IDC_CUSTOM, m_custom);
  DDX_Radio(pDX, IDC_AF_DEG, m_angle_fmt);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLocationFmt_dlg, CDialog)
  //{{AFX_MSG_MAP(CLocationFmt_dlg)
  ON_BN_CLICKED(IDC_RESET, OnReset)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLocationFmt_dlg message handlers

BOOL CLocationFmt_dlg::OnInitDialog() 
{
  switch (target.angle_f) {
  case AF_DEGREE: m_angle_fmt=0; break;
  case AF_DM: m_angle_fmt=1; break;
  case AF_DMS: m_angle_fmt=2; break;
  case AF_RADIAN: m_angle_fmt=3; break;
  case AF_GRAD: m_angle_fmt=4; break;
  case AF_UTM: m_angle_fmt=5; break;
  default:
    assert(0);
    break;
  }
  m_news=target.news_f;
  m_custom=target.custom;

  CDialog::OnInitDialog();

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}

void CLocationFmt_dlg::set_target(Location_fmt& tgt) const
{
  switch (m_angle_fmt) {
  case 0: tgt.angle_f=AF_DEGREE; break;
  case 1: tgt.angle_f=AF_DM; break;
  case 2: tgt.angle_f=AF_DMS; break;
  case 3: tgt.angle_f=AF_RADIAN; break;
  case 4: tgt.angle_f=AF_GRAD; break;
  case 5: tgt.angle_f=AF_UTM; break;
  default:
    assert(0);
    break;
  }
  tgt.news_f=m_news!=FALSE;
  tgt.custom=m_custom;
}

void CLocationFmt_dlg::OnOK()
{
  if (!UpdateData(TRUE)) return;

  if (m_angle_fmt==5 && !m_custom.IsEmpty()) {
    MessageBox("Custom format is not allowed with UTM.",
	       "Winprom Location Format Error",MB_OK|MB_ICONERROR);
    return;
  }

  const char *custm=m_custom;
  for (const char *fc=custm; *fc; ++fc) {
    if (*fc=='%') {
      switch (*++fc) {
      case 'A': case 'a':
      case 'O': case 'o':
      case 'Z': case 'z':
      case 'N': case 'n':
      case 'E': case 'e':
      case '%':
	break;
      default:
	MessageBox("An invalid character appears after \"%\" in the custom format.\n"
		   "Valid format strings are %A, %O, %Z, %N, %E, and %%.",
		   "Winprom Location Format Error",MB_OK|MB_ICONERROR);
	m_custom_edit.SetFocus();
	m_custom_edit.SetSel(fc-custm,fc-custm+1);
	return;
      }
    }
  }

  set_target(target);

  EndDialog(IDOK);
}

void CLocationFmt_dlg::OnReset()
{
  m_custom_edit.SetWindowText("");
  UpdateData(TRUE);
  Location_fmt tmp_tgt;
  set_target(tmp_tgt);
  tmp_tgt.set(false);
  m_custom_edit.SetWindowText(GridPoint::format);
}
