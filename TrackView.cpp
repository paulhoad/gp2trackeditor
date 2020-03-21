// TrackView.cpp : implementation of the CTrackView class
//

#include "stdafx.h"
#include "TrackEditor.h"
#include "ZoomFactor.h"

#include "TrackEditorDoc.h"
#include "TrackView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrackView

IMPLEMENT_DYNCREATE(CTrackView, CScrollView)

BEGIN_MESSAGE_MAP(CTrackView, CScrollView)
//{{AFX_MSG_MAP(CTrackView)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDBLCLK()
ON_WM_RBUTTONDBLCLK()
ON_WM_RBUTTONDOWN()
ON_WM_RBUTTONUP()
ON_WM_CHAR()
ON_WM_SETCURSOR()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackView construction/destruction

CTrackView::CTrackView()
{
  // TODO: add construction code here
  m_pZoom = new CZoomFactor(16, 16);
  m_bDragging = FALSE;
  m_bBouncing = FALSE;
  m_ptDragStart = CPoint(-1, -1);
  m_ptDragCurrent = CPoint(-1, -1);

  // Load the track bitmap just as an example
  m_bmpTrack.LoadBitmap(IDB_TRACK);
}

CTrackView::~CTrackView()
{
  if (NULL != m_pZoom) {
    delete m_pZoom;
    m_pZoom = NULL;
  }

  m_bmpTrack.DeleteObject();
}

BOOL CTrackView::PreCreateWindow(CREATESTRUCT& cs)
{
  // TODO: Modify the Window class or styles here by modifying
  //  the CREATESTRUCT cs

  return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTrackView drawing

void CTrackView::OnDraw(CDC* pDC)
{
  CTrackEditorDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);

  // Disclaimer: this is very scrappy/buggy code to
  // do some test drawing ;)
  CDC copyDC;
  copyDC.CreateCompatibleDC(pDC);
  CBitmap* pOldBitmap = copyDC.SelectObject(&m_bmpTrack);

  CRect rctClient;
  GetClientRect(&rctClient);
  CPoint ptScrollOffset = GetScrollPosition();

  // Show non-picture areas as grey hatch - could use solid COLOR_APPWORKSPACE
  // instead, I suppose
  CBrush brBorder;
  brBorder.CreateHatchBrush(HS_DIAGCROSS, RGB(192, 192, 192));
  CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&brBorder);
  pDC->PatBlt(0, 0, rctClient.Width(), rctClient.Height(), PATCOPY);
  // Delete/reselect brush
  pDC->SelectObject(pOldBrush);
  brBorder.DeleteObject();

  // Get the zoom factor
  float fZoomFactor = m_pZoom->GetFactor();
  CSize sizTrack = GetTrackSize();
  CSize sizZoomedTrack(sizTrack);
  sizZoomedTrack.cx = (long)((float)sizZoomedTrack.cx * fZoomFactor);
  sizZoomedTrack.cy = (long)((float)sizZoomedTrack.cy * fZoomFactor);

  // Get the size of the area to draw in
  CSize sizDraw = rctClient.Size();
  if (sizDraw.cx > sizZoomedTrack.cx) sizDraw.cx = sizZoomedTrack.cx;
  if (sizDraw.cy > sizZoomedTrack.cy) sizDraw.cy = sizZoomedTrack.cy;

  // Get the offset in track bitmap terms
  CPoint ptBmpOffset(ptScrollOffset);
  ptBmpOffset.x = (long)((float)ptBmpOffset.x / fZoomFactor);
  ptBmpOffset.y = (long)((float)ptBmpOffset.y / fZoomFactor);
  CSize sizBmpView(sizDraw);
  sizBmpView.cx = (long)((float)sizBmpView.cx / fZoomFactor);
  sizBmpView.cy = (long)((float)sizBmpView.cy / fZoomFactor);
  if (sizBmpView.cx > sizTrack.cx) sizBmpView.cx = sizTrack.cx;
  if (sizBmpView.cy > sizTrack.cy) sizBmpView.cy = sizTrack.cy;

  // Copy the bitmap
  pDC->StretchBlt(ptScrollOffset.x, ptScrollOffset.y, sizDraw.cx, sizDraw.cy, &copyDC, ptBmpOffset.x, ptBmpOffset.y, sizBmpView.cx, sizBmpView.cy, SRCCOPY);

  // Tidy up
  copyDC.SelectObject(pOldBitmap);
  copyDC.DeleteDC();
}

