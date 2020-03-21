// DataEditor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataEditor dialog

class CDataEditor : public CDialog
{
  // Construction
public:
  CDataEditor(CWnd* pParent = NULL);// standard constructor

  // Dialog Data
  //{{AFX_DATA(CDataEditor)
  enum {
    IDD = IDD_VARDATAEDITOR
  };
  int m_NewValue;
  CString m_Desc;
  CString m_DataName;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDataEditor)
protected:
  virtual void
    DoDataExchange(CDataExchange* pDX);// DDX/DDV support
    //}}AFX_VIRTUAL

  // Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CDataEditor)
  // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
