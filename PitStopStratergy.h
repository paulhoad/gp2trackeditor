// PitStopStratergy.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPitStopStratergy dialog

class CPitStopStratergy : public CDialog
{
  // Construction
public:
  CPitStopStratergy(GPTrack* mytrack,
    CWnd* pParent = NULL);// standard constructor

  // Dialog Data
  //{{AFX_DATA(CPitStopStratergy)
  enum {
    IDD = IDD_PITSTOP
  };
  CButton m_OkButton;
  CSpinButtonCtrl m_ThreeStopPercentageSpin;
  CSpinButtonCtrl m_TwoStopPercentageSpin;
  CSpinButtonCtrl m_ThreeStopDurThreeSpin;
  CSpinButtonCtrl m_ThreeStopDurTwoSpin;
  CSpinButtonCtrl m_ThreeStopDurOneSpin;
  CSpinButtonCtrl m_TwoStopDurTwoSpin;
  CSpinButtonCtrl m_TwoStopDurOneSpin;
  CSpinButtonCtrl m_OneStopDurOneSpin;
  CSpinButtonCtrl m_ThreeStopBeginThreeSpin;
  CSpinButtonCtrl m_ThreeStopBeginTwoSpin;
  CSpinButtonCtrl m_ThreeStopBeginOneSpin;
  CSpinButtonCtrl m_TwoStopBeginTwoSpin;
  CSpinButtonCtrl m_TwoStopBeginOneSpin;
  CSpinButtonCtrl m_OneStopBeginOneSpin;
  CSpinButtonCtrl m_OneStopPercentageSpin;
  CSpinButtonCtrl m_LapSpin;
  int m_OneStopPercentage;
  int m_ThreeStopDurOne;
  int m_ThreeStopDurThree;
  int m_ThreeStopBeginThree;
  int m_ThreeStopBeginTwo;
  int m_ThreeStopPercentage;
  int m_TwoStopPercentage;
  int m_TwoStopDurOne;
  int m_TwoStopDurTwo;
  int m_TwoStopBeginTwo;
  int m_TwoStopBeginOne;
  int m_ThreeStopBeginOne;
  int m_ThreeStopDurTwo;
  int m_OneStopBeginOne;
  int m_OneStopDurOne;
  int m_Laps;
  //}}AFX_DATA

  GPTrack* track;

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CPitStopStratergy)
protected:
  virtual void
    DoDataExchange(CDataExchange* pDX);// DDX/DDV support
    //}}AFX_VIRTUAL

  // Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CPitStopStratergy)
  virtual void
    OnOK();
  virtual BOOL
    OnInitDialog();
  afx_msg void
    OnChangeThreestop();
  afx_msg void
    OnChangeOnestop();
  afx_msg void
    OnChangeTwostop();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
