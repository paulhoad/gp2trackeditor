// TrackTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "TrackTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrackTreeCtrl

CTrackTreeCtrl::CTrackTreeCtrl() {}

CTrackTreeCtrl::~CTrackTreeCtrl() {}

BEGIN_MESSAGE_MAP(CTrackTreeCtrl, CTreeCtrl)
//{{AFX_MSG_MAP(CTrackTreeCtrl)
ON_COMMAND(ID_REMOVE_TRACK_CMD_TREE, OnRemoveTrackCmdTree)
ON_COMMAND(ID_INSERT_TRACKCMD_TREE, OnInsertTrackcmdTree)
ON_UPDATE_COMMAND_UI(ID_REMOVE_TRACK_CMD_TREE, OnUpdateRemoveTrackCmdTree)
ON_UPDATE_COMMAND_UI(ID_INSERT_TRACKCMD_TREE, OnUpdateInsertTrackcmdTree)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackTreeCtrl message handlers

void CTrackTreeCtrl::OnRemoveTrackCmdTree()
{
  // TODO: Add your command handler code here
  AfxMessageBox("Remove");
}

void CTrackTreeCtrl::OnInsertTrackcmdTree()
{
  // TODO: Add your command handler code here
  AfxMessageBox("Insert");
}

void CTrackTreeCtrl::OnUpdateRemoveTrackCmdTree(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  // AfxMessageBox("Update Remove");
}

void CTrackTreeCtrl::OnUpdateInsertTrackcmdTree(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  // AfxMessageBox("Update Insert");
}
