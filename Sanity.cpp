// Sanity.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "Sanity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString buffer;

/////////////////////////////////////////////////////////////////////////////
// CSanity dialog

CSanity::CSanity(CWnd* pParent /*=NULL*/) : CDialog(CSanity::IDD, pParent)
{
  //{{AFX_DATA_INIT(CSanity)
  //}}AFX_DATA_INIT
}

void CSanity::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CSanity)
  DDX_Control(pDX, IDC_LIST2, m_SanityList);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSanity, CDialog)
//{{AFX_MSG_MAP(CSanity)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSanity message handlers

void CSanity::OnOK()
{
  // TODO: Add extra validation here
  EndDialog(IDOK);

  CDialog::OnOK();
}

void CSanity::Output(CString message)
{
  m_SanityList.AddString(message);
}
