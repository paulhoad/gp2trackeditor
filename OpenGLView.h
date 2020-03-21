
#ifndef _INCLUDED_OPENGL_H_
#define _INCLUDED_OPENGL_H_

// OpenGLView.h : header file
//
static int m_LMargin = 1;
static int m_RMargin = 1;
static int m_TMargin = 1;
static int m_BMargin = 1;

/////////////////////////////////////////////////////////////////////////////
// COpenGLView view

class COpenGLView : public CScrollView
{
protected:
  COpenGLView();// protected constructor used by dynamic creation
  DECLARE_DYNCREATE(COpenGLView)

  // Attributes
public:
  // Operations
public:
  BOOL renderToDIB;
  HDC hDCFrontBuffer;

  BOOL
    SetWindowPixelFormat(HDC hDC);
  BOOL
    CreateViewGLContext(HDC hDC);

  float m_red[256], m_blue[256], m_green[256];
  HPALETTE m_hPalette;

  double scaler;
  double dist;
  double fov;

  virtual void
    getLookPoint();

  void
    SwapAnyBuffers(HDC hdc);

  CDC* m_pDC;
  // HGLRC m_hRC;
  HGLRC m_hGLContext;
  int m_GLPixelIndex;
  void
    RenderScene(CDC* dc);
  // virtual void OpenGLDrawScene(CPaintDC *dc);
  virtual void
    OpenGLDrawScene(HDC hdc);
  double m_transY;
  double m_transX;
  double m_angle1;
  double m_angle2;
  float m_size;
  double m_xRotate;
  double m_yRotate;
  double m_zRotate;

  int m_width;
  int m_height;
  int m_exp;

  CPoint m_RightDownPos;
  CPoint m_LeftDownPos;
  BOOL m_RightButtonDown;
  BOOL m_LeftButtonDown;
  BOOL m_ShiftDown;

  void
    CheckForOpenGLErrors(char* file, int line);

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(COpenGLView)
public:
  virtual void
    OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

protected:
  virtual void
    OnDraw(CDC* pDC);// overridden to draw this view
  virtual void
    OnInitialUpdate();// first time after construct
  virtual void
    OnPrint(CDC* pDC, CPrintInfo* pInfo);
  virtual void
    OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void
    OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual BOOL
    OnPreparePrinting(CPrintInfo* pInfo);
  virtual void
    OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
  virtual BOOL
    PreCreateWindow(CREATESTRUCT& cs);
  //}}AFX_VIRTUAL

  // Implementation
protected:
  virtual ~COpenGLView();
#ifdef _DEBUG
  virtual void
    AssertValid() const;
  virtual void
    Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
  //{{AFX_MSG(COpenGLView)
  afx_msg void
    OnDestroy();
  afx_msg int
    OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void
    OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void
    OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void
    OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void
    OnSize(UINT nType, int cx, int cy);
  afx_msg void
    OnIn();
  afx_msg void
    OnOut();
  afx_msg void
    OnZoomHome();
  afx_msg void
    OnFilePrint();
  afx_msg void
    OnFilePrintPreview();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif
