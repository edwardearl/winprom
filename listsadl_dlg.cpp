// ListBS_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "ListSadl_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void SadlListFilter::get_prom(const Domain::Sadl_prom& sp,
			      Elevation& s_prom, Elevation& s_omp) const
{
  s_prom=sp.*sadl_prom_types[0][cell];
  if (offmap) s_omp=sp.*sadl_prom_types[1][cell];
  if (ridge) {
    if (sp.rr_onmap>s_prom)
      s_prom=sp.rr_onmap;
    if (offmap && sp.rr_offmap)
      s_omp=elev_infty;
  }
}


/////////////////////////////////////////////////////////////////////////////
// CListSadl_dlg dialog

void CListSadl_dlg::data_init()
{
  //{{AFX_DATA_INIT(CListSadl_dlg)
  m_cell = FALSE;
  m_ridge = FALSE;
  //}}AFX_DATA_INIT
}

CListSadl_dlg::CListSadl_dlg(const Database& db, SadlListFilter& lf,
			     UINT icon_id, CWnd* pParent /*=NULL*/)
  : CListFilter_dlg(CListSadl_dlg::IDD,db,lf,icon_id,pParent),filter(lf)
{
  data_init();
}

CListSadl_dlg::CListSadl_dlg(UINT nID, const Database& db, SadlListFilter& lf,
			     UINT icon_id, CWnd *pParent)
  : CListFilter_dlg(nID,db,lf,icon_id,pParent),filter(lf)
{
  data_init();
}

void CListSadl_dlg::DoDataExchange(CDataExchange* pDX)
{
  CListFilter_dlg::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CListSadl_dlg)
  DDX_Check(pDX, IDC_CELL, m_cell);
  DDX_Check(pDX, IDC_RIDGE, m_ridge);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListSadl_dlg, CListFilter_dlg)
  //{{AFX_MSG_MAP(CListSadl_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CListSadl_dlg message handlers

BOOL CListSadl_dlg::OnInitDialog()
{
  m_ridge=filter.ridge;
  m_cell=filter.cell;

  CListFilter_dlg::OnInitDialog();

  return TRUE;
}

void CListSadl_dlg::OnOK() 
{
  CListFilter_dlg::OnOK();

  filter.ridge=m_ridge!=FALSE;
  filter.cell=m_cell!=FALSE;
}
