// KerbEditor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKerbEditor dialog

class CKerbEditor : public CDialog
{
// Construction
public:
	CKerbEditor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CKerbEditor)
	enum { IDD = IDD_KERB_EDITOR };
	int		m_X1;
	int		m_X2;
	int		m_Y1;
	int		m_Y2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKerbEditor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKerbEditor)
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
