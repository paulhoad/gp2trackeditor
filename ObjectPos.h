// ObjectPos.h : header file
//
#include "GPTrack.h"
#include "TrackObject.h"


/////////////////////////////////////////////////////////////////////////////
// CObjectPos dialog

class CObjectPos : public CDialog
{
// Construction
public:
	CObjectPos(GPTrack *track,CWnd* pParent = NULL);   // standard constructor

	GPTrack *track;
	int locatorStart;

	//CToolTipWnd m_BalloonToolTip;

	void setSection(TrackObject *t,int idx=-1)
	{
		TrackObjectDefinition *objdef = t->getObjectDef();

		if (objdef)
		{
		 m_Desc = t->getObjectDef()->getName();
		 m_Distance = t->getDistance();
		 locatorStart = 0;

 		 locatorStart = (int)t->getLocator();
		}
		else
		{
         AfxMessageBox("Bad Object Definition offset selecting you one!");
		 m_Desc = "Bad Object";
		 m_Distance = t->getDistance();
		 locatorStart = 0;

 		 locatorStart = (int)t->getLocator();
		}

	}

	void getSection(TrackObject *t)
	{
	    t->setLocator(locatorStart);
		t->setDistance(m_Distance);		
	}

// Dialog Data
	//{{AFX_DATA(CObjectPos)
	enum { IDD = IDD_OBJ_POS };
	CSpinButtonCtrl	m_PositionSpin;
	CComboBox	m_ObjectCombo;
	int		m_Distance;
	CString	m_Desc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectPos)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CObjectPos)
	afx_msg void OnEdit();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnNewdesc();
	afx_msg void OnSelchangeLocator();
	afx_msg void OnObjhelp();
	afx_msg void OnEditupdateLocator();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
