// InternalView.h : header file
//
#ifndef _INTERNAL_VIEW_INCLUDED_
#define _INTERNAL_VIEW_INCLUDED_


#include "InternalObject.h"
#include "CDib.h"
#include "OpenGLView.h"




/////////////////////////////////////////////////////////////////////////////
// CInternalView view

class CInternalView : public COpenGLView
{
protected:
	CInternalView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CInternalView)

// Attributes
public:

// Operations
public:
	InternalObject *myobject;

	
	BOOL showText;
	//BOOL showPointNumbers;
	//BOOL showVertexNumbers;
	//BOOL showPolygonNumbers;
	BOOL showFilled;
	BOOL showTextures;
	//BOOL showCulling;
	//BOOL showNormals;

	CPoint dragPoint;

	void update();
	void updateBox();
	void repaint();

	//int CreateGLWindow();
	//BOOL SetWindowPixelFormat(HDC hDC);
	//BOOL CreateViewGLContext(HDC hDC);

	//float m_red[256],m_blue[256],m_green[256];
	//HPALETTE m_hPalette;

	
	void TextureMap(CBitmap &pDib);
	//void CreateColorTables(CTextureMap *pDib);
    //void SetupColorTables();
    //void SetupLogicalPalette();

	CBitmap testbmp;

	//HGLRC m_hGLContext;

	//int m_GLPixelIndex;
	void OpenGLDrawScene(HDC hdc);
	double m_transY;
	double m_transX;
	double m_angle1;
	double m_angle2;
	float m_size;
	double m_xRotate;
	double m_yRotate;
	double m_zRotate;

	double m_Dist;

	int m_width;
	int m_height;
	int m_exp;

	CPoint m_RightDownPos;
	CPoint m_LeftDownPos;
	BOOL m_RightButtonDown;
	BOOL m_LeftButtonDown;
	BOOL m_ShiftDown;
	//BOOL showLines;
	//BOOL showPoints;
	//BOOL showAxis;

	void LoadJamTexture(GPTrack *mytrack,CBitmap &output,int idx,int hres,int vres);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInternalView)
	protected:
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CInternalView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CInternalView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnShowAnnotations();
	afx_msg void OnUpdateShowAnnotations(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnShowpointNumbers();
	afx_msg void OnShowVertexnumbers();
	afx_msg void OnShowPolygonNumbers();
	afx_msg void OnUpdateShowPolygonNumbers(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowVertexnumbers(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowpointNumbers(CCmdUI* pCmdUI);
	afx_msg void OnTexturemapobjects();
	afx_msg void OnFillobjects();
	afx_msg void OnUpdateTexturemapobjects(CCmdUI* pCmdUI);
	afx_msg void OnDrawaxis();
	afx_msg void OnUpdateDrawaxis(CCmdUI* pCmdUI);
	afx_msg void OnShowpoints();
	afx_msg void OnUpdateShowpoints(CCmdUI* pCmdUI);
	afx_msg void OnShowlines();
	afx_msg void OnUpdateShowlines(CCmdUI* pCmdUI);
	afx_msg void OnShownormals();
	afx_msg void OnUpdateShownormals(CCmdUI* pCmdUI);
	afx_msg void OnShowculling();
	afx_msg void OnUpdateShowculling(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif
