// WorldView.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "WorldView.h"
#include "Point3D.h"
#include "TrackSection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef MK_ALT
#define MK_ALT 0x20
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorldView

IMPLEMENT_DYNCREATE(CWorldView, CScrollView)

CWorldView::CWorldView()
  : center(NULL), WireFrameBox(NULL), mid(NULL), world(NULL), trackImage(NULL)
{
  WorldTransform = new Matrix3D();
  // WorldTransform->xrot(110);
  // WorldTransform->yrot(30);
  // WorldTransform->zrot(-10);

  WorldTransform->xrot(270);
  WorldTransform->yrot(150);
  WorldTransform->xrot(-10);

  XDragging = FALSE;
  YDragging = FALSE;
  ZDragging = FALSE;
  showBlue = FALSE;

  scalefactor = 1.0;

  display = new Display();
  world = new WorldView3D();
}

CWorldView::~CWorldView()
{
  delete display;
  delete WorldTransform;
  if (world) delete world;
  if (WireFrameBox) delete WireFrameBox;
  if (mid) delete mid;
  if (center) delete center;
}

BEGIN_MESSAGE_MAP(CWorldView, CScrollView)
//{{AFX_MSG_MAP(CWorldView)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_COMMAND(ZOOM_IN, OnIn)
ON_COMMAND(ZOOM_OUT, OnOut)
ON_COMMAND(ID_SHOW_BLUE, OnShowBlue)
ON_UPDATE_COMMAND_UI(ID_SHOW_BLUE, OnUpdateShowBlue)
ON_COMMAND(ID_INSERT_REMOVEOVERLAY, OnInsertRemoveoverlay)
ON_UPDATE_COMMAND_UI(ID_INSERT_REMOVEOVERLAY, OnUpdateInsertRemoveoverlay)
ON_COMMAND(UNDER_LAYBITMAP, OnLaybitmap)
ON_COMMAND(ID_ZOOM_HOME, OnZoomHome)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorldView drawing

void CWorldView::OnInitialUpdate()
{
  CScrollView::OnInitialUpdate();

  CSize sizeTotal;
  // TODO: calculate the total size of this view
  sizeTotal.cx = sizeTotal.cy = 1000;
  SetScrollSizes(MM_TEXT, sizeTotal);
  update();
}

void CWorldView::OnDraw(CDC *pDC)
{
  // TODO: add draw code here
  CTrackEditorDoc *pDoc = GetDocument();
  GPTrack *mytrack = pDoc->getTrack();

  display->setGraphics(pDC);

  if (trackImage != NULL) {
    RECT rect;
    GetClientRect(&rect);
    trackImage->Draw(pDC, &rect);
  }

  if (showBlue) {
    RECT rect;
    GetClientRect(&rect);
    int height = rect.bottom - rect.top;
    double box_size = (double)height / 256.0;

    for (int i = 0; i < 256; i++) {
      CBrush *brush = new CBrush();
      CPen *pen = new CPen(PS_SOLID, 1, RGB(0, 0, i));

      if (brush) {
        brush->CreateSolidBrush(RGB(0, 0, i));
      }

      if (brush && pDC) pDC->SelectObject(brush);

      if (pen && pDC) pDC->SelectObject(pen);

      if (pDC)
        pDC->Rectangle(rect.left, (int)(i * box_size), rect.right, (int)((i + 1) * box_size));

      if (pen) delete pen;
      if (pen) delete brush;
    }
  }

  CSize midval = GetTotalSize();

  Point3D *cancenter = new Point3D(midval.cx / 2, midval.cy / 2, 0);

  if (showBlue)
    display->setColor(5);
  else
    display->setColor(0);

  if (isDragging()) {
    if (WireFrameBox) WireFrameBox->draw(display, cancenter);
  } else {
    if (world) {
      if (mytrack->showFilledObjects) {
        world->fill(display, cancenter);
      } else
        world->draw(display, cancenter);
    }
  }
}

