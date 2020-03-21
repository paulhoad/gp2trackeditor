// TrackEditorScrollView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TrackEditorScrollView view
#include "Tool.h"
#include "dibkernel.h"
#include "Ruler.h"

class TrackEditorScrollView : public CScrollView
{
protected:
  TrackEditorScrollView();// protected constructor used by dynamic creation
  DECLARE_DYNCREATE(TrackEditorScrollView)

  // Attributes
public:
  inline CTrackEditorDoc*
    GetDocument()
  {
    return (CTrackEditorDoc*)m_pDocument;
  }

  void
    CreateViewPopupMenu();

  CMenu popup;

  CDib* trackImage;

  Ruler* ruler;

  CTreeCtrl*
    getTree();

  Display* display;

  void
    Resize(double scale);

  void
    OnMyDraw(CDC* pDC);

  // Operations
public:
  void
    repaint();
  void
    update();

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(TrackEditorScrollView)
public:
  virtual BOOL
    Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
  virtual void
    OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

protected:
  virtual void
    OnDraw(CDC* pDC);// overridden to draw this view
  virtual void
    OnInitialUpdate();// first time after construct
  virtual void
    OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void
    OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void
    OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
  virtual BOOL
    OnPreparePrinting(CPrintInfo* pInfo);
  virtual void
    OnPrint(CDC* pDC, CPrintInfo* pInfo);
  //}}AFX_VIRTUAL

  // Implementation
public:
  virtual ~TrackEditorScrollView();
#ifdef _DEBUG
  virtual void
    AssertValid() const;
  virtual void
    Dump(CDumpContext& dc) const;
#endif

  // CPoint dragstartpoint;

  Tool* currentTool;

