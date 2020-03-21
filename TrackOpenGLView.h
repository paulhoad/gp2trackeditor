// TrackOpenGLView.h : header file
//
#include "OpenGLView.h"

typedef enum {
  t_Air,
  t_Cockpit,
  t_ViewCamera
} t_GLView;

/////////////////////////////////////////////////////////////////////////////
// CTrackOpenGLView view

class CTrackOpenGLView : public COpenGLView
{
protected:
  CTrackOpenGLView();// protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CTrackOpenGLView)

  // Attributes
public:
  // Operations
public:
  void
    OpenGLDrawScene(HDC hdc);
  void
    getLookPoint();
  void
    InitView(int from, int to);

  void
    DrawScenery(GPTrack *track);

  void
    DrawRibbon3DGL(GPTrack *track, double xpos, double ypos, double zpos, double x1, double y1, double z1, double nx1, double ny1, double nz1, double x2, double y2, double z2, double nx2, double ny2, double nz2, int textureId);

  void
    OpenGLDrawPitlane(HDC hdc);

  double MidX;
  double MidY;
  double MidZ;
  double SC;
  int currentSectorNumber;
  int positionIntoSector;

  TrackSection *currentSector;
  int currentSectorLength;
  int currentTrackSize;
  BOOL inited;

  t_GLView currentView;

  static unsigned char *
    TextureMap(CDC *pDC, CBitmap &cbmp, unsigned char *lpBits, BOOL transparent);
  static void
    LoadJamTexture(GPTrack *mytrack, CBitmap &output, int idx, int hres, int vres, int *width, int *height);

  void
    InitDisplayList(HDC hdc);

  CBitmap RoadBmp;
  CBitmap VergeLeftBmp;
  CBitmap VergeRightBmp;
  CBitmap FenceLeftBmp;
  CBitmap FenceRightBmp;
  CBitmap Kerb1Bmp;
  CBitmap Kerb2Bmp;

  unsigned char *RoadBits;
  unsigned char *VergeLeftBits;
  unsigned char *VergeRightBits;
  unsigned char *FenceLeftBits;
  unsigned char *FenceRightBits;
  unsigned char *Kerb1Bits;
  unsigned char *Kerb2Bits;

  void
    drawTextureSides(TrackSection *t, GPTrack *track);
  void
    drawSingleSectionSide(TrackSection *t1, int beginOffset, int length, int side);
  void
    drawSingleSide(TrackSection *t1, int beginOffset, int length, int side);

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTrackOpenGLView)
protected:
  virtual void
    OnInitialUpdate();// first time after construct
  virtual void
    OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);
  virtual BOOL
    PreCreateWindow(CREATESTRUCT &cs);
  //}}AFX_VIRTUAL

  // Implementation
protected:
  virtual ~CTrackOpenGLView();
#ifdef _DEBUG
  virtual void
    AssertValid() const;
  virtual void
    Dump(CDumpContext &dc) const;
#endif

  // Generated message map functions
  //{{AFX_MSG(CTrackOpenGLView)
  afx_msg void
    OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void
    OnAirview();
  afx_msg void
    OnUpdateAirview(CCmdUI *pCmdUI);
  afx_msg void
    OnViewCockpit();
  afx_msg void
    OnUpdateViewCockpit(CCmdUI *pCmdUI);
  afx_msg void
    OnViewcameras();
  afx_msg void
    OnUpdateViewcameras(CCmdUI *pCmdUI);
  afx_msg void
    OnDrawaxis();
  afx_msg void
    OnUpdateDrawaxis(CCmdUI *pCmdUI);
  afx_msg void
    OnShowlines();
  afx_msg void
    OnUpdateShowlines(CCmdUI *pCmdUI);
  afx_msg void
    OnShowculling();
  afx_msg void
    OnUpdateShowculling(CCmdUI *pCmdUI);
  afx_msg void
    OnShowScenery();
  afx_msg void
    OnUpdateShowScenery(CCmdUI *pCmdUI);
  afx_msg void
    OnRefreshTree();
  afx_msg void
    OnShowTextures();
  afx_msg void
    OnUpdateShowTextures(CCmdUI *pCmdUI);
  afx_msg void
    OnTexturemapobjects();
  afx_msg void
    OnUpdateTexturemapobjects(CCmdUI *pCmdUI);
  afx_msg void
    OnShowtrees();
  afx_msg void
    OnUpdateShowtrees(CCmdUI *pCmdUI);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
