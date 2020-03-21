// TrackEditorScrollView.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "TrackEditor.h"

#include "TrackEditorDoc.h"
#include "Tool.h"
#include "TrackEditorScrollView.h"

#include "TrackSection.h"

#include "dibkernel.h"

#include "CMemDC.h"
#include "TextureChooser.h"
#include "TrackTree.h"

#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define UPDATE_TRACK(x)                  \
  CTrackEditorDoc* pDoc = GetDocument(); \
  GPTrack* mytrack = pDoc->getTrack();   \
  pCmdUI->SetCheck(mytrack->x);

extern CTrackEditorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// TrackEditorScrollView

IMPLEMENT_DYNCREATE(TrackEditorScrollView, CScrollView)

TrackEditorScrollView::TrackEditorScrollView() : trackImage(NULL), ruler(NULL)
{
  display = new Display();
  // changingAngle = FALSE;
  currentTool = new PointerTool();

  if (trackImage == NULL) delete trackImage;
  {
    CString file =
      theApp.GetProfileString("Preferences", "UnderLayImageName", "none");
    if (file != "none") {
      trackImage = new CDib();
      trackImage->Read(file);
    }
  }
}

TrackEditorScrollView::~TrackEditorScrollView()
{
  if (display != NULL) delete display;
  if (currentTool != NULL) delete currentTool;
  if (ruler != NULL) delete ruler;
}

BEGIN_MESSAGE_MAP(TrackEditorScrollView, CScrollView)
//{{AFX_MSG_MAP(TrackEditorScrollView)
ON_COMMAND(ZOOM_IN, OnIn)
ON_COMMAND(ZOOM_OUT, OnOut)
ON_COMMAND(OVERLAY_TRACK, OnTrack)
ON_WM_LBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONUP()
ON_WM_RBUTTONDOWN()
ON_WM_RBUTTONUP()
ON_WM_MBUTTONDOWN()
ON_WM_MBUTTONUP()
ON_COMMAND(VIEW_LINE, OnLine)
ON_COMMAND(ID_VIEW_SHOWWALLS, OnViewShowwalls)
ON_COMMAND(ID_VIEW_SHOWALL, OnViewShowall)
ON_COMMAND(ID_VIEW_HIDEALL, OnViewHideall)
ON_COMMAND(SHOW_WALL, OnWall)
ON_COMMAND(SHOW_DRIVING_LINE, OnDrivingLine)
ON_COMMAND(ID_VIEW_SHOWCAMERAS, OnViewShowcameras)
ON_COMMAND(ID_VIEW_OBJ_BITMAPS, OnViewObjBitmaps)
ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
ON_COMMAND(ID_OBJECT_TOOL, OnObjectTool)
ON_COMMAND(ID_POINTER_TOOL, OnPointer)
ON_COMMAND(ID_TRACK_TOOL, OnTrackTool)
ON_COMMAND(ID_ZOOMTOOL, OnZoomtool)
ON_UPDATE_COMMAND_UI(VIEW_CCLINE, OnUpdateCcline)
ON_COMMAND(VIEW_CCLINE, OnCcline)
ON_UPDATE_COMMAND_UI(ID_VIEW_OBJ_BITMAPS, OnUpdateViewObjBitmaps)
ON_UPDATE_COMMAND_UI(ID_ZOOMTOOL, OnUpdateZoomtool)
ON_UPDATE_COMMAND_UI(ID_POINTER, OnUpdatePointer)
ON_UPDATE_COMMAND_UI(ID_OBJECT_TOOL, OnUpdateObjectTool)
ON_UPDATE_COMMAND_UI(ID_TRACK_TOOL, OnUpdateTrackTool)
ON_COMMAND(ID_PITLANE_TOOL, OnPitlaneTool)
ON_UPDATE_COMMAND_UI(ID_PITLANE_TOOL, OnUpdatePitlaneTool)
ON_WM_LBUTTONDBLCLK()
ON_COMMAND(ID_WALLTOOL, OnWalltool)
ON_COMMAND(ID_CCTOOL, OnCctool)
ON_UPDATE_COMMAND_UI(ID_CCTOOL, OnUpdateCctool)
ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWCAMERAS, OnUpdateViewShowcameras)
ON_COMMAND(ID_INSERT_OBJ, OnInsertObj)
ON_COMMAND(ID_INSERT_TRACKSEGMENT, OnInsertTracksegment)
ON_COMMAND(IDC_SAVE_TRACK_IMAGE, OnSaveTrackImage)
ON_COMMAND(ID_REMOVE_CC_SECTION, OnRemoveCcSection)
ON_COMMAND(ID_REMOVE_PIT_OBJECT, OnRemovePitObject)
ON_COMMAND(ID_REMOVE_PIT_SECTION, OnRemovePitSection)
ON_COMMAND(ID_REMOVE_TRACK_OBJECT, OnRemoveTrackObject)
ON_COMMAND(ID_REMOVE_TRACK_SECTION, OnRemoveTrackSection)
ON_COMMAND(ID_INSERT_CCLINESECTION, OnInsertCclinesection)
ON_COMMAND(ID_INSERT_PITCMD, OnInsertPitcmd)
ON_COMMAND(ID_INSERT_PITOBJECT, OnInsertPitobject)
ON_COMMAND(ID_INSERT_PITSECTION, OnInsertPitsection)
ON_COMMAND(ID_INSERT_TRACKCMD, OnInsertTrackcmd)
ON_COMMAND(ID_SHOW_GREEN, OnShowGreen)
ON_UPDATE_COMMAND_UI(ID_SHOW_GREEN, OnUpdateShowGreen)
ON_WM_ERASEBKGND()
ON_UPDATE_COMMAND_UI(ID_INSERT_PITCMD, OnUpdateInsertPitcmd)
ON_UPDATE_COMMAND_UI(ID_INSERT_PITOBJECT, OnUpdateInsertPitobject)
ON_UPDATE_COMMAND_UI(ID_INSERT_PITSECTION, OnUpdateInsertPitsection)
ON_UPDATE_COMMAND_UI(ID_INSERT_OBJ, OnUpdateInsertObj)
ON_UPDATE_COMMAND_UI(ID_INSERT_CCLINESECTION, OnUpdateInsertCclinesection)
ON_UPDATE_COMMAND_UI(ID_INSERT_TRACKSEGMENT, OnUpdateInsertTracksegment)
ON_UPDATE_COMMAND_UI(ID_INSERT_TRACKCMD, OnUpdateInsertTrackcmd)
ON_UPDATE_COMMAND_UI(ID_REMOVE_CC_SECTION, OnUpdateRemoveCcSection)
ON_UPDATE_COMMAND_UI(ID_REMOVE_PIT_OBJECT, OnUpdateRemovePitObject)
ON_UPDATE_COMMAND_UI(ID_REMOVE_PIT_SECTION, OnUpdateRemovePitSection)
ON_COMMAND(ID_REMOVE_PITCMDS, OnRemovePitcmds)
ON_UPDATE_COMMAND_UI(ID_REMOVE_PITCMDS, OnUpdateRemovePitcmds)
ON_UPDATE_COMMAND_UI(ID_REMOVE_TRACK_OBJECT, OnUpdateRemoveTrackObject)
ON_UPDATE_COMMAND_UI(ID_REMOVE_TRACK_SECTION, OnUpdateRemoveTrackSection)
ON_COMMAND(ID_REMOVE_PIT_CMD, OnRemovePitCmd)
ON_UPDATE_COMMAND_UI(ID_REMOVE_PIT_CMD, OnUpdateRemovePitCmd)
ON_COMMAND(ID_REMOVE_TRACK_CMD, OnRemoveTrackCmd)
ON_UPDATE_COMMAND_UI(ID_REMOVE_TRACK_CMD, OnUpdateRemoveTrackCmd)
ON_COMMAND(UNDER_LAYBITMAP, OnImportImage)
ON_COMMAND(ID_SHOW_FINISH, OnShowFinish)
ON_UPDATE_COMMAND_UI(ID_SHOW_FINISH, OnUpdateShowFinish)
ON_COMMAND(SHOW_PITLANE, OnPitlane)
ON_COMMAND(SHOW_OBJECTS, OnObjects)
ON_COMMAND(SHOW_TRACK, OnViewTrack)
ON_UPDATE_COMMAND_UI(SHOW_TRACK, OnUpdateTrack)
ON_UPDATE_COMMAND_UI(ID_WALLTOOL, OnUpdateWalltool)
ON_COMMAND(ID_RULER, OnRuler)
ON_UPDATE_COMMAND_UI(ID_RULER, OnUpdateRuler)
ON_COMMAND(ID_INSERT_REMOVEOVERLAY, OnInsertRemoveoverlay)
ON_UPDATE_COMMAND_UI(ID_INSERT_REMOVEOVERLAY, OnUpdateInsertRemoveoverlay)
ON_COMMAND(ID_ZOOM_HOME, OnZoomHome)
ON_COMMAND(ID_CHANGE_TRACK_WIDTH_LEFT, OnChangeTrackWidthLeft)
ON_COMMAND(ID_CHANGE_TRACKWIDTH, OnChangeTrackwidth)
ON_COMMAND(ID_CHANGE_TRACKWIDTH_RIGHT, OnChangeTrackwidthRight)
ON_UPDATE_COMMAND_UI(ID_CHANGE_TRACK_WIDTH_LEFT, OnUpdateChangeTrackWidthLeft)
ON_UPDATE_COMMAND_UI(ID_CHANGE_TRACKWIDTH, OnUpdateChangeTrackwidth)
ON_UPDATE_COMMAND_UI(ID_CHANGE_TRACKWIDTH_RIGHT, OnUpdateChangeTrackwidthRight)
ON_COMMAND(ID_CHANGE_TRACKWALL_LEFT, OnChangeTrackwallLeft)
ON_UPDATE_COMMAND_UI(ID_CHANGE_TRACKWALL_LEFT, OnUpdateChangeTrackwallLeft)
ON_COMMAND(ID_CHANGE_TRACKWALL_RIGHT, OnChangeTrackwallRight)
ON_UPDATE_COMMAND_UI(ID_CHANGE_TRACKWALL_RIGHT, OnUpdateChangeTrackwallRight)
ON_COMMAND(ID_SHOW_REMOVED_WALLS, OnShowRemovedWalls)
ON_UPDATE_COMMAND_UI(ID_SHOW_REMOVED_WALLS, OnUpdateShowRemovedWalls)
ON_COMMAND(ID_SHOW_ROADSIGNS, OnShowRoadsigns)
ON_UPDATE_COMMAND_UI(ID_SHOW_ROADSIGNS, OnUpdateShowRoadsigns)
ON_COMMAND(ID_SHOW_COMMANDS, OnShowCommands)
ON_UPDATE_COMMAND_UI(ID_SHOW_COMMANDS, OnUpdateShowCommands)
ON_COMMAND(ID_ADD_KERB_IN, OnAddKerbIn)
ON_COMMAND(ID_KERB_ADD_OUT, OnKerbAddOut)
ON_COMMAND(ID_KERB_REMOVE_IN, OnKerbRemoveIn)
ON_COMMAND(ID_KERB_REMOVE_OUT, OnKerbRemoveOut)
ON_COMMAND(ID_KERB_REMOVE_STRAIGHT, OnKerbRemoveStraight)
ON_COMMAND(ID_KERB_RUNOFF, OnKerbRunoff)
ON_COMMAND(ID_KERB_REMOVEALL, OnKerbRemoveall)
ON_COMMAND(ID_SHOW_CENTERLINE, OnShowCenterline)
ON_UPDATE_COMMAND_UI(ID_SHOW_CENTERLINE, OnUpdateShowCenterline)
ON_COMMAND(ID_ADD_GRAVEL, OnAddGravel)
ON_COMMAND(ID_ADD_GRAVEL_LEFT, OnAddGravelLeft)
ON_COMMAND(ID_ADD_GRAVEL_RIGHT, OnAddGravelRight)
ON_COMMAND(ID_ADD_TARMAC_LEFT, OnAddTarmacLeft)
ON_COMMAND(ID_ADD_TARMAC_RIGHT, OnAddTarmacRight)
ON_COMMAND(ID_ADD_TYPE_WALL_RIGHT, OnAddTypeWallRight)
ON_COMMAND(ID_ADD_TYRE_LEFT, OnAddTyreWallLeft)
ON_COMMAND(ID_ADD_TRACKSIDE, OnAddTrackside)
ON_COMMAND(ID_SHOW_KERBS, OnShowKerbs)
ON_UPDATE_COMMAND_UI(ID_SHOW_KERBS, OnUpdateShowKerbs)
ON_COMMAND(ID_SHOW_GRIP, OnShowGrip)
ON_UPDATE_COMMAND_UI(ID_SHOW_GRIP, OnUpdateShowGrip)
ON_COMMAND(ID_SHOW_BLACK_FLAGS, OnShowBlackFlags)
ON_UPDATE_COMMAND_UI(ID_SHOW_BLACK_FLAGS, OnUpdateShowBlackFlags)
ON_UPDATE_COMMAND_UI(ID_POINTER_TOOL, OnUpdatePointerTool)
ON_COMMAND(ID_SHOW_SCENERY, OnShowScenery)
ON_UPDATE_COMMAND_UI(ID_SHOW_SCENERY, OnUpdateShowScenery)
ON_COMMAND(ID_VIEW_SHOWTRACKPIES, OnViewShowtrackpies)
ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWTRACKPIES, OnUpdateViewShowtrackpies)
ON_COMMAND(ID_INSERT_BLACK_LEFT, OnInsertBlackLeft)
ON_COMMAND(ID_INSERT_BLACK_RIGHT, OnInsertBlackRight)
ON_COMMAND(ID_SHOW_GRIDLINES, OnShowGridlines)
ON_UPDATE_COMMAND_UI(ID_SHOW_GRIDLINES, OnUpdateShowGridlines)
ON_COMMAND(ID_SHOW_WORLD_EXTENT, OnShowWorldExtent)
ON_UPDATE_COMMAND_UI(ID_SHOW_WORLD_EXTENT, OnUpdateShowWorldExtent)
ON_COMMAND(ID_SHOW_TRACKNUMBERS, OnShowTracknumbers)
ON_UPDATE_COMMAND_UI(ID_SHOW_TRACKNUMBERS, OnUpdateShowTracknumbers)
ON_COMMAND(ID_SHOW_TEXTURES, OnShowTextures)
ON_UPDATE_COMMAND_UI(ID_SHOW_TEXTURES, OnUpdateShowTextures)
ON_WM_KEYDOWN()
ON_COMMAND(ID_SHOW_PITCMDS, OnShowPitcmds)
ON_UPDATE_COMMAND_UI(ID_SHOW_PITCMDS, OnUpdateShowPitcmds)
ON_COMMAND(ID_SHOW_DISTANCES, OnShowDistances)
ON_UPDATE_COMMAND_UI(ID_SHOW_DISTANCES, OnUpdateShowDistances)
ON_COMMAND(ID_SHOW_UNK_VARIOUS, OnShowUnkVarious)
ON_UPDATE_COMMAND_UI(ID_SHOW_UNK_VARIOUS, OnUpdateShowUnkVarious)
ON_COMMAND(ID_SHOW_CC_NUMBERS, OnShowCcNumbers)
ON_UPDATE_COMMAND_UI(ID_SHOW_CC_NUMBERS, OnUpdateShowCcNumbers)
ON_COMMAND(ID_SHOW_SHOWCURVEDWALLSASLINES, OnShowShowcurvedwallsaslines)
ON_UPDATE_COMMAND_UI(ID_SHOW_SHOWCURVEDWALLSASLINES,
  OnUpdateShowShowcurvedwallsaslines)
