#include "stdafx.h"
#include "Display.h"
#include "math.h"
#include "resource.h"


#define PI 3.141592695

Display::Display()
{
		   xoffset = 0;
		   yoffset = 0;
		   scale   = 1;
		   roadBrush    = new CBrush(0xC0C0C0);
		   drkroadBrush  = new CBrush(0x404040);
		   grassBrush   = new CBrush(0x00FF00);
		   tarmacBrush  = new CBrush(0x909090);
		   sandBrush    = new CBrush(0x00FFFF);
		   redBrush     = new CBrush(0x0000FF);
		   cyanBrush    = new CBrush(0xFFFF00);
		   ccselpen   = new CPen(PS_SOLID,2,0xFF0000);
		   //ccpen      = new CPen(PS_SOLID,2,0xFF00FF);
		   ccpen      = new CPen(PS_SOLID,1,0xFF00FF);
		   roadselpen =	new CPen(PS_SOLID,1,0x00FF00);
		   redpen     = new CPen(PS_SOLID,1,0x0000FF);
		   kerbpen    = new CPen(PS_DOT,1,0xFF0000);
		   kerblowerpen = new CPen(PS_DOT,1,0x0000FF);
		   grassPen   = new CPen(PS_SOLID,1,0x00FF00);
		   rulerpen   = new CPen(PS_DOT,1,RGB(0,0,0));
		   //gripPen    = new CPen(PS_SOLID,5,RGB(colorval,0,0));
		   gridpen    = new CPen(PS_SOLID,1,RGB(192,192,192));
		   bluePen    = new CPen(PS_SOLID,1,RGB(0x00,0x00,0xFF));
		   redPen    = new CPen(PS_SOLID,1,RGB(0xFF,0x00,0x00));

		   CBitmap bmpGrass,bmpSand,bmpTarmac,bmpDrkTarmac,bmpCrowd;
		   bmpGrass.LoadBitmap(IDB_GRASS_BMP);
		   bmpSand.LoadBitmap(IDB_TARMAC_BMP);
		   bmpTarmac.LoadBitmap(IDB_SAND_BMP);
		   bmpDrkTarmac.LoadBitmap(IDB_DRK_TARMAC_BMP);
		   bmpCrowd.LoadBitmap(IDB_CROWD_BMP);

		   grassBitmapBrush  = new CBrush(&bmpGrass);
		   sandBitmapBrush   = new CBrush(&bmpSand);
		   tarmacBitmapBrush = new CBrush(&bmpTarmac);
		   drkroadBitmapBrush = new CBrush(&bmpDrkTarmac);
		   crowdBitmapBrush   = new CBrush(&bmpCrowd);
		   printing=FALSE;
	   }
#define Deg2Rad(x) ((double)(x)*PI/180.0)

void Display::drawArc(double x,double y,double x1,double y1,double cx,double cy,double angle1,double angle2,double radius)
{
	//drawSpiral(cx,cy,Deg2Rad(angle1), Deg2Rad(angle1)+Deg2Rad(angle2)/2.0,radius,radius);

	int sx = getScreenX(x1);
	int sy = getScreenY(y1);
	int ex = getScreenX(x);
	int ey = getScreenY(y);
	double csx = getScreenX(cx);
	double csy = getScreenY(cy);
	double rad = (double)(radius*scale);

	int irad = (int) rad;

	if (irad == 0) return;

	if (sx == ex && sy == ey) return;
	if ((angle1+angle2) == angle1) return;

	
	int scx =  (int)(csx-rad);
	int scy =  (int)(csy-rad);
	int ecx =  (int)(csx+rad);
	int ecy =  (int)(csy+rad);

	if (scx == ecx) 
		return;

	if ((angle1+angle2) > angle1)
	{
	 gr->Arc(scx,scy,ecx,ecy,ex,ey,sx,sy);
	}
	else
	 gr->Arc(scx,scy,ecx,ecy,sx,sy,ex,ey);

}

void Display::drawEllipse(double x,double y,double x1,double y1)
{
	int sx = getScreenX(x1);
	int sy = getScreenY(y1);
	int ex = getScreenX(x);
	int ey = getScreenY(y);
	
	gr->Ellipse(sx,sy,ex,ey);
}

void Display::drawRect(double x,double y,double x1,double y1)
{
	int sx = getScreenX(x1);
	int sy = getScreenY(y1);
	int ex = getScreenX(x);
	int ey = getScreenY(y);
	
	gr->Rectangle(sx,sy,ex,ey);
}

void Display::drawBitmap(int id,double x,double y)
{
	int sx = getScreenX(x);
	int sy = getScreenY(y);
	CBitmap bmp, *poldbmp;
    CDC memdc;

	// Load the bitmap resource
    bmp.LoadBitmap( id );
 
    // Create a compatible memory DC
    memdc.CreateCompatibleDC( gr );
 
    // Select the bitmap into the DC
    poldbmp = memdc.SelectObject( &bmp );
 
	if (isPrinting())
	{
	int w = (getScreenX(x+16)-getScreenX(x));
	int h = (getScreenX(y)-getScreenX(y+15));
	w = (int)((double)w*0.75);
	h = (int)((double)h*0.75);
	//int w = 1600;
	//int h = 1500;
	gr->StretchBlt( (int)sx, (int)sy, w, -h, &memdc, 0, 0, 16,15,SRCAND );
	}
	else
	{
	// Copy (BitBlt) bitmap from memory DC to screen DC
    gr->BitBlt( (int)sx, (int)sy, 16, 15, &memdc, 0, 0, SRCAND );
	}
 
    memdc.SelectObject( poldbmp );
	memdc.DeleteDC();
}

void Display::drawPie(double x,double y,double x1,double y1,double cx,double cy,double angle1,double angle2,double radius)
{
	int sx = getScreenX(x1);
	int sy = getScreenY(y1);
	int ex = getScreenX(x);
	int ey = getScreenY(y);
	int csx = getScreenX(cx);
	int csy = getScreenY(cy);
	int rad = (int)(radius*scale);

	if (sx == ex && sy == ey) return;
	
	if ((angle1+angle2) > angle1)
	{
	 gr->Pie(csx-rad,csy-rad,csx+(rad),csy+(rad),ex,ey,sx,sy);
	}
	else
	 gr->Pie(csx-rad,csy-rad,csx+(rad),csy+(rad),sx,sy,ex,ey);

}

void Display::drawChord(double x,double y,double x1,double y1,double cx,double cy,double angle1,double angle2,double radius)
{
	int sx = getScreenX(x1);
	int sy = getScreenY(y1);
	int ex = getScreenX(x);
	int ey = getScreenY(y);
	int csx = getScreenX(cx);
	int csy = getScreenY(cy);
	int rad = (int)(radius*scale);

	if (sx == ex && sy == ey) return;
	
	if ((angle1+angle2) > angle1)
	{
	 gr->Chord(csx-rad,csy-rad,csx+(rad),csy+(rad),ex,ey,sx,sy);
	}
	else
	 gr->Chord(csx-rad,csy-rad,csx+(rad),csy+(rad),sx,sy,ex,ey);

}

void Display::drawArcSpecial(double x,double y,double x1,double y1,double cx,double cy,double angle1,double angle2,double radius1,double radius,BOOL square)
{
	int sx = getScreenX(x1);
	int sy = getScreenY(y1);
	int ex = getScreenX(x);
	int ey = getScreenY(y);
	int csx = getScreenX(cx);
	int csy = getScreenY(cy);

	double rad = (double)(radius*scale);
	double r2  = (double)(radius1*scale);
	double r1  = rad;

	if (sx == ex && sy == ey) return;

	CPoint center = gr->GetViewportOrg();

	int csxr = csx + center.x;
	int csyr = csy + center.y;

	HRGN rg1 = ::CreateEllipticRgn((int)(csxr-r1), (int)(csyr-r1),(int)(csxr+r1),(int)(csyr+r1));
    HRGN rg2 = ::CreateEllipticRgn((int)(csxr-r2), (int)(csyr-r2), (int)(csxr+r2) ,(int)(csyr+r2));
	HRGN rg3 = ::CreateRectRgn((int)(csxr-r1),(int)(csyr-r1),(int)(csxr+r1),(int)(csyr+r1));

	if (square)
	{
	 //setColor(-1);
	 //gr->Rectangle(csxr-r2, csyr-r2,csxr+r2,csyr+r2);
	 //gr->Rectangle(csxr-r1, csyr-r1,csxr+r1,csyr+r1);
	}
	
	int res = ::CombineRgn(rg3,rg1,rg2,RGN_DIFF);

	::SelectClipRgn(gr->m_hDC,rg3);

	if ((angle1+angle2) > angle1)
	{
	 gr->Pie((int)(csx-rad),(int)(csy-rad),(int)(csx+rad),(int)(csy+rad),ex,ey,sx,sy);
	 ::SelectClipRgn(gr->m_hDC,NULL);
	 gr->Arc((int)(csx-rad),(int)(csy-rad),(int)(csx+rad),(int)(csy+rad),ex,ey,sx,sy);
	}
	else
	{
	 gr->Pie((int)(csx-rad),(int)(csy-rad),(int)(csx+rad),(int)(csy+rad),sx,sy,ex,ey);
	 ::SelectClipRgn(gr->m_hDC,NULL);
	 gr->Arc((int)(csx-rad),(int)(csy-rad),(int)(csx+rad),(int)(csy+rad),sx,sy,ex,ey);
	}


	

	

	DeleteObject(rg1);
	DeleteObject(rg2);
	DeleteObject(rg3);
	
}


/*
void Display::drawArcSpecial(double x,double y,double x1,double y1,double cx,double cy,double angle1,double angle2,double radius)
{
	int sx  = getScreenX(x1);
	int sy  = getScreenY(y1);
	int ex  = getScreenX(x);
	int ey  = getScreenY(y);
	int csx = getScreenX(cx);
	int csy = getScreenY(cy);
	int rad = (int)(radius*scale);
	

	double sdx = x1-cx;
	double sdy = y1-cy;
	double edx = x-cx;
	double edy = y-cy;

	angle1 = (180/PI)*atan2(sdy,sdx);
	angle2 = (180/PI)*atan2(edy,edx);

	double right = cx + abs(radius);
	double left = cx - abs(radius);
	double top = cy - abs(radius);
	double bottom = cy + abs(radius);

	double xcenter = (x + x1) / 2;
    double ycenter = (y + y1) / 2;
	double xstart  = x;
	double ystart  = y;
	double xend = x1;
	double yend = y1;

    angle1 = atan2( (double)(ycenter-ystart)*(right-left),
                             (double)(xstart-xcenter)*(bottom-top) );
    angle2 = atan2( (double)(ycenter-yend)*(right-left),
                             (double)(xend-xcenter)*(bottom-top) );

	while (angle2 < angle1) {
		angle2 += (2.0*PI);
	}

	double stAngle   = angle1;
	double diffAngle = (angle2 - angle1);
	double ndiff = diffAngle/2.0;
    double nx = cx+radius*cos(stAngle+ndiff);
	double ny = cy-radius*sin(stAngle+ndiff);
	drawLine(x,y,nx,ny);
	drawLine(nx,ny,x1,y1);
}
*/


void Display::setColor(int col)
{
		   
		   switch(col)
		   {
		      case -1: gr->SelectStockObject(NULL_BRUSH);break;
		      case 0: gr->SelectStockObject(BLACK_PEN);break;
		      case 1: gr->SelectStockObject(WHITE_PEN);break;
			  case -2: gr->SelectStockObject(BLACK_PEN);break;
		      
			  case 2: gr->SelectObject(ccpen);break;
			  case 3: gr->SelectObject(roadselpen);break;
		      case 4: gr->SelectObject(ccselpen);break;
		      case 5: gr->SelectObject(redpen);break;
			  case KERB_LOWER_PEN: gr->SelectObject(kerblowerpen);break;
			  case KERB_PEN: gr->SelectObject(kerbpen);break;
			  case GRASS_BRUSH: 
				  {
					  gr->SelectObject(grassBrush);
					  gr->SelectObject(grassPen);
					  break;
				  }
			  case SAND_BRUSH: gr->SelectObject(sandBrush);break;
			  case ROAD_BRUSH: 
				  {
					  gr->SelectObject(roadBrush);
					  gr->SelectStockObject(BLACK_PEN);
				  }break;
		      case RED_BRUSH: gr->SelectObject(redBrush);break;
			  case CYAN_BRUSH: gr->SelectObject(cyanBrush);break;
		      case TARMAC_BRUSH: 
				  {
					  gr->SelectObject(tarmacBrush);
					  gr->SelectStockObject(BLACK_PEN);
					  break;
				  }
			  case RULER_PEN:
				  {
					  gr->SelectObject(rulerpen);
				  }break;
			  //case GRIP_PEN:
			//	  {
			//		  gr->SelectObject(gripPen);
			//	  }break;
			  case GRID_PEN:
				  {
					  gr->SelectObject(gridpen);
				  }break;
			  case GRASS_BITMAP_BRUSH:
				  gr->SelectObject(grassBitmapBrush);
				  break;
			  case SAND_BITMAP_BRUSH:
				  gr->SelectObject(sandBitmapBrush);
				  break;
			  case ROAD_BITMAP_BRUSH:
				  gr->SelectObject(tarmacBitmapBrush);
				  break;
			  case DRK_ROAD_BRUSH:
				  gr->SelectObject(drkroadBrush);
				  break;
			  case DRK_ROAD_BITMAP_BRUSH:
				  gr->SelectObject(drkroadBitmapBrush);
				  break; 
			  case CROWD_BITMAP_BRUSH:
				  gr->SelectObject(crowdBitmapBrush);
				  break;
			  case BLUE_PEN:
				  gr->SelectObject(bluePen);
				  break;
			  case RED_PEN:
				  gr->SelectObject(redPen);
				  break;
		   }
		   

}

void sprialDrawPoint(HDC hdc,double x,double y)
{
	MoveToEx(hdc,x-5,y-5,NULL);
	LineTo(hdc,x+5,y+5);
	MoveToEx(hdc,x-5,y+5,NULL);
	LineTo(hdc,x+5,y-5);
}

void sprialDrawLine(HDC hdc,double x,double y,double x1,double y1)
{
	MoveToEx(hdc,x,y,NULL);
	LineTo(hdc,x1,y1);
}

//#define PI 3.14159265

#define DEG_TO_RAD(x) (((double)(x)*PI)/180.0)

void Display::drawSpiral(double cx,double cy,double angle1,double angle2,double r1,double r2)
{
	double sA = angle1-(PI/2);
	double eB = angle2-(PI/2);
	double de = eB-sA;
	double eA = sA + 2.0*(de);

	double sX = cx + (r1*cos(sA));
	double sY = cy + (r1*sin(sA));

	double eX = cx + (r2*cos(eA));
	double eY = cy + (r2*sin(eA));
    double count = 20.0;
	double deltaAngle = (eA - sA);
	double dA = deltaAngle/count;
	double deltaRadius = (r2-r1);
	double dr = deltaRadius/count;
	double otX,otY;

	for(int i=0;i<=count;i++)
	{
		double ndA = (double)i*dA;
		double ndr = (double)i*dr;
		double tX = cx + ((r1+ndr)*cos(sA+ndA));
		double tY = cy + ((r1+ndr)*sin(sA+ndA));

		if (i>0){
		sprialDrawLine(gr->m_hDC,getScreenX(otX),getScreenY(otY),getScreenX(tX),getScreenY(tY));
		}

		otX = tX;
		otY = tY;
	}
	
}

