

#ifndef _INCLUDED_WORLD_H
#define _INCLUDED_WORLD_H

#include "Vector.h"
#include "math.h"
#include "Display.h"

class Matrix3D;
class Point3D;

class BBox3D
{
public:
  BBox3D()
  {
    xmin = 10E8;
    ymin = 10E8;
    zmin = 10E8;
    xmax = -10E8;
    ymax = -10E8;
    zmax = -10E8;
  }

  void
    adjustWith(double x, double y, double z)
  {
    xmin = min(xmin, x);
    ymin = min(ymin, y);
    zmin = min(zmin, z);

    xmax = max(xmax, x);
    ymax = max(ymax, y);
    zmax = max(zmax, z);
  }

  void
    adjustWith(BBox3D *box)
  {
    xmin = min(xmin, box->xmin);
    ymin = min(ymin, box->ymin);
    zmin = min(zmin, box->zmin);

    xmax = max(xmax, box->xmax);
    ymax = max(ymax, box->ymax);
    zmax = max(zmax, box->zmax);
  }

  double xmin;
  double ymin;
  double zmin;

  double xmax;
  double ymax;
  double zmax;
};

class Primitive3D : public CObject
{
public:
  Primitive3D() {}

  virtual void
    draw(Display *d, Point3D *center)
  {}

  virtual void
    fill(Display *d, Point3D *center)
  {}

  virtual void
    Transform(Matrix3D *mat, Point3D *mid, Point3D *center, double scalefactor)
  {}

  virtual BBox3D *
    findBoundingBox()
  {
    return NULL;
  }
};

#define PI 3.14159265

class Matrix3D
{
public:
  double xx, xy, xz, xo;
  double yx, yy, yz, yo;
  double zx, zy, zz, zo;

  /** Create a new unit matrix */
  Matrix3D() { unit(); }
  /** Scale by f in all dimensions */
  void
    scale(double f)
  {
    xx *= f;
    xy *= f;
    xz *= f;
    xo *= f;
    yx *= f;
    yy *= f;
    yz *= f;
    yo *= f;
    zx *= f;
    zy *= f;
    zz *= f;
    zo *= f;
  }
  /** Scale along each axis independently */
  void
    scale(double xf, double yf, double zf)
  {
    xx *= xf;
    xy *= xf;
    xz *= xf;
    xo *= xf;
    yx *= yf;
    yy *= yf;
    yz *= yf;
    yo *= yf;
    zx *= zf;
    zy *= zf;
    zz *= zf;
    zo *= zf;
  }
  /** Translate the origin */
  void
    translate(double x, double y, double z)
  {
    xo += x;
    yo += y;
    zo += z;
  }
  /** rotate theta degrees about the y axis */
  void
    yrot(double theta)
  {
    theta *= (PI / 180.0);
    double ct = cos(theta);
    double st = sin(theta);

    double Nxx = (double)(xx * ct + zx * st);
    double Nxy = (double)(xy * ct + zy * st);
    double Nxz = (double)(xz * ct + zz * st);
    double Nxo = (double)(xo * ct + zo * st);

    double Nzx = (double)(zx * ct - xx * st);
    double Nzy = (double)(zy * ct - xy * st);
    double Nzz = (double)(zz * ct - xz * st);
    double Nzo = (double)(zo * ct - xo * st);

    xo = Nxo;
    xx = Nxx;
    xy = Nxy;
    xz = Nxz;
    zo = Nzo;
    zx = Nzx;
    zy = Nzy;
    zz = Nzz;
  }
  /** rotate theta degrees about the x axis */
  void
    xrot(double theta)
  {
    theta *= (PI / 180.0);
    double ct = cos(theta);
    double st = sin(theta);

    double Nyx = (double)(yx * ct + zx * st);
    double Nyy = (double)(yy * ct + zy * st);
    double Nyz = (double)(yz * ct + zz * st);
    double Nyo = (double)(yo * ct + zo * st);

    double Nzx = (double)(zx * ct - yx * st);
    double Nzy = (double)(zy * ct - yy * st);
    double Nzz = (double)(zz * ct - yz * st);
    double Nzo = (double)(zo * ct - yo * st);

    yo = Nyo;
    yx = Nyx;
    yy = Nyy;
    yz = Nyz;
    zo = Nzo;
    zx = Nzx;
    zy = Nzy;
    zz = Nzz;
  }
  /** rotate theta degrees about the z axis */
  void
    zrot(double theta)
  {
    theta *= (PI / 180.0);
    double ct = cos(theta);
    double st = sin(theta);

    double Nyx = (double)(yx * ct + xx * st);
    double Nyy = (double)(yy * ct + xy * st);
    double Nyz = (double)(yz * ct + xz * st);
    double Nyo = (double)(yo * ct + xo * st);

    double Nxx = (double)(xx * ct - yx * st);
    double Nxy = (double)(xy * ct - yy * st);
    double Nxz = (double)(xz * ct - yz * st);
    double Nxo = (double)(xo * ct - yo * st);

    yo = Nyo;
    yx = Nyx;
    yy = Nyy;
    yz = Nyz;
    xo = Nxo;
    xx = Nxx;
    xy = Nxy;
    xz = Nxz;
  }
  /** Multiply this matrix by a second: M = M*R */
  void
    mult(Matrix3D rhs)
  {
    double lxx = xx * rhs.xx + yx * rhs.xy + zx * rhs.xz;
    double lxy = xy * rhs.xx + yy * rhs.xy + zy * rhs.xz;
    double lxz = xz * rhs.xx + yz * rhs.xy + zz * rhs.xz;
    double lxo = xo * rhs.xx + yo * rhs.xy + zo * rhs.xz + rhs.xo;

    double lyx = xx * rhs.yx + yx * rhs.yy + zx * rhs.yz;
    double lyy = xy * rhs.yx + yy * rhs.yy + zy * rhs.yz;
    double lyz = xz * rhs.yx + yz * rhs.yy + zz * rhs.yz;
    double lyo = xo * rhs.yx + yo * rhs.yy + zo * rhs.yz + rhs.yo;

    double lzx = xx * rhs.zx + yx * rhs.zy + zx * rhs.zz;
    double lzy = xy * rhs.zx + yy * rhs.zy + zy * rhs.zz;
    double lzz = xz * rhs.zx + yz * rhs.zy + zz * rhs.zz;
    double lzo = xo * rhs.zx + yo * rhs.zy + zo * rhs.zz + rhs.zo;

    xx = lxx;
    xy = lxy;
    xz = lxz;
    xo = lxo;

    yx = lyx;
    yy = lyy;
    yz = lyz;
    yo = lyo;

    zx = lzx;
    zy = lzy;
    zz = lzz;
    zo = lzo;
  }

