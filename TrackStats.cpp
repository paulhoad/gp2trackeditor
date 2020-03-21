// TrackStats.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEditor.h"
#include "TrackStats.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrackStats dialog


CTrackStats::CTrackStats(CWnd* pParent /*=NULL*/)
	: CDialog(CTrackStats::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrackStats)
	m_TrackName = _T("");
	m_TrackCountry = _T("");
	m_NumObjects = _T("");
	m_CCSectionsStr = _T("");
	m_PitSectionsStr = _T("");
	m_TrackSectionsStr = _T("");
	m_TrackLengthStr  = _T("");
	m_TrackMinStLen = 0;
	m_TrackMaxStLen = 0;
	m_TrackMinCuLen = 0;
	m_TrackMaxCuLen = 0;
	m_TrackLenKm = 0;
	m_TrackLenMiles = 0;
	m_TrackLenKmPits = 0;
	m_TrackLenKmBoth = 0;
	m_TotalHeight = _T("");
	m_TotalAngle = _T("");
	m_TotalAltitude = _T("");
	m_CCRads = _T("");
	m_PitRads = _T("");
	m_TrackRads = _T("");
	m_TrackCmds = _T("");
	m_PitCmds = _T("");
	m_TotalCmds = _T("");
	//}}AFX_DATA_INIT
}


void CTrackStats::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrackStats)
	DDX_Text(pDX, IDC_TRACK_NAME, m_TrackName);
	DDX_Text(pDX, IDC_TRACK_COUNTRY, m_TrackCountry);
	DDX_Text(pDX, IDC_OBJECTS, m_NumObjects);
	DDX_Text(pDX, IDC_CC_SECTIONS, m_CCSectionsStr);
	DDX_Text(pDX, IDC_NUM_PIT_SECTIONS, m_PitSectionsStr);
	DDX_Text(pDX, IDC_NUM_TRACK_SECTIONS, m_TrackSectionsStr);
	DDX_Text(pDX, IDC_TOTAL_LENGTH, m_TrackLengthStr);
	DDX_Text(pDX, IDC_MINSTLEN, m_TrackMinStLen);
	DDX_Text(pDX, IDC_MAXSTLEN, m_TrackMaxStLen);
	DDX_Text(pDX, IDC_MINCULEN, m_TrackMinCuLen);
	DDX_Text(pDX, IDC_MAXCULEN, m_TrackMaxCuLen);
	DDX_Text(pDX, IDC_LENKM, m_TrackLenKm);
	DDX_Text(pDX, IDC_LENMILES, m_TrackLenMiles);
	DDX_Text(pDX, IDC_LENKM_PITS, m_TrackLenKmPits);
	DDX_Text(pDX, IDC_LENKM_BOTH, m_TrackLenKmBoth);
	DDX_Text(pDX, IDC_TOTAL_HEIGHT, m_TotalHeight);
	DDX_Text(pDX, IDC_TOTAL_ANGLE, m_TotalAngle);
	DDX_Text(pDX, IDC_TOTAL_ALTITUDE, m_TotalAltitude);
	DDX_Text(pDX, IDC_CCRADS, m_CCRads);
	DDX_Text(pDX, IDC_PITRAD, m_PitRads);
	DDX_Text(pDX, IDC_TRACKRAD, m_TrackRads);
	DDX_Text(pDX, IDC_TRKCMDS, m_TrackCmds);
	DDX_Text(pDX, IDC_PITCMDS, m_PitCmds);
	DDX_Text(pDX, IDC_TOTALCMDS, m_TotalCmds);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrackStats, CDialog)
	//{{AFX_MSG_MAP(CTrackStats)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackStats message handlers
