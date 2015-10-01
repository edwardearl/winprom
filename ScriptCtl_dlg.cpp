// ScriptCtl_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "winprom.h"
#include "ScriptCtl_dlg.h"
#include "PromDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScriptCtl_dlg dialog

CScriptCtl_dlg::CScriptCtl_dlg(CPromDoc& doc, const CString& n)
  : CBackgdTask_dlg(CScriptCtl_dlg::IDD),document(doc),name(n)
{
  //{{AFX_DATA_INIT(CScriptCtl_dlg)
  //}}AFX_DATA_INIT
}


void CScriptCtl_dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CScriptCtl_dlg)
  DDX_Control(pDX, IDC_COMMAND, m_command);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScriptCtl_dlg, CDialog)
  //{{AFX_MSG_MAP(CScriptCtl_dlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CScriptCtl_dlg *CScriptCtl_dlg::current_script_ctl;

int CScriptCtl_dlg::proc() throw()
{
  current_script_ctl=this;
  document.runScript(name,*this);
  return IDOK;
}


/////////////////////////////////////////////////////////////////////////////
// CScriptCtl_dlg message handlers

BOOL CScriptCtl_dlg::OnInitDialog() 
{
  CBackgdTask_dlg::OnInitDialog();

  return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}

void CScriptCtl_dlg::OnScriptCmd(const char *text)
{
  m_command.SetWindowText(text);
}
