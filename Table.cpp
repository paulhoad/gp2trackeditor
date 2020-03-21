// Table.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEditor.h"
#include "Table.h"
#include "TableColumn.h"
#include "TableRowElement.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CTable

CTable::CTable(CWnd *parentView):
parent(parentView)
{
	TableRows    = new Vector("New TableRows",__LINE__);
	TableColumns = new Vector("New TableColumns",__LINE__);
	brush   = new CBrush();
	hibrush = new CBrush();
	ltpen   = new CPen(PS_SOLID,1,GetSysColor(COLOR_3DHILIGHT));
	dkpen   = new CPen(PS_SOLID,1,GetSysColor(COLOR_3DSHADOW));
	pen     = new CPen(PS_SOLID,1,GetSysColor(COLOR_BTNTEXT));
	brush->CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	hibrush->CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
	ColumnBeingDragged = -1;
	draggingColumn = FALSE;
	
}

CTable::~CTable()
{
   delete TableRows;
   delete TableColumns;
   delete brush;
   delete hibrush;
   delete ltpen;
   delete dkpen;
   delete pen;
} 

void CTable::empty()
{
	if (TableRows)    
		 TableRows->removeAllData();
	if (TableColumns) 
		TableColumns->removeAllData();

}


void CTable::addColumn(LPCSTR title,int width)
{
   CTableColumn *col = new CTableColumn(title,width);
   TableColumns->addElement(col);
}

/*
void CTable::addRow(CString **array,int cols)
{
	Vector *row = new Vector("Addrow new row",__LINE__);
	for(int i=0;i<cols;i++)
	{
		CTableRowElement *elm = new CTableRowElement(array[i]);
		row->addElement(elm);
	}
	TableRows->addElement(row);
}
*/

void CTable::addRow(CString **array,int cols,int icon,WORD extraData)
{
	Vector *row = new Vector("Addrow new row",__LINE__);
	for(int i=0;i<cols;i++)
	{
		CTableRowElement *elm = new CTableRowElement(array[i],icon,extraData);
		row->addElement(elm);
	}
	TableRows->addElement(row);
}

void CTable::Draw(CDC *pDC,RECT *rect)
{
#ifdef DOUBLE_BUFFER
	//RECT rect;

	CDC *pMemDC;

	pMemDC = new CDC();

	//GetClientRect(&rect);
	CBitmap * bitmap = new CBitmap();
	CBitmap * oldBitmap;

	int width  = rect->right-rect->left;
	int height = rect->bottom-rect->top;

	bitmap->CreateCompatibleBitmap(pDC,width,height);
	pMemDC->CreateCompatibleDC(NULL);

	oldBitmap = pMemDC->SelectObject(bitmap);
	pMemDC->SelectStockObject(WHITE_BRUSH);
	pMemDC->SelectStockObject(WHITE_PEN);
	pMemDC->Rectangle(0,0,width,height);
	pMemDC->SelectStockObject(BLACK_PEN);

	//CPoint offset = GetDeviceScrollPosition();
	//pMemDC->SetViewportOrg(offset.x,offset.y);
	//pMemDC->SetWindowOrg(offset);
	OnMyDraw(pMemDC,rect);

	CPoint offset = getOffset();

	pDC->BitBlt(0,0,width,height,pMemDC,0,0,SRCCOPY);

	delete bitmap;
	delete pMemDC;
#else
	OnMyDraw(pDC,rect);
#endif
}

