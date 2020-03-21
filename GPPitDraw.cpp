#include "stdafx.h"

#include "TrackEditor.h"
#include "GPTrack.h"
#include "stdio.h"
#include "math.h"
#include "TrackSection.h"
#include "TrackEditorScrollView.h"
#include "Tool.h"

void GPTrack::drawPitLines(Display *g)
{
  TrackSection *tn;

  int size = PitlaneSections->size();

  /*
  for(int i=0;i<size;i++)
{
  TrackSection *t = (TrackSection*)PitlaneSections->elementAt(i);


          if (i<PitlaneSections->size()-1)
          {
           tn = (TrackSection*)PitlaneSections->elementAt(i+1);
          }
          else
          {
               tn = (TrackSection*)PitlaneSections->elementAt(0);
          }
  }
  */

  for (int i = 0; i < size; i++) {
    TrackSection *t = (TrackSection *)PitlaneSections->elementAt(i);

    if (i < PitlaneSections->size() - 1) {
      tn = (TrackSection *)PitlaneSections->elementAt(i + 1);
    } else {
      tn = (TrackSection *)PitlaneSections->elementAt(0);
    }

    if (showTrack) t->drawTrackLine(g, t->getAngle(), this);
  }

  /*
  for(i=0;i<size;i++)
{
  TrackSection *t = (TrackSection*)PitlaneSections->elementAt(i);
}
  */
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

extern double
  distanceTo(LinePt *A, CPoint B);

void PitLineTool::OnLButtonDown(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
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

  int size = mytrack->PitlaneSections->size();
  for (int i = 0; i < size; i++) {
    TrackSection *t = (TrackSection *)mytrack->PitlaneSections->elementAt(i);
    double distance = distanceTo(t->linePt, pt);

    if (minDistance > distance) {
      minDistance = distance;
      closestSector = t;
    }
  }
}

void PitLineTool::OnMouseMove(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
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

void PitLineTool::OnLButtonUp(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  closestSector = NULL;
}

void CCLineTool::OnLButtonDown(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
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

  int size = mytrack->CCLineSections->size();
  for (int i = 0; i < size; i++) {
    CCLineSection *t = (CCLineSection *)mytrack->CCLineSections->elementAt(i);
    double distance = distanceTo(t->linePt, pt);

    if (minDistance > distance) {
      minDistance = distance;
      closestSector = t;
    }
  }
}

void CCLineTool::OnMouseMove(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  if (closestSector != NULL) {
    // move point
    TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
    GPTrack *mytrack = pDoc->getTrack();
    CPoint pt;
    CPoint offset = sview->GetDeviceScrollPosition();

    pt.x = point.x + offset.x;
    pt.y = point.y + offset.y;
    pt.x = (long)sview->display->getObjectX(pt.x);
    pt.y = (long)sview->display->getObjectY(pt.y);

    closestSector->setStartX(pt.x);
    closestSector->setStartY(pt.y);
    CCLineSection *prev = mytrack->GetPreviousSection(closestSector);
    double dx = pt.x - prev->getStartX();
    double dy = pt.y - prev->getStartY();
    prev->setEndX(dx);
    prev->setEndY(dy);

    CCLineSection *next = mytrack->GetNextSection(closestSector);
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

    prev->setAngle(degangle / len);
    prev->setLength((int)len);

    mytrack->createTrack(mytrack->GetDocument());
    mytrack->regenerate = TRUE;

    sview->repaint();
  }
}

void CCLineTool::OnLButtonUp(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  // move track to fit
  closestSector = NULL;
}
