// TrackTree.h : header file
//

#ifndef _TRACK_TREE_H_
#define _TRACK_TREE_H_

#include "GPTrack.h"
#include "sizecbar.h"
#include "TrackTreeCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTrackTree dialog

class CTrackTree : public CSizingControlBar
{
  // Construction
public:
  CTrackTree();// standard constructor
  ~CTrackTree();

  // Dialog Data
  //{{AFX_DATA(CTrackTree)
  // enum { IDD = IDD_VIEW };
  CTreeCtrl m_Tree;
  //}}AFX_DATA

  // virtual CSize CalcDynamicLayout(int nLength, DWORD nMode);

  void
    Reposition(CRect rc);

  CImageList* pImageList;

  BOOL ImagesNotLoaded;

  void
    loadTree();
  void
    LoadImages();

  void
    EmptyContents();

  void
    UpdateTree(GPTrack* track, Observer* objToReExpandTo);
  void
    OnMoveCommandUp();
  void
    OnMoveCommandDown();
  void
    OnExportObject();

  void
    OnRemoveInternalObject();
  void
    OnRemoveObjectDef();

  Vector* extraDataList;

  HTREEITEM
  insertInfoNode(HTREEITEM parent, CString& title, int bitmap);

  inline CTreeCtrl*
    getTree()
  {
    return &m_TrackTree;
  }

  // HTREEITEM insertTrack(LPCSTR TrackName);
  HTREEITEM
  insertTrack(GPTrack* track);

  void
    deleteNode(HTREEITEM node);

  void
    OnRemoveCamera();
  void
    OnAddCamera();
  void
    OnRemoveJamFile();
  void
    OnAddJamFile();
  void
    OnRenameJamFile();
  void
    OnCopy();
  void
    OnPaste();
  void
    OnCut();

  Observer* clipboard;

  void
    OnUpdateRemoveCamera(CCmdUI* pCmdUI);
  void
    OnUpdateAddCamera(CCmdUI* pCmdUI);

  HTREEITEM
  GetParent(HTREEITEM node);

  // BOOL Create(CWnd* pParent , char * Id, int flags, int CId);

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTrackTree)
protected:
  virtual void
    DoDataExchange(CDataExchange* pDX);// DDX/DDV support
    //}}AFX_VIRTUAL

  // Implementation
protected:
  CTrackTreeCtrl m_TrackTree;

  // Generated message map functions
  //{{AFX_MSG(CTrackTree)
  afx_msg int
    OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void
    OnPaint();
  afx_msg void
    OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void
    OnDblclkTracktree(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void
    OnSize(UINT nType, int cx, int cy);
  afx_msg void
    OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void
    OnRButtonUp(UINT nFlags, CPoint point);
  afx_msg void
    OnRclickTracktree(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void
    OnRemoveTrackCmdTree();
  afx_msg void
    OnInsertTrackcmdTree();
  afx_msg void
    OnUpdateRemoveTrackCmdTree(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateInsertTrackcmdTree(CCmdUI* pCmdUI);
  afx_msg void
    OnSelchangedTracktree(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void
    OnShowJam();
  afx_msg void
    OnClickTracktree(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void
    OnItemexpandingTracktree(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void
    OnExportObjectInfo();
  afx_msg void
    OnClose();
  afx_msg void
    OnDestroy();
  afx_msg void
    OnRightpopMovetonextsection();
  afx_msg void
    OnRightpopMovetoprevioussection();
  afx_msg void
    OnRightpopMoveto();
  afx_msg void
    OnUpdateRightpopMoveto(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateRightpopMovetonextsection(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateRightpopMovetoprevioussection(CCmdUI* pCmdUI);
  afx_msg void
    OnHelpObject();
  afx_msg void
    OnReplaceinternalobject();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

#endif
