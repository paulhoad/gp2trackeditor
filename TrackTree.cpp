// TrackTree.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEditor.h"
#include "TrackTree.h"
#include "TrackSection.h"
#include "TrackCmd.h"
#include "InternalObject.h"
#include "JamFileEditor.h"
#include "SingleVarObserver.h"
#include "TextureChooser.h"
#include "TrackEditorDoc.h"
#include "MoveTo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PROFILE(x, y, b) \
  AfxGetApp()->GetProfileInt(CString(#y), CString(#x), b);
#define WR_PROFILE(x, y, b) AfxGetApp()->WriteProfileInt(CString(#y), #x, b);

extern int
  getOldDisplayIconType(GPTrack *track, int id1, int id2);
extern LPCSTR
  getSceneryLocation(int id);

/////////////////////////////////////////////////////////////////////////////
// CTrackTree dialog
extern CTrackEditorApp theApp;
extern LPCSTR
  getTextureLocation(int id);

CString
  getTypeBLocation(int loc)
{
  if (loc == 63) return "All Ribbons/Banks";
  CString msg;

  if (loc & 16 || loc & 32) {
    msg += "Banks:";
    if (loc & 16) msg += "Left ";
    if (loc & 32) msg += "Right ";
  }

  if ((loc & 1) || (loc & 2) || (loc & 4) || (loc & 8)) {
    msg += "Ribbons:";
    if (loc & 1) msg += "1 ";
    if (loc & 2) msg += "2 ";
    if (loc & 4) msg += "3 ";
    if (loc & 8) msg += "4 ";
  }
  return msg;
}

CTrackTree::CTrackTree() : CSizingControlBar(), pImageList(NULL)
{
  //{{AFX_DATA_INIT(CTrackTree)
  //}}AFX_DATA_INIT
  // m_TrackTree = NULL;
  ImagesNotLoaded = TRUE;
  extraDataList = new Vector("TrackTree", __LINE__);
  clipboard = NULL;
}

CTrackTree::~CTrackTree()
{
  // if (pImageList != NULL) delete pImageList;
  EmptyContents();
}

void CTrackTree::LoadImages()
{
  pImageList = (((CTrackEditorApp *)AfxGetApp())->getSharedImageList());
  m_TrackTree.SetImageList(pImageList, TVSIL_NORMAL);

  ImagesNotLoaded = FALSE;

  loadTree();
}

void CTrackTree::UpdateTree(GPTrack *track, Observer *objToReExpandTo)
{
  if (track) {
    track->TrackRootNode = insertTrack(track);
    if (objToReExpandTo && objToReExpandTo->getTreeNode()) {
      HTREEITEM Node = objToReExpandTo->getTreeNode();
      HTREEITEM parentNode = m_TrackTree.GetParentItem(Node);
      m_TrackTree.SelectItem(Node);
      m_TrackTree.Expand(Node, TVE_EXPAND);
    }
    m_TrackTree.Expand(track->TrackRootNode, TVE_EXPAND);
    track->GetDocument()->UpdateAllViews(NULL);
  }
}

HTREEITEM
CTrackTree::GetParent(HTREEITEM node)
{
  return m_TrackTree.GetParentItem(node);
}

#define TO_ID(x) (x - IDB_CIRCUIT)

void CTrackTree::loadTree()
{
  TV_INSERTSTRUCT tvstruct;

  tvstruct.hParent = NULL;
  tvstruct.hInsertAfter = TVI_FIRST;
  tvstruct.item.iImage = TO_ID(IDB_FOLDER);
  tvstruct.item.iSelectedImage = TO_ID(IDB_FOLDER_OPEN);
  tvstruct.item.pszText = _T("All Tracks");
  tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
  HTREEITEM hTreeRoot = m_TrackTree.InsertItem(&tvstruct);

  m_TrackTree.Expand(m_TrackTree.GetRootItem(), TVE_EXPAND);
}

void CTrackTree::deleteNode(HTREEITEM node)
{
  m_TrackTree.DeleteItem(node);
}

/*
HTREEITEM CTrackTree::insertTrack(LPCSTR TrackName)
{
        HTREEITEM hThisItem;

        if (m_TrackTree!=NULL)
        {
                TV_INSERTSTRUCT tvstruct;

        tvstruct.hParent      = m_TrackTree.GetRootItem();
            tvstruct.hInsertAfter = TVI_LAST;
            tvstruct.item.iImage  = TO_ID(IDB_CIRCUIT);
            tvstruct.item.iSelectedImage = TO_ID(IDB_CIRCUIT);
            tvstruct.item.pszText = _T((char*)TrackName);
            tvstruct.item.mask    = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
            hThisItem   = m_TrackTree.InsertItem(&tvstruct);
            m_TrackTree.Expand(m_TrackTree.GetRootItem(), TVE_EXPAND);
        }
        return hThisItem;
}
*/

#define SINGLE_VAR(_parent, _buffer, _id, _var, _name, _desc)      \
  {                                                                \
    HTREEITEM node = insertInfoNode(_parent, _buffer, TO_ID(_id)); \
    SingleVariableObserver *data =                                 \
      new SingleVariableObserver(track, _var, _name, _desc);       \
    extraDataList->addElement(data);                               \
    getTree()->SetItemData(node, (DWORD)data);                     \
  }

#define SINGLE_RGB_VAR(_parent, _buffer, _id, _var, _name, _desc)  \
  {                                                                \
    HTREEITEM node = insertInfoNode(_parent, _buffer, TO_ID(_id)); \
    SingleColorObserver *data =                                    \
      new SingleColorObserver(track, _var, _name, _desc);          \
    extraDataList->addElement(data);                               \
    getTree()->SetItemData(node, (DWORD)data);                     \
  }

char *
  getVariousString(int i)
{
  if (i >= 50) {
    if (!(i % 2))
      return "Bump Table Data (Position from start of track)";
    else
      return "Bump Table Data (value at above position)";
  }

  switch (i) {
  case 11:
    return "Number of Laps";
  case 14:
    return "% of cars doing one stop";
  case 15:
    return "1 stopping cars stop from lap";
  case 16:
    return "1 stopping cars stop during next n laps";
  case 22:
    return "% of cars doing two stop";
  case 23:
    return "2 stopping cars stop 1 from lap";
  case 24:
    return "2 stopping cars stop 1 during next n laps";
  case 25:
    return "2 stopping cars stop 2 from lap";
  case 26:
    return "2 stopping cars stop 2 during next n laps";
  case 30:
    return "% of cars doing three stop";
  case 31:
    return "3 stopping cars stop 1 from lap";
  case 32:
    return "3 stopping cars stop 1 during next n laps";
  case 33:
    return "3 stopping cars stop 2 from lap";
  case 34:
    return "3 stopping cars stop 2 during next n laps";
  case 35:
    return "3 stopping cars stop 3 from lap";
  case 36:
    return "3 stopping cars stop 3 during next n laps";

  case 27:
  case 28:
  case 29:
  case 17:
  case 18:
  case 19:
  case 20:
  case 21:
    return "Possible Unused";
  }
  return NULL;
}

void CTrackTree::EmptyContents()
{
  if (extraDataList != NULL) {
    delete extraDataList;
    extraDataList = NULL;
  }
}

char *
  GetPolePositionSide(int value)
{
  if (value == 768)
    return "Left";
  else if (value == 64768)
    return "Right";
  else
    return "Unable to Determine Pole Position Side";
}

char *
  GetPitSide(int value)
{
  unsigned char bits = 0x0F & LOBYTE(value);
  if (bits == 10)
    return "Left";
  else if (bits == 0)
    return "Right";
  else
    return "Unable to Determine Pit Lane Side";
}

CString
  GetKerbColors(int val)
{
  switch (val) {
  case 0:
    return "Black";
  case 31:
    return "White";
  case 41:
    return "Red";
  case 57:
    return "Yellow";
  case 156:
    return "Blue";
  case 106:
    return "Green";
  }
  return "Unknown Color Number";
}

HTREEITEM
CTrackTree::insertTrack(GPTrack *track)
{
  CString buffer;
  HTREEITEM TrackRoot;

  getTree()->SetRedraw(FALSE);

  if (track->TrackRootNode != NULL) {
    deleteNode(track->TrackRootNode);
    track->TrackRootNode = NULL;
  }

  if (m_TrackTree) {
    TV_INSERTSTRUCT tvstruct;

    buffer.Format("%s", (LPCTSTR)track->getCircuitName());

    tvstruct.hParent = m_TrackTree.GetRootItem();
    tvstruct.hInsertAfter = TVI_LAST;
    tvstruct.item.iImage = TO_ID(IDB_CIRCUIT);
    tvstruct.item.iSelectedImage = TO_ID(IDB_CIRCUIT);
    tvstruct.item.pszText = _T((LPSTR)(LPCSTR)buffer);
    tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
    TrackRoot = m_TrackTree.InsertItem(&tvstruct);

    // GP2INFO
    HTREEITEM HGP2INFO =
      insertInfoNode(TrackRoot, CString("Track Infomation"), TO_ID(IDB_FILE));
    buffer.Format("Country:%s", (LPCTSTR)track->getCircuitCountry());
    insertInfoNode(HGP2INFO, buffer, TO_ID(IDB_OFFSET));

    buffer.Format("Event:%s", (LPCTSTR)track->getCircuitEvent());
    insertInfoNode(HGP2INFO, buffer, TO_ID(IDB_OFFSET));

    buffer.Format("Years:%s", (LPCTSTR)track->getCircuitYears());
    insertInfoNode(HGP2INFO, buffer, TO_ID(IDB_OFFSET));

    buffer.Format("Author:%s", (LPCTSTR)track->getCircuitAuthor());
    insertInfoNode(HGP2INFO, buffer, TO_ID(IDB_OFFSET));

    buffer.Format("Description:%s", (LPCTSTR)track->getCircuitDesc());
    insertInfoNode(HGP2INFO, buffer, TO_ID(IDB_OFFSET));

    buffer.Format("Copyright:%s", (LPCTSTR)track->getCircuitCopyright());
    insertInfoNode(HGP2INFO, buffer, TO_ID(IDB_OFFSET));

    buffer.Format("Laps:%d", track->getCircuitLaps());
    insertInfoNode(HGP2INFO, buffer, TO_ID(IDB_OFFSET));

    buffer.Format("Split1:%d", track->split1);
    insertInfoNode(HGP2INFO, buffer, TO_ID(IDB_OFFSET));

    buffer.Format("Split2:%d", track->split2);
    insertInfoNode(HGP2INFO, buffer, TO_ID(IDB_OFFSET));

    // Statics
    HTREEITEM HPTrackStats =
      insertInfoNode(TrackRoot, CString("Track Config"), TO_ID(IDB_OFFSET));
    char *str = track->getTrackSectionsStr();
    buffer.Format("Track Sections=%s", str);
    free(str);
    HTREEITEM HPTrackDataStats =
      insertInfoNode(HPTrackStats, buffer, TO_ID(IDB_TRACK));

    buffer.Format("Track Start Angle=%d", track->TrackStartAngle.GetValue());
    SINGLE_VAR(HPTrackDataStats, buffer, IDB_FILE, &track->TrackStartAngle, "TrackStartAngle", "Track Starting Angle (0-65535)(0-360) (*)")

    buffer.Format("Track Start Height (Delta)=%d",
      track->TrackStartClimb.GetValue());
    SINGLE_VAR(HPTrackDataStats, buffer, IDB_FILE, &track->TrackStartClimb, "TrackStartClimb", "Track Starting Climbing Angle (*)")

    buffer.Format("Track Start X=%d", track->TrackStartX.GetValue());
    SINGLE_VAR(HPTrackDataStats, buffer, IDB_FILE, &track->TrackStartX, "TrackStartX", "Track Starting X Position in World (*)")

    buffer.Format("Track Start Y=%d", track->TrackStartY.GetValue());
    SINGLE_VAR(HPTrackDataStats, buffer, IDB_FILE, &track->TrackStartY, "TrackStartY", "Track Starting Y Position in World (*)")

    buffer.Format("Track Start Z=%d", track->TrackStartZ.GetValue());
    SINGLE_VAR(HPTrackDataStats, buffer, IDB_FILE, &track->TrackStartZ, "TrackStartUnk", "Track Starting Z Poition in World ")

    buffer.Format("Unknown Number ?=%d", track->UnknownNumber.GetValue());
    SINGLE_VAR(HPTrackDataStats, buffer, IDB_FILE, &track->UnknownNumber, "UnknownNumber", "Unknown to do with start of track (*)")

    buffer.Format("Begin Left Bank =%d", track->BeginTrackWidthLeft.GetValue());
    SINGLE_VAR(HPTrackDataStats, buffer, IDB_FILE, &track->BeginTrackWidthLeft, "Begin Left Bank", "Beginning Road to Bank Width Left")

    buffer.Format("Begin Right Bank =%d",
      track->BeginTrackWidthRight.GetValue());
    SINGLE_VAR(HPTrackDataStats, buffer, IDB_FILE, &track->BeginTrackWidthRight, "Begin Right Bank", "Beginning Road to Bank Width Right")

    buffer.Format("Pole Position Side=%s",
      GetPolePositionSide(track->TrackUnknown1.GetValue()));
    SINGLE_VAR(HPTrackDataStats, buffer, IDB_LIGHTS, &track->TrackUnknown1, "Pole Position Side", "Pole Position Side (768 pole on left 64758 pole on right")

    buffer.Format("PitLane Side=%s (+0xC5 code related?)val=%d",
      GetPitSide(track->C5Code.GetValue()),
      track->C5Code.GetValue());
    CString msg;
    int rightvalue = 0xFFF0 & track->C5Code.GetValue();
    int leftvalue = (0xFFF0 & track->C5Code.GetValue()) | 10;
    msg.Format(
      "Command Value affects C5 argument number - PitLane Side "
      "Left=%d/Right=%d",
      leftvalue,
      rightvalue);

    SINGLE_VAR(HPTrackDataStats, buffer, IDB_PIT, &track->C5Code, "PitLane Side and (C5 Code)", (LPSTR)(LPCSTR)msg)

    buffer.Format("Offset to starting light object=%x",
      track->JamFileUnknown.GetValue());
    SINGLE_VAR(HPTrackDataStats, buffer, IDB_LIGHTS, &track->JamFileUnknown, "Offset to starting light object", "Points to starting light object")

    for (int k = 0; k < track->UnknownNumberCount; k++) {
      buffer.Format("(Untextured Kerb Colors ?=%d  Color=%s",
        track->UnknownNumbers[k].GetValue(),
        GetKerbColors(track->UnknownNumbers[k].GetValue()));
      SINGLE_RGB_VAR(HPTrackDataStats, buffer, IDB_KERB, &track->UnknownNumbers[k], "Untextured Kerb Colors", "Rear Mirror Kerb Colors")
    }

    str = track->getPitSectionsStr();
    buffer.Format("Pit Sections=%s", str);
    HTREEITEM HPPitDataStats =
      insertInfoNode(HPTrackStats, buffer, TO_ID(IDB_PITLANE));
    free(str);

    buffer.Format("PitLaneEntryIndex Section %d", track->PitLaneEntryIndex);
    insertInfoNode(HPPitDataStats, buffer, TO_ID(IDB_PITLANE));
    // SINGLE_VAR(HPPitDataStats,buffer,IDB_PITLANE,track->PitLaneEntryIndex,"PitLaneEntryIndex","Track
    // Section Where Pit Lane Entrance command can be found")

    buffer.Format("PitLaneEntryIndex Section %d", track->PitLaneExitIndex);
    insertInfoNode(HPPitDataStats, buffer, TO_ID(IDB_PITLANE));
    // SINGLE_VAR(HPPitDataStats,buffer,IDB_PITLANE,track->PitLaneExitIndex,"PitLaneExitIndex","Track
    // Section Where Pit Lane Exit command can be found")

    if (track->isPitLaneLeftSide())
      buffer.Format("PitLane Side Left");
    else
      buffer.Format("PitLane Side Right");
    insertInfoNode(HPPitDataStats, buffer, TO_ID(IDB_PITLANE));

    str = track->getCCSectionsStr();
    buffer.Format("CC Line Sections=%s", str);
    HTREEITEM HGP2CCStats =
      insertInfoNode(HPTrackStats, buffer, TO_ID(IDB_CC_LINE));
    free(str);

    buffer.Format("CCLine StartX %d", (int)track->CCLineStartX.GetValue());
    SINGLE_VAR(HGP2CCStats, buffer, IDB_OFFSET, &track->CCLineStartX, "CCLineStartX",
      "X Coordinate for beginning of cc line pushes the cc line to "
      "left/right at beginning")

    buffer.Format("CCLine StartX (High)%d",
      (int)track->CCLineStartXHigh.GetValue());
    SINGLE_VAR(HGP2CCStats, buffer, IDB_OFFSET, &track->CCLineStartXHigh, "CCLineStartXHigh", "X Coordinate (Possibly high bit?) for beginning of cc line")

    buffer.Format("CCLine StartY %d", (int)track->CCLineStartY.GetValue());
    SINGLE_VAR(HGP2CCStats, buffer, IDB_OFFSET, &track->CCLineStartY, "CCLineStartY", "Y Coordinate for beginning of cc line")

    buffer.Format("CCLine Unknown1 %d", (int)track->CCUnknown1.GetValue());
    SINGLE_VAR(HGP2CCStats, buffer, IDB_OFFSET, &track->CCUnknown1, "CCUnknown1", "Unknown at beginning of cc line")

    buffer.Format("CCLine Unknown2 %d", (int)track->CCUnknown2.GetValue());
    SINGLE_VAR(HGP2CCStats, buffer, IDB_OFFSET, &track->CCUnknown2, "CCUnknown2", "Unknown at beginning of cc line")

    buffer.Format("CCLine Unknown3 %d", (int)track->CCUnknown3.GetValue());
    SINGLE_VAR(HGP2CCStats, buffer, IDB_OFFSET, &track->CCUnknown3, "CCUnknown3", "Unknown at beginning of cc line")

    buffer.Format("CCLine Unknown4 %d", (int)track->CCUnknown4.GetValue());
    SINGLE_VAR(HGP2CCStats, buffer, IDB_OFFSET, &track->CCUnknown4, "CCUnknown4", "Unknown at beginning of cc line")

    str = track->getCamerasStr();
    buffer.Format("Cameras=%s", str);
    insertInfoNode(HPTrackStats, buffer, TO_ID(IDB_CAMERA));
    free(str);

    buffer.Format("Various Data Blocks=%d", track->VariousDataCount);
    HTREEITEM various = insertInfoNode(HPTrackStats, buffer, TO_ID(IDB_OFFSET));
    for (int j = 0; j < track->VariousDataCount; j++) {
      char *str = getVariousString(j);
      if (str) {
        buffer.Format("%s = %d", getVariousString(j), (int)track->VariousData[j].GetValue());
      } else {
        buffer.Format("Unknown Various Data [%d] = %d", j, (int)track->VariousData[j].GetValue());
      }
      int iid = IDB_OFFSET;
      if (j >= 11 && j <= 37) {
        iid = IDB_PIT;
      }
      if (j >= 50 && j < track->VariousDataCount - 1) {
        iid = IDB_BUMP;
      }
      char *vstr = _strdup(getVariousString(j));
      if (j == track->VariousDataCount - 1) {
        strcpy(vstr, "Bump Table Terminator");
      }

      SINGLE_VAR(various, buffer, iid, &track->VariousData[j], vstr, "Unknown Code in Various Data Block")
    }

    if (track->carsetup) {
      buffer.Format("All CC CarSetup Data =19 Words");
      HTREEITEM carsetup =
        insertInfoNode(HPTrackStats, buffer, TO_ID(IDB_OFFSET));
      for (int j = 0; j < 19; j++) {
        buffer.Format("CC CarSetup Data %d = %d  - %s", j, (int)track->carsetup->CCSetpData[j].GetValue(), (LPSTR)(LPCSTR)track->carsetup->CCSetpData[j].GetDesc());
        SINGLE_VAR(carsetup, buffer, IDB_OFFSET, &track->carsetup->CCSetpData[j], "CC Setup Data + Unknowns", (LPSTR)(LPCSTR)track->carsetup->CCSetpData[j].GetDesc())
      }
    }

    HTREEITEM HGPOffsets =
      insertInfoNode(TrackRoot, CString("Track Offsets"), TO_ID(IDB_FILE));
    buffer.Format("Large Number (1)=0x%x", track->LargeNumber1.GetValue());
    SINGLE_VAR(HGPOffsets, buffer, IDB_OFFSET, &track->LargeNumber1, "LargeNumber 1", "Large Number One Memory Address (Do not alter!)")

    buffer.Format("Large Number (2)=0x%x", track->LargeNumber2.GetValue());
    SINGLE_VAR(HGPOffsets, buffer, IDB_OFFSET, &track->LargeNumber2, "LargeNumber 1", "Large Number Two Memory Address (Do not alter!)")

    buffer.Format("Track Data Offset=0x%x", track->TrackDataOffset);
    insertInfoNode(HGPOffsets, buffer, TO_ID(IDB_FILE));
    buffer.Format("PitLane Data Offset=0x%x", track->TrackPitLaneStart);
    insertInfoNode(HGPOffsets, buffer, TO_ID(IDB_PITLANE));
    buffer.Format("Internal Object Definitions=%d", track->NumberOfObjects);
    insertInfoNode(HGPOffsets, buffer, TO_ID(IDB_OBJECT_INTERNAL));
    buffer.Format("Object List Data Offset=0x%x", track->ListObjectOffset);
    insertInfoNode(HGPOffsets, buffer, TO_ID(IDB_OBJECT));
    // buffer.Format("Various Offset=0x%x",track->BlockBoffset);
    // insertInfoNode(HGPOffsets,buffer,TO_ID(IDB_FILE));
    buffer.Format("Camera Offset=0x%x", track->CameraOffset);
    insertInfoNode(HGPOffsets, buffer, TO_ID(IDB_CAMERA));
    buffer.Format("SetupOffset Offset=0x%x", track->SetupOffset);
    insertInfoNode(HGPOffsets, buffer, TO_ID(IDB_FILE));
    buffer.Format("Jam Start Offset=0x%x", track->JamFileWritingStartOffset);
    insertInfoNode(HGPOffsets, buffer, TO_ID(IDB_FILE));
    buffer.Format("Jamfiles Offset=0x%x", track->JamFileWritingOffset);
    insertInfoNode(HGPOffsets, buffer, TO_ID(IDB_FILE));
    buffer.Format("CheckSum Offset=0x%x", track->CheckSumOffset);
    insertInfoNode(HGPOffsets, buffer, TO_ID(IDB_SUM));

    // GP2Track
    str = track->getTrackSectionsStr();
    buffer.Format("Track Data (%s)", str);
    free(str);
    HTREEITEM HGP2Track = insertInfoNode(TrackRoot, buffer, TO_ID(IDB_TRACK));

    int trackSize = track->TrackSections->size();
    for (int i = 0; i < trackSize; i++) {
      // char trackStrBuffer[256];
      CString trackStrBuffer;
      TrackSection *t = (TrackSection *)track->TrackSections->elementAt(i);

      t->LoadTree(this, HGP2Track, i, TRUE);
    }

    // GP2Pit
    str = track->getPitSectionsStr();
    buffer.Format("Pit Data (%s)", str);
    HTREEITEM HGP2Pit = insertInfoNode(TrackRoot, buffer, TO_ID(IDB_PITLANE));
    free(str);

    int pitSections = track->PitlaneSections->size();
    for (int i = 0; i < pitSections; i++) {
      CString buffer;
      TrackSection *t = (TrackSection *)track->PitlaneSections->elementAt(i);
      t->LoadTree(this, HGP2Pit, i, FALSE);
    }

    // GP2CCLine
    str = track->getCCSectionsStr();
    buffer.Format("CC Line Data (%s)", str);
    HTREEITEM HGP2CCLine =
      insertInfoNode(TrackRoot, buffer, TO_ID(IDB_CC_LINE));
    free(str);

    int ccsections = track->CCLineSections->size();
    for (int i = 0; i < ccsections; i++) {
      CCLineSection *t = (CCLineSection *)track->CCLineSections->elementAt(i);
      buffer.Format("[%d] len=%d rad=%d", i, (int)t->getLength(), (int)t->getRadius());
      int id = t->getIcon();

      // int directction = (int)t->getArg2();

      // if (directction > 0)      id = IDB_CC_RIGHT;
      // else if (directction < 0) id = IDB_CC_LEFT;
      HTREEITEM HGP2TrackSection =
        insertInfoNode(HGP2CCLine, buffer, TO_ID(id));
      getTree()->SetItemData(HGP2TrackSection, (DWORD)t);
      t->setTreeNode(HGP2TrackSection);
    }

    // GP2Objects
    str = track->getObjectDefinitionsStr();
    buffer.Format("Object Definitions(%s)", str);
    HTREEITEM HGP2Objects =
      insertInfoNode(TrackRoot, buffer, TO_ID(IDB_OBJECT));
    free(str);

    int numberOfObjectDefs = track->TrackObjectDefintions->size();
    for (int i = 0; i < numberOfObjectDefs; i++) {
      TrackObjectDefinition *t =
        (TrackObjectDefinition *)track->TrackObjectDefintions->elementAt(i);
      buffer.Format("[%d](%s)  Offset=%d DFC=%d", i, t->getName(), t->getLocator(), t->getDistance());
      int id = t->getDisplayIconType();
      HTREEITEM HGP2TrackSection =
        insertInfoNode(HGP2Objects, buffer, TO_ID(id));
      getTree()->SetItemData(HGP2TrackSection, (DWORD)t);
      t->setTreeNode(HGP2TrackSection);
    }

    // GP2InternalObjects
    int numberOfInternalObjects = track->InternalObjectOffsets->size();
    buffer.Format("Internal Object Definitions(%d)", numberOfInternalObjects);
    HTREEITEM HGP2IntObjects =
      insertInfoNode(TrackRoot, buffer, TO_ID(IDB_OBJECT_INTERNAL));
    for (int i = 0; i < numberOfInternalObjects; i++) {
      InternalObject *t =
        (InternalObject *)track->InternalObjectOffsets->elementAt(i);

      buffer.Format("[%d] id1=%d %s 0x%x size=%d Points=%d", i, i + 17, t->getName(), t->getOffset(), t->getSize(), t->getNumPoints());
      HTREEITEM HP2Obj =
        insertInfoNode(HGP2IntObjects, buffer, TO_ID(getOldDisplayIconType(track, i + 17, 0)));
      getTree()->SetItemData(HP2Obj, (DWORD)t);
      // t->LoadTreeTop(this,HP2Obj);
      t->setTreeNode(HP2Obj);
      t->SubTree =
        insertInfoNode(HP2Obj, CString("Undiscovered"), TO_ID(IDB_OFFSET));
    }

    // GP2Cameras
    str = track->getCamerasStr();
    buffer.Format("Camera Definitions(%s)", str);
    free(str);

    HTREEITEM HGP2Cameras =
      insertInfoNode(TrackRoot, buffer, TO_ID(IDB_CAMERA));
    int numberOfCameras = track->TrackCameras->size();
    for (int i = 0; i < numberOfCameras; i++) {
      Camera *t = (Camera *)track->TrackCameras->elementAt(i);
      t->setNumber(i);
      if (t) {
        buffer.Format("Camera#%d dist=%d height=%d", i, t->getDistance() - 16384, t->getZ1());

        int id = IDB_CAMERA;
        if ((t->getDistance() - 16384) > track->totalLength()) {
          id = IDB_CAMERA_ERR;
        }

        HTREEITEM HGP2Camera = insertInfoNode(HGP2Cameras, buffer, TO_ID(id));
        getTree()->SetItemData(HGP2Camera, (DWORD)t);
        t->setTreeNode(HGP2Camera);
      }
    }

    if (track->carsetup) {
      buffer.Format("CC Car Setup");
      HTREEITEM HGP2Setup = insertInfoNode(TrackRoot, buffer, TO_ID(IDB_FILE));
      getTree()->SetItemData(HGP2Setup, (DWORD)track->carsetup);

      buffer.Format("CC Front Wing=%d", track->carsetup->CCFrontWing);
      HTREEITEM HGP2FWingSetup =
        insertInfoNode(HGP2Setup, buffer, TO_ID(IDB_FRONT_WING));
      getTree()->SetItemData(HGP2FWingSetup, (DWORD)track->carsetup);

      buffer.Format("CC Rear Wing=%d", track->carsetup->CCRearWing);
      HTREEITEM HGP2RWingSetup =
        insertInfoNode(HGP2Setup, buffer, TO_ID(IDB_REAR_WING));
      getTree()->SetItemData(HGP2RWingSetup, (DWORD)track->carsetup);

      buffer.Format("CC 1st Gear=%d", track->carsetup->CCFirstGear);
      HTREEITEM H1Gear = insertInfoNode(HGP2Setup, buffer, TO_ID(IDB_GEAR));
      getTree()->SetItemData(H1Gear, (DWORD)track->carsetup);

      buffer.Format("CC 2nd Gear=%d", track->carsetup->CCSecondGear);
      HTREEITEM H2Gear = insertInfoNode(HGP2Setup, buffer, TO_ID(IDB_GEAR));
      getTree()->SetItemData(H2Gear, (DWORD)track->carsetup);

      buffer.Format("CC 3rd Gear=%d", track->carsetup->CCThirdGear);
      HTREEITEM H3Gear = insertInfoNode(HGP2Setup, buffer, TO_ID(IDB_GEAR));
      getTree()->SetItemData(H3Gear, (DWORD)track->carsetup);

      buffer.Format("CC 4th Gear=%d", track->carsetup->CCFourthGear);
      HTREEITEM H4Gear = insertInfoNode(HGP2Setup, buffer, TO_ID(IDB_GEAR));
      getTree()->SetItemData(H4Gear, (DWORD)track->carsetup);

      buffer.Format("CC 5th Gear=%d", track->carsetup->CCFifthGear);
      HTREEITEM H5Gear = insertInfoNode(HGP2Setup, buffer, TO_ID(IDB_GEAR));
      getTree()->SetItemData(H5Gear, (DWORD)track->carsetup);

      buffer.Format("CC 6th Gear=%d", track->carsetup->CCSixthGear);
      HTREEITEM H6Gear = insertInfoNode(HGP2Setup, buffer, TO_ID(IDB_GEAR));
      getTree()->SetItemData(H6Gear, (DWORD)track->carsetup);

      buffer.Format("Tyre Compound =%c val=%d",
        (char)((track->carsetup->CCTyreType) + 13),
        track->carsetup->CCTyreType);
      HTREEITEM HType = insertInfoNode(HGP2Setup, buffer, TO_ID(IDB_TYRE));
      getTree()->SetItemData(HType, (DWORD)track->carsetup);

      buffer.Format("Acceleration=%d", track->carsetup->CCAccel);
      HTREEITEM HAccel = insertInfoNode(HGP2Setup, buffer, TO_ID(IDB_FILE));
      getTree()->SetItemData(HAccel, (DWORD)track->carsetup);

      buffer.Format("Track Grip=%d", track->carsetup->CCTrackGrip);
      HTREEITEM HGrip = insertInfoNode(HGP2Setup, buffer, TO_ID(IDB_FILE));
      getTree()->SetItemData(HGrip, (DWORD)track->carsetup);

      buffer.Format("Brake Balance=%d", track->carsetup->CCBrakeBalance);
      HTREEITEM HBal = insertInfoNode(HGP2Setup, buffer, TO_ID(IDB_FILE));
      getTree()->SetItemData(HBal, (DWORD)track->carsetup);

      buffer.Format("Air Resistance=%d", track->carsetup->CCAirResist);
      HTREEITEM HAir = insertInfoNode(HGP2Setup, buffer, TO_ID(IDB_FILE));
      getTree()->SetItemData(HAir, (DWORD)track->carsetup);

      buffer.Format("CC Fuel Consumption? =%d",
        track->carsetup->CCFuelConsumption);
      HTREEITEM HFuel = insertInfoNode(HGP2Setup, buffer, TO_ID(IDB_FUEL));
      getTree()->SetItemData(HFuel, (DWORD)track->carsetup);
    }

    // GP2JamFiles
    if (track->JamFiles != NULL) {
      int numberOfJamFiles = track->JamFiles->size();
      buffer.Format("Jam Files(%d)", numberOfJamFiles);
      HTREEITEM HGP2JamFiles =
        insertInfoNode(TrackRoot, buffer, TO_ID(IDB_JAMFILE));
      for (int i = 0; i < numberOfJamFiles; i++) {
        JamFileEditor *t = (JamFileEditor *)track->JamFiles->elementAt(i);
        if (t) {
          // t->LoadJamTree(this,HGP2JamFiles);

          buffer.Format("%d %s", i, t->getJamFileName());
          int id = IDB_JAMFILE;
          if (t->isDefault()) id = IDB_JAMFILE_INTERNAL;
          HTREEITEM HGP2Jam = insertInfoNode(HGP2JamFiles, buffer, TO_ID(id));
          t->setTreeNode(getTree(), HGP2Jam);
          getTree()->SetItemData(HGP2Jam, (DWORD)t);
          t->LoadJamTree(this, HGP2Jam);
        }
      }
    }

    HTREEITEM HPMisc =
      insertInfoNode(TrackRoot, CString("Misc Config"), TO_ID(IDB_OFFSET));

    buffer.Format("UnderLayBitmapX %d", (int)track->UnderLayBitmapX.GetValue());
    SINGLE_VAR(HPMisc, buffer, IDB_OFFSET, &track->UnderLayBitmapX, "UnderLayBitmapX", "X Coordinate underlay bitmap")
    buffer.Format("UnderLayBitmapY %d", (int)track->UnderLayBitmapY.GetValue());
    SINGLE_VAR(HPMisc, buffer, IDB_OFFSET, &track->UnderLayBitmapY, "UnderLayBitmapY", "Y Coordinate underlay bitmap")
    buffer.Format("UnderLayBitmapWidth %d",
      (int)track->UnderLayBitmapWidth.GetValue());
    SINGLE_VAR(HPMisc, buffer, IDB_OFFSET, &track->UnderLayBitmapWidth, "UnderLayBitmapWidth", "Width underlay bitmap")
    buffer.Format("UnderLayBitmapHeight %d",
      (int)track->UnderLayBitmapHeight.GetValue());
    SINGLE_VAR(HPMisc, buffer, IDB_OFFSET, &track->UnderLayBitmapHeight, "UnderLayBitmapHeight", "Height underlay bitmap")

    m_TrackTree.Expand(m_TrackTree.GetRootItem(), TVE_EXPAND);
  }
  getTree()->SetRedraw(TRUE);
  return TrackRoot;
}

TrackObjectDefinition *
  GPTrack::FindObjectDefinition(int Offset)
{
  for (int i = 0; i < TrackObjectDefintions->size(); i++) {
    TrackObjectDefinition *t =
      (TrackObjectDefinition *)TrackObjectDefintions->elementAt(i);
    if (t->getLocator() == Offset) return t;
  }
  return NULL;
}

HTREEITEM
CTrackTree::insertInfoNode(HTREEITEM parent, CString &title, int bitmap)
{
  if (m_TrackTree) {
    TV_INSERTSTRUCT tvstruct;

    tvstruct.hParent = parent;
    tvstruct.hInsertAfter = TVI_LAST;
    tvstruct.item.iImage = bitmap;
    tvstruct.item.iSelectedImage = bitmap;
    tvstruct.item.pszText = _T((LPSTR)(LPCSTR)title);
    tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
    // tvstruct.item.mask    = TVIF_IMAGE | TVIF_TEXT;
    HTREEITEM hThisItem = m_TrackTree.InsertItem(&tvstruct);
    return hThisItem;
  } else {
    char msg[256];

    wsprintf(msg, "m_TrackTree is NULL for adding in %s", title);
    AfxMessageBox(msg);
  }
  return 0;
}

void CTrackTree::DoDataExchange(CDataExchange *pDX)
{
  // CDialogBar::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CTrackTree)
  DDX_Control(pDX, IDC_TRACKTREE, m_Tree);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTrackTree, CSizingControlBar)
//{{AFX_MSG_MAP(CTrackTree)
ON_WM_CREATE()
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_NOTIFY(NM_DBLCLK, IDC_TRACKTREE, OnDblclkTracktree)
ON_WM_SIZE()
ON_WM_RBUTTONDOWN()
ON_WM_RBUTTONUP()
ON_NOTIFY(NM_RCLICK, IDC_TRACKTREE, OnRclickTracktree)
ON_COMMAND(ID_REMOVE_TRACK_CMD_TREE, OnRemoveTrackCmdTree)
ON_COMMAND(ID_INSERT_TRACKCMD_TREE, OnInsertTrackcmdTree)
ON_UPDATE_COMMAND_UI(ID_REMOVE_TRACK_CMD_TREE, OnUpdateRemoveTrackCmdTree)
ON_UPDATE_COMMAND_UI(ID_INSERT_TRACKCMD_TREE, OnUpdateInsertTrackcmdTree)
ON_NOTIFY(TVN_SELCHANGED, IDC_TRACKTREE, OnSelchangedTracktree)
ON_COMMAND(ID_SHOW_JAM, OnShowJam)
ON_NOTIFY(NM_CLICK, IDC_TRACKTREE, OnClickTracktree)
ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TRACKTREE, OnItemexpandingTracktree)
ON_COMMAND(ID_EXPORT_OBJECT_INFO, OnExportObjectInfo)
ON_WM_CLOSE()
ON_WM_DESTROY()
ON_COMMAND(ID_RIGHTPOP_MOVETONEXTSECTION, OnRightpopMovetonextsection)
ON_COMMAND(ID_RIGHTPOP_MOVETOPREVIOUSSECTION, OnRightpopMovetoprevioussection)
ON_COMMAND(ID_RIGHTPOP_MOVETO, OnRightpopMoveto)
ON_UPDATE_COMMAND_UI(ID_RIGHTPOP_MOVETO, OnUpdateRightpopMoveto)
ON_UPDATE_COMMAND_UI(ID_RIGHTPOP_MOVETONEXTSECTION,
  OnUpdateRightpopMovetonextsection)
ON_UPDATE_COMMAND_UI(ID_RIGHTPOP_MOVETOPREVIOUSSECTION,
  OnUpdateRightpopMovetoprevioussection)
ON_COMMAND(ID_HELP_OBJECT, OnHelpObject)
ON_COMMAND(ID_REMOVE_CAMERA, OnRemoveCamera)
ON_COMMAND(ID_INSERT_CAMERA, OnAddCamera)
ON_UPDATE_COMMAND_UI(ID_REMOVE_CAMERA, OnUpdateRemoveCamera)
ON_UPDATE_COMMAND_UI(ID_INSERT_CAMERA, OnUpdateAddCamera)
ON_COMMAND(ID_REMOVE_JAMFILE, OnRemoveJamFile)
ON_COMMAND(ID_INSERT_JAMFILE, OnAddJamFile)
ON_COMMAND(ID_RENAME_JAMFILE, OnRenameJamFile)
ON_COMMAND(IDC_COMMAND_VIEW_JAM, OnShowJam)
ON_COMMAND(ID_MOVE_COMMAND_UP, OnMoveCommandUp)
ON_COMMAND(ID_MOVE_COMMAND_DOWN, OnMoveCommandDown)
ON_COMMAND(ID_EXPORT_INTERNAL, OnExportObject)
ON_COMMAND(ID_REMOVE_INTERNALOBJ, OnRemoveInternalObject)
ON_COMMAND(ID_REMOVE_OBJECTDEF, OnRemoveObjectDef)
ON_COMMAND(ID_EDIT_COPY, OnCopy)
ON_COMMAND(ID_EDIT_PASTE, OnPaste)
ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateRemoveTrackCmdTree)
ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateRemoveTrackCmdTree)
ON_COMMAND(ID_REPLACEINTERNALOBJECT, OnReplaceinternalobject)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackTree message handlers

