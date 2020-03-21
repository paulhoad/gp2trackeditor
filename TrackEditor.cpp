// TrackEditor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TrackEditor.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "TrackEditorDoc.h"
#include "TrackEditorScrollView.h"
#include "TrackInfo.h"
#include "TrackTableView.h"
#include "Info.h"
#include "PitInfo.h"
#include "ObjectInfo.h"
#include "CameraView.h"
#include "ObjectView.h"
#include "WorldView.h"
#include "TrackOpenGLView.h"
#include "Credit.h"
#include "TrackSelection.h"
#include "CommandStats.h"
#include "JamView.h"
#include "InternalView.h"

#include "Register.h"
#include "CmdList.h"
#include "ChangeLog.txt"
#include "HyperLinkButton.h"

#include "Shlobj.h"

#include <winspool.h>


#include "htmlhelp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "FileVersion.h"
#include "Splash.h"

void LoadControlBarPosition(CFrameWnd *frame,CControlBar *cntrl,CString strStringItem);
void SaveControlBarPosition(CFrameWnd *frame,CControlBar *cntrl,CString strStringItem);

void HtmlHelpFunc(HWND caller,LPCSTR filename,UINT cmd,DWORD dwData);

/////////////////////////////////////////////////////////////////////////////
// CTrackEditorApp

BEGIN_MESSAGE_MAP(CTrackEditorApp, CWinApp)
	ON_COMMAND(CG_IDS_TIPOFTHEDAY, ShowTipOfTheDay)
	//{{AFX_MSG_MAP(CTrackEditorApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_WINDOW_TILE_HORZ, OnWindowTileHorz)
	ON_COMMAND(ID_WINDOW_TILEVERTICAL, OnWindowTilevertical)
	ON_COMMAND(ID_TREE, OnTree)
	ON_UPDATE_COMMAND_UI(ID_TREE, OnUpdateTree)
	ON_COMMAND(ID_HELP_CREDITS, OnHelpCredits)
	ON_COMMAND(ID_INSTALL_TRACKS, OnInstallTracks)
	ON_COMMAND(ID_TABLEPROPERTIES, OnTableproperties)
	ON_UPDATE_COMMAND_UI(ID_TABLEPROPERTIES, OnUpdateTableproperties)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_REGISTER, OnRegister)
	ON_COMMAND(ID_COMMAND_INDEX, OnCommandIndex)
	ON_COMMAND(ID_VIEW_TOOLS_TOOLBAR, OnViewToolsToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLS_TOOLBAR, OnUpdateViewToolsToolbar)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_EDIT_LOCATION, OnEditLocation)
	ON_COMMAND(ID_IMPORT_NAMES, OnImportNames)
	ON_COMMAND(ID_GP2EXE, OnGp2exe)
	ON_COMMAND(ID_TABLE_IMAGES, OnTableImages)
	ON_UPDATE_COMMAND_UI(ID_TABLE_IMAGES, OnUpdateTableImages)
	ON_COMMAND(ID_SHOWJAM_NUMBERS, OnShowjamNumbers)
	ON_UPDATE_COMMAND_UI(ID_SHOWJAM_NUMBERS, OnUpdateShowjamNumbers)
	ON_COMMAND(ID_VIEW_VIEWTOOLBAR, OnViewViewtoolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_VIEWTOOLBAR, OnUpdateViewViewtoolbar)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
	ON_COMMAND(ID_VIEW_OBJECTTOOLBAR, OnViewObjecttoolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OBJECTTOOLBAR, OnUpdateViewObjecttoolbar)
	ON_COMMAND(ID_VIEW_READOBJNAMES, OnViewReadobjnames)
	ON_UPDATE_COMMAND_UI(ID_VIEW_READOBJNAMES, OnUpdateViewReadobjnames)
	ON_COMMAND(ID_EDIT_AUTOBACKUP, OnEditAutobackup)
	ON_UPDATE_COMMAND_UI(ID_EDIT_AUTOBACKUP, OnUpdateEditAutobackup)
	ON_COMMAND(ID_FILE_OPEN_F1GP, OnFileOpenF1gp)
	ON_COMMAND(ID_HELP_QUICKKEYREMINDER, OnHelpQuickkeyreminder)
	ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateViewToolbar)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	
END_MESSAGE_MAP()

//ON_COMMAND( ID_HELP, OnHelp )

#define PROFILE(x,b) x =  GetProfileInt(strSection, #x,b) == 1 ? TRUE : FALSE;
#define WR_PROFILE(x) WriteProfileInt(strSection, #x,x);


/////////////////////////////////////////////////////////////////////////////
// CTrackEditorApp construction

CTrackEditorApp::CTrackEditorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	pImageList = NULL;
	isF1GP = false;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTrackEditorApp object

CTrackEditorApp theApp;

extern BOOL CheckRegistrationName();

/////////////////////////////////////////////////////////////////////////////
// CTrackEditorApp initialization

