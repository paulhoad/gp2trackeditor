
#include "stdafx.h"
#include "TrackObject.h"
#include "ObjectPos.h"
#include "TrackSection.h"

int TrackObject::Open()
{
		CObjectPos *dlg = new CObjectPos(track);

		dlg->setSection(this);
		
		int result=0;
		

		if (dlg)
		{
		  result = dlg->DoModal();
		}

		if (result!=IDCANCEL)
		{
		 dlg->getSection(this);
		 track->regenerate = TRUE;
		}

		//write(track,offset);
		return result;
}

void TrackObject::write(GPTrack *track,int off)
{
	track->WritePCByte(off,getDistance());off+=1;
	track->WritePCByte(off,0x80);off++;
	track->WritePCWord(off,getLocator());off+=2;
}

void TrackObject::draw(Display *g,double x1,double y1,double stAngle,double trackWidth,BOOL showBitmaps,double angle)
    {
		 double nextypos;
		 double nextxpos;

		 double remainder = getDistance();
		 double length    = owner->getLength();

		 if (angle != 0)
		 {
		  CPoint trackPt = GetTrackPoint(stAngle,angle,remainder);
          nextypos = trackPt.x;
          nextxpos = trackPt.y;
	      double deltaAngle = DegToRad(angle*length)*(remainder/length);
	      angle=stAngle+deltaAngle;
		 }
		 else
		 {
		  nextypos = getDistance()*sin(stAngle);
          nextxpos = getDistance()*cos(stAngle);
		  angle = stAngle;
		 }
		 

		 double xr = x1+nextxpos;
		 double yr = y1+nextypos;

		 if (track->showObjectPositionMarker)
		 {
		  g->drawLine(xr-2,yr,xr+2,yr);
		  g->drawLine(xr,yr-2,xr,yr+2);
		 }
		 
		 double leftSideX;
		 double leftSideY;

		 TrackObjectDefinition *obj = getObjectDef();

		
		 double bX;
		 double bY;

         if (obj!=NULL)
		 {
		   double distance = obj->getDistance();
		   g->setColor(0);
		   if (distance > 0)
		   {
			double di = WIDTH_TO_TRACKWIDTH_OBJECTS(fabs(distance));
			leftSideX = x1+nextxpos+getRightSideX(angle,di);
            leftSideY = y1+nextypos+getRightSideY(angle,di);
			if (isSelected())
				g->setColor(SAND_BRUSH);
			else
			    g->setColor(CYAN_BRUSH);


			if (track->showObjectPositionMarker)
		    {
			 bX = x1+nextxpos+getRightSideX(angle,g->getObjectX(16)+di);
             bY = y1+nextypos+getRightSideY(angle,g->getObjectX(16)+di);
			}
			else
			{
			 bX = g->getObjectX(-8)+x1+nextxpos+getRightSideX(angle,di);
             bY = g->getObjectX(-8)+y1+nextypos+getRightSideY(angle,di);
			}

			if (track->showObjectPositionMarker)
		    {
			  g->drawLine(leftSideX,leftSideY,bX,bY);
			}
			
			
		   }
		   else
		   {
			double di = WIDTH_TO_TRACKWIDTH_OBJECTS(fabs(distance));
			leftSideX = x1+nextxpos+getLeftSideX(angle,di);
            leftSideY = y1+nextypos+getLeftSideY(angle,di);
			if (isSelected())
				g->setColor(SAND_BRUSH);
			else
			    g->setColor(RED_BRUSH);


			if (track->showObjectPositionMarker)
		    {
			 bX = x1+nextxpos+getLeftSideX(angle,g->getObjectX(16)+di);
             bY = y1+nextypos+getLeftSideY(angle,g->getObjectX(16)+di);
			}
			else
			{
			 bX = g->getObjectX(-8)+x1+nextxpos+getLeftSideX(angle,di);
             bY = g->getObjectY(-8)+y1+nextypos+getLeftSideY(angle,di);
			}
		
			if (track->showObjectPositionMarker)
		    {
			 g->drawLine(leftSideX,leftSideY,bX,bY);
			}
			
			
		   }
		 
           x = leftSideX;
		   y = leftSideY;
		   if (track->showObjectPositionMarker)
		   {
           draw(g,leftSideX,leftSideY);
		   }

		   if (showBitmaps)
		      g->drawBitmap(obj->getDisplayIconType(),bX,bY);
		   else
			  draw(g,leftSideX,leftSideY);

		   if (isSelected())
			{
				g->setColor(-1);
				g->setColor(0);
				double a = (double)g->getObjectX(4);
				double b = (double)g->getObjectX(18);
				double c = (double)g->getObjectX(20);
				g->drawEllipse(bX-a,bY-a,bX+b,bY+b);
				g->drawBitmap(obj->getDisplayIconType(),bX,bY);
				CString msg;
				msg.Format("%d",obj->getLocator());
				g->drawText(bX+c,bY,msg);
			}
		    else if (track->showAnnotations)
		    {
			  double c = (double)g->getObjectX(20);
			  CString msg;
		      msg.Format("%d",obj->getLocator());
			  g->drawText(bX+c,bY,msg);
		    }


		  
		  g->setColor(-1);
		 }
    }


Observer * TrackObject::Copy()
{
	TrackObject *copy = new TrackObject(this);
	return (Observer*)copy;
}

TrackObject::TrackObject(TrackObject* objIn):
TrackCmd(objIn),
//TrackCmd(objIn->track,objIn->owner,0x80,0,"Copy of TrackObject"),
selected(FALSE)
{
}