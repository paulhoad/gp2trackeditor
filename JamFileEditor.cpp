

#include "stdafx.h"
#include "JamFileEditor.h"

void AddJamDefaults(GPTrack *track)
{
  JamFileEditor *jam = new JamFileEditor(track, "gamejams\\roadsgn.jam", TRUE);
  track->JamFiles->addElement(jam);
  jam = new JamFileEditor(track, "gamejams\\stands.jam", TRUE);
  track->JamFiles->addElement(jam);
  jam = new JamFileEditor(track, "gamejams\\crowd4.jam", TRUE);
  track->JamFiles->addElement(jam);
  // jam = new JamFileEditor(track,"gamejams\\ftrees.jam",TRUE);
  // track->JamFiles->addElement(jam);
  // jam = new JamFileEditor(track,"gamejams\\clumps1.jam",TRUE);
  // track->JamFiles->addElement(jam);
  // jam = new JamFileEditor(track,"gamejams\\clumps2.jam",TRUE);
  // track->JamFiles->addElement(jam);
  jam = new JamFileEditor(track, "gamejams\\mpush.jam", TRUE);
  track->JamFiles->addElement(jam);
  jam = new JamFileEditor(track, "gamejams\\pc-extr1.jam", TRUE);
  track->JamFiles->addElement(jam);
  jam = new JamFileEditor(track, "gamejams\\pc-wheel.jam", TRUE);
  track->JamFiles->addElement(jam);
  jam = new JamFileEditor(track, "gamejams\\pc-jack.jam", TRUE);
  track->JamFiles->addElement(jam);
  jam = new JamFileEditor(track, "gamejams\\pc-extr2.jam", TRUE);
  track->JamFiles->addElement(jam);
  jam = new JamFileEditor(track, "gamejams\\pitprop1.jam", TRUE);
  track->JamFiles->addElement(jam);
  jam = new JamFileEditor(track, "gamejams\\pitprop2.jam", TRUE);
  track->JamFiles->addElement(jam);
  jam = new JamFileEditor(track, "gamejams\\fuel.jam", TRUE);
  track->JamFiles->addElement(jam);
  jam = new JamFileEditor(track, "gamejams\\flags1.jam", TRUE);
  track->JamFiles->addElement(jam);
  jam = new JamFileEditor(track, "gamejams\\chequ1.jam", TRUE);
  track->JamFiles->addElement(jam);
  jam = new JamFileEditor(track, "gamejams\\flagcheq.jam", TRUE);
  track->JamFiles->addElement(jam);
  jam = new JamFileEditor(track, "gamejams\\spgrid2.jam", TRUE);
  track->JamFiles->addElement(jam);
  jam = new JamFileEditor(track, "gamejams\\damage.jam", TRUE);
  track->JamFiles->addElement(jam);
}

void JamFileEditor::Delete()
{
  track->JamFiles->removeElement(this);
}

int JamFileEditor::Open()
{
  char fileName[256];

  strcpy(fileName, getJamFileName());
  CFileDialog *fdlg =
    new CFileDialog(TRUE, "jam", (LPCSTR)&fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszJamFilter);

  int result = fdlg->DoModal();

  if (result != IDCANCEL) {
    char message[256];
    CString filename = fdlg->GetFileName();
    CString path = fdlg->GetPathName();

    char drive[10];
    // char directoy[256];
    char file[256];
    char ext[256];
    char directory[256];

    _splitpath(path, drive, directory, file, ext);

    CString newDir = CString(directory);
    newDir.MakeLower();
    filename.MakeLower();

    int stidx = 0;

    if ((stidx = newDir.Find("gamejams")) == -1) {
      AfxMessageBox("Cannot find Jam files not in GameJams Tree");
      return IDCANCEL;
    }

    int length = newDir.GetLength();

    CString adjustedPath = newDir.Right(length - stidx);

    wsprintf(message, "Setting Jam File to %s", filename);
    result = AfxMessageBox(message, MB_YESNO);

    if (result == IDYES) {
      char buffer[256];
      wsprintf(buffer, "%s%s", adjustedPath, filename);
      setJamFileName(buffer);
      if (getTreeNode()) {
        BOOL done = tree->SetItem(getTreeNode(), TVIF_TEXT, getJamFileName(), 0, 0, 0, 0, 0);
        if (done == FALSE) {
          AfxMessageBox("Cannot reset the CListTree's label", MB_OK);
        }
      }
    }
  }

  delete fdlg;
  return result;
}
