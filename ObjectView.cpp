// ObjectView.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "ObjectView.h"
#include "TrackEditorDoc.h"
#include "TrackSection.h"
#include "GPTrack.h"
#include "DataEntry.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjectView

extern LPSTR toString(int data);
extern LPSTR toString(double data);
extern LPSTR toHexString(int data);

/*
CString toHexString(int data)
{
	char mess[256];

	wsprintf(mess,"0x%x",data);
	return CString(mess);
}
*/





IMPLEMENT_DYNCREATE(CObjectView, CMyTableView)

CObjectView::CObjectView():
inter_obj(NULL)
{

}

CObjectView::~CObjectView()
{
}

#define DESC_COL 4
#define SHORT_DATA 2

void CObjectView::UpdateInfo(BOOL update)
{
    CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack * mytrack = pDoc->getTrack();
	InternalObject *obj = GetObject();

	if (mytrack!=NULL)
	{
		  if (!update)
		  {
		   int colidx=0;
		   EmptyTable();
		   addColumn("Index",50,colidx++);
		   addColumn("Offset",50,colidx++);
	       addColumn("Data (Hex)",100,colidx++);
		   addColumn("Data (Dec:short)",100,colidx++);
	       addColumn("Description",300,colidx++);
		  }

		  int rowidx=0;
		  
		   if (obj)
		  {
		   int offset=0;
		   int pos =0;
		   ObjectData *dat = (ObjectData*)obj->objectData->elementAt(pos);

		   
		   UpdateTableStringPtr(FALSE,rowidx,0,toString(pos));
		   UpdateTableStringPtr(FALSE,rowidx,1,toString(offset));
		   UpdateTableStringPtr(FALSE,rowidx,2,toHexString(dat->GetValue()));
		   UpdateTableStringPtr(FALSE,rowidx,3,toString((short)dat->GetValue()));
		   UpdateTableStringPtr(FALSE,rowidx,DESC_COL,CString("Item Identifier"));
		   offset+=dat->getSize();
		   pos++;
		   rowidx++;

		   // unknown possible offset or id
		   dat = (ObjectData*)obj->objectData->elementAt(pos);
		   UpdateTableStringPtr(FALSE,rowidx,0,toString(pos));pos++;
		   UpdateTableStringPtr(FALSE,rowidx,1,toString(offset));
		   if (dat)
		   {
		     UpdateTableStringPtr(FALSE,rowidx,2,toHexString(dat->GetValue()));
			 UpdateTableStringPtr(FALSE,rowidx,3,toString((short)dat->GetValue()));
		   }
		   UpdateTableStringPtr(FALSE,rowidx,DESC_COL,CString("Unknown Possible Item Id or Offset"));
		   offset+=dat->getSize();
		   rowidx++;

		   CString OffsetDesc[] =
		   {
			   "Pointer to Scale Section",
			   "Pointer to Unk Section",
			   "Pointer to Texture Section",
			   "Pointer to Point Coordinate Section",
			   "Pointer to Vertex List Section",
			   "Pointer to Unk Section",
			   "Pointer to End Section",
			   "Pointer to End Of Object Section",
			   "Pointer to End Of Object Section"
		   };

		   // 9 unknown
		   for(int i=0;i<9;i++)
		   { 
			   char buffer[256];
			   dat = (ObjectData*)obj->objectData->elementAt(pos);
			   wsprintf(buffer,"%d of 9 = %s",i+1,OffsetDesc[i]);		       
			   UpdateTableStringPtr(FALSE,rowidx,0,toString(pos));pos++;
			   UpdateTableStringPtr(FALSE,rowidx,1,toString(offset));
			   if (dat)
			   {
		         UpdateTableStringPtr(FALSE,rowidx,2,toHexString(dat->GetValue()));
				 UpdateTableStringPtr(FALSE,rowidx,3,toString((short)dat->GetValue()));
			   }
		       UpdateTableStringPtr(FALSE,rowidx,DESC_COL,CString(buffer));
		   	   rowidx++;
			   offset+=dat->getSize();
		   }

		   // always 0
		   dat = (ObjectData*)obj->objectData->elementAt(pos);
		   UpdateTableStringPtr(FALSE,rowidx,0,toString(pos));pos++;
		   UpdateTableStringPtr(FALSE,rowidx,1,toString(offset));
		   if (dat)
		   {
		     UpdateTableStringPtr(FALSE,rowidx,2,toHexString(dat->GetValue()));
			 UpdateTableStringPtr(FALSE,rowidx,3,toString((short)dat->GetValue()));
		   }
		   UpdateTableStringPtr(FALSE,rowidx,DESC_COL,CString("Always Zero"));
		   rowidx++;
		   offset+=dat->getSize();

		   // always 1
		   dat = (ObjectData*)obj->objectData->elementAt(pos);
		   UpdateTableStringPtr(FALSE,rowidx,0,toString(pos));pos++;
		   UpdateTableStringPtr(FALSE,rowidx,1,toString(offset));
		   if (dat)
		   {
		    UpdateTableStringPtr(FALSE,rowidx,2,toHexString(dat->GetValue()));
			UpdateTableStringPtr(FALSE,rowidx,3,toString((short)dat->GetValue()));
		   }
		   UpdateTableStringPtr(FALSE,rowidx,DESC_COL,CString("Always One"));
		   rowidx++;
		   offset+=dat->getSize();

		   // unknown
		   dat = (ObjectData*)obj->objectData->elementAt(pos);
		   UpdateTableStringPtr(FALSE,rowidx,0,toString(pos));pos++;
		   UpdateTableStringPtr(FALSE,rowidx,1,toString(offset));
		   if (dat)
		   {
		     UpdateTableStringPtr(FALSE,rowidx,2,toHexString(dat->GetValue()));
			 UpdateTableStringPtr(FALSE,rowidx,3,toString((short)dat->GetValue()));
		   }
		   UpdateTableStringPtr(FALSE,rowidx,DESC_COL,CString("Unknown"));
		   rowidx++;
		   offset+=dat->getSize();

		   // size
		   dat = (ObjectData*)obj->objectData->elementAt(pos);
		   UpdateTableStringPtr(FALSE,rowidx,0,toString(pos));pos++;
		   UpdateTableStringPtr(FALSE,rowidx,1,toString(offset));
		   if (dat)
		   {
		     UpdateTableStringPtr(FALSE,rowidx,2,toHexString(dat->GetValue()));
			 UpdateTableStringPtr(FALSE,rowidx,3,toString((short)dat->GetValue()));
		   }
		   UpdateTableStringPtr(FALSE,rowidx,DESC_COL,CString("Size"));
		   rowidx++;
		   offset+=dat->getSize();

		   // size * 8
		   dat = (ObjectData*)obj->objectData->elementAt(pos);
		   UpdateTableStringPtr(FALSE,rowidx,0,toString(pos));pos++;
		   UpdateTableStringPtr(FALSE,rowidx,1,toString(offset));
		   if (dat)
		   {
		     UpdateTableStringPtr(FALSE,rowidx,2,toHexString(dat->GetValue()));
			 UpdateTableStringPtr(FALSE,rowidx,3,toString((short)dat->GetValue()));
		   }
		   UpdateTableStringPtr(FALSE,rowidx,DESC_COL,CString("Size *8"));
		   rowidx++;
		   offset+=dat->getSize();

		   // 9 byte common patron?
		   for(i=pos;i<pos+7;i++)
		   {
			   char buffer[256];
			   dat = (ObjectData*)obj->objectData->elementAt(i);
		       wsprintf(buffer,"%d of 7 Words of same byte sequence for all objs",i-pos+1);
			   UpdateTableStringPtr(FALSE,rowidx,0,toString(i));
		       UpdateTableStringPtr(FALSE,rowidx,1,toString(offset));
			   if (dat)
			   {
		         UpdateTableStringPtr(FALSE,rowidx,2,toHexString(dat->GetValue()));
				 UpdateTableStringPtr(FALSE,rowidx,3,toString((short)dat->GetValue()));
			   }
		       UpdateTableStringPtr(FALSE,rowidx,DESC_COL,CString(buffer));
		       rowidx++;
		       offset+=dat->getSize();
		   }

		   pos+=7;

		   for(i=pos;i<obj->objectData->size();i++)
		   { 
			   dat       = (ObjectData*)obj->objectData->elementAt(i);
			   
			   UpdateTableStringPtr(FALSE,rowidx,0,toString(i));
			   UpdateTableStringPtr(FALSE,rowidx,1,toString(offset));
			   if (dat)
			   {
		         UpdateTableStringPtr(FALSE,rowidx,2,toHexString(dat->GetValue()));
				 UpdateTableStringPtr(FALSE,rowidx,3,toString((short)dat->GetValue()));
			   }

			   CString str = CString("Unknown"); 

			   if (dat->GetValue() == 0x8400) 
			   {
				   str = CString ("Possible start of XYZ?");
			   }
			   if (dat->GetValue() == 0x0083) 
			   {
				   str = CString ("Texture Command 0x8300?");
			   }
			   if (dat->GetValue() == 0x0003) 
			   {
				   str = CString ("Texture Command 0x0300?");
			   }

		       UpdateTableStringPtr(FALSE,rowidx,DESC_COL,str);
		       rowidx++;
		       offset+=dat->getSize();
		   }		  
		  }
		  else
		  {
		   UpdateTableStringPtr(FALSE,rowidx,0,CString("No Valid Object"));
		   UpdateTableStringPtr(FALSE,rowidx,1,CString("No Valid Object"));
		   UpdateTableStringPtr(FALSE,rowidx,2,CString("No Valid Object"));
		   UpdateTableStringPtr(FALSE,rowidx,DESC_COL,CString("No Valid Object"));
		   rowidx++;
		  }
		}


	  if (obj)
	  {
	  //offsets
	  int beginOff = ((ObjectData*)obj->objectData->elementAt(2))->GetValue();
	  for(int i=2;i<11;i++)
	  {
		 char buff[256];

		 ObjectData *dat = (ObjectData*)obj->objectData->elementAt(i);
		 int newoffset = dat->GetValue();
		 int diff      = newoffset - beginOff;

		 wsprintf(buff,"Offset(%d) offset from begin=%d",i-2,diff);
		 UpdateTableStringPtr(FALSE,i,DESC_COL,CString(buff));
	  }

	  // comment the model.....	
	  UpdateTableStringPtr(FALSE,13,DESC_COL,CString("Always One"));

	  // beginning of constant
	  int idx = 16;
	  UpdateTableStringPtr(FALSE,idx,DESC_COL,CString("Constant Start"));
	  idx+=2;
	  UpdateTableStringPtr(FALSE,idx++,DESC_COL,CString("Constant End"));
	  UpdateTableStringPtr(FALSE,idx++,DESC_COL,CString("Normally 0x3AE"));
	  UpdateTableStringPtr(FALSE,idx++,DESC_COL,CString("Normally Zero"));
	  UpdateTableStringPtr(FALSE,idx++,DESC_COL,CString("Normally Zero (but not always!)"));
	  UpdateTableStringPtr(FALSE,idx++,DESC_COL,CString("Normally 0xAAA"));
	  UpdateTableStringPtr(FALSE,idx++,DESC_COL,CString("Size of 1st sector after vertex list"));
	  int LastDataSize = ((ObjectData*)obj->objectData->elementAt(24))->GetValue();
	  UpdateTableStringPtr(FALSE,idx++,DESC_COL,CString("End of vertexlist calculator (val1)"));
	  UpdateTableStringPtr(FALSE,idx++,DESC_COL,CString("Normally 0x3800"));
	  UpdateTableStringPtr(FALSE,idx++,DESC_COL,CString("val - val1 = Size of 2nd sector after vertex list"));
	  UpdateTableStringPtr(FALSE,idx++,DESC_COL,CString("val - val1 = Size of 3nd sector after vertex list"));
	  // 78  scale
	  UpdateTableStringPtr(FALSE,idx++,DESC_COL,CString("Scale"));
	  // 80 padding always zero
	  UpdateTableStringPtr(FALSE,idx++,DESC_COL,CString("0000 structure padding"));
	  // 82
	  UpdateTableStringPtr(FALSE,idx++,DESC_COL,CString("Unkown Command"));
	  // 84
	  int size = ((ObjectData*)obj->objectData->elementAt(14))->GetValue();
	  int size8 = ((ObjectData*)obj->objectData->elementAt(15))->GetValue();
	  int verticies = size8/8;
	  int texturestart = 78+size+2;
	  int texturestartpos = 29 + (size/2) + 1; 
	  UpdateTableStringPtr(FALSE,texturestartpos,DESC_COL,CString("Start of Texture Commands"));


	   for(int k=idx;k<obj->objectData->size();k++)
	   {
		   ObjectData *dat = (ObjectData*)obj->objectData->elementAt(k);
		   int val = dat->GetValue();

		   if (val == 0x0083)
		   {
			   for(int j=k - 2;j<k+9;j++)
			   {
				   UpdateTableStringPtr(FALSE,j,DESC_COL,CString("Part of Texture Commands 0x0083"));
			   }
		   }
		   if (val == 0x0003)
		   {
			   for(int j=k - 2;j<k+9;j++)
			   {
				   UpdateTableStringPtr(FALSE,j,DESC_COL,CString("Part of Texture Commands 0x0003"));
			   }
		   }
		   if (val == 0x0084)
		   {
			   UpdateTableStringPtr(FALSE,k,DESC_COL,CString("Start of XY Data"));
			   k++;
			   for(int j=0;j<verticies;j++)
			   {
				   char buff[256];
				   wsprintf(buff,"X begin vertex %d of %d",j,verticies);

				   if (k < obj->objectData->size())
				   {
				   ObjectData *Xdat = (ObjectData*)obj->objectData->elementAt(k);
				   int X = Xdat->GetValue();
				   if (X > 32767) X = (32768-X);
				   UpdateTableStringPtr(FALSE,k,DESC_COL,CString(buff));
				   wsprintf(buff,"%d",X);
				   UpdateTableStringPtr(FALSE,k++,SHORT_DATA,CString(buff));
				   }

				   if (k < obj->objectData->size())
				   {
				    ObjectData *Zdat = (ObjectData*)obj->objectData->elementAt(k);
				    int Z = Zdat->GetValue();
				    if (Z > 32767) Z = (32768-Z);
				   
				    UpdateTableStringPtr(FALSE,k,DESC_COL,CString("Z"));
				    wsprintf(buff,"%d",Z);
				    UpdateTableStringPtr(FALSE,k++,SHORT_DATA,CString(buff));
				    UpdateTableStringPtr(FALSE,k++,DESC_COL,CString("Unk"));
				   }


				   if (k < obj->objectData->size())
				   {
				     ObjectData *Ydat = (ObjectData*)obj->objectData->elementAt(k);
				    int Y = Ydat->GetValue();
				    if (Y > 32767) Y = (32768-Y);
				    UpdateTableStringPtr(FALSE,k,DESC_COL,CString("Y"));
				    wsprintf(buff,"%d",Y);
				    UpdateTableStringPtr(FALSE,k++,SHORT_DATA,CString(buff));
				   }
				   
			   }
			   for(j=0;j<verticies;j++)
			   {
				   UpdateTableStringPtr(FALSE,k++,DESC_COL,CString("Vectex List (Unused)"));

				   char buff[256];
				   
				   if (k < obj->objectData->size())
				   {
				   ObjectData *dat = (ObjectData*)obj->objectData->elementAt(k);
		           int val = dat->GetValue();
				   int pt1 = LOBYTE(val);
				   int pt2 = HIBYTE(val);
				   wsprintf(buff,"vertex %d  connecting %d to %d",j,pt1,pt2);				   
				   UpdateTableStringPtr(FALSE,k++,DESC_COL,CString(buff));
				   }

			   }
			   //UpdateTableStringPtr(k++,DESC_COL,CString("0x8 separator"));
			   for(j=0;j<2;j++)
			   {
				   for(int l=0;l<LastDataSize/2;l++)
				   {
					 char buff[256];

					 wsprintf(buff,"Trailing Command Sequence %d subword %d",j,l);
				     UpdateTableStringPtr(FALSE,k++,DESC_COL,CString(buff));   
				   }
			   }
		   }
	   }
	   UpdateTableStringPtr(FALSE,texturestartpos,DESC_COL,CString("Start of Texture Commands"));
	  }
}


BEGIN_MESSAGE_MAP(CObjectView, CMyTableView)
	//{{AFX_MSG_MAP(CObjectView)
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_EXPORT_TABLE, OnExportTable)
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectView drawing

void CObjectView::OnInitialUpdate()
{
	
}

void CObjectView::OnDraw(CDC* pDC)
{
	

}

/////////////////////////////////////////////////////////////////////////////
// CObjectView diagnostics

#ifdef _DEBUG
void CObjectView::AssertValid() const
{
	CMyTableView::AssertValid();
}

void CObjectView::Dump(CDumpContext& dc) const
{
	CMyTableView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CObjectView message handlers

void CObjectView::OnSetFocus(CWnd* pOldWnd) 
{
	CMyTableView::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	GPTrack * mytrack     = pDoc->getTrack();
	
    if (mytrack && mytrack->isValid())
	{
	 //UpdateInfo();
	}
	
}

BOOL CObjectView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CMyTableView::OnScroll(nScrollCode, nPos, bDoScroll);
}

BOOL CObjectView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CMyTableView::OnScrollBy(sizeScroll, bDoScroll);
}

void CObjectView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMyTableView::OnLButtonDown(nFlags, point);
}

void CObjectView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int idx = getTableSectionIndex();

	
	char buffer[256];
	
	CDataEntry *entry = new CDataEntry();
	
	InternalObject *obj = GetObject();
	ObjectData *dat = (ObjectData*)obj->objectData->elementAt(idx);
	int val = dat->GetValue();
	entry->m_NewValue = val;
    wsprintf(buffer,"%d",val);
	entry->m_PreviousValue = CString(buffer);

	int result = entry->DoModal();

	if (result==IDOK)
	{
	   CTrackEditorDoc* pDoc = (CTrackEditorDoc*)GetDocument();
	   GPTrack * mytrack     = pDoc->getTrack();

	   int nval = entry->m_NewValue;
	   dat->SetValue(nval);

	   UpdateInfo(FALSE);
	   mytrack->regenerate=TRUE;
	}

	CMyTableView::OnLButtonDblClk(nFlags, point);
}

BOOL CObjectView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CObjectView::OnExportTable() 
{
	// TODO: Add your command handler code here
	ExportTableData();
}

int CObjectView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMyTableView::OnCreate(lpCreateStruct) == -1)
		return -1;

	
	// TODO: Add your specialized creation code here
	UpdateInfo(FALSE);

	return 0;
}

void CObjectView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
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
