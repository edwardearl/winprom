// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// DataAlign_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "DataAlign_dlg.h"
#include "Assoc_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDataAlign_dlg dialog

CDataAlign_dlg::CDataAlign_dlg(bool e, bool d, const Database& db, CWnd* pParent /*=NULL*/)
  : CDialog(CDataAlign_dlg::IDD, pParent),
    have_elev(e),have_dom(d),data(db),m_assoc(0,false)
{
  //{{AFX_DATA_INIT(CDataAlign_dlg)
  m_orphan = FALSE;
  m_mult_rec_peak = -1;
  m_mult_rec_sadl = -1;
  m_distance = FALSE;
  m_pritype = -1;
  m_excise = FALSE;
  m_prom = FALSE;
  //}}AFX_DATA_INIT
}

void CDataAlign_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDataAlign_dlg)
  DDX_Control(pDX, IDC_MULT_REC_SADL, m_mult_rec_sadl_list);
  DDX_Control(pDX, IDC_MULT_REC_PEAK, m_mult_rec_peak_list);
  DDX_Check(pDX, IDC_ORPHAN, m_orphan);
  DDX_CBIndex(pDX, IDC_MULT_REC_PEAK, m_mult_rec_peak);
  DDX_CBIndex(pDX, IDC_MULT_REC_SADL, m_mult_rec_sadl);
  DDX_Check(pDX, IDC_DISTANCE, m_distance);
  DDX_Radio(pDX, IDC_PSR, m_pritype);
  DDX_Check(pDX, IDC_EXCISE, m_excise);
  DDX_Check(pDX, IDC_PROM, m_prom);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataAlign_dlg, CDialog)
  //{{AFX_MSG_MAP(CDataAlign_dlg)
  ON_BN_CLICKED(IDC_ASSOC, OnAssoc)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDataAlign_dlg message handlers

void CDataAlign_dlg::OnAssoc() 
{
  CAssoc_dlg dlg(m_assoc,have_elev,have_dom,false);
  dlg.DoModal();
}

BOOL CDataAlign_dlg::OnInitDialog()
{
  m_mult_rec_peak=m_mult_rec_sadl=0;

  CDialog::OnInitDialog();

  CString mrc;
  int n;
  for (Database::const_FieldID fi=data.begin_field(); fi!=data.end_field(); ++fi) {
    if (data.field_type(fi)==FLD_INTEGER || data.field_type(fi)==FLD_REAL) {
      mrc.Format("Min %s",(*fi).name);
      n=m_mult_rec_peak_list.AddString(mrc);
      m_mult_rec_peak_list.SetItemDataPtr(n, (void *) (fi - data.begin_field()));
      n=m_mult_rec_sadl_list.AddString(mrc);
      m_mult_rec_sadl_list.SetItemDataPtr(n, (void *) (fi - data.begin_field()));
      mrc.Format("Max %s",(*fi).name);
      n=m_mult_rec_peak_list.AddString(mrc);
      m_mult_rec_peak_list.SetItemDataPtr(n, (void *) (fi - data.begin_field()));
      n=m_mult_rec_sadl_list.AddString(mrc);
      m_mult_rec_sadl_list.SetItemDataPtr(n, (void *) (fi - data.begin_field()));
    }
  }

  return TRUE;	// return TRUE unless you set the focus to a control
}

void CDataAlign_dlg::OnOK()
{
  CDialog::OnOK();
  mrf_peak= data.begin_field() + ((int) m_mult_rec_peak_list.GetItemDataPtr(m_mult_rec_peak));
  mrf_sadl= data.begin_field() + ((int) m_mult_rec_sadl_list.GetItemDataPtr(m_mult_rec_sadl));
}
