// ViewSections.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewSections dialog

class CViewSections : public CDialog
{
// Construction
public:
	CViewSections(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CViewSections)
	enum { IDD = IDD_VIEWSECTONS };
	int		m_From;
	int		m_To;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewSections)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CViewSections)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
