
#include "stdafx.h"

#include "GPTrack.h"
#include "TrackSection.h"
#include "TrackOpenGLView.h"
#include "Texture.h"

#include <gl\glaux.h>
#include <gl\gl.h>
#include <gl\glu.h>

extern double GetTrackMoveOnPoint(double &nextxpos,double &nextypos,double stAngle,double angle,double length,double remainder);

#define GET_GL_PT_X(x) (((x)-MidX)/SC)
#define GET_GL_PT_Y(y) (((y)-MidY)/SC)
#define GET_GL_PT_Z(z) (((z)-MidZ)/SC)


void CTrackOpenGLView::drawTextureSides(TrackSection *t,GPTrack *track)
{
	//int lastJamId=-1;
	//unsigned char *lastJamMemory=NULL;
	//CBitmap *lastjamBmp;

	if (t->TrackCommands)
	{
	int cmdSize = t->TrackCommands->size();
	for(int i=0;i<cmdSize;i++)
	{		
		TrackCmd *cmd = (TrackCmd*)t->TrackCommands->elementAt(i);

		if (cmd->cmd == 0xbc)
		{
			int startPos = cmd->getNthArg(1);
			int length   = cmd->getNthArg(3);
			int side    = cmd->getNthArg(2);
			int texture = cmd->getNthArg(4);

			//g->setColor(TextureIdToBrush(track,texture));
			if (track->showTextures){ 
			 glEnable(GL_TEXTURE_2D);

			 unsigned char *jamBits=NULL;
			 CBitmap jamBit;

			 //if (lastJamId!=-1 && lastJamId==texture)
			 //{
			 //  jamBits = lastJamMemory;
			 //  jamBits = TextureMap(*lastjamBmp,jamBits);
			 //}
			 //else
			 //{
			  //if (lastJamMemory!=NULL) delete lastJamMemory;
			  //lastJamMemory = NULL;
			  //jamBits = NULL
			  int width,height;
			  LoadJamTexture(track,jamBit,texture,256,256,&width,&height);
			  jamBits = TextureMap(GetDC(),jamBit,jamBits,FALSE); 
			  delete jamBits;
			  jamBits = NULL;
			 //}
		     //lastJamId = texture;
			 //lastJamMemory = jamBits;
			 //lastjamBmp = &jamBit;

	        }

			drawSingleSide(t,startPos,length,side);

			glDisable(GL_TEXTURE_2D);
		}		
	 }
	}

	//g->setColor(-1);
	
}


void CTrackOpenGLView::drawSingleSide(TrackSection *t1,int beginOffset,int length,int side)
{
	int sectorLength = t1->getLength();

	if (beginOffset==0 && sectorLength==length)
	{
		drawSingleSectionSide(t1,beginOffset,length,side);		
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
			  drawSingleSectionSide(t1,beginOffset,length,side);
		  }
		  else
		  {
			  // the offset and the length need altering
			  //TRACE("(2)Texture map does not run a whole section from=%d len=%d -> %d\n",beginOffset,length,sectorLength);
			  drawSingleSectionSide(t1,beginOffset,length,side);  
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
		      drawSingleSectionSide(t1,beginOffset,secLen,side);
			}
			else
			{
			  drawSingleSectionSide(t1,beginOffset,textureLength,side);
			}
			// next section
			t1 = t1->next();
			beginOffset = 0;
			textureLength -= secLen;

		  }
		}
	}
	
}

#define ZAD -0.1
#define WALL_DELTA 5

