// JamView.h : header file
//
#include "Jam.h"

/////////////////////////////////////////////////////////////////////////////
// CJamView view

class CJamView : public CScrollView
{
protected:
  CJamView();// protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CJamView)

  // Attributes
public:
  // Operations
public:
  JAM* myjam;

  double currentScale;

  void
    Resize();

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CJamView)
protected:
  virtual void
    OnDraw(CDC* pDC);// overridden to draw this view
  virtual void
    OnInitialUpdate();// first time after construct
    //}}AFX_VIRTUAL

  // Implementation
protected:
  virtual ~CJamView();
#ifdef _DEBUG
  virtual void
    AssertValid() const;
  virtual void
    Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
  //{{AFX_MSG(CJamView)
  afx_msg int
    OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void
    OnZoomHome();
  afx_msg void
    OnIn();
  afx_msg void
    OnOut();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