BOOL CTrackEditorApp::InitInstance()
{
	// CG: The following block was added by the Splash Screen component.
\
	{
\
		CCommandLineInfo cmdInfo;
\
		ParseCommandLine(cmdInfo);
\

\
		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
\
	}
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	SetRegistryKey("GP2TrackEditorProject\\TrackEditor");

	LoadStdProfileSettings(8);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CString strSection       = "Preferences";

	PROFILE(showMainTree,TRUE)
	PROFILE(showTableTree,TRUE)
	PROFILE(showImageInTable,TRUE)
	PROFILE(showJamNumbers,TRUE)
	PROFILE(showToolsToolbar,TRUE)
	PROFILE(showViewToolbar,TRUE)
	PROFILE(showObjectToolbar,TRUE)
	PROFILE(showReadObjNames,TRUE)
	PROFILE(AutoBackup,FALSE)
	PROFILE(showToolbar,TRUE)
	

	

	
	pDocTemplate = new CMultiDocTemplate(
		IDR_TRACKETYPE,
		RUNTIME_CLASS(CTrackEditorDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(TrackEditorScrollView));
	AddDocTemplate(pDocTemplate);

	EnableShellOpen();
    RegisterShellFileTypes(TRUE);

	pDocTableTemplate = new CMultiDocTemplate(IDR_TRACKDATA,
			RUNTIME_CLASS(CTrackEditorDoc),
			RUNTIME_CLASS(CChildFrame),
			RUNTIME_CLASS(CTrackInfo));
	AddDocTemplate(pDocTableTemplate);

	pCCTemplate = new CMultiDocTemplate(IDR_CCDATA,
			RUNTIME_CLASS(CTrackEditorDoc),
			RUNTIME_CLASS(CChildFrame),
			RUNTIME_CLASS(CInfo));
	AddDocTemplate(pCCTemplate);

	pPitTemplate = new CMultiDocTemplate(IDR_PITDATA,
			RUNTIME_CLASS(CTrackEditorDoc),
			RUNTIME_CLASS(CChildFrame),
			RUNTIME_CLASS(CPitInfo));
	AddDocTemplate(pPitTemplate);

	pObjectTemplate	= new CMultiDocTemplate(IDR_OBJECT,
			RUNTIME_CLASS(CTrackEditorDoc),
			RUNTIME_CLASS(CChildFrame),
			RUNTIME_CLASS(CObjectInfo));
	AddDocTemplate(pObjectTemplate);

	pCameraTemplate	= new CMultiDocTemplate(IDR_CAMERA,
			RUNTIME_CLASS(CTrackEditorDoc),
			RUNTIME_CLASS(CChildFrame),
			RUNTIME_CLASS(CCameraView));
	AddDocTemplate(pCameraTemplate);

	pObjectViewTemplate	= new CMultiDocTemplate(IDR_OBJECT_INTERNAL,
			RUNTIME_CLASS(CTrackEditorDoc),
			RUNTIME_CLASS(CChildFrame),
			RUNTIME_CLASS(CObjectView));
	AddDocTemplate(pObjectViewTemplate);

    p3DViewTemplate	= new CMultiDocTemplate(IDR_WORLD,
			RUNTIME_CLASS(CTrackEditorDoc),
			RUNTIME_CLASS(CChildFrame),
			RUNTIME_CLASS(CWorldView));
	AddDocTemplate(p3DViewTemplate);

	
	pCommandStatsTemplate	= new CMultiDocTemplate(IDR_TRACKDATA,
			RUNTIME_CLASS(CTrackEditorDoc),
			RUNTIME_CLASS(CChildFrame),
			RUNTIME_CLASS(CCommandStats));
	AddDocTemplate(pCommandStatsTemplate);

	pJamFileTemplate = new CMultiDocTemplate(IDR_TRACKDATA,
			RUNTIME_CLASS(CTrackEditorDoc),
			RUNTIME_CLASS(CChildFrame),
			RUNTIME_CLASS(CJamView));
	AddDocTemplate(pJamFileTemplate);

	pInternalFileTemplate = new CMultiDocTemplate(IDR_TRACKDATA,
			RUNTIME_CLASS(CTrackEditorDoc),
			RUNTIME_CLASS(CChildFrame),
			RUNTIME_CLASS(CInternalView));
	AddDocTemplate(pInternalFileTemplate);

	pTrackGLViewTemplate = new CMultiDocTemplate(IDR_TRACKDATA,
			RUNTIME_CLASS(CTrackEditorDoc),
			RUNTIME_CLASS(CChildFrame),
			RUNTIME_CLASS(CTrackOpenGLView));
	AddDocTemplate(pTrackGLViewTemplate);


	




	// create main MDI Frame window
	pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// enable file manager drag/drop and DDE Execute open
	// Add the follow code.
	//EnableShellOpen();
    RegisterShellFileTypes(TRUE);
    m_pMainWnd->DragAcceptFiles();

    



	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	

	WINDOWPLACEMENT placement;

	placement.length = sizeof(WINDOWPLACEMENT);
	pMainFrame->GetWindowPlacement(&placement);

	//RECT rect1;
	//RECT rect;
	//pMainFrame->GetWindowRect(&rect1);
	placement.flags             = AfxGetApp()->GetProfileInt(strSection,"SizeFlags",placement.flags);
	placement.showCmd           = AfxGetApp()->GetProfileInt(strSection,"SizeFlags",placement.showCmd);
	placement.ptMinPosition.x   = AfxGetApp()->GetProfileInt(strSection,"SizeptMinPosX",placement.ptMinPosition.x);
	placement.ptMinPosition.y   = AfxGetApp()->GetProfileInt(strSection,"SizeptMinPosY",placement.ptMinPosition.y);
	placement.ptMaxPosition.x   = AfxGetApp()->GetProfileInt(strSection,"SizeptMaxPosX",placement.ptMaxPosition.x);
	placement.ptMaxPosition.y   = AfxGetApp()->GetProfileInt(strSection,"SizeptMaxPosY",placement.ptMaxPosition.y);	
    placement.rcNormalPosition.left   = AfxGetApp()->GetProfileInt(strSection,"SizeLeft",placement.rcNormalPosition.left);
	placement.rcNormalPosition.top    = AfxGetApp()->GetProfileInt(strSection,"SizeTop",placement.rcNormalPosition.top);
	placement.rcNormalPosition.right  = AfxGetApp()->GetProfileInt(strSection,"SizeRight",placement.rcNormalPosition.right);
	placement.rcNormalPosition.bottom = AfxGetApp()->GetProfileInt(strSection,"SizeBottom",placement.rcNormalPosition.bottom);

	//pMainFrame->SetWindowPos(NULL,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,SWP_SHOWWINDOW);

	LoadImages();

	showTableTree = !showTableTree;
	OnTableproperties();
	showMainTree = !showMainTree;
	OnTree();

	showToolsToolbar = !showToolsToolbar;
	OnViewToolsToolbar();

	showViewToolbar = !showViewToolbar;
	OnViewViewtoolbar();

	showObjectToolbar = !showObjectToolbar;
	OnViewObjecttoolbar();
	
	//CheckCurrentEditorVersion();


	pMainFrame->SetWindowPlacement(&placement);
	
	//pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();


	//if (!CheckRegistrationName())
	//{
		//Sleep(3);
	//}


	if (!CheckRegistrationName())
	{
	   OnAppAbout();
	}
	
	// CG: This line inserted by 'Tip of the Day' component.
	ShowTipAtStartup();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//CHyperLinkButton	m_Gp2Com;
	//CHyperLinkButton	m_HomePage;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual void OnCancel();
	afx_msg void OnRegister();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnForumSite();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnButton3();
	afx_msg void OnButtonXoom();
	afx_msg void OnButtonGrandPrix2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

//#include "buildnumber.h"

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	char VersionString[10];

	CFileVersion fileVersion;

	// get the name of the programme
	char ModuleName[256];
	GetModuleFileName(0,ModuleName,256);

	fileVersion.Open(ModuleName);

	//wsprintf(VersionString,"%d.%d.%d.%d.%d",MAJOR_VERSION,MINOR_VERSION,DEBUG_VERSION,DEBUG_SUB_VERSION,BUILDNUMBER);
	wsprintf(VersionString,"%s",fileVersion.GetProductVersion());
	//SetDlgItemText(IDC_VERSION_STR,VersionString);
	CString ver(VersionString);

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//DDX_Control(pDX, IDC_GP2_COM, m_Gp2Com);
	//DDX_Control(pDX, IDC_HOME_PAGE, m_HomePage);
	DDX_Text(pDX, IDC_VERSION_STR, ver);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDREGISTER, OnRegister)
	ON_BN_CLICKED(IDC_FORUM_SITE, OnForumSite)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON2, OnButtonXoom)
	ON_BN_CLICKED(IDC_BUTTON4, OnButtonGrandPrix2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CTrackEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CTrackEditorApp commands

void CAboutDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();

	int result = AfxMessageBox("Are you sure you want to Quit!",MB_YESNO);

	if (result == IDYES)
	{
	  PostQuitMessage(0);
	}
	else
	{
	 ((CTrackEditorApp*)AfxGetApp())->OnAppAbout();
	}
}

