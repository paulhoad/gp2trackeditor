// ObjectView.h : header file
//
#ifndef _INCLUDED_OBJECT_VIEW_H
#define _INCLUDED_OBJECT_VIEW_H

#include "InternalObject.h"
#include "MyTableView.h"

/////////////////////////////////////////////////////////////////////////////
// CObjectView view

class CObjectView : public CMyTableView
{
protected:
	CObjectView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CObjectView)

// Attributes
public:
	InternalObject *inter_obj;

// Operations
public:
	void UpdateInfo(BOOL);

	void SetObject(InternalObject *obj)
	{
		inter_obj = obj;
	}

	InternalObject * GetObject()
	{
		return inter_obj;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectView)
	public:
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CObjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CObjectView)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnExportTable();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif
