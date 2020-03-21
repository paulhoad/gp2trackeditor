// TrackEditorView.h : interface of the CTrackEditorView class
//
/////////////////////////////////////////////////////////////////////////////

class CTrackEditorView : public CView
{
protected:// create from serialization only
  CTrackEditorView();
  DECLARE_DYNCREATE(CTrackEditorView)

  // Attributes
public:
  CTrackEditorDoc*
    GetDocument();
  Display* display;

  // Operations
public:
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTrackEditorView)
public:
  virtual void
    OnDraw(CDC* pDC);// overridden to draw this view
  virtual BOOL
    PreCreateWindow(CREATESTRUCT& cs);

protected:
  virtual BOOL
    OnPreparePrinting(CPrintInfo* pInfo);
  virtual void
    OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void
    OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
  //}}AFX_VIRTUAL

  // Implementation
public:
  virtual ~CTrackEditorView();
#ifdef _DEBUG
  virtual void
    AssertValid() const;
  virtual void
    Dump(CDumpContext& dc) const;
#endif

protected:
  // Generated message map functions
protected:
  //{{AFX_MSG(CTrackEditorView)
  afx_msg void
    OnIn();
  afx_msg void
    OnOut();
  afx_msg void
    OnTrack();
  afx_msg void
    OnPitlane();
  afx_msg void
    OnObjects();
  afx_msg void
    OnLine();
  afx_msg void
    OnTrackinfo();
  afx_msg void
    OnEditRemoveallccarg1();
  afx_msg void
    OnEditRemoveallarg2();
  afx_msg void
    OnUpdateTrack(CCmdUI* pCmdUI);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG// debug version in TrackEditorView.cpp
inline CTrackEditorDoc*
  CTrackEditorView::GetDocument()
{
  return (CTrackEditorDoc*)m_pDocument;
}
#endif

/////////////////////////////////////////////////////////////////////////////
