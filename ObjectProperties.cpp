// ObjectProperties.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "ObjectProperties.h"
#include "Vertex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CTrackEditorApp theApp;
extern BOOL
  LoadJamTexture(GPTrack *mytrack, CBitmap &output, int idx);

/////////////////////////////////////////////////////////////////////////////
// CObjectProperties dialog

CObjectProperties::CObjectProperties(InternalObject *obj,
  CWnd *pParent /*=NULL*/)
  : CDialog(CObjectProperties::IDD, pParent), obj(obj)
{
  //{{AFX_DATA_INIT(CObjectProperties)
  m_ObjName = _T("");
  //}}AFX_DATA_INIT

  jamId = -1;
}

void CObjectProperties::DoDataExchange(CDataExchange *pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CObjectProperties)
  DDX_Control(pDX, IDC_LIST1, m_ObjList);
  DDX_Text(pDX, IDC_OBJNAME, m_ObjName);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CObjectProperties, CDialog)
//{{AFX_MSG_MAP(CObjectProperties)
ON_WM_DRAWITEM()
ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectProperties message handlers

BOOL CObjectProperties::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_ObjName = obj->getName();
  obj->Expand(theApp.getMainTree(), obj->getTreeNode());

  // TODO: Add extra initialization here
  if (obj->polygonPoints != NULL) {
    int size = obj->polygonPoints->size();
    for (int i = 0; i < size; i++) {
      CString msg;
      Vertex *ver = (Vertex *)obj->polygonPoints->elementAt(i);
      char *filename = obj->track->GetJAMFile(ver->textureid);
      msg.Format("v[%d] Txt:%d %s", i, ver->textureid, filename);
      int idx = m_ObjList.AddString(msg);
      m_ObjList.SetItemData(idx, ver->textureid);
    }
    m_ObjList.SetCurSel(0);
  }

  return TRUE;// return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CObjectProperties::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
  if (obj && nIDCtl == IDC_PREVIEW && jamId != -1) {
    CBitmap bmp, *poldbmp;
    LoadJamTexture(obj->track, bmp, jamId);

    CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rcItem(lpDrawItemStruct->rcItem);
    BITMAP cbmp;
    bmp.GetBitmap(&cbmp);
    CDC memdc;

    pDC->FillRect(rcItem, &CBrush(::GetSysColor(COLOR_WINDOW)));
    pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));

    // Create a compatible memory DC
    memdc.CreateCompatibleDC(pDC);

    // Select the bitmap into the DC
    poldbmp = memdc.SelectObject(&bmp);

    int nWidth = cbmp.bmWidth;
    int nHeight = cbmp.bmHeight;
    pDC->BitBlt(0, 0, nWidth, nHeight, &memdc, 0, 0, SRCCOPY);
    memdc.SelectObject(&poldbmp);
  }

  CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CObjectProperties::OnSelchangeList1()
{
  int idx = m_ObjList.GetCurSel();
  jamId = m_ObjList.GetItemData(idx);
  GetDlgItem(IDC_PREVIEW)->InvalidateRect(NULL, FALSE);
}
