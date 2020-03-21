#include "stdafx.h"
#include "TrackSection.h"
#include "CmdList.h"
#include "TrackPropertySheet.h"
#include "DataChangeObserver.h"
#include "Texture.h"
#include "TrChangeDlg.h"
#include "TrackTree.h"
#include "TrackEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "GeneralDataEnry.h"

extern LPCSTR getTextureLocation(int id);
extern LPCSTR getTextureDescription(int id);
extern int getTextureIcon(int id);
extern LPCSTR getSceneryLocation(int id);
extern CString getTypeBLocation(int loc);

extern void drawSingleSide(Display *g,TrackSection *t1,int beginOffset,int length,int side);
void drawSpiralPoly(Display *g,GPPolygon &poly,double cx,double cy,double angle1,double angle2,double r1,double r2,bool rev,int pts);

Vector *TrackSection::getCommandIterator()
{
	     int size = TrackCommands->size();
		 Vector *objs = new Vector("getCommandIterator",__LINE__);

		 for(int i=0;i<size;i++)
		 {
			 TrackCmd *t = (TrackCmd*)TrackCommands->elementAt(i);

			 objs->addElementReference((TrackObject*)t);
		 }
		 return objs;
}

Vector *TrackSection::getObjects()
{
		 int size = TrackCommands->size();
		 Vector *objs = new Vector("getObjects",__LINE__);
		 for(int i=0;i<size;i++)
		 {
			 TrackCmd *t = (TrackCmd*)TrackCommands->elementAt(i);

			 if (t->cmd == 0x80)
			 {
				 objs->addElementReference((TrackObject*)t);
			 }
		 }
		 return objs;
	}

void TrackSection::drawStartingLine(Display *g)
{
   double xpos     = getStartX();
   double ypos     = getStartY();
   double nextxpos = getEndX();
   double nextypos = getEndY();
   double sectorStartAngle = getStartAngle();
   double sectorEndAngle   = getEndAngle();

   double leftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft())+2);
   double leftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft())+2);

   double rightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthRight())+2);
   double rightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthRight())+2);

   // track parts
   double leftSideNX = xpos+nextxpos+getLeftSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft())+2);
   double leftSideNY = ypos+nextypos+getLeftSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft())+2);
         
   
   CPen *StartFinishPen;

   StartFinishPen      = new CPen(PS_SOLID,3,RGB(00,00,00));

   g->SelectObject(StartFinishPen);

   g->drawLine(leftSideX,leftSideY,rightSideX,rightSideY);

   g->setColor(0);

   g->drawLine(leftSideX,leftSideY,leftSideNX,leftSideNY);

   g->drawBitmap(IDB_CHECKED_FLAG,leftSideX,leftSideY-16);

   delete StartFinishPen;
}

void TrackSection::drawFlag(Display *g,BOOL right)
{
   double xpos     = getStartX();
   double ypos     = getStartY();
   double nextxpos = getEndX();
   double nextypos = getEndY();
   double sectorStartAngle = getStartAngle();
   double sectorEndAngle   = getEndAngle();

   double leftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()));
   double leftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()));

   double rightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()));
   double rightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()));

   if (right)
   {
    g->drawBitmap(IDB_BLACK_FLAG,rightSideX,rightSideY-16);
   }
   else
   {
	g->drawBitmap(IDB_BLACK_FLAG,leftSideX,leftSideY-16);
   }

}

extern CPoint GetTrackPoint(double currrad,double angle,double length);

CPoint TrackSection::TrackNextPoint(double xpos,double ypos,double angle,double stAngle,int inDistance)
{
		CPoint retPt;
		double nextxpos,nextypos;

         if (angle != 0)
		 {
		  CPoint trackPt = GetTrackPoint(angle,stAngle,inDistance);
          nextypos = trackPt.x;
          nextxpos = trackPt.y;
		 }
		 else
		 {
		  nextypos = inDistance*sin(stAngle);
          nextxpos = inDistance*cos(stAngle);
		 }

		 retPt.x = (int)(xpos+nextxpos);
		 retPt.y = (int)(ypos+nextypos);
		 return retPt;
}


void TrackSection::drawRoadSign(Display *g)
{
   double xpos     = getStartX();
   double ypos     = getStartY();
   double nextxpos = getEndX();
   double nextypos = getEndY();
   double sectorEndAngle   = getEndAngle();

   // track parts
   double leftSideNX = xpos+nextxpos+getLeftSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft())+2);
   double leftSideNY = ypos+nextypos+getLeftSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft())+2);

   TrackSection *prev = track->GetPreviousSection(this);

   int length = prev->getLength();

   CPoint Sign1 = TrackNextPoint(prev->getStartX(),prev->getStartY(),prev->getAngle(),prev->getStartAngle(),length-24);
   CPoint Sign2 = TrackNextPoint(prev->getStartX(),prev->getStartY(),prev->getAngle(),prev->getStartAngle(),length-16);
   CPoint Sign3 = TrackNextPoint(prev->getStartX(),prev->getStartY(),prev->getAngle(),prev->getStartAngle(),length-8);
   
   BOOL left = TRUE;

   if (getAngle() > 0) left =  FALSE;

   if (hasRoadSigns())
   {
	  int sign1 = FirstSign(left);
	  int sign2 = SecondSign(left);
	  int sign3 = ThirdSign(left);

	  if (sign1 != IDB_UNK)
        g->drawBitmap(sign1,Sign1.x,Sign1.y);
	  if (sign2 != IDB_UNK)
        g->drawBitmap(sign2,Sign2.x,Sign2.y);
	  if (sign3 != IDB_UNK)
        g->drawBitmap(sign3,Sign3.x,Sign3.y);
   }
}



void TrackSection::drawCommands(Display *g)
{
   double xpos     = getStartX();
   double ypos     = getStartY();
   double nextxpos = getEndX();
   double nextypos = getEndY();
   double sectorStartAngle   = getStartAngle();

   // track parts
   double leftSideNX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft())+2);
   double leftSideNY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft())+2);

   double inc = g->getObjectX(16);

    if (TrackCommands)
	{
	 for(int i=0;i<TrackCommands->size();i++)
	 {		
		TrackCmd *cmd = (TrackCmd*)TrackCommands->elementAt(i);
		if (cmd->getCmd() != 0x80)
		{
		  g->drawBitmap(cmd->getCmdIcon(),leftSideNX,leftSideNY);
		  leftSideNX+=inc;
		}
	 }
    }
}



Primitive3D *TrackSection::drawTrack3D(Display *g,double angleNotUsed,GPTrack *track,TrackSection *t1,TrackSection *t2)
{
	Model3D   *allTrackParts    = new Model3D();
	Polygon3D *trackSection     = new Polygon3D(TARMAC_BRUSH);
	
	double xpos = getStartX();
	double ypos = getStartY();
	double zpos = getStartZ();
	double nextxpos = getEndX();
	double nextypos = getEndY();
	double nextzpos = t2->getStartZ();
	double sectorStartAngle = getStartAngle();
	double sectorEndAngle   = getEndAngle();

	// track parts
	double leftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()));
	double leftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()));

	double rightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()));
	double rightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()));

           
	// track parts
	double leftSideNX = xpos+nextxpos+getLeftSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()));
	double leftSideNY = ypos+nextypos+getLeftSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()));

           
	double rightSideNX = xpos+nextxpos+getRightSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()));
	double rightSideNY = ypos+nextypos+getRightSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()));

	trackSection->addPoint(leftSideX,leftSideY,zpos);
	trackSection->addPoint(rightSideX,rightSideY,zpos);           
	trackSection->addPoint(rightSideNX,rightSideNY,nextzpos);
	trackSection->addPoint(leftSideNX,leftSideNY,nextzpos);


	int leftWall =  t1->getLeftToBank()*0x40;
	int rightWall =  t1->getRightToBank()*0x40;

	int EleftWall = t2->getLeftToBank()*0x40;
	int ErightWall = t2->getRightToBank()*0x40;
		  
    // track parts
     leftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()+leftWall));
     leftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()+leftWall));

     rightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()+rightWall));
     rightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()+rightWall));

           
    // track parts
     leftSideNX = xpos+nextxpos+getLeftSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()+EleftWall));
     leftSideNY = ypos+nextypos+getLeftSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()+EleftWall));

           
     rightSideNX = xpos+nextxpos+getRightSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()+ErightWall));
     rightSideNY = ypos+nextypos+getRightSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()+ErightWall));

	 

	


	if (track->showTrackSide)
	{
	// track parts
	double TleftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()));
	double TleftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()));
	double TleftSideNX = xpos+nextxpos+getLeftSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()));
	double TleftSideNY = ypos+nextypos+getLeftSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()));

    double TrightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()));
	double TrightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()));
    double TrightSideNX = xpos+nextxpos+getRightSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()));
	double TrightSideNY = ypos+nextypos+getRightSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()));



	// walls parts
    double WleftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()+leftWall));
    double WleftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()+leftWall));
    double WleftSideNX = xpos+nextxpos+getLeftSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()+EleftWall));
    double WleftSideNY = ypos+nextypos+getLeftSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()+EleftWall));


    double WrightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()+rightWall));
    double WrightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()+rightWall));
	double WrightSideNX = xpos+nextxpos+getRightSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()+ErightWall));
    double WrightSideNY = ypos+nextypos+getRightSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()+ErightWall));

	Polygon3D *grassLeft        = new Polygon3D(TextureIdToBrush(track,TXT_GRASS));
	Polygon3D *grassRight       = new Polygon3D(TextureIdToBrush(track,TXT_GRASS));



	grassLeft->addPoint(TleftSideX,TleftSideY,zpos);
	grassLeft->addPoint(WleftSideX,WleftSideY,zpos);
	grassLeft->addPoint(WleftSideNX,WleftSideNY,nextzpos);
	grassLeft->addPoint(TleftSideNX,TleftSideNY,nextzpos);

	grassRight->addPoint(TrightSideX,TrightSideY,zpos);
	grassRight->addPoint(WrightSideX,WrightSideY,zpos);
	grassRight->addPoint(WrightSideNX,WrightSideNY,nextzpos);
	grassRight->addPoint(TrightSideNX,TrightSideNY,nextzpos);
	
	 allTrackParts->addPrimitive(grassLeft);
	 allTrackParts->addPrimitive(grassRight);


	if (TrackCommands)
	{
	 for(int i=0;i<TrackCommands->size();i++)
	 {
		Polygon3D *RoadSideMarkings;
		TrackCmd *cmd = (TrackCmd*)TrackCommands->elementAt(i);

		if (cmd->cmd == 0xbc)
		{
			int side = cmd->getNthArg(2);
			int texture = cmd->getNthArg(4);

			RoadSideMarkings = new Polygon3D(TextureIdToBrush(track,texture));
			

			if (side == LOC_FLOOR_LEFT)
			{
				// left
	            
				RoadSideMarkings->addPoint(TleftSideX,TleftSideY,zpos);
				RoadSideMarkings->addPoint(WleftSideX,WleftSideY,zpos);
				RoadSideMarkings->addPoint(WleftSideNX,WleftSideNY,nextzpos);
				RoadSideMarkings->addPoint(TleftSideNX,TleftSideNY,nextzpos);
			}
			if (side == LOC_FLOOR_RIGHT)
			{
				// right
				RoadSideMarkings->addPoint(TrightSideX,TrightSideY,zpos);           
				RoadSideMarkings->addPoint(WrightSideX,WrightSideY,zpos);           
				RoadSideMarkings->addPoint(WrightSideNX,WrightSideNY,nextzpos);           
				RoadSideMarkings->addPoint(TrightSideNX,TrightSideNY,nextzpos);                    
			}
			allTrackParts->addPrimitive(RoadSideMarkings);
		}
	 }
	}
	}

	if (track->showWalls)
	{
	Polygon3D *trackWallLeft    = new Polygon3D(TARMAC_BRUSH);
	Polygon3D *trackWallRight   = new Polygon3D(TARMAC_BRUSH);
	
	trackWallLeft->addPoint(leftSideX,leftSideY,zpos);
	trackWallLeft->addPoint(leftSideNX,leftSideNY,nextzpos);	
	trackWallLeft->addPoint(leftSideNX,leftSideNY,nextzpos+2);
	trackWallLeft->addPoint(leftSideX,leftSideY,zpos+2);


	trackWallRight->addPoint(rightSideX,rightSideY,zpos);           
	trackWallRight->addPoint(rightSideNX,rightSideNY,nextzpos);	
	trackWallRight->addPoint(rightSideNX,rightSideNY,nextzpos+2);
	trackWallRight->addPoint(rightSideX,rightSideY,zpos+2);           
	
	

	
      allTrackParts->addPrimitive(trackWallLeft);
	  allTrackParts->addPrimitive(trackWallRight);
    }


	allTrackParts->addPrimitive(trackSection);



	return allTrackParts;
}




