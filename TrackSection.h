#include "math.h"

#ifndef _INCLUDED_TRACK_SECTION_H_
#define _INCLUDED_TRACK_SECTION_H_

#include "Display.h"
#include "Polygon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CTrackPropertySheet;
class LinePt;
class CTrackOpenGLView;

//#define WIDTH_TO_TRACKWIDTH(x) (((double)(x))/900.0)
//#define TRACKWIDTH_TO_WIDTH(x) (((double)(x))*900.0)
#define WIDTH_TO_TRACKWIDTH(x) (((double)(x)) / 800.0)
#define TRACKWIDTH_TO_WIDTH(x) (((double)(x)) * 800.0)

#include "TrackObject.h"
#include "Point3D.h"
#include "DataValue.h"

typedef enum {
  t_BothScenery,
  t_LeftScenery,
  t_RightScenery
} e_ScenerySide;

class TrackSection : public Observer
  , public CObject
{
public:
  char *annotation;
  int dataoffset;
  DataValue rs;
  DataValue cb;
  int index;
  DataValue length;
  DataValue angle;
  int angleval;
  DataValue heightDelta;
  double heightIsoDelta;
  int Altitude;
  double TrueAltitude;
  double AltitudeDelta;
  int borderItems;
  DataValue trackToWallWidthRight;
  DataValue trackToWallWidthLeft;
  double startTrackWidthRight;
  double startTrackWidthLeft;
  Vector *owner;
  LinePt *linePt;

  int LookAhead;
  int LookBehind;

  TrackSection(Vector *own, GPTrack *track, int idx, int len, int ang, int dz, int wl, int wr, int rs, int cb, Vector *objs, int offset, double widthLeft, double widthRight, Vector *cmds, ObserverType section_type)
    : owner(own),
      index(idx),
      rs(rs),
      cb(cb),
      startTrackWidthLeft(widthLeft),
      startTrackWidthRight(widthRight),
      Observer(track, section_type, NULL),
      length(len),
      trackToWallWidthRight(wr),
      trackToWallWidthLeft(wl),
      subTree(NULL)
  {
    dataoffset = offset;
    angleval = (int)AngleTo((double)ang);
    angle = AngleToDeg((double)ang);
    heightDelta.SetValue(HeightTo((double)dz));
    linePt = NULL;
    LookAhead = -1;
    LookBehind = -1;
    heightIsoDelta = 0;

    setTreeNode(NULL);

    if (cmds == NULL) {
      TrackCommands = new Vector("TrackCommands", __LINE__);
    } else {
      TrackCommands = cmds;

      // reparent all track cmds
      for (int i = 0; i < TrackCommands->size(); i++) {
        TrackCmd *cmd = (TrackCmd *)TrackCommands->elementAt(i);
        cmd->owner = this;
        cmd->parent = this;
      }
    }

    selected = FALSE;

    trackPolygon = new GPPolygon();
    annotation = NULL;
  }

  Vector *
    getVector()
  {
    return owner;
  }

  virtual ~TrackSection()
  {
    // TRACE("Calling TrackSection::Destructor 0x%x\n",trackPolygon);
    if (trackPolygon) delete trackPolygon;
    if (TrackCommands) delete TrackCommands;
    // if (linePt) delete linePt;
    if (annotation) free(annotation);
  }

  void
    setAnnotation(char *ann);
  char *
    getAnnotation();

  int getIcon(BOOL pitlane = FALSE);

  int getRoadSignData()
  {
    return rs.GetValue();
  }

  void
    setRoadSignData(int val)
  {
    rs.SetValue(val);
  }

  int getKerbData()
  {
    return cb.GetValue();
  }

  void
    setKerbData(int val)
  {
    cb.SetValue(val);
  }

  int write(GPTrack *track, int offset);
  int debug(FILE *fp, GPTrack *track, int offset);

  void
    setLeftToBank(int data)
  {
    if (data > 256) data = 256;
    trackToWallWidthLeft = data;
  }
  void
    setRightToBank(int data)
  {
    if (data > 256) data = 256;
    trackToWallWidthRight = data;
  }
  int getLeftToBank()
  {
    return trackToWallWidthLeft.GetValue();
  }
  int getRightToBank()
  {
    return trackToWallWidthRight.GetValue();
  }
  int getLeftToBankData()
  {
    // return (trackToWallWidthLeft*2)/0x80;
    return trackToWallWidthLeft.GetValue();
  }
  int getRightToBankData()
  {
    // return (trackToWallWidthRight*2)/0x80;
    return trackToWallWidthRight.GetValue();
  }
  void
    setHeight(int data)
  {
    heightDelta = data;
  }

  int getOffset()
  {
    return dataoffset;
  }

  BOOL
    isPitLaneEntrance();
  BOOL
    isPitLaneExit();
  BOOL
    IsRibbonOn(int ribbonid);

  double
    getTrueLength()
  {
    if (getAngle() != 0) {
      double len = getLength();
      double anglelen = len * getAngle();
      double anglerad = DegToRad(anglelen);
      double beta = anglelen / 2.0;
      double inrad = DegToRad(beta);
      double sectorRadius = (len / anglerad);

      return (sectorRadius * anglerad);
    } else {
      return getLength();
    }
  }

  double
    getX(double angleoffset)
  {
    double totalAngle = angleoffset;
    double radangle = DegToRad(totalAngle);
    double xlen = cos(radangle);

    return ((double)(getScaledLength()) * xlen);
  }

  double
    getScaledLength()
  {
    return (double)(2 * length.GetValue());
  }

  double
    getY(double angleoffset)
  {
    double totalAngle = angleoffset;
    double radangle = DegToRad(totalAngle);

    return -((double)(getScaledLength()) * sin(totalAngle));
  }

  double
    getAngle()
  {
    return angle.GetDoubleValue();
  }

  void
    setAngle(double ang)
  {
    angle.SetValue(ang);
  }

  int getAngleInt()
  {
    return angleval;
  }

  int getLength()
  {
    return length.GetValue();
  }

  void
    setLength(int len)
  {
    length.SetValue(len);
  }

  int getHeight()
  {
    return heightDelta.GetValue();
  }

  double
    getIsoHeight()
  {
    return heightIsoDelta;
  }

  int getAltitude()
  {
    return Altitude;
  }

  void
    setAltitude(int alt)
  {
    Altitude = alt;
  }

  double
    getTrueAltitude()
  {
    return TrueAltitude;
  }

  void
    setTrueAltitude(double alt)
  {
    TrueAltitude = alt;
  }

  double
    getAltitudeDelta()
  {
    return AltitudeDelta;
  }

  void
    setTrueAltitudeDelta(double alt)
  {
    AltitudeDelta = alt;
  }

  double
    getStartX()
  {
    return startX;
  }

  void
    setStartX(double X)
  {
    startX = X;
  }

  double
    getStartY()
  {
    return startY;
  }

  double
    getStartZ()
  {
    return startZ;
  }

  void
    setStartZ(double Z)
  {
    startZ = Z;
  }

  void
    setStartY(double Y)
  {
    startY = Y;
  }

  double
    getEndZ()
  {
    return startZ;
  }

  void
    setEndZ(double Z)
  {
    endZ = Z;
  }

  double
    getEndX()
  {
    return endX;
  }

  void
    setEndX(double X)
  {
    endX = X;
  }

  double
    getEndY()
  {
    return endY;
  }

  void
    setEndY(double Y)
  {
    endY = Y;
  }

  double
    getStartAngle()
  {
    return startAngle;
  }

  void
    setStartAngle(double angle)
  {
    startAngle = angle;
  }

  double
    getEndAngle()
  {
    return endAngle;
  }

  void
    setEndAngle(double angle)
  {
    endAngle = angle;
  }

  double
    getRadius()
  {
    return radius;
  }

  void
    setRadius(double ang)
  {
    radius = ang;
  }

  double
    getWidthLeft()
  {
    return startTrackWidthLeft;
  }

  double
    getWidthRight()
  {
    return startTrackWidthRight;
  }

  void
    setWidthLeft(int d)
  {
    startTrackWidthLeft = d;
  }

  void
    setWidthRight(int d)
  {
    startTrackWidthRight = d;
  }

  void
    adjustLengthAngleFromXY()
  {
    double sx = getStartX();
    double sy = getStartY();
    double ex = getEndX();
    double ey = getEndY();

    double dx = ex - sx;
    double dy = ey - sy;

    setLength((int)sqrt(dx * dx + dy * dy));
  }

  void
    drawTrack(Display *g, double angle, GPTrack *track, BOOL drawGray = FALSE);
  void
    HighlightSector(Display *g, GPTrack *track, double distanceIn, char *text);
  LinePt *
    drawTrackLine(Display *g, double angle, GPTrack *track);

  void
    drawSides(Display *g, double angle, GPTrack *track, TrackSection *t1, TrackSection *t2, TrackSection *prev);
  void
    drawTextureSides(Display *g, GPTrack *track);

  Primitive3D *
    drawTrack3D(Display *g, double angle, GPTrack *track, TrackSection *t1, TrackSection *t2);
  void
    drawRoadSign(Display *g);
  void
    drawCommands(Display *g);
  void
    drawTextureCommands(Display *g);
  void
    drawViewDistanceCommands(Display *g);
  void
    drawFlag(Display *g, BOOL right);
  void
    drawScenery(Display *g);
  Model3D *
    drawScenery3D(Display *g);
  void
    drawScenery3DGL(CTrackOpenGLView *view);

  BOOL
    isLeftKerb();
  BOOL
    isRightKerb();
  BOOL
    isLeftWallRemoved();
  BOOL
    isRightWallRemoved();
  BOOL
    isRoadSignLeft();
  BOOL
    isRoadSignRight();
  BOOL
    hasRoadSigns();
  BOOL
    isKerbLower();
  BOOL
    isLeftWallBridged();
  BOOL
    isRightWallBridged();

  void
    setLeftWallBridged(BOOL val);
  void
    setRightWallBridged(BOOL val);

  void
    setLeftKerb(BOOL on);
  void
    setRightKerb(BOOL on);
  void
    setLeftRoadSign(int val);
  void
    setRightRoadSign(int val);

  void
    GetBeginLeftWall(double &leftSideX, double &leftSideY);
  void
    GetBeginRightWall(double &leftSideX, double &leftSideY);
  void
    GetEndLeftWall(double &leftSideX, double &leftSideY);
  void
    GetEndRightWall(double &leftSideX, double &leftSideY);

  void
    CreateRibbons(TrackCmd *scenery, double xpos, double ypos, double zpos, double dist, double angle, double &x1, double &y1, double &z1, double &x2, double &y2, double &z2, double &x3, double &y3, double &z3, double &x4, double &y4, double &z4, double &x5, double &y5, double &z5, double &x6, double &y6, double &z6, double zscale, double LeftAngle, double RightAngle, e_ScenerySide Side);

  void
    AddGravelTrapLeft();
  void
    AddGravelTrapRight();
  void
    AddTyreWallLeft();
  void
    AddTypeWallRight();
  void
    AddTarmacLeft();
  void
    AddTarmacRight();
  void
    AddWallMarkingLeft(int id);
  void
    AddWallMarkingRight(int id);
  void
    AddTrackSideMarkingLeft(int id);
  void
    AddTrackSideMarkingRight(int id);
  void
    AddTrackSideMarking(int offset, int location, int length, int texture, int unk1, int unk2, int unk3, int rotation);
  void
    AddBlackFlagRight(int speed);
  void
    AddBlackFlagLeft(int speed);

  void
    setSelected(BOOL val)
  {
    selected = val;
  }

  BOOL
    isSelected()
  {
    return selected;
  }

  BOOL
    contains(CPoint pt)
  {
    return trackPolygon->contains(pt);
  }

  void
    drawStartingLine(Display *g);

  void
    drawObjects(Display *g, double x, double y, double stAngle, int showBitmaps)
  {
    Vector *TrackSideObjects = getObjects();
    for (int i = 0; i < TrackSideObjects->size(); i++) {
      TrackObject *t = (TrackObject *)TrackSideObjects->elementAt(i);
      if (t != NULL) {
        t->draw(g, x, y, stAngle, getWidthLeft(), showBitmaps, getAngle());
      }
    }
    // this could be dangerous
    if (TrackSideObjects) delete (TrackSideObjects);
  }

  void
    drawWalls(Display *g, double angle, GPTrack *track, TrackSection *t1, TrackSection *t2);

  void
    RemoveObject(TrackObject *obj);
  void
    RemoveCommand(TrackCmd *cmd);

  TrackCmd *
    getCommand(int cmd);
  Vector *
    getCommands(int cmd);
  void
    RemoveCommand(int cmd);
  void
    InsertCommand(GPTrack *track, int cmd);

  BOOL
    containsCommand(int cmd)
  {
    if (getCommand(cmd) != NULL)
      return TRUE;
    else
      return FALSE;
  }

  int numCommands(int cmd)
  {
    Vector *cmds = getCommands(cmd);
    if (cmds)
      return cmds->size();
    else
      return 0;
  }

  TrackCmd *
    NthCommandOccurance(int cmd, int occurance)
  {
    Vector *cmds = getCommands(cmd);

    if (cmds && cmds->size()) {
      int found = 0;
      for (int i = 0; i < cmds->size(); i++) {
        TrackCmd *ca = (TrackCmd *)cmds->elementAt(i);
        int cmdval = ca->getCmd();
        if (cmdval == cmd) found++;
        if (occurance == found) return ca;
      }
    }
    return NULL;
  }

  int GetNthOccurance(TrackCmd *cmd)
  {
    Vector *cmds = getCommands();
    int cmdval = cmd->getCmd();

    if (cmds && cmds->size()) {
      int found = 0;
      for (int i = 0; i < cmds->size(); i++) {
        TrackCmd *ca = (TrackCmd *)cmds->elementAt(i);
        if (cmdval == ca->getCmd()) found++;
        if (cmd == ca) return found;
      }
    }
    return 0;
  }

  void
    drawGrip(Display *g, double angle, GPTrack *track);

  double startX;
  double startY;
  double endX;
  double endY;
  double startAngle;
  double endAngle;
  double radius;
  double startZ;
  double endZ;
  int gripLevel;

  int getGripLevel()
  {
    return gripLevel;
  }

  void
    setGripLevel(int grip)
  {
    gripLevel = grip;
  }

  int getIndex()
  {
    return index;
  }

  void
    setIndex(int idx)
  {
    index = idx;
  }

  // HTREEITEM treeNode;

  BOOL selected;

  Vector *
    getCommands()
  {
    return TrackCommands;
  }

  void
    addCommand(TrackCmd *cmd)
  {
    cmd->parent = this;
    TrackCommands->addElement(cmd);
  }

  BOOL
    isInside(CPoint begin, CPoint end);

  Vector *
    getCommandIterator();

  void
    InsertNew();
  void
    InsertNew(Observer *obj);
  void
    InsertCmdAt(int idx, int cmd);

  BOOL
    isMultiVariable()
  {
    return TRUE;
  }

  void
    LoadPropertyTable(CTrackPropertySheet *table);

  TrackSection *
    next()
  {
    return track->GetNextSection(this);
  }

  TrackSection *
    previous()
  {
    return track->GetPreviousSection(this);
  }

  int FirstSign(BOOL turnLeft);
  int SecondSign(BOOL turnLeft);
  int ThirdSign(BOOL turnLeft);

  CPoint
    TrackNextPoint(double xpos, double ypos, double angle, double stAnge, int inDistance);

  void
    Selected();
  int Open();

  Vector *
    getObjects();
  Vector *TrackCommands;
  GPPolygon *trackPolygon;

  HTREEITEM
  LoadTree(CTrackTree *mainTree, HTREEITEM HGP2Track, int index, BOOL TrackOrPit);
  void
    Expand(CTrackTree *tracktree, HTREEITEM node);
  HTREEITEM subTree;
};

#endif