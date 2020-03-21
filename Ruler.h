#include "Display.h"

#define LENGTH_TO_KM 4.87680
#define METERES_IN_MILE 1600


class Ruler
{
   public:

   Ruler()
   {
   }

   void draw(Display *dis);

   double getDistance(Display *dis);
   double getDistanceMiles(Display *dis);
   double getDistanceKm(Display *dis);


   CPoint BeginPoint;
   CPoint EndPoint;
};