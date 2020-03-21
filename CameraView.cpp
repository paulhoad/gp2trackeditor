// CameraView.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "CameraView.h"
#include "TrackEditorDoc.h"
#include "TrackSection.h"
#include "GPTrack.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern LPSTR
  toString(int data);
extern LPSTR
  toString(double data);

/////////////////////////////////////////////////////////////////////////////
// CCameraView

IMPLEMENT_DYNCREATE(CCameraView, CMyTableView)

CCameraView::CCameraView() {}

CCameraView::~CCameraView() {}

BEGIN_MESSAGE_MAP(CCameraView, CMyTableView)
//{{AFX_MSG_MAP(CCameraView)
ON_WM_SETFOCUS()
ON_WM_CREATE()
ON_WM_LBUTTONDBLCLK()
ON_WM_KEYDOWN()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCameraView drawing

void CCameraView::OnInitialUpdate()
{}

void CCameraView::OnDraw(CDC* pDC)
{
  CDocument* pDoc = GetDocument();
  // TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CCameraView diagnostics

#ifdef _DEBUG
void CCameraView::AssertValid() const
{
  CMyTableView::AssertValid();
}

void CCameraView::Dump(CDumpContext& dc) const
{
  CMyTableView::Dump(dc);
}
#endif//_DEBUG

#define UPDATE_TABLE_STRING_PTR(a, b, c, d) \
  str = d;                                  \
  UpdateTableStringPtr(a, b, c, str);       \
  free(str);

void CCameraView::UpdateInfo(BOOL update)
{
  CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  if (mytrack != NULL) {
    if (!update) {
      int colidx = 0;
      EmptyTable();
      addColumn("Index", 50, colidx++);
      addColumn("Camera Position (DFS)", 70, colidx++);
      addColumn("Camera to Track (DFC)", 70, colidx++);
      addColumn("Height", 70, colidx++);
      addColumn("Height during Practice", 70, colidx++);
      addColumn("Zoom In/Out (ZF1)", 70, colidx++);
      addColumn("Zoom Adjust (ZF2)", 70, colidx++);

      addColumn("Next Camera (SM)", 70, colidx++);
      addColumn("Object View Behind (VR1)", 70, colidx++);
      addColumn("Object View Ahead  (VR2)", 70, colidx++);

      addColumn("Unk-1", 70, colidx++);
      addColumn("Unk-2", 70, colidx++);
      addColumn("Unk-3", 70, colidx++);
      addColumn("Unk-4", 70, colidx++);
      addColumn("Unk-5", 70, colidx++);
      addColumn("Unk-6", 70, colidx++);
      addColumn("Unk-7", 70, colidx++);
    }

    if (mytrack->TrackCameras) {
      for (int i = 0; i < mytrack->TrackCameras->size(); i++) {
        Camera* t = (Camera*)mytrack->TrackCameras->elementAt(i);
        int tidx = 0;

        LPSTR str = toString(i);
        UpdateTableStringPtr(update, i, tidx++, str, TO_ID(IDB_CAMERA));
        free(str);

        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->X - 16384));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->Y));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->Z1));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->Z2));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->ZF1));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->ZF2));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->SM - 16384));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->VR1));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->VR2));

        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->UK1 - 16384));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->UK2));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->UK3));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->UK4));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->UK5));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->UK6));
        UPDATE_TABLE_STRING_PTR(update, i, tidx++, toString(t->UK7));
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// CCameraView message handlers

void CCameraView::OnSetFocus(CWnd* pOldWnd)
{
  CMyTableView::OnSetFocus(pOldWnd);

  CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  if (mytrack && mytrack->isValid()) {
    // UpdateInfo();
  }
}

BOOL CCameraView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
  BOOL res = CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

  // SetImageList((((CTrackEditorApp*)AfxGetApp())->getSharedImageList()),LVSIL_SMALL);

  return res;
}

int CCameraView::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

void CCameraView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default

  CMyTableView::OnLButtonDblClk(nFlags, point);

  int idx = getTableSectionIndex();

  CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
  GPTrack* mytrack = pDoc->getTrack();

  Camera* t = (Camera*)mytrack->TrackCameras->elementAt(idx);

  if (t) {
    t->Open();
  }

  UpdateInfo(TRUE);
}

void CCameraView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
  // TODO: Add your specialized code here and/or call the base class
  UpdateInfo(TRUE);
}

void CCameraView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  CPoint pt;

  pt.x = 0;
  pt.y = 0;

  if (nChar == 13) {
    OnLButtonDblClk(0, pt);
  }
  CMyTableView::OnKeyDown(nChar, nRepCnt, nFlags);
}
