// InternalView.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "InternalView.h"
#include "TrackEditorDoc.h"
#include "Vertex.h"
#include "SubObject.h"
#include "TrackOpenGLView.h"

#include "RgbPal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef MK_ALT 
#define MK_ALT 0x20
#endif

#ifdef USE_OPEN_GL
#include <gl\glaux.h>
#include <gl\gl.h>
#include <gl\glu.h>
#endif

extern void Mak256SquareImage(CBitmap &jamBitmap,CBitmap &currentImg,int nWidth,int nHeight,int hr,int vr,int ow,int oh);
extern GLvoid CalculateVectorNormal(GLdouble fVert1[], GLdouble fVert2[],GLdouble fVert3[], GLdouble *fNormalX,GLdouble *fNormalY, GLdouble *fNormalZ);

/////////////////////////////////////////////////////////////////////////////
// CInternalView

IMPLEMENT_DYNCREATE(CInternalView, COpenGLView)

char * getFullPathToJamFileName(char *filename)
{
		  static char buffer[512];
		  CString gp2Location = ((CTrackEditorApp*)AfxGetApp())->getGP2Location();

		  wsprintf(buffer,"%s\\%s",gp2Location,filename);
		  return buffer;
}

/*
GLvoid CalculateVectorNormal(GLdouble fVert1[], GLdouble fVert2[],
                             GLdouble fVert3[], GLdouble *fNormalX,
                             GLdouble *fNormalY, GLdouble *fNormalZ)
{
    GLdouble Qx, Qy, Qz, Px, Py, Pz;
 
    Qx = fVert2[0]-fVert1[0];
    Qy = fVert2[1]-fVert1[1];
    Qz = fVert2[2]-fVert1[2];
    Px = fVert3[0]-fVert1[0];
    Py = fVert3[1]-fVert1[1];
    Pz = fVert3[2]-fVert1[2];
 
    *fNormalX = Py*Qz - Pz*Qy;
    *fNormalY = Pz*Qx - Px*Qz;
    *fNormalZ = Px*Qy - Py*Qx;
}

void Mak256SquareImage(CBitmap &jamBitmap,CBitmap &currentImg,int nWidth,int nHeight,int hr,int vr)
{
	   
       CDC memdcA,memdcB;
	   CBitmap *poldbmps,*poldbmpd;
	   // Create a compatible memory DC
       memdcA.CreateCompatibleDC( NULL );
	   memdcB.CreateCompatibleDC( NULL );
	   
 
	   // Select the bitmap into the DC
       poldbmps = memdcA.SelectObject(&jamBitmap);	   
	   poldbmpd = memdcB.SelectObject(&currentImg);

	   int subImageSizeH = (256/hr);
	   int subImageSizeV = (256/vr);

	   for(int i=0;i<hr;i++)
	   {
	       for(int j=0;j<vr;j++)
		   {
			 int xpos = (i*subImageSizeH);
			 int ypos = (j*subImageSizeV);
	         memdcB.StretchBlt( xpos,ypos, subImageSizeH,subImageSizeV, &memdcA, 0,0, nWidth, nHeight,SRCCOPY );
		   }
	   }

	   memdcA.SelectObject( poldbmps );
	   memdcB.SelectObject( poldbmpd );
}
*/


void CInternalView::LoadJamTexture(GPTrack *mytrack,CBitmap &output,int idx,int hres,int vres)
{
	//int vr = vres/256;
	//int hr = hres/256;
	//if (vres == -99 || (vres % 256) != 0) vr = 1;
	//if (hres == -99 || (hres % 256) != 0) hr = 1;

	int vr = 1;
	int hr = 1;

	CBitmap temp;
	output.LoadBitmap(IDB_TESTTEXTURE_256);
	JAM *jam = mytrack->GetJAM(idx);
	if (jam == NULL) return;
	int sidx  = jam->getImageIndex(idx);
	jam->LoadSingleBMPImage(temp,
		  jam->getImageWidth(sidx), 
		  jam->getImageHeight(sidx),sidx);
	Mak256SquareImage(temp,output,jam->getImageWidth(sidx),jam->getImageHeight(sidx),hr,vr,256,256);
}

CInternalView::CInternalView()
{
    testbmp.LoadBitmap(IDB_TESTTEXTURE_256);
	myobject = NULL;
	showText = FALSE;
	

	m_LeftButtonDown = FALSE;

	dist = 12.0;

	m_xRotate = 0;
	m_yRotate = 0;
	m_zRotate = 0;

	m_size = (float)1.5;
	m_exp = 8;

	fov = 60;
}

CInternalView::~CInternalView()
{
}


BEGIN_MESSAGE_MAP(CInternalView, COpenGLView)
	//{{AFX_MSG_MAP(CInternalView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_SHOW_ANNOTATIONS, OnShowAnnotations)
	ON_UPDATE_COMMAND_UI(ID_SHOW_ANNOTATIONS, OnUpdateShowAnnotations)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_MOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_SHOWPOINT_NUMBERS, OnShowpointNumbers)
	ON_COMMAND(ID_SHOW_VERTEXNUMBERS, OnShowVertexnumbers)
	ON_COMMAND(ID_SHOW_POLYGON_NUMBERS, OnShowPolygonNumbers)
	ON_UPDATE_COMMAND_UI(ID_SHOW_POLYGON_NUMBERS, OnUpdateShowPolygonNumbers)
	ON_UPDATE_COMMAND_UI(ID_SHOW_VERTEXNUMBERS, OnUpdateShowVertexnumbers)
	ON_UPDATE_COMMAND_UI(ID_SHOWPOINT_NUMBERS, OnUpdateShowpointNumbers)
	ON_COMMAND(ID_TEXTUREMAPOBJECTS, OnTexturemapobjects)
	ON_UPDATE_COMMAND_UI(ID_TEXTUREMAPOBJECTS, OnUpdateTexturemapobjects)
	ON_COMMAND(ID_DRAWAXIS, OnDrawaxis)
	ON_UPDATE_COMMAND_UI(ID_DRAWAXIS, OnUpdateDrawaxis)
	ON_COMMAND(ID_SHOWPOINTS, OnShowpoints)
	ON_UPDATE_COMMAND_UI(ID_SHOWPOINTS, OnUpdateShowpoints)
	ON_COMMAND(ID_SHOWLINES, OnShowlines)
	ON_UPDATE_COMMAND_UI(ID_SHOWLINES, OnUpdateShowlines)
	ON_COMMAND(ZOOM_IN, OnIn)
	ON_COMMAND(ZOOM_OUT, OnOut)
	ON_COMMAND(ID_ZOOM_HOME, OnZoomHome)
	ON_COMMAND(ID_SHOWNORMALS, OnShownormals)
	ON_UPDATE_COMMAND_UI(ID_SHOWNORMALS, OnUpdateShownormals)
	ON_COMMAND(ID_SHOWCULLING, OnShowculling)
	ON_UPDATE_COMMAND_UI(ID_SHOWCULLING, OnUpdateShowculling)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInternalView drawing

void CInternalView::OnInitialUpdate()
{
	COpenGLView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 500;
	SetScrollSizes(MM_TEXT, sizeTotal);

	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	myobject = pDoc->getCurrentObject();
	myobject->CreateObject();
	//update();
}

/////////////////////////////////////////////////////////////////////////////
// CInternalView diagnostics

#ifdef _DEBUG
void CInternalView::AssertValid() const
{
	COpenGLView::AssertValid();
}

void CInternalView::Dump(CDumpContext& dc) const
{
	COpenGLView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInternalView message handlers

void CInternalView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_LeftButtonDown = TRUE;
	m_LeftDownPos = point;
	m_ShiftDown  = (nFlags & MK_CONTROL) ? TRUE : FALSE;
	COpenGLView::OnLButtonDown(nFlags, point);
}

void CInternalView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_LeftButtonDown = FALSE;
	m_ShiftDown = FALSE;
	InvalidateRect(NULL,FALSE);
	COpenGLView::OnLButtonUp(nFlags, point);
}

void CInternalView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_LeftButtonDown)
	 {
		CSize rotate = m_LeftDownPos - point;
		m_LeftDownPos = point;
		
		m_xRotate -= rotate.cy;
		if (m_ShiftDown)
		{
		  m_zRotate -= rotate.cx;
		}
		else
		{
		  m_yRotate -= rotate.cx;
		}
		InvalidateRect(NULL,FALSE);
	 }
	COpenGLView::OnMouseMove(nFlags, point);
}

void CInternalView::repaint()
{
	RECT rect;

	GetClientRect(&rect);
	InvalidateRect(&rect);
}

void CInternalView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	repaint();	
}

void CInternalView::OnShowAnnotations() 
{
	showText = ! showText;
}

void CInternalView::OnUpdateShowAnnotations(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(showText);
}

int CInternalView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COpenGLView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void DrawText2D(CDC *dc,COpenGLView *scview,GLdouble wx,GLdouble wy,GLdouble wz,int idx)
{
        GLdouble sx;
		GLdouble sy;
		GLdouble sz;
		GLdouble model[16];
		GLdouble projection[16];
		GLint view[4];

		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glGetDoublev(GL_MODELVIEW_MATRIX,model);
		glGetDoublev(GL_PROJECTION_MATRIX,projection);
		glGetIntegerv(GL_VIEWPORT,view);
		gluProject(wx,wy,wz,model,projection,view,&sx,&sy,&sz);

		char buffer[10];
		wsprintf(buffer,"%d",idx);
		dc->SelectStockObject(ANSI_VAR_FONT);
		//if (!((CTrackEditorView*)(scview))->showFilled)
		//{
		dc->SetBkMode(TRANSPARENT);
		//}

		RECT rect;
		scview->GetClientRect(&rect);

		int midy = (rect.top+rect.bottom)/2;
		int midx = (rect.left+rect.right)/2;

		int winx = (int)(sx-midx);
		int winy = (int)(sy-midy);
		
		dc->TextOut((int)(midx+winx),(int)(midy-winy),buffer);
		glPopMatrix();
}


void CInternalView::OnSize(UINT nType, int cx, int cy) 
{
	COpenGLView::OnSize(nType, cx, cy);
}


void CInternalView::OpenGLDrawScene(HDC hdc)
{
	CheckForOpenGLErrors(__FILE__,__LINE__);


	float x = m_size;


	myobject->XYZData = myobject->getListOffset(myobject->getXYZOffset());
	int objOffset = myobject->XYZData;
    int idx       = objOffset;
	
	double MaxSize= 0.0;
	double XMaxSize= 0.0;
	double YMaxSize= 0.0;
	double ZMaxSize= 0.0;

	int scaleIDx = myobject->getListOffset(myobject->getScaleOffset());

	Point3D **ptArray = (Point3D**)malloc(myobject->Verticies*sizeof(Point3D*));
	for(int i=0;i<myobject->Verticies;i++)
	{
	  ObjectData *Xdat = (ObjectData*)myobject->objectData->elementAt(idx++);	  
	  ObjectData *Ydat = (ObjectData*)myobject->objectData->elementAt(idx++);	  	  	  	  
	  ObjectData *Zdat = (ObjectData*)myobject->objectData->elementAt(idx++);
	  ObjectData *Udat = (ObjectData*)myobject->objectData->elementAt(idx++);

	  if (Xdat)
	  {
	  short X = Xdat->GetValue();
	  int Y = Ydat->GetValue();
	  int Z = Zdat->GetValue();

	  if (Z > 0x8000) Z = 0xFFFF-Z;

      //Z = Z/10;	  

	  Point3D *ptA;

	  int scaleOff = myobject->getScaleOffset();

	  if ((X & 0xFF00)  == 0x8000)
	  {
		  int ptidx = (X & 0xFF);
		  //TRACE("Use previous X,Y = %d %d\n",X,ptidx);
		  ptA = new Point3D(ptArray[ptidx]);
	      ptA->z = (double)Z;	  
	  }
	  else
	  {
		  if (X > 0)
		  {
		   if (X > 0x80 && X < 0xFF)
		   {
		   int id       = ((X-0x84)/2)+scaleOff;
		   int scaleIDx = myobject->getListOffset(id);
		   ObjectData *Xdat = (ObjectData*)myobject->objectData->elementAt(scaleIDx);	  
		   X = Xdat->GetValue();
		   X = -X;
		   }
		   else
		   {
		   int id       = ((X-0x4)/2)+scaleOff;
		   int scaleIDx = myobject->getListOffset(id);
		   ObjectData *Xdat = (ObjectData*)myobject->objectData->elementAt(scaleIDx);	  
		   X = Xdat->GetValue();
		   }
		  }
		  
		  
		  if (Y > 0)
		  {
		   if (Y > 0x80 && Y < 0xFF)
		   {		    		    	        
			int id = ((Y-0x84)/2)+scaleOff;
		    int scaleIDx = myobject->getListOffset(id);
		    ObjectData *Ydat = (ObjectData*)myobject->objectData->elementAt(scaleIDx);
		    Y = Ydat->GetValue();
		    Y = - Y;
		   }
		   else
		   {
			int id = ((Y-0x4)/2)+scaleOff;
		    int scaleIDx = myobject->getListOffset(id);
		    ObjectData *Ydat = (ObjectData*)myobject->objectData->elementAt(scaleIDx);
		    Y = Ydat->GetValue();
		   }
		  }


		  ptA = new Point3D((double)X,(double)Y,(double)Z);	     		  
		  XMaxSize = max(XMaxSize,(double)X);
	      YMaxSize = max(YMaxSize,(double)Y);
	      ZMaxSize = max(ZMaxSize,(double)Z);
	  }

	   //Point3D *ptB = new Point3D(ptA);	  
	   ptArray[i] = ptA;
	  }
	}

	CheckForOpenGLErrors(__FILE__,__LINE__);

	idx  = myobject->getListOffset(myobject->getVertexOffset());

	int connections = myobject->getNumVertexConnections();

	MaxSize = max(XMaxSize,max(YMaxSize,ZMaxSize));

	double SC = (2*x)/MaxSize;

	double midx = SC*(XMaxSize/2);
	double midy = SC*(YMaxSize/2);
	double midz = SC*(ZMaxSize/2);

	
	t_SingleVertex *Verticies = new t_SingleVertex[connections];
	
	for(i=0;i<connections;i++)
	{	    	  
	  ObjectData *PtPt   = (ObjectData*)myobject->objectData->elementAt(idx++);	
	  ObjectData *Unused = (ObjectData*)myobject->objectData->elementAt(idx++);

	  if (PtPt)
	  {
	  int verticies = PtPt->GetValue(); 

	  int pt1 = LOBYTE(verticies);
	  int pt2 = HIBYTE(verticies);

	  if (pt1 < 0 ) pt1 = 0;
	  if (pt1 >= myobject->Verticies ) pt1 = myobject->Verticies-1;

	  if (pt2 < 0 ) pt2 = 0;
	  if (pt2 >= myobject->Verticies ) pt2 = myobject->Verticies-1;

	  Verticies[i].pt1 = pt1;
	  Verticies[i].pt2 = pt2;
	  }
	}

	          

	

	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack * mytrack = pDoc->getTrack();

	
	if (mytrack->showFilledObjects && myobject->polygonPoints!=NULL)
	{
		CheckForOpenGLErrors(__FILE__,__LINE__);
		int size = myobject->polygonPoints->size();
		BOOL transparent = FALSE;

		for(int i=0;i<size;i++)
		{
			Vertex *ver = (Vertex*)myobject->polygonPoints->elementAt(i);
			
			int ap1 = abs(ver->getPoint(0));
			int ap2 = abs(ver->getPoint(1));
			int ap3 = abs(ver->getPoint(2));
			int ap4 = abs(ver->getPoint(3));
			
			if (ver->GetImageLoadState() == FALSE)
			{
			  LoadJamTexture(mytrack,ver->bmp,ver->textureid,ver->m_VerticalRepeat,ver->unk4);
			  ver->SetImageLoadState(TRUE);
			}

			if (mytrack->showTextures && !m_LeftButtonDown)
			{ 
			 glEnable(GL_TEXTURE_2D);
			 CheckForOpenGLErrors(__FILE__,__LINE__);
			 TextureMap(ver->bmp);
		     CheckForOpenGLErrors(__FILE__,__LINE__);
			}
			else
			{
			 glDisable(GL_TEXTURE_2D);
			}

		

			if (transparent)
			{
			float alpha=0.7f;
            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
            glDepthMask(GL_FALSE);// disable depth  buffer writing
            glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);     
            glEnable(GL_BLEND);
            glColor4f(1.0f,1.0f,1.0f,alpha);// note: because of decal mode only alfa is important here
			}
            



			if (mytrack->showCulling)
		{
		 glCullFace(GL_FRONT);
		 glEnable(GL_CULL_FACE);
		}
		else
		{
		 glDisable(GL_CULL_FACE);
		}
	
			//
	

			int colorIdx = (i % numRandomColors);
			t_Color currentColor = RandomColors[colorIdx];

			  GLdouble norm[6][3];
			  int ptcount=0;
			  GLdouble nx,ny,nz;

			  // calculate normal first
			  for(int i=0;i<ver->points;i++)
			  {
				int ap1 = ver->getPoint(i);
				if (ap1 == -32768) continue;
				int abap1 = abs(ap1);
				if (abap1 >= connections) continue;
				
				int vidx = ap1 >= 0 ? (Verticies[abap1].pt1) : (Verticies[abap1].pt2);
				norm[ptcount][0] = ptArray[vidx]->x;
				norm[ptcount][1] = ptArray[vidx]->y;
				norm[ptcount][2] = ptArray[vidx]->z;
				ptcount++;
			  }

			  if (ver->points >=3)
			  {			    	
				CalculateVectorNormal(norm[0],norm[1],norm[2],&nx,&ny,&nz);
				nx = nx/100;
			    ny = ny/100;
			    nz = nz/100;
				glNormal3d(nx,ny,ny);
			  }
			// Ok now you can draw the polygon
			
			glBegin(GL_POLYGON);
			glColor3f (currentColor.r, currentColor.g, currentColor.b);
			ver->cx = 0;
			ver->cy = 0;
			ver->cz = 0;

			BOOL ignoreNextTexCoord=FALSE;
			int  cornerProcessed=0;
			for(i=0;i<ver->points;i++)
			{
				int ap1 = ver->getPoint(i);
				int abap1 = abs(ap1);
				if (abap1 == 32768) {
					ignoreNextTexCoord=TRUE;
					continue;
				}
				//if (abap1 > myobject->Verticies) continue;
				if (abap1 >= connections) continue;
				
				
				int vidx = ap1 >= 0 ? (Verticies[abap1].pt1) : (Verticies[abap1].pt2);
				double dx1 = ((double)(ptArray[vidx]->x)*SC);
			    double dy1 = ((double)(ptArray[vidx]->y)*SC);
		        double dz1 = ((double)(ptArray[vidx]->z)*SC);
				
				float hr = ver->m_HorizontalRepeat/256.0;
				float vr = ver->m_VerticalRepeat/256.0;
				if (ver->m_HorizontalRepeat == -99 ) hr = 1.0;
				if (ver->m_HorizontalRepeat == -99 ) vr = 1.0;
				if (!ignoreNextTexCoord)
				{				 
				 switch(cornerProcessed)
				 {
				  default:
				  case 0:glTexCoord2f(hr*1.0f,0.0f);break;
				  case 1:glTexCoord2f(0.0f,0.0f);break;
				  case 2:
					  if (ver->points == 3)
					  {
					  glTexCoord2f(hr*0.5f,vr*1.0f);
					  }
					  else
					  {
					  glTexCoord2f(0.0f,vr*1.0f);
					  }
					  break;
				  case 3:glTexCoord2f(hr*1.0f,vr*1.0f);break;
				 }
				 cornerProcessed++;
				
			    glVertex3d(dx1,dy1,dz1);
				if (i>0)
				{
				 ver->cx += dx1;
				 ver->cy += dy1;
				 ver->cz += dz1;
				 ver->cx /= 2.0;
				 ver->cy /= 2.0;
				 ver->cz /= 2.0;
				}
				}
				else
				{
					ignoreNextTexCoord=FALSE;
				}
			}
			
			glEnd();
			if (mytrack->showNormals)
	        {
		      glColor3d(0.0f,0.0f,0.0f);
		      glBegin(GL_LINES);
		      glVertex3d(ver->cx,ver->cy,ver->cz);
		      glVertex3d(ver->cx+(0.00001*nx),ver->cy+(0.00001*ny),ver->cz+(0.00001*nz));
		      glEnd();
	        }
		}
		if (transparent)
	    {
	      glDisable(GL_BLEND);
          glDepthMask(TRUE);
	    }
	}	

	

	glDisable(GL_TEXTURE_2D);

	//if (showNormals)
	

	if (mytrack->showLines)
	{
	for(i=0;i<connections;i++)
	 {
		glColor3d(0.0f,0.0f,0.0f);
		int pt1 = Verticies[i].pt1;
	    int pt2 = Verticies[i].pt2;
	  	glBegin(GL_LINES);
		glVertex3d( ((double)(ptArray[pt1]->x)*SC),  ((double)(ptArray[pt1]->y)*SC), ((double)(ptArray[pt1]->z)*SC));
		glVertex3d( ((double)(ptArray[pt2]->x)*SC),  ((double)(ptArray[pt2]->y)*SC), ((double)(ptArray[pt2]->z)*SC));
	  	glEnd();
	  }
	}

	if (mytrack->showPoints)
	{
		 glPointSize(5.0f);
		 
		 glBegin(GL_POINTS);

		 for(i=0;i<myobject->Verticies;i++)
		 {	    	
		  glColor3f(1.0f,0.0f,0.0f);
		  //int pt1 = Verticies[i].pt1;
	      //int pt2 = Verticies[i].pt2;
		  glVertex3d( ((double)(ptArray[i]->x)*SC),  ((double)(ptArray[i]->y)*SC), ((double)(ptArray[i]->z)*SC));
		  //glVertex3d( ((double)(ptArray[pt2]->x)*SC),  ((double)(ptArray[pt2]->y)*SC), ((double)(ptArray[pt2]->z)*SC));
		 }
		 glEnd();
    }

		// draw carthesian axes
	if (mytrack->showAxis)
	{
	    glColor3f(1.f,0.f,0.f);
	    glBegin(GL_LINES);
		// red x axis		
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(1.0f,0.0f,0.0f);
		glVertex3f(1.0f,0.0f,0.0f);
		glVertex3f(0.9f,0.1f,0.0f);
		glVertex3f(1.0f,0.0f,0.0f);
		glVertex3f(0.9f,-0.1f,0.0f);
		glEnd();
		// green y axis
		glColor3f(0.f,1.f,0.f);
		glBegin(GL_LINES);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,1.0f,0.0f);
		glVertex3f(0.0f,1.0f,0.0f);
		glVertex3f(0.1f,0.9f,0.0f);
		glVertex3f(0.0f,1.0f,0.0f);
		glVertex3f(-0.1f,0.9f,0.0f);
		glEnd();
		// blue z axis
		glColor3f(0.f,0.f,1.f);
		glBegin(GL_LINES);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f,0.1f,0.9f);
		glVertex3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f,-0.1f,0.9f);
	    glEnd();
	}

	//glDisable(GL_TEXTURE_2D);
	if (myobject->subObjects!=NULL)
	{
		double s = 2048;
		double s2 = 2048;
		int width = 2048;
		int height = 2048;
		
		for(int i=0;i<myobject->subObjects->size();i++)
		{
		 SubObject *subObj = (SubObject*)myobject->subObjects->elementAt(i);
		 glPushMatrix();
		 int ptidx =  subObj->unk1;
		 double x = (ptArray[ptidx]->x)*SC;
		 double y = (ptArray[ptidx]->y)*SC;
		 double z = (ptArray[ptidx]->z)*SC;
		 glTranslated(x,y,z);
		 if (mytrack->showTextures)
		 { 
			  CBitmap tree;
			  unsigned char *TreeBits = NULL;

			  int textureId = (subObj->unk3+256);

			  glEnable(GL_TEXTURE_2D);
			  
			  float alpha=0.7f;			   
			  glEnable (GL_ALPHA_TEST);
              glAlphaFunc (GL_GREATER, 0.5f);

			  CTrackOpenGLView::LoadJamTexture(mytrack,tree,textureId,256,256,&width,&height);
			  glColor4f(1.0f,1.0f,1.0f,1.0f);
			  CDC *pDC = CDC::FromHandle(hdc);
              TreeBits = CTrackOpenGLView::TextureMap(pDC,tree,TreeBits,TRUE);
			  delete TreeBits;
			  s  = width*16;
		      s2 = height*16;
	     }		 
		 double scale = 0.001f;
		 glScaled(scale,scale,scale);
		 
		 
		 if (mytrack->showExternalObjects)
		 {
		  glBegin(GL_POLYGON);
		  glTexCoord2f(1.0f,0.0f);
	      glVertex3d(s,0,-s2);
		  glTexCoord2f(0.0f,0.0f);
		  glVertex3d(-s,0,-s2);
		  glTexCoord2f(0.0f,1.0f);
		  glVertex3d(-s,0,s2);
		  glTexCoord2f(1.0f,1.0f);
		  glVertex3d(s,0,s2);
		  glEnd();
		  glDisable (GL_ALPHA_TEST);
		 }
		 glPopMatrix();
	   }
	 }
    glDisable(GL_TEXTURE_2D);
	

	//SwapBuffers(dc->m_ps.hdc);
	SwapBuffers(hdc);

	CDC* dc = CDC::FromHandle(hdc);

	if (mytrack->showVertexNumbers)
	{
     for(i=1;i<connections;i++)
	 {
		int pt1 = Verticies[i].pt1;
		int pt2 = Verticies[i].pt2;
		
		GLdouble wx;
		GLdouble wy;
		GLdouble wz;
		wx = ((double)(ptArray[pt1]->x)*SC);
		wy = ((double)(ptArray[pt1]->y)*SC);
		wz = ((double)(ptArray[pt1]->z)*SC);

		GLdouble wx1;
		GLdouble wy1;
		GLdouble wz1;
		wx1 = ((double)(ptArray[pt2]->x)*SC);
		wy1 = ((double)(ptArray[pt2]->y)*SC);
		wz1 = ((double)(ptArray[pt2]->z)*SC);

		wx = (wx+wx1)/2;
		wy = (wy+wy1)/2;
		wz = (wz+wz1)/2;

		DrawText2D(dc,this,wx,wy,wz,i);		
	 }
	}

	if (mytrack->showPointNumbers)
	{
	 dc->SetTextColor(RGB(0xFF,0,0));			
	 for(i=0;i<myobject->Verticies;i++)
	 {
		GLdouble wx;
		GLdouble wy;
		GLdouble wz;
		wx = ((double)(ptArray[i]->x)*SC);
		wy = ((double)(ptArray[i]->y)*SC);
		wz = ((double)(ptArray[i]->z)*SC);
		DrawText2D(dc,this,wx,wy,wz,i);		
	 }
	}

	if (mytrack->showPolygonNumbers)
	{
	  dc->SetTextColor(RGB(0,0,0xFF));
	  if (myobject->polygonPoints!=NULL)
	  {
		int size = myobject->polygonPoints->size();

		for(int i=0;i<size;i++)
		{
			Vertex *ver = (Vertex*)myobject->polygonPoints->elementAt(i);
			DrawText2D(dc,this,ver->cx,ver->cy,ver->cz,i);
		}
		
		}
	}

	

	for(i=0;i<myobject->Verticies;i++)
	{
		delete ptArray[i];
	}

	free(ptArray);
	delete 	Verticies;
}



void CInternalView::OnDestroy() 
{
	COpenGLView::OnDestroy();
	

	if(wglGetCurrentContext() != NULL)
		wglMakeCurrent(NULL,NULL);

	if(m_hGLContext != NULL)
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}
}


void CInternalView::OnMove(int x, int y) 
{
	COpenGLView::OnMove(x, y);
}

void CInternalView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	COpenGLView::OnRButtonDown(nFlags, point);
}

void CInternalView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	COpenGLView::OnRButtonUp(nFlags, point);
}


void CInternalView::TextureMap(CBitmap &cbmp)
{
  BITMAP bmp;
  cbmp.GetBitmap( &bmp );

  if (bmp.bmWidth > 1024) return;
  if (bmp.bmHeight > 1024) return;

  CDC*          bitmapDc = new CDC;
  bitmapDc -> CreateCompatibleDC(  GetDC() );

  CBitmap* oldBitMap = (CBitmap*) bitmapDc -> SelectObject( &cbmp );

  unsigned char *lpvBits = new unsigned char[bmp.bmWidth * bmp.bmHeight *3];
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

  for(unsigned int i=0;i<size;i+=3)
  {
	  unsigned char *ptr =lpvBits+i;
	  unsigned char *r = ptr;
	  unsigned char *g = ptr+1;
	  unsigned char *b = ptr+2;

	  unsigned char red = *r;
	  unsigned char blue = *b;
	  *b = red;
	  *r = blue;
  }


  GLint val;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE,&val);
  //TRACE("%d %d %d %d\n",bmp.bmWidth, bmp.bmHeight,val,GL_RGB);
  //CheckForOpenGLErrors(__FI92LE__,__LINE__);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, bmp.bmWidth, bmp.bmHeight, 0,GL_RGB, GL_UNSIGNED_BYTE, lpvBits);
  //CheckForOpenGLErrors(__FILE__,__LINE__);




  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  //CheckForOpenGLErrors(__FILE__,__LINE__);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //CheckForOpenGLErrors(__FILE__,__LINE__);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //CheckForOpenGLErrors(__FILE__,__LINE__);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  //CheckForOpenGLErrors(__FILE__,__LINE__);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  //CheckForOpenGLErrors(__FILE__,__LINE__);
  glEnable(GL_TEXTURE_2D);
  //CheckForOpenGLErrors(__FILE__,__LINE__);
  glShadeModel(GL_SMOOTH);
  //CheckForOpenGLErrors(__FILE__,__LINE__);

  bitmapDc -> SelectObject( oldBitMap );
  delete bitmapDc;
  delete[] lpvBits;
  delete bmpinfo;
}





void CInternalView::OnShowpointNumbers() 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack * mytrack = pDoc->getTrack();
	mytrack->showPointNumbers = ! mytrack->showPointNumbers;
	InvalidateRect(NULL,FALSE);
}

void CInternalView::OnShowVertexnumbers() 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack * mytrack = pDoc->getTrack();
	mytrack->showVertexNumbers = ! mytrack->showVertexNumbers;
	InvalidateRect(NULL,FALSE);
}

void CInternalView::OnShowPolygonNumbers() 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack * mytrack = pDoc->getTrack();
	mytrack->showPolygonNumbers = ! mytrack->showPolygonNumbers;
	InvalidateRect(NULL,FALSE);
}

void CInternalView::OnUpdateShowPolygonNumbers(CCmdUI* pCmdUI) 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack * mytrack = pDoc->getTrack();
	pCmdUI->SetCheck(mytrack->showPolygonNumbers);
}

void CInternalView::OnUpdateShowVertexnumbers(CCmdUI* pCmdUI) 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack * mytrack = pDoc->getTrack();
	pCmdUI->SetCheck(mytrack->showVertexNumbers);
}

void CInternalView::OnUpdateShowpointNumbers(CCmdUI* pCmdUI) 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack * mytrack = pDoc->getTrack();
	pCmdUI->SetCheck(mytrack->showPointNumbers);
}

void CInternalView::OnTexturemapobjects() 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack *track = pDoc->getTrack();
	track->showTextures = !track->showTextures;
	InvalidateRect(NULL,FALSE);
}


void CInternalView::OnUpdateTexturemapobjects(CCmdUI* pCmdUI) 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack *mytrack = pDoc->getTrack();
	pCmdUI->SetCheck(mytrack->showTextures);
}

void CInternalView::OnDrawaxis() 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack *mytrack = pDoc->getTrack();
	mytrack->showAxis = !mytrack->showAxis;
	InvalidateRect(NULL,FALSE);
}

void CInternalView::OnUpdateDrawaxis(CCmdUI* pCmdUI) 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack *mytrack = pDoc->getTrack();
	pCmdUI->SetCheck(mytrack->showAxis);
}

void CInternalView::OnShowpoints() 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack *mytrack = pDoc->getTrack();
	mytrack->showPoints = !mytrack->showPoints;
	InvalidateRect(NULL,FALSE);
}

void CInternalView::OnUpdateShowpoints(CCmdUI* pCmdUI) 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack *mytrack = pDoc->getTrack();
	pCmdUI->SetCheck(mytrack->showPoints);
}

void CInternalView::OnShowlines() 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack *mytrack = pDoc->getTrack();
	mytrack->showLines = !mytrack->showLines;
	InvalidateRect(NULL,FALSE);
}

void CInternalView::OnUpdateShowlines(CCmdUI* pCmdUI) 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack *mytrack = pDoc->getTrack();
	pCmdUI->SetCheck(mytrack->showLines);
}

void CInternalView::OnShownormals() 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack *mytrack = pDoc->getTrack();
	mytrack->showNormals = !mytrack->showNormals;
	InvalidateRect(NULL,FALSE);
}

void CInternalView::OnUpdateShownormals(CCmdUI* pCmdUI) 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack *mytrack = pDoc->getTrack();
	pCmdUI->SetCheck(mytrack->showNormals);
}

void CInternalView::OnShowculling() 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack *mytrack = pDoc->getTrack();
	mytrack->showCulling = !mytrack->showCulling;
	InvalidateRect(NULL,FALSE);
}

void CInternalView::OnUpdateShowculling(CCmdUI* pCmdUI) 
{
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack *mytrack = pDoc->getTrack();
	pCmdUI->SetCheck(mytrack->showCulling);
}

void CInternalView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	COpenGLView::OnPrint(pDC, pInfo);
}

void CInternalView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	COpenGLView::OnEndPrinting(pDC, pInfo);
}

void CInternalView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	COpenGLView::OnEndPrintPreview(pDC, pInfo, point, pView);
}

void CInternalView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	COpenGLView::OnBeginPrinting(pDC, pInfo);
}

BOOL CInternalView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return COpenGLView::OnPreparePrinting(pInfo);
}
