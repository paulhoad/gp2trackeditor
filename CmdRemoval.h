// CmdRemoval.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCmdRemoval dialog

class CCmdRemoval : public CDialog
{
// Construction
public:
	CCmdRemoval(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCmdRemoval)
	enum { IDD = IDD_CMDRMV };
	CListCtrl	m_CmdList;
	CString	m_CmdVal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdRemoval)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCmdRemoval)
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
