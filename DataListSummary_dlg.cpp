// DataListSummary_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "DataListSummary_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static char (&buf)[sizeof CDataListView::buf]=CDataListView::buf;


/////////////////////////////////////////////////////////////////////////////
// CDataListSummary_dlg dialog

CDataListSummary_dlg::CDataListSummary_dlg(CDataListView *parent)
  : CDialog(CDataListSummary_dlg::IDD,parent)
{
  //{{AFX_DATA_INIT(CDataListSummary_dlg)
  m_area_e = _T("");
  m_area_n = _T("");
  m_area_s = _T("");
  m_area_w = _T("");
  m_count = 0;
  //}}AFX_DATA_INIT
  CListCtrl& listCtrl=parent->GetListCtrl();
  m_count=listCtrl.GetItemCount();
  const Database& data=parent->GetDocument()->data;
  for (int item=0; item<m_count; ++item) {
    area|=data.get_location(listCtrl.GetItemData(item));
  }
  summary_fmt=parent->list_fmt;
}

void CDataListSummary_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDataListSummary_dlg)
  DDX_Text(pDX, IDC_AREA_E, m_area_e);
  DDX_Text(pDX, IDC_AREA_N, m_area_n);
  DDX_Text(pDX, IDC_AREA_S, m_area_s);
  DDX_Text(pDX, IDC_AREA_W, m_area_w);
  DDX_Text(pDX, IDC_COUNT, m_count);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataListSummary_dlg, CDialog)
  //{{AFX_MSG_MAP(CDataListSummary_dlg)
  ON_BN_CLICKED(IDC_FORMAT, OnFormat)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDataListSummary_dlg message handlers

void CDataListSummary_dlg::reformat()
{
  ((GridPoint)area.corner2()).sprint1(buf);
  m_area_n=buf;
  ((GridPoint)area.corner1()).sprint1(buf);
  m_area_s=buf;
  ((GridPoint)area.corner2()).sprint2(buf);
  m_area_e=buf;
  ((GridPoint)area.corner1()).sprint2(buf);
  m_area_w=buf;
}

BOOL CDataListSummary_dlg::OnInitDialog() 
{
  summary_fmt.set();
  reformat();

  CDialog::OnInitDialog();

  return TRUE;
}

void CDataListSummary_dlg::OnFormat()
{
  CLocationFmt_dlg dlg(this,summary_fmt);
  if (dlg.DoModal()==IDOK) {
    summary_fmt.set();
    reformat();
    UpdateData(FALSE);
  }
}