  /** Reinitialize to the unit matrix */
  void
    unit()
  {
    xo = 0.0;
    xx = 1.0;
    xy = 0.0;
    xz = 0.0;
    yo = 0.0;
    yx = 0.0;
    yy = 1.0;
    yz = 0.0;
    zo = 0.0;
    zx = 0.0;
    zy = 0.0;
    zz = 1.0;
  }
  /** Transform nvert points from v into tv.  v contains the input
      coordinates in doubleing point.  Three successive entries in
      the array constitute a point.  tv ends up holding the transformed
      points as integers; three successive entries per point */

  Point3D *
    transform(Point3D *v, Point3D *mid);
};

#define SCALE(da) ((da)*d->scale)

class Point3D : public Primitive3D
{
public:
  Point3D(double x, double y, double z) : x(x), y(y), z(z) {}

  // Point3D(Point3D *pt):
  // x(pt->x),y(pt->y),z(pt->z)
  //{}

  Point3D() : x(0), y(0), z(0) {}

  Point3D(Point3D *A) : x(A->x), y(A->y), z(A->z) {}

  void
    draw(Display *d, Point3D *center)
  {
    d->drawLine(center->x + SCALE(tx) - 2, center->y + SCALE(ty) - 2, center->x + SCALE(tx) + 2, center->y + SCALE(ty) + 2);
    d->drawLine(center->x + SCALE(tx) - 2, center->y + SCALE(ty) + 2, center->x + SCALE(tx) + 2, center->y + SCALE(ty) - 2);
  }

  void
    fill(Display *d, Point3D *center)
  {
    d->drawLine(center->x + SCALE(tx) - 2, center->y + SCALE(ty) - 2, center->x + SCALE(tx) + 2, center->y + SCALE(ty) + 2);
    d->drawLine(center->x + SCALE(tx) - 2, center->y + SCALE(ty) + 2, center->x + SCALE(tx) + 2, center->y + SCALE(ty) - 2);
  }

  void
    Transform(Matrix3D *mat, Point3D *mid, Point3D *center, double scalefactor)
  {
    Point3D *res = mat->transform(this, mid);
    mat->scale(scalefactor);

    tx = res->x + center->x;
    ty = res->y + center->y;

    delete (res);

    // TRACE("%f %f %f %f %f\n",x,y,z,tx,ty);
  }

  BBox3D *
    findBoundingBox()
  {
    BBox3D *bbox = new BBox3D();

    bbox->zmin = z;
    bbox->zmax = z;
    bbox->xmin = x;
    bbox->xmax = x;
    bbox->ymin = y;
    bbox->ymax = y;
    return bbox;
  }

  double x;
  double y;
  double z;
  double tx;
  double ty;
};

class Text3D : public Primitive3D
{
public:
  char *text;

  virtual ~Text3D()
  {
    if (text) free(text);
  }

  void
    setText(char *str)
  {
    text = _strdup(str);
  }

  Text3D(Point3D *A) : B(A) {}

  void
    draw(Display *d, Point3D *center)
  {
    d->drawText(center->x + B->tx, center->y + B->ty, text);
  }

  void
    fill(Display *d, Point3D *center)
  {
    d->drawText(center->x + B->tx, center->y + B->ty, text);
  }

  void
    Transform(Matrix3D *mat, Point3D *mid, Point3D *center, double scalefactor)
  {
    B->Transform(mat, mid, center, scalefactor);
  }

  Point3D *B;
};

class Line3D : public Primitive3D
{
public:
  Line3D(Point3D *A, Point3D *B) : A(A), B(B) {}

  virtual ~Line3D()
  {
    delete A;
    delete B;
  }

  void
    draw(Display *d, Point3D *center)
  {
    d->drawLine(center->x + A->tx, center->y + A->ty, center->x + B->tx, center->y + B->ty);
  }

  void
    fill(Display *d, Point3D *center)
  {
    d->drawLine(center->x + A->tx, center->y + A->ty, center->x + B->tx, center->y + B->ty);
  }

  void
    Transform(Matrix3D *mat, Point3D *mid, Point3D *center, double scalefactor)
  {
    A->Transform(mat, mid, center, scalefactor);
    B->Transform(mat, mid, center, scalefactor);
  }

  Point3D *A;
  Point3D *B;
};

class Polygon3D : public Primitive3D
{
public:
  Vector *pts;
  int color;

  Polygon3D(int color = -1) : color(color)
  {
    pts = new Vector("Polygon3D", __LINE__);
  }

  virtual ~Polygon3D() { delete pts; }

  void
    addPoint(Point3D *pt)
  {
    pts->addElement(pt);
  }

  void
    addPoint(double x, double y, double z)
  {
    Point3D *pt = new Point3D(x, y, z);
    pts->addElement(pt);
  }

  void
    draw(Display *d, Point3D *center)
  {
    for (int i = 0; i < pts->size(); i++) {
      Point3D *B = (Point3D *)pts->elementAt(0);
      Point3D *A = (Point3D *)pts->elementAt(i);
      if (i < pts->size() - 1) {
        B = (Point3D *)pts->elementAt(i + 1);
      }
      d->drawLine(center->x + SCALE(A->tx), center->y + SCALE(A->ty), center->x + SCALE(B->tx), center->y + SCALE(B->ty));
    }
  }

  void
    fill(Display *d, Point3D *center)
  {
    int size = pts->size();

    LPPOINT points = (LPPOINT)malloc(size * sizeof(POINT));

    for (int i = 0; i < size; i++) {
      Point3D *A = (Point3D *)pts->elementAt(i);

      points[i].x = (long)(center->x + SCALE(A->tx));
      points[i].y = (long)(center->y + SCALE(A->ty));
    }

    d->setColor(color);
    d->fillPolygon(points, size);

    free(points);
  }

  void
    Transform(Matrix3D *mat, Point3D *mid, Point3D *center, double scalefactor)
  {
    for (int i = 0; i < pts->size(); i++) {
      Point3D *model = (Point3D *)pts->elementAt(i);
      model->Transform(mat, mid, center, scalefactor);
    }
  }

