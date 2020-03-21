#include "stdafx.h"
#include "Tool.h"
#include "GPTrack.h"
#include "TrackEditorScrollView.h"
#include "TrackEditor.h"
#include "Camera.h"
#include "CameraEdit.h"

extern CTrackEditorApp theApp;

//////////////////////////////////////////////
// Object Tool
void CameraTool::OnLButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point)
{
	TrackEditorScrollView *sview  = (TrackEditorScrollView*)view;
	CPoint pt;
	CPoint offset = sview->GetDeviceScrollPosition();

	pt.x = point.x+offset.x;
	pt.y = point.y+offset.y;

    //pt.x = (long)sview->display->getObjectX(pt.x);	
	//pt.y = (long)sview->display->getObjectY(pt.y);
	
	if (pDoc)
	{
	 GPTrack *over         = pDoc->getTrack();

	 if (over)
	 {
	 over->selectedCamera(pt);
	 m_dragging = TRUE;
	 }
	}

	sview->repaint();
}

void CameraTool::OnMouseMove(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point)
{
	TrackEditorScrollView *sview  = (TrackEditorScrollView*)view;

	if (pDoc)
	{
	 GPTrack *over         = pDoc->getTrack();

	 if (over && m_dragging)
	 {
	   Camera *t = over->GetSelectedCamera();
	   if (t)
	   {
		 /*
		 int dx = point.x;
		 int dy = point.y;
		 CPoint pt = sview->GetScrollPosition();

		 dx+=pt.x;
		 dy+=pt.y;

		 double sx = t->getTrackSection()->getStartX();
		 double sy = t->getTrackSection()->getStartY();
		 double ex = sview->display->getObjectX(dx);
		 double ey = sview->display->getObjectY(dy);

		 double pdx = ex-sx;
		 double pdy = ey-sy;

		 int length = (int)sqrt(pdx*pdx+pdy*pdy);


		 //TRACE("length=%d",length);

		 t->setDistance(length);

		 //t->setLength(length);	 
		 
		 //over->PutTrackSection(t);
		 */

		 over->createTrack(pDoc);
	   
	     sview->repaint();
	   }
	}
	}
}

void CameraTool::OnLButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point)
{
	TrackEditorScrollView *sview  = (TrackEditorScrollView*)view;
	CPoint pt;
	CPoint offset = sview->GetDeviceScrollPosition();

	pt.x = point.x+offset.x;
	pt.y = point.y+offset.y;
	if (pDoc)
	{
	 GPTrack *over         = pDoc->getTrack();

	 if (over)
	 {
	 Camera *t = over->GetSelectedCamera();
	 if (t)
	 {
	   sview->getTree()->Expand(sview->getTree()->GetParentItem(t->getTreeNode()),TVE_EXPAND);
	   sview->getTree()->SelectItem(t->getTreeNode());
	 }
	 }
	}

	sview->repaint();
	m_dragging = FALSE;
}

void CameraTool::OnLButtonDblClk(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point)
{
	TrackEditorScrollView *sview  = (TrackEditorScrollView*)view;
	GPTrack * mytrack     = pDoc->getTrack();

	if (mytrack!=NULL)
	{
	 Camera *t = mytrack->GetSelectedCamera();
	
	 if (t)
	 {
	 t->Open();	
	 }
	 else
	 {
		 AfxMessageBox("No Camera Selected");
	 }
	}	
	sview->repaint();
}

void CameraTool::OnRButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point)
{
	changingAngle = TRUE;
}

void CameraTool::OnRButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point)
{
	changingAngle = FALSE;
}

void  CameraTool::InsertObject(CView *view,CTrackEditorDoc *pDoc)
{
	
	TrackEditorScrollView *sview  = (TrackEditorScrollView*)view;
	//AfxMessageBox("Insert Object",MB_OK);

	if (pDoc)
	{
	 GPTrack *over         = pDoc->getTrack();
	 over->MakeBackupNow();

	 if (over)
	 {
       over->regenerate = TRUE;
	   TrackSection *t = over->getTrackSelection();
	   if (t)
	   {
	    TrackObjectDefinition *obj = (TrackObjectDefinition*)over->TrackObjectDefintions->elementAt(0);

	    Vector *SectionCmds = t->getCommands();

	    TrackObject *trackcmd = new TrackObject(over,t,0,obj->getLocator(),0);
	    trackcmd->AddWordArgument(10,"Inserted Object Distance");
	    trackcmd->AddWordArgument(obj->getLocator(),"Inserted Object Locator");
	    SectionCmds->addElement(trackcmd);

		if (trackcmd)
	    {
	     CCameraEdit *trkdlg = new CCameraEdit(view);

		 int res = trkdlg->DoModal();
	   }
	  }
	 }
	}
	sview->repaint();
}

void CameraTool::OnKeyDown(CView *view,CTrackEditorDoc *pDoc,UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
	if (pDoc)
	{
	  GPTrack *over         = pDoc->getTrack();
	  over->regenerate = TRUE;
	  Camera *t = over->GetSelectedCamera();
	  if (over)
	  {
		switch(nChar)
		{
		   
		   case VK_LEFT:				   
				   if (t)
				   {
					 t->setDistance(t->getDistance()-1);
					 over->createTrack(pDoc);
				   }				 				
			    break;
		   case VK_RIGHT:
				   if (t)
				   {
					 t->setDistance(t->getDistance()+1);
					 over->createTrack(pDoc);
				   }				 				
			    break;
		   case VK_UP:
				   if (t)
				   {
					 t->setPerpendicularDistance(t->getPerpendicularDistance()+20);
					 over->createTrack(pDoc);
				   }				 				
			    break;
		   case VK_DOWN:
				   if (t)
				   {
					 t->setPerpendicularDistance(t->getPerpendicularDistance()-20);
					 over->createTrack(pDoc);
				   }				 				
			    break;
			
		   default:
			{
			char buf[256];
			wsprintf(buf,"%d",nChar);
			//AfxMessageBox(buf);
			return;
			}
		}
	  }
	pDoc->UpdateAllViews(NULL);
	}
}


