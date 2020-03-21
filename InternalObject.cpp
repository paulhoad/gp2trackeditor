
#include "stdafx.h"
#include "InternalObject.h"
#include "TrackEditor.h"
#include "InternalObjectCmd.h"
#include "TextureChooser.h"
#include "DataChangeObserver.h"
#include "SingleVarObserver.h"
#include "TrackSection.h"
#include "JAM.h"
#include "SubObject.h"

#include "sys/stat.h"

#include "Vertex.h"

#ifdef USE_OPEN_GL
#include <gl\gl.h>
#include <gl\glu.h>
#endif

extern void
  Mak256SquareImage(CBitmap &jamBitmap, CBitmap &currentImg, int nWidth, int nHeight, int hr, int vr, int ow, int oh);
extern GLvoid
  CalculateVectorNormal(GLdouble fVert1[], GLdouble fVert2[], GLdouble fVert3[], GLdouble *fNormalX, GLdouble *fNormalY, GLdouble *fNormalZ);

extern CTrackEditorApp theApp;

InternalObject::~InternalObject()
{
  delete objectData;
  if (polygonPoints) delete polygonPoints;
  if (objectName) free(objectName);
  if (objectLowerName) delete objectLowerName;

  if (ptArray != NULL) {
    for (int i = 0; i < Verticies; i++) {
      delete ptArray[i];
    }
    free(ptArray);
    ptArray = NULL;
  }

  if (VerticiesArray) delete VerticiesArray;

  if (treeItemData) delete treeItemData;
}

int GetFileLength(FILE *fp)
{
  struct _stat buf;
  int result;

  /* Get data associated with "stat.c": */
  result = _fstat(_fileno(fp), &buf);

  /* Check if statistics are valid: */
  return (int)(buf.st_size);
}

void InternalObjectCmd::LoadPropertyTable(CTrackPropertySheet *table)
{
  // AfxMessageBox("Load Property Table for InteralObjectCmd");
  int numArgs = getCmdNumArgs();

  // CString cmdName  = getCmdDescription();

  for (int i = 1; i <= numArgs; i++) {
    CString buffer;

    int data = getNthArg(i);
    DataValue *dataPtr = getNthArgDataValue(i);
    // CString desc = getCmdNthArgDescription(i);
    CString desc = "Unk";
    switch (i) {
    case 1:
      desc = "Unk";
      break;
    case 2:
      desc = "Texture Id";
      break;
    case 3:
      desc = "Cmd";
      break;
    case 4:
      desc = "Texture Bending?";
      break;
    case 5:
      desc = "ncolumns of Texture";
      break;
    case 6:
      desc = "nrows of Texture";
      break;
    case 7:
      desc = "Unk";
      break;
    case 8:
      desc = "Unk";
      break;
    case 9:
      desc = "Unk";
      break;
    }

    char argStr[15];

    wsprintf(argStr, "Arg %d", i);

    DataChangeObserver *observer = new DataChangeObserver(
      getTrack(), dataPtr, t_INT, argStr, (LPSTR)(LPCTSTR)desc);

    buffer.Format("Cmd Arg (%d)", i);
    table->InsertTableString(i - 1, 0, (LPSTR)(LPCSTR)buffer, (LPARAM)observer);
    buffer.Format("%d", data);
    table->InsertTableString(i - 1, 1, (LPSTR)(LPCSTR)buffer, (LPARAM)observer);
    table->InsertTableString(i - 1, 2, (LPSTR)(LPCTSTR)desc, (LPARAM)observer);
    table->m_TrackProperty.SetItemData(i - 1, (LPARAM)observer);
  }
}

void InternalObject::Expand(CTrackTree *tracktree, HTREEITEM node)
{
  if (tracktree && SubTree) {
    HTREEITEM itemToDelete = SubTree;

    while (itemToDelete) {
      HTREEITEM next = tracktree->getTree()->GetNextSiblingItem(itemToDelete);
      tracktree->getTree()->DeleteItem(itemToDelete);
      itemToDelete = next;
    }
  }
  LoadTreeTop(tracktree, node);
  // LoadTree(tracktree,SubTree);
}

void InternalObject::UpdateStructure()
{
  Expand(theApp.getMainTree(), getTreeNode());
}

int InternalObject::Open()
{
  // AfxMessageBox("Internal Object Editor");

  CObjectView *view = theApp.OnInternalObjectView(GetDocument());
  view->SetObject(this);
  view->UpdateInfo(FALSE);
  return IDOK;
}

int InternalObjectCmd::Open()
{
  CTextureChooser *tc = new CTextureChooser(track);

  int textureid = getNthArg(2);
  tc->setInfo(0, 0, 0, textureid, 0, 0, 0, 0);
  int result = tc->DoModal();
  return 0;
}

#define OFFSET_NODE(x, y)                                                   \
  dat = (ObjectData *)objectData->elementAt(y);                             \
  pos = dat->GetValue() - track->LargeNumber1.GetValue() - Offset + 0x1020; \
  buffer.Format("%s %d", x, pos);                                           \
  tree->insertInfoNode(offNode, buffer, TO_ID(IDB_OFFSET));

int InternalObject::getXYZOffset()
{
  ObjectData *dat = (ObjectData *)objectData->elementAt(5);
  int pos = dat->GetValue() - track->LargeNumber1.GetValue() - Offset + 0x1020;
  return pos;
}

int InternalObject::getVertexOffset()
{
  ObjectData *dat = (ObjectData *)objectData->elementAt(6);
  int pos = dat->GetValue() - track->LargeNumber1.GetValue() - Offset + 0x1020;
  return pos;
}

int InternalObject::getScaleOffset()
{
  ObjectData *dat = (ObjectData *)objectData->elementAt(2);
  int pos = dat->GetValue() - track->LargeNumber1.GetValue() - Offset + 0x1020;
  return pos;
}

int InternalObject::getScaleEndOffset()
{
  ObjectData *dat = (ObjectData *)objectData->elementAt(3);
  int pos = dat->GetValue() - track->LargeNumber1.GetValue() - Offset + 0x1020;
  return pos;
}

int InternalObject::getTextureOffset()
{
  ObjectData *dat = (ObjectData *)objectData->elementAt(4);
  int pos = dat->GetValue() - track->LargeNumber1.GetValue() - Offset + 0x1020;
  return pos;
}

int InternalObject::getTextureEndOffset()
{
  ObjectData *dat = (ObjectData *)objectData->elementAt(7);
  int pos = dat->GetValue() - track->LargeNumber1.GetValue() - Offset + 0x1020;
  return pos;
}

int InternalObject::getEndVertexOffset()
{
  ObjectData *dat = (ObjectData *)objectData->elementAt(8);
  int pos = dat->GetValue() - track->LargeNumber1.GetValue() - Offset + 0x1020;
  return pos;
}

int InternalObject::getEndOffset()
{
  ObjectData *dat = (ObjectData *)objectData->elementAt(9);
  int pos = dat->GetValue() - track->LargeNumber1.GetValue() - Offset + 0x1020;
  return pos;
}

int InternalObject::getNumVertexConnections()
{
  return (getEndVertexOffset() - getVertexOffset()) / 4;
}

int InternalObject::getListOffset(int pos)
{
  if (pos >= 78) {
    // 28 == 78
    int npos = pos - 78;
    int nindex = (npos / 2);
    return 28 + nindex;
  }

  return pos;
}

#define GET_INT(x) ((ObjectData *)objectData->elementAt(x))->GetValue();
#define GET_OBJ(x) ((ObjectData *)objectData->elementAt(x));

void InternalObject::LoadTreeTop(CTrackTree *tree, HTREEITEM node)
{
  if (treeItemData) delete treeItemData;
  treeItemData = new Vector("ItemData", __LINE__);

  CString buffer;

  // SubTree = tree->insertInfoNode(node,CString("Textures
  // (Old)"),TO_ID(IDB_JAMFILE));

  HTREEITEM offNode = SubTree =
    tree->insertInfoNode(node, CString("Offsets"), TO_ID(IDB_OFFSET));
  int pos = 0;
  ObjectData *dat;

  // ObjectData *dat = (ObjectData*)objectData->elementAt(2); dat->GetValue()-
  // track->LargeNumber1.GetValue()-Offset+0x1020; buffer.Format("Scale Section
  // %d",pos); tree->insertInfoNode(offNode,buffer,TO_ID(IDB_OFFSET));
  OFFSET_NODE("Scale Section", 2);
  OFFSET_NODE("Unk 1 Section", 3);

  OFFSET_NODE("Texture Section", 4);
  OFFSET_NODE("Point Coordinates Section", 5);
  OFFSET_NODE("Vertex List Section", 6);
  OFFSET_NODE("End of Texture List", 7);
  OFFSET_NODE("End Sections", 8);
  OFFSET_NODE("End of Object File Sections", 9);

  int idx = getListOffset(getScaleOffset());
  int idxend = getListOffset(getScaleEndOffset());

  int numScale = idxend - idx;
  buffer.Format("Scale Data [%d]", numScale);
  HTREEITEM scaldata = tree->insertInfoNode(node, buffer, TO_ID(IDB_SCALE));

  for (int i = 0; i < numScale; i++) {
    CString buffer;

    ObjectData *scaleData = (ObjectData *)objectData->elementAt(idx++);

    if (scaleData) {
      int scale = scaleData->GetValue();

      buffer.Format("[%d] -> %d", i, scale);
      HTREEITEM node = tree->insertInfoNode(scaldata, buffer, TO_ID(IDB_SCALE));
      // tree->getTree()->SetItemData(node,(DWORD)scaleData);
      SingleVariableObserver *itemData = new SingleVariableObserver(
        NULL, scaleData, "Scale Value", "Scale Values Used for Object");
      treeItemData->addElement(itemData);
      tree->getTree()->SetItemData(node, (DWORD)itemData);
    }
  }

  buffer.Format("Point Data [%d]", Verticies);
  HTREEITEM pts = tree->insertInfoNode(node, buffer, TO_ID(IDB_POINT));

  int objOffset = getListOffset(getXYZOffset());
  idx = objOffset;

  Point3D **ptArrayLocal = (Point3D **)malloc(Verticies * sizeof(Point3D *));
  for (int i = 0; i < Verticies; i++) {
    CString buffer;
    ObjectData *Xdat = (ObjectData *)objectData->elementAt(idx++);
    ObjectData *Ydat = (ObjectData *)objectData->elementAt(idx++);
    ObjectData *Zdat = (ObjectData *)objectData->elementAt(idx++);
    ObjectData *Udat = (ObjectData *)objectData->elementAt(idx++);

    if (Xdat == NULL) continue;
    int X = Xdat->GetValue();
    int Y = Ydat->GetValue();
    int Z = Zdat->GetValue();

    Point3D *ptA;

    if ((X & 0xFF00) == 0x8000) {
      int ptidx = (X & 0xFF);
      ptA = new Point3D(ptArrayLocal[ptidx]);
      // ptA->y = Y;
      ptA->z = (double)Z;
      // buffer.Format("[%d] (A) %x %x %x  -> %x %x %x  (%d %d
      // %d)",i,X,Y,Z,(int)ptA->x,(int)ptA->y,(int)ptA->z,(int)ptA->x,(int)ptA->y,(int)ptA->z);
      buffer.Format("[%d] (%d %d %d)", i, (int)ptA->x, (int)ptA->y, (int)ptA->z, (int)ptA->x, (int)ptA->y, (int)ptA->z);
      HTREEITEM xyzp = tree->insertInfoNode(pts, buffer, TO_ID(IDB_POINT));

      if (X >= 32768) {
        int ptid = X - 32768;
        buffer.Format("X=%d  use XY from p[%d]", X, ptid);
      } else {
        buffer.Format("X=%d", X);
      }
      HTREEITEM xnode = tree->insertInfoNode(xyzp, buffer, TO_ID(IDB_POINT));
      SingleVariableObserver *x_itemData = new SingleVariableObserver(
        NULL, Xdat, "X Data", "X Scale Index/Previous Point");
      treeItemData->addElement(x_itemData);
      tree->getTree()->SetItemData(xnode, (DWORD)x_itemData);

      buffer.Format("Y=%d", Y);
      HTREEITEM ynode = tree->insertInfoNode(xyzp, buffer, TO_ID(IDB_POINT));
      SingleVariableObserver *y_itemData = new SingleVariableObserver(
        NULL, Ydat, "Y Data", "Y Scale Index/Previous Point");
      treeItemData->addElement(y_itemData);
      tree->getTree()->SetItemData(ynode, (DWORD)y_itemData);

      buffer.Format("Z=%d", Z);
      HTREEITEM znode = tree->insertInfoNode(xyzp, buffer, TO_ID(IDB_POINT));
      SingleVariableObserver *z_itemData = new SingleVariableObserver(
        NULL, Zdat, "Z Data", "Z Scale Index/Previous Point");
      treeItemData->addElement(z_itemData);
      tree->getTree()->SetItemData(znode, (DWORD)z_itemData);
    } else {
      int Xid = X;
      int Yid = Y;
      int id = -1;
      int scaleIDx = -1;

      int xid;
      int yid;
      int xxid;
      int yyid;

      int scaleOff = getScaleOffset();

      if (X != 0) {
        if (X > 0x80 && X < 0xFF) {
          xid = ((X - 0x84) / 2) + scaleOff;
          xxid = ((X - 0x84) / 4);
          int scaleIDx = getListOffset(xid);
          ObjectData *Xdat = (ObjectData *)objectData->elementAt(scaleIDx);
          X = Xdat->GetValue();
          X = -X;
        } else {
          xid = ((X - 0x4) / 2) + scaleOff;
          xxid = ((X - 0x4) / 4);
          int scaleIDx = getListOffset(xid);
          ObjectData *Xdat = (ObjectData *)objectData->elementAt(scaleIDx);
          X = Xdat->GetValue();
        }
      }

      if (Y != 0) {
        if (Y > 0x80 && Y < 0xFF) {
          yid = ((Y - 0x84) / 2) + scaleOff;
          yyid = ((Y - 0x84) / 4);
          int scaleIDx = getListOffset(yid);
          ObjectData *Ydat = (ObjectData *)objectData->elementAt(scaleIDx);
          Y = Ydat->GetValue();
          Y = -Y;
        } else {
          yid = ((Y - 0x4) / 2) + scaleOff;
          yyid = ((Y - 0x4) / 4);
          int scaleIDx = getListOffset(yid);
          ObjectData *Ydat = (ObjectData *)objectData->elementAt(scaleIDx);
          Y = Ydat->GetValue();
        }
      }

      ptA = new Point3D((double)X, (double)Y, (double)Z);

      // buffer.Format("[%d] %x %x %x  -> %x %x %x (%d %d %d) %d
      // %d",i,Xid,Yid,Z,(int)ptA->x,(int)ptA->y,(int)ptA->z,(int)ptA->x,(int)ptA->y,(int)ptA->z,id,scaleIDx);
      buffer.Format("[%d] (%d %d %d)", i, (int)ptA->x, (int)ptA->y, (int)ptA->z);
      HTREEITEM xyzp = tree->insertInfoNode(pts, buffer, TO_ID(IDB_POINT));

      buffer.Format("X=%d uses scale s[%d]", X, xxid);
      HTREEITEM xnode = tree->insertInfoNode(xyzp, buffer, TO_ID(IDB_POINT));
      SingleVariableObserver *x_itemData = new SingleVariableObserver(
        NULL, Xdat, "X Data", "X Scale Index/Previous Point");
      treeItemData->addElement(x_itemData);
      tree->getTree()->SetItemData(xnode, (DWORD)x_itemData);

      buffer.Format("Y=%d uses scale s[%d]", Y, yyid);
      HTREEITEM ynode = tree->insertInfoNode(xyzp, buffer, TO_ID(IDB_POINT));
      SingleVariableObserver *y_itemData = new SingleVariableObserver(
        NULL, Ydat, "Y Data", "Y Scale Index/Previous Point");
      treeItemData->addElement(y_itemData);
      tree->getTree()->SetItemData(ynode, (DWORD)y_itemData);

      buffer.Format("Z=%d", Z);
      HTREEITEM znode = tree->insertInfoNode(xyzp, buffer, TO_ID(IDB_POINT));
      SingleVariableObserver *z_itemData = new SingleVariableObserver(
        NULL, Zdat, "Z Data", "Z Scale Index/Previous Point");
      treeItemData->addElement(z_itemData);
      tree->getTree()->SetItemData(znode, (DWORD)z_itemData);
    }

    // Point3D *ptB = new Point3D(ptA);
    ptArrayLocal[i] = ptA;
  }

  // interconnections
  idx = getListOffset(getVertexOffset());
  int connections = getNumVertexConnections();

  buffer.Format("Vertex Connection List [%d]", connections);
  HTREEITEM intercon = tree->insertInfoNode(node, buffer, TO_ID(IDB_VERTICIES));

  for (int i = 0; i < connections; i++) {
    CString buffer;
    ObjectData *PtPt = (ObjectData *)objectData->elementAt(idx++);
    ObjectData *Unused = (ObjectData *)objectData->elementAt(idx++);

    if (PtPt == NULL) continue;
    int verticies = PtPt->GetValue();

    int pt1 = LOBYTE(verticies);
    int pt2 = HIBYTE(verticies);

    if (pt1 < 0) pt1 = 0;
    if (pt1 >= Verticies) pt1 = Verticies - 1;

    if (pt2 < 0) pt2 = 0;
    if (pt2 >= Verticies) pt2 = Verticies - 1;

    buffer.Format("[%d] %d -> %d", i, pt1, pt2);
    HTREEITEM vp = tree->insertInfoNode(intercon, buffer, TO_ID(IDB_VERTICIES));
    // SingleVariableObserver * itemData = new
    // SingleVariableObserver(NULL,PtPt,"From/To Vertex","(From + (256*To)) Index
    // for Point");
    SingleVertexObserver *itemData = new SingleVertexObserver(
      NULL, PtPt, "From/To Vertex", "(From + (256*To)) Index for Point");
    treeItemData->addElement(itemData);
    tree->getTree()->SetItemData(vp, (DWORD)itemData);
  }

  ///////////////////////////////////////////////////////
  // Textures again!
  idx = getListOffset(getTextureOffset());
  idxend = getListOffset(getTextureEndOffset());
  int idxStart = idx;

  int numBytes = (getTextureEndOffset() - getTextureOffset() / 4);
  int numText = idxend - idx;

  if (numText > 0) {
    buffer.Format("Texture Data [%d]", numText);
    HTREEITEM textdata = tree->insertInfoNode(node, buffer, TO_ID(IDB_OFFSET));

    while (((idx - idxStart) < numBytes)) {
      CString buffer;
      CString vertexStringList;

      int num = GET_INT(idx++);
      int unk1 = GET_INT(idx++);

      int unk2, unk3, unk4;
      int unk5, unk6;

      int texture;
      int cmd;
      ObjectData *textureObj;
      ObjectData *rotObj;

      BOOL Finished = FALSE;

      unk2 = -99;
      unk3 = -99;
      unk4 = -99;
      unk5 = -99;
      unk6 = -99;

      ObjectData *unk2Obj, *unk3Obj, *unk4Obj, *unk5Obj, *unk6Obj;
      // Vertex *ver;
      BOOL added = FALSE;
      int flags = -1;
      Vector vertexObjs("vertex", __LINE__);

      switch (unk1) {
      case 0x20:
      case 0x80: {
        cmd = 0x80;
        texture = 0x80;
        unk2 = GET_INT(idx++);
        unk3 = GET_INT(idx++);
        unk4 = GET_INT(idx++);
        CString msg;
        msg.Format("[%d] Object %d %d %d %d", num, unk1, unk2, unk3, unk4);
        HTREEITEM node =
          tree->insertInfoNode(textdata, msg, TO_ID(IDB_OBJECT));

        if (subObjects == NULL)
          subObjects = new Vector("SubObjects", __LINE__);

        SubObject *subObj = new SubObject();
        subObj->unk1 = unk2;
        subObj->unk2 = unk3;
        subObj->unk3 = unk4;
        subObjects->addElement(subObj);
        added = TRUE;
      } break;
      case 0x22:
      case 0x24:
      case 0x82: {
        // an object repeater
        cmd = 0x82;
        texture = 0x82;
        unk2 = GET_INT(idx++);
        unk3 = GET_INT(idx++);
        unk4 = GET_INT(idx++);
        unk5 = GET_INT(idx++);
        CString msg;
        msg.Format("[%d] Object %d %d %d %d", num, unk1, unk2, unk3, unk4);
        HTREEITEM node =
          tree->insertInfoNode(textdata, msg, TO_ID(IDB_OBJECT));

        if (subObjects == NULL)
          subObjects = new Vector("SubObjects", __LINE__);

        SubObject *subObj = new SubObject();
        subObj->unk1 = unk2;
        subObj->unk2 = unk3;
        subObj->unk3 = unk4;
        subObjects->addElement(subObj);
        added = TRUE;
      } break;
      case 0x86: {
        // an object repeater
        cmd = 0x86;
        texture = 0x86;
        unk2 = GET_INT(idx++);
        unk3 = GET_INT(idx++);
        unk4 = GET_INT(idx++);
        unk5 = GET_INT(idx++);
        CString msg;
        msg.Format("[%d] Object %d %d %d %d %d", num, unk1, unk2, unk3, unk4, unk5);
        HTREEITEM node =
          tree->insertInfoNode(textdata, msg, TO_ID(IDB_OBJECT));

        if (subObjects == NULL)
          subObjects = new Vector("SubObjects", __LINE__);

        SubObject *subObj = new SubObject();
        subObj->unk1 = unk2;
        subObj->unk2 = unk3;
        subObj->unk3 = unk4;
        subObjects->addElement(subObj);
        added = TRUE;
      } break;
      case 0x88: {
        // an object repeater
        cmd = 0x86;
        texture = 0x86;
        unk2 = GET_INT(idx++);
        unk3 = GET_INT(idx++);
        unk4 = GET_INT(idx++);
        CString msg;
        msg.Format("[%d] Object %d %d %d %d %d", num, unk1, unk2, unk3, unk4, unk5);
        HTREEITEM node =
          tree->insertInfoNode(textdata, msg, TO_ID(IDB_OBJECT));

        if (subObjects == NULL)
          subObjects = new Vector("SubObjects", __LINE__);

        SubObject *subObj = new SubObject();
        subObj->unk1 = unk2;
        subObj->unk2 = unk3;
        subObj->unk3 = unk4;
        subObjects->addElement(subObj);
        added = TRUE;
      } break;

      case 0x1:
      case 0x2:
      case 0x3:
      case 0x4:
      case 0x5:
      case 0x6:
      case 0x7:
      case 0x8:
      case 0x9:
      case 0xa:
      case 0xb:
      case 0xc:
      case 0xd:
      case 0xe:
      case 0xf: {
        // max 20 verticies
        // short v[20];
        int vcount = 0;
        texture = 0;
        // rotation = 0;

        vertexObjs.removeAllData();
        Vertex *ver = new Vertex(texture, unk2, unk3, unk4, unk5, unk6);
        {
          int val = -1;
          vertexObjs.removeAllData();
          vertexStringList = "v_list=(";
          while (val != 0) {
            ObjectData *verObj = GET_OBJ(idx);
            vertexObjs.addElement(verObj);
            val = GET_INT(idx++);
            if (val == -32768) {
              ObjectData *verObj = GET_OBJ(idx);
              vertexObjs.addElement(verObj);
              val = GET_INT(idx++);
            }
            // int val = GET_INT(idx++);

            // while(val != 0)
            //{
            //  if (val == -32768){
            //      val = GET_INT(idx++);
            //}
            ver->addPoint(val);

            CString msg;
            if (val > 32769) {
              val = -(0x10000 - val);
            }
            msg.Format(" %d", val);
            vertexStringList += msg;
            val = GET_INT(idx++);
          }
          vertexStringList += ") ";
          idx--;
        }
        // polygons->addElement(ver);
        int term = GET_INT(idx++);
      } break;

      case 0x10:
      case 0x11:
      case 0x12:
      case 0x13:
      case 0x14:
      case 0x15:
      case 0x16:
      case 0x17:
      case 0x18:
      case 0x19:
      case 0x1a:
      case 0x1b:
      case 0x1c:
      case 0x1d:
      case 0x1e:
      case 0x1f: {
        textureObj = GET_OBJ(idx);
        texture = GET_INT(idx++);
        rotObj = GET_OBJ(idx);
        int rotation = GET_INT(idx++);
        cmd = rotation;

        Vertex *ver = new Vertex(texture, unk2, unk3, unk4, unk5, unk6);

        if (rotation >= 0x80) {
          flags = GET_INT(idx++);

          switch (flags) {
          case 0x1: {
            unk2Obj = GET_OBJ(idx);
            unk2 = GET_INT(idx++);
            unk3Obj = GET_OBJ(idx);
            unk3 = GET_INT(idx++);
          } break;
          case 0x3: {
            unk2Obj = GET_OBJ(idx);
            unk2 = GET_INT(idx++);
            unk3Obj = GET_OBJ(idx);
            unk3 = GET_INT(idx++);
            unk4Obj = GET_OBJ(idx);
            unk4 = GET_INT(idx++);
            unk5Obj = GET_OBJ(idx);
            unk5 = GET_INT(idx++);
          } break;
          case 0x4: {
            unk2Obj = GET_OBJ(idx);
            unk2 = GET_INT(idx++);
          } break;
          case 0x5: {
            unk2Obj = GET_OBJ(idx);
            unk2 = GET_INT(idx++);
            unk3Obj = GET_OBJ(idx);
            unk3 = GET_INT(idx++);
            unk4Obj = GET_OBJ(idx);
            unk4 = GET_INT(idx++);
          } break;
          case 0x7: {
            unk2Obj = GET_OBJ(idx);
            unk2 = GET_INT(idx++);
            unk3Obj = GET_OBJ(idx);
            unk3 = GET_INT(idx++);
            unk4Obj = GET_OBJ(idx);
            unk4 = GET_INT(idx++);
            unk5Obj = GET_OBJ(idx);
            unk5 = GET_INT(idx++);
            unk6Obj = GET_OBJ(idx);
            unk6 = GET_INT(idx++);
          } break;
          case 0x10: {
            unk2Obj = GET_OBJ(idx);
            unk2 = GET_INT(idx++);
            ver->m_HorizontalRepeat = unk2;
            // unk2 = GET_INT(idx++);
            // unk3 = GET_INT(idx++);
          } break;
          case 0x11: {
            unk2Obj = GET_OBJ(idx);
            unk2 = GET_INT(idx++);
            unk3Obj = GET_OBJ(idx);
            unk3 = GET_INT(idx++);
            unk4Obj = GET_OBJ(idx);
            unk4 = GET_INT(idx++);
          } break;
          case 0x14: {
            unk2Obj = GET_OBJ(idx);
            unk2 = GET_INT(idx++);
            unk3Obj = GET_OBJ(idx);
            unk3 = GET_INT(idx++);
            unk4Obj = GET_OBJ(idx);
            unk4 = GET_INT(idx++);
            unk5Obj = GET_OBJ(idx);
            unk5 = GET_INT(idx++);
            unk6Obj = GET_OBJ(idx);
            unk6 = GET_INT(idx++);
          } break;
          case 0x15: {
            unk2Obj = GET_OBJ(idx);
            unk2 = GET_INT(idx++);
            unk3Obj = GET_OBJ(idx);
            unk3 = GET_INT(idx++);
            unk4Obj = GET_OBJ(idx);
            unk4 = GET_INT(idx++);
            unk5Obj = GET_OBJ(idx);
            unk5 = GET_INT(idx++);
            unk6Obj = GET_OBJ(idx);
            unk6 = GET_INT(idx++);
          } break;
          default: {
            CString msg;
            msg.Format("Unknown Flags %x", flags);
            AfxMessageBox(msg);
          } break;
          }// flags
        }// rotation
        int vcount = 0;
        // ver = new Vertex(texture,unk2,unk3,unk4,unk5,unk6);
        ver->m_HorizontalRepeat = unk2;
        ver->m_VerticalRepeat = unk3;
        ver->unk4 = unk4;
        ver->unk5 = unk5;
        ver->unk6 = unk6;
        vertexStringList = "v_list=(";

        {
          int val = -1;
          vertexObjs.removeAllData();
          while (val != 0) {
            ObjectData *verObj = GET_OBJ(idx);
            vertexObjs.addElement(verObj);
            val = GET_INT(idx++);
            if (val == -32768) {
              ObjectData *verObj = GET_OBJ(idx);
              vertexObjs.addElement(verObj);
              val = GET_INT(idx++);
            }
            ver->addPoint(val);
            CString msg;
            if (val > 32769) {
              val = -(0x10000 - val);
            }
            msg.Format(" %d", val);
            vertexStringList += msg;
            val = GET_INT(idx++);
          }
          vertexStringList += ") ";
          idx--;
        }
        // polygonPoints->addElement(ver);
        int term = GET_INT(idx++);
      } break;
      case 0x0: {
        Finished = TRUE;
      } break;
      default: {
        CString msg;
        msg.Format("Unknown Commands %x", unk1);
        AfxMessageBox(msg);
        Finished = TRUE;
      } break;
      }

      if (Finished == TRUE) break;
      if (added) continue;

      CString msg;

      if (flags != -1) {
        msg.Format("t[%d] cmd=%x txt=%x rot=%x flags=%x ", num, unk1, texture, cmd, flags);
      } else {
        msg.Format("t[%d] cmd=%x txt=%x rot=%x ", num, unk1, texture, cmd);
      }
      if (unk2 != -99) {
        CString msga;
        msga.Format("%x ", unk2);
        msg += msga;
      }
      if (unk3 != -99) {
        CString msga;
        msga.Format("%x ", unk3);
        msg += msga;
      }
      if (unk4 != -99) {
        CString msga;
        msga.Format("%x ", unk4);
        msg += msga;
      }
      if (unk5 != -99) {
        CString msga;
        msga.Format("%x ", unk5);
        msg += msga;
      }
      if (unk6 != -99) {
        CString msga;
        msga.Format("%x ", unk6);
        msg += msga;
      }

      msg += vertexStringList;

      HTREEITEM node = tree->insertInfoNode(textdata, msg, TO_ID(IDB_JAMFILE));

      buffer.Format("Texture?=%d", texture);
      HTREEITEM textureNode =
        tree->insertInfoNode(node, buffer, TO_ID(IDB_OFFSET));
      SingleVariableObserver *itemData = new SingleVariableObserver(
        NULL, textureObj, "Texture Cmd Argument", "Possible Texture Id");
      treeItemData->addElement(itemData);
      tree->getTree()->SetItemData(textureNode, (DWORD)itemData);

      buffer.Format("Rotation?=%d", cmd);
      HTREEITEM textureRot =
        tree->insertInfoNode(node, buffer, TO_ID(IDB_OFFSET));
      SingleVariableObserver *itemDataRot = new SingleVariableObserver(
        NULL, rotObj, "Texture Rotation", "Possible Texture Rotation");
      treeItemData->addElement(itemDataRot);
      tree->getTree()->SetItemData(textureRot, (DWORD)itemDataRot);
      //
      if (unk2 != -99) {
        buffer.Format("Horizontal Resolution (/256)=%d (%d times)", unk2, unk2 / 256);
        HTREEITEM textureNode2 =
          tree->insertInfoNode(node, buffer, TO_ID(IDB_OFFSET));
        SingleVariableObserver *itemData = new SingleVariableObserver(
          NULL, unk2Obj, "Texture Cmd Argument", "Possible Repeat Number");
        treeItemData->addElement(itemData);
        tree->getTree()->SetItemData(textureNode2, (DWORD)itemData);
      }
      //
      if (unk3 != -99) {
        buffer.Format("Vertical Resolution (/256)=%d (%d times)", unk3, unk3 / 256);
        HTREEITEM textureNode2 =
          tree->insertInfoNode(node, buffer, TO_ID(IDB_OFFSET));
        SingleVariableObserver *itemData = new SingleVariableObserver(
          NULL, unk3Obj, "Texture Cmd Argument", "Possible Repeat Number");
        treeItemData->addElement(itemData);
        tree->getTree()->SetItemData(textureNode2, (DWORD)itemData);
      }
      //
      if (unk4 != -99) {
        buffer.Format("Horizontal Resolution (/256)=%d", unk4);
        HTREEITEM textureNode2 =
          tree->insertInfoNode(node, buffer, TO_ID(IDB_OFFSET));
        SingleVariableObserver *itemData = new SingleVariableObserver(
          NULL, unk4Obj, "Texture Cmd Argument", "Possible Repeat Number");
        treeItemData->addElement(itemData);
        tree->getTree()->SetItemData(textureNode2, (DWORD)itemData);
      }
      if (unk5 != -99) {
        buffer.Format("Unk5?=%d", unk5);
        HTREEITEM textureNode2 =
          tree->insertInfoNode(node, buffer, TO_ID(IDB_OFFSET));
        SingleVariableObserver *itemData = new SingleVariableObserver(
          NULL, unk5Obj, "Texture Cmd Argument", "Possible Repeat Number");
        treeItemData->addElement(itemData);
        tree->getTree()->SetItemData(textureNode2, (DWORD)itemData);
      }
      if (unk6 != -99) {
        buffer.Format("Unk6?=%d", unk6);
        HTREEITEM textureNode2 =
          tree->insertInfoNode(node, buffer, TO_ID(IDB_OFFSET));
        SingleVariableObserver *itemData = new SingleVariableObserver(
          NULL, unk6Obj, "Texture Cmd Argument", "Possible Repeat Number");
        treeItemData->addElement(itemData);
        tree->getTree()->SetItemData(textureNode2, (DWORD)itemData);
      }

      for (int i = 0; i < vertexObjs.size(); i++) {
        ObjectData *vertexObj = (ObjectData *)vertexObjs.elementAt(i);
        buffer.Format("Vertex[%d]=%d", i, vertexObj->GetValue());
        HTREEITEM textureNode2 =
          tree->insertInfoNode(node, buffer, TO_ID(IDB_OFFSET));
        SingleVariableObserver *itemData = new SingleVariableObserver(
          NULL, vertexObj, "Vertex", "Vertex Number for polygon");
        treeItemData->addElement(itemData);
        tree->getTree()->SetItemData(textureNode2, (DWORD)itemData);
      }
    }
  }
  // CreateObject();

  int uidx = getListOffset(getEndVertexOffset());
  int uidxend = getListOffset(getEndOffset());

  int numUnknowns = uidxend - uidx;

  buffer.Format("Unknown Data [%d]", numUnknowns);
  HTREEITEM unkParent = tree->insertInfoNode(node, buffer, TO_ID(IDB_OFFSET));

  for (int i = 0; i < numUnknowns; i++) {
    CString buffer;
    ObjectData *scaleData = (ObjectData *)objectData->elementAt(idx++);

    if (scaleData) {
      int scale = scaleData->GetValue();
      buffer.Format("Unknown Object Code[%d] -> %d", i, scale);
      HTREEITEM node =
        tree->insertInfoNode(unkParent, buffer, TO_ID(IDB_OFFSET));
      SingleVariableObserver *itemData = new SingleVariableObserver(
        NULL, scaleData, "Unk Value", "Unk Values Used for Object");
      treeItemData->addElement(itemData);
      tree->getTree()->SetItemData(node, (DWORD)itemData);
    }
  }

  // CreateObject();
  for (int i = 0; i < Verticies; i++) {
    delete ptArrayLocal[i];
  }
  free(ptArrayLocal);
}

