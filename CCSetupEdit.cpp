// CCSetupEdit.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "CCSetupEdit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCCSetupEdit dialog


CCCSetupEdit::CCCSetupEdit(GPTrack *mytrack,CWnd* pParent /*=NULL*/)
	: CDialog(CCCSetupEdit::IDD, pParent), track(mytrack)
{
	//{{AFX_DATA_INIT(CCCSetupEdit)
	m_Air = 0;
	m_Accel = 0;
	m_FrontWing = 0;
	m_Gear1 = 0;
	m_Gear2 = 0;
	m_Gear3 = 0;
	m_Gear4 = 0;
	m_Gear5 = 0;
	m_Gear6 = 0;
	m_Grip = 0;
	m_RearWing = 0;
	m_Tyre = 0;
	m_Fuel = 0;
	m_BrakeBalance = 0;
	//}}AFX_DATA_INIT
}


void CCCSetupEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCCSetupEdit)
	DDX_Control(pDX, IDC_GRIPSPIN, m_GripSpin);
	DDX_Control(pDX, IDC_GEAR6SPIN, m_Gear6Spin);
	DDX_Control(pDX, IDC_GEAR5SPIN, m_Gear5Spin);
	DDX_Control(pDX, IDC_GEAR4SPIN, m_Gear4Spin);
	DDX_Control(pDX, IDC_GEAR3SPIN, m_Gear3Spin);
	DDX_Control(pDX, IDC_GEAR2SPIN, m_Gear2Spin);
	DDX_Control(pDX, IDC_GEAR1SPIN, m_Gear1Spin);
	DDX_Control(pDX, IDC_CORRECT_FUEL, m_FuelSpin);
	DDX_Control(pDX, IDC_BRAKEBALSPIN, m_BrakeBalSpin);
	DDX_Control(pDX, IDC_AIRSPIN, m_AirSpin);
	DDX_Control(pDX, IDC_REARWINGSPIN, m_RearWingSpin);
	DDX_Control(pDX, IDC_FRONTWINGSPIN, m_FrontWingSpin);
	DDX_Control(pDX, IDC_ACCELERATION_SPIN, m_AccelSpin);
	DDX_Control(pDX, IDC_TYRECOMBO, m_TyreCombo);
	DDX_Text(pDX, IDC_AIR, m_Air);
	DDX_Text(pDX, IDC_ACCEL, m_Accel);
	DDX_Text(pDX, IDC_FRONTWING, m_FrontWing);
	DDX_Text(pDX, IDC_GEAR1, m_Gear1);
	DDX_Text(pDX, IDC_GEAR2, m_Gear2);
	DDX_Text(pDX, IDC_GEAR3, m_Gear3);
	DDX_Text(pDX, IDC_GEAR4, m_Gear4);
	DDX_Text(pDX, IDC_GEAR5, m_Gear5);
	DDX_Text(pDX, IDC_GEAR6, m_Gear6);
	DDX_Text(pDX, IDC_GRIP, m_Grip);
	DDX_Text(pDX, IDC_READWING, m_RearWing);
	DDX_Text(pDX, IDC_TYRE, m_Tyre);
	DDX_Text(pDX, IDC_FUEL, m_Fuel);
	DDX_Text(pDX, IDC_BRAKEBALANCE, m_BrakeBalance);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCCSetupEdit, CDialog)
	//{{AFX_MSG_MAP(CCCSetupEdit)
	ON_BN_CLICKED(IDC_CORRECT_FUEL, OnCorrectFuel)
	ON_CBN_SELCHANGE(IDC_TYRECOMBO, OnSelchangeTyrecombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCCSetupEdit message handlers

void CCCSetupEdit::OnCorrectFuel() 
{
	// TODO: Add your control notification handler code here
	m_Fuel = (int)(track->totalLength()*5.73);
	SetDlgItemInt(IDC_FUEL,m_Fuel);
}

void CCCSetupEdit::OnSelchangeTyrecombo() 
{
	int idx = m_TyreCombo.GetCurSel();

	switch(idx)
	{
	   case 0: m_Tyre = 52 ;break;
	   case 1: m_Tyre = 53 ;break;
	   case 2: m_Tyre = 54 ;break;
	   case 3: m_Tyre = 55 ;break;
	}

	SetDlgItemInt(IDC_TYRE,m_Tyre);
}

BOOL CCCSetupEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_TyreCombo.AddString("A");
	m_TyreCombo.AddString("B");
	m_TyreCombo.AddString("C");
	m_TyreCombo.AddString("D");
	m_TyreCombo.AddString("Unk");

	switch (m_Tyre)
	{
	   case 52: m_TyreCombo.SetCurSel(0);break;
	   case 53: m_TyreCombo.SetCurSel(1);break;
	   case 54: m_TyreCombo.SetCurSel(2);break;
	   case 55: m_TyreCombo.SetCurSel(3);break;
	   default: m_TyreCombo.SetCurSel(4);break;
	}


	m_GripSpin.SetRange(0,1000);
	m_Gear6Spin.SetRange(0,1000);
	m_Gear5Spin.SetRange(0,1000);
	m_Gear4Spin.SetRange(0,1000);
	m_Gear3Spin.SetRange(0,1000);
	m_Gear2Spin.SetRange(0,1000);
	m_Gear1Spin.SetRange(0,1000);
	//m_FuelSpin.SetRange(0,10000);
	m_BrakeBalSpin.SetRange(0,1000);
	m_AirSpin.SetRange(0,1000);
	m_RearWingSpin.SetRange(0,1000);
	m_FrontWingSpin.SetRange(0,1000);
	m_AccelSpin.SetRange(0,1000);

		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
