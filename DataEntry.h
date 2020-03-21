// DataEntry.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataEntry dialog

class CDataEntry : public CDialog
{
  // Construction
public:
  CDataEntry(CWnd* pParent = NULL);// standard constructor

  // Dialog Data
  //{{AFX_DATA(CDataEntry)
  enum {
    IDD = IDD_DATAEDITOR
  };
  int m_NewValue;
  CString m_PreviousValue;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDataEntry)
protected:
  virtual void
    DoDataExchange(CDataExchange* pDX);// DDX/DDV support
    //}}AFX_VIRTUAL

  // Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CDataEntry)
  // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