void InternalObject::LoadTree(CTrackTree *tree, HTREEITEM node)
{
  // Texture start =
  ObjectData *dat = (ObjectData *)objectData->elementAt(4);
  int pos = dat->GetValue();

  Vector *cmds = new Vector("ObjectCmds", __LINE__);

  int listsize = objectData->size();

  for (int count = 32; count < listsize; count++) {
    ObjectData *cmd = (ObjectData *)objectData->elementAt(count);
    int cmdType = cmd->GetValue();

    switch (cmdType) {
    case 0x83: {
      InternalObjectCmd *IOcmd =
        new InternalObjectCmd(track, INTERAL_CMD_TEXTURE_83, 8);

      // texture
      int unk1 = ((ObjectData *)objectData->elementAt(count - 2))->GetValue();
      int textureid =
        ((ObjectData *)objectData->elementAt(count - 1))->GetValue();
      IOcmd->setNthArg(1, unk1);
      IOcmd->setNthArg(2, textureid);
      IOcmd->setNthArg(
        3, ((ObjectData *)objectData->elementAt(count))->GetValue());
      IOcmd->setNthArg(
        4, ((ObjectData *)objectData->elementAt(count + 1))->GetValue());
      IOcmd->setNthArg(
        5, ((ObjectData *)objectData->elementAt(count + 2))->GetValue());
      IOcmd->setNthArg(
        6, ((ObjectData *)objectData->elementAt(count + 3))->GetValue());

      cmds->addElement(IOcmd);
      count += 5;
    } break;
    case 0x03: {
      InternalObjectCmd *IOcmd =
        new InternalObjectCmd(track, INTERAL_CMD_TEXTURE_03, 9);
      // texture
      int unk1 = ((ObjectData *)objectData->elementAt(count - 2))->GetValue();
      int textureid =
        ((ObjectData *)objectData->elementAt(count - 1))->GetValue();
      IOcmd->setNthArg(1, unk1);
      IOcmd->setNthArg(2, textureid);
      IOcmd->setNthArg(
        3, ((ObjectData *)objectData->elementAt(count))->GetValue());
      IOcmd->setNthArg(
        4, ((ObjectData *)objectData->elementAt(count + 1))->GetValue());
      IOcmd->setNthArg(
        5, ((ObjectData *)objectData->elementAt(count + 2))->GetValue());
      IOcmd->setNthArg(
        6, ((ObjectData *)objectData->elementAt(count + 3))->GetValue());

      cmds->addElement(IOcmd);
      count += 6;
    } break;
    default:
      count++;
      break;
    }
  }

  for (int j = 0; j < cmds->size(); j++) {
    InternalObjectCmd *IOcmd = (InternalObjectCmd *)cmds->elementAt(j);
    int textureid = IOcmd->getNthArg(2);
    CString buff;
    buff.Format("TexureId=%d Unk1=%d Cmd=%d Unk3=%d Unk4=%d", textureid, IOcmd->getNthArg(1), IOcmd->getNthArg(3), IOcmd->getNthArg(4), IOcmd->getNthArg(5));
    HTREEITEM texture = tree->insertInfoNode(SubTree, buff, TO_ID(IDB_JAMFILE));
    tree->getTree()->SetItemData(texture, (DWORD)IOcmd);
  }
}

