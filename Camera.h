
#ifndef _INCLUDED_CAMERA_H_
#define _INCLUDED_CAMERA_H_

#include "Observer.h"
#include "GPTrack.h"

class CPosAngle
{
public:
  CPosAngle(double x, double y, double angle, double cx, double cy, double z)
    : x(x), y(y), angle(angle), cx(cx), cy(cy), z(z)
  {}

  double x;
  double y;
  double angle;
  double cx;
  double cy;
  double z;
};

CPosAngle *
  DistanceToPoint(GPTrack *track, double distance);

class Camera : public Observer
  , public CObject
{
public:
  int offset;
  BOOL selected;

  Camera(GPTrack *mytrack, int myoffset)
    : X(0),
      offset(myoffset),
      Observer(mytrack, t_Camera, NULL),
      selected(FALSE)
  {}

  Camera(GPTrack *mytrack, int number, int X, int Y, int Z1, int Z2, int SM, int ZF1, int ZF2, int VR1, int VR2, int UK1, int UK2)
    : number(number),
      X(X),
      Y(Y),
      Z1(Z1),
      Z2(Z2),
      SM(SM),
      ZF1(ZF1),
      ZF2(ZF2),
      VR1(VR1),
      VR2(VR2),
      UK1(UK1),
      UK2(UK2),
      Observer(mytrack, t_Camera, NULL)
  {
    UK3 = 1;
    UK4 = 0;
    UK5 = 0;
    UK6 = 1;
    UK7 = 0;
    selected = FALSE;
  }

  void
    setSelected(BOOL sel)
  {
    selected = sel;
  }

  double
    DistanceFromPt(CPoint pt);

  BOOL
    getSelected()
  {
    return selected;
  }

  // void DrawCamera(Display *g,TrackSection *t,double inDistance,BOOL
  // showBitmaps=TRUE);
  void
    DrawCamera(Display *g, GPTrack *track);

  void
    Delete();
  void
    InsertNew();

  void
    setNumber(int num)
  {
    number = num;
  }

  int getNumber()
  {
    return number;
  }

  int getDistance()
  {
    return X;
  }

  void
    setDistance(int d)
  {
    X = d;
  }

  int getSwitchMoment()
  {
    return SM;
  }

  void
    setSwitchMoment(int d)
  {
    SM = d;
  }

  int getPerpendicularDistance()
  {
    return Y;
  }

  void
    setPerpendicularDistance(int d)
  {
    Y = d;
  }

  int getZ1()
  {
    return Z1;
  }

  void
    setZ1(int d)
  {
    Z1 = d;
  }

  int getZ2()
  {
    return Z2;
  }

  void
    setZ2(int d)
  {
    Z2 = d;
  }

  int getZF1()
  {
    return ZF1;
  }

  void
    setZF1(int d)
  {
    ZF1 = d;
  }

  int getZF2()
  {
    return ZF2;
  }

  void
    setZF2(int d)
  {
    ZF2 = d;
  }

  int write(GPTrack *track, int offset);
  int read(GPTrack *track, int offset);

  void
    Selected();
  int Open();

  int X;
  int Y;
  int Z1;
  int Z2;
  int SM;
  int ZF1;
  int ZF2;
  int VR1;
  int VR2;
  int UK1;
  int UK2;
  int UK3;
  int UK4;
  int UK5;
  int UK6;
  int UK7;

  // where it is drawn on the screen
  double x;
  double y;

  void exportFile(FILE *fp, int num)
  {
    fprintf(fp, "%d %d %d %d %d %d %d %d %d %d %d %d\n", num, X, Y, Z1, Z2, SM, ZF1, ZF2, VR1, VR2, UK1, UK2);
  }

  // int SwitchTime;
  int number;
  // int distance;
  // int height;
  // short perpendist;
  // int zoomEnd1;
  // int zoomEnd2;
  // int end1distance;
  // int end2distance;
  // int objdist;
  // int stilltime;
  // int beginZoom;
  // int SpeedBeginZoom;
  // int endZoom;
  // int SpeedEndZoom;
  // int Unknown1;
  // int Unknown2;
  // int Unknown3;
  // int Unknown4;
};

#endif