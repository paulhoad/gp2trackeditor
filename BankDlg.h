// BankDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBankDlg dialog

class CBankDlg : public CDialog
{
  // Construction
public:
  CBankDlg(int begin, int end, CWnd* pParent = NULL);// standard constructor

  // Dialog Data
  //{{AFX_DATA(CBankDlg)
  enum {
    IDD = IDD_BANKEDIT
  };
  int m_LeftBank;
  int m_RightBank;
  int m_From;
  int m_To;
  //}}AFX_DATA

  BOOL m_ChangeLeft;
  BOOL m_ChangeRight;

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CBankDlg)
protected:
  virtual void
    DoDataExchange(CDataExchange* pDX);// DDX/DDV support
    //}}AFX_VIRTUAL

  // Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CBankDlg)
  virtual BOOL
    OnInitDialog();
  virtual void
    OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