void InternalObject::WriteObject(int offset)
{
  int listsize = objectData->size();

  /*
  for(int i=0;i<listsize;i++)
  {
            ObjectData *dat = (ObjectData*)objectData->elementAt(i);

            switch(dat->getType())
            {
            case GP_OBJECT_WORD:
              track->WritePCWord(offset,dat->GetValue());offset+=2;
                  break;
            case GP_OBJECT_DWORD:
                  track->WritePCDWord(offset,dat->GetValue());offset+=4;
                  break;
            case GP_OBJECT_BYTE:
                  track->WritePCByte(offset,dat->GetValue());offset+=1;
                  break;
            }

  }
  */
  ObjectData *last = (ObjectData *)objectData->lastElement();
  ObjectData *first = (ObjectData *)objectData->elementAt(0);
  ObjectData *dat;

  for (dat = first; dat != last; dat = (ObjectData *)objectData->next(dat)) {
    switch (dat->getType()) {
    case GP_OBJECT_WORD:
      track->WritePCWord(offset, dat->GetValue());
      offset += 2;
      break;
    case GP_OBJECT_DWORD:
      track->WritePCDWord(offset, dat->GetValue());
      offset += 4;
      break;
    case GP_OBJECT_BYTE:
      track->WritePCByte(offset, dat->GetValue());
      offset += 1;
      break;
    }
  }
}

void InternalObject::ReadObject(GPTrack *track)
{
  int offset = Offset;
  int start = offset;

  itemId = track->ReadPCWord(offset);
  objectData->addElement(new ObjectWord(itemId));

  if (itemId != 0x8002) {
    char msg[256];
    wsprintf(msg,
      "Internal Object Error: Invalid Magic Number! 0x8002 at offset "
      "0x%x id was 0x%x",
      offset,
      itemId);
    AfxMessageBox(msg, MB_OK);
    return;
  }
  offset += 2;
  PossibleId = track->ReadPCWord(offset);
  objectData->addElement(new ObjectWord(PossibleId));
  offset += 2;

  for (int i = 0; i < 9; i++) {
    int val = track->ReadPCDWord(offset);
    objectData->addElement(new ObjectDWord(val));
    offset += 4;
  }
  int alwaysZero = track->ReadPCDWord(offset);
  objectData->addElement(new ObjectDWord(alwaysZero));

  offset += 4;
  int alwaysOne = track->ReadPCDWord(offset);
  objectData->addElement(new ObjectDWord(alwaysOne));
  offset += 4;

  int Unknown = track->ReadPCWord(offset);// 1
  objectData->addElement(new ObjectWord(Unknown));
  offset += 2;
  int Size = track->ReadPCWord(offset);// 4;
  objectData->addElement(new ObjectWord(Size));
  offset += 2;
  int Size8 = track->ReadPCWord(offset);
  objectData->addElement(new ObjectWord(Size8));
  offset += 2;
  Verticies = Size8 / 8;

  for (int i = 0; i < 11; i++) {
    int Unk = track->ReadPCWord(offset);
    objectData->addElement(new ObjectWord(Unk));
    offset += 2;
  }
  int toread = (getSize() - (offset - start)) / 2;

  int idx = 27;
  XYZData = idx;

  for (int i = 0; i < toread; i++) {
    int Unk = track->ReadPCWord(offset);

    objectData->addElement(new ObjectWord(Unk));
    offset += 2;
    idx++;
    if (Unk == 0x84 && XYZData == 27) {
      XYZData = idx;
    }
  }

  VertexData = ((Size8 / 8) * 4) + XYZData;
}

int GetPCWord(FILE *fp)
{
  char buffer[2];

  fread(buffer, 2, sizeof(char), fp);
  int PCword;
  PCword = buffer[1] * 256 + buffer[0];
  return 0x0000FFFF & PCword;
}

long GetPCDWord(FILE *fp)
{
  unsigned char data[4];
  int PCword;
  fread(data, 4, sizeof(char), fp);

  PCword =
    data[3] * 256 * 256 * 256 + data[2] * 256 * 256 + data[1] * 256 + data[0];
  return PCword;
}

BOOL InternalObject::ImportFromFile(LPCSTR filename)
{
  AfxMessageBox("Add Internal Object");

  FILE *fp = fopen(filename, "r");

  if (fp == NULL) {
    AfxMessageBox("Failed to open object");
    return FALSE;
  }

  int objectLength = GetFileLength(fp);
  itemId = GetPCWord(fp);

  DataSize = objectLength;

  if (itemId != 0x8002) {
    AfxMessageBox("Not a GP2 Object! cannot include");
    return FALSE;
  }
  objectData->addElement(new ObjectWord(itemId));
  PossibleId = GetPCWord(fp);
  objectData->addElement(new ObjectWord(PossibleId));

  for (int i = 0; i < 9; i++) {
    int val = GetPCDWord(fp);
    objectData->addElement(new ObjectDWord(val));
  }
  int alwaysZero = GetPCDWord(fp);
  objectData->addElement(new ObjectDWord(alwaysZero));

  int alwaysOne = GetPCDWord(fp);
  objectData->addElement(new ObjectDWord(alwaysOne));

  int Unknown = GetPCWord(fp);// 1
  objectData->addElement(new ObjectWord(Unknown));

  int Size = GetPCWord(fp);// 4;
  objectData->addElement(new ObjectWord(Size));

  int Size8 = GetPCWord(fp);
  objectData->addElement(new ObjectWord(Size8));

  Verticies = Size8 / 8;

  for (int i = 54; i < objectLength; i += 2) {
    int val = GetPCWord(fp);
    objectData->addElement(new ObjectWord(val));
  }
  fclose(fp);
  return TRUE;
}

#define EXPORT_HEX_ITEM(x, msg)                                \
  {                                                            \
    ObjectData *data = (ObjectData *)objectData->elementAt(x); \
    data->exportHex(fp, (LPSTR)(LPCSTR)msg);                   \
  }
#define EXPORT_DEC_ITEM(x, msg)                                \
  {                                                            \
    ObjectData *data = (ObjectData *)objectData->elementAt(x); \
    data->exportFile(fp, (LPSTR)(LPCSTR)msg);                  \
  }
#define EXPORT_HEX_WORD(x)                                     \
  {                                                            \
    ObjectData *data = (ObjectData *)objectData->elementAt(x); \
    fprintf(fp, "%0x%x\t", (int)data->GetValue());             \
  }
#define GET_ITEM(x) (((ObjectData *)objectData->elementAt(x))->GetValue());
#define BRL fprintf(fp, "\n");
#define HR(t) fprintf(fp, "##########%s ##########\n", (LPSTR)(t));

void InternalObject::exportFile()
{
  CString buffer;

  FILE *fp = fopen("obj.txt", "w");

  if (fp == NULL) {
    AfxMessageBox("Cannot open output file");
    return;
  }
  AfxMessageBox("Export Internal Object");

  HR("Object Output")
  EXPORT_HEX_ITEM(0, "Internal Object Locator Identifier")
  BRL EXPORT_HEX_ITEM(1, "Internal Object Track Identifier")
    HR("Memory Offsets") EXPORT_HEX_ITEM(2, "Offset 1") int base =
      GET_ITEM(2);
  int off2 = GET_ITEM(3);
  int off3 = GET_ITEM(4);
  int off4 = GET_ITEM(5);
  int off5 = GET_ITEM(6);
  int off6 = GET_ITEM(7);
  int off7 = GET_ITEM(8);
  int off8 = GET_ITEM(9);
  int off9 = GET_ITEM(10);

  buffer.Format("Offset 2 - %d", off2 - base);
  EXPORT_HEX_ITEM(3, buffer)
  buffer.Format("Offset 2 - %d, %d", off3 - base, off3 - off2);
  EXPORT_HEX_ITEM(4, buffer)
  buffer.Format("Offset 3 - %d %d", off4 - base, off4 - off3);
  EXPORT_HEX_ITEM(5, buffer)
  buffer.Format("Offset 4 - %d %d", off5 - base, off5 - off4);
  EXPORT_HEX_ITEM(6, buffer)
  buffer.Format("Offset 5 - %d %d", off6 - base, off6 - off5);
  EXPORT_HEX_ITEM(7, buffer)
  buffer.Format("Offset 6 - %d %d", off7 - base, off7 - off6);
  EXPORT_HEX_ITEM(8, buffer)
  buffer.Format("Offset 7 - %d %d", off8 - base, off8 - off7);
  EXPORT_HEX_ITEM(9, buffer)
  buffer.Format("Offset 8 - %d %d", off9 - base, off9 - off8);
  EXPORT_HEX_ITEM(10, buffer)
  BRL HR("Unknown 3 words") EXPORT_HEX_ITEM(11, "Unknown 1")
    EXPORT_HEX_ITEM(12, "Unknown 2") EXPORT_HEX_ITEM(13, "Unknown 3")
      BRL EXPORT_DEC_ITEM(14, "Number of Points") int size = GET_ITEM(14);
  EXPORT_DEC_ITEM(15, "Number of Points*8")
  BRL

    HR("Unknown Unk words") int idx = 16;
  for (int i = 0; i < 17; i++) {
    EXPORT_HEX_ITEM(16 + i, "Unknown")
    idx++;
  }
  BRL HR("Assuming size/2 textures") for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < 9; j++) {
      int val = GET_ITEM(idx);
      fprintf(fp, "0x%4.4x ", val);
      idx++;
    }
    BRL
  }
  HR("End of textures")

  fclose(fp);
}

void ObjectData::exportHex(FILE *fp, LPCSTR msg)
{
  fprintf(fp, "%s - 0x%x\n", msg, GetValue());
}

void ObjectData::exportFile(FILE *fp, LPCSTR msg)
{
  fprintf(fp, "%s - %d\n", msg, GetValue());
}

#define UPDATE_OFFSET(x)                                                     \
  {                                                                          \
    ObjectData *dat = (ObjectData *)objectData->elementAt(x);                \
    dat->SetValue(                                                           \
      (dat->GetValue() - track->LargeNumber1.GetValue() - Offset + 0x1020)); \
  }

void InternalObject::ExportObject(CString filename)
{
  CreateObject();
  FILE *fp = fopen(filename, "w");

  if (fp == NULL) {
    AfxMessageBox("Failed to open export file");
    return;
  }

  int old2 = ((ObjectData *)objectData->elementAt(2))->GetValue();
  int old3 = ((ObjectData *)objectData->elementAt(3))->GetValue();
  int old4 = ((ObjectData *)objectData->elementAt(4))->GetValue();
  int old5 = ((ObjectData *)objectData->elementAt(5))->GetValue();
  int old6 = ((ObjectData *)objectData->elementAt(6))->GetValue();
  int old7 = ((ObjectData *)objectData->elementAt(7))->GetValue();
  int old8 = ((ObjectData *)objectData->elementAt(8))->GetValue();
  int old9 = ((ObjectData *)objectData->elementAt(9))->GetValue();
  int old10 = ((ObjectData *)objectData->elementAt(10))->GetValue();

  // Adjust pointers
  UPDATE_OFFSET(2);
  UPDATE_OFFSET(3);
  UPDATE_OFFSET(4);
  UPDATE_OFFSET(5);
  UPDATE_OFFSET(6);
  UPDATE_OFFSET(7);
  UPDATE_OFFSET(8);
  UPDATE_OFFSET(9);
  UPDATE_OFFSET(10);

  int listsize = objectData->size();
  int offset = 0;

  fprintf(fp, "# number of data elements\n");
  fprintf(fp, "%d\n", listsize);
  fprintf(fp, "# offset bytes value\n");

  for (int i = 0; i < listsize; i++) {
    ObjectData *dat = (ObjectData *)objectData->elementAt(i);

    switch (dat->getType()) {
    case GP_OBJECT_WORD:
      fprintf(fp, "%d 2 0x%x \t//%d\n", offset, dat->GetValue(), dat->GetValue());
      offset += 2;
      break;
    case GP_OBJECT_DWORD:
      fprintf(fp, "%d 4 0x%x \t//%d\n", offset, dat->GetValue(), dat->GetValue());
      offset += 4;
      break;
    case GP_OBJECT_BYTE:
      fprintf(fp, "%d 1 0x%x \t//%d\n", offset, dat->GetValue(), dat->GetValue());
      offset += 1;
      break;
    }
  }

  // Export the JAM names in comments
  if (polygonPoints) {
    int size = polygonPoints->size();
    fprintf(fp, "##JAM_TEXTURES %d\n", size);
    for (int i = 0; i < size; i++) {
      CString msg;
      Vertex *ver = (Vertex *)polygonPoints->elementAt(i);
      char *filename = track->GetJAMFile(ver->textureid);
      fprintf(fp, "##JAM_FILE %d %d %s\n", i, ver->textureid, filename);
    }
  }

  if (fp) fclose(fp);

  ((ObjectData *)objectData->elementAt(2))->SetValue(old2);
  ((ObjectData *)objectData->elementAt(3))->SetValue(old3);
  ((ObjectData *)objectData->elementAt(4))->SetValue(old4);
  ((ObjectData *)objectData->elementAt(5))->SetValue(old5);
  ((ObjectData *)objectData->elementAt(6))->SetValue(old6);
  ((ObjectData *)objectData->elementAt(7))->SetValue(old7);
  ((ObjectData *)objectData->elementAt(8))->SetValue(old8);
  ((ObjectData *)objectData->elementAt(9))->SetValue(old9);
  ((ObjectData *)objectData->elementAt(10))->SetValue(old10);
}

static char *
  getObjectline(FILE *fp)
{
  static char buffer[512];

  fscanf(fp, "%[^\n]\n", buffer);
  return buffer;
}

BOOL InternalObject::ImportObject(CString filename)
{
  FILE *fp = fopen(filename, "r");

  if (fp == NULL) {
    AfxMessageBox("Failed to open import file");
    return FALSE;
  }

  // get number of datavalues
  int listsize = 0;

  while (!feof(fp)) {
    char *line = getObjectline(fp);

    if (line[0] == '#') continue;
    if (line[0] == '/' && line[1] == '/') continue;

    sscanf(line, "%d", &listsize);
    break;
  }

  int objectLength = 0;

  for (int i = 0; i < listsize; i++) {
    char *line = getObjectline(fp);

    // don't gobble lines
    if (line[0] == '#') {
      i--;
      continue;
    }
    if (line[0] == '/' && line[1] == '/') {
      i--;
      continue;
    }

    int offset;
    int size;
    int value;

    sscanf(line, "%d %d 0x%x", &offset, &size, &value);
    objectLength += size;
    switch (size) {
    case 2:
      objectData->addElement(new ObjectWord(value));
      break;
    case 4:
      objectData->addElement(new ObjectDWord(value));
      break;
    }
  }

  fclose(fp);
  DataSize = objectLength;

  ObjectData *dat = (ObjectData *)objectData->elementAt(0);
  int itemId = dat->GetValue();

  if (itemId != 0x8002) {
    CString str;
    str.Format("Not a GP2 Object! cannot include MagicNumber=%d elements=%d",
      itemId,
      listsize);
    AfxMessageBox(str);
    return FALSE;
  }

  dat = (ObjectData *)objectData->elementAt(15);
  int Size8 = dat->GetValue();
  Verticies = Size8 / 8;

  // adjust pointers

  return TRUE;
}

#define UPDATE_NEW_OFFSET(x)                                                 \
  {                                                                          \
    ObjectData *dat = (ObjectData *)objectData->elementAt(x);                \
    dat->setValue(                                                           \
      (dat->GetValue() + track->LargeNumber1.GetValue() + Offset - 0x1020)); \
  }

void InternalObject::AdjustPointers()
{
  // char buff[256];

  ObjectData *dat = (ObjectData *)objectData->elementAt(2);
  long val =
    (dat->GetValue() + track->LargeNumber1.GetValue() + Offset - 0x1020 - 4);
  dat->SetValue(val);

  dat = (ObjectData *)objectData->elementAt(3);
  val =
    (dat->GetValue() + track->LargeNumber1.GetValue() + Offset - 0x1020 - 4);
  dat->SetValue(val);

  dat = (ObjectData *)objectData->elementAt(4);
  val =
    (dat->GetValue() + track->LargeNumber1.GetValue() + Offset - 0x1020 - 4);
  dat->SetValue(val);

  dat = (ObjectData *)objectData->elementAt(5);
  val =
    (dat->GetValue() + track->LargeNumber1.GetValue() + Offset - 0x1020 - 4);
  dat->SetValue(val);

  dat = (ObjectData *)objectData->elementAt(6);
  val =
    (dat->GetValue() + track->LargeNumber1.GetValue() + Offset - 0x1020 - 4);
  dat->SetValue(val);

  dat = (ObjectData *)objectData->elementAt(7);
  val =
    (dat->GetValue() + track->LargeNumber1.GetValue() + Offset - 0x1020 - 4);
  dat->SetValue(val);

  dat = (ObjectData *)objectData->elementAt(8);
  val =
    (dat->GetValue() + track->LargeNumber1.GetValue() + Offset - 0x1020 - 4);
  dat->SetValue(val);

  dat = (ObjectData *)objectData->elementAt(9);
  val =
    (dat->GetValue() + track->LargeNumber1.GetValue() + Offset - 0x1020 - 4);
  dat->SetValue(val);

  dat = (ObjectData *)objectData->elementAt(10);
  val =
    (dat->GetValue() + track->LargeNumber1.GetValue() + Offset - 0x1020 - 4);
  dat->SetValue(val);

  // wsprintf(buff,"NewVal=%d %x",val);
  // AfxMessageBox(buff);

  // UPDATE_NEW_OFFSET(2)
  // UPDATE_NEW_OFFSET(3)
  // UPDATE_NEW_OFFSET(4)
  // UPDATE_NEW_OFFSET(5)
  // UPDATE_NEW_OFFSET(6)
  // UPDATE_NEW_OFFSET(7)
  // UPDATE_NEW_OFFSET(8)
  // UPDATE_NEW_OFFSET(9)
  // UPDATE_NEW_OFFSET(10)
}

void InternalObject::Delete()
{
  int index = track->InternalObjectOffsets->findElementIndex(this);
  CString msg;

  msg.Format("Removing Internal Object Number %d\nShall I continue?", index);
  int result = AfxMessageBox(msg, MB_YESNO);
  if (result == IDYES) {
    track->InternalObjectOffsets->removeElementRef(this);
  }
  result = AfxMessageBox(
    "Shall I automatically renumber the track object descriptions\n to cater "
    "for the removed object",
    MB_YESNO);
  int result1 = AfxMessageBox(
    "Shall I remove any track object description that reference the removed "
    "object?",
    MB_YESNO);
  int result2 = AfxMessageBox(
    "Shall I also remove track and pit objects\n that reference these track "
    "object descriptions",
    MB_YESNO);

  if (result == IDYES) {
    int trackObjectsRemoved = 0;
    int pitObjectsRemoved = 0;
    for (int i = 0; i < track->TrackObjectDefintions->size(); i++) {
      TrackObjectDefinition *def =
        (TrackObjectDefinition *)track->TrackObjectDefintions->elementAt(i);

      // only alter internal objects
      if (def->id > 17) {
        int currentIndex = def->id - 17;
        if (currentIndex > index) {
          def->id--;
        } else if (currentIndex == index && result1 == IDYES) {
          track->TrackObjectDefintions->removeElementRef(def);
          if (result2 == IDYES) {
            int objectCommand = def->locator;
            for (int j = 0; j < track->TrackSections->size(); j++) {
              TrackSection *t =
                (TrackSection *)track->TrackSections->elementAt(j);
              if (t->containsCommand(0x80)) {
                Vector *objs = t->getObjects();
                for (int k = 0; k < objs->size(); k++) {
                  TrackObject *obj = (TrackObject *)objs->elementAt(k);
                  if (obj->getLocator() == objectCommand) {
                    t->RemoveCommand(obj);
                    trackObjectsRemoved++;
                  }// if
                }// all objects
              }// contains objects
            }// all tracks
            for (int j = 0; j < track->PitlaneSections->size(); j++) {
              TrackSection *t =
                (TrackSection *)track->PitlaneSections->elementAt(j);
              if (t->containsCommand(0x80)) {
                Vector *objs = t->getObjects();
                for (int k = 0; k < objs->size(); k++) {
                  TrackObject *obj = (TrackObject *)objs->elementAt(k);
                  if (obj->getLocator() == objectCommand) {
                    t->RemoveCommand(obj);
                    pitObjectsRemoved++;
                  }// if
                }// all objects
              }// contains objects
            }// all tracks

          }// result2
        }// result1
      }// internal objects
    }// all track descrptions
    CString msg;
    msg.Format("Removed %d Track Objects\nRemoved %d Pit Objects",
      trackObjectsRemoved,
      pitObjectsRemoved);
    AfxMessageBox(msg);
  }// if remove
}

typedef struct
{
  int checkSum;
  char *name;
} t_InternalObjectName;

#include "MagicNumber.h"

static int NoMagicNumbers = sizeof(objectNames) / sizeof(objectNames[0]);

LPSTR
getObjectName(long id)
{
  char *buffer = (char *)malloc(99 * sizeof(char));

  for (int i = 0; i < NoMagicNumbers; i++) {
    if (objectNames[i].checkSum == id) {
      wsprintf(buffer, "%s %d", objectNames[i].name, id);
      return buffer;
    }
  }

  wsprintf(buffer, "Unkown Object %d", id);
  return buffer;
}

