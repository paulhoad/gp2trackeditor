// CameraEdit.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "CameraEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCameraEdit dialog


CCameraEdit::CCameraEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CCameraEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCameraEdit)
	m_Number = 0;
	m_SM = 0;
	m_UK1 = 0;
	m_UK2 = 0;
	m_VR1 = 0;
	m_VR2 = 0;
	m_X = 0;
	m_Y = 0;
	m_Z1 = 0;
	m_ZF1 = 0;
	m_ZF2 = 0;
	m_Z2 = 0;
	m_UK3 = 0;
	m_UK4 = 0;
	m_UK5 = 0;
	m_UK6 = 0;
	m_UK7 = 0;
	m_TrackLength = _T("");
	//}}AFX_DATA_INIT
}


void CCameraEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCameraEdit)
	DDX_Text(pDX, IDC_NUMBER, m_Number);
	DDX_Text(pDX, IDC_SM, m_SM);
	DDX_Text(pDX, IDC_UK1, m_UK1);
	DDX_Text(pDX, IDC_UK2, m_UK2);
	DDX_Text(pDX, IDC_VR1, m_VR1);
	DDV_MinMaxInt(pDX, m_VR1, 0, 504);
	DDX_Text(pDX, IDC_VR2, m_VR2);
	DDV_MinMaxInt(pDX, m_VR2, 0, 504);
	DDX_Text(pDX, IDC_X, m_X);
	DDX_Text(pDX, IDC_Y, m_Y);
	DDX_Text(pDX, IDC_Z1, m_Z1);
	DDX_Text(pDX, IDC_ZF1, m_ZF1);
	DDX_Text(pDX, IDC_ZF2, m_ZF2);
	DDX_Text(pDX, IDC_Z2, m_Z2);
	DDX_Text(pDX, IDC_UK3, m_UK3);
	DDX_Text(pDX, IDC_UK4, m_UK4);
	DDX_Text(pDX, IDC_UK5, m_UK5);
	DDX_Text(pDX, IDC_UK6, m_UK6);
	DDX_Text(pDX, IDC_UK7, m_UK7);
	DDX_Text(pDX, IDC_TRACKLENGTH, m_TrackLength);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCameraEdit, CDialog)
	//{{AFX_MSG_MAP(CCameraEdit)
	ON_NOTIFY(NM_CLICK, IDC_CENTER_DISTANCE_SPIN, OnClickCenterDistanceSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCameraEdit message handlers

void CCameraEdit::OnClickCenterDistanceSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}




