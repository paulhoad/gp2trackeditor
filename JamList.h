// JamList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJamList dialog

class CJamList : public CDialog
{
// Construction
public:
	CJamList(GPTrack *track,CWnd* pParent = NULL);   // standard constructor

	~CJamList();

	GPTrack *track;

	int currentSelectionIndex;

// Dialog Data
	//{{AFX_DATA(CJamList)
	enum { IDD = IDD_JAMLIST };
	CComboBox	m_UnusedList;
	CListCtrl	m_ListBox;
	CString	m_Index;
	CString	m_Filename;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJamList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJamList)
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