void CTrackTree::OnPaint()
{
  CPaintDC dc(this);// device context for painting

  if (m_TrackTree && ImagesNotLoaded) LoadImages();

  // Do not call CDialogBar::OnPaint() for painting messages
}

void CTrackTree::OnLButtonDown(UINT nFlags, CPoint point)
{}

void CTrackTree::OnDblclkTracktree(NMHDR *pNMHDR, LRESULT *pResult)
{
  if (m_TrackTree) {
    HTREEITEM TreeNode = m_TrackTree.GetSelectedItem();

    DWORD data = m_TrackTree.GetItemData(TreeNode);

    Observer *dlg = (Observer *)data;
    if (dlg != NULL) {
      dlg->Open();
      if (dlg->track) {
        dlg->track->GetDocument()->UpdateAllViews(NULL);
      }
    }
  } else {
    AfxMessageBox("Tree NULL", MB_OK);
  }

  *pResult = 0;
}

void CTrackTree::OnRButtonDown(UINT nFlags, CPoint point)
{
  CSizingControlBar::OnRButtonDown(nFlags, point);
}

void CTrackTree::OnRButtonUp(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default

  CSizingControlBar::OnRButtonUp(nFlags, point);
}

void CTrackTree::OnRclickTracktree(NMHDR *pNMHDR, LRESULT *pResult)
{
  // TODO: Add your control notification handler code here
  // TODO: Add your message handler code here and/or call default
  CMenu menu;
  CMenu *submenu;

  // load the menu
  menu.LoadMenu(IDR_LAUNCH);

  // get the popup menu
  HTREEITEM selected = m_TrackTree.GetSelectedItem();

  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);
    if (obj != NULL) {
      if (obj->isJamFile())
        submenu = menu.GetSubMenu(2);
      else if (obj->isInternalObject())
        submenu = menu.GetSubMenu(3);
      else if (obj->isCamera())
        submenu = menu.GetSubMenu(5);
      else
        submenu = menu.GetSubMenu(1);
    }
  } else {
    submenu = menu.GetSubMenu(1);
  }

  // submenu = menu.GetSubMenu(1);

  CPoint point;

  GetCursorPos(&point);

  // convert to screen coordinates
  // ClientToScreen(&point);

  // post the menu
  submenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this, NULL);

  *pResult = 0;
}

void CTrackTree::OnRemoveTrackCmdTree()
{
  // TODO: Add your command handler code here
  HTREEITEM selected = m_TrackTree.GetSelectedItem();

  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);

    if (obj != NULL) {
      GPTrack *track = obj->getTrack();
      HTREEITEM node = obj->getTreeNode();
      m_TrackTree.SetItemData(node, NULL);
      Observer *myparent = obj->parent;
      obj->Delete();
      UpdateTree(track, myparent);
    } else {
      AfxMessageBox("Cannot Remove item does not support removal");
    }
  } else {
    AfxMessageBox("No Selection");
  }
}

void CTrackTree::OnInsertTrackcmdTree()
{
  // TODO: Add your command handler code he
  HTREEITEM selected = m_TrackTree.GetSelectedItem();

  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);
    if (obj != NULL) {
      obj->InsertNew();
      obj->getTrack()->createTrack(NULL);
      UpdateTree(obj->getTrack(), obj);
    }
  } else {
    AfxMessageBox("No Selection");
  }
}

void CTrackTree::OnRemoveCamera()
{
  // TODO: Add your command handler code he
  HTREEITEM selected = m_TrackTree.GetSelectedItem();

  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);
    if (obj != NULL) {
      GPTrack *track = obj->getTrack();
      HTREEITEM node = obj->getTreeNode();
      // HTREEITEM parentnode = GetParent(node);
      m_TrackTree.SetItemData(node, NULL);
      Observer *myparent = obj->parent;
      obj->Delete();
      m_TrackTree.DeleteItem(node);
      UpdateTree(track, myparent);
    }
  } else {
    AfxMessageBox("No Selection");
  }
}

