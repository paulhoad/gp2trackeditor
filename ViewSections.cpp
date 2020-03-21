// ViewSections.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "ViewSections.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewSections dialog

CViewSections::CViewSections(CWnd* pParent /*=NULL*/)
  : CDialog(CViewSections::IDD, pParent)
{
  //{{AFX_DATA_INIT(CViewSections)
  m_From = 0;
  m_To = 0;
  //}}AFX_DATA_INIT
}

void CViewSections::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CViewSections)
  DDX_Text(pDX, IDC_FROM, m_From);
  DDX_Text(pDX, IDC_TO, m_To);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CViewSections, CDialog)
//{{AFX_MSG_MAP(CViewSections)
// NOTE: the ClassWizard will add message map macros here
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewSections message handlers
