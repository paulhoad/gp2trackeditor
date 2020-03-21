// GPTrack.h : header file
//

#ifndef _GP_TRACK_H_
#define _GP_TRACK_H_

#include "DataValue.h"
#include "Point3D.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

double
  AngleToDeg(double angle);
double
  ConvertDeg(double angle);
double
  AngleTo(double angle);

void RadiusFrom(double &radius, int &direction);
int AngleFrom(double angle);
double
  HeightTo(double height);
double
  DegToRad(double angle);
double
  RadToDeg(double angle);
double
  getLeftSideX(double angle, double length);
double
  getLeftSideY(double angle, double length);
double
  getRightSideX(double angle, double length);
double
  getRightSideY(double angle, double length);

#define TRACK_START_POSX 300
#define TRACK_START_POSY 300

#define TEN_METERS 0
#define FIFTY_METERS 1
#define ONE_HUNDRED_METERS 2
#define FIVE_HUNDRED_METERS 3
#define ONE_KM 4
#define HALF_MILE 5
#define ONE_MILE 6

#include "Vector.h"

class TrackObject;
class TrackSection;
class TrackCmd;
class JAM;

//#include "TrackSection.h"
#include "TrackObjectDefinition.h"
#include "CCLineSection.h"
#include "Camera.h"
#include "Display.h"

class GPTrack;
class Camera;

#include "CCCarSetup.h"

#define WIDTH_TO_CCWIDTH(x) ((((double)(x)) * 2.0) / 1024.0)

/////////////////////////////////////////////////////////////////////////////
// GPTrack window

class GPTrack : public CWnd
{
  // Construction
public:
  GPTrack(BOOL create = TRUE);
  ~GPTrack();

  // Attributes
public:
  // Operations
public:
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(GPTrack)
  //}}AFX_VIRTUAL

  // Implementatiom
  CDocument *document;

public:
  // virtual ~GPTrack();

  void
    Create();

  CDocument *
    GetDocument()
  {
    return document;
  }

  void
    SetDocument(CDocument *doc)
  {
    document = doc;
  }

  BOOL
    isF1GP();

  void
    WriteInfo();
  void
    ReadInfo(BOOLEAN complainNoGPInfo = TRUE);
  void
    ReadTrackFile(CDocument *pDoc, const char *filename);
  int ReadTrackInfoFile(const char *filename);
  void
    WriteTrackFile(const char *filename);
  void
    ParseTrack(CDocument *pDoc);
  void
    ReadOffsets(CDocument *pDoc);
  int ReadTrack(int offset);
  int ReadPitLane(int offset);
  int ReadCCLine(int offset);
  int ReadCameras(int start);
  int ReadSetup(int offset);
  int ReadJamFiles(int start);
  char *
    ReadNullTerminatedString(int start);
  void
    WriteNullTermintedString(int start, char *file, int len);
  void
    createTrack(CDocument *pDoc);
  void
    createPitlane();
  void
    createCC();
  void
    drawTrack(Display *g, BOOL drawGray = FALSE);
  void
    drawPitlane(Display *g);
  void
    drawCCLine(Display *g);
  void
    drawCameras(Display *g);
  void
    drawBlackFlags(Display *g);
  void
    drawPitCmds(Display *g);
  void
    drawTrackMarkings(Display *g);
  void
    drawVarious(Display *g);
  void
    CreateVRMLFile(char *filename);

  Vector *
    getSelectedSections(CPoint begin, CPoint end);

  BOOL regenerate;

  double
    LengthToTrackSector(TrackSection *in);
  TrackSection *
    SectorForLength(double len);
  double
    LengthToPitSectorEnd(TrackSection *in);

  int ReadVariousData(int start);
  int WriteVariousData(int start);
  DataValue VariousData[256];
  int VariousDataCount;

  void
    ScaleTrack(float scale);

  int ReadPCDWord(int offset);
  int ReadPCByte(int offset);
  int ReadPCWord(int offset);
  void
    WritePCWord(int offset, int value);
  void
    WritePCDWord(int offset, int value);
  void
    WritePCByte(int offset, int value);

  void
    RecreateData();
  int WriteOffsets();
  int WriteTrackObjectDefinitions(int offset);
  int WriteTrack(int offset);
  int WritePitLane(int offset);
  int WriteCCLine(int offset);
  int WriteCameras(int offset);
  int WriteSetup(int offset);
  int WriteJamFiles(int offset);
  int WriteObjNames(FILE *fp);

  void
    NewTrack();

  CString TrackFilename;
  void
    setFileName(CString name)
  {
    TrackFilename = name;
  }

  CString
    getFileName()
  {
    return TrackFilename;
  }

  void
    GetTrackObjectDefinition(int start);
  int GetTrackWidth(Vector *SectionCmds, int arg1, int offset);
  int GetTrackWidthLeft(Vector *SectionCmds, int arg1, int offset, BOOL left, int cmd);
  int GetTrackGrip(Vector *SectionCmds, int arg1, int offset, int cmd);
  void
    ReadTrackObjectDefinitions(int start, int end);
  TrackObjectDefinition *
    getObjectDefinition(int code);

  TrackObjectDefinition *
    FindObjectDefinition(int Offset);
  TrackObjectDefinition *
    AddNewTrackObjectDefinition();
  TrackSection *
    GetTrackSection(Vector *vec, int idx);

  void
    drawGrid(Display *g);

  Model3D *
    createSceneryModel(Display *g);

  BOOL showTrack;
  BOOL showWalls;
  BOOL showPitLane;
  BOOL showObjects;
  BOOL showCCLine;
  BOOL showHiddenAsGray;
  BOOL showTrackPie;
  BOOL showCameras;
  BOOL showBitmaps;
  BOOL showOutlines;
  BOOL showGreenScreen;
  BOOL showTrackSide;
  BOOL showFinishLine;
  BOOL showFilledObjects;
  BOOL showRemovedWalls;
  BOOL showRoadSigns;
  BOOL showCommands;
  BOOL showCenterLine;
  BOOL showKerbs;
  BOOL showGrip;
  BOOL showBlackFlags;
  BOOL showScenery;
  BOOL showGridLines;
  BOOL showTrackNumbers;
  BOOL showCCNumbers;
  BOOL showUseTextures;
  BOOL showPitCmds;
  BOOL showTrackDistances;
  BOOL showUnkVariousTable;
  BOOL showCurvedWallStraight;
  BOOL showObjectPositionMarker;
  BOOL maintainBitmapAspectRatio;
  BOOL showGraphPaper;
  BOOL showMarkings;
  BOOL showAnnotations;
  BOOL showDefaultSides;
  BOOL showCCDataLog;
  BOOL showSplits;
  BOOL showTextures;
  BOOL showPitWalls;
  BOOL showUnderlayBitmap;
  BOOL showAxis;
  BOOL showPointNumbers;
  BOOL showVertexNumbers;
  BOOL showPolygonNumbers;
  BOOL showLines;
  BOOL showPoints;
  BOOL showCulling;
  BOOL showNormals;
  BOOL showExternalObjects;
  BOOL showCameraViewDistances;
  BOOL showTextureMappingCommands;
  BOOL showViewDistanceCommands;
  BOOL showJamPreviews;
  BOOL showViewDistances;
  BOOL useSwivelAngles;
  BOOL showSceneryArms;
  BOOL showToolbar;

  int gridSizeOption;

  DataValue UnderLayBitmapX;
  DataValue UnderLayBitmapY;
  DataValue UnderLayBitmapWidth;
  DataValue UnderLayBitmapHeight;

  void
    ShowHideAll(BOOL state);

  unsigned char trackdata[65535];
  int fileLength;
  int TrackDataOffset;
  int TrackPitLaneStart;
  int ListObjectOffset;
  int NumberOfObjects;
  int BlockBoffset;
  int CheckSumOffset;
  int CameraOffset;
  int SetupOffset;
  int JamFileWritingStartOffset;
  int JamZeroCount;
  int split1;
  int split2;
  char trackName[128];
  CString circuitName;
  CString countryName;
  CString authorName;
  CString circuitEvent;
  CString circuitYear;
  CString circuitDesc;
  CString circuitCopyright;
  int circuitLaps;
  BOOL circuitReal;
  int circuitSlot;
  int circuitTyreConsumption;
  CString lapRecord;
  CString lapRecordQualify;

  int DebugTrack(FILE *fp, int offset);
  int GP2BackupNumber;
  int GP2MaxBackupNumber;

  int getBestSlotInfo()
  {
    return circuitSlot;
  }

  int getTyreConsumption()
  {
    return circuitTyreConsumption;
  }

  void
    setBestSlotInfo(int slot)
  {
    circuitSlot = slot;
  }

  void
    setTyreConsumption(int tyre)
  {
    circuitTyreConsumption = tyre;
  }

  CString
    getLapRecord()
  {
    return lapRecord;
  }

  CString
    getLapRecordQualify()
  {
    return lapRecordQualify;
  }

  void
    setLapRecordQualify(CString str)
  {
    lapRecordQualify = CString(str);
  }

  void
    setLapRecord(CString str)
  {
    lapRecord = CString(str);
  }

  CString
    getCircuitName()
  {
    return circuitName;
  }

  CString
    getCircuitCopyright()
  {
    return circuitCopyright;
  }

  CString
    getCircuitCountry()
  {
    return countryName;
  }

  CString
    getCircuitYears()
  {
    return circuitYear;
  }

  CString
    getCircuitAuthor()
  {
    return authorName;
  }

  CString
    getCircuitEvent()
  {
    return circuitEvent;
  }

  CString
    getCircuitDesc()
  {
    return circuitDesc;
  }

  int getCircuitLaps()
  {
    return circuitLaps;
  }

  char *
    getCircuitLapsStr()
  {
    char *laps = (char *)malloc(128 * sizeof(char));
    wsprintf(laps, "%d", circuitLaps);
    return laps;
  }

  char *
    getTrackSectionsStr()
  {
    char *buffer = (char *)malloc(128 * sizeof(char));

    wsprintf(buffer, "%d", TrackSections->size());
    return buffer;
  }
  char *
    getPitSectionsStr()
  {
    char *buffer = (char *)malloc(128);

    wsprintf(buffer, "%d", PitlaneSections->size());
    return buffer;
  }
  char *
    getCCSectionsStr()
  {
    char *buffer = (char *)malloc(128);

    wsprintf(buffer, "%d", CCLineSections->size());
    return buffer;
  }
  char *
    getObjectDefinitionsStr()
  {
    char *buffer = (char *)malloc(128);

    wsprintf(buffer, "%d", TrackObjectDefintions->size());
    return buffer;
  }
  char *
    getCamerasStr()
  {
    char *buffer = (char *)malloc(128);

    wsprintf(buffer, "%d", TrackCameras->size());
    return buffer;
  }

  BOOL
    getCircuitReal()
  {
    return circuitReal;
  }

  void
    setCircuitName(CString name)
  {
    // if (circuitName) delete circuitName;
    circuitName = CString(name);
  }
  void
    setCircuitCopyright(CString name)
  {
    // if (circuitCopyright) delete circuitCopyright;
    circuitCopyright = CString(name);
  }
  void
    setCircuitCountry(CString name)
  {
    // if (countryName) delete countryName;
    countryName = CString(name);
  }
  void
    setCircuitYears(CString name)
  {
    // if (circuitYear) delete circuitYear;
    circuitYear = CString(name);
  }
  void
    setCircuitAuthor(CString name)
  {
    // if (authorName) delete authorName;
    authorName = CString(name);
  }
  void
    setCircuitEvent(CString name)
  {
    // if (circuitEvent) delete circuitEvent;
    circuitEvent = CString(name);
  }
  void
    setCircuitDesc(CString name)
  {
    // if (circuitDesc) delete circuitDesc;
    circuitDesc = CString(name);
  }
  void
    setCircuitReal(BOOL real)
  {
    circuitReal = real;
  }
  void
    setCircuitLaps(int laps)
  {
    circuitLaps = laps;
  }

  TrackSection *
    GetTrackSection(int idx);
  int GetTrackSectionIndex(TrackSection *t);
  TrackSection *
    GetNextSection(TrackSection *t);
  TrackSection *
    GetPreviousSection(TrackSection *t);

  CCLineSection *
    GetCCSection(int idx);
  int GetCCSectionIndex(CCLineSection *t);
  CCLineSection *
    GetNextSection(CCLineSection *t);
  CCLineSection *
    GetPreviousSection(CCLineSection *t);
  Camera *
    GetPreviousCamera(Camera *t);
  TrackSection *
    FindTrackCommand(int cmd, int pos = 0);

  BOOL
    PitLaneEntranceToLeft();
  BOOL
    PitLaneExitToLeft();

  void
    drawTrackLines(Display *g);
  void
    drawPitLines(Display *g);
  void
    drawCCLines(Display *g);

  DataValue TrackStartAngle;
  DataValue TrackStartClimb;
  DataValue TrackStartX;
  DataValue TrackStartZ;
  DataValue TrackStartY;
  DataValue TrackBeginWidth;
  // DataValue Unknown;
  DataValue BeginTrackWidthLeft;
  DataValue BeginTrackWidthRight;
  int PitLaneEntryIndex;
  int PitLaneExitIndex;
  DataValue LargeNumber1;
  DataValue LargeNumber2;
  int JamFileWritingOffset;

  int TrackWidthLeft;
  int TrackWidthRight;

