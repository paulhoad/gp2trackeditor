// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TrackEditor.h"

#include "MainFrm.h"
#include "ToolBarUtils.h"

#include "TrackInfo.h"
#include "TrackPropertySheet.h"
#include "Splash.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
extern CTrackEditorApp theApp;

extern void LoadControlBarPosition(CFrameWnd *frame,CControlBar *cntrl,CString strStringItem);
extern void SaveControlBarPosition(CFrameWnd *frame,CControlBar *cntrl,CString strStringItem);

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_INITMENU()
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(IDC_SAVE_TRACK_IMAGE, OnSaveTrackImage)
	ON_COMMAND(ID_ZOOMCOMBO, OnZoomcombo)	
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
	//ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	//ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	//ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)


static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::SavePositions()
{
	//SaveControlBarPosition(this,&m_wndToolBar,"Track_Toolbar");
	//SaveControlBarPosition(this,&m_wndToolsToolBar,"Tools_Toolbar");
	//SaveControlBarPosition(this,&m_wndShowToolBar,"View_Toolbar");
	//SaveControlBarPosition(this,&m_wndObjectToolBar,"Object_Toolbar");

	

}

#define SNAP_WIDTH 80 

#define PROFILE(x,y,b) AfxGetApp()->GetProfileInt(CString(#y), CString(#x),b);

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	
	
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this,TBSTYLE_FLAT| TBSTYLE_TRANSPARENT,WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP,CRect(0, 0, 0, 0),IDR_MAINFRAME) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndToolsToolBar.CreateEx(this,TBSTYLE_FLAT| TBSTYLE_TRANSPARENT,WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP,CRect(0, 0, 0, 0),IDR_TOOLS) ||
		!m_wndToolsToolBar.LoadToolBar(IDR_TOOLS))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	
	if (!m_wndObjectToolBar.CreateEx(this,TBSTYLE_FLAT| TBSTYLE_TRANSPARENT,WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP,CRect(0, 0, 0, 0),IDR_OBJECT) ||
		!m_wndObjectToolBar.LoadToolBar(IDR_OBJECT))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndShowToolBar.CreateEx(this,TBSTYLE_FLAT| TBSTYLE_TRANSPARENT,WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP,CRect(0, 0, 0, 0),IDR_SHOW) ||
		!m_wndShowToolBar.LoadToolBar(IDR_SHOW))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_GRIPPER | CBRS_BORDER_3D | CBRS_TOOLTIPS  |  CBRS_FLYBY | CBRS_SIZE_DYNAMIC | TBSTYLE_WRAPABLE);
	m_wndToolBar.SetHeight(28);

	m_wndToolsToolBar.SetBarStyle(m_wndToolsToolBar.GetBarStyle() |
		CBRS_GRIPPER | CBRS_BORDER_3D | CBRS_TOOLTIPS |  CBRS_FLYBY | CBRS_SIZE_DYNAMIC | TBSTYLE_WRAPABLE);
	m_wndToolsToolBar.SetHeight(28);

	
	m_wndObjectToolBar.SetBarStyle(m_wndObjectToolBar.GetBarStyle() |
		CBRS_GRIPPER | CBRS_BORDER_3D | CBRS_TOOLTIPS |  CBRS_FLYBY | CBRS_SIZE_DYNAMIC | TBSTYLE_WRAPABLE);
	m_wndObjectToolBar.SetHeight(28);

	m_wndShowToolBar.SetBarStyle(m_wndShowToolBar.GetBarStyle() |
		CBRS_GRIPPER | CBRS_BORDER_3D | CBRS_TOOLTIPS |  CBRS_FLYBY | CBRS_SIZE_DYNAMIC | TBSTYLE_WRAPABLE);
	m_wndShowToolBar.SetHeight(28);


	



	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndShowToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndObjectToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolsToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndToolsToolBar);
	DockControlBar(&m_wndObjectToolBar);
	DockControlBar(&m_wndShowToolBar);

	

	int TreeHeight = PROFILE(TreeHeight,Preferences,10);
	int TreeWidth  = PROFILE(TreeWidth,Preferences,110);

	if (!m_wndDlgBar.Create(this, CSize(TreeWidth,TreeHeight),AFX_IDW_CONTROLBAR_FIRST + 32,WS_CHILD | WS_VISIBLE | CBRS_ALIGN_LEFT))
	{
		AfxMessageBox("Failed to create DlgBar",MB_OK);
		return -1;      // fail to create
	}
	m_wndDlgBar.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	DockControlBar(&m_wndDlgBar);

	theApp.theMainTree = &m_wndDlgBar;

	int PropHeight = PROFILE(PropHeight,Preferences,100);
	int PropWidth = PROFILE(PropWidth,Preferences,200);

	if (!m_wndProperty.Create(this, CSize(PropWidth,PropHeight),AFX_IDW_CONTROLBAR_FIRST + 33, LVS_REPORT | WS_CHILD | WS_VISIBLE | CBRS_ALIGN_BOTTOM))
	{
		AfxMessageBox("Failed to create DlgBar",MB_OK);
		return -1;      // fail to create
	}
	m_wndProperty.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndProperty);

	m_listControlBars.AddTail(&m_wndDlgBar);
	m_listControlBars.AddTail(&m_wndProperty);

	theApp.theMainPropertyTable = &m_wndProperty;
	EnableDocking(CBRS_ALIGN_ANY);

	if (IsIE4Capable()){
		LoadBarState("ToolBars");
	}
	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);
	return 0;
}