/////////////////////////////////////////////////////////////////////////////
// CTrackView diagnostics

#ifdef _DEBUG
void CTrackView::AssertValid() const
{
  CScrollView::AssertValid();
}

void CTrackView::Dump(CDumpContext& dc) const
{
  CScrollView::Dump(dc);
}

CTrackEditorDoc*
  CTrackView::GetDocument()// non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTrackEditorDoc)));
  return (CTrackEditorDoc*)m_pDocument;
}
#endif//_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTrackView message handlers

// Button down
//

void CTrackView::OnLButtonDown(UINT nFlags, CPoint point)
{
  // Zoom tool code only as demo
  if (GetCurrentTool() == ID_TOOL_ZOOM) {
    // If the other mouse button is already down, we want to cancel
    // whatever we're doing
    if (m_bBouncing || m_bDragging) {
      CancelDrag(m_bDragging);
      return;
    }

    // Capture all mouse movement
    SetCapture();

    // Set drag coordinates
    m_ptDragCurrent = m_ptDragStart = point;

    // Set the appropriate flags
    m_bBouncing = TRUE;// still just a tentative action
  }
}

void CTrackView::OnRButtonDown(UINT nFlags, CPoint point)
{
  // Zoom tool code only as demo
  if (GetCurrentTool() == ID_TOOL_ZOOM) {
    // If the other mouse button is already down, we want to cancel
    // whatever we're doing
    if (m_bBouncing || m_bDragging) {
      CancelDrag(m_bDragging);
      return;
    }

    // Capture all mouse movement
    SetCapture();

    // Set drag coordinates
    m_ptDragCurrent = m_ptDragStart = point;

    // Set the appropriate flags
    m_bBouncing = TRUE;// still just a tentative action
  }
}

// Mouse movement
//

void CTrackView::OnMouseMove(UINT nFlags, CPoint point)
{
  // Zoom tool code only as demo
  if (GetCurrentTool() == ID_TOOL_ZOOM) {
    // Take action according to the dragging flags set
    if (m_bDragging)// AOI zooming
    {
      // Draw the new rubber band
      MoveRubberBand(point);
    } else if (m_bBouncing)// impending AOI zoom?
    {
      if (abs(point.x - m_ptDragStart.x) > ZOOM_BOUNCE || abs(point.y - m_ptDragStart.y) > ZOOM_BOUNCE) {
        // Draw the initial rubber band
        m_bBouncing = FALSE;
        m_bDragging = TRUE;
        m_ptDragCurrent = point;
        DrawRubberBand(m_ptDragStart, m_ptDragCurrent);
      }
    }
  }
}

// Button up
//

void CTrackView::OnLButtonUp(UINT nFlags, CPoint point)
{
  // Zoom tool code only as demo
  if (GetCurrentTool() == ID_TOOL_ZOOM) {
    // Relinquish mouse control
    if (GetCapture() == this) ReleaseCapture();

    // Take action according to which dragging flags are set
    if (m_bDragging) {
      // The AOI zooming drag is now over
      m_bDragging = FALSE;

      // Calculate new rubber band and use for zoom
      m_ptDragCurrent = point;
      CRect rctRubberBand(m_ptDragStart, m_ptDragCurrent);
      rctRubberBand.NormalizeRect();

      // Get the location of the center of zoom
      CPoint ptCenter = rctRubberBand.CenterPoint();
      ClientToDoc(ptCenter);

      // Get the other sizes needed for the zoom
      CRect rctClient;
      GetClientRect(&rctClient);
      CSize sizTrack = GetTrackSize();

      // Do the fancy feedback stuff
      AnimateZoom(rctRubberBand, ZOOM_IMAGE_IN);

      // Set the new zoom factor
      CSize sizScroll = m_pZoom->MagnifyArea(rctClient.Size(),
        rctRubberBand.Size(),
        sizTrack);

      // Delay redrawing the view
      InvalidateRect(NULL, FALSE);

      // Set the scroll size
      if (sizScroll != CSize(-1, -1)) SetScrollSizes(MM_TEXT, sizScroll);

      // Calculate the new scroll position in logical coordinates
      float fZoomFactor = m_pZoom->GetFactor();
      CPoint ptTopLeft = ptCenter;
      ptTopLeft.x = (long)((float)ptTopLeft.x * fZoomFactor);
      ptTopLeft.y = (long)((float)ptTopLeft.y * fZoomFactor);
      ptTopLeft -= CSize(rctClient.Width() / 2, rctClient.Height() / 2);

      // Make sure the scroll position is valid by clipping
      // to min and max possible values
      CPoint ptMaxOffset(GetTotalSize() - rctClient.Size());
      ptTopLeft.x = min(ptMaxOffset.x, max(0, ptTopLeft.x));
      ptTopLeft.y = min(ptMaxOffset.y, max(0, ptTopLeft.y));
      // Scroll to the new position
      ScrollToPosition(ptTopLeft);

      // Do a full redraw
      Invalidate();
    } else if (m_bBouncing) {
      // Thank god for that - nothing complicated, just
      // a standard zoom-in to point
      CPoint ptZoom(point);
      ClientToDoc(ptZoom);
      ZoomToPoint(ptZoom, ZOOM_IMAGE_IN);

      // Reset the bouncing flag
      m_bBouncing = FALSE;
    } else
      // Probably just chord-clicked
      return;
  }
}

void CTrackView::OnRButtonUp(UINT nFlags, CPoint point)
{
  // Zoom tool code only as demo
  if (GetCurrentTool() == ID_TOOL_ZOOM) {
    // Relinquish mouse control
    if (GetCapture() == this) ReleaseCapture();

    // Take action according to which dragging flags are set
    if (m_bDragging) {
      // The AOI zooming drag is now over
      m_bDragging = FALSE;

      // Calculate new rubber band and use for zoom
      m_ptDragCurrent = point;
      CRect rctRubberBand(m_ptDragStart, m_ptDragCurrent);
      rctRubberBand.NormalizeRect();

      // Get the other sizes needed for the zoom
      CRect rctClient;
      GetClientRect(&rctClient);
      CSize sizTrack = GetTrackSize();

      // Get the current Dib location of the center of the client area
      CPoint ptCenterOldCR = rctClient.CenterPoint();
      ClientToDoc(ptCenterOldCR);

      // Do the fancy feedback stuff
      AnimateZoom(rctRubberBand, ZOOM_IMAGE_OUT);

      // Set the new zoom factor
      CSize sizScroll =
        m_pZoom->ShrinkArea(rctClient.Size(), rctRubberBand.Size(), sizTrack);

      // Delay redrawing the view
      InvalidateRect(NULL, FALSE);

      // Set the scroll size
      if (sizScroll != CSize(-1, -1)) SetScrollSizes(MM_TEXT, sizScroll);

      // Calculate the new scroll position in logical coordinates
      float fZoomFactor = m_pZoom->GetFactor();
      CPoint ptNewOffset = ptCenterOldCR;
      ptNewOffset.x = (long)((float)ptNewOffset.x * fZoomFactor);
      ptNewOffset.y = (long)((float)ptNewOffset.y * fZoomFactor);
      ptNewOffset -= rctRubberBand.CenterPoint();

      // Make sure the scroll position is valid by clipping
      // to min and max possible values
      CPoint ptMaxOffset(GetTotalSize() - rctClient.Size());
      ptNewOffset.x = min(ptMaxOffset.x, max(0, ptNewOffset.x));
      ptNewOffset.y = min(ptMaxOffset.y, max(0, ptNewOffset.y));
      // Scroll to the new position
      ScrollToPosition(ptNewOffset);

      // Do a full redraw
      Invalidate();
    } else if (m_bBouncing) {
      // Thank god for that - nothing complicated, just
      // a standard zoom-out from point
      CPoint ptZoom(point);
      ClientToDoc(ptZoom);
      ZoomToPoint(ptZoom, ZOOM_IMAGE_OUT);

      // Reset the bouncing flag
      m_bBouncing = FALSE;
    } else
      // Probably just chord-clicked
      return;
  }
}

// Double-clicking
//

void CTrackView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  // Zoom tool code only as demo
  if (GetCurrentTool() == ID_TOOL_ZOOM) {
    // If the other mouse button is already down, we want to cancel
    // whatever we're doing
    if (m_bBouncing || m_bDragging) {
      CancelDrag(m_bDragging);
      return;
    }

    // Capture all mouse movement
    SetCapture();

    // Allow drag from double-click - no reason not to!
    m_ptDragCurrent = m_ptDragStart = point;

    // Set the appropriate flags
    m_bBouncing = TRUE;// still just a tentative action
  }
}

void CTrackView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
  // Zoom tool code only as demo
  if (GetCurrentTool() == ID_TOOL_ZOOM) {
    // If the other mouse button is already down, we want to cancel
    // whatever we're doing
    if (m_bBouncing || m_bDragging) {
      // Release mouse capture if necessary
      if (GetCapture() == this) ReleaseCapture();

      // Stop dragging etc.
      m_bDragging = m_bBouncing = FALSE;

      // Get rid of last rubber band
      if (m_bDragging) DrawRubberBand(m_ptDragStart, m_ptDragCurrent);

      return;
    }

    // Capture all mouse movement
    SetCapture();

    // Allow drag from double-click - no reason not to!
    m_ptDragCurrent = m_ptDragStart = point;

    // Set the appropriate flags
    m_bBouncing = TRUE;// still just a tentative action
  }
}

// Keyboard input
//

void CTrackView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  // Zoom tool code only as demo
  if (GetCurrentTool() == ID_TOOL_ZOOM) {
    // Escape key pressed?
    if ((27 == nChar) && m_bDragging) CancelDrag(TRUE);
  }
}

// Miscellaneous functions supporting zooming
//

void CTrackView::CancelDrag(BOOL bKillRubberBand)
{
  // Release mouse capture if necessary
  if (GetCapture() == this) ReleaseCapture();

  // Stop dragging etc.
  m_bDragging = m_bBouncing = FALSE;

  // Get rid of last rubber band
  if (bKillRubberBand) DrawRubberBand(m_ptDragStart, m_ptDragCurrent);
}

void CTrackView::MoveRubberBand(CPoint ptNewEndPoint)
{
  // Get rid of the old rubber band then draw the new one
  DrawRubberBand(m_ptDragStart, m_ptDragCurrent);
  m_ptDragCurrent = ptNewEndPoint;
  DrawRubberBand(m_ptDragStart, m_ptDragCurrent);
}

void CTrackView::DrawRubberBand(CPoint ptStart, CPoint ptEnd)
{
  // Points passed in are in client coordinates
  CDC* pDC = GetDC();
  CRect rctBand(ptStart, ptEnd);
  pDC->DrawFocusRect(&rctBand);
  ReleaseDC(pDC);
}

UINT CTrackView::GetCurrentTool()
{
  // Just for me to test compilation
  return ID_TOOL_ZOOM;
}

// Coordinate conversion functions
void CTrackView::ClientToDoc(CPoint& ptConv, CPoint scroll_pos /* = CPoint(-1,-1)*/, float zoom_factor /*= 0.0f*/)
{
  // If scroll position and/or zoom factor have not been supplied, use current
  // values
  CPoint ptScrollPos =
    (CPoint(-1, -1) != scroll_pos) ? scroll_pos : GetScrollPosition();
  float fZoomFactor =
    (0.0f != zoom_factor) ? zoom_factor : m_pZoom->GetFactor();

  // Calculate the scroll offset in terms of document coords
  CPoint ptDocOffset(ptScrollPos);
  ptDocOffset.x = (long)((float)ptDocOffset.x / fZoomFactor);
  ptDocOffset.y = (long)((float)ptDocOffset.y / fZoomFactor);

  // Convert to document coordinates using zoom factor
  ptConv.x = (long)((float)ptConv.x / fZoomFactor);
  ptConv.y = (long)((float)ptConv.y / fZoomFactor);

  // Convert point to be relative to top left of image rather than
  // the top left of the client area
  ptConv += ptDocOffset;
}

