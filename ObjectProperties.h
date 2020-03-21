// ObjectProperties.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObjectProperties dialog
class InternalObject;

class CObjectProperties : public CDialog
{
// Construction
public:
	CObjectProperties(InternalObject *obj,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectProperties)
	enum { IDD = IDD_OBJECT_PROPERTIES };
	CListBox	m_ObjList;
	CString	m_ObjName;
	//}}AFX_DATA

	InternalObject *obj;
	int jamId;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CObjectProperties)
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnSelchangeList1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
