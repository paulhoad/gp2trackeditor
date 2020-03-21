

#include "stdafx.h"
#include "TrackObjectDefinition.h"
#include "GPTrack.h"
#include "ObjectEdit.h"
#include "Vector.h"
#include "TrackEditor.h"
#include "TrackOpenGLView.h"

#include <gl\gl.h>
#include <gl\glu.h>

#include "ObjectLookup.h"

class ObjecDesc : public CObject
{
public:
  ObjecDesc(int locator, char *_name)
  {
    offset = locator;
    name = _strdup(_name);
  }

  int offset;
  char *name;
};

static Vector *ObjectDescriptions = NULL;

void ImportObjectDefinitions(CString filename)
{
  FILE *fpDesc;

  if (ObjectDescriptions != NULL) {
    delete ObjectDescriptions;
  }

  ObjectDescriptions = new Vector("ObjDesc", __LINE__);

  fpDesc = fopen(filename, "r");

  if (fpDesc == NULL) {
    AfxMessageBox("Failed to open objects file");
    return;
  }

  char line[1024];
  int id;
  char desc[256];

  while (!feof(fpDesc)) {
    fscanf(fpDesc, "%[^\n]\n", line);

    sscanf(line, "%d - %[^\0]\0", &id, desc);

    ObjecDesc *objdesc = new ObjecDesc(id, desc);
    ObjectDescriptions->addElement(objdesc);
  }

  char msg[256];
  wsprintf(msg, "Added %d Object Definitions", ObjectDescriptions->size());
  AfxMessageBox(msg);

  fclose(fpDesc);
}

char *
  TrackObjectDefinition::getItemName()
{
  for (int i = 0; i < ObjectDescriptions->size(); i++) {
    ObjecDesc *desc = (ObjecDesc *)ObjectDescriptions->elementAt(i);
    if (locator == desc->offset) return desc->name;
  }
  return NULL;
}

char *
  getItemName(int locator)
{
  for (int i = 0; i < ObjectDescriptions->size(); i++) {
    ObjecDesc *desc = (ObjecDesc *)ObjectDescriptions->elementAt(i);
    if (locator == desc->offset) return desc->name;
  }
  return NULL;
}

int TrackObjectDefinition::read(GPTrack *track, int offset, int ListObjectOffset)
{
  int start = offset;
  id = track->ReadPCByte(offset);
  offset += 1;
  detail = track->ReadPCByte(offset);
  offset += 1;
  Unk1 = track->ReadPCWord(offset);
  offset += 2;
  y = track->ReadPCWord(offset);
  offset += 2;
  angleX = track->ReadPCWord(offset);
  offset += 2;
  id2 = track->ReadPCWord(offset);
  offset += 2;
  Unk2 = track->ReadPCWord(offset);
  offset += 2;
  height = track->ReadPCWord(offset);
  offset += 2;
  angleY = track->ReadPCWord(offset);
  offset += 2;

  locator = (start - ListObjectOffset);
  return offset;
}

int TrackObjectDefinition::write(GPTrack *track, int offset)
{
  int start = offset;

  track->WritePCByte(offset, id);
  offset += 1;
  track->WritePCByte(offset, detail);
  offset += 1;
  track->WritePCWord(offset, Unk1);
  offset += 2;
  track->WritePCWord(offset, y);
  offset += 2;
  track->WritePCWord(offset, angleX);
  offset += 2;
  track->WritePCWord(offset, id2);
  offset += 2;
  track->WritePCWord(offset, Unk2);
  offset += 2;
  track->WritePCWord(offset, height);
  offset += 2;
  track->WritePCWord(offset, angleY);
  offset += 2;

  // locator = (start-ListObjectOffset);
  return offset;
}

int TrackObjectDefinition::Open()
{
  CObjectEdit *trkdlg = new CObjectEdit(track, NULL);

  if (trkdlg) trkdlg->setSection(this);

  int result = trkdlg->DoModal();

  if (trkdlg) {
    trkdlg->getSection(this);
    // write(track,originaloffset,track->ListObjectOffset);
    track->regenerate = TRUE;
  }
  return result;
}

extern CTrackEditorApp theApp;

LPCSTR
FindInternalObjectName(GPTrack *track, int id1, int id2)
{
  int internalId = id1 - 17;

  InternalObject *obj =
    (InternalObject *)track->InternalObjectOffsets->elementAt(internalId);
  if (obj) {
    return obj->getName();
  }
  return "Find Object Error";
}

