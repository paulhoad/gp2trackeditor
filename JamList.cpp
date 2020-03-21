// JamList.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "JamList.h"
#include "JamFileEditor.h"
#include "JamPlus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJamList dialog


CJamList::CJamList(GPTrack *mytrack,CWnd* pParent /*=NULL*/)
	: CDialog(CJamList::IDD, pParent), track(mytrack)
{
	//{{AFX_DATA_INIT(CJamList)
	m_Index = _T("");
	m_Filename = _T("");
	//}}AFX_DATA_INIT

	currentSelectionIndex = -1;
}

CJamList::~CJamList()
{
	
}


void CJamList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJamList)
	DDX_Control(pDX, IDC_USUSED, m_UnusedList);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Text(pDX, IDC_INDEX, m_Index);
	DDX_Text(pDX, IDC_FILENAME, m_Filename);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJamList, CDialog)
	//{{AFX_MSG_MAP(CJamList)
	ON_WM_DRAWITEM()
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST1, OnKeydownList1)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJamList message handlers
void LoadJamListBox(GPTrack *track,CListCtrl *list);
void LoadUnused(CComboBox &box);

BOOL CJamList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	LoadJamListBox(track,&m_ListBox);

	m_ListBox.SetItemState (0,LVIS_FOCUSED | LVIS_SELECTED, 0x000F);

    LoadUnused(m_UnusedList);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

static int InsertString(CListCtrl *list,int i,int index,LPSTR name,int ImageId)
{
	//LV_ITEM NameVal;
	int mask = LVIF_TEXT;
	
	/*
	NameVal.mask = LVIF_TEXT | LVIF_IMAGE;
	if (ImageId!=-1 && index == 0)
	{
		NameVal.mask |= LVIF_IMAGE;
		NameVal.iImage = ImageId;
	}
	
	NameVal.iImage = ImageId;

	NameVal.iItem = 0;
	NameVal.iSubItem = index;
	NameVal.pszText = _T(name);
	NameVal.cchTextMax = 128;
	*/

	if (index == 0) 
	{
		//return list->InsertItem(&NameVal);
		return list->InsertItem(mask,0,name,0,0,ImageId,0);
	    //list->InsertItem(NameVal.mask,i,name,0,0,NameVal.iImage,0);
	}
	else
	{
		//return list->SetItem(&NameVal);
		return list->SetItem(0,index,mask,name,ImageId,0,0,0);
	    //list->SetItem(i,index,NameVal.mask,name,NameVal.iImage,0,0,0);
	}


}

int CALLBACK CompareJams(LPARAM lParam1, LPARAM lParam2, 
	LPARAM lParamSort)
{
	JAMExtraData *extraData1 = (JAMExtraData*)lParam1;
	JAMExtraData *extraData2 = (JAMExtraData*)lParam2;

	if (extraData1!=NULL && extraData2!=NULL)
	{

		return 	(extraData1->id - extraData2->id);
	}
	else
	{
		return 0;
	}
}

#define MAX_JAM_ID 600
static BOOL ususedID[MAX_JAM_ID];

void clearIndicators()
{
	for(int i=0;i<MAX_JAM_ID;i++)
	{
	   ususedID[i] = FALSE;
	}
}

void LoadUnused(CComboBox &box)
{
	for(int i=1;i<MAX_JAM_ID;i++)
	{
	   if (!ususedID[i])
	   {
		   CString msg;
		   msg.Format("ID %d (0x%x Hex)",i,i);
		   box.AddString(msg);
	   }
	}
	box.SetCurSel(0);
}

void LoadJamListBox(GPTrack *track,CListCtrl *list)
{
	char buffer[256];

	int colName = list->InsertColumn(0,"Id",LVCFMT_LEFT,50,0);
	int colHex = list->InsertColumn(1,"Id(Hex)",LVCFMT_LEFT,50,1);
	int colData = list->InsertColumn(2,"JamFile",LVCFMT_LEFT,150,2);
	//int colDesc = list->InsertColumn(2,"Args",LVCFMT_LEFT,100,2);
	clearIndicators();
   
	int size = track->JamFiles->size();
	int count=0;

	for(int i=0;i<size;i++)
	{
	   JamFileEditor *t = (JamFileEditor*)track->JamFiles->elementAt(i);

	   char *name = t->getJamFileName();

	   JAM *jams = t->getJAM();
	   jams->OpenToRead(TRUE);

	   int subimages = jams->getNumberOfImages();

	   for(int j=0;j<subimages;j++)
	   {
		int idx = jams->getImageId(j);
		if (idx > 0 && idx < MAX_JAM_ID)
		{
		  ususedID[idx] = TRUE;
		}

		wsprintf(buffer,"%d",idx);
	    int pos = InsertString(list,count,0,buffer,TO_ID(IDB_JAMFILE));
		JAMExtraData *extraData = new JAMExtraData(jams,count,j,name,idx);
		list->SetItemData(pos,(DWORD)extraData);

		wsprintf(buffer,"0x%x",idx);
		InsertString(list,pos,1,buffer,TO_ID(IDB_JAMFILE));

	    wsprintf(buffer,"%s",name);
	    InsertString(list,pos,2,buffer,TO_ID(IDB_JAMFILE));
	
		count++;
	   }
	}


	list->SortItems(CompareJams,0);
	
}


void CJamList::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDCtl == IDC_PREVIEW)
	{
	    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
        CRect rcItem(lpDrawItemStruct->rcItem);
        
        // Save dc state
        int nSavedDC = pDC->SaveDC();
		
		pDC->FillRect(rcItem, &CBrush(::GetSysColor(COLOR_WINDOW)));
		pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));

		JAMExtraData *extraData = NULL;

		if (currentSelectionIndex >=0)
			extraData = (JAMExtraData*)m_ListBox.GetItemData(currentSelectionIndex);
		else
			return;
		
		 
		if (extraData)
		{		
		 CBitmap bmp, *poldbmp;

		 JAM *jam = extraData->jam;
		 int idx  = extraData->subidx;

	     jam->LoadSingleBMPImage(bmp,jam->getImageWidth(idx), jam->getImageHeight(idx),idx);

		 char msg[256];
		 wsprintf(msg,"%d",extraData->id);
		 SetDlgItemText(IDC_INDEX,msg);
		 SetDlgItemText(IDC_FILENAME,extraData->filename);
	    
		
	     CDC memdc;
	   
         // Create a compatible memory DC
         memdc.CreateCompatibleDC( pDC );
 
	     // Select the bitmap into the DC
         poldbmp = memdc.SelectObject(&bmp);

	     int nWidth  = jam->getImageWidth(idx);
	     int nHeight = jam->getImageHeight(idx);
         pDC->BitBlt( 0,0, nWidth, nHeight, &memdc, 0,0, SRCCOPY );
	     memdc.SelectObject( &poldbmp );
		}		
	  	 // Restore dc
         pDC->RestoreDC( nSavedDC );
	}	
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CJamList::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	currentSelectionIndex = m_ListBox.GetNextItem(-1, LVNI_SELECTED);
	GetDlgItem(IDC_PREVIEW)->InvalidateRect(NULL);
	
	*pResult = 0;
}

void CJamList::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	currentSelectionIndex = m_ListBox.GetNextItem(-1, LVNI_SELECTED);
	GetDlgItem(IDC_PREVIEW)->InvalidateRect(NULL);
	
	*pResult = 0;
}

void CJamList::OnKeydownList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	currentSelectionIndex = m_ListBox.GetNextItem(-1, LVNI_SELECTED);
	GetDlgItem(IDC_PREVIEW)->InvalidateRect(NULL);
	
	*pResult = 0;
}

void CJamList::OnClose() 
{
	CDialog::OnClose();
}

void CJamList::OnCancel() 
{
	for(int i=0;i<m_ListBox.GetItemCount();i++)
	{	  
	  JAMExtraData *extraData = (JAMExtraData*)m_ListBox.GetItemData(i);
	  if (extraData!=NULL)
	  {
		  delete extraData;
		  extraData = NULL;
	  }
	}
	CDialog::OnCancel();
}

void CJamList::OnOK() 
{
	for(int i=0;i<m_ListBox.GetItemCount();i++)
	{	  
	  JAMExtraData *extraData = (JAMExtraData*)m_ListBox.GetItemData(i);
	  if (extraData!=NULL)
	  {
		  delete extraData;
		  extraData = NULL;
	  }
	}	
	CDialog::OnOK();
}
