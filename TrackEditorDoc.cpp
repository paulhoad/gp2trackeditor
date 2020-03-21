// TrackEditorDoc.cpp : implementation of the CTrackEditorDoc class
//
#include "stdafx.h"

#include <iostream>
#include "TrackEditor.h"

#include "TrackEditorDoc.h"
#include "TrackSection.h"
#include "TrackNameInfo.h"
#include "TrackStats.h"
#include "BankDlg.h"
#include "RandomObject.h"
#include "CmdRemoval.h"
#include "PitEntrance.h"
#include "FindCmd.h"
#include "DirivingLine.h"
#include "JamFileEditor.h"
#include "ScaleData.h"
#include "CScale.h"
#include "JamList.h"
#include "JamPlus.h"
#include "TextureChooser.h"
#include "PitStopStratergy.h"

#include "SceneryEditor.h"

#include "math.h"

#include "Sanity.h"
#include "ObjectName.h"
#include "ObjectProperties.h"
#include "PaletteChooser.h"
#include "IO_Name.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CTrackEditorApp theApp;
// extern CTrackTree      *theMainTree;

/////////////////////////////////////////////////////////////////////////////
// CTrackEditorDoc

IMPLEMENT_DYNCREATE(CTrackEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CTrackEditorDoc, CDocument)
//{{AFX_MSG_MAP(CTrackEditorDoc)
ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
ON_COMMAND(MAKE_CIRCLE, OnCircle)
ON_COMMAND(VIEW_TRACK, OnTrack)
ON_COMMAND(VIEW_PITLANE, OnPitlane)
ON_COMMAND(VIEW_OBJECTS, OnObjects)
ON_COMMAND(VIEW_LINE, OnLine)
ON_COMMAND(VIEW_CHECKSUM, OnChecksum)
ON_COMMAND(ID_EDIT_REMOVEALLBANKS, OnEditRemoveallbanks)
ON_COMMAND(ID_EDIT_REMOVEALLHEIGHT, OnEditRemoveallheight)
ON_COMMAND(ID_VIEW_TRACKSTATISTICS, OnViewTrackstatistics)
ON_COMMAND(ID_EDIT_REMOVEALLANGLES, OnEditRemoveallangles)
ON_COMMAND(ID_EDIT_REMOVEALLCCANGLES, OnEditRemoveallccangles)
ON_COMMAND(ID_EDIT_REMOVEALLCCARGS, OnEditRemoveallccargs)
ON_COMMAND(ID_EDIT_REMOVEALLCCRADIUS, OnEditRemoveallccradius)
ON_COMMAND(ID_EDIT_SETTRACKLENGTHS20, OnEditSettracklengths20)
ON_COMMAND(ID_EDIT_REMOVEALLARG2, OnEditRemoveallarg2)
ON_COMMAND(ID_EDIT_REMOVEALLCCARG1, OnEditRemoveallccarg1)
ON_COMMAND(ID_EDIT_TRACKNAME, OnEditTrackname)
ON_COMMAND(ID_FILE_SAVE, OnFileSave)
ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
ON_COMMAND(ID_REGENERATE_TRACK, OnRegenerateTrack)
ON_COMMAND(ID_TABLES_CCLINEDATA, OnTablesCclinedata)
ON_COMMAND(ID_TABLES_PITLANEDATA, OnTablesPitlanedata)
ON_COMMAND(ID_TABLES_TRACKDATA, OnTablesTrackdata)
ON_COMMAND(ID_EDIT_REMOVEALLFEATURE1, OnEditRemoveallfeature1)
ON_COMMAND(ID_EDIT_REMOVEALLFEATURE2, OnEditRemoveallfeature2)
ON_COMMAND(ID_EDIT_CCCARSETUP, OnEditCccarsetup)
ON_COMMAND(ID_OBJECTDATA, OnObjectdata)
ON_COMMAND(ID_IMPORT, OnImport)
ON_COMMAND(ID_EXPORT, OnexportFile)
ON_COMMAND(ID_FILE_DEBUGOUTPUT, OnFileDebugoutput)
ON_UPDATE_COMMAND_UI(ID_EXPORT, OnUpdateexportFile)
ON_UPDATE_COMMAND_UI(ID_IMPORT, OnUpdateImport)
ON_COMMAND(ID_EDIT_CLEARCCLINE, OnEditClearccline)
ON_COMMAND(ID_CREATE_CCLINE, OnCreateCcline)
ON_COMMAND(ID_RANDOM_OBJECT, OnRandomObject)
ON_COMMAND(ID_REMOVE_TRACKCMDS, OnRemoveTrackcmds)
ON_COMMAND(ID_REMOVE_PITOBJS, OnRemovePitobjs)
ON_COMMAND(ID_REMOVE_PITCMDS, OnRemovePitcmds)
ON_COMMAND(ID_REMOVE_TRACKOBJS, OnRemoveTrackobjs)
ON_COMMAND(ID_WORLDVIEW, OnWorldview)
ON_COMMAND(VIEW_WALLS, OnWalls)
ON_UPDATE_COMMAND_UI(VIEW_WALLS, OnUpdateWalls)
ON_UPDATE_COMMAND_UI(VIEW_TRACK, OnUpdateTrack)
ON_UPDATE_COMMAND_UI(VIEW_PITLANE, OnUpdatePitlane)
ON_UPDATE_COMMAND_UI(VIEW_OBJECTS, OnUpdateObjects)
ON_COMMAND(ID_FILLPOLYGON, OnFillpolygon)
ON_UPDATE_COMMAND_UI(ID_FILLPOLYGON, OnUpdateFillpolygon)
ON_COMMAND(ID_SHOW_TRACKSIDES, OnShowTracksides)
ON_UPDATE_COMMAND_UI(ID_SHOW_TRACKSIDES, OnUpdateShowTracksides)
ON_COMMAND(ID_TABLES_TRACKVIEW, OnTablesTrackview)
ON_COMMAND(ID_VIEW_COMMANDS, OnViewCommands)
ON_COMMAND(ID_REMOVE_XX_TRACKCMDS, OnRemoveXxTrackcmds)
ON_COMMAND(ID_REMOVE_SCENERY, OnRemoveScenery)
ON_COMMAND(ID_REMOVE_SAND, OnRemoveSand)
ON_COMMAND(ID_MOVE_PITENTRANCE, OnMovePitentrance)
ON_COMMAND(ID_VIEW_PITLANE_INFO, OnViewPitlaneInfo)
ON_COMMAND(ID_FIND_COMMAND, OnFindCommand)
ON_COMMAND(ID_FIND_AGAIN, OnFindAgain)
ON_UPDATE_COMMAND_UI(ID_FIND_AGAIN, OnUpdateFindAgain)
ON_COMMAND(ID_REFRESH_TREE, OnRefreshTree)
ON_COMMAND(ID_CAMERA_VIEW, OnCameraView)
ON_COMMAND(ID_ADD_ROADSIGNS, OnAddRoadsigns)
ON_COMMAND(ID_ZERO_SCENERY, OnZeroScenery)
ON_COMMAND(ID_REMOVE_PITLANE_HEIGHT, OnRemovePitlaneHeight)
ON_COMMAND(ID_SHOW_JAM, OnShowJam)
ON_COMMAND(ID_EDIT_DRIVING_LINE, OnEditDrivingLine)
ON_COMMAND(ID_EXPORT_TRACK_CMDS, OnExportTrackCmds)
ON_COMMAND(ID_INSERT_INTERNAL_OBJECT, OnInsertInternalObject)
ON_COMMAND(ID_EXPORT_OBJECT_INFO, OnExportObjectInfo)
ON_COMMAND(ID_VIEW_3DOBJECT, OnView3dobject)
ON_COMMAND(ID_EDIT_SCALETRACK, OnEditScaletrack)
ON_COMMAND(ID_CC_SCALAR, OnCcScalar)
ON_COMMAND(ID_EXPORT_NAMELIST, OnExportNamelist)
ON_COMMAND(ID_CREATE_CC_LINETWO, OnCreateCcLinetwo)
ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
ON_COMMAND(ID_EDIT_CUT, OnEditCut)
ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
ON_COMMAND(ID_TABLE_SCENERY, OnTableScenery)
ON_COMMAND(ID_STANDARDSCENERY, OnStandardscenery)
ON_COMMAND(ID_NEW_OBJECT_DEFINITION, OnNewObjectDefinition)
ON_COMMAND(ID_IMPORT_TRIF, OnImportTrif)
ON_COMMAND(ID_REMOVE_TRACKCMDS_NONZERO, OnRemoveTrackcmdsNonzero)
ON_COMMAND(ID_TRACK_REMOVE_ALL, OnTrackRemoveAll)
ON_COMMAND(ID_JAM_INDEX, OnJamIndex)
ON_COMMAND(ID_REMOVE_TRACK_ANNOTATIONS, OnRemoveTrackAnnotations)
ON_COMMAND(ID_DEFAULT_TYRE_WALL_TEXTURE, OnDefaultTyreWallTexture)
ON_COMMAND(ID_DEFAULT_GRAVEL_TEXTURE, OnDefaultGravelTexture)
ON_COMMAND(ID_DEFAULT_TARMAC_TEXTURE, OnDefaultTarmacTexture)
ON_COMMAND(ID_TRACKGLVIEW, OnTrackglview)
ON_COMMAND(ID_FILE_IMPORTCCRECORD, OnFileImportccrecord)
ON_COMMAND(ID_FILE_IMPORT_IMPORTTRACKSEGMENTDATA,
           OnFileImportImporttracksegmentdata)
ON_COMMAND(ID_EXPORT_TRACK_XY, OnExportTrackXy)
ON_COMMAND(ID_CREATE_CCLINE_PONK, OnCreateCclinePonk)
ON_COMMAND(ID_REMOVE_REMOVEALLWALLHEIGHTCHANGES,
           OnRemoveRemoveallwallheightchanges)
ON_COMMAND(ID_REMOVE_REMOVEALLTRACKWIDTHCHANGES,
           OnRemoveRemovealltrackwidthchanges)
ON_COMMAND(ID_REMOVE_REMOVEEVERYSCENERYCOMMAND,
           OnRemoveRemoveeveryscenerycommand)
ON_COMMAND(ID_REMOVE_REMOVEALL, OnRemoveRemoveall)
ON_COMMAND(ID_EDIT_PERFORMSANITYCHECK, OnEditPerformsanitycheck)
ON_COMMAND(ID_HELP_SCENERYLOCATIONTYPEB, OnHelpScenerylocationtypeb)
ON_COMMAND(ID_REMOVE_REMOVEMARTJINSSUGGESTEDSCENERY,
           OnRemoveRemovemartjinssuggestedscenery)
ON_COMMAND(ID_PITLANE_PITSTOPSTRATERGY, OnPitlanePitstopstratergy)
ON_COMMAND(ID_FILE_EXPORT_EXPORTVARIOUSBLOCKS, OnFileExportExportvariousblocks)
ON_COMMAND(ID_FILE_EXPORT_EXPORTCCSETUP, OnFileExportExportccsetup)
ON_COMMAND(ID_HELP_SCENERYLOCATIONTYPEBC, OnHelpScenerylocationtypebc)
ON_COMMAND(ID_HELP_SCENERYLOCATIONTYPEA, OnHelpScenerylocationtypea)
ON_COMMAND(ID_HELP_JAMIDSUNUSABLE, OnHelpJamidsunusable)
ON_COMMAND(ID_TABLE_PIT_SCENERY, OnTablePitScenery)
ON_COMMAND(ID_CCLINE_STRAIGHTENCCLINE, OnCclineStraightenccline)
ON_COMMAND(ID_SHOW_TEXTURES, OnShowTextures)
ON_UPDATE_COMMAND_UI(ID_SHOW_TEXTURES, OnUpdateShowTextures)
ON_COMMAND(ID_TEXTUREMAPOBJECTS, OnTexturemapobjects)
ON_UPDATE_COMMAND_UI(ID_TEXTUREMAPOBJECTS, OnUpdateTexturemapobjects)
ON_COMMAND(ID_RIGHTPOP_RENAMEOBJECT, OnRightpopRenameobject)
ON_COMMAND(ID_RIGHTPOP_PROPERTIES, OnRightpopProperties)
ON_COMMAND(ID_HELP_GP2PALETTE, OnHelpGp2palette)
ON_COMMAND(ID_VIEW_SHOWCAMERAS, OnViewShowcameras)
ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWCAMERAS, OnUpdateViewShowcameras)
ON_COMMAND(ID_REMOVE_REMOVEALLILLEGALCAMERAS, OnRemoveRemoveallillegalcameras)
ON_COMMAND(ID_REMOVE_AUTOSPACECAMERAS, OnRemoveAutospacecameras)
ON_COMMAND(ID_REMOVE_REMOVEINTERNALOBJECTNAMES,
           OnRemoveRemoveinternalobjectnames)
ON_COMMAND(ID_VIEW_JAMTOTALSIZE, OnViewJamtotalsize)
ON_COMMAND(ID_OBJECTJAMS_TOTALJAMSIZE, OnObjectjamsTotaljamsize)
ON_COMMAND(ID_FILE_EXPORT_EXPORTTRACKINFO, OnFileExportExporttrackinfo)
ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
ON_COMMAND(ID_FILE_EXPORT_EXPORTTOTXFTRACKEXCHANGEFORMAT,
           OnFileExportExporttotxftrackexchangeformat)
ON_COMMAND(ID_FILE_EXPORT_EXPORTTOVRML, OnFileExportExporttovrml)
ON_COMMAND(ID_HELP_0XC5LOCATIONTYPED, OnHelp0xc5locationtyped)
ON_COMMAND(ID_HELP_0XA0LOCATIONTYPEE, OnHelp0xa0locationtypee)
ON_COMMAND(ID_DEBUG_OUTPUT, OnFileDebugoutput)
ON_COMMAND(ID_AIRVIEW, OnTrackglview)
ON_COMMAND(ID_SHOW_SHOWSCENERYARMS, OnShowShowsceneryarms)
ON_UPDATE_COMMAND_UI(ID_SHOW_SHOWSCENERYARMS, OnUpdateShowShowsceneryarms)
ON_COMMAND(ID_SHOW_USESWIVELANGLES, OnShowUseswivelangles)
ON_UPDATE_COMMAND_UI(ID_SHOW_USESWIVELANGLES, OnUpdateShowUseswivelangles)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackEditorDoc construction/destruction

CTrackEditorDoc::CTrackEditorDoc()
    : ccTable(NULL),
      pitTable(NULL),
      trackTable(NULL),
      objectTable(NULL),
      F1GP(false)
{
  maintrack = NULL;
  overlay = NULL;
  currentJam = NULL;
}

CTrackEditorDoc::~CTrackEditorDoc() {}

void
CTrackEditorDoc::DeleteContents()
{
  GPTrack *mytrack = getTrack();

  if (mytrack != NULL) {
    if (mytrack->regenerate) {
      char buff[256];
      wsprintf(buff,
               "The track %s has been modified since it was last would you "
               "like to save this file?",
               mytrack->getCircuitName());
      int result = AfxMessageBox(buff, MB_YESNO, MB_ICONWARNING);

      if (result == IDYES) {
        OnFileSaveAs();
      }
    }

    if (mytrack->TrackRootNode)
      theApp.getMainTree()->deleteNode(mytrack->TrackRootNode);
    mytrack->TrackRootNode = NULL;

    mytrack->setValid(FALSE);
    delete mytrack;
  }
  mytrack = NULL;
}

BOOL
CTrackEditorDoc::OnNewDocument()
{
  AfxMessageBox(
      "This is an experimental option to create new track from scratch, track "
      "may not work!");

  if (!CDocument::OnNewDocument()) return FALSE;

  maintrack = new GPTrack();
  maintrack->NewTrack();
  getTrack()->TrackRootNode = theApp.getMainTree()->insertTrack(getTrack());
  OnCreateCcline();
  maintrack->SetDocument(this);

  return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CTrackEditorDoc serialization

void
CTrackEditorDoc::Serialize(CArchive &ar)
{
  if (ar.IsStoring()) {
    // TODO: add storing code here
  }
  else {
    // TODO: add loading code here
  }
}

/////////////////////////////////////////////////////////////////////////////
// CTrackEditorDoc diagnostics

#ifdef _DEBUG
void
CTrackEditorDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void
CTrackEditorDoc::Dump(CDumpContext &dc) const
{
  CDocument::Dump(dc);
}
#endif  //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTrackEditorDoc commands

BOOL
CTrackEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
  CString strSection = "Preferences";
  CString strStringItem = "GP2TrackLoadLocation";
  CWinApp *pApp = AfxGetApp();

  if (!CDocument::OnOpenDocument(lpszPathName)) return FALSE;

  F1GP = ((CTrackEditorApp *)pApp)->isF1GP;

  if (maintrack != NULL) delete maintrack;

  maintrack = new GPTrack();
  getTrack()->SetDocument(this);

  getTrack()->ReadTrackFile(this, lpszPathName);
  getTrack()->TrackRootNode = theApp.getMainTree()->insertTrack(getTrack());

  char pwd[1026];
  GetCurrentDirectory(1026, pwd);

  pApp->WriteProfileString(strSection, strStringItem, CString(pwd));

  return TRUE;
}

void
CTrackEditorDoc::MakeVisible(HTREEITEM Node)
{
  if (Node != NULL) {
    HTREEITEM parent = getTree()->GetParentItem(Node);
    if (parent != NULL) {
      getTree()->Expand(parent, TVE_EXPAND);
    }
    getTree()->SelectItem(Node);
    getTree()->Expand(Node, TVE_EXPAND);
  }
}

CSize
CTrackEditorDoc::GetSize(double scale)
{
  GPTrack *track = getTrack();
  CSize totalSize;

  totalSize.cx = 512;
  totalSize.cy = 512;

  if (track) {
    int size = track->TrackSections->size();
    double twoscale = 2.0 * scale;

    for (int i = 0; i < size; i++) {
      TrackSection *t = (TrackSection *)track->TrackSections->elementAt(i);
      totalSize.cx = (long)max(totalSize.cx, (twoscale)*t->getStartX());
      totalSize.cy = (long)max(totalSize.cy, (twoscale)*t->getStartY());
      totalSize.cx = (long)max(totalSize.cx, (twoscale)*t->getEndX());
      totalSize.cy = (long)max(totalSize.cy, (twoscale)*t->getEndY());
    }
  }
  return totalSize;
}

CSize
CTrackEditorDoc::FindTopLeft()
{
  GPTrack *track = getTrack();
  CSize totalSize;

  totalSize.cx = 0;
  totalSize.cy = 0;

  if (track) {
    for (int i = 0; i < track->TrackSections->size(); i++) {
      TrackSection *t = (TrackSection *)track->TrackSections->elementAt(i);
      totalSize.cx = (long)min(totalSize.cx, t->getStartX());
      totalSize.cy = (long)min(totalSize.cy, t->getStartY());
      totalSize.cx = (long)min(totalSize.cx, t->getEndX());
      totalSize.cy = (long)min(totalSize.cy, t->getEndY());
    }
  }
  TRACE("%d %d\n", totalSize.cx, totalSize.cy);
  return totalSize;
}

char lpszFileFilter[] = "F1GP2 Track Files (*.dat)|*.dat|All Files (*.*)|*.*|";

extern BOOL
CheckRegistrationName();

void
CTrackEditorDoc::OnFileSaveAs()
{
  GPTrack *mytrack = getTrack();
  mytrack->regenerate = TRUE;

  mytrack->RecreateData();
  mytrack->regenerate = FALSE;

  LPCSTR name = (LPCSTR)mytrack->getCircuitName();
  LPCSTR country = (LPCSTR)mytrack->getCircuitCountry();

  if (!isF1GP()) {
    if (!mytrack->isCircuitDefined() && !mytrack->isCountryDefined()) {
      CTrackNameInfo *trkdlg = new CTrackNameInfo();
      trkdlg->DoModal();
      trkdlg->getTrackInfo(mytrack);
    }
  }
  CString lastName = mytrack->getFileName();
  CFileDialog *fdlg;

  char fileName[1024];
  char pwd[1024];

  CString strSection = "Preferences";
  CString strStringItem = "GP2TrackSaveLocation";
  CWinApp *pApp = AfxGetApp();

  CString dirName =
      pApp->GetProfileString(strSection, strStringItem, "c:\\gp2\\Circuits");

  // SetCurrentDirectory(dirName);

  if (mytrack->isCountryDefined()) {
    wsprintf(fileName, "%s_%s_%s.dat", mytrack->circuitName,
             mytrack->countryName, mytrack->circuitYear);
  }
  else {
    wsprintf(fileName, "%s", lastName);
  }

  fdlg =
      new CFileDialog(FALSE, "dat", (LPCSTR)&fileName,
                      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFileFilter);

  int result = fdlg->DoModal();

  if (result == IDOK) {
    CString filename = fdlg->GetFileName();
    CString path = fdlg->GetPathName();
    mytrack->setFileName(filename);
    mytrack->WriteTrackFile(filename);
    GetCurrentDirectory(1024, pwd);
    pApp->WriteProfileString(strSection, strStringItem, CString(pwd));
    pApp->AddToRecentFileList(path);
  }
  delete fdlg;
}

void
CTrackEditorDoc::OnCircle()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();
  mytrack->regenerate = TRUE;

  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);
    t->setAngle(-1);
    t->setLength(5);
  }
}

void
CTrackEditorDoc::OnTrack()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  mytrack->showTrack = !mytrack->showTrack;

  UpdateAllViews(NULL);
}

void
CTrackEditorDoc::OnPitlane()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  mytrack->showPitLane = !mytrack->showPitLane;

  UpdateAllViews(NULL);
}

void
CTrackEditorDoc::OnObjects()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  mytrack->showObjects = !mytrack->showObjects;

  UpdateAllViews(NULL);
}

void
CTrackEditorDoc::OnLine()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();
}

void
CTrackEditorDoc::OnChecksum()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  mytrack->CalculateCheckSum();
  mytrack->WriteSum(0, FALSE, TRUE);
}

void
CTrackEditorDoc::OnEditRemoveallbanks()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  CBankDlg *dlg = new CBankDlg(0, mytrack->TrackSections->size());
  int result = dlg->DoModal();

  if (result == IDOK) {
    mytrack->regenerate = TRUE;
    int begin = dlg->m_From;
    int end = dlg->m_To;

    if (end <= begin) {
      AfxMessageBox("End is before begin");
      return;
    }

    if (!dlg->m_ChangeLeft && !dlg->m_ChangeRight) {
      AfxMessageBox("No side change boxes checked");
    }

    for (int i = begin; i < end; i++) {
      TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);
      if (dlg->m_ChangeLeft) t->setLeftToBank(dlg->m_LeftBank);
      if (dlg->m_ChangeRight) t->setRightToBank(dlg->m_RightBank);
    }

    mytrack->createTrack(this);
    UpdateAllViews(NULL);
  }
}

void
CTrackEditorDoc::OnEditRemoveallheight()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();
  mytrack->regenerate = TRUE;

  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);
    t->setHeight(0);
    // mytrack->PutTrackSection(t);
  }
}

void
CTrackEditorDoc::OnViewTrackstatistics()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();
  CTrackStats *nameinfo = new CTrackStats();

  nameinfo->setTrack(mytrack);

  nameinfo->DoModal();

  // nameinfo->getTrackInfo(mytrack);
}

void
CTrackEditorDoc::OnEditRemoveallangles()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();
  mytrack->regenerate = TRUE;

  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);
    t->setAngle(0);
    // mytrack->PutTrackSection(t);
  }
}

void
CTrackEditorDoc::OnEditRemoveallccangles()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  for (int i = 0; i < mytrack->CCLineSections->size(); i++) {
    CCLineSection *t = (CCLineSection *)mytrack->CCLineSections->elementAt(i);
    t->setRadius(0);
    // mytrack->PutCCSection(t);
  }
  mytrack->regenerate = TRUE;
}

void
CTrackEditorDoc::OnEditRemoveallccargs()
{
  // TODO: Add your command handler code here
}

void
CTrackEditorDoc::OnEditRemoveallccradius()
{
  // TODO: Add your command handler code here
}

void
CTrackEditorDoc::OnEditSettracklengths20()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  int size = mytrack->TrackSections->size();
  int offset = 20;
  int arcsize = 4;
  int mid = size / 2;

  mytrack->regenerate = TRUE;

  for (int i = 0; i < size; i++) {
    TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);

    if (i > offset && i < offset + arcsize) {
      // put arc in
      t->setAngle(2);
      t->setLength(30);
    }
    else if (i > (mid + offset) && i < (mid + offset + arcsize)) {
      // other corner
      t->setAngle(2);
      t->setLength(30);
    }
    else {
      t->setAngle(0);
      t->setLength(10);
    }

    // mytrack->PutTrackSection(t);
  }
}

void
CTrackEditorDoc::OnEditRemoveallarg2()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  for (int i = 0; i < mytrack->CCLineSections->size(); i++) {
    CCLineSection *t = (CCLineSection *)mytrack->CCLineSections->elementAt(i);
    t->setArg2(0);
    // mytrack->PutCCSection(t);
  }
  mytrack->regenerate = TRUE;
}

void
CTrackEditorDoc::OnEditRemoveallccarg1()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  for (int i = 0; i < mytrack->CCLineSections->size(); i++) {
    CCLineSection *t = (CCLineSection *)mytrack->CCLineSections->elementAt(i);
    t->setArg1(0);
    // mytrack->PutCCSection(t);
  }
  mytrack->regenerate = TRUE;
}

void
CTrackEditorDoc::OnEditTrackname()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();
  CTrackNameInfo *nameinfo = new CTrackNameInfo();

  nameinfo->setTrack(mytrack);

  nameinfo->DoModal();

  nameinfo->getTrackInfo(mytrack);
}

void
CTrackEditorDoc::OnFileSave()
{
  // TODO: Add your command handler code here
  // OnFileSaveAs();
  GPTrack *mytrack = getTrack();
  mytrack->regenerate = TRUE;

  mytrack->RecreateData();
  mytrack->regenerate = FALSE;

  CString filename = mytrack->getFileName();
  CString circuit = mytrack->getCircuitName();

  if (filename == "unnamed.dat") {
    OnFileSaveAs();
  }
  else {
    char buff[256];

    wsprintf(buff, "Saving track '%s' as %s\n", circuit, filename);
    int result = AfxMessageBox(buff, MB_YESNO);

    if (result == IDYES) mytrack->WriteTrackFile(filename);
  }
}

void
CTrackEditorDoc::OnFileClose()
{
  CDocument::OnFileClose();
}

void
CTrackEditorDoc::OnRegenerateTrack()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  int res = AfxMessageBox(
      "This option is a current in development, it may trash your track "
      "completely",
      MB_YESNO);

  if (res == IDYES) {
    mytrack->RecreateData();
    AfxMessageBox("Track Modified", MB_OK);
  }
}

void
CTrackEditorDoc::OnTablesCclinedata()
{
  // TODO: Add your command handler code here
  theApp.OnCCView(this);
}

void
CTrackEditorDoc::OnTablesPitlanedata()
{
  // TODO: Add your command handler code here
  theApp.OnPitView(this);
}

void
CTrackEditorDoc::OnTablesTrackdata()
{
  // TODO: Add your command handler code here
  theApp.OnTrackView(this);
}

void
CTrackEditorDoc::OnEditRemoveallfeature1()
{
  // TODO: Add your command handler code here
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();
  mytrack->regenerate = TRUE;
  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);
    t->rs = 0;
    // mytrack->PutTrackSection(t);
  }
}

void
CTrackEditorDoc::OnEditRemoveallfeature2()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();
  mytrack->regenerate = TRUE;
  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);
    t->cb = 0;
    // mytrack->PutTrackSection(t);
  }
}

void
CTrackEditorDoc::OnEditCccarsetup()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  if (mytrack) {
    if (mytrack->carsetup) {
      mytrack->carsetup->Open();
    }
  }
}

void
CTrackEditorDoc::OnObjectdata()
{
  // TODO: Add your command handler code here
  theApp.OnObjectView(this);
}

static char lpszCamFilter[] =
    "F1GP2 Camera Files (*.cam)|*.cam|All Files (*.*)|*.*|";

char *
getLine(FILE *fp)
{
  static char buff[256];

  fscanf(fp, "%[^\n]\n", buff);  // read a line

  return buff;
}

char *
extractSection(char *buff)
{
  static char sec[256];

  memset(sec, 0, 256);

  for (int i = 0; i < (int)strlen(buff); i++) {
    if (buff[i + 1] == ']') {
      return sec;
    }
    sec[i] = buff[i + 1];
  }
  return NULL;
}

void
GetInfo(GPTrack *mytrack, FILE *fp)
{
  char *Comment = getLine(fp);
  char *ver = getLine(fp);
  char *titles = getLine(fp);
  char *lines = getLine(fp);

  int X;
  int Y, Z1, Z2, SM, ZF1, ZF2, UNK1, UNK2;
  int number;
  int VR1, VR2;

  while (!feof(fp)) {
    char *CamStr = getLine(fp);
    int pos;

    sscanf(CamStr, "%d%n", &number, &pos);
    CamStr += pos;
    sscanf(CamStr, "%d%n", &X, &pos);
    CamStr += pos;
    sscanf(CamStr, "%d%n", &Y, &pos);
    CamStr += pos;
    sscanf(CamStr, "%d%n", &Z1, &pos);
    CamStr += pos;
    sscanf(CamStr, "%d%n", &Z2, &pos);
    CamStr += pos;
    sscanf(CamStr, "%d%n", &SM, &pos);
    CamStr += pos;
    sscanf(CamStr, "%d%n", &ZF1, &pos);
    CamStr += pos;
    sscanf(CamStr, "%d%n", &ZF2, &pos);
    CamStr += pos;
    sscanf(CamStr, "%d%n", &VR1, &pos);
    CamStr += pos;
    sscanf(CamStr, "%d%n", &VR2, &pos);
    CamStr += pos;
    sscanf(CamStr, "%d%n", &UNK1, &pos);
    CamStr += pos;
    sscanf(CamStr, "%d%n", &UNK2, &pos);
    CamStr += pos;

    Camera *cam = new Camera(mytrack, number, X, Y, Z1, Z2, SM, ZF1, ZF2, VR1,
                             VR2, UNK1, UNK2);

    mytrack->TrackCameras->addElement(cam);
  }
}

void
importCam(GPTrack *mytrack, CString filename)
{
  char *buff;

  FILE *fp = fopen(filename, "r");

  mytrack->TrackCameras->removeAllData();

  while (!feof(fp)) {
    buff = getLine(fp);
    if (buff[0] == '[') {
      char *sec = extractSection(buff);
      TRACE("Found Section (%s)\n", sec);
      if (!strcmp(sec, "cameras")) {
        GetInfo(mytrack, fp);
      }
    }
  }
  fclose(fp);
}

void
exportCam(GPTrack *mytrack, CString filename)
{
  FILE *fp = fopen(filename, "w");

  fprintf(fp, "[comments]\n");
  fprintf(fp, "Exported from (Track Editor by Paul Hoad)\n\n");

  fprintf(fp, "[cameras]\n");
  fprintf(fp, "Track Cameras for %s %s\n", (LPCSTR)mytrack->circuitName,
          (LPCSTR)mytrack->countryName);
  fprintf(fp, "v1.3\n");
  fprintf(fp,
          "#    X      Y      Z1     Z2    SM     ZF1    ZF2    VR1    VR2    "
          "UK1    UK2\n");
  fprintf(fp,
          "===================================================================="
          "===================\n");

  for (int i = 0; i < mytrack->TrackCameras->size(); i++) {
    Camera *t = (Camera *)mytrack->TrackCameras->elementAt(i);
    if (t) {
      t->exportFile(fp, i);
    }
  }
  fclose(fp);
}

void
CTrackEditorDoc::OnImport()
{
  // TODO: Add your control notification handler code here
  // AfxMessageBox("Importing Cameras Currently Removed",MB_OK);

  char fileName[256];
  GPTrack *mytrack = getTrack();

  strcpy(fileName, "unnamed.cam");
  CFileDialog *fdlg =
      new CFileDialog(TRUE, "cam", (LPCSTR)&fileName,
                      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszCamFilter);

  int result = fdlg->DoModal();

  if (result != IDCANCEL) {
    char message[256];
    CString filename = fdlg->GetFileName();
    wsprintf(message,
             "Importing Camera Settings from file %s (Currently not supported)",
             filename);

    result = AfxMessageBox(message, MB_YESNO);

    importCam(mytrack, filename);
  }
  delete fdlg;

  // ResetTree();
  // ReloadTree();
}

void
CTrackEditorDoc::OnexportFile()
{
  // TODO: Add your command handler code here

  GPTrack *mytrack = getTrack();

  char fileName[256];

  strcpy(fileName, "unnamed.cam");
  CFileDialog *fdlg =
      new CFileDialog(FALSE, "cam", (LPCSTR)&fileName,
                      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszCamFilter);

  int result = fdlg->DoModal();

  if (result != IDCANCEL) {
    char message[256];
    CString filename = fdlg->GetFileName();
    wsprintf(message, "Exporting Camera Settings to file %s", filename);

    result = AfxMessageBox(message, MB_YESNO);

    exportCam(mytrack, filename);
  }
  delete fdlg;
}

void
CTrackEditorDoc::OnFileDebugoutput()
{
  // TODO: Add your command handler code here
  FILE *fp = fopen("debug.txt", "w");

  GPTrack *mytrack = getTrack();

  mytrack->DebugTrack(fp, 0);

  fclose(fp);
}

void
CTrackEditorDoc::OnUpdateexportFile(CCmdUI *pCmdUI)
{
  // TODO: Add your command update UI handler code here
  // pCmdUI->SetEnabled(FALSE);
}

void
CTrackEditorDoc::OnUpdateImport(CCmdUI *pCmdUI)
{
  // TODO: Add your command update UI handler code here
  // pCmdUI->Enabled(FALSE);
}

void
CTrackEditorDoc::OnCreateCcline()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  mytrack->CCLineSections->removeAllData();
  mytrack->regenerate = TRUE;
  int currentLength = 0;

  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);
    int len = t->getLength();
    double rad = t->getAngle();
    double sectorRadius = 0;
    double radiusAbs = 0;
    int sign = 0;

    if (rad != 0) {
      double anglelen = (rad * len);
      double anglerad = DegToRad(anglelen);
      double inrad = DegToRad(anglelen / 2.0);
      sectorRadius = (len / anglerad);
      sectorRadius = sectorRadius * 1024;
      radiusAbs = fabs(sectorRadius);
    }

    RadiusFrom(sectorRadius, sign);
    currentLength += len;

    if (currentLength >= mytrack->CCLineStartX.GetValue()) {
      CCLineSection *nCCsec =
          new CCLineSection(mytrack, len, sectorRadius, 0, sign, 0, 0, 0x50, 0);

      mytrack->CCLineSections->addElement(nCCsec);
      mytrack->CCLineStartY.SetValue(0);
      mytrack->CCLineStartYData.SetValue(0);
    }
  }

  mytrack->createCC();
  UpdateAllViews(NULL);
}

void
CTrackEditorDoc::OnRandomObject()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  srand((unsigned)time(NULL));

  mytrack->regenerate = TRUE;

  CRandomObject *objRand = new CRandomObject();

  int result = objRand->DoModal();

  if (result != IDOK) return;

  int num = objRand->m_ObjectsPerSection;

  int size = mytrack->TrackObjectDefintions->size();

  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);
    Vector *SectionCmds = t->getCommands();
    double length = (double)t->getLength();

    for (int j = 0; j < num; j++) {
      double ran = ((double)rand() / (double)RAND_MAX);
      int pos = (int)(ran * size);
      int xpos = (int)(ran * length);

      TRACE("pos=%d xpos=%d\n", pos, xpos);
      TrackObjectDefinition *obj =
          (TrackObjectDefinition *)mytrack->TrackObjectDefintions->elementAt(
              pos);

      TrackObject *trackcmd =
          new TrackObject(mytrack, t, xpos, obj->getLocator(), 0);
      trackcmd->AddWordArgument(10, "Inserted Object Distance");
      trackcmd->AddWordArgument(obj->getLocator(), "Inserted Object Locator");
      SectionCmds->addElement(trackcmd);
    }
  }
  UpdateAllViews(NULL);
}

BOOL
CheckDoYouWantTo()
{
  CString msg;

  msg.Format("Are you sure you want to do this!");

  int result = AfxMessageBox(msg, MB_YESNO);

  if (result == IDYES) return TRUE;
  return FALSE;
}

void
CTrackEditorDoc::OnRemoveTrackcmds()
{
  if (!CheckDoYouWantTo()) return;
  GPTrack *mytrack = getTrack();

  mytrack->RemoveAllCmds(-1);
}

void
CTrackEditorDoc::OnRemovePitobjs()
{
  if (!CheckDoYouWantTo()) return;
  GPTrack *mytrack = getTrack();

  mytrack->RemoveAllPitObjects();
}

void
CTrackEditorDoc::OnRemovePitcmds()
{
  if (!CheckDoYouWantTo()) return;
  GPTrack *mytrack = getTrack();

  mytrack->RemoveAllPitCmds();
}

void
CTrackEditorDoc::OnRemoveTrackobjs()
{
  if (!CheckDoYouWantTo()) return;
  GPTrack *mytrack = getTrack();

  mytrack->RemoveAllObjects();
}

void
CTrackEditorDoc::OnWorldview()
{
  // TODO: Add your command handler code here
  theApp.OnWorldView(this);
}

void
CTrackEditorDoc::OnWalls()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  mytrack->showWalls = !mytrack->showWalls;

  UpdateAllViews(NULL);
}

#define UPDATE_TRACK(x)          \
  GPTrack *mytrack = getTrack(); \
  pCmdUI->SetCheck(mytrack->x);

void
CTrackEditorDoc::OnUpdateWalls(CCmdUI *pCmdUI)
{
  // TODO: Add your command update UI handler code here
  UPDATE_TRACK(showWalls);
}

void
CTrackEditorDoc::OnUpdateTrack(CCmdUI *pCmdUI)
{
  // TODO: Add your command update UI handler code here
  UPDATE_TRACK(showTrack);
}

void
CTrackEditorDoc::OnUpdatePitlane(CCmdUI *pCmdUI)
{
  // TODO: Add your command update UI handler code here
  UPDATE_TRACK(showPitLane);
}

void
CTrackEditorDoc::OnUpdateObjects(CCmdUI *pCmdUI)
{
  // TODO: Add your command update UI handler code here
  UPDATE_TRACK(showObjects);
}

void
CTrackEditorDoc::OnFillpolygon()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  mytrack->showFilledObjects = !mytrack->showFilledObjects;

  UpdateAllViews(NULL);
}

void
CTrackEditorDoc::OnUpdateFillpolygon(CCmdUI *pCmdUI)
{
  // TODO: Add your command update UI handler code here
  UPDATE_TRACK(showFilledObjects);
}

void
CTrackEditorDoc::OnShowTracksides()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  mytrack->showTrackSide = !mytrack->showTrackSide;

  UpdateAllViews(NULL);
}

void
CTrackEditorDoc::OnUpdateShowTracksides(CCmdUI *pCmdUI)
{
  // TODO: Add your command update UI handler code here
  UPDATE_TRACK(showTrackSide);
}

void
CTrackEditorDoc::OnTablesTrackview()
{
  // TODO: Add your command handler code here
  theApp.OnTrackScrollView(this);
}

void
CTrackEditorDoc::OnCloseDocument()
{
  CDocument::OnCloseDocument();
}

void
CTrackEditorDoc::OnViewCommands()
{
  // TODO: Add your command handler code here
  theApp.OnCommandStatsView(this);
}

void
CTrackEditorDoc::OnRemoveXxTrackcmds()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  CCmdRemoval *cmds = new CCmdRemoval();
  int result = cmds->DoModal();
  if (result == IDOK) {
    CString cmd = cmds->m_CmdVal;
    int cmdval;
    if (cmd.Find("0x") != -1) {
      // command specified in hex
      sscanf(cmd, "%x", &cmdval);
    }
    else {
      // command is decimal
      sscanf(cmd, "%d", &cmdval);
    }
    CString msg;

    msg.Format(
        "About to Remove all commands %d 0x%x\nDo you want me to continue",
        cmdval, cmdval);

    int result = AfxMessageBox(msg, MB_YESNO);
    if (result == IDYES) {
      mytrack->RemoveAllCmds(cmdval);
      mytrack->regenerate = TRUE;
    }
  }
  delete cmds;
}

CTreeCtrl *
CTrackEditorDoc::getTree()
{
  return theApp.getMainTree()->getTree();
}

