// CameraEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCameraEdit dialog

#ifndef _INCLUDED_CAMERA_EDIT_H
#define _INCLUDED_CAMERA_EDIT_H

class CCameraEdit : public CDialog
{
// Construction
public:
	CCameraEdit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCameraEdit)
	enum { IDD = IDD_CAMERA_DLG };
	int		m_Number;
	int		m_SM;
	int		m_UK1;
	int		m_UK2;
	int		m_VR1;
	int		m_VR2;
	int		m_X;
	int		m_Y;
	int		m_Z1;
	int		m_ZF1;
	int		m_ZF2;
	int		m_Z2;
	int		m_UK3;
	int		m_UK4;
	int		m_UK5;
	int		m_UK6;
	int		m_UK7;
	CString	m_TrackLength;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCameraEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCameraEdit)
	afx_msg void OnClickCenterDistanceSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
