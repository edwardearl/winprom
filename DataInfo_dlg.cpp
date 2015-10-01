// DataInfo_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "DataInfo_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Location_fmt CDataInfo_dlg::info_fmt;

char (&buf)[sizeof CPromDoc::buf]=CPromDoc::buf;


/////////////////////////////////////////////////////////////////////////////
// CDataInfo_dlg dialog

CDataInfo_dlg::CDataInfo_dlg(const CPromDoc& d)
  : CDialog(CDataInfo_dlg::IDD),doc(d)
{
  //{{AFX_DATA_INIT(CDataInfo_dlg)
  m_file = _T("");
  m_area_n = _T("");
  m_area_s = _T("");
  m_area_w = _T("");
  m_records = 0;
  m_area_e = _T("");
  //}}AFX_DATA_INIT
}


void CDataInfo_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDataInfo_dlg)
  DDX_Control(pDX, IDC_FIELD_LIST, m_field_list);
  DDX_Text(pDX, IDC_FILE, m_file);
  DDX_Text(pDX, IDC_AREA_N, m_area_n);
  DDX_Text(pDX, IDC_AREA_S, m_area_s);
  DDX_Text(pDX, IDC_AREA_W, m_area_w);
  DDX_Text(pDX, IDC_RECORDS, m_records);
  DDX_Text(pDX, IDC_AREA_E, m_area_e);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataInfo_dlg, CDialog)
  //{{AFX_MSG_MAP(CDataInfo_dlg)
  ON_BN_CLICKED(IDC_FORMAT, OnFormat)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDataInfo_dlg message handlers

void CDataInfo_dlg::reformat()
{
  if (m_records==0) return;

  ((GridPoint)area.corner2()).sprint1(buf);
  m_area_n=buf;
  ((GridPoint)area.corner1()).sprint1(buf);
  m_area_s=buf;
  ((GridPoint)area.corner2()).sprint2(buf);
  m_area_e=buf;
  ((GridPoint)area.corner1()).sprint2(buf);
  m_area_w=buf;
}

BOOL CDataInfo_dlg::OnInitDialog()
{
  m_file=doc.db_name;
  if (doc.db_state==CHANGED) m_file+=" *";

  m_records=doc.data.get_nrec();
  for (Database::RecordID record=0; record<m_records; ++record)
    area|=doc.data.get_location(record);
  info_fmt.set();
  reformat();

  CDialog::OnInitDialog();

  m_field_list.SetTabStops(80);
  for (Database::const_FieldID fid=doc.data.begin_field();
       fid!=doc.data.end_field(); ++fid) {
    sprintf(buf,"%s\t%c",(const char *)(*fid).name,
	    Field::ft_char[(*fid).get_type()-1]);
    m_field_list.AddString(buf);
  }

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}

void CDataInfo_dlg::OnFormat()
{
  CLocationFmt_dlg dlg(this,info_fmt);
  if (dlg.DoModal()==IDOK) {
    info_fmt.set();
    reformat();
    UpdateData(FALSE);
  }
}
