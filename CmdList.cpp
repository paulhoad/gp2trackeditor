// CmdList.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "CmdList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdList dialog
extern CTrackEditorApp theApp;

CCmdList::CCmdList(CWnd* pParent /*=NULL*/)
	: CDialog(CCmdList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdList)
	//}}AFX_DATA_INIT
	SelectedItem = -1;
	SelectedItemCmd= -1;
}


void CCmdList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdList)
	DDX_Control(pDX, IDC_LIST1, m_CmdList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdList, CDialog)
	//{{AFX_MSG_MAP(CCmdList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_BN_CLICKED(ID_HELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdList message handlers

extern void LoadCmdListBox(CListCtrl *list);

BOOL CCmdList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	LoadCmdListBox(&m_CmdList);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCmdList::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//char  msg[256];


	

	if (pNMListView->uNewState)
	{
	  //wsprintf(msg,"Selected %d %d %d",	pNMListView->iItem,pNMListView->uNewState,pNMListView->uOldState);
	  //AfxMessageBox(msg);
	  SelectedItem = pNMListView->iItem;
	  SelectedItemCmd = m_CmdList.GetItemData(pNMListView->iItem);
	}
	
	*pResult = 0;
}

BOOL CCmdList::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	BOOL res=CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	m_CmdList.SetImageList((((CTrackEditorApp*)AfxGetApp())->getSharedImageList()),LVSIL_SMALL);

	return res;
}

void CCmdList::OnHelp() 
{
	CString top = "cmdlib.htm#";
	CString cmd;

	if (SelectedItemCmd != -1)
	{
	 cmd.Format("0x%x",SelectedItemCmd);

	 CString url = top+cmd;
	 theApp.OnHelpContentString(url);
	}
}

