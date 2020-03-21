
#include "stdafx.h"
#include "math.h"

class Point3D
{
    public:

	Point3D(double x,double y,double z):
    x(x),y(y),z(z)
	{

	}

	double x;
	double y;
	double z;
};


class Square3D
{
    public:

	Square3D(Point3D a,Point3D b,Point3D c,Point3D d)
	{
	}

	Point3D *a;
	Point3D *b;
	Point3D *c;
	Point3D *d;
};

class zMatrix3D 
{
    double xx, xy, xz, xo;
    double yx, yy, yz, yo;
    double zx, zy, zz, zo;

    double pi;

    /** Create a new unit matrix */
     zMatrix3D () 
	 {
		 pi = 3.14159265;
	     xx = 1.0;
	     yy = 1.0;
	     zz = 1.0;
    }
    /** Scale by f in all dimensions */
     void scale(double f) {
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
     void scale(double xf, double yf, double zf) {
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
     void translate(double x, double y, double z) 
	{
	xo += x;
	yo += y;
	zo += z;
    }

    /** rotate theta degrees about the y axis */
    void yrot(double theta) {
	theta *= (pi / 180);
	double ct = cos(theta);
	double st = sin(theta);

	double Nxx = (double) (xx * ct + zx * st);
	double Nxy = (double) (xy * ct + zy * st);
	double Nxz = (double) (xz * ct + zz * st);
	double Nxo = (double) (xo * ct + zo * st);

	double Nzx = (double) (zx * ct - xx * st);
	double Nzy = (double) (zy * ct - xy * st);
	double Nzz = (double) (zz * ct - xz * st);
	double Nzo = (double) (zo * ct - xo * st);

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
     void xrot(double theta) {
	theta *= (pi / 180);
	double ct = cos(theta);
	double st = sin(theta);

	double Nyx = (double) (yx * ct + zx * st);
	double Nyy = (double) (yy * ct + zy * st);
	double Nyz = (double) (yz * ct + zz * st);
	double Nyo = (double) (yo * ct + zo * st);

	double Nzx = (double) (zx * ct - yx * st);
	double Nzy = (double) (zy * ct - yy * st);
	double Nzz = (double) (zz * ct - yz * st);
	double Nzo = (double) (zo * ct - yo * st);

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
     void zrot(double theta) {
	theta *= (pi / 180);
	double ct = cos(theta);
	double st = sin(theta);

	double Nyx = (double) (yx * ct + xx * st);
	double Nyy = (double) (yy * ct + xy * st);
	double Nyz = (double) (yz * ct + xz * st);
	double Nyo = (double) (yo * ct + xo * st);

	double Nxx = (double) (xx * ct - yx * st);
	double Nxy = (double) (xy * ct - yy * st);
	double Nxz = (double) (xz * ct - yz * st);
	double Nxo = (double) (xo * ct - yo * st);

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
    void mult(zMatrix3D rhs) 
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
    void unit() {
	xo = 0;
	xx = 1;
	xy = 0;
	xz = 0;
	yo = 0;
	yx = 0;
	yy = 1;
	yz = 0;
	zo = 0;
	zx = 0;
	zy = 0;
	zz = 1;
    }


    void transform(Point3D v, Point3D tv, Point3D mid) 
	{
		double lxx = xx, lxy = xy, lxz = xz, lxo = xo;
		double lyx = yx, lyy = yy, lyz = yz, lyo = yo;
		double lzx = zx, lzy = zy, lzz = zz, lzo = zo;
		double x = v.x-mid.x;
		double y = v.y-mid.y;
		double z = v.z-mid.z;
		
		tv.x = (double) (x * lxx + y * lxy + z * lxz + lxo);
		tv.y = (double) (x * lyx + y * lyy + z * lyz + lyo);
		tv.z = (double) (x * lzx + y * lzy + z * lzz + lzo);
		
    }

};
