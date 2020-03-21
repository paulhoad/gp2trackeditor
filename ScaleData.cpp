// ScaleData.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "ScaleData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScaleData dialog


CScaleData::CScaleData(CWnd* pParent /*=NULL*/)
	: CDialog(CScaleData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScaleData)
	m_ScaleVal = 0.0f;
	//}}AFX_DATA_INIT
}


void CScaleData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScaleData)
	DDX_Text(pDX, IDC_SCALE, m_ScaleVal);
	DDV_MinMaxFloat(pDX, m_ScaleVal, 1.e-004f, 2000.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScaleData, CDialog)
	//{{AFX_MSG_MAP(CScaleData)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScaleData message handlers
