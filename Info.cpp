// Info.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEditor.h"
#include "Info.h"
#include "TrackEditorDoc.h"
#include "TrackSection.h"
#include "GPTrack.h"
#include "CCLineSection.h"
#include "ImportDataElement.h"
//#include "CLineEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfo

IMPLEMENT_DYNCREATE(CInfo, CMyTableView)

CInfo::CInfo()
{

	cclinedlg = new CCLineEditor(this);

}

CInfo::~CInfo()
{
	if (cclinedlg!=NULL) delete cclinedlg;

	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();

	if (pDoc)
	{
	  pDoc->ccTable = NULL;
	}
}


BEGIN_MESSAGE_MAP(CInfo, CMyTableView)
	//{{AFX_MSG_MAP(CInfo)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_EXPORT_TABLE, OnExportTable)
	ON_WM_CREATE()
	ON_COMMAND(ID_IMPORT_TABLE, OnImportTable)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfo drawing

void CInfo::OnInitialUpdate()
{
	
}


void CInfo::UpdateInfo(BOOL update)
{
    CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	pDoc->ccTable = this;
	GPTrack * mytrack = pDoc->getTrack();

	if (mytrack!=NULL)
	{
		  if (!update)
		  {
		  int colidx=0;
		  EmptyTable();
		  addColumn("Index",50,colidx++);
		  addColumn("Cmd",50,colidx++);
	      addColumn("Length",50,colidx++);		  
	      addColumn("Radius",50,colidx++);
		  addColumn("Sign/Scale ?",50,colidx++);
	      addColumn("Tight/Wider Offset",50,colidx++);
		  addColumn("Angle",50,colidx++);
		  addColumn("Arc",50,colidx++);
		  addColumn("Arc (Rad)",50,colidx++);
		  addColumn("0x70 (only) Sign/Scale for 0x70",50,colidx++);
		  addColumn("0x70 (only) Radius for 0x70",50,colidx++);
		  addColumn("StAng ",50,colidx++);
		  addColumn("StEnd ",50,colidx++);
		  }
	    
		  if (mytrack->TrackSections)
		  {
		   TrackSection *t1 = (TrackSection*)mytrack->TrackSections->elementAt(0);

	       double xpos = t1->getStartX();
	       double ypos = t1->getStartY();
	       double currentangle  = ((360*mytrack->TrackStartAngle.GetValue())/65535);
	       double currrad = 0.0;

		   xpos += mytrack->CCLineStartX.GetValue();
		   ypos += WIDTH_TO_CCWIDTH(mytrack->CCLineStartY.GetValue());

		   for(int i=0;i<mytrack->CCLineSections->size();i++)
           {
             CCLineSection *t = (CCLineSection*)mytrack->CCLineSections->elementAt(i);
		     double nextxpos,nextypos;

		     double  direction = (double)t->getArg2();
			 double  ndirection = (double)t->getArg3();

		     if (direction > 0x8000) direction = -(0x10000 - direction);
			 if (ndirection > 0x8000) ndirection = -(0x10000 - ndirection);

		     double rad = t->getRadius();
			 double length  = t->getLength();
			 double sectorStartAngle = currrad;
             double sectorEndAngle   = currrad;
			 double anglelen;
             			 
			 double rad2=0;
			 rad = RadiusTo(mytrack,rad,direction,(double)t->getArg1(),t->getCmd(),t->getArg3(),t->getArg4(),rad2);


			 if (rad !=0)
             {
		     
		     double anglerad = (length/rad);
		     anglelen = RadToDeg(anglerad);		   
	         double sectorRadius     = 0;

             
              double inrad = anglerad/2.0;
			  double len  = 2.0*(rad*sin(inrad));

              nextypos =  len*sin((currrad+inrad));
              nextxpos =  len*cos((currrad+inrad));
            
              sectorEndAngle = currrad+inrad;
              currrad += anglerad;
             }
             else
             {
              nextypos = length*sin(currrad);
              nextxpos = length*cos(currrad);
              anglelen = 0;
             }

             //t->setStartX(xpos);
             //t->setStartY(ypos);
             //t->setEndX(nextxpos);
             //t->setEndY(nextypos);
             //t->setStartAngle(sectorStartAngle);
             //t->setEndAngle(sectorEndAngle);

		     xpos =(xpos+nextxpos);
	         ypos =(ypos+nextypos);

			int tidx = 0;

		    UpdateTableStringPtr(update,i,tidx++,toString(i),TO_ID(t->getIcon()));
			UpdateTableStringPtr(update,i,tidx++,toString(t->getCmd()));
		    UpdateTableStringPtr(update,i,tidx++,toString((int)length));		    
		    UpdateTableStringPtr(update,i,tidx++,toString((int)t->getRadius()));
		    UpdateTableStringPtr(update,i,tidx++,toString((int)direction));
		    UpdateTableStringPtr(update,i,tidx++,toString(t->getArg1()));
		    UpdateTableStringPtr(update,i,tidx++,toString(anglelen));
		    UpdateTableStringPtr(update,i,tidx++,toString(RadToDeg(currrad)));
			UpdateTableStringPtr(update,i,tidx++,toString(currrad));

		    if (t->getCmd() != 0x50)
		    {
		     UpdateTableStringPtr(update,i,tidx++,toString((int)ndirection));
		     UpdateTableStringPtr(update,i,tidx++,toString(t->getArg4()));
		    }
		    else
		    {
		     UpdateTableStringPtr(update,i,tidx++,CString("-"));
		     UpdateTableStringPtr(update,i,tidx++,CString("-"));
		    }

			UpdateTableStringPtr(update,i,tidx++,toString(RadToDeg(sectorStartAngle)));
			UpdateTableStringPtr(update,i,tidx++,toString(RadToDeg(sectorEndAngle)));
		   
           }
		  }
	}
}


void CInfo::OnDraw(CDC* pDC)
{
	
}

CString CInfo::toString(int data)
{
	static char mess[256];

	wsprintf(mess,"%d",data);
	return CString(mess);
}

CString CInfo::toString(double data)
{
	static char mess[256];

	sprintf(mess,"%2.2f",(float)data);

	return CString(mess);
}

/////////////////////////////////////////////////////////////////////////////
// CInfo diagnostics

#ifdef _DEBUG
void CInfo::AssertValid() const
{
	CMyTableView::AssertValid();
}

void CInfo::Dump(CDumpContext& dc) const
{
	CMyTableView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInfo message handlers

BOOL CInfo::OnScrollBy(CSize sizeScroll, BOOL bDoScroll) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CMyTableView::OnScrollBy(sizeScroll, bDoScroll);
}

BOOL CInfo::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CMyTableView::OnScroll(nScrollCode, nPos, bDoScroll);
}

void CInfo::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//CPoint offset = GetDeviceScrollPosition();
	// TODO: Add your message handler code here and/or call default
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack * mytrack     = pDoc->getTrack();
	int idx = getTableSectionIndex();

	for(int i=0;i<mytrack->CCLineSections->size();i++)
	{
		CCLineSection *t = (CCLineSection*)mytrack->CCLineSections->elementAt(i);
		t->setSelected(FALSE);
	}
	

	if (idx >= 0 && idx < mytrack->CCLineSections->size())
	{
	 CCLineSection *t = (CCLineSection*)mytrack->CCLineSections->elementAt(idx);

	 t->setSelected(TRUE);

	 if (t)
	 {
	   pDoc->getTree()->Expand(pDoc->getTree()->GetParentItem(t->getTreeNode()),TVE_EXPAND);
	   pDoc->getTree()->SelectItem(t->getTreeNode());
	 }
	}


	
	
	CMyTableView::OnLButtonDown(nFlags, point);
}

void CInfo::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int idx = getTableSectionIndex();

	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack * mytrack     = pDoc->getTrack();

	if (idx >= 0)
	{
	 CCLineSection *t = (CCLineSection*)mytrack->CCLineSections->elementAt(idx);


	 int result = cclinedlg->EditCCSections(t);
	 mytrack->createTrack(pDoc);
	 UpdateInfo(TRUE);

	 /*
	 if (cclinedlg && t)
	  cclinedlg->setSection(t,idx);

	 int res = cclinedlg->DoModal();

	 if (cclinedlg && t && res!=IDCANCEL)
	 {	  
	  cclinedlg->getSection(t);
	  //mytrack->PutCCSection(t);
	  mytrack->regenerate=TRUE;
	  mytrack->createCC();
	  UpdateInfo(TRUE);
	 }
	 */
	}

    CMyTableView::OnLButtonDblClk(nFlags, point);
}

void CInfo::OnSetFocus(CWnd* pOldWnd) 
{
	CMyTableView::OnSetFocus(pOldWnd);
	
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack * mytrack     = pDoc->getTrack();
	
    if (mytrack && mytrack->isValid())
	{
	 //UpdateInfo();
	}
}

void CInfo::OnLButtonUp(UINT nFlags, CPoint point) 
{
     CMyTableView::OnLButtonUp(nFlags, point);
}

void CInfo::OnExportTable() 
{
	// TODO: Add your command handler code here
	ExportTableData();
}

BOOL CInfo::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	BOOL res = CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	//SetImageList((((CTrackEditorApp*)AfxGetApp())->getSharedImageList()),LVSIL_SMALL);

	return res;
}

int CInfo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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


void CInfo::OnImportTable() 
{
	// TODO: Add your command handler code here
	Vector *tabledata = ImportTableData();
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack * mytrack = pDoc->getTrack();

	BOOL ignoreExtra = 0;
	BOOL removeExtra = 0;

	if (tabledata!=NULL)
	{
		int result = AfxMessageBox("About to Import CC Table Data!, are you sure you want to do this",MB_YESNO);

		if (result == IDYES)
		{
			if (tabledata->size() > mytrack->CCLineSections->size())
			{
				//ignoreExtra = AfxMessageBox("More Table Data Than Track (Ignore Extra Table Data)?",MB_YESNO);
 				AfxMessageBox("More Table Data Than CC Data (Ignoring Extra Table Data)?");
				ignoreExtra = TRUE;
			}
			else if (tabledata->size() < mytrack->CCLineSections->size())
			{
				//removeExtra = AfxMessageBox("More Track Data Than Table (Remove Extra Track Data)?",MB_YESNO);
				AfxMessageBox("More CC  Data Than Table (leaving end of ccline unaltered!)");
			}

			int importSize = tabledata->size();
			if (ignoreExtra == TRUE)
			{
			   importSize = mytrack->CCLineSections->size();
			}

			for(int i=0;i<importSize;i++)
			{
				Vector *row = (Vector *)tabledata->elementAt(i);
				CCLineSection *t = (CCLineSection*)mytrack->CCLineSections->elementAt(i);

				if (t!=NULL)
				{
				 ImportDataElement *elm = (ImportDataElement*)row->elementAt(2);
				 t->setLength(elm->toDouble());	

				 elm = (ImportDataElement*)row->elementAt(3);
				 t->setRadius(elm->toInt());	

				 elm = (ImportDataElement*)row->elementAt(4);
				 t->setArg2(elm->toInt());	                 			
				}
			}
		}
	}
	mytrack->createTrack(pDoc);
	mytrack->createCC();
	UpdateInfo(TRUE);
}


void CInfo::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CPoint pt;

	pt.x = 0;
	pt.y = 0;

	if (nChar == 13)
	{
	  OnLButtonDblClk(0,pt);
	}
	CMyTableView::OnKeyDown(nChar, nRepCnt, nFlags);
}
