// CommandStats.h : header file
//

#ifndef _INCLUDED_COMMAND_STATS_H_
#define _INCLUDED_COMMAND_STATS_H_

#include "MyTableView.h"

/////////////////////////////////////////////////////////////////////////////
// CCommandStats view

class CCommandStats : public CMyTableView
{
protected:
  CCommandStats();// protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CCommandStats)

  // Attributes
public:
  void UpdateInfo(BOOL);

  // Operations
public:
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CCommandStats)
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
    //}}AFX_VIRTUAL

  // Implementation
protected:
  virtual ~CCommandStats();
#ifdef _DEBUG
  virtual void
    AssertValid() const;
  virtual void
    Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
  //{{AFX_MSG(CCommandStats)
  afx_msg void
    OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void
    OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void
    OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void
    OnExportTable();
  afx_msg int
    OnCreate(LPCREATESTRUCT lpCreateStruct);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif
