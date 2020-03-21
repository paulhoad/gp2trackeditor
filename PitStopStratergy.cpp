// PitStopStratergy.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "PitStopStratergy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPitStopStratergy dialog


CPitStopStratergy::CPitStopStratergy(GPTrack *mytrack,CWnd* pParent /*=NULL*/)
	: CDialog(CPitStopStratergy::IDD, pParent),track(mytrack)
{
	//{{AFX_DATA_INIT(CPitStopStratergy)
	m_OneStopPercentage = 0;
	m_ThreeStopDurOne = 0;
	m_ThreeStopDurThree = 0;
	m_ThreeStopBeginThree = 0;
	m_ThreeStopBeginTwo = 0;
	m_ThreeStopPercentage = 0;
	m_TwoStopPercentage = 0;
	m_TwoStopDurOne = 0;
	m_TwoStopDurTwo = 0;
	m_TwoStopBeginTwo = 0;
	m_TwoStopBeginOne = 0;
	m_ThreeStopBeginOne = 0;
	m_ThreeStopDurTwo = 0;
	m_OneStopBeginOne = 0;
	m_OneStopDurOne = 0;
	m_Laps = 0;
	//}}AFX_DATA_INIT

	m_Laps = track->VariousData[11].GetValue();

	m_OneStopPercentage = track->VariousData[14].GetValue();
	m_OneStopBeginOne   = track->VariousData[15].GetValue();
	m_OneStopDurOne     = track->VariousData[16].GetValue();


	m_TwoStopPercentage = track->VariousData[22].GetValue();
	m_TwoStopBeginOne   = track->VariousData[23].GetValue();
	m_TwoStopDurOne     = track->VariousData[24].GetValue();
	m_TwoStopBeginTwo   = track->VariousData[25].GetValue();
	m_TwoStopDurTwo     = track->VariousData[26].GetValue();

	m_ThreeStopPercentage = track->VariousData[30].GetValue();
	m_ThreeStopBeginOne   = track->VariousData[31].GetValue();
	m_ThreeStopDurOne     = track->VariousData[32].GetValue();
	m_ThreeStopBeginTwo   = track->VariousData[33].GetValue();
	m_ThreeStopDurTwo     = track->VariousData[34].GetValue();
	m_ThreeStopBeginThree   = track->VariousData[35].GetValue();
	m_ThreeStopDurThree     = track->VariousData[36].GetValue();


	

}


void CPitStopStratergy::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPitStopStratergy)
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDC_SPIN18, m_ThreeStopPercentageSpin);
	DDX_Control(pDX, IDC_SPIN17, m_TwoStopPercentageSpin);
	DDX_Control(pDX, IDC_SPIN16, m_ThreeStopDurThreeSpin);
	DDX_Control(pDX, IDC_SPIN15, m_ThreeStopDurTwoSpin);
	DDX_Control(pDX, IDC_SPIN14, m_ThreeStopDurOneSpin);
	DDX_Control(pDX, IDC_SPIN13, m_TwoStopDurTwoSpin);
	DDX_Control(pDX, IDC_SPIN12, m_TwoStopDurOneSpin);
	DDX_Control(pDX, IDC_SPIN9, m_OneStopDurOneSpin);
	DDX_Control(pDX, IDC_SPIN8, m_ThreeStopBeginThreeSpin);
	DDX_Control(pDX, IDC_SPIN7, m_ThreeStopBeginTwoSpin);
	DDX_Control(pDX, IDC_SPIN6, m_ThreeStopBeginOneSpin);
	DDX_Control(pDX, IDC_SPIN5, m_TwoStopBeginTwoSpin);
	DDX_Control(pDX, IDC_SPIN4, m_TwoStopBeginOneSpin);
	DDX_Control(pDX, IDC_SPIN3, m_OneStopBeginOneSpin);
	DDX_Control(pDX, IDC_SPIN2, m_OneStopPercentageSpin);
	DDX_Control(pDX, IDC_SPIN19, m_LapSpin);
	DDX_Text(pDX, IDC_ONESTOP, m_OneStopPercentage);
	DDX_Text(pDX, IDC_THREE_STOP_DUR_ONE, m_ThreeStopDurOne);
	DDX_Text(pDX, IDC_THREE_STOP_DUR_THREE, m_ThreeStopDurThree);
	DDX_Text(pDX, IDC_THREE_STOP_EBGIN_THREE, m_ThreeStopBeginThree);
	DDX_Text(pDX, IDC_THREE_STOP_EBGIN_TWO, m_ThreeStopBeginTwo);
	DDX_Text(pDX, IDC_THREESTOP, m_ThreeStopPercentage);
	DDX_Text(pDX, IDC_TWOSTOP, m_TwoStopPercentage);
	DDX_Text(pDX, IDC_TWO_STOP_DUR_ONE, m_TwoStopDurOne);
	DDX_Text(pDX, IDC_TWO_STOP_DUR_TWO, m_TwoStopDurTwo);
	DDX_Text(pDX, IDC_TWO_STOP_BEGIN_TWO, m_TwoStopBeginTwo);
	DDX_Text(pDX, IDC_TWO_STOP_BEGIN_ONE, m_TwoStopBeginOne);
	DDX_Text(pDX, IDC_THREE_STOP_EBGIN_ONE, m_ThreeStopBeginOne);
	DDX_Text(pDX, IDC_THREE_STOP_DUR_TWO, m_ThreeStopDurTwo);
	DDX_Text(pDX, IDC_ONE_STOP_LAPS, m_OneStopBeginOne);
	DDX_Text(pDX, IDC_ONE_STOP_DUR, m_OneStopDurOne);
	DDX_Text(pDX, IDC_LAPS, m_Laps);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPitStopStratergy, CDialog)
	//{{AFX_MSG_MAP(CPitStopStratergy)
	ON_EN_CHANGE(IDC_THREESTOP, OnChangeThreestop)
	ON_EN_CHANGE(IDC_ONESTOP, OnChangeOnestop)
	ON_EN_CHANGE(IDC_TWOSTOP, OnChangeTwostop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPitStopStratergy message handlers

void CPitStopStratergy::OnOK() 
{
	UpdateData(TRUE);
	// TODO: Add extra validation here
	track->VariousData[11].SetValue(m_Laps);

	track->VariousData[14].SetValue(m_OneStopPercentage);
	track->VariousData[15].SetValue(m_OneStopBeginOne);
	track->VariousData[16].SetValue(m_OneStopDurOne);


    track->VariousData[22].SetValue(m_TwoStopPercentage);
	track->VariousData[23].SetValue(m_TwoStopBeginOne);
	track->VariousData[24].SetValue(m_TwoStopDurOne);
	track->VariousData[25].SetValue(m_TwoStopBeginTwo);
	track->VariousData[26].SetValue(m_TwoStopDurTwo);

	track->VariousData[30].SetValue(m_ThreeStopPercentage);
	track->VariousData[31].SetValue(m_ThreeStopBeginOne);
	track->VariousData[32].SetValue(m_ThreeStopDurOne);
	track->VariousData[33].SetValue(m_ThreeStopBeginTwo);
	track->VariousData[34].SetValue(m_ThreeStopDurTwo);
	track->VariousData[35].SetValue(m_ThreeStopBeginThree);
	track->VariousData[36].SetValue(m_ThreeStopDurThree);

	if ((m_OneStopPercentage + m_TwoStopPercentage + m_ThreeStopPercentage)!= 100)
	{
		AfxMessageBox("Total Percentage do not add up to 100%");
	}
	
	CDialog::OnOK();
}

BOOL CPitStopStratergy::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_OneStopPercentageSpin.SetRange(0,100);
	m_TwoStopPercentageSpin.SetRange(0,100);
	m_ThreeStopPercentageSpin.SetRange(0,100);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPitStopStratergy::OnChangeThreestop() 
{
	if (::IsWindow(m_OkButton.m_hWnd))
	{
	UpdateData(TRUE);
	if ((m_OneStopPercentage + m_TwoStopPercentage + m_ThreeStopPercentage)!= 100)
	{
		m_OkButton.EnableWindow(FALSE);
	}
	else
		m_OkButton.EnableWindow(TRUE);
	}
}

void CPitStopStratergy::OnChangeOnestop() 
{
	if (::IsWindow(m_OkButton.m_hWnd))
	{
	UpdateData(TRUE);
	if ((m_OneStopPercentage + m_TwoStopPercentage + m_ThreeStopPercentage)!= 100)
	{
		m_OkButton.EnableWindow(FALSE);
	}
	else
		m_OkButton.EnableWindow(TRUE);
	}
}

void CPitStopStratergy::OnChangeTwostop() 
{
	

	if (::IsWindow(m_OkButton.m_hWnd))
	{
	UpdateData(TRUE);
	if ((m_OneStopPercentage + m_TwoStopPercentage + m_ThreeStopPercentage)!= 100)
	{
		m_OkButton.EnableWindow(FALSE);
	}
	else
		m_OkButton.EnableWindow(TRUE);
	}
}