void CMainFrame::RecalcLayout(BOOL bNotify)
{
	CMDIFrameWnd::RecalcLayout(bNotify);
	CMDIFrameWnd::RecalcLayout(bNotify);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS,
		AfxGetApp()->LoadStandardCursor(IDC_ARROW), (HBRUSH)(COLOR_WINDOW+1),
		AfxGetApp()->LoadIcon(IDR_MAINFRAME));

	return CMDIFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers



void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIFrameWnd::OnSize(nType, cx, cy);

	RecalcLayout(TRUE);

	

	//m_wndDlgBar.SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE);
	// TODO: Add your message handler code here
	
}

void CMainFrame::OnSaveTrackImage() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnZoomcombo() 
{
	// TODO: Add your command handler code here
	AfxMessageBox("Combo");
}

void CMainFrame::OnClose() 
{
	// Save main window position
    CWinApp* app = AfxGetApp();
    WINDOWPLACEMENT wp;
    GetWindowPlacement(&wp);
    app->WriteProfileInt("Frame", "Status", wp.showCmd);
    app->WriteProfileInt("Frame", "Top",    wp.rcNormalPosition.top);
    app->WriteProfileInt("Frame", "Left",   wp.rcNormalPosition.left);
    app->WriteProfileInt("Frame", "Bottom", wp.rcNormalPosition.bottom);
    app->WriteProfileInt("Frame", "Right",  wp.rcNormalPosition.right);

	SaveToolBarPositions();

	CMDIFrameWnd::OnClose();
}

void CMainFrame::SaveToolBarPositions()
{
		if (IsIE4Capable()){
			SaveBarState("ToolBars");
		}
}

