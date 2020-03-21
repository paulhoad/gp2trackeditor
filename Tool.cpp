
#include "stdafx.h"
#include "Tool.h"
#include "GPTrack.h"
#include "TrackEditorScrollView.h"
#include "TrackSection.h"
#include "TrackEditor.h"
#include "TrChangeDlg.h"
#include "ObjectPos.h"
#include "CLineEditor.h"
#include "TrackCmd.h"
#include "TrackObject.h"

extern CTrackEditorApp theApp;

void TrackTool::InsertSection(CView *view, CTrackEditorDoc *pDoc)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;

  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over) {
      over->regenerate = TRUE;
      TrackSection *t = over->getTrackSelection();

      if (t) {
        Vector *sectionCmds = new Vector("Adding Track Section", __LINE__);
        TrackSection *ts = new TrackSection(
          over->TrackSections, over, 0, 20, 0, 0, t->getLeftToBank(), t->getRightToBank(), t->getRoadSignData(), t->getKerbData(), NULL, 0, t->getWidthLeft(), t->getWidthRight(), sectionCmds, t_TrackSection);

        over->TrackSections->insertAfterElement(t, ts);
        over->createTrack(pDoc);
      }
    }
  }
  sview->repaint();
}

void TrackTool::RemoveSection(CView *view, CTrackEditorDoc *pDoc)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;

  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over) {
      over->regenerate = TRUE;
      TrackSection *t = over->getTrackSelection();
      TrackSection *next = over->GetNextSection(t);

      if (t) {
        over->TrackSections->removeElement(t);
        if (next) {
          over->setTrackSelection(next);
        }
        over->createTrack(pDoc);
      }
    }
  }
  sview->repaint();
}

void TrackTool::InsertObject(CView *view, CTrackEditorDoc *pDoc)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;

  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over) {
      over->regenerate = TRUE;
      TrackSection *t = over->getTrackSelection();
      if (t) {
        TrackObjectDefinition *obj =
          (TrackObjectDefinition *)over->TrackObjectDefintions->elementAt(0);

        Vector *SectionCmds = t->getCommands();

        // TrackCmd *trackcmd = new TrackCmd(over,0x80,0,"Inserted
        // TrackObject");
        TrackObject *trackcmd =
          new TrackObject(over, t, 10, obj->getLocator(), 0);
        trackcmd->AddWordArgument(10, "Inserted Object Distance");
        trackcmd->AddWordArgument(obj->getLocator(), "Inserted Object Locator");
        SectionCmds->addElement(trackcmd);

        if (trackcmd) {
          CObjectPos *trkdlg = new CObjectPos(over, view);

          if (trkdlg && trackcmd) trkdlg->setSection(trackcmd);

          int res = trkdlg->DoModal();

          if (res != IDCANCEL && trkdlg && trackcmd) {
            trkdlg->getSection(trackcmd);
            over->regenerate = TRUE;
          }
          delete trkdlg;
        }
      }
    }
  }
  sview->repaint();
}

void TrackTool::OnLButtonDown(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  if (pDoc) {
    GPTrack *over = pDoc->getTrack();
    m_dragStart = point;

    if (over) {
      CPoint pt;
      CPoint offset = sview->GetDeviceScrollPosition();

      pt.x = point.x + offset.x;
      pt.y = point.y + offset.y;

      over->selectedSection(pt);
      TrackSection *t = over->getTrackSelection();
      if (t) {
        m_dragging = FALSE;
      } else {
        m_dragging = FALSE;
      }
    } else {
      m_dragging = FALSE;
    }
  }
}

void TrackTool::OnMouseMove(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;

  if (nFlags & MK_LBUTTON) {
    double dx = (point.x - m_dragStart.x);
    double dy = (point.y - m_dragStart.y);
    double dist = sqrt(dx * dx + dy * dy);
    if (dist > 5) {
      m_dragging = true;
    }
  } else
    m_dragging = false;

  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over && m_dragging) {
      TrackSection *t = over->getTrackSelection();
      if (t) {
        int dx = point.x;
        int dy = point.y;
        CPoint pt = sview->GetScrollPosition();

        dx += pt.x;
        dy += pt.y;

        double sx = t->getStartX();
        double sy = t->getStartY();
        double ex = sview->display->getObjectX(dx);
        double ey = sview->display->getObjectY(dy);

        double pdx = ex - sx;
        double pdy = ey - sy;

        int length = (int)sqrt(pdx * pdx + pdy * pdy);

        if (length >= 255) {
          length = 255;
        }

        if (length <= 1) length = 1;

        t->setLength(length);

        // over->PutTrackSection(t);
        over->regenerate = TRUE;

        over->createTrack(pDoc);

        sview->repaint();
      }
    } else if (over && changingAngle) {
      TrackSection *t = over->getTrackSelection();
      if (t) {
        CPoint pt = sview->GetScrollPosition();
        int dx = point.x;// - dragstartpoint.x;
        int dy = point.y;// - dragstartpoint.y;
        dx += pt.x;
        dy += pt.y;

        double sx = t->getStartX();
        double sy = t->getStartY();
        double ex = sview->display->getObjectX(dx);
        double ey = sview->display->getObjectY(dy);

        double pdx = ex - sx;
        double pdy = ey - sy;

        double length = (double)sqrt(pdx * pdx + pdy * pdy);

        double SectorLength = (double)t->getLength();

        if (dx != 0 && dy != 0) {
          double newangle = atan(pdy / 50);
          t->setAngle(newangle);

          // over->PutTrackSection(t);
          over->regenerate = TRUE;

          over->createTrack(pDoc);

          sview->repaint();
        }
      }
    }
  }
}