ON_COMMAND(ID_SHOW_OBJ_MARKERS, OnShowObjMarkers)
ON_UPDATE_COMMAND_UI(ID_SHOW_OBJ_MARKERS, OnUpdateShowObjMarkers)
ON_COMMAND(ID_MAINTAIN_ASPECT, OnMaintainAspect)
ON_UPDATE_COMMAND_UI(ID_MAINTAIN_ASPECT, OnUpdateMaintainAspect)
ON_COMMAND(ID_SHOW_GRAPH_PAPER, OnShowGridPaper)
ON_UPDATE_COMMAND_UI(ID_SHOW_GRAPH_PAPER, OnUpdateShowGridPaper)
ON_COMMAND(ID_HALFMILE, OnHalfmile)
ON_UPDATE_COMMAND_UI(ID_HALFMILE, OnUpdateHalfmile)
ON_COMMAND(ID_10METERS, On10meters)
ON_UPDATE_COMMAND_UI(ID_10METERS, OnUpdate10meters)
ON_COMMAND(ID_500METERS, On500meters)
ON_UPDATE_COMMAND_UI(ID_500METERS, OnUpdate500meters)
ON_COMMAND(ID_50METERS, On50meters)
ON_UPDATE_COMMAND_UI(ID_50METERS, OnUpdate50meters)
ON_COMMAND(ID_ONEKM, OnOnekm)
ON_UPDATE_COMMAND_UI(ID_ONEKM, OnUpdateOnekm)
ON_COMMAND(ID_100METERS, On100meters)
ON_UPDATE_COMMAND_UI(ID_100METERS, OnUpdate100meters)
ON_COMMAND(ID_ONEMILE, OnOnemile)
ON_UPDATE_COMMAND_UI(ID_ONEMILE, OnUpdateOnemile)
ON_COMMAND(ID_LINETOOL, OnLinetool)
ON_UPDATE_COMMAND_UI(ID_LINETOOL, OnUpdateLinetool)
ON_COMMAND(ID_SHOW_TRACK_MARKINGS, OnShowTrackMarkings)
ON_UPDATE_COMMAND_UI(ID_SHOW_TRACK_MARKINGS, OnUpdateShowTrackMarkings)
ON_COMMAND(ID_PITLINETOOL, OnPitlinetool)
ON_UPDATE_COMMAND_UI(ID_PITLINETOOL, OnUpdatePitlinetool)
ON_COMMAND(ID_SHOW_ANNOTATIONS, OnShowAnnotations)
ON_UPDATE_COMMAND_UI(ID_SHOW_ANNOTATIONS, OnUpdateShowAnnotations)
ON_COMMAND(ID_HANDTOOL, OnHandtool)
ON_UPDATE_COMMAND_UI(ID_HANDTOOL, OnUpdateHandtool)
ON_COMMAND(ID_CCLINETOOL, OnCclinetool)
ON_UPDATE_COMMAND_UI(ID_CCLINETOOL, OnUpdateCclinetool)
ON_COMMAND(ID_CAMERATOOL, OnCameratool)
ON_UPDATE_COMMAND_UI(ID_CAMERATOOL, OnUpdateCameratool)
ON_COMMAND(ID_DEFAULT_SIDES, OnDefaultSides)
ON_UPDATE_COMMAND_UI(ID_DEFAULT_SIDES, OnUpdateDefaultSides)
ON_COMMAND(ID_SHOW_SHOWCCDATALOG, OnShowShowccdatalog)
ON_UPDATE_COMMAND_UI(ID_SHOW_SHOWCCDATALOG, OnUpdateShowShowccdatalog)
ON_COMMAND(ID_SHOW_SHOWSPLITS, OnShowShowsplits)
ON_UPDATE_COMMAND_UI(ID_SHOW_SHOWSPLITS, OnUpdateShowShowsplits)
ON_COMMAND(ID_SHOW_SHOWPITFENCES, OnShowShowpitfences)
ON_UPDATE_COMMAND_UI(ID_SHOW_SHOWPITFENCES, OnUpdateShowShowpitfences)
ON_COMMAND(ID_REMOVE_REMOVEUNDERLAYIMAGE, OnRemoveRemoveunderlayimage)
ON_UPDATE_COMMAND_UI(ID_REMOVE_REMOVEUNDERLAYIMAGE,
  OnUpdateRemoveRemoveunderlayimage)
ON_COMMAND(ID_SHOW_SHOWUNDERLAYBITMAP, OnShowShowunderlaybitmap)
ON_UPDATE_COMMAND_UI(ID_SHOW_SHOWUNDERLAYBITMAP, OnUpdateShowShowunderlaybitmap)
ON_COMMAND(ID_CCLINE_INSERTCCSECTIONCMD0X70, OnCclineInsertccsectioncmd0x70)
ON_COMMAND(ID_DRAWOUTLINES, OnDrawoutlines)
ON_UPDATE_COMMAND_UI(ID_DRAWOUTLINES, OnUpdateDrawoutlines)
ON_COMMAND(ID_SHOWTREES, OnShowtrees)
ON_UPDATE_COMMAND_UI(ID_SHOWTREES, OnUpdateShowtrees)
ON_COMMAND(ID_SHOW_SHOWTEXTUREMAPPINGCOMMANDS, OnShowShowtexturemappingcommands)
ON_UPDATE_COMMAND_UI(ID_SHOW_SHOWTEXTUREMAPPINGCOMMANDS,
  OnUpdateShowShowtexturemappingcommands)
ON_COMMAND(ID_SHOW_SHOWVIEWDISTANCECOMMANDS, OnShowShowviewdistancecommands)
ON_UPDATE_COMMAND_UI(ID_SHOW_SHOWVIEWDISTANCECOMMANDS,
  OnUpdateShowShowviewdistancecommands)
ON_COMMAND(ID_SHOW_SHOWVIEWDISTANCES, OnShowShowviewdistances)
ON_UPDATE_COMMAND_UI(ID_SHOW_SHOWVIEWDISTANCES, OnUpdateShowShowviewdistances)
ON_WM_CHAR()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TrackEditorScrollView drawing

void TrackEditorScrollView::OnInitialUpdate()
{
  CScrollView::OnInitialUpdate();

  CSize sizeTotal;
  // TODO: calculate the total size of this view
  sizeTotal.cx = sizeTotal.cy = 1000;
  SetScrollSizes(MM_TEXT, sizeTotal);
}

void TrackEditorScrollView::Resize(double scale)
{
  CSize sizeTotal;
  // TODO: calculate the total size of this view
  CTrackEditorDoc* pDoc = GetDocument();
  if (pDoc) {
    sizeTotal = pDoc->GetSize(scale);
  } else {
    sizeTotal.cx = sizeTotal.cy = 1000;
  }

  if (sizeTotal.cx >= 32000) sizeTotal.cx = 32000;
  if (sizeTotal.cy >= 32000) sizeTotal.cy = 32000;
  if (sizeTotal.cx <= -32000) sizeTotal.cx = -32000;
  if (sizeTotal.cy <= -32000) sizeTotal.cy = -32000;
  SetScrollSizes(MM_TEXT, sizeTotal);
}

void TrackEditorScrollView::OnDraw(CDC* pDC)
{
  CRect rect;
  GetClientRect(rect);

  bool orientation = true;

  int m_nPageWidth = pDC->GetDeviceCaps(HORZRES);
  int m_nPageHeight = pDC->GetDeviceCaps(VERTRES);

  POINT pt;
  // Init our pt struct in case escape not supported
  pt.x = 0;
  pt.y = 0;

  // In Windows, use GETPRINTINGOFFSET to fill the POINT struct
  // Drivers are not required to support the GETPRINTINGOFFSET escape,
  // so call the QUERYESCSUPPORT printer escape to make sure
  // it is supported.
  if (pDC->IsPrinting()) {
    if (true) {
      int nTemp = GETPRINTINGOFFSET;
      int bBandInfoDevice =
        pDC->Escape(QUERYESCSUPPORT, sizeof(int), (LPSTR)&nTemp, NULL);

      if (bBandInfoDevice) {
        pDC->Escape(GETPRINTINGOFFSET, NULL, NULL, (LPPOINT)&pt);
      } else {
        std::cerr << "Printer does not support margins" << std::endl;
      }
    } else {
      // In Windows NT, the following 2 calls replace GETPRINTINGOFFSET:
      pt.x = pDC->GetDeviceCaps(PHYSICALOFFSETX);
      pt.y = pDC->GetDeviceCaps(PHYSICALOFFSETY);
    }
  }

  int xOffset = max(0, pt.x);
  int yOffset = max(0, pt.y);

  if (pDC->IsPrinting()) {
    orientation = ((CTrackEditorApp*)AfxGetApp())->IsLandScape();
    pDC->SetMapMode(MM_ANISOTROPIC);// allows independent axes
    // pDC->SetWindowOrg(0,0);
    pDC->SetWindowOrg(0, 0);

    // ok ok its abit of a hack but when you have the screen
    // in a normal windows size great width than height this
    // goes really bad when printing to portriat!
    // I had alot of problems fitting the printing model to
    // the old way of doing things this is about as good as I
    // can get it for now
    // an orientation of true mean landscape false being portriat
    double aspect_ratio = (double)rect.Height() / (double)rect.Width();
    double nneight = aspect_ratio * 1000.0;
    if (orientation) {
      pDC->SetWindowExt(1000, (int)nneight);// sets   logical window units
        // t.scale( 1000, nneight );
      // pDC->SetWindowExt( rect.Width(),rect.Height()) ;   // sets   logical
      // window units t.scale( rect.Width(), rect.Height() ); t.scale(
      // m_nPageWidth, m_nPageHeight );
      // set the printer margin
    } else {
      pDC->SetWindowExt(12 * rect.Height(),
        12 * rect.Width());// sets   logical window units
      // t.scale( rect.Height(), rect.Width() );
    }

    pDC->SetViewportOrg(xOffset, yOffset);

    // set the page size
    pDC->SetViewportExt(m_nPageWidth, m_nPageHeight);
  }
  OnMyDraw(pDC);
}

void TrackEditorScrollView::OnMyDraw(CDC* pDC)
{
  CTrackEditorDoc* pDoc = GetDocument();
  // TODO: add draw code here

  GPTrack* track = pDoc->getTrack();
  track->setView(this);

  // instead of erasebkgrnd
  CBrush backBrush(GetSysColor(COLOR_WINDOW));
  // Save old brush
  CBrush* pOldBrush = pDC->SelectObject(&backBrush);

  CRect rect;
  pDC->GetClipBox(&rect);// Erase the area needed

  pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
  pDC->SelectObject(pOldBrush);

  char buff[256];
  if (track) {
    wsprintf(buff, "%s : %2.2f:1", track->getCircuitName(), display->scale);
    SetWindowText(buff);
  }

  if (track->showUnderlayBitmap && trackImage != NULL) {
    RECT rect;
    GetClientRect(&rect);

    rect.left = track->UnderLayBitmapX.GetValue();
    rect.top = track->UnderLayBitmapY.GetValue();

    CSize imageSize = trackImage->GetDimensions();

    if (track->maintainBitmapAspectRatio) {
      rect.right = rect.left + track->UnderLayBitmapWidth.GetValue();
      double aspect = ((double)imageSize.cy / (double)imageSize.cx);
      double owidth = (double)track->UnderLayBitmapWidth.GetValue();
      int height = (int)(aspect * owidth);
      rect.bottom = rect.top + height;
    } else {
      rect.right = rect.left + track->UnderLayBitmapWidth.GetValue();
      rect.bottom = rect.top + track->UnderLayBitmapHeight.GetValue();
    }

    rect.left = display->getScreenX(rect.left);
    rect.right = display->getScreenX(rect.right);
    rect.top = display->getScreenX(rect.top);
    rect.bottom = display->getScreenX(rect.bottom);
    trackImage->Draw(pDC, &rect);
  }

  if (track) {
    if (display) {
      display->setGraphics(pDC);
      display->setColor(0);

      if (currentTool->isLineTool()) {
        track->drawTrack(display);
        track->drawGrid(display);
        track->drawTrackLines(display);
      } else if (currentTool->isPitLineTool()) {
        track->drawGrid(display);
        track->drawTrack(display, TRUE);
        track->drawPitlane(display);
        track->drawPitLines(display);
      } else if (currentTool->isCCLineTool()) {
        track->drawGrid(display);
        track->drawTrack(display, TRUE);
        track->drawCCLines(display);
      } else {
        track->drawGrid(display);
        track->drawTrack(display);
        track->drawPitlane(display);
        track->drawCCLine(display);
        track->drawCameras(display);
        track->drawBlackFlags(display);
        track->DrawCCDataLog(display);
      }

      TrackSection* t = track->getTrackSelection();
      TrackSection* pitsel = track->getPitLaneSelection();

      if (t && currentTool->isTrackTool()) {
        char message[256];
        CPoint pt = GetScrollPosition();
        pDC->SelectObject(GetStockObject(ANSI_VAR_FONT));
        wsprintf(
          message,
          "Selected Track Section: [%d] Angle=%d Length=%d Delta_Height=%d",
          t->index,
          (int)AngleFrom(t->getAngle()),
          (int)t->getLength(),
          t->getHeight());
        theApp.SetMessageText(message);
      }

      else if (pitsel && currentTool->isPitTool()) {
        char message[256];
        CPoint pt = GetScrollPosition();
        pDC->SelectObject(GetStockObject(ANSI_VAR_FONT));
        wsprintf(
          message,
          "Selected Pit Section: [%d] Angle=%d Length=%d Delta_Height=%d",
          pitsel->index,
          (int)AngleFrom(pitsel->getAngle()),
          (int)pitsel->getLength(),
          pitsel->getHeight());
        theApp.SetMessageText(message);
      }
      TrackObject* o = track->GetSelectedObject();
      if (o && currentTool->isObjectTool()) {
        char message[256];
        CPoint pt = GetScrollPosition();
        pDC->SelectObject(GetStockObject(ANSI_VAR_FONT));
        TrackObjectDefinition* obj = o->getObjectDef();
        if (obj) {
          wsprintf(message, "Selected Object: %s (id1=%d id2=%d)", obj->getName(), obj->id, obj->id2);
          theApp.SetMessageText(message);
        }
      }

      CCLineSection* line = track->getCCSelection();
      if (line && currentTool->isCCTool()) {
        int idx = track->getCCSelectionIndex();
        char message[256];
        CPoint pt = GetScrollPosition();
        pDC->SelectObject(GetStockObject(ANSI_VAR_FONT));
        wsprintf(message, "Selected CC Line: idx=%d length=%d radius=%d", idx, line->getLength(), (int)line->getRadius());
        // pDC->TextOut(10+pt.x,25+pt.y,message);
        theApp.SetMessageText(message);
      }
    }
  }

  GPTrack* overlay = pDoc->getOverlayTrack();

  if (overlay) {
    if (overlay->isValid()) {
      if (display) {
        display->setGraphics(pDC);
        display->setColor(2);
        overlay->drawTrack(display);
        overlay->drawPitlane(display);
      }
    }
  }

  if (ruler != NULL) {
    ruler->draw(display);
  }

  if (track->showSplits) track->DrawSplits(display);
}

/////////////////////////////////////////////////////////////////////////////
// TrackEditorScrollView diagnostics

#ifdef _DEBUG
void TrackEditorScrollView::AssertValid() const
{
  CScrollView::AssertValid();
}

void TrackEditorScrollView::Dump(CDumpContext& dc) const
{
  CScrollView::Dump(dc);
}
#endif//_DEBUG

/////////////////////////////////////////////////////////////////////////////
// TrackEditorScrollView message handlers

void TrackEditorScrollView::OnIn()
{
  // TODO: Add your command handler code here
  display->ZoomIn();
  Resize(display->getScale());
  repaint();
}

void TrackEditorScrollView::OnOut()
{
  // TODO: Add your command handler code here
  display->ZoomOut();
  Resize(display->getScale());
  repaint();
}

void TrackEditorScrollView::repaint()
{
  RECT rect;

  GetClientRect(&rect);
  InvalidateRect(&rect);
  update();
}

void TrackEditorScrollView::update()
{
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  int pitid = mytrack->getPitSelectionIndex();
  int trackid = mytrack->getTrackSelectionIndex();
  int ccid = mytrack->getCCSelectionIndex();
  int objid = mytrack->getObjectSelectionIndex();

  if (ccid >= 0) {
    if (pDoc->ccTable) {
      if (pDoc->ccTable) {
        pDoc->ccTable->setSelected(ccid);
      }
    }
  }

  if (trackid >= 0) {
    if (pDoc->trackTable) {
      pDoc->trackTable->setSelected(trackid);
    }
  }

  if (pitid >= 0 && pDoc->pitTable) {
    pDoc->pitTable->setSelected(pitid);
  }

  if (objid >= 0 && pDoc->objectTable) {
    pDoc->objectTable->setSelected(objid);
  }
}

extern char lpszFileFilter[];

void TrackEditorScrollView::OnTrack()
{
  char fileName[256];

  CString strSection = "Preferences";
  CString strStringItem = "GP2TrackLoadLocation";
  CWinApp* pApp = AfxGetApp();

  CString dirName =
    pApp->GetProfileString(strSection, strStringItem, "c:\\gp2\\Circuits");

  // SetCurrentDirectory(dirName);

  strcpy(fileName, "*.dat");
  CFileDialog* fdlg =
    new CFileDialog(FALSE, "dat", (LPCSTR)&fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFileFilter);

  int result = fdlg->DoModal();
  if (result == IDOK) {
    CString filename = fdlg->GetFileName();

    CTrackEditorDoc* pDoc = GetDocument();

    GPTrack* over = pDoc->getOverlayTrack();

    if (over) {
      over->ReadTrackFile(pDoc, (const char*)filename);
    } else {
      AfxMessageBox("Overlay is NULL", MB_OK);
    }

    char pwd[1024];
    GetCurrentDirectory(1024, pwd);
    pApp->WriteProfileString(strSection, strStringItem, CString(pwd));
  }
}

void TrackEditorScrollView::OnLButtonDown(UINT nFlags, CPoint point)
{
  CTrackEditorDoc* pDoc = GetDocument();
  if (pDoc) {
    if (currentTool) {
      if (!currentTool->isA(TOOL_ZOOM_ID)) {
        GPTrack* track = pDoc->getTrack();
        track->MakeBackupNow();
      }
    }
  }

  if (currentTool != NULL) {
    if (nFlags & MK_SHIFT) {
      OnMButtonDown(nFlags, point);
    } else
      currentTool->OnLButtonDown(this, pDoc, nFlags, point);
  }

  CScrollView::OnLButtonDown(nFlags, point);
}

void TrackEditorScrollView::OnMouseMove(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  CTrackEditorDoc* pDoc = GetDocument();
  if (currentTool != NULL) currentTool->OnMouseMove(this, pDoc, nFlags, point);

  CScrollView::OnMouseMove(nFlags, point);
}

void TrackEditorScrollView::OnLButtonUp(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  CTrackEditorDoc* pDoc = GetDocument();

  if (currentTool != NULL) {
    if (nFlags & MK_SHIFT)
      OnMButtonUp(nFlags, point);
    else
      currentTool->OnLButtonUp(this, pDoc, nFlags, point);
  }

  CScrollView::OnLButtonUp(nFlags, point);
}

void TrackEditorScrollView::OnMButtonDown(UINT nFlags, CPoint point)
{
  CTrackEditorDoc* pDoc = GetDocument();
  if (currentTool != NULL)
    currentTool->OnRButtonDown(this, pDoc, nFlags, point);

  CScrollView::OnMButtonDown(nFlags, point);
}

void TrackEditorScrollView::OnMButtonUp(UINT nFlags, CPoint point)
{
  CTrackEditorDoc* pDoc = GetDocument();
  if (currentTool != NULL) currentTool->OnRButtonUp(this, pDoc, nFlags, point);

  CScrollView::OnMButtonUp(nFlags, point);
}

void TrackEditorScrollView::OnPitlane()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  mytrack->showPitLane = !mytrack->showPitLane;

  repaint();
}

void TrackEditorScrollView::OnObjects()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  mytrack->showObjects = !mytrack->showObjects;

  repaint();
}

void TrackEditorScrollView::OnLine()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  mytrack->showCCLine = !mytrack->showCCLine;

  repaint();
}

void TrackEditorScrollView::OnViewTrack()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  mytrack->showTrack = !mytrack->showTrack;

  // Update Menu
  int flags;

  CMenu* menu = GetMenu();

  if (mytrack->showTrack)
    flags = MF_CHECKED;
  else
    flags = MF_UNCHECKED;

  if (menu) {
    // menu->CheckMenuItem(VIEW_TRACK,flags);
  }

  repaint();
}

BOOL TrackEditorScrollView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
  // TODO: Add your specialized code here and/or call the base class
  CListCtrl m_ListCtl;

  int id = CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

  return id;
}

void TrackEditorScrollView::OnViewShowwalls()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  mytrack->showWalls = !mytrack->showWalls;

  // Update Menu
  int flags;

  CMenu* menu = GetMenu();

  if (mytrack->showWalls)
    flags = MF_BYCOMMAND | MF_CHECKED;
  else
    flags = MF_BYCOMMAND | MF_UNCHECKED;

  // menu->CheckMenuItem(VIEW_TRACK,flags);

  repaint();
}

void TrackEditorScrollView::OnUpdateTrack(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  UPDATE_TRACK(showTrack);
}

void TrackEditorScrollView::OnViewShowall()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  mytrack->ShowHideAll(TRUE);
  repaint();
}

void TrackEditorScrollView::OnViewHideall()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  mytrack->ShowHideAll(FALSE);
  repaint();
}

void TrackEditorScrollView::OnWall()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  mytrack->showWalls = !mytrack->showWalls;

  // Update Menu
  int flags;

  CMenu* menu = GetMenu();

  if (mytrack->showWalls)
    flags = MF_BYCOMMAND | MF_CHECKED;
  else
    flags = MF_BYCOMMAND | MF_UNCHECKED;

  // menu->CheckMenuItem(VIEW_TRACK,flags);

  repaint();
}

void TrackEditorScrollView::OnDrivingLine()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  mytrack->showCCLine = !mytrack->showCCLine;

  repaint();
}

void TrackEditorScrollView::OnViewShowcameras()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  mytrack->showCameras = !mytrack->showCameras;

  repaint();
}

void TrackEditorScrollView::OnViewObjBitmaps()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  mytrack->showBitmaps = !mytrack->showBitmaps;

  repaint();
}

void TrackEditorScrollView::OnFilePrint()
{
  CView::OnFilePrint();
  // TODO: Add your command handler code here
}

void TrackEditorScrollView::OnFilePrintPreview()
{
  // TODO: Add your command handler code here
  CView::OnFilePrintPreview();
}

void TrackEditorScrollView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
  // TODO: Add your specialized code here and/or call the base class

  CScrollView::OnBeginPrinting(pDC, pInfo);
}

void TrackEditorScrollView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
  // TODO: Add your specialized code here and/or call the base class

  CScrollView::OnEndPrinting(pDC, pInfo);
}

void TrackEditorScrollView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView)
{
  // TODO: Add your specialized code here and/or call the base class

  CScrollView::OnEndPrintPreview(pDC, pInfo, point, pView);
}

BOOL TrackEditorScrollView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // TODO: Add your specialized code here and/or call the base class
  return DoPreparePrinting(pInfo);
  // return CScrollView::OnPreparePrinting(pInfo);
}

void TrackEditorScrollView::OnObjectTool()
{
  // TODO: Add your command handler code here
  if (currentTool != NULL) delete currentTool;

  currentTool = new ObjectTool();
  CTrackEditorDoc* pDoc = GetDocument();
  pDoc->UpdateAllViews(NULL);
}

void TrackEditorScrollView::OnPointer()
{
  // TODO: Add your command handler code here
  if (currentTool != NULL) delete currentTool;

  currentTool = new PointerTool();
  CTrackEditorDoc* pDoc = GetDocument();
  pDoc->UpdateAllViews(NULL);
}

void TrackEditorScrollView::OnTrackTool()
{
  // TODO: Add your command handler code here
  if (currentTool != NULL) delete currentTool;

  currentTool = new TrackTool();
  AfxGetApp()->LoadStandardCursor(IDC_ARROW);
  CTrackEditorDoc* pDoc = GetDocument();
  pDoc->UpdateAllViews(NULL);
}

void TrackEditorScrollView::OnZoomtool()
{
  // TODO: Add your command handler code here
  if (currentTool != NULL) delete currentTool;

  currentTool = new ZoomTool();
  // LoadStandardCursor(MAKEINTRESOURCE(IDC_ZOOMCUR));
  CTrackEditorDoc* pDoc = GetDocument();
  pDoc->UpdateAllViews(NULL);
}

/*
void TrackEditorScrollView::OnUpdatePitlane(CCmdUI* pCmdUI)
{
        UPDATE_TRACK(showPitLane);
}

  */
void TrackEditorScrollView::OnUpdateCcline(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showCCLine);
}

/*
void TrackEditorScrollView::OnUpdateObjects(CCmdUI* pCmdUI)
{
        UPDATE_TRACK(showObjects);
}

void TrackEditorScrollView::OnUpdateWalls(CCmdUI* pCmdUI)
{
        UPDATE_TRACK(showWalls);
}

void TrackEditorScrollView::OnWalls()
{
        // TODO: Add your command handler code here
        OnWall();
}

*/
void TrackEditorScrollView::OnCcline()
{
  // TODO: Add your command handler code here
  OnDrivingLine();
}

void TrackEditorScrollView::OnUpdateViewObjBitmaps(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  UPDATE_TRACK(showBitmaps);
}

void TrackEditorScrollView::OnUpdateZoomtool(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool) pCmdUI->SetCheck(currentTool->isA(TOOL_ZOOM_ID));
}

void TrackEditorScrollView::OnUpdatePointer(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool) pCmdUI->SetCheck(currentTool->isA(TOOL_POINTER_ID));
}

void TrackEditorScrollView::OnUpdateObjectTool(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool) pCmdUI->SetCheck(currentTool->isA(TOOL_OBJECT_ID));
}

void TrackEditorScrollView::OnUpdateTrackTool(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool) pCmdUI->SetCheck(currentTool->isA(TOOL_TRACK_ID));
}

void TrackEditorScrollView::OnPitlaneTool()
{
  // TODO: Add your command handler code here
  if (currentTool != NULL) delete currentTool;

  currentTool = new PitLaneTool();
  AfxGetApp()->LoadStandardCursor(IDC_CROSS);
  CTrackEditorDoc* pDoc = GetDocument();
  pDoc->UpdateAllViews(NULL);
}

void TrackEditorScrollView::OnUpdatePitlaneTool(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool) pCmdUI->SetCheck(currentTool->isA(TOOL_PITLANE_ID));
}

void TrackEditorScrollView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  CTrackEditorDoc* pDoc = GetDocument();
  if (currentTool != NULL)
    currentTool->OnLButtonDblClk(this, pDoc, nFlags, point);

  CScrollView::OnLButtonDblClk(nFlags, point);
}

void TrackEditorScrollView::OnWalltool()
{
  // TODO: Add your command handler code here
  if (currentTool != NULL) delete currentTool;

  currentTool = new WallTool();
  AfxGetApp()->LoadStandardCursor(IDC_CROSS);
  CTrackEditorDoc* pDoc = GetDocument();
  pDoc->UpdateAllViews(NULL);
}

void TrackEditorScrollView::OnUpdateWalltool(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool) pCmdUI->SetCheck(currentTool->isA(TOOL_WALL_ID));
}

void TrackEditorScrollView::OnCctool()
{
  // TODO: Add your command handler code here
  if (currentTool != NULL) delete currentTool;

  currentTool = new CCTool();
  AfxGetApp()->LoadStandardCursor(IDC_CROSS);
  CTrackEditorDoc* pDoc = GetDocument();
  pDoc->UpdateAllViews(NULL);
}

void TrackEditorScrollView::OnUpdateCctool(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool) pCmdUI->SetCheck(currentTool->isA(TOOL_CC_ID));
}

void TrackEditorScrollView::OnUpdateViewShowcameras(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  UPDATE_TRACK(showCameras);
}

void TrackEditorScrollView::OnInsertObj()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  if (currentTool != NULL) currentTool->InsertObject(this, pDoc);
}

void TrackEditorScrollView::OnInsertTracksegment()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  if (currentTool != NULL) currentTool->InsertSection(this, pDoc);
}

void TrackEditorScrollView::OnSaveTrackImage()
{
  // TODO: Add your command handler code her
#ifdef PAUL
  RECT rect;
  GetClientRect(&rect);

  CDC* pDC = GetDC();

  CBitmap* bitmap = new CBitmap();

  int width = rect.right - rect.left;
  int height = rect.bottom - rect.top;

  bitmap->CreateCompatibleBitmap(pDC, width, height);

  pDC->SelectObject(bitmap);
  OnDraw(pDC);

  int size = (width * height);
  // char *buffer = (char*)malloc(width*height*sizeof(char));

  // bitmap->GetBitmapBits(size,buffer);

  CSize imgSize(width, height);
  CDib* trackImage = new CDib(imgSize, 8, TRUE);

  int palCount = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      DWORD color = pDC->GetPixel(j, i);

      BOOL FoundColor = FALSE;
      for (int k = 0; k < palCount; k++) {
        DWORD palCol = trackImage->GetPaletteEntry(i);
        if (color == palCol) {
          FoundColor = TRUE;
          break;
        }
      }
      if (FoundColor != TRUE) {
        trackImage->SetPaletteEntry(palCount, color);
        palCount++;
      }
    }
  }

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      CPoint pt(j, i);
      DWORD color = pDC->GetPixel(j, i);
      for (int k = 0; k < palCount; k++) {
        DWORD palCol = trackImage->GetPaletteEntry(i);
        if (color == palCol) {
          trackImage->SetPixel(pt, palCol);
          break;
        }
      }
    }
  }

  delete bitmap;
  // free(buffer);

  /*
  int palSubSize = 20;

  for(int i=0;i<palSubSize;i++)
  {
    // BGR
    DWORD color = RGB(100+(i*(100/palSubSize)),0,0);
    trackImage->SetPaletteEntry(i,color);
  }

  for(int y=0;y<400;y++)
  {
          DWORD colorIndex = (y/(400/palSubSize));
          for(int x=0;x<640;x++)
          {
                  CPoint pt(x,y);
                  trackImage->SetPixel(pt,colorIndex);
          }
  }
  */
  trackImage->Write("track.bmp", TRUE);
#else
  AfxMessageBox("Currently Not Supported");
#endif
}

void TrackEditorScrollView::OnRemoveCcSection()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  if (currentTool != NULL && currentTool->isCCTool()) {
    currentTool->RemoveSection(this, pDoc);
    pDoc->OnRefreshTree();
  }
}

void TrackEditorScrollView::OnRemovePitObject()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  if (currentTool != NULL && currentTool->isPitTool()) {
    currentTool->RemoveObject(this, pDoc);
    pDoc->OnRefreshTree();
  }
}

void TrackEditorScrollView::OnRemovePitSection()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  if (currentTool != NULL && currentTool->isPitTool()) {
    currentTool->RemoveSection(this, pDoc);
    pDoc->OnRefreshTree();
  }
}

void TrackEditorScrollView::OnRemoveTrackObject()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  if (currentTool != NULL && (currentTool->isTrackTool() || currentTool->isObjectTool())) {
    currentTool->RemoveObject(this, pDoc);
    pDoc->OnRefreshTree();
  }
}

void TrackEditorScrollView::OnRemoveTrackSection()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  if (currentTool != NULL && currentTool->isTrackTool()) {
    currentTool->RemoveSection(this, pDoc);
    pDoc->OnRefreshTree();
  }
}

void TrackEditorScrollView::OnInsertCclinesection()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  if (currentTool != NULL && currentTool->isCCTool())
    currentTool->InsertSection(this, pDoc);
}

void TrackEditorScrollView::OnInsertPitcmd()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();
  if (currentTool != NULL && currentTool->isPitTool()) {
    TrackSection* selected = mytrack->getPitLaneSelection();
    if (selected != NULL) {
      selected->InsertNew();
      pDoc->UpdateTree(selected->getTreeNode());
    } else
      AfxMessageBox("No Pit Section Selected");
  }
}

void TrackEditorScrollView::OnInsertPitobject()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  if (currentTool != NULL && currentTool->isPitTool())
    currentTool->InsertObject(this, pDoc);
}

void TrackEditorScrollView::OnInsertPitsection()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  if (currentTool != NULL && currentTool->isPitTool())
    currentTool->InsertSection(this, pDoc);
}

void TrackEditorScrollView::OnInsertTrackcmd()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();
  if (currentTool != NULL && currentTool->isTrackTool()) {
    TrackSection* selected = mytrack->getTrackSelection();
    if (selected != NULL) {
      selected->InsertNew();
      mytrack->createTrack(pDoc);
      pDoc->UpdateTree(selected->getTreeNode());
    } else
      AfxMessageBox("No Track Section Selected");
  }
}

void TrackEditorScrollView::CreateViewPopupMenu()
{
  // HMENU hmnu = ::GetMenu(m_hWnd);
  // popup.Attach( hmnu );
}

void TrackEditorScrollView::OnRButtonDown(UINT nFlags, CPoint point)
{
  CMenu menu;
  CMenu* submenu;

  // load the menu
  menu.LoadMenu(IDR_LAUNCH);

  // get the popup menu
  submenu = menu.GetSubMenu(0);

  // convert to screen coordinates
  ClientToScreen(&point);

  // post the menu
  submenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetApp()->m_pMainWnd, NULL);

  CScrollView::OnRButtonDown(nFlags, point);
}

void TrackEditorScrollView::OnRButtonUp(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  // changingAngle = FALSE;

  CTrackEditorDoc* pDoc = GetDocument();
  if (currentTool != NULL) currentTool->OnMButtonUp(this, pDoc, nFlags, point);

  CScrollView::OnRButtonUp(nFlags, point);
}

void TrackEditorScrollView::OnShowGreen()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  mytrack->showGreenScreen = !mytrack->showGreenScreen;

  repaint();
}

void TrackEditorScrollView::OnUpdateShowGreen(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  UPDATE_TRACK(showGreenScreen);
}

BOOL TrackEditorScrollView::OnEraseBkgnd(CDC* pDC)
{
  // TODO: Add your message handler code here and/or call default
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  if (mytrack->showGreenScreen) {
    CBrush backBrush(RGB(0, 128, 0));
    // Save old brush
    CBrush* pOldBrush = pDC->SelectObject(&backBrush);

    CRect rect;
    pDC->GetClipBox(&rect);// Erase the area needed

    pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
    pDC->SelectObject(pOldBrush);
    return TRUE;
  } else {
    // return CScrollView::OnEraseBkgnd(pDC);
    return TRUE;
  }
}

/*
void TrackEditorScrollView::OnShowTracksides()
{
        // TODO: Add your command handler code here
        CTrackEditorDoc* pDoc = GetDocument();
        GPTrack * mytrack = pDoc->getTrack();

        mytrack->showTrackSide = !mytrack->showTrackSide;

        repaint();
}

void TrackEditorScrollView::OnUpdateShowTracksides(CCmdUI* pCmdUI)
{
        // TODO: Add your command update UI handler code here
        UPDATE_TRACK(showTrackSide);
}
*/

void TrackEditorScrollView::OnShowFinish()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  mytrack->showFinishLine = !mytrack->showFinishLine;

  repaint();
}

void TrackEditorScrollView::OnUpdateShowFinish(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  UPDATE_TRACK(showFinishLine);
}

void TrackEditorScrollView::OnUpdateInsertPitcmd(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool != NULL) pCmdUI->Enable(currentTool->isPitTool());
}

void TrackEditorScrollView::OnUpdateInsertPitobject(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool != NULL) pCmdUI->Enable(currentTool->isPitTool());
}

void TrackEditorScrollView::OnUpdateInsertPitsection(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool != NULL) pCmdUI->Enable(currentTool->isPitTool());
}

void TrackEditorScrollView::OnUpdateInsertObj(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool != NULL)
    pCmdUI->Enable(currentTool->isTrackTool() || currentTool->isObjectTool());
}

void TrackEditorScrollView::OnUpdateInsertCclinesection(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool != NULL) pCmdUI->Enable(currentTool->isCCTool());
}

void TrackEditorScrollView::OnUpdateInsertTracksegment(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool != NULL) pCmdUI->Enable(currentTool->isTrackTool());
}

void TrackEditorScrollView::OnUpdateInsertTrackcmd(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool != NULL) pCmdUI->Enable(currentTool->isTrackTool());
}

void TrackEditorScrollView::OnUpdateRemoveCcSection(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool != NULL) pCmdUI->Enable(currentTool->isCCTool());
}

void TrackEditorScrollView::OnUpdateRemovePitObject(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool != NULL) pCmdUI->Enable(currentTool->isPitTool());
}

void TrackEditorScrollView::OnUpdateRemovePitSection(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool != NULL) pCmdUI->Enable(currentTool->isPitTool());
}

void TrackEditorScrollView::OnRemovePitcmds()
{
  // TODO: Add your command handler code here
  AfxMessageBox("Removing Pitlane Commands not supported");
}

void TrackEditorScrollView::OnUpdateRemovePitcmds(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool != NULL) pCmdUI->Enable(currentTool->isPitTool());
}

void TrackEditorScrollView::OnUpdateRemoveTrackObject(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool != NULL)
    pCmdUI->Enable(currentTool->isTrackTool() || currentTool->isObjectTool());
}

void TrackEditorScrollView::OnUpdateRemoveTrackSection(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool != NULL) pCmdUI->Enable(currentTool->isTrackTool());
}

void TrackEditorScrollView::OnRemovePitCmd()
{
  // TODO: Add your command handler code here
  AfxMessageBox("Removing Pitlane Commands not supported");
}

void TrackEditorScrollView::OnUpdateRemovePitCmd(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool != NULL) pCmdUI->Enable(currentTool->isPitTool());
}

void TrackEditorScrollView::OnRemoveTrackCmd()
{
  // TODO: Add your command handler code here
  AfxMessageBox(
    "Removing Track Commands not supported from view yet used listtree!");
}

void TrackEditorScrollView::OnUpdateRemoveTrackCmd(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool != NULL) pCmdUI->Enable(currentTool->isTrackTool());
}

char lpszBmpFilter[] = "BMP Image File (*.bmp)|*.bmp|All Files (*.*)|*.*|";

void TrackEditorScrollView::OnImportImage()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  char fileName[256];

  strcpy(fileName, "*.bmp");

  CFileDialog* fdlg = new CFileDialog(TRUE, "bmp", (LPCSTR)&fileName, OFN_HIDEREADONLY, lpszBmpFilter);

  int result = fdlg->DoModal();
  if (result == IDOK) {
    CString file = fdlg->GetFileName();
    CString dir = fdlg->GetPathName();
    // clear old image
    if (trackImage != NULL) delete trackImage;

    trackImage = new CDib();
    trackImage->Read(file);
    mytrack->showUnderlayBitmap = TRUE;
    theApp.WriteProfileString("Preferences", "UnderLayImageName", dir);
  }
}

void TrackEditorScrollView::OnRuler()
{
  // TODO: Add your command handler code here
  if (currentTool != NULL) delete currentTool;

  currentTool = new RulerTool();
  CTrackEditorDoc* pDoc = GetDocument();
  pDoc->UpdateAllViews(NULL);
}

void TrackEditorScrollView::OnUpdateRuler(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  // if (currentTool != NULL) pCmdUI->Enable(currentTool->isRulerTool());
  if (currentTool) pCmdUI->SetCheck(currentTool->isA(TOOL_RULER_ID));
}

void TrackEditorScrollView::OnInsertRemoveoverlay()
{
  // TODO: Add your command handler code here
  if (trackImage != NULL) delete trackImage;

  trackImage = NULL;
  repaint();
}

void TrackEditorScrollView::OnUpdateInsertRemoveoverlay(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  pCmdUI->Enable((trackImage != NULL));
}

CTreeCtrl*
  TrackEditorScrollView::getTree()
{
  CTrackEditorDoc* pDoc = GetDocument();
  return pDoc->getTree();
}

void TrackEditorScrollView::OnZoomHome()
{
  // TODO: Add your command handler code here
  POINT pos;
  pos.x = 0;
  pos.y = 0;
  ScrollToPosition(pos);

  display->ZoomHome();
  Resize(display->getScale());
  repaint();
}

// Insertion Macro
#define INSERT_TRACK_CMD(x)                                   \
  {                                                           \
    CTrackEditorDoc* pDoc = GetDocument();                    \
    GPTrack* mytrack = pDoc->getTrack();                      \
    if (currentTool != NULL && currentTool->isTrackTool()) {  \
      TrackSection* selected = mytrack->getTrackSelection();  \
      int selectionIndex = mytrack->getTrackSelectionIndex(); \
      if (selected != NULL) {                                 \
        selected->InsertCmdAt(selectionIndex, x);             \
        pDoc->UpdateTree(selected->getTreeNode());            \
      } else                                                  \
        AfxMessageBox("No Track Section Selected");           \
    }                                                         \
  }

void TrackEditorScrollView::OnChangeTrackWidthLeft()
{
  INSERT_TRACK_CMD(0xB4)
}

void TrackEditorScrollView::OnChangeTrackwidth()
{
  INSERT_TRACK_CMD(0x85)
}

void TrackEditorScrollView::OnChangeTrackwidthRight()
{
  // TODO: Add your command handler code here
  INSERT_TRACK_CMD(0xB5)
}

void TrackEditorScrollView::OnUpdateChangeTrackWidthLeft(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool != NULL) pCmdUI->Enable(currentTool->isTrackTool());
}

void TrackEditorScrollView::OnUpdateChangeTrackwidth(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool != NULL) pCmdUI->Enable(currentTool->isTrackTool());
}

void TrackEditorScrollView::OnUpdateChangeTrackwidthRight(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool != NULL) pCmdUI->Enable(currentTool->isTrackTool());
}

void TrackEditorScrollView::OnChangeTrackwallLeft()
{
  // TODO: Add your command handler code here
  INSERT_TRACK_CMD(0x98)
}

void TrackEditorScrollView::OnUpdateChangeTrackwallLeft(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool != NULL) pCmdUI->Enable(currentTool->isTrackTool());
}

void TrackEditorScrollView::OnChangeTrackwallRight()
{
  // TODO: Add your command handler code here
  INSERT_TRACK_CMD(0x99)
}

void TrackEditorScrollView::OnUpdateChangeTrackwallRight(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool != NULL) pCmdUI->Enable(currentTool->isTrackTool());
}

void TrackEditorScrollView::OnShowRemovedWalls()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  mytrack->showRemovedWalls = !mytrack->showRemovedWalls;

  repaint();
}

void TrackEditorScrollView::OnUpdateShowRemovedWalls(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  UPDATE_TRACK(showRemovedWalls);
}

void TrackEditorScrollView::OnShowRoadsigns()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  mytrack->showRoadSigns = !mytrack->showRoadSigns;

  repaint();
}

void TrackEditorScrollView::OnUpdateShowRoadsigns(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  UPDATE_TRACK(showRoadSigns);
}

void TrackEditorScrollView::OnShowCommands()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  mytrack->showCommands = !mytrack->showCommands;

  repaint();
}

void TrackEditorScrollView::OnUpdateShowCommands(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  UPDATE_TRACK(showCommands);
}

/*
void TrackEditorScrollView::OnRefreshTree()
{
        // TODO: Add your command handler code here
        CTrackEditorDoc* pDoc = GetDocument();
        GPTrack * mytrack = pDoc->getTrack();

    mytrack->createTrack(pDoc);
        mytrack->createCC();
        mytrack->createPitlane();
        repaint();
}
*/

void TrackEditorScrollView::OnAddKerbIn()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection* t = (TrackSection*)mytrack->TrackSections->elementAt(i);

    if (t->getAngle() != 0) {
      if (t->getAngle() < 0) {
        t->setLeftKerb(TRUE);
      } else {
        t->setRightKerb(TRUE);
      }
    }
  }
  repaint();
}

void TrackEditorScrollView::OnKerbAddOut()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection* t = (TrackSection*)mytrack->TrackSections->elementAt(i);

    if (t->getAngle() != 0) {
      if (t->getAngle() < 0) {
        t->setRightKerb(TRUE);
      } else {
        t->setLeftKerb(TRUE);
      }
    }
  }
  repaint();
}

void TrackEditorScrollView::OnKerbRemoveIn()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection* t = (TrackSection*)mytrack->TrackSections->elementAt(i);

    if (t->getAngle() != 0) {
      if (t->getAngle() < 0) {
        t->setLeftKerb(FALSE);
      } else {
        t->setRightKerb(FALSE);
      }
    }
  }
  repaint();
}

void TrackEditorScrollView::OnKerbRemoveOut()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection* t = (TrackSection*)mytrack->TrackSections->elementAt(i);

    if (t->getAngle() != 0) {
      if (t->getAngle() < 0) {
        t->setRightKerb(FALSE);
      } else {
        t->setLeftKerb(FALSE);
      }
    }
  }
  repaint();
}

void TrackEditorScrollView::OnKerbRemoveStraight()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection* t = (TrackSection*)mytrack->TrackSections->elementAt(i);

    if (t->getAngle() == 0) {
      t->setLeftKerb(FALSE);
      t->setRightKerb(FALSE);
    }
  }
  repaint();
}

