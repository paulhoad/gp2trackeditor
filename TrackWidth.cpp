// TrackWidth.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "TrackWidth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrackWidth dialog


CTrackWidth::CTrackWidth(GPTrack *track,CWnd* pParent /*=NULL*/)
	: CDialog(CTrackWidth::IDD, pParent),Observer(track,t_TrackWidth,NULL)
{
	//{{AFX_DATA_INIT(CTrackWidth)
	m_TrackWidthLeft = 0;
	m_TrackWidthRight = 0;
	//}}AFX_DATA_INIT
}

int CTrackWidth::Open()
{
	return DoModal();
}


void CTrackWidth::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrackWidth)
	DDX_Text(pDX, IDC_LEFT_TRACK_WIDTH, m_TrackWidthLeft);
	DDX_Text(pDX, IDC_RIGHT_TRACK_WIDTH, m_TrackWidthRight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrackWidth, CDialog)
	//{{AFX_MSG_MAP(CTrackWidth)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackWidth message handlers
