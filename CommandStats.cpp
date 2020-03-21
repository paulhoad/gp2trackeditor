// CommandStats.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "CommandStats.h"
#include "TrackEditorDoc.h"
#include "TrackSection.h"
#include "GPTrack.h"
#include "TrackCmd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern LPSTR toString(int data);
extern LPSTR toString(double data);
extern LPSTR toHexString(int data);
extern CString getCmdDescription(int id);

extern LPCSTR FindInternalObjectName(GPTrack *track,int id1,int id2);


/////////////////////////////////////////////////////////////////////////////
// CCommandStats

IMPLEMENT_DYNCREATE(CCommandStats, CMyTableView)

CCommandStats::CCommandStats()
{	
}


CCommandStats::~CCommandStats()
{
}


BEGIN_MESSAGE_MAP(CCommandStats, CMyTableView)
	//{{AFX_MSG_MAP(CCommandStats)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_EXPORT_TABLE, OnExportTable)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

extern int getDisplayIconType(GPTrack *track,int locator,int id1,int id2);

/////////////////////////////////////////////////////////////////////////////
// CCommandStats drawing

void CCommandStats::OnInitialUpdate()
{
}

void CCommandStats::OnDraw(CDC* pDC)
{
}

void CCommandStats::UpdateInfo(BOOL update)
{
    CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack * mytrack = pDoc->getTrack();
	int rowidx=0;


	if (mytrack!=NULL)
	{
		  int foundCommands[1024];
		  int numFound = 0;
		  
		  if (!update)
		  {
          int colidx=0;
		  EmptyTable();

		  addColumn("Cmd Code(hex)",150,colidx++);
		  addColumn("Cmd Code(Dec)",50,colidx++);
		  addColumn("Number of Commands",100,colidx++);
		  addColumn("Sections With commands in",100,colidx++);
	      addColumn("Command Description",100,colidx++);
		  }
		  
		  int tidx = 0;
		

		  UpdateTableStringPtr(update,rowidx,tidx++,"Track Commands",TO_ID(IDB_EMPTY));
		  UpdateTableStringPtr(update,rowidx,tidx++,"");
		  UpdateTableStringPtr(update,rowidx,tidx++,"");
		  UpdateTableStringPtr(update,rowidx,tidx++,"");
		  UpdateTableStringPtr(update,rowidx,tidx++,"");
		  rowidx++;

		  for(int j=0;j<255;j++)
		  {
		   int found_cmds = 0;
		   numFound=0;
		   for(int i=0;i<mytrack->TrackSections->size();i++)
           {
           TrackSection *t = (TrackSection*)mytrack->TrackSections->elementAt(i);
		   Vector *cmds = t->getCommands();
		   

		    for(int k=0;k<cmds->size();k++)
		    {
				TrackCmd *cmd = (TrackCmd*)cmds->elementAt(k);

				if (cmd->getCmd() == j)
				{
				  found_cmds ++;
				  foundCommands[numFound++] = i;
				}
		    }
           }
		   // end of scan for this command
		   if (found_cmds > 0)
		   {
			 int tidx = 0;
		     
		     UpdateTableStringPtr(update,rowidx,tidx++,toHexString(j),TO_ID(getCmdIcon(j)));
			 UpdateTableStringPtr(update,rowidx,tidx++,toString(j));
			 UpdateTableStringPtr(update,rowidx,tidx++,toString(found_cmds));

			 char buffer[1024];
			 strcpy(buffer,"");
			 for(int l=0;l<numFound;l++)
			 {
			   char smbuff[5];
			   wsprintf(smbuff,"%d,",foundCommands[l]);
			   strcat(buffer,smbuff);
			 }
			 UpdateTableStringPtr(update,rowidx,tidx++,CString(buffer));
			 UpdateTableStringPtr(update,rowidx,tidx++,CString(getCmdDescription(j)));
			 rowidx++;
		   }
	      }

		  int foundPitCommands[1024];
		  int numPitFound = 0;
		  
		  tidx = 0;
          UpdateTableStringPtr(update,rowidx,tidx++,CString(""),TO_ID(IDB_EMPTY));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  rowidx++;

		  tidx = 0;
		  UpdateTableStringPtr(update,rowidx,tidx++,CString("PitLane Commands"),TO_ID(IDB_EMPTY));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  rowidx++;
		  
		  for(j=0;j<255;j++)
		  {
		   int found_cmds = 0;
		   numPitFound=0;
		   for(int i=0;i<mytrack->PitlaneSections->size();i++)
           {
           TrackSection *t = (TrackSection*)mytrack->PitlaneSections->elementAt(i);
		   Vector *cmds = t->getCommands();
		   

		    for(int k=0;k<cmds->size();k++)
		    {
				TrackCmd *cmd = (TrackCmd*)cmds->elementAt(k);

				if (cmd->getCmd() == j)
				{
				  found_cmds ++;
				  foundPitCommands[numPitFound++] = i;
				}
		    }
           }
		   // end of scan for this command
		   if (found_cmds > 0)
		   {
			 tidx = 0;

			 UpdateTableStringPtr(update,rowidx,tidx++,toHexString(j),TO_ID(getCmdIcon(j)));
			 UpdateTableStringPtr(update,rowidx,tidx++,toString(j));
			 UpdateTableStringPtr(update,rowidx,tidx++,toString(found_cmds));


			 char buffer[1024];
			 strcpy(buffer,"");
			 for(int l=0;l<numPitFound;l++)
			 {
			   char smbuff[5];
			   wsprintf(smbuff,"%d,",foundPitCommands[l]);
			   strcat(buffer,smbuff);
			 }
			 UpdateTableStringPtr(update,rowidx,tidx++,CString(buffer));
			 UpdateTableStringPtr(update,rowidx,tidx++,CString(getCmdDescription(j)));
			 rowidx++;
		   }
		  
	   }

	   tidx = 0;
          UpdateTableStringPtr(update,rowidx,tidx++,CString(""),TO_ID(IDB_EMPTY));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  rowidx++;

		  tidx = 0;
		  UpdateTableStringPtr(update,rowidx,tidx++,CString("InternalObjects"),TO_ID(IDB_EMPTY));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  rowidx++;

		  for(int i=0;i<mytrack->NumberOfObjects+18;i++)
		  {
			  tidx=0;
			  int found_objs=0;
			  
			  
			  char buffer[256];

			  wsprintf(buffer,"");
			  for(int k=0;k<mytrack->TrackObjectDefintions->size();k++)
			  {
				  TrackObjectDefinition *def = (TrackObjectDefinition*)mytrack->TrackObjectDefintions->elementAt(k);
				  if (def->id == i)
				  {
					char msg[5];
					wsprintf(msg,"%d,",k);
				    strcat(buffer,msg);
					found_objs++;
				  }
		      }
			  UpdateTableStringPtr(update,rowidx,tidx++,toHexString(i),TO_ID(getDisplayIconType(mytrack,-1,i,0)));
		      UpdateTableStringPtr(update,rowidx,tidx++,toString(i));		      
			  UpdateTableStringPtr(update,rowidx,tidx++,toString(found_objs));
			  UpdateTableStringPtr(update,rowidx,tidx++,CString(buffer));
			  UpdateTableStringPtr(update,rowidx,tidx++,CString("Internal Object"));
			  rowidx++;
		  }


		  tidx = 0;
		  UpdateTableStringPtr(update,rowidx,tidx++,CString("Object Definition References"),TO_ID(IDB_EMPTY));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  rowidx++;

		  for( i=0;i<mytrack->TrackObjectDefintions->size();i++)
		  {
			  TrackObjectDefinition *def = (TrackObjectDefinition*)mytrack->TrackObjectDefintions->elementAt(i);				  
			  char buffer[256];

			  wsprintf(buffer,"");
			  int found_objs=0;
			  for(int k=0;k<mytrack->TrackSections->size();k++)
			  {
				  TrackSection *t = (TrackSection*)mytrack->TrackSections->elementAt(k);
				  Vector *cmds  = t->getCommands();

				 
			      
			  
				  for(int l=0;l<cmds->size();l++)
				  {
					  TrackCmd *cmd = (TrackCmd*)cmds->elementAt(l);
					  if (cmd->getCmd() == 0x80)
					  {
						  //found a object commands
						  int locator = cmd->getNthArg(2);
						  if (locator == def->locator)
						  {
						   found_objs++;
						   char msg[5];
					       wsprintf(msg,"%d,",k);
				           strcat(buffer,msg);
						  }
					  }
				  }
			  }
			  tidx=0;   				
			  UpdateTableStringPtr(update,rowidx,tidx++,toHexString(i),TO_ID(getDisplayIconType(mytrack,def->locator,def->id,def->id2)));
		      UpdateTableStringPtr(update,rowidx,tidx++,toString(i));		      
			  UpdateTableStringPtr(update,rowidx,tidx++,toString(found_objs));
			  UpdateTableStringPtr(update,rowidx,tidx++,CString(buffer));
			  UpdateTableStringPtr(update,rowidx,tidx++,CString("Object Definition Track References"));
			  rowidx++;
		  }


		  tidx = 0;
		  UpdateTableStringPtr(update,rowidx,tidx++,CString("Internal Objects"),TO_ID(IDB_EMPTY));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString("Magic Numbers"));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  UpdateTableStringPtr(update,rowidx,tidx++,CString(""));
		  rowidx++;

		  for( i=0;i<mytrack->InternalObjectOffsets->size();i++)
		  {
			  InternalObject *obj = (InternalObject*)mytrack->InternalObjectOffsets->elementAt(i);

			  tidx=0;   				
			  UpdateTableStringPtr(update,rowidx,tidx++,toString(i),TO_ID(getDisplayIconType(mytrack,-1,i,0)));
		      UpdateTableStringPtr(update,rowidx,tidx++,toString(i));		      
			  UpdateTableStringPtr(update,rowidx,tidx++,toString(obj->CalculateDigitalSignature()));
			  UpdateTableStringPtr(update,rowidx,tidx++,CString(FindInternalObjectName(mytrack,1,0)));
			  UpdateTableStringPtr(update,rowidx,tidx++,CString("Internal Object"));
			  rowidx++;
		  }



	}
	//GetListCtrl().SetColumnWidth(1,LVSCW_AUTOSIZE_USEHEADER);
    //GetListCtrl().SetColumnWidth(2,LVSCW_AUTOSIZE_USEHEADER);
	//GetListCtrl().SetColumnWidth(3,LVSCW_AUTOSIZE_USEHEADER);
	//GetListCtrl().SetColumnWidth(4,LVSCW_AUTOSIZE_USEHEADER);
}


