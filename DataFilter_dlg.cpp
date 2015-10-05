// WinProm Copyright 2015 Edward Earl
// All rights reserved.
//
// This software is distributed under a license that is described in
// the LICENSE file that accompanies it.
//
// DataFilter_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "DataFilter_dlg.h"
#include "DataListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_FIELD_MIN 128
#define IDC_FIELD_MAX 192
#define IDC_STRING_RULE 256

static char (&buf)[sizeof CDataListView::buf]=CDataListView::buf;


/////////////////////////////////////////////////////////////////////////////
// CDataFilter_dlg dialog

CDataFilter_dlg::CDataFilter_dlg(Record_filter& rf, CWnd* pParent /*=NULL*/)
  : CDialog(CDataFilter_dlg::IDD,pParent),rec_filter(rf)
{
  init_data();
}

CDataFilter_dlg::CDataFilter_dlg(Record_filter& rf, UINT rcTpl, CWnd* pParent /*=NULL*/)
  : CDialog(rcTpl,pParent),rec_filter(rf)
{
  init_data();
}

void CDataFilter_dlg::init_data()
{
  //{{AFX_DATA_INIT(CDataFilter_dlg)
  m_accept_nodata = FALSE;
  m_case = FALSE;
  m_latin1 = FALSE;
  //}}AFX_DATA_INIT
}

void CDataFilter_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDataFilter_dlg)
  DDX_Check(pDX, IDC_ACCEPT_NODATA, m_accept_nodata);
  DDX_Check(pDX, IDC_CASE, m_case);
  DDX_Check(pDX, IDC_LATIN1, m_latin1);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataFilter_dlg, CDialog)
  //{{AFX_MSG_MAP(CDataFilter_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


#define MINMAX_INTERVAL 24
#define RECT_INTERVAL 20

/////////////////////////////////////////////////////////////////////////////
// CDataFilter_dlg message handlers

void CDataFilter_dlg::create_minmax_text(const CRect& minval_rect,
					 const CRect& maxval_rect,
					 unsigned fld_ndx)
{
  min_value[fld_ndx].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP,
    minval_rect,this,IDC_FIELD_MIN+fld_ndx);
  min_value[fld_ndx].ModifyStyleEx(0,WS_EX_CLIENTEDGE,SWP_DRAWFRAME);
  min_value[fld_ndx].SetFont(GetFont(),FALSE);
  max_value[fld_ndx].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP,
    maxval_rect,this,IDC_FIELD_MAX+fld_ndx);
  max_value[fld_ndx].ModifyStyleEx(0,WS_EX_CLIENTEDGE,SWP_DRAWFRAME);
  max_value[fld_ndx].SetFont(GetFont(),FALSE);
}

BOOL CDataFilter_dlg::OnInitDialog()
{
  m_accept_nodata=rec_filter.accept_norec;
  m_case=rec_filter.get_casens();
  m_latin1=rec_filter.get_isosens();

  CDialog::OnInitDialog();

  CRect win_rect,name_rect,type_rect,minval_rect,maxval_rect,
    sr_rect,east_rect,west_rect;
  GetWindowRect(&win_rect);
  unsigned fld_ndx=0;
  int ew_midpt;
  minval_rect.top=maxval_rect.top=win_rect.bottom-win_rect.top-28;
  type_rect.top=name_rect.top=minval_rect.top+2;
  name_rect.left=10;
  name_rect.bottom=type_rect.bottom=minval_rect.bottom=maxval_rect.bottom=minval_rect.top+18;
  name_rect.right=name_rect.left+152;
  type_rect.left=name_rect.right+8;
  type_rect.right=type_rect.left+8;
  minval_rect.left=type_rect.right+8;
  minval_rect.right=minval_rect.left+120;
  maxval_rect.left=minval_rect.right+8;
  maxval_rect.right=maxval_rect.left+120;
  for (vector<Field_filter>::iterator ffi=rec_filter.field_filters.begin();
       ffi!=rec_filter.field_filters.end(); ++ffi,++fld_ndx) {
    if ((*ffi).type==FLD_LOC) {
      name_rect.top+=RECT_INTERVAL;
      name_rect.bottom+=RECT_INTERVAL;
      type_rect.top+=RECT_INTERVAL;
      type_rect.bottom+=RECT_INTERVAL;
    }
    name_label[fld_ndx].Create((*ffi).name,WS_CHILD|WS_VISIBLE,name_rect,this);
    name_label[fld_ndx].SetFont(GetFont(),FALSE);
    buf[0]=Field::ft_char[(*ffi).type-1];
    buf[1]='\0';
    type_label[fld_ndx].Create(buf,WS_CHILD|WS_VISIBLE,type_rect,this);
    type_label[fld_ndx].SetFont(GetFont(),FALSE);
    if ((*ffi).type==FLD_LOC) {
      name_rect.top+=RECT_INTERVAL;
      name_rect.bottom+=RECT_INTERVAL;
      type_rect.top+=RECT_INTERVAL;
      type_rect.bottom+=RECT_INTERVAL;
    }
    switch ((*ffi).type) {
    case FLD_STRING:
      sr_rect=minval_rect;
      sr_rect.bottom=sr_rect.top+120;
      string_rule[fld_ndx].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|CBS_DROPDOWNLIST,
	sr_rect,this,IDC_STRING_RULE+fld_ndx);
      string_rule[fld_ndx].SetFont(GetFont(),FALSE);
      string_rule[fld_ndx].AddString("is");
      string_rule[fld_ndx].AddString("is not");
      string_rule[fld_ndx].AddString("contains");
      string_rule[fld_ndx].AddString("does not contain");
      string_rule[fld_ndx].AddString("begins with");
      string_rule[fld_ndx].AddString("ends with");
      string_rule[fld_ndx].SetCurSel((*ffi).type_filter.string_filter.rule);
      max_value[fld_ndx].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP,
	maxval_rect,this,IDC_FIELD_MAX+fld_ndx);
      max_value[fld_ndx].ModifyStyleEx(0,WS_EX_CLIENTEDGE,SWP_DRAWFRAME);
      max_value[fld_ndx].SetFont(GetFont(),FALSE);
      max_value[fld_ndx].SetWindowText((*ffi).string_ftr_val);
      break;
    case FLD_LOC:
      west_rect=minval_rect;
      east_rect=maxval_rect;
      ew_midpt=(west_rect.right+east_rect.left)/2;
      west_rect.right=ew_midpt-2;
      west_rect.left=west_rect.right-72;
      east_rect.left=ew_midpt+2;
      east_rect.right=east_rect.left+72;
      // prepare north limit
      sr_rect.top=west_rect.top;
      sr_rect.bottom=west_rect.bottom;
      sr_rect.left=(west_rect.left+east_rect.left)/2;
      sr_rect.right=(west_rect.right+east_rect.right)/2;
      north_value[fld_ndx].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP,
	sr_rect,this,IDC_FIELD_MIN+fld_ndx);
      north_value[fld_ndx].ModifyStyleEx(0,WS_EX_CLIENTEDGE,SWP_DRAWFRAME);
      north_value[fld_ndx].SetFont(GetFont(),FALSE);
      // prepare west and east limits
      west_rect.top+=RECT_INTERVAL;
      west_rect.bottom+=RECT_INTERVAL;
      east_rect.top+=RECT_INTERVAL;
      east_rect.bottom+=RECT_INTERVAL;
      create_minmax_text(west_rect,east_rect,fld_ndx);
      // prepare south limit
      sr_rect.top+=2*RECT_INTERVAL;
      sr_rect.bottom+=2*RECT_INTERVAL;
      south_value[fld_ndx].Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP,
	sr_rect,this,IDC_FIELD_MIN+fld_ndx);
      south_value[fld_ndx].ModifyStyleEx(0,WS_EX_CLIENTEDGE,SWP_DRAWFRAME);
      south_value[fld_ndx].SetFont(GetFont(),FALSE);
      // fill in the text fields
      {
	CString n_limit,s_limit,e_limit,w_limit;
	CPromDoc::set_rect((*ffi).loc_limits,n_limit,s_limit,e_limit,w_limit);
	north_value[fld_ndx].SetWindowText(n_limit);
	min_value[fld_ndx].SetWindowText(w_limit);
	max_value[fld_ndx].SetWindowText(e_limit);
	south_value[fld_ndx].SetWindowText(s_limit);
      }
      // adjust window rectangles
      minval_rect.top+=2*RECT_INTERVAL;
      minval_rect.bottom+=2*RECT_INTERVAL;
      maxval_rect.top+=2*RECT_INTERVAL;
      maxval_rect.bottom+=2*RECT_INTERVAL;
      win_rect.bottom+=2*RECT_INTERVAL;
      break;
    case FLD_INTEGER:
      create_minmax_text(minval_rect,maxval_rect,fld_ndx);
      if ((*ffi).type_filter.int_filter.have_min) {
	sprintf(buf,"%d",(*ffi).type_filter.int_filter.min);
	min_value[fld_ndx].SetWindowText(buf);
      }
      if ((*ffi).type_filter.int_filter.have_max) {
	sprintf(buf,"%d",(*ffi).type_filter.int_filter.max);
	max_value[fld_ndx].SetWindowText(buf);
      }
      break;
    case FLD_REAL:
      create_minmax_text(minval_rect,maxval_rect,fld_ndx);
      if ((*ffi).type_filter.real_filter.have_min) {
	sprintf(buf,"%g",(*ffi).type_filter.real_filter.min);
	min_value[fld_ndx].SetWindowText(buf);
      }
      if ((*ffi).type_filter.real_filter.have_max) {
	sprintf(buf,"%g",(*ffi).type_filter.real_filter.max);
	max_value[fld_ndx].SetWindowText(buf);
      }
      break;
    }
    minval_rect.top+=MINMAX_INTERVAL;
    maxval_rect.top+=MINMAX_INTERVAL;
    name_rect.top+=MINMAX_INTERVAL;
    type_rect.top+=MINMAX_INTERVAL;
    minval_rect.bottom+=MINMAX_INTERVAL;
    maxval_rect.bottom+=MINMAX_INTERVAL;
    name_rect.bottom+=MINMAX_INTERVAL;
    type_rect.bottom+=MINMAX_INTERVAL;
    win_rect.bottom+=MINMAX_INTERVAL;
  }
  MoveWindow(win_rect);

  return TRUE;  // return TRUE unless you set the focus to a control
}

void CDataFilter_dlg::OnOK()
{
  CString value;
  unsigned fld_ndx=0;
  for (vector<Field_filter>::iterator ffi=rec_filter.field_filters.begin();
       ffi!=rec_filter.field_filters.end(); ++ffi,++fld_ndx) {
    switch ((*ffi).type) {
    case FLD_STRING:
      (*ffi).type_filter.string_filter.rule=
	(Field_filter::String_rule)string_rule[fld_ndx].GetCurSel();
      max_value[fld_ndx].GetWindowText(value);
      (*ffi).string_ftr_val=value;
      break;
    case FLD_INTEGER:
      min_value[fld_ndx].GetWindowText(value);
      if ((*ffi).type_filter.int_filter.have_min=!value.IsEmpty())
	(*ffi).type_filter.int_filter.min=atoi(value);
      max_value[fld_ndx].GetWindowText(value);
      if ((*ffi).type_filter.int_filter.have_max=!value.IsEmpty())
	(*ffi).type_filter.int_filter.max=atoi(value);
      break;
    case FLD_REAL:
      min_value[fld_ndx].GetWindowText(value);
      if ((*ffi).type_filter.real_filter.have_min=!value.IsEmpty())
	(*ffi).type_filter.int_filter.min=atof(value);
      max_value[fld_ndx].GetWindowText(value);
      if ((*ffi).type_filter.real_filter.have_max=!value.IsEmpty())
	(*ffi).type_filter.real_filter.max=atof(value);
      break;
    case FLD_LOC:
      {
	CString n_value,s_value,e_value,w_value;
	min_value[fld_ndx].GetWindowText(w_value);
	max_value[fld_ndx].GetWindowText(e_value);
	north_value[fld_ndx].GetWindowText(n_value);
	south_value[fld_ndx].GetWindowText(s_value);
	if (!CPromDoc::get_rect((*ffi).loc_limits,this,n_value,s_value,e_value,w_value,
				north_value[fld_ndx],south_value[fld_ndx],
				max_value[fld_ndx],min_value[fld_ndx]))
	  return;
      }
      break;
    }
  }

  UpdateData();
  rec_filter.accept_norec=m_accept_nodata!=FALSE;
  rec_filter.set_sens(m_case!=FALSE,m_latin1!=FALSE);
  EndDialog(IDOK);
}