int sceneryCodes[] = {0xB8, 0xB9, 0xAF};
int numberSceneryCodes = 3;

void
CTrackEditorDoc::OnRemoveScenery()
{
  if (!CheckDoYouWantTo()) return;
  GPTrack *mytrack = getTrack();

  for (int i = 0; i < numberSceneryCodes; i++) {
    int cmdval = sceneryCodes[i];
    mytrack->RemoveAllCmds(cmdval);
  }
}

void
CTrackEditorDoc::OnZeroScenery()
{
  if (!CheckDoYouWantTo()) return;
  GPTrack *mytrack = getTrack();

  for (int i = 0; i < numberSceneryCodes; i++) {
    int cmdval = sceneryCodes[i];
    mytrack->ZeroAllArgs(cmdval);
  }
}

void
CTrackEditorDoc::OnRemoveSand()
{
  if (!CheckDoYouWantTo()) return;
  GPTrack *mytrack = getTrack();

  mytrack->RemoveAllCmds(0xBC);
}

void
CTrackEditorDoc::OnMovePitentrance()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  CPitEntrance *dlg = new CPitEntrance();

  dlg->EnterToLeft = mytrack->PitLaneEntranceToLeft();
  dlg->LeaveToLeft = mytrack->PitLaneExitToLeft();

  int lastEntrance = mytrack->PitLaneEntryIndex;
  int lastExit = mytrack->PitLaneExitIndex;

  dlg->m_Entrance = mytrack->PitLaneEntryIndex;
  dlg->m_Exit = mytrack->PitLaneExitIndex;

  int result = dlg->DoModal();

  if (result == IDOK) {
    if (dlg->m_Entrance < 0 ||
        dlg->m_Entrance > (mytrack->TrackSections->size() - 1)) {
      AfxMessageBox("Not a valid Track Section for entrance");
      return;
    }

    if (dlg->m_Exit < 0 || dlg->m_Exit > (mytrack->TrackSections->size() - 1)) {
      AfxMessageBox("Not a valid Track Section for exit");
      return;
    }
    mytrack->PitLaneEntryIndex = dlg->m_Entrance;
    mytrack->PitLaneExitIndex = dlg->m_Exit;

    mytrack->createTrack(this);
    mytrack->createPitlane();

    if (lastEntrance != mytrack->PitLaneEntryIndex) {
      TrackSection *Start =
          (TrackSection *)mytrack->TrackSections->elementAt(lastEntrance);
      TrackSection *newStart =
          (TrackSection *)mytrack->TrackSections->elementAt(
              mytrack->PitLaneEntryIndex);

      Start->RemoveCommand(0x86);
      newStart->InsertCommand(mytrack, 0x86);
      AfxMessageBox("Moved Pit Entrance");
    }

    if (lastExit != mytrack->PitLaneExitIndex) {
      TrackSection *Start =
          (TrackSection *)mytrack->TrackSections->elementAt(lastExit);
      TrackSection *newStart =
          (TrackSection *)mytrack->TrackSections->elementAt(
              mytrack->PitLaneExitIndex);

      Start->RemoveCommand(0x87);
      newStart->InsertCommand(mytrack, 0x87);
      AfxMessageBox("Moved Pit Exit");
    }
  }

  delete dlg;
}

void
CTrackEditorDoc::OnMovePitexit()
{
  // TODO: Add your command handler code here
}

void
CTrackEditorDoc::OnViewPitlaneInfo()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  CPitEntrance *dlg = new CPitEntrance();

  dlg->EnterToLeft = mytrack->PitLaneEntranceToLeft();
  dlg->LeaveToLeft = mytrack->PitLaneExitToLeft();

  int lastEntrance = mytrack->PitLaneEntryIndex;
  int lastExit = mytrack->PitLaneExitIndex;

  dlg->m_Entrance = mytrack->PitLaneEntryIndex;
  dlg->m_Exit = mytrack->PitLaneExitIndex;

  int result = dlg->DoModal();

  delete dlg;
}

static int lastSearch = 0;
static int lastCommand = -1;

void
CTrackEditorDoc::OnFindCommand()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  CFindCmd *dlg = new CFindCmd();

  int result = dlg->DoModal();

  if (result == IDOK) {
    lastCommand = dlg->getCmd();
    TrackSection *t = mytrack->FindTrackCommand(lastCommand, lastSearch);

    if (t) {
      getTree()->Expand(getTree()->GetParentItem(t->getTreeNode()), TVE_EXPAND);
      getTree()->SelectItem(t->getTreeNode());
      getTree()->Expand(t->getTreeNode(), TVE_EXPAND);
    }
    else {
      int result = AfxMessageBox("Start From the Beginning", MB_YESNO);
      lastSearch = 0;
      if (result == IDYES) {
        lastSearch = 0;
        TrackSection *t = mytrack->FindTrackCommand(lastCommand, lastSearch);
        if (t) {
          getTree()->Expand(getTree()->GetParentItem(t->getTreeNode()),
                            TVE_EXPAND);
          getTree()->SelectItem(t->getTreeNode());
          getTree()->Expand(t->getTreeNode(), TVE_EXPAND);
        }
        else {
          lastSearch = 0;
          AfxMessageBox("Failed to Find Track Command..");
          lastCommand = -1;
        }
      }
    }
  }

  delete dlg;
}

void
CTrackEditorDoc::OnFindAgain()
{
  GPTrack *mytrack = getTrack();
  TrackSection *t = mytrack->FindTrackCommand(lastCommand, lastSearch);

  if (t) {
    getTree()->Expand(getTree()->GetParentItem(t->getTreeNode()), TVE_EXPAND);
    getTree()->SelectItem(t->getTreeNode());
    getTree()->Expand(t->getTreeNode(), TVE_EXPAND);
  }
  else {
    int result = AfxMessageBox("Start From the Beginning", MB_YESNO);
    lastSearch = 0;
    if (result == IDYES) {
      lastSearch = 0;
      TrackSection *t = mytrack->FindTrackCommand(lastCommand, lastSearch);
      if (t) {
        getTree()->Expand(getTree()->GetParentItem(t->getTreeNode()),
                          TVE_EXPAND);
        getTree()->SelectItem(t->getTreeNode());
        getTree()->Expand(t->getTreeNode(), TVE_EXPAND);
      }
      else {
        lastSearch = 0;
        AfxMessageBox("Failed to Find Track Command..");
        lastCommand = -1;
      }
    }
  }
}

void
CTrackEditorDoc::OnUpdateFindAgain(CCmdUI *pCmdUI)
{
  // TODO: Add your command update UI handler code here
  pCmdUI->Enable(lastCommand != -1);
}

void
CTrackEditorDoc::OnRefreshTree()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();
  mytrack->createTrack(this);
  theApp.getMainTree()->UpdateTree(mytrack, NULL);
}

void
CTrackEditorDoc::OnCameraView()
{
  // TODO: Add your command handler code here
  theApp.OnCameraView(this);
}

void
CTrackEditorDoc::OnAddRoadsigns()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);
    TrackSection *prev = t->previous();

    if ((prev->getAngle() == 0) && (t->getAngle() != 0)) {
      if (t->getAngle() < 0) {
        t->setLeftRoadSign(0x80);
      }
      else {
        t->setRightRoadSign(0x40);
      }
    }
  }
  UpdateAllViews(NULL);
}

void
CTrackEditorDoc::OnRemovePitlaneHeight()
{
  if (!CheckDoYouWantTo()) return;
  GPTrack *mytrack = getTrack();
  mytrack->regenerate = TRUE;

  for (int i = 0; i < mytrack->PitlaneSections->size(); i++) {
    TrackSection *t = (TrackSection *)mytrack->PitlaneSections->elementAt(i);
    t->setHeight(0);
  }
}

void
CTrackEditorDoc::OnShowJam()
{
  // TODO: Add your command handler code here
  HTREEITEM selected = getTree()->GetSelectedItem();

  if (selected != NULL) {
    Observer *obj = (Observer *)getTree()->GetItemData(selected);

    if (obj && obj->isJamFile()) {
      GPTrack *track = obj->getTrack();

      JamFileEditor *JAFE = (JamFileEditor *)obj;

      setCurrentJam(JAFE->JamFileStructure);

      theApp.OnJamView(this);
    }
    else {
      AfxMessageBox("No Jam File Selected");
    }
  }
  else {
    AfxMessageBox("No Selection");
  }
}

void
CTrackEditorDoc::OnEditDrivingLine()
{
  // TODO: Add your command handler code here
  CDirivingLine *dlg = new CDirivingLine();
  GPTrack *mytrack = getTrack();

  dlg->beginYPos = mytrack->CCLineStartY.GetValue();
  dlg->beginXPos = mytrack->CCLineStartX.GetValue();
  dlg->PoleValue = mytrack->TrackUnknown1.GetValue();

  int result = dlg->DoModal();

  if (result == IDOK) {
    mytrack->CCLineStartY.SetValue(dlg->beginYPos);
    mytrack->CCLineStartX.SetValue(dlg->beginXPos);
    mytrack->TrackUnknown1.SetValue(dlg->PoleValue);
    mytrack->createCC();
    UpdateAllViews(NULL);
  }
}

void
CTrackEditorDoc::OnExportTrackCmds()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  char fileName[256];

  strcpy(fileName, "cmds.txt");
  CFileDialog *fdlg =
      new CFileDialog(FALSE, "txt", (LPCSTR)&fileName,
                      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL);

  int result = fdlg->DoModal();

  if (result != IDCANCEL) {
    // char message[256];
    CString filename = fdlg->GetFileName();
    // wsprintf(message,"Exporting Track Info to file %s",filename);

    // result = AfxMessageBox(message,MB_YESNO);

    mytrack->ExportTrackCommands(filename);
  }
  delete fdlg;
}

static char lpszObjFilter[] =
    "F1GP2 Object Files (*.gpo)|*.gpo|All Files (*.*)|*.*|";

void
CTrackEditorDoc::OnInsertInternalObject()
{
  GPTrack *mytrack = getTrack();
  // TODO: Add your command handler code here
  char fileName[256];

  strcpy(fileName, "unnamed.gpo");
  CFileDialog *fdlg =
      new CFileDialog(TRUE, "gpo", (LPCSTR)&fileName,
                      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszObjFilter);

  int result = fdlg->DoModal();

  if (result == IDOK) {
    mytrack->ImportNewInternalObject(this, fdlg->GetFileName());
  }

  delete fdlg;
}

void
CTrackEditorDoc::OnExportObjectInfo()
{
  // TODO: Add your command handler code here
  HTREEITEM selected = getTree()->GetSelectedItem();

  if (selected != NULL) {
    Observer *obj = (Observer *)getTree()->GetItemData(selected);

    if (obj && obj->isInternalObject()) {
      GPTrack *track = obj->getTrack();

      InternalObject *InObj = (InternalObject *)obj;
      InObj->exportFile();
    }
    else {
      AfxMessageBox("No Internal Object  Selected");
    }
  }
  else {
    AfxMessageBox("No Selection");
  }
}

void
CTrackEditorDoc::OnView3dobject()
{
  CTreeCtrl *tree = getTree();

  if (tree != NULL) {
    HTREEITEM selected = tree->GetSelectedItem();

    if (selected != NULL) {
      Observer *obj = (Observer *)getTree()->GetItemData(selected);

      if (obj && obj->isInternalObject()) {
        GPTrack *track = obj->getTrack();

        InternalObject *InObj = (InternalObject *)obj;
        // View internal object
        setCurrentObject(InObj);
        InObj->Expand(theApp.getMainTree(), InObj->getTreeNode());

        theApp.OnInternalView(this);
      }
      else {
        AfxMessageBox("No Internal Object  Selected");
      }
    }
    else {
      AfxMessageBox("No Selection");
    }
  }
  else {
    AfxMessageBox("No Tree");
  }
}

void
CTrackEditorDoc::OnEditScaletrack()
{
  // TODO: Add your command handler code here
  CScaleData *dlg = new CScaleData();

  dlg->m_ScaleVal = (float)1.0;

  int result = dlg->DoModal();

  if (result != IDCANCEL) {
    float scaleval = dlg->m_ScaleVal;
    GPTrack *mytrack = getTrack();
    mytrack->ScaleTrack(scaleval);
  }

  delete dlg;
}

void
CTrackEditorDoc::OnCcScalar()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  CCScale *scalevalue = new CCScale();

  TRACE("%f \n", mytrack->CCScaleFactor);

  scalevalue->m_CCScale = (float)(mytrack->CCScaleFactor);

  TRACE("%f \n", scalevalue->m_CCScale);

  int result = scalevalue->DoModal();

  if (result == IDOK) {
    mytrack->CCScaleFactor = (double)scalevalue->m_CCScale;
    mytrack->createCC();
    UpdateAllViews(NULL);
  }
}

void
CTrackEditorDoc::OnExportNamelist()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  mytrack->ExportCleverNames();
}

void
CTrackEditorDoc::OnCreateCcLinetwo()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  mytrack->CCLineSections->removeAllData();
  mytrack->regenerate = TRUE;
  int currentLength = 0;
  int lastX = 0;

  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);
    int len = t->getLength();
    double rad = t->getAngle();
    double sectorRadius = 0;
    double radiusAbs = 0;
    int sign = 0;

    if (rad != 0) {
      double anglelen = (rad * len);
      double anglerad = DegToRad(anglelen);
      double inrad = DegToRad(anglelen / 2.0);
      sectorRadius = (len / anglerad);
      sectorRadius = sectorRadius * 1024;
      radiusAbs = fabs(sectorRadius);
    }

    RadiusFrom(sectorRadius, sign);
    currentLength += len;

    if (currentLength >= mytrack->CCLineStartX.GetValue()) {
      if (rad != 0 && len > 10) {
        int num = len / 10;
        int rem = len % 10;
        for (int i = 0; i < num; i++) {
          CCLineSection *nCCsec = new CCLineSection(mytrack, 10, sectorRadius,
                                                    0, sign, 0, 0, 0x50, 0);
          mytrack->CCLineSections->addElement(nCCsec);
        }
        if (rem != 0) {
          CCLineSection *nCCsec = new CCLineSection(mytrack, rem, sectorRadius,
                                                    0, sign, 0, 0, 0x50, 0);
          mytrack->CCLineSections->addElement(nCCsec);
        }
      }
      else {
        CCLineSection *nCCsec = new CCLineSection(mytrack, len, sectorRadius, 0,
                                                  sign, 0, 0, 0x50, 0);
        mytrack->CCLineSections->addElement(nCCsec);
      }
    }
    else {
      lastX = currentLength;
    }
  }

  mytrack->CCLineStartX.SetValue(lastX);
  mytrack->CCLineStartY.SetValue(0);
  mytrack->CCLineStartYData.SetValue(0);

  mytrack->createCC();
  UpdateAllViews(NULL);
}

void
CTrackEditorDoc::OnEditCopy()
{
  if (theApp.getMainTree()) theApp.getMainTree()->OnCopy();
}

void
CTrackEditorDoc::OnEditCut()
{
  if (theApp.getMainTree()) theApp.getMainTree()->OnCut();
}

void
CTrackEditorDoc::OnEditPaste()
{
  if (theApp.getMainTree()) theApp.getMainTree()->OnPaste();
}

void
CTrackEditorDoc::OnTableScenery()
{
  CSceneryEditor *dlg = new CSceneryEditor(getTrack());

  dlg->DoModal();

  if (dlg) delete dlg;
}

void
CTrackEditorDoc::OnStandardscenery()
{
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  for (int i = 0; i < numberSceneryCodes; i++) {
    int cmdval = sceneryCodes[i];
    mytrack->DefaultAllArgs(cmdval);
  }
}

void
CTrackEditorDoc::OnNewObjectDefinition()
{
  GPTrack *track = getTrack();

  if (track) {
    TrackObjectDefinition *t = track->AddNewTrackObjectDefinition();

    CObjectEdit *trkdlg = new CObjectEdit(track);

    if (trkdlg && t) trkdlg->setSection(t);

    int res = trkdlg->DoModal();

    if (trkdlg && t && res != IDCANCEL) {
      trkdlg->getSection(t);
      track->regenerate = TRUE;
    }

    if (trkdlg) delete trkdlg;
  }
}

void
CTrackEditorDoc::SetTitle(LPCTSTR lpszTitle)
{
  CDocument::SetTitle(lpszTitle);
}

char lpszTrifFilter[] =
    "Track Interchange File Format (*.trif)|*.trif|All Files (*.*)|*.*|";

void
ParseTrifFile(FILE *fp, GPTrack *track);
void
ImportTrifFile(GPTrack *track, CString filename);

void
CTrackEditorDoc::OnImportTrif()
{
  GPTrack *mytrack = getTrack();

  char fileName[256];

  strcpy(fileName, "*.trif");

  CFileDialog *fdlg = new CFileDialog(TRUE, "trif", (LPCSTR)&fileName,
                                      OFN_HIDEREADONLY, lpszTrifFilter);

  int result = fdlg->DoModal();
  if (result == IDOK) {
    ImportTrifFile(mytrack, fdlg->GetFileName());
    mytrack->regenerate = TRUE;
    mytrack->createTrack(this);
    mytrack->createPitlane();
  }
}

void
ImportTrifFile(GPTrack *track, CString filename)
{
  FILE *fp;

  fp = fopen(filename, "r");

  if (fp == NULL) {
    AfxMessageBox("Cannot File TRIF file");
    return;
  }

  ParseTrifFile(fp, track);

  fclose(fp);
}

/*
static char * getLine(FILE *fp)
{
        static char line[256];
        fscanf(fp,"%[^\n]\n",line);
        return line;
}
*/

void
ParseTrifFile(FILE *fp, GPTrack *track)
{
  track->RemoveAllCmds(-1, 1);

  while (!feof(fp)) {
    char *line = getLine(fp);

    if (line[0] == '#') continue;
    if (line[0] == ';' && line[1] == ';') {
      char sectorName[256];
      // found sector
      line += 2;
      sscanf(line, "%[^\n]", sectorName);
      AfxMessageBox(sectorName);
    }

    if (line[0] == 't') {
      int index;
      int cmd;
      // track section or command
      sscanf(line, "t%d %x", &index, &cmd);

      if (index >= 0 && index < track->TrackSections->size()) {
        TrackSection *t =
            (TrackSection *)track->TrackSections->elementAt(index);
        if (cmd == 0) {
          int length, ang, climb, rs, cb, wr, wl;
          sscanf(line, "t%*d %*x %d %d %d %d %d %d %d", &length, &ang, &climb,
                 &rs, &cb, &wr, &wl);
          if (t) {
            t->setLength(length);
            t->setAngle(AngleToDeg(ang));
            t->setHeight(climb);
            t->setRoadSignData(rs);
            t->setKerbData(cb);
            t->setRightToBank(wr);
            t->setLeftToBank(wl);
          }
          else {
            AfxMessageBox("Cannot find section to modify");
          }
        }
        else if (cmd >= 0x80) {
          int nargs;
          int npos;
          sscanf(line, "t%*d %*x %d%n", &nargs, &npos);
          nargs += 1;
          TrackCmd *trackcmd =
              new TrackCmd(track, t, cmd, 0, "Unknown Command");
          Vector *SectionCmds = t->getCommands();
          line += npos;
          for (int i = 0; i < nargs; i++) {
            int arg;
            sscanf(line, "%d%n", &arg, &npos);
            trackcmd->AddWordArgument(arg, "Imported Command");
            line += npos;
          }
          SectionCmds->addElement(trackcmd);
        }
        else {
          AfxMessageBox("Unknown Command");
        }
      }
    }

    if (line[0] == 'p') {
      // pitsection or command
      int index;
      int cmd;
      // track section or command
      sscanf(line, "p%d %x", &index, &cmd);

      if (index >= 0 && index < track->PitlaneSections->size()) {
        TrackSection *t =
            (TrackSection *)track->PitlaneSections->elementAt(index);
        if (cmd == 0) {
          int length, ang, climb, rs, cb, wr, wl;
          sscanf(line, "p%*d %*x %d %d %d %d %d %d %d", &length, &ang, &climb,
                 &rs, &cb, &wr, &wl);
          if (t) {
            t->setLength(length);
            t->setAngle(AngleToDeg(ang));
            t->setHeight(climb);
            t->setRoadSignData(rs);
            t->setKerbData(cb);
            t->setRightToBank(wr);
            t->setLeftToBank(wl);
          }
          else {
            AfxMessageBox("Cannot find section to modify");
          }
        }
        else {
          int nargs;
          int npos;
          sscanf(line, "p%*d %*x %d%n", &nargs, &npos);
          nargs += 1;
          TrackCmd *trackcmd =
              new TrackCmd(track, t, cmd, 0, "Unknown Command");
          Vector *SectionCmds = t->getCommands();
          line += npos;
          for (int i = 0; i < nargs; i++) {
            int arg;
            sscanf(line, "%d%n", &arg, &npos);
            trackcmd->AddWordArgument(arg, "Imported Command");
            line += npos;
          }
          SectionCmds->addElement(trackcmd);
        }
      }
    }
  }
}

void
CTrackEditorDoc::OnRemoveTrackcmdsNonzero()
{
  GPTrack *mytrack = getTrack();
  int result = AfxMessageBox(
      "Are you sure you want to remove all Track Commands", MB_YESNO);

  if (result == IDYES) {
    mytrack->RemoveAllCmds(-1, 1);
  }
}

void
CTrackEditorDoc::OnTrackRemoveAll()
{
  GPTrack *mytrack = getTrack();

  int result = AfxMessageBox(
      "Are you sure you want to remove all TrackSections", MB_YESNO);

  if (result == IDYES) {
    if (mytrack) {
      int size = mytrack->TrackSections->size();
      for (int i = 1; i < size; i++) {
        TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(1);
        mytrack->TrackSections->removeElement(t);
      }

      mytrack->createTrack(NULL);
    }
  }
}

void
CTrackEditorDoc::OnJamIndex()
{
  CJamList *jamlist = new CJamList(getTrack());

  jamlist->DoModal();
}

void
CTrackEditorDoc::OnRemoveTrackAnnotations()
{
  if (!CheckDoYouWantTo()) return;
  GPTrack *track = getTrack();

  for (int i = 0; i < track->TrackSections->size(); i++) {
    TrackSection *t = (TrackSection *)track->TrackSections->elementAt(i);
    t->setAnnotation('\0');
  }

  for (int i = 0; i < track->PitlaneSections->size(); i++) {
    TrackSection *t = (TrackSection *)track->PitlaneSections->elementAt(i);
    t->setAnnotation('\0');
  }
}

void
CTrackEditorDoc::OnDefaultTyreWallTexture()
{
  GPTrack *track = getTrack();
  CTextureChooser *tc = new CTextureChooser(track);

  tc->setInfo(0, 0, 0, track->DefaultTyreWallBrush, 0, 0, 0, 0);
  int result = tc->DoModal();

  if (result != IDCANCEL) {
    track->DefaultTyreWallBrush = tc->getTextureId();
  }
  if (tc) delete tc;
}

void
CTrackEditorDoc::OnDefaultGravelTexture()
{
  GPTrack *track = getTrack();
  CTextureChooser *tc = new CTextureChooser(track);

  tc->setInfo(0, 0, 0, track->DefaultGravelBrush, 0, 0, 0, 0);
  int result = tc->DoModal();

  if (result != IDCANCEL) {
    track->DefaultGravelBrush = tc->getTextureId();
  }
  if (tc) delete tc;
}

void
CTrackEditorDoc::OnDefaultTarmacTexture()
{
  GPTrack *track = getTrack();
  CTextureChooser *tc = new CTextureChooser(track);

  tc->setInfo(0, 0, 0, track->DefaultTarmacBrush, 0, 0, 0, 0);
  int result = tc->DoModal();

  if (result != IDCANCEL) {
    track->DefaultTarmacBrush = tc->getTextureId();
  }
  if (tc) delete tc;
}

void
CTrackEditorDoc::OnTrackglview()
{
  theApp.OnTrackglView(this);
}

static char lpszCCLogFilter[] =
    "CC Data Log Files (*.coo)|*.coo|All Files (*.*)|*.*|";

void
CTrackEditorDoc::OnFileImportccrecord()
{
  char fileName[256];
  GPTrack *track = getTrack();

  strcpy(fileName, "unnamed.txt");
  CFileDialog *fdlg =
      new CFileDialog(TRUE, "coo", (LPCSTR)&fileName,
                      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszCCLogFilter);

  int result = fdlg->DoModal();

  if (result != IDCANCEL) {
    CString filename = fdlg->GetFileName();
    track->ImportCCDataLog(filename);
  }
}

static char lpszTrackSegmentLogFilter[] =
    "Track Segment Files (*.seg)|*.seg|All Files (*.*)|*.*|";

void
CTrackEditorDoc::OnFileImportImporttracksegmentdata()
{
  char fileName[256];
  GPTrack *track = getTrack();

  strcpy(fileName, "unnamed.seg");
  CFileDialog *fdlg = new CFileDialog(TRUE, "seg", (LPCSTR)&fileName,
                                      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                                      lpszTrackSegmentLogFilter);

  int result = fdlg->DoModal();

  if (result != IDCANCEL) {
    CString filename = fdlg->GetFileName();
    track->ImportTrackSegmentsDataLog(filename);
  }
}

static char lpszXYLogFilter[] =
    "XY Data Log Files (*.csv)|*.csv|All Files (*.*)|*.*|";

void
CTrackEditorDoc::OnExportTrackXy()
{
  char fileName[256];
  GPTrack *track = getTrack();

  strcpy(fileName, "unnamed.csv");
  CFileDialog *fdlg =
      new CFileDialog(FALSE, "csv", (LPCSTR)&fileName,
                      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszXYLogFilter);

  int result = fdlg->DoModal();

  if (result != IDCANCEL) {
    CString filename = fdlg->GetFileName();
    track->ExportXYDataLog(filename);
  }
}

void
RadiusFrom(int &radius, int &direction);

void
CTrackEditorDoc::OnCreateCclinePonk()
{
  GPTrack *mytrack = getTrack();

  mytrack->CCLineSections->removeAllData();
  mytrack->regenerate = TRUE;
  int currentLength = 0;
  int lastX = 0;

  double aa = -0.1053;
  double ab = 1.1005;
  double ac = 0.6991;
  double ad = 12.612;
  double ba = -1259.2;
  double bb = -10339;
  double bc = -1878.1;
  double bd = 84853;

  FILE *fp = fopen("ponk.math", "r");
  if (fp == NULL) {
    AfxMessageBox("Cannot Find ponk.math Using defaults");
  }
  else {
    fscanf(fp, "%lf", &aa);
    fscanf(fp, "%lf", &ab);
    fscanf(fp, "%lf", &ac);
    fscanf(fp, "%lf", &ad);
    fscanf(fp, "%lf", &ba);
    fscanf(fp, "%lf", &bb);
    fscanf(fp, "%lf", &bc);
    fscanf(fp, "%lf", &bd);
    if (fp) fclose(fp);
  }

  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);
    int len = t->getLength();
    double rad = t->getAngle();
    double sectorRadius = 0;
    double radiusAbs = 0;
    int sign = 0;

    if (rad != 0) {
      double anglelen = (rad * len);
      double anglerad = DegToRad(anglelen);
      double inrad = DegToRad(anglelen / 2.0);
      sectorRadius = (len / anglerad);
      sectorRadius = sectorRadius * 1024;
      radiusAbs = fabs(sectorRadius);
    }

    // RadiusFrom(sectorRadius,sign);
    currentLength += len;

    if (currentLength >= mytrack->CCLineStartX.GetValue()) {
      double TrackLength = len;
      double TrackAngle = rad;
      double cclen = len;
      int ccradius = (int)rad;

      if (rad != 0) {
        cclen = (((aa * TrackAngle) + ab) * (TrackLength)) +
                ((ac * TrackAngle) + ad);
        ccradius = static_cast<int>((((ba * TrackAngle) + bb) * log(TrackLength)) +
                   ((bc * TrackAngle)) + bd);

        if (rad < 0) ccradius = -ccradius;

        RadiusFrom(ccradius, sign);
      }
      CCLineSection *nCCsec = new CCLineSection(mytrack, (int)cclen, ccradius,
                                                0, sign, 0, 0, 0x50, 0);
      mytrack->CCLineSections->addElement(nCCsec);
    }
    else {
      lastX = currentLength;
    }
  }

  mytrack->CCLineStartX.SetValue(lastX);
  mytrack->CCLineStartY.SetValue(0);
  mytrack->CCLineStartYData.SetValue(0);

  mytrack->createCC();
  UpdateAllViews(NULL);
}

void
CTrackEditorDoc::OnRemoveRemoveallwallheightchanges()
{
  if (!CheckDoYouWantTo()) return;
  GPTrack *mytrack = getTrack();
  mytrack->RemoveAllCmds(0x98);
  mytrack->RemoveAllCmds(0x99);
  mytrack->regenerate = TRUE;
  OnRefreshTree();
}

void
CTrackEditorDoc::OnRemoveRemovealltrackwidthchanges()
{
  if (!CheckDoYouWantTo()) return;
  GPTrack *mytrack = getTrack();
  mytrack->RemoveAllCmds(0xB4);
  mytrack->RemoveAllCmds(0xB5);
  mytrack->RemoveAllCmds(0x85);
  mytrack->regenerate = TRUE;
  OnRefreshTree();
}

void
CTrackEditorDoc::OnRemoveRemoveeveryscenerycommand()
{
  if (!CheckDoYouWantTo()) return;

  // 175, 176, 184, 185, 186, 192, 193, 199, 208, 217, 218
  GPTrack *mytrack = getTrack();
  mytrack->RemoveAllCmds(175);
  mytrack->RemoveAllCmds(176);
  mytrack->RemoveAllCmds(184);
  mytrack->RemoveAllCmds(185);
  mytrack->RemoveAllCmds(186);
  mytrack->RemoveAllCmds(192);
  mytrack->RemoveAllCmds(193);
  mytrack->RemoveAllCmds(199);
  mytrack->RemoveAllCmds(208);
  mytrack->RemoveAllCmds(217);
  mytrack->RemoveAllCmds(218);
  mytrack->regenerate = TRUE;
  OnRefreshTree();
}

void
CTrackEditorDoc::OnRemoveRemoveall()
{
  if (!CheckDoYouWantTo()) return;

  // Wall Bridges
  GPTrack *mytrack = getTrack();
  mytrack->regenerate = TRUE;

  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);
    t->setLeftWallBridged(FALSE);
    t->setRightWallBridged(FALSE);
  }
  OnRefreshTree();
}

#define TEST_IT(a, x)               \
  {                                 \
    CString msg;                    \
    msg = a;                        \
    msg += "\t:";                   \
    msg += ((x) ? "PASS" : "FAIL"); \
    failureCount += ((x) ? 0 : 1);  \
    dlg->Output(msg);               \
  }

BOOL
CTrackEditorDoc::CheckTrackObjectOverlap(CDialog *idlg)
{
  BOOL overlap = FALSE;
  CSanity *dlg = (CSanity *)idlg;
  GPTrack *mytrack = getTrack();
  int len = (int)mytrack->totalLength();
  int pitlen = (int)mytrack->totalPitLength();

  TrackObject **positions = (TrackObject **)malloc(sizeof(TrackObject *) * len);
  for (int i = 0; i < len; i++) {
    positions[i] = NULL;
  }

  int posinto = 0;

  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);

    if (t->containsCommand(0x80)) {
      Vector *objs = t->getObjects();

      for (int j = 0; j < objs->size(); j++) {
        TrackObject *obj = (TrackObject *)objs->elementAt(j);
        int posO = obj->getDistance();
        int index = posinto + posO;
        if (index < len) {
          if (positions[index] != NULL) {
            int idx = mytrack->TrackSections->findElementIndex(
                (positions[index]->owner));
            CString msg;
            msg.Format("An object in t%d [%s] overlays object in t%d [%s]", idx,
                       positions[index]->getObjectName(), i,
                       obj->getObjectName());
            dlg->Output(msg);
            overlap = TRUE;
          }
          else {
            positions[index] = obj;
          }
        }
        else {
          CString msg;
          msg.Format("An object in t%d placed past the end of the track", i);
          dlg->Output(msg);
          overlap = TRUE;
        }
      }
    }
    posinto += t->getLength();
  }
  delete positions;

  positions = (TrackObject **)malloc(sizeof(TrackObject *) * len);
  for (int i = 0; i < len; i++) {
    positions[i] = NULL;
  }

  posinto = 0;

  for (int i = 0; i < mytrack->PitlaneSections->size(); i++) {
    TrackSection *t = (TrackSection *)mytrack->PitlaneSections->elementAt(i);

    if (t->containsCommand(0x80)) {
      Vector *objs = t->getObjects();

      for (int j = 0; j < objs->size(); j++) {
        TrackObject *obj = (TrackObject *)objs->elementAt(j);
        int posO = obj->getDistance();
        int index = posinto + posO;
        if (index < len) {
          if (positions[index] != NULL) {
            int idx = mytrack->PitlaneSections->findElementIndex(
                (positions[index]->owner));
            CString msg;
            msg.Format("An object in p%d [%s] overlays object in p%d [%s]", idx,
                       positions[index]->getObjectName(), i,
                       obj->getObjectName());
            dlg->Output(msg);
            overlap = TRUE;
          }
          else {
            positions[index] = obj;
          }
        }
        else {
          CString msg;
          msg.Format("An object in p%d placed past the end of the pitlane", i);
          dlg->Output(msg);
          overlap = TRUE;
        }
      }
    }
    posinto += t->getLength();
  }
  delete positions;

  return overlap;
}

#define Deg2Rad(x) ((double)(x)*PI / 180.0)

double
ToMeters(double d)
{
  return d * 4.8768;
}

void
CTrackEditorDoc::OnEditPerformsanitycheck()
{
  int failureCount = 0;
  CSanity *dlg = new CSanity();
  GPTrack *mytrack = getTrack();
  int pitEntrance = 0;
  int pitExit = 0;
  int a1 = 0;
  int a2 = 0;
  int a3 = 0;
  int a4 = 0;
  int pit9b = 0;
  int pit9f = 0;
  int pit96 = 0;
  int pit97 = 0;
  int pita0 = 0;

  dlg->Create(IDD_SANITY);
  dlg->ShowWindow(SW_SHOW);

  dlg->Output("");
  dlg->Output("Track Editor Sanity Check version 1.2");
  dlg->Output("=====================================");
  dlg->Output("by Paul Hoad");
  dlg->Output("");

  dlg->Output("Started checking");
  dlg->Output("");

  int pit88_or_89 = 0;

  int minPitLength = 256;
  int minTrkLength = 256;

  int curved = 0;
  int numB8s = 0;
  int trkLen = mytrack->totalLength();
  double distancetoA1 = -1;
  double distancetoA2 = -1;
  double distancetoA3 = -1;
  double distancetoA4 = -1;

  // TODO: Add your command handler code here
  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);
    if (t->containsCommand(0x86)) pitEntrance++;
    if (t->containsCommand(0x87)) pitExit++;
    if (t->containsCommand(0xa1)) {
      a1++;
      distancetoA1 = mytrack->LengthToTrackSector(t);
    }
    if (t->containsCommand(0xa2)) {
      a2++;
      distancetoA2 = mytrack->LengthToTrackSector(t);
    }
    if (t->containsCommand(0xa3)) {
      a3++;
      distancetoA3 = mytrack->LengthToTrackSector(t);
    }
    if (t->containsCommand(0xa4)) {
      a4++;
      distancetoA4 = mytrack->LengthToTrackSector(t);
    }
    minTrkLength = min(minTrkLength, t->getLength());
    int B8size = t->numCommands(0xb8);
    numB8s += B8size;
  }

  bool possibleVenetianEffect = false;
  if (distancetoA1 != -1 && distancetoA2 != -1 && distancetoA3 != -1 &&
      distancetoA4 != -1) {
    if (distancetoA1 < 10 || distancetoA1 > trkLen - 10) {
      possibleVenetianEffect = true;
    }
    if (distancetoA2 < 10 || distancetoA2 > trkLen - 10) {
      possibleVenetianEffect = true;
    }
    if (distancetoA3 < 10 || distancetoA3 > trkLen - 10) {
      possibleVenetianEffect = true;
    }
    if (distancetoA4 < 10 || distancetoA4 > trkLen - 10) {
      possibleVenetianEffect = true;
    }
  }

  for (int i = 0; i < mytrack->PitlaneSections->size(); i++) {
    TrackSection *t = (TrackSection *)mytrack->PitlaneSections->elementAt(i);
    if (t->containsCommand(0x9b)) pit9b++;
    if (t->containsCommand(0x9f)) pit9f++;
    if (t->containsCommand(0x96)) pit96++;
    if (t->containsCommand(0x97)) pit97++;
    if (t->containsCommand(0xa0)) pita0++;
    if (t->containsCommand(0x88)) pit88_or_89++;
    if (t->containsCommand(0x89)) pit88_or_89++;
    minPitLength = min(minPitLength, t->getLength());
    if (t->getAngle() != 0) curved++;

    int B8size = t->numCommands(0xb8);
    numB8s += B8size;
  }

  for (int i = 0; i < mytrack->CCLineSections->size(); i++) {
    CCLineSection *t = (CCLineSection *)mytrack->CCLineSections->elementAt(i);
    if (t->getRadius() != 0) curved++;
  }

  TEST_IT("Pit Entrance Exist Test", (pitEntrance != 0))
  TEST_IT("Only one Pit Entrance Test", (pitEntrance == 1))
  TEST_IT("Pit Exit Exist Test", (pitExit != 0))
  TEST_IT("Only one Pit Exit Test", (pitExit == 1))

  TEST_IT("No Pit->Track Connections a1", (a1 != 0))
  TEST_IT("No Pit->Track Connections a2", (a2 != 0))
  TEST_IT("No Pit->Track Connections a3", (a3 != 0))
  TEST_IT("No Pit->Track Connections a4", (a4 != 0))

  TEST_IT("Pit Command 0x9b Existance", (pit9b != 0))
  TEST_IT("Pit Command 0x9f Existance", (pit9f != 0))
  TEST_IT("Pit Command 0x96 Existance", (pit96 != 0))
  TEST_IT("Pit Command 0x97 Existance", (pit97 != 0))
  TEST_IT("Pit Command 0xA0 Existance", (pita0 != 0))
  TEST_IT("Venetian Effect Existance", !possibleVenetianEffect)

  TEST_IT("No Pit Side Determination 0x88 or 0x89", (pit88_or_89 != 0))

  dlg->Output("");
  CString max_length;
  // was 1450 = 1450 LENGTH_TO_KM
  TEST_IT("Track Too Long", (mytrack->totalLength() < 1450));

  // was 1500
  TEST_IT("Track+Pit Too Long",
          ((mytrack->totalLength() + mytrack->totalPitLength()) <
           (1000.0 * (7.789 / LENGTH_TO_KM))));
  max_length.Format("Max Length (Track + Pit) is currently %d Units",
                    (int)(1000.0 * (7.789 / LENGTH_TO_KM)));
  dlg->Output(max_length);
  TEST_IT("Track Too Short ", (mytrack->totalLength() > 500));
  dlg->Output("");
  TEST_IT("Too Many Jam Files ( < 27)",
          (mytrack->JamFiles->size() <= 27 + NON_JAMS));
  TEST_IT("Too Many Curved CC+Pit Sections (rads < 70)", (curved < 70));
  TEST_IT("Too Many Scenery Commands (num < 128)", (numB8s < 128));

  TEST_IT("Zero length Track Sections", (minTrkLength > 0));
  TEST_IT("Zero length Pit Sections", (minPitLength > 0));

  int numCommands = (mytrack->totalCommands() <= 1048);
  TEST_IT("Total Commands < 1048 approx (spa has 1048)", numCommands);

  dlg->Output("");
  dlg->Output("Performing OverLap Tests...");
  BOOL overlap = CheckTrackObjectOverlap(dlg);
  TEST_IT(
      "Checking for Overlapping Objects (2 or more objects at the same "
      "distance from s/f)",
      !overlap);
  dlg->Output("");
  dlg->Output("Performing Pitlane CheckBox Tests...");
  BOOL pitlanetest = CheckPitlaneBoxes(dlg);
  TEST_IT("Pit Lane Check.....", pitlanetest);
  dlg->Output("");
  dlg->Output("Track Contained in World");
  BOOL worldtest = CheckTrackInWorld(dlg);
  TEST_IT("World Check.....", worldtest);
  dlg->Output("");

  int size = mytrack->JamFiles->size();
  int count = 0;
  int totalSize = 0;
  int internalTotal = 0;
  CString msgTotal;
  dlg->Output("===========================================");
  dlg->Output("=  Jam Tests                              =");
  dlg->Output("===========================================");

  bool found32Id = false;

  for (int i = 0; i < size; i++) {
    CString msg;
    JamFileEditor *t = (JamFileEditor *)mytrack->JamFiles->elementAt(i);
    char *name = t->getJamFileName();
    JAM *jams = t->getJAM();
    if (jams) {
      int jsize = jams->OpenToRead(false);
      int subimages = jams->getNumberOfImages();

      for (int j = 0; j < subimages; j++) {
        int idx = jams->getImageId(j);
        if (idx == 32) found32Id;
      }
      if (!t->Internal) {
        msg.Format("External JAM=%s   size=%d", name, jsize);
        totalSize += jsize;
      }
      else {
        msg.Format("Internal JAM=%s   size=%d", name, jsize);
        internalTotal += jsize;
      }

      dlg->Output(msg);
    }
  }
  msgTotal.Format("External Total size=%d\n", totalSize);
  dlg->Output(msgTotal);
  msgTotal.Format("Internal Total size=%d\n", internalTotal);
  dlg->Output(msgTotal);
  msgTotal.Format("Total size=%d\n", internalTotal + totalSize);
  dlg->Output(msgTotal);
  TEST_IT("Black Horizon Test.....", found32Id);
  dlg->Output("===========================================");
  dlg->Output("");
  dlg->Output("===========================================");
  dlg->Output(" Iso Height Table");
  dlg->Output("===========================================");

  double GradientAtBeginning = mytrack->TrackStartClimb.GetValue();
  GradientAtBeginning = (45.0 * GradientAtBeginning) / 8192.0;
  double height = 0;
  double ad = 0;
  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);
    double dheight = t->getHeight() - height;
    double length = t->getLength();
    height = t->getHeight();

    // you say height delta but which delta between segment or the true height
    // value!
    if (height != 0) {
      double m = (45.0 * height * length) / 8192.0;
      double den = m / sin(Deg2Rad(m / 2.0)) /
                   sin(Deg2Rad(m / 2.0 + GradientAtBeginning));
      ad += (2.0 * length * (180.0 / PI)) / den;
      GradientAtBeginning += m;
    }
    else {
      ad += length * sin(Deg2Rad(GradientAtBeginning));
    }
    CString msg;
    msg.Format("Section %d Height=%4.4f (meters)", i, ToMeters(ad));
    dlg->Output(msg);
  }

  dlg->Output("===========================================");

  dlg->Output("");
  dlg->Output("Finshed...");
  dlg->Output("");
  if (failureCount) {
    CString msg;
    msg.Format(
        "You track failed %d tests and may need some work before working",
        failureCount);
    dlg->Output(msg);
    dlg->Output("");
  }
  else {
    dlg->Output("All tests passed...");
  }
  dlg->Output("");
  dlg->Output("Sanity Check Complete...");
}

void
CTrackEditorDoc::OnRemoveRemovemartjinssuggestedscenery()
{
  int result = AfxMessageBox(
      "This command removes all scenery commands, "
      "(0xB0,0xB9,0xBA,0xC0,0xC1,0xD9) ok!",
      MB_YESNO);

  if (result == IDYES) {
    GPTrack *mytrack = getTrack();
    mytrack->RemoveAllCmds(0xB0);
    mytrack->RemoveAllCmds(0xB9);
    mytrack->RemoveAllCmds(0xBA);
    mytrack->RemoveAllCmds(0xC0);
    mytrack->RemoveAllCmds(0xC1);
    mytrack->RemoveAllCmds(0xD9);
    mytrack->regenerate = TRUE;
    OnRefreshTree();
  }
}

void
CTrackEditorDoc::OnHelpScenerylocationtypeb()
{
  AfxMessageBox(
      "Ribbon 1 = 1\nRibbon 2 = 2\nRibbon 3=4\nRibbon 4=8\nBank Left  = "
      "16\nBank Right = 32",
      MB_ICONINFORMATION);
}

void
CTrackEditorDoc::OnPitlanePitstopstratergy()
{
  CPitStopStratergy *dlg = new CPitStopStratergy(getTrack());

  dlg->DoModal();
}

static char lpszVariousFilter[] =
    "Various Data Files (*.txt)|*.txt|Comma Separated (*.csv)|*.csv|All Files "
    "(*.*)|*.*|";

void
CTrackEditorDoc::OnFileExportExportvariousblocks()
{
  char fileName[256];

  strcpy(fileName, "various.csv");
  CFileDialog *fdlg = new CFileDialog(FALSE, "csv", (LPCSTR)&fileName,
                                      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                                      lpszVariousFilter);

  int result = fdlg->DoModal();

  if (result != IDCANCEL) {
    CString filename = fdlg->GetFileName();

    FILE *fp = fopen(filename, "w");
    if (!fp) {
      AfxMessageBox("Unable to open filename");
      return;
    }

    GPTrack *track = getTrack();

    for (int j = 0; j < track->VariousDataCount; j++) {
      int val = track->VariousData[j].GetValue();
      fprintf(fp, "%d\n", val);
    }
    fprintf(fp, "\n");

    fclose(fp);
  }
}

static char lpszCCSetupFilter[] =
    "CCSetup Data Files (*.txt)|*.txt|Comma Separated (*.csv)|*.csv|All Files "
    "(*.*)|*.*|";

void
CTrackEditorDoc::OnFileExportExportccsetup()
{
  char fileName[256];

  strcpy(fileName, "ccsetup.csv");
  CFileDialog *fdlg = new CFileDialog(FALSE, "csv", (LPCSTR)&fileName,
                                      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                                      lpszCCSetupFilter);

  int result = fdlg->DoModal();

  if (result != IDCANCEL) {
    CString filename = fdlg->GetFileName();

    FILE *fp = fopen(filename, "w");
    if (!fp) {
      AfxMessageBox("Unable to open filename");
      return;
    }

    GPTrack *track = getTrack();

    for (int j = 0; j < 19; j++) {
      int lval = (int)LOBYTE(track->carsetup->CCSetpData[j].GetValue());
      int hval = (int)HIBYTE(track->carsetup->CCSetpData[j].GetValue());
      fprintf(fp, "%d\n", lval);
      fprintf(fp, "%d\n", hval);
    }
    fprintf(fp, "\n");

    fclose(fp);
  }
}

void
CTrackEditorDoc::OnHelpScenerylocationtypebc()
{
  AfxMessageBox(
      "Road = 10\nPit Road = 20\nLeft Verge = 9\nRight Verge = 8\nLeft Fence = "
      "5\nRight Fence = 4\nLeft Bank = 18\nRight Bank=19\nRibbon 1 = "
      "11\nRibbon 2 = 12\nRibbon 3=13\nRibbon 4=14\n",
      MB_ICONINFORMATION);
}

void
CTrackEditorDoc::OnHelpScenerylocationtypea()
{
  AfxMessageBox(
      "Left Fence = 1\nRight Fence = 2\nLeft Verge = 16\nRight Verge = "
      "32\nLeft Bank = 64\nRight Bank=128\nRibbon 1 = 256\nRibbon 2 = "
      "512\nRibbon 3=1024\nRibbon 4=2048\n",
      MB_ICONINFORMATION);  // TODO: Add your command handler code here
}

void
CTrackEditorDoc::OnHelpJamidsunusable()
{
  AfxMessageBox(
      "fuel.jam 18-21 \n"
      "cshad1.jam 225 \n"
      "b2rtr.jam 	256-268\n"
      "b1rtr.jam 	269-288\n"
      "b2ftr.jam 	289-301\n"
      "b1ftr.jam 	302-321\n"
      "damage.jam 405-409 \n"
      "mpush.jam 411-415, 404 \n"
      "pitprop1.jam 431, 671 \n"
      "pitprop2.jam 426, 427 \n"
      "rcr1.jam 432-464 \n"
      "rcr2a.jam 465-476 \n"
      "rcr2b.jam (same as rcr2) \n"
      "Cars (lotus.jam, jordan.jam, etc.) 531-544\n"
      "Helmets.jam 545-572\n"
      "rcr3.jam 577-641 \n"
      "rcr2.jam 642-666 \n"
      "pc-wheel.jam 672,673,678,667 \n"
      "pc-extra1.jam 669, 670, 676, 679 \n"
      "pc-extra2.jam 681, 682, 677, 668, 403 \n"
      "pcjack.jam 674, 675, 680, 683, 684 \n"
      "rcr4.jam 685-725 \n"
      "rcr5.jam 726-746 ",
      MB_ICONINFORMATION);
}

void
CTrackEditorDoc::OnTablePitScenery()
{
  CSceneryEditor *dlg = new CSceneryEditor(getTrack(), TRUE);

  dlg->DoModal();

  if (dlg) delete dlg;
}

void
CTrackEditorDoc::OnCclineStraightenccline()
{
  if (!CheckDoYouWantTo()) return;
  GPTrack *mytrack = getTrack();

  for (int i = 0; i < mytrack->CCLineSections->size(); i++) {
    CCLineSection *t = (CCLineSection *)mytrack->CCLineSections->elementAt(i);
    t->Clear(1);
    // mytrack->PutCCSection(t);
  }
  mytrack->CCLineStartYData = 0;
  mytrack->CCLineStartY = 0;
  mytrack->createCC();
  mytrack->regenerate = TRUE;
  UpdateAllViews(NULL);
}

void
CTrackEditorDoc::OnEditClearccline()
{
  if (!CheckDoYouWantTo()) return;
  // TODO: Add your command handler code here
  GPTrack *mytrack = getTrack();

  CCLineSection *t1 = (CCLineSection *)mytrack->CCLineSections->elementAt(0);
  CCLineSection *t = new CCLineSection(t1);

  delete mytrack->CCLineSections;
  mytrack->CCLineSections = new Vector("NewCCLine", __LINE__);
  mytrack->CCLineSections->addElement(t);

  mytrack->CCLineStartYData = 0;
  mytrack->CCLineStartY = 0;
  mytrack->createCC();
  mytrack->regenerate = TRUE;
  UpdateAllViews(NULL);
}

void
CTrackEditorDoc::OnShowTextures()
{
  GPTrack *track = getTrack();
  track->showTextures = !track->showTextures;
  UpdateAllViews(NULL);
}

void
CTrackEditorDoc::OnUpdateShowTextures(CCmdUI *pCmdUI)
{
  GPTrack *track = getTrack();
  pCmdUI->SetCheck(track->showTextures);
}

void
CTrackEditorDoc::OnTexturemapobjects()
{
  GPTrack *track = getTrack();
  track->showTextures = !track->showTextures;
  UpdateAllViews(NULL);
}

void
CTrackEditorDoc::OnUpdateTexturemapobjects(CCmdUI *pCmdUI)
{
  GPTrack *track = getTrack();
  pCmdUI->SetCheck(track->showTextures);
}

void
CTrackEditorDoc::OnRightpopRenameobject()
{
  CTreeCtrl *tree = getTree();

  if (tree != NULL) {
    HTREEITEM selected = tree->GetSelectedItem();

    if (selected != NULL) {
      Observer *obj = (Observer *)getTree()->GetItemData(selected);

      if (obj && obj->isInternalObject()) {
        GPTrack *track = obj->getTrack();

        InternalObject *InObj = (InternalObject *)obj;
        CObjectName *dlg = new CObjectName();
        dlg->m_ObjName = InObj->getName();
        if (dlg->DoModal() == IDOK) {
          track->InternalObjectNames->addElement(
              new IO_name(InObj, _strdup((LPSTR)(LPCSTR)dlg->m_ObjName)));
          InObj->setObjectName(_strdup((LPSTR)(LPCSTR)dlg->m_ObjName));
        }
        delete dlg;
      }
      else {
        AfxMessageBox("No Internal Object  Selected");
      }
    }
    else {
      AfxMessageBox("No Selection");
    }
  }
  else {
    AfxMessageBox("No Tree");
  }
}

void
CTrackEditorDoc::OnRightpopProperties()
{
  CTreeCtrl *tree = getTree();

  if (tree != NULL) {
    HTREEITEM selected = tree->GetSelectedItem();

    if (selected != NULL) {
      Observer *obj = (Observer *)getTree()->GetItemData(selected);

      if (obj && obj->isInternalObject()) {
        GPTrack *track = obj->getTrack();

        InternalObject *InObj = (InternalObject *)obj;
        CObjectProperties *dlg = new CObjectProperties(InObj);
        dlg->DoModal();
        delete dlg;
      }
      else {
        AfxMessageBox("No Internal Object  Selected");
      }
    }
    else {
      AfxMessageBox("No Selection");
    }
  }
  else {
    AfxMessageBox("No Tree");
  }
}

void
CTrackEditorDoc::OnHelpGp2palette()
{
  CPaletteChooser *dlg = new CPaletteChooser();
  dlg->DoModal();
}

void
CTrackEditorDoc::OnViewShowcameras()
{
  GPTrack *mytrack = getTrack();
  mytrack->showCameras = !mytrack->showCameras;
  UpdateAllViews(NULL);
}

void
CTrackEditorDoc::OnUpdateViewShowcameras(CCmdUI *pCmdUI)
{
  GPTrack *mytrack = getTrack();
  pCmdUI->SetCheck(mytrack->showCameras);
}

void
CTrackEditorDoc::OnRemoveRemoveallillegalcameras()
{
  GPTrack *mytrack = getTrack();

  int totalLength = mytrack->totalLength();
  int size = mytrack->TrackCameras->size();

  for (int i = size - 1; i >= 0; i--) {
    Camera *cam = (Camera *)mytrack->TrackCameras->elementAt(i);
    if ((cam->getDistance() - 16384) > totalLength) {
      mytrack->TrackCameras->removeElement(cam);
    }
  }
  OnRefreshTree();
}

