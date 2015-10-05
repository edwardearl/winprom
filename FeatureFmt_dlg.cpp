// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// FeatureFmt_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "FeatureFmt_dlg.h"
#include "LocationFmt_dlg.h"
#include "point.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static char buf1[256],buf2[256];


/////////////////////////////////////////////////////////////////////////////
// CFeatureFmt_dlg dialog

CFeatureFmt_dlg::CFeatureFmt_dlg(const Database& d, CWnd *parent, Feature_fmt& ftr_fmt)
  : CDialog(CFeatureFmt_dlg::IDD,parent),target(ftr_fmt),
    temp_loc_fmt(target.loc_f),data(d)
{
  //{{AFX_DATA_INIT(CFeatureFmt_dlg)
  m_name_fmt = -1;
  m_precise = FALSE;
  m_show_loc = FALSE;
  m_show_elev = FALSE;
  m_elev_fmt = -1;
  m_custom = _T("");
  //}}AFX_DATA_INIT
}


void CFeatureFmt_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CFeatureFmt_dlg)
  DDX_Control(pDX, IDC_RESET_TEXT, m_reset_text_butn);
  DDX_Control(pDX, IDC_RESET_HTML, m_reset_html_butn);
  DDX_Control(pDX, IDC_RESET_CSV, m_reset_csv_butn);
  DDX_Control(pDX, IDC_CUSTOM, m_custom_ctl);
  DDX_Radio(pDX, IDC_NAME_NONE, m_name_fmt);
  DDX_Check(pDX, IDC_PRECISE, m_precise);
  DDX_Check(pDX, IDC_SHOW_LOC, m_show_loc);
  DDX_Check(pDX, IDC_SHOW_ELEV, m_show_elev);
  DDX_Radio(pDX, IDC_ELEV_MIDPT, m_elev_fmt);
  DDX_Text(pDX, IDC_CUSTOM, m_custom);
  DDV_MaxChars(pDX, m_custom, 255);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeatureFmt_dlg, CDialog)
  //{{AFX_MSG_MAP(CFeatureFmt_dlg)
  ON_BN_CLICKED(ID_LOCATION, OnLocation)
  ON_BN_CLICKED(IDC_RESET, OnReset)
  ON_BN_CLICKED(IDC_RESET_TEXT, OnResetText)
  ON_BN_CLICKED(IDC_RESET_HTML, OnResetHTML)
  ON_BN_CLICKED(IDC_RESET_CSV, OnResetCSV)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Format structure implementation

static const char *elev_fmt[][3]={ // [csv][Feature_fmt::Elev_fmt]
  "%M","%M±%R","%L to %H",
  "%M","%M,%R","%L,%H"
};
static const char *precise_fmt[]={ // [Feature_fmt::elev_fmt]
  "%5d","  %5d  ","    %5d     "
};
static const char *max_fmt[][3]={ // [csv][Feature_fmt::elev_fmt]
  "%4d-","  %5d- ","    %5d-    ",
  "%d-",",%d",",%d"
};
static const char *min_fmt[][3]={ // [csv][Feature_fmt::elev_fmt]
  "%4d+","  %5d+ ","    %5d+    ",
  "%d+","%d,","%d,"
};
static const char *elev_field_fmt[][3]={ // [csv][Feature_fmt::elev_fmt]
  "%5s","  %5s  ","    %5s     ",
  "%5s","%5s,","%5s,"
};

static const char *noflag_feature_fmt[][2][2]={ // [name_fmt][show_loc][show_elev]
  "","%E","%L","%E%L",
  "%AE","%AE","%L %AE","%L %AE",
  "%AL","%E %AL","%AL","%E %AL",
  "%N","%E %N","%L %N","%E%L %N"
};

static const char *flag_feature_fmt[][2][2]={ // [name_fmt][show_loc][show_elev]
  "%O %M","%O %M %E","%O %M%L","%O %M %E%L",
  "%O %M %AE","%O %M %AE","%O %M%L %AE","%O %M%L %AE",
  "%O %M %AL","%O %M %E %AL","%O %M %AL","%O %M %E %AL",
  "%O %M %N","%O %M %E %N","%O %M%L %N","%O %M %E%L %N"
};

static const char *html_feature_fmt[][2][2]={ // [name_fmt][show_loc][show_elev]
  "","<TD>%E","<TD>%L","<TD>%E<TD>%L",
  "<TD>%AE","<TD>%AE","<TD>%L<TD>%AE","<TD>%L<TD>%AE",
  "<TD>%AL","<TD>%E<TD>%AL","<TD>%AL","<TD>%E<TD>%AL",
  "<TD>%N","<TD>%E<TD>%N","<TD>%L<TD>%N","<TD>%E<TD>%L<TD>%N"
};

