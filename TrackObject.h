
#ifndef _TRACK_OBJ_H_
#define _TRACK_OBJ_H_

#include "math.h"
#include "Display.h"
#include "GPTrack.h"
#include "TrackObjectDefinition.h"
#include "TrackCmd.h"

class GPTrack;

//#define WIDTH_TO_TRACKWIDTH_OBJECTS(x) (double)(((double)(x)*2.0)/1400)
//#define WIDTH_TO_TRACKWIDTH_OBJECTS(x) ((x)/1024.0)
#define TRACKWIDTH_OBJECTS_TO_WIDTH(x) ((x)*150.0)
#define WIDTH_TO_TRACKWIDTH_OBJECTS(x) ((x)/150.0)
//#define WIDTH_TO_TRACKWIDTH_OBJECTS(x) (double)(((double)(x)*2.0)/1400)

class TrackObject: public TrackCmd
{
	public:

	TrackSection *trackPiece;
	BOOL    selected;
	TrackObjectDefinition *objectDefintionPtr;
	
	TrackObject(GPTrack *track,TrackSection *owner,int lengthfromstart,int _code,int myoffset):
	TrackCmd(track,owner,0x80,myoffset,"TrackObject"),
	selected(FALSE)
	{ 
		objectDefintionPtr = track->getObjectDefinition(_code); 
    }

	TrackObject(TrackObject *objIn);
	
	Observer * Copy();

	TrackObjectDefinition *getObjectDef()
	{
	   int code = getLocator();
	   TrackObjectDefinition *t = track->getObjectDefinition(code);
	   /*
	   if (objectDefintionPtr != t)
	   {
		   int result = AfxMessageBox("An Object that maps to a removed Object Description\nWould you like me to remove this object?",MB_YESNO);
		   if (result == IDYES)
		   {
			   Delete();
		   }
		   else
		   {
			   result = AfxMessageBox("Shall I point it to the next object description then?",MB_YESNO);
			   if (result == IDYES)
				   objectDefintionPtr = t;
		   }
	   }
	   */
	   return t;
	}

	int getDistance()
	{
		return TrackCmd::getNthArg(1);
	}

	void setDistance(int dist)
	{
		if (dist > 255) dist = 255;
		if (dist <= 0 ) dist = 0;
		TrackCmd::setNthArg(1,dist);
	}

	void setTrackSection(TrackSection *t)
	{
	   trackPiece = t;
	}

	int getLocator()
	{
		return TrackCmd::getNthArg(2);
	}

	int getNthArg(int idx)
	{
		return TrackCmd::getNthArg(idx);
	}

	void setNthArg(int idx,int val)
	{
		AfxMessageBox("Invalid Call to setNthArg on object");
		TrackCmd::setNthArg(idx,val);
	}

	void setLocator(int co)
	{		
		if (co != getLocator())
		{
			int result = AfxMessageBox("You are about to change the object description ok?",MB_YESNO);
			if (result == IDYES){
				TrackCmd::setNthArg(2,co);
				objectDefintionPtr = track->getObjectDefinition(co);
			}
		}
	}

	TrackSection* getTrackSection()
	{
		return trackPiece;
	}

     void draw(Display *g,double x1,double y1,double angle,double trackWidth,BOOL showBitmaps,double stAngle);
	

     void draw(Display *g,double x1,double y1)
    {
        double pix  = (double)g->getObjectX(4);
		double pix1 = (double)g->getObjectX(3);

		g->drawEllipse(x1-pix1,y1-pix1,x1+pix1,y1+pix1);
		g->drawLine(x1,y1-pix,x1,y1+pix);
        g->drawLine(x1-pix,y1,x1+pix,y1);
		
		
    }


	CPoint GetTrackPoint(double currrad,double angle,double length)
	{
		   CPoint pt;
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

	void setSelected(BOOL val)
	{
		selected = val;
	}

	BOOL isSelected()
	{
		return selected;
	}

	double DistanceFromPt(CPoint pt)
	{
		double dx = (pt.x-x);
		double dy = (pt.y-y);
		return sqrt(dx*dx+dy*dy);
	}

	double getX()
	{
		return x;
	}

	double getY()
	{
		return y;
	}

	char *getObjectName()
	{
		   char *trackStrBuffer = new char[256];
	       int dist = getNthArg(1);
		   int val  = getNthArg(2);
		   TrackObjectDefinition *obj = track->getObjectDefinition(val);
		   char *name = "Unknown";
		   if (obj)
		   {
				name =    obj->getName();
		   }
	       wsprintf(trackStrBuffer,"%s (dist=%d)",name,dist);
		   return trackStrBuffer;
	}
				      

	int Open();

	void write(GPTrack *track,int offset);

    double x;
    double y;
};

#endif
