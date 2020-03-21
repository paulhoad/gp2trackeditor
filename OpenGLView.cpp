// OpenGLView.cpp : implementation file
//

#include "stdafx.h"
#include "OpenGLView.h"
#include "RgbPal.h"
#include "Pallette.h"
#include "resource.h"
#include "TrackEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <gl\glaux.h>
#include <gl\gl.h>
#include <gl\glu.h>


void setupDIB(HDC hDC);
void setupPalette(HDC hDC);
void resizeDIB(HDC hDC);


int winWidth = 512, winHeight = 256;
HBITMAP hBitmap, hOldBitmap;
HPALETTE hPalette;
HDC hDC;

void
resize(void)
{
    //setProjection();
    //glViewport(0, 0, winWidth, winHeight);
}

/* Struct used to manage color ramps */
struct colorIndexState {
    GLfloat amb[3];     /* ambient color / bottom of ramp */
    GLfloat diff[3];    /* diffuse color / middle of ramp */
    GLfloat spec[3];    /* specular color / top of ramp */
    GLfloat ratio;      /* ratio of diffuse to specular in ramp */
    GLint indexes[3];   /* where ramp was placed in palette */
};

/*
** Each entry in this array corresponds to a color ramp in the
** palette.  The indexes member of each struct is updated to
** reflect the placement of the color ramp in the palette.
*/
#define NUM_COLORS (sizeof(colors) / sizeof(colors[0]))
struct colorIndexState colors[] = {
    {
        { 0.0F, 0.0F, 0.0F },
        { 0.1F, 0.6F, 0.3F },
        { 1.0F, 1.0F, 1.0F },
        0.75F, { 0, 0, 0 },
    },
    {
        { 0.0F, 0.0F, 0.0F },
        { 0.0F, 0.2F, 0.5F },
        { 1.0F, 1.0F, 1.0F },
        0.75F, { 0, 0, 0 },
    },
    {
        { 0.0F, 0.05F, 0.05F },
        { 0.6F, 0.0F, 0.8F },
        { 1.0F, 1.0F, 1.0F },
        0.75F, { 0, 0, 0 },
    },
};


/////////////////////////////////////////////////////////////////////////////
// COpenGLView

IMPLEMENT_DYNCREATE(COpenGLView, CScrollView)

COpenGLView::COpenGLView()
{
	m_LeftButtonDown = FALSE; 
	scaler=1.0;
	dist=50.0;
	renderToDIB = FALSE;
	fov = 45;

	m_xRotate = -70;
	m_yRotate = 0;
	m_zRotate = 140;

}

COpenGLView::~COpenGLView()
{
}


BEGIN_MESSAGE_MAP(COpenGLView, CScrollView)
	//{{AFX_MSG_MAP(COpenGLView)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_COMMAND(ZOOM_IN, OnIn)
	ON_COMMAND(ZOOM_OUT, OnOut)
	ON_COMMAND(ID_ZOOM_HOME, OnZoomHome)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenGLView drawing

void COpenGLView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 500;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void COpenGLView::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
	glClearColor (1.0f, 1.0f, 1.0f, 1.0f);
    glClearDepth (1.0);

	RenderScene(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// COpenGLView diagnostics

#ifdef _DEBUG
void COpenGLView::AssertValid() const
{
	CScrollView::AssertValid();
}

void COpenGLView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COpenGLView message handlers

void COpenGLView::OnDestroy() 
{
	CScrollView::OnDestroy();
	
	if(wglGetCurrentContext() != NULL)
		wglMakeCurrent(NULL,NULL);

	if(m_hGLContext != NULL)
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}
}

int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	//m_pDC = new CClientDC (this);
	
	HWND hWnd = GetSafeHwnd();
	//hDC = ::GetDC(hWnd);
	CDC* cdc = m_pDC = GetDC();
	hDC = cdc->m_hDC;

	m_hGLContext = NULL;
	m_GLPixelIndex = 0;


	

	m_size = (float)1.5;
	m_exp = 8;

	if(SetWindowPixelFormat(hDC)==FALSE)
		return 0;

	int nBPP = ::GetDeviceCaps( hDC,BITSPIXEL ) ;
 
    // If we're running in a palette mode, generate the palette
    if ( nBPP < 16 )
    {
	   CPalette pal;
	   glEnable(GL_DITHER);
       pal.m_hObject = NULL ;
       pal.CreatePalette( (LOGPALETTE *) &rgb8palette ) ;
 
       ::SelectPalette(hDC, (HPALETTE) pal, FALSE ) ;
    }

	//hDCFrontBuffer = hDC;

	if (renderToDIB) {
         //hDC = CreateCompatibleDC(NULL);
         setupDIB(hDC);
    }

	setupPalette(hDC);
	//resize();
	
	if(CreateViewGLContext(hDC)==FALSE)
		return 0;

	COLORREF color = ::GetSysColor(COLOR_WINDOW);
	glClearColor((float)GetRValue(color)/255.0f,
				 (float)GetGValue(color)/255.0f,
				 (float)GetBValue(color)/255.0f,
				 (float)1.0);

	return 0;
}

void COpenGLView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_LeftButtonDown = FALSE;
	m_ShiftDown = FALSE;

	CScrollView::OnLButtonUp(nFlags, point);
}

void COpenGLView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_LeftButtonDown = TRUE;
	m_LeftDownPos = point;
	m_ShiftDown  = (nFlags & MK_CONTROL) ? TRUE : FALSE;

	CScrollView::OnLButtonDown(nFlags, point);
}

void COpenGLView::OnMouseMove(UINT nFlags, CPoint point) 
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

	CString message;
	message.Format("%2.2f %2.2f %2.2f",m_xRotate,m_yRotate,m_zRotate);
	theApp.SetMessageText(message);
	CScrollView::OnMouseMove(nFlags, point);
}

//void COpenGLView::OnPaint() 
//{
	//if (IsIconic())
	//{
	//}
	//else
	//{
	// CPaintDC dc(this); // device context for painting
	// RenderScene(&dc);
	// CScrollView::OnPaint();
	//}
//}

void COpenGLView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);

	
	GLsizei width,height;
	GLdouble aspect;

	width = cx;
	height = cy;

	if(cy==0)
		aspect = (GLdouble)width;
	else
		aspect = (GLdouble)width/(GLdouble)height;

	winWidth = width;
	winHeight = height;

	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov,aspect,0.3,1000.0);
	glMatrixMode(GL_MODELVIEW);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glLoadIdentity();
	
	glDrawBuffer(GL_BACK);

	//glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);


}

BOOL COpenGLView::CreateViewGLContext(HDC hdc)
{

   m_hGLContext = wglCreateContext(hdc);

   if(m_hGLContext==NULL)
   {
	AfxMessageBox("Cannot Make OpenGL Context");
	return FALSE;
   }

   if(wglMakeCurrent(hdc,m_hGLContext)==FALSE)
       return FALSE;

   return TRUE;
}

BOOL COpenGLView::SetWindowPixelFormat(HDC hDC)
{
   PIXELFORMATDESCRIPTOR pixelDesc;

   pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
   pixelDesc.nVersion = 1;

   pixelDesc.dwFlags = PFD_SUPPORT_OPENGL |
					   PFD_STEREO_DONTCARE;

   BOOL  colorIndexMode = FALSE;
   BOOL  doubleBuffered = TRUE;

   pixelDesc.cColorBits = GetDeviceCaps(hDC, BITSPIXEL);

    if (colorIndexMode) {
        pixelDesc.iPixelType = PFD_TYPE_COLORINDEX;
    } else {
        pixelDesc.iPixelType = PFD_TYPE_RGBA;
    }

    if (doubleBuffered) {
        pixelDesc.dwFlags |= PFD_DOUBLEBUFFER;
    }



   if (renderToDIB) {
        pixelDesc.dwFlags |= PFD_DRAW_TO_BITMAP;
    } else {
        pixelDesc.dwFlags |= PFD_DRAW_TO_WINDOW;
   }

   //pixelDesc.iPixelType = PFD_TYPE_RGBA;
   //pixelDesc.iPixelType = PFD_TYPE_COLORINDEX;
   //pixelDesc.cColorBits = 8;
   pixelDesc.cDepthBits = 32;
   //pixelDesc.cStencilBits = 0;


   /*
   pixelDesc.cRedBits = 8;
   pixelDesc.cRedShift = 16;
   pixelDesc.cGreenBits = 8;
   pixelDesc.cGreenShift = 8;
   pixelDesc.cBlueBits = 8;
   pixelDesc.cBlueShift = 0;
   pixelDesc.cAlphaBits = 0;
   pixelDesc.cAlphaShift = 0;
   pixelDesc.cAccumBits = 64;
   pixelDesc.cAccumRedBits = 16;
   pixelDesc.cAccumGreenBits = 16;
   pixelDesc.cAccumBlueBits = 16;
   pixelDesc.cAccumAlphaBits = 0;
   
   pixelDesc.cStencilBits = 8;
   pixelDesc.cAuxBuffers = 0;
   pixelDesc.iLayerType = PFD_MAIN_PLANE;
   pixelDesc.bReserved = 0;
   pixelDesc.dwLayerMask = 0;
   pixelDesc.dwVisibleMask = 0;
   pixelDesc.dwDamageMask = 0;
   */

   m_GLPixelIndex = ChoosePixelFormat(hDC,&pixelDesc);
   if(m_GLPixelIndex==0) // Choose default
   {
	   m_GLPixelIndex = 1;
	   if(DescribePixelFormat(hDC,m_GLPixelIndex,
		sizeof(PIXELFORMATDESCRIPTOR),&pixelDesc)==0)
	   {
		return FALSE;
	   }
   }
   if(SetPixelFormat(hDC,m_GLPixelIndex,&pixelDesc)==FALSE)
   {
	return FALSE;
   }
   return TRUE;
}

void COpenGLView::getLookPoint()
{
   glTranslated(0.0,0.0,-dist);
   glRotated(m_xRotate, 1.0, 0.0, 0.0);
   glRotated(m_yRotate, 0.0, 1.0, 0.0);
   glRotated(m_zRotate, 0.0, 0.0, 1.0);

   /*
   gluLookAt(0.0f,0.0f,0.0f,
			  1.0f,0.0f,0.0f,
			  0.0f,0.0f,1.0f);
   */
}

void COpenGLView::RenderScene(CDC *dc)
{
	// prepare a semaphore
	static BOOL 	bBusy = FALSE;
    // use the semaphore to enter this critic section
	if(bBusy) return;
	bBusy = TRUE;

	GLfloat LightAmbient[] =  { 0.9f, 0.9f, 0.9f, 0.9f};
	GLfloat LightDiffuse[] =  { 0.7f, 0.7f, 0.7f, 0.7f};
	GLfloat LightSpecular[] = { 0.5f, 0.5f, 0.5f, 0.1f};
	GLfloat LightPosition0[] = { 0.0f, 0.0f, 1.0f, 0.0f}; 

	//GLfloat RedSurface[]   = { 1.0f, 0.0f, 0.0f, 1.0f};
	//GLfloat GreenSurface[] = { 0.0f, 1.0f, 0.0f, 1.0f};
	//GLfloat BlueSurface[]  = { 0.0f, 0.0f, 1.0f, 1.0f};
	//GLfloat WhiteSurface[]  = { 1.0f, 1.0f, 1.0f, 1.0f};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT);

	// LIGHT0 parameters
	//glLightfv(GL_LIGHT0,GL_AMBIENT,LightAmbient);
	//glLightfv(GL_LIGHT0,GL_DIFFUSE,LightDiffuse);
	//glLightfv(GL_LIGHT0,GL_SPECULAR,LightSpecular);
	//glLightfv(GL_LIGHT0,GL_POSITION,LightPosition0);
	//glEnable(GL_LIGHT0);
	//GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 0.1 };
    //GLfloat light_diffuse[] = { 0.1, 0.1, 0.1, 0.5 };
    //GLfloat light_specular[] = { 0.1, 0.0, 0.0, 0.1 };
/*      light_position is NOT default value     */
    //GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.1 };

    //glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
    //glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    //glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular);
    //glLightfv (GL_LIGHT0, GL_POSITION, light_position);
    
    //glEnable (GL_LIGHTING);
    //glEnable (GL_LIGHT0);
	//glDepthFunc(GL_LESS);
    //glEnable(GL_DEPTH_TEST);

	//GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    //GLfloat mat_shininess[] = { 50.0 };
    //GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

    //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    //glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    //glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    //glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

	


	glPushMatrix();

	// Position / translation
	//glTranslated(0.0,0.0,-dist);
	//glRotated(m_xRotate, 1.0, 0.0, 0.0);
	//glRotated(m_yRotate, 0.0, 1.0, 0.0);
	//glRotated(m_zRotate, 0.0, 0.0, 1.0);

	getLookPoint();


	//glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_POLYGON_SMOOTH);

	// Material aspect
    GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat mat_ambient_color[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    //GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat high_shininess[] = { 50.0f };
    GLfloat mat_emission[] = {0.2f, 0.2f, 0.2f, 0.0f};

    //glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    //glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    //glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

  
	glPolygonMode(GL_FRONT,GL_FILL);
	glPolygonMode(GL_BACK,GL_FILL);

	float x = m_size;
    glShadeModel(GL_SMOOTH);
	//glEnable(GL_LIGHTING);

	//OpenGLDrawScene(dc->m_ps.hdc);
	OpenGLDrawScene(dc->m_hDC);
	

	//SwapBuffers(dc->m_ps.hdc);

	glPopMatrix();

	bBusy = FALSE;
}


/*
void COpenGLView::OpenGLDrawScene(CPaintDC *dc)
{
	 AfxMessageBox("Empty Base OpenGLDrawScene\n");
}
*/
void COpenGLView::OpenGLDrawScene(HDC hDC)
{
	 AfxMessageBox("Empty Base OpenGLDrawScene\n");
}




void COpenGLView::OnOut() 
{
	scaler *=0.8;
	dist*=1.2;

	InvalidateRect(NULL,FALSE);
}

void COpenGLView::OnIn() 
{
	scaler *=1.2;
	dist*=0.8;
	InvalidateRect(NULL,FALSE);
}

void COpenGLView::OnZoomHome() 
{
	scaler = 1.0;
	dist=50.0;
	InvalidateRect(NULL,FALSE);
}

void setupDIB(HDC hDC)
{
    BITMAPINFO *bmInfo;
    BITMAPINFOHEADER *bmHeader;
    UINT usage;
    VOID *base;
    int bmiSize;
    int bitsPerPixel;

    bmiSize = sizeof(*bmInfo);
    bitsPerPixel = GetDeviceCaps(hDC, BITSPIXEL);

    switch (bitsPerPixel) {
    case 8:
        /* bmiColors is 256 WORD palette indices */
        bmiSize += (256 * sizeof(WORD)) - sizeof(RGBQUAD);
        break;
    case 16:
        /* bmiColors is 3 WORD component masks */
        bmiSize += (3 * sizeof(DWORD)) - sizeof(RGBQUAD);
        break;
    case 24:
    case 32:
    default:
        /* bmiColors not used */
        break;
    }

    bmInfo = (BITMAPINFO *) calloc(1, bmiSize);
    bmHeader = &bmInfo->bmiHeader;

    bmHeader->biSize = sizeof(*bmHeader);
    bmHeader->biWidth = winWidth;
    bmHeader->biHeight = winHeight;
    bmHeader->biPlanes = 1;                     /* must be 1 */
    bmHeader->biBitCount = bitsPerPixel;
    bmHeader->biXPelsPerMeter = 0;
    bmHeader->biYPelsPerMeter = 0;
    bmHeader->biClrUsed = 0;                    /* all are used */
    bmHeader->biClrImportant = 0;               /* all are important */

    switch (bitsPerPixel) {
    case 8:
        bmHeader->biCompression = BI_RGB;
        bmHeader->biSizeImage = 0;
        usage = DIB_PAL_COLORS;
        /* bmiColors is 256 WORD palette indices */
        {
            WORD *palIndex = (WORD *) &bmInfo->bmiColors[0];
            int i;

            for (i=0; i<256; i++) {
                palIndex[i] = i;
            }
        }
        break;
    case 16:
        bmHeader->biCompression = BI_RGB;
        bmHeader->biSizeImage = 0;
        usage = DIB_RGB_COLORS;
        /* bmiColors is 3 WORD component masks */
        {
            DWORD *compMask = (DWORD *) &bmInfo->bmiColors[0];

            compMask[0] = 0xF800;
            compMask[1] = 0x07E0;
            compMask[2] = 0x001F;
        }
        break;
    case 24:
    case 32:
    default:
        bmHeader->biCompression = BI_RGB;
        bmHeader->biSizeImage = 0;
        usage = DIB_RGB_COLORS;
        /* bmiColors not used */
        break;
    }

    hBitmap = CreateDIBSection(hDC, bmInfo, usage, &base, NULL, 0);
    if (hBitmap == NULL) {
        (void) MessageBox(WindowFromDC(hDC),
                "Failed to create DIBSection.",
                "OpenGL application error",
                MB_ICONERROR | MB_OK);
        exit(1);
    }

    hOldBitmap = (HBITMAP)SelectObject(hDC, hBitmap);

	SetPixel(hDC,20,10,RGB(255,255,0));
	SetPixel(hDC,21,10,RGB(255,255,0));

    free(bmInfo);
}

void setupPalette(HDC hDC)
{
    PIXELFORMATDESCRIPTOR pfd;
    LOGPALETTE* pPal;
    int pixelFormat = GetPixelFormat(hDC);
    int paletteSize;

    DescribePixelFormat(hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    /*
    ** Determine if a palette is needed and if so what size.
    */
    if (pfd.dwFlags & PFD_NEED_PALETTE) {
        paletteSize = 1 << pfd.cColorBits;
    } else if (pfd.iPixelType == PFD_TYPE_COLORINDEX) {
        paletteSize = 4096;
    } else {
        return;
    }

    pPal = (LOGPALETTE*)
        malloc(sizeof(LOGPALETTE) + paletteSize * sizeof(PALETTEENTRY));
    pPal->palVersion = 0x300;
    pPal->palNumEntries = paletteSize;

    if (pfd.iPixelType == PFD_TYPE_RGBA) {
        /*
        ** Fill the logical paletee with RGB color ramps
        */
        int redMask = (1 << pfd.cRedBits) - 1;
        int greenMask = (1 << pfd.cGreenBits) - 1;
        int blueMask = (1 << pfd.cBlueBits) - 1;
        int i;

        for (i=0; i<paletteSize; ++i) {
            pPal->palPalEntry[i].peRed =
                    (((i >> pfd.cRedShift) & redMask) * 255) / redMask;
            pPal->palPalEntry[i].peGreen =
                    (((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
            pPal->palPalEntry[i].peBlue =
                    (((i >> pfd.cBlueShift) & blueMask) * 255) / blueMask;
            pPal->palPalEntry[i].peFlags = 0;
        }
    } else {
        /*
        ** Fill the logical palette with color ramps.
        **
        ** Set up the logical palette so that it can be realized
        ** into the system palette as an identity palette.
        **
        ** 1) The default static entries should be present and at the right
        **    location.  The easiest way to do this is to grab them from
        **    the current system palette.
        **
        ** 2) All non-static entries should be initialized to unique values.
        **    The easiest way to do this is to ensure that all of the non-static
        **    entries have the PC_NOCOLLAPSE flag bit set.
        */
        int numRamps = NUM_COLORS;
        int rampSize = (paletteSize - 20) / numRamps;
        int extra = (paletteSize - 20) - (numRamps * rampSize);
        int i, r;

        /*
        ** Initialize static entries by copying them from the
        ** current system palette.
        */
        GetSystemPaletteEntries(hDC, 0, paletteSize, &pPal->palPalEntry[0]);

        /*
        ** Fill in non-static entries with desired colors.
        */
        for (r=0; r<numRamps; ++r) {
            int rampBase = r * rampSize + 10;
            PALETTEENTRY *pe = &pPal->palPalEntry[rampBase];
            int diffSize = (int) (rampSize * colors[r].ratio);
            int specSize = rampSize - diffSize;

            for (i=0; i<rampSize; ++i) {
                GLfloat *c0, *c1;
                GLint a;

                if (i < diffSize) {
                    c0 = colors[r].amb;
                    c1 = colors[r].diff;
                    a = (i * 255) / (diffSize - 1);
                } else {
                    c0 = colors[r].diff;
                    c1 = colors[r].spec;
                    a = ((i - diffSize) * 255) / (specSize - 1);
                }

                pe[i].peRed = (BYTE) (a * (c1[0] - c0[0]) + 255 * c0[0]);
                pe[i].peGreen = (BYTE) (a * (c1[1] - c0[1]) + 255 * c0[1]);
                pe[i].peBlue = (BYTE) (a * (c1[2] - c0[2]) + 255 * c0[2]);
                pe[i].peFlags = PC_NOCOLLAPSE;
            }

            colors[r].indexes[0] = rampBase;
            colors[r].indexes[1] = rampBase + (diffSize-1);
            colors[r].indexes[2] = rampBase + (rampSize-1);
        }

        /*
        ** Initialize any remaining non-static entries.
        */
        for (i=0; i<extra; ++i) {
            int index = numRamps*rampSize+10+i;
            PALETTEENTRY *pe = &pPal->palPalEntry[index];

            pe->peRed = (BYTE) 0;
            pe->peGreen = (BYTE) 0;
            pe->peBlue = (BYTE) 0;
            pe->peFlags = PC_NOCOLLAPSE;
        }
    }

    hPalette = CreatePalette(pPal);
    free(pPal);

    if (hPalette) {
        SelectPalette(hDC, hPalette, FALSE);
        RealizePalette(hDC);
    }
}

void resizeDIB(HDC hDC)
{
    SelectObject(hDC, hOldBitmap);
    DeleteObject(hBitmap);
    setupDIB(hDC);
}


void COpenGLView::SwapAnyBuffers(HDC hdc)
{
  if (renderToDIB) {
             glFinish();
			 //SetPixel(hDC,10,10,RGB(255,255,0));
		     //BitBlt(hdc, 0, 0, winWidth, winHeight, hDC, 0, 0, SRCCOPY);
			 /*
			 double aspect = winWidth/winHeight;
			 double nHeight = 100;
			 double nWidth = aspect*nHeight;
			 StretchBlt(hdc, 0, 0, nWidth, nHeight, hDC, 0, 0, winWidth,winHeight,SRCCOPY);
			 GdiFlush();	         
	         for(int j=0;j<nHeight;j++)
	          {
	            for(int i=0;i<nWidth;i++)
	            {
				  COLORREF clr =  GetPixel(hdc,i,j);
				  int xpos = GetRValue(clr);
				  int ypos = GetGValue(clr);
				  myRGB valX = palette[xpos];
				  myRGB valY = palette[ypos];
				  COLORREF clrx = RGB(valX.r,valX.g,valX.b);
				  COLORREF clry = RGB(valY.r,valY.g,valY.b);
				  
		          SetPixel(hdc,nWidth+(i*2),j,clrx);
				  SetPixel(hdc,nWidth+(i*2)+1,j,clry);
				}
			  }
			 */
			 GdiFlush();
	        
	  
	 


             
        } else {
             SwapBuffers(hdc);
  }
}

void COpenGLView::CheckForOpenGLErrors(char *file,int line)
{
	GLenum errcode = glGetError();
   if (errcode != GL_NO_ERROR) 
   {
	CString Message;
	Message.Format("OpenGL Error: %s : %d\nReason: %s",file,line,(LPCSTR)gluErrorString(errcode));
	//AfxMessageBox(Message);
   }
}



void COpenGLView::OnPrint(CDC* pDC, CPrintInfo* pInfo) {
        //  Let's not worry for now about print preview...
        if (pInfo->m_bPreview) return;  //  or print a message to preview window...
                
                UINT CurPage = pInfo->m_nCurPage;
        GLsizei HRes = pDC->GetDeviceCaps(HORZRES);
        GLsizei VRes = pDC->GetDeviceCaps(VERTRES);
        GLsizei HPixelsPerInch = pDC->GetDeviceCaps(LOGPIXELSX);
        GLsizei VPixelsPerInch = pDC->GetDeviceCaps(LOGPIXELSY);
        pDC->SetMapMode (MM_TEXT);
        // m_LMargin, etc are margins, in inches...
        GLint l = (GLint) (m_LMargin*HPixelsPerInch);
        GLint r = (GLint) (m_RMargin*HPixelsPerInch);
        GLint t = (GLint) (m_TMargin*VPixelsPerInch);
        GLint b = (GLint) (m_BMargin*VPixelsPerInch);
        //  Image width and height...
        GLsizei w = HRes - l - r;
        GLsizei h = VRes - t - b;
        //  Probably don't need this...
        int SavedDC = pDC->SaveDC ();
        if (CurPage == 1) {
                //  Save the current OpenGL settings...
                HDC   hDCOld   = wglGetCurrentDC ();
                HGLRC hGLRCOld = wglGetCurrentContext ();
                //  Make the printer the current
                //  OpenGL rendering device...
                HGLRC hGLRCPrinter = wglCreateContext (pDC->GetSafeHdc ());
                BOOL bRet = wglMakeCurrent (pDC->GetSafeHdc (), hGLRCPrinter);
                
                ASSERT (bRet);
                glClearColor (1.0f, 1.0f, 1.0f, 1.0f);
                glClearDepth (1.0);
                //  This makes a square image...
                if (w < h) h = w;
                if (h < w) w = h;
                //  This centers the images...
                l = (HRes - w)/2;
                b = (VRes - h)/2;
                //
                glViewport(l, b, w, h);
                OnDraw (pDC);
                //  Go back to the saved OpenGL settings...
                wglMakeCurrent (hDCOld, hGLRCOld);
                wglDeleteContext(hGLRCPrinter);
        }
        //  Probably don't need this...
        if (SavedDC != 0) pDC->RestoreDC (SavedDC);
}

void COpenGLView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CScrollView::OnBeginPrinting(pDC, pInfo);
}

void COpenGLView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CScrollView::OnEndPrinting(pDC, pInfo);
}

BOOL COpenGLView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	return DoPreparePrinting(pInfo);
}

void COpenGLView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CScrollView::OnEndPrintPreview(pDC, pInfo, point, pView);
}

void COpenGLView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CScrollView::OnPrepareDC(pDC, pInfo);
}