void CMainFrame::OnInitMenu(CMenu* pMenu)
{
   CMDIFrameWnd::OnInitMenu(pMenu);
  
	// CG: This block added by 'Tip of the Day' component.
	{
		// TODO: This code adds the "Tip of the Day" menu item
		// on the fly.  It may be removed after adding the menu
		// item to all applicable menu items using the resource
		// editor.

		// Add Tip of the Day menu item on the fly!
		static CMenu* pSubMenu = NULL;

		CString strHelp; strHelp.LoadString(CG_IDS_TIPOFTHEDAYHELP);
		CString strMenu;
		int nMenuCount = pMenu->GetMenuItemCount();
		BOOL bFound = FALSE;
		for (int i=0; i < nMenuCount; i++) 
		{
			pMenu->GetMenuString(i, strMenu, MF_BYPOSITION);
			if (strMenu == strHelp)
			{ 
				pSubMenu = pMenu->GetSubMenu(i);
				bFound = TRUE;
				ASSERT(pSubMenu != NULL);
			}
		}

		CString strTipMenu;
		strTipMenu.LoadString(CG_IDS_TIPOFTHEDAYMENU);
		if (!bFound)
		{
			// Help menu is not available. Please add it!
			if (pSubMenu == NULL) 
			{
				// The same pop-up menu is shared between mainfrm and frame 
				// with the doc.
				static CMenu popUpMenu;
				pSubMenu = &popUpMenu;
				pSubMenu->CreatePopupMenu();
				pSubMenu->InsertMenu(0, MF_STRING|MF_BYPOSITION, 
					CG_IDS_TIPOFTHEDAY, strTipMenu);
			} 
			pMenu->AppendMenu(MF_STRING|MF_BYPOSITION|MF_ENABLED|MF_POPUP, 
				(UINT)pSubMenu->m_hMenu, strHelp);
			DrawMenuBar();
		} 
		else
		{      
			// Check to see if the Tip of the Day menu has already been added.
			pSubMenu->GetMenuString(0, strMenu, MF_BYPOSITION);

			if (strMenu != strTipMenu) 
			{
				// Tip of the Day submenu has not been added to the 
				// first position, so add it.
				pSubMenu->InsertMenu(0, MF_BYPOSITION);  // Separator
				pSubMenu->InsertMenu(0, MF_STRING|MF_BYPOSITION, 
					CG_IDS_TIPOFTHEDAY, strTipMenu);
			}
		}
	}

}

#include <windows.h>
#include <shlwapi.h>

HRESULT GetComCtlVersion(LPDWORD pdwMajor, LPDWORD pdwMinor)
{
HINSTANCE   hComCtl;

if(   IsBadWritePtr(pdwMajor, sizeof(DWORD)) || 
      IsBadWritePtr(pdwMinor, sizeof(DWORD)))
   return E_INVALIDARG;

//load the DLL
hComCtl = LoadLibrary(TEXT("comctl32.dll"));

if(hComCtl)
   {
   HRESULT           hr = S_OK;
   DLLGETVERSIONPROC pDllGetVersion;
   
   /*
   You must get this function explicitly because earlier versions of the DLL 
   don't implement this function. That makes the lack of implementation of the 
   function a version marker in itself.
   */
   pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hComCtl, TEXT("DllGetVersion"));
   
   if(pDllGetVersion)
      {
      DLLVERSIONINFO    dvi;
      
      ZeroMemory(&dvi, sizeof(dvi));
      dvi.cbSize = sizeof(dvi);
   
      hr = (*pDllGetVersion)(&dvi);
      
      if(SUCCEEDED(hr))
         {
         *pdwMajor = dvi.dwMajorVersion;
         *pdwMinor = dvi.dwMinorVersion;
         }
      else
         {
         hr = E_FAIL;
         }   
      }
   else
      {
      /*
      If GetProcAddress failed, then the DLL is a version previous to the one 
      shipped with IE 3.x.
      */
      *pdwMajor = 4;
      *pdwMinor = 0;
      }
   
   FreeLibrary(hComCtl);

   return hr;
   }

return E_FAIL;
}

BOOL CMainFrame::IsIE4Capable()
{
		DWORD pdwMajor;
		DWORD pdwMinor;

		if (FAILED(GetComCtlVersion(&pdwMajor,&pdwMinor))){
			return FALSE;
		}
		if (pdwMajor == 4 && pdwMinor < 71){
			return FALSE;
		}
		return TRUE;
}


