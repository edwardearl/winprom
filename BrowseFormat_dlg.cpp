// BrowseFormat_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "BrowseFormat_dlg.h"

#pragma warning (disable : 4305)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CBrowseFormat_dlg dialog

CBrowseFormat_dlg::CBrowseFormat_dlg(bool r, CWnd* pParent /*=NULL*/)
  : CDialog(CBrowseFormat_dlg::IDD, pParent),reload(r)
{
  //{{AFX_DATA_INIT(CBrowseFormat_dlg)
  m_custom = _T("");
  m_size = -1;
  m_provider = -1;
  m_scale = -1;
  m_option = -1;
  //}}AFX_DATA_INIT
}

void CBrowseFormat_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CBrowseFormat_dlg)
  DDX_Control(pDX, IDC_RELOAD, m_reload_butn);
  DDX_Control(pDX, IDC_OPTION, m_option_ctl);
  DDX_Control(pDX, IDC_MAPSIZE, m_size_ctl);
  DDX_Control(pDX, IDC_SCALE, m_scale_ctl);
  DDX_Control(pDX, IDC_CUSTOM, m_custom_edit);
  DDX_Text(pDX, IDC_CUSTOM, m_custom);
  DDX_CBIndex(pDX, IDC_MAPSIZE, m_size);
  DDX_CBIndex(pDX, IDC_PROVIDER, m_provider);
  DDX_CBIndex(pDX, IDC_SCALE, m_scale);
  DDX_CBIndex(pDX, IDC_OPTION, m_option);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBrowseFormat_dlg, CDialog)
  //{{AFX_MSG_MAP(CBrowseFormat_dlg)
  ON_BN_CLICKED(IDC_RESET, OnReset)
  ON_BN_CLICKED(IDC_RELOAD, OnReload)
  ON_CBN_SELCHANGE(IDC_PROVIDER, OnChangeProvider)
  ON_CBN_SELCHANGE(IDC_OPTION, OnChangeOption)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBrowseFormat_dlg message handlers

static const char *topo="Topographic map",*photo="Overhead photo",
  *street="Street map",*hybrid="Hybrid",*terrain="Terrain";

static const Scale_info power2_scales[] = {
  // The zoom numbers below are for Google, Bing, FlashEarth,
  // and their derivatives. For Yahoo, add 1.
  {0.0625,"6cm/pixel"},	// 21
  {0.125,"12cm/pixel"},	// 20
  {0.25,"25cm/pixel"},	// 19
  {0.5,"50cm/pixel"},	// 18
  {1,"1m/pixel"},	// 17
  {2,"2m/pixel"},	// 16
  {4,"4m/pixel"},	// 15
  {8,"8m/pixel"},	// 14
  {16,"16m/pixel"},	// 13
  {32,"32m/pixel"},
  {64,"64m/pixel"},
  {128,"128m/pixel"},
  {256,"256m/pixel"},	// 9
  {512,"512m/pixel"},
  {1024,"1km/pixel"},
  {2048,"2km/pixel"},	// 6
  {4096,"4km/pixel"},
  {8192,"8km/pixel"},
  {16384,"16km/pixel"},	// 3
  {32768,"33km/pixel"},	// 2
  {65536,"66km/pixel"},	// 1
  {0,0}
};

static const Option_info acme_options[] = {
  {power2_scales+5,topo,0},	// 6 to 16
  {power2_scales+1,photo,0},	// 0 to 20
  {power2_scales,street,0},	// 0 to 21
  {power2_scales,hybrid,0},	// 0 to 21
  {power2_scales+6,terrain,0},	// 0 to 15
  {power2_scales+3,"DOQ",0},	// 4 to 18
  {power2_scales,"NEXRAD",0},	// 0 to 21
  {power2_scales+3,"Mapnik",0},	// 0 to 18
  {0,0,0}
};

static const Option_info LoJ_options[] = {
  {power2_scales+5,"MyTopo",2},	// 9 to 16
  {power2_scales+4,"USGS",2},	// 4 to 17
  {power2_scales+2,photo,2},	// 0 to 19
  {power2_scales,street,2},	// 0 to 21
  {power2_scales,hybrid,2},	// 0 to 21
  {power2_scales+6,terrain,2},	// 0 to 15
  {0,0,0}
};

static const Option_info topoquest_options[] = {
  {power2_scales+6,"Auto topo",3},	// 4 to 128
  {power2_scales+6,"USGS 24K topo",3},	// 4 to 128
  {power2_scales+7,"NRCAN 50K topo",3},	// 8 to 128
  {power2_scales+8,"USGS 100K topo",3},	// 16 to 512
  {power2_scales+9,"USGS 250K topo",3},	// 32 to 1km
  {power2_scales+4,photo,3},		// 1 to 128
  {0,0,0}
};

