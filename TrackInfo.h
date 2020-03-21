// TrackInfo.h : header file
//
#ifndef _INCLUDED_CTRACK_INFO_H_
#define _INCLUDED_CTRACK_INFO_H_

#include "TrChangeDlg.h"
#include "MyTableView.h"

/////////////////////////////////////////////////////////////////////////////
// CTrackInfo view

// class CTrackInfo : public CScrollView
class CTrackInfo : public CMyTableView
{
public:
  CTrackInfo();// protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CTrackInfo)

  CTrChangeDlg* trkdlg;

  // Attributes
public:
  // Operations
public:
  // CString *toString(int data);
  // CString *toString(double data);
  // CString *toHexString(int data);

  void
    UpdateInfo(BOOL update = FALSE);

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTrackInfo)
public:
  virtual BOOL
    Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
  virtual BOOL
    OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);

protected:
  virtual void
    OnDraw(CDC* pDC);// overridden to draw this view
  virtual void
    OnInitialUpdate();// first time after construct
  virtual BOOL
    OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
  virtual void
    OnPrint(CDC* pDC, CPrintInfo* pInfo);
  //}}AFX_VIRTUAL

  // Implementation
protected:
  virtual ~CTrackInfo();
#ifdef _DEBUG
  virtual void
    AssertValid() const;
  virtual void
    Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
  //{{AFX_MSG(CTrackInfo)
  afx_msg void
    OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void
    OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void
    OnSetFocus(CWnd* pOldWnd);
  afx_msg void
    OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void
    OnExportTable();
  afx_msg void
    OnMouseMove(UINT nFlags, CPoint point);
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
