// TrackNameInfo.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEditor.h"
#include "TrackNameInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrackNameInfo dialog

CTrackNameInfo::CTrackNameInfo(CWnd* pParent /*=NULL*/)
  : CDialog(CTrackNameInfo::IDD, pParent)
{
  //{{AFX_DATA_INIT(CTrackNameInfo)
  m_Real = FALSE;
  m_Author = _T("");
  m_Country = _T("");
  m_Name = _T("");
  m_Event = _T("");
  m_Year = _T("");
  m_Desc = _T("");
  m_Copyright = _T("");
  m_Laps = _T("");
  m_Tyre = 0;
  m_slot = 1;
  m_LapRecord = _T("");
  m_LapRecordQualify = _T("");
  m_Split1 = 0;
  m_Split2 = 0;
  //}}AFX_DATA_INIT
}

void CTrackNameInfo::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CTrackNameInfo)
  DDX_Check(pDX, IDC_CHECK1, m_Real);
  DDX_Text(pDX, IDC_TRACK_AUTHOR, m_Author);
  DDV_MaxChars(pDX, m_Author, 128);
  DDX_Text(pDX, IDC_TRACK_COUNTRY, m_Country);
  DDV_MaxChars(pDX, m_Country, 128);
  DDX_Text(pDX, IDC_TRACK_NAME, m_Name);
  DDV_MaxChars(pDX, m_Name, 128);
  DDX_Text(pDX, IDC_TRACK_EVENT, m_Event);
  DDV_MaxChars(pDX, m_Event, 128);
  DDX_Text(pDX, IDC_TRACK_YEAR, m_Year);
  DDV_MaxChars(pDX, m_Year, 128);
  DDX_Text(pDX, IDC_TRACK_DESC, m_Desc);
  DDV_MaxChars(pDX, m_Desc, 256);
  DDX_Text(pDX, IDC_COPYRIGHT, m_Copyright);
  DDV_MaxChars(pDX, m_Copyright, 128);
  DDX_Text(pDX, IDC_TRACK_LAPS, m_Laps);
  DDV_MaxChars(pDX, m_Laps, 4);
  DDX_Text(pDX, IDC_TRACKTYRE_CONSUMPTION, m_Tyre);
  DDX_Text(pDX, IDC_TRACKSLOT, m_slot);
  DDX_Text(pDX, IDC_LAPRECORD, m_LapRecord);
  DDX_Text(pDX, IDC_LAPRECORD2, m_LapRecordQualify);
  DDX_Text(pDX, IDC_SPLIT1, m_Split1);
  DDX_Text(pDX, IDC_SPLIT2, m_Split2);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTrackNameInfo, CDialog)
//{{AFX_MSG_MAP(CTrackNameInfo)
// NOTE: the ClassWizard will add message map macros here
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackNameInfo message handlers
