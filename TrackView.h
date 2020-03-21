// TrackView.h : interface of the CTrackView class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _TRACK_VIEW_H_
#define _TRACK_VIEW_H_

class CZoomFactor;

// Just for me to test compilation
#define ZOOM_BOUNCE                    \
  4// Number of pixels movement before \
    // being committed to dragging.

#define ID_TOOL_ZOOM 1// Any old tool ID for test

class CTrackView : public CScrollView
{
protected:// create from serialization only
  CTrackView();
  DECLARE_DYNCREATE(CTrackView)

  // Enumerations
public:
  enum eZoomDirection// Zoom in or out of the image
  {
    ZOOM_IMAGE_IN,
    ZOOM_IMAGE_OUT,
    ZOOM_ONE2ONE
  };

  // Attributes
public:
  CTrackEditorDoc*
    GetDocument();

  // Get the ID of the current editing tool
  UINT
    GetCurrentTool();

  // Size of the track area, or whatever (== scroll size at 1:1)
  CSize
    GetTrackSize();

  // Operations
public:
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTrackView)
public:
  virtual void
    OnDraw(CDC* pDC);// overridden to draw this view
  virtual BOOL
    PreCreateWindow(CREATESTRUCT& cs);
  virtual void
    OnInitialUpdate();
  //}}AFX_VIRTUAL

  // Implementation
public:
  virtual ~CTrackView();
#ifdef _DEBUG
  virtual void
    AssertValid() const;
  virtual void
    Dump(CDumpContext& dc) const;
#endif

protected:
  // Dragging/zooming variables
  CZoomFactor* m_pZoom;
  BOOL m_bDragging;
  BOOL m_bBouncing;
  CPoint m_ptDragStart;
  CPoint m_ptDragCurrent;

  // Just a test bitmap
  CBitmap m_bmpTrack;

  // Dragging and drawing stuff
  void
    CancelDrag(BOOL bKillRubberBand);
  void
    MoveRubberBand(CPoint ptNewEndPoint);
  void
    DrawRubberBand(CPoint ptStart, CPoint ptEnd);
  BOOL
    AnimateZoom(const CRect& rctZoom, const eZoomDirection zoom_dirn);

  // Coordinate conversion functions
  void
    ClientToDoc(CPoint& ptConv, CPoint scroll_pos = CPoint(-1, -1), float zoom_factor = 0.0f);
  void
    DocToClient(CPoint& ptConv, CPoint scroll_pos = CPoint(-1, -1), float zoom_factor = 0.0f);

  // Main single-click zoom function
  BOOL
    ZoomToPoint(const CPoint& ptZoomCenter, const eZoomDirection zoom_dirn);

  // Generated message map functions
protected:
  //{{AFX_MSG(CTrackView)
  afx_msg void
    OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void
    OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void
    OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void
    OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void
    OnRButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void
    OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void
    OnRButtonUp(UINT nFlags, CPoint point);
  afx_msg void
    OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg BOOL
    OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG// debug version in TrackView.cpp
inline CTrackEditorDoc*
  CTrackView::GetDocument()
{
  return (CTrackEditorDoc*)m_pDocument;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#endif// ~ _TRACK_VIEW_H_
