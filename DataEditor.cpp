// DataEditor.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "DataEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataEditor dialog


CDataEditor::CDataEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDataEditor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataEditor)
	m_NewValue = 0;
	m_Desc = _T("");
	m_DataName = _T("");
	//}}AFX_DATA_INIT
}


void CDataEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataEditor)
	DDX_Text(pDX, IDC_EDIT1, m_NewValue);
	DDX_Text(pDX, IDC_DESCR, m_Desc);
	DDX_Text(pDX, IDC_NAME, m_DataName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataEditor, CDialog)
	//{{AFX_MSG_MAP(CDataEditor)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataEditor message handlers
