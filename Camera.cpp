

#include "stdafx.h"
#include "Camera.h"
#include "CameraEdit.h"
#include "TrackObject.h"
#include "TrackSection.h"

CPoint GetCameraTrackPoint(double currrad,double angle,double length)
{
		   static CPoint pt;
		   double anglelen     = length*angle;
           double anglerad     = DegToRad(anglelen);
           double sectorRadius = 0;
           double inrad        = DegToRad(anglelen/2.0);
           sectorRadius        = (length/anglerad);
           double chordrad     = DegToRad((anglelen/2.0));
           double len          = 2*(sectorRadius*sin(inrad));

           pt.x =  (long)(len*sin((currrad+inrad)));
           pt.y =  (long)(len*cos((currrad+inrad))); 
		   return pt;
	}




CPosAngle *DistanceToPoint(GPTrack *track,double distance)
{
    TrackSection *TrackSectionIn = track->SectorForLength(distance);

	double distance_to_section_begin   = track->LengthToTrackSector(TrackSectionIn);
	double remainder = distance-distance_to_section_begin;

	double nextypos;
	double nextxpos;
	double angle   = TrackSectionIn->getAngle();
	double stAngle = TrackSectionIn->getStartAngle();
	double x1      = TrackSectionIn->getStartX();
	double y1      = TrackSectionIn->getStartY();
	double sz      = TrackSectionIn->getStartZ();
	TrackSection *nextT = track->GetNextSection(TrackSectionIn);
	double ez      = nextT->getStartZ();		
	double dz = (ez-sz)/TrackSectionIn->getLength();
	double az = sz+(dz*remainder);
	
	if (angle != 0)
	{
	   CPoint trackPt = GetCameraTrackPoint(stAngle,angle,remainder);
       nextypos = trackPt.x;
       nextxpos = trackPt.y;
	   double length = TrackSectionIn->getLength();
	   double deltaAngle = DegToRad(angle*length)*(remainder/length);
	   angle=stAngle+deltaAngle;
	}
	else
	{
	   nextypos = remainder*sin(stAngle);
       nextxpos = remainder*cos(stAngle);
	   angle=stAngle;
	}

	double xr = x1+nextxpos;
	double yr = y1+nextypos;

	return new CPosAngle (xr,yr,angle,x1,y1,az);
}


static void drawPoint(Display *g,double x1,double y1)
{
	double pix  = (double)g->getObjectX(4);
	double pix1 = (double)g->getObjectX(3);

	g->drawEllipse(x1-pix1,y1-pix1,x1+pix1,y1+pix1);
	g->drawLine(x1,y1-pix,x1,y1+pix);
    g->drawLine(x1-pix,y1,x1+pix,y1);
}

void Camera::DrawCamera(Display *g,GPTrack *track)
{
	     char buffer[10];

	     double cameraPos       = getDistance()-16384;
	     double switchMoment    = getSwitchMoment()-16384;
		 

	     CPosAngle *CameraPos    = DistanceToPoint(track,cameraPos);
	     CPosAngle *SwitchMoment = DistanceToPoint(track,switchMoment);
		 Camera *last = (Camera*)track->GetPreviousCamera(this);
		 double lastSwitchMoment = 	last->getSwitchMoment()-16384;
		 CPosAngle *LastSwitchMoment = DistanceToPoint(track,lastSwitchMoment);

		 wsprintf(buffer,"%d",getNumber());
	     double xr = CameraPos->x;
		 double yr = CameraPos->y;
	     //	Draw the position of the camera!
	     double leftSideX;
		 double leftSideY;
		 double bX;
		 double bY;

		 double distance = getPerpendicularDistance();

         g->setColor(0);

		   if (distance > 0)
		   {
			double di = WIDTH_TO_TRACKWIDTH_OBJECTS(fabs(distance));
		    leftSideX = xr+getRightSideX(CameraPos->angle,di+WIDTH_TO_TRACKWIDTH_OBJECTS(0));
            leftSideY = yr+getRightSideY(CameraPos->angle,di+WIDTH_TO_TRACKWIDTH_OBJECTS(0));
			
			bX = xr+getRightSideX(CameraPos->angle,g->getObjectX(16)+di+WIDTH_TO_TRACKWIDTH_OBJECTS(0));
            bY = yr+getRightSideY(CameraPos->angle,g->getObjectX(16)+di+WIDTH_TO_TRACKWIDTH_OBJECTS(0));

			g->drawLine(xr,yr,leftSideX,leftSideY);
			//g->drawLine(leftSideX,leftSideY,bX,bY);
		   }
		   else
		   {
			double di = WIDTH_TO_TRACKWIDTH_OBJECTS(fabs(distance));
			leftSideX = xr+getLeftSideX(CameraPos->angle,di+WIDTH_TO_TRACKWIDTH_OBJECTS(0));
            leftSideY = yr+getLeftSideY(CameraPos->angle,di+WIDTH_TO_TRACKWIDTH_OBJECTS(0));
			
			 bX = xr+getLeftSideX(CameraPos->angle,g->getObjectX(16)+di+WIDTH_TO_TRACKWIDTH_OBJECTS(0));
             bY = yr+getLeftSideY(CameraPos->angle,g->getObjectX(16)+di+WIDTH_TO_TRACKWIDTH_OBJECTS(0));

			//g->drawLine(leftSideX,leftSideY,bX,bY);
			g->drawLine(xr,yr,leftSideX,leftSideY);
			g->setColor(5);
			
		   }

		   drawPoint(g,CameraPos->cx,CameraPos->cy);
		   g->drawLine(CameraPos->x,CameraPos->y,CameraPos->cx,CameraPos->cy);
		   
			
			if (track->showBitmaps)
		      g->drawBitmap(IDB_CAMERA,bX,bY);

			g->drawText(leftSideX,leftSideY,buffer);

			g->setColor(2);
		    g->drawLine(leftSideX,leftSideY,SwitchMoment->x,SwitchMoment->y);
			drawPoint(g,SwitchMoment->cx,SwitchMoment->cy);

			g->setColor(4);
			g->drawLine(leftSideX,leftSideY,LastSwitchMoment->x,LastSwitchMoment->y);
			drawPoint(g,LastSwitchMoment->cx,LastSwitchMoment->cy);

			x = g->getScreenX(bX);
			y = g->getScreenY(bY);

			if (getSelected())
			{
				g->setColor(-1);
				g->setColor(0);
				double a = (double)g->getObjectX(4);
				double b = (double)g->getObjectY(18);
				g->drawEllipse(bX-a,bY-a,bX+b,bY+b);
				g->drawBitmap(IDB_CAMERA,bX,bY);
			}
			//g->setColor(0);
			//g->drawLine(leftSideX,leftSideY,SwitchMoment->x,SwitchMoment->y);

			if (getSelected() && track->showCameraViewDistances)
			{				
				g->setColor(-1);
				g->setColor(0);
				drawPoint(g,leftSideX,leftSideY);
				
				int distBefore = VR1;
				int distAfter  = VR2;
			
				if (distBefore == 0) distBefore = 64;
				if (distAfter == 0) distAfter = 64;

				int a = distBefore;
				int b = distAfter;

				if (a == b)
					a+=1;

				g->setColor(RED_PEN);
			    g->drawEllipse(leftSideX-a,leftSideY-a,leftSideX+a,leftSideY+a);
				g->setColor(BLUE_PEN);
				g->drawEllipse(leftSideX-b,leftSideY-b,leftSideX+b,leftSideY+b);
			}

			// free leaked memory
			if (CameraPos)        delete CameraPos;
		    if (SwitchMoment)     delete SwitchMoment;
		    if (LastSwitchMoment) delete LastSwitchMoment;
}

int Camera::Open()
	  {
		CCameraEdit *dlg = new CCameraEdit();

		dlg->m_Z1   = getZ1();
		dlg->m_Z2   = getZ2();
		dlg->m_Y = getPerpendicularDistance();
		//dlg->m_Change   =  SwitchTime;
		dlg->m_Number   = number;
		dlg->m_X = X-16384;
		dlg->m_SM = SM-16384;
		dlg->m_ZF1 = ZF1;
		dlg->m_ZF2 = ZF2;
		dlg->m_VR1 = VR1;
		dlg->m_VR2 = VR2;
		dlg->m_UK1 = UK1-16384;
		dlg->m_UK2 = UK2;
		dlg->m_UK3 = UK3;
		dlg->m_UK4 = UK4;
		dlg->m_UK5 = UK5;
		dlg->m_UK6 = UK6;
		dlg->m_UK7 = UK7;

		char buffer[10];
		wsprintf(buffer,"%d",track->totalLength());
		dlg->m_TrackLength =CString(buffer);
		
		
		int result=0;

		if (dlg)
		{
		result = dlg->DoModal();
		
		if (result!=IDCANCEL)
		{

		Z1       = dlg->m_Z1;
		Z2       = dlg->m_Z2;
		Y        = dlg->m_Y;
		X         = dlg->m_X+16384;
		SM        = dlg->m_SM+16384;
		ZF1       = dlg->m_ZF1;
		ZF2       = dlg->m_ZF2;
		UK1       = dlg->m_UK1+16384;
		UK2       = dlg->m_UK2;
		VR1       = dlg->m_VR1;
		VR2       = dlg->m_VR2;
		UK3       = dlg->m_UK3;
		UK4       = dlg->m_UK4;
		UK5       = dlg->m_UK5;
		UK6       = dlg->m_UK6;
		UK7       = dlg->m_UK7;


		//write(track,offset);
		track->regenerate=TRUE;
		}
		}

		if (dlg) delete dlg;
		return result;
	  }

int Camera::write(GPTrack *track,int offset)
{
	        track->WritePCWord(offset,X);offset+=2;	 //0
			track->WritePCWord(offset,Y);offset+=2;	 //2
			track->WritePCWord(offset,Z2);offset+=2; //4
			track->WritePCWord(offset,UK1);offset+=2; //6
			track->WritePCWord(offset,UK2);offset+=2; //8
			track->WritePCWord(offset,SM);offset+=2;  //10
			track->WritePCWord(offset,VR1);offset+=2; //12
			track->WritePCWord(offset,VR2);offset+=2; //14
			track->WritePCWord(offset,ZF1);offset+=2; //16
			track->WritePCWord(offset,ZF2);offset+=2; //18
			track->WritePCWord(offset,UK3);offset+=2; //20
			track->WritePCWord(offset,UK4);offset+=2; //22
			track->WritePCWord(offset,UK5);offset+=2; //24
			track->WritePCWord(offset,UK6);offset+=2; //26
			track->WritePCWord(offset,UK7);offset+=2;  //28
			track->WritePCWord(offset,Z1);offset+=2; //30
		  return offset;
}

int Camera::read(GPTrack *track,int offset)
{
	        X      = track->ReadPCWord(offset);offset+=2;
			//int cmd       = track->ReadPCByte(offset);offset+=1;
			if (X == 0xFFFF) 
			{
				return offset;
			}
			Y          = (short)track->ReadPCWord(offset);     offset+=2;
			Z2         = track->ReadPCWord(offset);     offset+=2;
			UK1        = track->ReadPCWord(offset);     offset+=2;
			UK2        = track->ReadPCWord(offset);     offset+=2;
			SM         = track->ReadPCWord(offset);     offset+=2;
			VR1        = track->ReadPCWord(offset);     offset+=2;
			VR2        = track->ReadPCWord(offset);     offset+=2;
			ZF1        = track->ReadPCWord(offset);     offset+=2;
			ZF2        = track->ReadPCWord(offset);     offset+=2;
			UK3        = track->ReadPCWord(offset);     offset+=2;
			UK4        = track->ReadPCWord(offset);     offset+=2;
			UK5        = track->ReadPCWord(offset);     offset+=2;
			UK6        = track->ReadPCWord(offset);     offset+=2;
			UK7        = track->ReadPCWord(offset);     offset+=2;
			Z1         = track->ReadPCWord(offset);     offset+=2;

	return offset;
}

void Camera::Delete()
{
		track->MakeBackupNow();
		int res = AfxMessageBox("Are you sure you want to delete a Camera?",MB_YESNO);
		if (res == IDYES)
		{
		  track->TrackCameras->removeElement(this);		
		}
}

void Camera::InsertNew()
{
		track->MakeBackupNow();
		Camera *t = new Camera(track,0,16384,0,0,0,16384,0,0,0,0,16384,0);
		track->TrackCameras->addElement(t);
}

double Camera::DistanceFromPt(CPoint pt)
{
		double dx = (pt.x-x);
		double dy = (pt.y-y);
		return sqrt(dx*dx+dy*dy);
}

void Camera::Selected()
{
	track->setCameraSelection(this);
}