void TrackTool::OnLButtonUp(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  CPoint pt;
  CPoint offset = sview->GetDeviceScrollPosition();

  pt.x = point.x + offset.x;
  pt.y = point.y + offset.y;
  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over) {
      TrackSection *t = over->getTrackSelection();
      if (t) {
        BOOL sel = sview->getTree()->Expand(
          sview->getTree()->GetParentItem(t->getTreeNode()), TVE_EXPAND);
        TRACE("sel=%d\n" + sel);
        sel = sview->getTree()->SelectItem(t->getTreeNode());
        TRACE("sel=%d\n" + sel);
      }
    }
  }

  sview->repaint();
  m_dragging = FALSE;
}

void TrackTool::OnLButtonDblClk(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  GPTrack *mytrack = pDoc->getTrack();

  TrackSection *t = mytrack->getTrackSelection();

  CTrChangeDlg *trkdlg = new CTrChangeDlg(view);

  trkdlg->EditTrackSections(t);

  delete trkdlg;
  sview->repaint();
}

#define DELTA_ANG (10.0 * 360.0 / (double)0xFFFF)

#define VK_A 0x41
#define VK_S 0x53
#define VK_Z 0x5a
#define VK_0 0x30
#define VK_9 0x39
#define VK_O 0x4f
#define VK_P 0x50

#define DELTA_WALL 10

void TrackTool::OnKeyDown(CView *view, CTrackEditorDoc *pDoc, UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if (pDoc) {
    GPTrack *over = pDoc->getTrack();
    over->regenerate = TRUE;
    TrackSection *t = over->getTrackSelection();
    if (over) {
      switch (nChar) {
      case VK_A: {
        TrackSection *next = over->GetNextSection(t);
        if (next) over->setTrackSelection(next);
      } break;
      case VK_Z: {
        TrackSection *next = over->GetPreviousSection(t);
        if (next) over->setTrackSelection(next);
      } break;
      case VK_9: {
        if (t) t->setLeftToBank(t->getLeftToBank() + DELTA_WALL);
        over->createTrack(pDoc);
      } break;
      case VK_0: {
        if (t) t->setLeftToBank(t->getLeftToBank() - DELTA_WALL);
        over->createTrack(pDoc);
      } break;
      case VK_O: {
        if (t) t->setRightToBank(t->getRightToBank() + DELTA_WALL);
        over->createTrack(pDoc);
      } break;
      case VK_P: {
        if (t) t->setRightToBank(t->getRightToBank() - DELTA_WALL);
        over->createTrack(pDoc);
      } break;

      case VK_S: {
        if (t) {
          t->setAngle(0);
          over->createTrack(pDoc);
        }
      }
      case VK_LEFT:
        if (t) {
          t->setAngle(t->getAngle() + DELTA_ANG);
          over->createTrack(pDoc);
        }
        break;
      case VK_RIGHT:
        if (t) {
          t->setAngle(t->getAngle() - DELTA_ANG);
          over->createTrack(pDoc);
        }
        break;
      case VK_UP:
        if (t) {
          t->setLength(t->getLength() + 1);
          over->createTrack(pDoc);
        }
        break;
      case VK_DOWN:
        if (t) {
          t->setLength(t->getLength() - 1);
          over->createTrack(pDoc);
        }
        break;
      case VK_DELETE:
        RemoveSection(view, pDoc);
        break;

      case VK_INSERT:
        InsertSection(view, pDoc);
        break;
      case 36:
        ((TrackEditorScrollView *)view)->OnZoomHome();

        break;
      case 109:
        ((TrackEditorScrollView *)view)->display->ZoomIn();

        break;
      case 108:
        ((TrackEditorScrollView *)view)->display->ZoomOut();

        break;
      default: {
        char buf[256];
        wsprintf(buf, "%d", nChar);
        // AfxMessageBox(buf);
        return;
      }
      }
    }
    pDoc->UpdateAllViews(NULL);
  }
}

void TrackTool::OnRButtonDown(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  changingAngle = TRUE;
}

void TrackTool::OnRButtonUp(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  changingAngle = FALSE;
}

void TrackTool::OnMButtonDown(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  // changingAngle = TRUE;
}

void TrackTool::OnMButtonUp(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  // changingAngle = FALSE;
}

//////////////////////////////////////////////
// Object Tool
void ObjectTool::OnLButtonDown(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  m_dragStart = point;
  CPoint pt;
  CPoint offset = sview->GetDeviceScrollPosition();

  pt.x = point.x + offset.x;
  pt.y = point.y + offset.y;

  pt.x = (long)sview->display->getObjectX(pt.x);
  pt.y = (long)sview->display->getObjectY(pt.y);

  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over) {
      over->selectedObject(pt);
      m_dragging = TRUE;
    }
  }

  sview->repaint();
}

