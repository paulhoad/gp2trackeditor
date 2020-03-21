
#include "Display.h"
#include "math.h"
#include "GPTrack.h"

class LinePt;

#ifndef _CC_LINESECTION_H_
#define _CC_LINESECTION_H_

class CCLineSection: public Observer, public CObject 
{
  int offset;
  BOOL selected;

public:
   GPTrack *track;
   LinePt *linePt;
  
  

  
	CCLineSection(GPTrack *mytrack,int length,double radius,int d1,int d2,int d3,int d4,int cmd,int offset):
	track(mytrack),
	length(length),radius(radius),d1(d1),d2(d2),d3(d3),d4(d4),cmd(cmd),offset(offset),
	Observer(mytrack,t_CCSection,NULL)
    {
     	selected = FALSE;
		startX = 0;
		startY = 0;
		endX = 0;
		endY = 0;
		startAngle = 0;
		endAngle = 0;
		setTreeNode(NULL);
		linePt = NULL;
    }

	CCLineSection(CCLineSection *t):
		track(t->track),
	    length(t->length),
		radius(t->radius),d1(t->d1),d2(t->d2),d3(t->d3),d4(t->d4),cmd(t->cmd),offset(t->offset),
		Observer(t)
	{
		linePt = NULL;
	}

	~CCLineSection();
	
	int write(GPTrack *track,int offset);
	int debug(FILE *fp,GPTrack *track,int offset);

	void Clear(int minLength);

    void setSelected(BOOL val)
	{
		selected = val;
	}

	BOOL isSelected()
	{
		return selected;
	}

	int getOffset()
	{
		 return offset;
	}

    double getLength()
    {
         return length;
    }

	void setLength(double len)
    {
          length=len;
    }

    int getCmd()
	{
		 return cmd;
	}

	void setCmd(int data)
	{
		 cmd=data;
	}

     double getRadius()
     {
         return radius;
     }

	 void setRadius(int data)
	 {
		 radius = data;
	 }

	 void setAngle(double angle);

     double getArg1()
     {
         return d1;
     }

     double getArg2()
     {
         return d2;
     }

	 double getArg3()
     {
         return d3;
     }

     double getArg4()
     {
         return d4;
     }

	 void setArg1(int data)
     {
          d1 = data;
     }

     void setArg2(double data)
     {
		 if (data > 0x8000)
			 data = 0xFFFF - data;

         d2 = data;
     }

	 void setArg3(int data)
     {
         d3 = data;
     }

     void setArg4(int data)
     {
         d4 = data;
     }



	LinePt *drawLine(Display *g,double *x,double *y,double *curr,double nrad);
	

	void draw(Display *g,double x1,double y1)
    {
        g->drawLine(x1-2,y1-2,x1+2,y1+2);
        g->drawLine(x1-2,y1+2,x1+2,y1-2);
    }

	Vector * getVector();
	
	double getStartX()
    {
        return startX;
    }

     void setStartX(double X)
    {
        startX=X;
    }

     double getStartY()
    {
        return startY;
    }

     void setStartY(double Y)
    {
        startY=Y;
    }

     double getEndX()
    {
        return endX;
    }

     void setEndX(double X)
    {
        endX=X;
    }

     double getEndY()
    {
        return endY;
    }

     void setEndY(double Y)
    {
        endY=Y;
    }

	  double getStartAngle()
    {
        return startAngle;
    }

     void setStartAngle(double angle)
    {
        startAngle=angle;
    }

     double getEndAngle()
    {
        return endAngle;
    }

     void setEndAngle(double angle)
    {
        endAngle=angle;
    }

	 int getIcon()
	 {
		 int id = IDB_CC_LINE;
		 if (cmd == 0x70)
		 {
		    id = IDB_CC_LINE1;

		    int directction = (int)getArg2();
		    double rad      = getRadius();

		    if (directction >= 0)      id = IDB_CC_RIGHT1;
		    else if (directction < 0) id = IDB_CC_LEFT1;

		    if (rad == 0)
		    {
		  	   id = IDB_CC_LINE1; 
		    }
		 }
		 else
		 {
			 id = IDB_CC_LINE;

		     int directction = (int)getArg2();
		     double rad      = getRadius();

		     if (directction >= 0)      id = IDB_CC_RIGHT;
		     else if (directction < 0) id = IDB_CC_LEFT;

		     if (rad == 0)
		     {
			   id = IDB_CC_LINE; 
		     }
		 }

		 return id;
	 }

	 void Selected();
	 int Open();



     double length;
     double radius;
     double d1;
     double d2;
	 double d3;
	 double d4;
	 int cmd;
	 double startX;
	 double startY;
	 double endX;
	 double endY;
	 double startAngle;
	 double endAngle;

	 int arg2;
	 int arg3;
	 

	 //HTREEITEM treeNode;

};

#endif
