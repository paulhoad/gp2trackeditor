// MoveTo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMoveTo dialog

class CMoveTo : public CDialog
{
  // Construction
public:
  CMoveTo(CWnd* pParent = NULL);// standard constructor

  // Dialog Data
  //{{AFX_DATA(CMoveTo)
  enum {
    IDD = IDD_MoveTo
  };
  int m_MoveTo;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMoveTo)
protected:
  virtual void
    DoDataExchange(CDataExchange* pDX);// DDX/DDV support
    //}}AFX_VIRTUAL

  // Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CMoveTo)
  // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
