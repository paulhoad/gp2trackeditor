// FindCmd.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "FindCmd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindCmd dialog

CFindCmd::CFindCmd(CWnd* pParent /*=NULL*/) : CDialog(CFindCmd::IDD, pParent)
{
  //{{AFX_DATA_INIT(CFindCmd)
  m_FindCmd = _T("");
  //}}AFX_DATA_INIT
}

void CFindCmd::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CFindCmd)
  DDX_Text(pDX, IDC_LOOKFORCOMMAND, m_FindCmd);
  DDV_MaxChars(pDX, m_FindCmd, 4);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFindCmd, CDialog)
//{{AFX_MSG_MAP(CFindCmd)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindCmd message handlers

void CFindCmd::OnOK()
{
  // TODO: Add extra validation here
  CString data;
  char buff[256];

  BOOL hex = FALSE;

  CmdToFind = 0;

  GetDlgItemText(IDC_LOOKFORCOMMAND, data);

  wsprintf(buff, "%s", (LPCTSTR)data);

  hex = data.Find('x');

  if (hex != -1) {
    sscanf(buff, "%x", &CmdToFind);
  } else {
    sscanf(buff, "%d", &CmdToFind);
  }

  CDialog::OnOK();
}