void ObjectTool::OnMouseMove(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;

  if (nFlags & MK_LBUTTON) {
    double dx = (point.x - m_dragStart.x);
    double dy = (point.y - m_dragStart.y);
    double dist = sqrt(dx * dx + dy * dy);
    if (dist > 5) {
      m_dragging = true;
    }
  } else
    m_dragging = false;

  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over && m_dragging) {
      TrackObject *t = over->GetSelectedObject();
      if (t) {
        int dx = point.x;
        int dy = point.y;
        CPoint pt = sview->GetScrollPosition();

        dx += pt.x;
        dy += pt.y;

        double sx = t->getTrackSection()->getStartX();
        double sy = t->getTrackSection()->getStartY();
        double ex = sview->display->getObjectX(dx);
        double ey = sview->display->getObjectY(dy);

        double pdx = ex - sx;
        double pdy = ey - sy;

        int length = (int)sqrt(pdx * pdx + pdy * pdy);

        // TRACE("length=%d",length);

        t->setDistance(length);

        // t->setLength(length);

        // over->PutTrackSection(t);

        over->createTrack(pDoc);

        sview->repaint();
      }
    } else if (over && changingAngle) {
      TrackObject *t = over->GetSelectedObject();
      if (t) {
        int dx = point.x;
        int dy = point.y;
        CPoint pt = sview->GetScrollPosition();

        dx += pt.x;
        dy += pt.y;

        double sx = t->getTrackSection()->getStartX();
        double sy = t->getTrackSection()->getStartY();
        double ex = sview->display->getObjectX(dx);
        double ey = sview->display->getObjectY(dy);

        double pdx = ex - sx;
        double pdy = ey - sy;

        double length = (int)sqrt(pdx * pdx + pdy * pdy);

        // TRACE("length=%d",length);

        TrackObjectDefinition *obj = over->getObjectDefinition(t->getLocator());

        obj->setDistance((int)(TRACKWIDTH_OBJECTS_TO_WIDTH(length)));

        over->createTrack(pDoc);

        sview->repaint();
      }
    }
  }
}

void ObjectTool::OnLButtonUp(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  CPoint pt;
  CPoint offset = sview->GetDeviceScrollPosition();

  pt.x = point.x + offset.x;
  pt.y = point.y + offset.y;
  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over) {
      over->selectedSection(pt);

      TrackObject *t = over->GetSelectedObject();
      if (t) {
        sview->getTree()->Expand(
          sview->getTree()->GetParentItem(t->getTreeNode()), TVE_EXPAND);
        sview->getTree()->SelectItem(t->getTreeNode());
      }
    }
  }

  sview->repaint();
  m_dragging = FALSE;
}

void ObjectTool::OnLButtonDblClk(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  GPTrack *mytrack = pDoc->getTrack();

  if (mytrack != NULL) {
    TrackObject *t = mytrack->GetSelectedObject();

    if (t) {
      CObjectPos *trkdlg = new CObjectPos(mytrack, view);

      if (trkdlg && t) trkdlg->setSection(t);

      int res = trkdlg->DoModal();

      if (res != IDCANCEL && trkdlg && t) {
        trkdlg->getSection(t);
        // mytrack->PutTrackSection(t);
        mytrack->regenerate = TRUE;
      }
      delete trkdlg;
    }
  }

  sview->repaint();
}

void ObjectTool::OnKeyDown(CView *view, CTrackEditorDoc *pDoc, UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if (pDoc) {
    GPTrack *over = pDoc->getTrack();
    over->regenerate = TRUE;
    TrackObject *t = over->GetSelectedObject();
    if (over) {
      switch (nChar) {
      case VK_LEFT:
        if (t) {
          t->setDistance(t->getDistance() - 1);
          over->createTrack(pDoc);
        }
        break;
      case VK_RIGHT:
        if (t) {
          t->setDistance(t->getDistance() + 1);
          over->createTrack(pDoc);
        }
        break;
      case VK_UP:
        if (t) {
          TrackObjectDefinition *od = t->getObjectDef();
          od->setDistance(od->getDistance() + 20);
          over->createTrack(pDoc);
        }
        break;
      case VK_DOWN:
        if (t) {
          TrackObjectDefinition *od = t->getObjectDef();
          od->setDistance(od->getDistance() - 20);
          over->createTrack(pDoc);
        }
        break;

      default: {
        char buf[256];
        wsprintf(buf, "%d", nChar);
        // AfxMessageBox(buf);
        return;
      }
      }
    }
    pDoc->UpdateAllViews(NULL);
  }
}

void ObjectTool::OnRButtonDown(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  changingAngle = TRUE;
}

void ObjectTool::OnRButtonUp(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  changingAngle = FALSE;
}

void ObjectTool::InsertObject(CView *view, CTrackEditorDoc *pDoc)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  // AfxMessageBox("Insert Object",MB_OK);

  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over) {
      over->regenerate = TRUE;
      TrackSection *t = over->getTrackSelection();
      if (t) {
        TrackObjectDefinition *obj =
          (TrackObjectDefinition *)over->TrackObjectDefintions->elementAt(0);

        Vector *SectionCmds = t->getCommands();

        TrackObject *trackcmd =
          new TrackObject(over, t, 0, obj->getLocator(), 0);
        trackcmd->AddWordArgument(10, "Inserted Object Distance");
        trackcmd->AddWordArgument(obj->getLocator(), "Inserted Object Locator");
        SectionCmds->addElement(trackcmd);

        if (trackcmd) {
          CObjectPos *trkdlg = new CObjectPos(over, view);

          if (trkdlg && trackcmd) trkdlg->setSection(trackcmd);

          int res = trkdlg->DoModal();

          if (res != IDCANCEL && trkdlg && trackcmd) {
            trkdlg->getSection(trackcmd);
            over->regenerate = TRUE;
          }
          delete trkdlg;
        }
      }
    }
  }
  sview->repaint();
}

void ObjectTool::RemoveObject(CView *view, CTrackEditorDoc *pDoc)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;

  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over) {
      over->regenerate = TRUE;
      TrackObject *tobj = over->GetSelectedObject();
      // TrackSection *t = over->getTrackSelection();
      TrackSection *t = tobj->owner;
      if (t) {
        int result =
          AfxMessageBox("Removing Object From Track, Continue?", MB_YESNO);
        if (result == IDYES) {
          t->getCommands()->removeElement(tobj);
        }
      }
    }
  }
  sview->repaint();
}

void ZoomTool::OnLButtonDown(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  m_dragStart = point;
  CPoint pt;
  CPoint offset = sview->GetDeviceScrollPosition();
  pt.x = point.x + offset.x;
  pt.y = point.y + offset.y;

  double wptx = sview->display->getObjectX(pt.x);
  double wpty = sview->display->getObjectY(pt.y);

  if ((nFlags & MK_SHIFT) || (nFlags & MK_CONTROL)) {
    sview->display->ZoomIn();
    sview->display->ZoomIn();
    sview->display->ZoomIn();
    sview->display->ZoomIn();
    sview->display->ZoomIn();
    sview->display->ZoomIn();
    sview->display->ZoomIn();
    sview->display->ZoomIn();
  } else {
    sview->display->ZoomIn();
  }
  sview->Resize(sview->display->getScale());

  double nptx = sview->display->getScreenX(wptx);
  double npty = sview->display->getScreenY(wpty);

  pt.x = (long)nptx;
  pt.y = (long)npty;

  RECT rect;
  sview->GetClientRect(&rect);

  int width = rect.right - rect.left;
  int height = rect.bottom - rect.top;

  pt.x -= (width / 2);
  pt.y -= (height / 2);

  sview->ScrollToPosition(pt);
  // POINT scptr;
  // scptr.x = pt.x;
  // scptr.y = pt.y;
  // sview->ClientToScreen(&scptr);
  // SetCursorPos(scptr.x,scptr.y);

  sview->repaint();
}

void ZoomTool::OnRButtonDown(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  CPoint pt;
  CPoint offset = sview->GetDeviceScrollPosition();
  pt.x = point.x + offset.x;
  pt.y = point.y + offset.y;

  double wptx = sview->display->getObjectX(pt.x);
  double wpty = sview->display->getObjectY(pt.y);

  if ((nFlags & MK_SHIFT) || (nFlags & MK_CONTROL)) {
    sview->display->ZoomOut();
    sview->display->ZoomOut();
    sview->display->ZoomOut();
    sview->display->ZoomOut();
    sview->display->ZoomOut();
    sview->display->ZoomOut();
    sview->display->ZoomOut();
    sview->display->ZoomOut();
  } else {
    sview->display->ZoomOut();
  }
  sview->Resize(sview->display->getScale());

  double nptx = sview->display->getScreenX(wptx);
  double npty = sview->display->getScreenY(wpty);

  pt.x = (long)nptx;
  pt.y = (long)npty;

  RECT rect;
  sview->GetClientRect(&rect);

  int width = rect.right - rect.left;
  int height = rect.bottom - rect.top;

  pt.x -= width / 2;
  pt.y -= height / 2;

  sview->ScrollToPosition(pt);
  // POINT scptr;
  // scptr.x = pt.x;
  // scptr.y = pt.y;
  // sview->ClientToScreen(&scptr);
  // SetCursorPos(scptr.x,scptr.y);
  sview->repaint();
}

//////////////////////////////////////////////////////
// Pitlane
void PitLaneTool::OnLButtonDown(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  m_dragStart = point;
  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over) {
      TrackSection *t = over->getPitLaneSelection();
      if (t) {
        m_dragging = TRUE;
      } else {
        m_dragging = FALSE;
      }
    } else {
      m_dragging = FALSE;
    }
  }
}

void PitLaneTool::OnMouseMove(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;

  if (nFlags & MK_LBUTTON) {
    double dx = (point.x - m_dragStart.x);
    double dy = (point.y - m_dragStart.y);
    double dist = sqrt(dx * dx + dy * dy);
    if (dist > 5) {
      m_dragging = true;
    }
  } else
    m_dragging = false;

  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over && m_dragging) {
      TrackSection *t = over->getPitLaneSelection();
      if (t) {
        int dx = point.x;
        int dy = point.y;
        CPoint pt = sview->GetScrollPosition();

        dx += pt.x;
        dy += pt.y;

        double sx = t->getStartX();
        double sy = t->getStartY();
        double ex = sview->display->getObjectX(dx);
        double ey = sview->display->getObjectY(dy);

        double pdx = ex - sx;
        double pdy = ey - sy;

        int length = (int)sqrt(pdx * pdx + pdy * pdy);

        if (length >= 255) {
          length = 255;
        }

        if (length <= 1) length = 1;

        t->setLength(length);

        over->regenerate = TRUE;

        over->createTrack(pDoc);

        sview->repaint();
      }
    } else if (over && changingAngle) {
      TrackSection *t = over->getPitLaneSelection();
      if (t) {
        CPoint pt = sview->GetScrollPosition();
        int dx = point.x;
        int dy = point.y;
        dx += pt.x;
        dy += pt.y;

        double sx = t->getStartX();
        double sy = t->getStartY();
        double ex = sview->display->getObjectX(dx);
        double ey = sview->display->getObjectY(dy);

        double pdx = ex - sx;
        double pdy = ey - sy;

        double length = (double)sqrt(pdx * pdx + pdy * pdy);

        double SectorLength = (double)t->getLength();

        if (dx != 0 && dy != 0) {
          double newangle = atan(pdy / 50);
          t->setAngle(newangle);
          over->regenerate = TRUE;
          over->createTrack(pDoc);
          sview->repaint();
        }
      }
    }
  }
}

void PitLaneTool::OnLButtonDblClk(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  GPTrack *mytrack = pDoc->getTrack();

  TrackSection *t = mytrack->getPitLaneSelection();

  CTrChangeDlg *trkdlg = new CTrChangeDlg(view);

  trkdlg->EditTrackSections(t);

  delete trkdlg;
  sview->repaint();
}

void PitLaneTool::OnLButtonUp(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  CPoint pt;
  CPoint offset = sview->GetDeviceScrollPosition();

  pt.x = point.x + offset.x;
  pt.y = point.y + offset.y;
  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over) {
      over->selectedPitSection(pt);

      TrackSection *t = over->getPitLaneSelection();

      if (t) {
        sview->getTree()->Expand(
          sview->getTree()->GetParentItem(t->getTreeNode()), TVE_EXPAND);
        sview->getTree()->SelectItem(t->getTreeNode());
      }
    }
  }

  sview->repaint();
  m_dragging = FALSE;
}

void PitLaneTool::OnKeyDown(CView *view, CTrackEditorDoc *pDoc, UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if (pDoc) {
    GPTrack *over = pDoc->getTrack();
    over->regenerate = TRUE;
    TrackSection *t = over->getPitLaneSelection();
    if (over) {
      switch (nChar) {
      case VK_A: {
        TrackSection *next = over->GetNextSection(t);
        if (next) over->setPitSelection(next);
      } break;
      case VK_Z: {
        TrackSection *next = over->GetPreviousSection(t);
        if (next) over->setPitSelection(next);
      } break;
      case VK_9: {
        if (t) t->setLeftToBank(t->getLeftToBank() + DELTA_WALL);
        over->createTrack(pDoc);
      } break;
      case VK_0: {
        if (t) t->setLeftToBank(t->getLeftToBank() - DELTA_WALL);
        over->createTrack(pDoc);
      } break;
      case VK_O: {
        if (t) t->setRightToBank(t->getRightToBank() + DELTA_WALL);
        over->createTrack(pDoc);
      } break;
      case VK_P: {
        if (t) t->setRightToBank(t->getRightToBank() - DELTA_WALL);
        over->createTrack(pDoc);
      } break;
      case VK_S: {
        if (t) {
          t->setAngle(0);
          over->createTrack(pDoc);
        }
      }
      case VK_LEFT:
        if (t) {
          t->setAngle(t->getAngle() + DELTA_ANG);
          over->createTrack(pDoc);
        }
        break;
      case VK_RIGHT:
        if (t) {
          t->setAngle(t->getAngle() - DELTA_ANG);
          over->createTrack(pDoc);
        }
        break;
      case VK_UP:
        if (t) {
          t->setLength(t->getLength() + 1);
          over->createTrack(pDoc);
        }
        break;
      case VK_DOWN:
        if (t) {
          t->setLength(t->getLength() - 1);
          over->createTrack(pDoc);
        }
        break;
      case VK_DELETE:
        RemoveSection(view, pDoc);
        break;

      case VK_INSERT:
        InsertSection(view, pDoc);
        break;
      case 36:
        ((TrackEditorScrollView *)view)->OnZoomHome();

        break;
      case 109:
        ((TrackEditorScrollView *)view)->display->ZoomIn();

        break;
      case 108:
        ((TrackEditorScrollView *)view)->display->ZoomOut();

        break;
      default: {
        char buf[256];
        wsprintf(buf, "%d", nChar);
        // AfxMessageBox(buf);
        return;
      }
      }
    }
    pDoc->UpdateAllViews(NULL);
  }
}

void PitLaneTool::OnRButtonDown(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  changingAngle = TRUE;
}

void PitLaneTool::OnRButtonUp(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  changingAngle = FALSE;
}

void PitLaneTool::InsertSection(CView *view, CTrackEditorDoc *pDoc)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;

  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over) {
      over->regenerate = TRUE;
      TrackSection *t = over->getPitLaneSelection();

      if (t) {
        Vector *sectionCmds = new Vector("Adding Pit Section", __LINE__);
        TrackSection *ts = new TrackSection(
          over->PitlaneSections, over, 0, 20, 0, 0, t->getLeftToBank(), t->getRightToBank(), t->getRoadSignData(), t->getKerbData(), NULL, 0, t->getWidthLeft(), t->getWidthRight(), sectionCmds, t_PitSection);

        over->PitlaneSections->insertAfterElement(t, ts);
        over->createTrack(pDoc);
      }
    }
  }
  sview->repaint();
}

void PitLaneTool::RemoveSection(CView *view, CTrackEditorDoc *pDoc)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;

  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over) {
      over->regenerate = TRUE;
      TrackSection *t = over->getPitLaneSelection();

      if (t) {
        over->PitlaneSections->removeElement(t);
        over->createTrack(pDoc);
      }
    }
  }
  sview->repaint();
}

void PitLaneTool::InsertObject(CView *view, CTrackEditorDoc *pDoc)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  // AfxMessageBox("Insert Object",MB_OK);

  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over) {
      over->regenerate = TRUE;
      TrackSection *t = over->getPitLaneSelection();
      if (t) {
        TrackObjectDefinition *obj =
          (TrackObjectDefinition *)over->TrackObjectDefintions->elementAt(0);

        Vector *SectionCmds = t->getCommands();

        TrackObject *trackcmd =
          new TrackObject(over, t, 10, obj->getLocator(), 0);
        trackcmd->AddWordArgument(10, "Inserted Object Distance");
        trackcmd->AddWordArgument(obj->getLocator(), "Inserted Object Locator");
        SectionCmds->addElement(trackcmd);
      }
    }
  }
  sview->repaint();
}

void PitLaneTool::RemoveObject(CView *view, CTrackEditorDoc *pDoc)
{
  AfxMessageBox("Not implemented yet");
}

////////////////////////////////////
// WallTool
void WallTool::OnLButtonDown(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  m_dragStart = point;
  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over) {
      TrackSection *t = over->getTrackSelection();
      if (t) {
        m_dragging = TRUE;
      } else {
        m_dragging = FALSE;
      }
    } else {
      m_dragging = FALSE;
    }
  }
}

void WallTool::OnMouseMove(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;

  if (nFlags & MK_LBUTTON) {
    double dx = (point.x - m_dragStart.x);
    double dy = (point.y - m_dragStart.y);
    double dist = sqrt(dx * dx + dy * dy);
    if (dist > 5) {
      m_dragging = true;
    }
  } else
    m_dragging = false;

  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over && m_dragging) {
      TrackSection *t = over->getTrackSelection();
      if (t) {
        int dx = point.x;
        int dy = point.y;
        CPoint pt = sview->GetScrollPosition();

        dx += pt.x;
        dy += pt.y;

        double sx = t->getStartX();
        double sy = t->getStartY();
        double ex = sview->display->getObjectX(dx);
        double ey = sview->display->getObjectY(dy);

        double pdx = ex - sx;
        double pdy = ey - sy;

        double length = sqrt(pdx * pdx + pdy * pdy);

        t->setLeftToBank((int)(length * 0x10));

        // t->setLength(length);

        // over->PutTrackSection(t);
        over->regenerate = TRUE;

        over->createTrack(pDoc);
        sview->repaint();
      }
    } else if (over && changingAngle) {
      TrackSection *t = over->getTrackSelection();
      if (t) {
        int dx = point.x;
        int dy = point.y;
        CPoint pt = sview->GetScrollPosition();

        dx += pt.x;
        dy += pt.y;

        double sx = t->getStartX();
        double sy = t->getStartY();
        double ex = sview->display->getObjectX(dx);
        double ey = sview->display->getObjectY(dy);

        double pdx = ex - sx;
        double pdy = ey - sy;

        double length = sqrt(pdx * pdx + pdy * pdy);

        t->setRightToBank((int)(length * 0x10));

        // t->setLength(length);

        // over->PutTrackSection(t);
        over->regenerate = TRUE;

        over->createTrack(pDoc);
        sview->repaint();
      }
    }
  }
}

void WallTool::OnLButtonUp(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  CPoint pt;
  CPoint offset = sview->GetDeviceScrollPosition();

  pt.x = point.x + offset.x;
  pt.y = point.y + offset.y;
  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over) {
      over->selectedSection(pt);

      TrackSection *t = over->getTrackSelection();
    }
  }

  sview->repaint();
  m_dragging = FALSE;
}

void WallTool::OnKeyDown(CView *view, CTrackEditorDoc *pDoc, UINT nChar, UINT nRepCnt, UINT nFlags)
{}

void WallTool::OnRButtonDown(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  changingAngle = TRUE;
}

void WallTool::OnRButtonUp(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  changingAngle = FALSE;
}

///////////////////////////////////////
void CCTool::OnLButtonDown(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  m_dragStart = point;
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;

  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over) {
      CCLineSection *t = over->getCCSelection();
      if (t) {
        m_dragging = TRUE;
      } else {
        m_dragging = FALSE;
      }
    } else {
      m_dragging = FALSE;
    }
  }
}

void CCTool::OnMouseMove(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;

  if (nFlags & MK_LBUTTON) {
    double dx = (point.x - m_dragStart.x);
    double dy = (point.y - m_dragStart.y);
    double dist = sqrt(dx * dx + dy * dy);
    if (dist > 5) {
      m_dragging = true;
    }
  } else
    m_dragging = false;

  if (pDoc) {
    GPTrack *over = pDoc->getTrack();
    BOOL m_dragging = nFlags & MK_LBUTTON;

    if (over && m_dragging) {
      CCLineSection *t = over->getCCSelection();
      if (t) {
        int dx = point.x;
        int dy = point.y;
        CPoint pt = sview->GetScrollPosition();

        dx += pt.x;
        dy += pt.y;

        double sx = t->getStartX();
        double sy = t->getStartY();
        double ex = sview->display->getObjectX(dx);
        double ey = sview->display->getObjectY(dy);

        double pdx = ex - sx;
        double pdy = ey - sy;

        int length = (int)sqrt(pdx * pdx + pdy * pdy);

        if (length >= 255) {
          length = 255;
        }

        if (length <= 1) length = 1;

        t->setLength(length);

        // over->PutCCSection(t);
        over->regenerate = TRUE;

        over->createCC();

        sview->repaint();
      }
    } else if (over && changingAngle) {
      CCLineSection *t = over->getCCSelection();
      if (t) {
        CPoint pt = sview->GetScrollPosition();
        int dx = point.x;// - dragstartpoint.x;
        int dy = point.y;// - dragstartpoint.y;
        dx += pt.x;
        dy += pt.y;

        double sx = t->getStartX();
        double sy = t->getStartY();
        double ex = sview->display->getObjectX(dx);
        double ey = sview->display->getObjectY(dy);

        double pdx = ex - sx;
        double pdy = ey - sy;

        double length = (double)sqrt(pdx * pdx + pdy * pdy);

        double SectorLength = (double)t->getLength();

        int rad = (int)(length * 1024.0);

        if (rad >= 65535) rad = 65535;

        t->setRadius(rad);

        if (dx != 0 && dy != 0) {
          double newangle = atan(pdy / 50);
          // t->setAngle(newangle);

          // over->PutCCSection(t);
          over->regenerate = TRUE;

          over->createCC();

          sview->repaint();
        }
      }
    }
  }
}