void CTrackView::DocToClient(CPoint& ptConv, CPoint scroll_pos /* = CPoint(-1,-1)*/, float zoom_factor /*= 0.0f*/)
{
  // If scroll position and/or zoom factor have not been supplied, use current
  // values
  CPoint ptScrollPos =
    (CPoint(-1, -1) != scroll_pos) ? scroll_pos : GetScrollPosition();
  float fZoomFactor =
    (0.0f != zoom_factor) ? zoom_factor : m_pZoom->GetFactor();
  ASSERT(fZoomFactor != 0.0f);

  // Calculate the scroll offset in terms of document coords
  CPoint ptDocOffset(ptScrollPos);
  ptDocOffset.x = (long)((float)ptDocOffset.x / fZoomFactor);
  ptDocOffset.y = (long)((float)ptDocOffset.y / fZoomFactor);

  // Convert point to be relative to top left of client area rather
  // than the top left of the image
  ptConv -= ptDocOffset;

  // Convert from document coords to client coordinates using zoom factor
  ptConv.x = (long)((float)ptConv.x * fZoomFactor);
  ptConv.y = (long)((float)ptConv.y * fZoomFactor);
}

// Do the zooming animation
BOOL CTrackView::AnimateZoom(const CRect& rctZoom, const eZoomDirection zoom_dirn)
{
  // Make sure the caller is passing in a proper parameter for the zoom
  // direction (not sure how strong type-checking is for enumerated types)
  ASSERT(zoom_dirn == ZOOM_IMAGE_IN || zoom_dirn == ZOOM_IMAGE_OUT);
  if (!(zoom_dirn == ZOOM_IMAGE_IN || zoom_dirn == ZOOM_IMAGE_OUT))
    return FALSE;

  const UINT nStepCount = 10;// number of rectangles
  const UINT nPause = 20;// pause between each

  // Get the client rectangle
  CRect rctClient;
  GetClientRect(&rctClient);

  // Get the DC
  CDC* pDC = GetDC();

  // Do the drawing etc.
  CRect rctDiff(abs(rctZoom.left - rctClient.left),
    abs(rctZoom.top - rctClient.top),
    abs(rctZoom.right - rctClient.right),
    abs(rctZoom.bottom - rctClient.bottom));
  CRect rctDraw(0, 0, 0, 0);
  CRect rctPrevDraw(0, 0, 0, 0);
  RECT* pLastRect = NULL;
  for (UINT nStepNum = 0; nStepNum < nStepCount; nStepNum++) {
    // Work out this rectangle's location
    if (ZOOM_IMAGE_IN == zoom_dirn) {
      rctDraw.left = rctZoom.left - (nStepNum * (rctDiff.left / nStepCount));
      rctDraw.top = rctZoom.top - (nStepNum * (rctDiff.top / nStepCount));
      rctDraw.right = rctZoom.right + (nStepNum * (rctDiff.right / nStepCount));
      rctDraw.bottom =
        rctZoom.bottom + (nStepNum * (rctDiff.bottom / nStepCount));
    } else {
      rctDraw.left = rctClient.left + (nStepNum * (rctDiff.left / nStepCount));
      rctDraw.top = rctClient.top + (nStepNum * (rctDiff.top / nStepCount));
      rctDraw.right =
        rctClient.right - (nStepNum * (rctDiff.right / nStepCount));
      rctDraw.bottom =
        rctClient.bottom - (nStepNum * (rctDiff.bottom / nStepCount));
    }

    // Draw the first time...
    pDC->DrawDragRect(&rctDraw, CSize(1, 1), pLastRect, CSize(1, 1));
    rctPrevDraw = rctDraw;
    pLastRect = &rctPrevDraw;

    // ...and pause for while
    ::Sleep(nPause);
  }

  // Release the DC
  ReleaseDC(pDC);

  return TRUE;
}

