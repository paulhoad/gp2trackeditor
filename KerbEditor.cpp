// KerbEditor.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "KerbEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKerbEditor dialog

CKerbEditor::CKerbEditor(CWnd* pParent /*=NULL*/)
  : CDialog(CKerbEditor::IDD, pParent)
{
  //{{AFX_DATA_INIT(CKerbEditor)
  m_X1 = 0;
  m_X2 = 0;
  m_Y1 = 0;
  m_Y2 = 0;
  //}}AFX_DATA_INIT
}

void CKerbEditor::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CKerbEditor)
  DDX_Text(pDX, IDC_X1, m_X1);
  DDX_Text(pDX, IDC_X2, m_X2);
  DDX_Text(pDX, IDC_Y1, m_Y1);
  DDX_Text(pDX, IDC_Y2, m_Y2);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKerbEditor, CDialog)
//{{AFX_MSG_MAP(CKerbEditor)
ON_WM_DRAWITEM()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKerbEditor message handlers

BOOL CKerbEditor::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO: Add extra initialization here

  return TRUE;// return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

static void
  drawPoint(CDC* g, short x1, short y1)
{
  int size = 2;

  g->Ellipse(x1 - size, y1 - size, x1 + size, y1 + size);
  g->MoveTo(x1, y1 - size);
  g->LineTo(x1, y1 + size);
  g->MoveTo(x1 - size, y1);
  g->LineTo(x1 + size, y1);
}

void CKerbEditor::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
  if (nIDCtl == IDC_PREVIEW) {
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rcItem(lpDrawItemStruct->rcItem);

    pDC->FillRect(rcItem, &CBrush(::GetSysColor(COLOR_WINDOW)));
    pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));

    CBrush *oldbrush, *redBrush;
    redBrush = new CBrush(0x0000FF);
    oldbrush = pDC->SelectObject(redBrush);
    pDC->MoveTo(0, rcItem.Height());
    pDC->LineTo(m_X1 / 2, rcItem.Height() - m_Y1);
    pDC->LineTo(m_X2 / 2, rcItem.Height() - m_Y2);
    drawPoint(pDC, m_X1 / 2, rcItem.Height() - m_Y1);
    drawPoint(pDC, m_X2 / 2, rcItem.Height() - m_Y2);
    pDC->SelectObject(oldbrush);
    delete redBrush;
  }

  CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
