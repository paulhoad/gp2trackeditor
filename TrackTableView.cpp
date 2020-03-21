// TrackTableView.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEditor.h"
#include "TrackTableView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrackTableView

IMPLEMENT_DYNCREATE(CTrackTableView, CView)

CTrackTableView::CTrackTableView() {}

CTrackTableView::~CTrackTableView() {}

BEGIN_MESSAGE_MAP(CTrackTableView, CListCtrl)
//{{AFX_MSG_MAP(CTrackTableView)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackTableView message handlers
