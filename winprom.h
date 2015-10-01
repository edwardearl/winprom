// winprom.h : main header file for the WINPROM application
//

#if !defined(AFX_WINPROM_H__9927AF46_C393_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_WINPROM_H__9927AF46_C393_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
  #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "PromDoc.h"
#include "Splash.h"

#define LARGE_ICON_SIZE 32
#define MEDIUM_ICON_SIZE 24
#define SMALL_ICON_SIZE 16
#define LARGE_ICON_Y_OFFSET 19
#define MEDIUM_ICON_Y_OFFSET 14
#define SMALL_ICON_Y_OFFSET 9

class CGraphView;


/////////////////////////////////////////////////////////////////////////////
// CPromApp:
// See winprom.cpp for the implementation of this class
//

class CPromApp : public CWinApp
{
  void load_initial_settings();
  void save_initial_settings();
  bool auto_save;
  CSplashWnd m_splash;
  void Navigate_loc(const GridPoint&);

public:
  CPromApp();

  static CPromApp *getApp() {return (CPromApp *)AfxGetApp();}

  CSingleDocTemplate ListViewTpl,GenericListViewTpl,
    PeakListViewTpl,SaddleListViewTpl,
    RunoffListViewTpl,BasinSadlListViewTpl,
    PromSadlListViewTpl,CycleListViewTpl,ParentListViewTpl,
    NbrListViewTpl,RunoffNbrListViewTpl,
    ErrorListViewTpl,WalkListViewTpl,SadlWalkInvViewTpl,
    LPRtreeViewTpl,DataListViewTpl;
  CMultiDocTemplate MapViewTpl,ProfileViewTpl;

  CPromDoc *elev_map_clip,*domap_clip,*divide_tree_clip,*database_clip;

  CImageList largeImageList,mediumImageList,smallImageList,flagImageList;

  CPromDoc *GetActiveDocument() const;
  CGraphView *GetActiveView() const;
  void SetImageLists();

  void viewFeature(const CPromDoc *, const Feature&, FT_index);
  void viewLocation(const GridPoint&);
  static void set_browse_format(int, int, int, int);
  void Navigate(const GridPoint&);
  void Navigate(const CPromDoc *, const Feature&, FT_index);
  void check_all_closed() const;

  IWebBrowser2 *browser;
  const Feature *browse_tgt;
  FT_index browse_tgt_type;
  const CPromDoc *browse_doc;
  GridPoint browse_loc;
  bool reuse_browser;
  int browse_provider,browse_option,browse_scale,browse_size,old_size;
  float old_scale;
  CString custm_format;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CPromApp)
  public:
  virtual BOOL InitInstance();
  virtual int ExitInstance();
  //}}AFX_VIRTUAL

// Implementation

  //{{AFX_MSG(CPromApp)
  afx_msg void OnAppAbout();
  afx_msg void OnOpenDLPRmap();
  afx_msg void OnOpenDLPRtree();
  afx_msg void OnOpenElevMap();
  afx_msg void OnFileNew();
  afx_msg void OnOpRun();
  afx_msg void OnOpenDatabase();
  afx_msg void OnHelpQuick();
  afx_msg void OnViewFactory();
  afx_msg void OnViewAutosave();
  afx_msg void OnViewReload();
  afx_msg void OnViewSave();
  afx_msg void OnUpdateViewAutosave(CCmdUI* pCmdUI);
  afx_msg void OnViewBrowserURL();
  afx_msg void OnViewBrowserReuse();
  afx_msg void OnUpdateViewBrowserReuse(CCmdUI* pCmdUI);
  afx_msg void OnViewBrowserReload();
  afx_msg void OnViewBrowserView();
  afx_msg void OnUpdateBrowserHaveLoc(CCmdUI* pCmdUI);
  //}}AFX_MSG
  afx_msg void OnBackgdTaskDone(WPARAM, LPARAM);
  afx_msg void OnAnalLine(WPARAM, LPARAM);
  afx_msg void OnAnalWalkup(WPARAM, LPARAM);
  afx_msg void OnScriptCmd(WPARAM, LPARAM);
  DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CViewColorDialog dialog

class CViewColorDialog : public CColorDialog
{
  DECLARE_DYNAMIC(CViewColorDialog)
  const char *title;

public:
  CViewColorDialog(COLORREF clrInit, const char *title);

protected:
  //{{AFX_MSG(CViewColorDialog)
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINPROM_H__9927AF46_C393_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
