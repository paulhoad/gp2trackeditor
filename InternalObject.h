
#ifndef _INCLUDED_INTERNAL_OBJECT_H
#define _INCLUDED_INTERNAL_OBJECT_H

#define GP_OBJECT_BYTE 1
#define GP_OBJECT_WORD 2
#define GP_OBJECT_DWORD 4

#include "Observer.h"
#include "Vector.h"
#include "GPTrack.h"
#include "Vertex.h"
#include <fstream>

class ObjectData : public CObject
  , public DataValue
  , public Observer
{
public:
  ObjectData(int type = GP_OBJECT_DWORD, int val = 0)
    : DataValue(val), Observer(NULL, t_ObjectData, NULL), objtype(type)
  {}

  // virtual int getValue(){return 0;}
  // virtual void setValue(int val){}

  int getType()
  {
    return objtype;
  }

  int getSize()
  {
    return objtype;
  }

  void
    exportHex(FILE *fp, LPCSTR msg);
  void exportFile(FILE *fp, LPCSTR msg);

  void
    Open(CTrackTree *tree);

  int objtype;
};

class ObjectWord : public ObjectData
{
public:
  ObjectWord(int val) : ObjectData(GP_OBJECT_WORD, val)
  // value(val)
  {}

  /*
           int getValue()
           {
                   return value;
           }

           void setValue(int val)
           {
                   value = val;
           }

int value;
*/
};

class ObjectDWord : public ObjectData
{
public:
  ObjectDWord(int val) : ObjectData(GP_OBJECT_DWORD, val)
  // value(val)
  {}

  /*
  int getValue()
  {
          return value;
  }

  void setValue(int val)
  {
          value = val;
  }

int value;
*/
};

class InternalObject : public Observer
  , public CObject
{
private:
  LPSTR objectName;

public:
  int Verticies;
  int XYZData;
  int VertexData;
  Vector *objectData;
  Vector *treeItemData;
  Vector *subObjects;
  int DataSize;
  CDocument *pDoc;
  HTREEITEM SubTree;

  CString *objectLowerName;

  InternalObject(GPTrack *mytrack, CDocument *Doc, int offset, int endoffset)
    : Offset(offset),
      pDoc(Doc),
      Observer(mytrack, t_InternalObject, NULL),
      treeItemData(NULL)
  {
    objectData = new Vector("Internal Objects", __LINE__);
    DataSize = endoffset - offset;
    MagicNumber = -1;
    SubTree = NULL;
    polygonPoints = NULL;
    objectName = NULL;
    objectLowerName = NULL;
    created = FALSE;
    VerticiesArray = NULL;
    ptArray = NULL;
    subObjects = NULL;
  }

  char *
    getIntObjectName()
  {
    return objectName;
  }

  void
    setObjectName(char *str)
  {
    objectName = str;
  }

  void
    Expand(CTrackTree *tracktree, HTREEITEM node);
  void
    LoadTree(CTrackTree *tree, HTREEITEM node);
  void
    LoadTreeTop(CTrackTree *tree, HTREEITEM node);
  void
    LoadJamTexture(GPTrack *mytrack, CBitmap &output, int idx, int hres, int vres);
  void
    TextureMap(CDC *pDC, CBitmap &cbmp);

  void
    Parse();

  Vector *polygonPoints;

  int getXYZOffset();
  int getVertexOffset();

  int getNumVertexConnections();
  int getScaleOffset();
  int getScaleEndOffset();
  int getTextureOffset();
  int getTextureEndOffset();
  int getDefaultRotation();
  int getEndVertexOffset();
  int getEndOffset();

  int getListOffset(int pos);

  CDocument *
    GetDocument()
  {
    return pDoc;
  }

  virtual ~InternalObject();

  int getOffset()
  {
    return Offset;
  }

  void
    setOffset(int off)
  {
    Offset = off;
  }

  int getSize()
  {
    return DataSize;
  }

  int getNumPoints()
  {
    return Verticies;
  }

  void
    Delete();

  void
    ReadObject(GPTrack *track);
  void
    WriteObject(int offset);

  BOOL
    ImportFromFile(LPCSTR filename);

  void
    exportFile();

  int write(GPTrack *track, int offset)
  {
    // using original offset until we need more objects
    offset = Offset;
    // TRACE("%x %d\n",offset,objectData->size());
    // int listsize = objectData->size();

    ObjectData *last = (ObjectData *)objectData->lastElement();
    ObjectData *first = (ObjectData *)objectData->elementAt(0);
    ObjectData *dat;

    int size = objectData->size();
    int count = 0;

    for (dat = first; count < size;
         dat = (ObjectData *)objectData->next(dat), count++) {
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
    return offset;
  }

  int Open();

  int Offset;
  int itemId;
  int PossibleId;
  int MagicNumber;

  void
    ExportObject(CString filename);
  BOOL
    ImportObject(CString filename);
  void
    ReplaceObject(CString filename);

  void
    AdjustPointers();

  LPCSTR
  getName();
  CString *
    getLowerName();

  int CalculateDigitalSignature();

  void
    AddSumValue(int da)
  {
    // where unsigned shorts
    int d1, d2, d3;
    d2 = secondCheckSum;
    // was short int
    d1 = (int)da;
    firstCheckSum += d1;
    d1 = d1 & 0x000000FF;
    d3 = d2 & 0x0000E000;
    d3 = d3 >> 13;
    d2 = d2 << 3;
    d2 = d2 | d3;
    d2 = d2 + d1;
    secondCheckSum = d2;
  }

  short firstCheckSum;
  short secondCheckSum;

  void
    DrawVML(std::ofstream &file, double x, double y, double z, double angle, BOOL drawAxis);
  void
    DrawGL(CDC *pDC, double x, double y, double z, double angle, BOOL drawAxis);
  void
    Draw2D(Display *g, double x, double y, double z, double angle, BOOL drawAxis);

  static void
    Draw2DExternal(Display *g, double x, double y, double z, double angle, BOOL drawAxis);

  void
    CreateObject();
  BOOL created;

  t_SingleVertex *VerticiesArray;
  Point3D **ptArray;

  void
    UpdateStructure();
};

#endif