
#include "stdafx.h"
#include "GPTrack.h"
#include "Point3D.h"
#include "TrackSection.h"

extern long GetPCDWord(FILE *fp);
extern long GetPCWord(FILE *fp);

typedef struct GP2SegStr
{
        WORD            angleH;
        WORD            angleV;
        short           xPos;
        short           yPos;                   // height!
        short           zPos;
        WORD            field_A;
        short           xSide;                  // b0-5=width,  b6-7=curbs
        short           ySide;                  // b0-5=unused, b6-7=curbs
        BYTE            unk010;
        BYTE            unk011;
        BYTE            flags_12;
        BYTE            field_13;
        WORD            field_14;
        short           bestLine;               // better known as the CC line
        BYTE            field_18;
        //BYTE            field_19;
        //WORD            nr;                             // except high 3 bits (which are flags)
        //BYTE            the_rest[0x50];
		WORD            segnr;          // except high 5 bits (which are flags)
		short           f5Line;         // +1Ch
		BYTE            field_1e;
        char            flags_1f;
        WORD            field_20;
        char            field_22;
        char            field_23;
        char            field_24;
        char            field_25;
        WORD            field_26;
        char            field_28;
        char            field_29;
        WORD            field_2a;

        // some flags
		DWORD           curbs_2c;               // b2=right curb, b3=left curb
        DWORD           dword_30;
        DWORD           dword_34;
        DWORD           dword_38;
        DWORD           dword_3C;
        DWORD           dword_40;

        struct GP2SegStr   *equalseg;

        WORD            flags_48;               // b2=low curb
        WORD            flags_4A;
        short           xSide_4c;         // slightly different at very few places!
        short           ySide_4e;
  
        short           field_50;
        short           field_52;
        short           field_54;
        BYTE            field_56;

        BYTE            field_57;
        BYTE            unk058[0x0a];
        WORD            width;                  // width of track * 4
        BYTE            unk064[0x08];
} GP2Seg;

static void drawPoint(Display *g,double x1,double y1)
{
	double pix  = (double)g->getObjectX(4);
	double pix1 = (double)g->getObjectX(3);

	g->drawEllipse(x1-pix1,y1-pix1,x1+pix1,y1+pix1);
	g->drawLine(x1,y1-pix,x1,y1+pix);
    g->drawLine(x1-pix,y1,x1+pix,y1);
}

void GPTrack::ImportTrackSegmentsDataLog(LPCSTR filename)
{
	FILE *fp = fopen(filename,"rb");

	if (fp==NULL)
	{
		AfxMessageBox("Cannot open Track Segment file");
		return;
	}

	CCDataLog = new Vector("CCDataLog",__LINE__);

	double xpos = TrackXBeginPos;
    double ypos = TrackYBeginPos;

	int count=0;

	while(!feof(fp))
	{
		GP2Seg segment;

		fread(&segment,1,sizeof(GP2Seg),fp);

		double x = (double)segment.xPos;
		double y = (double)segment.zPos;
		double bl = (double)(segment.bestLine >> 4);
		//double x1 = x+ ((int)segment.xSide >> 5);
		//double y1 = y+ ((int)segment.xSide >> 5);
		double angle = (double)segment.angleH;
		double angleDeg = angle/364.0;
		double angleRad = angleDeg*PI/180;

		double xbl = bl*sin(angleRad);
		double ybl = bl*cos(angleRad);

		double y1 = y+ybl;
		double x1 = x+xbl;

		x= x/128.0;
		y= y/128.0;

		//x = x / 262144.0;
		//y = y / 262144.0;

		count++;
		
		//Point3D *CCPt = new Point3D(xpos+(double)y,ypos+(double)x,0);
		//CCDataLog->addElement(CCPt);

		//double width = 0x20 & (double)segment.xSide;
		

		x1= x1/128.0;
		y1= y1/128.0;

		Point3D *CCPt = new Point3D(xpos+(double)y1,ypos+(double)x1,0);
		CCDataLog->addElement(CCPt);
		CCPt = new Point3D(xpos+(double)y,ypos+(double)x,0);
		CCDataLog->addElement(CCPt);
		
	}
	fclose(fp);
}

void GPTrack::ExportXYDataLog(LPCSTR filename)
{
	FILE *fp = fopen(filename,"w");

	if (fp==NULL)
	{
		AfxMessageBox("Cannot open Track XY file");
		return;
	}

	for(int i=0;i<TrackSections->size();i++)
	{
		TrackSection *t = (TrackSection*)TrackSections->elementAt(i);
		double xpos = t->getStartX();
		double ypos = t->getStartY();

		fprintf(fp,"%2.3f,%2.3f\n",xpos,ypos);
	}
	
	fclose(fp);
}

void GPTrack::ImportCCDataLog(LPCSTR filename)
{
	FILE *fp = fopen(filename,"rb");

	if (fp==NULL)
	{
		AfxMessageBox("Cannot open Data Log file");
		return;
	}

	CCDataLog = new Vector("CCDataLog",__LINE__);

	double xpos = TrackXBeginPos;
    double ypos = TrackYBeginPos;

	int count=0;

	while(!feof(fp))
	{
		double x = (double)GetPCDWord(fp);	
		double y   = (double)GetPCDWord(fp);
		//long x,y;
		//fscanf(fp,"%d,%d",&x,&y);

		x = x / 262144.0;
		y = y / 262144.0;

		//TRACE("%d %d\n",x,y);

		count++;
		
		Point3D *CCPt = new Point3D(xpos+(double)y,ypos+(double)x,0);
		CCDataLog->addElement(CCPt);
	}


	TRACE("read in %d data elements",count);


	fclose(fp);
}

void GPTrack::DrawCCDataLog(Display *g)
{
	if (!showCCDataLog) return;

	if (CCDataLog!=NULL)
	{
	 int size = CCDataLog->size();


	 for(int i=0;i<size;i++)
	 {
		Point3D *pt = (Point3D*)CCDataLog->elementAt(i);
		Point3D *ptB;
		
		if (i<size-1)
		{
		   ptB = (Point3D*)CCDataLog->elementAt(i+1);	
		}
		else
		{
		   ptB = (Point3D*)CCDataLog->elementAt(0);
		}
		g->drawLine(pt->x,pt->y,ptB->x,ptB->y);
		drawPoint(g,pt->x,pt->y);
	 }
	}
}