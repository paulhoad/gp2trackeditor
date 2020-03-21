// TrackWidth.h : header file
//
#include "Observer.h"

/////////////////////////////////////////////////////////////////////////////
// CTrackWidth dialog

class CTrackWidth : public CDialog
  , Observer
{
  // Construction
public:
  CTrackWidth(GPTrack* track, CWnd* pParent = NULL);// standard constructor

  int Open();

  // Dialog Data
  //{{AFX_DATA(CTrackWidth)
  enum {
    IDD = IDD_WIDTTEDIT
  };
  int m_TrackWidthLeft;
  int m_TrackWidthRight;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTrackWidth)
protected:
  virtual void
    DoDataExchange(CDataExchange* pDX);// DDX/DDV support
    //}}AFX_VIRTUAL

  // Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CTrackWidth)
  // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
