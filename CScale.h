// CScale.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCScale dialog

class CCScale : public CDialog
{
// Construction
public:
	CCScale(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCScale)
	enum { IDD = IDD_CCSCALE };
	float	m_CCScale;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCScale)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCScale)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
