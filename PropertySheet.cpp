// TrackTree.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEditor.h"
#include "PropertySheet.h"
#include "TrackSection.h"
#include "TrackCmd.h"
#include "InternalObject.h"
#include "JamFileEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertySheet dialog

CPropertySheet::CPropertySheet() : CSizingControlBar(), pImageList(NULL)
{
  //{{AFX_DATA_INIT(CPropertySheet)
  //}}AFX_DATA_INIT
  // m_TrackTree = NULL;
  ImagesNotLoaded = TRUE;
}

CPropertySheet::~CPropertySheet()
{
  if (pImageList != NULL) delete pImageList;
}

void CPropertySheet::LoadImages()
{
  pImageList = new CImageList();
  pImageList->Create(16, 15, TRUE, 150, 2);

  CBitmap bitmap;
  for (UINT iCnt = IDB_CIRCUIT; iCnt <= IDB_TYRE + 100; iCnt++) {
    bitmap.LoadBitmap(iCnt);
    pImageList->Add(&bitmap, (COLORREF)0xFFFFFF);
    bitmap.DeleteObject();
  }
  m_TrackTree.SetImageList(pImageList, TVSIL_NORMAL);

  ImagesNotLoaded = FALSE;
}

void CPropertySheet::DoDataExchange(CDataExchange* pDX)
{
  // CDialogBar::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CPropertySheet)
  DDX_Control(pDX, IDC_TRACKTREE, m_Tree);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPropertySheet, CSizingControlBar)
//{{AFX_MSG_MAP(CPropertySheet)
ON_WM_CREATE()
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_NOTIFY(NM_DBLCLK, IDC_TRACKTREE, OnDblclkTracktree)
ON_WM_SIZE()

//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertySheet message handlers

int CPropertySheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CSizingControlBar::OnCreate(lpCreateStruct) == -1) return -1;

  // TODO: Add your message handler code here
  if (!m_TrackTree.CreateEx(
        WS_EX_CLIENTEDGE, "SysTreeView32", "", TVS_HASLINES | TVS_HASBUTTONS | TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP | WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 10, 100, 100, GetSafeHwnd(), (HMENU)IDC_TRACKTREE))
    return -1;

  return 0;
}

void CPropertySheet::OnPaint()
{
  CPaintDC dc(this);// device context for painting

  if (m_TrackTree != NULL && ImagesNotLoaded) LoadImages();

  // Do not call CDialogBar::OnPaint() for painting messages
}

void CPropertySheet::OnLButtonDown(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  // AfxMessageBox("ButtonDown",MB_OK);

  // CDialogBar::OnLButtonDown(nFlags, point);
}

void CPropertySheet::OnSize(UINT nType, int cx, int cy)
{
  CSizingControlBar::OnSize(nType, cx, cy);

  int border = 4;

  // TODO: Add your message handler code here
  CRect rc;
  GetClientRect(rc);

  int bottom = (IsHorz() || IsFloating()) ? rc.Height() - (2 * border)
                                          : rc.Height() - (2 * border) - 4;

  m_TrackTree.SetWindowPos(NULL, rc.left + border, rc.top + border, rc.Width() - (2 * border), bottom, SWP_NOACTIVATE | SWP_NOZORDER);
  // m_TrackTree.SetWindowPos(NULL,0,0,200,100,SWP_NOACTIVATE);
}