void
CTrackEditorDoc::OnRemoveAutospacecameras()
{
  GPTrack *mytrack = getTrack();

  srand((unsigned)time(NULL));

  int totalLength = mytrack->totalLength();
  int numCameras = mytrack->TrackCameras->size();

  // setPerpendicularDistance();

  int interCamDistance = totalLength / numCameras;

  for (int i = 0; i < mytrack->TrackCameras->size(); i++) {
    double perpen = ((double)rand() / (double)RAND_MAX);
    double height = ((double)rand() / (double)RAND_MAX);
    double camz1 = ((double)rand() / (double)RAND_MAX);
    double camz2 = ((double)rand() / (double)RAND_MAX);
    double distp = (perpen * 20000) - 10000;
    double heightp = (height * 2048);
    double camz1p = (height * 8000);
    double camz2p = (height * 160);
    Camera *cam = (Camera *)mytrack->TrackCameras->elementAt(i);
    cam->setDistance((i * interCamDistance) + 16384);
    cam->setSwitchMoment((i * interCamDistance + (interCamDistance / 2)) +
                         16384);
    cam->setPerpendicularDistance((int)(distp));
    cam->setZ1((int)(heightp));
    cam->setZ2((int)(heightp));
    cam->setZF1((int)(camz1p));
    cam->setZF2((int)(camz2p));
  }
  OnRefreshTree();
}

BOOL
CTrackEditorDoc::CheckPitlaneBoxes(CSanity *dlg)
{
  GPTrack *mytrack = getTrack();

  BOOL Before0x9f = TRUE;
  BOOL Before0xA0 = TRUE;

  int BeforeCheckBoth = TRUE;
  int AfterCheckBoth = FALSE;

  int checkState = 0;

  for (int i = 0; i < mytrack->PitlaneSections->size(); i++) {
    TrackSection *t = (TrackSection *)mytrack->PitlaneSections->elementAt(i);
    BOOL has9f = t->containsCommand(0x9f);
    BOOL hasA0 = t->containsCommand(0xA0);

    BOOL pitEntrance = t->isPitLaneEntrance();
    BOOL pitExit = t->isPitLaneExit();

    if (hasA0) Before0xA0 = FALSE;
    if (has9f) Before0x9f = FALSE;

    if (Before0x9f) {
      if (pitEntrance) {
        dlg->Output("Pit Entrance Box Checked before 0x9f command");
        return FALSE;
      }
    }

    if (!Before0xA0) {
      if (pitExit) {
        dlg->Output("Pit Exit Box Checked after 0xA0 command");
        return FALSE;
      }
    }

    if (!Before0x9f && Before0xA0) {
      // middle section
      if (!pitEntrance && !pitExit) {
        CString msg;
        msg.Format(
            "Pit Entrance & Exit Boxes Not Checked between 0x9f && 0xA0 "
            "commands p[%d]",
            i);
        dlg->Output(msg);
        return FALSE;
      }
      if (pitEntrance && pitExit) {
        if (checkState == 1 || checkState == 2) {
          checkState = 2;
        }
        else {
          CString msg;
          msg.Format(
              "Both Entrance and Exit  are checked before a period of just "
              "entrance being checked p[%d]",
              i);
          dlg->Output(msg);
          return FALSE;
        }
      }
      else if (pitEntrance) {
        if (checkState == 0 || checkState == 1) {
          checkState = 1;
        }
        else {
          CString msg;
          msg.Format(
              "Pit Entrance can only be checked after the 0x9f and before both "
              "Entrance and Exit are checked p[%d]",
              i);
          dlg->Output(msg);
          return FALSE;
        }
      }
      else if (pitExit) {
        if (checkState == 2 || checkState == 3) {
          checkState = 3;
        }
        else {
          CString msg;
          msg.Format(
              "Pit Exit can only be checked before the 0xA0 and after both "
              "Entrance and Exit are checked p[%d]",
              i);
          dlg->Output(msg);
          return FALSE;
        }
      }
    }
  }
  return TRUE;
}

void
CTrackEditorDoc::OnRemoveRemoveinternalobjectnames()
{
  GPTrack *mytrack = getTrack();
  mytrack->InternalObjectNames->removeAllElements();
}

void
CTrackEditorDoc::OnViewJamtotalsize()
{
  GPTrack *track = getTrack();
  int size = 0;
  if (track->JamFiles != NULL) {
    int numberOfJamFiles = track->JamFiles->size();
    for (int i = 0; i < numberOfJamFiles; i++) {
      JamFileEditor *t = (JamFileEditor *)track->JamFiles->elementAt(i);
      if (t) {
        t->getJAM()->OpenToRead(FALSE);
        if (t->isDefault()) {
        }
        else {
          size += 256 * (t->getJAM()->header->image_total_size);
        }
      }
    }
  }
  CString msg;
  msg.Format("Total Jam Sizes = %d", size);
  AfxMessageBox(msg);
}

BOOL
CTrackEditorDoc::CheckTrackInWorld(CSanity *dlg)
{
  GPTrack *mytrack = getTrack();

  double xoff = mytrack->TrackStartX.GetValue() / 128;
  double yoff = mytrack->TrackStartY.GetValue() / 128;

  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);

    double sx = t->getStartX() - mytrack->TrackXBeginPos;
    double sy = t->getStartY() - mytrack->TrackYBeginPos;

    double dx = sx + xoff;
    double dy = sy - yoff;

    if (fabs(dx) > 128.0 || fabs(dy) > 128.0) {
      CString msg;
      msg.Format("Track section t[%d] outside allowable world extent", i);
      dlg->Output(msg);
      return FALSE;
    }
  }
  return TRUE;
}

void
CTrackEditorDoc::OnObjectjamsTotaljamsize()
{
  GPTrack *track = getTrack();
  int numberOfJamFiles = track->JamFiles->size();

  int totalSize = 0;
  CString msg;
  for (int i = 0; i < numberOfJamFiles; i++) {
    JamFileEditor *t = (JamFileEditor *)track->JamFiles->elementAt(i);
    if (t) {
      if (!t->isDefault()) {
        int Size = t->JamFileStructure->OpenToRead(TRUE);
        totalSize += Size;
        CString nmsg;
        nmsg.Format("%s \t%d\tTotal:=%d\n", t->getFullPathToJamFileName(), Size,
                    totalSize);
        msg += nmsg;
      }
    }
  }

  CString nmsg;
  nmsg.Format("\n===========================\nTotal Jam Size = %d", totalSize);
  msg += nmsg;
  AfxMessageBox(msg);
}

void
CTrackEditorDoc::OnFileExportExporttrackinfo()
{
  GPTrack *mytrack = getTrack();
  FILE *fp = fopen("Track.txt", "w");

  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);

    double xpos = t->getStartX();
    double ypos = t->getStartY();
    // double zpos = t->getStartZ();
    double nextxpos = t->getEndX();
    double nextypos = t->getEndY();
    // double nextzpos = t2->getStartZ();
    double sectorStartAngle = t->getStartAngle();
    double sectorEndAngle = t->getEndAngle();

    // track parts
    double leftSideX =
        xpos +
        getLeftSideX(sectorStartAngle, WIDTH_TO_TRACKWIDTH(t->getWidthLeft()));
    double leftSideY =
        ypos +
        getLeftSideY(sectorStartAngle, WIDTH_TO_TRACKWIDTH(t->getWidthLeft()));

    double rightSideX =
        xpos + getRightSideX(sectorStartAngle,
                             WIDTH_TO_TRACKWIDTH(t->getWidthRight()));
    double rightSideY =
        ypos + getRightSideY(sectorStartAngle,
                             WIDTH_TO_TRACKWIDTH(t->getWidthRight()));

    // track parts
    double leftSideNX =
        xpos + nextxpos +
        getLeftSideX(sectorEndAngle, WIDTH_TO_TRACKWIDTH(t->getWidthLeft()));
    double leftSideNY =
        ypos + nextypos +
        getLeftSideY(sectorEndAngle, WIDTH_TO_TRACKWIDTH(t->getWidthLeft()));

    double rightSideNX =
        xpos + nextxpos +
        getRightSideX(sectorEndAngle, WIDTH_TO_TRACKWIDTH(t->getWidthRight()));
    double rightSideNY =
        ypos + nextypos +
        getRightSideY(sectorEndAngle, WIDTH_TO_TRACKWIDTH(t->getWidthRight()));

    fprintf(fp, "%d %f %f %f %f %f %f %f %f\n", i, leftSideX, leftSideY,
            rightSideX, rightSideY, rightSideNX, rightSideNY, leftSideNX,
            leftSideNY);
  }
  fprintf(fp, "-1 0 0\n");
  fclose(fp);
}

void
CTrackEditorDoc::OnEditUndo()
{
  CString msg;
  char buff[1024];
  GPTrack *mytrack = getTrack();

  if (!((CTrackEditorApp *)AfxGetApp())->AutoBackup) {
    AfxMessageBox("You Must Enable AutoBackup for Undo to work", MB_ICONERROR);
    return;
  }

  int result = AfxMessageBox(
      "GP2 TE Can undo to the last major change\nWould you like me to Undo?",
      MB_YESNO);

  if (result == IDNO) {
    return;
  }

  mytrack->GP2BackupNumber--;

  if (mytrack->GP2BackupNumber < 1)
    mytrack->GP2BackupNumber = (mytrack->GP2MaxBackupNumber - 1);

  GetTempPath(1024, buff);
  msg.Format("%s\\GP2TE\\TE%4.4d.dat", buff, mytrack->GP2BackupNumber);

  AfxGetApp()->OpenDocumentFile(msg);
  // mytrack->GP2BackupNumber++;
}

void
CTrackEditorDoc::OnFileExportExporttotxftrackexchangeformat()
{
  GPTrack *mytrack = getTrack();
  FILE *fp = fopen("Track.txf", "w");

  double currentangle = ((360 * mytrack->TrackStartAngle.GetValue()) / 65535);
  double xpos = mytrack->TrackStartX.GetValue() / 128;
  double ypos = mytrack->TrackStartY.GetValue() / 128;

  fprintf(fp, "<TRACK>\n");
  // fprintf(fp,"<TRACKINFO> startx=%d starty=%d starta=%d
  // </TRACKINFO>\n",xpos,ypos,currentangle);
  for (int i = 0; i < mytrack->TrackSections->size(); i++) {
    TrackSection *t = (TrackSection *)mytrack->TrackSections->elementAt(i);
    fprintf(fp,
            "<TRK> length=%d angle=%2.2f altitude=%d widthleft=%d "
            "widthright=%d </TRK>\n",
            t->getLength(), t->getAngle(), (int)t->getAltitude(),
            (int)t->getWidthLeft(), (int)t->getWidthRight());
  }
  fprintf(fp, "</TRACK>\n");
  fclose(fp);
}

static char lpszVRMLFilter[] = "VRML File (*.wrl)|*.wrl|All Files (*.*)|*.*|";

void
CTrackEditorDoc::OnFileExportExporttovrml()
{
  char fileName[256];
  GPTrack *mytrack = getTrack();

  strcpy(fileName, "unnamed.wrl");
  CFileDialog *fdlg =
      new CFileDialog(FALSE, "wrl", (LPCSTR)&fileName,
                      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszVRMLFilter);

  int result = fdlg->DoModal();

  if (result != IDCANCEL) {
    CString filename = fdlg->GetPathName();
    mytrack->CreateVRMLFile((LPSTR)(LPCSTR)filename);
  }
  delete fdlg;
}

void
CTrackEditorDoc::OnHelp0xc5locationtyped()
{
  // TODO: Add your command handler code here
  /*
  Switch #: 1 - grey road(no texture) 1
2 - ? 2
3 - fences/walls 4
4 - ? 8
5 - road 16
6 - verges 32
7 - banks 64
8 - all ribbons 128
9 - pitroad 256
10 - pitroad lines 512
11 - ribbon 1 1024
12 - ribbon 2 2048
13 - ribbon 3 4096
14 - ribbon 4 8192
15 - ? 16384
16 - n/a too high value x 32768
*/
  AfxMessageBox(
      "grey road(no texture)= 1\n? = "
      "2\nfences/walls=4\n?=8\nroad=16\nverges=32\nbanks=64\nall "
      "ribbons=128\npitroad=256\npitroad lines=512\nribbon 1=1024\nribbon "
      "2=2048\nribbon 3=4096\nribbon 4=8192\n? = 16384\n",
      MB_ICONINFORMATION);
}

void
CTrackEditorDoc::OnHelp0xa0locationtypee()
{
  // TODO: Add your command handler code here

  AfxMessageBox(
      "1: right verge \n2: left verge \n4: right fence \n8: left fence \n16: "
      "right bank \n32: left bank \n",
      MB_ICONINFORMATION);
}

void
CTrackEditorDoc::OnShowShowsceneryarms()
{
  GPTrack *mytrack = getTrack();

  mytrack->showSceneryArms = !mytrack->showSceneryArms;

  UpdateAllViews(NULL);
}

void
CTrackEditorDoc::OnUpdateShowShowsceneryarms(CCmdUI *pCmdUI)
{
  UPDATE_TRACK(showSceneryArms);
}

void
CTrackEditorDoc::OnShowUseswivelangles()
{
  GPTrack *mytrack = getTrack();

  mytrack->useSwivelAngles = !mytrack->useSwivelAngles;

  UpdateAllViews(NULL);
}

void
CTrackEditorDoc::OnUpdateShowUseswivelangles(CCmdUI *pCmdUI)
{
  UPDATE_TRACK(useSwivelAngles);
}
