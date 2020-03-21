// Register.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegister dialog

class CRegister : public CDialog
{
// Construction
public:
	CRegister(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRegister)
	enum { IDD = IDD_REGISTER };
	CEdit	m_Message;
	CEdit	m_RegistrationName;
	CEdit	m_RegistrationNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegister)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegister)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
