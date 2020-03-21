
#include "stdafx.h"

#include "Point3D.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


Point3D *Matrix3D::transform(Point3D *v, Point3D *mid) 
	{
		Point3D *tv = new Point3D();
		double lxx = xx, lxy = xy, lxz = xz, lxo = xo;
		double lyx = yx, lyy = yy, lyz = yz, lyo = yo;
		double lzx = zx, lzy = zy, lzz = zz, lzo = zo;
		double x = v->x-mid->x;
		double y = v->y-mid->y;
		double z = v->z-mid->z;
		
		tv->x = (x * lxx + y * lxy + z * lxz + lxo);
		tv->y = (x * lyx + y * lyy + z * lyz + lyo);
		tv->z = (x * lzx + y * lzy + z * lzz + lzo);
		return tv;
    }
