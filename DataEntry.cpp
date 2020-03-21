// DataEntry.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "DataEntry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataEntry dialog

CDataEntry::CDataEntry(CWnd* pParent /*=NULL*/)
  : CDialog(CDataEntry::IDD, pParent)
{
  //{{AFX_DATA_INIT(CDataEntry)
  m_NewValue = 0;
  m_PreviousValue = _T("");
  //}}AFX_DATA_INIT
}

void CDataEntry::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDataEntry)
  DDX_Text(pDX, IDC_NEWVALUE, m_NewValue);
  DDX_Text(pDX, IDC_PREVIOUSDATA, m_PreviousValue);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDataEntry, CDialog)
//{{AFX_MSG_MAP(CDataEntry)
// NOTE: the ClassWizard will add message map macros here
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataEntry message handlers