int InternalObject::CalculateDigitalSignature()
{
  int listsize = objectData->size();

  firstCheckSum = 0;
  secondCheckSum = 0;

  ObjectData *dat;

  // dat = (ObjectData*)objectData->elementAt(11);
  // for(int i=11;i<listsize;i++)
  //{
  // ObjectData *dat = (ObjectData*)objectData->elementAt(i);
  // dat = (ObjectData*)objectData->next(dat);
  // AddSumValue(dat->GetValue());
  //}

  ObjectData *last = (ObjectData *)objectData->lastElement();

  for (dat = (ObjectData *)objectData->elementAt(11); dat != last;
       dat = (ObjectData *)objectData->next(dat)) {
    // ObjectData *dat = (ObjectData*)objectData->elementAt(i);
    AddSumValue(dat->GetValue());
  }

  return (firstCheckSum + secondCheckSum);
}

LPCSTR
InternalObject::getName()
{
  if (MagicNumber == -1 || objectName == NULL) {
    MagicNumber = CalculateDigitalSignature();
    objectName = (char *)getObjectName(MagicNumber);
  }
  return objectName;
}

CString *
  InternalObject::getLowerName()
{
  if (objectLowerName == NULL) {
    objectLowerName = new CString(getName());
    objectLowerName->MakeLower();
  }
  return objectLowerName;
}

int InternalObject::getDefaultRotation()
{
  if (objectData != NULL) {
    if (objectData->size() > 12) {
      ObjectData *dat = (ObjectData *)objectData->elementAt(12);
      if (dat) {
        return (dat->GetValue());
      }
    }
  }
  return 0;
}

void InternalObject::DrawGL(CDC *pDC, double x, double y, double z, double angle, BOOL drawAxis)
{
  CreateObject();

  int connections = getNumVertexConnections();
  double SC = 1.0;

  if (track->showCulling) {
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
  } else {
    glDisable(GL_CULL_FACE);
  }

  if (track->showFilledObjects && polygonPoints != NULL) {
    int size = polygonPoints->size();

    for (int i = 0; i < size; i++) {
      Vertex *ver = (Vertex *)polygonPoints->elementAt(i);
      if (ver) {
        if (ver->GetImageLoadState() == FALSE) {
          LoadJamTexture(track, ver->bmp, ver->textureid, ver->m_VerticalRepeat, ver->unk4);
          ver->SetImageLoadState(TRUE);
        }

        if (track->showTextures) {
          glEnable(GL_TEXTURE_2D);
          TextureMap(pDC, ver->bmp);
        } else {
          glDisable(GL_TEXTURE_2D);
        }

        int colorIdx = (i % numRandomColors);
        t_Color currentColor = RandomColors[colorIdx];

        GLdouble norm[6][3];
        int ptcount = 0;

        GLdouble nx, ny, nz;

        // calculate normal first
        for (int i = 0; i < ver->points; i++) {
          int ap1 = ver->getPoint(i);
          if (ap1 == -32768) continue;
          int abap1 = abs(ap1);
          if (abap1 >= connections) continue;

          int vidx = ap1 >= 0 ? (VerticiesArray[abap1].pt1)
                              : (VerticiesArray[abap1].pt2);
          norm[ptcount][0] = ptArray[vidx]->x;
          norm[ptcount][1] = ptArray[vidx]->y;
          norm[ptcount][2] = ptArray[vidx]->z;
          ptcount++;
        }

        if (ver->points >= 3) {
          CalculateVectorNormal(norm[0], norm[1], norm[2], &nx, &ny, &nz);
          nx = nx / 100;
          ny = ny / 100;
          nz = nz / 100;
          glNormal3d(nx, ny, ny);
        }
        // Ok now you can draw the polygon

        glBegin(GL_POLYGON);
        glColor3f(currentColor.r, currentColor.g, currentColor.b);
        ver->cx = 0;
        ver->cy = 0;
        ver->cz = 0;

        BOOL ignoreNextTexCoord = FALSE;
        int cornerProcessed = 0;
        for (int i = 0; i < ver->points; i++) {
          int ap1 = ver->getPoint(i);
          int abap1 = abs(ap1);
          if (abap1 == 32768) {
            ignoreNextTexCoord = TRUE;
            continue;
          }
          if (abap1 >= connections) continue;

          int vidx = ap1 >= 0 ? (VerticiesArray[abap1].pt1)
                              : (VerticiesArray[abap1].pt2);
          double dx1 = ((double)(ptArray[vidx]->x) * SC);
          double dy1 = ((double)(ptArray[vidx]->y) * SC);
          double dz1 = ((double)(ptArray[vidx]->z) * SC);
          if (!ignoreNextTexCoord) {
            switch (cornerProcessed) {
            default:
            case 0:
              glTexCoord2f(1.0f, 0.0f);
              break;
            case 1:
              glTexCoord2f(0.0f, 0.0f);
              break;
            case 2:
              if (ver->points == 3) {
                glTexCoord2f(0.5f, 1.0f);
              } else {
                glTexCoord2f(0.0f, 1.0f);
              }
              break;
            case 3:
              glTexCoord2f(1.0f, 1.0f);
              break;
            }
            cornerProcessed++;

            glVertex3d(dx1, dy1, dz1);
            if (i > 0) {
              ver->cx += dx1;
              ver->cy += dy1;
              ver->cz += dz1;
              ver->cx /= 2.0;
              ver->cy /= 2.0;
              ver->cz /= 2.0;
            }
          } else {
            ignoreNextTexCoord = FALSE;
          }
        }

        glEnd();
        glDisable(GL_TEXTURE_2D);
        if (track->showNormals) {
          glColor3d(0.0f, 0.0f, 0.0f);
          glBegin(GL_LINES);
          glVertex3d(ver->cx, ver->cy, ver->cz);
          glVertex3d(ver->cx + (0.00001 * ver->nx),
            ver->cy + (0.00001 * ver->ny),
            ver->cz + (0.00001 * ver->nz));
          glEnd();
        }
      }
    }
  }

  if (track->showLines) {
    glColor3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < connections; i++) {
      int pt1 = VerticiesArray[i].pt1;
      int pt2 = VerticiesArray[i].pt2;
      glBegin(GL_LINES);

      glVertex3d(((double)(ptArray[pt1]->x) * SC),
        ((double)(ptArray[pt1]->y) * SC),
        ((double)(ptArray[pt1]->z) * SC));
      glColor3f(0.0f, 0.0f, 0.0f);
      glVertex3d(((double)(ptArray[pt2]->x) * SC),
        ((double)(ptArray[pt2]->y) * SC),
        ((double)(ptArray[pt2]->z) * SC));
      glEnd();
    }
  }

  /*
  if (drawAxis)
  {
      // draw carthesian axes
      glBegin(GL_LINES);
          // red x axis
          glColor3f(1.f,0.f,0.f);
          glVertex3f(0.0f,0.0f,0.0f);
          glVertex3d(1.0*MaxSize,0.0f,0.0f);
          glVertex3d(1.0*MaxSize,0.0f,0.0f);
          glVertex3f(0.9f,0.1f,0.0f);
          glVertex3d(1.0*MaxSize,0.0f,0.0f);
          glVertex3f(0.9f,-0.1f,0.0f);
          // green y axis
          glColor3f(0.f,1.f,0.f);
          glVertex3f(0.0f,0.0f,0.0f);
          glVertex3d(0.0f,1.0*MaxSize,0.0f);
          glVertex3d(0.0f,1.0*MaxSize,0.0f);
          glVertex3f(0.1f,0.9f,0.0f);
          glVertex3d(0.0f,1.0*MaxSize,0.0f);
          glVertex3f(-0.1f,0.9f,0.0f);
          // blue z axis
          glColor3f(0.f,0.f,1.f);
          glVertex3f(0.0f,0.0f,0.0f);
          glVertex3d(0.0f,0.0f,1.0*MaxSize);
          glVertex3d(0.0f,0.0f,1.0*MaxSize);
          glVertex3f(0.0f,0.1f,0.9f);
          glVertex3d(0.0f,0.0f,1.0*MaxSize);
          glVertex3f(0.0f,-0.1f,0.9f);
      glEnd();
  }



  for(i=0;i<Verticies;i++)
  {
          delete ptArray[i];
  }
  free(ptArray);
  delete[] VerticiesArray;
  */
}

void ObjectData::Open(CTrackTree *tree)
{
  AfxMessageBox("Edit me!");
}

void InternalObject::TextureMap(CDC *pDC, CBitmap &cbmp)
{
  BITMAP bmp;
  cbmp.GetBitmap(&bmp);

  if (bmp.bmWidth > 1024) return;
  if (bmp.bmHeight > 1024) return;

  CDC *bitmapDc = new CDC;
  bitmapDc->CreateCompatibleDC(pDC);

  CBitmap *oldBitMap = (CBitmap *)bitmapDc->SelectObject(&cbmp);

  unsigned char *lpvBits = new unsigned char[bmp.bmWidth * bmp.bmHeight * 3];
  BITMAPINFO *bmpinfo = new BITMAPINFO;

  bmpinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmpinfo->bmiHeader.biWidth = bmp.bmWidth;
  bmpinfo->bmiHeader.biHeight = bmp.bmHeight;
  bmpinfo->bmiHeader.biPlanes = 1;
  bmpinfo->bmiHeader.biBitCount = 24;
  bmpinfo->bmiHeader.biCompression = BI_RGB;
  bmpinfo->bmiHeader.biSizeImage = bmp.bmWidth * bmp.bmHeight * 3;
  bmpinfo->bmiHeader.biXPelsPerMeter = 0;
  bmpinfo->bmiHeader.biYPelsPerMeter = 0;
  bmpinfo->bmiHeader.biClrImportant = 0;
  bmpinfo->bmiHeader.biClrUsed = 0;

  if (!::GetDIBits(bitmapDc->GetSafeHdc(), HBITMAP(cbmp), 0, bmp.bmHeight, lpvBits, bmpinfo, DIB_RGB_COLORS)) {
    AfxMessageBox("GetDIBits error");
  }

  unsigned int size = (bmpinfo->bmiHeader.biSizeImage);

  for (unsigned int i = 0; i < size; i += 3) {
    unsigned char *ptr = lpvBits + i;
    unsigned char *r = ptr;
    unsigned char *g = ptr + 1;
    unsigned char *b = ptr + 2;

    unsigned char red = *r;
    unsigned char blue = *b;
    *b = red;
    *r = blue;
  }

  GLint val;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &val);

  glTexImage2D(GL_TEXTURE_2D, 0, 3, bmp.bmWidth, bmp.bmHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, lpvBits);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);

  bitmapDc->SelectObject(oldBitMap);
  delete bitmapDc;
  delete[] lpvBits;
  delete bmpinfo;
}

void InternalObject::LoadJamTexture(GPTrack *mytrack, CBitmap &output, int idx, int hres, int vres)
{
  // int vr = vres/256;
  // int hr = hres/256;
  // if (vres == -99 || (vres % 256) != 0) vr = 1;
  // if (hres == -99 || (hres % 256) != 0) hr = 1;

  int vr = 1;
  int hr = 1;

  int id = IDB_TESTTEXTURE_128;
  int td = 128;
  int currentRes = 0;

  /*
  switch(currentRes)
  {
      case 0:
                  id = IDB_TESTTEXTURE_64;
                  td = 64;
                  break;
  }
  */

  CBitmap temp;
  output.LoadBitmap(id);
  JAM *jam = mytrack->GetJAM(idx);
  if (jam == NULL) return;
  int sidx = jam->getImageIndex(idx);
  jam->LoadSingleBMPImage(temp, jam->getImageWidth(sidx), jam->getImageHeight(sidx), sidx);
  Mak256SquareImage(temp, output, jam->getImageWidth(sidx), jam->getImageHeight(sidx), hr, vr, td, td);
}

