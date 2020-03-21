// CmdRemoval.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "CmdRemoval.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern void
  LoadCmdListBox(CListCtrl* list);

/////////////////////////////////////////////////////////////////////////////
// CCmdRemoval dialog

CCmdRemoval::CCmdRemoval(CWnd* pParent /*=NULL*/)
  : CDialog(CCmdRemoval::IDD, pParent)
{
  //{{AFX_DATA_INIT(CCmdRemoval)
  m_CmdVal = _T("");
  //}}AFX_DATA_INIT
}

void CCmdRemoval::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CCmdRemoval)
  DDX_Control(pDX, IDC_LIST1, m_CmdList);
  DDX_Text(pDX, IDC_CMD_VAL, m_CmdVal);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCmdRemoval, CDialog)
//{{AFX_MSG_MAP(CCmdRemoval)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
ON_WM_CREATE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdRemoval message handlers

void CCmdRemoval::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
  NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

  if (pNMListView->uNewState) {
    int SelectedItem = pNMListView->iItem;
    int SelectedItemCmd = m_CmdList.GetItemData(pNMListView->iItem);
    CString cmdSelected;
    cmdSelected.Format("0x%x", SelectedItemCmd);
    m_CmdVal = cmdSelected;
    UpdateData(FALSE);
  }

  *pResult = 0;
}

BOOL CCmdRemoval::OnInitDialog()
{
  CDialog::OnInitDialog();

  LoadCmdListBox(&m_CmdList);

  return TRUE;// return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

int CCmdRemoval::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CDialog::OnCreate(lpCreateStruct) == -1) return -1;

  // TODO: Add your specialized creation code here

  return 0;
}