static const Option_info mytopo_options[] = {
  {power2_scales+6,topo,0},	// 9 to 15
  {power2_scales,street,0},	// 0 to 21
  {0,0,0}
};

// still feet
static const Scale_info topozone_scales[] = {
  {3,"1:10,000"},
  {8,"1:24,000"},
  {8,"1:25,000"},
  {17,"1:50,000"},
  {21,"1:63,360"},
  {33,"1:100,000"},
  {67,"1:200,000"},
  {83,"1:250,000"},
  {167,"1:500,000"},
  {333,"1:1,000,000"},
  {0,0}
};
static const Option_info topozone_options[] = {
  {topozone_scales,topo,3},
  {0,0,0}
};

// still feet
static const Scale_info esri_scales[] = {
  {24,"1:24,000"},
  {100,"1:100,000"},
  {250,"1:250,000"},
  {0,0}
};
static const Option_info esri_options[] = {
  {esri_scales,topo,0},
  {0,0,0}
};

static const Scale_info msr_scales[] = {
  {1,"1m/pixel"},	// 10
  {2,"2m/pixel"},	// 11
  {4,"4m/pixel"},	// 12
  {8,"8m/pixel"},
  {16,"16m/pixel"},
  {32,"32m/pixel"},
  {64,"64m/pixel"},
  {128,"128m/pixel"},
  {256,"256m/pixel"},
  {512,"512m/pixel"},	// 19
  {1024,"1km/pixel"},	// 20
  {2048,"2km/pixel"},	// 21
  {0,0}
};
static const Option_info msr_options[] = {
  {msr_scales+1,topo,3},	// 11 to 21
  {msr_scales,photo,3},		// 10 to 21
  {0,0,0}
};

// small  405,320
// medium 575,470
// large  800,740
static const Scale_info msn_world_scales[] = {
  {113.7,"40 miles"},
  {190.2,"65 miles"},
  {380,"135 miles"},
  {763,"270 miles"},
  {1901,"650 miles"},
  {3830,"1350 miles"},
  {6120,"2100 miles"},
  {8450,"3000 miles"},
  {11500,"4000 miles"},
  {15380,"5400 miles"},
  {23160,"8000 miles"},
  {0,0}
};
static const Scale_info msn_street_scales[] = {
  {0.756,"0.25 mile"},
  {2.283,"0.8 mile"},
  {4.57,"1.6 miles"},
  {9.13,"3.2 miles"},
  {19.01,"6.5 miles"},
  {38.3,"13.5 miles"},
  {114.9,"40 miles"},
  {615,"210 miles"},
  {1538,"540 miles"},
  {5370,"1900 miles"},
  {9260,"3200 miles"},
  {0,0}
};
static const Option_info msn_options[] = {
  {msn_world_scales,"World atlas",3},
  {msn_street_scales,"North America streets",3},
  {msn_street_scales,"Europe streets",3},
  {msn_street_scales,"Australia streets",3},
  {msn_street_scales,"Brazil streets",3},
  {0,0,0}
};

static const Scale_info mapquest_scales[] = {
  {0.263,"10in/pixel"},	// 16
  {0.395,"15in/pixel"},	// 15
  {0.658,"2ft/pixel"},	// 14
  {1.316,"4ft/pixel"},
  {2.632,"9ft/pixel"},
  {5.26,"17ft/pixel"},
  {10.53,"35ft/pixel"},
  {21.05,"69ft/pixel"},	// 9
  {42.1,"140ft/pixel"},
  {92.1,"300ft/pixel"},
  {184.2,"600ft/pixel"},
  {421,"0.26mi/pixel"},
  {934,"0.58mi/pixel"},
  {2763,"1.7mi/pixel"},
  {8290,"5mi/pixel"},	// 2
  {24210,"15mi/pixel"},	// 1
  {0,0}
};
static const Option_info mapquest_options[] = {
  {mapquest_scales,street,0},
  {mapquest_scales,photo,0},
  {mapquest_scales,hybrid,0},
  {0,0,0}
};

