#include "StdAfx.h"
#include "ToolBarUtils.h"

#include "afxpriv.h"

bool SaveToolBarInfo(CWinApp* app, CString& registryEntry, DWORD style, CRect& toolRect, bool docked, UINT dockBarID)
{
  bool ok = true;
  app->WriteProfileInt(registryEntry, "style", style);
  if (!docked) {
    ok = ok && app->WriteProfileInt(registryEntry, "top", toolRect.top);
    ok = ok && app->WriteProfileInt(registryEntry, "left", toolRect.left);
    ok = ok && app->WriteProfileInt(registryEntry, "Docked", 0);
  } else {
    ok = ok && app->WriteProfileInt(registryEntry, "top", toolRect.top);
    ok = ok && app->WriteProfileInt(registryEntry, "left", toolRect.left);
    ok = ok && app->WriteProfileInt(registryEntry, "bottom", toolRect.bottom);
    ok = ok && app->WriteProfileInt(registryEntry, "right", toolRect.right);
    ok = ok && app->WriteProfileInt(registryEntry, "Docked", 1);
    ok = ok && app->WriteProfileInt(registryEntry, "dockBarID", dockBarID);
  }
  return ok;
}

void LoadToolBarInfo(CWinApp* app, CString& registryEntry, DWORD* style, CRect& toolRect, bool* docked, UINT* dockBarID)
{
  *style = app->GetProfileInt(registryEntry, "style", *style);
  *docked = (app->GetProfileInt(registryEntry, "Docked", 1)) ? true : false;
  if (!(*docked)) {
    (*toolRect).top = app->GetProfileInt(registryEntry, "top", (*toolRect).top);
    (*toolRect).left =
      app->GetProfileInt(registryEntry, "left", (*toolRect).left);
    (*toolRect).bottom = (*toolRect).top;
    (*toolRect).right = (*toolRect).left;
    *dockBarID = 0;
  } else {
    (*toolRect).top = app->GetProfileInt(registryEntry, "top", (*toolRect).top);
    (*toolRect).left =
      app->GetProfileInt(registryEntry, "left", (*toolRect).left);
    (*toolRect).bottom =
      app->GetProfileInt(registryEntry, "bottom", (*toolRect).bottom);
    (*toolRect).right =
      app->GetProfileInt(registryEntry, "right", (*toolRect).right);
    *dockBarID = app->GetProfileInt(registryEntry, "dockBarID", *dockBarID);
  }
}

bool GetDockBarID(CToolBar* controlBar, UINT* id)
{
  if (controlBar->m_pDockBar) {
    UINT dockBarID = 0;
    if ((controlBar->m_dwStyle & CBRS_ALIGN_TOP) == CBRS_ALIGN_TOP)
      dockBarID |= AFX_IDW_DOCKBAR_TOP;
    if ((controlBar->m_dwStyle & CBRS_ALIGN_BOTTOM) == CBRS_ALIGN_BOTTOM)
      dockBarID |= AFX_IDW_DOCKBAR_BOTTOM;
    if ((controlBar->m_dwStyle & CBRS_ALIGN_RIGHT) == CBRS_ALIGN_RIGHT)
      dockBarID |= AFX_IDW_DOCKBAR_RIGHT;
    if ((controlBar->m_dwStyle & CBRS_ALIGN_LEFT) == CBRS_ALIGN_LEFT)
      dockBarID |= AFX_IDW_DOCKBAR_LEFT;

    *id = dockBarID;
    return true;
  }
  return false;
}

void SaveToolBar(CWinApp* app, CFrameWnd* frame, CToolBar* toolBar, CString registryEntry)
{
  CRect rect;
  int captionSize = GetSystemMetrics(SM_CYSIZE);
  int x_border = GetSystemMetrics(SM_CXEDGE);
  DWORD style = toolBar->GetBarStyle();
  UINT dockBarID = 0;

  // get windows
  toolBar->GetWindowRect(&rect);

  if (toolBar->IsFloating()) {
    rect.top -= captionSize;
    rect.left -= x_border;
  } else {
    CRect frameRect;
    frame->GetClientRect(&frameRect);
    frame->ClientToScreen(&frameRect);
    // get dock bar info
    CDockBar* pDock = toolBar->m_pDockBar;
    CRect dBarRect;
    // if (pDock){
    pDock->GetWindowRect(&dBarRect);
    //}
    // else {
    //  toolBar->GetWindowRect(&dBarRect);
    //}

    int x_dist = (dBarRect.left - frameRect.left);
    int y_dist = (dBarRect.top - frameRect.top);

    rect.left = rect.left - x_dist;
    rect.top = rect.top - y_dist;
    rect.right = rect.right - x_dist;
    rect.bottom = rect.bottom - y_dist;
  }
  GetDockBarID((CToolBar*)toolBar, &dockBarID);
  SaveToolBarInfo(app, registryEntry, style, rect, (toolBar->IsFloating()) ? false : true, dockBarID);
}

void LoadToolBar(CWinApp* app, CFrameWnd* frame, CToolBar* toolBar, CString registryEntry, DWORD style, UINT dockingBarID)
{
  bool docked = true;
  CRect rect(0, 0, 0, 0);
  toolBar->GetWindowRect(&rect);
  LoadToolBarInfo(app, registryEntry, &style, rect, &docked, &dockingBarID);
  if (docked)
    frame->DockControlBar(toolBar, dockingBarID, rect);
  else
    frame->FloatControlBar(toolBar, CPoint(rect.left, rect.top), style);
}
void SaveDockBar(CWinApp* app, CFrameWnd* frame, CString& registryEntry, UINT bar)
{
  CControlBar* pDock = frame->GetControlBar(bar);
  if (pDock) {
    CRect rect;
    pDock->GetWindowRect(&rect);
    app->WriteProfileInt(registryEntry, "top", rect.top);
    app->WriteProfileInt(registryEntry, "bottom", rect.bottom);
    app->WriteProfileInt(registryEntry, "left", rect.left);
    app->WriteProfileInt(registryEntry, "right", rect.right);
  }
}

void LoadDockBar(CWinApp* app, CFrameWnd* frame, CString& registryEntry, UINT bar)
{
  CControlBar* pDock = frame->GetControlBar(bar);
  if (pDock) {
    CRect rect;
    // pDock->GetWindowRect( &rect );
    rect.top = app->GetProfileInt(registryEntry, "top", -1);
    rect.bottom = app->GetProfileInt(registryEntry, "bottom", -1);
    rect.left = app->GetProfileInt(registryEntry, "left", -1);
    rect.right = app->GetProfileInt(registryEntry, "right", -1);
    if ((rect.top != rect.bottom) && (rect.top != -1))
      pDock->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS);
  }
}