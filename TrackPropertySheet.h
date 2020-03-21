// TrackTree.h : header file
//

#ifndef _TRACK_PROPERTY_H_
#define _TRACK_PROPERTY_H_

#include "GPTrack.h"
#include "sizecbar.h"
#include "TrackTreeCtrl.h"
#include "Observer.h"
#include "MyListCtrl.h"


/////////////////////////////////////////////////////////////////////////////
// CTrackPropertySheet dialog

class CTrackPropertySheet : public CSizingControlBar
{
// Construction
public:

	CTrackPropertySheet();   // standard constructor
    ~CTrackPropertySheet();

// Dialog Data
	//{{AFX_DATA(CTrackPropertySheet)
	CMyListCtrl m_TrackProperty;
	//}}AFX_DATA

	//virtual CSize CalcDynamicLayout(int nLength, DWORD nMode);

	CImageList* pImageList;
	
	BOOL ImagesNotLoaded;

	void UpdateInfo();

	void LoadTable(Observer *obj);

	void InsertTableString(int row,int index,LPSTR name,LPARAM extraData);
	void InsertTableString(int row,int index,CString name,LPARAM extraData);
	void EmptyTable();




	//BOOL Create(CWnd* pParent , char * Id, int flags, int CId);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrackPropertySheet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CTrackPropertySheet)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
