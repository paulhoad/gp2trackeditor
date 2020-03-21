
#ifndef _POLYGON_INCLUDED_H_
#define _POLYGON_INCLUDED_H_

#include "Vector.h"

class Point: public CObject
{
    public:
	Point(long x1,long y1)
	{
		x = x1;
		y = y1;
	}

	virtual ~Point()
	{
	}

	long x;
	long y;
};


class GPPolygon: public CObject
{
    public:

	GPPolygon()
	{
		points = new Vector("GPPolygon1",__LINE__);
	}

	virtual ~GPPolygon()
	{
		if (points != NULL) 
			delete points;
		points = NULL;
	}

	void empty()
	{
		points->removeAllData();
	}


	void addPoint(long x,long y)
	{
		Point *pt = new Point(x,y);
		points->addElement(pt);
	}

	void drawPolygon(Display *dpy)
	{
		int size = points->size();

		LPPOINT pts = (LPPOINT)malloc(size*sizeof(POINT));

		for(int i=0;i<size;i++)
		{
			Point *pt = (Point*)(points)->elementAt(i);
			pts[i].x = pt->x;
			pts[i].y = pt->y;
		}

		dpy->drawPolygon(pts,size);

		free(pts);
	}

	void fillPolygon(Display *dpy)
	{
		int size = points->size();

		LPPOINT pts = (LPPOINT)malloc(size*sizeof(POINT));

		for(int i=0;i<size;i++)
		{
			Point *pt = (Point*)(points)->elementAt(i);
			pts[i].x = pt->x;
			pts[i].y = pt->y;
		}

		dpy->fillPolygon(pts,size);

		free(pts);
	}

	BOOL contains(CPoint pt)
	{
		int minX=10000000;
		int minY=10000000;
		int maxX=-10000000;
		int maxY=-10000000;

		int size = points->size();

	    for(int i=0;i<size;i++)
		{
			Point *pt = (Point*)(points)->elementAt(i);			
			minX = min(minX,pt->x);
			minY = min(minY,pt->y);
			maxX = max(maxX,pt->x);
			maxY = max(maxY,pt->y);
		}




		//AfxMessageBox("Min="+minX,MB_OK);
		//AfxMessageBox("Min="+minY,MB_OK);

		if (pt.x > minX && pt.x < maxX)
		{
			if (pt.y > minY && pt.y < maxY)
			{
				return TRUE;
			}
		}
		
		return FALSE;
	}

	

	Vector *points;
};


#endif



