// PitInfo.h : header file
//
#ifndef _INCLUDED_CPIT_INFO_H_
#define _INCLUDED_CPIT_INFO_H_

#include "TrChangeDlg.h"
#include "MyTableView.h"

/////////////////////////////////////////////////////////////////////////////
// CPitInfo view

class CPitInfo : public CMyTableView
{
protected:
  CPitInfo();// protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CPitInfo)

  // CTable *table;
  CTrChangeDlg* trkdlg;

  /*
  CString toString(int data);
  CString toString(double data);
  CString toHexString(int data);
  */

  void UpdateInfo(BOOL);

  // Attributes
public:
  // Operations
public:
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CPitInfo)
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
  virtual ~CPitInfo();
#ifdef _DEBUG
  virtual void
    AssertValid() const;
  virtual void
    Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
  //{{AFX_MSG(CPitInfo)
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
    OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void
    OnImportTable();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif
