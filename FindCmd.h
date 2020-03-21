// FindCmd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFindCmd dialog

class CFindCmd : public CDialog
{
  // Construction
public:
  CFindCmd(CWnd* pParent = NULL);// standard constructor

  // Dialog Data
  //{{AFX_DATA(CFindCmd)
  enum {
    IDD = IDD_FINDCOMMAND
  };
  CString m_FindCmd;
  //}}AFX_DATA

  int CmdToFind;

  int getCmd()
  {
    return CmdToFind;
  }

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFindCmd)
protected:
  virtual void
    DoDataExchange(CDataExchange* pDX);// DDX/DDV support
    //}}AFX_VIRTUAL

  // Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CFindCmd)
  virtual void
    OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
