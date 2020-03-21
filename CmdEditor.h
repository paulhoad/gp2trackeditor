// CmdEditor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCmdEditor dialog

class CCmdEditor : public CDialog
{
// Construction
public:
	CCmdEditor(CWnd* pParent = NULL,int numArgs=0);   // standard constructor

	int numArgs;

// Dialog Data
	//{{AFX_DATA(CCmdEditor)
	enum { IDD = IDD_CMD_EDITOR };
	CEdit	m_Ar1;
	CEdit	m_Ar2;
	CEdit	m_Ar3;
	CEdit	m_Ar4;
	CEdit	m_Ar5;
	CEdit	m_Ar6;
	CEdit	m_Ar7;
	CEdit	m_Ar8;
	CEdit	m_Ar9;
	CEdit	m_Ar10;
	CEdit	m_Ar11;
	CEdit	m_Ar12;
	CEdit	m_Ar13;
	short		m_Arg4;
	short		m_Arg5;
	short		m_Arg6;
	short		m_Arg7;
	short		m_Arg8;
	short		m_Cmd;
	CString	m_Desc2;
	CString	m_Desc1;
	CString	m_Desc3;
	CString	m_Desc4;
	CString	m_Desc5;
	CString	m_Desc6;
	CString	m_Desc7;
	CString	m_Desc8;
	CString	m_DescriptionText;
	CString	m_Desc10;
	CString	m_Desc11;
	CString	m_Desc12;
	CString	m_Desc13;
	CString	m_Desc9;
	short	m_Arg11;
	short	m_Arg10;
	short	m_Arg1;
	short	m_Arg12;
	short	m_Arg13;
	short	m_Arg2;
	short	m_Arg3;
	short	m_Arg9;
	short	m_Arg14;
	CString	m_Desc14;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdEditor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCmdEditor)
	virtual BOOL OnInitDialog();
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