void TrackSection::drawTrack(Display *g,double angle,GPTrack *track,BOOL drawGray)
{
	double xpos = getStartX();
	double ypos = getStartY();
	double nextxpos = getEndX();
	double nextypos = getEndY();
	double sectorStartAngle = getStartAngle();
	double sectorEndAngle   = getEndAngle();

	TrackSection *prev = track->GetPreviousSection(this);

	double previousLeft = getWidthLeft();
	double previousRight = getWidthRight();

	if (prev!=NULL)
	{
	   previousLeft = prev->getWidthLeft();
	   previousRight = prev->getWidthRight();
	}

	
	// track parts
	double leftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft));
	double leftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft));

	double rightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight));
	double rightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight));

           
	// track parts
	double leftSideNX = xpos+nextxpos+getLeftSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()));
	double leftSideNY = ypos+nextypos+getLeftSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()));

           
	double rightSideNX = xpos+nextxpos+getRightSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()));
	double rightSideNY = ypos+nextypos+getRightSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()));

	trackPolygon->empty();
	trackPolygon->addPoint(g->getScreenX(leftSideX),g->getScreenY(leftSideY));
	trackPolygon->addPoint(g->getScreenX(rightSideX),g->getScreenY(rightSideY));           
	trackPolygon->addPoint(g->getScreenX(rightSideNX),g->getScreenY(rightSideNY));
	trackPolygon->addPoint(g->getScreenX(leftSideNX),g->getScreenY(leftSideNY));

    //g->drawLine(xpos,ypos,(xpos+nextxpos),(ypos+nextypos));

           
   	if (angle == 0)
	{
		if (!drawGray)
		{
		  if (track->showFilledObjects)
			g->setColor(TARMAC_BRUSH);
		  else
			g->setColor(0);
		}
		else
		{
		  g->setColor(GRID_PEN);
		}

		trackPolygon->fillPolygon(g);

		if (track->showKerbs && isLeftKerb())
		{
			if (track->showKerbs && isKerbLower())
			{
				g->setColor(KERB_LOWER_PEN);
			}
			else
			{
		        g->setColor(KERB_PEN);
			}
		}

		else if (selected)
			g->setColor(0);
		else 
		{
		if (track->showFilledObjects)
			g->setColor(TARMAC_BRUSH);
		else
			g->setColor(0);
		}


		

		g->drawLine(leftSideX,leftSideY,leftSideNX,leftSideNY);

		if (track->showKerbs && isRightKerb())
		{
			if (track->showKerbs && isKerbLower())
			{
				g->setColor(KERB_LOWER_PEN);
			}
			else
			{
		        g->setColor(KERB_PEN);
			}
		}
		else if (selected)
			g->setColor(3);
		else 
			g->setColor(0);
		
		g->drawLine(rightSideX,rightSideY,rightSideNX,rightSideNY);

		g->setColor(0);
	}
	else
	{
		g->setColor(5);

		double cx = xpos+getRightSideX(sectorStartAngle,getRadius());
		double cy = ypos+getRightSideY(sectorStartAngle,getRadius());

		if (getStartAngle() > getAngle()*getLength())
		{
			g->setColor(5);
		}

		if (selected)
			g->setColor(3);

		
		if (track->showTrackPie)
		{
		  g->drawPie((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,getStartAngle(),getAngle()*getLength(),getRadius());
		}
		else
		{
		  if (track->showCenterLine)
		  {
            g->drawArc((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,getStartAngle(),getAngle()*getLength(),getRadius()); 
		  }
		}

		if (track->showKerbs && isLeftKerb())
		{
			if (isKerbLower())
			{
				g->setColor(KERB_LOWER_PEN);
			}
			else
			{
		        g->setColor(KERB_PEN);
			}
		}
		else 
			g->setColor(0);

		if (getAngle() < 0)
		{
		  g->drawArc((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,getStartAngle(),getAngle()*getLength(),getRadius()+WIDTH_TO_TRACKWIDTH(getWidthLeft()));
		}
		else
		{
		  g->drawArc((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,getStartAngle(),getAngle()*getLength(),getRadius()+WIDTH_TO_TRACKWIDTH(getWidthRight()));
		}

		if (track->showKerbs && isRightKerb())
		{
			if (isKerbLower())
			{
				g->setColor(KERB_LOWER_PEN);
			}
			else
			{
		        g->setColor(KERB_PEN);
			}
		}
		else 
		{
		    g->setColor(0);
		}

		
		{
		  //if (track->showFilledObjects)
		  {
			g->setColor(TARMAC_BRUSH);
			if (getAngle() < 0){
			   //drawSpiralPoly
			   double cx = xpos+getRightSideX(sectorStartAngle,getRadius());
			   double cy = ypos+getRightSideY(sectorStartAngle,getRadius());
			   double r1 = getRadius()-WIDTH_TO_TRACKWIDTH(previousRight);
			   double r2 = getRadius()-WIDTH_TO_TRACKWIDTH(getWidthRight());
			   double r3 = getRadius()+WIDTH_TO_TRACKWIDTH(previousLeft);
			   double r4 = getRadius()+WIDTH_TO_TRACKWIDTH(getWidthLeft());
			   GPPolygon edge;	
			   edge.empty();
			   //edge.addPoint(g->getScreenX(TleftSideX),g->getScreenY(TleftSideY));
			   //edge.addPoint(g->getScreenX(WleftSideX),g->getScreenY(WleftSideY));
			   drawSpiralPoly(g,edge,cx,cy,getStartAngle(),getEndAngle(),r3,r4,false,-1);
			   //edge.addPoint(g->getScreenX(WleftSideNX),g->getScreenY(WleftSideNY));
			   //edge.addPoint(g->getScreenX(TleftSideNX),g->getScreenY(TleftSideNY));
			   drawSpiralPoly(g,edge,cx,cy,getStartAngle(),getEndAngle(),r1,r2,true,-1);
			   if (track->showFilledObjects){
				 edge.fillPolygon(g);
			   }
			   else {
				 edge.drawPolygon(g);
			   }
		       //g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,getStartAngle(),getAngle()*getLength(),getRadius()+WIDTH_TO_TRACKWIDTH(getWidthRight()),getRadius()-WIDTH_TO_TRACKWIDTH(getWidthLeft()));
		    }
		    else{
			   double cx = xpos+getRightSideX(sectorStartAngle,getRadius());
			   double cy = ypos+getRightSideY(sectorStartAngle,getRadius());
			   double r1 = getRadius()-WIDTH_TO_TRACKWIDTH(previousRight);
			   double r2 = getRadius()-WIDTH_TO_TRACKWIDTH(getWidthRight());
			   double r3 = getRadius()+WIDTH_TO_TRACKWIDTH(previousLeft);
			   double r4 = getRadius()+WIDTH_TO_TRACKWIDTH(getWidthLeft());
			   GPPolygon edge;	
			   edge.empty();
			   //edge.addPoint(g->getScreenX(TleftSideX),g->getScreenY(TleftSideY));
			   //edge.addPoint(g->getScreenX(WleftSideX),g->getScreenY(WleftSideY));
			   drawSpiralPoly(g,edge,cx,cy,getStartAngle(),getEndAngle(),r3,r4,false,-1);
			   //edge.addPoint(g->getScreenX(WleftSideNX),g->getScreenY(WleftSideNY));
			   //edge.addPoint(g->getScreenX(TleftSideNX),g->getScreenY(TleftSideNY));
			   drawSpiralPoly(g,edge,cx,cy,getStartAngle(),getEndAngle(),r1,r2,true,-1);
			   if (track->showFilledObjects){
				 edge.fillPolygon(g);
			   }
			   else {
				 edge.drawPolygon(g);
			   }
		       //g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,getStartAngle(),getAngle()*getLength(),getRadius()-WIDTH_TO_TRACKWIDTH(getWidthLeft()),getRadius()+WIDTH_TO_TRACKWIDTH(getWidthRight()));		  
		    }
		  }
		  /*
		  else
		  {
			g->setColor(0);
			if (getAngle() < 0){
		       g->drawArc((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,getStartAngle(),getAngle()*getLength(),getRadius()-WIDTH_TO_TRACKWIDTH(getWidthLeft()));
		    }
		    else{
		       g->drawArc((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,getStartAngle(),getAngle()*getLength(),getRadius()-WIDTH_TO_TRACKWIDTH(getWidthRight()));
		    }
		  }
		  */

		  /*
		  if (getAngle() < 0){
		   g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,getStartAngle(),getAngle()*getLength(),getRadius()+WIDTH_TO_TRACKWIDTH(getWidthRight()),getRadius()-WIDTH_TO_TRACKWIDTH(getWidthLeft()));
		  }
		  else{
		   g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,getStartAngle(),getAngle()*getLength(),getRadius()-WIDTH_TO_TRACKWIDTH(getWidthLeft()),getRadius()+WIDTH_TO_TRACKWIDTH(getWidthRight()));		  
		  }
		  */
		}
		g->setColor(0);
	}

	if (selected)
	{
		    int size=4;
			int sx = g->getScreenX(xpos);
	        int sy = g->getScreenY(ypos);
	
			g->drawLine(xpos-size,ypos-size,xpos+size,ypos+size);
			g->drawLine(xpos-size,ypos+size,xpos+size,ypos-size);
			sx = g->getScreenX((xpos+nextxpos));
	        sy = g->getScreenY((ypos+nextypos));
			g->fillRect(sx-size,sy-size,2*size,2*size);

			if (track->showViewDistances)
			{
			// draw look points
			g->setColor(-1);
			int ahsize = g->getScreenX(LookAhead/2);
			g->setColor(RED_PEN);
			g->drawEllipse(xpos-ahsize,ypos-ahsize,xpos+ahsize,ypos+ahsize);
			int bhsize = g->getScreenX(LookBehind/2);
			if (ahsize = bhsize) bhsize -=2;
			g->setColor(BLUE_PEN);
			g->drawEllipse(xpos-bhsize,ypos-bhsize,xpos+bhsize,ypos+bhsize);

			if (track->showAnnotations){
			  char buffer[10];
		      double txpos = (xpos+(xpos+nextxpos))/2;
	          double typos = (ypos+(ypos+nextypos))/2;

			  wsprintf(buffer,"Ahead=%d Behind=%d",LookAhead,LookBehind);
		      g->drawText(txpos,typos,buffer);
			 }
			}
	}

	if (track->showTrackNumbers)
	{
		char buffer[10];
		double txpos = (xpos+(xpos+nextxpos))/2;
	    double typos = (ypos+(ypos+nextypos))/2;

		wsprintf(buffer,"%d",track->GetTrackSectionIndex(this));

		g->drawText(txpos,typos,buffer);
	}

	if (track->showTrackDistances)
	{
		char buffer[10];
		//double txpos = (xpos+(xpos+nextxpos))/2;
	    //double typos = (ypos+(ypos+nextypos))/2;
		double txpos = xpos+nextxpos;
		double typos = ypos+nextypos;

		wsprintf(buffer,"%d",(int)track->LengthToTrackSectorEnd(this));

		g->drawText(txpos,typos,buffer);
	}

	if (track->showAnnotations)
	{
		char buffer[256];
		double txpos = (xpos+(xpos+nextxpos))/2;
	    double typos = (ypos+(ypos+nextypos))/2;

		double leftSideX = txpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(4000));
	    double leftSideY = typos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(4000));

		LPSTR text = (LPSTR)getAnnotation();


		if (text && strlen(text) < 250)
		{
		  wsprintf(buffer,"%s",text);
		  g->drawText(leftSideX,leftSideY,buffer);
		  g->drawLine(txpos,typos,leftSideX,leftSideY);
		}

		
	}
    g->drawLine(xpos,ypos,leftSideX,leftSideY);
	g->drawLine(xpos, ypos,rightSideX,rightSideY);

}

static void drawPoint(Display *g,double x1,double y1)
{
	double pix  = (double)g->getObjectX(4);
	double pix1 = (double)g->getObjectX(3);

	g->drawEllipse(x1-pix1,y1-pix1,x1+pix1,y1+pix1);
	g->drawLine(x1,y1-pix,x1,y1+pix);
    g->drawLine(x1-pix,y1,x1+pix,y1);
}


void TrackSection::drawGrip(Display *g,double angle,GPTrack *track)
{
	double xpos = getStartX();
	double ypos = getStartY();
	double nextxpos = getEndX();
	double nextypos = getEndY();
	double sectorStartAngle = getStartAngle();
	double sectorEndAngle   = getEndAngle();

	int maxGrip = 16;

	int scale = (255/maxGrip);

	int grip = getGripLevel();

	if (grip > maxGrip)
	{
		char msg[256];
		wsprintf(msg,"Maximum Grip Level Exceeded %d",grip);
		AfxMessageBox(msg);
		grip = maxGrip;
	}
	
	int colorval = grip*scale;
    
	CPen *gripPen = new CPen(PS_SOLID,5,RGB(colorval,0,0));

	g->getGraphics()->SelectObject(gripPen);
	//g->setColor(GRIP_PEN);

   	if (angle == 0)
	{
	    g->drawLine(xpos,ypos,(xpos+nextxpos),(ypos+nextypos));	
	}
	else
	{
		double cx = xpos+getRightSideX(sectorStartAngle,getRadius());
		double cy = ypos+getRightSideY(sectorStartAngle,getRadius());

        g->drawArc((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,getStartAngle(),getAngle()*getLength(),getRadius()); 
	}

	gripPen->DeleteObject();
	//g->setColor(0);

	delete gripPen;
}

#define Deg2Rad(x) ((double)(x)*PI/180.0)

void TrackSection::CreateRibbons(TrackCmd *scenery,
				   double xpos,double ypos,double zpos,double dist,double angle,
				   double &x1,double &y1,double &z1,
				   double &x2,double &y2,double &z2,
				   double &x3,double &y3,double &z3,
				   double &x4,double &y4,double &z4,
				   double &x5,double &y5,double &z5,
				   double &x6,double &y6,double &z6,double zscale,
				   double LeftAngle,double RightAngle,e_ScenerySide ScenerySide)
{
	        //double rightSide = getWidthRight()+(getRightToBank()*0x40);
			//double leftSide  = getWidthLeft()+(getLeftToBank()*0x40);
	        double rightSide = 0;
			double leftSide  = 0;

			if (LeftAngle > 0x8000) LeftAngle = -(0x10000-LeftAngle);
			if (RightAngle > 0x8000) RightAngle = -(0x10000-RightAngle);


			double LeftAngleDeg  = LeftAngle*(45.0/8192.0);
			double RightAngleDeg = RightAngle*(45.0/8192.0);
			double LeftAdjust  = -Deg2Rad(90-(-LeftAngleDeg));
			double RightAdjust = -Deg2Rad(90-RightAngleDeg);

			if (!track->useSwivelAngles){
				LeftAdjust  = 0.0;
				RightAdjust = 0.0;
			}
			
			
	        ////////////////// Arg3
			short DistToPt = scenery->getNthArg(3);
			z1 =  (scenery->getNthArg(4)/zscale);
			double   DistAbs = (double)abs(DistToPt);
			// multiplier
			DistAbs *= 4;
			x1 = -1;
			if (DistToPt < 0)
			{
			  x1 = xpos+getLeftSideX(angle+LeftAdjust,WIDTH_TO_TRACKWIDTH(leftSide+DistAbs));
	          y1 = ypos+getLeftSideY(angle+LeftAdjust,WIDTH_TO_TRACKWIDTH(leftSide+DistAbs));
			}
			else if (DistToPt > 0)
			{
			  x1 = xpos+getRightSideX(angle+RightAdjust,WIDTH_TO_TRACKWIDTH(rightSide+DistAbs));
	          y1 = ypos+getRightSideY(angle+RightAdjust,WIDTH_TO_TRACKWIDTH(rightSide+DistAbs));
			}

			////////////////// Arg5
			DistToPt = scenery->getNthArg(5);
			z2 =  (scenery->getNthArg(6)/zscale);
			DistAbs = abs(DistToPt);
			DistAbs *= 4;
			x2 = -1;
			if (DistToPt < 0)
			{
			  x2 = xpos+getLeftSideX(angle+LeftAdjust,WIDTH_TO_TRACKWIDTH(leftSide+DistAbs));
	          y2 = ypos+getLeftSideY(angle+LeftAdjust,WIDTH_TO_TRACKWIDTH(leftSide+DistAbs));
			}
			else if (DistToPt > 0)
			{
			  x2 = xpos+getRightSideX(angle+RightAdjust,WIDTH_TO_TRACKWIDTH(rightSide+DistAbs));
	          y2 = ypos+getRightSideY(angle+RightAdjust,WIDTH_TO_TRACKWIDTH(rightSide+DistAbs));
			}

			
			////////////////// Arg7
			DistToPt = scenery->getNthArg(7);
			z3 =  (scenery->getNthArg(8)/zscale);
			DistAbs = abs(DistToPt);
			DistAbs *= 4;
			x3 = -1;
			if (DistToPt < 0)
			{
			  x3 = xpos+getLeftSideX(angle+LeftAdjust,WIDTH_TO_TRACKWIDTH(leftSide+DistAbs));
	          y3 = ypos+getLeftSideY(angle+LeftAdjust,WIDTH_TO_TRACKWIDTH(leftSide+DistAbs));
			}
			else if (DistToPt > 0)
			{
			  x3 = xpos+getRightSideX(angle+RightAdjust,WIDTH_TO_TRACKWIDTH(rightSide+DistAbs));
	          y3 = ypos+getRightSideY(angle+RightAdjust,WIDTH_TO_TRACKWIDTH(rightSide+DistAbs));
			}
	   		
			////////////////// Arg9
			DistToPt = scenery->getNthArg(9);
			z4 =  (scenery->getNthArg(10)/zscale);
			DistAbs = abs(DistToPt);
			DistAbs *= 4;
			x4 = -1;
			if (DistToPt < 0)
			{
			  x4 = xpos+getLeftSideX(angle+LeftAdjust,WIDTH_TO_TRACKWIDTH(leftSide+DistAbs));
	          y4 = ypos+getLeftSideY(angle+LeftAdjust,WIDTH_TO_TRACKWIDTH(leftSide+DistAbs));
			}
			else if (DistToPt > 0)
			{
			  x4 = xpos+getRightSideX(angle+RightAdjust,WIDTH_TO_TRACKWIDTH(rightSide+DistAbs));
	          y4 = ypos+getRightSideY(angle+RightAdjust,WIDTH_TO_TRACKWIDTH(rightSide+DistAbs));
			}

			////////////////// Arg11
			DistToPt = scenery->getNthArg(11);
			z5 =  (scenery->getNthArg(12)/zscale);
			DistAbs = abs(DistToPt);
			DistAbs *= 4;
			x5 = -1;
			if (DistToPt < 0)
			{
			  x5 = xpos+getLeftSideX(angle+LeftAdjust,WIDTH_TO_TRACKWIDTH(leftSide+DistAbs));
	          y5 = ypos+getLeftSideY(angle+LeftAdjust,WIDTH_TO_TRACKWIDTH(leftSide+DistAbs));
			}
			else if (DistToPt > 0)
			{
			  x5 = xpos+getRightSideX(angle+RightAdjust,WIDTH_TO_TRACKWIDTH(rightSide+DistAbs));
	          y5 = ypos+getRightSideY(angle+RightAdjust,WIDTH_TO_TRACKWIDTH(rightSide+DistAbs));
			}

			////////////////// Arg13
			DistToPt = scenery->getNthArg(13);
			z6 =  (scenery->getNthArg(14)/zscale);
			DistAbs = abs(DistToPt);
			DistAbs *= 4;
			x6 = -1;
			if (DistToPt < 0)
			{
			  x6 = xpos+getLeftSideX(angle+LeftAdjust,WIDTH_TO_TRACKWIDTH(leftSide+DistAbs));
	          y6 = ypos+getLeftSideY(angle+LeftAdjust,WIDTH_TO_TRACKWIDTH(leftSide+DistAbs));
			}
			else if (DistToPt > 0)
			{
			  x6 = xpos+getRightSideX(angle+RightAdjust,WIDTH_TO_TRACKWIDTH(rightSide+DistAbs));
	          y6 = ypos+getRightSideY(angle+RightAdjust,WIDTH_TO_TRACKWIDTH(rightSide+DistAbs));
			}
}

static void GetTrackPos(double currrad,double angle,double length,double& xpos,double& ypos,double& endangle)
{
		   CPoint pt;
		   double anglelen     = length*angle;
           double anglerad     = DegToRad(anglelen);
           double sectorRadius = 0;
           double inrad        = DegToRad(anglelen/2.0);
           sectorRadius        = (length/anglerad);
           double chordrad     = DegToRad((anglelen/2.0));
           double len          = 2*(sectorRadius*sin(inrad));

           xpos =  (long)(len*sin((currrad+inrad)));
           ypos =  (long)(len*cos((currrad+inrad))); 
		   endangle = currrad+inrad;
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

int RibbonToColor(int ribbonid)
{
	switch(ribbonid)
	{
	  case 11:return RED_BRUSH;
	  case 12:return SAND_BRUSH;
	  case 13:return CYAN_BRUSH;
	  case 14:return GRASS_BRUSH;
	  case 15:return RED_BRUSH;
	}
	return RED_BRUSH;
}


void DrawRibbon(GPTrack *track,Display *g,double xpos,double ypos,double x1,double y1,double nx1,double ny1,
				double x2,double y2,double nx2,double ny2,int textureId,int ribbonNumber)
{
            if (x1 != -1 && x2 !=-1 && nx1 != -1 && nx2 != -1)
			{
			 GPPolygon *poly1 = new GPPolygon();
			 if (track->showTextures )
				 g->setColor(TextureIdToBrush(track,textureId));			 
			 else
			 {
				 g->setColor(RibbonToColor(ribbonNumber));
			 }

		     poly1->addPoint(g->getScreenX(x1),g->getScreenY(y1));
			 poly1->addPoint(g->getScreenX(nx1),g->getScreenY(ny1));
			 poly1->addPoint(g->getScreenX(nx2),g->getScreenY(ny2));
			 poly1->addPoint(g->getScreenX(x2),g->getScreenY(y2));
			 poly1->addPoint(g->getScreenX(x1),g->getScreenY(y1));
			 poly1->fillPolygon(g);
			 delete poly1;
			}

			g->setColor(0);
			g->setColor(-1);


			if (nx1 != -1) drawPoint(g,nx1,ny1);
			if (nx2 != -1) drawPoint(g,nx2,ny2);
			if (nx1 != -1) g->drawLine(xpos,ypos,nx1,ny1);
			if (nx2 != -1) g->drawLine(xpos,ypos,nx2,ny2);
			if (x1 != -1 && nx1 != -1) g->drawLine(x1,y1,nx1,ny1);
			if (x2 != -1 && nx2 != -1) g->drawLine(x2,y2,nx2,ny2);
			
}

extern int  IntToRibbon(int val);

BOOL TrackSection::IsRibbonOn(int ribbonid)
{
	 int size = getIndex();
	 BOOL ribbon=FALSE;

	 VectorIter *iter =getVector()->firstIter();
	 for(int i=0;i<size;i++)
	 {
		 TrackSection *t=NULL;
		 //t = (TrackSection*)getVector()->elementAt(i);
		 t = (TrackSection*)iter->ptr;
		 

		 Vector *cmds = t->getCommands();
		 VectorIter *iterCmds =cmds->firstIter();
		 for(int k=0;k<cmds->size();k++)
		 {
			 //TrackCmd *cmd = (TrackCmd *)cmds->elementAt(k);
			 TrackCmd *cmd = (TrackCmd *)iterCmds->ptr;
			 int cmdid = cmd->getCmd();
			 switch(cmdid)
			 {
			   case 0xD9:
			   case 0xB9:
			   {
				   int value = cmd->getNthArg(2);
				   int myribbon = IntToRibbon(ribbonid);
				   if (value & myribbon){
					   if (cmdid == 0xD9)
					   {
						 ribbon = !ribbon;
					   }
					   else {
					     ribbon = TRUE;
					   }
				   }
			   } break;
			   case 0xB0:
			   {
				   int value = cmd->getNthArg(2);
				   int myribbon = IntToRibbon(ribbonid);
				   if (value & myribbon)
					   ribbon = FALSE;
			   } break;
			 }
			 iterCmds = cmds->nextIter(iterCmds);
		 }
		 iter = getVector()->nextIter(iter);
	 }
	 return ribbon;
}
				   

void TrackSection::drawScenery(Display *g)
{
	double xpos = getStartX();
    double ypos = getStartY();
	double zpos = getStartY();
    double nextxpos = getEndX();
    double nextypos = getEndY();
	double nextzpos = getEndY();
    double sectorStartAngle = getStartAngle();
    double sectorEndAngle   = getStartAngle();
	double LeftAngle;
	double RightAngle;

	static double x1 = -1;
	static double y1 = 0;
	static double x2 = -1;
	static double y2 = 0;
	static double x3 = -1;
	static double y3 = 0;
    static double x4 = -1;
	static double y4 = 0;
	static double x5 = -1;
	static double y5 = 0;
	static double x6 = -1;
	static double y6 = 0;
	static double z1 = 0;
	static double z2 = 0;
	static double z3 = 0;
	static double z4 = 0;
	static double z5 = 0;
	static double z6 = 0;
	
	//Set default positions for scenery by reading scenery definition
	if (this->getTrack()->GetTrackSectionIndex(this) == 0)
	{
		// work on first section
		double xpos = getStartX();
        double ypos = getStartY();
		double zpos = getStartZ();

		x1 = xpos;
		x2 = xpos;
		x3 = xpos;
		x4 = xpos;
		x5 = xpos;
		x6 = xpos;

		y1 = ypos;
		y2 = ypos;
		y3 = ypos;
		y4 = ypos;
		y5 = ypos;
		y6 = ypos;		

		z1 = zpos;
		z2 = zpos;
		z4 = zpos;
		z5 = zpos;
		z6 = zpos;
		z3 = zpos;
	}
	


	g->setColor(0);


	Vector *cmds = getCommands();

	e_ScenerySide CurrentScenerySide = t_BothScenery;
	LeftAngle = -16384;
	RightAngle = 16384;

	if (cmds!=NULL && cmds->size() > 0)
	{
		int distanceIn = 0;
		for(int i=0;i<cmds->size();i++)
		{
			TrackCmd *scenery = (TrackCmd*)cmds->elementAt(i);
			int cmdCode= scenery->getCmd();

			// get the side info
			if (cmdCode == 0xaf) {
				CurrentScenerySide = t_BothScenery;
				LeftAngle  = scenery->getNthArg(2);
				RightAngle = scenery->getNthArg(3);
			}
			if (cmdCode == 0xc0) {
				CurrentScenerySide = t_LeftScenery;
				LeftAngle  = scenery->getNthArg(2);
				
			}
			if (cmdCode == 0xc1) {
				CurrentScenerySide = t_RightScenery;
				RightAngle = scenery->getNthArg(2);
			}

			if (cmdCode != 0xb8) continue;

			if (cmdCode == 0xb9)
			{
					distanceIn = 0;
				    //distanceIn =  scenery->getNthArg(2);
					//relative movement
					//continue;
			}
			
			double newx = xpos;
			double newy = ypos;
			double newz = zpos;
			double newangle = sectorStartAngle;
			double dx=0;
			double dy=0;
			double nangle = getAngle();

			if (distanceIn != 0)
			{
			  g->setColor(2);

			  if (angle.GetValue() != 0)
			  {
			    CPoint trackPt = GetTrackPoint(sectorStartAngle,nangle,distanceIn);			  
                newx = xpos +trackPt.y;
                newy = ypos +trackPt.x;
	 			double deltaAngle = DegToRad(nangle*getLength())*(distanceIn/getLength());
				newangle = sectorStartAngle+deltaAngle;
			  }
			  else
			  {
				newx = 	xpos+distanceIn*cos(sectorStartAngle);
				newy = 	ypos+distanceIn*sin(sectorStartAngle);
				newangle = sectorStartAngle;
			  }
			  g->drawLine(xpos,ypos,newx,newy);
			  drawPoint(g,newx,newy);
			}
			
         	double nx1,nx2,nx3,nx4,nx5,nx6;
			double ny1,ny2,ny3,ny4,ny5,ny6;
			double nz1,nz2,nz3,nz4,nz5,nz6;
			

			CreateRibbons(scenery,newx,newy,newz,0,newangle,
						  nx1,ny1,nz1,
						  nx2,ny2,nz2,
						  nx3,ny3,nz3,
						  nx4,ny4,nz4,
						  nx5,ny5,nz5,
						  nx6,ny6,nz6,2048,LeftAngle,RightAngle,CurrentScenerySide);


			

			if (track->showFilledObjects)
			{
			  if (IsRibbonOn(1))
			  DrawRibbon(track,g,xpos,ypos,x1,y1,nx1,ny1,x2,y2,nx2,ny2,track->DefaultRibbon11Brush,11);
			  if (IsRibbonOn(5))
			  DrawRibbon(track,g,xpos,ypos,x5,y5,nx5,ny5,x6,y6,nx6,ny6,track->DefaultRibbon15Brush,15);
			  if (IsRibbonOn(4))
			  DrawRibbon(track,g,xpos,ypos,x4,y4,nx4,ny4,x5,y5,nx5,ny5,track->DefaultRibbon14Brush,14);
			  if (IsRibbonOn(3))
			  DrawRibbon(track,g,xpos,ypos,x3,y3,nx3,ny3,x4,y4,nx4,ny4,track->DefaultRibbon13Brush,13);
			  if (IsRibbonOn(2))
			  DrawRibbon(track,g,xpos,ypos,x2,y2,nx2,ny2,x3,y3,nx3,ny3,track->DefaultRibbon12Brush,12);			  			  			  
			}

			g->setColor(0);
			g->setColor(-1);
			g->setColor(0);

			
			if (track->showSceneryArms){
				if (nx1 != -1) drawPoint(g,nx1,ny1);
				if (nx2 != -1) drawPoint(g,nx2,ny2);
				if (nx3 != -1) drawPoint(g,nx3,ny3);
				if (nx4 != -1) drawPoint(g,nx4,ny4);
				if (nx5 != -1) drawPoint(g,nx5,ny5);
				if (nx6 != -1) drawPoint(g,nx6,ny6);
			}

				if (CurrentScenerySide == t_RightScenery)
				{
				g->setColor(RULER_PEN);
				}
				if (track->showSceneryArms){
					if (nx1 != -1) g->drawLine(xpos,ypos,nx1,ny1);
					if (nx2 != -1) g->drawLine(xpos,ypos,nx2,ny2);
					if (nx3 != -1) g->drawLine(xpos,ypos,nx3,ny3);
				}

				g->setColor(0);

				if (CurrentScenerySide == t_LeftScenery)
				{
				g->setColor(RULER_PEN);
				}

				if (track->showSceneryArms){
					if (nx4 != -1) g->drawLine(xpos,ypos,nx4,ny4);
					if (nx5 != -1) g->drawLine(xpos,ypos,nx5,ny5);
					if (nx6 != -1) g->drawLine(xpos,ypos,nx6,ny6);
				}
				

				if (z1 && nz1 == 0) g->setColor(RULER_PEN);
				else g->setColor(0);
				if (x1 != -1 && nx1 != -1) g->drawLine(x1,y1,nx1,ny1);

				if (z2 && nz2 == 0) g->setColor(RULER_PEN);
				else g->setColor(0);
				
				if (x2 != -1 && nx2 != -1) g->drawLine(x2,y2,nx2,ny2);

				if (z3 && nz3 == 0) g->setColor(RULER_PEN);
				else g->setColor(0);
				
				if (x3 != -1 && nx3 != -1) g->drawLine(x3,y3,nx3,ny3);

				if (z4 && nz4 == 0) g->setColor(RULER_PEN);
				else g->setColor(0);
				
				if (x4 != -1 && nx4 != -1) g->drawLine(x4,y4,nx4,ny4);

				if (z5 && nz5 == 0) g->setColor(RULER_PEN);
				else g->setColor(0);
				
				if (x5 != -1 && nx5 != -1) g->drawLine(x5,y5,nx5,ny5);

				if (z6 && nz6 == 0) g->setColor(RULER_PEN);
				else g->setColor(0);
				
				if (x6 != -1 && nx6 != -1) g->drawLine(x6,y6,nx6,ny6);

			x1 = nx1;
			x2 = nx2;
			x3 = nx3;
			x4 = nx4;
			x5 = nx5;
			x6 = nx6;

			y1 = ny1;
			y2 = ny2;
			y3 = ny3;
			y4 = ny4;
			y5 = ny5;
			y6 = ny6;

			z1 = nz1;
			z2 = nz2;
			z3 = nz3;
			z4 = nz4;
			z5 = nz5;
			z6 = nz6;

			g->setColor(0);
		}
	}
}



void drawSpiralPoly(Display *g,GPPolygon &poly,double cx,double cy,double angle1,double angle2,double r1,double r2,bool rev,int pts)
{
	double sA = angle1-(PI/2);
	double eB = angle2-(PI/2);
	double de = eB-sA;
	double eA = sA + 2.0*(de);

	double count = 10.0;
	if (pts != -1){
      count = (double)pts;
	}
	double deltaAngle = (eA - sA);
	double dA = deltaAngle/count;
	double deltaRadius = (r2-r1);
	double dr = deltaRadius/count;
	//double otX,otY;

	if (!rev)
	{
		for(int i=0;i<=count;i++)
		{
			double ndA = (double)i*dA;
			double ndr = (double)i*dr;
			double tX = cx + ((r1+ndr)*cos(sA+ndA));
			double tY = cy + ((r1+ndr)*sin(sA+ndA));
			poly.addPoint(g->getScreenX(tX),g->getScreenY(tY));
		}
	}
	else {
		for(int i=(int)count;i>=0;i--)
		{
			double ndA = (double)i*dA;
			double ndr = (double)i*dr;
			double tX = cx + ((r1+ndr)*cos(sA+ndA));
			double tY = cy + ((r1+ndr)*sin(sA+ndA));
			poly.addPoint(g->getScreenX(tX),g->getScreenY(tY));
		}

	}
	
}

void TrackSection::drawSides(Display *g,double angle, GPTrack *track, TrackSection *t1,TrackSection *t2,TrackSection *prev)
{
	double xpos = t1->getStartX();
    double ypos = t1->getStartY();
    double nextxpos = t1->getEndX();
    double nextypos = t1->getEndY();
    double sectorStartAngle = t1->getStartAngle();
    double sectorEndAngle   = t2->getStartAngle();
	//double sectorEndAngleN   = sectorEndAngle;

	double previousLeft = getWidthLeft();
	double previousRight = getWidthRight();

	if (prev!=NULL)
	{
	   previousLeft = prev->getWidthLeft();
	   previousRight = prev->getWidthRight();
	}

	int leftWall =  prev->getLeftToBank()*0x40;
	int rightWall =  prev->getRightToBank()*0x40;

	int EleftWall = t1->getLeftToBank()*0x40;
	int ErightWall = t1->getRightToBank()*0x40;

	if (angle!=0)
	{
		//EleftWall = leftWall;
	    //ErightWall = rightWall;	
	}



	double 	TleftSideX,TleftSideY,TleftSideNX,TleftSideNY;
	double 	TrightSideX,TrightSideY,TrightSideNX,TrightSideNY;
	double 	WleftSideX,WleftSideY,WleftSideNX,WleftSideNY;
	double 	WrightSideX,WrightSideY,WrightSideNX,WrightSideNY;
	
	//if (angle ==0)
	{
	// track parts
	 TleftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()));
	 TleftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()));
	 TleftSideNX = xpos+nextxpos+getLeftSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()));
	 TleftSideNY = ypos+nextypos+getLeftSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()));
     TrightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()));
	 TrightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()));
     TrightSideNX = xpos+nextxpos+getRightSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()));
	 TrightSideNY = ypos+nextypos+getRightSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()));
	// walls parts
     WleftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()+leftWall));
     WleftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()+leftWall));
     WleftSideNX = xpos+nextxpos+getLeftSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()+EleftWall));
     WleftSideNY = ypos+nextypos+getLeftSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()+EleftWall));
     WrightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()+rightWall));
     WrightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()+rightWall));
	 WrightSideNX = xpos+nextxpos+getRightSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()+ErightWall));
     WrightSideNY = ypos+nextypos+getRightSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()+ErightWall));
	}

    
	//int ptsLeft  = isLeftWallBridged() ? 10 : 2;
	//int ptsRight = isRightWallBridged() ? 10 : 2;
	int ptsLeft = 10;
	int ptsRight = 10;
	

	double cx = xpos+getRightSideX(sectorStartAngle,getRadius());
	double cy = ypos+getRightSideY(sectorStartAngle,getRadius());

	g->setColor(track->DefaultLeftFloorBrush);

	if (getAngle()<0)
	{
	   double cx = xpos+getRightSideX(sectorStartAngle,getRadius());
	   double cy = ypos+getRightSideY(sectorStartAngle,getRadius());
	   double r1 = getRadius()+WIDTH_TO_TRACKWIDTH(previousLeft+leftWall);
	   double r2 = getRadius()+WIDTH_TO_TRACKWIDTH(getWidthLeft()+EleftWall);
	   double r3 = getRadius()+WIDTH_TO_TRACKWIDTH(getWidthLeft());
	   double r4 = getRadius()+WIDTH_TO_TRACKWIDTH(previousLeft);
	   GPPolygon edge;	
	   edge.empty();
	   edge.addPoint(g->getScreenX(TleftSideX),g->getScreenY(TleftSideY));
	   edge.addPoint(g->getScreenX(WleftSideX),g->getScreenY(WleftSideY));
	   drawSpiralPoly(g,edge,cx,cy,getStartAngle(),getEndAngle(),r1,r2,false,ptsLeft);
	   edge.addPoint(g->getScreenX(WleftSideNX),g->getScreenY(WleftSideNY));
	   edge.addPoint(g->getScreenX(TleftSideNX),g->getScreenY(TleftSideNY));
	   drawSpiralPoly(g,edge,cx,cy,getStartAngle(),getEndAngle(),r4,r3,true,-1);
	   if (track->showFilledObjects){
	     edge.fillPolygon(g);
	   }
	   else {
		 edge.drawPolygon(g);
	   }
	   //g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,getStartAngle(),getAngle()*getLength(),getRadius()+WIDTH_TO_TRACKWIDTH(getWidthLeft()+EleftWall),getRadius()+WIDTH_TO_TRACKWIDTH(getWidthLeft()),TRUE);
	}
	else if (getAngle() > 0)
	{
	   double cx = xpos+getRightSideX(sectorStartAngle,getRadius());
	   double cy = ypos+getRightSideY(sectorStartAngle,getRadius());
	   double r1 = getRadius()+WIDTH_TO_TRACKWIDTH(getWidthLeft()+leftWall);
	   double r2 = getRadius()+WIDTH_TO_TRACKWIDTH(getWidthLeft()+EleftWall);
	   double r3 = getRadius()+WIDTH_TO_TRACKWIDTH(getWidthLeft());
	   double r4 = getRadius()+WIDTH_TO_TRACKWIDTH(getWidthLeft());
	   GPPolygon edge;	
	   edge.empty();
	   edge.addPoint(g->getScreenX(TleftSideX),g->getScreenY(TleftSideY));
	   edge.addPoint(g->getScreenX(WleftSideX),g->getScreenY(WleftSideY));
	   drawSpiralPoly(g,edge,cx,cy,getStartAngle(),getEndAngle(),r1,r2,false,ptsLeft);
	   edge.addPoint(g->getScreenX(WleftSideNX),g->getScreenY(WleftSideNY));
	   edge.addPoint(g->getScreenX(TleftSideNX),g->getScreenY(TleftSideNY));
	   drawSpiralPoly(g,edge,cx,cy,getStartAngle(),getEndAngle(),r4,r3,true,-1);
	   if (track->showFilledObjects){
	     edge.fillPolygon(g);
	   }
	   else {
		 edge.drawPolygon(g);
	   }
	   //g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,getStartAngle(),getAngle()*getLength(),getRadius()+WIDTH_TO_TRACKWIDTH(getWidthLeft()),getRadius()+WIDTH_TO_TRACKWIDTH(getWidthLeft()+EleftWall));
	}
	else
	{
	    // left
		GPPolygon *edge = new GPPolygon();	
	    edge->empty();
	    edge->addPoint(g->getScreenX(TleftSideX),g->getScreenY(TleftSideY));
		edge->addPoint(g->getScreenX(WleftSideX),g->getScreenY(WleftSideY));
		edge->addPoint(g->getScreenX(WleftSideNX),g->getScreenY(WleftSideNY));
		edge->addPoint(g->getScreenX(TleftSideNX),g->getScreenY(TleftSideNY));
		edge->addPoint(g->getScreenX(TleftSideX),g->getScreenY(TleftSideY));
		if (track->showFilledObjects){
			edge->fillPolygon(g);
		}
		else {
			edge->drawPolygon(g);
		}
		delete edge;
	}

	g->setColor(track->DefaultRightFloorBrush);
			
	if (getAngle()<0)
	{
	   double cx = xpos+getRightSideX(sectorStartAngle,getRadius());
	   double cy = ypos+getRightSideY(sectorStartAngle,getRadius());
	   double r1 = getRadius()-WIDTH_TO_TRACKWIDTH(getWidthRight()+rightWall);
	   double r2 = getRadius()-WIDTH_TO_TRACKWIDTH(getWidthRight()+ErightWall);
	   double r3 = getRadius()-WIDTH_TO_TRACKWIDTH(getWidthRight());
	   double r4 = getRadius()-WIDTH_TO_TRACKWIDTH(getWidthRight());
	   GPPolygon edge;	
	   edge.empty();
	   edge.addPoint(g->getScreenX(TrightSideX),g->getScreenY(TrightSideY));
	   edge.addPoint(g->getScreenX(WrightSideX),g->getScreenY(WrightSideY));
	   drawSpiralPoly(g,edge,cx,cy,getStartAngle(),getEndAngle(),r1,r2,false,ptsRight);
	   edge.addPoint(g->getScreenX(WrightSideNX),g->getScreenY(WrightSideNY));
	   edge.addPoint(g->getScreenX(TrightSideNX),g->getScreenY(TrightSideNY));
	   drawSpiralPoly(g,edge,cx,cy,getStartAngle(),getEndAngle(),r4,r3,true,-1);
	   if (track->showFilledObjects){
	     edge.fillPolygon(g);
	   }
	   else {
		 edge.drawPolygon(g);
	   }
	  //g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,getStartAngle(),getAngle()*getLength(),getRadius()-WIDTH_TO_TRACKWIDTH(getWidthRight()),getRadius()-WIDTH_TO_TRACKWIDTH(getWidthRight()+ErightWall));
	}
	else if (getAngle() > 0)
	{
	   double cx = xpos+getRightSideX(sectorStartAngle,getRadius());
	   double cy = ypos+getRightSideY(sectorStartAngle,getRadius());
	   double r1 = getRadius()-WIDTH_TO_TRACKWIDTH(getWidthRight()+rightWall);
	   double r2 = getRadius()-WIDTH_TO_TRACKWIDTH(getWidthRight()+ErightWall);
	   double r3 = getRadius()-WIDTH_TO_TRACKWIDTH(getWidthRight());
	   double r4 = getRadius()-WIDTH_TO_TRACKWIDTH(getWidthRight());
	   GPPolygon edge;	
	   edge.empty();
	   edge.addPoint(g->getScreenX(TrightSideX),g->getScreenY(TrightSideY));
	   edge.addPoint(g->getScreenX(WrightSideX),g->getScreenY(WrightSideY));
	   drawSpiralPoly(g,edge,cx,cy,getStartAngle(),getEndAngle(),r1,r2,false,ptsRight);
	   edge.addPoint(g->getScreenX(WrightSideNX),g->getScreenY(WrightSideNY));
	   edge.addPoint(g->getScreenX(TrightSideNX),g->getScreenY(TrightSideNY));
	   drawSpiralPoly(g,edge,cx,cy,getStartAngle(),getEndAngle(),r4,r3,true,-1);
	   if (track->showFilledObjects){
	     edge.fillPolygon(g);
	   }
	   else {
		 edge.drawPolygon(g);
	   }
	   //g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,getStartAngle(),getAngle()*getLength(),getRadius()-WIDTH_TO_TRACKWIDTH(getWidthRight()+ErightWall),getRadius()-WIDTH_TO_TRACKWIDTH(getWidthRight()));
	}
	else
	{
		// right
		GPPolygon *edge = new GPPolygon();	
	    edge->empty();
		edge->addPoint(g->getScreenX(TrightSideX),g->getScreenY(TrightSideY));           
		edge->addPoint(g->getScreenX(WrightSideX),g->getScreenY(WrightSideY));           
		edge->addPoint(g->getScreenX(WrightSideNX),g->getScreenY(WrightSideNY));           
		edge->addPoint(g->getScreenX(TrightSideNX),g->getScreenY(TrightSideNY));           
		edge->addPoint(g->getScreenX(TrightSideX),g->getScreenY(TrightSideY));           
		if (track->showFilledObjects){
			edge->fillPolygon(g);
		}
		else {
			edge->drawPolygon(g);
		}
		delete edge;
    }
}

