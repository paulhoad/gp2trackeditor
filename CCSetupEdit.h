// CCSetupEdit.h : header file
//
#include "GPTrack.h"


/////////////////////////////////////////////////////////////////////////////
// CCCSetupEdit dialog

class CCCSetupEdit : public CDialog
{
// Construction
public:
	CCCSetupEdit(GPTrack *mytrack,CWnd* pParent = NULL);   // standard constructor

	GPTrack *track;
// Dialog Data
	//{{AFX_DATA(CCCSetupEdit)
	enum { IDD = IDD_CCSETUP };
	CSpinButtonCtrl	m_GripSpin;
	CSpinButtonCtrl	m_Gear6Spin;
	CSpinButtonCtrl	m_Gear5Spin;
	CSpinButtonCtrl	m_Gear4Spin;
	CSpinButtonCtrl	m_Gear3Spin;
	CSpinButtonCtrl	m_Gear2Spin;
	CSpinButtonCtrl	m_Gear1Spin;
	CButton	m_FuelSpin;
	CSpinButtonCtrl	m_BrakeBalSpin;
	CSpinButtonCtrl	m_AirSpin;
	CSpinButtonCtrl	m_RearWingSpin;
	CSpinButtonCtrl	m_FrontWingSpin;
	CSpinButtonCtrl	m_AccelSpin;
	CComboBox	m_TyreCombo;
	int		m_Air;
	int		m_Accel;
	int		m_FrontWing;
	int		m_Gear1;
	int		m_Gear2;
	int		m_Gear3;
	int		m_Gear4;
	int		m_Gear5;
	int		m_Gear6;
	int		m_Grip;
	int		m_RearWing;
	int		m_Tyre;
	int		m_Fuel;
	int		m_BrakeBalance;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCCSetupEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCCSetupEdit)
	afx_msg void OnCorrectFuel();
	afx_msg void OnSelchangeTyrecombo();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
