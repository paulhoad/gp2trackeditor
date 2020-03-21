// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////
#ifndef _MAIN_FRAME_H
#define _MAIN_FRAME_H

#include "TrackTree.h"
#include "TrackPropertySheet.h"
#include "MainToolbar.h"

class CMainFrame : public CMDIFrameWnd
{
  DECLARE_DYNAMIC(CMainFrame)
public:
  CMainFrame();

  void
    SetMessageText(LPCSTR str)
  {
    if (m_wndStatusBar) {
      m_wndStatusBar.SetPaneText(ID_SEPARATOR, str, TRUE);
    }
  }

  // Attributes
public:
  void
    SaveToolBarPositions();

  // Operations
public:
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMainFrame)
public:
  virtual BOOL
    PreCreateWindow(CREATESTRUCT& cs);
  //}}AFX_VIRTUAL

  // Implementation
public:
  virtual ~CMainFrame();
#ifdef _DEBUG
  virtual void
    AssertValid() const;
  virtual void
    Dump(CDumpContext& dc) const;
#endif

  // CTrackTree *getTree()
  //{
  //   return &m_wndDlgBar;
  //}

  void
    RecalcLayout(BOOL bNotify);

  CStatusBar m_wndStatusBar;
  CMainToolbar m_wndToolBar;
  CToolBar m_wndToolsToolBar;
  CToolBar m_wndShowToolBar;
  CToolBar m_wndObjectToolBar;
  CTrackTree m_wndDlgBar;
  CTrackPropertySheet m_wndProperty;

  void
    SavePositions();

  BOOL
    IsIE4Capable();

  // CSplitterWnd m_splitterWnd;
protected:// control bar embedded members
          // Generated message map functions
protected:
  afx_msg void
    OnInitMenu(CMenu* pMenu);
  //{{AFX_MSG(CMainFrame)
  afx_msg int
    OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void
    OnSize(UINT nType, int cx, int cy);
  afx_msg void
    OnSaveTrackImage();
  afx_msg void
    OnZoomcombo();
  afx_msg void
    OnClose();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

#endif

/////////////////////////////////////////////////////////////////////////////
