// GeneralDataEnry.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGeneralDataEnry dialog

class CGeneralDataEnry : public CDialog
{
// Construction
public:
	CGeneralDataEnry(CString title,CString label1,int val1,CString label2,int val2,CString label3,int val3,CWnd* pParent = NULL);

	int getValue(int idx);

	int vals[10];

	CString title;

	BOOL ProcessEntry();
// Dialog Data
	//{{AFX_DATA(CGeneralDataEnry)
	enum { IDD = IDD_DATAENTRY };
	CString	m_Label1;
	CString	m_Label2;
	CString	m_Label3;
	int		m_EditVal1;
	int		m_EditVal2;
	UINT	m_EditVal3;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGeneralDataEnry)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGeneralDataEnry)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
