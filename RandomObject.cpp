// RandomObject.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "RandomObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRandomObject dialog

CRandomObject::CRandomObject(CWnd* pParent /*=NULL*/)
  : CDialog(CRandomObject::IDD, pParent)
{
  //{{AFX_DATA_INIT(CRandomObject)
  m_ObjectsPerSection = 0;
  //}}AFX_DATA_INIT
}

void CRandomObject::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CRandomObject)
  DDX_Text(pDX, IDC_OBJECT_SEC, m_ObjectsPerSection);
  DDV_MinMaxInt(pDX, m_ObjectsPerSection, 0, 255);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRandomObject, CDialog)
//{{AFX_MSG_MAP(CRandomObject)
// NOTE: the ClassWizard will add message map macros here
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRandomObject message handlers
