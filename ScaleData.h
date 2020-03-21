// ScaleData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScaleData dialog

class CScaleData : public CDialog
{
// Construction
public:
	CScaleData(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScaleData)
	enum { IDD = IDD_SCALEDATA };
	float	m_ScaleVal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScaleData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScaleData)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
