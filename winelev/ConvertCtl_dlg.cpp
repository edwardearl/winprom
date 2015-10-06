// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// ConvertCtl_dlg.cpp : implementation file
//

#include "stdafx.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "winelev.h"
#include "ConvertCtl_dlg.h"
#include "Elev_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CConvertCtl_dlg dialog

CConvertCtl_dlg::CConvertCtl_dlg(CElev_dlg& ed)
  : CBackgdTask_dlg(CConvertCtl_dlg::IDD),elev_win(ed)
{
  //{{AFX_DATA_INIT(CConvertCtl_dlg)
  m_fromfile = _T("");
  m_stage = _T("");
  //}}AFX_DATA_INIT
}


void CConvertCtl_dlg::DoDataExchange(CDataExchange* pDX)
{
  CBackgdTask_dlg::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CConvertCtl_dlg)
  DDX_Control(pDX, IDC_PROGRESS, m_progress);
  DDX_Text(pDX, IDC_FROMFILE, m_fromfile);
  DDX_Text(pDX, IDC_STAGE, m_stage);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConvertCtl_dlg, CBackgdTask_dlg)
  //{{AFX_MSG_MAP(CConvertCtl_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CConvertCtl_dlg message handlers

BOOL CConvertCtl_dlg::OnInitDialog()
{
  CBackgdTask_dlg::OnInitDialog();

  int total_steps=0;
  FromType from_type;
  CString path;

  POSITION p=elev_win.files_dlg->GetStartPosition();
  while (p!=NULL) {
    path=elev_win.files_dlg->GetNextPathName(p);
    from_type=elev_win.get_from_type(path);
    if (from_type!=FROM_ELEV) ++total_steps; // external to native conversion
    if (elev_win.m_to_divtree || elev_win.m_to_domap) {
      // analysis
      struct _stat stat;
      if (from_type==FROM_GTOPO30) {
	if (_stat(path,&stat)==0) {
	  if (stat.st_size==58320000L) total_steps+=36;
	  else if (stat.st_size==57600000L) total_steps+=80;
	  else if (stat.st_size==51840000L) total_steps+=72;
	}
      }
      else if (from_type==FROM_GLOBE) {
	if (_stat(path,&stat)==0) {
	  if (stat.st_size==129600000L) total_steps+=180;
	  else if (stat.st_size==103680000L) total_steps+=144;
	}
      }
      else if (from_type==FROM_ETOPO2) {
	total_steps+=72;
      }
      else ++total_steps;
    }
  }

  m_progress.SetRange(0,total_steps);
  m_progress.SetStep(1);

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}

int CConvertCtl_dlg::proc() throw()
{
  return elev_win.convert_files();
}
