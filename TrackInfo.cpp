// TrackInfo.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEditor.h"
#include "TrackInfo.h"
#include "TrackEditorDoc.h"
#include "TrackSection.h"
#include "GPTrack.h"
#include "Ruler.h"
#include "ImportDataElement.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern LPSTR toString(int data);
extern LPSTR toString(double data);
extern LPSTR toHexString(int data);



LPSTR toBinString(int data,int size)
{
	char *buffer = (char*)malloc(258*sizeof(char));
	memset(buffer,0,258);
	
	for(int i=0;i<size;i++)
	{
		int powD = (int)pow(2,i);

		if (data & powD)
		{
			buffer[size-i-1]='1';
		}
		else
			buffer[size-i-1]='0';
	}
	//CString returnStr(buffer);
	//free(buffer);
	return buffer;
}




/////////////////////////////////////////////////////////////////////////////
// CTrackInfo

//IMPLEMENT_DYNCREATE(CTrackInfo, CMyTableView)
IMPLEMENT_DYNCREATE(CTrackInfo, CMyTableView)

CTrackInfo::CTrackInfo()
{
	 trkdlg = new CTrChangeDlg(this);

}

CTrackInfo::~CTrackInfo()
{
	if (trkdlg!=NULL) delete trkdlg;
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();

	if (pDoc)
	{
	  pDoc->trackTable = NULL;
	}
}


//BEGIN_MESSAGE_MAP(CTrackInfo, CMyTableView)

BEGIN_MESSAGE_MAP(CTrackInfo, CMyTableView)
	//{{AFX_MSG_MAP(CTrackInfo)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_EXPORT_TABLE, OnExportTable)
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_IMPORT_TABLE, OnImportTable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackInfo drawing

void CTrackInfo::OnInitialUpdate()
{
}

#define UPDATE_TABLE_STRING_PTR(a,b,c,d) str =  d;UpdateTableStringPtr(a,b,c,str);free(str);

