
#include "stdafx.h"

#include "GPTrack.h"
#include "TrackSection.h"
#include "Texture.h"

void drawSingleSectionSide(Display *g,TrackSection *t1,int beginOffset,int length,int side);

extern CPoint GetTrackPoint(double currrad,double angle,double length);

extern void drawSpiralPoly(Display *g,GPPolygon &poly,double cx,double cy,double angle1,double angle2,double r1,double r2,bool rev,int pts);

double GetTrackMoveOnPoint(double &nextxpos,double &nextypos,double stAngle,double angle,double length,double remainder)
{
	     double nangle;

         if (angle != 0)
		 {
		  CPoint trackPt = GetTrackPoint(stAngle,angle,remainder);
          nextypos = trackPt.x;
          nextxpos = trackPt.y;
	      double deltaAngle = DegToRad(angle*length)*(remainder/length);
	      nangle=stAngle+deltaAngle;
		 }
		 else
		 {
		  nextypos = remainder*sin(stAngle);
          nextxpos = remainder*cos(stAngle);
		  nangle = stAngle;
		 }
		 return nangle;
}

void drawSingleSide(Display *g,TrackSection *t1,int beginOffset,int length,int side)
{
	int sectorLength = t1->getLength();

	if (beginOffset==0 && sectorLength==length)
	{
		drawSingleSectionSide(g,t1,beginOffset,length,side);		
	}
	else
	{
		if (beginOffset+length < sectorLength)
		{
		  // not quite as complicated but simply adjust the length
		  // and the offset
		  if (beginOffset == 0)
		  {
			  // only the length is different
			  //TRACE("(1)Texture map does not run a whole section from=%d len=%d -> %d\n",beginOffset,length,sectorLength);
			  drawSingleSectionSide(g,t1,beginOffset,length,side);
		  }
		  else
		  {
			  // the offset and the length need altering
			  //TRACE("(2)Texture map does not run a whole section from=%d len=%d -> %d\n",beginOffset,length,sectorLength);
			  drawSingleSectionSide(g,t1,beginOffset,length,side);  
		  }
		  
		}
		else
		{
		  // texture spans multiple sections
		  // loop from beginnig to end
		  //TRACE("(3)Texture map does not fit whole section from=%d len=%d -> %d\n",beginOffset,length,sectorLength);
		  int textureLength = length;
		  int textureBegin  = beginOffset;
		  //drawSingleSectionSide(g,t1,beginOffset,sectorLength-beginOffset,side);
		  while(textureLength > 0)
		  {
			int secLen = t1->getLength()-beginOffset;

			if (textureLength > secLen)
			{			
		      drawSingleSectionSide(g,t1,beginOffset,secLen,side);
			}
			else
			{
			  drawSingleSectionSide(g,t1,beginOffset,textureLength,side);
			}
			// next section
			t1 = t1->next();
			beginOffset = 0;
			textureLength -= secLen;

		  }
		}
	}
	
}

#define Rad2Deg(x) ((double)(x)*180.0/PI)
#define Deg2Rad(x) ((double)(x)*PI/180.0)

#define FENCE 0.5