void CTable::OnMyDraw(CDC *pDC,RECT *rect)
{
	int xpos = 3;
	int ypos = 2;
	int i;
	CPoint offset = getOffset();

	pDC->SelectObject(GetStockObject(ANSI_VAR_FONT));

	rect->top = offset.y;
	rect->bottom = rect->top+18;

	int height = rect->bottom - rect->top;

	if (TableColumns != NULL)
	{
	  ypos +=14;

	  for(i=0;i<TableRows->size();i++)
	  {
		Vector  *row = (Vector*)TableRows->elementAt(i);
		xpos = 3;
		for(int j=0;j<row->size();j++)
	    { 
		 int width=0;
		 if (row!=NULL)
		 {
		  CTableColumn      *col = (CTableColumn*)TableColumns->elementAt(j);
		  CTableRowElement  *elm = (CTableRowElement *)row->elementAt(j);
		  
		  if (col!=NULL && elm!=NULL)
		  {
		   width       = col->getWidth();

		   if (j >= TableColumns->size()-1)
		   {
			width = rect->right-xpos;
			//AfxMessageBox("Last Column"+str,MB_OK);
		   }
		   CString *str = elm->getValue();

		   int x1 = xpos+3;

		   if (j == 0 && elm->icon != -1)
		   {				 
	            CBitmap bmp, *poldbmp;
                CDC memdc;

	            // Load the bitmap resource
                bmp.LoadBitmap( elm->icon );
 
                // Create a compatible memory DC
                memdc.CreateCompatibleDC( pDC );
 
                // Select the bitmap into the DC
                poldbmp = memdc.SelectObject( &bmp );
 
                // Copy (BitBlt) bitmap from memory DC to screen DC
                pDC->BitBlt( (int)x1, (int)ypos, 16, 15, &memdc, 0, 0, SRCAND );
 
                memdc.SelectObject( poldbmp );
				x1 += 18;
		   }

		   if (!isSelected(i))
		   {
			 
		     pDC->SetBkColor(GetSysColor(COLOR_WINDOW));
			 pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
			 pDC->SelectObject(GetStockObject(WHITE_BRUSH));
			 pDC->SelectObject(GetStockObject(WHITE_PEN));
		     pDC->Rectangle(x1,ypos,x1+width,ypos+height);				    
			 pDC->SelectObject(GetStockObject(COLOR_WINDOWTEXT));
		     pDC->TextOut(x1,ypos,*str);
			 
		   }
		   else
		   {
		    pDC->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
		    RECT hightlight;
		    hightlight.left  = x1+offset.x;
		    hightlight.right = rect->right+offset.x;
		    hightlight.top   = ypos;
		    hightlight.bottom= ypos+14;

		    pDC->FillRect(&hightlight,hibrush);

		    pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
			//pDC->SelectObject(GetStockObject(WHITE_BRUSH));
		    //pDC->Rectangle(xpos,ypos,xpos+width,ypos+height);		
		    pDC->TextOut(x1,ypos,*str);
		   }
		  }
		 }
		 xpos+=width;
		}
		ypos +=14;
	  }

	  RECT nrect;

	  nrect.top    = rect->top;
	  nrect.bottom = rect->bottom;
	  nrect.left   = rect->left;
	  nrect.right  = rect->right;

	  nrect.left  += offset.x;
	  nrect.right += offset.x;

	  pDC->FillRect(&nrect,brush);

	  ypos = offset.y;
	  xpos = 0;
	  for(i=0;i<TableColumns->size();i++)
	  {
		CTableColumn *col = (CTableColumn*)TableColumns->elementAt(i);
		int width   = col->getWidth();
		CString str = col->getTitle();

		if (i==TableColumns->size()-1)
		{
			width = rect->right-xpos;
			//AfxMessageBox("Last Column"+str,MB_OK);
		}
		
		

		
	    pDC->SetBkColor(GetSysColor(COLOR_3DFACE));
		pDC->TextOut(xpos+3,ypos,str);


		pDC->SelectObject(ltpen);
		pDC->MoveTo(xpos,ypos+1);
		pDC->LineTo(xpos,ypos+height-2);
		pDC->MoveTo(xpos,ypos+1);
		pDC->LineTo(xpos+width-1,ypos+1);

		pDC->SelectObject(dkpen);
		pDC->MoveTo(xpos+width-2,ypos+1);
		pDC->LineTo(xpos+width-2,ypos+height-2);
		pDC->LineTo(xpos,ypos+height-2);

		pDC->SelectObject(pen);
		pDC->MoveTo(xpos,ypos+0);
		pDC->LineTo(xpos+width-1,ypos+0);
		pDC->MoveTo(xpos,ypos+height-1);
		pDC->LineTo(xpos+width-1,ypos+height-1);
		pDC->MoveTo(xpos+width-1,ypos+0);
		pDC->LineTo(xpos+width-1,ypos+height);
		xpos+=width;
	  }

	}
}

static char lpszTabFilter[] = "Table Files (*.txt)|*.txt|Comma Separated (*.csv)|*.csv|All Files (*.*)|*.*|";


void CTable::ExportTableData()
{
	char fileName[256];

	strcpy(fileName,"unnamed.txt");
	CFileDialog *fdlg = new CFileDialog(FALSE,"csv",(LPCSTR)&fileName,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,lpszTabFilter);
	
	int result = fdlg->DoModal();

	if (result != IDCANCEL)
	{
	   CString filename = fdlg->GetFileName();
	   Export(filename);
	}
}

void CTable::Export(CString filename)
{
    int i;

	FILE *fp = fopen(filename,"w");

	if (fp == NULL)
	{
		AfxMessageBox("Failed to Export File");
		return;
	}

	for(i=0;i<TableColumns->size();i++)
	  {
		CTableColumn *col = (CTableColumn*)TableColumns->elementAt(i);
		CString str = col->getTitle();

		if (i>0)
		{
		  fprintf(fp,",");
		}
		
	
		fprintf(fp,"%s",(LPCTSTR)str);
	  }
	  fprintf(fp,"\n");


    if (TableColumns != NULL)
	{
	   for(i=0;i<TableRows->size();i++)
	   {
		Vector  *row = (Vector*)TableRows->elementAt(i);
	    for(int j=0;j<row->size();j++)
	    { 
		 int width=0;
		 if (row!=NULL)
		 {
		  CTableColumn      *col = (CTableColumn*)TableColumns->elementAt(j);
		  CTableRowElement  *elm = (CTableRowElement *)row->elementAt(j);
		  
		  if (col!=NULL && elm!=NULL)
		  {
		   width       = col->getWidth();

		   if (j >= 1)
		   {
			  fprintf(fp,",");

		   }

		    CString *str = elm->getValue();
		    fprintf(fp,"%s",(LPCTSTR)*str);
		   }
		  }		 
		 }
		 fprintf(fp,"\n");
		}
	  }


	  

    if (fp != NULL)
	{
		char msg[256];

		fclose(fp);
		

		wsprintf(msg,"Exported Table Data to %s",filename);
		AfxMessageBox(msg);
	}
}


BOOL CTable::isSelected(int i)
{
	if (i == selectedColumn) return TRUE;
	return FALSE;
}





BEGIN_MESSAGE_MAP(CTable, CWnd)
	//{{AFX_MSG_MAP(CTable)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTable message handlers

void CTable::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint off = getOffset();
	if (point.y > 14)
	{
	  selectedColumn = ((point.y+off.y)/14)-1; 
	}
	else
	{
	  //in table heading
	  int xpos = (point.x+off.x);
	  int x1pos=0;
	  for(int i=0;i<TableColumns->size();i++)
	  {
		CTableColumn *col = (CTableColumn*)TableColumns->elementAt(i);
		int width   = col->getWidth();
		PreviousColumnWidth = x1pos;
		x1pos+=width;
		
		if (abs(x1pos-xpos)<5)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENS));
			ColumnBeingDragged = i;
			draggingColumn=TRUE;
			break;
		}
	  }

	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CTable::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (dlg!=NULL)
	{
		dlg->DoModal();
	}
	
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CTable::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	draggingColumn=FALSE;
	ColumnBeingDragged = -1;

	RECT rect;
    parent->GetClientRect(&rect);
	parent->InvalidateRect(&rect);

	
	CWnd::OnLButtonUp(nFlags, point);
}

void CTable::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint off = getOffset();

	if (draggingColumn && ColumnBeingDragged >= 0)
	{
	  
	  CTableColumn *col = (CTableColumn*)TableColumns->elementAt(ColumnBeingDragged);
	  int xpos   = (point.x+off.x);
	  int colWidth = xpos - PreviousColumnWidth;

	  if (colWidth > 7)
	  {
	    col->width = colWidth;
	  }

	  RECT rect;
	  parent->GetClientRect(&rect);
	  parent->InvalidateRect(&rect);
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CTable::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	

	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}
