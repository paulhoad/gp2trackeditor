// TrackOpenGLView.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "TrackOpenGLView.h"
#include "TrackEditor.h"
#include "TrackEditorDoc.h"
#include "GPTrack.h"
#include "TrackSection.h"
#include "ViewSections.h"
#include "Vector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include <gl\glaux.h>
#include <gl\gl.h>
#include <gl\glu.h>

#define KDIFF 0.01
#define KWIDTH 150

#define KERB_1_WIDTH 100
#define KERB_2_WIDTH 500
#define KERB_1_HEIGHT 0.05
#define KERB_2_HEIGHT 0.1

/////////////////////////////////////////////////////////////////////////////
// CTrackOpenGLView

extern void Mak256SquareImage(CBitmap &jamBitmap,CBitmap &currentImg,int nWidth,int nHeight,int hr,int vr,int ow,int oh);
extern CPoint GetTrackPoint(double currrad,double angle,double length);

double car_height = 0.1;

int from;
int to;

IMPLEMENT_DYNCREATE(CTrackOpenGLView, COpenGLView)

CTrackOpenGLView::CTrackOpenGLView()
{
   currentSectorNumber = 0;
   currentView = t_Air;
   positionIntoSector = 0;
   currentSector = NULL;
   inited = FALSE;
   fov = 30;

   m_xRotate = 110;
   m_yRotate = 3;
   m_zRotate = -151;

   RoadBits=NULL;
   VergeLeftBits=NULL;
   VergeRightBits=NULL;
   FenceLeftBits=NULL;
   FenceRightBits=NULL;
   Kerb1Bits=NULL;
   Kerb2Bits=NULL;
}

CTrackOpenGLView::~CTrackOpenGLView()
{
   if (RoadBits)       delete RoadBits;
   if (VergeLeftBits)  delete VergeLeftBits;
   if (VergeRightBits) delete VergeRightBits;
   if (FenceLeftBits)  delete FenceLeftBits;
   if (FenceRightBits) delete FenceRightBits;
   if (Kerb1Bits)      delete Kerb1Bits;
   if (Kerb2Bits)      delete Kerb2Bits;
}


BEGIN_MESSAGE_MAP(CTrackOpenGLView, COpenGLView)
	//{{AFX_MSG_MAP(CTrackOpenGLView)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_AIRVIEW, OnAirview)
	ON_UPDATE_COMMAND_UI(ID_AIRVIEW, OnUpdateAirview)
	ON_COMMAND(ID_VIEW_COCKPIT, OnViewCockpit)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COCKPIT, OnUpdateViewCockpit)
	ON_COMMAND(ID_VIEWCAMERAS, OnViewcameras)
	ON_UPDATE_COMMAND_UI(ID_VIEWCAMERAS, OnUpdateViewcameras)
	ON_COMMAND(ID_DRAWAXIS, OnDrawaxis)
	ON_UPDATE_COMMAND_UI(ID_DRAWAXIS, OnUpdateDrawaxis)
	ON_COMMAND(ID_SHOWLINES, OnShowlines)
	ON_UPDATE_COMMAND_UI(ID_SHOWLINES, OnUpdateShowlines)
	ON_COMMAND(ID_SHOWCULLING, OnShowculling)
	ON_UPDATE_COMMAND_UI(ID_SHOWCULLING, OnUpdateShowculling)
	ON_COMMAND(ID_SHOW_SCENERY, OnShowScenery)
	ON_UPDATE_COMMAND_UI(ID_SHOW_SCENERY, OnUpdateShowScenery)
	ON_COMMAND(ID_REFRESH_TREE, OnRefreshTree)
	ON_COMMAND(ID_SHOW_TEXTURES, OnShowTextures)
	ON_UPDATE_COMMAND_UI(ID_SHOW_TEXTURES, OnUpdateShowTextures)
	ON_COMMAND(ID_TEXTUREMAPOBJECTS, OnTexturemapobjects)
	ON_UPDATE_COMMAND_UI(ID_TEXTUREMAPOBJECTS, OnUpdateTexturemapobjects)
	ON_COMMAND(ID_SHOWTREES, OnShowtrees)
	ON_UPDATE_COMMAND_UI(ID_SHOWTREES, OnUpdateShowtrees)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackOpenGLView drawing

void CTrackOpenGLView::OnInitialUpdate()
{
	COpenGLView::OnInitialUpdate();

	


	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
   GPTrack * mytrack = pDoc->getTrack();
   int size = mytrack->TrackSections->size();
   //mytrack->showTextures = FALSE;
    to = size;
    from = 0;
   for(int i=0;i<mytrack->InternalObjectOffsets->size();i++)
   {
	   InternalObject *obj = (InternalObject*)mytrack->InternalObjectOffsets->elementAt(i);
	   obj->UpdateStructure();
   }

   int 	DefaultKerb1Id = 161;
   int 	DefaultKerb2Id = 162;
   int  width,height;

   LoadJamTexture(mytrack,RoadBmp,mytrack->DefaultRoadId,256,256,&width,&height);
   LoadJamTexture(mytrack,VergeLeftBmp,mytrack->DefaultLeftVergeId,256,256,&width,&height);
   LoadJamTexture(mytrack,VergeRightBmp,mytrack->DefaultRightVergeId,256,256,&width,&height);
   LoadJamTexture(mytrack,FenceLeftBmp,mytrack->DefaultLeftFenceId,256,256,&width,&height);
   LoadJamTexture(mytrack,FenceRightBmp,mytrack->DefaultRightFenceId,256,256,&width,&height);
   LoadJamTexture(mytrack,Kerb1Bmp,DefaultKerb1Id,256,256,&width,&height);
   LoadJamTexture(mytrack,Kerb2Bmp,DefaultKerb2Id,256,256,&width,&height);

   InitDisplayList(NULL);
}

//void CTrackOpenGLView::OnDraw(CDC* pDC)
//{
//	
//
//	
//}

/////////////////////////////////////////////////////////////////////////////
// CTrackOpenGLView diagnostics

#ifdef _DEBUG
void CTrackOpenGLView::AssertValid() const
{
	COpenGLView::AssertValid();
}

void CTrackOpenGLView::Dump(CDumpContext& dc) const
{
	COpenGLView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTrackOpenGLView message handlers

#define GET_GL_PT_X(x) (((x)-MidX)/SC)
#define GET_GL_PT_Y(y) (((y)-MidY)/SC)
#define GET_GL_PT_Z(z) (((z)-MidZ)/SC)

void CTrackOpenGLView::getLookPoint()
{
   if (currentView == t_Air)
   {
   glTranslated(0.0,0.0,-dist);
   glRotated(m_xRotate, 1.0, 0.0, 0.0);
   glRotated(m_yRotate, 0.0, 1.0, 0.0);
   glRotated(m_zRotate, 0.0, 0.0, 1.0);
   }
   else if (currentView == t_Cockpit)
   {
   CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
   GPTrack * mytrack = pDoc->getTrack();

   if (currentSector==NULL)
   {
	   currentSector = (TrackSection*)mytrack->TrackSections->elementAt(currentSectorNumber);
   }
   // only draw a small area ahead
   from = currentSectorNumber-1;
   to   = currentSectorNumber+10;
   int  size = mytrack->TrackSections->size();
   if (from < 0)   from = 0;
   if (to >= size) to = (to-size);

   TrackSection *t = currentSector;
   TrackSection *nextT = mytrack->GetNextSection(t);
   double xpos = t->getStartX();
   double ypos = t->getStartY();
   double zpos = t->getStartZ();
   double nextxpos = t->getEndX();
   double nextypos = t->getEndY();
   double nextzpos = nextT->getStartZ();
   double angle    = t->getAngle();
   double stAngle  = t->getStartAngle();
   double x1,y1;
   double x2,y2;
   double az = zpos+(((double)positionIntoSector)*(nextzpos-zpos)/(double)t->getLength());

   if (angle != 0)
   {
      CPoint trackPt = GetTrackPoint(stAngle,angle,positionIntoSector);
      y1 = trackPt.x;
      x1 = trackPt.y;
      CPoint trackPt2 = GetTrackPoint(stAngle,angle,positionIntoSector+10);
	  x2 = 	trackPt2.y;
	  y2 =  trackPt2.x;
	  }
   else
   {
      y1 = positionIntoSector*sin(stAngle);
      x1 = positionIntoSector*cos(stAngle);
	  y2 = (10+positionIntoSector)*sin(stAngle);
      x2 = (10+positionIntoSector)*cos(stAngle);
   }

   xpos = xpos+x1;
   ypos = ypos+y1;
   // positionIntoSector
   // adjust position relative to positionIntoSector

   double eyeX = GET_GL_PT_X(xpos);
   double eyeY = GET_GL_PT_Y(ypos);
   double eyeZ = -GET_GL_PT_Z((az));

   double lookX = GET_GL_PT_X(xpos+nextxpos);
   double lookY = GET_GL_PT_Y(ypos+nextypos);
   double lookZ = -GET_GL_PT_Z(nextzpos);

   gluLookAt(eyeX,eyeY,(eyeZ-car_height),
			  lookX,lookY,(lookZ-car_height),
			  0.0f,0.0f,-1.0f);
   }
   else
   {
	   // this is for the camera;
	   CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
   GPTrack * mytrack = pDoc->getTrack();

   //TrackSection *t = (TrackSection*)mytrack->TrackSections->elementAt(currentSectorNumber);
   if (currentSector==NULL)
   {
	   currentSector = (TrackSection*)mytrack->TrackSections->elementAt(currentSectorNumber);
   }
   TrackSection *t = currentSector;
   TrackSection *nextT = mytrack->GetNextSection(t);
   double xpos = t->getStartX();
   double ypos = t->getStartY();
   double zpos = t->getStartZ();
   double nextxpos = t->getEndX();
   double nextypos = t->getEndY();
   double nextzpos = nextT->getStartZ();
   double angle    = t->getAngle();
   double stAngle  = t->getStartAngle();
   
   double x1,y1;

   if (angle != 0)
   {  
      CPoint trackPt = GetTrackPoint(stAngle,angle,positionIntoSector);
      y1 = trackPt.x;
      x1 = trackPt.y;
   }
   else
   {
      y1 = positionIntoSector*sin(stAngle);
      x1 = positionIntoSector*cos(stAngle);
   }

   xpos = xpos+x1;
   ypos = ypos+y1;
   // positionIntoSector
   // adjust position relative to positionIntoSector
   double car_height = 0.2;

   double cameraLookX = GET_GL_PT_X(xpos);
   double cameraLookY = GET_GL_PT_Y(ypos);
   double cameraLookZ = -GET_GL_PT_Z((zpos));

   double cameraFromX =  cameraLookX + 10;
   double cameraFromY =  cameraLookY + 1;
   double cameraFromZ =  cameraLookZ + 2;

   double distance_to_section_begin   = mytrack->LengthToTrackSector(currentSector);
   double distance_to_carpos =distance_to_section_begin + positionIntoSector;

   Camera* currentCam = mytrack->DistanceToCamera(distance_to_carpos);


    if (currentCam != NULL)
    {    
		    CPosAngle *CameraPos    = DistanceToPoint(mytrack,(currentCam->getDistance()-16384));
			double distance = currentCam->getPerpendicularDistance();        
			  double di = WIDTH_TO_TRACKWIDTH_OBJECTS(fabs(distance));
			  double leftSideX,leftSideY;
			  double xr = CameraPos->x;
			  double yr = CameraPos->y;
		      if (distance > 0)
		      {
			   
		       leftSideX = xr+getRightSideX(CameraPos->angle,di+WIDTH_TO_TRACKWIDTH_OBJECTS(0));
               leftSideY = yr+getRightSideY(CameraPos->angle,di+WIDTH_TO_TRACKWIDTH_OBJECTS(0));
		      }
		      else
		      {			  
			   leftSideX = xr+getLeftSideX(CameraPos->angle,di+WIDTH_TO_TRACKWIDTH_OBJECTS(0));
               leftSideY = yr+getLeftSideY(CameraPos->angle,di+WIDTH_TO_TRACKWIDTH_OBJECTS(0));			
		      }
			cameraFromX = GET_GL_PT_X(leftSideX);
			cameraFromY = GET_GL_PT_X(leftSideY);
		    //cameraFromZ = 1;
			//+currentCam->getZ1()/512
			TRACE("Camera %2.2f %2.2f %2.2f %2.2f\n",CameraPos->x, CameraPos->y,CameraPos->z,(float)currentCam->getZ1());
		    cameraFromZ = GET_GL_PT_Z(CameraPos->z+((double)currentCam->getZ1()/256.0));
	        gluLookAt(cameraFromX,cameraFromY,-cameraFromZ,
			  cameraLookX,cameraLookY,(cameraLookZ-car_height),
			  0.0f,0.0f,-1.0f);
    }
   }
   
}

void CTrackOpenGLView::InitView(int from,int to)
{
	    

	    CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	    GPTrack * mytrack = pDoc->getTrack();
	    int size = mytrack->TrackSections->size();
		currentSector = (TrackSection*)mytrack->TrackSections->elementAt(currentSectorNumber);
		currentSectorLength = currentSector->getLength();
		
		double MinX=10E8;
		double MinY=10E8;
		double MinZ=10E8;
		double MaxX=-10E8;
		double MaxY=-10E8;
		double MaxZ=-10E8;

		for(int i=from;i<to;i++)
		{
		 TrackSection *t = (TrackSection*)mytrack->TrackSections->elementAt(i);

		 double xpos = t->getStartX();
		 double ypos = t->getStartY();
		 double zpos = t->getStartZ();

		 MinX = min(MinX,xpos);
		 MinY = min(MinY,ypos);
		 MinZ = min(MinZ,zpos);
		 MaxX = max(MaxX,xpos);
		 MaxY = max(MaxY,ypos);
		 MaxZ = max(MaxZ,zpos);
		}

		MidX = (MaxX+MinX)/2;
		MidY = (MaxY+MinY)/2;
		MidZ = 0.0;

		SC = 5.0;

		
}


void drawPoint(double wx,double wy,double wz,BOOL showAxis)
{						  
	glPushMatrix();
    GLUquadricObj* tyre         =  gluNewQuadric();
	glTranslated(wx,wy,wz);
	gluQuadricDrawStyle(tyre,GLU_SILHOUETTE);
	glColor3f(1.0f,0.f,0.f);
	gluSphere(tyre,0.5,5,5);

	//glRotated(camera->Direction,0.0f,0.0f,1.0f);
	//glRotated(camera->Roll,0.0f,1.0f,0.0f);
	//glRotated(camera->Pitch,1.0f,0.0f,0.0f);

	if (showAxis)
	{
	    float v = 50.0f;
	// draw carthesian axes
	    glBegin(GL_LINES);
		// red x axis
		glColor3f(1.0f,0.f,0.f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(v,0.0f,0.0f);
		
		// green y axis
		glColor3f(0.f,1.f,0.f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,v,0.0f);
		// blue z axis
		glColor3f(0.f,0.f,1.f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,0.0f,v);
		glEnd();
	}

	glPopMatrix();


}

#define TRACK 1

void CTrackOpenGLView::OpenGLDrawScene(HDC hdc)
{
	    glCallList(TRACK);
	    //InitDisplayList(hdc);
		SwapBuffers(hdc);
}

void CTrackOpenGLView::InitDisplayList(HDC hdc)
{
		glNewList(TRACK, GL_COMPILE);
	    CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	    GPTrack * mytrack = pDoc->getTrack();
	    //int to=10;
		//int from=0;
	    glDisable(GL_TEXTURE_2D);
	    if (inited==FALSE){
		  CViewSections *dlg = new CViewSections();
          
		  dlg->m_To   = to;
		  dlg->m_From = from;
		  int result = dlg->DoModal();


		  if (result == IDOK)
		  {
		    to   = dlg->m_To;
			from = dlg->m_From;
			if (from <= 0) 
				from = 0;
			int size = mytrack->TrackSections->size();
			if (to >= size) to = size;
		  }
		  InitView(from,to);
	      inited=TRUE;
		  if (dlg) delete dlg;
      	}
		glColor3f (0.0f, 0.0f, 0.0f);
		glEnable(GL_TEXTURE_2D);

		if (mytrack->showScenery)
		    DrawScenery(mytrack);

		if (mytrack->showAxis)
		{
		// draw carthesian axes
	    glBegin(GL_LINES);
		// red x axis
		glColor3f(1.f,0.f,0.f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(1.0f,0.0f,0.0f);
		glVertex3f(1.0f,0.0f,0.0f);
		glVertex3f(0.9f,0.1f,0.0f);
		glVertex3f(1.0f,0.0f,0.0f);
		glVertex3f(0.9f,-0.1f,0.0f);
		// green y axis
		glColor3f(0.f,1.f,0.f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,1.0f,0.0f);
		glVertex3f(0.0f,1.0f,0.0f);
		glVertex3f(0.1f,0.9f,0.0f);
		glVertex3f(0.0f,1.0f,0.0f);
		glVertex3f(-0.1f,0.9f,0.0f);
		// blue z axis
		glColor3f(0.f,0.f,1.f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f,0.1f,0.9f);
		glVertex3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f,-0.1f,0.9f);
	    glEnd();
		}

		int size = mytrack->TrackSections->size();
		
		
		TrackSection *tFrom = (TrackSection*)mytrack->TrackSections->elementAt(from);
		if (to >= size) to = size-1;
		TrackSection *tTo   = (TrackSection*)mytrack->TrackSections->elementAt(to);
		TrackSection *nextT = mytrack->GetNextSection(tFrom);
		TrackSection *tEnd  = mytrack->GetNextSection(tTo);
		TrackSection *tTerm = tTo;

		if (mytrack->showTrack)
		{
		//for(int i=from;i<to;i++)
		//
		TrackSection *t = tFrom;
		while (t)
		{ 
		 ///////////////////////////////////////////////////////////////////////////
		 ///////////////////////////////////////////////////////////////////////////
		 //int idx = mytrack->TrackSections->findElementIndex(t);
		 //TRACE("T[%d]\n",idx);
		 glDisable(GL_TEXTURE_2D);
		 //TrackSection *t = (TrackSection*)mytrack->TrackSections->elementAt(i);
		 nextT = mytrack->GetNextSection(t);

		 double xpos = t->getStartX();
	     double ypos = t->getStartY();
		 double zpos = -t->getStartZ();
	     double nextxpos = nextT->getStartX();
		 double nextypos = nextT->getStartY();
		 double nextzpos = -nextT->getStartZ();
	     double sectorStartAngle = t->getStartAngle();
	     double sectorEndAngle   = t->getEndAngle();
		 double sectorStartAngleN  = nextT->getStartAngle();
		 //double zpos=0;

	     TrackSection *prev = mytrack->GetPreviousSection(t);

	     double previousLeft = t->getWidthLeft();
	     double previousRight = t->getWidthRight();

	     if (prev!=NULL)
	     {
	      previousLeft = prev->getWidthLeft();
	      previousRight = prev->getWidthRight();
	     }
	
		 if (mytrack->showTextures){ 
			 glEnable(GL_TEXTURE_2D);
			 RoadBits = TextureMap(GetDC(),RoadBmp,RoadBits,FALSE);
	     }

	     // track parts
		 
	     double leftSideX   = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft));
	     double leftSideY   = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft));
	     double rightSideX  = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight));
	     double rightSideY  = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight));
		 double leftSideNX  = nextxpos+getLeftSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()));
	     double leftSideNY  = nextypos+getLeftSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()));   
	     double rightSideNX = nextxpos+getRightSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()));
	     double rightSideNY = nextypos+getRightSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()));
		 
		 //road
		 glColor3f(0.6f,0.6f,0.6f);
		 if (mytrack->showFilledObjects)
		 {
		     glBegin(GL_POLYGON); 
		 }
		 else {
			 glBegin(GL_LINE_STRIP ); 
		 }
		 glTexCoord2f(0.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(leftSideX),GET_GL_PT_Y(leftSideY),GET_GL_PT_Z(zpos));
		 glTexCoord2f(1.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(leftSideNX),GET_GL_PT_Y(leftSideNY),GET_GL_PT_Z(nextzpos));		 		 
	  	 glTexCoord2f(1.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(rightSideNX),GET_GL_PT_Y(rightSideNY),GET_GL_PT_Z(nextzpos));
		 glTexCoord2f(0.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(rightSideX),GET_GL_PT_Y(rightSideY),GET_GL_PT_Z(zpos));
		 glEnd();
		 

		 double KleftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft-KWIDTH));
	     double KleftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft-KWIDTH));
	     double KrightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight-KWIDTH));
	     double KrightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight-KWIDTH));

		 double Kerb1leftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft+KERB_1_WIDTH));
	     double Kerb1leftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft+KERB_1_WIDTH));
	     double Kerb1rightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight+KERB_1_WIDTH));
	     double Kerb1rightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight+KERB_1_WIDTH));

		 double Kerb2leftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft+KERB_2_WIDTH));
	     double Kerb2leftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft+KERB_2_WIDTH));
	     double Kerb2rightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight+KERB_2_WIDTH));
	     double Kerb2rightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight+KERB_2_WIDTH));



           
	     // track parts
	     //double leftSideNX = nextxpos+getLeftSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()));
	     //double leftSideNY = nextypos+getLeftSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()));   
	     //double rightSideNX = nextxpos+getRightSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()));
	     //double rightSideNY = nextypos+getRightSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()));
		 double KleftSideNX = nextxpos+getLeftSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()-KWIDTH));
	     double KleftSideNY = nextypos+getLeftSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()-KWIDTH));   
	     double KrightSideNX = nextxpos+getRightSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()-KWIDTH));
	     double KrightSideNY = nextypos+getRightSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()-KWIDTH));


		 double Kerb1leftSideNX = nextxpos+getLeftSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+KERB_1_WIDTH));
	     double Kerb1leftSideNY = nextypos+getLeftSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+KERB_1_WIDTH));   
	     double Kerb1rightSideNX = nextxpos+getRightSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+KERB_1_WIDTH));
	     double Kerb1rightSideNY = nextypos+getRightSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+KERB_1_WIDTH));
		 double Kerb2leftSideNX = nextxpos+getLeftSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+KERB_2_WIDTH));
	     double Kerb2leftSideNY = nextypos+getLeftSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+KERB_2_WIDTH));   
	     double Kerb2rightSideNX = nextxpos+getRightSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+KERB_2_WIDTH));
	     double Kerb2rightSideNY = nextypos+getRightSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+KERB_2_WIDTH));



		 


		   int leftWall   =  prev->getLeftToBank()*0x40;
	       int rightWall  =  prev->getRightToBank()*0x40;

	       int EleftWall  = t->getLeftToBank()*0x40;
	       int ErightWall = t->getRightToBank()*0x40;
		  
           // track parts
           double WleftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+leftWall));
           double WleftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+leftWall));
		   double WrightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+rightWall));
           double WrightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+rightWall));
 
           // track parts
           double WleftSideNX = nextxpos+getLeftSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+EleftWall));
           double WleftSideNY = nextypos+getLeftSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+EleftWall));           
           double WrightSideNX = nextxpos+getRightSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+ErightWall));
           double WrightSideNY = nextypos+getRightSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+ErightWall));


		 
		 

		 if (mytrack->showTextures){ 
			 glEnable(GL_TEXTURE_2D);
			 VergeLeftBits = TextureMap(GetDC(),VergeLeftBmp,VergeLeftBits,FALSE);
	     }

		 // left bank
		 glColor3f(0.0f,0.6f,0.0f);
		 if (mytrack->showFilledObjects)
		 {
		     glBegin(GL_POLYGON); 
		 }
		 else {
			 glBegin(GL_LINE_STRIP ); 
		 }
		 glTexCoord2f(0.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(leftSideX),GET_GL_PT_Y(leftSideY),GET_GL_PT_Z(zpos));
		 glTexCoord2f(0.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(WleftSideX),GET_GL_PT_Y(WleftSideY),GET_GL_PT_Z(zpos));
		 glTexCoord2f(1.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(WleftSideNX),GET_GL_PT_Y(WleftSideNY),GET_GL_PT_Z(nextzpos));
		 glTexCoord2f(1.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(leftSideNX),GET_GL_PT_Y(leftSideNY),GET_GL_PT_Z(nextzpos));		 		 	 
		 glEnd();

		 
		 if (mytrack->showTextures){ 
			 glEnable(GL_TEXTURE_2D);
			 VergeRightBits= TextureMap(GetDC(),VergeRightBmp,VergeRightBits,FALSE);
	     }

		 // right bank
		 glColor3f(0.0f,0.6f,0.0f);
		 if (mytrack->showFilledObjects)
		 {
		     glBegin(GL_POLYGON); 
		 }
		 else {
			 glBegin(GL_LINE_STRIP); 
		 }
		 glTexCoord2f(0.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(rightSideX),GET_GL_PT_Y(rightSideY),GET_GL_PT_Z(zpos));
		 glTexCoord2f(1.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(rightSideNX),GET_GL_PT_Y(rightSideNY),GET_GL_PT_Z(nextzpos));		 
		 glTexCoord2f(1.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(WrightSideNX),GET_GL_PT_Y(WrightSideNY),GET_GL_PT_Z(nextzpos));
		 

		 glTexCoord2f(0.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(WrightSideX),GET_GL_PT_Y(WrightSideY),GET_GL_PT_Z(zpos));
		 
		 glEnd();

		 if (mytrack->showWalls)
		 {

		 if (mytrack->showTextures){ 
			 glEnable(GL_TEXTURE_2D);
			 FenceLeftBits = TextureMap(GetDC(),FenceLeftBmp,FenceLeftBits,FALSE);
	     }
		 double wall_height = -0.3;
		 // left wall
		 glColor3f(0.8f,0.8f,0.8f);
		 if (mytrack->showFilledObjects)
		 {
		     glBegin(GL_POLYGON); 
		 }
		 else {
			 glBegin(GL_LINE_STRIP); 
		 }
		 glTexCoord2f(0.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(WleftSideX),GET_GL_PT_Y(WleftSideY),GET_GL_PT_Z(zpos));
		 glTexCoord2f(0.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(WleftSideX),GET_GL_PT_Y(WleftSideY),GET_GL_PT_Z(zpos+wall_height));
		 glTexCoord2f(1.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(WleftSideNX),GET_GL_PT_Y(WleftSideNY),GET_GL_PT_Z(nextzpos+wall_height));
		 glTexCoord2f(1.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(WleftSideNX),GET_GL_PT_Y(WleftSideNY),GET_GL_PT_Z(nextzpos));		 
		 
		 glEnd();

		 if (mytrack->showTextures){ 
			 glEnable(GL_TEXTURE_2D);
			 FenceRightBits = TextureMap(GetDC(),FenceRightBmp,FenceRightBits,FALSE);
	     }

		 // right wall
		 glColor3f(0.8f,0.8f,0.8f);
		 if (mytrack->showFilledObjects)
		 {
		     glBegin(GL_POLYGON); 
		 }
		 else {
			 glBegin(GL_LINE_STRIP); 
		 }
		 glTexCoord2f(1.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(WrightSideX),GET_GL_PT_Y(WrightSideY),GET_GL_PT_Z(zpos));
		 glTexCoord2f(0.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(WrightSideNX),GET_GL_PT_Y(WrightSideNY),GET_GL_PT_Z(nextzpos));		 		 
		 glTexCoord2f(0.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(WrightSideNX),GET_GL_PT_Y(WrightSideNY),GET_GL_PT_Z(nextzpos+wall_height));
		 glTexCoord2f(1.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(WrightSideX),GET_GL_PT_Y(WrightSideY),GET_GL_PT_Z(zpos+wall_height));
		 glEnd();
		 }

		 

		 if (t->isLeftKerb())
		 {
		   if (mytrack->showTextures){ 
			 glEnable(GL_TEXTURE_2D);
			 Kerb1Bits = TextureMap(GetDC(),Kerb1Bmp,Kerb1Bits,FALSE);
	       }
		 // red kerb
		 glColor3f(1.0f,0.0f,0.0f);
		 glBegin(GL_POLYGON);		 
		 glTexCoord2f(0.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(leftSideX),GET_GL_PT_Y(leftSideY),GET_GL_PT_Z(zpos));		 
		 glTexCoord2f(0.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(Kerb1leftSideX),GET_GL_PT_Y(Kerb1leftSideY),GET_GL_PT_Z(zpos-KERB_1_HEIGHT));		 
		 glTexCoord2f(1.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(Kerb1leftSideNX),GET_GL_PT_Y(Kerb1leftSideNY),GET_GL_PT_Z(nextzpos-KERB_1_HEIGHT));		 
		 glTexCoord2f(1.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(leftSideNX),GET_GL_PT_Y(leftSideNY),GET_GL_PT_Z(nextzpos));		 		 	 
		 glEnd();

		 // blue kerb
		 glColor3f(0.0f,0.0f,1.0f);
		 glBegin(GL_POLYGON);	
		 glTexCoord2f(0.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(Kerb1leftSideX),GET_GL_PT_Y(Kerb1leftSideY),GET_GL_PT_Z(zpos-KERB_1_HEIGHT));		 
		 glTexCoord2f(0.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(Kerb2leftSideX),GET_GL_PT_Y(Kerb2leftSideY),GET_GL_PT_Z(zpos-KERB_2_HEIGHT));		 
		 glTexCoord2f(1.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(Kerb2leftSideNX),GET_GL_PT_Y(Kerb2leftSideNY),GET_GL_PT_Z(nextzpos-KERB_2_HEIGHT));		 
		 glTexCoord2f(1.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(Kerb1leftSideNX),GET_GL_PT_Y(Kerb1leftSideNY),GET_GL_PT_Z(nextzpos-KERB_1_HEIGHT));		 		 	 		 
		 glEnd();


		 }

		 if (t->isRightKerb())
		 {
		   if (mytrack->showTextures){ 
			 glEnable(GL_TEXTURE_2D);
			 Kerb1Bits = TextureMap(GetDC(),Kerb1Bmp,Kerb1Bits,FALSE);
	       }
		 glColor3f(1.0f,0.0f,0.0f);
		 glBegin(GL_POLYGON);		 
		 glTexCoord2f(0.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(rightSideX),GET_GL_PT_Y(rightSideY),GET_GL_PT_Z(zpos));		 
		 glTexCoord2f(0.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(Kerb1rightSideX),GET_GL_PT_Y(Kerb1rightSideY),GET_GL_PT_Z(zpos-KERB_1_HEIGHT));		 
		 glTexCoord2f(1.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(Kerb1rightSideNX),GET_GL_PT_Y(Kerb1rightSideNY),GET_GL_PT_Z(nextzpos-KERB_1_HEIGHT));		 
		 glTexCoord2f(1.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(rightSideNX),GET_GL_PT_Y(rightSideNY),GET_GL_PT_Z(nextzpos));		 		 	 
		 glEnd();

		 glColor3f(0.0f,0.0f,1.0f);
		 glBegin(GL_POLYGON);		 
		 glTexCoord2f(0.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(Kerb1rightSideX),GET_GL_PT_Y(Kerb1rightSideY),GET_GL_PT_Z(zpos-KERB_1_HEIGHT));		 
		 glTexCoord2f(0.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(Kerb2rightSideX),GET_GL_PT_Y(Kerb2rightSideY),GET_GL_PT_Z(zpos-KERB_2_HEIGHT));		 
		 glTexCoord2f(1.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(Kerb2rightSideNX),GET_GL_PT_Y(Kerb2rightSideNY),GET_GL_PT_Z(nextzpos-KERB_2_HEIGHT));		 
		 glTexCoord2f(1.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(Kerb1rightSideNX),GET_GL_PT_Y(Kerb1rightSideNY),GET_GL_PT_Z(nextzpos-KERB_1_HEIGHT));		 		 	 
		 glEnd();

		 }

		 glDisable(GL_TEXTURE_2D);


		 

		 

		 

		 // white kerb
		 glColor3f(1.0f,1.0f,1.0f);
		 glBegin(GL_POLYGON);		 
		 glVertex3d(GET_GL_PT_X(leftSideX),GET_GL_PT_Y(leftSideY),GET_GL_PT_Z(zpos-KDIFF));		 
		 glVertex3d(GET_GL_PT_X(leftSideNX),GET_GL_PT_Y(leftSideNY),GET_GL_PT_Z(nextzpos-KDIFF));		 		 	 
		 glVertex3d(GET_GL_PT_X(KleftSideNX),GET_GL_PT_Y(KleftSideNY),GET_GL_PT_Z(nextzpos-KDIFF));		 
		 glVertex3d(GET_GL_PT_X(KleftSideX),GET_GL_PT_Y(KleftSideY),GET_GL_PT_Z(zpos-KDIFF));		 			 
		 glEnd();

		 // white kerb
		 glBegin(GL_POLYGON);
		 glVertex3d(GET_GL_PT_X(rightSideX),GET_GL_PT_Y(rightSideY),GET_GL_PT_Z(zpos-KDIFF));
		 glVertex3d(GET_GL_PT_X(KrightSideX),GET_GL_PT_Y(KrightSideY),GET_GL_PT_Z(zpos-KDIFF));
		 glVertex3d(GET_GL_PT_X(KrightSideNX),GET_GL_PT_Y(KrightSideNY),GET_GL_PT_Z(nextzpos-KDIFF));
		 glVertex3d(GET_GL_PT_X(rightSideNX),GET_GL_PT_Y(rightSideNY),GET_GL_PT_Z(nextzpos-KDIFF));		 
		 glEnd();

		 

		 glColor3f(0.0f,0.0f,0.0f);

		 if (mytrack->showObjects)
		 {
		 Vector *TrackSideObjects = t->getObjects();
		 int numObjects = TrackSideObjects->size();
         for(int i=0;i<numObjects;i++)
         {
			 TrackObject *ob = (TrackObject*)TrackSideObjects->elementAt(i);
		     if (t!=NULL)
		     {
               double trackWidth = t->getWidthLeft();
			   double nextypos;
		       double nextxpos;
		       double remainder = ob->getDistance();
		       double length    = t->getLength();
			   double angle     = t->getAngle();
			   double stAngle   = sectorStartAngle;

		       if (angle != 0)
		       {
		         CPoint trackPt = GetTrackPoint(stAngle,angle,remainder);
                 nextypos = trackPt.x;
                 nextxpos = trackPt.y;
	             double deltaAngle = DegToRad(angle*length)*(remainder/length);
	             angle=stAngle+deltaAngle;
		       }
		       else
		       {
		         nextypos = ob->getDistance()*sin(stAngle);
                 nextxpos = ob->getDistance()*cos(stAngle);
		         angle = stAngle;
		        }
		 

				double xr = xpos+nextxpos;
				double yr = ypos+nextypos;
				double obposx;
				double obposy;
				double obposz;

				TrackObjectDefinition *obj = ob->getObjectDef();

		
				if (obj!=NULL)
				{
					double distance = obj->getDistance();
					obposz = zpos;
					double di = WIDTH_TO_TRACKWIDTH_OBJECTS(fabs(distance));
					if (distance > 0)
					{
					obposx = xr+getRightSideX(angle,di);
					obposy = yr+getRightSideY(angle,di);
					}
					else
					{
					obposx = xr+getLeftSideX(angle,di);
					obposy = yr+getLeftSideY(angle,di);						
					}
		 
					double glxpos =  GET_GL_PT_X(obposx);
					double glypos =  GET_GL_PT_Y(obposy);
					double glzpos =  GET_GL_PT_Z(obposz);

					CDC *pDC = CDC::FromHandle(hdc);
					obj->DrawGL(pDC,glxpos,glypos,glzpos,angle,mytrack->showAxis);
				}
			}
		  }
		  delete TrackSideObjects;
		 }		 
		 if (mytrack->showTrackSide) drawTextureSides(t,mytrack);
		 // next track section
		 if (t == tTo) break;
		 t = nextT;
		}
		//if (mytrack->showTrackSide)
		//{
		// for(int i=to;i<from;i++)
        // {
		//   TrackSection *t = (TrackSection*)mytrack->TrackSections->elementAt(i);		   		   
		//   
	    // }
		//}
        } // if show track

		if (mytrack->showPitLane) OpenGLDrawPitlane(hdc);

		

		if (mytrack->showCameras)
		 {
		   for(int i=0;i<mytrack->TrackCameras->size();i++)
           {
			  Camera* cam = (Camera*)mytrack->TrackCameras->elementAt(i);
		      CPosAngle *CameraPos    = DistanceToPoint(mytrack,(cam->getDistance()-16384));
			  double distance = cam->getPerpendicularDistance();        
			  double di = WIDTH_TO_TRACKWIDTH_OBJECTS(fabs(distance));
			  double leftSideX,leftSideY;
			  double xr = CameraPos->x;
			  double yr = CameraPos->y;
		      if (distance > 0)
		      {
			   
		       leftSideX = xr+getRightSideX(CameraPos->angle,di+WIDTH_TO_TRACKWIDTH_OBJECTS(0));
               leftSideY = yr+getRightSideY(CameraPos->angle,di+WIDTH_TO_TRACKWIDTH_OBJECTS(0));
		      }
		      else
		      {			  
			   leftSideX = xr+getLeftSideX(CameraPos->angle,di+WIDTH_TO_TRACKWIDTH_OBJECTS(0));
               leftSideY = yr+getLeftSideY(CameraPos->angle,di+WIDTH_TO_TRACKWIDTH_OBJECTS(0));			
		      }

			  drawPoint(GET_GL_PT_X(leftSideX),GET_GL_PT_Y(leftSideY),GET_GL_PT_Z(CameraPos->z),FALSE);
		   }
		 }

		glEndList();
		
}

void CTrackOpenGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if (currentView== t_Air) return;

	if (nChar == VK_DOWN)
	{
		if (positionIntoSector > 0)
	    {
		positionIntoSector--;
	    }
	    else
	    {
		CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
        GPTrack * mytrack = pDoc->getTrack();
		currentSectorNumber--;
		int size = mytrack->TrackSections->size();
		if (currentSectorNumber < 0) currentSectorNumber = size-1;
		currentSector = (TrackSection*)mytrack->TrackSections->elementAt(currentSectorNumber);
		currentSectorLength = currentSector->getLength();
		positionIntoSector = currentSectorLength; 

		// recreate the object list
		glDeleteLists(TRACK,1);
	    InitDisplayList(NULL);
	    }
	}
	else
	{
	 if (positionIntoSector < currentSectorLength)
	 {
		positionIntoSector++;
	 }
	 else
	 {
		CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
        GPTrack * mytrack = pDoc->getTrack();
		currentSectorNumber++;
		int size = mytrack->TrackSections->size();
		if (currentSectorNumber >= size) currentSectorNumber = 0;
		currentSector = (TrackSection*)mytrack->TrackSections->elementAt(currentSectorNumber);
		currentSectorLength = currentSector->getLength();
		positionIntoSector = 0;  

		// recreate the object list
		glDeleteLists(TRACK,1);
	    InitDisplayList(NULL);
	 }
	}
	InvalidateRect(NULL,FALSE);
	COpenGLView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTrackOpenGLView::OnAirview() 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
    GPTrack * mytrack = pDoc->getTrack();	
	from = 0;
	to = mytrack->TrackSections->size();
	currentView = t_Air;	
	OnRefreshTree();
}

void CTrackOpenGLView::OnUpdateAirview(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(currentView == t_Air);
}

void CTrackOpenGLView::OnViewCockpit() 
{
	currentView = t_Cockpit;
	OnRefreshTree();
}


void CTrackOpenGLView::OnUpdateViewCockpit(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(currentView == t_Cockpit);	
}

/*
void CTrackOpenGLView::OnDrawaxis() 
{
	drawAxis != drawAxis;
	InvalidateRect(NULL,FALSE);
}

void CTrackOpenGLView::OnUpdateDrawaxis(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(drawAxis);

}
*/

void CTrackOpenGLView::DrawScenery(GPTrack *track)
{
    for(int i=0;i<track->TrackSections->size();i++)
	{
		TrackSection *t = (TrackSection*)track->TrackSections->elementAt(i);

		t->drawScenery3DGL(this);	
	}
}

void CTrackOpenGLView::DrawRibbon3DGL(GPTrack *track,
						double xpos,double ypos,double zpos,
				double x1,double y1,double z1,
				double nx1,double ny1,double nz1,
				double x2,double y2,double z2,
				double nx2,double ny2,double nz2,int textureId)
{
			
			switch(textureId)
			{
			  case 1:glColor3f(1.0f,0.0f,0.0f);break;
			  case 2:glColor3f(0.0f,1.0f,1.0f);break;
			  case 3:glColor3f(0.0f,0.0f,1.0f);break;
			  case 4:glColor3f(1.0f,1.0f,0.0f);break;
			  case 5:glColor3f(1.0f,0.0f,1.0f);break;
			}
            if (x1 != -1 && x2 !=-1 && nx1 != -1 && nx2 != -1)
			{
             if (z1 != 0 && nz1 != 0 && nz2 !=0 && z2 != 0)
			 {
			  glBegin(GL_POLYGON);
		      glVertex3d(GET_GL_PT_X(x1),GET_GL_PT_Y(y1),GET_GL_PT_Z(-z1));
		      glVertex3d(GET_GL_PT_X(nx1),GET_GL_PT_Y(ny1),GET_GL_PT_Z(-nz1));
		      glVertex3d(GET_GL_PT_X(nx2),GET_GL_PT_Y(ny2),GET_GL_PT_Z(-nz2));
		      glVertex3d(GET_GL_PT_X(x2),GET_GL_PT_Y(y2),GET_GL_PT_Z(-z2));		 
		      glEnd();
			 }
			}
}





void TrackSection::drawScenery3DGL(CTrackOpenGLView *view)
{
	double xpos = getStartX();
    double ypos = getStartY();
	//double zpos = getStartY();
	double zpos = 0;
    double nextxpos = getEndX();
    double nextypos = getEndY();
	//double nextzpos = getEndY();
	double nextzpos = 0;
    double sectorStartAngle = getStartAngle();
    double sectorEndAngle   = getStartAngle();
	

	static double x1 = -1;
	static double y1 = 0;
	static double x2 = -1;
	static double y2 = 0;
	static double x3 = -1;
	static double y3 = 0;
    static double x4 = -1;
	static double y4 = 0;
	static double x5 = -1;
	static double y5 = 0;
	static double x6 = -1;
	static double y6 = 0;
	static double z1 = 0;
	static double z2 = 0;
	static double z3 = 0;
	static double z4 = 0;
	static double z5 = 0;
	static double z6 = 0;

	//Set default positions for scenery by reading scenery definition
	if (this->getTrack()->GetTrackSectionIndex(this) == 0)
	{
		// work on first section
		double xpos = getStartX();
        double ypos = getStartY();
		//double zpos = getStartZ();
		double zpos = 0;

		x1 = xpos;
		x2 = xpos;
		x3 = xpos;
		x4 = xpos;
		x5 = xpos;
		x6 = xpos;

		y1 = ypos;
		y2 = ypos;
		y3 = ypos;
		y4 = ypos;
		y5 = ypos;
		y6 = ypos;
		
		/*z1 = zpos;
		z2 = zpos;
		z3 = zpos;
		z4 = zpos;
		z5 = zpos;
		z6 = zpos;
		*/
	}

	Vector *cmds = getCommands();
	e_ScenerySide CurrentScenerySide = t_BothScenery;
	double LeftAngle = 0;
	double RightAngle = 0;

	if (cmds!=NULL && cmds->size() > 0)
	{
		int distanceIn = 0;
		for(int i=0;i<cmds->size();i++)
		{
			TrackCmd *scenery = (TrackCmd*)cmds->elementAt(i);
			int cmdCode= scenery->getCmd();

			// get the side info
			if (cmdCode == 0xaf) {
				CurrentScenerySide = t_BothScenery;
				LeftAngle  = scenery->getNthArg(2);
				RightAngle = scenery->getNthArg(3);
			}
			if (cmdCode == 0xc0) {
				CurrentScenerySide = t_LeftScenery;
				LeftAngle  = scenery->getNthArg(2);
				
			}
			if (cmdCode == 0xc1) {
				CurrentScenerySide = t_RightScenery;
				RightAngle = scenery->getNthArg(2);
			}

			if (cmdCode != 0xb8) continue;

			if (cmdCode == 0xb9)
			{
					distanceIn = 0;
				    //distanceIn =  scenery->getNthArg(2);
					//relative movement
					//continue;
			}
			
			double newx = xpos;
			double newy = ypos;
			double newz = zpos;
			double newangle = sectorStartAngle;
			double dx=0;
			double dy=0;

         	double rightSide = getWidthRight()+(getRightToBank()*0x40);
			double leftSide  = getWidthLeft()+(getLeftToBank()*0x40);
			

			double nx1,nx2,nx3,nx4,nx5,nx6;
			double ny1,ny2,ny3,ny4,ny5,ny6;
			double nz1,nz2,nz3,nz4,nz5,nz6;
			

			CreateRibbons(scenery,newx,newy,newz,0,newangle,
						  nx1,ny1,nz1,nx2,ny2,nz2,nx3,ny3,nz3,nx4,ny4,nz4,
						  nx5,ny5,nz5,nx6,ny6,nz6,4096,0,0,t_BothScenery);

			view->DrawRibbon3DGL(track,xpos,ypos,zpos,x1,y1,z1,nx1,ny1,nz1,x2,y2,z2,nx2,ny2,nz2,1);
			view->DrawRibbon3DGL(track,xpos,ypos,zpos,x5,y5,z5,nx5,ny5,nz5,x6,y6,z6,nx6,ny6,nz6,5);
			view->DrawRibbon3DGL(track,xpos,ypos,zpos,x4,y4,z4,nx4,ny4,nz4,x5,y5,z5,nx5,ny5,nz5,4);
			view->DrawRibbon3DGL(track,xpos,ypos,zpos,x3,y3,z3,nx3,ny3,nz3,x4,y4,z4,nx4,ny4,nz4,3);
			view->DrawRibbon3DGL(track,xpos,ypos,zpos,x2,y2,z2,nx2,ny2,nz2,x3,y3,z3,nx3,ny3,nz3,2);			  			  			  
			
			x1 = nx1;
			x2 = nx2;
			x3 = nx3;
			x4 = nx4;
			x5 = nx5;
			x6 = nx6;

			y1 = ny1;
			y2 = ny2;
			y3 = ny3;
			y4 = ny4;
			y5 = ny5;
			y6 = ny6;

			z1 = nz1;
			z2 = nz2;
			z3 = nz3;
			z4 = nz4;
			z5 = nz5;
			z6 = nz6;
		}
	}
}

void CTrackOpenGLView::OnViewcameras() 
{
	currentView = t_ViewCamera;
	OnRefreshTree();
	//InvalidateRect(NULL,FALSE);
	
}

void CTrackOpenGLView::OnUpdateViewcameras(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(currentView == t_ViewCamera);
}

void CTrackOpenGLView::OnDrawaxis() 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
    GPTrack * mytrack = pDoc->getTrack();
	mytrack->showAxis = !mytrack->showAxis;
	pDoc->UpdateAllViews(NULL);
}

void CTrackOpenGLView::OnUpdateDrawaxis(CCmdUI* pCmdUI) 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
    GPTrack * mytrack = pDoc->getTrack();
	pCmdUI->SetCheck(mytrack->showAxis);
}




void CTrackOpenGLView::LoadJamTexture(GPTrack *mytrack,CBitmap &output,int idx,int hres,int vres,int *width,int *height)
{
	//int vr = vres/256;
	//int hr = hres/256;
	//if (vres == -99 || (vres % 256) != 0) vr = 1;
	//if (hres == -99 || (hres % 256) != 0) hr = 1;

	int vr = 1;
	int hr = 1;

	int id = IDB_TESTTEXTURE_128;
	int td = 128;
	int currentRes = 0;

	/*
	switch(currentRes)
	{
	    case 0:
			id = IDB_TESTTEXTURE_64;
			td = 64;
			break;
	}
	*/

	CBitmap temp;
	output.LoadBitmap(id);
	JAM *jam = mytrack->GetJAM(idx);
	if (jam == NULL) return;
	int sidx  = jam->getImageIndex(idx);
	*width =  jam->getImageWidth(sidx);
	*height = jam->getImageHeight(sidx);
	jam->LoadSingleBMPImage(temp,
		  *width, 
		  *height,sidx);
	Mak256SquareImage(temp,output,*width,*height,hr,vr,td,td);
}

unsigned char * CTrackOpenGLView::TextureMap(CDC *pDC,CBitmap &cbmp,unsigned char *lpvBits,BOOL transparent)
{ 
  
  BITMAP bmp;
  cbmp.GetBitmap( &bmp );

  if (bmp.bmWidth > 1024) return NULL;
  if (bmp.bmHeight > 1024) return NULL;

  if (lpvBits==NULL)
  {
  CDC*          bitmapDc = new CDC;
  bitmapDc -> CreateCompatibleDC(pDC );

  CBitmap* oldBitMap = (CBitmap*) bitmapDc -> SelectObject( &cbmp );
  unsigned char *RealImage;
  lpvBits = new unsigned char[bmp.bmWidth * bmp.bmHeight *3];

  //unsigned char *lpvBits = new unsigned char[bmp.bmWidth * bmp.bmHeight *3];
  if (transparent)
  {
    RealImage = new unsigned char[bmp.bmWidth * bmp.bmHeight *4];
  }
	

  BITMAPINFO *bmpinfo = new BITMAPINFO;

  bmpinfo->bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
  bmpinfo->bmiHeader.biWidth         = bmp.bmWidth;
  bmpinfo->bmiHeader.biHeight        = bmp.bmHeight;
  bmpinfo->bmiHeader.biPlanes        = 1;
  bmpinfo->bmiHeader.biBitCount      = 24;
  bmpinfo->bmiHeader.biCompression   = BI_RGB;
  bmpinfo->bmiHeader.biSizeImage     = bmp.bmWidth * bmp.bmHeight * 3;
  bmpinfo->bmiHeader.biXPelsPerMeter = 0;
  bmpinfo->bmiHeader.biYPelsPerMeter = 0;
  bmpinfo->bmiHeader.biClrImportant  = 0;
  bmpinfo->bmiHeader.biClrUsed       = 0;

  if (!::GetDIBits( bitmapDc -> GetSafeHdc(), HBITMAP( cbmp ), 0,bmp.bmHeight, lpvBits, bmpinfo, DIB_RGB_COLORS ))
  {
   AfxMessageBox("GetDIBits error");
  }

  unsigned int size = (bmpinfo->bmiHeader.biSizeImage);

  int rcount = 0;
  for(unsigned int i=0;i<size;i+=3)
  {
	  unsigned char *ptr =lpvBits+i;
	  unsigned char *r = ptr;
	  unsigned char *g = ptr+1;
	  unsigned char *b = ptr+2;

	  unsigned char red = *r;
	  unsigned char blue = *b;
	  unsigned char green = *g;
	  *b = red;
	  *r = blue;

	  if (transparent)
	  {
	   RealImage[rcount] = red;
	   RealImage[rcount+1] = green;
	   RealImage[rcount+2] = blue;
	   if (red == 206 && blue == 206 && green == 207)
	   {
		 //RealImage[rcount] = 255;
		 //RealImage[rcount+1] = 255;
		 //RealImage[rcount+2] = 255;
	     RealImage[rcount+3] = 0;
	   }
	   else
	   {
	     RealImage[rcount+3] = 255;
	   }
	   rcount+=4;
	  }
  }

  if (transparent)
  {
	  delete(lpvBits);
	  lpvBits = RealImage;
  }


  GLint val;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE,&val);
  bitmapDc -> SelectObject( oldBitMap );
  delete bitmapDc;
  delete bmpinfo;
  }

  if (transparent)
    glTexImage2D(GL_TEXTURE_2D, 0, 4, bmp.bmWidth, bmp.bmHeight, 0,GL_RGBA, GL_UNSIGNED_BYTE, lpvBits);
  else
	glTexImage2D(GL_TEXTURE_2D, 0, 3, bmp.bmWidth, bmp.bmHeight, 0,GL_RGB, GL_UNSIGNED_BYTE, lpvBits);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);
 
  //delete[] lpvBits;
  return lpvBits;
}

void CTrackOpenGLView::OnShowlines() 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
    GPTrack * mytrack = pDoc->getTrack();
	mytrack->showLines = !mytrack->showLines;
	OnRefreshTree();
	pDoc->UpdateAllViews(NULL);
}

void CTrackOpenGLView::OnUpdateShowlines(CCmdUI* pCmdUI) 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
    GPTrack * mytrack = pDoc->getTrack();
	pCmdUI->SetCheck(mytrack->showLines);
}

void CTrackOpenGLView::OnShowculling() 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
    GPTrack * mytrack = pDoc->getTrack();
	mytrack->showCulling = !mytrack->showCulling;
	OnRefreshTree();
	pDoc->UpdateAllViews(NULL);
}

void CTrackOpenGLView::OnUpdateShowculling(CCmdUI* pCmdUI) 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
    GPTrack * mytrack = pDoc->getTrack();
	pCmdUI->SetCheck(mytrack->showCulling);
}

void CTrackOpenGLView::OnShowScenery() 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
    GPTrack * mytrack = pDoc->getTrack();
	mytrack->showScenery = !mytrack->showScenery;
	OnRefreshTree();
	pDoc->UpdateAllViews(NULL);
}

void CTrackOpenGLView::OnUpdateShowScenery(CCmdUI* pCmdUI) 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
    GPTrack * mytrack = pDoc->getTrack();
	pCmdUI->SetCheck(mytrack->showScenery);
}

void CTrackOpenGLView::OpenGLDrawPitlane(HDC hdc)
{
	    glDisable(GL_TEXTURE_2D);

		//AfxMessageBox("Here");
	    CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	    GPTrack * mytrack = pDoc->getTrack();

		glColor3f (0.0f, 0.0f, 0.0f);
		glEnable(GL_TEXTURE_2D);


		int size = mytrack->PitlaneSections->size();
		
		

		for(int i=0;i<size-1;i++)
		{
		 glDisable(GL_TEXTURE_2D);
		 TrackSection *t = (TrackSection*)mytrack->PitlaneSections->elementAt(i);
		 TrackSection *nextT = mytrack->GetNextSection(t);

		 double xpos = t->getStartX();
	     double ypos = t->getStartY();
		 double zpos = -t->getStartZ();
	     double nextxpos = nextT->getStartX();
		 double nextypos = nextT->getStartY();
		 double nextzpos = -nextT->getStartZ();
	     double sectorStartAngle = t->getStartAngle();
	     double sectorEndAngle   = t->getEndAngle();
		 double sectorStartAngleN  = nextT->getStartAngle();
		 //double zpos=0;

	     TrackSection *prev = mytrack->GetPreviousSection(t);

	     double previousLeft = t->getWidthLeft();
	     double previousRight = t->getWidthRight();

	     if (prev!=NULL)
	     {
	      previousLeft = prev->getWidthLeft();
	      previousRight = prev->getWidthRight();
	     }

	
	     // track parts
	     double leftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft));
	     double leftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft));
	     double rightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight));
	     double rightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight));
		 double KleftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft-KWIDTH));
	     double KleftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft-KWIDTH));
	     double KrightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight-KWIDTH));
	     double KrightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight-KWIDTH));

		 double Kerb1leftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft+KERB_1_WIDTH));
	     double Kerb1leftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft+KERB_1_WIDTH));
	     double Kerb1rightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight+KERB_1_WIDTH));
	     double Kerb1rightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight+KERB_1_WIDTH));

		 double Kerb2leftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft+KERB_2_WIDTH));
	     double Kerb2leftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft+KERB_2_WIDTH));
	     double Kerb2rightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight+KERB_2_WIDTH));
	     double Kerb2rightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight+KERB_2_WIDTH));



           
	     // track parts
	     double leftSideNX = nextxpos+getLeftSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()));
	     double leftSideNY = nextypos+getLeftSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()));   
	     double rightSideNX = nextxpos+getRightSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()));
	     double rightSideNY = nextypos+getRightSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()));
		 double KleftSideNX = nextxpos+getLeftSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()-KWIDTH));
	     double KleftSideNY = nextypos+getLeftSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()-KWIDTH));   
	     double KrightSideNX = nextxpos+getRightSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()-KWIDTH));
	     double KrightSideNY = nextypos+getRightSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()-KWIDTH));


		 double Kerb1leftSideNX = nextxpos+getLeftSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+KERB_1_WIDTH));
	     double Kerb1leftSideNY = nextypos+getLeftSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+KERB_1_WIDTH));   
	     double Kerb1rightSideNX = nextxpos+getRightSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+KERB_1_WIDTH));
	     double Kerb1rightSideNY = nextypos+getRightSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+KERB_1_WIDTH));
		 double Kerb2leftSideNX = nextxpos+getLeftSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+KERB_2_WIDTH));
	     double Kerb2leftSideNY = nextypos+getLeftSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+KERB_2_WIDTH));   
	     double Kerb2rightSideNX = nextxpos+getRightSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+KERB_2_WIDTH));
	     double Kerb2rightSideNY = nextypos+getRightSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+KERB_2_WIDTH));



		 


		   int leftWall   =  prev->getLeftToBank()*0x40;
	       int rightWall  =  prev->getRightToBank()*0x40;

	       int EleftWall  = t->getLeftToBank()*0x40;
	       int ErightWall = t->getRightToBank()*0x40;
		  
           // track parts
           double WleftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+leftWall));
           double WleftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+leftWall));
		   double WrightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+rightWall));
           double WrightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+rightWall));
 
           // track parts
           double WleftSideNX = nextxpos+getLeftSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+EleftWall));
           double WleftSideNY = nextypos+getLeftSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+EleftWall));           
           double WrightSideNX = nextxpos+getRightSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+ErightWall));
           double WrightSideNY = nextypos+getRightSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+ErightWall));


		 
		 if (mytrack->showTextures){ 
			 glEnable(GL_TEXTURE_2D);
			 RoadBits = TextureMap(GetDC(),RoadBmp,RoadBits,FALSE);
	     }
		 //road
		 glColor3f(0.6f,0.6f,0.6f);
		 glBegin(GL_POLYGON); 
		 glTexCoord2f(0.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(leftSideX),GET_GL_PT_Y(leftSideY),GET_GL_PT_Z(zpos));
		 glTexCoord2f(1.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(leftSideNX),GET_GL_PT_Y(leftSideNY),GET_GL_PT_Z(nextzpos));		 		 
	  	 glTexCoord2f(1.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(rightSideNX),GET_GL_PT_Y(rightSideNY),GET_GL_PT_Z(nextzpos));
		 glTexCoord2f(0.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(rightSideX),GET_GL_PT_Y(rightSideY),GET_GL_PT_Z(zpos));
		 glEnd();

		 if (mytrack->showTextures){ 
			 glEnable(GL_TEXTURE_2D);
			 VergeLeftBits = TextureMap(GetDC(),VergeLeftBmp,VergeLeftBits,FALSE);
	     }

		 // left bank
		 glColor3f(0.0f,0.6f,0.0f);
		 glBegin(GL_POLYGON);
		 glTexCoord2f(0.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(leftSideX),GET_GL_PT_Y(leftSideY),GET_GL_PT_Z(zpos));
		 glTexCoord2f(0.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(WleftSideX),GET_GL_PT_Y(WleftSideY),GET_GL_PT_Z(zpos));
		 glTexCoord2f(1.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(WleftSideNX),GET_GL_PT_Y(WleftSideNY),GET_GL_PT_Z(nextzpos));
		 glTexCoord2f(1.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(leftSideNX),GET_GL_PT_Y(leftSideNY),GET_GL_PT_Z(nextzpos));		 		 	 
		 glEnd();

		 
		 if (mytrack->showTextures){ 
			 glEnable(GL_TEXTURE_2D);
			 VergeRightBits= TextureMap(GetDC(),VergeRightBmp,VergeRightBits,FALSE);
	     }

		 // right bank
		 glColor3f(0.0f,0.6f,0.0f);
		 glBegin(GL_POLYGON);
		 glTexCoord2f(0.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(rightSideX),GET_GL_PT_Y(rightSideY),GET_GL_PT_Z(zpos));
		 glTexCoord2f(1.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(rightSideNX),GET_GL_PT_Y(rightSideNY),GET_GL_PT_Z(nextzpos));		 
		 glTexCoord2f(1.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(WrightSideNX),GET_GL_PT_Y(WrightSideNY),GET_GL_PT_Z(nextzpos));
		 

		 glTexCoord2f(0.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(WrightSideX),GET_GL_PT_Y(WrightSideY),GET_GL_PT_Z(zpos));
		 
		 glEnd();

		 if (mytrack->showWalls)
		 {
		  double wall_height = -0.3;
          if (!t->isLeftWallRemoved())
		  {
		  if (mytrack->showTextures){ 
			 glEnable(GL_TEXTURE_2D);
			 FenceLeftBits = TextureMap(GetDC(),FenceLeftBmp,FenceLeftBits,FALSE);
	      }
		  
		  // left wall
		  glColor3f(0.8f,0.8f,0.8f);
		  glBegin(GL_POLYGON);
		  glTexCoord2f(0.0f,1.0f);
		  glVertex3d(GET_GL_PT_X(WleftSideX),GET_GL_PT_Y(WleftSideY),GET_GL_PT_Z(zpos));
		  glTexCoord2f(0.0f,0.0f);
		  glVertex3d(GET_GL_PT_X(WleftSideX),GET_GL_PT_Y(WleftSideY),GET_GL_PT_Z(zpos+wall_height));
		  glTexCoord2f(1.0f,0.0f);
		  glVertex3d(GET_GL_PT_X(WleftSideNX),GET_GL_PT_Y(WleftSideNY),GET_GL_PT_Z(nextzpos+wall_height));
		  glTexCoord2f(1.0f,1.0f);
		  glVertex3d(GET_GL_PT_X(WleftSideNX),GET_GL_PT_Y(WleftSideNY),GET_GL_PT_Z(nextzpos));		 		 
		  glEnd();
		 }

		 if (!t->isRightWallRemoved())
		 {
		  if (mytrack->showTextures){ 
			 glEnable(GL_TEXTURE_2D);
			 FenceRightBits = TextureMap(GetDC(),FenceRightBmp,FenceRightBits,FALSE);
	      }

		  // right wall
		  glColor3f(0.8f,0.8f,0.8f);
		  glBegin(GL_POLYGON);
		  glTexCoord2f(1.0f,1.0f);
		  glVertex3d(GET_GL_PT_X(WrightSideX),GET_GL_PT_Y(WrightSideY),GET_GL_PT_Z(zpos));
		  glTexCoord2f(0.0f,1.0f);
		  glVertex3d(GET_GL_PT_X(WrightSideNX),GET_GL_PT_Y(WrightSideNY),GET_GL_PT_Z(nextzpos));		 		 
		  glTexCoord2f(0.0f,0.0f);
		  glVertex3d(GET_GL_PT_X(WrightSideNX),GET_GL_PT_Y(WrightSideNY),GET_GL_PT_Z(nextzpos+wall_height));
		  glTexCoord2f(1.0f,0.0f);
		  glVertex3d(GET_GL_PT_X(WrightSideX),GET_GL_PT_Y(WrightSideY),GET_GL_PT_Z(zpos+wall_height));
		  glEnd();
		 }
		 }

		 

		 if (t->isLeftKerb())
		 {
		   if (mytrack->showTextures){ 
			 glEnable(GL_TEXTURE_2D);
			 Kerb1Bits = TextureMap(GetDC(),Kerb1Bmp,Kerb1Bits,FALSE);
	       }
		 // red kerb
		 glColor3f(1.0f,0.0f,0.0f);
		 glBegin(GL_POLYGON);		 
		 glTexCoord2f(0.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(leftSideX),GET_GL_PT_Y(leftSideY),GET_GL_PT_Z(zpos));		 
		 glTexCoord2f(0.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(Kerb1leftSideX),GET_GL_PT_Y(Kerb1leftSideY),GET_GL_PT_Z(zpos-KERB_1_HEIGHT));		 
		 glTexCoord2f(1.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(Kerb1leftSideNX),GET_GL_PT_Y(Kerb1leftSideNY),GET_GL_PT_Z(nextzpos-KERB_1_HEIGHT));		 
		 glTexCoord2f(1.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(leftSideNX),GET_GL_PT_Y(leftSideNY),GET_GL_PT_Z(nextzpos));		 		 	 
		 glEnd();

		 // blue kerb
		 glColor3f(0.0f,0.0f,1.0f);
		 glBegin(GL_POLYGON);	
		 glTexCoord2f(0.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(Kerb1leftSideX),GET_GL_PT_Y(Kerb1leftSideY),GET_GL_PT_Z(zpos-KERB_1_HEIGHT));		 
		 glTexCoord2f(0.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(Kerb2leftSideX),GET_GL_PT_Y(Kerb2leftSideY),GET_GL_PT_Z(zpos-KERB_2_HEIGHT));		 
		 glTexCoord2f(1.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(Kerb2leftSideNX),GET_GL_PT_Y(Kerb2leftSideNY),GET_GL_PT_Z(nextzpos-KERB_2_HEIGHT));		 
		 glTexCoord2f(1.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(Kerb1leftSideNX),GET_GL_PT_Y(Kerb1leftSideNY),GET_GL_PT_Z(nextzpos-KERB_1_HEIGHT));		 		 	 		 
		 glEnd();


		 }

		 if (t->isRightKerb())
		 {
		   if (mytrack->showTextures){ 
			 glEnable(GL_TEXTURE_2D);
			 Kerb1Bits = TextureMap(GetDC(),Kerb1Bmp,Kerb1Bits,FALSE);
	       }
		 glColor3f(1.0f,0.0f,0.0f);
		 glBegin(GL_POLYGON);		 
		 glTexCoord2f(0.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(rightSideX),GET_GL_PT_Y(rightSideY),GET_GL_PT_Z(zpos));		 
		 glTexCoord2f(0.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(Kerb1rightSideX),GET_GL_PT_Y(Kerb1rightSideY),GET_GL_PT_Z(zpos-KERB_1_HEIGHT));		 
		 glTexCoord2f(1.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(Kerb1rightSideNX),GET_GL_PT_Y(Kerb1rightSideNY),GET_GL_PT_Z(nextzpos-KERB_1_HEIGHT));		 
		 glTexCoord2f(1.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(rightSideNX),GET_GL_PT_Y(rightSideNY),GET_GL_PT_Z(nextzpos));		 		 	 
		 glEnd();

		 glColor3f(0.0f,0.0f,1.0f);
		 glBegin(GL_POLYGON);		 
		 glTexCoord2f(0.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(Kerb1rightSideX),GET_GL_PT_Y(Kerb1rightSideY),GET_GL_PT_Z(zpos-KERB_1_HEIGHT));		 
		 glTexCoord2f(0.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(Kerb2rightSideX),GET_GL_PT_Y(Kerb2rightSideY),GET_GL_PT_Z(zpos-KERB_2_HEIGHT));		 
		 glTexCoord2f(1.0f,1.0f);
		 glVertex3d(GET_GL_PT_X(Kerb2rightSideNX),GET_GL_PT_Y(Kerb2rightSideNY),GET_GL_PT_Z(nextzpos-KERB_2_HEIGHT));		 
		 glTexCoord2f(1.0f,0.0f);
		 glVertex3d(GET_GL_PT_X(Kerb1rightSideNX),GET_GL_PT_Y(Kerb1rightSideNY),GET_GL_PT_Z(nextzpos-KERB_1_HEIGHT));		 		 	 
		 glEnd();

		 }

		 glDisable(GL_TEXTURE_2D);


		 

		 

		 

		 // white kerb
		 glColor3f(1.0f,1.0f,1.0f);
		 glBegin(GL_POLYGON);		 
		 glVertex3d(GET_GL_PT_X(leftSideX),GET_GL_PT_Y(leftSideY),GET_GL_PT_Z(zpos-KDIFF));		 
		 glVertex3d(GET_GL_PT_X(leftSideNX),GET_GL_PT_Y(leftSideNY),GET_GL_PT_Z(nextzpos-KDIFF));		 		 	 
		 glVertex3d(GET_GL_PT_X(KleftSideNX),GET_GL_PT_Y(KleftSideNY),GET_GL_PT_Z(nextzpos-KDIFF));		 
		 glVertex3d(GET_GL_PT_X(KleftSideX),GET_GL_PT_Y(KleftSideY),GET_GL_PT_Z(zpos-KDIFF));		 			 
		 glEnd();

		 // white kerb
		 glBegin(GL_POLYGON);
		 glVertex3d(GET_GL_PT_X(rightSideX),GET_GL_PT_Y(rightSideY),GET_GL_PT_Z(zpos-KDIFF));
		 glVertex3d(GET_GL_PT_X(KrightSideX),GET_GL_PT_Y(KrightSideY),GET_GL_PT_Z(zpos-KDIFF));
		 glVertex3d(GET_GL_PT_X(KrightSideNX),GET_GL_PT_Y(KrightSideNY),GET_GL_PT_Z(nextzpos-KDIFF));
		 glVertex3d(GET_GL_PT_X(rightSideNX),GET_GL_PT_Y(rightSideNY),GET_GL_PT_Z(nextzpos-KDIFF));		 
		 glEnd();

		 

		 glColor3f(0.0f,0.0f,0.0f);

		 if (mytrack->showObjects)
		 {
		 Vector *TrackSideObjects = t->getObjects();
		 int numObjects = TrackSideObjects->size();
         for(int i=0;i<numObjects;i++)
         {
			 TrackObject *ob = (TrackObject*)TrackSideObjects->elementAt(i);
		     if (t!=NULL)
		     {
               double trackWidth = t->getWidthLeft();
			   double nextypos;
		       double nextxpos;
		       double remainder = ob->getDistance();
		       double length    = t->getLength();
			   double angle     = t->getAngle();
			   double stAngle   = sectorStartAngle;

		       if (angle != 0)
		       {
		         CPoint trackPt = GetTrackPoint(stAngle,angle,remainder);
                 nextypos = trackPt.x;
                 nextxpos = trackPt.y;
	             double deltaAngle = DegToRad(angle*length)*(remainder/length);
	             angle=stAngle+deltaAngle;
		       }
		       else
		       {
		         nextypos = ob->getDistance()*sin(stAngle);
                 nextxpos = ob->getDistance()*cos(stAngle);
		         angle = stAngle;
		        }
		 

				double xr = xpos+nextxpos;
				double yr = ypos+nextypos;
				double obposx;
				double obposy;

				TrackObjectDefinition *obj = ob->getObjectDef();

		
				if (obj!=NULL)
				{
					double distance = obj->getDistance();
					double di = WIDTH_TO_TRACKWIDTH_OBJECTS(fabs(distance));
					if (distance > 0)
					{
					obposx = xr+getRightSideX(angle,di);
					obposy = yr+getRightSideY(angle,di);
					}
					else
					{
					obposx = xr+getLeftSideX(angle,di);
					obposy = yr+getLeftSideY(angle,di);						
					}
		 
					double glxpos =  GET_GL_PT_X(obposx);
					double glypos =  GET_GL_PT_Y(obposy);
					double glzpos =  GET_GL_PT_Z(zpos);

					CDC *pDC = CDC::FromHandle(hdc);
					obj->DrawGL(pDC,glxpos,glypos,glzpos,angle,mytrack->showAxis);
				}
			}
		  }
		 delete TrackSideObjects;
		 }
		}

		if (mytrack->showTrackSide)
		{
		 for(i=to;i<from;i++)
         {
		   TrackSection *t = (TrackSection*)mytrack->PitlaneSections->elementAt(i);		   		   
		   drawTextureSides(t,mytrack);
	     }
		}
}

/*
void CTrackOpenGLView::OnShowTextures() 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	pDoc->OnShowTextures();
	glDeleteLists(TRACK,1);
	InitDisplayList(NULL);
}
*/

void CTrackOpenGLView::OnRefreshTree() 
{
	glDeleteLists(TRACK,1);
	InitDisplayList(NULL);
	InvalidateRect(NULL,FALSE);
}

void CTrackOpenGLView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	if (inited)
	{
	 //glDeleteLists(TRACK,1);
	 //InitDisplayList(NULL);
	 //InvalidateRect(NULL,TRUE);
	}
}

void CTrackOpenGLView::OnShowTextures() 
{
    
}

void CTrackOpenGLView::OnUpdateShowTextures(CCmdUI* pCmdUI) 
{
	
}


void CTrackOpenGLView::OnTexturemapobjects() 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
    GPTrack * mytrack = pDoc->getTrack();
	mytrack->showTextures = !mytrack->showTextures;
	OnRefreshTree();
	pDoc->UpdateAllViews(NULL);
}

void CTrackOpenGLView::OnUpdateTexturemapobjects(CCmdUI* pCmdUI) 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
    GPTrack * mytrack = pDoc->getTrack();
	pCmdUI->SetCheck(mytrack->showTextures);
}

void CTrackOpenGLView::OnShowtrees() 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
    GPTrack * mytrack = pDoc->getTrack();
	mytrack->showExternalObjects = !mytrack->showExternalObjects;
	OnRefreshTree();
	pDoc->UpdateAllViews(NULL);
}

void CTrackOpenGLView::OnUpdateShowtrees(CCmdUI* pCmdUI) 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
    GPTrack * mytrack = pDoc->getTrack();
	pCmdUI->SetCheck(mytrack->showExternalObjects);
}

BOOL CTrackOpenGLView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return COpenGLView::PreCreateWindow(cs);
}
