// MyTableView.h : header file
//
#ifndef _INCLUDED_TABLE_VIEW_H_
#define _INCLUDED_TABLE_VIEW_H_
#include <afxcview.h>

/////////////////////////////////////////////////////////////////////////////
// CMyTableView view

class CMyTableView : public CListView
{
protected:
  CMyTableView();// protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CMyTableView)

  // Attributes
public:
  enum EHighlight {
    HIGHLIGHT_NORMAL,
    HIGHLIGHT_ALLCOLUMNS,
    HIGHLIGHT_ROW
  };

protected:
  int m_nHighlight;// Indicate type of selection highlighting

  // Operations
public:
  void
    ExportTableData();
  Vector*
    ImportTableData();
  void
    exportFile(CString filename);
  Vector*
    Import(CString filename);
  void
    EmptyTable();

  BOOL useImages;

  // CImageList* pImageList;
  int SelectedItemIndex;

  int getTableSectionIndex()
  {
    return GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
  }

  void
    addColumn(LPCSTR name, int width, int idx);
  // void InsertTableString(int row,int index,CString name,int ImageId=-1);
  // void UpdateTableString(BOOL update,int row,int index,CString name,int
  // ImageId=-1);
  void
    UpdateTableStringPtr(BOOL update, int row, int index, LPCSTR name, int ImageId = -1);

  void
    DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
  void
    RepaintSelectedItems();
  int SetHighlightType(EHighlight hilite);

  void
    SetImageList(CImageList* pImageList, int flags)
  {
    if (pImageList != NULL) {
      GetListCtrl().SetImageList(pImageList, flags);
    }
  }

  void
    setSelected(int row)
  {
    int oldrow = getTableSectionIndex();

    if (oldrow >= 0) {
      GetListCtrl().SetItemState(oldrow, 0, LVIS_SELECTED | LVIS_FOCUSED);
    }

    GetListCtrl().SetItemState(row, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
  }

  /*
  void OnBeginPrint(CDC *pDC, CPrintInfo *pInfo);
  void PrintHeader(CDC *pDC, CPrintInfo *pInfo);
  void PrintFooter(CDC *pDC, CPrintInfo *pInfo);
  int DrawRow(CDC *pDC, int nItem);

  int m_nCharWidth;
int m_nRowHeight;
  int m_nRowsPerPage;
  CHeaderCtrl* m_HeaderCtrl;
  //m_HeaderList;
  */

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMyTableView)
protected:
  virtual void
    OnDraw(CDC* pDC);// overridden to draw this view
  virtual BOOL
    PreCreateWindow(CREATESTRUCT& cs);
  virtual void
    OnPrint(CDC* pDC, CPrintInfo* pInfo);
  //}}AFX_VIRTUAL

  // Implementation
protected:
  virtual ~CMyTableView();
#ifdef _DEBUG
  virtual void
    AssertValid() const;
  virtual void
    Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CMyTableView)
  afx_msg void
    OnSetFocus(CWnd* pOldWnd);
  afx_msg void
    OnPaint();
  afx_msg void
    OnKillFocus(CWnd* pNewWnd);
  afx_msg int
    OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void
    OnClick(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void
    OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void
    OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif
