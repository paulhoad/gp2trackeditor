#if !defined( \
  AFX_HYPERLINKBUTTON_H__0D364516_7D79_11D3_A43E_00105AD85245__INCLUDED_)
#define AFX_HYPERLINKBUTTON_H__0D364516_7D79_11D3_A43E_00105AD85245__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif// _MSC_VER > 1000
// HyperLinkButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHyperLinkButton window

class CHyperLinkButton : public CButton
{
  // Construction
public:
  CHyperLinkButton();

  // Attributes
public:
  // Operations
public:
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CHyperLinkButton)
public:
  virtual void
    DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
  //}}AFX_VIRTUAL

  // Implementation
public:
  void SetNormalColour(COLORREF);
  void SetOverColor(COLORREF);

  bool m_Over;
  virtual ~CHyperLinkButton();
  COLORREF m_OverColor;
  COLORREF m_NotOverColor;

  // Generated message map functions
protected:
  //{{AFX_MSG(CHyperLinkButton)
  afx_msg void
    OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void
    OnMouseMove(UINT nFlags, CPoint point);
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before
// the previous line.

#endif// !defined(AFX_HYPERLINKBUTTON_H__0D364516_7D79_11D3_A43E_00105AD85245__INCLUDED_)