static const Scale_info maptech_nautical_scales[] = {
  {2.50,"1:25,000"},
  {8.00,"1:80,000"},
  {20.0,"1:200,000"},
  {73.7,"1:736,560"},
  {112.6,"1:1,126,321"},
  {120.0,"1:1,200,000"},
  {1772,"1:17,716,535"},
  {0,0}
};
static const Scale_info maptech_topo_scales[] = {
  {2.40,"1:24,000"},
  {10.0,"1:100,000"},
  {25.0,"1:250,000"},
  {0,0}
};
static const Scale_info maptech_aero_scales[] = {
  {25.0,"Class B"},
  {50.0,"Sectional"},
  {100.0,"WAC"},
  {0,0}
};
static const Option_info maptech_options[] = {
  {maptech_nautical_scales,"Nautical chart",0},
  {maptech_topo_scales,topo,0},
  {maptech_aero_scales,"Aeronautical chart",0},
  {0,0,0}
};

static const Scale_info offroute_scales[] = {
  {50,"10 miles"},
  {100,"20 miles"},
  {200,"40 miles"},
  {400,"80 miles"},
  {800,"160 miles"},
  {1600,"320 miles"},
  {3200,"640 miles"},
  {6400,"1280 miles"},
  {0,0}
};
static const Option_info offroute_options[] = {
  {offroute_scales,topo,0},
  {0,0,0}
};

static const Option_info bing_options[] = {
  {power2_scales+1,street,0},		// 1 to 20
  {power2_scales+3,photo,0},		// 1 to 18
  {power2_scales+3,hybrid,0},		// 1 to 18
  {power2_scales+3,"3D",0},		// 1 to 18
  {power2_scales+3,"3D hybrid",0},	// 1 to 18
  {0,0,0}
};

static const Option_info google_options[] = {
  {power2_scales+2,street,0},	// 1 to 19
  {power2_scales+6,terrain,0},	// 1 to 15
  {power2_scales+2,photo,0},	// 1 to 19
  {power2_scales+2,hybrid,0},	// 1 to 19
  {0,0,0}
};

static const Option_info gmap4_options[] = {
  {power2_scales+5,"USGS topo",0},	// 1 to 16
  {power2_scales+5,"MyTopo",0},		// 0 to 16
  {power2_scales+1,street,0},		// 0 to 20
  {power2_scales+2,photo,0},		// 0 to 19
  {power2_scales+2,hybrid,0},		// 0 to 19
  {power2_scales+6,terrain,0},		// 0 to 15
  {0,0,0}
};

static const Option_info yahoo_options[] = {
  {power2_scales+4,street,0},	// 2 to 18
  {power2_scales+2,photo,0},	// 2 to 20
  {power2_scales+2,hybrid,0},	// 2 to 20
  {0,0,0}
};

static const Scale_info usgs_gnis_scale[] = {
  {128,"128m/pixel"},
  {0,0}
};
static const Option_info usgs_gnis_options[] = {
  {usgs_gnis_scale,street,0},
  {0,0,0}
};

static const Option_info fe_options[] = {
  {power2_scales+12,"NASA Terra",0},		// 1 to 9
  {power2_scales+12,"OpenLayers",0},		// 1 to 9 TODO: try again
  {power2_scales+3,"Virtualearth photo",0},	// 1 to 18
  {power2_scales+3,"Virtualearth labels",0},	// 1 to 18
  {power2_scales+1,"Yahoo!",0},			// 1 to 20
  {0,0,0}
};

static const Option_info pb_options[] = {
  {0,"Radius search",0},
  {0,"Elevation ladder",0},
  {0,"Prominence ladder",0},
  {power2_scales+5,"MyTopo",0},
  {power2_scales+5,"MSR Topo",0},
  {power2_scales+5,"Terrain",0},
  {power2_scales+5,"BC Basemap",0},
  {power2_scales+4,"Ontario",0},
  {power2_scales+5,"Norway",0},
  {power2_scales+4,"Spain",0},
  {0,0,0}
};

static const Option_info toolserver_options[] = {
  {0,"Home",0},
  {0,0,0}
};

static const Option_info openstreetmap_options[] = {
  {power2_scales+3,"Mapnik",0},		// 0 to 18
  {power2_scales+4,"OSMArender",0},	// 0 to 17
  {power2_scales+3,"Cycle",0},		// 0 to 18
  {power2_scales+3,"Transport",0},	// 0 to 18
  {power2_scales+3,"MapQuest",0},	// 0 to 18
  {0,0,0}
};

static Provider_info providers[] = {
  {acme_options},
  {LoJ_options},
  {topoquest_options},
  {mytopo_options},
  {topozone_options},
  {esri_options},
  {msr_options},
  {msn_options},
  {mapquest_options},
  {maptech_options},
  {offroute_options},
  {bing_options},
  {google_options},
  {gmap4_options},
  {yahoo_options},
  {usgs_gnis_options},
  {fe_options},
  {pb_options},
  {toolserver_options},
  {openstreetmap_options}
};


