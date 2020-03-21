// WorldView.h : header file
//
#include "TrackEditorDoc.h"
#include "Point3D.h"
#include "Display.h"
#include "dibkernel.h"

/////////////////////////////////////////////////////////////////////////////
// CWorldView view

class CWorldView : public CScrollView
{
protected:
  CWorldView();// protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CWorldView)

  // Attributes
public:
  Matrix3D* WorldTransform;
  WorldView3D* world;
  Model3D* WireFrameBox;
  Point3D* mid;

  BOOL XDragging;
  BOOL YDragging;
  BOOL ZDragging;
  BOOL showBlue;

  CPoint dragPoint;

  CDib* trackImage;

  void
    Resize(double scale);

  CSize
    Get3DSize(double scale);

  Display* display;
  Point3D* center;
  double scalefactor;

  // Operations
public:
  inline CTrackEditorDoc*
    GetDocument()
  {
    return (CTrackEditorDoc*)m_pDocument;
  }

  void
    repaint();
  void
    update();
  void
    updateBox();

  BOOL
    isDragging()
  {
    if (XDragging || YDragging || ZDragging) return TRUE;
    return FALSE;
  }

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CWorldView)
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
  virtual ~CWorldView();
#ifdef _DEBUG
  virtual void
    AssertValid() const;
  virtual void
    Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
  //{{AFX_MSG(CWorldView)
  afx_msg void
    OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void
    OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void
    OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void
    OnIn();
  afx_msg void
    OnOut();
  afx_msg void
    OnShowBlue();
  afx_msg void
    OnUpdateShowBlue(CCmdUI* pCmdUI);
  afx_msg void
    OnInsertRemoveoverlay();
  afx_msg void
    OnUpdateInsertRemoveoverlay(CCmdUI* pCmdUI);
  afx_msg void
    OnLaybitmap();
  afx_msg void
    OnZoomHome();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