void TrackSection::drawTextureSides(Display *g, GPTrack *track)
{
	if (TrackCommands)
	{
	int cmdSize = TrackCommands->size();
	for(int i=0;i<cmdSize;i++)
	{		
		TrackCmd *cmd = (TrackCmd*)TrackCommands->elementAt(i);

		if (cmd->cmd == 0xbc)
		{
			int startPos = cmd->getNthArg(1);
			int length   = cmd->getNthArg(3);
			int side    = cmd->getNthArg(2);
			int texture = cmd->getNthArg(4);

			g->setColor(TextureIdToBrush(track,texture));

			drawSingleSide(g,this,startPos,length,side);
		}		
	 }
	}

	g->setColor(-1);
	
}

void TrackSection::GetBeginLeftWall(double &leftSideX,double &leftSideY)
{
  double xpos             = getStartX();
  double ypos             = getStartY();
  double sectorStartAngle = getStartAngle();
  
  TrackSection *prev = track->GetPreviousSection(this);
  int leftWall =  prev->getLeftToBank()*0x40;
  leftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()+leftWall));
  leftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()+leftWall));
}

void TrackSection::GetBeginRightWall(double &leftSideX,double &leftSideY)
{ 
  double xpos             = getStartX();
  double ypos             = getStartY();
  double sectorStartAngle = getStartAngle();
  
  TrackSection *prev = track->GetPreviousSection(this);
  int rightWall =  prev->getRightToBank()*0x40;
  double sectorEndAngle = getStartAngle();
  leftSideX = xpos+getRightSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()+rightWall));
  leftSideY = ypos+getRightSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()+rightWall));
}

void TrackSection::GetEndLeftWall(double &leftSideX,double &leftSideY)
{
  double xpos             = getStartX();
  double ypos             = getStartY();
  double nextxpos         = getEndX();
  double nextypos         = getEndY();
  
  //double sectorEndAngle   = getEndAngle();

  TrackSection *next = track->GetNextSection(this);
  int leftWall =  getLeftToBank()*0x40;
  double sectorStartAngle = next->getStartAngle();
  leftSideX = xpos+nextxpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()+leftWall));
  leftSideY = ypos+nextypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()+leftWall));
}

void TrackSection::GetEndRightWall(double &leftSideX,double &leftSideY)
{ 
  double xpos             = getStartX();
  double ypos             = getStartY();
  double nextxpos         = getEndX();
  double nextypos         = getEndY();
  //double sectorStartAngle = getStartAngle();
  //double sectorEndAngle   = getEndAngle();

  TrackSection *next = track->GetNextSection(this);
  int rightWall =  getRightToBank()*0x40;
  double sectorStartAngle = next->getStartAngle();
  leftSideX = xpos+nextxpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()+rightWall));
  leftSideY = ypos+nextypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()+rightWall));
}



void TrackSection::drawWalls(Display *g,double angle, GPTrack *track, TrackSection *t1,TrackSection *t2)
{
		   TrackSection *nextT = track->GetNextSection(t2);
           double xpos             = t2->getStartX();
           double ypos             = t2->getStartY();
           double nextxpos         = t2->getEndX();
		   double nextypos         = t2->getEndY();
           double sectorStartAngle = t2->getStartAngle();
           double sectorEndAngle   = t2->getEndAngle();
		   double sectorEndAngleN  = nextT->getStartAngle();


		   int leftWall   =  t1->getLeftToBank()*0x40;
	       int rightWall  =  t1->getRightToBank()*0x40;

	       int EleftWall  = t2->getLeftToBank()*0x40;
	       int ErightWall = t2->getRightToBank()*0x40;
		  
           // track parts
           double leftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()+leftWall));
           double leftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft()+leftWall));
		   double rightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()+rightWall));
           double rightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthRight()+rightWall));
 
           // track parts
           double leftSideNX = xpos+nextxpos+getLeftSideX(sectorEndAngleN,WIDTH_TO_TRACKWIDTH(getWidthLeft()+EleftWall));
           double leftSideNY = ypos+nextypos+getLeftSideY(sectorEndAngleN,WIDTH_TO_TRACKWIDTH(getWidthLeft()+EleftWall));           
           double rightSideNX = xpos+nextxpos+getRightSideX(sectorEndAngleN,WIDTH_TO_TRACKWIDTH(getWidthRight()+ErightWall));
           double rightSideNY = ypos+nextypos+getRightSideY(sectorEndAngleN,WIDTH_TO_TRACKWIDTH(getWidthRight()+ErightWall));

		   if (isTrackSection())
		   {
			   double pitX = rightSideX;
			   double pitY = rightSideY;

			   if (track->isPitLaneLeftSide())
			   {
				   pitX = leftSideX;
				   pitY = leftSideY;
			   }

			   if (containsCommand(0xa2))
			   {
				   track->trackWalltoPitLeftWallBeginX = pitX;
				   track->trackWalltoPitLeftWallBeginY = pitY;
			   }
			   else if (containsCommand(0xa1))
			   {
				   track->trackWalltoPitRightWallBeginX = pitX;
				   track->trackWalltoPitRightWallBeginY = pitY;
			   }
			   else if (containsCommand(0xa4))
			   {
				  track->trackWalltoPitLeftWallEndX = pitX;
				  track->trackWalltoPitLeftWallEndY = pitY;
			   }
			   else if (containsCommand(0xa3))
			   {
				   track->trackWalltoPitRightWallEndX = pitX;
				   track->trackWalltoPitRightWallEndY = pitY;	   
			   }			   	           	         	           
		   }
		   else
		   {
			   if (containsCommand(0x9f))
			   {
				   track->pitLeftWallBeginX  = leftSideX;
				   track->pitLeftWallBeginY  = leftSideY;
				   track->pitRightWallBeginX = rightSideX;
				   track->pitRightWallBeginY = rightSideY;	              
			   }
			   else if (containsCommand(0xa0))			   
			   {
				   track->pitLeftWallEndX  = leftSideX;
				   track->pitLeftWallEndY  = leftSideY;
				   track->pitRightWallEndX = rightSideX;
				   track->pitRightWallEndY = rightSideY;	              	       
			   }
			   
		   }
		   
		   g->setColor(TextureIdToBrush(track,TXT_ROAD));
		   

		   double cx = xpos+getRightSideX(sectorStartAngle,getRadius());
		   double cy = ypos+getRightSideY(sectorStartAngle,getRadius());

		   BOOL drawnLeft=FALSE;
		   BOOL drawnRight=FALSE;

		   if (isRightWallBridged())
           {
			double beginX,beginY,endX,endY;
			// right side
			
			//TrackSection *prev = t2->track->GetPreviousSection(t2);
			if (!t1->isRightWallRemoved() && !t1->isRightWallBridged())
			{
			 t1->GetEndRightWall(beginX,beginY);

			 endX=beginX;
			 endY=beginY;

			 TrackSection *next = this;
			 while(next->isRightWallRemoved())
			 {
			  //next->GetBeginRightWall(endX,endY);
			  next = next->track->GetNextSection(next);
			 }
			 next->GetBeginRightWall(endX,endY);
			 g->drawLine(beginX,beginY,endX,endY);
			 drawnRight = TRUE;
			}
		  }

		   if (isLeftWallBridged())
           {
			double beginX,beginY,endX,endY;
			// leftside

			//TrackSection *prev = t1->track->GetPreviousSection(t1);
			
			if (!t1->isLeftWallRemoved() && !t1->isLeftWallBridged())
			{
			 t1->GetEndLeftWall(beginX,beginY);

			 endX=beginX;
			 endY=beginY;

			 TrackSection * next = this;
			 while(next->isLeftWallRemoved())
			 {
			  //next->GetEndLeftWall(endX,endY);
			  next = next->track->GetNextSection(next);
			 }
			 next->GetBeginLeftWall(endX,endY);
			 g->drawLine(beginX,beginY,endX,endY);
			 drawnLeft = TRUE;
			}
		
           }


            if (angle == 0)
            {
			  if (!isLeftWallRemoved() || getTrack()->showRemovedWalls && !drawnLeft){
			    g->drawLine(leftSideX,leftSideY,leftSideNX,leftSideNY);
			  }
			  if (!isRightWallRemoved()|| getTrack()->showRemovedWalls && !drawnRight){
                g->drawLine(rightSideX,rightSideY,rightSideNX,rightSideNY);
			  }
             }
             else{
			  if (!isLeftWallRemoved()|| getTrack()->showRemovedWalls && !drawnLeft){
				if (!track->showCurvedWallStraight)
				{
			      //g->drawArc((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,getStartAngle(),getAngle()*getLength(),t2->getRadius()+WIDTH_TO_TRACKWIDTH(getWidthLeft()+leftWall));
				  double r1 = t2->getRadius()+WIDTH_TO_TRACKWIDTH(getWidthLeft()+leftWall);
				  double r2 = t2->getRadius()+WIDTH_TO_TRACKWIDTH(getWidthLeft()+EleftWall);
				  g->setColor(-1);
				  g->setColor(0);
				  g->drawSpiral(cx,cy,getStartAngle(),getEndAngle(),r1,r2);

				}
				else
				{
				  g->drawLine(leftSideX,leftSideY,leftSideNX,leftSideNY);
				}
			  }
			  if (!isRightWallRemoved()|| getTrack()->showRemovedWalls && !drawnRight){
				  if (!track->showCurvedWallStraight)
				  {
                      //g->drawArc((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,getStartAngle(),getAngle()*getLength(),t2->getRadius()-WIDTH_TO_TRACKWIDTH(getWidthRight()+rightWall));
					  double r1 = t2->getRadius()-WIDTH_TO_TRACKWIDTH(getWidthRight()+rightWall);
				      double r2 = t2->getRadius()-WIDTH_TO_TRACKWIDTH(getWidthRight()+ErightWall);
					  g->setColor(-1);
					  g->setColor(0);
				      g->drawSpiral(cx,cy,getStartAngle(),getEndAngle(),r1,r2);
				  }
				  else
				  {
				   g->drawLine(rightSideX,rightSideY,rightSideNX,rightSideNY);
				  }
				//int rad1 = t2->getRadius()-WIDTH_TO_TRACKWIDTH(getWidthRight()+ErightWall);
				//int rad2 = t2->getRadius()-WIDTH_TO_TRACKWIDTH(getWidthRight()+rightWall);
				//g->drawArc((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,getStartAngle(),getAngle()*getLength(),t2->getRadius()-WIDTH_TO_TRACKWIDTH(getWidthRight()+rightWall));
				//g->setColor(-1);
				//g->drawEllipse(cx-rad1,cy-rad1,cx+rad1,cy+rad1);
				//g->drawEllipse(cx-rad2,cy-rad2,cx+rad2,cy+rad2);
				//g->drawEllipse(cx-rad2,cy-rad1,cx+rad2,cy+rad1);
			  }
             }
		   
		   g->setColor(-1);
    }

BOOL TrackSection::isLeftKerb()
{
	if (getKerbData() & 0x08)
	    return TRUE;
	return FALSE;
}

BOOL TrackSection::isRightKerb()
{
	if (getKerbData() & 0x04)
	    return TRUE;
	return FALSE;
}

void TrackSection::setLeftKerb(BOOL on)
{
	int currentVal = getKerbData();

	if (on && isLeftKerb()) return;

	if (on)
	{
	  setKerbData((currentVal | 0x08));
	}
	else
	{
	  // turn off
	  if (isRightKerb())
	  {
	    setKerbData((currentVal & ~0x08));
	  }
	  // it off anyway
	}
}

void TrackSection::setRightKerb(BOOL on)
{
	int currentVal = getKerbData();

	if (on && isRightKerb()) return;

	if (on)
	{
	  setKerbData((currentVal | 0x04));
	}
	else
	{
	  // turn off
	  if (isRightKerb())
	  {
	    setKerbData((currentVal & ~0x04));
	  }
	  // it off anyway
	}
}


BOOL TrackSection::isLeftWallRemoved()
{
	if (getKerbData() & 0x20)
	    return TRUE;
	return FALSE;
}

BOOL TrackSection::isRightWallRemoved()
{
	if (getKerbData() & 0x10)
	    return TRUE;
	return FALSE;
}

