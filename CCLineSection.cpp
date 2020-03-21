#include "stdafx.h"
#include "CCLineSection.h"
#include "CLineEditor.h"
#include "math.h"
#include "Tool.h"

void CCLineSection::setAngle(double angle)
{}

CCLineSection::~CCLineSection()
{
  if (linePt) delete linePt;
}

LinePt *
  CCLineSection::drawLine(Display *g, double *x, double *y, double *curr, double nrad)
{
  if (linePt) delete linePt;
  double xpos = getStartX();
  double ypos = getStartY();
  double nextxpos = getEndX();
  double nextypos = getEndY();
  double sectorStartAngle = getStartAngle();
  double sectorEndAngle = getEndAngle();

  double width = 4;

  // track parts
  double leftSideX = xpos + getLeftSideX(sectorStartAngle, width);
  double leftSideY = ypos + getLeftSideY(sectorStartAngle, width);

  double rightSideX = xpos + getRightSideX(sectorStartAngle, width);
  double rightSideY = ypos + getRightSideY(sectorStartAngle, width);

  double angle = sectorStartAngle;

  double rad = getRadius();
  // the ponk way
  // double rad       = getRadius()-nrad;
  double direction = (double)getArg2();
  double ndirection = (double)getArg3();
  if (direction > 0x8000) direction = -(0x10000 - direction);
  if (ndirection > 0x8000) ndirection = -(0x10000 - ndirection);

  // rad = 0;
  double rad2 = 0;
  rad = RadiusTo(track, rad, direction, (double)getArg1(), getCmd(), ndirection, getArg4(), rad2);
  // rad = arg2 >> 10;

  // int arg1 = (int)getArg1();

  if (cmd == 0x50) {
    g->setColor(2);
  } else {
    g->setColor(5);
  }

  if (fabs(rad) > 1000000.0) {
    rad = 0;
  }

  if (selected) g->setColor(4);

  if (rad == 0) {
    // draw large curves as lines

    double cx = xpos + getRightSideX(sectorStartAngle, rad);
    double cy = ypos + getRightSideY(sectorStartAngle, rad);

    g->drawLine(xpos, ypos, leftSideX, leftSideY);
    g->drawLine(xpos, ypos, rightSideX, rightSideY);

    g->drawLine(xpos, ypos, (xpos + nextxpos), (ypos + nextypos));
  } else {
    double cx = xpos + getRightSideX(sectorStartAngle, rad);
    double cy = ypos + getRightSideY(sectorStartAngle, rad);

    if (selected) {
      g->setColor(4);
    } else {
      g->setColor(3);
    }
    g->drawLine(xpos, ypos, leftSideX, leftSideY);
    g->drawLine((xpos + nextxpos), (ypos + nextypos), xpos, ypos);
    g->drawLine(xpos, ypos, rightSideX, rightSideY);

    if (selected) {
      g->setColor(4);
    } else {
      if (cmd == 0x50) {
        g->setColor(2);
      } else {
        g->setColor(5);
      }
    }
    if (getCmd() == 0x50) {
      g->drawSpiral(cx, cy, getStartAngle(), getStartAngle() + (getEndAngle() - getStartAngle()) / 2.0, rad, rad);
      // g->drawArc((xpos+nextxpos),
      // (ypos+nextypos),xpos,ypos,cx,cy,getStartAngle(),getEndAngle()-getStartAngle(),rad);
      // g->drawLine(cx,cy,xpos,ypos);
      // g->drawLine(cx,cy,xpos+nextxpos,ypos+nextypos);
    } else {
      rad2 = rad + rad2;
      double angle1 = getStartAngle() - PI / 2.0;
      double angle2 = getEndAngle() - PI / 2.0;
      double delta = (angle2 - angle1) / 2.0;
      g->drawSpiral(cx, cy, getStartAngle(), getStartAngle() + delta, rad, rad);
      // g->drawArc((xpos+nextxpos),
      // (ypos+nextypos),xpos,ypos,cx,cy,getStartAngle(),getEndAngle()-getStartAngle(),rad);
      // g->drawSpiral(cx,cy,getStartAngle(),getEndAngle(),rad,rad);
      /*
      //rad2 = rad2/2.0;
      //rad = rad/2.0;
      double radBegin = rad2;
      double radDelta = (rad-rad2)/((double)length);
      double nx,ny;
      nx = 0;
      ny = 0;
      double currrad = getStartAngle();
      for(int k=0;k<length;k++)
      {
              double anglerad = ((1.0)/(radBegin));
              //anglerad = DegToRad((int)(RadToDeg(anglerad)));
              double inrad = anglerad/2.0;
              double len   = 2.0*radBegin*sin(inrad);
              double angle1 = currrad+inrad;
              nextypos =  (len*sin((angle1)));
              nextxpos =  (len*cos((angle1)));
              g->drawLine(xpos+nx, ypos+ny,xpos+nx+nextxpos,ypos+ny+nextypos);
              double cx = xpos+getRightSideX(currrad,radBegin);
              double cy = ypos+getRightSideY(currrad,radBegin);
              //g->drawLine(cx,cy,xpos+nx,ypos+ny);
              nx += nextxpos;
              ny += nextypos;
              //sectorEndAngle = angle1;
              currrad += anglerad;
              radBegin+=radDelta;
      }
      //g->drawSpiral(cx,cy,getStartAngle(),getEndAngle(),rad,rad2);
      */
    }
  }

  linePt = new LinePt((int)xpos, (int)ypos);
  return linePt;
}

Vector *
  CCLineSection::getVector()
{
  return track->CCLineSections;
}

