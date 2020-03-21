// VertexEditor.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "VertexEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVertexEditor dialog


CVertexEditor::CVertexEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CVertexEditor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVertexEditor)
	m_From = 0;
	m_To = 0;
	//}}AFX_DATA_INIT
}


void CVertexEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVertexEditor)
	DDX_Text(pDX, IDC_VERTEX_FROM, m_From);
	DDX_Text(pDX, IDC_VERTEX_TO, m_To);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVertexEditor, CDialog)
	//{{AFX_MSG_MAP(CVertexEditor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVertexEditor message handlers

BOOL CVertexEditor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_To =   (int)(PtPt.GetValue())/256;
	m_From = PtPt.GetValue() - (m_To*256);
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVertexEditor::OnOK() 
{
	UpdateData(TRUE);
	// TODO: Add extra validation here
	int vertexVal = (m_To*256)+m_From;
	PtPt.SetValue(vertexVal);
	
	CDialog::OnOK();
}
