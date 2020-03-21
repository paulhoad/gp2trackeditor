// Info.h : header file
//
#ifndef _INCLUDED_CINFO_INFO_H_
#define _INCLUDED_CINFO_INFO_H_

#include "CLineEditor.h"

/////////////////////////////////////////////////////////////////////////////
// CInfo view
#include "MyTableView.h"

class CInfo : public CMyTableView
{
protected:
  CInfo();// protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CInfo)

  // Attributes
public:
  CCLineEditor* cclinedlg;

  // Operations
public:
  CString
    toString(int data);
  CString
    toString(double data);

  void UpdateInfo(BOOL);
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CInfo)
public:
  virtual BOOL
    OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
  virtual BOOL
    Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

protected:
  virtual void
    OnDraw(CDC* pDC);// overridden to draw this view
  virtual void
    OnInitialUpdate();// first time after construct
  virtual BOOL
    OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
  //}}AFX_VIRTUAL

  // Implementation
protected:
  virtual ~CInfo();
#ifdef _DEBUG
  virtual void
    AssertValid() const;
  virtual void
    Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
  //{{AFX_MSG(CInfo)
  afx_msg void
    OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void
    OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void
    OnSetFocus(CWnd* pOldWnd);
  afx_msg void
    OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void
    OnExportTable();
  afx_msg int
    OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void
    OnImportTable();
  afx_msg void
    OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif
