// PaletteCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPaletteCtrl window

class CPaletteCtrl : public CButton
{
// Construction
public:
	CPaletteCtrl();

// Attributes
public:
   int selectedColor;

   int getSelectedColor()
   {
	   return selectedColor;
   }

   void setSelectedColor(int col)
   {
	   selectedColor = col; 
   }

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaletteCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPaletteCtrl();

	// Generated message map functions
public:
	//{{AFX_MSG(CPaletteCtrl)
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