char *
  getDisplayObjectType(GPTrack *track, int id1, int id2)
{
  static char mess[256];

  if (id1 < 17) {
    switch (id1) {
    // internal item 1
    case 1: {
      switch (id2) {
      case 78:
        return "Sign Arrow Right";
      case 79:
        return "Sign Arrow Staight Right";
      case 80:
        return "Sign Arrow Left";
      case 81:
        return "Sign Arrow Staight Left";
      }
    } break;
    case 9:
      return "Pit Hand";
      break;
    case 10:
      return "Pit Lollypop";
      break;
    case 11:
      return "Pit Monitor";
      break;
    case 12:
      return "Helments";
      break;
    case 0:
      return "Cars";
      break;

    case 5: {
      switch (id2) {
      case 82:
        return "Column";
      case 83:
        return "Yacht";
      }
      if (id2 < 65)
        return "Tyre";
      else if (id2 < 110 && id2 > 83)
        return "Tree";
      else if (id2 >= 100 && id2 < 130)
        return "Bush";
      else if (id2 < 226 && id2 >= 221)
        return "Monaco Tree";
      return "Bush/Tree Unk";
    } break;
    case 4: {
      switch (id2) {
      case 160:
        return "Pit Crew";
      }
      break;
    } break;
    case 2: {
      return "Landscape";
    } break;
    case 6:
      return "Marshall with black/white flag";
    case 7:
      return "Marshall with blue flag";
    case 14:
      return "Marshall with blue flag";
    case 17:
      return "Large Pit Tower";

    case 13: {
      switch (id2) {
      case 75:
        return "100 Metre Sign";
      case 76:
        return "200 Metre Sign";
      case 77:
        return "300 Metre Sign";
      }
    } break;
    }
    wsprintf(mess, "Unknown Predefined Object %d %d", id1, id2);
  } else {
    return (LPSTR)FindInternalObjectName(track, id1, id2);
  }
  return mess;
}

char *
  getNameLocal(GPTrack *track, int locator, int id, int id2)
{
  // if (theApp.cleverNames && id > 17)
  //{
  //		char *name =  getItemName(locator);
  //		if (name != NULL) return name;
  //		else return "Unknown Object";
  //}
  // else
  //{
  return getDisplayObjectType(track, id, id2);
  //}
}

char *
  TrackObjectDefinition::getName()
{
  return getNameLocal(track, locator, id, id2);
  /*
  if (theApp.cleverNames && id > 17)
  {
                  char *name =  getItemName();
                  if (name != NULL) return name;
                  else return "Unknown Object";
  }
  else
  {
                  return getDisplayObjectType(id,id2);
  }
  */
}

int parseObjectName(char *name)
{
  CString newObjectname = CString(name);
  newObjectname.MakeLower();

  for (int i = 0; i < SizeOfLookUp; i++) {
    if (newObjectname.Find(ObjectLookUpTable[i].name) != -1)
      return ObjectLookUpTable[i].id;
  }
  /*
  if (newObjectname.Find("bush")!=-1) return IDB_BUSH;
  else if (newObjectname.Find("tree")!=-1) return IDB_TREE;
  else if (newObjectname.Find("side advert")!=-1) return IDB_SIDE_ADVERT;
  else if (newObjectname.Find("advert")!=-1) return IDB_ADVERT;
  else if (newObjectname.Find("roof")!=-1) return IDB_STANDROOT;
  else if (newObjectname.Find("stand")!=-1) return IDB_STAND;
  //else if (newObjectname.Find("house")!=-1) return IDB_BUILDING;
  else if (newObjectname.Find("building")!=-1) return IDB_BUILDING;
  else if (newObjectname.Find("apartment")!=-1) return IDB_BUILDING;
  else if (newObjectname.Find("flyover")!=-1) return IDB_OVER_ADVERT;
  else if (newObjectname.Find("tower")!=-1) return IDB_BUILDING;
  else if (newObjectname.Find("100 metre")!=-1) return IDB_100_SIGN;
  else if (newObjectname.Find("200 metre")!=-1) return IDB_200_SIGN;
  else if (newObjectname.Find("300 metre")!=-1) return IDB_300_SIGN;
  //
  else if (newObjectname.Find("marshall")!=-1) return IDB_MARSHALL;
  else if (newObjectname.Find("post")!=-1) return IDB_MARSHALL;

  else if (newObjectname.Find("crane")!=-1) return IDB_CRANE;
  else if (newObjectname.Find("starting lights")!=-1) return IDB_LIGHTS;
else if (newObjectname.Find("bridge")!=-1) return IDB_BRIDGE;
  else if (newObjectname.Find("observation")!=-1) return IDB_OBSERVATION;
  else if (newObjectname.Find("shadow")!=-1) return IDB_SHADOW;
  else if (newObjectname.Find("house")!=-1) return IDB_HOUSE;
  else if (newObjectname.Find("tent")!=-1) return IDB_TENT;
  else if (newObjectname.Find("yacht")!=-1) return IDB_YACHT;
  else if (newObjectname.Find("pillar")!=-1) return IDB_PILLAR;
  else if (newObjectname.Find("crowd")!=-1) return IDB_CROWDICON;
  else if (newObjectname.Find("score")!=-1) return IDB_SCOREBOARD;
  else if (newObjectname.Find("reporter")!=-1) return IDB_REPORTER;
  */

  return IDB_OBJECT_UNK;
}

int getOldDisplayIconType(GPTrack *track, int id, int id2)
{
  if (id < 17) {
    switch (id) {
    case 1: {
      switch (id2) {
      case 78:
        return IDB_TURN_RIGHT;
      case 79:
        return IDB_TURN_RIGHT;
      case 80:
        return IDB_TURN_LEFT;
      case 81:
        return IDB_TURN_LEFT;
      }
      return IDB_UNK;
    } break;

    case 5: {
      if (id2 < 65)
        return IDB_TYRE;
      else if (id2 < 110 && id2 > 83)
        return IDB_TREE;
      else if (id2 >= 100 && id2 < 130)
        return IDB_BUSH;
      else if (id2 >= 130 && id2 < 220)
        return IDB_PALM;
      else if (id2 < 238 && id2 >= 221)
        return IDB_TREE;
      if (id2 == 83) return IDB_YACHT;
      return IDB_UNK;
    } break;
    case 4: {
      return IDB_PIT;
    } break;
    case 6:
      return IDB_CHECKED_FLAG;
    case 7:
      return IDB_BLUE_FLAG;
    case 13: {
      switch (id2) {
      case 75:
        return IDB_100_SIGN;
      case 76:
        return IDB_200_SIGN;
      case 77:
        return IDB_300_SIGN;
      }
      return IDB_100_SIGN;
    } break;
    case 17:
      IDB_BUILDING;
    }

    return IDB_OBJECT;
  } else {
    // return IDB_OBJECT_INTERNAL;
    return parseObjectName(getDisplayObjectType(track, id, id2));
  }
}

int getDisplayIconType(GPTrack *track, int locator, int id1, int id2)
{
  if (theApp.cleverNames && locator != -1) {
    return parseObjectName(getNameLocal(track, locator, id1, id2));
  } else {
    return getOldDisplayIconType(track, id1, id2);
  }
}

int TrackObjectDefinition::getDisplayIconType()
{
  if (theApp.cleverNames) {
    return parseObjectName(getName());
  } else {
    return getOldDisplayIconType(track, id, id2);
  }
}

void TrackObjectDefinition::Delete()
{
  track->TrackObjectDefintions->removeElement(this);
}

