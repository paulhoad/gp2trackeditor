// TrackNameInfo.h : header file
//
#include "GPTrack.h"

/////////////////////////////////////////////////////////////////////////////
// CTrackNameInfo dialog

class CTrackNameInfo : public CDialog
{
  // Construction
public:
  CTrackNameInfo(CWnd* pParent = NULL);// standard constructor

  void
    setTrack(GPTrack* mytrack)
  {
    m_Name = CString((LPCSTR)mytrack->getCircuitName());
    m_Country = CString((LPCSTR)mytrack->getCircuitCountry());
    m_Year = CString((LPCSTR)mytrack->getCircuitYears());
    m_Real = mytrack->getCircuitReal();
    m_Event = CString((LPCSTR)mytrack->getCircuitEvent());
    m_Author = CString((LPCSTR)mytrack->getCircuitAuthor());
    m_Desc = CString((LPCSTR)mytrack->getCircuitDesc());
    m_Copyright = CString((LPCSTR)mytrack->getCircuitCopyright());
    m_Laps = CString((LPCSTR)mytrack->getCircuitLapsStr());
    m_slot = mytrack->getBestSlotInfo();
    m_Tyre = mytrack->getTyreConsumption();
    m_LapRecord = mytrack->getLapRecord();
    m_LapRecordQualify = mytrack->getLapRecordQualify();
    m_Split1 = mytrack->split1;
    m_Split2 = mytrack->split2;
  }

  void
    getTrackInfo(GPTrack* mytrack)
  {
    // track->setTrackName();
    mytrack->setCircuitName(m_Name);
    mytrack->setCircuitCountry(m_Country);
    mytrack->setCircuitYears(m_Year);
    mytrack->setCircuitEvent(m_Event);
    mytrack->setCircuitAuthor(m_Author);
    mytrack->setCircuitReal(m_Real);
    mytrack->setCircuitDesc(m_Desc);
    mytrack->setCircuitLaps(atoi(m_Laps));
    mytrack->setBestSlotInfo(m_slot);
    mytrack->setTyreConsumption(m_Tyre);
    mytrack->setLapRecord(m_LapRecord);
    mytrack->setLapRecordQualify(m_LapRecordQualify);
    mytrack->split1 = m_Split1;
    mytrack->split2 = m_Split2;
  }

  // Dialog Data
  //{{AFX_DATA(CTrackNameInfo)
  enum {
    IDD = IDD_TRACK_INFO
  };
  BOOL m_Real;
  CString m_Author;
  CString m_Country;
  CString m_Name;
  CString m_Event;
  CString m_Year;
  CString m_Desc;
  CString m_Copyright;
  CString m_Laps;
  int m_Tyre;
  int m_slot;
  CString m_LapRecord;
  CString m_LapRecordQualify;
  int m_Split1;
  int m_Split2;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTrackNameInfo)
protected:
  virtual void
    DoDataExchange(CDataExchange* pDX);// DDX/DDV support
    //}}AFX_VIRTUAL

  // Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CTrackNameInfo)
  // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