void CWorldView::update()
{
  CTrackEditorDoc *pDoc = GetDocument();
  GPTrack *mytrack = pDoc->getTrack();

  if (world != NULL) delete world;

  world = new WorldView3D();

  if (center) delete center;

  /*
  RECT rect;
  GetClientRect(&rect);

  int midx = (rect.right-rect.left)/2;
  int midy = (rect.bottom-rect.top)/2;

  center = new Point3D(midx,midy,0);
  */

  // CSize midval = GetTotalSize();

  // center = new Point3D(midval.cx/2,midval.cy/2,0);
  center = new Point3D(0, 0, 0);

  TrackSection *tn;

  if (mytrack->showTrack) {
    Model3D *model = new Model3D();

    for (int i = 0; i < mytrack->TrackSections->size(); i++) {
      TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);

      if (i < mytrack->TrackSections->size() - 1) {
        tn = (TrackSection *)mytrack->TrackSections->elementAt(i + 1);
      } else {
        tn = (TrackSection *)mytrack->TrackSections->elementAt(0);
      }

      Primitive3D *trackSec = t->drawTrack3D(display, 0, mytrack, t, tn);
      double x = t->getStartX();
      double y = t->getStartY();
      // Point3D *ptA = new Point3D(x,y,0);
      // polygon->addPoint(ptA);
      model->addPrimitive(trackSec);
    }
    // model->addPrimitive(polygon);
    world->addModel(model);
  }

  if (mytrack->showPitLane) {
    Model3D *pitmodel = new Model3D();

    for (int i = 0; i < mytrack->PitlaneSections->size(); i++) {
      TrackSection *t = (TrackSection *)mytrack->PitlaneSections->elementAt(i);

      if (i < mytrack->PitlaneSections->size() - 1) {
        tn = (TrackSection *)mytrack->PitlaneSections->elementAt(i + 1);
      } else {
        tn = (TrackSection *)mytrack->PitlaneSections->elementAt(0);
      }

      Primitive3D *trackSec = t->drawTrack3D(display, 0, mytrack, t, tn);
      double x = t->getStartX();
      double y = t->getStartY();
      // Point3D *ptA = new Point3D(x,y,0);
      // polygon->addPoint(ptA);
      pitmodel->addPrimitive(trackSec);
    }
    world->addModel(pitmodel);
  }

  if (mytrack->showObjects) {
    Model3D *objects = new Model3D();

    for (int i = 0; i < mytrack->TrackSections->size(); i++) {
      TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);
      Vector *TrackSideObjects = t->getObjects();

      double x1 = t->getStartX();
      double y1 = t->getStartY();
      for (int j = 0; j < TrackSideObjects->size(); j++) {
        TrackObject *to = (TrackObject *)TrackSideObjects->elementAt(j);
        if (to != NULL) {
          double nextypos;
          double nextxpos;
          Point3D *objectpt;

          if (t->getAngle() != 0) {
            CPoint trackPt = to->GetTrackPoint(
              t->getAngle(), t->getStartAngle(), to->getDistance());
            nextypos = trackPt.x;
            nextxpos = trackPt.y;
          } else {
            nextypos = to->getDistance() * sin(t->getAngle());
            nextxpos = to->getDistance() * cos(t->getAngle());
          }
          double xr = x1 + nextxpos;
          double yr = y1 + nextypos;

          objectpt = new Point3D(xr, yr, 0);
          objects->addPrimitive(objectpt);
        }
      }
      // this could be dangerous
      if (TrackSideObjects) delete (TrackSideObjects);
    }
    world->addModel(objects);
  }

  if (mytrack->showScenery) {
    Model3D *model = mytrack->createSceneryModel(display);
    if (model) {
      world->addModel(model);
    }
  }

  if (WireFrameBox) delete WireFrameBox;
  WireFrameBox = world->findBox();

  if (center) delete center;

  // CSize midval = GetTotalSize();

  // center = new Point3D(midval.cx/2,midval.cy/2,0);
  center = new Point3D(0, 0, 0);

  if (mid) delete mid;
  mid = world->findCenter();

  world->Transform(WorldTransform, mid, center, 1.0);
}

void CWorldView::updateBox()
{
  // if (WireFrameBox) delete WireFrameBox;
  // WireFrameBox = world->findBox();

  if (center) delete center;

  // CSize midval = GetTotalSize();

  center = new Point3D(0, 0, 0);

  if (mid) delete mid;
  mid = world->findCenter();

  if (WireFrameBox) WireFrameBox->Transform(WorldTransform, mid, center, 1.0);
}

/////////////////////////////////////////////////////////////////////////////
// CWorldView diagnostics

#ifdef _DEBUG
void CWorldView::AssertValid() const
{
  CScrollView::AssertValid();
}

void CWorldView::Dump(CDumpContext &dc) const
{
  CScrollView::Dump(dc);
}
#endif//_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWorldView message handlers

void CWorldView::OnLButtonDown(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  if (nFlags & MK_SHIFT) {
    YDragging = TRUE;
  } else if ((nFlags & MK_ALT) || (nFlags & MK_CONTROL)) {
    ZDragging = TRUE;
  } else {
    XDragging = TRUE;
  }
  updateBox();

  CScrollView::OnLButtonDown(nFlags, point);
}

