// TrackEditorView.cpp : implementation of the CTrackEditorView class
//

#include "stdafx.h"
#include "TrackEditor.h"

#include "TrackEditorDoc.h"
#include "TrackEditorView.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrackEditorView

IMPLEMENT_DYNCREATE(CTrackEditorView, CView)

BEGIN_MESSAGE_MAP(CTrackEditorView, CView)
	//{{AFX_MSG_MAP(CTrackEditorView)
	ON_COMMAND(ZOOM_IN, OnIn)
	ON_COMMAND(ZOOM_OUT, OnOut)
	ON_COMMAND(VIEW_TRACK, OnTrack)
	ON_COMMAND(VIEW_PITLANE, OnPitlane)
	ON_COMMAND(VIEW_OBJECTS, OnObjects)
	ON_COMMAND(VIEW_LINE, OnLine)
	ON_COMMAND(VIEW_TRACKINFO, OnTrackinfo)
	ON_COMMAND(ID_EDIT_REMOVEALLCCARG1, OnEditRemoveallccarg1)
	ON_COMMAND(ID_EDIT_REMOVEALLARG2, OnEditRemoveallarg2)
	ON_COMMAND(ID_VIEW_SHOWWALLS, OnViewShowwalls)
	ON_UPDATE_COMMAND_UI(VIEW_TRACK, OnUpdateTrack)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackEditorView construction/destruction

CTrackEditorView::CTrackEditorView()
{
	// TODO: add construction code here
	display = new Display();
}

CTrackEditorView::~CTrackEditorView()
{
}

BOOL CTrackEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTrackEditorView drawing

void CTrackEditorView::OnDraw(CDC* pDC)
{
	CTrackEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
	GPTrack * track = pDoc->getTrack();

	
	if (track)
	{
		if (display)
		{
		 display->setGraphics(pDC);
		 track->drawTrack(display);
		 track->drawPitlane(display);
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
// CTrackEditorView printing

BOOL CTrackEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTrackEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTrackEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTrackEditorView diagnostics

#ifdef _DEBUG
void CTrackEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CTrackEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTrackEditorDoc* CTrackEditorView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTrackEditorDoc)));
	return (CTrackEditorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTrackEditorView message handlers

void CTrackEditorView::OnIn() 
{
	// TODO: Add your command handler code here
	display->ZoomIn();
	RECT rect;

	GetClientRect(&rect);
	InvalidateRect(&rect);
}

void CTrackEditorView::OnOut() 
{
	// TODO: Add your command handler code here
	display->ZoomOut();
}

void CTrackEditorView::OnTrack() 
{
	// TODO: Add your command handler code here
	
}

void CTrackEditorView::OnPitlane() 
{
	// TODO: Add your command handler code here
	
}

void CTrackEditorView::OnObjects() 
{
	// TODO: Add your command handler code here
	
}

void CTrackEditorView::OnLine() 
{
	// TODO: Add your command handler code here
	
}

void CTrackEditorView::OnTrackinfo() 
{
	// TODO: Add your command handler code here
	
}

void CTrackEditorView::OnEditRemoveallccarg1() 
{
	// TODO: Add your command handler code here
	
}

void CTrackEditorView::OnEditRemoveallarg2() 
{
	// TODO: Add your command handler code here
	
}

void CTrackEditorView::OnViewShowwalls() 
{
	// TODO: Add your command handler code here
	
}

void CTrackEditorView::OnUpdateTrack(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