void TrackEditorScrollView::OnKerbRunoff()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  int runnofflength = 20;

  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection* t = (TrackSection*)mytrack->TrackSections->elementAt(i);
    TrackSection* prev = mytrack->GetPreviousSection(t);

    // previous section was a corner
    if (prev->getAngle() != 0) {
      // now we are on the straight
      if (t->getAngle() == 0) {
        int runnlen = 0;

        // while(runnlen < runnofflength)
        //{
        // runnlen += t->getLength();
        if (prev->getAngle() < 0) {
          t->setRightKerb(TRUE);
        } else {
          t->setLeftKerb(TRUE);
        }
        // t = mytrack->GetNextSection(t);
        //}
      }
    }
  }
  repaint();
}

void TrackEditorScrollView::OnKerbRemoveall()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection* t = (TrackSection*)mytrack->TrackSections->elementAt(i);

    t->setLeftKerb(FALSE);
    t->setRightKerb(FALSE);
  }
  repaint();
}

void TrackEditorScrollView::OnShowCenterline()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  mytrack->showCenterLine = !mytrack->showCenterLine;

  repaint();
}

void TrackEditorScrollView::OnUpdateShowCenterline(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  UPDATE_TRACK(showCenterLine);
}

void TrackEditorScrollView::OnAddGravel()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection* t = (TrackSection*)mytrack->TrackSections->elementAt(i);

    if (t->getAngle() != 0) {
      // left corner
      if (t->getAngle() < 0) {
        TrackCmd* gravel = new TrackCmd(mytrack, t, 0xBC, 0, "GRAVEL", 0, 8, t->getLength(), 164, 3, 16, 0, 1);
        t->addCommand(gravel);
        t->setRightToBank(160);
        t->setLeftToBank(40);
      }
      // right corner
      else {
        TrackCmd* gravel = new TrackCmd(mytrack, t, 0xBC, 0, "GRAVEL", 0, 9, t->getLength(), 164, 3, 16, 0, 33);
        t->addCommand(gravel);
        t->setRightToBank(40);
        t->setLeftToBank(160);
      }
    }
  }
  repaint();
}

void TrackEditorScrollView::OnAddGravelLeft()
{
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  if (currentTool != NULL && currentTool->isTrackTool()) {
    TrackSection* selected = mytrack->getTrackSelection();
    if (selected != NULL) {
      selected->AddGravelTrapLeft();
      pDoc->UpdateTree(selected->getTreeNode());
    } else {
      AfxMessageBox("No Track Section Selected");
    }
  } else
    AfxMessageBox("Switch to Track Editing tool");
  repaint();
}

void TrackEditorScrollView::OnAddGravelRight()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  if (currentTool != NULL && currentTool->isTrackTool()) {
    TrackSection* selected = mytrack->getTrackSelection();
    if (selected != NULL) {
      selected->AddGravelTrapRight();
      pDoc->UpdateTree(selected->getTreeNode());
    } else {
      AfxMessageBox("No Track Section Selected");
    }
  } else
    AfxMessageBox("Switch to Track Editing tool");
  repaint();
}

void TrackEditorScrollView::OnAddTarmacLeft()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  if (currentTool != NULL && currentTool->isTrackTool()) {
    TrackSection* selected = mytrack->getTrackSelection();
    if (selected != NULL) {
      selected->AddTarmacLeft();
      pDoc->UpdateTree(selected->getTreeNode());
    } else {
      AfxMessageBox("No Track Section Selected");
    }
  } else
    AfxMessageBox("Switch to Track Editing tool");
  repaint();
}

void TrackEditorScrollView::OnAddTarmacRight()
{
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  if (currentTool != NULL && currentTool->isTrackTool()) {
    TrackSection* selected = mytrack->getTrackSelection();
    if (selected != NULL) {
      selected->AddTarmacRight();
      pDoc->UpdateTree(selected->getTreeNode());
    } else {
      AfxMessageBox("No Track Section Selected");
    }
  } else
    AfxMessageBox("Switch to Track Editing tool");
  repaint();
}

void TrackEditorScrollView::OnAddTypeWallRight()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  if (currentTool != NULL && currentTool->isTrackTool()) {
    TrackSection* selected = mytrack->getTrackSelection();
    if (selected != NULL) {
      selected->AddTypeWallRight();
      pDoc->UpdateTree(selected->getTreeNode());
    } else {
      AfxMessageBox("No Track Section Selected");
    }
  } else
    AfxMessageBox("Switch to Track Editing tool");
  repaint();
}

void TrackEditorScrollView::OnAddTyreWallLeft()
{
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  if (currentTool != NULL && currentTool->isTrackTool()) {
    TrackSection* selected = mytrack->getTrackSelection();
    if (selected != NULL) {
      selected->AddTyreWallLeft();
      pDoc->UpdateTree(selected->getTreeNode());
    } else {
      AfxMessageBox("No Track Section Selected");
    }
  } else
    AfxMessageBox("Switch to Track Editing tool");
  repaint();
}

void TrackEditorScrollView::OnAddTrackside()
{
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  if (currentTool != NULL && currentTool->isTrackTool()) {
    TrackSection* selected = mytrack->getTrackSelection();
    CTextureChooser* tc = new CTextureChooser(mytrack);

    if (selected != NULL) {
      tc->setInfo(0, selected->length.GetValue(), 0, 0, 0, 0, 0, 0);
      int res = tc->DoModal();

      if (res != IDCANCEL) {
        selected->AddTrackSideMarking(
          tc->SelectedOffset, tc->SelectedLocation, tc->SelectedLength, tc->SelectedTexture, tc->SelectedRepeats, tc->SelectedResolution, tc->SelectedYOffset, tc->SelectedRotation);
        pDoc->UpdateTree(selected->getTreeNode());
        mytrack->regenerate = TRUE;
      }
    } else {
      AfxMessageBox("No Track Section Selected");
    }
    delete tc;
  } else
    AfxMessageBox("Switch to Track Editing tool");

  repaint();
}

void TrackEditorScrollView::OnShowKerbs()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  mytrack->showKerbs = !mytrack->showKerbs;

  repaint();
}

void TrackEditorScrollView::OnUpdateShowKerbs(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  UPDATE_TRACK(showKerbs);
}

#define CHECK_OUT(x)                     \
  CTrackEditorDoc* pDoc = GetDocument(); \
  GPTrack* mytrack = pDoc->getTrack();   \
  mytrack->##x = !mytrack->##x;          \
  repaint();

void TrackEditorScrollView::OnShowGrip()
{
  CHECK_OUT(showGrip)
}

void TrackEditorScrollView::OnUpdateShowGrip(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showGrip);
}

void TrackEditorScrollView::OnShowBlackFlags()
{
  CHECK_OUT(showBlackFlags)
}

void TrackEditorScrollView::OnUpdateShowBlackFlags(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showBlackFlags);
}

void TrackEditorScrollView::OnUpdatePointerTool(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (currentTool) pCmdUI->SetCheck(currentTool->isPointerTool());
}

void TrackEditorScrollView::OnShowScenery()
{
  // TODO: Add your command handler code here
  CHECK_OUT(showScenery)
}

void TrackEditorScrollView::OnUpdateShowScenery(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  UPDATE_TRACK(showScenery);
}

void TrackEditorScrollView::OnViewShowtrackpies()
{
  // TODO: Add your command handler code here
  CHECK_OUT(showTrackPie)
}

void TrackEditorScrollView::OnUpdateViewShowtrackpies(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  UPDATE_TRACK(showTrackPie)
}

void TrackEditorScrollView::OnInsertBlackLeft()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  if (currentTool != NULL && currentTool->isTrackTool()) {
    TrackSection* selected = mytrack->getTrackSelection();
    if (selected != NULL) {
      selected->AddBlackFlagLeft(100);
      pDoc->UpdateTree(selected->getTreeNode());
    } else {
      AfxMessageBox("No Track Section Selected");
    }
  } else
    AfxMessageBox("Switch to Track Editing tool");
  repaint();
}

void TrackEditorScrollView::OnInsertBlackRight()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  if (currentTool != NULL && currentTool->isTrackTool()) {
    TrackSection* selected = mytrack->getTrackSelection();
    if (selected != NULL) {
      selected->AddBlackFlagRight(100);
      pDoc->UpdateTree(selected->getTreeNode());
    } else {
      AfxMessageBox("No Track Section Selected");
    }
  } else
    AfxMessageBox("Switch to Track Editing tool");
  repaint();
}

void TrackEditorScrollView::OnShowGridlines()
{
  // TODO: Add your command handler code here
}

void TrackEditorScrollView::OnUpdateShowGridlines(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
}

void TrackEditorScrollView::OnShowWorldExtent()
{
  CHECK_OUT(showGridLines)
}

void TrackEditorScrollView::OnUpdateShowWorldExtent(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showGridLines);
}

void TrackEditorScrollView::OnShowTracknumbers()
{
  CHECK_OUT(showTrackNumbers)
}

void TrackEditorScrollView::OnUpdateShowTracknumbers(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showTrackNumbers);
}

void TrackEditorScrollView::OnShowTextures()
{
  CHECK_OUT(showUseTextures)
}

void TrackEditorScrollView::OnUpdateShowTextures(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showUseTextures);
}

void TrackEditorScrollView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  // TODO: Add your message handler code here and/or call default
  CTrackEditorDoc* pDoc = GetDocument();
  if (currentTool != NULL)
    currentTool->OnKeyDown(this, pDoc, nChar, nRepCnt, nFlags);

  CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void TrackEditorScrollView::OnShowPitcmds()
{
  CHECK_OUT(showPitCmds)
}

void TrackEditorScrollView::OnUpdateShowPitcmds(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showPitCmds);
}

void TrackEditorScrollView::OnShowDistances()
{
  CHECK_OUT(showTrackDistances)
}

void TrackEditorScrollView::OnUpdateShowDistances(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showTrackDistances)
}

void TrackEditorScrollView::OnShowUnkVarious()
{
  CHECK_OUT(showUnkVariousTable)
}

void TrackEditorScrollView::OnUpdateShowUnkVarious(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showUnkVariousTable)
}

void TrackEditorScrollView::OnShowCcNumbers()
{
  CHECK_OUT(showCCNumbers)
}

void TrackEditorScrollView::OnUpdateShowCcNumbers(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showCCNumbers)
}

void TrackEditorScrollView::OnShowShowcurvedwallsaslines()
{
  CHECK_OUT(showCurvedWallStraight)
}

void TrackEditorScrollView::OnUpdateShowShowcurvedwallsaslines(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showCurvedWallStraight)
}

void TrackEditorScrollView::OnShowObjMarkers()
{
  CHECK_OUT(showObjectPositionMarker)
}

void TrackEditorScrollView::OnUpdateShowObjMarkers(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showObjectPositionMarker)
}

void TrackEditorScrollView::OnMaintainAspect()
{
  CHECK_OUT(maintainBitmapAspectRatio)
}

void TrackEditorScrollView::OnUpdateMaintainAspect(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(maintainBitmapAspectRatio)
}

void TrackEditorScrollView::OnShowGridPaper()
{
  CHECK_OUT(showGraphPaper)
}

void TrackEditorScrollView::OnUpdateShowGridPaper(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showGraphPaper)
}

#define CHECK_GRAPH_SIZE(x)              \
  CTrackEditorDoc* pDoc = GetDocument(); \
  GPTrack* mytrack = pDoc->getTrack();   \
  mytrack->gridSizeOption = x;
#define UPDATE_GRAPH_SIZE(x)                      \
  CTrackEditorDoc* pDoc = GetDocument();          \
  GPTrack* mytrack = pDoc->getTrack();            \
  pCmdUI->SetCheck(mytrack->gridSizeOption == x); \
  pDoc->UpdateAllViews(NULL);

void TrackEditorScrollView::OnHalfmile()
{
  CHECK_GRAPH_SIZE(HALF_MILE)
}

void TrackEditorScrollView::OnUpdateHalfmile(CCmdUI* pCmdUI)
{
  UPDATE_GRAPH_SIZE(HALF_MILE)
}

void TrackEditorScrollView::On10meters()
{
  CHECK_GRAPH_SIZE(TEN_METERS)
}

void TrackEditorScrollView::OnUpdate10meters(CCmdUI* pCmdUI)
{
  UPDATE_GRAPH_SIZE(TEN_METERS)
}

void TrackEditorScrollView::On500meters()
{
  CHECK_GRAPH_SIZE(FIVE_HUNDRED_METERS)
}

void TrackEditorScrollView::OnUpdate500meters(CCmdUI* pCmdUI)
{
  UPDATE_GRAPH_SIZE(FIVE_HUNDRED_METERS)
}

void TrackEditorScrollView::On50meters()
{
  CHECK_GRAPH_SIZE(FIFTY_METERS)
}

void TrackEditorScrollView::OnUpdate50meters(CCmdUI* pCmdUI)
{
  UPDATE_GRAPH_SIZE(FIFTY_METERS)
}

void TrackEditorScrollView::OnOnekm()
{
  CHECK_GRAPH_SIZE(ONE_KM)
}

void TrackEditorScrollView::OnUpdateOnekm(CCmdUI* pCmdUI)
{
  UPDATE_GRAPH_SIZE(ONE_KM)
}

/*
void TrackEditorScrollView::OnMile()
{
        CHECK_GRAPH_SIZE(ONE_MILE)
}

void TrackEditorScrollView::OnUpdateMile(CCmdUI* pCmdUI)
{
        UPDATE_GRAPH_SIZE(ONE_MILE)
}
*/

void TrackEditorScrollView::On100meters()
{
  CHECK_GRAPH_SIZE(ONE_HUNDRED_METERS)
}

void TrackEditorScrollView::OnUpdate100meters(CCmdUI* pCmdUI)
{
  UPDATE_GRAPH_SIZE(ONE_HUNDRED_METERS)
}

void TrackEditorScrollView::OnOnemile()
{
  CHECK_GRAPH_SIZE(ONE_MILE)
}

void TrackEditorScrollView::OnUpdateOnemile(CCmdUI* pCmdUI)
{
  UPDATE_GRAPH_SIZE(ONE_MILE)
}

void TrackEditorScrollView::OnLinetool()
{
  // AfxMessageBox("Tool Not Available Yet!");

  if (currentTool != NULL) delete currentTool;

  currentTool = new LineTool();
  CTrackEditorDoc* pDoc = GetDocument();
  pDoc->UpdateAllViews(NULL);
}

void TrackEditorScrollView::OnUpdateLinetool(CCmdUI* pCmdUI)
{
  if (currentTool) pCmdUI->SetCheck(currentTool->isLineTool());
}

void TrackEditorScrollView::OnShowTrackMarkings()
{
  CHECK_OUT(showMarkings)
}

void TrackEditorScrollView::OnUpdateShowTrackMarkings(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showMarkings)
}

void TrackEditorScrollView::OnPitlinetool()
{
  if (currentTool != NULL) delete currentTool;

  currentTool = new PitLineTool();
  CTrackEditorDoc* pDoc = GetDocument();
  pDoc->UpdateAllViews(NULL);
}

void TrackEditorScrollView::OnUpdatePitlinetool(CCmdUI* pCmdUI)
{
  if (currentTool) pCmdUI->SetCheck(currentTool->isPitLineTool());
}

void TrackEditorScrollView::OnShowAnnotations()
{
  CHECK_OUT(showAnnotations)
}

void TrackEditorScrollView::OnUpdateShowAnnotations(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showAnnotations)
}

void TrackEditorScrollView::OnHandtool()
{
  if (currentTool != NULL) delete currentTool;

  currentTool = new HandTool();
  CTrackEditorDoc* pDoc = GetDocument();
  pDoc->UpdateAllViews(NULL);
}

void TrackEditorScrollView::OnUpdateHandtool(CCmdUI* pCmdUI)
{
  if (currentTool) pCmdUI->SetCheck(currentTool->isHandTool());
}

void TrackEditorScrollView::OnCclinetool()
{
  AfxMessageBox("Current not working");
  // if (currentTool != NULL) delete currentTool;

  // currentTool = new CCLineTool();
  // CTrackEditorDoc* pDoc = GetDocument();
  // pDoc->UpdateAllViews(NULL);
}

void TrackEditorScrollView::OnUpdateCclinetool(CCmdUI* pCmdUI)
{
  // if (currentTool) pCmdUI->SetCheck(currentTool->isCCLineTool());
  pCmdUI->SetCheck(FALSE);
}

void TrackEditorScrollView::OnCameratool()
{
  if (currentTool != NULL) delete currentTool;

  currentTool = new CameraTool();
  CTrackEditorDoc* pDoc = GetDocument();
  pDoc->UpdateAllViews(NULL);
}

void TrackEditorScrollView::OnUpdateCameratool(CCmdUI* pCmdUI)
{
  if (currentTool) pCmdUI->SetCheck(currentTool->isCameraTool());
}

void TrackEditorScrollView::OnDefaultSides()
{
  CHECK_OUT(showDefaultSides)
}

void TrackEditorScrollView::OnUpdateDefaultSides(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showDefaultSides)
}

void TrackEditorScrollView::OnShowShowccdatalog()
{
  CHECK_OUT(showCCDataLog)
}

void TrackEditorScrollView::OnUpdateShowShowccdatalog(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showCCDataLog)
}

void TrackEditorScrollView::OnShowShowsplits()
{
  CHECK_OUT(showSplits)
}

void TrackEditorScrollView::OnUpdateShowShowsplits(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showSplits)
}

void TrackEditorScrollView::OnShowShowpitfences()
{
  CHECK_OUT(showPitWalls)
}

void TrackEditorScrollView::OnUpdateShowShowpitfences(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showPitWalls)
}

void TrackEditorScrollView::OnRemoveRemoveunderlayimage()
{
  CTrackEditorDoc* pDoc = GetDocument();
  trackImage = NULL;
  theApp.WriteProfileString("Preferences", "UnderLayImageName", "none");
  pDoc->UpdateAllViews(NULL);
}

void TrackEditorScrollView::OnUpdateRemoveRemoveunderlayimage(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(trackImage != NULL);
}

void TrackEditorScrollView::OnShowShowunderlaybitmap()
{
  CHECK_OUT(showUnderlayBitmap)
}

void TrackEditorScrollView::OnUpdateShowShowunderlaybitmap(CCmdUI* pCmdUI)
{
  // BOOL bitmap = (showUnderlayBitmap && trackImage!=NULL);
  UPDATE_TRACK(showUnderlayBitmap)
}

void TrackEditorScrollView::OnCclineInsertccsectioncmd0x70()
{
  // TODO: Add your command handler code here
  CTrackEditorDoc* pDoc = GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  mytrack->Insert112CmdSection();
}

void TrackEditorScrollView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
  // TODO: Add your specialized code here and/or call the base class
  // double oldscale = display->scale;
  // display->ZoomIn();
  // Resize(display->getScale());
  // repaint();

  CScrollView::OnPrepareDC(pDC, pInfo);
}

void TrackEditorScrollView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
  // TODO: Add your specialized code here and/or call the base class
  double oldscale = display->scale;

  display->scale *= 10.0;
  display->beginPrinting();

  CScrollView::OnPrint(pDC, pInfo);

  display->endPrinting();

  display->scale = oldscale;
}

void TrackEditorScrollView::OnDrawoutlines()
{
  CHECK_OUT(showOutlines)
}

void TrackEditorScrollView::OnUpdateDrawoutlines(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showOutlines)
}

void TrackEditorScrollView::OnShowtrees()
{
  CHECK_OUT(showExternalObjects)
}

void TrackEditorScrollView::OnUpdateShowtrees(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showExternalObjects)
}

void TrackEditorScrollView::OnShowShowtexturemappingcommands()
{
  CHECK_OUT(showTextureMappingCommands)
}

void TrackEditorScrollView::OnUpdateShowShowtexturemappingcommands(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showTextureMappingCommands)
}

void TrackEditorScrollView::OnShowShowviewdistancecommands()
{
  CHECK_OUT(showViewDistanceCommands)
}

void TrackEditorScrollView::OnUpdateShowShowviewdistancecommands(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showViewDistanceCommands)
}

void TrackEditorScrollView::OnShowShowviewdistances()
{
  CHECK_OUT(showViewDistances)
}

void TrackEditorScrollView::OnUpdateShowShowviewdistances(CCmdUI* pCmdUI)
{
  UPDATE_TRACK(showViewDistances)
}

void TrackEditorScrollView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  CTrackEditorDoc* pDoc = GetDocument();
  if (currentTool != NULL)
    currentTool->OnKeyDown(this, pDoc, nChar, nRepCnt, nFlags);

  CScrollView::OnChar(nChar, nRepCnt, nFlags);
}
