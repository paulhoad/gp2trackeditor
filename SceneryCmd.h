// SceneryCmd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSceneryCmd dialog

class CSceneryCmd : public CDialog
{
  // Construction
public:
  CSceneryCmd(CWnd* pParent = NULL);// standard constructor

  // Dialog Data
  //{{AFX_DATA(CSceneryCmd)
  enum {
    IDD = IDD_CMD_SCENERYEDITOR
  };
  // NOTE: the ClassWizard will add data members here
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CSceneryCmd)
protected:
  virtual void
    DoDataExchange(CDataExchange* pDX);// DDX/DDV support
    //}}AFX_VIRTUAL

  // Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CSceneryCmd)
  // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
