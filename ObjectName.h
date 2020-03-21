// ObjectName.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObjectName dialog

class CObjectName : public CDialog
{
// Construction
public:
	CObjectName(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectName)
	enum { IDD = IDD_OBJECTNAME };
	CListBox	m_ObjList;
	CString	m_ObjName;
	//}}AFX_DATA

	int selectedBitmap;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CObjectName)
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeList1();
	afx_msg void OnChangeEdit1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
