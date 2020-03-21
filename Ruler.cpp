
#include "stdafx.h"
#include "Ruler.h"
#include "resource.h"
#include "math.h"

void Ruler::draw(Display *dis)
{
	dis->setColor(RULER_PEN);
	
	double x1=	dis->getObjectX(BeginPoint.x);
	double y1= dis->getObjectY(BeginPoint.y);

	double x2=	dis->getObjectX(EndPoint.x);
	double y2= dis->getObjectY(EndPoint.y);

	dis->drawLine(x1,y1,x2,y2);
	dis->drawBitmap(IDB_PIN,x1,y1-dis->getObjectX(16));
	dis->drawBitmap(IDB_PIN_BLUE,x2,y2-dis->getObjectX(16));
	
}

double Ruler::getDistance(Display *dis)
{
	double dx = dis->getObjectX(EndPoint.x) - dis->getObjectX(BeginPoint.x);
	double dy = dis->getObjectY(EndPoint.y) - dis->getObjectY(BeginPoint.y);

	return (double)sqrt((dx*dx + dy*dy));
}

double Ruler::getDistanceKm(Display *dis)
{
	return (double)(LENGTH_TO_KM * getDistance(dis))/1000.0;
 
}

double Ruler::getDistanceMiles(Display *dis)
{
	return (double)(LENGTH_TO_KM * getDistance(dis))/METERES_IN_MILE;
}