// CmdEditor.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "CmdEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdEditor dialog

CCmdEditor::CCmdEditor(CWnd* pParent /*=NULL*/, int numArgs)
  : CDialog(CCmdEditor::IDD, pParent), numArgs(numArgs)
{
  //{{AFX_DATA_INIT(CCmdEditor)
  m_Arg4 = 0;
  m_Arg5 = 0;
  m_Arg6 = 0;
  m_Arg7 = 0;
  m_Arg8 = 0;
  m_Cmd = 0;
  m_Desc2 = _T("");
  m_Desc1 = _T("");
  m_Desc3 = _T("");
  m_Desc4 = _T("");
  m_Desc5 = _T("");
  m_Desc6 = _T("");
  m_Desc7 = _T("");
  m_Desc8 = _T("");
  m_DescriptionText = _T("");
  m_Desc10 = _T("");
  m_Desc11 = _T("");
  m_Desc12 = _T("");
  m_Desc13 = _T("");
  m_Desc9 = _T("");
  m_Arg11 = 0;
  m_Arg10 = 0;
  m_Arg1 = 0;
  m_Arg12 = 0;
  m_Arg13 = 0;
  m_Arg2 = 0;
  m_Arg3 = 0;
  m_Arg9 = 0;
  m_Arg14 = 0;
  m_Desc14 = _T("");
  //}}AFX_DATA_INIT
}

void CCmdEditor::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CCmdEditor)
  DDX_Control(pDX, IDC_ARG1, m_Ar1);
  DDX_Text(pDX, IDC_ARG4, m_Arg4);
  DDX_Text(pDX, IDC_ARG5, m_Arg5);
  DDX_Text(pDX, IDC_ARG6, m_Arg6);
  DDX_Text(pDX, IDC_ARG7, m_Arg7);
  DDX_Text(pDX, IDC_ARG8, m_Arg8);
  DDX_Text(pDX, IDC_CMD, m_Cmd);
  DDX_Text(pDX, IDC_STATIC2, m_Desc2);
  DDX_Text(pDX, IDC_STATIC1, m_Desc1);
  DDX_Text(pDX, IDC_STATIC3, m_Desc3);
  DDX_Text(pDX, IDC_STATIC4, m_Desc4);
  DDX_Text(pDX, IDC_STATIC5, m_Desc5);
  DDX_Text(pDX, IDC_STATIC6, m_Desc6);
  DDX_Text(pDX, IDC_STATIC7, m_Desc7);
  DDX_Text(pDX, IDC_STATIC8, m_Desc8);
  DDX_Text(pDX, IDC_DESCRIPTION_TEXT, m_DescriptionText);
  DDX_Text(pDX, IDC_STATIC10, m_Desc10);
  DDX_Text(pDX, IDC_STATIC11, m_Desc11);
  DDX_Text(pDX, IDC_STATIC12, m_Desc12);
  DDX_Text(pDX, IDC_STATIC13, m_Desc13);
  DDX_Text(pDX, IDC_STATIC9, m_Desc9);
  DDX_Text(pDX, IDC_ARG11, m_Arg11);
  DDX_Text(pDX, IDC_ARG10, m_Arg10);
  DDX_Text(pDX, IDC_ARG1, m_Arg1);
  DDX_Text(pDX, IDC_ARG12, m_Arg12);
  DDX_Text(pDX, IDC_ARG13, m_Arg13);
  DDX_Text(pDX, IDC_ARG2, m_Arg2);
  DDX_Text(pDX, IDC_ARG3, m_Arg3);
  DDX_Text(pDX, IDC_ARG9, m_Arg9);
  DDX_Text(pDX, IDC_ARG14, m_Arg14);
  DDX_Text(pDX, IDC_STATIC15, m_Desc14);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCmdEditor, CDialog)
//{{AFX_MSG_MAP(CCmdEditor)
ON_BN_CLICKED(ID_HELP, OnHelp)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdEditor message handlers

#define ENABLE_ARG(x)                             \
  if (numArgs >= ##x) {                           \
    CEdit* ctrl = (CEdit*)GetDlgItem(IDC_ARG##x); \
    if (ctrl) ctrl->SetReadOnly(FALSE);           \
  } else {                                        \
    CEdit* ctrl = (CEdit*)GetDlgItem(IDC_ARG##x); \
    if (ctrl) ctrl->SetReadOnly(TRUE);            \
  }

BOOL CCmdEditor::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO: Add extra initialization here

  ENABLE_ARG(1)
  ENABLE_ARG(2)
  ENABLE_ARG(3)
  ENABLE_ARG(4)
  ENABLE_ARG(5)
  ENABLE_ARG(6)
  ENABLE_ARG(7)
  ENABLE_ARG(8)
  ENABLE_ARG(9)
  ENABLE_ARG(10)
  ENABLE_ARG(11)
  ENABLE_ARG(12)
  ENABLE_ARG(13)
  ENABLE_ARG(14)

  return TRUE;// return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

extern CString
  getCmdHelp(int id);

void CCmdEditor::OnHelp()
{
  // TODO: Add your control notification handler code here
  CString help = getCmdHelp(m_Cmd);

  AfxMessageBox(help, MB_OK | MB_ICONINFORMATION);
}
