#include "stdafx.h"

#include "TrackEditor.h"
#include "GPTrack.h"
#include "stdio.h"
#include "math.h"
#include "TrackSection.h"
#include "TrackEditorScrollView.h"
#include "Tool.h"
#include "JamFileEditor.h"

void GPTrack::drawTrackLines(Display *g)
{
  TrackSection *tn;

  int size = TrackSections->size();

  for (int i = 0; i < size; i++) {
    TrackSection *t = (TrackSection *)TrackSections->elementAt(i);

    if (i < TrackSections->size() - 1) {
      tn = (TrackSection *)TrackSections->elementAt(i + 1);
    } else {
      tn = (TrackSection *)TrackSections->elementAt(0);
    }

    if (showTrack) t->drawTrackLine(g, t->getAngle(), this);
  }
}

static void
  drawPoint(Display *g, double x1, double y1)
{
  double pix = (double)g->getObjectX(4);
  double pix1 = (double)g->getObjectX(3);

  g->setColor(-2);
  // g->drawEllipse(x1-pix1,y1-pix1,x1+pix1,y1+pix1);
  // g->drawLine(x1,y1-pix,x1,y1+pix);
  // g->drawLine(x1-pix,y1,x1+pix,y1);
  g->drawRect(x1 - pix1, y1 - pix1, x1 + pix1, y1 + pix1);
}

LinePt *
  TrackSection::drawTrackLine(Display *g, double angle, GPTrack *track)
{
  double xpos = getStartX();
  double ypos = getStartY();
  double nextxpos = getEndX();
  double nextypos = getEndY();
  double sectorStartAngle = getStartAngle();
  double sectorEndAngle = getEndAngle();

  TrackSection *prev = track->GetPreviousSection(this);

  double previousLeft = getWidthLeft();
  double previousRight = getWidthRight();

  if (prev != NULL) {
    previousLeft = prev->getWidthLeft();
    previousRight = prev->getWidthRight();
  }

  g->setColor(0);

  drawPoint(g, xpos, ypos);

  if (linePt != NULL) delete linePt;
  linePt = new LinePt((int)xpos, (int)ypos);

  if (angle == 0) {
    g->drawLine(xpos, ypos, (xpos + nextxpos), (ypos + nextypos));
  } else {
    double cx = xpos + getRightSideX(sectorStartAngle, getRadius());
    double cy = ypos + getRightSideY(sectorStartAngle, getRadius());

    g->drawArc((xpos + nextxpos), (ypos + nextypos), xpos, ypos, cx, cy, getStartAngle(), getAngle() * getLength(), getRadius());
  }
  return linePt;
}

double
  distanceTo(LinePt *A, CPoint B)
{
  double dx = A->x - B.x;
  double dy = A->y - B.y;
  return sqrt(dx * dx + dy * dy);
}

void LineTool::OnLButtonDown(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  // find closes point
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  GPTrack *mytrack = pDoc->getTrack();
  CPoint pt;
  CPoint offset = sview->GetDeviceScrollPosition();

  pt.x = point.x + offset.x;
  pt.y = point.y + offset.y;
  pt.x = (long)sview->display->getObjectX(pt.x);
  pt.y = (long)sview->display->getObjectY(pt.y);

  double minDistance = 100000;

  int size = mytrack->TrackSections->size();
  for (int i = 0; i < size; i++) {
    TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);
    double distance = distanceTo(t->linePt, pt);

    if (minDistance > distance) {
      minDistance = distance;
      closestSector = t;
    }
  }
}

void LineTool::OnMouseMove(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  if (closestSector != NULL) {
    // move point
    TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
    GPTrack *mytrack = pDoc->getTrack();
    CPoint pt;
    CPoint offset = sview->GetDeviceScrollPosition();

    bool constrain = (nFlags & MK_SHIFT) ? true : false;
    bool constrainZero = (nFlags & MK_CONTROL) ? true : false;

    pt.x = point.x + offset.x;
    pt.y = point.y + offset.y;
    pt.x = (long)sview->display->getObjectX(pt.x);
    pt.y = (long)sview->display->getObjectY(pt.y);

    closestSector->setStartX(pt.x);
    closestSector->setStartY(pt.y);
    TrackSection *prev = mytrack->GetPreviousSection(closestSector);
    double dx = pt.x - prev->getStartX();
    double dy = pt.y - prev->getStartY();
    prev->setEndX(dx);
    prev->setEndY(dy);

    TrackSection *next = mytrack->GetNextSection(closestSector);
    double dx1 = next->getStartX() - pt.x;
    double dy1 = next->getStartY() - pt.y;
    closestSector->setEndX(dx1);
    closestSector->setEndY(dy1);
    double t1x = prev->getStartX();
    double t1y = prev->getStartY();
    double t2x = closestSector->getStartX();
    double t2y = closestSector->getStartY();
    double tdx = t2x - t1x;
    double tdy = t2y - t1y;
    double len = sqrt(tdx * tdx + tdy * tdy);
    double angle = atan2(tdy, tdx);
    double nangle;
    double prevangle = prev->getStartAngle();

    nangle = angle;

    if (prevangle > 0) {
      while (prevangle > PI) {
        prevangle -= 2 * PI;
      }
    } else {
      while (prevangle < -PI) {
        prevangle += 2 * PI;
      }
    }

    if (angle > 0 && prevangle > 0 && prevangle <= PI) {
      // TRACE("1:%f %f %f\n",nangle,angle,prevangle);
      nangle = (angle);
    } else if (angle <= -PI / 2 && angle < 0 && prevangle >= 0 && prevangle <= PI) {
      // TRACE("2c: %f %f %f\n",nangle,angle,prevangle);
      nangle = (2 * PI + angle);
    } else if (angle <= 0 && angle > -PI / 2 && prevangle >= 0 && prevangle <= PI) {
      // TRACE("2d: %f %f %f\n",nangle,angle,prevangle);
      nangle = (angle);
    } else if (angle >= PI / 2 && prevangle < 0 && prevangle >= -PI) {
      // TRACE("2a: %f %f %f\n",nangle,angle,prevangle);
      nangle = ((-2 * PI) + angle);
    } else if (angle > 0 && angle < PI / 2 && prevangle < 0 && prevangle >= -PI) {
      // TRACE("2b: %f %f %f\n",nangle,angle,prevangle);
      nangle = (angle);
    }

    nangle = nangle - prevangle;

    double degangle = RadToDeg(nangle);

    if (constrainZero) {
      prev->setAngle(0);
    } else if (!constrain) {
      prev->setAngle(degangle / len);
    }

    prev->setLength((int)len);

    mytrack->createTrack(mytrack->GetDocument());
    mytrack->regenerate = TRUE;

    sview->repaint();
  }
}

void LineTool::OnLButtonUp(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  // move track to fit
  closestSector = NULL;
}

void GPTrack::drawCCLines(Display *g)
{
  CCLineSection *tn;

  int size = CCLineSections->size();

  double x1, y1, curr;

  for (int i = 0; i < size; i++) {
    CCLineSection *t = (CCLineSection *)CCLineSections->elementAt(i);

    if (i < CCLineSections->size() - 1) {
      tn = (CCLineSection *)CCLineSections->elementAt(i + 1);
    } else {
      tn = (CCLineSection *)CCLineSections->elementAt(0);
    }

    if (showTrack) t->drawLine(g, &x1, &y1, &curr, 0);

    if (t->linePt) {
      drawPoint(g, t->linePt->x, t->linePt->y);
    }
  }
}

Vector *
  GPTrack::getSelectedSections(CPoint begin, CPoint end)
{
  Vector *selected = new Vector("Selected Sections", __LINE__);

  int size = TrackSections->size();

  for (int i = 0; i < size; i++) {
    TrackSection *t = (TrackSection *)TrackSections->elementAt(i);
    if (t->isInside(begin, end)) {
      selected->addElementReference(t);
    }
  }
  return selected;
}

JAM *GPTrack::GetJAM(int id)
{
  int size = JamFiles->size();
  int count = 0;

  for (int i = size - 1; i >= 0; i--) {
    JamFileEditor *t = (JamFileEditor *)JamFiles->elementAt(i);

    char *name = t->getJamFileName();

    JAM *jams = t->getJAM();
    jams->OpenToRead(FALSE);

    int subimages = jams->getNumberOfImages();

    for (int j = 0; j < subimages; j++) {
      int idx = jams->getImageId(j);
      if (idx == id) return jams;
    }
  }
  return NULL;
}

char *
  GPTrack::GetJAMFile(int id)
{
  int size = JamFiles->size();
  int count = 0;

  for (int i = size - 1; i >= 0; i--) {
    JamFileEditor *t = (JamFileEditor *)JamFiles->elementAt(i);

    char *name = t->getJamFileName();

    JAM *jams = t->getJAM();
    jams->OpenToRead(FALSE);

    int subimages = jams->getNumberOfImages();

    for (int j = 0; j < subimages; j++) {
      int idx = jams->getImageId(j);
      if (idx == id) return name;
    }
  }
  return "Cannot Find Texture in Jam File List";
}

Camera *
  GPTrack::DistanceToCamera(double distance)
{
  Camera *verLast =
    (Camera *)TrackCameras->elementAt((TrackCameras->size() - 1));

  for (int i = 0; i < TrackCameras->size(); i++) {
    Camera *cam = (Camera *)TrackCameras->elementAt(i);
    Camera *last = (Camera *)GetPreviousCamera(cam);
    double lastSwitchMoment = last->getSwitchMoment() - 16384;
    double cameraPos = cam->getDistance() - 16384;
    double sm = cam->getSwitchMoment() - 16384;

    if (TrackCameras->isFirstElement(cam)) {
      // lastSwitchMoment =;
    }

    if (distance >= lastSwitchMoment && distance < sm) return cam;
  }

  return verLast;
}
