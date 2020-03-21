// ObjectInfo.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "ObjectInfo.h"
#include "TrackEditorDoc.h"
#include "TrackSection.h"
#include "GPTrack.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjectInfo

IMPLEMENT_DYNCREATE(CObjectInfo, CMyTableView)

CObjectInfo::CObjectInfo()
{
}

CObjectInfo::~CObjectInfo()
{
}


BEGIN_MESSAGE_MAP(CObjectInfo, CMyTableView)
	//{{AFX_MSG_MAP(CObjectInfo)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_EXPORT_TABLE, OnExportTable)
	ON_COMMAND(ID_NEW_OBJECT_DEFINITION, OnNewObjectDefinition)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectInfo drawing

void CObjectInfo::OnInitialUpdate()
{
	
}

/*
CString *toString(int data)
{
	char mess[256];

	wsprintf(mess,"%d",data);
	return new CString(mess);
}

CString *toString(double data)
{
	char mess[256];

	sprintf(mess,"%2.2f",(float)data);

	return new CString(mess);
}
*/

LPSTR toString(int data)
{
	char *buff = (char*)malloc(50*sizeof(char));
	wsprintf(buff,"%d",data);
	return buff;
}

LPSTR toHexString(int data)
{
	char *buff = (char*)malloc(50*sizeof(char));

	wsprintf(buff,"0x%4.4x",data);
	return buff;
}


LPSTR toString(double data)
{
	char *buff = (char*)malloc(50*sizeof(char));

	sprintf(buff,"%2.2f",(float)data);

	return buff;
}


void CObjectInfo::UpdateInfo(BOOL update)
{
    CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	if (pDoc)
	{
	  pDoc->objectTable = this;
	  GPTrack * mytrack = pDoc->getTrack();

	  if (mytrack!=NULL)
	  {
		  if (!update)
		  {
		  int colidx=0;
		  EmptyTable();
		  addColumn("Index",50,colidx++);
		  addColumn("Code",50,colidx++);
		  addColumn("Name?",100,colidx++);
	      addColumn("Distance (Y)",50,colidx++);
		  addColumn("AngleX",50,colidx++);
		  addColumn("AngleY",50,colidx++);
		  addColumn("Height",50,colidx++);
		  addColumn("Detail?",50,colidx++);
		  addColumn("Id1?",50,colidx++);
		  addColumn("Id2?",50,colidx++);
		  addColumn("Unk1",50,colidx++);
		  addColumn("Unk2",50,colidx++);
		  }
		  
		  for(int i=0;i<mytrack->TrackObjectDefintions->size();i++)
          {
           TrackObjectDefinition *t = (TrackObjectDefinition*)mytrack->TrackObjectDefintions->elementAt(i);
		   
		   int tidx = 0;

		   UpdateTableStringPtr(update,i,tidx++,toString(i),TO_ID(t->getDisplayIconType()));
		   UpdateTableStringPtr(update,i,tidx++,toString(t->getLocator()));
		   UpdateTableStringPtr(update,i,tidx++,CString(t->getName()));
		   UpdateTableStringPtr(update,i,tidx++,toString(t->getDistance()));
		   UpdateTableStringPtr(update,i,tidx++,toString(t->getAngleX()));
		   UpdateTableStringPtr(update,i,tidx++,toString(t->getAngleY()));
		   UpdateTableStringPtr(update,i,tidx++,toString(t->getHeight()));
		   UpdateTableStringPtr(update,i,tidx++,toString(t->getDetail()));
		   UpdateTableStringPtr(update,i,tidx++,toString(t->getId1()));
		   UpdateTableStringPtr(update,i,tidx++,toString(t->getId2()));
		   UpdateTableStringPtr(update,i,tidx++,toString(t->getUnk1()));
		   UpdateTableStringPtr(update,i,tidx++,toString(t->getUnk2()));
		   
	
	   }
	}
   }
   else
   {
	   AfxMessageBox("No Doc!");
   }
}


void CObjectInfo::OnDraw(CDC* pDC)
{
	
}

/////////////////////////////////////////////////////////////////////////////
// CObjectInfo diagnostics

#ifdef _DEBUG
void CObjectInfo::AssertValid() const
{
	CMyTableView::AssertValid();
}

void CObjectInfo::Dump(CDumpContext& dc) const
{
	CMyTableView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CObjectInfo message handlers

BOOL CObjectInfo::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CMyTableView::OnScroll(nScrollCode, nPos, bDoScroll);
}

void CObjectInfo::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	int idx = getTableSectionIndex();
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack * mytrack = pDoc->getTrack();

    trkdlg = new CObjectEdit(mytrack,this);
	
	TrackObjectDefinition *t = (TrackObjectDefinition*)mytrack->TrackObjectDefintions->elementAt(idx);

	if (trkdlg && t)
	  trkdlg->setSection(t);

	int res = trkdlg->DoModal();

	if (trkdlg && t && res!=IDCANCEL)
	{	  
	  trkdlg->getSection(t);
	  mytrack->regenerate=TRUE;
	  UpdateInfo(TRUE);
	}

	if (trkdlg) delete trkdlg;

    CMyTableView::OnLButtonDblClk(nFlags, point);
}

void CObjectInfo::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack * mytrack     = pDoc->getTrack();

	int idx = getTableSectionIndex();

	for(int i=0;i<mytrack->TrackObjectDefintions->size();i++)
	{
		TrackObjectDefinition *t = (TrackObjectDefinition*)mytrack->TrackObjectDefintions->elementAt(i);
		t->setSelected(FALSE);
	}
	

	if (idx >= 0 && idx < mytrack->TrackObjectDefintions->size())
	{
	 TrackObjectDefinition *t = (TrackObjectDefinition*)mytrack->TrackObjectDefintions->elementAt(idx);

	 t->setSelected(TRUE);

	 if (t)
	 {
	   pDoc->getTree()->Expand(pDoc->getTree()->GetParentItem(t->getTreeNode()),TVE_EXPAND);
	   pDoc->getTree()->SelectItem(t->getTreeNode());
	 }
	}

	CMyTableView::OnLButtonDown(nFlags, point);
}

void CObjectInfo::OnSetFocus(CWnd* pOldWnd) 
{
	CMyTableView::OnSetFocus(pOldWnd);

	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack * mytrack     = pDoc->getTrack();
	
    if (mytrack && mytrack->isValid())
	{
	 //UpdateInfo();
	}
}






void CObjectInfo::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    CMyTableView::OnLButtonUp(nFlags, point);
}

void CObjectInfo::OnExportTable() 
{
	// TODO: Add your command handler code here
	ExportTableData();
}

BOOL CObjectInfo::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL res=CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	return res;
}

void CObjectInfo::OnNewObjectDefinition() 
{
	// TODO: Add your command handler code here
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack * mytrack     = pDoc->getTrack();


	TrackObjectDefinition *t = mytrack->AddNewTrackObjectDefinition();

	trkdlg = new CObjectEdit(mytrack,this);
	
	if (trkdlg && t)
	  trkdlg->setSection(t);

	int res = trkdlg->DoModal();

	if (trkdlg && t && res!=IDCANCEL)
	{	  
	  trkdlg->getSection(t);
	  mytrack->regenerate=TRUE;
	}

	if (trkdlg) delete trkdlg;

}

int CObjectInfo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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
