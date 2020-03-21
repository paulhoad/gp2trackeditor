// PitEntrance.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPitEntrance dialog

class CPitEntrance : public CDialog
{
// Construction
public:
	CPitEntrance(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPitEntrance)
	enum { IDD = IDD_PITENTRACE };
	int		m_Entrance;
	int		m_Exit;
	//}}AFX_DATA


	BOOL LeaveToLeft;
	BOOL EnterToLeft;



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPitEntrance)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPitEntrance)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