CDocument* CTrackEditorApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{
	// TODO: Add your specialized code here and/or call the base class
	CDocument *pDoc = CWinApp::OpenDocumentFile(lpszFileName);
    //pMainFrame->MDITile(MDITILE_HORIZONTAL);

	return pDoc;
}

void CTrackEditorApp::OnWindowTileHorz() 
{
	// TODO: Add your command handler code here
	//CMDIFrameWnd* pMDIFrameWnd = m_pMainWnd->GetMDIFrame();
	//ASSERT(pMDIFrameWnd != NULL);
	 pMainFrame->MDITile(MDITILE_HORIZONTAL);	
}

void CTrackEditorApp::OnWindowTilevertical() 
{
	// TODO: Add your command handler code here
	pMainFrame->MDITile(MDITILE_VERTICAL);

}

void CTrackEditorApp::OnTrackView(CDocument *pDoc)
{
    CFrameWnd* pNewFrame  = pDocTableTemplate->CreateNewFrame(pDoc, NULL);
	pDocTableTemplate->InitialUpdateFrame(pNewFrame, pDoc);
}

void CTrackEditorApp::OnTrackScrollView(CDocument *pDoc)
{
    CFrameWnd* pNewFrame  = pDocTemplate->CreateNewFrame(pDoc, NULL);
	pDocTemplate->InitialUpdateFrame(pNewFrame, pDoc);
}

void CTrackEditorApp::OnPitView(CDocument *pDoc)
{
    CFrameWnd* pNewFrame  = pPitTemplate->CreateNewFrame(pDoc, NULL);
	pPitTemplate->InitialUpdateFrame(pNewFrame, pDoc);
}

void CTrackEditorApp::OnCCView(CDocument *pDoc)
{
    CFrameWnd* pNewFrame  = pCCTemplate->CreateNewFrame(pDoc, NULL);
	pCCTemplate->InitialUpdateFrame(pNewFrame, pDoc);
}

void CTrackEditorApp::OnCameraView(CDocument *pDoc)
{
    CFrameWnd* pNewFrame  = pCameraTemplate->CreateNewFrame(pDoc, NULL);
	pCameraTemplate->InitialUpdateFrame(pNewFrame, pDoc);
}


CObjectView *CTrackEditorApp::OnInternalObjectView(CDocument *pDoc)
{
    CFrameWnd* pNewFrame  = pObjectViewTemplate->CreateNewFrame(pDoc, NULL);
	pObjectViewTemplate->InitialUpdateFrame(pNewFrame, pDoc);
	pDoc->UpdateAllViews(NULL);
	return (CObjectView *)pNewFrame->GetActiveView();
}

void CTrackEditorApp::OnCommandStatsView(CDocument *pDoc)
{
    CFrameWnd* pNewFrame  = pCommandStatsTemplate->CreateNewFrame(pDoc, NULL);
	pCommandStatsTemplate->InitialUpdateFrame(pNewFrame, pDoc);

}






void CTrackEditorApp::OnObjectView(CDocument *pDoc)
{
    CFrameWnd* pNewFrame  = pObjectTemplate->CreateNewFrame(pDoc, NULL);
	pObjectTemplate->InitialUpdateFrame(pNewFrame, pDoc);
}

void CTrackEditorApp::OnWorldView(CDocument *pDoc)
{
    CFrameWnd* pNewFrame  = p3DViewTemplate->CreateNewFrame(pDoc, NULL);
	p3DViewTemplate->InitialUpdateFrame(pNewFrame, pDoc);
}

void CTrackEditorApp::OnTrackglView(CDocument *pDoc)
{
	CFrameWnd* pNewFrame  = pTrackGLViewTemplate->CreateNewFrame(pDoc, NULL);
	pTrackGLViewTemplate->InitialUpdateFrame(pNewFrame, pDoc);
}

void CTrackEditorApp::OnJamView(CDocument *pDoc)
{
    CFrameWnd* pNewFrame  = pJamFileTemplate->CreateNewFrame(pDoc, NULL);
	pJamFileTemplate->InitialUpdateFrame(pNewFrame, pDoc);
}

void CTrackEditorApp::OnInternalView(CDocument *pDoc)
{
    CFrameWnd* pNewFrame  = pInternalFileTemplate->CreateNewFrame(pDoc, NULL);
	pInternalFileTemplate->InitialUpdateFrame(pNewFrame, pDoc);
}



void CTrackEditorApp::OnTree() 
{
	// TODO: Add your command handler code here
	showMainTree = !showMainTree;

	if (pMainFrame)
		{
			if (pMainFrame->m_wndDlgBar)
			{
				if (showMainTree)
					pMainFrame->m_wndDlgBar.ShowWindow(TRUE);
	            else
		            pMainFrame->m_wndDlgBar.ShowWindow(FALSE);

				pMainFrame->RecalcLayout(TRUE);
			}
			else
			{
				AfxMessageBox("No Dialog Bar",MB_OK);
			}
		
	}
	else
	{
		AfxMessageBox("No Main Frame",MB_OK);
	}
}

void CTrackEditorApp::OnUpdateTree(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(showMainTree);
}

void CTrackEditorApp::OnHelpCredits() 
{
	// TODO: Add your command handler code here
	CCredit credit;
	credit.DoModal();
}


void CTrackEditorApp::OnFileNew() 
{
//#ifdef NDEBUG
	// TODO: Add your command handler code here
	CString msg = "The TrackEditor works by editing exiting cirucits only. Simply select a circuit from the GP2/circuits directroy which you wish to use as your base circuit, edit this circuit then resave to the GP2/circuits directory under the original name.\n\n(be sure to backup this circuit before saving)";
	msg += "\nThe TrackEditor assumes you have installed NOCDROM or TracksOnHD (availabe from www.grandprix2.com) and you have already copied your tracks from the GP2 cdrom so that they are on your disk (where they can be modified once they are changed from readonly!)\n";
	msg += "\nTracks must be save at f1ctXX.dat (where XX is a number 01-16, denoting which coutry to replace (01 = Brazil etc..). Also sure to ensure you have backup up your GP2 directory.";

	AfxMessageBox(msg,MB_ICONINFORMATION);
	//OnFileOpen();
/*
	CDocument *pDoc = pDocTemplate->CreateNewDocument();
	pDoc->OnNewDocument();
	CFrameWnd* pNewFrame  = pDocTemplate->CreateNewFrame(pDoc, NULL);
	pDocTemplate->InitialUpdateFrame(pNewFrame, pDoc);
*/
}