// Zoom into or out of the image at the specified point
// The point passed in is in document coordinates, not logical or device
// coordinates
BOOL CTrackView::ZoomToPoint(const CPoint& ptZoomCenter,
  const eZoomDirection zoom_dirn)
{
  // Make sure the caller is passing in a proper parameter for the zoom
  // direction (not sure how strong type-checking is for enumerated types)
  if (!(zoom_dirn == ZOOM_IMAGE_IN || zoom_dirn == ZOOM_IMAGE_OUT || zoom_dirn == ZOOM_ONE2ONE))
    return FALSE;

  // Change the zoom ratio
  switch (zoom_dirn) {
  case ZOOM_IMAGE_IN:
    if (m_pZoom->CanMagnify())
      // Zoom in one notch
      m_pZoom->Magnify();
    else
      // Unable to zoom any further in
      return FALSE;
    break;
  case ZOOM_IMAGE_OUT:
    if (m_pZoom->CanShrink())
      // Zoom out one notch
      m_pZoom->Shrink();
    else
      // Unable to zoom any further in
      return FALSE;
    break;
  case ZOOM_ONE2ONE:
    // Set to a 1:1 zoom ratio
    m_pZoom->MagnifyTo(1);
    break;
  default:
    // Something's gone horrendously wrong - should have checked zoom_dirn on
    // entry to this function
    ASSERT(FALSE);
    return FALSE;
    break;
  }

  // Delay redrawing the view
  InvalidateRect(NULL, FALSE);

  // Set the new scroll size
  CSize sizImage = GetTrackSize();
  float fZoomFactor = m_pZoom->GetFactor();
  sizImage.cx = (long)((float)sizImage.cx * fZoomFactor);
  sizImage.cy = (long)((float)sizImage.cy * fZoomFactor);
  SetScrollSizes(MM_TEXT, sizImage);

  // Calculate the new scroll position in logical coordinates
  CPoint ptTopLeft = ptZoomCenter;
  CRect rctClient;
  GetClientRect(&rctClient);
  if (ZOOM_ONE2ONE == zoom_dirn) {
    CSize sizWindow = rctClient.Size();
    if (sizImage.cx > sizWindow.cx)
      ptTopLeft.x = (sizImage.cx - sizWindow.cx) / 2;
    if (sizImage.cy > sizWindow.cy)
      ptTopLeft.y = (sizImage.cy - sizWindow.cy) / 2;
  } else {
    ptTopLeft.x = (long)((float)ptTopLeft.x * fZoomFactor);
    ptTopLeft.y = (long)((float)ptTopLeft.y * fZoomFactor);
    ptTopLeft -= CSize(rctClient.Width() / 2, rctClient.Height() / 2);
  }

  // Make sure the scroll position is valid by clipping
  // to min and max possible values
  CPoint ptMaxOffset(GetTotalSize() - rctClient.Size());
  ptTopLeft.x = min(ptMaxOffset.x, max(0, ptTopLeft.x));
  ptTopLeft.y = min(ptMaxOffset.y, max(0, ptTopLeft.y));
  // Scroll to the new position
  ScrollToPosition(ptTopLeft);

  // Can redraw now
  Invalidate();

  if (ZOOM_ONE2ONE != zoom_dirn) {
    // Move the mouse cursor to the position clicked on the image
    CPoint ptMouse(ptZoomCenter);
    DocToClient(ptMouse);
    ClientToScreen(&ptMouse);
    ::SetCursorPos(ptMouse.x, ptMouse.y);
  }

  return TRUE;
}

CSize CTrackView::GetTrackSize()
{
  // Do whatever you have to here.  Basically, return the
  // scroll size for a 1:1 zoom factor
  return CSize(565, 450);// track bitmap size
}

void CTrackView::OnInitialUpdate()
{
  CScrollView::OnInitialUpdate();

  // Calculate the total size of this view at
  // current zoom (assume 1:1 on initial update)
  CSize sizTotal = GetTrackSize();
  SetScrollSizes(MM_TEXT, sizTotal);
}

BOOL CTrackView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
  // TODO: Add your message handler code here and/or call default
  if (nHitTest == HTCLIENT) {
    ::SetCursor(AfxGetApp()->LoadCursor(IDC_ZOOM));
    return TRUE;
  } else
    return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}
