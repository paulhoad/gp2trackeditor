// PitEntrance.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "PitEntrance.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPitEntrance dialog

CPitEntrance::CPitEntrance(CWnd* pParent /*=NULL*/)
  : CDialog(CPitEntrance::IDD, pParent)
{
  //{{AFX_DATA_INIT(CPitEntrance)
  m_Entrance = 0;
  m_Exit = 0;
  //}}AFX_DATA_INIT
  LeaveToLeft = FALSE;
  EnterToLeft = FALSE;
}

void CPitEntrance::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPitEntrance)
  DDX_Text(pDX, IDC_ENTRACE, m_Entrance);
  DDX_Text(pDX, IDC_EXIT, m_Exit);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPitEntrance, CDialog)
//{{AFX_MSG_MAP(CPitEntrance)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPitEntrance message handlers

BOOL CPitEntrance::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO: Add extra initialization here

  CheckRadioButton(IDC_ENTRANCE_LEFT, IDC_ENTRANCE_RIGHT, LeaveToLeft ? IDC_ENTRANCE_LEFT : IDC_ENTRANCE_RIGHT);
  CheckRadioButton(IDC_EXIT_LEFT, IDC_EXIT_RIGHT, EnterToLeft ? IDC_EXIT_LEFT : IDC_EXIT_RIGHT);

  return TRUE;// return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
