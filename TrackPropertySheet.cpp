// TrackTree.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEditor.h"
#include "TrackPropertySheet.h"
#include "TrackSection.h"
#include "TrackCmd.h"
#include "InternalObject.h"
#include "JamFileEditor.h"
#include "SingleVarObserver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WR_PROFILE(x, y, b) AfxGetApp()->WriteProfileInt(CString(#y), #x, b);

extern LPSTR
  toString(int data);
/////////////////////////////////////////////////////////////////////////////
// CTrackPropertySheet dialog

CTrackPropertySheet::CTrackPropertySheet()
  : CSizingControlBar(), pImageList(NULL)
{
  //{{AFX_DATA_INIT(CTrackPropertySheet)
  //}}AFX_DATA_INIT
  // m_TrackTree = NULL;
  ImagesNotLoaded = TRUE;
}

CTrackPropertySheet::~CTrackPropertySheet()
{
  if (pImageList != NULL) delete pImageList;
}

void CTrackPropertySheet::DoDataExchange(CDataExchange *pDX)
{
  // CDialogBar::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CTrackPropertySheet)
  DDX_Control(pDX, ID_TRACK_PROPERTY, m_TrackProperty);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTrackPropertySheet, CSizingControlBar)
//{{AFX_MSG_MAP(CTrackPropertySheet)
ON_WM_CREATE()
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_SIZE()
ON_WM_DESTROY()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackPropertySheet message handlers

int CTrackPropertySheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CSizingControlBar::OnCreate(lpCreateStruct) == -1) return -1;

  CRect rc;
  rc.left = 10;
  rc.top = 10;
  rc.right = 100;
  rc.bottom = 100;
  // TODO: Add your message handler code here
  if (!m_TrackProperty.CreateEx(
        WS_EX_CLIENTEDGE,
        WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_EDITLABELS,
        rc,
        this,
        ID_TRACK_PROPERTY))
    return -1;

  UpdateInfo();

  return 0;
}

void CTrackPropertySheet::UpdateInfo()
{
  int colName = m_TrackProperty.InsertColumn(0, "Name", LVCFMT_LEFT, 150, 0);
  int colData = m_TrackProperty.InsertColumn(1, "Data", LVCFMT_LEFT, 75, 1);
  int colDesc =
    m_TrackProperty.InsertColumn(2, "Description", LVCFMT_LEFT, 500, 2);
}

void CTrackPropertySheet::InsertTableString(int row, int index, LPSTR name, LPARAM extraData)
{
  LV_ITEM NameVal;

  NameVal.mask = LVIF_TEXT;
  NameVal.iItem = row;
  NameVal.iSubItem = index;
  NameVal.lParam = (LPARAM)extraData;
  NameVal.pszText = _T(name);
  NameVal.cchTextMax = 128;

  if (index == 0)
    m_TrackProperty.InsertItem(&NameVal);
  else
    m_TrackProperty.SetItem(&NameVal);
}

void CTrackPropertySheet::InsertTableString(int row, int index, CString name, LPARAM extraData)
{
  LV_ITEM NameVal;

  NameVal.mask = LVIF_TEXT;
  NameVal.iItem = row;
  NameVal.iSubItem = index;
  NameVal.lParam = (LPARAM)extraData;
  NameVal.pszText = (LPTSTR)&name;
  NameVal.cchTextMax = 128;

  if (index == 0)
    m_TrackProperty.InsertItem(&NameVal);
  else
    m_TrackProperty.SetItem(&NameVal);
}

void CTrackPropertySheet::OnPaint()
{
  CPaintDC dc(this);// device context for painting
}

void CTrackPropertySheet::EmptyTable()
{
  m_TrackProperty.DeleteAllItems();
}

void CTrackPropertySheet::LoadTable(Observer *obj)
{
  if (obj->isSingleVariable()) {
    SingleVariableObserver *svobj = (SingleVariableObserver *)obj;
    EmptyTable();
    char buff[256];

    wsprintf(buff, "%s", *(svobj->GetName()));
    InsertTableString(0, 0, buff, (LPARAM)svobj);
    InsertTableString(0, 1, toString(svobj->ptr->GetValue()), (LPARAM)svobj);
    // InsertTableString(
    // 0,2,(LPSTR)(LPCTSTR)(svobj->GetDescription()),(LPARAM)svobj);

    wsprintf(buff, "%s", *(svobj->GetDescription()));
    InsertTableString(0, 2, buff, (LPARAM)svobj);

    m_TrackProperty.SetItemData(0, (LPARAM)svobj);
  }
  if (obj->isMultiVariable()) {
    EmptyTable();
    obj->LoadPropertyTable(this);
  }
}

void CTrackPropertySheet::OnLButtonDown(UINT nFlags, CPoint point)
{}

void CTrackPropertySheet::OnSize(UINT nType, int cx, int cy)
{
  CSizingControlBar::OnSize(nType, cx, cy);

  int border = 4;

  CRect rc;
  GetClientRect(rc);

  int bottom = (IsHorz() || IsFloating()) ? rc.Height() - (2 * border)
                                          : rc.Height() - (2 * border) - 4;

  m_TrackProperty.SetWindowPos(NULL, rc.left + border, rc.top + border, rc.Width() - (2 * border), bottom, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CTrackPropertySheet::OnDestroy()
{
  CRect rc;
  GetClientRect(rc);

  WR_PROFILE(PropWidth, Preferences, rc.Width());
  WR_PROFILE(PropHeight, Preferences, rc.Height() + 8);

  CSizingControlBar::OnDestroy();
}