void COpenGLView::OnFilePrint() 
{
	// TODO: Add your command handler code here
	//CView::OnFilePrint();
	AfxMessageBox("Print  of OPENGL Object Not Supported");
}

void COpenGLView::OnFilePrintPreview() 
{
	// TODO: Add your command handler code here
	AfxMessageBox("Print Preview of OPENGL Object Not Supported");
	//CView::OnFilePrintPreview();
}

/*
void COpenGLView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	//  Save the current OpenGL settings...
        HDC   hDCOld   = wglGetCurrentDC ();
        HGLRC hGLRCOld = wglGetCurrentContext ();
        //  Make this view the current OpenGL rendering context...
        BOOL bRet = wglMakeCurrent (m_pDC->GetSafeHdc(), m_hGLContext);

		

        //  Draw!!!
        OnDraw(m_pDC);
        //  Go back to the saved OpenGL settings...
        wglMakeCurrent (hDCOld, hGLRCOld);
        //wglDeleteContext(hGLRCPrinter);	

		

}

void COpenGLView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	if (bActivate) {
                wglMakeCurrent (hDC, m_hGLContext);
    }
}
*/


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

void Mak256SquareImage(CBitmap &jamBitmap,CBitmap &currentImg,int nWidth,int nHeight,int hr,int vr,int ow,int oh)
{
	   
       CDC memdcA,memdcB;
	   CBitmap *poldbmps,*poldbmpd;
	   // Create a compatible memory DC
       memdcA.CreateCompatibleDC( NULL );
	   memdcB.CreateCompatibleDC( NULL );
	   
 
	   // Select the bitmap into the DC
       poldbmps = memdcA.SelectObject(&jamBitmap);	   
	   poldbmpd = memdcB.SelectObject(&currentImg);

	   if (hr <= 1) hr = 1;
	   if (vr <= 1) vr = 1;

	   int subImageSizeH = (ow/hr);
	   int subImageSizeV = (oh/vr);

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

#define CUSTOMVIEWCLASSNAME _T("OpenGLViewGeneric")
#define CUSTOM_CLASSNAME _T("OpenGLViewClass")

BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs) 
{
   // modify window styles and such here
   //cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CS_OWNDC);          
 
   return CScrollView::PreCreateWindow(cs);
   // call base class PreCreateWindow to get the cs.lpszClass filled in with the MFC default class name
   //if( !CScrollView::PreCreateWindow(cs) )
   //  return 0;

   /*
   // Register the window class if it has not already been registered.
   
   WNDCLASS wndcls;
   HINSTANCE hInst = AfxGetInstanceHandle();

   if(!(::GetClassInfo(hInst, CUSTOM_CLASSNAME, &wndcls)))      // check if our class is registered
   {
     if(::GetClassInfo(hInst, cs.lpszClass, &wndcls))           // get default MFC class settings 
     {
	    wndcls.lpszClassName = CUSTOM_CLASSNAME;                // set our class name
	    wndcls.style |= CS_OWNDC;			        // change settings for your custom class
        wndcls.hbrBackground = NULL;

        if (!AfxRegisterClass(&wndcls))                         // register class
          AfxThrowResourceException();				// could not register class
     }
     else
       AfxThrowResourceException();				// default MFC class not registered
    }

    cs.lpszClass = CUSTOMVIEWCLASSNAME;                         // set our class name in CREATESTRUCT

    return 1;                                                   // we're all set
	//return CScrollView::PreCreateWindow(cs);
	*/
}