  // Generated message map functions
  //{{AFX_MSG(TrackEditorScrollView)
  afx_msg void
    OnIn();
  afx_msg void
    OnOut();
  afx_msg void
    OnTrack();
  afx_msg void
    OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void
    OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void
    OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void
    OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void
    OnRButtonUp(UINT nFlags, CPoint point);
  afx_msg void
    OnMButtonDown(UINT nFlags, CPoint point);
  afx_msg void
    OnMButtonUp(UINT nFlags, CPoint point);
  afx_msg void
    OnLine();
  afx_msg void
    OnViewShowwalls();
  afx_msg void
    OnViewShowall();
  afx_msg void
    OnViewHideall();
  afx_msg void
    OnWall();
  afx_msg void
    OnDrivingLine();
  afx_msg void
    OnViewShowcameras();
  afx_msg void
    OnViewObjBitmaps();
  afx_msg void
    OnFilePrint();
  afx_msg void
    OnFilePrintPreview();
  afx_msg void
    OnObjectTool();
  afx_msg void
    OnPointer();
  afx_msg void
    OnTrackTool();
  afx_msg void
    OnZoomtool();
  afx_msg void
    OnUpdateCcline(CCmdUI* pCmdUI);
  afx_msg void
    OnCcline();
  afx_msg void
    OnUpdateViewObjBitmaps(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateZoomtool(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdatePointer(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateObjectTool(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateTrackTool(CCmdUI* pCmdUI);
  afx_msg void
    OnPitlaneTool();
  afx_msg void
    OnUpdatePitlaneTool(CCmdUI* pCmdUI);
  afx_msg void
    OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void
    OnWalltool();
  afx_msg void
    OnCctool();
  afx_msg void
    OnUpdateCctool(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateViewShowcameras(CCmdUI* pCmdUI);
  afx_msg void
    OnInsertObj();
  afx_msg void
    OnInsertTracksegment();
  afx_msg void
    OnSaveTrackImage();
  afx_msg void
    OnRemoveCcSection();
  afx_msg void
    OnRemovePitObject();
  afx_msg void
    OnRemovePitSection();
  afx_msg void
    OnRemoveTrackObject();
  afx_msg void
    OnRemoveTrackSection();
  afx_msg void
    OnInsertCclinesection();
  afx_msg void
    OnInsertPitcmd();
  afx_msg void
    OnInsertPitobject();
  afx_msg void
    OnInsertPitsection();
  afx_msg void
    OnInsertTrackcmd();
  afx_msg void
    OnShowGreen();
  afx_msg void
    OnUpdateShowGreen(CCmdUI* pCmdUI);
  afx_msg BOOL
    OnEraseBkgnd(CDC* pDC);
  afx_msg void
    OnUpdateInsertPitcmd(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateInsertPitobject(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateInsertPitsection(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateInsertObj(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateInsertCclinesection(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateInsertTracksegment(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateInsertTrackcmd(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateRemoveCcSection(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateRemovePitObject(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateRemovePitSection(CCmdUI* pCmdUI);
  afx_msg void
    OnRemovePitcmds();
  afx_msg void
    OnUpdateRemovePitcmds(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateRemoveTrackObject(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateRemoveTrackSection(CCmdUI* pCmdUI);
  afx_msg void
    OnRemovePitCmd();
  afx_msg void
    OnUpdateRemovePitCmd(CCmdUI* pCmdUI);
  afx_msg void
    OnRemoveTrackCmd();
  afx_msg void
    OnUpdateRemoveTrackCmd(CCmdUI* pCmdUI);
  afx_msg void
    OnImportImage();
  afx_msg void
    OnShowFinish();
  afx_msg void
    OnUpdateShowFinish(CCmdUI* pCmdUI);
  afx_msg void
    OnPitlane();
  afx_msg void
    OnObjects();
  afx_msg void
    OnViewTrack();
  afx_msg void
    OnUpdateTrack(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateWalltool(CCmdUI* pCmdUI);
  afx_msg void
    OnRuler();
  afx_msg void
    OnUpdateRuler(CCmdUI* pCmdUI);
  afx_msg void
    OnInsertRemoveoverlay();
  afx_msg void
    OnUpdateInsertRemoveoverlay(CCmdUI* pCmdUI);
  afx_msg void
    OnZoomHome();
  afx_msg void
    OnChangeTrackWidthLeft();
  afx_msg void
    OnChangeTrackwidth();
  afx_msg void
    OnChangeTrackwidthRight();
  afx_msg void
    OnUpdateChangeTrackWidthLeft(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateChangeTrackwidth(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateChangeTrackwidthRight(CCmdUI* pCmdUI);
  afx_msg void
    OnChangeTrackwallLeft();
  afx_msg void
    OnUpdateChangeTrackwallLeft(CCmdUI* pCmdUI);
  afx_msg void
    OnChangeTrackwallRight();
  afx_msg void
    OnUpdateChangeTrackwallRight(CCmdUI* pCmdUI);
  afx_msg void
    OnShowRemovedWalls();
  afx_msg void
    OnUpdateShowRemovedWalls(CCmdUI* pCmdUI);
  afx_msg void
    OnShowRoadsigns();
  afx_msg void
    OnUpdateShowRoadsigns(CCmdUI* pCmdUI);
  afx_msg void
    OnShowCommands();
  afx_msg void
    OnUpdateShowCommands(CCmdUI* pCmdUI);
  afx_msg void
    OnAddKerbIn();
  afx_msg void
    OnKerbAddOut();
  afx_msg void
    OnKerbRemoveIn();
  afx_msg void
    OnKerbRemoveOut();
  afx_msg void
    OnKerbRemoveStraight();
  afx_msg void
    OnKerbRunoff();
  afx_msg void
    OnKerbRemoveall();
  afx_msg void
    OnShowCenterline();
  afx_msg void
    OnUpdateShowCenterline(CCmdUI* pCmdUI);
  afx_msg void
    OnAddGravel();
  afx_msg void
    OnAddGravelLeft();
  afx_msg void
    OnAddGravelRight();
  afx_msg void
    OnAddTarmacLeft();
  afx_msg void
    OnAddTarmacRight();
  afx_msg void
    OnAddTypeWallRight();
  afx_msg void
    OnAddTyreWallLeft();
  afx_msg void
    OnAddTrackside();
  afx_msg void
    OnShowKerbs();
  afx_msg void
    OnUpdateShowKerbs(CCmdUI* pCmdUI);
  afx_msg void
    OnShowGrip();
  afx_msg void
    OnUpdateShowGrip(CCmdUI* pCmdUI);
  afx_msg void
    OnShowBlackFlags();
  afx_msg void
    OnUpdateShowBlackFlags(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdatePointerTool(CCmdUI* pCmdUI);
  afx_msg void
    OnShowScenery();
  afx_msg void
    OnUpdateShowScenery(CCmdUI* pCmdUI);
  afx_msg void
    OnViewShowtrackpies();
  afx_msg void
    OnUpdateViewShowtrackpies(CCmdUI* pCmdUI);
  afx_msg void
    OnInsertBlackLeft();
  afx_msg void
    OnInsertBlackRight();
  afx_msg void
    OnShowGridlines();
  afx_msg void
    OnUpdateShowGridlines(CCmdUI* pCmdUI);
  afx_msg void
    OnShowWorldExtent();
  afx_msg void
    OnUpdateShowWorldExtent(CCmdUI* pCmdUI);
  afx_msg void
    OnShowTracknumbers();
  afx_msg void
    OnUpdateShowTracknumbers(CCmdUI* pCmdUI);
  afx_msg void
    OnShowTextures();
  afx_msg void
    OnUpdateShowTextures(CCmdUI* pCmdUI);
  afx_msg void
    OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void
    OnShowPitcmds();
  afx_msg void
    OnUpdateShowPitcmds(CCmdUI* pCmdUI);
  afx_msg void
    OnShowDistances();
  afx_msg void
    OnUpdateShowDistances(CCmdUI* pCmdUI);
  afx_msg void
    OnShowUnkVarious();
  afx_msg void
    OnUpdateShowUnkVarious(CCmdUI* pCmdUI);
  afx_msg void
    OnShowCcNumbers();
  afx_msg void
    OnUpdateShowCcNumbers(CCmdUI* pCmdUI);
  afx_msg void
    OnShowShowcurvedwallsaslines();
  afx_msg void
    OnUpdateShowShowcurvedwallsaslines(CCmdUI* pCmdUI);
  afx_msg void
    OnShowObjMarkers();
  afx_msg void
    OnUpdateShowObjMarkers(CCmdUI* pCmdUI);
  afx_msg void
    OnMaintainAspect();
  afx_msg void
    OnUpdateMaintainAspect(CCmdUI* pCmdUI);
  afx_msg void
    OnShowGridPaper();
  afx_msg void
    OnUpdateShowGridPaper(CCmdUI* pCmdUI);
  afx_msg void
    OnHalfmile();
  afx_msg void
    OnUpdateHalfmile(CCmdUI* pCmdUI);
  afx_msg void
    On10meters();
  afx_msg void
    OnUpdate10meters(CCmdUI* pCmdUI);
  afx_msg void
    On500meters();
  afx_msg void
    OnUpdate500meters(CCmdUI* pCmdUI);
  afx_msg void
    On50meters();
  afx_msg void
    OnUpdate50meters(CCmdUI* pCmdUI);
  afx_msg void
    OnOnekm();
  afx_msg void
    OnUpdateOnekm(CCmdUI* pCmdUI);
  afx_msg void
    On100meters();
  afx_msg void
    OnUpdate100meters(CCmdUI* pCmdUI);
  afx_msg void
    OnOnemile();
  afx_msg void
    OnUpdateOnemile(CCmdUI* pCmdUI);
  afx_msg void
    OnLinetool();
  afx_msg void
    OnUpdateLinetool(CCmdUI* pCmdUI);
  afx_msg void
    OnShowTrackMarkings();
  afx_msg void
    OnUpdateShowTrackMarkings(CCmdUI* pCmdUI);
  afx_msg void
    OnPitlinetool();
  afx_msg void
    OnUpdatePitlinetool(CCmdUI* pCmdUI);
  afx_msg void
    OnShowAnnotations();
  afx_msg void
    OnUpdateShowAnnotations(CCmdUI* pCmdUI);
  afx_msg void
    OnHandtool();
  afx_msg void
    OnUpdateHandtool(CCmdUI* pCmdUI);
  afx_msg void
    OnCclinetool();
  afx_msg void
    OnUpdateCclinetool(CCmdUI* pCmdUI);
  afx_msg void
    OnCameratool();
  afx_msg void
    OnUpdateCameratool(CCmdUI* pCmdUI);
  afx_msg void
    OnDefaultSides();
  afx_msg void
    OnUpdateDefaultSides(CCmdUI* pCmdUI);
  afx_msg void
    OnShowShowccdatalog();
  afx_msg void
    OnUpdateShowShowccdatalog(CCmdUI* pCmdUI);
  afx_msg void
    OnShowShowsplits();
  afx_msg void
    OnUpdateShowShowsplits(CCmdUI* pCmdUI);
  afx_msg void
    OnShowShowpitfences();
  afx_msg void
    OnUpdateShowShowpitfences(CCmdUI* pCmdUI);
  afx_msg void
    OnRemoveRemoveunderlayimage();
  afx_msg void
    OnUpdateRemoveRemoveunderlayimage(CCmdUI* pCmdUI);
  afx_msg void
    OnShowShowunderlaybitmap();
  afx_msg void
    OnUpdateShowShowunderlaybitmap(CCmdUI* pCmdUI);
  afx_msg void
    OnCclineInsertccsectioncmd0x70();
  afx_msg void
    OnDrawoutlines();
  afx_msg void
    OnUpdateDrawoutlines(CCmdUI* pCmdUI);
  afx_msg void
    OnShowtrees();
  afx_msg void
    OnUpdateShowtrees(CCmdUI* pCmdUI);
  afx_msg void
    OnShowShowtexturemappingcommands();
  afx_msg void
    OnUpdateShowShowtexturemappingcommands(CCmdUI* pCmdUI);
  afx_msg void
    OnShowShowviewdistancecommands();
  afx_msg void
    OnUpdateShowShowviewdistancecommands(CCmdUI* pCmdUI);
  afx_msg void
    OnShowShowviewdistances();
  afx_msg void
    OnUpdateShowShowviewdistances(CCmdUI* pCmdUI);
  afx_msg void
    OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