void CWorldView::OnLButtonUp(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  XDragging = FALSE;
  YDragging = FALSE;
  ZDragging = FALSE;

  update();
  repaint();

  CScrollView::OnLButtonUp(nFlags, point);
}

void CWorldView::OnMouseMove(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  int dy = point.y - dragPoint.y;
  int dx = point.x - dragPoint.x;

  dragPoint = point;

  if (XDragging) {
    WorldTransform->xrot(dy);
    updateBox();
    repaint();
  }
  if (YDragging) {
    WorldTransform->yrot(dx);
    updateBox();
    repaint();
  }
  if (ZDragging) {
    WorldTransform->zrot(dy);
    updateBox();
    repaint();
  }

  // char msg[256];
  // wsprintf(msg,"X=%d Y=%d Z=%d\n",dx,dy,10);
  // SetMessageText(msg)

  CScrollView::OnMouseMove(nFlags, point);
}

void CWorldView::repaint()
{
  RECT rect;

  GetClientRect(&rect);
  InvalidateRect(&rect);
}

void CWorldView::OnIn()
{
  // TODO: Add your command handler code here
  display->scale *= 1.1;
  updateBox();
  Resize(display->getScale());
  repaint();
}

void CWorldView::OnOut()
{
  // TODO: Add your command handler code here
  display->scale *= 0.9;
  updateBox();
  Resize(display->getScale());
  repaint();
}

void CWorldView::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
  // TODO: Add your specialized code here and/or call the base class
  updateBox();
  repaint();
}

void CWorldView::OnShowBlue()
{
  // TODO: Add your command handler code here
  showBlue = !showBlue;

  repaint();
}

void CWorldView::OnUpdateShowBlue(CCmdUI *pCmdUI)
{
  // TODO: Add your command update UI handler code here
  pCmdUI->SetCheck(showBlue);
}

void CWorldView::OnInsertRemoveoverlay()
{
  // TODO: Add your command handler code here
  if (trackImage != NULL) delete trackImage;

  trackImage = NULL;

  repaint();
}

void CWorldView::OnUpdateInsertRemoveoverlay(CCmdUI *pCmdUI)
{
  // TODO: Add your command update UI handler code here
  pCmdUI->Enable((trackImage != NULL));
}

extern char lpszBmpFilter[];

void CWorldView::OnLaybitmap()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc *pDoc = GetDocument();
  GPTrack *mytrack = pDoc->getTrack();

  char fileName[256];

  strcpy(fileName, "*.bmp");

  CFileDialog *fdlg = new CFileDialog(TRUE, "bmp", (LPCSTR)&fileName, OFN_HIDEREADONLY, lpszBmpFilter);

  int result = fdlg->DoModal();
  if (result == IDOK) {
    CString file = fdlg->GetFileName();
    // clear old image
    if (trackImage != NULL) delete trackImage;

    trackImage = new CDib();
    trackImage->Read(file);
  }
  repaint();
}

void CWorldView::OnZoomHome()
{
  POINT pos;
  pos.x = 0;
  pos.y = 0;
  ScrollToPosition(pos);

  display->scale = 1.0;
  Resize(display->getScale());
  repaint();
}

void CWorldView::Resize(double scale)
{
  CSize sizeTotal;
  // TODO: calculate the total size of this view
  CTrackEditorDoc *pDoc = GetDocument();
  if (pDoc) {
    sizeTotal = Get3DSize(scale);
  } else {
    sizeTotal.cx = sizeTotal.cy = 1000;
  }

  if (sizeTotal.cx >= 32000) sizeTotal.cx = 32000;
  if (sizeTotal.cy >= 32000) sizeTotal.cy = 32000;
  SetScrollSizes(MM_TEXT, sizeTotal);
}

CSize CWorldView::Get3DSize(double scale)
{
  Model3D *worldExtent = WireFrameBox->findPointsBox();
  CSize totalSize;

  totalSize.cx = 512;
  totalSize.cy = 512;

  CSize midval = GetTotalSize();

  if (worldExtent) {
    worldExtent->Transform(WorldTransform, mid, center, 1.0);

    for (int i = 0; i < worldExtent->primitives->size(); i++) {
      Point3D *pt = (Point3D *)worldExtent->primitives->elementAt(i);

      totalSize.cx = max(midval.cx + display->getScreenX(pt->tx), totalSize.cx);
      totalSize.cy = max(midval.cy + display->getScreenX(pt->ty), totalSize.cy);
    }
  }
  return totalSize;
}