void CTrackEditorApp::OnInstallTracks() 
{
	// TODO: Add your command handler code here
	//AfxMessageBox("Not Supported in this release");
	CTrackSelection *trkSel = new CTrackSelection();

	if (trkSel) trkSel->DoModal();


	if (trkSel) delete trkSel;
}

void CTrackEditorApp::OnTableproperties() 
{
	// TODO: Add your command handler code here
	showTableTree = !showTableTree;

	if (pMainFrame)
		{
			if (pMainFrame->m_wndProperty)
			{
				if (showTableTree)
					pMainFrame->m_wndProperty.ShowWindow(TRUE);
	            else
		            pMainFrame->m_wndProperty.ShowWindow(FALSE);

				pMainFrame->RecalcLayout(TRUE);
			}
			else
			{
				AfxMessageBox("No Dialog Bar",MB_OK);
			}
		
	}
	else
	{
		AfxMessageBox("No Main Frame",MB_OK);
	}

}

void CTrackEditorApp::OnUpdateTableproperties(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(showTableTree);
}

void CTrackEditorApp::OnAppExit() 
{

	// TODO: Add your command handler code here
	CString strSection       = "Preferences";

	//RECT rect;
	//pMainFrame->GetWindowRect(&rect);
	//AfxGetApp()->WriteProfileInt(strSection,"SizeLeft",rect.left);
	//AfxGetApp()->WriteProfileInt(strSection,"SizeTop",rect.top);
	//AfxGetApp()->WriteProfileInt(strSection,"SizeRight",rect.right);
	//AfxGetApp()->WriteProfileInt(strSection,"SizeBottom",rect.bottom);

	WINDOWPLACEMENT placement;

	placement.length = sizeof(WINDOWPLACEMENT);
	pMainFrame->GetWindowPlacement(&placement);

	//RECT rect1;
	//RECT rect;
	//pMainFrame->GetWindowRect(&rect1);
	AfxGetApp()->WriteProfileInt(strSection,"SizeFlags",placement.flags);
	AfxGetApp()->WriteProfileInt(strSection,"SizeFlags",placement.showCmd);
	AfxGetApp()->WriteProfileInt(strSection,"SizeptMinPosX",placement.ptMinPosition.x);
	AfxGetApp()->WriteProfileInt(strSection,"SizeptMinPosY",placement.ptMinPosition.y);
	AfxGetApp()->WriteProfileInt(strSection,"SizeptMaxPosX",placement.ptMaxPosition.x);
	AfxGetApp()->WriteProfileInt(strSection,"SizeptMaxPosY",placement.ptMaxPosition.y);	
    AfxGetApp()->WriteProfileInt(strSection,"SizeLeft",placement.rcNormalPosition.left);
	AfxGetApp()->WriteProfileInt(strSection,"SizeTop",placement.rcNormalPosition.top);
	AfxGetApp()->WriteProfileInt(strSection,"SizeRight",placement.rcNormalPosition.right);
	AfxGetApp()->WriteProfileInt(strSection,"SizeBottom",placement.rcNormalPosition.bottom);




	WR_PROFILE(showMainTree)
	WR_PROFILE(showTableTree)
	WR_PROFILE(showImageInTable)
	WR_PROFILE(showJamNumbers)
	WR_PROFILE(showToolsToolbar)
	WR_PROFILE(showViewToolbar)
	WR_PROFILE(showObjectToolbar)
	WR_PROFILE(showReadObjNames)
	WR_PROFILE(showToolbar)
	


	pMainFrame->SavePositions();

	if (pImageList!=NULL)
	{
		delete pImageList;
		pImageList = NULL;
	}

	CWinApp::OnAppExit();
}

#define IDB_MIN  IDB_CIRCUIT

void CTrackEditorApp::LoadImages()
{
	if (pImageList!=NULL) delete pImageList;

	pImageList = new CImageList();

	int size = (IDB_MAX - IDB_MIN);
	pImageList->Create(16, 15, TRUE, size, 2);	

	CBitmap bitmap;
	for (UINT iCnt=IDB_MIN; iCnt<=IDB_MAX; iCnt++)
	{
		bitmap.LoadBitmap(iCnt);
		pImageList->Add(&bitmap, (COLORREF)0xFFFFFF);
		bitmap.DeleteObject();
	}	
}



void CTrackEditorApp::OnRegister() 
{
	// TODO: Add your command handler code here
    CRegister *reg = new CRegister();
	reg->DoModal();
	if (reg) delete reg;
}

void CTrackEditorApp::OnCommandIndex() 
{
	// TODO: Add your command handler code here
	CCmdList *cmdlist = new CCmdList();

	cmdlist->DoModal();
}

void CAboutDlg::OnRegister() 
{
	// TODO: Add your control notification handler code here
	theApp.OnRegister();
}

void CTrackEditorApp::OnViewToolsToolbar() 
{
	// TODO: Add your command handler code here
	showToolsToolbar = !showToolsToolbar;


	pMainFrame->m_wndToolsToolBar.ShowWindow(showToolsToolbar?SW_SHOW:SW_HIDE);
	pMainFrame->RecalcLayout(TRUE);
}

void CTrackEditorApp::OnUpdateViewToolsToolbar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(showToolsToolbar);
}

static char lpszTrackFileFilter[] = "F1GP2 Track Files (*.dat)|.dat|";


void CTrackEditorApp::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CString strSection       = "Preferences";
    CString strStringItem    = "GP2TrackLoadLocation";
	CWinApp* pApp = AfxGetApp();

	CString dirName = pApp->GetProfileString(strSection, strStringItem,"c:\\");
	//SetCurrentDirectory(dirName);

	//CWinApp::OnFileOpen();
	char fileName[256];

	strcpy(fileName,"*.dat");
	CFileDialog *fdlg = new CFileDialog(TRUE,"txt",(LPCSTR)&fileName,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,lpszTrackFileFilter);
	
	int result = fdlg->DoModal();

	if (result == IDOK)
	{
	  AfxGetApp()->OpenDocumentFile(fdlg->GetFileName());
	}

}

void CTrackEditorApp::OnEditLocation() 
{
	char fileName[256];

	strcpy(fileName,"*.*");
	
	BROWSEINFO browser;
	char buffer[MAX_PATH];

	browser.hwndOwner = NULL;
	browser.pszDisplayName = buffer;
	browser.lpszTitle = "Select GP2 Location";
	browser.lpfn = NULL;
	browser.pidlRoot = NULL;
	browser.ulFlags = BIF_RETURNONLYFSDIRS;

	LPITEMIDLIST files;

	files = SHBrowseForFolder(&browser);

	if (files !=NULL)
	{
		char filename[256];

		if (SHGetPathFromIDList(files,filename))
		{
		 CString strSection       = "Preferences";
         CString strStringItem    = "GP2Location";
         CWinApp* pApp = AfxGetApp();

		 pApp->WriteProfileString(strSection, strStringItem, CString(filename));
		}
		else
		{
	     AfxMessageBox("Failed to Locate GP2 directory");
		}
   	}
	else
	{
		AfxMessageBox("Failed to find GP2 directory");
	}
}

