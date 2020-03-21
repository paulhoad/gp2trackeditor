// Sanity.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSanity dialog

class CSanity : public CDialog
{
  // Construction
public:
  CSanity(CWnd* pParent = NULL);// standard constructor

  // Dialog Data
  //{{AFX_DATA(CSanity)
  enum {
    IDD = IDD_SANITY
  };
  CListBox m_SanityList;
  //}}AFX_DATA

  void
    Output(CString message);

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CSanity)
protected:
  virtual void
    DoDataExchange(CDataExchange* pDX);// DDX/DDV support
    //}}AFX_VIRTUAL

  // Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CSanity)
  virtual void
    OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