void drawSingleSectionSide(Display *g,TrackSection *t1,int beginOffset,int length,int side)
{
	TrackSection *t2   = t1->next();
	TrackSection *prev = t1->previous();

	double xpos     = t1->getStartX();
    double ypos     = t1->getStartY();
    double nextxpos = t1->getEndX();
    double nextypos = t1->getEndY();
    double sectorStartAngle = t1->getStartAngle();
    double sectorEndAngle   = t2->getStartAngle();
	//double sectorEndAngleN  = sectorEndAngle;
	double Radius  = t1->getRadius();
	double Length  = t1->getLength();
	double cx = xpos+getRightSideX(sectorStartAngle,Radius);
	double cy = ypos+getRightSideY(sectorStartAngle,Radius);

	//double cx = xpos+getRightSideX(sectorStartAngle,t1->getRadius());
	//double cy = ypos+getRightSideY(sectorStartAngle,t1->getRadius());

	double previousLeft = t1->getWidthLeft();
	double previousRight = t1->getWidthRight();

	if (prev!=NULL)
	{
	  previousLeft = prev->getWidthLeft();
	  previousRight = prev->getWidthRight();
	}

	double LeftWidth  = t1->getWidthLeft();
	double RightWidth = t1->getWidthRight();

	double leftWall   =  prev->getLeftToBank()*0x40;
	double rightWall  =  prev->getRightToBank()*0x40;
	double EleftWall  =  t1->getLeftToBank()*0x40;
	double ErightWall =  t1->getRightToBank()*0x40;

	double WidthLeftDelta  = (LeftWidth-previousLeft)/Length;
	double WidthRightDelta = (RightWidth-previousRight)/Length;
	double WallLeftDelta   = (EleftWall-leftWall)/Length;
	double WallRightDelta  = (ErightWall-rightWall)/Length;

	
	double BeginLeftWidth  = previousLeft;
	double BeginRightWidth = previousRight;
	double EndRightWidth   = RightWidth;
	double EndLeftWidth    = LeftWidth;
	double BeginLeftWall   = leftWall;
	double EndLeftWall	   = EleftWall;
	double BeginRightWall  = rightWall;
	double EndRightWall    = ErightWall;
	

	if ((beginOffset+length) < Length){
	 EndLeftWidth  = BeginLeftWidth  + ((beginOffset+length)*WidthLeftDelta);
	 EndRightWidth = BeginRightWidth + ((beginOffset+length)*WidthRightDelta);
	 EndLeftWall   = BeginLeftWall   + (((double)(beginOffset+length))*WallLeftDelta);
	 EndRightWall  = BeginRightWall  + (((double)(beginOffset+length))*WallRightDelta);
	}

	if (beginOffset!=0)
	{
	 double x1 = xpos;
	 double y1 = ypos;
	 double nangle = GetTrackMoveOnPoint(x1,y1,t1->getStartAngle(),t1->getAngle(),Length,beginOffset+1);
	 sectorStartAngle = nangle;
	 xpos += x1;
	 ypos += y1;
	 BeginLeftWidth  = previousLeft + (beginOffset*WidthLeftDelta);
	 BeginRightWidth = previousRight + (beginOffset*WidthRightDelta);
	 BeginLeftWall   = leftWall  + ((double)beginOffset*WallLeftDelta);
	 BeginRightWall  = rightWall + ((double)beginOffset*WallRightDelta);
	}

	if (Length!=(beginOffset+length))
	{
	 double x1 = xpos;
	 double y1 = ypos;
	 double nangle = GetTrackMoveOnPoint(x1,y1,t1->getStartAngle(),t1->getAngle(),Length,length);
	 sectorEndAngle = nangle;
	 nextxpos = x1;
	 nextypos = y1;
	}
	
	double TotalAngle = t1->getAngle()*t1->getLength();
	double angle = TotalAngle;
	
	double 	TleftSideX,TleftSideY,TleftSideNX,TleftSideNY;
	double 	TrightSideX,TrightSideY,TrightSideNX,TrightSideNY;
	double 	WleftSideX,WleftSideY,WleftSideNX,WleftSideNY;
	double 	WrightSideX,WrightSideY,WrightSideNX,WrightSideNY;
	
	
	if (angle == 0)
	{
	// track parts
	 TleftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(LeftWidth));
	 TleftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(LeftWidth));
	 TleftSideNX = xpos+nextxpos+getLeftSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(LeftWidth));
	 TleftSideNY = ypos+nextypos+getLeftSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(LeftWidth));
     TrightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(RightWidth));
	 TrightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(RightWidth));
     TrightSideNX = xpos+nextxpos+getRightSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(RightWidth));
	 TrightSideNY = ypos+nextypos+getRightSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(RightWidth));

	 // walls parts
     WleftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(LeftWidth+BeginLeftWall));
     WleftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(LeftWidth+BeginLeftWall));
     WleftSideNX = xpos+nextxpos+getLeftSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(LeftWidth+EndLeftWall));
     WleftSideNY = ypos+nextypos+getLeftSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(LeftWidth+EndLeftWall));
     WrightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(RightWidth+BeginRightWall));
     WrightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(RightWidth+BeginRightWall));
	 WrightSideNX = xpos+nextxpos+getRightSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(RightWidth+EndRightWall));
     WrightSideNY = ypos+nextypos+getRightSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(RightWidth+EndRightWall));
	}

	if (side == LOC_FLOOR_LEFT)
	{														
				if (angle<0 || angle > 0)
				{
				   
				   double r1 = t1->getRadius()+WIDTH_TO_TRACKWIDTH(BeginLeftWidth+BeginLeftWall);
				   double r2 = t1->getRadius()+WIDTH_TO_TRACKWIDTH(EndLeftWidth+EndLeftWall);
				   double r3 = t1->getRadius()+WIDTH_TO_TRACKWIDTH(EndLeftWidth);
				   double r4 = t1->getRadius()+WIDTH_TO_TRACKWIDTH(BeginLeftWidth);
				   GPPolygon edge;	
				   edge.empty();
				   drawSpiralPoly(g,edge,cx,cy,sectorStartAngle,sectorStartAngle+((sectorEndAngle-sectorStartAngle)/2.0),r1,r2,false,-1);
				   drawSpiralPoly(g,edge,cx,cy,sectorStartAngle,sectorStartAngle+((sectorEndAngle-sectorStartAngle)/2.0),r4,r3,true,-1);
				   if (t1->track->showFilledObjects){
					 edge.fillPolygon(g);
				   }
				   else {
					 edge.drawPolygon(g);
				   }
				   //g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,sectorStartAngle,TotalAngle,Radius+WIDTH_TO_TRACKWIDTH(LeftWidth+EleftWall),Radius+WIDTH_TO_TRACKWIDTH(LeftWidth));
				}
				//else if (angle > 0)
				//{
				 
				 //g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,sectorStartAngle,TotalAngle,Radius+WIDTH_TO_TRACKWIDTH(LeftWidth),Radius+WIDTH_TO_TRACKWIDTH(LeftWidth+EleftWall));
				//}
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
				  if (t1->track->showFilledObjects){
					 edge->fillPolygon(g);
				   }
				   else {
					 edge->drawPolygon(g);
				   }
				  delete edge;
				}	   
	}
	else if (side == LOC_FLOOR_RIGHT)
			{
				//double cx = xpos+getRightSideX(sectorStartAngle,Radius);
	            //double cy = ypos+getRightSideY(sectorStartAngle,Radius);
				
				if (angle<0 || angle > 0)
				{
				   //double cx = xpos+getRightSideX(sectorStartAngle,t1->getRadius());
				   //double cy = ypos+getRightSideY(sectorStartAngle,t1->getRadius());
				   double r1 = t1->getRadius()-WIDTH_TO_TRACKWIDTH(BeginRightWidth+BeginRightWall);
				   double r2 = t1->getRadius()-WIDTH_TO_TRACKWIDTH(EndRightWidth+EndRightWall);
				   double r3 = t1->getRadius()-WIDTH_TO_TRACKWIDTH(EndRightWidth);
				   double r4 = t1->getRadius()-WIDTH_TO_TRACKWIDTH(BeginRightWidth);
				   GPPolygon edge;	
				   edge.empty();
				   drawSpiralPoly(g,edge,cx,cy,sectorStartAngle,sectorStartAngle+((sectorEndAngle-sectorStartAngle)/2.0),r1,r2,false,-1);
				   drawSpiralPoly(g,edge,cx,cy,sectorStartAngle,sectorStartAngle+((sectorEndAngle-sectorStartAngle)/2.0),r4,r3,true,-1);
				   if (t1->track->showFilledObjects){
					 edge.fillPolygon(g);
				   }
				   else {
					 edge.drawPolygon(g);
				   }
				 //GPPolygon
				 //drawSpiralPoly
				 //g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,sectorStartAngle,TotalAngle,Radius-WIDTH_TO_TRACKWIDTH(RightWidth),Radius-WIDTH_TO_TRACKWIDTH(RightWidth+ErightWall));
				}
				//else if (angle > 0)
				//{
				 //g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,sectorStartAngle,TotalAngle,Radius-WIDTH_TO_TRACKWIDTH(RightWidth+ErightWall),Radius-WIDTH_TO_TRACKWIDTH(RightWidth));
				//}
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
				if (t1->track->showFilledObjects){
					 edge->fillPolygon(g);
				   }
				   else {
					 edge->drawPolygon(g);
				   }
				delete edge;
				}
				
	}
	else if (side == LOC_ROAD)
	{
				//double cx = xpos+getRightSideX(sectorStartAngle,Radius);
	            //double cy = ypos+getRightSideY(sectorStartAngle,Radius);
				
				if (angle<0 || angle > 0)
				{
					//double cx = xpos+getRightSideX(sectorStartAngle,t1->getRadius());
				   //double cy = ypos+getRightSideY(sectorStartAngle,t1->getRadius());
				   double r1 = t1->getRadius()+WIDTH_TO_TRACKWIDTH(BeginLeftWidth);
				   double r2 = t1->getRadius()+WIDTH_TO_TRACKWIDTH(EndLeftWidth);
				   double r3 = t1->getRadius()-WIDTH_TO_TRACKWIDTH(EndRightWidth);
				   double r4 = t1->getRadius()-WIDTH_TO_TRACKWIDTH(BeginRightWidth);
				   GPPolygon edge;	
				   edge.empty();
				   drawSpiralPoly(g,edge,cx,cy,sectorStartAngle,sectorStartAngle+((sectorEndAngle-sectorStartAngle)/2.0),r1,r2,false,-1);
				   drawSpiralPoly(g,edge,cx,cy,sectorStartAngle,sectorStartAngle+((sectorEndAngle-sectorStartAngle)/2.0),r4,r3,true,-1);
				   if (t1->track->showFilledObjects){
					 edge.fillPolygon(g);
				   }
				   else {
					 edge.drawPolygon(g);
				   }
				 //g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,sectorStartAngle,TotalAngle,Radius+WIDTH_TO_TRACKWIDTH(RightWidth),Radius-WIDTH_TO_TRACKWIDTH(LeftWidth));
				}
				//else if (angle > 0)
				//{
				 //g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,sectorStartAngle,TotalAngle,Radius-WIDTH_TO_TRACKWIDTH(LeftWidth),Radius+WIDTH_TO_TRACKWIDTH(RightWidth));
				//}
				else
				{
				 // road
				 GPPolygon *edge = new GPPolygon();	
	             edge->empty();				  
				 edge->addPoint(g->getScreenX(TrightSideX),g->getScreenY(TrightSideY));           
				 edge->addPoint(g->getScreenX(TrightSideNX),g->getScreenY(TrightSideNY));           
				 edge->addPoint(g->getScreenX(TleftSideNX),g->getScreenY(TleftSideNY));           
				 edge->addPoint(g->getScreenX(TleftSideX),g->getScreenY(TleftSideY));           
				 edge->addPoint(g->getScreenX(TrightSideX),g->getScreenY(TrightSideY));           
				 if (t1->track->showFilledObjects){
					 edge->fillPolygon(g);
				   }
				   else {
					 edge->drawPolygon(g);
				   }
				 delete edge;
				}
	}
	else if (side == LOC_WALL_LEFT)
	{	
		g->setColor(RED_BRUSH);
		if (angle<0 || angle > 0)
		{
           //double cx = xpos+getRightSideX(sectorStartAngle,t1->getRadius());
		   //double cy = ypos+getRightSideY(sectorStartAngle,t1->getRadius());
		   double r1 = t1->getRadius()+WIDTH_TO_TRACKWIDTH(BeginLeftWidth+BeginLeftWall)+0.5;
		   double r2 = t1->getRadius()+WIDTH_TO_TRACKWIDTH(EndLeftWidth+EndLeftWall)+0.5;
		   double r3 = t1->getRadius()+WIDTH_TO_TRACKWIDTH(EndLeftWidth+EndLeftWall);
		   double r4 = t1->getRadius()+WIDTH_TO_TRACKWIDTH(BeginLeftWidth+BeginLeftWall);
		   GPPolygon edge;	
		   edge.empty();
		   drawSpiralPoly(g,edge,cx,cy,sectorStartAngle,sectorStartAngle+((sectorEndAngle-sectorStartAngle)/2.0),r1,r2,false,-1);
		   drawSpiralPoly(g,edge,cx,cy,sectorStartAngle,sectorStartAngle+((sectorEndAngle-sectorStartAngle)/2.0),r4,r3,true,-1);
		   if (t1->track->showFilledObjects){
			 edge.fillPolygon(g);
		   }
		   else {
			 edge.drawPolygon(g);
		   }
		  //g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,sectorStartAngle,TotalAngle,Radius+WIDTH_TO_TRACKWIDTH(LeftWidth+EleftWall)+1,Radius+WIDTH_TO_TRACKWIDTH(LeftWidth+EleftWall));
		}
		//else if (angle > 0)
		//{
		  //g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,sectorStartAngle,TotalAngle,Radius+WIDTH_TO_TRACKWIDTH(LeftWidth+EleftWall),Radius+WIDTH_TO_TRACKWIDTH(LeftWidth+EleftWall)+1);
		//}
		else
		{
		  // walls parts
          double OWleftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(LeftWidth+BeginLeftWall)+FENCE);
          double OWleftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(LeftWidth+BeginLeftWall)+FENCE);
          double OWleftSideNX = xpos+nextxpos+getLeftSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(LeftWidth+EndLeftWall)+FENCE);
          double OWleftSideNY = ypos+nextypos+getLeftSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(LeftWidth+EndLeftWall)+FENCE);

     //WrightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(RightWidth+rightWall));
     //WrightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(RightWidth+rightWall));
	 //WrightSideNX = xpos+nextxpos+getRightSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(RightWidth+ErightWall));
     //WrightSideNY = ypos+nextypos+getRightSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(RightWidth+ErightWall));
		  // left
		  GPPolygon *edge = new GPPolygon();	
	      edge->empty();
		  edge->addPoint(g->getScreenX(WleftSideX),g->getScreenY(WleftSideY));
		  edge->addPoint(g->getScreenX(OWleftSideX),g->getScreenY(OWleftSideY));
		  edge->addPoint(g->getScreenX(OWleftSideNX),g->getScreenY(OWleftSideNY));
		  edge->addPoint(g->getScreenX(WleftSideNX),g->getScreenY(WleftSideNY));
		  edge->addPoint(g->getScreenX(WleftSideX),g->getScreenY(WleftSideY));
		  if (t1->track->showFilledObjects){
					 edge->fillPolygon(g);
				   }
				   else {
					 edge->drawPolygon(g);
				   }
		  delete edge;
		}

	   
	}
	else if (side == LOC_WALL_RIGHT)
	{	
		g->setColor(RED_BRUSH);
		if (angle<0 || angle > 0)
				{
				   //double cx = xpos+getRightSideX(sectorStartAngle,t1->getRadius());
				   //double cy = ypos+getRightSideY(sectorStartAngle,t1->getRadius());
				   double r1 = t1->getRadius()-WIDTH_TO_TRACKWIDTH(BeginRightWidth+BeginRightWall)+FENCE;
				   double r2 = t1->getRadius()-WIDTH_TO_TRACKWIDTH(EndRightWidth+EndRightWall)+FENCE;
				   double r3 = t1->getRadius()-WIDTH_TO_TRACKWIDTH(EndRightWidth+EndRightWall);
				   double r4 = t1->getRadius()-WIDTH_TO_TRACKWIDTH(BeginRightWidth+BeginRightWall);
				   GPPolygon edge;	
				   edge.empty();
				   drawSpiralPoly(g,edge,cx,cy,sectorStartAngle,sectorStartAngle+((sectorEndAngle-sectorStartAngle)/2.0),r1,r2,false,-1);
				   drawSpiralPoly(g,edge,cx,cy,sectorStartAngle,sectorStartAngle+((sectorEndAngle-sectorStartAngle)/2.0),r4,r3,true,-1);
				   if (t1->track->showFilledObjects){
					 edge.fillPolygon(g);
				   }
				   else {
					 edge.drawPolygon(g);
				   }
				 //GPPolygon
				 //drawSpiralPoly
				 //g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,sectorStartAngle,TotalAngle,Radius-WIDTH_TO_TRACKWIDTH(RightWidth),Radius-WIDTH_TO_TRACKWIDTH(RightWidth+ErightWall));
				}
			//g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,sectorStartAngle,TotalAngle,Radius-WIDTH_TO_TRACKWIDTH(RightWidth+ErightWall),Radius-WIDTH_TO_TRACKWIDTH(RightWidth+ErightWall)+1);
		//else if (angle > 0)
		//{
			//g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,sectorStartAngle,TotalAngle,Radius-WIDTH_TO_TRACKWIDTH(RightWidth+ErightWall)+1,Radius-WIDTH_TO_TRACKWIDTH(RightWidth+ErightWall));
		//}
		else
		{
			double OWrightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(RightWidth+BeginRightWall)+FENCE);
            double OWrightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(RightWidth+BeginRightWall)+FENCE);
	        double OWrightSideNX = xpos+nextxpos+getRightSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(RightWidth+EndRightWall)+FENCE);
            double OWrightSideNY = ypos+nextypos+getRightSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(RightWidth+EndRightWall)+FENCE);
	
		   // right
		   GPPolygon *edge = new GPPolygon();	
	       edge->empty();				  
		   edge->addPoint(g->getScreenX(WrightSideX),g->getScreenY(WrightSideY));           
		   edge->addPoint(g->getScreenX(OWrightSideX),g->getScreenY(OWrightSideY));           
		   edge->addPoint(g->getScreenX(OWrightSideNX),g->getScreenY(OWrightSideNY));           
		   edge->addPoint(g->getScreenX(WrightSideNX),g->getScreenY(WrightSideNY));           
		   edge->addPoint(g->getScreenX(WrightSideX),g->getScreenY(WrightSideY));           
		   if (t1->track->showFilledObjects){
					 edge->fillPolygon(g);
				   }
				   else {
					 edge->drawPolygon(g);
				   }
		   delete edge;
		}
	}
}