void InternalObject::Draw2D(Display *g, double x, double y, double z, double angle, BOOL drawAxis)
{
  CreateObject();

  int connections = getNumVertexConnections();

  double SC = 0.0007;

  g->setColor(BLUE_PEN);
  double cAngle = cos(angle);
  double sAngle = sin(angle);

  double x1, y1, x2, y2, xa, ya, xb, yb;
  int pt1, pt2;

  for (int i = 0; i < connections; i++) {
    pt1 = VerticiesArray[i].pt1;
    pt2 = VerticiesArray[i].pt2;

    x1 = ((double)(ptArray[pt1]->x) * SC);
    y1 = ((double)(ptArray[pt1]->y) * SC);
    x2 = ((double)(ptArray[pt2]->x) * SC);
    y2 = ((double)(ptArray[pt2]->y) * SC);
    xa = x1 * cAngle - y1 * sAngle;
    ya = x1 * sAngle + y1 * cAngle;
    xb = x2 * cAngle - y2 * sAngle;
    yb = x2 * sAngle + y2 * cAngle;

    xa += x;
    ya += y;
    xb += x;
    yb += y;

    g->drawLine(xa, ya, xb, yb);
  }
}

void InternalObject::CreateObject()
{
  if (created) return;

  XYZData = getListOffset(getXYZOffset());
  int objOffset = XYZData;
  int idx = objOffset;

  double MaxSize = 0.0;
  double XMaxSize = 0.0;
  double YMaxSize = 0.0;
  double ZMaxSize = 0.0;

  int scaleIDx = getListOffset(getScaleOffset());

  // Point3D **ptArray = (Point3D**)malloc(Verticies*sizeof(Point3D*));
  ptArray = (Point3D **)malloc(Verticies * sizeof(Point3D *));
  for (int i = 0; i < Verticies; i++) {
    ObjectData *Xdat = (ObjectData *)objectData->elementAt(idx++);
    ObjectData *Ydat = (ObjectData *)objectData->elementAt(idx++);
    ObjectData *Zdat = (ObjectData *)objectData->elementAt(idx++);
    ObjectData *Udat = (ObjectData *)objectData->elementAt(idx++);

    int X = Xdat->GetValue();
    int Y = Ydat->GetValue();
    int Z = Zdat->GetValue();

    if (Z > 0x8000) Z = 0xFFFF - Z;

    if ((X & 0xFF00) == 0x8000) {
      int ptidx = (X & 0xFF);
      ptArray[i] = new Point3D(ptArray[ptidx]);
      ptArray[i]->z = (double)Z;
    } else {
      if (X > 0) {
        if (X > 0x80 && X < 0xFF) {
          int id = ((X - 0x84) / 2) + 78;
          int scaleIDx = getListOffset(id);
          ObjectData *Xdat = (ObjectData *)objectData->elementAt(scaleIDx);
          X = Xdat->GetValue();
          X = -X;
        } else {
          int id = ((X - 0x4) / 2) + 78;
          int scaleIDx = getListOffset(id);
          ObjectData *Xdat = (ObjectData *)objectData->elementAt(scaleIDx);
          X = Xdat->GetValue();
        }
      }

      if (Y > 0) {
        if (Y > 0x80 && Y < 0xFF) {
          int id = ((Y - 0x84) / 2) + 78;
          int scaleIDx = getListOffset(id);
          ObjectData *Ydat = (ObjectData *)objectData->elementAt(scaleIDx);
          Y = Ydat->GetValue();
          Y = -Y;
        } else {
          int id = ((Y - 0x4) / 2) + 78;
          int scaleIDx = getListOffset(id);
          ObjectData *Ydat = (ObjectData *)objectData->elementAt(scaleIDx);
          Y = Ydat->GetValue();
        }
      }

      ptArray[i] = new Point3D((double)X, (double)Y, (double)Z);

      XMaxSize = max(XMaxSize, (double)X);
      YMaxSize = max(YMaxSize, (double)Y);
      ZMaxSize = max(ZMaxSize, (double)Z);
    }
  }

  idx = getListOffset(getVertexOffset());

  int connections = getNumVertexConnections();

  // MaxSize = max(XMaxSize,max(YMaxSize,ZMaxSize));

  // double sizeMax=1.5;

  // double SC = (2*sizeMax)/MaxSize;
  // double SC=5.0;
  double SC = 1.0;
  // double midx = 0;
  // double midy = 0;
  // double midz = 0;

  // t_SingleVertex *VerticiesArray = new t_SingleVertex[connections];
  VerticiesArray = new t_SingleVertex[connections];

  for (int i = 0; i < connections; i++) {
    ObjectData *PtPt = (ObjectData *)objectData->elementAt(idx++);
    idx++;
    // ObjectData *Unused = (ObjectData*)objectData->elementAt(idx++);

    int verticies = PtPt->GetValue();

    int pt1 = LOBYTE(verticies);
    int pt2 = HIBYTE(verticies);

    if (pt1 < 0) pt1 = 0;
    if (pt1 >= Verticies) pt1 = Verticies - 1;

    if (pt2 < 0) pt2 = 0;
    if (pt2 >= Verticies) pt2 = Verticies - 1;

    VerticiesArray[i].pt1 = pt1;
    VerticiesArray[i].pt2 = pt2;
  }

  Parse();

  if (polygonPoints != NULL) {
    int size = polygonPoints->size();

    for (int i = 0; i < size; i++) {
      Vertex *ver = (Vertex *)polygonPoints->elementAt(i);

      if (ver) {
        int ap1 = abs(ver->getPoint(0));
        int ap2 = abs(ver->getPoint(1));
        int ap3 = abs(ver->getPoint(2));
        int ap4 = abs(ver->getPoint(3));

        if (ver->GetImageLoadState() == FALSE) {
          LoadJamTexture(track, ver->bmp, ver->textureid, ver->m_VerticalRepeat, ver->unk4);
          ver->SetImageLoadState(TRUE);
        }

        GLdouble norm[6][3];
        int ptcount = 0;
        GLdouble nx, ny, nz;

        // calculate normal first
        for (int i = 0; i < ver->points; i++) {
          int ap1 = ver->getPoint(i);
          if (ap1 == -32768) continue;
          int abap1 = abs(ap1);
          // if (abap1 > myobject->Verticies) continue;
          if (abap1 >= connections) continue;

          int vidx = ap1 >= 0 ? (VerticiesArray[abap1].pt1)
                              : (VerticiesArray[abap1].pt2);
          if (ptcount < 6) {
            norm[ptcount][0] = ptArray[vidx]->x;
            norm[ptcount][1] = ptArray[vidx]->y;
            norm[ptcount][2] = ptArray[vidx]->z;
          }
          ptcount++;
        }

        if (ver->points >= 3) {
          CalculateVectorNormal(norm[0], norm[1], norm[2], &nx, &ny, &nz);
          ver->nx = nx / 100;
          ver->ny = ny / 100;
          ver->nz = nz / 100;
        }
        // Ok now you can draw the polygon

        ver->cx = 0;
        ver->cy = 0;
        ver->cz = 0;

        for (int i = 0; i < ver->points; i++) {
          int ap1 = ver->getPoint(i);
          if (ap1 == -32768) {
            continue;
          }
          int abap1 = abs(ap1);
          if (abap1 >= connections) continue;

          int vidx = ap1 >= 0 ? (VerticiesArray[abap1].pt1)
                              : (VerticiesArray[abap1].pt2);
          double dx1 = ((double)(ptArray[vidx]->x) * SC);
          double dy1 = ((double)(ptArray[vidx]->y) * SC);
          double dz1 = ((double)(ptArray[vidx]->z) * SC);
          if (i > 0) {
            ver->cx += dx1;
            ver->cy += dy1;
            ver->cz += dz1;
            ver->cx /= 2.0;
            ver->cy /= 2.0;
            ver->cz /= 2.0;
          }
        } /* FOR */
      }
    }
  }

  created = TRUE;
}

void InternalObject::Draw2DExternal(Display *g, double x, double y, double z, double angle, BOOL drawAxis)
{
  double SC = 0.0007;

  g->setColor(RED_PEN);
  double cAngle = cos(angle);
  double sAngle = sin(angle);

  double x1, y1, x2, y2, xa, ya, xb, yb;
  double x1a = -2048;
  double x2a = 2048;
  double y1a = 0;
  double y2a = 0;

  x1 = ((double)(x1a)*SC);
  y1 = ((double)(y1a)*SC);
  x2 = ((double)(x2a)*SC);
  y2 = ((double)(y2a)*SC);
  xa = x1 * cAngle - y1 * sAngle;
  ya = x1 * sAngle + y1 * cAngle;
  xb = x2 * cAngle - y2 * sAngle;
  yb = x2 * sAngle + y2 * cAngle;

  xa += x;
  ya += y;
  xb += x;
  yb += y;

  g->drawLine(xa, ya, xb, yb);
}

void InternalObject::Parse()
{
  if (polygonPoints) delete polygonPoints;
  polygonPoints = new Vector("PolygonPoints", __LINE__);
  int idx = getListOffset(getTextureOffset());
  int idxend = getListOffset(getTextureEndOffset());
  int idxStart = idx;

  int numBytes = (getTextureEndOffset() - getTextureOffset() / 4);
  int numText = idxend - idx;

  if (numText > 0) {
    while (((idx - idxStart) < numBytes)) {
      CString buffer;

      int num = GET_INT(idx++);
      int unk1 = GET_INT(idx++);

      int unk2, unk3, unk4;
      int unk5, unk6;

      int texture;
      int cmd;

      unk2 = -99;
      unk3 = -99;
      unk4 = -99;
      unk5 = -99;
      unk6 = -99;

      switch (unk1) {
      case 0x20:
      case 0x80: {
        // an object repeater
        cmd = 0x80;
        texture = 0x80;
        unk2 = GET_INT(idx++);
        unk3 = GET_INT(idx++);
        unk4 = GET_INT(idx++);

        if (subObjects == NULL)
          subObjects = new Vector("SubObjects", __LINE__);

        SubObject *subObj = new SubObject();
        subObj->unk1 = unk2;
        subObj->unk2 = unk3;
        subObj->unk3 = unk4;
        subObjects->addElement(subObj);
      } break;
      case 0x22:
      // case 0x24:
      // case 0x2b:
      // case 0x2e:
      case 0x82: {
        // an object repeater
        cmd = 0x82;
        texture = 0x82;
        unk2 = GET_INT(idx++);
        unk3 = GET_INT(idx++);
        unk4 = GET_INT(idx++);

        if (subObjects == NULL)
          subObjects = new Vector("SubObjects", __LINE__);

        SubObject *subObj = new SubObject();
        subObj->unk1 = unk2;
        subObj->unk2 = unk3;
        subObj->unk3 = unk4;
        subObjects->addElement(subObj);
      } break;
      case 0x86: {
        // an object repeater
        cmd = 0x86;
        texture = 0x86;
        unk2 = GET_INT(idx++);
        unk3 = GET_INT(idx++);
        unk4 = GET_INT(idx++);
        unk5 = GET_INT(idx++);

        if (subObjects == NULL)
          subObjects = new Vector("SubObjects", __LINE__);

        SubObject *subObj = new SubObject();
        subObj->unk1 = unk2;
        subObj->unk2 = unk3;
        subObj->unk3 = unk4;
        subObjects->addElement(subObj);
      } break;
      case 0x88: {
        // an object repeater
        cmd = 0x88;
        texture = 0x88;
        unk2 = GET_INT(idx++);
        unk3 = GET_INT(idx++);
        unk4 = GET_INT(idx++);

        if (subObjects == NULL)
          subObjects = new Vector("SubObjects", __LINE__);

        SubObject *subObj = new SubObject();
        subObj->unk1 = unk2;
        subObj->unk2 = unk3;
        subObj->unk3 = unk4;
        subObjects->addElement(subObj);
      } break;
      case 0x1:
      case 0x2:
      case 0x3:
      case 0x4:
      case 0x5:
      case 0x6:
      case 0x7:
      case 0x8:
      case 0x9:
      case 0xa:
      case 0xb:
      case 0xc:
      case 0xd:
      case 0xe:
      case 0xf: {
        int vcount = 0;

        Vertex *ver = new Vertex(texture, unk2, unk3, unk4, unk5, unk6);
        {
          int val = GET_INT(idx++);
          while (val != 0) {
            if (val == -32768) val = GET_INT(idx++);
            ver->addPoint(val);
            val = GET_INT(idx++);
          }
          idx--;
        }
        polygonPoints->addElement(ver);
        int term = GET_INT(idx++);
      } break;

      case 0x10:
      case 0x11:
      case 0x12:
      case 0x13:
      case 0x14:
      case 0x15:
      case 0x16:
      case 0x17:
      case 0x18:
      case 0x19:
      case 0x1a:
      case 0x1b:
      case 0x1c:
      case 0x1d:
      case 0x1e:
      case 0x1f: {
        texture = GET_INT(idx++);
        int rotation = GET_INT(idx++);

        Vertex *ver = new Vertex(texture, unk2, unk3, unk4, unk5, unk6);

        int horizontal = 256;
        int vertical = 256;

        if (rotation >= 0x80) {
          int flags = GET_INT(idx++);

          if (flags == 0x11) {
            horizontal = GET_INT(idx++);
            vertical = GET_INT(idx++);
            int a = GET_INT(idx++);
          } else if (flags == 0x10) {
            // read nothing
            horizontal = GET_INT(idx++);
            // int vertical   = GET_INT(idx++);
          } else if (flags == 0x1) {
            horizontal = GET_INT(idx++);
            vertical = GET_INT(idx++);
          } else if (flags == 0x3) {
            horizontal = GET_INT(idx++);
            vertical = GET_INT(idx++);
            int a = GET_INT(idx++);
            int b = GET_INT(idx++);
          } else if (flags == 0x4) {
            horizontal = GET_INT(idx++);
          } else if (flags == 0x5) {
            horizontal = GET_INT(idx++);
            vertical = GET_INT(idx++);
            int a = GET_INT(idx++);
          } else if (flags == 0x7) {
            horizontal = GET_INT(idx++);
            vertical = GET_INT(idx++);
            int unkflag1 = GET_INT(idx++);
            int unkflag2 = GET_INT(idx++);
            int unkflag3 = GET_INT(idx++);
          } else if (flags == 0x13) {
            horizontal = GET_INT(idx++);
            vertical = GET_INT(idx++);
            int unkflag1 = GET_INT(idx++);
            int unkflag2 = GET_INT(idx++);
            int unkflag3 = GET_INT(idx++);
          } else if (flags == 0x14) {
            horizontal = GET_INT(idx++);
            vertical = GET_INT(idx++);
            int unkflag1 = GET_INT(idx++);
            int unkflag2 = GET_INT(idx++);
            int unkflag3 = GET_INT(idx++);
          } else if (flags == 0x15) {
            horizontal = GET_INT(idx++);
            vertical = GET_INT(idx++);
            int unkflag1 = GET_INT(idx++);
            int unkflag2 = GET_INT(idx++);
            int unkflag3 = GET_INT(idx++);
          } else if (flags == 0x0) {
          } else {
            CString msg;
            msg.Format("Unknown Flags %x in %s cmd=%x", flags, getName(), unk1);
            AfxMessageBox(msg);
          }
        }

        int vcount = 0;

        ver->m_HorizontalRepeat = horizontal;
        ver->m_VerticalRepeat = vertical;

        // Vertex *ver = new Vertex(texture,unk2,unk3,unk4,unk5,unk6);
        {
          int val = GET_INT(idx++);
          while (val != 0) {
            if (val == -32768) val = GET_INT(idx++);
            ver->addPoint(val);
            val = GET_INT(idx++);
          }
          idx--;
        }
        polygonPoints->addElement(ver);
        int term = GET_INT(idx++);
      } break;

      default: {
        if (unk1 == 0) return;
        if (unk1 > 0xFF) return;
        CString msg;
        msg.Format("Unknown Command %d %x in %s   cmd=%d", unk1, unk1, getName(), num);
        AfxMessageBox(msg);
        return;
      } break;
      }
    }
  }
}

