// TrackTreeCtrl.h : header file
//
#ifndef _INCLUDED_TRACK_TREE_CTRL_H
#define _INCLUDED_TRACK_TREE_CTRL_H

/////////////////////////////////////////////////////////////////////////////
// CTrackTreeCtrl window

class CTrackTreeCtrl : public CTreeCtrl
{
  // Construction
public:
  CTrackTreeCtrl();

  // Attributes
public:
  // Operations
public:
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTrackTreeCtrl)
  //}}AFX_VIRTUAL

  // Implementation
public:
  virtual ~CTrackTreeCtrl();

  // Generated message map functions
protected:
  //{{AFX_MSG(CTrackTreeCtrl)
  afx_msg void
    OnRemoveTrackCmdTree();
  afx_msg void
    OnInsertTrackcmdTree();
  afx_msg void
    OnUpdateRemoveTrackCmdTree(CCmdUI* pCmdUI);
  afx_msg void
    OnUpdateInsertTrackcmdTree(CCmdUI* pCmdUI);
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif