// DirivingLine.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "DirivingLine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirivingLine dialog


CDirivingLine::CDirivingLine(CWnd* pParent /*=NULL*/)
	: CDialog(CDirivingLine::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDirivingLine)
	m_Distance = 0;
	m_DistanceFromSF = 0;
	//}}AFX_DATA_INIT
}


void CDirivingLine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDirivingLine)
	DDX_Text(pDX, IDC_EDIT1, m_Distance);
	DDV_MinMaxInt(pDX, m_Distance, 0, 65535);
	DDX_Text(pDX, IDC_EDIT2, m_DistanceFromSF);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDirivingLine, CDialog)
	//{{AFX_MSG_MAP(CDirivingLine)
	ON_BN_CLICKED(IDC_RADIO1, OnCCChange)
	ON_BN_CLICKED(IDC_RADIO2, OnCCChange)
	ON_BN_CLICKED(IDC_RADIO3, OnPoleChange)
	ON_BN_CLICKED(IDC_RADIO4, OnPoleChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirivingLine message handlers

BOOL CDirivingLine::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Distance = abs(beginYPos);
	m_DistanceFromSF = abs(beginXPos);

	int m_Position  = ((beginYPos < 0) ? IDC_RADIO3:IDC_RADIO4);

	CheckRadioButton(IDC_RADIO3,IDC_RADIO4,m_Position); 

	int m_Pole = ((PoleValue == 768) ? IDC_RADIO2:IDC_RADIO1);
	CheckRadioButton(IDC_RADIO1,IDC_RADIO2,m_Pole); 
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDirivingLine::OnOK() 
{
	// TODO: Add extra validation here

	UpdateData(TRUE);

	beginYPos = m_Distance;
	beginXPos = m_DistanceFromSF;
	if (IsDlgButtonChecked(IDC_RADIO3)) beginYPos *=-1;
	if (IsDlgButtonChecked(IDC_RADIO1)) 
	{
	   PoleValue=64768;
	}
	else
	{
		PoleValue=768;
	}
	
	CDialog::OnOK();
}

void CDirivingLine::OnCCChange() 
{
	// TODO: Add your control notification handler code here
	
}

void CDirivingLine::OnPoleChange() 
{
	// TODO: Add your control notification handler code here
	
}