void CTrackTree::OnAddCamera()
{
  // TODO: Add your command handler code he
  HTREEITEM selected = m_TrackTree.GetSelectedItem();

  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);
    if (obj != NULL) {
      obj->InsertNew();
      UpdateTree(obj->getTrack(), obj);
    }
  } else {
    AfxMessageBox("No Selection");
  }
}

void CTrackTree::OnRemoveJamFile()
{
  HTREEITEM selected = m_TrackTree.GetSelectedItem();

  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);
    GPTrack *track = obj->getTrack();

    if (obj != NULL) {
      HTREEITEM node = obj->getTreeNode();
      m_TrackTree.SetItemData(node, NULL);
      Observer *myparent = obj->parent;
      obj->Delete();
      // m_TrackTree.DeleteItem(node);
      UpdateTree(track, myparent);
    }
  } else {
    AfxMessageBox("No Selection");
  }
}

void CTrackTree::OnAddJamFile()
{
  // TODO: Add your command handler code he
  HTREEITEM selected = m_TrackTree.GetSelectedItem();

  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);

    if (obj != NULL) {
      GPTrack *track = obj->getTrack();
      JamFileEditor *jam = new JamFileEditor(track, "gamejams\\unnamed.jam");

      if (jam->Open() != IDCANCEL) {
        track->JamFiles->addElement(jam);
        UpdateTree(track, jam);
      }
    } else {
      AfxMessageBox(
        "Which Track? No Selection (select jamfile or tracksection");
    }
  } else {
    AfxMessageBox("No Selection");
  }
}

