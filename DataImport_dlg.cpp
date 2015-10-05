// DataImport_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "DataImport_dlg.h"
#include "FeatureTypeFilter_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDataImport_dlg dialog

CDataImport_dlg::CDataImport_dlg(const Database& db, CWnd* pParent /*=NULL*/)
  : CDialog(CDataImport_dlg::IDD, pParent),data(db)
{
  //{{AFX_DATA_INIT(CDataImport_dlg)
  m_edit = TRUE;
  m_bs_status = -1;
  m_ps_status = -1;
  //}}AFX_DATA_INIT
}

void CDataImport_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDataImport_dlg)
  DDX_Control(pDX, IDC_NAME_FLD, m_name_cb);
  DDX_Control(pDX, IDC_ELEV_FLD, m_elev_cb);
  DDX_Check(pDX, IDC_EDIT, m_edit);
  DDX_CBIndex(pDX, IDC_BS_STATUS, m_bs_status);
  DDX_CBIndex(pDX, IDC_PS_STATUS, m_ps_status);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDataImport_dlg, CDialog)
  //{{AFX_MSG_MAP(CDataImport_dlg)
  ON_BN_CLICKED(IDC_FILTER, OnFilter)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDataImport_dlg message handlers

void CDataImport_dlg::OnFilter()
{
  CFeatureTypeFilter_dlg dlg(m_filter,0);
  dlg.DoModal();
}

BOOL CDataImport_dlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_elev_cb.AddString("<none>");
  m_name_cb.AddString("<none>");
  for (Database::const_FieldID fi=data.begin_field(); fi!=data.end_field(); ++fi) {
    switch ((*fi).get_type()) {
    case FLD_INTEGER: case FLD_REAL:
      m_elev_cb.SetItemData(m_elev_cb.AddString((*fi).name),fi - data.begin_field());
      break;
    case FLD_STRING:
      m_name_cb.SetItemData(m_name_cb.AddString((*fi).name),fi - data.begin_field());
      break;
    case FLD_LOC:
      break;
    default:
      assert(0);
    }
  }

  m_elev_cb.SetCurSel(0);
  m_name_cb.SetCurSel(0);

  return TRUE;  // return TRUE unless you set the focus to a control
}

void CDataImport_dlg::OnOK()
{
  name_fld= data.begin_field() + m_name_cb.GetItemData(m_name_cb.GetCurSel());
  elev_fld= data.begin_field() + m_elev_cb.GetItemData(m_elev_cb.GetCurSel());

  CDialog::OnOK();
}
