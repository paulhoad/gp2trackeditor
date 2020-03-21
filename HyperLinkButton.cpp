// HyperLinkButton.cpp : implementation file
//

#include "stdafx.h"
#include "HyperLinkButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHyperLinkButton

CHyperLinkButton::CHyperLinkButton()
{
  m_Over = false;
  m_OverColor = RGB(0, 0, 0xFF);
  m_NotOverColor = RGB(0, 0, 0x90);
}

CHyperLinkButton::~CHyperLinkButton() {}

BEGIN_MESSAGE_MAP(CHyperLinkButton, CButton)
//{{AFX_MSG_MAP(CHyperLinkButton)
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHyperLinkButton message handlers

void CHyperLinkButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
  CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
  CRect rcItem(lpDrawItemStruct->rcItem);

  if (m_Over)
    pDC->SetTextColor(m_OverColor);
  else
    pDC->SetTextColor(m_NotOverColor);

  CString str;
  GetWindowText(str);
  pDC->TextOut(0, 0, str);
}

void CHyperLinkButton::OnLButtonUp(UINT nFlags, CPoint point)
{
  CString str;
  GetWindowText(str);
  HINSTANCE h = ShellExecute(NULL, (LPCTSTR) "open", (LPCSTR)str, NULL, NULL, SW_SHOWNORMAL);

  if ((UINT)h > 32) {
    // visited
  } else {
    MessageBeep(0);// unable to execute file!
  }
  CButton::OnLButtonUp(nFlags, point);
}

void CHyperLinkButton::OnMouseMove(UINT nFlags, CPoint point)
{
  CRect rect;
  GetClientRect(rect);

  if (!rect.PtInRect(point)) {
    if (m_Over) {
      ReleaseCapture();
      m_Over = false;
      InvalidateRect(NULL);
    }
  } else {
    if (!m_Over) {
      SetCapture();
      m_Over = true;
      InvalidateRect(NULL);
    }
  }

  CButton::OnMouseMove(nFlags, point);
}

void CHyperLinkButton::SetOverColor(COLORREF colour)
{
  m_OverColor = colour;
}

void CHyperLinkButton::SetNormalColour(COLORREF colour)
{
  m_NotOverColor = colour;
}