static char lpszObjOffFilter[] = "Offset - Object Files (*.txt)|*.txt";

extern void ImportObjectDefinitions(CString name);

void CTrackEditorApp::OnImportNames() 
{
	char fileName[256];

	strcpy(fileName,"unnamed.txt");
	CFileDialog *fdlg = new CFileDialog(TRUE,"txt",(LPCSTR)&fileName,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,lpszObjOffFilter);
	
	int result = fdlg->DoModal();

	if (result != IDCANCEL)
	{
	   CString filename = fdlg->GetFileName();
	   ImportObjectDefinitions(filename);
	   cleverNames=TRUE;
	}

	delete fdlg;	
}

void CTrackEditorApp::OnGp2exe() 
{
	static char buffer[512];
	char   workingDir[512];
	CString gp2Location = ((CTrackEditorApp*)AfxGetApp())->getGP2Location();

	GetCurrentDirectory(512,workingDir);

	SetCurrentDirectory(gp2Location);
	wsprintf(buffer,"%s\\gp2.exe",gp2Location);
		  
	WinExec(buffer,SW_SHOW);
	SetCurrentDirectory(workingDir);
}

void CTrackEditorApp::OnTableImages() 
{
	// TODO: Add your command handler code here
	showImageInTable = ! showImageInTable;
}

void CTrackEditorApp::OnUpdateTableImages(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(showImageInTable);
}

void CTrackEditorApp::OnShowjamNumbers() 
{
	showJamNumbers = ! showJamNumbers;

	if (pMainFrame)
	{
	 CDocument *pDoc = pMainFrame->GetActiveDocument();
	 if (pDoc)
	 {
	  pDoc->UpdateAllViews(NULL);
	 }
	}
}

void CTrackEditorApp::OnUpdateShowjamNumbers(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(showJamNumbers);
}


void CTrackEditorApp::OnViewViewtoolbar() 
{
	// TODO: Add your command handler code here
	showViewToolbar = !showViewToolbar;


	pMainFrame->m_wndShowToolBar.ShowWindow(showViewToolbar?SW_SHOW:SW_HIDE);
	pMainFrame->RecalcLayout(TRUE);
	if (!pMainFrame->m_wndShowToolBar.IsVisible()){
		pMainFrame->DockControlBar(&pMainFrame->m_wndShowToolBar);
	}
}

void CTrackEditorApp::OnUpdateViewViewtoolbar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(showViewToolbar);
}


void SaveControlBarPosition(CFrameWnd *frame,CControlBar *cntrl,CString strStringItem)
{
    CString strSection       = strStringItem;    
	CWinApp* pApp = AfxGetApp();

	//((CToolBar *)cntrl)->GetToolBarCtrl().SaveState(HKEY_CURRENT_USER,"Software\\GP2TrackEditorProject\\TrackEditor",strSection);

	int style = cntrl->GetBarStyle();
	BOOL floating = cntrl->IsFloating(); 
	
	pApp->WriteProfileInt(strSection, "isFloating",floating);
    pApp->WriteProfileInt(strSection, "Style",style);

	if (floating)
	{
		RECT rect;
		cntrl->GetWindowRect(&rect);

		pApp->WriteProfileInt(strSection, "X",rect.left);
		pApp->WriteProfileInt(strSection, "Y",rect.top);
		pApp->WriteProfileInt(strSection, "Width",rect.right-rect.left);
		pApp->WriteProfileInt(strSection, "Height",rect.bottom-rect.top);
	}
	else
	{
		RECT rect;
		cntrl->GetClientRect(&rect);
		cntrl->MapWindowPoints(frame,&rect);

		pApp->WriteProfileInt(strSection, "X",rect.left-14);
		pApp->WriteProfileInt(strSection, "Y",rect.top);
		pApp->WriteProfileInt(strSection, "Width",rect.right-rect.left);
		pApp->WriteProfileInt(strSection, "Height",rect.bottom-rect.top);
		
	}

}

void LoadControlBarPosition(CFrameWnd *frame,CControlBar *cntrl,CString strStringItem)
{
    CString strSection       = strStringItem;    
	CWinApp* pApp = AfxGetApp();
	
	
	//((CToolBar *)cntrl)->GetToolBarCtrl().RestoreState(HKEY_CURRENT_USER,"Software\\GP2TrackEditorProject\\TrackEditor",strSection);
	//((CToolBarCtrl *)cntrl)->AutoSize();


	BOOL floating = pApp->GetProfileInt(strSection, "isFloating",FALSE);
	int style = pApp->GetProfileInt(strSection, "Style",-1);

	if (style!=-1)
	{
	  //cntrl->SetBarStyle(style);
	}

	if (floating)
	{
		CPoint pt;
		pt.x = pApp->GetProfileInt(strSection, "X",0);
		pt.y = pApp->GetProfileInt(strSection, "Y",0);
		int width = pApp->GetProfileInt(strSection, "Width",-1);
		int height = pApp->GetProfileInt(strSection, "Height",-1);

		

		frame->FloatControlBar(cntrl,pt);

		if (width > 5 && height > 5)
		{
		  CFrameWnd *wnd = cntrl->GetDockingFrame();
		  //wnd->SetWindowPos(NULL,0,0,width+5,height+20,SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
		  //cntrl->SetWindowPos(NULL,pt.x,pt.y,width,height,SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
		  //cntrl->GetParent()->SetWindowPos(NULL,pt.x,pt.y,width,height,SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
		  //((CToolBar *)cntrl)->SetHeight(height);
		  //((CToolBar *)cntrl)->GetToolBarCtrl().SetRows((int)(height/20),TRUE,NULL);
		  //((CToolBar *)cntrl)->GetToolBarCtrl().AutoSize();
		  wnd->RecalcLayout(TRUE);
		}
		//((CToolBarCtrl *)cntrl)->AutoSize();

		frame->RecalcLayout(TRUE);
	}
	else
	{
		RECT rect;
		rect.left   = pApp->GetProfileInt(strSection, "X",0);
		rect.top    = pApp->GetProfileInt(strSection, "Y",0);

		if (rect.left == 0 && rect.top==0)
		{
		  frame->DockControlBar(cntrl);
		}
		else
		{
		rect.right  = rect.left + pApp->GetProfileInt(strSection, "Width",0);
		rect.bottom = rect.top  + pApp->GetProfileInt(strSection, "Height",0);
		frame->ClientToScreen(&rect);

		frame->DockControlBar(cntrl,(UINT)0,(LPCRECT)&rect);
		}
		//frame->RecalcLayout(TRUE);
	}
}


void CTrackEditorApp::OnHelp() 
{
	// TODO: Add your command handler code here
	// WinHelp(0,HELP_CONTENTS);
	int useHelp = GetProfileInt("Settings","UseHtmlHelp",0);

	if (useHelp == 0){
		int result = AfxMessageBox("Would you like to enable HtmlHelp in this application!",MB_YESNO);
		if (result == IDYES){
			WriteProfileInt("Settings","UseHtmlHelp",1);
			useHelp = 1;
		}
		else {
			WriteProfileInt("Settings","UseHtmlHelp",0);
			useHelp = 0;
		}
	}

	if (useHelp){
		OnStartHelp(FALSE);
	}
}

void CAboutDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CTrackEditorApp::OnHelpFinder() 
{
	int useHelp = GetProfileInt("Settings","UseHtmlHelp",0);

	if (useHelp == 0){
		int result = AfxMessageBox("Would you like to enable HtmlHelp in this application!",MB_YESNO);
		if (result == IDYES){
			WriteProfileInt("Settings","UseHtmlHelp",1);
			useHelp = 1;
		}
		else {
			WriteProfileInt("Settings","UseHtmlHelp",0);
			useHelp = 0;
		}
	}

	if (useHelp){
		OnStartHelp(FALSE);
	}
}

void CTrackEditorApp::OnStartHelp(BOOL reposition)
{
	RECT rect;
	CRect Wrect;
	CString helppath ="TrackEditor.chm";

	char szModuleName[260];
	GetModuleFileName(GetModuleHandle(NULL),szModuleName,sizeof(szModuleName));
	char *pos = strrchr(szModuleName,'\\');
	if (pos) 
	{
		*pos='\0';
		helppath = szModuleName;
		helppath += "\\TrackEditor.chm";
	}

	AfxGetMainWnd()->GetWindowRect(&Wrect);

	rect.left = Wrect.left + Wrect.Width();
	rect.top = Wrect.top;
	rect.right = rect.left+500;
	rect.bottom = Wrect.top+Wrect.Height();

	HH_WINTYPE typeInfo;

	CString  m_TypeName = "TrackEditor.chm";
	CString tempType = ">";
    CString prefix = "mk:@MSITStore:";
    CString tempToc = "";
    CString tempIndex = "";
    CString tempStart = "";
    CString tempHome = "";
	CString m_chmUrl = helppath;
	CString	m_tocUrl = "TrackEditor.hhc";
	CString	m_indexUrl = "TrackEditor.hhc";
	CString	m_startUrl = "index.htm";
	CString	m_homeUrl = "index.htm";

	prefix += m_chmUrl;
    prefix += "::/";

    tempToc = prefix;
    tempToc += m_tocUrl;
    typeInfo.pszToc = tempToc;

    tempIndex = prefix;
    tempIndex += m_indexUrl;
    typeInfo.pszIndex = tempIndex;

    tempStart = prefix;
    tempStart += m_startUrl;
    typeInfo.pszFile = tempStart;

    tempHome = prefix;
    tempHome += m_homeUrl;
    typeInfo.pszHome = tempHome;

	CString TypeName = 	"trackeditor";

	typeInfo.fUniCodeStrings = FALSE;
    typeInfo.pszType = TypeName;
    typeInfo.pszCaption = "Track Editor Help File";
	
    typeInfo.fsValidMembers = HHWIN_PARAM_PROPERTIES | HHWIN_PARAM_TABPOS |
                                                                HHWIN_PARAM_SHOWSTATE | HHWIN_PARAM_TB_FLAGS;
                                                             
	if (reposition) typeInfo.fsValidMembers |= (HHWIN_PARAM_NAV_WIDTH | HHWIN_PARAM_RECT);
	
    typeInfo.fsWinProperties = 0;
	typeInfo.fsWinProperties = HHWIN_PROP_TRI_PANE;
	typeInfo.fNotExpanded = FALSE;
   	typeInfo.curNavType = HHWIN_NAVTYPE_INDEX;
    typeInfo.tabpos = HHWIN_NAVTAB_TOP;        
	typeInfo.nShowState = SW_SHOW;
    typeInfo.hwndCaller = AfxGetMainWnd()->m_hWnd;
    typeInfo.iNavWidth = 200;
	typeInfo.cbStruct = sizeof(typeInfo);
	typeInfo.fsToolBarFlags = 0;
    typeInfo.fsToolBarFlags |= HHWIN_BUTTON_EXPAND;
    typeInfo.fsToolBarFlags |= HHWIN_BUTTON_BACK;
    typeInfo.fsToolBarFlags |= HHWIN_BUTTON_FORWARD;
    typeInfo.fsToolBarFlags |= HHWIN_BUTTON_STOP;
    typeInfo.fsToolBarFlags |= HHWIN_BUTTON_REFRESH;
    typeInfo.fsToolBarFlags |= HHWIN_BUTTON_HOME;
    typeInfo.fsToolBarFlags |= HHWIN_BUTTON_HISTORY;	
    typeInfo.fsToolBarFlags |= HHWIN_BUTTON_INDEX;
	typeInfo.rcWindowPos = rect;

    HtmlHelpFunc( AfxGetMainWnd()->m_hWnd,NULL, HH_SET_WIN_TYPE,(DWORD) (&typeInfo));

	tempType += TypeName;

	HtmlHelpFunc(AfxGetMainWnd()->m_hWnd,tempType,HH_DISPLAY_TOPIC,NULL);

}

void CTrackEditorApp::OnHelpContentString(CString url)
{
	int useHelp = GetProfileInt("Settings","UseHtmlHelp",0);

	if (useHelp == 0){
		int result = AfxMessageBox("Would you like to enable HtmlHelp in this application!",MB_YESNO);
		if (result == IDYES){
			WriteProfileInt("Settings","UseHtmlHelp",1);
			useHelp = 1;
		}
		else {
			WriteProfileInt("Settings","UseHtmlHelp",0);
			useHelp = 0;
			return;
		}
	}

	if (useHelp){
		OnStartHelp(FALSE);
	}
	else {
		return;
	}

	CString helppath ="TrackEditor.chm";

	char szModuleName[260];
	GetModuleFileName(GetModuleHandle(NULL),szModuleName,sizeof(szModuleName));
	char *pos = strrchr(szModuleName,'\\');
	if (pos) 
	{
		*pos='\0';
		helppath = szModuleName;
		helppath += "\\TrackEditor.chm";
	}
	
	CString m_chmUrl = helppath;
	CString TypeName = 	"trackeditor";
	CString tempType = ">";
	tempType += TypeName;
	CString prefix = "mk:@MSITStore:";
	prefix += m_chmUrl;
    prefix += "::/";
	prefix += url;
	HtmlHelpFunc(AfxGetMainWnd()->m_hWnd,tempType,HH_DISPLAY_TOPIC,(DWORD)(LPCSTR)prefix);
}