void CBrowseFormat_dlg::OnReset()
{
  UpdateData();
  CPromApp::set_browse_format(m_provider,m_option,m_scale,m_size);
  m_custom_edit.SetWindowText(CPromDoc::buf);
}


void CBrowseFormat_dlg::set_option_list()
{
  const Provider_info& pi=providers[m_provider];
  while (m_option_ctl.GetCount()>0) m_option_ctl.DeleteString(0);
  for (unsigned i=0; pi.options[i].text; ++i)
    m_option_ctl.AddString(pi.options[i].text);
  m_option_ctl.SetCurSel(m_option);

  set_scale_size_list();
}

void CBrowseFormat_dlg::set_scale_size_list()
{
  const Option_info& opt=providers[m_provider].options[m_option];

  while (m_scale_ctl.GetCount()>0) m_scale_ctl.DeleteString(0);
  if (opt.scales) {
    for (int i=0; opt.scales[i].text; ++i)
      m_scale_ctl.AddString(opt.scales[i].text);
    m_scale_ctl.SetCurSel(m_scale);
  }
  m_scale_ctl.EnableWindow(opt.scales!=0);

  while (m_size_ctl.GetCount()>0) m_size_ctl.DeleteString(0);
  if (opt.nsize>=2) m_size_ctl.AddString("Small");
  if (opt.nsize>=3) m_size_ctl.AddString("Medium");
  if (opt.nsize>=2) m_size_ctl.AddString("Large");
  if (opt.nsize>0) m_size_ctl.SetCurSel(m_size);
  m_size_ctl.EnableWindow(opt.nsize>0);
}

BOOL CBrowseFormat_dlg::OnInitDialog()
{
  CPromApp *app=CPromApp::getApp();
  m_provider=app->browse_provider;
  m_option=app->browse_option;
  m_size=app->browse_size;
  old_size=app->old_size;
  m_scale=app->browse_scale;
  old_scale=app->old_scale;
  m_custom=app->custm_format;

  CDialog::OnInitDialog();

  if (!reload) m_reload_butn.EnableWindow(FALSE);
  set_option_list();

  return TRUE;
}

void CBrowseFormat_dlg::update_close(UINT rID)
{
  if (!UpdateData()) return;

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

  CPromApp *app=CPromApp::getApp();
  app->browse_provider=m_provider;
  app->browse_option=m_option;
  app->browse_size=m_size;
  app->old_size=old_size;
  app->browse_scale=m_scale;
  app->old_scale=old_scale;
  app->custm_format=m_custom;

  CDialog::EndDialog(rID);
}

void CBrowseFormat_dlg::OnOK()
{
  update_close(IDOK);
}

void CBrowseFormat_dlg::OnReload()
{
  update_close(IDC_RELOAD);
}

void CBrowseFormat_dlg::set_best_size(int nsize)
{
  if (nsize==3) m_size=old_size;
  else if (nsize==2) m_size=old_size/2;
  else assert(nsize==0);
}

void CBrowseFormat_dlg::set_best_scale(const Scale_info scales[])
{
  if (scales) {
    float scale_ratio,best_ratio=0;
    for (int i=0; scales[i].scale>0; ++i) {
      scale_ratio=scales[i].scale/old_scale;
      if (scale_ratio>1) scale_ratio=1/scale_ratio;
      if (scale_ratio>best_ratio) {
	best_ratio=scale_ratio;
	m_scale=i;
      }
    }
  }
}

void CBrowseFormat_dlg::update_option(const Option_info& old_opt,
				      const Option_info& new_opt)
{
  // change the scale to reflect new option
  if (old_opt.scales) old_scale=old_opt.scales[m_scale].scale;
  set_best_scale(new_opt.scales);

  // change the size to reflect new option
  if (old_opt.nsize==3) old_size=m_size;
  else if (old_opt.nsize==2) old_size=m_size*2;
  set_best_size(new_opt.nsize);
}

void CBrowseFormat_dlg::OnChangeOption()
{
  int old_option=m_option;
  UpdateData();
  const Provider_info &prov=providers[m_provider];
  update_option(prov.options[old_option],prov.options[m_option]);
  set_scale_size_list();
}

void CBrowseFormat_dlg::OnChangeProvider()
{
  int old_provider=m_provider,new_option;
  UpdateData();
  assert(m_provider<sizeof providers/sizeof(Provider_info));
  Provider_info &new_prov=providers[m_provider],
    &old_prov=providers[old_provider];
  new_option=new_prov.last_option;
  old_prov.last_option=m_option;
  m_option_ctl.SetCurSel(new_option);
  update_option(old_prov.options[m_option],new_prov.options[new_option]);
  m_option=new_option;
  set_option_list();
}
