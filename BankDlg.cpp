// BankDlg.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "BankDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBankDlg dialog


CBankDlg::CBankDlg(int begin,int end,CWnd* pParent /*=NULL*/)
	: CDialog(CBankDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBankDlg)
	m_LeftBank = 0;
	m_RightBank = 0;
	//}}AFX_DATA_INIT

	m_From = begin;
	m_To   = end;
}


void CBankDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBankDlg)
	DDX_Text(pDX, IDC_LEFT_BANK_WIDTH, m_LeftBank);
	DDV_MinMaxInt(pDX, m_LeftBank, 0, 65535);
	DDX_Text(pDX, IDC_RIGHT_BANK_WIDTH, m_RightBank);
	DDV_MinMaxInt(pDX, m_RightBank, 0, 65535);
	DDX_Text(pDX, IDC_FROM, m_From);
	DDX_Text(pDX, IDC_TO, m_To);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBankDlg, CDialog)
	//{{AFX_MSG_MAP(CBankDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBankDlg message handlers

BOOL CBankDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CheckDlgButton(IDC_CHECK_LEFT,TRUE);
	CheckDlgButton(IDC_CHECK_RIGHT,TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBankDlg::OnOK() 
{
	m_ChangeLeft  = IsDlgButtonChecked(IDC_CHECK_LEFT);
	m_ChangeRight = IsDlgButtonChecked(IDC_CHECK_RIGHT);

	CDialog::OnOK();
}
