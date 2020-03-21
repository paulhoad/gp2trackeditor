// PaletteCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "PaletteCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "Pallette.h"

/////////////////////////////////////////////////////////////////////////////
// CPaletteCtrl

CPaletteCtrl::CPaletteCtrl()
{
	setSelectedColor(0);

}

CPaletteCtrl::~CPaletteCtrl()
{
}


BEGIN_MESSAGE_MAP(CPaletteCtrl, CButton)
	//{{AFX_MSG_MAP(CPaletteCtrl)
	ON_WM_DRAWITEM()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaletteCtrl message handlers

#define SIZE 12


void CPaletteCtrl::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	  CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
      CRect rcItem(lpDrawItemStruct->rcItem);
	  pDC->SelectStockObject(BLACK_PEN);

	  //int SIZE = rcItem.Width()/16;
	 
	  int colourCount = 0;
	  for(int i=0;i<16;i++)
	  {
		  for(int j=0;j<16;j++)
		  {
			  int x1,y1;
			  x1 = j*SIZE;
			  y1 = i*SIZE;
			  CBrush *brush = new CBrush(RGB(palette[colourCount].r,palette[colourCount].g,palette[colourCount].b));
			  colourCount++;
			  CBrush *brushold = pDC->SelectObject(brush);  
			  pDC->Rectangle(x1,y1,x1+SIZE,y1+SIZE);
			  pDC->SelectObject(brushold);
			  delete brush;
		  }
	  }
	
	//CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CPaletteCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int xPos = point.x;  // horizontal position of cursor 
    int yPos = point.y;  // vertical position of cursor 

	int idx = ((yPos/SIZE)*16)+(xPos/SIZE);

	setSelectedColor(idx);
	//InvalidateRect(NULL,TRUE);

	GetParent()->InvalidateRect(NULL,TRUE);
	//GetParent()->Update();
	
	CButton::OnLButtonDown(nFlags, point);
}