  DataValue TrackUnknown1;
  DataValue C5Code;
  DataValue UnknownNumber;
  int UnknownNumberCount;
  DataValue UnknownNumbers[256];
  DataValue JamFileUnknown;

  BOOL
    isPitLaneLeftSide();

  DataValue CCLineStartX;
  DataValue CCLineStartY;
  DataValue CCLineStartXHigh;
  DataValue CCUnknown1;
  DataValue CCUnknown2;
  DataValue CCUnknown3;
  DataValue CCUnknown4;
  int CCLineToSetupGap;
  int TrackXBeginPos;
  int TrackYBeginPos;
  DataValue CCLineStartYData;

  BOOL
    FindTopLeft();

  BOOL
    isCircuitDefined();
  BOOL
    isCountryDefined();

  CView *view;

  CView *
    getView()
  {
    return view;
  }

  void
    setView(CView *myview)
  {
    view = myview;
  }

  Vector *TrackSections;
  Vector *PitlaneSections;
  Vector *TrackObjectDefintions;
  Vector *CCLineSections;
  Vector *TrackCameras;
  Vector *JamFiles;
  Vector *TrackObjectDescriptions;
  Vector *InternalObjectOffsets;
  Vector *InternalObjectNames;

  HTREEITEM TrackRootNode;

  BOOL valid;

  BOOL
    isValid()
  {
    return (valid > 0) ? 1 : 0;
  }

  void
    setValid(BOOL val)
  {
    valid = val;
  }

  void
    MakeBackupNow();

  int getTrackAngle()
  {
    return TrackStartAngle.GetValue();
  }

  int getTrackHeight()
  {
    return TrackStartClimb.GetValue();
  }

  const CString
    getTrackName()
  {
    if (trackName == NULL)
      return CString("[None Loaded]");
    else
      return CString(trackName);
  }

  void
    setTrackName(const char *name)
  {
    strcpy(trackName, name);
  }

  int GetNWordUnknownCommand(Vector *sectorCmds, int cmd, int arg1, int offset, int num);
  TrackObject *
    GetTrackObject(int offset, int len, Vector *sectorCmds);
  int GetTrackSection(int offset, int length, Vector *objects, double widthLeft, double widthRight, Vector *sectionsCmds, int gripLevel);
  int GetPitLaneSection(int offset, int length, Vector *objects, double widthLeft, double widthRight, Vector *sectionsCmds, int gripLevel);
  Camera *
    GetCamera(int offset, int num);
  // int PutTrackSection(TrackSection *t);
  // int PutPitLaneSection(TrackSection *t);
  int PutCCSection(CCLineSection *t);

  void
    selectedSection(CPoint pt);
  void
    selectedPitSection(CPoint pt);
  void
    selectedCCSection(CPoint pt);
  TrackSection *
    getTrackSelection();
  TrackSection *
    getPitLaneSelection();
  CCLineSection *
    getCCSelection();

  int getTrackSelectionIndex();
  int getPitSelectionIndex();
  int getCCSelectionIndex();
  int getObjectSelectionIndex();
  void
    clearTrackSelections();
  void
    clearPitSelections();
  void
    setTrackSelection(TrackSection *t);
  void
    setPitSelection(TrackSection *t);
  void
    setCCSelection(CCLineSection *t);

  TrackObject *
    selectedObject(CPoint pt);
  TrackObject *
    GetSelectedObject();

  Camera *
    selectedCamera(CPoint pt);
  Camera *
    GetSelectedCamera();
  void
    clearSelectedCameras();
  void
    setCameraSelection(Camera *cam);

  void
    DrawSplits(Display *display);

  void
    CalculateCheckSum()
  {
    ResetSum();
    for (int i = 0; i < fileLength - 4; i++) {
      AddSumValue(trackdata[i]);
    }
  }

  BOOL
    isRibbonOn(int ts, int myribbon);

  int totalCommands();
  int totalTrackCommands();
  int totalPitCommands();
  int totalPitLength();
  int totalLength();
  int MaxStraightLength();
  int MinStraightLength();
  int MaxCurveLength();
  int MinCurveLength();

  void
    RemoveAllCmds(int cmdval, int startingAt = 0, int finishinAt = -1);
  void
    ZeroAllArgs(int cmdval);
  void
    DefaultAllArgs(int cmdval);
  void
    RemoveAllObjects();
  void
    RemoveAllPitCmds();
  void
    RemoveAllPitObjects();

  void
    clearCCSelection();

  void
    RemoveCommand(TrackCmd *cmd);

  int DefaultLeftFloorBrush;
  int DefaultRightFloorBrush;
  int DefaultRoadBrush;
  int DefaultRibbon11Brush;
  int DefaultRibbon12Brush;
  int DefaultRibbon13Brush;
  int DefaultRibbon14Brush;
  int DefaultRibbon15Brush;
  int DefaultRibbon16Brush;
  int DefaultTyreWallBrush;
  int DefaultGravelBrush;
  int DefaultTarmacBrush;

  int DefaultLeftVergeId;
  int DefaultRightVergeId;
  int DefaultRoadId;
  int DefaultLeftFenceId;
  int DefaultRightFenceId;

  void
    CheckDefaultSetting();

  double
    LengthToTrackSectorEnd(TrackSection *in);

  JAM *
    GetJAM(int id);
  char *
    GetJAMFile(int id);

  void
    WriteSum(int offset, BOOL write, BOOL dialog)
  {
    char mess[256];

    unsigned short low1 = firstCheckSum & 0x00FF;
    unsigned short high1 = (firstCheckSum & 0xFF00) >> 8;

    if (write) WritePCByte(offset, low1);
    if (write) WritePCByte(offset + 1, high1);

    unsigned short low2 = secondCheckSum & 0x00FF;
    unsigned short high2 = (secondCheckSum & 0xFF00) >> 8;

    if (write) WritePCByte(offset + 2, low2);
    if (write) WritePCByte(offset + 3, high2);

    wsprintf(mess, "%x %x %x %x %x %x", firstCheckSum, secondCheckSum, low1, high1, low2, high2);

    if (dialog) AfxMessageBox(mess, MB_OK);
  }

  void
    ResetSum()
  {
    firstCheckSum = 0;
    secondCheckSum = 0;
  }

  void
    AddSumValue(int da)
  {
    int d1, d2, d3;// temporary variables
    d2 = secondCheckSum;// copy secondChecksum into d2
    d1 = da;// make sure da is an int (which it is!)
    firstCheckSum += d1;// add d1 to first check sum
    d1 = d1 & 0x000000FF;// only look at the bottom 8 bytes of d1 so d1 will
      // be 0-255;
    d3 = d2 & 0x0000E000;// and with 0E00 locaical AND
    d3 = d3 >> 13;// shift right 13 places similar to divide by 2^13
    d2 = d2 << 3;// shift left 3 places similar to times by 2^3
    d2 = d2 | d3;// d2 = d2 OR d3
    d2 = d2 + d1;// d2 = d2 plus d1
    secondCheckSum = d2;// second Sum is d2
  }

  short firstCheckSum;
  short secondCheckSum;

  void
    GuessTrackIdenty(BOOL complain = TRUE);

  CCCarsetup *carsetup;

  void
    ExportTrackCommands(LPCSTR filename);

  void
    ImportNewInternalObject(CDocument *pDoc, LPCSTR filename);

  void
    ExportCleverNames();

  double CCScaleFactor;

  void
    WriteAnnotations();
  void
    ReadAnnotations();

  double pitLeftWallBeginX;
  double pitRightWallBeginX;
  double pitLeftWallEndX;
  double pitRightWallEndX;
  double trackWalltoPitLeftWallBeginX;
  double trackWalltoPitRightWallBeginX;
  double trackWalltoPitLeftWallEndX;
  double trackWalltoPitRightWallEndX;

  double pitLeftWallBeginY;
  double pitRightWallBeginY;
  double pitLeftWallEndY;
  double pitRightWallEndY;
  double trackWalltoPitLeftWallBeginY;
  double trackWalltoPitRightWallBeginY;
  double trackWalltoPitLeftWallEndY;
  double trackWalltoPitRightWallEndY;

  void
    drawPitToTrackConnections(Display *g);

  void
    ImportCCDataLog(LPCSTR filename);
  void
    ImportTrackSegmentsDataLog(LPCSTR filename);
  void
    ExportXYDataLog(LPCSTR filename);

  void
    DrawCCDataLog(Display *g);
  Vector *CCDataLog;

  void
    ReadObjNames();

  BOOL showReadObjNames;

  void
    Insert112CmdSection();

  Camera *
    DistanceToCamera(double distance);

  void
    DrawObjectOutline(Display *g, TrackSection *t);

  // Generated message map functions
protected:
  //{{AFX_MSG(GPTrack)
  // NOTE - the ClassWizard will add and remove member functions here.
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

double
  RadiusTo(GPTrack *track, double radius, double direction, double arg1, int cmd, double arg3, double arg4, double &radius2);
int RadiusVal(GPTrack *track, double rad, double direction, double arg1, int cmd, double arg3, double arg4, double &radius2);

/////////////////////////////////////////////////////////////////////////////

#endif
