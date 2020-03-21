// JamView.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "JamView.h"
#include "TrackEditorDoc.h"
#include "JAM.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJamView

IMPLEMENT_DYNCREATE(CJamView, CScrollView)

CJamView::CJamView()
{
	currentScale = 1.0;
}

CJamView::~CJamView()
{
}


BEGIN_MESSAGE_MAP(CJamView, CScrollView)
	//{{AFX_MSG_MAP(CJamView)
	ON_WM_CREATE()
	ON_COMMAND(ID_ZOOM_HOME, OnZoomHome)
	ON_COMMAND(ZOOM_IN, OnIn)
	ON_COMMAND(ZOOM_OUT, OnOut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJamView drawing

void CJamView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 256;
	SetScrollSizes(MM_TEXT, sizeTotal);

	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	myjam = pDoc->getCurrentJam();
}

void CJamView::OnDraw(CDC* pDC)
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	
	if (myjam)
	{
	  myjam->DrawAllJams(pDC,currentScale);	  
	}
}

/////////////////////////////////////////////////////////////////////////////
// CJamView diagnostics

#ifdef _DEBUG
void CJamView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CJamView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CJamView message handlers

int CJamView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CJamView::OnZoomHome() 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	currentScale = 1.0;

	pDoc->UpdateAllViews(NULL);
	Resize();
	
}

void CJamView::OnIn() 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	currentScale *= 2;
	pDoc->UpdateAllViews(NULL);
	Resize();	
}


void CJamView::Resize()
{
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = (int)(currentScale*256);
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CJamView::OnOut() 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	currentScale /= 2;
	pDoc->UpdateAllViews(NULL);
	Resize();
}
