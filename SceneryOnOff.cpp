// SceneryOnOff.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "SceneryOnOff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSceneryOnOff dialog


CSceneryOnOff::CSceneryOnOff(GPTrack *mytrack,int offset,int value,CWnd* pParent /*=NULL*/)
	: CDialog(CSceneryOnOff::IDD, pParent),track(mytrack),val(value)
{
	//{{AFX_DATA_INIT(CSceneryOnOff)
	m_LeftBank = FALSE;
	m_Ribbon1 = FALSE;
	m_Ribbon2 = FALSE;
	m_Ribbon3 = FALSE;
	m_Ribbon4 = FALSE;
	m_RightBank = FALSE;
	m_OnOff = _T("");
	m_Offset = 0;
	//}}AFX_DATA_INIT

	m_Offset = offset;
}


void CSceneryOnOff::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSceneryOnOff)
	DDX_Check(pDX, IDC_LEFTBANK, m_LeftBank);
	DDX_Check(pDX, IDC_RIBBON1, m_Ribbon1);
	DDX_Check(pDX, IDC_RIBBON2, m_Ribbon2);
	DDX_Check(pDX, IDC_RIBBON3, m_Ribbon3);
	DDX_Check(pDX, IDC_RIBBON4, m_Ribbon4);
	DDX_Check(pDX, IDC_RIGHTBANK, m_RightBank);
	DDX_Text(pDX, IDC_ONOFF, m_OnOff);
	DDX_Text(pDX, IDC_OFFSET, m_Offset);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSceneryOnOff, CDialog)
	//{{AFX_MSG_MAP(CSceneryOnOff)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSceneryOnOff message handlers

BOOL CSceneryOnOff::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_RightBank = (val & 32) ? TRUE : FALSE; 
	m_LeftBank  =(val & 16) ? TRUE : FALSE; 
	m_Ribbon1  =(val & 1) ? TRUE : FALSE; 
	m_Ribbon2  =(val & 2) ? TRUE : FALSE; 
	m_Ribbon3  =(val & 4) ? TRUE : FALSE; 
	m_Ribbon4  =(val & 8) ? TRUE : FALSE; 

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSceneryOnOff::OnOK() 
{
	val = 0;

	UpdateData(TRUE);

	// TODO: Add extra validation here
	val |= (m_RightBank) ? 32 : 0;
	val |= (m_LeftBank) ? 16 : 0;
	val |= (m_Ribbon1) ? 1 : 0;
	val |= (m_Ribbon2) ? 2 : 0;
	val |= (m_Ribbon3) ? 4 : 0;
	val |= (m_Ribbon4) ? 8 : 0;
	
	CDialog::OnOK();
}