void InternalObject::ReplaceObject(CString filename)
{
  FILE *fp = fopen(filename, "r");

  if (fp == NULL) {
    AfxMessageBox("Failed to open import file");
    return;
  }

  // clear out old object
  objectData->removeAllElements();

  // get number of datavalues
  int listsize = 0;

  while (!feof(fp)) {
    char *line = getObjectline(fp);

    if (line[0] == '#') continue;
    if (line[0] == '/' && line[1] == '/') continue;

    sscanf(line, "%d", &listsize);
    break;
  }

  int objectLength = 0;
  created = FALSE;

  for (int i = 0; i < listsize; i++) {
    char *line = getObjectline(fp);

    // don't gobble lines
    if (line[0] == '#') {
      i--;
      continue;
    }
    if (line[0] == '/' && line[1] == '/') {
      i--;
      continue;
    }

    int offset;
    int size;
    int value;

    sscanf(line, "%d %d 0x%x", &offset, &size, &value);
    objectLength += size;
    switch (size) {
    case 2:
      objectData->addElement(new ObjectWord(value));
      break;
    case 4:
      objectData->addElement(new ObjectDWord(value));
      break;
    }
  }

  fclose(fp);
  DataSize = objectLength;

  ObjectData *dat = (ObjectData *)objectData->elementAt(0);
  int itemId = dat->GetValue();

  if (itemId != 0x8002) {
    CString str;
    str.Format("Not a GP2 Object! cannot include MagicNumber=%d elements=%d",
      itemId,
      listsize);
    AfxMessageBox(str);
    return;
  }

  dat = (ObjectData *)objectData->elementAt(15);
  int Size8 = dat->GetValue();
  Verticies = Size8 / 8;
  MagicNumber = -1;

  return;
}

#include <fstream>
#include "vml.h"

void InternalObject::DrawVML(std::ofstream &file, double x, double y, double z, double angle, BOOL drawAxis)
{
  CreateObject();

  int connections = getNumVertexConnections();
  double SC = 1.0;

  if (track->showCulling) {
    // glCullFace(GL_FRONT);
    // glEnable(GL_CULL_FACE);
  } else {
    // glDisable(GL_CULL_FACE);
  }

  if (track->showFilledObjects && polygonPoints != NULL) {
    int size = polygonPoints->size();

    for (int i = 0; i < size; i++) {
      Vertex *ver = (Vertex *)polygonPoints->elementAt(i);
      if (ver) {
        if (ver->GetImageLoadState() == FALSE) {
          LoadJamTexture(track, ver->bmp, ver->textureid, ver->m_VerticalRepeat, ver->unk4);
          ver->SetImageLoadState(TRUE);
        }

        if (track->showTextures) {
          // glEnable(GL_TEXTURE_2D);
          // TextureMap(pDC,ver->bmp);
        } else {
          // glDisable(GL_TEXTURE_2D);
        }

        int colorIdx = (i % numRandomColors);
        t_Color currentColor = RandomColors[colorIdx];

        GLdouble norm[6][3];
        int ptcount = 0;

        GLdouble nx, ny, nz;

        // calculate normal first
        for (int i = 0; i < ver->points; i++) {
          int ap1 = ver->getPoint(i);
          if (ap1 == -32768) continue;
          int abap1 = abs(ap1);
          if (abap1 >= connections) continue;

          int vidx = ap1 >= 0 ? (VerticiesArray[abap1].pt1)
                              : (VerticiesArray[abap1].pt2);
          norm[ptcount][0] = ptArray[vidx]->x;
          norm[ptcount][1] = ptArray[vidx]->y;
          norm[ptcount][2] = ptArray[vidx]->z;
          ptcount++;
        }

        if (ver->points >= 3) {
          CalculateVectorNormal(norm[0], norm[1], norm[2], &nx, &ny, &nz);
          nx = nx / 100;
          ny = ny / 100;
          nz = nz / 100;
          // glNormal3d(nx,ny,ny);
        }
        // Ok now you can draw the polygon

        vmlBegin(GL_POLYGON);
        vmlColor3f(currentColor.r, currentColor.g, currentColor.b);
        ver->cx = 0;
        ver->cy = 0;
        ver->cz = 0;

        BOOL ignoreNextTexCoord = FALSE;
        int cornerProcessed = 0;
        for (int i = 0; i < ver->points; i++) {
          int ap1 = ver->getPoint(i);
          int abap1 = abs(ap1);
          if (abap1 == 32768) {
            ignoreNextTexCoord = TRUE;
            continue;
          }
          if (abap1 >= connections) continue;

          int vidx = ap1 >= 0 ? (VerticiesArray[abap1].pt1)
                              : (VerticiesArray[abap1].pt2);
          double dx1 = ((double)(ptArray[vidx]->x) * SC);
          double dy1 = ((double)(ptArray[vidx]->y) * SC);
          double dz1 = ((double)(ptArray[vidx]->z) * SC);
          if (!ignoreNextTexCoord) {
            switch (cornerProcessed) {
            default:
            case 0:
              vmlTexCoord2f(1.0f, 0.0f);
              break;
            case 1:
              vmlTexCoord2f(0.0f, 0.0f);
              break;
            case 2:
              if (ver->points == 3) {
                vmlTexCoord2f(0.5f, 1.0f);
              } else {
                vmlTexCoord2f(0.0f, 1.0f);
              }
              break;
            case 3:
              vmlTexCoord2f(1.0f, 1.0f);
              break;
            }
            cornerProcessed++;

            vmlVertex3d(dx1, dy1, dz1);
            if (i > 0) {
              ver->cx += dx1;
              ver->cy += dy1;
              ver->cz += dz1;
              ver->cx /= 2.0;
              ver->cy /= 2.0;
              ver->cz /= 2.0;
            }
          } else {
            ignoreNextTexCoord = FALSE;
          }
        }

        vmlEnd(file);
        // glDisable(GL_TEXTURE_2D);
        /*
        if (track->showNormals)
{
      glColor3d(0.0f,0.0f,0.0f);
      glBegin(GL_LINES);
      glVertex3d(ver->cx,ver->cy,ver->cz);
      glVertex3d(ver->cx+(0.00001*ver->nx),ver->cy+(0.00001*ver->ny),ver->cz+(0.00001*ver->nz));
      glEnd();
}
        */
      }
    }
  }

  if (track->showLines) {
    glColor3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < connections; i++) {
      int pt1 = VerticiesArray[i].pt1;
      int pt2 = VerticiesArray[i].pt2;
      glBegin(GL_LINES);

      glVertex3d(((double)(ptArray[pt1]->x) * SC),
        ((double)(ptArray[pt1]->y) * SC),
        ((double)(ptArray[pt1]->z) * SC));
      glColor3f(0.0f, 0.0f, 0.0f);
      glVertex3d(((double)(ptArray[pt2]->x) * SC),
        ((double)(ptArray[pt2]->y) * SC),
        ((double)(ptArray[pt2]->z) * SC));
      glEnd();
    }
  }

  /*
  if (drawAxis)
  {
      // draw carthesian axes
      glBegin(GL_LINES);
          // red x axis
          glColor3f(1.f,0.f,0.f);
          glVertex3f(0.0f,0.0f,0.0f);
          glVertex3d(1.0*MaxSize,0.0f,0.0f);
          glVertex3d(1.0*MaxSize,0.0f,0.0f);
          glVertex3f(0.9f,0.1f,0.0f);
          glVertex3d(1.0*MaxSize,0.0f,0.0f);
          glVertex3f(0.9f,-0.1f,0.0f);
          // green y axis
          glColor3f(0.f,1.f,0.f);
          glVertex3f(0.0f,0.0f,0.0f);
          glVertex3d(0.0f,1.0*MaxSize,0.0f);
          glVertex3d(0.0f,1.0*MaxSize,0.0f);
          glVertex3f(0.1f,0.9f,0.0f);
          glVertex3d(0.0f,1.0*MaxSize,0.0f);
          glVertex3f(-0.1f,0.9f,0.0f);
          // blue z axis
          glColor3f(0.f,0.f,1.f);
          glVertex3f(0.0f,0.0f,0.0f);
          glVertex3d(0.0f,0.0f,1.0*MaxSize);
          glVertex3d(0.0f,0.0f,1.0*MaxSize);
          glVertex3f(0.0f,0.1f,0.9f);
          glVertex3d(0.0f,0.0f,1.0*MaxSize);
          glVertex3f(0.0f,-0.1f,0.9f);
      glEnd();
  }



  for(i=0;i<Verticies;i++)
  {
          delete ptArray[i];
  }
  free(ptArray);
  delete[] VerticiesArray;
  */
}
