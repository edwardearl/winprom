// winprom.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxpriv.h"
#include "winprom.h"

#include "PromFrame.h"
#include "GraphFrame.h"
#include "ProfileFrame.h"
#include "ListFrame.h"
#include "TreeFrame.h"
#include "DataFrame.h"

#include "MapView.h"
#include "ProfileView.h"

#include "GenericListView.h"
#include "PeakListView.h"
#include "SaddleListView.h"
#include "RunoffListView.h"
#include "BasinSadlListView.h"
#include "PromSadlListView.h"
#include "CycleListView.h"
#include "PeakNbrListView.h"
#include "RunoffNbrListView.h"
#include "ParentListView.h"
#include "WalkListView.h"
#include "ErrorListView.h"
#include "SaddleWalkInvView.h"
#include "DataListView.h"
#include "LPRtreeView.h"

#include "ScriptCtl_dlg.h"
#include "AnalyzeCtl_dlg.h"
#include "BrowseFormat_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPromApp

BEGIN_MESSAGE_MAP(CPromApp, CWinApp)
  //{{AFX_MSG_MAP(CPromApp)
  ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
  ON_COMMAND(ID_OPEN_DLPR_MAP, OnOpenDLPRmap)
  ON_COMMAND(ID_OPEN_DLPR_TREE, OnOpenDLPRtree)
  ON_COMMAND(ID_OPEN_ELEV_MAP, OnOpenElevMap)
  ON_COMMAND(ID_FILE_NEW, OnFileNew)
  ON_COMMAND(ID_OP_RUN, OnOpRun)
  ON_COMMAND(ID_OPEN_DATABASE, OnOpenDatabase)
  ON_COMMAND(ID_HELP_QUICK, OnHelpQuick)
  ON_COMMAND(ID_VIEW_FACTORY, OnViewFactory)
  ON_COMMAND(ID_VIEW_AUTOSAVE, OnViewAutosave)
  ON_COMMAND(ID_VIEW_RELOAD, OnViewReload)
  ON_COMMAND(ID_VIEW_SAVE, OnViewSave)
  ON_UPDATE_COMMAND_UI(ID_VIEW_AUTOSAVE, OnUpdateViewAutosave)
  ON_COMMAND(ID_VIEW_BROWSER_URL, OnViewBrowserURL)
  ON_COMMAND(ID_VIEW_BROWSER_REUSE, OnViewBrowserReuse)
  ON_UPDATE_COMMAND_UI(ID_VIEW_BROWSER_REUSE, OnUpdateViewBrowserReuse)
  ON_COMMAND(ID_VIEW_BROWSER_RELOAD, OnViewBrowserReload)
  ON_COMMAND(ID_VIEW_BROWSER_VIEW, OnViewBrowserView)
  ON_UPDATE_COMMAND_UI(ID_VIEW_BROWSER_RELOAD, OnUpdateBrowserHaveLoc)
  ON_UPDATE_COMMAND_UI(ID_VIEW_BROWSER_VIEW, OnUpdateBrowserHaveLoc)
  //}}AFX_MSG_MAP
  ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
  ON_COMMAND(ID_HELP_INDEX, OnHelpIndex)
  ON_COMMAND(ID_HELP, OnHelp)
  ON_COMMAND(ID_CONTEXT_HELP, OnContextHelp)
  // Standard file based document commands
  //ON_COMMAND(ID_FILE_NEW, CPromApp::OnFileNew)
  //ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
  // Standard print setup command
  ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
  // Background task termination cleanup
  ON_THREAD_MESSAGE(IDT_BACKGD_TASK_DONE,OnBackgdTaskDone)
  ON_THREAD_MESSAGE(IDT_ANALYZE_LINE,OnAnalLine)
  ON_THREAD_MESSAGE(IDT_ANALYZE_WALKUP,OnAnalWalkup)
  ON_THREAD_MESSAGE(IDT_SCRIPT_CMD,OnScriptCmd)
END_MESSAGE_MAP()


void CPromApp::SetImageLists()
{
  // one-time setup of image lists used by all feature list & tree views.
  // must be called at program startup.

  // These must be added in the same order specified in enum FT_index.
  largeImageList.Create(LARGE_ICON_SIZE,LARGE_ICON_SIZE,ILC_MASK|ILC_COLOR8,FT_NUM_IMG,1);
  largeImageList.Add(LoadIcon(IDI_PEAK));
  largeImageList.Add(LoadIcon(IDI_SADDLE));
  largeImageList.Add(LoadIcon(IDI_RUNOFF));
  largeImageList.Add(LoadIcon(IDI_BASINSADL));
  largeImageList.Add(LoadIcon(IDI_BASIN));
  largeImageList.Add(LoadIcon(IDI_SBRO));
  largeImageList.Add(LoadIcon(IDR_LPR_TREE));
  largeImageList.Add(LoadIcon(IDI_OFFMAP));
  largeImageList.SetOverlayImage(7,1);
  mediumImageList.Create(MEDIUM_ICON_SIZE,MEDIUM_ICON_SIZE,ILC_MASK|ILC_COLOR8,FT_NUM_IMG,1);
  mediumImageList.Add(LoadIcon(IDI_PEAK));
  mediumImageList.Add(LoadIcon(IDI_SADDLE));
  mediumImageList.Add(LoadIcon(IDI_RUNOFF));
  mediumImageList.Add(LoadIcon(IDI_BASINSADL));
  mediumImageList.Add(LoadIcon(IDI_BASIN));
  mediumImageList.Add(LoadIcon(IDI_SBRO));
  smallImageList.Create(SMALL_ICON_SIZE,SMALL_ICON_SIZE,ILC_MASK|ILC_COLOR8,FT_NUM_IMG,1);
  smallImageList.Add(LoadIcon(IDI_PEAK));
  smallImageList.Add(LoadIcon(IDI_SADDLE));
  smallImageList.Add(LoadIcon(IDI_RUNOFF));
  smallImageList.Add(LoadIcon(IDI_BASINSADL));
  smallImageList.Add(LoadIcon(IDI_BASIN));
  smallImageList.Add(LoadIcon(IDI_SBRO));
  smallImageList.Add(LoadIcon(IDR_LPR_TREE));
  smallImageList.Add(LoadIcon(IDI_MIXED));
  smallImageList.Add(LoadIcon(IDI_MIXED_DRAIN));
  smallImageList.Add(LoadIcon(IDI_OFFMAP));
  smallImageList.SetOverlayImage(9,1);
  flagImageList.Create(16,16,ILC_MASK|ILC_COLOR8,4,1);
  flagImageList.Add(LoadIcon(IDI_NOEDIT));
  flagImageList.Add(LoadIcon(IDI_EDGE));
  flagImageList.Add(LoadIcon(IDI_NOEDIT_EDGE));
  flagImageList.Add(LoadIcon(IDI_NOEDIT)); // needed because tree control can't seem to find image 0
}


/////////////////////////////////////////////////////////////////////////////
// CPromApp construction

CPromApp::CPromApp()
  : MapViewTpl(IDR_GRAPHTYPE,RUNTIME_CLASS(CPromDoc),
	RUNTIME_CLASS(CGraphFrame),RUNTIME_CLASS(CMapView)),
    ProfileViewTpl(IDR_GRAPHTYPE,RUNTIME_CLASS(CPromDoc),
	RUNTIME_CLASS(CProfileFrame),RUNTIME_CLASS(CProfileView)),
    ListViewTpl(IDR_FEATURE_LIST,RUNTIME_CLASS(CPromDoc),
	RUNTIME_CLASS(CListFrame),RUNTIME_CLASS(CFeatureListView)),
    GenericListViewTpl(IDR_FEATURE_LIST,RUNTIME_CLASS(CPromDoc),
	RUNTIME_CLASS(CListFrame),RUNTIME_CLASS(CGenericListView)),
    PeakListViewTpl(IDR_FEATURE_LIST,RUNTIME_CLASS(CPromDoc),
	RUNTIME_CLASS(CListFrame),RUNTIME_CLASS(CPeakListView)),
    SaddleListViewTpl(IDR_FEATURE_LIST,RUNTIME_CLASS(CPromDoc),
	RUNTIME_CLASS(CListFrame),RUNTIME_CLASS(CSaddleListView)),
    RunoffListViewTpl(IDR_FEATURE_LIST,RUNTIME_CLASS(CPromDoc),
	RUNTIME_CLASS(CListFrame),RUNTIME_CLASS(CRunoffListView)),
    BasinSadlListViewTpl(IDR_FEATURE_LIST,RUNTIME_CLASS(CPromDoc),
	RUNTIME_CLASS(CListFrame),RUNTIME_CLASS(CBasinSadlListView)),
    PromSadlListViewTpl(IDR_FEATURE_LIST,RUNTIME_CLASS(CPromDoc),
	RUNTIME_CLASS(CListFrame),RUNTIME_CLASS(CPromSadlListView)),
    CycleListViewTpl(IDR_FEATURE_LIST,RUNTIME_CLASS(CPromDoc),
	RUNTIME_CLASS(CListFrame),RUNTIME_CLASS(CCycleListView)),
    ParentListViewTpl(IDR_FEATURE_LIST,RUNTIME_CLASS(CPromDoc),
	RUNTIME_CLASS(CListFrame),RUNTIME_CLASS(CParentListView)),
    NbrListViewTpl(IDR_FEATURE_LIST,RUNTIME_CLASS(CPromDoc),
	RUNTIME_CLASS(CListFrame),RUNTIME_CLASS(CPeakNbrListView)),
    RunoffNbrListViewTpl(IDR_FEATURE_LIST,RUNTIME_CLASS(CPromDoc),
	RUNTIME_CLASS(CListFrame),RUNTIME_CLASS(CRunoffNbrListView)),
    ErrorListViewTpl(IDR_FEATURE_LIST,RUNTIME_CLASS(CPromDoc),
	RUNTIME_CLASS(CListFrame),RUNTIME_CLASS(CErrorListView)),
    WalkListViewTpl(IDR_FEATURE_LIST,RUNTIME_CLASS(CPromDoc),
	RUNTIME_CLASS(CListFrame),RUNTIME_CLASS(CWalkListView)),
    SadlWalkInvViewTpl(IDR_FEATURE_LIST,RUNTIME_CLASS(CPromDoc),
	RUNTIME_CLASS(CListFrame),RUNTIME_CLASS(CSaddleWalkInvView)),
    LPRtreeViewTpl(IDR_LPR_TREE,RUNTIME_CLASS(CPromDoc),
	RUNTIME_CLASS(CTreeFrame),RUNTIME_CLASS(CLPRtreeView)),
    DataListViewTpl(IDR_DATABASE,RUNTIME_CLASS(CPromDoc),
	RUNTIME_CLASS(CDataFrame),RUNTIME_CLASS(CDataListView))
{
  elev_map_clip=domap_clip=divide_tree_clip=0;
}


/////////////////////////////////////////////////////////////////////////////
// The one and only CPromApp object

CPromApp theApp;


/////////////////////////////////////////////////////////////////////////////
// CPromApp initialization

void CPromApp::load_initial_settings()
{
  auto_save=GetProfileInt("Setting","auto_save",true)!=0;
  CGraphView::load_initial_settings();
  CMapView::load_initial_settings();
  CProfileView::load_initial_settings();
  CLPRtreeView::load_initial_settings();
}

BOOL CPromApp::InitInstance()
{
  m_splash.Show();

  AfxEnableControlContainer();

  // Standard initialization
  // If you are not using these features and wish to reduce the size
  //  of your final executable, you should remove from the following
  //  the specific initialization routines you do not need.

  // Set up constant ASCII formats
  GridPoint::hdr_string="Location";
  Elev_intvl::range_format="%-3d";
  Elev_intvl::hdr_string="Elev";
  Elev_intvl::plus_infty_string="HP ";
  Elev_intvl::minus_infty_string="LP ";
  Elev_intvl::empty_string="";
  Feature::edit_hdr="E";
  Feature::edit_string=" ";
  Feature::noedit_string="*";
  Feature::edge_hdr="Ej";
  Feature::edge_string="* ";
  Feature::noedge_string="  ";
  Feature::name_hdr="Name";
  Basin_saddle::walk_risk_hdr="  CWR";
  Basin_saddle::WRO_hdr=" CWRO";
  Basin_saddle::cycle_format="%3u";
  Basin_saddle::cycle_hdr="  C";
  Basin_saddle::WS_string=" Safe";
  Domain::prom_hdr="Prom";
  Domain::usable_peak_hdr="UPE";
  Domain::usable_saddle_hdr="USE";

  browser=0;
  browse_tgt=0;
  browse_doc=0;
  reuse_browser=true;
  // set up browser format defaults
  browse_provider=0; // Acme
  browse_option=0; // topo
  browse_scale=1; // 4m/pixel
  old_scale=4;
  browse_size=0; // small
  old_size=0;

  // Change the registry key under which our settings are stored.
  // You should modify this string to be something appropriate
  // such as the name of your company or organization.
  SetRegistryKey(_T("Edward Earl"));

  load_initial_settings();
  LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

  SetImageLists();

  // create main MDI Frame window
  CPromFrame *pMainFrame = new CPromFrame;
  if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
    return FALSE;
  m_pMainWnd = pMainFrame;

  // Enable drag/drop open
  //m_pMainWnd->DragAcceptFiles();

  // Enable DDE Execute open
  //EnableShellOpen();
  //RegisterShellFileTypes(TRUE);

  #if 0 // I'll do my own parsing.  Leaving the AppWizard code here just in case, though.
  // Parse command line for standard shell commands, DDE, file open
  CCommandLineInfo cmdInfo;
  ParseCommandLine(cmdInfo);

  // Dispatch commands specified on the command line
  if (!ProcessShellCommand(cmdInfo))
    return FALSE;
  #endif

  GridPoint::clear_step();
  OnFileNew();
  if (m_lpCmdLine!=0 && m_lpCmdLine[0]!='\0') {
    char (&buf)[sizeof CPromDoc::buf]=CPromDoc::buf;
    char *suffix=strrchr(m_lpCmdLine,'.');
    if (suffix) {
      for (char *c=suffix; *c!='\0'; ++c)
	*c=tolower(*c);
      CPromDoc *document=GetActiveDocument();
      CFrameWnd *frame=pMainFrame->GetActiveFrame();
      CGraphView *view=GetActiveView();
      if (!strcmp(suffix,".elv")) {
	document->readElevMap(m_lpCmdLine);
	frame->InitialUpdateFrame(document,TRUE);
      }
      else if (!strncmp(suffix,".dom",3)) {
	document->readDomap(m_lpCmdLine);
	frame->InitialUpdateFrame(document,TRUE);
      }
      else if (!strncmp(suffix,".div",3)) {
	document->readDivideTree(m_lpCmdLine);
	frame->InitialUpdateFrame(document,TRUE);
      }
      else if (!strcmp(suffix,".wpd")) {
	document->readDatabase(m_lpCmdLine);
	frame->InitialUpdateFrame(document,TRUE);
      }
      else if (!strcmp(suffix,".wps")) {
	CString script_name=m_lpCmdLine;
	document->BeginWork();
	CScriptCtl_dlg dlg(*document,script_name);
	dlg.DoModal();
	document->EndWork();
	frame->InitialUpdateFrame(document,TRUE);
      }
      else {
	sprintf(buf,"File extension \"%s\" not recognized",suffix);
	MessageBox(NULL,buf,"Winprom New File Error",MB_OK|MB_ICONERROR);
      }
    }
    else {
      sprintf(buf,"File \"%s\" has no extension",m_lpCmdLine);
      MessageBox(NULL,buf,"Winprom New File Error",MB_OK|MB_ICONERROR);
    }
  }

#ifdef NDEBUG
  Sleep(1500);
#endif
  m_splash.Hide();

  // The main window has been initialized, so show and update it.
  pMainFrame->ShowWindow(m_nCmdShow);
  pMainFrame->UpdateWindow();

  return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// Helper functions to get active document/view on the main frame window

CPromDoc *CPromApp::GetActiveDocument() const
{
  return (CPromDoc *)((CMDIFrameWnd *)m_pMainWnd)->
    GetActiveFrame()->GetActiveDocument();
}

CGraphView *CPromApp::GetActiveView() const
{
  return (CGraphView *)((CMDIFrameWnd *)m_pMainWnd)->
    GetActiveFrame()->GetActiveView();
}


void CPromApp::viewFeature(const CPromDoc *document, const Feature& featr, FT_index type)
{
  if (((CMDIFrameWnd *)m_pMainWnd)->GetActiveFrame()==m_pMainWnd) {
    ::MessageBox(NULL,"There is currently no active graphical view.",
		 "Winprom View",MB_OK|MB_ICONERROR);
    return;
  }
  GetActiveView()->viewFeature(document,featr,type);
}

void CPromApp::viewLocation(const GridPoint& gp)
{
  if (((CMDIFrameWnd *)m_pMainWnd)->GetActiveFrame()==m_pMainWnd) {
    ::MessageBox(NULL,"There is currently no active graphical view.",
		 "Winprom View",MB_OK|MB_ICONERROR);
    return;
  }
  GetActiveView()->viewLocation(gp);
}

// determine if all elements of all workspaces are closed
void CPromApp::check_all_closed() const
{
  CDocTemplate *tpl;
  CPromDoc *doc;
  POSITION pt=GetFirstDocTemplatePosition(),pd;
  while (pt!=NULL) { // loop over doc templates
    tpl=GetNextDocTemplate(pt);
    pd=tpl->GetFirstDocPosition();
    while (pd!=NULL) { // loop over workspaces
      doc=(CPromDoc *)tpl->GetNextDoc(pd);
      // if anything is loaded, return
      if (doc->em_state!=NOT_LOADED || doc->dm_state!=NOT_LOADED ||
	  doc->dt_state!=NOT_LOADED || doc->db_state!=NOT_LOADED) return;
    }
  }
  // nothing is loaded into any workspace. Clear the steps.
  GridPoint::clear_step();
}


/////////////////////////////////////////////////////////////////////////////
// Browsing functions

void CPromApp::OnViewBrowserURL()
{
  if (CBrowseFormat_dlg(browse_loc).DoModal()==IDC_RELOAD) {
    if (browse_tgt) Navigate(browse_doc,*browse_tgt,browse_tgt_type);
    else Navigate(browse_loc);
  }
}

void CPromApp::set_browse_format(int provider, int option, int scale, int size)
{
  static const char
    acme_type[] = {'T','S','M','H','R','O','N','K'},
    *loj_type[] = {"mm","uq","ss","nn","hh","tt"},
    *tq_type[] = {"auto","24k","50k","100k","250k","sat1m"},
    *topozone_size[] = {"s","m","l"},
    *topozone_scale[] = {"12","24","25","50","63.360","100","200","250","500","1000"},
    *msn_style[] = {"WLD","USA","EUR","AP","BR"},
    *msn_size[] = {"405,320","575,470","800,740"},
    *mapquest_type[] = {"street","aerial","hybrid"},
    bing_type[] = {'r','a','h','o','b'},
    *google_type[] = {"","&t=p","&t=k","&t=h"},
    *gmap4_type[] = {"t3","t2","m","s","h","t1"},
    yahoo_type[] = {'m','s','h'},
    *fe_provider[] = {"nasa","ol","msa","msl","yh"},
    pb_search_type[] = {'R','E','P'},
    *pb_map_type[] = {"NA","MS","TR","BC","ON","NO","ES"},
    osm_type[] = {'M','O','C','T','Q'};
  static const int
    acme_zoom_limit[] = {16,20,21,21,15,18,21,18},
    loj_zoom_limit[] = {16,17,19,21,21,15},
    tq_zoom_limit[] = {4,4,8,16,32,1},
    gmap4_zoom_limit[] = {16,16,20,19,19,15},
    topozone_layer[] = {25,25,25,25,50,100,100,250,250,250},
    msn_world_alt[] = {170,250,500,1000,2500,5000,8000,11000,15000,20000,30000},
    msn_street_alt[] = {1,3,6,12,25,50,150,800,2000,7000,12000},
    maptech_scale[][7] = {{25000,80000,200000,736560,1126321,1200000,17716535},
			  {24000,100000,250000},
			  {250000,500000,1000000}},
    fe_zoom_limit[] = {9,9,18,18,20},
    osm_zoom_limit[] = {18,17,18,18,18};
  switch (provider) {
  case 0: // acme
    sprintf(CPromDoc::buf,
	    "http://mapper.acme.com/?ll=%%A,%%O&t=%c&z=%d",
	    acme_type[option],acme_zoom_limit[option]-scale);
    break;
  case 1: // ListsOfJohn
    sprintf(CPromDoc::buf,
	    "http://listsofjohn.com/PeakStats/%s.php?lat=%%A&lon=%%O&t=%c&z=%d",
	    size?"mapf":"QMap",
	    loj_type[option][size],loj_zoom_limit[option]-scale);
    break;
  case 2: // TopoQuest
    sprintf(CPromDoc::buf,
	    "http://www.topoquest.com/map.php?lat=%%A&lon=%%O&map=%s&zoom=%d&size=%s",
	    tq_type[option],tq_zoom_limit[option]<<scale,topozone_size[size]);
    break;
  case 3: // MyTopo
    sprintf(CPromDoc::buf,
	    "http://www.mytopo.com/maps.cfm?mtlat=%%A&mtlon=%%O&maptype=%s&z=%d",
	    option?"street":"Mytopo",(option?21:15)-scale);
    break;
  case 4: // Topozone
    sprintf(CPromDoc::buf,
	    "http://www.topozone.com/map.asp?lat=%%A&lon=%%O&s=%s&layer=DRG%d&size=%s",
	    topozone_scale[scale],topozone_layer[scale],topozone_size[size]);
    break;
  case 5: // ESRI
    sprintf(CPromDoc::buf,
	    "http://arcdata.esri.com/data_downloader/DataDownloader_SM?part=89173&mapScale=%d&cx=%%O&cy=%%A",
	    scale+1);
    break;
  case 6: // MSR maps
    sprintf(CPromDoc::buf,
	    "http://msrmaps.com/image.aspx?lat=%%A&lon=%%O&T=%d&S=%d&W=%d",
	    2-option,11+scale-option,size+1);
    break;
  case 7: // MSN Maps
    sprintf(CPromDoc::buf,
	    "http://maps.msn.com/(ecl1hk2kpwpwbbyidmkrkl55)/map.aspx?C=%%A,%%O&L=%s&A=%d&S=%s",
	    msn_style[option],(option?msn_street_alt:msn_world_alt)[scale],
	    msn_size[size]);
    break;
  case 8: // MapQuest
    sprintf(CPromDoc::buf,
	    "http://www.mapquest.com/?q=%%A,%%O&maptype=%s&zoom=%d",
	    mapquest_type[option],16-scale);
    break;
  case 9: // MapTech
    sprintf(CPromDoc::buf,
	    "http://mapserver.maptech.com/homepage/index.cfm?lat=%%A&lon=%%O&type=%d&scale=%d&zoom=100",
	    option,maptech_scale[option][scale]);
    break;
  case 10: // Offroute
    sprintf(CPromDoc::buf,
	    "http://offroute.com/geofinder/fs_geographic-finder.asp?lat=%%A&lon=%%O&zoom=%d",
	    10<<scale);
    break;
  case 11: // Bing
    sprintf(CPromDoc::buf,
	    "http://www.bing.com/maps/?cp=%%A~%%O&sty=%c&lvl=%d",
	    bing_type[option],(option?18:20)-scale);
    break;
  case 12: // Google
    sprintf(CPromDoc::buf,
	    "http://maps.google.com/maps?ll=%%A,%%O%s&z=%d",
	    google_type[option],(option==1?15:19)-scale);
    break;
  case 13: // GMap4
    sprintf(CPromDoc::buf,
	    "http://www.mappingsupport.com/p/gmap4.php?ll=%%A,%%O&t=%s&z=%d",
	    gmap4_type[option],gmap4_zoom_limit[option]-scale);
    break;
  case 14: // Yahoo!
    sprintf(CPromDoc::buf,
	    "http://maps.yahoo.com/index.php#mvt=%c&lat=%%A&lon=%%O&zoom=%d",
	    yahoo_type[option],(option?20:18)-scale);
    break;
  case 15: // USGS GNIS
    sprintf(CPromDoc::buf,
	    "http://geonames.usgs.gov/pls/gnis538/getgooglemap?p_lat=%%A&p_longi=%%O&fid=1000000");
    break;
  case 16: // Flashearth
    sprintf(CPromDoc::buf,
	    "http://www.flashearth.com/?lat=%%A&lon=%%O&src=%s&z=%d&r=0",
	    fe_provider[option],fe_zoom_limit[option]-scale);
    break;
  case 17: // Peakbagger
    if (option<3)
      sprintf(CPromDoc::buf,
	      "http://www.peakbagger.com/search.aspx?lat=%%A&lon=%%O&tid=%c",
	      pb_search_type[option]);
    else
      sprintf(CPromDoc::buf,
	      "http://www.peakbagger.com/map/BigMap.aspx?cy=%%A&cx=%%O&z=%d&l=%s&t=P&d=0",
	      21-scale,pb_map_type[option-3]);
    break;
  case 18: // Toolserver
    sprintf(CPromDoc::buf,
	    "http://toolserver.org/~geohack/geohack.php?params=%%A_N_%%O_E");
    break;
  case 19: // openstreetmap
    sprintf(CPromDoc::buf,
	    "http://www.openstreetmap.org/index.html?mlat=%%A&mlon=%%O&zoom=%d&layers=%c",
	    osm_zoom_limit[option]-scale,osm_type[option]);
    break;
  }
}

void CPromApp::Navigate_loc(const GridPoint& loc)
{
  browse_loc=loc;
  static char url_buf[256];
  GridPoint::real_format="%.4f";
  GridPoint::angle_format=AF_DEGREE;
  if (custm_format.IsEmpty()) {
    set_browse_format(browse_provider,browse_option,
		      browse_scale,browse_size);
    GridPoint::format=CPromDoc::buf;
  }
  else GridPoint::format=custm_format;
  loc.sprint(url_buf);

  if (browser) {
    long bHWND;
    if (!reuse_browser || browser->get_HWND(&bHWND)!=S_OK) {
      // user closed previous browser, or doesn't want to reuse it
      browser->Release();
      browser=0;
      CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, 
		       IID_IWebBrowser2, (void **)&browser);
    }
  }
  else if (SUCCEEDED(OleInitialize(NULL))) {
    CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, 
		     IID_IWebBrowser2, (void **)&browser);
  }

  if (browser) {
    VARIANT vEmpty;
    VariantInit(&vEmpty);
    BSTR bstrURL = CString(url_buf).AllocSysString();

    HRESULT hr = browser->Navigate(bstrURL, &vEmpty, &vEmpty, &vEmpty, &vEmpty);
    if (SUCCEEDED(hr)) {
      browser->put_Visible(VARIANT_TRUE);
    }
    else {
      browser->Quit();
      browser->Release();
      browser=0;
    }
    SysFreeString(bstrURL);
  }
  if (!browser) {
    MessageBox(NULL,"Sorry, unable to run navigate",
	       "Winprom Browse Error",MB_OK|MB_ICONERROR);
  }
}

void CPromApp::Navigate(const GridPoint& loc)
{
  browse_tgt=0;
  Navigate_loc(loc);
}

void CPromApp::Navigate(const CPromDoc *doc, const Feature& target, FT_index type)
{
  browse_tgt=&target;
  browse_tgt_type=type;
  browse_doc=doc;
  Navigate_loc(target.location);
}

void CPromApp::OnViewBrowserReload()
{
  if (browse_tgt) Navigate(browse_doc,*browse_tgt,browse_tgt_type);
  else Navigate(browse_loc);
}

void CPromApp::OnUpdateBrowserHaveLoc(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(browse_loc);
}

void CPromApp::OnViewBrowserReuse() 
{
  reuse_browser=!reuse_browser;
}

void CPromApp::OnUpdateViewBrowserReuse(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck(reuse_browser);
}

void CPromApp::OnViewBrowserView()
{
  if (browse_tgt) viewFeature(browse_doc,*browse_tgt,browse_tgt_type);
  else viewLocation(browse_loc);
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
  CAboutDlg();

// Dialog Data
  //{{AFX_DATA(CAboutDlg)
  enum { IDD = IDD_ABOUTBOX };
  //}}AFX_DATA

  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CAboutDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:
  //{{AFX_MSG(CAboutDlg)
    // No message handlers
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
  //{{AFX_DATA_INIT(CAboutDlg)
  //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAboutDlg)
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
  //{{AFX_MSG_MAP(CAboutDlg)
    // No message handlers
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CPromApp::OnAppAbout()
{
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CPromApp commands

void CPromApp::OnFileNew()
{
  CPromDoc *newdoc=(CPromDoc *)MapViewTpl.CreateNewDocument();
  newdoc->OnNewDocument();
  newdoc->OnWindowMap();
}

void CPromApp::OnOpenDLPRmap()
{
  CString name=CPromDoc::GetFileName(CPromDoc::FO_OPEN,NULL,"Domain map","domap");
  if (name.IsEmpty()) return;
  OnFileNew();
  GetActiveDocument()->readDomap(name);
}

void CPromApp::OnOpenDLPRtree()
{
  CString name=CPromDoc::GetFileName(CPromDoc::FO_OPEN,NULL,"Divide tree","divt");
  if (name.IsEmpty()) return;
  OnFileNew();
  GetActiveDocument()->readDivideTree(name);
}

void CPromApp::OnOpenElevMap()
{
  CString name=CPromDoc::GetFileName(CPromDoc::FO_OPEN,NULL,"Elevation map","elv");
  if (name.IsEmpty()) return;
  OnFileNew();
  GetActiveDocument()->readElevMap(name);
}

void CPromApp::OnOpenDatabase()
{
  CString name=CPromDoc::GetFileName(CPromDoc::FO_OPEN,NULL,"Database","wpd");
  if (name.IsEmpty()) return;
  OnFileNew();
  GetActiveDocument()->readDatabase(name);
}

void CPromApp::OnOpRun()
{
  CString name=CPromDoc::GetFileName(CPromDoc::FO_OPEN,NULL,"Script","wps");
  if (name.IsEmpty()) return;
  OnFileNew();
  CPromDoc *doc=GetActiveDocument();
  doc->BeginWork();
  CScriptCtl_dlg dlg(*doc,name);
  dlg.DoModal();
  doc->EndWork();
}

void CPromApp::OnHelpQuick()
{
  WinHelp(0x30068);
}

void CPromApp::OnBackgdTaskDone(WPARAM result, LPARAM)
{
  CBackgdTask_dlg::current_backgd_task->OnDone(result);
}

void CPromApp::OnAnalLine(WPARAM pci, LPARAM)
{
  CAnalyzeCtl_dlg::current_anal_ctl->update_line(pci);
}

void CPromApp::OnAnalWalkup(WPARAM out_count, LPARAM)
{
  CAnalyzeCtl_dlg::current_anal_ctl->update_walkup(out_count);
}

void CPromApp::OnScriptCmd(WPARAM, LPARAM text)
{
  CScriptCtl_dlg::current_script_ctl->OnScriptCmd((const char *)text);
}

void CPromApp::save_initial_settings()
{
  CGraphView::save_initial_settings();
  CMapView::save_initial_settings();
  CProfileView::save_initial_settings();
  CLPRtreeView::save_initial_settings();
}

int CPromApp::ExitInstance()
{
  WriteProfileInt("Setting","auto_save",auto_save);
  if (auto_save) save_initial_settings();

  if (browser) {
    if (!reuse_browser) browser->Quit();
    browser->Release();
  }
  OleUninitialize();

  return CWinApp::ExitInstance();
}

void CPromApp::OnViewFactory()
{
  auto_save=true;
  CGraphView::factory_initial_settings();
  CMapView::factory_initial_settings();
  CProfileView::factory_initial_settings();
  CLPRtreeView::factory_initial_settings();
}

void CPromApp::OnViewAutosave()
{
  auto_save=!auto_save;
}

void CPromApp::OnViewReload()
{
  load_initial_settings();
}

void CPromApp::OnViewSave()
{
  save_initial_settings();
}

void CPromApp::OnUpdateViewAutosave(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck(auto_save);
}


/////////////////////////////////////////////////////////////////////////////
// CViewColorDialog

IMPLEMENT_DYNAMIC(CViewColorDialog, CColorDialog)

CViewColorDialog::CViewColorDialog(COLORREF clrInit, const char *t) :
				   CColorDialog(clrInit),title(t)
{
}


BEGIN_MESSAGE_MAP(CViewColorDialog, CColorDialog)
  //{{AFX_MSG_MAP(CViewColorDialog)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CViewColorDialog::OnInitDialog()
{
  CColorDialog::OnInitDialog();

  SetWindowText(title);

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}