void CTrackOpenGLView::drawSingleSectionSide(TrackSection *t1,int beginOffset,int length,int side)
{
	//TrackSection *t2   = t1->next();
	TrackSection *t2 = t1->track->GetNextSection(t1);
	TrackSection *prev = t1->previous();
	double zpos     = -t1->getStartZ()+ZAD;
    double nextzpos = -t2->getStartZ()+ZAD;
	//double zpos = nextzpos;

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

	if (beginOffset!=0)
	{
	 double x1 = xpos;
	 double y1 = ypos;
	 double nangle = GetTrackMoveOnPoint(x1,y1,t1->getStartAngle(),t1->getAngle(),Length,beginOffset+1);
	 sectorStartAngle = nangle;
	 xpos += x1;
	 ypos += y1;
	 zpos = zpos+(beginOffset*((nextzpos-zpos)/Length));
	}

	if (length!=(beginOffset+Length))
	{
	 double x1 = xpos;
	 double y1 = ypos;
	 double nangle = GetTrackMoveOnPoint(x1,y1,t1->getStartAngle(),t1->getAngle(),Length,length);
	 sectorEndAngle = nangle;
	 nextxpos = x1;
	 nextypos = y1;
	 //nextzpos = (beginOffset+length)*((zpos-nextzpos)/Length);
	}

	double LeftWidth  = t1->getWidthLeft();
	double RightWidth = t1->getWidthRight();
	
	double TotalAngle = t1->getAngle()*t1->getLength();
	double angle = TotalAngle;
	

	int leftWall  =  prev->getLeftToBank()*0x40;
	int rightWall =  prev->getRightToBank()*0x40;

	int EleftWall,ErightWall; 

	
	if (angle!=0)
	{
		EleftWall  = leftWall;
	    ErightWall = rightWall;	
	}
	else
	{
		EleftWall  = t1->getLeftToBank()*0x40;
	    ErightWall = t1->getRightToBank()*0x40;
	}

	leftWall -= WALL_DELTA;
    rightWall -= WALL_DELTA;
	EleftWall -= WALL_DELTA;
    ErightWall -= WALL_DELTA;

	
	double 	TleftSideX,TleftSideY,TleftSideNX,TleftSideNY;
	double 	TrightSideX,TrightSideY,TrightSideNX,TrightSideNY;
	double 	WleftSideX,WleftSideY,WleftSideNX,WleftSideNY;
	double 	WrightSideX,WrightSideY,WrightSideNX,WrightSideNY;

	double wall_height = -0.3-ZAD;
	
	
	//if (angle == 0)
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
     WleftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(LeftWidth+leftWall));
     WleftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(LeftWidth+leftWall));
     WleftSideNX = xpos+nextxpos+getLeftSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(LeftWidth+EleftWall));
     WleftSideNY = ypos+nextypos+getLeftSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(LeftWidth+EleftWall));
     WrightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(RightWidth+rightWall));
     WrightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(RightWidth+rightWall));
	 WrightSideNX = xpos+nextxpos+getRightSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(RightWidth+ErightWall));
     WrightSideNY = ypos+nextypos+getRightSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(RightWidth+ErightWall));
	}

	if (side == LOC_FLOOR_LEFT)
	{														
				//if (angle<0)
				//{
				 //g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,sectorStartAngle,TotalAngle,Radius+WIDTH_TO_TRACKWIDTH(LeftWidth+EleftWall),Radius+WIDTH_TO_TRACKWIDTH(LeftWidth));
				//}
				//else if (angle > 0)
				//{
				 //g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,sectorStartAngle,TotalAngle,Radius+WIDTH_TO_TRACKWIDTH(LeftWidth),Radius+WIDTH_TO_TRACKWIDTH(LeftWidth+EleftWall));
				//}
				//else
				{
				  // left
				  //GPPolygon *edge = new GPPolygon();	
	              //edge->empty();
				  //edge->addPoint(g->getScreenX(TleftSideX),g->getScreenY(TleftSideY));
				  //edge->addPoint(g->getScreenX(WleftSideX),g->getScreenY(WleftSideY));
				  //edge->addPoint(g->getScreenX(WleftSideNX),g->getScreenY(WleftSideNY));
				  //edge->addPoint(g->getScreenX(TleftSideNX),g->getScreenY(TleftSideNY));
				  //edge->addPoint(g->getScreenX(TleftSideX),g->getScreenY(TleftSideY));
				  //edge->fillPolygon(g);
				  //delete edge;
				  // left bank
		          glColor3f(1.0f,1.0f,0.0f);
		          glBegin(GL_POLYGON);
		          glTexCoord2f(0.0f,0.0f);
		          glVertex3d(GET_GL_PT_X(TleftSideX),GET_GL_PT_Y(TleftSideY),GET_GL_PT_Z(zpos));
		          glTexCoord2f(0.0f,1.0f);
		          glVertex3d(GET_GL_PT_X(WleftSideX),GET_GL_PT_Y(WleftSideY),GET_GL_PT_Z(zpos));
		          glTexCoord2f(1.0f,1.0f);
		          glVertex3d(GET_GL_PT_X(WleftSideNX),GET_GL_PT_Y(WleftSideNY),GET_GL_PT_Z(nextzpos));
		          glTexCoord2f(1.0f,0.0f);
		          glVertex3d(GET_GL_PT_X(TleftSideNX),GET_GL_PT_Y(TleftSideNY),GET_GL_PT_Z(nextzpos));		 		 	 
		          glEnd();
				}

	   
	}
	else if (side == LOC_FLOOR_RIGHT)
			{
				//double cx = xpos+getRightSideX(sectorStartAngle,Radius);
	            //double cy = ypos+getRightSideY(sectorStartAngle,Radius);
				
				//if (angle<0)
				//{
				// g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,sectorStartAngle,TotalAngle,Radius-WIDTH_TO_TRACKWIDTH(RightWidth),Radius-WIDTH_TO_TRACKWIDTH(RightWidth+ErightWall));
				//}
				//else if (angle > 0)
				//{
				// g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,sectorStartAngle,TotalAngle,Radius-WIDTH_TO_TRACKWIDTH(RightWidth+ErightWall),Radius-WIDTH_TO_TRACKWIDTH(RightWidth));
				//}
				//else
				{
				// right
				//GPPolygon *edge = new GPPolygon();	
	            //edge->empty();				  
				//edge->addPoint(g->getScreenX(TrightSideX),g->getScreenY(TrightSideY));           
				//edge->addPoint(g->getScreenX(WrightSideX),g->getScreenY(WrightSideY));           
				//edge->addPoint(g->getScreenX(WrightSideNX),g->getScreenY(WrightSideNY));           
				//edge->addPoint(g->getScreenX(TrightSideNX),g->getScreenY(TrightSideNY));           
				//edge->addPoint(g->getScreenX(TrightSideX),g->getScreenY(TrightSideY));           
				//edge->fillPolygon(g);
				//delete edge;
				// right bank
		 		glColor3f(1.0f,1.0f,0.0f);                                                              
		 		glBegin(GL_POLYGON);                                                                    
		 		glTexCoord2f(0.0f,0.0f);                                                                
		 		glVertex3d(GET_GL_PT_X(TrightSideX),GET_GL_PT_Y(TrightSideY),GET_GL_PT_Z(zpos));          
		 		glTexCoord2f(0.0f,1.0f);                                                                
		 		glVertex3d(GET_GL_PT_X(WrightSideX),GET_GL_PT_Y(WrightSideY),GET_GL_PT_Z(zpos));        
		 		glTexCoord2f(1.0f,1.0f);                                                                
		 		glVertex3d(GET_GL_PT_X(WrightSideNX),GET_GL_PT_Y(WrightSideNY),GET_GL_PT_Z(nextzpos));  
		 		glTexCoord2f(1.0f,0.0f);                                                                
		 	 	glVertex3d(GET_GL_PT_X(TrightSideNX),GET_GL_PT_Y(TrightSideNY),GET_GL_PT_Z(nextzpos));			 		                                                                                        
		 		glEnd();                                                                                
				}
				
	}
	else if (side == LOC_ROAD)
	{
				//double cx = xpos+getRightSideX(sectorStartAngle,Radius);
	            //double cy = ypos+getRightSideY(sectorStartAngle,Radius);
				
				//if (angle<0)
				//{
				// g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,sectorStartAngle,TotalAngle,Radius+WIDTH_TO_TRACKWIDTH(RightWidth),Radius-WIDTH_TO_TRACKWIDTH(LeftWidth));
				//}
				//else if (angle > 0)
				//{
				// g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,sectorStartAngle,TotalAngle,Radius-WIDTH_TO_TRACKWIDTH(LeftWidth),Radius+WIDTH_TO_TRACKWIDTH(RightWidth));
				//}
				//else
				{
				 // road
				 //GPPolygon *edge = new GPPolygon();	
	             //edge->empty();				  
				 //edge->addPoint(g->getScreenX(TrightSideX),g->getScreenY(TrightSideY));           
				 //edge->addPoint(g->getScreenX(TrightSideNX),g->getScreenY(TrightSideNY));           
				 //edge->addPoint(g->getScreenX(TleftSideNX),g->getScreenY(TleftSideNY));           
				 //edge->addPoint(g->getScreenX(TleftSideX),g->getScreenY(TleftSideY));           
				 //edge->addPoint(g->getScreenX(TrightSideX),g->getScreenY(TrightSideY));           
				 //edge->fillPolygon(g);
				 //delete edge;
					//road
		 		  glColor3f(0.0f,0.0f,1.0f);                                                              
		 		  glBegin(GL_POLYGON);                                                                    
		 		  glTexCoord2f(0.0f,0.0f);                                                                
		 		  glVertex3d(GET_GL_PT_X(TleftSideX),GET_GL_PT_Y(TleftSideY),GET_GL_PT_Z(zpos));            
		 		  glTexCoord2f(0.0f,1.0f);                                                                
		 		  glVertex3d(GET_GL_PT_X(TrightSideX),GET_GL_PT_Y(TrightSideY),GET_GL_PT_Z(zpos));          
		 		  glTexCoord2f(1.0f,1.0f);                                                                
		 		  glVertex3d(GET_GL_PT_X(TrightSideNX),GET_GL_PT_Y(TrightSideNY),GET_GL_PT_Z(nextzpos));    
		 		  glTexCoord2f(1.0f,0.0f);                                                                
		 		  glVertex3d(GET_GL_PT_X(TleftSideNX),GET_GL_PT_Y(TleftSideNY),GET_GL_PT_Z(nextzpos));		 
	  	 		  glEnd();                                                                                
				}
	}
	else if (side == LOC_WALL_LEFT)
	{	
		/*
		g->setColor(RED_BRUSH);
		if (angle<0)
		{
		  g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,sectorStartAngle,TotalAngle,Radius+WIDTH_TO_TRACKWIDTH(LeftWidth+EleftWall)+1,Radius+WIDTH_TO_TRACKWIDTH(LeftWidth+EleftWall));
		}
		else if (angle > 0)
		{
		  g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,sectorStartAngle,TotalAngle,Radius+WIDTH_TO_TRACKWIDTH(LeftWidth+EleftWall),Radius+WIDTH_TO_TRACKWIDTH(LeftWidth+EleftWall)+1);
		}
		else
		{
		  // walls parts
          double OWleftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(LeftWidth+leftWall)+1);
          double OWleftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(LeftWidth+leftWall)+1);
          double OWleftSideNX = xpos+nextxpos+getLeftSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(LeftWidth+EleftWall)+1);
          double OWleftSideNY = ypos+nextypos+getLeftSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(LeftWidth+EleftWall)+1);

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
		  edge->fillPolygon(g);
		  delete edge;
		}
		*/
		 glColor3f(1.0f,0.0f,0.0f);
		 glBegin(GL_POLYGON);
		 glTexCoord2f(0.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(WleftSideX),GET_GL_PT_Y(WleftSideY),GET_GL_PT_Z(zpos));
		 glTexCoord2f(0.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(WleftSideX),GET_GL_PT_Y(WleftSideY),GET_GL_PT_Z(zpos+wall_height));
		 glTexCoord2f(1.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(WleftSideNX),GET_GL_PT_Y(WleftSideNY),GET_GL_PT_Z(nextzpos+wall_height));
		 glTexCoord2f(1.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(WleftSideNX),GET_GL_PT_Y(WleftSideNY),GET_GL_PT_Z(nextzpos));		 	 
		 glEnd();
	   
	}
	else if (side == LOC_WALL_RIGHT)
	{	
		/*
		g->setColor(RED_BRUSH);
		if (angle<0)
		{
			g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,sectorStartAngle,TotalAngle,Radius-WIDTH_TO_TRACKWIDTH(RightWidth+ErightWall),Radius-WIDTH_TO_TRACKWIDTH(RightWidth+ErightWall)+1);
		}
		else if (angle > 0)
		{
			g->drawArcSpecial((xpos+nextxpos), (ypos+nextypos),xpos,ypos,cx,cy,sectorStartAngle,TotalAngle,Radius-WIDTH_TO_TRACKWIDTH(RightWidth+ErightWall)+1,Radius-WIDTH_TO_TRACKWIDTH(RightWidth+ErightWall));
		}
		else
		{
			double OWrightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(RightWidth+rightWall)+1);
            double OWrightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(RightWidth+rightWall)+1);
	        double OWrightSideNX = xpos+nextxpos+getRightSideX(sectorEndAngle,WIDTH_TO_TRACKWIDTH(RightWidth+ErightWall)+1);
            double OWrightSideNY = ypos+nextypos+getRightSideY(sectorEndAngle,WIDTH_TO_TRACKWIDTH(RightWidth+ErightWall)+1);
	
		   // right
		   GPPolygon *edge = new GPPolygon();	
	       edge->empty();				  
		   edge->addPoint(g->getScreenX(WrightSideX),g->getScreenY(WrightSideY));           
		   edge->addPoint(g->getScreenX(OWrightSideX),g->getScreenY(OWrightSideY));           
		   edge->addPoint(g->getScreenX(OWrightSideNX),g->getScreenY(OWrightSideNY));           
		   edge->addPoint(g->getScreenX(WrightSideNX),g->getScreenY(WrightSideNY));           
		   edge->addPoint(g->getScreenX(WrightSideX),g->getScreenY(WrightSideY));           
		   edge->fillPolygon(g);
		   delete edge;
		}
		*/
		 glColor3f(1.0f,0.0f,0.0f);
		 glBegin(GL_POLYGON);
		 //glTexCoord2f(1.0f,1.0f);
		 glTexCoord2f(1.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(WrightSideX),GET_GL_PT_Y(WrightSideY),GET_GL_PT_Z(zpos));
		 //glTexCoord2f(1.0f,0.0f);
		 glTexCoord2f(1.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(WrightSideX),GET_GL_PT_Y(WrightSideY),GET_GL_PT_Z(zpos+wall_height));
		 //glTexCoord2f(0.0f,0.0f);
		 glTexCoord2f(0.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(WrightSideNX),GET_GL_PT_Y(WrightSideNY),GET_GL_PT_Z(nextzpos+wall_height));
		 //glTexCoord2f(0.0f,1.0f);
		 glTexCoord2f(0.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(WrightSideNX),GET_GL_PT_Y(WrightSideNY),GET_GL_PT_Z(nextzpos));		 		 
		 glEnd();
	}
}