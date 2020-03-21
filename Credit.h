// Credit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCredit dialog

class CCredit : public CDialog
{
  // Construction
public:
  CCredit(CWnd* pParent = NULL);// standard constructor

  // Dialog Data
  //{{AFX_DATA(CCredit)
  enum {
    IDD = IDD_CREDITS
  };
  // NOTE: the ClassWizard will add data members here
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CCredit)
protected:
  virtual void
    DoDataExchange(CDataExchange* pDX);// DDX/DDV support
    //}}AFX_VIRTUAL

  // Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CCredit)
  // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