typedef void (*HTMLHELPPROC)(HWND caller,LPCSTR filename,UINT cmd,DWORD dwData);

void HtmlHelpFunc(HWND caller,LPCSTR filename,UINT cmd,DWORD dwData)
{
	static HINSTANCE hLib = LoadLibrary("hhctrl.ocx");

	if (hLib!=NULL)
	{
		FARPROC lpfnProc;
		lpfnProc = GetProcAddress(hLib,"HtmlHelpA");

		if (lpfnProc)
		{
			HTMLHELPPROC helpFn = (HTMLHELPPROC)lpfnProc;
			helpFn(caller,filename,cmd,dwData);
		}
		else
		{
			AfxMessageBox("TrackEditor ERROR: Cannot Find HtmlHelp Functionality",MB_OK);
		}
		//FreeLibrary(hLib);
	}
	else
	{
		AfxMessageBox("Cannot Use HtmlHelp Functionality Can't find \"hhctl.ocx\"");
	}
}

void CTrackEditorApp::OnViewObjecttoolbar() 
{
	showObjectToolbar = !showObjectToolbar;

	pMainFrame->m_wndObjectToolBar.ShowWindow(showObjectToolbar?SW_SHOW:SW_HIDE);
	pMainFrame->RecalcLayout(TRUE);
}

void CTrackEditorApp::OnUpdateViewObjecttoolbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(showObjectToolbar);
}

void CTrackEditorApp::OnViewReadobjnames() 
{
	CString strSection       = "Preferences";
	showReadObjNames = !showReadObjNames;
	WR_PROFILE(showReadObjNames)
}

void CTrackEditorApp::OnUpdateViewReadobjnames(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(showReadObjNames);	
}

/*
#include <afxinet.h>

BOOL GetVersionPage(int &Major,int &Minor,int &Debug)
{
	//this code excerpt also demonstrates how to use //try/catch exception handling
	//assumes server, port and URL names have been initialized
	CInternetSession session("My Session");
	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;
	DWORD dwRet;
	char szBuff[1024];
	
	try{   
			CString strServerName="members.xoom.com"; 
			CString strObject="/paulhoad/ver.htm";
			INTERNET_PORT nPort=80;
			pServer = session.GetHttpConnection(strServerName);
			pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject);
			//pFile->AddRequestHeaders(szHeaders);   
			pFile->SendRequest();
			pFile->QueryInfoStatusCode(dwRet);   
			
			if (dwRet == HTTP_STATUS_OK)   {
				UINT nRead = pFile->Read(szBuff, 1023);       
				while (nRead > 0)  {     
					sscanf(szBuff,"%d\n%d\n%d\n",&Major,&Minor,&Debug);
					nRead = 0;
				}   
			}   
			delete pFile;   
			delete pServer;
		}
		catch (CInternetException* ){   
		    //catch errors from WinInet
			//psz->ReportError();
			AfxMessageBox("Unable to check online connection error");
			session.Close();
			return FALSE;
		}
		session.Close();
	
		return TRUE;
}
*/

BOOL CTrackEditorApp::CheckCurrentEditorVersion()
{
   CFileVersion fileVersion;
   // get the name of the programme
   char ModuleName[256];
   GetModuleFileName(0,ModuleName,256);

   fileVersion.Open(ModuleName);
   CString versionNumber = fileVersion.GetProductVersion();
   CString buff;
   int Major,Minor,Debug;
   Major = 0;
   Minor = 0;
   Debug = 0;
   int cmonth = GetProfileInt("Upgrade","CheckMonth",-1);
   int cday = GetProfileInt("Upgrade","CheckDay",-1);
   int timeChecked = GetProfileInt("Upgrade","CheckTimes",-1);
   timeChecked++;
   WriteProfileInt("Upgrade","CheckTimes",timeChecked);
   CTime t = CTime::GetCurrentTime();
   int month = t.GetMonth();
   int day   = t.GetDay();
   if (cmonth != month || cday != day || timeChecked >= 10)
   {
	 WriteProfileInt("Upgrade","CheckMonth",month);
	 WriteProfileInt("Upgrade","CheckDay",day);
	 timeChecked = 0;
	 WriteProfileInt("Upgrade","CheckTimes",timeChecked);
	 //cerr << "Checking with online version..." << endl;
	 /*
     if (GetVersionPage(Major,Minor,Debug))
	 {
	   int PMajor,PMinor,PDebug;
	   sscanf(versionNumber,"%d.%d.%d",&PMajor,&PMinor,&PDebug);
	   BOOL upgrade = FALSE;

	   if (PMajor <= Major) 
	   {
		  if (PMinor <= PMinor) 
		  {			  
				if (PDebug < Debug) 
				{
					upgrade = TRUE;
				}
		  }
	   }

	   if (upgrade)
	   {
		   CString ProVer;
		   CString CurVer;
		   ProVer.Format(" (Version %d.%d.%d)",PMajor,PMinor,PDebug);
		   CurVer.Format(" (Version %d.%d.%d)",Major,Minor,Debug);
		   CString msg;
		   msg += "Your version ";
		   msg += ProVer;
		   msg += " of the GP2 Editor is out of date.";
		   msg += CurVer;
		   msg += " has now been released.";
		   msg += " Shall I open Your browser so you can download the next version?";
		   int result = AfxMessageBox(msg,MB_YESNO);
		   if (result == IDYES)
		   {
			   ShellExecute(NULL,"open", "http://members.xoom.com/paulhoad/index.html",NULL, NULL, SW_SHOWNORMAL);
		   }
		   else
		   {
			   AfxMessageBox("You have an out of date version");
		   }
	   }
	   else
	   {
		   //cerr << "You have an upto date version" << endl;
	   }
	 }
	 */
     return TRUE;
   }
   else
   {
	   //cerr << "Version check upto date [" << timeChecked << "]" << endl;
	   return FALSE;
   }
}

void CTrackEditorApp::OnEditAutobackup() 
{
	CString strSection       = "Preferences";
	AutoBackup = !AutoBackup;
	WR_PROFILE(AutoBackup)
}

void CTrackEditorApp::OnUpdateEditAutobackup(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(AutoBackup);	
}