BOOL TrackSection::isRoadSignLeft()
{
	return FALSE;
}

BOOL TrackSection::isRoadSignRight()
{
	return FALSE;
}

BOOL TrackSection::isKerbLower()
{
	if (getRoadSignData() & 0x4) return TRUE;
	return FALSE;
}


BOOL TrackSection::isLeftWallBridged()
{
	if (getRoadSignData() & 0x20) return TRUE;
	return FALSE;
}

void TrackSection::setLeftWallBridged(BOOL val)
{
	int curr = getRoadSignData();

	if (val)
	{
		curr |= 0x20;
	}
	else
	{
		curr &= 0xFFFFFFDF;
	}
	setRoadSignData(curr);
}

void TrackSection::setRightWallBridged(BOOL val)
{
	int curr = getRoadSignData();

	if (val)
	{
		curr |= 0x10;
	}
	else
	{
		curr &= 0xFFFFFFEF;
	}
	setRoadSignData(curr);
}

BOOL TrackSection::isRightWallBridged()
{
	if (getRoadSignData() & 0x10) return TRUE;
	return FALSE;
}


BOOL TrackSection::hasRoadSigns()
{
	if (getRoadSignData() == 0x4) return FALSE;

	if (getRoadSignData() & 0x8 && getRoadSignData() & 0x80)  return FALSE;
	else if (getRoadSignData() & 0x8 && getRoadSignData() & 0x80 && getRoadSignData() & 0x40)  return FALSE;
	else if (getRoadSignData() & 0x8)  return TRUE;
	else if (getRoadSignData() & 0x80) return TRUE;
	else if (getRoadSignData() & 0x40) return TRUE;

	return FALSE;
}





int TrackSection::FirstSign(BOOL turnLeft)
{
   BOOL rs8 = (getRoadSignData() & 0x8);
   BOOL rs40 = (getRoadSignData() & 0x40);
   BOOL rs80 = (getRoadSignData() & 0x80);

   if (rs8 && rs40) return IDB_300_SIGN;
   if (rs8 ) return IDB_300_SIGN;
   if ((rs80 && rs40)) 
   {
	   if (turnLeft)
	     return IDB_TURN_LEFT;
	   else
         return IDB_TURN_RIGHT;
   }
   return IDB_UNK;
}

int TrackSection::SecondSign(BOOL turnLeft)
{
   BOOL rs8 = (getRoadSignData() & 0x8);
   BOOL rs40 = (getRoadSignData() & 0x40);
   BOOL rs80 = (getRoadSignData() & 0x80);

   
   
   if (rs40 && rs80 ) return IDB_200_SIGN;
   if ((rs8 && rs40) || rs80) 
   {
	   if (turnLeft)
	     return IDB_TURN_LEFT;
	   else
         return IDB_TURN_RIGHT;
   }
   if (rs8 ) return IDB_200_SIGN;
   return IDB_UNK;
}

/*
0x8: 300, 200, 100
0x40: corner arrow
0x80: corner arrow, 100
0x8+0x40: 300, corner arrow, 100
0x40+0x80: corner arrow, 200, 100

0x8+0x80: no road signs
0x8+0x40+0x80: no road signs
*/

int TrackSection::ThirdSign(BOOL turnLeft)
{
   BOOL rs8 = (getRoadSignData() & 0x8);
   BOOL rs40 = (getRoadSignData() & 0x40);
   BOOL rs80 = (getRoadSignData() & 0x80);

   if (rs8 && rs40) return IDB_100_SIGN;      
   if (rs8 ) return IDB_100_SIGN;
   if (rs40 && rs80 ) return IDB_100_SIGN;
   if (rs40) 
   {
	   if (turnLeft)
	     return IDB_TURN_LEFT;
	   else
         return IDB_TURN_RIGHT;
   }
   if (rs80 ) return IDB_100_SIGN;
   return IDB_UNK;
}



void TrackSection::setLeftRoadSign(int val)
{
	//int oldval =  getRoadSignData();
	setRoadSignData(val);
}

void TrackSection::setRightRoadSign(int val)
{
	//int oldval =  getRoadSignData();
	TrackSection::setRoadSignData(val);
}




int TrackSection::write(GPTrack *track,int myoffset)
{
			 int angleData  = AngleFrom(getAngle());
			 int lengthData = getLength();
			 int heightData = getHeight();
			 int leftData   = getLeftToBankData();
			 int rightData  = getRightToBankData();
			 int feature1   = getRoadSignData();
			 int feature2   = getKerbData();

			 //offset=-4
			 track->WritePCByte(myoffset,lengthData);myoffset+=1;
			 // cmd
			 track->WritePCByte(myoffset,0x00);myoffset+=1;

			 // angle
			 track->WritePCWord(myoffset,angleData);
			 myoffset+=2;
			 // height
			 track->WritePCWord(myoffset,heightData);
			 myoffset+=2;
			 // kerb and trackside objects
			 track->WritePCByte(myoffset,feature1);
			 myoffset+=1;
			 track->WritePCByte(myoffset,feature2);
			 myoffset+=1;
             // bank data			 
			 track->WritePCByte(myoffset,rightData);
			 myoffset+=1;
			 track->WritePCByte(myoffset,leftData);
			 myoffset+=1;
			 return myoffset;
}

int TrackSection::debug(FILE *fp,GPTrack *track,int offset)
{
			 int angleData  = AngleFrom(getAngle());
			 int lengthData = getLength();
			 int heightData = getHeight();
			 int leftData   = getLeftToBankData();
			 int rightData  = getRightToBankData();
			 int feature1   = getRoadSignData();
			 int feature2   = getKerbData();

			 fprintf(fp,"Track Section 0x%x\n",offset);
			 fprintf(fp,"Length %d\n",lengthData);
			 
			 // angle
			 fprintf(fp,"Angle %d\n",angleData);
			 fprintf(fp,"Height %d\n",heightData);

			 fprintf(fp,"RoadSigns %d\n",feature1);
			 fprintf(fp,"Kerbs %d\n",feature2);

			 fprintf(fp,"Wall Right %d\n",rightData);
			 fprintf(fp,"Wall Left %d\n",leftData);
			 fprintf(fp,"####################################\n");

			 return offset;
}

void TrackSection::RemoveObject(TrackObject *obj)
{
	Vector *objs = getCommands();

	objs->removeElement(obj);
}

void TrackSection::RemoveCommand(TrackCmd *cmd)
{
	Vector *objs = getCommands();

	if (!objs->removeElement(cmd))
	{
		AfxMessageBox("Commannd Not Removed");
	}
}

TrackCmd * TrackSection::getCommand(int cmd)
{
	int size = TrackCommands->size();
	for(int i=0;i<size;i++)
	{
		TrackCmd *cmds = (TrackCmd*)TrackCommands->elementAt(i);
		if (cmd == cmds->cmd) return cmds;
	}
	return NULL;
}

Vector * TrackSection::getCommands(int cmd)
{
	Vector *objs = new Vector("getCommands",__LINE__);

	int cmdSize = TrackCommands->size();
	for(int i=0;i<cmdSize;i++)
	{
		 TrackCmd *cmds = (TrackCmd*)TrackCommands->elementAt(i);
		 if (cmd == cmds->cmd)
		 {
		  objs->addElementReference((TrackObject*)cmds);
		 }
	}
	return objs;
}

void TrackSection::RemoveCommand(int cmdid)
{
	Vector *objs = getCommands();

	TrackCmd *cmd = getCommand(cmdid);

	if (!objs->removeElement(cmd))
	{
		AfxMessageBox("Commannd Not Removed");
	}
}


void TrackSection::InsertCommand(GPTrack *track,int cmd)
{
	TrackCmd *newCmd = new TrackCmd(track,this,cmd,0,"Unknown Command");
	TrackCommands->addElement(newCmd);
}


int TrackSection::getIcon(BOOL pitlane)
{
	int id;

	if (!pitlane)
	{
	  id = IDB_TRACK;

      if (getAngle() > 0)      id = IDB_CURVE_RIGHT;
	  else if (getAngle() < 0) id = IDB_CURVE_LEFT;

	  if (containsCommand(0x86)) id = IDB_TRACKPITBEGIN;
	  if (containsCommand(0x87)) id = IDB_TRACKPITEND;
	}
	else
	{
      id = IDB_PITLANE;

      if (getAngle() > 0)      id = IDB_PIT_RIGHT;
	  else if (getAngle() < 0) id = IDB_PIT_LEFT;
	}

	return id;
}

extern void LoadCmdListBox(CListBox *list);
extern int getCmdNumArgs(int id);

void TrackSection::InsertNew()
{
	track->MakeBackupNow();
	//AfxMessageBox("Add Something to section");
	CCmdList *dlg = new CCmdList();

	
	int result = dlg->DoModal();

	if (result == IDOK)
	{
		int idx    = dlg->SelectedItem;
		if (idx>-1)
		{
			InsertCmdAt(idx,dlg->SelectedItemCmd);
		}
		else
		{
			AfxMessageBox("No New Command Selected");
		}
		track->regenerate = TRUE;

	}

	delete dlg;
}

