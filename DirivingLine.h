// DirivingLine.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDirivingLine dialog

class CDirivingLine : public CDialog
{
  // Construction
public:
  CDirivingLine(CWnd* pParent = NULL);// standard constructor

  // Dialog Data
  //{{AFX_DATA(CDirivingLine)
  enum {
    IDD = IDD_DRIVINGLINE
  };
  int m_Distance;
  CString m_Side;
  int m_DistanceFromSF;
  //}}AFX_DATA

  int beginYPos;
  int beginXPos;
  int PoleValue;

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDirivingLine)
protected:
  virtual void
    DoDataExchange(CDataExchange* pDX);// DDX/DDV support
    //}}AFX_VIRTUAL

  // Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CDirivingLine)
  virtual BOOL
    OnInitDialog();
  virtual void
    OnOK();
  afx_msg void
    OnCCChange();
  afx_msg void
    OnPoleChange();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
