// TrackEditor.h : main header file for the TRACKEDITOR application
//

#ifndef _INCLUDED_TRACKEDITOR_H_
#define _INCLUDED_TRACKEDITOR_H_

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTrackEditorApp:
// See TrackEditor.cpp for the implementation of this class
#include "MainFrm.h"
#include "ObjectView.h"

#define TO_ID(x) (x - IDB_CIRCUIT)
//

class CTrackEditorApp : public CWinApp
{
public:
  virtual BOOL
    PreTranslateMessage(MSG* pMsg);
  CTrackEditorApp();

  CMainFrame* pMainFrame;
  CSplitterWnd* splitWnd;

  CMultiDocTemplate* pDocTableTemplate;
  CMultiDocTemplate* pDocTemplate;
  CMultiDocTemplate* pCCTemplate;
  CMultiDocTemplate* pPitTemplate;
  CMultiDocTemplate* pObjectTemplate;
  CMultiDocTemplate* pCameraTemplate;
  CMultiDocTemplate* pObjectViewTemplate;
  CMultiDocTemplate* p3DViewTemplate;
  CMultiDocTemplate* pCommandStatsTemplate;
  CMultiDocTemplate* pJamFileTemplate;
  CMultiDocTemplate* pInternalFileTemplate;
  CMultiDocTemplate* pTrackGLViewTemplate;

  CTrackTree* theMainTree;
  CTrackPropertySheet* theMainPropertyTable;
  BOOL showMainTree;
  BOOL showTableTree;
  BOOL showToolsToolbar;
  BOOL showViewToolbar;
  BOOL cleverNames;
  BOOL showImageInTable;
  BOOL showJamNumbers;
  BOOL showObjectToolbar;
  BOOL showToolbar;
  BOOL AutoBackup;
  BOOL isF1GP;

  CTrackTree*
    getMainTree()
  {
    return theMainTree;
  }

  BOOL
    CheckCurrentEditorVersion();

  void
    OnTrackView(CDocument* pDoc);
  void
    OnTrackScrollView(CDocument* pDoc);
  void
    OnPitView(CDocument* pDoc);
  void
    OnCCView(CDocument* pDoc);
  void
    OnObjectView(CDocument* pDoc);
  void
    OnWorldView(CDocument* pDoc);
  void
    OnCameraView(CDocument* pDoc);
  void
    OnJamView(CDocument* pDoc);
  void
    OnInternalView(CDocument* pDoc);
  void
    OnTrackglView(CDocument* pDoc);

  CObjectView*
    OnInternalObjectView(CDocument* pDoc);
  void
    OnCommandStatsView(CDocument* pDoc);

  bool
    IsLandScape();

  void
    LoadImages();
  CImageList* pImageList;

  CImageList*
    getSharedImageList()
  {
    return pImageList;
  }

  void
    SetMessageText(LPCSTR str)
  {
    if (pMainFrame) {
      pMainFrame->SetMessageText(str);
    }
  }

  CTrackPropertySheet*
    getTable()
  {
    return theMainPropertyTable;
  }

  CString
    getGP2Location()
  {
    CString strSection = "Preferences";
    CString strStringItem = "GP2Location";
    CWinApp* pApp = AfxGetApp();

    CString circuitsDir =
      pApp->GetProfileString(strSection, strStringItem, "c:\\gp2\\");
    return circuitsDir;
  }

  void
    OnHelpContentString(CString url);
  void
    OnStartHelp(BOOL reposition = FALSE);

  BOOL showReadObjNames;

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTrackEditorApp)
public:
  virtual BOOL
    InitInstance();
  virtual CDocument*
    OpenDocumentFile(LPCTSTR lpszFileName);
  //}}AFX_VIRTUAL

  // Implementation

  //{{AFX_MSG(CTrackEditorApp)
  afx_msg void
    OnAppAbout();
  afx_msg void
    OnWindowTileHorz();
  afx_msg void
    OnWindowTilevertical();
  afx_msg void
    OnTree();
  afx_msg void
    OnUpdateTree(CCmdUI* pCmdUI);
  afx_msg void
    OnHelpCredits();
  afx_msg void
    OnInstallTracks();
  afx_msg void
    OnTableproperties();
  afx_msg void
    OnUpdateTableproperties(CCmdUI* pCmdUI);
  afx_msg void
    OnAppExit();
  afx_msg void
    OnFileNew();
  afx_msg void
    OnRegister();
  afx_msg void
    OnCommandIndex();
  afx_msg void
    OnViewToolsToolbar();
  afx_msg void
    OnUpdateViewToolsToolbar(CCmdUI* pCmdUI);
  afx_msg void
    OnFileOpen();
  afx_msg void
    OnEditLocation();
  afx_msg void
    OnImportNames();
  afx_msg void
    OnGp2exe();
  afx_msg void
    OnTableImages();
  afx_msg void
    OnUpdateTableImages(CCmdUI* pCmdUI);
  afx_msg void
    OnShowjamNumbers();
  afx_msg void
    OnUpdateShowjamNumbers(CCmdUI* pCmdUI);
  afx_msg void
    OnViewViewtoolbar();
  afx_msg void
    OnUpdateViewViewtoolbar(CCmdUI* pCmdUI);
  afx_msg void
    OnHelp();
  afx_msg void
    OnHelpFinder();
  afx_msg void
    OnViewObjecttoolbar();
  afx_msg void
    OnUpdateViewObjecttoolbar(CCmdUI* pCmdUI);
  afx_msg void
    OnViewReadobjnames();
  afx_msg void
    OnUpdateViewReadobjnames(CCmdUI* pCmdUI);
  afx_msg void
    OnEditAutobackup();
  afx_msg void
    OnUpdateEditAutobackup(CCmdUI* pCmdUI);
  afx_msg void
    OnFileOpenF1gp();
  afx_msg void
    OnHelpQuickkeyreminder();
  afx_msg void
    OnViewToolbar();
  afx_msg void
    OnUpdateViewToolbar(CCmdUI* pCmdUI);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
private:
  void
    ShowTipAtStartup(void);

private:
  void
    ShowTipOfTheDay(void);
};

extern CTrackEditorApp theApp;

/////////////////////////////////////////////////////////////////////////////

#endif
