// SceneryEditor.h : header file
//
#include "GPTrack.h"

/////////////////////////////////////////////////////////////////////////////
// CSceneryEditor dialog

class CSceneryEditor : public CDialog
{
// Construction
public:
	CSceneryEditor(GPTrack *mytrack,BOOL pitlane=FALSE,int StartingPosition=0,int StartingCmd=1,CWnd* pParent = NULL);   // standard constructor

	~CSceneryEditor();

// Dialog Data
	//{{AFX_DATA(CSceneryEditor)
	enum { IDD = IDD_SCENERYEDITOR };
	CButton	m_DrawBox;
	int m_Index;
	int		m_RibbonsRight;
	CString	m_NumCmds;
	BOOL	m_ShowHidden;
	CString m_EditingCmds;
	//}}AFX_DATA

	GPTrack *track;
	int  Editing;
	BOOL pitlane;

	void DrawScenery(CDC* pDC,RECT &rect);
	void DrawProfile(CDC* pDC ,RECT &rect);
	void DrawRibbonState(CDC* pDC ,RECT &rect);

	TrackSection *currentSection;
	TrackCmd *currentCmd;

	BOOL edited;

	void UpdateDialogBoxes();

	short x[6];
	short z[6];

	CPen *bluePen;
	CBrush *redBrush;

	


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSceneryEditor)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSceneryEditor)
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnNext();
	afx_msg void OnPrev();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnApply();
	afx_msg void OnChangeX1();
	afx_msg void OnChangeX2();
	afx_msg void OnChangeX3();
	afx_msg void OnChangeX4();
	afx_msg void OnChangeX5();
	afx_msg void OnChangeX6();
	afx_msg void OnChangeZ1();
	afx_msg void OnChangeZ2();
	afx_msg void OnChangeZ3();
	afx_msg void OnChangeZ4();
	afx_msg void OnChangeZ5();
	afx_msg void OnChangeZ6();
	afx_msg void OnChangeRibbonsRight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
