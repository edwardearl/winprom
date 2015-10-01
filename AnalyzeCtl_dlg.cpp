// AnalyzeCtl_dlg.cpp : implementation file
// removed comment

#include "stdafx.h"
#include "winprom.h"
#include "AnalyzeCtl_dlg.h"
#include "domain_anal.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAnalyzeCtl_dlg dialog

CAnalyzeCtl_dlg::CAnalyzeCtl_dlg(CPromDoc& doc)
  : CBackgdTask_dlg(CAnalyzeCtl_dlg::IDD),document(doc)
{
  //{{AFX_DATA_INIT(CAnalyzeCtl_dlg)
  m_peak_count = 0;
  m_sadl_count = 0;
  m_search_pc = _T("");
  //}}AFX_DATA_INIT
}


void CAnalyzeCtl_dlg::DoDataExchange(CDataExchange* pDX)
{
  CBackgdTask_dlg::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAnalyzeCtl_dlg)
  DDX_Control(pDX, IDC_WALKUP_COUNT, m_walkup_count_edit);
  DDX_Control(pDX, IDC_FOUND, m_found);
  DDX_Control(pDX, IDC_SEARCH, m_search);
  DDX_Control(pDX, IDC_WALKUP_PROG, m_walkup_prog);
  DDX_Control(pDX, IDC_SEARCH_PROG, m_search_prog);
  DDX_Text(pDX, IDC_PEAK_COUNT, m_peak_count);
  DDX_Text(pDX, IDC_SADDLE_COUNT, m_sadl_count);
  DDX_Text(pDX, IDC_SEARCH_PC, m_search_pc);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnalyzeCtl_dlg, CBackgdTask_dlg)
  //{{AFX_MSG_MAP(CAnalyzeCtl_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


CAnalyzeCtl_dlg *CAnalyzeCtl_dlg::current_anal_ctl;
static char buf[64];

static void line_cb(float pc)
{
  AfxGetApp()->PostThreadMessage(IDT_ANALYZE_LINE,nint(pc*1000),0);
}

static void peak_cb()
{
  ++CAnalyzeCtl_dlg::current_anal_ctl->m_peak_count;
  if (CAnalyzeCtl_dlg::current_anal_ctl->stop_request) throw user_stop();
}

static void sadl_cb()
{
  ++CAnalyzeCtl_dlg::current_anal_ctl->m_sadl_count;
  if (CAnalyzeCtl_dlg::current_anal_ctl->stop_request) throw user_stop();
}

void CAnalyzeCtl_dlg::update_line(int pci)
{
  m_search_pc.Format("%.1f%%",pci*0.1);
  m_search_prog.SetPos(pci);
  UpdateData(FALSE);
  if (stop_request) throw user_stop();
}

static void walkup_cb()
{
  static unsigned out_count,last_out_count;
  unsigned& walkup_count=CAnalyzeCtl_dlg::current_anal_ctl->walkup_count;
  if (walkup_count==0) last_out_count=0;
  ++walkup_count;
  out_count=(walkup_count*1000)/CAnalyzeCtl_dlg::current_anal_ctl->m_sadl_count;
  if (out_count>last_out_count) {
    AfxGetApp()->PostThreadMessage(IDT_ANALYZE_WALKUP,out_count,0);
    last_out_count=out_count;
  }
  if (CAnalyzeCtl_dlg::current_anal_ctl->stop_request) throw user_stop();
}

void CAnalyzeCtl_dlg::update_walkup(int out_count)
{
  sprintf(buf,"%d",walkup_count);
  m_walkup_count_edit.SetWindowText(buf);
  m_walkup_prog.SetPos(out_count);
}

int CAnalyzeCtl_dlg::proc() throw()
{
  current_anal_ctl=this;
  walkup_count=0;
  try {
#ifdef administrator
    document.tree.walkup_analyze(document.elev,document.domain,
				 line_cb,peak_cb,sadl_cb,walkup_cb);
#else
    assert(0);
#endif
    error=0;
  }
  catch (user_stop u) {
    error=u.what();
    return IDCANCEL;
  }
  catch (exception e) {
    error=e.what();
    return IDCANCEL;
  }
  catch (...) {
    error="Unknown exception";
    return IDCANCEL;
  }
  return IDOK;
}


/////////////////////////////////////////////////////////////////////////////
// CAnalyzeCtl_dlg message handlers

BOOL CAnalyzeCtl_dlg::OnInitDialog()
{
  CBackgdTask_dlg::OnInitDialog();

  m_search_prog.SetRange(0,1000);
  m_walkup_prog.SetRange(0,1000);

  m_search.SetWindowText(document.is_drainage?
    "Basin && saddle search progress":"Peak && saddle search progress");
  m_found.SetWindowText(document.is_drainage?"Basins found:":"Peaks found:");

  return TRUE;  // return TRUE unless you set the focus to a control
}