  BBox3D *
    findBoundingBox()
  {
    BBox3D *bbox = new BBox3D();

    for (int i = 0; i < pts->size(); i++) {
      Point3D *model = (Point3D *)pts->elementAt(i);
      bbox->adjustWith(model->x, model->y, model->z);
    }
    return bbox;
  }
};

class Model3D : public Primitive3D
{
public:
  Point3D *origin;
  Vector *primitives;

  Model3D()
  {
    origin = new Point3D(0, 0, 0);
    primitives = new Vector("Model3D", __LINE__);
  }

  virtual ~Model3D()
  {
    delete primitives;
    delete origin;
  }

  void
    addPrimitive(Primitive3D *prim)
  {
    primitives->addElement(prim);
  }

  void
    draw(Display *d, Point3D *center)
  {
    for (int i = 0; i < primitives->size(); i++) {
      Primitive3D *model = (Primitive3D *)primitives->elementAt(i);
      model->draw(d, center);
    }
  }

  void
    fill(Display *d, Point3D *center)
  {
    for (int i = 0; i < primitives->size(); i++) {
      Primitive3D *model = (Primitive3D *)primitives->elementAt(i);
      model->fill(d, center);
    }
  }

  void
    Transform(Matrix3D *mat, Point3D *mid, Point3D *center, double scalefactor)
  {
    for (int i = 0; i < primitives->size(); i++) {
      Primitive3D *model = (Primitive3D *)primitives->elementAt(i);
      model->Transform(mat, mid, center, scalefactor);
    }
  }

  BBox3D *
    findBoundingBox()
  {
    BBox3D *bbox = new BBox3D();

    for (int i = 0; i < primitives->size(); i++) {
      Primitive3D *model = (Primitive3D *)primitives->elementAt(i);
      BBox3D *box = model->findBoundingBox();

      if (box) {
        bbox->adjustWith(box);
        delete box;
      }
    }
    return bbox;
  }

  Model3D *
    findPointsBox()
  {
    Model3D *box = new Model3D();

    BBox3D *worldExtent = findBoundingBox();

    Point3D *A =
      new Point3D(worldExtent->xmin, worldExtent->ymin, worldExtent->zmin);
    Point3D *B =
      new Point3D(worldExtent->xmax, worldExtent->ymin, worldExtent->zmin);
    Point3D *C =
      new Point3D(worldExtent->xmax, worldExtent->ymax, worldExtent->zmin);
    Point3D *D =
      new Point3D(worldExtent->xmin, worldExtent->ymax, worldExtent->zmin);

    Point3D *E =
      new Point3D(worldExtent->xmin, worldExtent->ymin, worldExtent->zmax);
    Point3D *F =
      new Point3D(worldExtent->xmax, worldExtent->ymin, worldExtent->zmax);
    Point3D *G =
      new Point3D(worldExtent->xmax, worldExtent->ymax, worldExtent->zmax);
    Point3D *H =
      new Point3D(worldExtent->xmin, worldExtent->ymax, worldExtent->zmax);

    box->addPrimitive(A);
    box->addPrimitive(B);
    box->addPrimitive(C);
    box->addPrimitive(D);

    box->addPrimitive(E);
    box->addPrimitive(F);
    box->addPrimitive(G);
    box->addPrimitive(H);

    delete worldExtent;

    return box;
  }
};

class WorldView3D
{
public:
  Vector *models;

  WorldView3D() { models = new Vector("WorldView3D", __LINE__); }

  virtual ~WorldView3D() { delete models; }

  void
    addModel(Model3D *model)
  {
    models->addElement(model);
  }

  void
    Transform(Matrix3D *mat, Point3D *mid, Point3D *center, double scalefactor)
  {
    for (int i = 0; i < models->size(); i++) {
      Model3D *model = (Model3D *)models->elementAt(i);
      model->Transform(mat, mid, center, scalefactor);
    }
  }

  void
    draw(Display *d, Point3D *center)
  {
    for (int i = 0; i < models->size(); i++) {
      Model3D *model = (Model3D *)models->elementAt(i);
      model->draw(d, center);
    }
  }

  void
    fill(Display *d, Point3D *center)
  {
    for (int i = 0; i < models->size(); i++) {
      Model3D *model = (Model3D *)models->elementAt(i);
      model->fill(d, center);
    }
  }