void CTrackEditorApp::OnFileOpenF1gp() 
{
	// TODO: Add your command handler code here
	CString strSection       = "Preferences";
    CString strStringItem    = "GP2TrackLoadLocation";
	CWinApp* pApp = AfxGetApp();

	CString dirName = pApp->GetProfileString(strSection, strStringItem,"c:\\");
	//SetCurrentDirectory(dirName);

	//CWinApp::OnFileOpen();
	char fileName[256];

	strcpy(fileName,"*.dat");
	CFileDialog *fdlg = new CFileDialog(TRUE,"txt",(LPCSTR)&fileName,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,lpszTrackFileFilter);
	
	int result = fdlg->DoModal();

	if (result == IDOK)
	{
	  isF1GP = true;
	  AfxGetApp()->OpenDocumentFile(fdlg->GetFileName());
	  isF1GP = false;
	}
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	LOGFONT lf;                        // Used to create the CFont.
	CFont   m_font;
   
    memset(&lf, 0, sizeof(LOGFONT));   // Clear out structure.
    lf.lfHeight = 11;                  // Request a 20-pixel-high font
    strcpy(lf.lfFaceName, "MS San Serif");    //    with face name "Arial".
    m_font.CreateFontIndirect(&lf);    // Create the font.
 
    // Use the font to paint a control. This code assumes
    // a control named IDC_TEXT1 in the dialog box.
    //GetDlgItem(IDC_LEGAL1)->SetFont(&m_font);
    //GetDlgItem(IDC_LEGAL2)->SetFont(&m_font);
	//GetDlgItem(IDC_LEGAL3)->SetFont(&m_font);
	//GetDlgItem(IDC_LEGAL4)->SetFont(&m_font);
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

bool CTrackEditorApp::IsLandScape()
{
	bool orientation = false;
	// DMORIENT_PORTRAIT 
	// DMORIENT_LANDSCAPE
    // Get default printer settings.
    PRINTDLG   pd;

    pd.lStructSize = (DWORD) sizeof(PRINTDLG);
    if (GetPrinterDeviceDefaults(&pd))
    {
        // Lock memory handle.
        DEVMODE FAR* pDevMode =
            (DEVMODE FAR*)::GlobalLock(m_hDevMode);
        LPDEVNAMES lpDevNames;
        LPTSTR lpszDriverName, lpszDeviceName, lpszPortName;
        HANDLE hPrinter;

        if (pDevMode)
        {
           // Change printer settings in here.
           // pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
           // Unlock memory handle.
           lpDevNames = (LPDEVNAMES)GlobalLock(pd.hDevNames);
           lpszDriverName = (LPTSTR )lpDevNames + lpDevNames->wDriverOffset;
           lpszDeviceName = (LPTSTR )lpDevNames + lpDevNames->wDeviceOffset;
           lpszPortName   = (LPTSTR )lpDevNames + lpDevNames->wOutputOffset;

           ::OpenPrinter(lpszDeviceName, &hPrinter, NULL);
           ::DeviceCapabilities(lpszDeviceName,lpszPortName,
                                DC_ORIENTATION, 0L, pDevMode);

		   if (pDevMode->dmOrientation == DMORIENT_LANDSCAPE){
			   orientation = true;
		   }
		   else {
			   orientation = false;
		   }

          // Sync the pDevMode.
          // See SDK help for DocumentProperties for more info.
          ::ClosePrinter(hPrinter);
          ::GlobalUnlock(m_hDevNames);
          ::GlobalUnlock(m_hDevMode);
       }
    }
	return orientation;
}

void CTrackEditorApp::ShowTipAtStartup(void)
{
	// CG: This function added by 'Tip of the Day' component.

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (cmdInfo.m_bShowSplash)
	{
		CTipDlg dlg;
		if (dlg.m_bStartup)
			dlg.DoModal();
	}

}

void CTrackEditorApp::ShowTipOfTheDay(void)
{
	// CG: This function added by 'Tip of the Day' component.

	CTipDlg dlg;
	dlg.DoModal();

}

BOOL CTrackEditorApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following lines were added by the Splash Screen component.
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}

void CAboutDlg::OnForumSite() 
{
	// TODO: Add your control notification handler code here
	CString linkText = "http://www.mircx.com/cgi-bin/forum.cgi?forum=Tracked";
    HINSTANCE h = ShellExecute(NULL, (LPCTSTR)"open", (LPCSTR)linkText,
                      NULL, NULL, SW_SHOWNORMAL);

    if ((UINT)h > 32) {
          // visited
    } else {
          MessageBeep(0);          // unable to execute file!
    }
}

void CAboutDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rcItem(lpDrawItemStruct->rcItem);

	pDC->SetTextColor(RGB(0x00,0x00,0xFF));
	CString str;
	GetDlgItemText(nIDCtl,str);
	pDC->TextOut(rcItem.left,rcItem.top,str);
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}


static visitURL(LPCSTR linkText)
{
    HINSTANCE h = ShellExecute(NULL, (LPCTSTR)"open", (LPCSTR)linkText,
                      NULL, NULL, SW_SHOWNORMAL);

    if ((UINT)h > 32) {
          // visited
    } else {
          MessageBeep(0);          // unable to execute file!
    }
}

void CAboutDlg::OnButton3() 
{
	visitURL("http://www.grandprix2.com/trackedit");
}

void CAboutDlg::OnButtonXoom() 
{
	visitURL("http://members.xoom.com/paulhoad");
}

void CAboutDlg::OnButtonGrandPrix2() 
{
	visitURL("http://www.grandprix2.com");
}

void CTrackEditorApp::OnHelpQuickkeyreminder() 
{
	CString msgA;
	CString msg;

	msg.Format("Quick Key Reminder\n=======================\n");
	msgA+=msg;
	msg.Format("In Track/Pitlane/CCline Tools\n");
	msgA+=msg;
	msgA+="========================\n";
	msgA+="A     - next     (track/pit/cc) section\n";
	msgA+="Z     - previous (track/pit/cc) section\n";
	msgA+="Up    - lengthen (track/pit/cc) section\n";
	msgA+="Down  - shorten (track/pit/cc) section\n";
	msgA+="Left  - turn left (track/pit/cc) section\n";
	msgA+="Right - turn right (track/pit/cc) section\n";
	msgA+="S     - Striaghten section\n";
	msgA+="O & P Move Right Fence in/out\n";
	msgA+="9 & 0 Move Left  Fence in/out\n";
	msgA+="========================\n";
	msg.Format("In Object Tools\n");
	msgA+=msg;
	msgA+="========================\n";
	msgA+="Up    - move object out\n";
	msgA+="Down  - move object in\n";
	msgA+="Left  - Move object forward \n";
	msgA+="Right - Move object back\n";
	


	AfxMessageBox(msgA,MB_ICONINFORMATION);
}

void CTrackEditorApp::OnViewToolbar() 
{
	showToolbar = !showToolbar;

	pMainFrame->m_wndToolBar.ShowWindow(showToolbar?SW_SHOW:SW_HIDE);
	pMainFrame->RecalcLayout(TRUE);
}



void CTrackEditorApp::OnUpdateViewToolbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(showToolbar);
}
