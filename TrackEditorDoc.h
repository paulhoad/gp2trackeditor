// TrackEditorDoc.h : interface of the CTrackEditorDoc class
//
/////////////////////////////////////////////////////////////////////////////
#ifndef _DOC_INCLUDED_H_
#define _DOC_INCLUDED_H_

#include "GPTrack.h"
#include "TrackInfo.h"
#include "Info.h"
#include "PitInfo.h"
#include "ObjectInfo.h"
#include "Jam.h"
#include "InternalObject.h"

class CSanity;

class CTrackEditorDoc : public CDocument
{
protected:// create from serialization only
  CTrackEditorDoc();
  DECLARE_DYNCREATE(CTrackEditorDoc)

  // Attributes
public:
  // Operations
  GPTrack* maintrack;
  GPTrack* overlay;

  CTrackInfo* trackTable;
  CInfo* ccTable;
  CPitInfo* pitTable;
  CObjectInfo* objectTable;
  JAM* currentJam;
  InternalObject* currentObj;
  BOOL F1GP;

  BOOL
    isF1GP()
  {
    return F1GP;
  }

  CTreeCtrl*
    getTree();

  JAM*
    getCurrentJam()
  {
    return currentJam;
  }

  void
    setCurrentJam(JAM* jam)
  {
    currentJam = jam;
  }

  void
    setCurrentObject(InternalObject* obj)
  {
    currentObj = obj;
  }

  InternalObject*
    getCurrentObject()
  {
    return currentObj;
  }

  GPTrack*
    getTrack()
  {
    return maintrack;
  }
  GPTrack*
    getOverlayTrack()
  {
    return overlay;
  }
  CSize
    GetSize(double scale);

  // HTREEITEM TrackRootNode;

  // void ResetTree();
  // void ReloadTree();
  void
    MakeVisible(HTREEITEM Node);

  void
    DeleteContents();

  void
    UpdateTree(HTREEITEM Node = NULL)
  {
    MakeVisible(Node);
  }

  BOOL
    CheckTrackObjectOverlap(CDialog* dlg);

  CSize
    FindTopLeft();

  BOOL
    CheckPitlaneBoxes(CSanity* dlg);
  BOOL
    CheckTrackInWorld(CSanity* dlg);

public:
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTrackEditorDoc)
public:
  virtual BOOL
    OnNewDocument();
  virtual void
    Serialize(CArchive& ar);
  virtual BOOL
    OnOpenDocument(LPCTSTR lpszPathName);
  virtual void
    OnCloseDocument();
  virtual void
    SetTitle(LPCTSTR lpszTitle);
  //}}AFX_VIRTUAL

  // Implementation
public:
  virtual ~CTrackEditorDoc();
#ifdef _DEBUG
  virtual void
    AssertValid() const;
  virtual void
    Dump(CDumpContext& dc) const;
#endif

  void
    OnShowJamToOthers()
  {
    OnShowJam();
  }

protected:
  // Generated message map functions
public:
  //{{AFX_MSG(CTrackEditorDoc)
  afx_msg void
    OnFileSaveAs();
  afx_msg void
    OnCircle();
  afx_msg void
    OnTrack();
  afx_msg void
    OnPitlane();
  afx_msg void
    OnObjects();
  afx_msg void
    OnLine();
  afx_msg void
    OnChecksum();
  afx_msg void
    OnEditRemoveallbanks();
  afx_msg void
    OnEditRemoveallheight();
  afx_msg void
    OnViewTrackstatistics();
  afx_msg void
    OnEditRemoveallangles();
  afx_msg void
    OnEditRemoveallccangles();
  afx_msg void
    OnEditRemoveallccargs();
  afx_msg void
    OnEditRemoveallccradius();
  afx_msg void
    OnEditSettracklengths20();
  afx_msg void
    OnEditRemoveallarg2();
  afx_msg void
    OnEditRemoveallccarg1();
  afx_msg void
    OnEditTrackname();
  afx_msg void
    OnFileSave();
  afx_msg void
    OnFileClose();
  afx_msg void
    OnRegenerateTrack();
  afx_msg void
    OnTablesCclinedata();
  afx_msg void
    OnTablesPitlanedata();
  afx_msg void
    OnTablesTrackdata();
  afx_msg void
    OnEditRemoveallfeature1();
  afx_msg void
    OnEditRemoveallfeature2();
  afx_msg void
    OnEditCccarsetup();
  afx_msg void
    OnObjectdata();
  afx_msg void
    OnImport();
  afx_msg void
    OnexportFile();
  afx_msg void
    OnFileDebugoutput();
  afx_msg void
    OnUpdateexportFile(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateImport(CCmdUI* pCmdUI);
  afx_msg void
    OnEditClearccline();
  afx_msg void
    OnCreateCcline();
  afx_msg void
    OnRandomObject();
  afx_msg void
    OnRemoveTrackcmds();
  afx_msg void
    OnRemovePitobjs();
  afx_msg void
    OnRemovePitcmds();
  afx_msg void
    OnRemoveTrackobjs();
  afx_msg void
    OnWorldview();
  afx_msg void
    OnWalls();
  afx_msg void
    OnUpdateWalls(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateTrack(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdatePitlane(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateObjects(CCmdUI* pCmdUI);
  afx_msg void
    OnFillpolygon();
  afx_msg void
    OnUpdateFillpolygon(CCmdUI* pCmdUI);
  afx_msg void
    OnShowTracksides();
  afx_msg void
    OnUpdateShowTracksides(CCmdUI* pCmdUI);
  afx_msg void
    OnTablesTrackview();
  afx_msg void
    OnViewCommands();
  afx_msg void
    OnRemoveXxTrackcmds();
  afx_msg void
    OnRemoveScenery();
  afx_msg void
    OnRemoveSand();
  afx_msg void
    OnMovePitentrance();
  afx_msg void
    OnMovePitexit();
  afx_msg void
    OnViewPitlaneInfo();
  afx_msg void
    OnFindCommand();
  afx_msg void
    OnFindAgain();
  afx_msg void
    OnUpdateFindAgain(CCmdUI* pCmdUI);
  afx_msg void
    OnRefreshTree();
  afx_msg void
    OnCameraView();
  afx_msg void
    OnAddRoadsigns();
  afx_msg void
    OnZeroScenery();
  afx_msg void
    OnRemovePitlaneHeight();
  afx_msg void
    OnShowJam();
  afx_msg void
    OnEditDrivingLine();
  afx_msg void
    OnExportTrackCmds();
  afx_msg void
    OnInsertInternalObject();
  afx_msg void
    OnExportObjectInfo();
  afx_msg void
    OnView3dobject();
  afx_msg void
    OnEditScaletrack();
  afx_msg void
    OnCcScalar();
  afx_msg void
    OnExportNamelist();
  afx_msg void
    OnCreateCcLinetwo();
  afx_msg void
    OnEditCopy();
  afx_msg void
    OnEditCut();
  afx_msg void
    OnEditPaste();
  afx_msg void
    OnTableScenery();
  afx_msg void
    OnStandardscenery();
  afx_msg void
    OnNewObjectDefinition();
  afx_msg void
    OnImportTrif();
  afx_msg void
    OnRemoveTrackcmdsNonzero();
  afx_msg void
    OnTrackRemoveAll();
  afx_msg void
    OnJamIndex();
  afx_msg void
    OnRemoveTrackAnnotations();
  afx_msg void
    OnDefaultTyreWallTexture();
  afx_msg void
    OnDefaultGravelTexture();
  afx_msg void
    OnDefaultTarmacTexture();
  afx_msg void
    OnTrackglview();
  afx_msg void
    OnFileImportccrecord();
  afx_msg void
    OnFileImportImporttracksegmentdata();
  afx_msg void
    OnExportTrackXy();
  afx_msg void
    OnCreateCclinePonk();
  afx_msg void
    OnRemoveRemoveallwallheightchanges();
  afx_msg void
    OnRemoveRemovealltrackwidthchanges();
  afx_msg void
    OnRemoveRemoveeveryscenerycommand();
  afx_msg void
    OnRemoveRemoveall();
  afx_msg void
    OnEditPerformsanitycheck();
  afx_msg void
    OnHelpScenerylocationtypeb();
  afx_msg void
    OnRemoveRemovemartjinssuggestedscenery();
  afx_msg void
    OnPitlanePitstopstratergy();
  afx_msg void
    OnFileExportExportvariousblocks();
  afx_msg void
    OnFileExportExportccsetup();
  afx_msg void
    OnHelpScenerylocationtypebc();
  afx_msg void
    OnHelpScenerylocationtypea();
  afx_msg void
    OnHelpJamidsunusable();
  afx_msg void
    OnTablePitScenery();
  afx_msg void
    OnCclineStraightenccline();
  afx_msg void
    OnShowTextures();
  afx_msg void
    OnUpdateShowTextures(CCmdUI* pCmdUI);
  afx_msg void
    OnTexturemapobjects();
  afx_msg void
    OnUpdateTexturemapobjects(CCmdUI* pCmdUI);
  afx_msg void
    OnRightpopRenameobject();
  afx_msg void
    OnRightpopProperties();
  afx_msg void
    OnHelpGp2palette();
  afx_msg void
    OnViewShowcameras();
  afx_msg void
    OnUpdateViewShowcameras(CCmdUI* pCmdUI);
  afx_msg void
    OnRemoveRemoveallillegalcameras();
  afx_msg void
    OnRemoveAutospacecameras();
  afx_msg void
    OnRemoveRemoveinternalobjectnames();
  afx_msg void
    OnViewJamtotalsize();
  afx_msg void
    OnObjectjamsTotaljamsize();
  afx_msg void
    OnFileExportExporttrackinfo();
  afx_msg void
    OnEditUndo();
  afx_msg void
    OnFileExportExporttotxftrackexchangeformat();
  afx_msg void
    OnFileExportExporttovrml();
  afx_msg void
    OnHelp0xc5locationtyped();
  afx_msg void
    OnHelp0xa0locationtypee();
  afx_msg void
    OnShowShowsceneryarms();
  afx_msg void
    OnUpdateShowShowsceneryarms(CCmdUI* pCmdUI);
  afx_msg void
    OnShowUseswivelangles();
  afx_msg void
    OnUpdateShowUseswivelangles(CCmdUI* pCmdUI);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif
