// MyTableView.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "MyTableView.h"
#include "ImportDataElement.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTableView

IMPLEMENT_DYNCREATE(CMyTableView, CListView)

CMyTableView::CMyTableView() { useImages = FALSE; }

CMyTableView::~CMyTableView() {}

BEGIN_MESSAGE_MAP(CMyTableView, CListView)
//{{AFX_MSG_MAP(CMyTableView)
ON_WM_SETFOCUS()
ON_WM_PAINT()
ON_WM_KILLFOCUS()
ON_WM_CREATE()
ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
ON_WM_KEYDOWN()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
// ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)

/////////////////////////////////////////////////////////////////////////////
// CMyTableView drawing

void CMyTableView::OnDraw(CDC* pDC)
{}

void CMyTableView::addColumn(LPCSTR name, int width, int idx)
{
  if (GetListCtrl()) {
    GetListCtrl().InsertColumn(idx, name, LVCFMT_LEFT, width, idx);
  } else {
    AfxMessageBox("Table Error 0001: Failed to add Column Correctly");
  }
}

void CMyTableView::UpdateTableStringPtr(BOOL update, int row, int index, LPCSTR name, int ImageId)
{
  int mask = LVIF_TEXT;

  if (ImageId != -1) {
    if (useImages && ((CTrackEditorApp*)AfxGetApp())->showImageInTable) {
      mask |= LVIF_IMAGE;
    }
  }

  if (index == 0 && !update) {
    GetListCtrl().InsertItem(mask, row, name, 0, 0, ImageId, 0);
  } else {
    GetListCtrl().SetItem(row, index, mask, name, ImageId, 0, 0, 0);
  }
}

/////////////////////////////////////////////////////////////////////////////
// CMyTableView diagnostics

#ifdef _DEBUG
void CMyTableView::AssertValid() const
{
  CListView::AssertValid();
}

void CMyTableView::Dump(CDumpContext& dc) const
{
  CListView::Dump(dc);
}
#endif//_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyTableView message handlers

BOOL CMyTableView::PreCreateWindow(CREATESTRUCT& cs)
{
  // default is report view and full row selection
  cs.style &= ~LVS_TYPEMASK;
  cs.style &= ~LVS_SHOWSELALWAYS;
  cs.style |= LVS_REPORT | LVS_OWNERDRAWFIXED | LVS_SHAREIMAGELISTS;
  // cs.style |= LVS_EDITLABELS;

  return CListView::PreCreateWindow(cs);
}

void CMyTableView::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
  CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
  CRect rcItem(lpDrawItemStruct->rcItem);
  int nItem = lpDrawItemStruct->itemID;
  CImageList* pImageList;

  // Save dc state
  int nSavedDC = pDC->SaveDC();

  // Get item image and state info
  LV_ITEM lvi;
  lvi.mask = LVIF_IMAGE | LVIF_STATE;
  lvi.iItem = nItem;
  lvi.iSubItem = 0;
  lvi.stateMask = 0xFFFF;// get all state flags
  GetListCtrl().GetItem(&lvi);

  // Should the item be highlighted
  BOOL bHighlight =
    ((lvi.state & LVIS_DROPHILITED) || ((lvi.state & LVIS_SELECTED) && ((GetFocus() == this) || (GetStyle() & LVS_SHOWSELALWAYS))));

  // Get rectangles for drawing
  CRect rcBounds, rcLabel, rcIcon;
  GetListCtrl().GetItemRect(nItem, rcBounds, LVIR_BOUNDS);
  GetListCtrl().GetItemRect(nItem, rcLabel, LVIR_LABEL);
  GetListCtrl().GetItemRect(nItem, rcIcon, LVIR_ICON);
  CRect rcCol(rcBounds);

  CString sLabel = GetListCtrl().GetItemText(nItem, 0);

  // Labels are offset by a certain amount
  // This offset is related to the width of a space character
  int offset = pDC->GetTextExtent(_T(" "), 1).cx * 2;

  CRect rcHighlight;
  CRect rcWnd;
  int nExt;
  switch (m_nHighlight) {
  case 0:
    nExt = pDC->GetOutputTextExtent(sLabel).cx + offset;
    rcHighlight = rcLabel;
    if (rcLabel.left + nExt < rcLabel.right)
      rcHighlight.right = rcLabel.left + nExt;
    break;
  case 1:
    rcHighlight = rcBounds;
    rcHighlight.left = rcLabel.left;
    break;
  case 2:
    GetClientRect(&rcWnd);
    rcHighlight = rcBounds;
    rcHighlight.left = rcLabel.left;
    rcHighlight.right = rcWnd.right;
    break;
  default:
    rcHighlight = rcLabel;
  }

  // Draw the background color
  if (bHighlight) {
    pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
    pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));

    pDC->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_HIGHLIGHT)));
  } else
    pDC->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_WINDOW)));

  // Set clip region
  rcCol.right = rcCol.left + GetListCtrl().GetColumnWidth(0);
  CRgn rgn;
  rgn.CreateRectRgnIndirect(&rcCol);
  pDC->SelectClipRgn(&rgn);
  rgn.DeleteObject();

  // Draw state icon
  /*
if (lvi.state & LVIS_STATEIMAGEMASK)
{
  int nImage = ((lvi.state & LVIS_STATEIMAGEMASK)>>12) - 1;
  pImageList = GetListCtrl().GetImageList(LVSIL_STATE);
  if (pImageList)
  {
          pImageList->Draw(pDC, nImage,
                  CPoint(rcCol.left, rcCol.top), ILD_TRANSPARENT);
  }
}
  */

  // Draw normal and overlay icon
  pImageList = GetListCtrl().GetImageList(LVSIL_SMALL);
  if (pImageList && ((CTrackEditorApp*)AfxGetApp())->showImageInTable) {
    UINT nOvlImageMask = lvi.state & LVIS_OVERLAYMASK;

    pImageList->Draw(
      pDC, lvi.iImage, CPoint(rcIcon.left, rcIcon.top), (bHighlight ? ILD_BLEND50 : 0) | ILD_TRANSPARENT | nOvlImageMask);
  }

  // Draw item label - Column 0
  rcLabel.left += offset / 2;
  rcLabel.right -= offset;

  pDC->DrawText(sLabel, -1, rcLabel, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER | DT_END_ELLIPSIS);

  // Draw labels for remaining columns
  LV_COLUMN lvc;
  lvc.mask = LVCF_FMT | LVCF_WIDTH;

  if (m_nHighlight == 0)// Highlight only first column
  {
    pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
    pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
  }

  rcBounds.right =
    rcHighlight.right > rcBounds.right ? rcHighlight.right : rcBounds.right;
  rgn.CreateRectRgnIndirect(&rcBounds);
  pDC->SelectClipRgn(&rgn);

  for (int nColumn = 1; GetListCtrl().GetColumn(nColumn, &lvc); nColumn++) {
    rcCol.left = rcCol.right;
    rcCol.right += lvc.cx;

    // Draw the background if needed
    if (m_nHighlight == HIGHLIGHT_NORMAL)
      pDC->FillRect(rcCol, &CBrush(::GetSysColor(COLOR_WINDOW)));

    sLabel = GetListCtrl().GetItemText(nItem, nColumn);
    if (sLabel.GetLength() == 0) continue;

    // Get the text justification
    UINT nJustify = DT_LEFT;
    switch (lvc.fmt & LVCFMT_JUSTIFYMASK) {
    case LVCFMT_RIGHT:
      nJustify = DT_RIGHT;
      break;
    case LVCFMT_CENTER:
      nJustify = DT_CENTER;
      break;
    default:
      break;
    }

    rcLabel = rcCol;
    rcLabel.left += offset;
    rcLabel.right -= offset;

    pDC->DrawText(
      sLabel, -1, rcLabel, nJustify | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
  }

  // Draw focus rectangle if item has focus
  if (lvi.state & LVIS_FOCUSED && (GetFocus() == this))
    pDC->DrawFocusRect(rcHighlight);

  // Restore dc
  pDC->RestoreDC(nSavedDC);
}

void CMyTableView::RepaintSelectedItems()
{
  CRect rcBounds, rcLabel;

  // Invalidate focused item so it can repaint
  int nItem = GetListCtrl().GetNextItem(-1, LVNI_FOCUSED);

  if (nItem != -1) {
    GetListCtrl().GetItemRect(nItem, rcBounds, LVIR_BOUNDS);
    GetListCtrl().GetItemRect(nItem, rcLabel, LVIR_LABEL);
    rcBounds.left = rcLabel.left;

    InvalidateRect(rcBounds, FALSE);
  }

  // Invalidate selected items depending on LVS_SHOWSELALWAYS
  if (!(GetListCtrl().GetStyle() & LVS_SHOWSELALWAYS)) {
    for (nItem = GetListCtrl().GetNextItem(-1, LVNI_SELECTED); nItem != -1;
         nItem = GetListCtrl().GetNextItem(nItem, LVNI_SELECTED)) {
      GetListCtrl().GetItemRect(nItem, rcBounds, LVIR_BOUNDS);
      GetListCtrl().GetItemRect(nItem, rcLabel, LVIR_LABEL);
      rcBounds.left = rcLabel.left;

      InvalidateRect(rcBounds, FALSE);
    }
  }

  UpdateWindow();
}

void CMyTableView::OnPaint()
{
  // in full row select mode, we need to extend the clipping region
  // so we can paint a selection all the way to the right
  if (m_nHighlight == HIGHLIGHT_ROW && (GetStyle() & LVS_TYPEMASK) == LVS_REPORT) {
    CRect rcBounds;
    GetListCtrl().GetItemRect(0, rcBounds, LVIR_BOUNDS);

    CRect rcClient;
    GetClientRect(&rcClient);
    if (rcBounds.right < rcClient.right) {
      CPaintDC dc(this);

      CRect rcClip;
      dc.GetClipBox(rcClip);

      rcClip.left = min(rcBounds.right - 1, rcClip.left);
      rcClip.right = rcClient.right;

      InvalidateRect(rcClip, FALSE);
    }
  }

  CListView::OnPaint();
}

void CMyTableView::OnKillFocus(CWnd* pNewWnd)
{
  CListView::OnKillFocus(pNewWnd);

  // check if we are losing focus to label edit box
  if (pNewWnd != NULL && pNewWnd->GetParent() == this) return;

  // repaint items that should change appearance
  if ((GetStyle() & LVS_TYPEMASK) == LVS_REPORT) RepaintSelectedItems();
}

void CMyTableView::OnSetFocus(CWnd* pOldWnd)
{
  CListView::OnSetFocus(pOldWnd);

  // check if we are getting focus from label edit box
  if (pOldWnd != NULL && pOldWnd->GetParent() == this) return;

  // repaint items that should change appearance
  if ((GetStyle() & LVS_TYPEMASK) == LVS_REPORT) RepaintSelectedItems();
}

int CMyTableView::SetHighlightType(EHighlight hilite)
{
  int oldhilite = m_nHighlight;
  if (hilite <= HIGHLIGHT_ROW) {
    m_nHighlight = hilite;
    Invalidate();
  }
  return oldhilite;
}

int CMyTableView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CListView::OnCreate(lpCreateStruct) == -1) return -1;

  // TODO: Add your specialized creation code here
  SetHighlightType(HIGHLIGHT_ROW);

  return 0;
}

void CMyTableView::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
  NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
  // TODO: Add your control notification handler code here
  // if (pNMListView->uNewState)
  //{
  SelectedItemIndex = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
  // TRACE("Selected=%d\n",SelectedItemIndex);
  //}
  *pResult = 0;
}

void CMyTableView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
  // TODO: Add your control notification handler code here
  OnClick(pNMHDR, pResult);
}

static char lpszTabFilter[] =
  "Table Files (*.txt)|*.txt|Comma Separated Text(*.csv)|*.csv|All Files "
  "(*.*)|*.*|";

void CMyTableView::ExportTableData()
{
  char fileName[256];

  strcpy(fileName, "unnamed.txt");
  CFileDialog* fdlg =
    new CFileDialog(FALSE, "csv", (LPCSTR)&fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszTabFilter);

  int result = fdlg->DoModal();

  if (result != IDCANCEL) {
    CString filename = fdlg->GetFileName();
    exportFile(filename);
  }

  delete fdlg;
}

static char lpszTabInFilter[] = "Table Files Comma Separated (*.csv)|*.csv|";

Vector*
  CMyTableView::ImportTableData()
{
  char fileName[256];

  strcpy(fileName, "*.csv");
  CFileDialog* fdlg =
    new CFileDialog(TRUE, "csv", (LPCSTR)&fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszTabInFilter);

  int result = fdlg->DoModal();

  if (result != IDCANCEL) {
    CString filename = fdlg->GetFileName();
    delete fdlg;
    return Import(filename);
  }

  delete fdlg;
  return NULL;
}

void CMyTableView::exportFile(CString filename)
{
  int i;

  FILE* fp = fopen(filename, "w");

  if (fp == NULL) {
    AfxMessageBox("Failed to Export File");
    return;
  }

  // Make sure that nCol is valid
  CHeaderCtrl* pHeader = (CHeaderCtrl*)GetListCtrl().GetDlgItem(0);
  int nColumnCount = pHeader->GetItemCount();

  for (int i = 0; i < nColumnCount; i++) {
    HD_ITEM hdi;
    hdi.mask = HDI_TEXT;
    hdi.pszText = (LPSTR)malloc(100 * sizeof(char));
    hdi.cchTextMax = 100;
    pHeader->GetItem(i, &hdi);
    if (i > 0) {
      fprintf(fp, ",");
    }
    fprintf(fp, "%s", (LPCSTR)hdi.pszText);
  }
  fprintf(fp, "\n");

  for (int j = 0; j < GetListCtrl().GetItemCount(); j++) {
    for (int i = 0; i < nColumnCount; i++) {
      CString str = GetListCtrl().GetItemText(j, i);

      if (i >= 1) {
        fprintf(fp, ",");
      }
      fprintf(fp, "%s", (LPCTSTR)str);
    }
    fprintf(fp, "\n");
  }

  if (fp != NULL) {
    char msg[256];

    fclose(fp);

    wsprintf(msg, "Exported Table Data to %s", filename);
    AfxMessageBox(msg);
  }
}

void CMyTableView::EmptyTable()
{
  GetListCtrl().DeleteAllItems();
  CHeaderCtrl* pHeader = (CHeaderCtrl*)GetListCtrl().GetDlgItem(0);
  if (pHeader != NULL) {
    int nColumnCount = pHeader->GetItemCount();

    for (int i = nColumnCount - 1; i >= 0; i--) {
      GetListCtrl().DeleteColumn(i);
    }
  }
}

Vector*
  CMyTableView::Import(CString filename)
{
  int i;

  FILE* fp = fopen(filename, "r");

  if (fp == NULL) {
    AfxMessageBox("Failed to Import File");
    return NULL;
  }

  // Make sure that nCol is valid
  CHeaderCtrl* pHeader = (CHeaderCtrl*)GetListCtrl().GetDlgItem(0);
  int nColumnCount = pHeader->GetItemCount();

  // read headings
  for (int i = 0; i < nColumnCount; i++) {
    char buff[256];
    // fscanf(fp,"%[^,],",buff);
    if (i == nColumnCount - 1) {
      fscanf(fp, "%[^\n]", buff);
    } else {
      fscanf(fp, "%[^,],", buff);
    }
    TRACE("%s\n", buff);
  }

  Vector* rows = new Vector("rows", __LINE__);

  int maxCount = 5000;

  while (!feof(fp)) {
    Vector* columns = new Vector("rows", __LINE__);
    for (int i = 0; i < nColumnCount; i++) {
      char buff[256];
      if (i == nColumnCount - 1) {
        fscanf(fp, "%[^\n]\n", buff);
      } else {
        fscanf(fp, "%[^,],", buff);
      }
      ImportDataElement* str = new ImportDataElement(buff);
      columns->addElement(str);
    }
    rows->addElement(columns);
    maxCount--;
    if (maxCount < 0) break;
  }

  return rows;
}

void CMyTableView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
  // TODO: Add your specialized code here and/or call the base class

  CListView::OnPrint(pDC, pInfo);
}

#define HEADER_HEIGHT 4
#define FOOTER_HEIGHT 3
#define LEFT_MARGIN 8
#define RIGHT_MARGIN 4

/*
void CMyTableView::OnBeginPrint(CDC *pDC, CPrintInfo *pInfo)
{
     // OnBeginPrinting() is called after the user has committed to
     // printing by OK'ing the Print dialog, and after the framework
     // has created a CDC object for the printer or the preview view.

     // This is the right opportunity to set up the page range.
     // Given the CDC object, we can determine how many rows will
     // fit on a page, so we can in turn determine how many printed
     // pages represent the entire document.

     if(NULL == pDC || NULL == pInfo)
          return;

     int  nMaxRowCount = GetListCtrl().GetItemCount();
     if(!nMaxRowCount)
          return;

     //let us do all dimesions in ListControl units (Device) rather than
     //printer device units. Since we have more control on them
     CDC  *pCtlDC = GetDC();
     if(NULL == pCtlDC)
          return ;
     TEXTMETRIC tm;
     pCtlDC->GetTextMetrics(&tm);
     m_nCharWidth = tm.tmAveCharWidth;

     pDC->SetMapMode(MM_ANISOTROPIC);

     CRect     rc;
     GetListCtrl().GetItemRect(0, &rc, LVIR_BOUNDS);
     m_nRowHeight = rc.Height();

     int nMargins = (LEFT_MARGIN+RIGHT_MARGIN)*m_nCharWidth;
     //this will optimize the column widths. If we have more column than
     //screen width and horizontal scroll on List control
     //this will reduce the column widths proportonately to fit all of them
     //on the page. If we have fewer column all the column
     //widths will increased propertionately.
     pDC->SetWindowExt(rc.Width() + nMargins,
pCtlDC->GetDeviceCaps(LOGPIXELSX));

     pDC->SetViewportExt(pDC->GetDeviceCaps(HORZRES),pDC->GetDeviceCaps(LOGPIXELSX));
     double d =
(double)pDC->GetDeviceCaps(LOGPIXELSY)/(double)pCtlDC->GetDeviceCaps(LOGPIXELSY);
     ReleaseDC(pCtlDC);

     nMargins = (int)(m_nRowHeight*d);
     int nPageHeight     = pDC->GetDeviceCaps(VERTRES);
     m_nRowsPerPage = nPageHeight/nMargins;   //nMargins reused
     m_nRowsPerPage -= (HEADER_HEIGHT+FOOTER_HEIGHT);
     m_nRowsPerPage -= 1; //adjustment for list control header
     int nMaxPage = nMaxRowCount/m_nRowsPerPage + 1;
     pInfo->SetMaxPage(nMaxPage);
     pInfo->m_nCurPage = 1;  // start printing at page# 1
}

void CMyTableView::OnPrint(CDC *pDC, CPrintInfo *pInfo)
{
     if(NULL == pDC || NULL == pInfo)
          return;

     // Draw as many rows as will fit on the printed page.
     // Clip the printed page so that there is no partially shown
     // row at the bottom of the page (the same row which will be fully
     // shown at the top of the next page).
     int       nStartRow = 0;
     int       nEndRow = 0;
     int       nMaxRowCount = GetListCtrl().GetItemCount();

     nStartRow = (pInfo->m_nCurPage - 1)*m_nRowsPerPage;
     nEndRow = nStartRow+m_nRowsPerPage;
     if(nEndRow > nMaxRowCount)
          nEndRow = nMaxRowCount;

     //create bold font for header and footer
     CFont     *pOldFont = NULL;
     CFont     BoldFont;
     LOGFONT  lf;
     pOldFont = GetFont();;
     pOldFont->GetLogFont(&lf);
     lf.lfWeight = FW_BOLD;
     BoldFont.CreateFontIndirect(&lf);

     pOldFont = pDC->SelectObject(&BoldFont);
     int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);
     //print the header
     PrintHeader(pDC, pInfo);
     //Print the footer
     PrintFooter(pDC, pInfo);
     pDC->SetBkMode(nPrevBkMode);
     pDC->SelectObject(pOldFont);
     BoldFont.DeleteObject();

     //Set origin to print header and Header control. Keep Y position at 0 to
     //print header information
     //Change Y position of origin to print List control header
     //Adjust the List control origin to start printing after page margins
     pDC->SetWindowOrg(-1*(LEFT_MARGIN*m_nCharWidth),
-1*HEADER_HEIGHT*m_nRowHeight);

         m_HeaderCtrl = (CHeaderCtrl*)GetListCtrl().GetDlgItem(0);

     //send a message to Header control to print itsef. There is little scope to
improve printing header.
     //I have m_HeaderCtrl in my class. If not take by HeaderCtrl =
GetDlgItem(0); m_HeaderCtrl->SendMessage(WM_PAINT, (WPARAM)pDC->m_hDC);

     //Chage window position to take care of ListControl Horizontal scrolling.
     //if List control is scrolled to left horizontally the above window origin
     //will not start painting from first column, instead it starts painting
from
     //first visible column, because rcBounds etc are will have -ve left value
     //Same thing with vertical scrolling also
     CRect     rcBounds;
     GetListCtrl().GetItemRect(nStartRow, &rcBounds, LVIR_BOUNDS);

     //offset top margin of rcBounds by ListControl header
     CRect     rc;
     m_HeaderCtrl->GetClientRect(&rc);
     rcBounds.OffsetRect(0, -rc.Height());
     pDC->OffsetWindowOrg(rcBounds.left, rcBounds.top);
     //start printing rows
     for(int i = nStartRow; i < nEndRow; i++)
          DrawRow(pDC, i);

     //SetWindowOrg back for next page
     pDC->SetWindowOrg(0,0);
     return;
}

void CMyTableView::PrintHeader(CDC *pDC, CPrintInfo *pInfo)
{
     CRect               rc(pInfo->m_rectDraw);
     CString             sTemp;

     //Print App title
     rc.left += LEFT_MARGIN*m_nCharWidth;
     rc.right -= RIGHT_MARGIN*m_nCharWidth;
     rc.bottom = rc.top+m_nRowHeight;

     //print App title on top right margin
     sTemp.LoadString(AFX_IDS_APP_TITLE);
     pDC->DrawText(sTemp, &rc, DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX |
DT_VCENTER);


     //print Header. One row for each string
     POSITION  pos = NULL;
     pos = m_HeaderList.GetHeadPosition();
     while(pos)
     {
          sTemp = m_HeaderList.GetNext(pos);
          pDC->DrawText(sTemp, &rc, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX |
DT_VCENTER); rc.OffsetRect(0, m_nRowHeight);
     }

}


//print footer with a line and date, and page number
void CMyTableView::PrintFooter(CDC *pDC, CPrintInfo *pInfo)
{
     CRect     rc(pInfo->m_rectDraw);

     //draw line
     rc.left += LEFT_MARGIN*m_nCharWidth;
     rc.right -= RIGHT_MARGIN*m_nCharWidth;
     rc.top = rc.bottom - FOOTER_HEIGHT*m_nRowHeight;
     rc.bottom = rc.top + m_nRowHeight;
     pDC->MoveTo(rc.left, rc.top);
     pDC->LineTo(rc.right, rc.top);

     //draw page number
     CString   sTemp ;
     rc.OffsetRect(0, m_nRowHeight/2);
     //sTemp.Format(IDS_PRINT_PAGE_TITLE, pInfo->m_nCurPage);
     pDC->DrawText(sTemp,-1,rc, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX |
DT_NOCLIP | DT_VCENTER);

     CTime     t = CTime::GetCurrentTime();
     sTemp = t.Format("%c");
     pDC->DrawText(sTemp,-1,rc, DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX |
DT_NOCLIP | DT_VCENTER);
}

//this is drawing code copied RowList sample and from www.codeguru.com site
int CMyTableView::DrawRow(CDC *pDC, int nItem)
{
     CImageList          *pImageList = NULL;
     CFont               *pOldFont = NULL;
     CFont               BoldFont;
     CString             sLabel;
     UINT           dtFlags = DT_SINGLELINE|DT_NOPREFIX|DT_VCENTER;
     int                 nSaveDC = pDC->SaveDC();

     if(!pDC->IsPrinting())
          dtFlags |= (DT_NOCLIP | DT_END_ELLIPSIS);     //no clip because we add
ellipsis at the end
     // get item data
     LV_ITEM lvi;
     lvi.mask = LVIF_IMAGE | LVIF_STATE | LVIF_INDENT;
     lvi.iItem=nItem;
     lvi.iSubItem=0;
     lvi.stateMask=0xFFFF;         // get all state flags
     GetListCtrl().GetItem(&lvi);

     BOOL bHighlight = ((lvi.state & LVIS_DROPHILITED) || ((lvi.state &
LVIS_SELECTED)
                    && ((GetFocus() == this) || (GetStyle() &
LVS_SHOWSELALWAYS))));

     //Get rectangles for painting
     CRect     rcBounds, rcLabel, rcIcon;
     GetListCtrl().GetItemRect(nItem, rcBounds, LVIR_BOUNDS);
     GetListCtrl().GetItemRect(nItem, rcLabel, LVIR_LABEL);
     GetListCtrl().GetItemRect(nItem, rcIcon, LVIR_ICON);
     CRect     rcCol(rcBounds);

     CRect     rcWnd;
     sLabel = GetListCtrl().GetItemText(nItem, 0);
     //Label offset
     int offset = pDC->GetTextExtent(_T(" "), 1).cx;

     CRect     rcHighlight;
     int       nExt = 0;
     switch(m_nHighlight)
     {
     case HIGHLIGHT_NORMAL:
          nExt = pDC->GetOutputTextExtent(sLabel).cx + offset;
          rcHighlight = rcLabel;
          if(rcLabel.left + nExt < rcLabel.right)
               rcHighlight.right = rcLabel.left + nExt;
          break;

     case HIGHLIGHT_ALLCOLUMNS:
          rcHighlight = rcBounds;
          rcHighlight.left = rcLabel.left;
          break;

     case HIGHLIGHT_ROW:
          GetClientRect(&rcWnd);
          rcHighlight = rcBounds;
          rcHighlight.left = rcLabel.left;
          rcHighlight.right = rcWnd.right;
          break;

     default:
          rcHighlight.left = rcLabel.left;
          break;
     }

     //draw highlight. printing may not be required
     if(bHighlight)
     {
          pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
          pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
          pDC->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_HIGHLIGHT)));
     }
     else {
          pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
          pDC->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_WINDOW)));
     }

     //set clip region
     rcCol.right = rcCol.left + GetListCtrl().GetColumnWidth(0);

     //Nice to have regions but they are not working on printer DC we may need
     //to take get device caps to support regions. Does not seems to help much
now
     //CRgn    rgn;
     //rgn.CreateRectRgnIndirect(&rcCol);
     //pDC->SelectClipRgn(&rgn);
     //rgn.DeleteObject();

     //Draw state icon
     if(lvi.state & LVIS_STATEIMAGEMASK)
     {
          int nImage = ((lvi.state & LVIS_STATEIMAGEMASK) >> 12) - 1;
          pImageList = GetListCtrl().GetImageList(LVSIL_STATE);
          //offset the state image icon indent levels.
          nExt = rcCol.left + lvi.iIndent*rcIcon.Width();    //nExt reused
          if(pImageList)
               pImageList->Draw(pDC, nImage, CPoint(nExt, rcCol.top),
ILD_TRANSPARENT);
     }

     //Draw Normal and overlay icon
     pImageList = GetListCtrl().GetImageList(LVSIL_SMALL);  //assuming printing
in report mode only if(pImageList)
     {
          UINT nOvlImageMask = lvi.state & LVIS_OVERLAYMASK;
          pImageList->Draw(pDC, lvi.iImage, CPoint(rcIcon.left, rcIcon.top),
                        (bHighlight?ILD_BLEND50:0)|ILD_TRANSPARENT|nOvlImageMask);
     }

     //if state image mask is on and indent is 0 then consider it as Server row
     if((lvi.state & LVIS_STATEIMAGEMASK) && !lvi.iIndent)
     {
          //create bold font
          LOGFONT  lf;
          pOldFont = GetFont();;
          pOldFont->GetLogFont(&lf);
          lf.lfWeight = FW_BOLD;
          BoldFont.CreateFontIndirect(&lf);
          pOldFont = pDC->SelectObject(&BoldFont);
          rcLabel.right = rcBounds.right;     //draw server name to full row
width
     }

     //Draw item label
     rcLabel.left += offset/2;
     rcLabel.right -= offset;
     dtFlags |= DT_LEFT;
     pDC->DrawText(sLabel, rcLabel, dtFlags);

     if((lvi.state & LVIS_STATEIMAGEMASK) && !lvi.iIndent)
     {
          pOldFont = pDC->SelectObject(pOldFont);
          BoldFont.DeleteObject();
          //focus rect if required
          if(lvi.state & LVIS_FOCUSED && (GetFocus() == this))
               pDC->DrawFocusRect(rcHighlight);
          pDC->RestoreDC(nSaveDC);
          return 0;
     }

     //dRAW LABELS FOR REMAINING COLUMNS
     LV_COLUMN lvc;
     lvc.mask = LVCF_FMT|LVCF_WIDTH;

     if(m_nHighlight == HIGHLIGHT_NORMAL)
     {
          pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
          pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
     }
     rcBounds.right = rcHighlight.right > rcBounds.right ?
rcHighlight.right:rcBounds.right;

     //Nice to have regions but they are not working on printer DC we may need
     //to take get device caps to support regions. Does not seems to help much
now
     //rgn.CreateRectRgnIndirect(&rcBounds);
     //pDC->SelectClipRgn(&rgn);
     //rgn.DeleteObject();

     for(int nColumn = 1; GetListCtrl().GetColumn(nColumn, &lvc); nColumn++)
     {
          rcCol.left = rcCol.right;
          rcCol.right += lvc.cx;

          //draw background if needed
          if(m_nHighlight == HIGHLIGHT_NORMAL)
               pDC->FillRect(rcCol, &CBrush(::GetSysColor(COLOR_WINDOW)));

          sLabel = GetListCtrl().GetItemText(nItem, nColumn);
          if(sLabel.IsEmpty())
               continue;

          //Get the text justification
          UINT nJustify = DT_LEFT;
          switch(lvc.fmt & LVCFMT_JUSTIFYMASK)
          {
          case LVCFMT_RIGHT:
               nJustify = DT_RIGHT;
               break;

          case LVCFMT_CENTER:
               nJustify = DT_CENTER;
               break;

          default:
               break;
          }
          rcLabel = rcCol;
          rcLabel.left += offset;
          rcLabel.right -= offset;

          dtFlags &= ~DT_RIGHT;
          dtFlags &= ~DT_CENTER;
          dtFlags |= nJustify;
          pDC->DrawText(sLabel, -1, rcLabel, dtFlags);
     }
     //focus rect if required
     if(lvi.state & LVIS_FOCUSED && (GetFocus() == this))
          pDC->DrawFocusRect(rcHighlight);

     pDC->RestoreDC(nSaveDC);
     return 0;
}

void CMyTableView::SetHeaderString(CStringList *HeaderList)
{
     m_HeaderList.RemoveAll();
     m_HeaderList.AddTail(HeaderList);
}
*/

void CMyTableView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  CPoint pt;

  pt.x = 0;
  pt.y = 0;

  if (nChar == 13) {
    OnLButtonDblClk(0, pt);
  }
  CListView::OnKeyDown(nChar, nRepCnt, nFlags);
}
