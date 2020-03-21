// RandomObject.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRandomObject dialog

class CRandomObject : public CDialog
{
// Construction
public:
	CRandomObject(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRandomObject)
	enum { IDD = IDD_RANDOM_OBJ };
	int		m_ObjectsPerSection;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRandomObject)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRandomObject)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
