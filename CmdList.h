// CmdList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCmdList dialog

class CCmdList : public CDialog
{
// Construction
public:
	CCmdList(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCmdList)
	enum { IDD = IDD_CMDLIST };
	CListCtrl	m_CmdList;
	//}}AFX_DATA

	int SelectedItem;
	int SelectedItemCmd;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdList)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCmdList)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHelp();
	afx_msg void OnHelp1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
