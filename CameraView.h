// CameraView.h : header file
//
#include "MyTableView.h"

/////////////////////////////////////////////////////////////////////////////
// CCameraView view

class CCameraView : public CMyTableView
{
protected:
  CCameraView();// protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CCameraView)

  // Attributes
public:
  void UpdateInfo(BOOL);

  // Operations
public:
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CCameraView)
public:
  virtual BOOL
    Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

protected:
  virtual void
    OnDraw(CDC* pDC);// overridden to draw this view
  virtual void
    OnInitialUpdate();// first time after construct
  virtual void
    OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  //}}AFX_VIRTUAL

  // Implementation
protected:
  virtual ~CCameraView();
#ifdef _DEBUG
  virtual void
    AssertValid() const;
  virtual void
    Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
  //{{AFX_MSG(CCameraView)
  afx_msg void
    OnSetFocus(CWnd* pOldWnd);
  afx_msg int
    OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void
    OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void
    OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
