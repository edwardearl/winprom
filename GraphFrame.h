// GraphFrame.h : interface of the CGraphFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHFRAME_H__9927AF4C_C393_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
#define AFX_GRAPHFRAME_H__9927AF4C_C393_11D3_8FA0_00A0C9C7BE10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CGraphFrame : public CMDIChildWnd
{
  DECLARE_DYNCREATE(CGraphFrame)
public:
  CGraphFrame();

// Attributes
public:

// Operations
public:

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CGraphFrame)
  public:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
  virtual void ActivateFrame(int nCmdShow = -1);
  //}}AFX_VIRTUAL
  virtual void OnUpdateFrameTitle(BOOL);

// Implementation
public:
  virtual ~CGraphFrame();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
  //{{AFX_MSG(CGraphFrame)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHFRAME_H__9927AF4C_C393_11D3_8FA0_00A0C9C7BE10__INCLUDED_)