static const char *csv_feature_fmt[][2][2]={ // [name_fmt][show_loc][show_elev]
  "","%E","%L","%E,%L",
  "%AE","%AE","%L,%AE","%L,%AE",
  "%AL","%E,%AL","%AL","%E,%AL",
  "%N","%E,%N","%L,%N","%E,%L,%N"
};

static char custom_fmt[256];

void Feature_fmt::set(const Database& data, Format_style style) const
{
  int csv=style==FMS_CSV;
  Elev_intvl::format=elev_fmt[csv][elev_f];
  Elev_intvl::precise_format=precise_f&&!csv?precise_fmt[elev_f]:0;
  Elev_intvl::min_format=min_fmt[csv][elev_f];
  Elev_intvl::max_format=max_fmt[csv][elev_f];
  Elev_intvl::field_format=elev_field_fmt[csv][elev_f];
  loc_f.set(csv!=0);
  if (custom.IsEmpty()) {
    switch (style) {
    case FMS_NOFLAG:
      Feature::format=noflag_feature_fmt[name_f][show_loc][show_elev];
      break;
    case FMS_FLAG:
      Feature::format=flag_feature_fmt[name_f][show_loc][show_elev];
      break;
    case FMS_HTML:
      Feature::format=html_feature_fmt[name_f][show_loc][show_elev];
      break;
    case FMS_CSV:
      Feature::format=csv_feature_fmt[name_f][show_loc][show_elev];
      break;
    }
    Feature::name_format=name_f==ALT_LOC?" %s":"%s";
  }
  else {
    strcpy(custom_fmt,custom);
    Feature::format=custom_fmt;
    Feature::name_format="%s";
  }
  Feature::data=&data;

  // TODO: Something like the next line is needed for wide format
  //Feature::name_format=NL_f==NL_replace?replace_fmt[loc_fmt_index(NL_f)]:"%s";
}

void Feature_fmt::load_registry(const char *section, const Feature_fmt& defalt)
{
  CWinApp *app=AfxGetApp();
  elev_f=(Elev_fmt)app->GetProfileInt(section,"Elevation",defalt.elev_f);
  precise_f=app->GetProfileInt(section,"Precise",defalt.precise_f)!=0;
  loc_f.angle_f=(Angle_fmt)app->GetProfileInt(section,"Angle",defalt.loc_f.angle_f);
  loc_f.news_f=app->GetProfileInt(section,"NEWS",defalt.loc_f.news_f)!=0;
  loc_f.custom=app->GetProfileString(section,"Location custom",defalt.loc_f.custom);
  show_elev=app->GetProfileInt(section,"Show elev",defalt.show_elev)!=0;
  show_loc=app->GetProfileInt(section,"Show loc",defalt.show_loc)!=0;
  name_f=(Name_fmt)app->GetProfileInt(section,"Name",defalt.name_f);
  custom=app->GetProfileString(section,"Custom",defalt.custom);
}

void Feature_fmt::save_registry(const char *section) const
{
  CWinApp *app=AfxGetApp();
  app->WriteProfileInt(section,"Elevation",elev_f);
  app->WriteProfileInt(section,"Precise",precise_f);
  app->WriteProfileInt(section,"Angle",loc_f.angle_f);
  app->WriteProfileInt(section,"NEWS",loc_f.news_f);
  app->WriteProfileString(section,"Location custom",loc_f.custom);
  app->WriteProfileInt(section,"Show elev",show_elev);
  app->WriteProfileInt(section,"Show loc",show_loc);
  app->WriteProfileInt(section,"Name",name_f);
  app->WriteProfileString(section,"Custom",custom);
}

/////////////////////////////////////////////////////////////////////////////
// CFeatureFmt_dlg message handlers

BOOL CFeatureFmt_dlg::OnInitDialog() 
{
  m_show_elev=target.show_elev;
  m_elev_fmt=target.elev_f;
  m_precise=target.precise_f;
  m_name_fmt=target.name_f;
  m_show_loc=target.show_loc;
  m_custom=target.custom;

  CDialog::OnInitDialog();

  CWinApp *app=AfxGetApp();
  text_bmp.LoadBitmap(IDB_TEXT);
  html_bmp.LoadBitmap(IDB_HTML);
  csv_bmp.LoadBitmap(IDB_CSV);
  m_reset_text_butn.SetBitmap(text_bmp);
  m_reset_html_butn.SetBitmap(html_bmp);
  m_reset_csv_butn.SetBitmap(csv_bmp);

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}