  BBox3D *
    findBoundingBox()
  {
    BBox3D *bbox = new BBox3D();

    for (int i = 0; i < models->size(); i++) {
      Model3D *model = (Model3D *)models->elementAt(i);
      BBox3D *box = model->findBoundingBox();

      bbox->adjustWith(box);

      delete box;
    }
    return bbox;
  }

  Model3D *
    findBox()
  {
    Model3D *box = new Model3D();

    BBox3D *worldExtent = findBoundingBox();

    Point3D *A =
      new Point3D(worldExtent->xmin, worldExtent->ymin, worldExtent->zmin);
    Point3D *B =
      new Point3D(worldExtent->xmax, worldExtent->ymin, worldExtent->zmin);
    Point3D *C =
      new Point3D(worldExtent->xmax, worldExtent->ymax, worldExtent->zmin);
    Point3D *D =
      new Point3D(worldExtent->xmin, worldExtent->ymax, worldExtent->zmin);

    Point3D *E =
      new Point3D(worldExtent->xmin, worldExtent->ymin, worldExtent->zmax);
    Point3D *F =
      new Point3D(worldExtent->xmax, worldExtent->ymin, worldExtent->zmax);
    Point3D *G =
      new Point3D(worldExtent->xmax, worldExtent->ymax, worldExtent->zmax);
    Point3D *H =
      new Point3D(worldExtent->xmin, worldExtent->ymax, worldExtent->zmax);

    Polygon3D *bottom = new Polygon3D();
    Polygon3D *top = new Polygon3D();
    Polygon3D *side1 = new Polygon3D();
    Polygon3D *side2 = new Polygon3D();

    bottom->addPoint(A);
    bottom->addPoint(B);
    bottom->addPoint(C);
    bottom->addPoint(D);

    top->addPoint(E);
    top->addPoint(F);
    top->addPoint(G);
    top->addPoint(H);

    side1->addPoint(new Point3D(A));
    side1->addPoint(new Point3D(B));
    side1->addPoint(new Point3D(F));
    side1->addPoint(new Point3D(E));

    side2->addPoint(new Point3D(C));
    side2->addPoint(new Point3D(D));
    side2->addPoint(new Point3D(H));
    side2->addPoint(new Point3D(G));

    box->addPrimitive(bottom);
    box->addPrimitive(top);
    box->addPrimitive(side1);
    box->addPrimitive(side2);

    /*
    int xsize = (int)(worldExtent->xmax+worldExtent->xmin)/3;
    int ysize = (int)(worldExtent->ymax+worldExtent->ymin)/3;
    int zsize = (int)(worldExtent->zmax+worldExtent->zmin)/3;

    Point3D *cen = new Point3D(
                 (worldExtent->xmin+worldExtent->xmax)/2,
                 (worldExtent->ymin+worldExtent->ymax)/2,
                 (worldExtent->zmin+worldExtent->zmax)/2
                 );

    Point3D *zcen = new Point3D(
             (worldExtent->xmin+worldExtent->xmax)/2,
                 (worldExtent->ymin+worldExtent->ymax)/2,
                 ((worldExtent->zmin+worldExtent->zmax)/2)+zsize
                 );

    Point3D *ycen = new Point3D(
             (worldExtent->xmin+worldExtent->xmax)/2,
                 ((worldExtent->ymin+worldExtent->ymax)/2)+ysize,
                 (worldExtent->zmin+worldExtent->zmax)/2
                 );

    Point3D *xcen = new Point3D(
             ((worldExtent->xmin+worldExtent->xmax)/2)+xsize,
                 (worldExtent->ymin+worldExtent->ymax)/2,
                 (worldExtent->zmin+worldExtent->zmax)/2
                 );


    Line3D *zline = new Line3D(cen,zcen);
    Line3D *xline = new Line3D(new Point3D(cen),xcen);
    Line3D *yline = new Line3D(new Point3D(cen),ycen);

    box->addPrimitive(zline);
    box->addPrimitive(xline);
    box->addPrimitive(yline);
    */

    delete worldExtent;

    return box;
  }

  Point3D *
    findCenter()
  {
    BBox3D *worldExtent = findBoundingBox();

    Point3D *cen = new Point3D((worldExtent->xmin + worldExtent->xmax) / 2,
      (worldExtent->ymin + worldExtent->ymax) / 2,
      (worldExtent->zmin + worldExtent->zmax) / 2);

    delete worldExtent;

    return cen;
  }
};

#endif