void CTrackInfo::UpdateInfo(BOOL update)
{
    CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	pDoc->trackTable = this;
	GPTrack * mytrack = pDoc->getTrack();

	//pDoc->SetTitle("Track Info");

	if (mytrack!=NULL)
	{
		  if (update == FALSE)
		  {
		   int colidx=0;
		   EmptyTable();

		  



		   addColumn("Index",50,colidx++);
		   addColumn("Angle",50,colidx++);
		   addColumn("Length",50,colidx++);
		   addColumn("Arc",50,colidx++);
		   addColumn("Delta Height ",50,colidx++);
		   addColumn("Iso Height ",50,colidx++);
		   addColumn("Gradient (HxL)",50,colidx++);
		   addColumn("Cumlative Gradient",50,colidx++);
		   addColumn("Altitude Delta",50,colidx++);
		   addColumn("Cumlative  Altitude",50,colidx++);
		   addColumn("StAngle",50,colidx++);
		   addColumn("EnAngle",50,colidx++);
		   addColumn("RoadSide",50,colidx++);
		   addColumn("RoadSide (Bin)",50,colidx++);
		   addColumn("Kerb",50,colidx++);
		   addColumn("Kerb(Bin)",50,colidx++);
		   addColumn("Left Road to Wall Distance",50,colidx++);
		   addColumn("Right Road to Wall Distance",50,colidx++);
		   addColumn("Track Width Left",50,colidx++);
		   addColumn("Track Width Right",50,colidx++);
		   addColumn("Length Meters",50,colidx++);
		   addColumn("Length Feet",50,colidx++);
		   addColumn("Cumlative Length",50,colidx++);
		   addColumn("Index",50,colidx++);
		  }

	    
		  if (mytrack->TrackSections)
		  {
		   for(int i=0;i<mytrack->TrackSections->size();i++)
           {
           TrackSection *t = (TrackSection*)mytrack->TrackSections->elementAt(i);
		   int tidx = 0;

		   //Column 1
		   LPSTR str = toString(i);
		   UpdateTableStringPtr(update,i,tidx++,str,TO_ID(t->getIcon()));free(str);

		   // Column 2
		   str = toString(t->getAngle());
		   UpdateTableStringPtr(update,i,tidx++,str);free(str);

		   // Column 3
		   str = toString(t->getLength());
		   UpdateTableStringPtr(update,i,tidx++,str);free(str);

		   // Column 4
		   str = toString(t->getAngle()*t->getLength());
		   UpdateTableStringPtr(update,i,tidx++,str);free(str);

		   // Column 5
		   str = toString(t->getHeight());
		   UpdateTableStringPtr(update,i,tidx++,str);free(str);

		   // Column 5
		   str = toString(t->getIsoHeight());
		   UpdateTableStringPtr(update,i,tidx++,str);free(str);

		   // Column 6
		   str = toString(t->getHeight()*t->getLength());
		   UpdateTableStringPtr(update,i,tidx++,str);free(str);

		   // Column 7
		   str = toString(t->getAltitude());
		   UpdateTableStringPtr(update,i,tidx++,str);free(str);

		   // Column 8
		   str =  toString(t->getAltitudeDelta());
		   UpdateTableStringPtr(update,i,tidx++,str);free(str);

		   UPDATE_TABLE_STRING_PTR(update,i,tidx++,toString(t->getTrueAltitude()));
		   UPDATE_TABLE_STRING_PTR(update,i,tidx++,toString(RadToDeg(t->getStartAngle())));
		   UPDATE_TABLE_STRING_PTR(update,i,tidx++,toString(RadToDeg(t->getEndAngle())));
		   UPDATE_TABLE_STRING_PTR(update,i,tidx++,toHexString(t->getRoadSignData()));
		   UPDATE_TABLE_STRING_PTR(update,i,tidx++,toBinString(t->getRoadSignData(),8));
		   UPDATE_TABLE_STRING_PTR(update,i,tidx++,toHexString(t->getKerbData()));
		   UPDATE_TABLE_STRING_PTR(update,i,tidx++,toBinString(t->getKerbData(),8));		   
		   UPDATE_TABLE_STRING_PTR(update,i,tidx++,toString(t->getLeftToBank()));
		   UPDATE_TABLE_STRING_PTR(update,i,tidx++,toString(t->getRightToBank()));
		   UPDATE_TABLE_STRING_PTR(update,i,tidx++,toString((int)t->getWidthLeft()));
		   UPDATE_TABLE_STRING_PTR(update,i,tidx++,toString((int)t->getWidthRight()));
		   UPDATE_TABLE_STRING_PTR(update,i,tidx++,toString((int)(LENGTH_TO_KM*t->getTrueLength())));
		   UPDATE_TABLE_STRING_PTR(update,i,tidx++,toString((int)(16.0*t->getTrueLength())));
		   UPDATE_TABLE_STRING_PTR(update,i,tidx++,toString((int)(t->track->LengthToTrackSectorEnd(t))));
		   str = toString(i);
		   UpdateTableStringPtr(update,i,tidx++,str,TO_ID(t->getIcon()));free(str);


           }
	   }
	}
}

 

void CTrackInfo::OnDraw(CDC* pDC)
{	
}




/////////////////////////////////////////////////////////////////////////////
// CTrackInfo diagnostics

#ifdef _DEBUG
void CTrackInfo::AssertValid() const
{
	CMyTableView::AssertValid();
}

void CTrackInfo::Dump(CDumpContext& dc) const
{
	CMyTableView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTrackInfo message handlers

BOOL CTrackInfo::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL res=CMyTableView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	
	

	


	

	return res;
}

BOOL CTrackInfo::OnScrollBy(CSize sizeScroll, BOOL bDoScroll) 
{
	return CMyTableView::OnScrollBy(sizeScroll, bDoScroll);
}

BOOL CTrackInfo::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) 
{
	return CMyTableView::OnScroll(nScrollCode, nPos, bDoScroll);
}

void CTrackInfo::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CMyTableView::OnLButtonDblClk(nFlags, point);
	// TODO: Add your message handler code here and/or call default
	int idx = getTableSectionIndex();

	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack * mytrack     = pDoc->getTrack();

	TrackSection *t = (TrackSection*)mytrack->TrackSections->elementAt(idx);


	int result = trkdlg->EditTrackSections(t);
	mytrack->createTrack(pDoc);
	UpdateInfo(TRUE);
}

