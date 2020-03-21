// Table.h : header file
//
#ifndef _CTABLE_H_
#define _CTABLE_H_

#include "Vector.h"

#include "TableColumn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CTable window

class CTable : public CWnd
{
// Construction
public:
	CTable(CWnd *parentView);

// Attributes
public:
	CBrush *brush;
	CBrush *hibrush;
	CPen   *ltpen;
	CPen   *dkpen;
	CPen   *pen;
	CWnd   *parent;

	BOOL draggingColumn;
	int  ColumnBeingDragged;
	int  PreviousColumnWidth;

// Operations
public:

	void Draw(CDC *pDC,RECT *rect);
	void OnMyDraw(CDC *pDC,RECT *rect);

	void addColumn(LPCSTR title,int width);

	//void addRow(CString **array,int cols);
	void addRow(CString **array,int cols,int icon=-1,WORD extraData=NULL);

	void empty();

	BOOL isSelected(int i);

	void Export(CString filename);
	void ExportTableData();

	void setSelected(int i)
	{
	   selectedColumn = i;
	   repaint();
	}

	void repaint()
	{
		RECT rect;
	    parent->GetClientRect(&rect);
	    parent->InvalidateRect(&rect);
	}

	int getSelectionIndex()
	{
		return selectedColumn;
	}

	void setTopRow(int idx)
	{
		int xpos = 0;
		int ypos = idx*14;
	    setOffset(CPoint(xpos,ypos));
	}

	void setOffset(CPoint offset)
	{
		begin = offset;
	}

	CPoint getOffset()
	{
		return begin;
	}

	CSize getSize()
	{
		CSize s;
		int width=0;

		for(int i=0;i<TableColumns->size();i++)
		{
			CTableColumn *col = (CTableColumn*)TableColumns->elementAt(i);
			width += col->getWidth();
		}

		width+=100;

		s.cx = width;
		s.cy = ((TableRows->size()+2)*15);

		return s;
	}

	int  selectedColumn;
	CPoint begin;

	Vector *TableRows;
	Vector *TableColumns;

	CDialog *dlg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTable)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTable();

	// Generated message map functions
public:
	//{{AFX_MSG(CTable)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif
