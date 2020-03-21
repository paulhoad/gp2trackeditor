// PaletteChooser.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "PaletteChooser.h"
#include "Pallette.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaletteChooser dialog

CPaletteChooser::CPaletteChooser(CWnd* pParent /*=NULL*/)
  : CDialog(CPaletteChooser::IDD, pParent)
{
  //{{AFX_DATA_INIT(CPaletteChooser)
  m_BVal = _T("");
  m_GVal = _T("");
  m_ColIndex = _T("");
  m_RVal = _T("");
  //}}AFX_DATA_INIT
  // selectedColor = 0;
}

void CPaletteChooser::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPaletteChooser)
  DDX_Control(pDX, IDC_PALETTE, m_PaletteCtrl);
  DDX_Text(pDX, IDC_BVAL, m_BVal);
  DDX_Text(pDX, IDC_GVAL, m_GVal);
  DDX_Text(pDX, IDC_COLORNUM, m_ColIndex);
  DDX_Text(pDX, IDC_RVAL, m_RVal);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPaletteChooser, CDialog)
//{{AFX_MSG_MAP(CPaletteChooser)
ON_WM_DRAWITEM()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaletteChooser message handlers

#define SIZE 12

void CPaletteChooser::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
  int selectedColor = m_PaletteCtrl.getSelectedColor();
#if 0
	// TODO: Add your message handler code here and/or call default
	if (nIDCtl == IDC_PALETTE)
	{
      CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
      CRect rcItem(lpDrawItemStruct->rcItem);
	  pDC->SelectStockObject(BLACK_PEN);
	 
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
	}
#endif
  if (nIDCtl == IDC_PALETTE) {
    m_PaletteCtrl.OnDrawItem(nIDCtl, lpDrawItemStruct);
  } else if (nIDCtl == IDC_SELECTED_COLOR) {
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rcItem(lpDrawItemStruct->rcItem);
    CBrush* brush =
      new CBrush(RGB(palette[selectedColor].r, palette[selectedColor].g, palette[selectedColor].b));
    CBrush* brushold = pDC->SelectObject(brush);
    pDC->SelectStockObject(BLACK_PEN);
    pDC->Rectangle(rcItem);
    pDC->SelectObject(brushold);
    delete brush;
  }

  m_ColIndex.Format("%d", selectedColor);
  m_RVal.Format("%d (0x%x)", palette[selectedColor].r, palette[selectedColor].r);
  m_GVal.Format("%d (0x%x)", palette[selectedColor].g, palette[selectedColor].g);
  m_BVal.Format("%d (0x%x)", palette[selectedColor].b, palette[selectedColor].b);
  UpdateData(FALSE);
  // CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

LRESULT
CPaletteChooser::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
  // TODO: Add your specialized code here and/or call the base class

  return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CPaletteChooser::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult)
{
  // TODO: Add your specialized code here and/or call the base class
  if (GetDlgItem(IDC_SELECTED_COLOR))
    GetDlgItem(IDC_SELECTED_COLOR)->InvalidateRect(NULL, TRUE);

  return CDialog::OnChildNotify(message, wParam, lParam, pLResult);
}
