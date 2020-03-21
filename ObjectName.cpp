// ObjectName.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "ObjectName.h"
#include "ObjectLookup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjectName dialog


CObjectName::CObjectName(CWnd* pParent /*=NULL*/)
	: CDialog(CObjectName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CObjectName)
	m_ObjName = _T("");
	//}}AFX_DATA_INIT

	selectedBitmap = IDB_OBJECT_UNK;
}


void CObjectName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectName)
	DDX_Control(pDX, IDC_LIST1, m_ObjList);
	DDX_Text(pDX, IDC_EDIT1, m_ObjName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CObjectName, CDialog)
	//{{AFX_MSG_MAP(CObjectName)
	ON_WM_DRAWITEM()
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_EN_UPDATE(IDC_EDIT1, OnChangeEdit1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectName message handlers

void CObjectName::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if (nIDCtl == IDC_PREVIEW)
	{
      CBitmap bmp,*poldbmp;

	  bmp.LoadBitmap(selectedBitmap);
	  CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
      CRect rcItem(lpDrawItemStruct->rcItem);
      BITMAP cbmp;
      bmp.GetBitmap( &cbmp );       
	  CDC memdc;

	  pDC->FillRect(rcItem, &CBrush(::GetSysColor(COLOR_WINDOW)));
	  pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));


	   
      // Create a compatible memory DC
      memdc.CreateCompatibleDC( pDC );
 
	  // Select the bitmap into the DC
      poldbmp = memdc.SelectObject(&bmp);

	  int nWidth  = cbmp.bmWidth;
	  int nHeight = cbmp.bmHeight;
      pDC->BitBlt( 0,0, nWidth, nHeight, &memdc, 0,0, SRCCOPY );
	  memdc.SelectObject( &poldbmp );
	}
	
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

BOOL CObjectName::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	for(int i=0;i<SizeOfLookUp;i++)
	{
	  int idx = m_ObjList.AddString(ObjectLookUpTable[i].name);
	  m_ObjList.SetItemData(idx,ObjectLookUpTable[i].id);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CObjectName::OnSelchangeList1() 
{	
	int idx = m_ObjList.GetCurSel();
	selectedBitmap = m_ObjList.GetItemData(idx);
	GetDlgItem(IDC_PREVIEW)->InvalidateRect(NULL,FALSE);
}

void CObjectName::OnChangeEdit1() 
{
	CString name;
	GetDlgItemText(IDC_EDIT1,name);
	CString newObjectname = CString(name);
	newObjectname.MakeLower();

	for(int i=0;i<SizeOfLookUp;i++)
	{
	   if (newObjectname.Find(ObjectLookUpTable[i].name)!=-1)
	   {
		  int idx = m_ObjList.FindStringExact(0,ObjectLookUpTable[i].name);
		  m_ObjList.SetCurSel(idx);
		  OnSelchangeList1();
	   }
	}
}