/////////////////////////////////////////////////////////////////////////////
// CCommandStats diagnostics

#ifdef _DEBUG
void CCommandStats::AssertValid() const
{
	CMyTableView::AssertValid();
}

void CCommandStats::Dump(CDumpContext& dc) const
{
	CMyTableView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCommandStats message handlers

BOOL CCommandStats::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CMyTableView::OnScroll(nScrollCode, nPos, bDoScroll);
}

void CCommandStats::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMyTableView::OnLButtonDown(nFlags, point);
}

void CCommandStats::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMyTableView::OnLButtonUp(nFlags, point);
}

void CCommandStats::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CMyTableView::OnMouseMove(nFlags, point);
}

void CCommandStats::OnExportTable() 
{
	// TODO: Add your command handler code here
	ExportTableData();
}

BOOL CCommandStats::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL res=CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	//SetImageList((((CTrackEditorApp*)AfxGetApp())->getSharedImageList()),LVSIL_SMALL);

	return res;
}

int CCommandStats::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMyTableView::OnCreate(lpCreateStruct) == -1)
		return -1;


	CImageList *pImages = (((CTrackEditorApp*)AfxGetApp())->getSharedImageList());

	if (pImages!=NULL)
	{
	  SetImageList(pImages,LVSIL_SMALL);
	  useImages = TRUE;
	}
	else
	{
	  useImages = FALSE;
	  AfxMessageBox("Failed to open Images");
	}
	
	// TODO: Add your specialized creation code here
	UpdateInfo(FALSE);
	
	return 0;
}