void CFeatureFmt_dlg::OnLocation() 
{
  CLocationFmt_dlg dlg(this,temp_loc_fmt);
  dlg.DoModal();
}

void CFeatureFmt_dlg::set_target(Feature_fmt& tgt) const
{
  tgt.show_elev=m_show_elev!=FALSE;
  tgt.precise_f=m_precise!=FALSE;
  switch (m_elev_fmt) {
  case 0: tgt.elev_f=Feature_fmt::MIDPOINT; break;
  case 1: tgt.elev_f=Feature_fmt::RANGE; break;
  case 2: tgt.elev_f=Feature_fmt::EXTREMES; break;
  default:
    assert(0);
  }

  tgt.show_loc=m_show_loc!=FALSE;
  tgt.loc_f=temp_loc_fmt;
  tgt.custom=m_custom;

  switch (m_name_fmt) {
  case 0: tgt.name_f=Feature_fmt::NONE; break;
  case 1: tgt.name_f=Feature_fmt::ALT_ELEV; break;
  case 2: tgt.name_f=Feature_fmt::ALT_LOC; break;
  case 3: tgt.name_f=Feature_fmt::SEPARATE; break;
  default:
    assert(0);
  }
}

void CFeatureFmt_dlg::OnOK() 
{
  if (!UpdateData(TRUE)) return;

  const char *custm=m_custom,*fc1;
  char del;

  for (const char *fc=custm; *fc; ++fc) {
    if (*fc=='%') {
      switch (*++fc) {
      case 'A': case 'a':
	switch (*++fc) {
	case 'L': case 'l':
	case 'E': case 'e':
	  break;
	default:
	  MessageBox("An invalid character appears after \"%A\" in the custom format.\n"
		     "Valid format strings are %AE and %AL.",
		     "Winprom Feature Format Error",MB_OK|MB_ICONERROR);
	  m_custom_ctl.SetSel(fc-custm,fc-custm+1);
	  return;
	}
	break;
      case 'L': case 'l':
      case 'E': case 'e':
      case 'M': case 'm':
      case 'O': case 'o':
      case 'N': case 'n':
      case '%':
	break;
      case 'D': case 'd':
	for (++fc; *fc>='0' && *fc<='9'; ++fc);
	del=*fc;
	switch (del) { 
	case '(': del=')'; break;
	case '[': del=']'; break;
	case '{': del='}'; break;
	case '<': del='>'; break;
	}
	for (fc1=++fc; *fc1!=del; ++fc1)
	  if (!*fc1) {
	    MessageBox("Undelimited data field",
		       "Winprom Feature Format Error",
		       MB_OK|MB_ICONERROR);
	    m_custom_ctl.SetSel(fc-custm-1,-1);
	    return;
	  }
	memcpy(buf1,fc,fc1-fc);
	buf1[fc1-fc]='\0';
	if (!data.valid_field_name(buf1)) {
	  sprintf(buf2,"Field \"%s\" does not exist in current database.\n"
		       "Do you want to continue?",buf1);
	  if (MessageBox(buf2,"Winprom Feature Format Warning",
			 MB_YESNO|MB_DEFBUTTON2|MB_ICONWARNING)!=IDYES) {
	    m_custom_ctl.SetSel(fc-custm,fc1-custm);
	    return;
	  }
	}
	fc=fc1;
	break;
      default:
	MessageBox("An invalid character appears after \"%\" in the custom format.\n"
		   "Valid format strings are %E, %L, %N, %O, %A, and %%.",
		   "Winprom Feature Format Error",
		   MB_OK|MB_ICONERROR);
	m_custom_ctl.SetFocus();
	m_custom_ctl.SetSel(fc-custm,fc-custm+1);
	return;
      } // switch (*fc)
    } // if (*fc=='%')
  } // for (fc)

  set_target(target);

  EndDialog(IDOK);
}

void CFeatureFmt_dlg::OnReset() {}

void CFeatureFmt_dlg::reset_custom(Format_style style)
{
  m_custom_ctl.SetWindowText("");
  UpdateData(TRUE);
  Feature_fmt tmp_tgt;
  set_target(tmp_tgt);
  tmp_tgt.set(data,style);
  m_custom_ctl.SetWindowText(Feature::format);
}

void CFeatureFmt_dlg::OnResetText()
{
  reset_custom(FMS_NOFLAG);
}

void CFeatureFmt_dlg::OnResetHTML()
{
  reset_custom(FMS_HTML);
}

void CFeatureFmt_dlg::OnResetCSV()
{
  reset_custom(FMS_CSV);
}
