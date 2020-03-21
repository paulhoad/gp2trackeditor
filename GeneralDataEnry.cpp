// GeneralDataEnry.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "GeneralDataEnry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeneralDataEnry dialog


CGeneralDataEnry::CGeneralDataEnry(CString title,CString label1,int val1,CString label2,int val2,CString label3,int val3,CWnd* pParent)
  : CDialog(CGeneralDataEnry::IDD, pParent), title(title)
{
   //{{AFX_DATA_INIT(CGeneralDataEnry)
	m_Label1 = _T(label1);
	m_Label2 = _T(label2);
	m_Label3 = _T(label3);
	m_EditVal1 = 0;
	m_EditVal2 = 0;
	m_EditVal3 = 0;
	//}}AFX_DATA_INIT

	vals[0] = val1;
	vals[1] = val2;
	vals[2] = val3;

	
}

int CGeneralDataEnry::getValue(int idx)
{
	int val;

	switch(idx)
	{
	  case 1:
		  val = GetDlgItemInt(IDC_EDIT1);
		  break;
	  case 2:
		  val = GetDlgItemInt(IDC_EDIT2);
		  break;
	  case 3:
		  val = GetDlgItemInt(IDC_EDIT3);
		  break;
	}
	return val;
}

BOOL CGeneralDataEnry::ProcessEntry()
{
	int result = DoModal();

	//int val1 = GetDlgItemText(IDC_EDIT1);
	//int val2 = GetDlgItemText(IDC_EDIT2);
	//int val3 = GetDlgItemText(IDC_EDIT3);
	//vals[0] = val1;
	//vals[1] = val2;
	//vals[2] = val3;

	if (result == IDOK) return TRUE;
	return FALSE;
}


void CGeneralDataEnry::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralDataEnry)
	DDX_Text(pDX, IDC_STATIC1, m_Label1);
	DDX_Text(pDX, IDC_STATIC2, m_Label2);
	DDX_Text(pDX, IDC_STATIC3, m_Label3);
	DDX_Text(pDX, IDC_EDIT1, m_EditVal1);
	DDX_Text(pDX, IDC_EDIT2, m_EditVal2);
	DDX_Text(pDX, IDC_EDIT3, m_EditVal3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGeneralDataEnry, CDialog)
	//{{AFX_MSG_MAP(CGeneralDataEnry)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeneralDataEnry message handlers

BOOL CGeneralDataEnry::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetDlgItemInt(IDC_EDIT1,vals[0]);
	SetDlgItemInt(IDC_EDIT2,vals[1]);
	SetDlgItemInt(IDC_EDIT3,vals[2]);
	SetWindowText(title);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