void CCTool::OnLButtonUp(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  CPoint pt;
  CPoint offset = sview->GetDeviceScrollPosition();

  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (m_dragging) {
      if (over) {
        over->clearCCSelection();
      }
    } else {
      pt.x = point.x + offset.x;
      pt.y = point.y + offset.y;
      pt.x = (long)sview->display->getObjectX(pt.x);
      pt.y = (long)sview->display->getObjectY(pt.y);

      if (over) {
        over->selectedCCSection(pt);
      }
    }
  }

  m_dragging = FALSE;
  sview->repaint();
}

#define DELTA_RAD 100

void CCTool::OnKeyDown(CView *view, CTrackEditorDoc *pDoc, UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if (pDoc) {
    GPTrack *over = pDoc->getTrack();
    over->regenerate = TRUE;
    CCLineSection *t = over->getCCSelection();
    if (over) {
      switch (nChar) {
      case VK_A: {
        CCLineSection *next = over->GetNextSection(t);
        if (next) over->setCCSelection(next);
      } break;
      case VK_Z: {
        CCLineSection *next = over->GetPreviousSection(t);
        if (next) over->setCCSelection(next);
      } break;
      case VK_S: {
        if (t) {
          t->setAngle(0);
          over->createTrack(pDoc);
        }
      }
      case VK_LEFT:
        if (t) {
          t->setRadius(t->getRadius() + DELTA_RAD);
          over->createCC();
        }
        break;
      case VK_RIGHT:
        if (t) {
          t->setRadius(t->getRadius() - DELTA_RAD);
          over->createCC();
        }
        break;
      case VK_UP:
        if (t) {
          t->setLength(t->getLength() + 1);
          over->createCC();
        }
        break;
      case VK_DOWN:
        if (t) {
          t->setLength(t->getLength() - 1);
          over->createCC();
        }
        break;
      default: {
        char buf[256];
        wsprintf(buf, "%d", nChar);
        // AfxMessageBox(buf);
        return;
      }
      }
    }
    pDoc->UpdateAllViews(NULL);
  }
}

void CCTool::OnRButtonDown(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  changingAngle = TRUE;
}

void CCTool::OnRButtonUp(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  changingAngle = FALSE;
}

void CCTool::OnLButtonDblClk(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  GPTrack *mytrack = pDoc->getTrack();
  CPoint pt;
  CPoint offset = sview->GetDeviceScrollPosition();

  pt.x = point.x + offset.x;
  pt.y = point.y + offset.y;
  pt.x = (long)sview->display->getObjectX(pt.x);
  pt.y = (long)sview->display->getObjectY(pt.y);

  if (mytrack) {
    mytrack->selectedCCSection(pt);
  }

  CCLineSection *t = mytrack->getCCSelection();

  CCLineEditor *trkdlg = new CCLineEditor(view);

  trkdlg->EditCCSections(t);

  delete trkdlg;
  sview->repaint();
}

void CCTool::InsertSection(CView *view, CTrackEditorDoc *pDoc)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;

  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over) {
      over->regenerate = TRUE;
      CCLineSection *t = over->getCCSelection();

      if (t) {
        CCLineSection *ts = new CCLineSection(over, 10, 0, 0, 0, 0, 0, 0x50, 0);

        over->CCLineSections->insertAfterElement(t, ts);
        over->createCC();
      }
    }
  }
  sview->repaint();
}

void CCTool::RemoveSection(CView *view, CTrackEditorDoc *pDoc)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;

  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over) {
      over->regenerate = TRUE;
      CCLineSection *t = over->getCCSelection();

      if (t) {
        over->CCLineSections->removeElement(t);
        over->createCC();
      }
    }
  }
  sview->repaint();
}

//////////////////////////////////////////
// Ruler Tool

void RulerTool::OnLButtonDown(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  m_dragStart = point;
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  CPoint offset = sview->GetDeviceScrollPosition();
  CPoint pt;

  if (sview->ruler) delete (sview->ruler);

  sview->ruler = new Ruler();

  // pt.x = (int)sview->display->getObjectX(point.x+offset.x);
  // pt.y = (int)sview->display->getObjectY(point.y+offset.y);
  pt.x = (int)(point.x + offset.x);
  pt.y = (int)(point.y + offset.y);

  sview->ruler->BeginPoint = pt;
  sview->ruler->EndPoint = pt;

  m_dragging = TRUE;
  sview->repaint();
}

void RulerTool::OnMouseMove(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  CPoint offset = sview->GetDeviceScrollPosition();
  CPoint pt;

  if (m_dragging) {
    char message[256];

    pt.x = (int)(point.x + offset.x);
    pt.y = (int)(point.y + offset.y);

    sview->ruler->EndPoint = pt;

    double miles = sview->ruler->getDistanceMiles(sview->display);
    double meters = sview->ruler->getDistanceKm(sview->display);
    double dist = sview->ruler->getDistance(sview->display);

    sprintf(message, "Distance=%2.3f [%2.3f Miles] [%2.3f Km]", dist, miles, meters);
    theApp.SetMessageText(message);

    sview->repaint();
  }
}

void RulerTool::OnLButtonUp(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;

  if (sview->ruler) {
    delete sview->ruler;
    sview->ruler = NULL;
  }

  sview->repaint();
  m_dragging = FALSE;
}

void PointerTool::OnLButtonDown(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  m_dragStart = point;
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over) {
      CPoint pt;
      CPoint offset = sview->GetDeviceScrollPosition();

      pt.x = point.x + offset.x;
      pt.y = point.y + offset.y;

      over->selectedSection(pt);
      TrackSection *t = over->getTrackSelection();

      if (t == NULL) {
        BeginDragPoint.x = pt.x;
        BeginDragPoint.y = pt.y;
        EndDragPoint.x = pt.x;
        EndDragPoint.y = pt.y;
        m_dragging = TRUE;
      } else {
        m_dragging = FALSE;
      }
    }
  }
}

void PointerTool::OnMouseMove(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  if (m_dragging) {
    CPoint pt;
    CPoint offset = sview->GetDeviceScrollPosition();
    CDC *pDC = sview->GetDC();

    pDC->SelectStockObject(NULL_BRUSH);
    pDC->SelectStockObject(BLACK_PEN);
    pDC->SetROP2(R2_NOT);
    pDC->Rectangle(BeginDragPoint.x, BeginDragPoint.y, EndDragPoint.x, EndDragPoint.y);

    pt.x = point.x + offset.x;
    pt.y = point.y + offset.y;
    EndDragPoint.x = pt.x;
    EndDragPoint.y = pt.y;

    // pDC->MoveTo(BeginDragPoint.x,BeginDragPoint.y);
    // pDC->LineTo(EndDragPoint.x,EndDragPoint.y);

    pDC->Rectangle(BeginDragPoint.x, BeginDragPoint.y, EndDragPoint.x, EndDragPoint.y);
    // pDC->SetROP2(R2_BLACK);
  }
}

void PointerTool::OnKeyDown(CView *view, CTrackEditorDoc *pDoc, UINT nChar, UINT nRepCnt, UINT nFlags)
{
  GPTrack *over = pDoc->getTrack();
  switch (nChar) {
  case 46: {
    // remove
    for (int i = 0; i < selections->size(); i++) {
      TrackSection *selt = (TrackSection *)selections->elementAt(i);
      over->TrackSections->removeElement(selt);
    }
    over->createTrack(pDoc);
  } break;
  case 45:// insert
    break;
  default: {
    char buf[256];
    wsprintf(buf, "%d", nChar);
    // AfxMessageBox(buf);
    return;
  }
  }
  pDoc->UpdateAllViews(NULL);
}

void PointerTool::OnLButtonUp(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  CPoint pt;
  CPoint offset = sview->GetDeviceScrollPosition();

  pt.x = point.x + offset.x;
  pt.y = point.y + offset.y;
  if (pDoc) {
    GPTrack *over = pDoc->getTrack();

    if (over) {
      if (!m_dragging) {
        TrackSection *t = over->getTrackSelection();
        if (t) {
          sview->getTree()->Expand(
            sview->getTree()->GetParentItem(t->getTreeNode()), TVE_EXPAND);
          sview->getTree()->SelectItem(t->getTreeNode());
        }
      } else {
        // capture track section in the area
        CPoint begin;
        CPoint end;

        begin.x = sview->display->getScreenX(BeginDragPoint.x);
        begin.y = sview->display->getScreenY(BeginDragPoint.y);

        end.x = sview->display->getScreenX(EndDragPoint.x);
        end.y = sview->display->getScreenY(EndDragPoint.y);

        selections = over->getSelectedSections(begin, end);
        for (int i = 0; i < selections->size(); i++) {
          TrackSection *selt = (TrackSection *)selections->elementAt(i);
          selt->setSelected(TRUE);
        }

        // char msg[256];
        // wsprintf(msg,"Selected %d",selections->size());
        // AfxMessageBox(msg);
      }
    }
  }
  sview->repaint();
  m_dragging = FALSE;
}

void PointerTool::OnLButtonDblClk(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  GPTrack *mytrack = pDoc->getTrack();

  TrackSection *t = mytrack->getTrackSelection();

  CTrChangeDlg *trkdlg = new CTrChangeDlg(view);

  trkdlg->EditTrackSections(t);

  delete trkdlg;
  sview->repaint();
}

void HandTool::OnLButtonDown(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  m_dragStart = point;
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;
  m_dragging = TRUE;
  BeginPoint.x = point.x;
  BeginPoint.y = point.y;
}

void HandTool::OnMouseMove(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;

  if (m_dragging) {
    int dy = BeginPoint.y - point.y;
    int dx = BeginPoint.x - point.x;

    CPoint pt = sview->GetScrollPosition();

    POINT pos;
    pos.x = pt.x + dx;
    pos.y = pt.y + dy;
    sview->ScrollToPosition(pos);

    BeginPoint = point;
  }
}

void HandTool::OnLButtonUp(CView *view, CTrackEditorDoc *pDoc, UINT nFlags, CPoint point)
{
  TrackEditorScrollView *sview = (TrackEditorScrollView *)view;

  m_dragging = FALSE;
}
