// PitInfo.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEditor.h"
#include "PitInfo.h"
#include "TrackInfo.h"
#include "TrackEditorDoc.h"
#include "TrackSection.h"
#include "GPTrack.h"
#include "Ruler.h"
#include "ImportDataElement.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// extern char *toBinString(int data,int size);
extern LPSTR
  toBinString(int data, int size);
extern LPSTR
  toString(int data);
extern LPSTR
  toString(double data);
extern LPSTR
  toHexString(int data);
// extern CString toBinString(int data,int size);

/////////////////////////////////////////////////////////////////////////////
// CPitInfo

IMPLEMENT_DYNCREATE(CPitInfo, CMyTableView)

CPitInfo::CPitInfo() { trkdlg = new CTrChangeDlg(this); }

CPitInfo::~CPitInfo()
{
  if (trkdlg != NULL) delete trkdlg;

  CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();

  if (pDoc) {
    pDoc->pitTable = NULL;
  }
}

#define UPDATE_TABLE_STRING_PTR(a, b, c, d)    \
  str = d;                                     \
  UpdateTableStringPtr(a, b, c, CString(str)); \
  free(str);

void CPitInfo::UpdateInfo(BOOL update)
{
  CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
  pDoc->pitTable = this;
  GPTrack* mytrack = pDoc->getTrack();

  // pDoc->SetTitle("Track Info");

  if (mytrack != NULL) {
    if (!update) {
      int colidx = 0;
      EmptyTable();
      addColumn("Index", 50, colidx++);
      addColumn("Angle", 50, colidx++);
      addColumn("Length", 50, colidx++);
      addColumn("Arc", 50, colidx++);
      addColumn("Height", 50, colidx++);
      addColumn("IsoHeight", 50, colidx++);

      addColumn("Gradient (HxL)", 50, colidx++);
      addColumn("Cumlative Gradient", 50, colidx++);
      addColumn("Altitude Delta", 50, colidx++);
      addColumn("Cumlative  Altitude", 50, colidx++);

      addColumn("StAngle", 50, colidx++);
      addColumn("EnAngle", 50, colidx++);

      addColumn("RoadSide", 50, colidx++);
      addColumn("RoadSide (Bin)", 100, colidx++);
      addColumn("Kerb", 50, colidx++);
      addColumn("Kerb (Bin)", 100, colidx++);

      addColumn("Left Bank", 50, colidx++);
      addColumn("Right Bank", 50, colidx++);
      addColumn("Track Left  Width", 50, colidx++);
      addColumn("Track Right Width", 50, colidx++);

      addColumn("Length Meters", 50, colidx++);
      addColumn("Length Feet", 50, colidx++);
      addColumn("Cumlative Length", 50, colidx++);
    }

    if (mytrack->PitlaneSections) {
      for (int i = 0; i < mytrack->PitlaneSections->size(); i++) {
        TrackSection* t = (TrackSection*)mytrack->PitlaneSections->elementAt(i);

        int tidx = 0;

        LPSTR str = toString(i);
        UpdateTableStringPtr(update, i, tidx++, str, TO_ID(t->getIcon(TRUE)));
        free(str);

        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->getAngle()));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->getLength()));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->getAngle() * t->getLength()));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->getHeight()));
        CString str1;
        str1.Format("%f", t->getIsoHeight());
        UpdateTableStringPtr(update, i, tidx++, str1);
        // UPDATE_TABLE_STRING_PTR(update,i,tidx++,));

        str = toString(t->getHeight() * t->getLength());
        UpdateTableStringPtr(update, i, tidx++, str);
        free(str);
        str = toString(t->getAltitude());
        UpdateTableStringPtr(update, i, tidx++, str);
        free(str);
        str = toString(t->getAltitudeDelta());
        UpdateTableStringPtr(update, i, tidx++, str);
        free(str);

        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->getTrueAltitude()));

        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(RadToDeg(t->getStartAngle())));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(RadToDeg(t->getEndAngle())));

        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toHexString(t->getRoadSignData()));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toBinString(t->getRoadSignData(), 8));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toHexString(t->getKerbData()));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toBinString(t->getKerbData(), 8));

        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->getLeftToBank()));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->getRightToBank()));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString((int)t->getWidthLeft()));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString((int)t->getWidthRight()));
        UPDATE_TABLE_STRING_PTR(
          update, i, tidx++, toString((int)(LENGTH_TO_KM * t->getTrueLength())));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString((int)(16.0 * t->getTrueLength())));
        UPDATE_TABLE_STRING_PTR(
          update, i, tidx++, toString((int)(t->track->LengthToPitSectorEnd(t))));
      }
    }
  }
  // CSize size = table->getSize();
  // SetScrollSizes(MM_TEXT, size);
}

