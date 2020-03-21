// SceneryOnOff.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSceneryOnOff dialog

class CSceneryOnOff : public CDialog
{
// Construction
public:
	CSceneryOnOff(GPTrack *mytrack,int offset,int value,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSceneryOnOff)
	enum { IDD = IDD_SCENERYONOFF };
	BOOL	m_LeftBank;
	BOOL	m_Ribbon1;
	BOOL	m_Ribbon2;
	BOOL	m_Ribbon3;
	BOOL	m_Ribbon4;
	BOOL	m_RightBank;
	CString	m_OnOff;
	int     m_Offset;
	//}}AFX_DATA

	GPTrack *track;
	int val;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSceneryOnOff)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSceneryOnOff)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