void RadiusFrom(int &radius, int &direction)
{
  if (radius == 0) {
    radius = 0;
    direction = 0;
  }

  if (direction == -1) {
    radius = 0xFFFF - radius;
  }

  if (direction > 1) {
    int size = (int)abs(direction);
    double dirPow = pow(2, size);
    radius = (int)(dirPow * abs(radius));
  }
  if (direction < -1) {
    int size = (int)abs(direction);
    double dirPow = pow(2, size);
    radius = (int)(dirPow * abs(radius));
  }
}

void RadiusFrom(double &radius, int &direction)
{
  double radiusAbs = fabs(radius);
  // straight line cases
  if (radius == 0) {
    radius = 0;
    direction = 0;
    return;
  }

  if (radiusAbs < 0xFFFF) {
    // -1 case
    if (radius < 0) {
      direction = -1;
      radius = 0xFFFF - radiusAbs;
      return;
    }
    // 0 case
    else {
      direction = 0;
      radius = radiusAbs;
      return;
    }
  }

  // ok radius is greater than 0xFFFF
  // lets guess the sign

  // direction          = (int)(radiusAbs/0xFFFF);
  int dirPow = (int)(radiusAbs / 0xFFFF);
  if (dirPow > 0) {
    direction = (int)(log((double)dirPow) / log(2.0));
    direction++;
  } else {
    direction = 0;
  }

  double totalRadius = pow(2.0, 15 + direction);

  // TRACE("total %f  %f\n",totalRadius,direction);
  int remainder = (int)(radiusAbs - totalRadius);

  // can do this before total worked out
  if (radius < 0) {
    direction = -1 * direction;
    direction--;
    remainder = 0xFFFF - remainder;
  }

  radius = remainder;
}

int RadiusVal(GPTrack *track, double rad, double direction, double arg1, int cmd, double ndirection, double arg4, double &radius2)
{
  int radius = (int)rad;
  radius2 = (int)arg4;
  if (fabs(direction) > 10.0) {
    radius = 0;
  }
  if (radius == 0) return 0;

  if (cmd == 0x70) {
    // radius = (arg4+radius)/2.0;
    // direction = arg3;
    if (direction < 0) {
      // radius = radius+1000;
    } else {
      // radius = radius-1000;
    }
  }

  if (direction == 1) {
    radius = 0xFFFF + abs(radius);
  }
  if (direction == -1) {
    radius = abs(0xFFFF - radius);
  }

  if (direction > 1) {
    int size = (int)fabs(direction);
    double dirPow = pow(2, (15 + size));
    radius = (int)((dirPow) + abs(radius));
  }
  if (direction < -1) {
    int size = (int)fabs(direction);
    double dirPow = pow(2, (15 + size - 1));
    radius = (int)((dirPow) + abs(0xFFFF - radius));
  }

  if (cmd == 0x70) {
    if (ndirection == 1) {
      radius2 = 0xFFFF + abs(radius2);
    }
    if (ndirection == -1) {
      radius2 = abs(0xFFFF - radius2);
    }

    if (ndirection > 1) {
      int size = (int)fabs(ndirection);
      double dirPow = pow(2, (15 + size));
      radius2 = (int)((dirPow) + abs(radius));
    }
    if (ndirection < -1) {
      int size = (int)fabs(ndirection);
      double dirPow = pow(2, (15 + size - 1));
      radius2 = (int)((dirPow) + abs(0xFFFF - radius));
    }
  }
  return radius;
}

double
  RadiusTo(GPTrack *track, double radius, double direction, double arg1, int cmd, double arg3, double arg4, double &rad2)
{
  radius =
    (double)RadiusVal(track, radius, direction, arg1, cmd, arg3, arg4, rad2);
  radius = ((radius - (arg1 * direction)) / (track->CCScaleFactor));
  // radius = ((radius)/(track->CCScaleFactor));
  rad2 = rad2 / (track->CCScaleFactor);

  if (fabs(radius) < 1.0) radius = 0.0;

  if (direction < 0) radius = -radius;
  if (arg3 < 0) rad2 = -rad2;

  return radius;
}

int CCLineSection::write(GPTrack *track, int offset)
{
  track->WritePCByte(offset, (int)length);
  offset++;
  track->WritePCByte(offset, cmd);
  offset++;

  int radius = (int)getRadius();
  int nRadius = radius;

  int scale = (int)getArg2();

  if (cmd == 0x70) {
    track->WritePCWord(offset, (int)getArg1());
    offset += 2;
    track->WritePCWord(offset, scale);
    offset += 2;
    track->WritePCWord(offset, nRadius);
    offset += 2;
    track->WritePCWord(offset, (int)getArg3());
    offset += 2;
    track->WritePCWord(offset, (int)getArg4());
    offset += 2;
  } else {
    track->WritePCWord(offset, (int)getArg1());
    offset += 2;
    track->WritePCWord(offset, scale);
    offset += 2;
    track->WritePCWord(offset, nRadius);
    offset += 2;
  }
  return offset;
}

int CCLineSection::debug(FILE *fp, GPTrack *track, int offset)
{
  fprintf(fp, "0x%x %x %d %d %d %d %d %d\n", offset, cmd, (int)getLength(), (int)getRadius(), (int)getArg1(), (int)getArg2(), (int)getArg3(), (int)getArg4());
  return offset;
}

void CCLineSection::Clear(int minLength)
{
  setLength(minLength);
  setArg1(0);
  setArg2(0);
  setArg3(0);
  setArg4(0);
  setRadius(0);
}

int CCLineSection::Open()
{
  CCLineEditor *ccdlg = new CCLineEditor(NULL);

  int result = ccdlg->EditCCSections(this);

  delete ccdlg;
  track->GetDocument()->UpdateAllViews(NULL);
  return result;
}

void CCLineSection::Selected()
{
  track->setCCSelection(this);
  track->GetDocument()->UpdateAllViews(NULL);
}