void TrackObjectDefinition::DrawVML(std::ofstream &file, double x, double y, double z, double angle, BOOL drawAxis)
{
  double side = 2048.0f;
  double s = side;
  double s1 = 2048.0f;
  double s2 = 2048.0f;
  double angleDeg = RadToDeg(angle);
  double nangle = 0;

  nangle = (angleX / 180);
  double rangle = angleDeg - nangle - 90;

  glPushMatrix();
  glTranslated(x, y, z);
  glRotated(rangle, 0.0, 0.0, 1.0);

  int idb = getDisplayIconType();

  // if (idb != IDB_UNK)
  if (id >= 17) {
    int internalId = id - 17;
    InternalObject *obj =
      (InternalObject *)track->InternalObjectOffsets->elementAt(internalId);

    if (obj) {
      double scale = 0.0002f;
      glScaled(-scale, scale, -scale);
      obj->DrawVML(file, x, y, z, 0, drawAxis);
    }
  } else {
    if (track->showExternalObjects) {
      BOOL transparent = TRUE;

      int width = 128;
      int height = 128;

      if (id == 5) {
        // road sign
        glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
        width = 2048;
        height = 2048;
      } else if ((id == 1) || (id == 13)) {
        // road sign
        glColor4f(0.8f, 0.8f, 0.8f, 1.0f);
        width = 16;
        height = 16;
        s = width * 16;
        s2 = height * 32;
      }

      if (track->showTextures) {
        if (id == 5) {
          CBitmap tree;
          unsigned char *TreeBits = NULL;

          int textureId = (id2 + 256);

          glEnable(GL_TEXTURE_2D);
          CTrackOpenGLView::LoadJamTexture(track, tree, textureId, 256, 256, &width, &height);
          glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
          if (transparent) {
            float alpha = 0.7f;
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.5f);
          }
          // TreeBits = CTrackOpenGLView::TextureMap(pDC,tree,TreeBits,TRUE);
          // delete TreeBits;
        }
        s = width * 16;
        s2 = height * 32;
      }

      double scale = 0.0002f;
      glScaled(-scale, scale, -scale);

      glBegin(GL_POLYGON);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3d(s, 0, 0);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3d(-s, 0, 0);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3d(-s, 0, s2);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3d(s, 0, s2);
      glEnd();

      glDisable(GL_TEXTURE_2D);
      if (transparent) {
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);
        // glDepthMask(TRUE);
      }
    }
  }

  glPopMatrix();
}
void TrackObjectDefinition::DrawGL(CDC *pDC, double x, double y, double z, double angle, BOOL drawAxis)
{
  double side = 2048.0f;
  double s = side;
  double s1 = 2048.0f;
  double s2 = 2048.0f;
  double angleDeg = RadToDeg(angle);
  double nangle = 0;

  nangle = (angleX / 180);
  double rangle = angleDeg - nangle - 90;

  glPushMatrix();
  glTranslated(x, y, z);
  glRotated(rangle, 0.0, 0.0, 1.0);

  int idb = getDisplayIconType();

  // if (idb != IDB_UNK)
  if (id >= 17) {
    int internalId = id - 17;
    InternalObject *obj =
      (InternalObject *)track->InternalObjectOffsets->elementAt(internalId);

    if (obj) {
      double scale = 0.0002f;
      glScaled(-scale, scale, -scale);
      obj->DrawGL(pDC, x, y, z, 0, drawAxis);
    }
  } else {
    if (track->showExternalObjects) {
      BOOL transparent = TRUE;

      int width = 128;
      int height = 128;

      if (id == 5) {
        // road sign
        glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
        width = 2048;
        height = 2048;
      } else if ((id == 1) || (id == 13)) {
        // road sign
        glColor4f(0.8f, 0.8f, 0.8f, 1.0f);
        width = 16;
        height = 16;
        s = width * 16;
        s2 = height * 32;
      }

      if (track->showTextures) {
        if (id == 5) {
          CBitmap tree;
          unsigned char *TreeBits = NULL;

          int textureId = (id2 + 256);

          glEnable(GL_TEXTURE_2D);
          CTrackOpenGLView::LoadJamTexture(track, tree, textureId, 256, 256, &width, &height);
          glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
          if (transparent) {
            float alpha = 0.7f;
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.5f);
          }
          TreeBits = CTrackOpenGLView::TextureMap(pDC, tree, TreeBits, TRUE);
          delete TreeBits;
        }
        s = width * 16;
        s2 = height * 32;
      }

      double scale = 0.0002f;
      glScaled(-scale, scale, -scale);

      glBegin(GL_POLYGON);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3d(s, 0, 0);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3d(-s, 0, 0);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3d(-s, 0, s2);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3d(s, 0, s2);
      glEnd();

      glDisable(GL_TEXTURE_2D);
      if (transparent) {
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);
        // glDepthMask(TRUE);
      }
    }
  }

  glPopMatrix();
}

void TrackObjectDefinition::Draw2D(Display *g, double x, double y, double z, double angle, BOOL drawAxis)
{
  double side = 0.3f;
  double s = side;
  double angleDeg = RadToDeg(angle);
  double nangle = 0;

  nangle = (angleX / 180) + 90;
  double rangle = DegToRad(angleDeg - nangle);

  int idb = getDisplayIconType();

  // if (idb != IDB_UNK)
  if (id >= 17) {
    int internalId = id - 17;
    InternalObject *obj =
      (InternalObject *)track->InternalObjectOffsets->elementAt(internalId);

    if (obj) {
      // double scale = 0.0002f;
      // glScaled(-scale,scale,-scale);
      obj->Draw2D(g, x, y, z, rangle, drawAxis);
    }
  } else {
    // show gp2 external object
    if (track->showExternalObjects)
      InternalObject::Draw2DExternal(g, x, y, z, rangle, drawAxis);
  }
}