void CTrackInfo::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack * mytrack     = pDoc->getTrack();

	int idx = getTableSectionIndex();

	for(int i=0;i<mytrack->TrackSections->size();i++)
	{
		TrackSection *t = (TrackSection*)mytrack->TrackSections->elementAt(i);
		t->setSelected(FALSE);
	}
	

	if (idx >= 0 && idx < mytrack->TrackSections->size())
	{
	 TrackSection *t = (TrackSection*)mytrack->TrackSections->elementAt(idx);

	 t->setSelected(TRUE);
	 if (t)
	 {
	   pDoc->getTree()->Expand(pDoc->getTree()->GetParentItem(t->getTreeNode()),TVE_EXPAND);
	   pDoc->getTree()->SelectItem(t->getTreeNode());
	 }
	}
	CMyTableView::OnLButtonDown(nFlags, point);
}

void CTrackInfo::OnSetFocus(CWnd* pOldWnd) 
{
	CMyTableView::OnSetFocus(pOldWnd);
}



void CTrackInfo::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CMyTableView::OnLButtonUp(nFlags, point);
}

void CTrackInfo::OnExportTable() 
{
	// TODO: Add your command handler code here
	CMyTableView::ExportTableData();
}

void CTrackInfo::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//table->OnMouseMove(nFlags,point);

	CMyTableView::OnMouseMove(nFlags, point);
}

int CTrackInfo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

	UpdateInfo(FALSE);

	return 0;
}

void CTrackInfo::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
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

#define TRACKANGLE 1
#define TRACKLENGTH 2
#define TRACKHEIGHT 4
#define TRACKRS     11
#define TRACKKB     13
#define TRACKWL     15
#define TRACKWR     16


void CTrackInfo::OnImportTable() 
{
	// TODO: Add your command handler code here
	Vector *tabledata = ImportTableData();
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack * mytrack = pDoc->getTrack();

	BOOL ignoreExtra = 0;
	BOOL removeExtra = 0;

	if (tabledata!=NULL)
	{
		int result = AfxMessageBox("About to Import Table Data!, are you sure you want to do this",MB_YESNO);

		if (result == IDYES)
		{
			if (tabledata->size() > mytrack->TrackSections->size())
			{
				//ignoreExtra = AfxMessageBox("More Table Data Than Track (Ignore Extra Table Data)?",MB_YESNO);
 				AfxMessageBox("More Table Data Than Track (Ignoring Extra Table Data)?");
				ignoreExtra = TRUE;
			}
			else if (tabledata->size() < mytrack->TrackSections->size())
			{
				//removeExtra = AfxMessageBox("More Track Data Than Table (Remove Extra Track Data)?",MB_YESNO);
				AfxMessageBox("More Track Data Than Table (leaving end of track unaltered!)");
			}

			int importSize = tabledata->size();
			if (ignoreExtra == TRUE)
			{
			   importSize = mytrack->TrackSections->size();
			}

			for(int i=0;i<importSize;i++)
			{
				Vector *row = (Vector *)tabledata->elementAt(i);
				TrackSection *t = (TrackSection*)mytrack->TrackSections->elementAt(i);

				if (t!=NULL)
				{
				 ImportDataElement *elm = NULL;
				 
				 elm= (ImportDataElement*)row->elementAt(TRACKANGLE);
				 t->setAngle(elm->toDouble());	

				 elm = (ImportDataElement*)row->elementAt(TRACKLENGTH);
				 t->setLength(elm->toInt());	

				 elm = (ImportDataElement*)row->elementAt(TRACKHEIGHT);
				 t->setHeight(elm->toInt());	 

                 elm = (ImportDataElement*)row->elementAt(TRACKRS);
				 t->setRoadSignData(elm->toInt());	 

				 elm = (ImportDataElement*)row->elementAt(TRACKKB);
				 t->setKerbData(elm->toInt());	 

				 elm = (ImportDataElement*)row->elementAt(TRACKWL);
				 t->setLeftToBank(elm->toInt());	 

				 elm = (ImportDataElement*)row->elementAt(TRACKWR);
				 t->setRightToBank(elm->toInt());	 

				}
			}
		}
	}
	mytrack->createTrack(pDoc);
	mytrack->createCC();
	UpdateInfo(TRUE);
}

void CTrackInfo::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CMyTableView::OnPrint(pDC, pInfo);
}
