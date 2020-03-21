// TrackStats.h : header file
//
#include "GPTrack.h"
#include "TrackSection.h"

#include "Ruler.h"

/////////////////////////////////////////////////////////////////////////////
// CTrackStats dialog

class CTrackStats : public CDialog
{
  // Construction
public:
  CTrackStats(CWnd *pParent = NULL);// standard constructor

  void
    setTrack(GPTrack *mytrack)
  {
    m_TrackName = CString((LPCSTR)mytrack->getCircuitName());
    m_TrackCountry = CString((LPCSTR)mytrack->getCircuitCountry());
    m_TrackSectionsStr = CString((LPCSTR)mytrack->getTrackSectionsStr());
    m_PitSectionsStr = CString((LPCSTR)mytrack->getPitSectionsStr());
    m_CCSectionsStr = CString((LPCSTR)mytrack->getCCSectionsStr());
    m_NumObjects = CString((LPCSTR)mytrack->getObjectDefinitionsStr());

    // char buffer[10];

    // wsprintf(buffer,"%d",mytrack->totalLength());
    // m_TrackLengthStr = CString(buffer);
    m_TrackLengthStr.Format("%d", mytrack->totalLength());

    m_TrackMaxStLen = mytrack->MaxStraightLength();
    m_TrackMinStLen = mytrack->MinStraightLength();
    m_TrackMaxCuLen = mytrack->MaxCurveLength();
    m_TrackMinCuLen = mytrack->MinCurveLength();

    m_TrackLenKm = (double)(LENGTH_TO_KM * mytrack->totalLength()) / 1000.0;
    m_TrackLenKmPits =
      (double)(LENGTH_TO_KM * mytrack->totalPitLength()) / 1000.0;
    m_TrackLenKmBoth = m_TrackLenKm + m_TrackLenKmPits;
    m_TrackLenMiles =
      (double)(LENGTH_TO_KM * mytrack->totalLength()) / METERES_IN_MILE;

    double totalheight = 0;
    double totalangle = 0;
    double totalaltitude = 0;
    for (int i = 0; i < mytrack->TrackSections->size(); i++) {
      TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);
      totalheight += ((double)t->getLength() * (double)t->getHeight());
      totalangle += ((double)t->getLength() * (double)t->getAngle());
    }
    TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(
      mytrack->TrackSections->size() - 1);
    totalaltitude = t->getTrueAltitude();

    char buff[256];

    // sprintf(buff,);
    m_TotalHeight.Format("%2.2f", totalheight);
    // sprintf(buff,"%f",totalangle);
    // m_TotalAngle  = CString(buff);
    m_TotalAngle.Format("%2.2f", totalangle);
    // sprintf(buff,"%f",totalaltitude);
    // m_TotalAltitude  = CString(buff);
    m_TotalAltitude.Format("%2.2f", totalaltitude);

    int trackRads = 0;
    int pitRads = 0;
    int ccRads = 0;

    for (int i = 0; i < mytrack->TrackSections->size(); i++) {
      TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);
      if (t->getAngle() != 0) trackRads++;
    }
    for (int i = 0; i < mytrack->PitlaneSections->size(); i++) {
      TrackSection *t = (TrackSection *)mytrack->PitlaneSections->elementAt(i);
      if (t->getAngle() != 0) pitRads++;
    }
    for (int i = 0; i < mytrack->CCLineSections->size(); i++) {
      CCLineSection *t = (CCLineSection *)mytrack->CCLineSections->elementAt(i);
      if (t->getRadius() != 0) ccRads++;
    }

    sprintf(buff, "%d", trackRads);
    m_TrackRads = CString(buff);
    sprintf(buff, "%d", pitRads);
    m_PitRads = CString(buff);
    sprintf(buff, "%d", ccRads);
    m_CCRads = CString(buff);

    m_TrackCmds.Format("%d", mytrack->totalTrackCommands());
    m_PitCmds.Format("%d", mytrack->totalPitCommands());
    m_TotalCmds.Format("%d", mytrack->totalCommands());
  }

  // Dialog Data
  //{{AFX_DATA(CTrackStats)
  enum {
    IDD = IDD_STATS
  };
  CString m_TrackName;
  CString m_TrackCountry;
  CString m_NumObjects;
  CString m_CCSectionsStr;
  CString m_PitSectionsStr;
  CString m_TrackSectionsStr;
  CString m_TrackLengthStr;
  int m_TrackMinStLen;
  int m_TrackMaxStLen;
  int m_TrackMinCuLen;
  int m_TrackMaxCuLen;
  double m_TrackLenKm;
  double m_TrackLenMiles;
  double m_TrackLenKmPits;
  double m_TrackLenKmBoth;
  CString m_TotalHeight;
  CString m_TotalAngle;
  CString m_TotalAltitude;
  CString m_CCRads;
  CString m_PitRads;
  CString m_TrackRads;
  CString m_TrackCmds;
  CString m_PitCmds;
  CString m_TotalCmds;

  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTrackStats)
protected:
  virtual void
    DoDataExchange(CDataExchange *pDX);// DDX/DDV support
    //}}AFX_VIRTUAL

  // Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CTrackStats)
  // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