void TrackSection::InsertCmdAt(int idx,int cmd)
{
		if (idx>-1)
		{
		  int newcmd = cmd;

		  TrackCmd *trackcmd = new TrackCmd(track,this,newcmd,0,"InsertCmdAt");
		  trackcmd->AddWordArgument(0,"Unknown Arg");

		  int num = getCmdNumArgs(newcmd);

          for(int i=0;i<num;i++)
          {
			trackcmd->AddWordArgument(0,"Unknown Arg");
          }

		 
		 int result  = trackcmd->Open();
		 if (result != IDCANCEL)
		 {
			 TrackCommands->addElement(trackcmd);
		 }
		}
}


#define  SINGLE_VAR(_parent,_buffer,_id,_var,_name,_desc){ new SingleVariableObserver(track,_var,_name,_desc));}



void TrackSection::LoadPropertyTable(CTrackPropertySheet *table)
{
	char buffer[256];

	wsprintf(buffer,"%d",getLength());
	DataChangeObserver *lengthobserver = new DataChangeObserver(getTrack(),&length,t_INT,"Length","Length of Section");
	table->InsertTableString(0,0,(LPSTR)"Length",(DWORD)NULL);
	table->InsertTableString(0,1,(LPSTR)buffer,(DWORD)NULL);
	table->InsertTableString(0,2,(LPSTR)"Length of the track section",(DWORD)NULL);
	table->m_TrackProperty.SetItemData(0,(DWORD)lengthobserver);

	sprintf(buffer,"%f",getAngle());
	DataChangeObserver *angleobserver = new DataChangeObserver(getTrack(),&angle,t_INT,"Angle","Angle of Section");
	table->InsertTableString(1,0,(LPSTR)"Angle",NULL);
	table->InsertTableString(1,1,(LPSTR)buffer,NULL);
	table->InsertTableString(1,2,(LPSTR)"Angle of the track section -ve=Left +ve=Right",NULL);
	table->m_TrackProperty.SetItemData(1,(DWORD)angleobserver);

	
	sprintf(buffer,"%d",getHeight());
	DataChangeObserver *heightobserver = new DataChangeObserver(getTrack(),&heightDelta,t_INT,"Height","Height Delta of Section");
	table->InsertTableString(2,0,(LPSTR)"Height Delta",NULL);
	table->InsertTableString(2,1,(LPSTR)buffer,NULL);
	table->InsertTableString(2,2,(LPSTR)"Change in Height over the track section",NULL);
	table->m_TrackProperty.SetItemData(2,(DWORD)heightobserver);

	wsprintf(buffer,"%d",getRightToBank());
	DataChangeObserver *brobserver = new DataChangeObserver(getTrack(),&trackToWallWidthRight,t_INT,"Bank Right","Track To Bank Right");
	table->InsertTableString(3,0,(LPSTR)"Track To Bank Right",NULL);
	table->InsertTableString(3,1,(LPSTR)buffer,NULL);
	table->InsertTableString(3,2,(LPSTR)"Distance Between the edit of the track and the Wall (Right)",NULL);
	table->m_TrackProperty.SetItemData(3,(DWORD)brobserver);


	wsprintf(buffer,"%d",getLeftToBank());
	DataChangeObserver *blobserver = new DataChangeObserver(getTrack(),&trackToWallWidthLeft,t_INT,"Bank Left","Track To Bank Left");
	table->InsertTableString(4,0,(LPSTR)"Track To Bank Left",NULL);
	table->InsertTableString(4,1,(LPSTR)buffer,NULL);
	table->InsertTableString(4,2,(LPSTR)"Distance Between the edit of the track and the Wall (Left)",NULL);
	table->m_TrackProperty.SetItemData(4,(DWORD)blobserver);


}

void TrackSection::AddGravelTrapLeft()
{
	TrackCmd *gravel = new TrackCmd(track,this,0xBC,0,"GRAVEL",0,9,getLength(),track->DefaultGravelBrush,3,16,0,3);  
	addCommand(gravel);
}

void TrackSection::AddGravelTrapRight()
{
	TrackCmd *gravel = new TrackCmd(track,this,0xBC,0,"GRAVEL",0,8,getLength(),track->DefaultGravelBrush,3,16,0,35);  
	addCommand(gravel);
}

void TrackSection::AddTyreWallLeft()
{
	TrackCmd *gravel = new TrackCmd(track,this,0xBC,0,"TYPEWALL",0,5,getLength(),track->DefaultTyreWallBrush,3,16,0,3);  
	addCommand(gravel);
}

void TrackSection::AddTypeWallRight()
{
	TrackCmd *gravel = new TrackCmd(track,this,0xBC,0,"TYPEWALL",0,4,getLength(),track->DefaultTyreWallBrush,3,16,0,35);  
	addCommand(gravel);
}

void TrackSection::AddTarmacLeft()
{
	TrackCmd *gravel = new TrackCmd(track,this,0xBC,0,"TARMAC",0,9,getLength(),track->DefaultTarmacBrush,3,16,0,3);  
	addCommand(gravel);
}

void TrackSection::AddTarmacRight()
{
	TrackCmd *gravel = new TrackCmd(track,this,0xBC,0,"TARMAC",0,8,getLength(),track->DefaultTarmacBrush,3,16,0,35);  
	addCommand(gravel);
}

void TrackSection::AddWallMarkingLeft(int id)
{
    TrackCmd *gravel = new TrackCmd(track,this,0xBC,0,"MARKING",0,9,getLength(),id,3,16,0,3);  
	addCommand(gravel);
}
void TrackSection::AddWallMarkingRight(int id)
{
    TrackCmd *gravel = new TrackCmd(track,this,0xBC,0,"MARKING",0,8,getLength(),id,16,0,35);  
	addCommand(gravel);
}
void TrackSection::AddTrackSideMarkingLeft(int id)
{
    TrackCmd *gravel = new TrackCmd(track,this,0xBC,0,"MARKING",0,5,getLength(),id,3,16,0,3);  
	addCommand(gravel);
}
void TrackSection::AddTrackSideMarkingRight(int id)
{
    TrackCmd *gravel = new TrackCmd(track,this,0xBC,0,"MARKING",0,4,getLength(),id,3,16,0,35);  
	addCommand(gravel);
}
void TrackSection::AddTrackSideMarking(int offset,int location,int length, int texture, int unk1,int unk2, int unk3, int rotation)
{
    TrackCmd *gravel = new TrackCmd(track,this,0xBC,0,"MARKING",offset,location,length,texture,unk1,unk2,unk3,rotation);  
	addCommand(gravel);
}

void TrackSection::AddBlackFlagRight(int speed)
{
	TrackSection *begin = (TrackSection*)track->TrackSections->elementAt(0);
	int lengthToBegin = (int)track->LengthToTrackSector(this);
	int endLength = (int)(lengthToBegin + getTrueLength());

	CGeneralDataEnry  *dataEntry = new CGeneralDataEnry("Black Flag RHS Data Entry","Start Pos",lengthToBegin,"End Pos",endLength,"Speed",150);

	if (dataEntry->ProcessEntry())
	{
	  TrackCmd *flag = new TrackCmd(track,this,0xDF,0,"FLAG",0,dataEntry->m_EditVal1,dataEntry->m_EditVal2,dataEntry->m_EditVal3);
	  begin->addCommand(flag);
	}
}

void TrackSection::AddBlackFlagLeft(int speed)
{
	TrackSection *begin = (TrackSection*)track->TrackSections->elementAt(0);
	int lengthToBegin = (int)track->LengthToTrackSector(this);
	int endLength = (int)(lengthToBegin + getTrueLength());

	CGeneralDataEnry  *dataEntry = new CGeneralDataEnry("Black Flag LHS Data Entry","Start Pos",lengthToBegin,"End Pos",endLength,"Speed",150);

	if (dataEntry->ProcessEntry())
	{
	  TrackCmd *flag = new TrackCmd(track,this,0xDE,0,"FLAG",0,dataEntry->m_EditVal1,dataEntry->m_EditVal2,dataEntry->m_EditVal3);
	  begin->addCommand(flag);
	}
}

void TrackSection::Selected()
{
    if (isTrackSection())
	{
	  track->setTrackSelection(this);
	}
	else if (isPitSection())
	{
	  track->setPitSelection(this);
	}
	track->GetDocument()->UpdateAllViews(NULL);
}

int TrackSection::Open()
{
	CTrChangeDlg *trkdlg = new CTrChangeDlg(NULL);

	int result = trkdlg->EditTrackSections(this);

	delete trkdlg;
	track->GetDocument()->UpdateAllViews(NULL);
	return result;
}

Polygon3D *DrawRibbon3D(GPTrack *track,Display *g,
						double xpos,double ypos,double zpos,
				double x1,double y1,double z1,
				double nx1,double ny1,double nz1,
				double x2,double y2,double z2,
				double nx2,double ny2,double nz2,int textureId)
{
			
            if (x1 != -1 && x2 !=-1 && nx1 != -1 && nx2 != -1)
			{
			 Polygon3D *poly1 = new Polygon3D();
		     poly1->addPoint(x1,y1,z1);
			 poly1->addPoint(nx1,ny1,nz1);
			 poly1->addPoint(nx2,ny2,nz2);
			 poly1->addPoint(x2,y2,z2);
			 poly1->addPoint(x1,y1,z1);
			 return poly1;
			}
			return NULL;
}


Model3D *TrackSection::drawScenery3D(Display *g)
{
	double xpos = getStartX();
    double ypos = getStartY();
	double zpos = getStartY();
    double nextxpos = getEndX();
    double nextypos = getEndY();
	double nextzpos = getEndY();
    double sectorStartAngle = getStartAngle();
    double sectorEndAngle   = getStartAngle();
	

	static double x1 = -1;
	static double y1 = 0;
	static double x2 = -1;
	static double y2 = 0;
	static double x3 = -1;
	static double y3 = 0;
    static double x4 = -1;
	static double y4 = 0;
	static double x5 = -1;
	static double y5 = 0;
	static double x6 = -1;
	static double y6 = 0;
	static double z1 = 0;
	static double z2 = 0;
	static double z3 = 0;
	static double z4 = 0;
	static double z5 = 0;
	static double z6 = 0;

	Model3D *scenerySegments = NULL;
		

	//Set default positions for scenery by reading scenery definition
	if (this->getTrack()->GetTrackSectionIndex(this) == 0)
	{
		// work on first section
		double xpos = getStartX();
        double ypos = getStartY();
		double zpos = getStartZ();

		x1 = xpos;
		x2 = xpos;
		x3 = xpos;
		x4 = xpos;
		x5 = xpos;
		x6 = xpos;

		y1 = ypos;
		y2 = ypos;
		y3 = ypos;
		y4 = ypos;
		y5 = ypos;
		y6 = ypos;
		
		z1 = zpos;
		z2 = zpos;
		z3 = zpos;
		z4 = zpos;
		z5 = zpos;
		z6 = zpos;
	}

	Vector *cmds = getCommands();

	if (cmds!=NULL && cmds->size() > 0)
	{
		int distanceIn = 0;
		for(int i=0;i<cmds->size();i++)
		{
			TrackCmd *scenery = (TrackCmd*)cmds->elementAt(i);
			int cmdCode= scenery->getCmd();
			if (cmdCode != 0xb8 && cmdCode != 0xc0) continue;

			if (cmdCode == 0xC0)
			{
					//distanceIn = 10;
				    distanceIn =  scenery->getNthArg(1);
					//relative movement
					continue;
			}
			if (scenerySegments == NULL)
			{
                 scenerySegments = new Model3D();
			}
			
			double newx = xpos;
			double newy = ypos;
			double newz = zpos;
			double newangle = sectorStartAngle;
			double dx=0;
			double dy=0;

         	//double rightSide = getWidthRight()+(getRightToBank()*0x40);
			//double leftSide  = getWidthLeft()+(getLeftToBank()*0x40);
			

			double nx1,nx2,nx3,nx4,nx5,nx6;
			double ny1,ny2,ny3,ny4,ny5,ny6;
			double nz1,nz2,nz3,nz4,nz5,nz6;
			

			CreateRibbons(scenery,newx,newy,newz,0,newangle,
						  nx1,ny1,nz1,nx2,ny2,nz2,nx3,ny3,nz3,nx4,ny4,nz4,
						  nx5,ny5,nz5,nx6,ny6,nz6,2048,0,0,t_BothScenery);

			Polygon3D *poly1 =DrawRibbon3D(track,g,xpos,ypos,zpos,x1,y1,z1,nx1,ny1,nz1,x2,y2,z2,nx2,ny2,nz2,track->DefaultRibbon11Brush);
			if (poly1) scenerySegments->addPrimitive(poly1);
			Polygon3D *poly2 =DrawRibbon3D(track,g,xpos,ypos,zpos,x5,y5,z5,nx5,ny5,nz5,x6,y6,z6,nx6,ny6,nz6,track->DefaultRibbon15Brush);
			if (poly2) scenerySegments->addPrimitive(poly2);
			Polygon3D *poly3 =DrawRibbon3D(track,g,xpos,ypos,zpos,x4,y4,z4,nx4,ny4,nz4,x5,y5,z5,nx5,ny5,nz5,track->DefaultRibbon14Brush);
			if (poly3) scenerySegments->addPrimitive(poly3);
			Polygon3D *poly4 =DrawRibbon3D(track,g,xpos,ypos,zpos,x3,y3,z3,nx3,ny3,nz3,x4,y4,z4,nx4,ny4,nz4,track->DefaultRibbon13Brush);
			if (poly4) scenerySegments->addPrimitive(poly4);
			Polygon3D *poly5 =DrawRibbon3D(track,g,xpos,ypos,zpos,x2,y2,z2,nx2,ny2,nz2,x3,y3,z3,nx3,ny3,nz3,track->DefaultRibbon12Brush);			  			  			  
			if (poly5) scenerySegments->addPrimitive(poly5);

			x1 = nx1;
			x2 = nx2;
			x3 = nx3;
			x4 = nx4;
			x5 = nx5;
			x6 = nx6;

			y1 = ny1;
			y2 = ny2;
			y3 = ny3;
			y4 = ny4;
			y5 = ny5;
			y6 = ny6;

			z1 = nz1;
			z2 = nz2;
			z3 = nz3;
			z4 = nz4;
			z5 = nz5;
			z6 = nz6;
		}
	}
	return scenerySegments;
}


