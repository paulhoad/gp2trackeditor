// Credit.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "Credit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCredit dialog


CCredit::CCredit(CWnd* pParent /*=NULL*/)
	: CDialog(CCredit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCredit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCredit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCredit)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCredit, CDialog)
	//{{AFX_MSG_MAP(CCredit)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCredit message handlers
