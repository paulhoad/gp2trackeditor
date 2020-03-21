// TableRowElement.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTableRowElement window

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CTableRowElement : public CWnd
{
// Construction
public:
	CTableRowElement(CString *name,int icon=-1,WORD extraData=NULL);

// Attributes
public:

	CString *name;
	int icon;

	CString *getValue()
	{
		return name;
	}

	WORD extraData;

	void SetItemData(WORD data)
	{
		extraData = data;
	}

	WORD GetItemData()
	{
		return extraData;
	}

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTableRowElement)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTableRowElement();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTableRowElement)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
