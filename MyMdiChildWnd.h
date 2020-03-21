// MyMdiChildWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyMdiChildWnd frame

class CMyMdiChildWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CMyMdiChildWnd)
protected:
	CMyMdiChildWnd();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyMdiChildWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyMdiChildWnd();

	// Generated message map functions
	//{{AFX_MSG(CMyMdiChildWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
