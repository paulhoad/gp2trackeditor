#ifndef _INCLUDED_TOOLBARUTILS_H_
#define _INCLUDED_TOOLBARUTILS_H_

bool SaveToolBarInfo(CWinApp* app,CString & registryEntry, DWORD style, CRect & toolRect, bool docked, UINT dockBarID = 0);
void LoadToolBarInfo(CWinApp* app,CString & registryEntry, DWORD * style, CRect & toolRect, bool * docked, UINT * dockBarID);
bool GetDockBarID(CToolBar * controlBar, UINT * id );
void SaveToolBar(CWinApp* app, CFrameWnd * frame, CToolBar * toolBar,CString registryEntry);
void LoadToolBar(CWinApp* app, CFrameWnd * frame, CToolBar * toolBar,CString registryEntry,DWORD style, UINT dockingBarID);			
void SaveDockBar(CWinApp* app, CFrameWnd * frame, CString & registryEntry, UINT bar);
void LoadDockBar(CWinApp* app, CFrameWnd * frame, CString & registryEntry, UINT bar);

#endif