void TrackSection::setAnnotation(char * ann)
{
		if (ann == NULL)
		{
			annotation = NULL;
		}
		else
		{
		    annotation = strdup(ann);
		}
}

char * TrackSection::getAnnotation()
{
		return annotation;
}

void TrackSection::InsertNew(Observer *obj)
{
	track->MakeBackupNow();
	if (obj->isCommand())
	{
		TrackCmd *t = (TrackCmd*)obj->Copy();
		t->owner = this;
	    TrackCommands->addElement(t);
	    track->regenerate = TRUE;
	}
	else
	{
		char buffer[256];

		if (obj->isTrackSection())
		{
		  wsprintf(buffer,"Not a command cannot paste: Observer is Track Section");
		}
		else if (obj->isPitSection())
		{
		  wsprintf(buffer,"Not a command cannot paste: Observer is Pit Section");
		}
		else if (obj->isObject())
		{
		  wsprintf(buffer,"Not a command cannot paste: Observer is Object");
		}
		else
		{
		  wsprintf(buffer,"Not a command cannot paste: Observer Unknown");
		}
		AfxMessageBox(buffer);
	}
}

BOOL isInBox(CPoint p1,CPoint p2,double x,double y)
{
	if (x >= p1.x && x <= p2.x)
	{
		if (y>= p1.y && y <= p2.y)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL TrackSection::isInside(CPoint begin,CPoint end)
{
	double x1 = getStartX();
	double y1 = getStartY();
	double x2 = x1+getEndX();
	double y2 = y1+getEndY();

	TRACE("B %d %d \n",begin.x,begin.y);
	TRACE("E %d %d \n",end.x,end.y);
	TRACE("E %f %f %f %f \n",x1,y1,x2,y2);

	if (isInBox(begin,end,x1,y1))
	{
		return TRUE;
	}
	if (isInBox(begin,end,x2,y2))
	{
		   return TRUE;
	}
	
	return FALSE;
}

void TrackSection::HighlightSector(Display *g,GPTrack *track,double inDistance,char *text)
{
	double xpos = getStartX();
	double ypos = getStartY();
	double nextxpos = getEndX();
	double nextypos = getEndY();
	double sectorStartAngle = getStartAngle();
	double sectorEndAngle   = getEndAngle();

	CPen *gripPen = new CPen(PS_SOLID,2,RGB(0xFF,0,0));

	g->getGraphics()->SelectObject(gripPen);

	double angle   = getAngle();

	if (angle != 0)
	{
		  CPoint trackPt = GetTrackPoint(sectorStartAngle,angle,inDistance);
          nextypos = trackPt.x;
          nextxpos = trackPt.y;
		  angle = DegToRad(angle*getLength())*(inDistance/getLength());
	}
	else
	{
		  nextypos = inDistance*sin(sectorStartAngle);
          nextxpos = inDistance*cos(sectorStartAngle);
		  angle = sectorStartAngle;
	}
		 
	double xr = xpos+nextxpos;
	double yr = ypos+nextypos;
	
   	drawPoint(g,xr,yr);
	
	g->drawText(xr,yr-5,text);
	

	gripPen->DeleteObject();

	delete gripPen;
}


HTREEITEM TrackSection::LoadTree(CTrackTree *mainTree,HTREEITEM HGP2Track,int index,BOOL trackOrPit)
{
	       CString trackStrBuffer;
           trackStrBuffer.Format("[%d] len=%d ang=%2.2g",index,getLength(),getAngle());
		   
		   int id = getIcon(!trackOrPit);
		   
		   HTREEITEM HGP2TrackSection = mainTree->insertInfoNode(HGP2Track,trackStrBuffer,TO_ID(id));
		   mainTree->getTree()->SetItemData(HGP2TrackSection,(DWORD)this);
		   setTreeNode(HGP2TrackSection);

		   Vector *trackSectionCmds = getCommands();
		   if (trackSectionCmds)
		   {
			   int NumberOfCmds = trackSectionCmds->size();
			   if (NumberOfCmds > 0)
			   {
				 subTree = mainTree->insertInfoNode(HGP2TrackSection,CString("Dummy"),-1);
			   }
		   }
		   return HGP2TrackSection;
}

void TrackSection::Expand(CTrackTree *mainTree,HTREEITEM HGP2TrackSection)
{
	       // if we haven't expanded before then delete the 
	       // I can expand identifier
	       if (subTree)
		   {
	         mainTree->getTree()->DeleteItem(subTree);
			 subTree = NULL;
		   }
		
		   if (mainTree->getTree()->ItemHasChildren(HGP2TrackSection)) return;
		
		   Vector *trackSectionCmds = getCommands();
		   HTREEITEM cmdnode;

		   if (trackSectionCmds)
		   {
			   int NumberOfCmds = trackSectionCmds->size();
			   if (NumberOfCmds > 0)
			   {
				   for(int j=0;j<NumberOfCmds;j++)
				   {
				   CString trackStrBuffer;
				   TrackCmd *cmd = (TrackCmd *)trackSectionCmds->elementAt(j);
				   trackStrBuffer.Format("[0x%x] %s ",cmd->getCmd(),cmd->getCmdDescription());
				   
				    if (cmd->getCmdIcon() == IDB_OBJECT)
				    {
					 //int dist = cmd->getNthArg(1);
					 //int val  = cmd->getNthArg(2);
					 int dist = ((TrackObject*)cmd)->getDistance();
					 int val  = ((TrackObject*)cmd)->getLocator();
					 TrackObjectDefinition *obj = track->getObjectDefinition(val);
					 int id   = IDB_OBJECT;
					 char *name = "Unknown";
					 if (obj)
					 {
					  id   =	obj->getDisplayIconType();
					  name =    obj->getName();
					 }
					 trackStrBuffer.Format("[0x%x] %s (%s) %d 0x%x ",cmd->getCmd(),cmd->getCmdDescription(),name,dist,val);
				      cmdnode = mainTree->insertInfoNode(HGP2TrackSection,trackStrBuffer,TO_ID(id));
					}
					else if (cmd->getCmd() == 0xbc)
					{
					 int textureid = cmd->getNthArg(4);
					 int loc       = cmd->getNthArg(2);
					 trackStrBuffer.Format("[0x%x] Loc=%s Texture=%s(0x%x)",cmd->getCmd(),getTextureLocation(loc),getTextureDescription(textureid),textureid);
					 cmdnode = mainTree->insertInfoNode(HGP2TrackSection,trackStrBuffer,TO_ID(getTextureIcon(textureid)));
					}
					else if (cmd->getCmd() == 0xc8)
					{
					 int textureid = cmd->getNthArg(4);
					 int loc       = cmd->getNthArg(2);
					 trackStrBuffer.Format("Scenery Definition (0x%x) Loc=%s Texture=%s(0x%x)",cmd->getCmd(),getTextureLocation(loc),getTextureDescription(textureid),textureid);
					 cmdnode = mainTree->insertInfoNode(HGP2TrackSection,trackStrBuffer,TO_ID(getTextureIcon(textureid)));
					}
					else if (cmd->getCmd() == 0xc9)
					{
					 int textureid = cmd->getNthArg(4);
					 int loc       = cmd->getNthArg(2);
					 trackStrBuffer.Format("[0x%x] %s - %s",cmd->getCmd(),cmd->getCmdDescription(),getSceneryLocation(loc));
				     cmdnode = mainTree->insertInfoNode(HGP2TrackSection,trackStrBuffer,TO_ID(cmd->getCmdIcon()));
					}
					else if (cmd->getCmd() == 0xb9 || cmd->getCmd() == 0xD9)
					{
					 int loc = cmd->getNthArg(2);
					 trackStrBuffer.Format("[0x%x] Turn Ribbons On (%s)",cmd->getCmd(),getTypeBLocation(loc));
					 cmdnode = mainTree->insertInfoNode(HGP2TrackSection,trackStrBuffer,TO_ID(IDB_SCENERY));
					}
					else if (cmd->getCmd() == 0xB0)
					{
					 int loc = cmd->getNthArg(2);
					 trackStrBuffer.Format("[0x%x] Turn Ribbons Off (%s)",cmd->getCmd(),getTypeBLocation(loc));
					 cmdnode = mainTree->insertInfoNode(HGP2TrackSection,trackStrBuffer,TO_ID(IDB_SCENERY));
					}
					else if (cmd->getCmd() == 0xAD)
					{
					 int bankdir       = cmd->getNthArg(2);
					 int icon = IDB_BANK_LEVEL;

					 if (bankdir < 0)
					 {
						 icon = IDB_BANK_LEFT;
					 }
					 else
					 {
						 icon = IDB_BANK_RIGHT;
					 }
					 cmdnode = mainTree->insertInfoNode(HGP2TrackSection,trackStrBuffer,TO_ID(icon));
					}
					else
					{
					 cmdnode = mainTree->insertInfoNode(HGP2TrackSection,trackStrBuffer,TO_ID(cmd->getCmdIcon()));					 
					}
					mainTree->getTree()->SetItemData(cmdnode,(DWORD)cmd);
					cmd->setTreeNode(cmdnode);
				   }
			   }
		     }
}


BOOL TrackSection::isPitLaneEntrance()
{
	if (getRoadSignData() & 0x1) return TRUE;
	return FALSE;
}

BOOL TrackSection::isPitLaneExit()
{
	if (getRoadSignData() & 0x2) return TRUE;
	return FALSE;
}

void TrackSection::drawTextureCommands(Display *g)
{
   double xpos     = getStartX();
   double ypos     = getStartY();
   double nextxpos = getEndX();
   double nextypos = getEndY();
   double sectorStartAngle   = getStartAngle();

   // track parts
   double leftSideNX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft())+2);
   double leftSideNY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft())+2);

   double inc = g->getObjectX(16);

   if (containsCommand(0x8c))
   {
	Vector *textures = getCommands(0x8c);
    for(int i=0;i<textures->size();i++)
	{		
		TrackCmd *cmd = (TrackCmd*)textures->elementAt(i);
		g->drawBitmap(cmd->getCmdIcon(),leftSideNX,leftSideNY);
		leftSideNX+=inc;
		
	}
   }

   if (containsCommand(0xbc))
   {
	Vector *textures = getCommands(0xbc);
    for(int i=0;i<textures->size();i++)
	{		
		TrackCmd *cmd = (TrackCmd*)textures->elementAt(i);
		g->drawBitmap(cmd->getCmdIcon(),leftSideNX,leftSideNY);
		leftSideNX+=inc;
		
	}
   }

   if (containsCommand(0xbb))
   {
	Vector *textures = getCommands(0xbb);
    for(int i=0;i<textures->size();i++)
	{		
		TrackCmd *cmd = (TrackCmd*)textures->elementAt(i);
		g->drawBitmap(cmd->getCmdIcon(),leftSideNX,leftSideNY);
		leftSideNX+=inc;
		
	}
   }
   
}


void TrackSection::drawViewDistanceCommands(Display *g)
{
   double xpos     = getStartX();
   double ypos     = getStartY();
   double nextxpos = getEndX();
   double nextypos = getEndY();
   double sectorStartAngle   = getStartAngle();

   // track parts
   double leftSideNX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft())+2);
   double leftSideNY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(getWidthLeft())+2);

   double inc = g->getObjectX(16);

   if (containsCommand(0x81))
   {
	Vector *textures = getCommands(0x81);
    for(int i=0;i<textures->size();i++)
	{		
		TrackCmd *cmd = (TrackCmd*)textures->elementAt(i);
		g->drawBitmap(cmd->getCmdIcon(),leftSideNX,leftSideNY);
		leftSideNX+=inc;
		
	}
   }

   if (containsCommand(0x82))
   {
	Vector *textures = getCommands(0x82);
    for(int i=0;i<textures->size();i++)
	{		
		TrackCmd *cmd = (TrackCmd*)textures->elementAt(i);
		g->drawBitmap(cmd->getCmdIcon(),leftSideNX,leftSideNY);
		leftSideNX+=inc;
		
	}
   }

   if (containsCommand(0xBE))
   {
	Vector *textures = getCommands(0xBE);
    for(int i=0;i<textures->size();i++)
	{		
		TrackCmd *cmd = (TrackCmd*)textures->elementAt(i);
		g->drawBitmap(cmd->getCmdIcon(),leftSideNX,leftSideNY);
		leftSideNX+=inc;
		
	}
   }
   
}






	



	
	