void CTrackTree::OnUpdateRemoveTrackCmdTree(CCmdUI *pCmdUI)
{
  // TODO: Add your command update UI handler code here
  HTREEITEM selected = m_TrackTree.GetSelectedItem();

  /*
  if (selected != NULL)
  {
          Observer *obj = (Observer*)m_TrackTree.GetItemData(selected);
          if (obj!=NULL)
          {
                  if (obj->isCamera())
                  {
                          pCmdUI->Enable(TRUE);
                          return;
                  }
          }
  }
  */
  pCmdUI->Enable(TRUE);
}

void CTrackTree::OnUpdateInsertTrackcmdTree(CCmdUI *pCmdUI)
{
  // TODO: Add your command update UI handler code here
}

void CTrackTree::OnUpdateRemoveCamera(CCmdUI *pCmdUI)
{
  // TODO: Add your command update UI handler code here
}

void CTrackTree::OnUpdateAddCamera(CCmdUI *pCmdUI)
{
  // TODO: Add your command update UI handler code here
}

void CTrackTree::OnSelchangedTracktree(NMHDR *pNMHDR, LRESULT *pResult)
{
  NM_TREEVIEW *pNMTreeView = (NM_TREEVIEW *)pNMHDR;

  HTREEITEM selected = m_TrackTree.GetSelectedItem();

  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);

    if (obj) {
      theApp.getTable()->LoadTable(obj);
      obj->Selected();
      if (obj->track) {
        obj->track->GetDocument()->UpdateAllViews(NULL);
      }
    } else {
      // AfxMessageBox("No Properties Table Observer");
    }
  } else {
    AfxMessageBox("No Selection");
  }

  *pResult = 0;
}

void CTrackTree::OnShowJam()
{
  HTREEITEM selected = m_TrackTree.GetSelectedItem();

  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);

    if (obj) {
      GPTrack *track = obj->getTrack();
      CTrackEditorDoc *pDoc = (CTrackEditorDoc *)track->GetDocument();
      pDoc->OnShowJamToOthers();
    } else {
      // AfxMessageBox("No Properties Table Observer");
    }
  } else {
    AfxMessageBox("No Selection");
  }
}

void CTrackTree::OnClickTracktree(NMHDR *pNMHDR, LRESULT *pResult)
{
  // TODO: Add your control notification handler code here
  HTREEITEM selected = m_TrackTree.GetSelectedItem();

  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);

    if (obj) {
      // GPTrack *track = obj->getTrack();
      obj->Selected();
      if (obj->track) {
        obj->track->GetDocument()->UpdateAllViews(NULL);
      }
    }
  }

  *pResult = 0;
}

void CTrackTree::OnItemexpandingTracktree(NMHDR *pNMHDR, LRESULT *pResult)
{
  NM_TREEVIEW *pNMTreeView = (NM_TREEVIEW *)pNMHDR;
  // TODO: Add your control notification handler code here

  if (m_TrackTree) {
    TV_ITEM *TreeItem = (TV_ITEM *)(&pNMTreeView->itemNew);

    if (TreeItem != NULL) {
      HTREEITEM TreeNode = TreeItem->hItem;
      if (TreeNode != 0) {
        DWORD data = m_TrackTree.GetItemData(TreeNode);

        Observer *dlg = (Observer *)data;
        if (dlg != NULL) {
          dlg->Expand(this, TreeNode);
        }
      }
    } else {
      AfxMessageBox("No Selection in tree");
    }
  }
  *pResult = 0;
}

void CTrackTree::OnExportObjectInfo()
{
  // TODO: Add your command handler code here
}

void CTrackTree::OnMoveCommandUp()
{
  // TODO: Add your command handler code he
  HTREEITEM selected = m_TrackTree.GetSelectedItem();

  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);
    if (obj != NULL && obj->isCommand()) {
      TrackCmd *cmd = (TrackCmd *)obj;

      TrackSection *CommandsSection = cmd->owner;

      if (CommandsSection != NULL) {
        Vector *cmds = CommandsSection->getCommands();

        TrackCmd *firstcmd = (TrackCmd *)cmds->firstElement();

        if (cmd != firstcmd) {
          // cmd is not the first command
          TrackCmd *prev = (TrackCmd *)cmds->previous(cmd);
          cmds->removeElementRef(cmd);
          cmds->insertBeforeElement(prev, cmd);
        } else {
          AfxMessageBox(
            "Cannot move the first command up select second commands");
        }
      } else {
        AfxMessageBox("Cannot find Commands Parent");
      }

      // obj->InsertNew();
      UpdateTree(obj->getTrack(), CommandsSection);
    } else if (obj != NULL && obj->isCamera()) {
      Camera *cam = (Camera *)obj;

      GPTrack *track = cam->track;

      if (track != NULL) {
        Vector *cmds = track->TrackCameras;

        Camera *firstcmd = (Camera *)cmds->firstElement();

        if (cam != firstcmd) {
          // cmd is not the first command
          Camera *prev = (Camera *)cmds->previous(cam);
          cmds->removeElementRef(cam);
          cmds->insertBeforeElement(prev, cam);
        } else {
          AfxMessageBox("Cannot move the first camera up select second camera");
        }
      } else {
        AfxMessageBox("Cannot find Cameras Parent");
      }
      UpdateTree(obj->getTrack(), obj);
    } else {
      AfxMessageBox("This Item Cannot be moved!");
    }
  } else {
    AfxMessageBox("No Selection");
  }
}

void CTrackTree::OnMoveCommandDown()
{
  // TODO: Add your command handler code he
  HTREEITEM selected = m_TrackTree.GetSelectedItem();

  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);
    if (obj != NULL && obj->isCommand()) {
      TrackCmd *cmd = (TrackCmd *)obj;

      TrackSection *CommandsSection = cmd->owner;

      if (CommandsSection != NULL) {
        Vector *cmds = CommandsSection->getCommands();

        // TrackCmd *lastcmd = (TrackCmd*)cmds->lastElement();
        TrackCmd *lastcmd = (TrackCmd *)cmds->elementAt(cmds->size() - 1);

        if (cmd != lastcmd) {
          // cmd is not the first command
          TrackCmd *next = (TrackCmd *)cmds->next(cmd);
          cmds->removeElementRef(cmd);
          cmds->insertAfterElement(next, cmd);
        } else {
          AfxMessageBox(
            "Cannot move the last command down select second to last "
            "commands");
        }
      } else {
        AfxMessageBox("Cannot find Commands Parent");
      }
      UpdateTree(obj->getTrack(), CommandsSection);
    } else if (obj != NULL && obj->isCamera()) {
      Camera *cam = (Camera *)obj;

      GPTrack *track = cam->track;

      if (track != NULL) {
        Vector *cmds = track->TrackCameras;

        // Camera *firstcmd = (Camera*)cmds->lastElement();
        Camera *firstcmd = (Camera *)cmds->elementAt(cmds->size() - 1);

        if (cam != firstcmd) {
          // cmd is not the first command
          Camera *next = (Camera *)cmds->next(cam);
          cmds->removeElementRef(cam);
          cmds->insertAfterElement(next, cam);
        } else {
          AfxMessageBox("Cannot move the first camera up select second camera");
        }
      } else {
        AfxMessageBox("Cannot find Cameras Parent");
      }
      UpdateTree(obj->getTrack(), obj);
    } else {
      AfxMessageBox("This Item Cannot be moved!");
    }
  } else {
    AfxMessageBox("No Selection");
  }
}

void CTrackTree::OnExportObject()
{
  // TODO: Add your command handler code he
  HTREEITEM selected = m_TrackTree.GetSelectedItem();

  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);
    if (obj != NULL && obj->isInternalObject()) {
      InternalObject *intobj = (InternalObject *)obj;
      static char lpszObjFilter[] =
        "F1GP2 Object Files (*.gpo)|*.gpo|All Files (*.*)|*.*|";

      char fileName[256];

      strcpy(fileName, "unnamed.gpo");
      CFileDialog *fdlg = new CFileDialog(
        FALSE, "gpo", (LPCSTR)&fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszObjFilter);

      int result = fdlg->DoModal();

      if (result == IDOK) {
        intobj->ExportObject(fdlg->GetFileName());
      }

      delete fdlg;
    }
  } else {
    AfxMessageBox("No Selection");
  }
}

void CTrackTree::OnClose()
{
  // TODO: Add your message handler code here and/or call default
  AfxMessageBox("OnClose");

  CSizingControlBar::OnClose();
}

void CTrackTree::OnDestroy()
{
  CRect rc;
  GetClientRect(rc);

  WR_PROFILE(TreeWidth, Preferences, rc.Width() + 8);
  WR_PROFILE(TreeHeight, Preferences, rc.Height());

  CSizingControlBar::OnDestroy();
}

int CTrackTree::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CSizingControlBar::OnCreate(lpCreateStruct) == -1) return -1;

  CRect rc;
  rc.left = PROFILE(TreeLeft, Preferences, 10);
  rc.right = PROFILE(TreeRight, Preferences, 100);
  rc.top = PROFILE(TreeTop, Preferences, 10);
  rc.bottom = PROFILE(TreeBottom, Preferences, 110);

  int cx = rc.right;
  int cy = rc.right;

  CRect rc2;
  rc2.left = 0;
  rc2.top = 0;
  rc2.right = cx;
  rc2.bottom = cy;

  // TODO: Add your message handler code here
  if (!m_TrackTree.CreateEx(WS_EX_CLIENTEDGE,
        TVS_HASLINES | TVS_HASBUTTONS | TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP | WS_VISIBLE | WS_CHILD | WS_BORDER,
        rc2,
        this,
        IDC_TRACKTREE))
    return -1;

  // OnSize(SIZE_RESTORED,cx,cy);

  // rc.left   = PROFILE(TreeLeft,Preferences,10);
  // rc.right  = PROFILE(TreeRight,Preferences,110);
  // rc.top    = PROFILE(TreeTop,Preferences,10);
  // rc.bottom = PROFILE(TreeBottom,Preferences,110);

  Reposition(rc);

  /*
  CMainFrame *mainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

  if (mainFrame)
  {
    mainFrame->RecalcLayout(TRUE);
  }
  */

  return 0;
}

void CTrackTree::OnSize(UINT nType, int cx, int cy)
{
  CSizingControlBar::OnSize(nType, cx, cy);

  CRect rc;
  GetClientRect(rc);

  Reposition(rc);
}

void CTrackTree::Reposition(CRect rc)
{
  int border = 4;

  int bottom = (IsHorz() || IsFloating()) ? rc.Height() - (2 * border)
                                          : rc.Height() - (2 * border) - 4;

  m_TrackTree.SetWindowPos(NULL, rc.left + border, rc.top + border, rc.Width() - (2 * border), bottom, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CTrackTree::OnRemoveObjectDef()
{
  HTREEITEM selected = m_TrackTree.GetSelectedItem();

  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);
    GPTrack *track = obj->getTrack();

    if (obj != NULL) {
      HTREEITEM node = obj->getTreeNode();
      m_TrackTree.SetItemData(node, NULL);
      Observer *myparent = obj->parent;
      obj->Delete();
      // m_TrackTree.DeleteItem(node);
      UpdateTree(track, myparent);
    }
  } else {
    AfxMessageBox("No Selection");
  }
}

void CTrackTree::OnRemoveInternalObject()
{
  HTREEITEM selected = m_TrackTree.GetSelectedItem();

  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);
    GPTrack *track = obj->getTrack();

    if (obj != NULL) {
      HTREEITEM node = obj->getTreeNode();
      m_TrackTree.SetItemData(node, NULL);
      Observer *myparent = obj->parent;
      m_TrackTree.DeleteItem(obj->getTreeNode());
      obj->Delete();
      // UpdateTree(track,myparent);
    }
  } else {
    AfxMessageBox("No Selection");
  }
}

void CTrackTree::OnRenameJamFile()
{
  // TODO: Add your command handler code he
  HTREEITEM selected = m_TrackTree.GetSelectedItem();

  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);

    if (obj != NULL && obj->isJamFile()) {
      GPTrack *track = obj->getTrack();
      obj->Open();
    } else {
      AfxMessageBox(
        "Which Track? No Selection (select jamfile or tracksection");
    }
  } else {
    AfxMessageBox("No Selection");
  }
}

void CTrackTree::OnPaste()
{
  // AfxMessageBox("Paste currently not implemented");
  // return;
  // TODO: Add your command handler code he
  HTREEITEM selected = m_TrackTree.GetSelectedItem();

  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);
    if (obj != NULL) {
      if (clipboard) {
        obj->InsertNew((Observer *)clipboard);
        UpdateTree(obj->getTrack(), obj);
      } else {
        AfxMessageBox("Clipboard is Empty");
      }
    }
  } else {
    AfxMessageBox("No Selection");
  }
}

void CTrackTree::OnCopy()
{
  HTREEITEM selected = m_TrackTree.GetSelectedItem();

  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);
    if (obj != NULL && obj->isCommand()) {
      clipboard = obj;
    } else {
      clipboard = NULL;
    }
  } else {
    AfxMessageBox("No Selection");
  }
}

void CTrackTree::OnCut()
{
  HTREEITEM selected = m_TrackTree.GetSelectedItem();

  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);
    if (obj != NULL && obj->isCommand()) {
      clipboard = obj->Copy();
      GPTrack *track = obj->getTrack();
      HTREEITEM node = obj->getTreeNode();
      // HTREEITEM parentnode = m_TrackTree.GetParent(node);
      m_TrackTree.SetItemData(node, NULL);
      Observer *myparent = obj->parent;
      m_TrackTree.DeleteItem(node);
      obj->Delete();
      // UpdateTree(track,myparent);
      // Vector *parentList = ((TrackCmd *)obj)->owner->getCommands();
      // if (parentList)
      //{
      //  parentList->removeElementRef((CObject*)obj);
      //}
    } else {
      clipboard = NULL;
    }
  } else {
    AfxMessageBox("No Selection");
  }
}

void CTrackTree::OnRightpopMovetonextsection()
{
  HTREEITEM selected = m_TrackTree.GetSelectedItem();
  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);
    if (obj != NULL && obj->isCommand()) {
      TrackCmd *cmd = (TrackCmd *)obj;
      TrackSection *CommandsSection = cmd->owner;

      if (CommandsSection != NULL) {
        TrackSection *nextSection = CommandsSection->next();
        Vector *cmds = CommandsSection->getCommands();
        Vector *ncmds = nextSection->getCommands();
        if (cmds && cmd && ncmds) {
          cmds->removeElementRef(cmd);
          ncmds->addElement(cmd);
        } else {
          AfxMessageBox("Cannot Move Command?");
        }
      }
      UpdateTree(obj->getTrack(), CommandsSection);
    }
  }
}

void CTrackTree::OnRightpopMovetoprevioussection()
{
  HTREEITEM selected = m_TrackTree.GetSelectedItem();
  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);
    if (obj != NULL && obj->isCommand()) {
      TrackCmd *cmd = (TrackCmd *)obj;
      TrackSection *CommandsSection = cmd->owner;

      if (CommandsSection != NULL) {
        TrackSection *nextSection = CommandsSection->previous();
        Vector *cmds = CommandsSection->getCommands();
        Vector *ncmds = nextSection->getCommands();
        if (cmds && cmd && ncmds) {
          cmds->removeElementRef(cmd);
          cmd->owner = nextSection;
          ncmds->addElement(cmd);
        } else {
          AfxMessageBox("Cannot Move Command?");
        }
      }
      UpdateTree(obj->getTrack(), CommandsSection);
    }
  }
}

void CTrackTree::OnRightpopMoveto()
{
  HTREEITEM selected = m_TrackTree.GetSelectedItem();
  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);
    if (obj != NULL && obj->isCommand()) {
      TrackCmd *cmd = (TrackCmd *)obj;
      TrackSection *CommandsSection = cmd->owner;

      if (CommandsSection != NULL) {
        CMoveTo *dlg = new CMoveTo();
        dlg->m_MoveTo = CommandsSection->getIndex();
        dlg->DoModal();
        int newIndex = dlg->m_MoveTo;

        Vector *sections = CommandsSection->getVector();

        if (newIndex >= 0 && newIndex < sections->size()) {
          TrackSection *nextSection =
            (TrackSection *)sections->elementAt(newIndex);
          Vector *cmds = CommandsSection->getCommands();
          Vector *ncmds = nextSection->getCommands();
          if (cmds && cmd && ncmds) {
            cmds->removeElementRef(cmd);
            cmd->owner = nextSection;
            ncmds->addElement(cmd);
          } else {
            AfxMessageBox("Cannot Move Command?");
          }
        } else {
          AfxMessageBox("Not that many sections");
        }
      }
      UpdateTree(obj->getTrack(), CommandsSection);
    }
  }
}

void CTrackTree::OnUpdateRightpopMoveto(CCmdUI *pCmdUI)
{
  // TODO: Add your command update UI handler code here
}

void CTrackTree::OnUpdateRightpopMovetonextsection(CCmdUI *pCmdUI)
{}

void CTrackTree::OnUpdateRightpopMovetoprevioussection(CCmdUI *pCmdUI)
{
  // TODO: Add your command update UI handler code here
}

void CTrackTree::OnHelpObject()
{
  CString top = "cmdlib.htm#";
  CString context;

  HTREEITEM selected = m_TrackTree.GetSelectedItem();
  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);
    if (obj != NULL && obj->isCommand()) {
      TrackCmd *cmd = (TrackCmd *)obj;
      TrackSection *CommandsSection = cmd->owner;

      int cmdval = cmd->getCmd();

      context.Format("0x%x", cmdval);
      CString url = top + context;
      theApp.OnHelpContentString(url);
      return;
    }
  }
  OnHelp();
}

void CTrackTree::OnReplaceinternalobject()
{
  HTREEITEM selected = m_TrackTree.GetSelectedItem();

  if (selected != NULL) {
    Observer *obj = (Observer *)m_TrackTree.GetItemData(selected);
    if (obj != NULL && obj->isInternalObject()) {
      InternalObject *intobj = (InternalObject *)obj;
      static char lpszObjFilter[] =
        "F1GP2 Object Files (*.gpo)|*.gpo|All Files (*.*)|*.*|";

      char fileName[256];

      strcpy(fileName, "unnamed.gpo");
      CFileDialog *fdlg = new CFileDialog(
        TRUE, "gpo", (LPCSTR)&fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszObjFilter);

      int result = fdlg->DoModal();

      if (result == IDOK) {
        intobj->ReplaceObject(fdlg->GetFileName());
        // adjust objects pointers.....
        ObjectData *dat = (ObjectData *)intobj->objectData->elementAt(2);
        long val = (dat->GetValue() + intobj->track->LargeNumber1.GetValue() + intobj->Offset - 0x1020);
        dat->SetValue(val);

        dat = (ObjectData *)intobj->objectData->elementAt(3);
        val = (dat->GetValue() + intobj->track->LargeNumber1.GetValue() + intobj->Offset - 0x1020);
        dat->SetValue(val);

        dat = (ObjectData *)intobj->objectData->elementAt(4);
        val = (dat->GetValue() + intobj->track->LargeNumber1.GetValue() + intobj->Offset - 0x1020);
        dat->SetValue(val);

        dat = (ObjectData *)intobj->objectData->elementAt(5);
        val = (dat->GetValue() + intobj->track->LargeNumber1.GetValue() + intobj->Offset - 0x1020);
        dat->SetValue(val);

        dat = (ObjectData *)intobj->objectData->elementAt(6);
        val = (dat->GetValue() + intobj->track->LargeNumber1.GetValue() + intobj->Offset - 0x1020);
        dat->SetValue(val);

        dat = (ObjectData *)intobj->objectData->elementAt(7);
        val = (dat->GetValue() + intobj->track->LargeNumber1.GetValue() + intobj->Offset - 0x1020);
        dat->SetValue(val);

        dat = (ObjectData *)intobj->objectData->elementAt(8);
        val = (dat->GetValue() + intobj->track->LargeNumber1.GetValue() + intobj->Offset - 0x1020);
        dat->SetValue(val);

        dat = (ObjectData *)intobj->objectData->elementAt(9);
        val = (dat->GetValue() + intobj->track->LargeNumber1.GetValue() + intobj->Offset - 0x1020);
        dat->SetValue(val);

        dat = (ObjectData *)intobj->objectData->elementAt(10);
        val = (dat->GetValue() + intobj->track->LargeNumber1.GetValue() + intobj->Offset - 0x1020);
        dat->SetValue(val);
      }

      delete fdlg;
    }
  } else {
    AfxMessageBox("No Selection");
  }
}
