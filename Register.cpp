// Register.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "Register.h"
#include "time.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegister dialog

extern CTrackEditorApp theApp;

CRegister::CRegister(CWnd* pParent /*=NULL*/) : CDialog(CRegister::IDD, pParent)
{
  //{{AFX_DATA_INIT(CRegister)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void CRegister::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CRegister)
  DDX_Control(pDX, IDC_MESSAGE, m_Message);
  DDX_Control(pDX, IDC_REGNAME, m_RegistrationName);
  DDX_Control(pDX, IDC_REGNUMBER, m_RegistrationNumber);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRegister, CDialog)
//{{AFX_MSG_MAP(CRegister)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegister message handlers

char reg[] = {
  "Track Editor is Shareware and as such you should register your copy.\nRegistering your copy of TrackEditor cost  £10 (UK Pounds).\n\nTo register simply send a Cheque or Postal Order made payable to Paul Hoad\
for  £10 UK Pounds or $20 US Dollars ($20 of which $15 is  for registration & the $5 it cost me in Bank Charges)\n\
Addititonal contributions above the registration fee are always welcome. Send contributions to:\n\n\
Paul Hoad\n\
c/o 41 Southern Way\n\
Cove\n\
Farnborough\n\
Hampshire\n\
GU14 ORE\n\
ENGLAND\n\n\
\n\
NOTE: YOU MUST SUPPLY AN EMAIL ADDRESS\n\
\n\
For this I will include you on my list of people who get notified of the very\
latest releases.\n To help out keep playing with the Editor, Finding bugs and playing with the\
numbers, send me your suggestions, info, tracks, ideas, requirements, etc..\n\
"
};

BOOL CRegister::OnInitDialog()
{
  CDialog::OnInitDialog();

  CString strSection = "License";
  CString name;
  int number;

  // buffer[1024];

  // wsprintf(buffer,"%s",reg);

  // TODO: Add extra initialization here
  SetDlgItemText(IDC_MESSAGE, reg);

  name = theApp.GetProfileString(strSection, "Name", "Unknown User");
  number = theApp.GetProfileInt(strSection, "Number", 9999);

  SetDlgItemText(IDC_REGNAME, name);
  SetDlgItemInt(IDC_REGNUMBER, number);

  return TRUE;// return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

static int
  GetCode(CString name)
{
  int length = name.GetLength();
  int sum = 0;

  for (int i = 0; i < length; i++) {
    sum += (int)name.GetAt(i);
    sum *= sum;
    sum % 0x120969;
  }
  return abs(sum);
}

BOOL CheckRegistrationName()
{
  CString name;
  int number;
  int date;

  CString strSection = "License";

  name = theApp.GetProfileString(strSection, "Name", "Unknown User");
  number = theApp.GetProfileInt(strSection, "Number", 9999);
  date = theApp.GetProfileInt(strSection, "ID", 9999);

  if (date == 9999) {
    CTime t = CTime::GetCurrentTime();
    time_t osBinaryTime = t.GetTime();
    theApp.WriteProfileInt(strSection, "ID", osBinaryTime);
  }

  int sum = -1;

  if (name.GetLength() > 4) {
    sum = GetCode(name);
  }

  if (sum != -1 && sum == number) return TRUE;
  return FALSE;
}

static BOOL
  isCorrectRegistration(CString name, int number)
{
  int sum = GetCode(name);

#ifndef NDEBUG
  if (number == 120969) {
    char buffer[256];
    wsprintf(buffer, "%s %d %d", name, number, sum);
    AfxMessageBox(buffer);
  }
#endif

  if (sum == number) return TRUE;
  return FALSE;
}

void CRegister::OnOK()
{
  CString name;
  int number;
  // TODO: Add extra validation here
  GetDlgItemText(IDC_REGNAME, name);
  number = GetDlgItemInt(IDC_REGNUMBER);

  CString strSection = "License";

  if (isCorrectRegistration(name, number)) {
    AfxMessageBox("Thank you for Registering");
    theApp.WriteProfileString(strSection, "Name", name);
    theApp.WriteProfileInt(strSection, "Number", number);
  } else {
    AfxMessageBox("You supplied an incorrect Registration Number");
  }

  //#ifdef NDEBUG

  //#endif
  CDialog::OnOK();
}
