// ObjectInfo.h : header file
//
#ifndef _INCLUDED_COBJECT_INFO_H_
#define _INCLUDED_COBJECT_INFO_H_

#include "MyTableView.h"
#include "ObjectEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CObjectInfo view

class CObjectInfo : public CMyTableView
{
protected:
	CObjectInfo();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CObjectInfo)

// Attributes
public:

// Operations
public:

	void UpdateInfo(BOOL);

	CObjectEdit *trkdlg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectInfo)
	public:
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CObjectInfo();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CObjectInfo)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnExportTable();
	afx_msg void OnNewObjectDefinition();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif
