// CScale.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "CScale.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCScale dialog


CCScale::CCScale(CWnd* pParent /*=NULL*/)
	: CDialog(CCScale::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCScale)
	m_CCScale = 0.0f;
	//}}AFX_DATA_INIT
}


void CCScale::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCScale)
	DDX_Text(pDX, IDC_CCSCALE, m_CCScale);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCScale, CDialog)
	//{{AFX_MSG_MAP(CCScale)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCScale message handlers
