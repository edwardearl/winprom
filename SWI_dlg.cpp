// SWI_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "SWI_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


bool SWI_filter::test(const Feature& f, const Database& data) const
{
  return !filter_both || sadl_filter.test(f,data);
}

bool SWI_filter::test(const Feature& f1, const Feature& f2,
		      const Elev_map& elev, const Database& data) const
{
  if (elev[f1.location]==elev[f2.location]) {
    if (!flat || &f1==&f2) return false;
  }
  else if (f1.elev && f2.elev) {
    if (!nondef) return false;
  }
  return (int)sadl_filter.test(f1,data)+(int)sadl_filter.test(f2,data)>
    (int)filter_both;
}


/////////////////////////////////////////////////////////////////////////////
// CSWI_dlg dialog

CSWI_dlg::CSWI_dlg(const CPromDoc& doc, SWI_filter& t, CWnd *pParent /*=NULL*/)
  : CDialog(CSWI_dlg::IDD, pParent),target(t),
    data(doc.data),drainage(doc.is_drainage)
{
  //{{AFX_DATA_INIT(CSWI_dlg)
  m_filter_both = -1;
  m_nondef = FALSE;
  m_flat = FALSE;
  m_basin = FALSE;
  //}}AFX_DATA_INIT
}

void CSWI_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CSWI_dlg)
  DDX_Control(pDX, IDC_BASIN, m_basin_ctl);
  DDX_Radio(pDX, IDC_ONE, m_filter_both);
  DDX_Check(pDX, IDC_NONDEF, m_nondef);
  DDX_Check(pDX, IDC_FLAT, m_flat);
  DDX_Check(pDX, IDC_BASIN, m_basin);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSWI_dlg, CDialog)
  //{{AFX_MSG_MAP(CSWI_dlg)
  ON_BN_CLICKED(IDC_FILTER, OnFilter)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSWI_dlg message handlers

void CSWI_dlg::OnFilter()
{
  CFeatureFilter_dlg dlg(temp_ff,this);
  dlg.DoModal();
}

void CSWI_dlg::OnOK()
{
  CDialog::OnOK();

  target.nondef=m_nondef!=FALSE;
  target.flat=m_flat!=FALSE;
  target.basin=m_basin!=FALSE;
  target.filter_both=m_filter_both!=0;
  target.sadl_filter=temp_ff;
}

BOOL CSWI_dlg::OnInitDialog() 
{
  m_nondef=target.nondef;
  m_flat=target.flat;
  m_basin=target.basin;
  m_filter_both=target.filter_both;
  temp_ff=target.sadl_filter;
  temp_ff.rec_filter.align(data);

  CDialog::OnInitDialog();

  m_basin_ctl.SetWindowText(drainage?"&Prominence saddles":"&Basin saddles");

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}
