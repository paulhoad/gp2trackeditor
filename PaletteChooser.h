// PaletteChooser.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPaletteChooser dialog
#include "PaletteCtrl.h"

class CPaletteChooser : public CDialog
{
  // Construction
public:
  CPaletteChooser(CWnd* pParent = NULL);// standard constructor

  // Dialog Data
  //{{AFX_DATA(CPaletteChooser)
  enum {
    IDD = IDD_PALETTE
  };
  CPaletteCtrl m_PaletteCtrl;
  CString m_BVal;
  CString m_GVal;
  CString m_ColIndex;
  CString m_RVal;
  //}}AFX_DATA

  int getSelectedColor()
  {
    return m_PaletteCtrl.selectedColor;
  }

  void
    setSelectedColor(int col)
  {
    m_PaletteCtrl.selectedColor = col;
  }

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CPaletteChooser)
public:
  virtual BOOL
    OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);

protected:
  virtual void
    DoDataExchange(CDataExchange* pDX);// DDX/DDV support
  virtual LRESULT
    WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
  //}}AFX_VIRTUAL

  // Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CPaletteChooser)
  afx_msg void
    OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