void CPitInfo::OnDraw(CDC* pDC)
{
  CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
  GPTrack* mytrack = pDoc->getTrack();
}

BEGIN_MESSAGE_MAP(CPitInfo, CMyTableView)
//{{AFX_MSG_MAP(CPitInfo)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONDBLCLK()
ON_WM_SETFOCUS()
ON_WM_LBUTTONUP()
ON_COMMAND(ID_EXPORT_TABLE, OnExportTable)
ON_WM_CREATE()
ON_WM_KEYDOWN()
ON_COMMAND(ID_IMPORT_TABLE, OnImportTable)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPitInfo drawing

void CPitInfo::OnInitialUpdate()
{
  // CMyTableView::OnInitialUpdate();

  // CSize sizeTotal;
  // TODO: calculate the total size of this view
  // sizeTotal.cx = sizeTotal.cy = 1000;
  // SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CPitInfo diagnostics

#ifdef _DEBUG
void CPitInfo::AssertValid() const
{
  CMyTableView::AssertValid();
}

void CPitInfo::Dump(CDumpContext& dc) const
{
  CMyTableView::Dump(dc);
}
#endif//_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPitInfo message handlers

BOOL CPitInfo::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
  // TODO: Add your specialized code here and/or call the base class

  return CMyTableView::OnScroll(nScrollCode, nPos, bDoScroll);
}

BOOL CPitInfo::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
  // TODO: Add your specialized code here and/or call the base class

  return CMyTableView::OnScrollBy(sizeScroll, bDoScroll);
}

void CPitInfo::OnLButtonDown(UINT nFlags, CPoint point)
{
  CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  int idx = getTableSectionIndex();
  TrackSection* t = (TrackSection*)mytrack->PitlaneSections->elementAt(idx);

  mytrack->clearPitSelections();

  if (t) {
    t->setSelected(TRUE);
    pDoc->getTree()->Expand(pDoc->getTree()->GetParentItem(t->getTreeNode()),
      TVE_EXPAND);
    pDoc->getTree()->SelectItem(t->getTreeNode());
  }

  CMyTableView::OnLButtonDown(nFlags, point);
}

void CPitInfo::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  int idx = getTableSectionIndex();

  CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  TrackSection* t = (TrackSection*)mytrack->PitlaneSections->elementAt(idx);

  int result = trkdlg->EditTrackSections(t);
  /*
  if (trkdlg && t)
    trkdlg->setSection(t);

  int result = trkdlg->DoModal();

  if (trkdlg && t && result!=IDCANCEL)
  {
    trkdlg->getSection(t);
    mytrack->regenerate=TRUE;

    UpdateInfo(TRUE);
  }
  */
  mytrack->createTrack(pDoc);
  UpdateInfo(TRUE);

  CMyTableView::OnLButtonDblClk(nFlags, point);
}

void CPitInfo::OnSetFocus(CWnd* pOldWnd)
{
  CMyTableView::OnSetFocus(pOldWnd);

  CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  if (mytrack && mytrack->isValid()) {
    // UpdateInfo();
  }
}

void CPitInfo::OnLButtonUp(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default

  CMyTableView::OnLButtonUp(nFlags, point);
}

void CPitInfo::OnExportTable()
{
  // TODO: Add your command handler code here
  ExportTableData();
}

BOOL CPitInfo::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
  // TODO: Add your specialized code here and/or call the base class

  BOOL res = CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

  // SetImageList((((CTrackEditorApp*)AfxGetApp())->getSharedImageList()),LVSIL_SMALL);

  return res;
}

int CPitInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CMyTableView::OnCreate(lpCreateStruct) == -1) return -1;

  CImageList* pImages = (((CTrackEditorApp*)AfxGetApp())->getSharedImageList());

  if (pImages != NULL) {
    SetImageList(pImages, LVSIL_SMALL);
    useImages = TRUE;
  } else {
    useImages = FALSE;
    AfxMessageBox("Failed to open Images");
  }

  // TODO: Add your specialized creation code here
  UpdateInfo(FALSE);

  return 0;
}

void CPitInfo::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  CPoint pt;

  pt.x = 0;
  pt.y = 0;

  if (nChar == 13) {
    OnLButtonDblClk(0, pt);
  }
  CMyTableView::OnKeyDown(nChar, nRepCnt, nFlags);
}

#define PITANGLE 1
#define PITLENGTH 2
#define PITHEIGHT 4
#define PITRS 7
#define PITKB 9
#define PITWL 11
#define PITWR 12

void CPitInfo::OnImportTable()
{
  // TODO: Add your command handler code here
  Vector* tabledata = ImportTableData();
  CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  BOOL ignoreExtra = 0;
  BOOL removeExtra = 0;

  if (tabledata != NULL) {
    int result = AfxMessageBox(
      "About to Import Pit Data!, are you sure you want to do this",
      MB_YESNO);

    if (result == IDYES) {
      if (tabledata->size() > mytrack->PitlaneSections->size()) {
        // ignoreExtra = AfxMessageBox("More Table Data Than Track (Ignore Extra
        // Table Data)?",MB_YESNO);
        AfxMessageBox(
          "More Table Data Than Track (Ignoring Extra Table Data, add "
          "section and reapply)?");
        ignoreExtra = TRUE;
      } else if (tabledata->size() < mytrack->PitlaneSections->size()) {
        // removeExtra = AfxMessageBox("More Track Data Than Table (Remove Extra
        // Track Data)?",MB_YESNO);
        AfxMessageBox(
          "More pit Data Than Table (leaving end of pit unaltered!)");
      }

      int importSize = tabledata->size();
      if (ignoreExtra == TRUE) {
        importSize = mytrack->PitlaneSections->size();
      }

      for (int i = 0; i < importSize; i++) {
        Vector* row = (Vector*)tabledata->elementAt(i);
        TrackSection* t = (TrackSection*)mytrack->PitlaneSections->elementAt(i);

        if (t != NULL) {
          ImportDataElement* elm = (ImportDataElement*)row->elementAt(PITANGLE);
          t->setAngle(elm->toDouble());

          elm = (ImportDataElement*)row->elementAt(PITLENGTH);
          t->setLength(elm->toInt());

          elm = (ImportDataElement*)row->elementAt(PITHEIGHT);
          t->setHeight(elm->toInt());

          elm = (ImportDataElement*)row->elementAt(PITRS);
          t->setRoadSignData(elm->toInt());

          elm = (ImportDataElement*)row->elementAt(PITKB);
          t->setKerbData(elm->toInt());

          elm = (ImportDataElement*)row->elementAt(PITWL);
          t->setLeftToBank(elm->toInt());

          elm = (ImportDataElement*)row->elementAt(PITWR);
          t->setRightToBank(elm->toInt());
        }
      }
    }
  }
  mytrack->createTrack(pDoc);
  mytrack->createCC();
  UpdateInfo(TRUE);
}
