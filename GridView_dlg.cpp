// GridView_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "GridView_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGridView_dlg dialog

CGridView_dlg::CGridView_dlg(unsigned short g, CWnd* pParent /*=NULL*/)
  : CDialog(CGridView_dlg::IDD, pParent),mug(g)
{
  //{{AFX_DATA_INIT(CGridView_dlg)
  m_note = _T("");
  m_grid_view = -1;
  //}}AFX_DATA_INIT
}

void CGridView_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CGridView_dlg)
  DDX_Text(pDX, IDC_NOTE, m_note);
  DDX_Radio(pDX, IDC_GV_NONE, m_grid_view);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGridView_dlg, CDialog)
  //{{AFX_MSG_MAP(CGridView_dlg)
  ON_BN_CLICKED(IDC_COLOR, OnColor)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGridView_dlg message handlers

BOOL CGridView_dlg::OnInitDialog()
{
  switch (mug) {
  case 0:
    m_note="The present map scale is to small to see grid points. "
	   "If you select points or elevation, you must zoom in to see it.";
    break;
  case 1:
    m_note="The present map scale is to small to see elevations. "
	   "If you select elevation, you must zoom in to see it.";
    break;
  }

  CDialog::OnInitDialog();

  m_color_box.SubclassDlgItem(IDC_COLOR_BOX,this);
  m_color_box.set_color(m_color);

  return TRUE;  // return TRUE unless you set the focus to a control
}

void CGridView_dlg::OnColor() 
{
  CViewColorDialog dlg(m_color,"Grid Color");
  if (dlg.DoModal()!=IDOK) return;

  m_color=dlg.GetColor();
  m_color_box.set_color(m_color);
}
