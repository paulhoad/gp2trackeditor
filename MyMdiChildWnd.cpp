// MyMdiChildWnd.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "MyMdiChildWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyMdiChildWnd

IMPLEMENT_DYNCREATE(CMyMdiChildWnd, CMDIChildWnd)

CMyMdiChildWnd::CMyMdiChildWnd()
{
}

CMyMdiChildWnd::~CMyMdiChildWnd()
{
}


BEGIN_MESSAGE_MAP(CMyMdiChildWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CMyMdiChildWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyMdiChildWnd message handlers

BOOL CMyMdiChildWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CMDIChildWnd::PreCreateWindow(cs);
}
