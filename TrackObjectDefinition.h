
#ifndef _TRACK_OBJ_DEF_H
#define _TRACK_OBJ_DEF_H

#include "resource.h"
//#include "stdafx.h"
#include "Observer.h"
#include "Display.h"
#include "fstream.h"

class GPTrack;

class TrackObjectDefinition: public Observer, public CObject
{
    public:
		GPTrack *track;
		//HTREEITEM treeNode;

		TrackObjectDefinition(GPTrack *mytrack,int offset):
		  track(mytrack),
		  originaloffset(offset),
		  locator(0),
		  id(0),id2(0),detail(0),y(0),angleX(0),height(0),
		  angleY(0),Unk1(0),Unk2(0),
		  Observer(mytrack,t_ObjectDefinition,NULL)
	      {
			  selected = FALSE;
	      }

		  int getLocator()
		  {
			  return locator;
		  }

		  void setLocator(int loc)
		  {
              locator = loc;
		  }

		  int Open();
		  

		  int getDistance()
		  {
			  return y;
		  }

		  void setDistance(int ny)
		  {
			  y = ny;
		  }

		  int getId1()
		  {
			  return id;
		  }

		  int getId2()
		  {
			  return id2;
		  }

		  int getAngleX()
		  {
			  return angleX;
		  }

		  int getAngleY()
		  {
			  return angleY;
		  }

		  int getHeight()
		  {
			  return height;
		  }

		  int getUnk1()
		  {
			  return Unk1;
		  }

		  int getUnk2()
		  {
			  return Unk2;
		  }

		  int getDetail()
		  {
			  return detail;
		  }

		  void Delete();

		  int write(GPTrack *track,int offset);
		  //{
		  //  return offset;
		  //}

		  char * getName();
		  
		  int getDisplayIconType();

		  


		  



		int read(GPTrack *track,int offset,int ListObjectOffset);
		//int write(GPTrack *track,int offset,int ListObjectOffset);


		BOOL isSelected()
		{
			return selected;
		}

		void setSelected(BOOL sel)
		{
			selected = sel;
		}

		char *getItemName();
		
		void DrawVML(ofstream &,double x,double y,double z,double angle,BOOL drawAxis);
		void DrawGL(CDC *pDC,double x,double y,double z,double angle,BOOL drawAxis);
		void Draw2D(Display *g,double x,double y,double z,double angle,BOOL drawAxis);


	int locator;
	int id,id2;
	int detail;
	signed short y;     //distance from center of road
	int angleX;
	int angleY;
	int height;
	int Unk1;
	int Unk2;
	int originaloffset;
	BOOL selected;
};

#endif