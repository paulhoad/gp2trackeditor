// SceneryCmd.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "SceneryCmd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSceneryCmd dialog


CSceneryCmd::CSceneryCmd(CWnd* pParent /*=NULL*/)
	: CDialog(CSceneryCmd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSceneryCmd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSceneryCmd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSceneryCmd)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSceneryCmd, CDialog)
	//{{AFX_MSG_MAP(CSceneryCmd)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSceneryCmd message handlers
