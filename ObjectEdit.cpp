// ObjectEdit.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "ObjectEdit.h"
#include "JAM.h"
#include "trackeditordoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjectEdit dialog

CObjectEdit::CObjectEdit(GPTrack *mytrack, CWnd *pParent /*=NULL*/)
  : CDialog(CObjectEdit::IDD, pParent), track(mytrack)
{
  //{{AFX_DATA_INIT(CObjectEdit)
  m_Id2 = 0;
  m_AngleX = 0;
  m_AngleY = 0;
  m_Detail = 0;
  m_Height = 0;
  m_Unk1 = 0;
  m_Unk2 = 0;
  m_Y = 0;
  m_Name = _T("");
  m_InternalExternal = _T("");
  m_InternalNumber = 0;
  //}}AFX_DATA_INIT
}

void CObjectEdit::DoDataExchange(CDataExchange *pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CObjectEdit)
  DDX_Control(pDX, IDC_UNK2_SPIN, m_Unk2Spin);
  DDX_Control(pDX, IDC_ID2_SPIN, m_Id2Spin);
  DDX_Control(pDX, IDC_UNK1_SPIN, m_Unk1Spin);
  DDX_Control(pDX, IDC_HEIGHT_SPIN, m_HeightSpin);
  DDX_Control(pDX, IDC_DFC_SPIN, m_DfcSpin);
  DDX_Control(pDX, IDC_ID_1, m_IdCombo);
  DDX_Control(pDX, IDC_DETAILCOMBO, m_DetailCombo);
  DDX_Control(pDX, IDC_LOCATOR, m_ObjectCombo);
  DDX_Text(pDX, IDC_ID_2, m_Id2);
  DDX_Text(pDX, IDC_OBJECT_ANGLE_X, m_AngleX);
  DDX_Text(pDX, IDC_OBJECT_ANGLE_Y, m_AngleY);
  DDX_Text(pDX, IDC_OBJECT_DETAIL, m_Detail);
  DDX_Text(pDX, IDC_OBJECT_HEIGHT, m_Height);
  DDX_Text(pDX, IDC_OBJECT_UNK1, m_Unk1);
  DDX_Text(pDX, IDC_OBJECT_UNK2, m_Unk2);
  DDX_Text(pDX, IDC_OBJECT_Y, m_Y);
  DDX_Text(pDX, IDC_NAME, m_Name);
  DDX_Text(pDX, IDC_OBJECTTYPE, m_InternalExternal);
  DDX_Text(pDX, IDC_INTERNALOBJECTNUMBER, m_InternalNumber);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CObjectEdit, CDialog)
//{{AFX_MSG_MAP(CObjectEdit)
ON_CBN_SELCHANGE(IDC_DETAILCOMBO, OnSelchangeDetailcombo)
ON_WM_DRAWITEM()
ON_NOTIFY(NM_CLICK, IDC_ID2_SPIN, OnClickId2Spin)
ON_EN_CHANGE(IDC_ID_2, OnChangeId2)
ON_CBN_SELCHANGE(IDC_ID_1, OnSelchangeId1)
ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
ON_BN_CLICKED(ID_HELP, OnHelp)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectEdit message handlers

BOOL CObjectEdit::OnInitDialog()
{
  CDialog::OnInitDialog();
  char buffer[7];

  CheckDlgButton(IDC_JAM_PREVIEW,
    track->showJamPreviews ? BST_CHECKED : BST_UNCHECKED);

  for (int i = 0; i < track->TrackObjectDefintions->size(); i++) {
    TrackObjectDefinition *tobj =
      (TrackObjectDefinition *)track->TrackObjectDefintions->elementAt(i);

    wsprintf(buffer, "%d", tobj->getLocator());
    m_ObjectCombo.AddString(buffer);
  }
  wsprintf(buffer, "%d", locatorStart);
  m_ObjectCombo.SelectString(-1, buffer);

  m_DetailCombo.AddString("Always On");
  m_DetailCombo.AddString("Detail Level High");
  m_DetailCombo.AddString("Detail Level Medium");
  m_DetailCombo.AddString("Detail Level Low");
  m_DetailCombo.AddString("Show Before Fences");

  if (m_Detail == 4) m_DetailCombo.SetCurSel(0);

  if (m_Detail == 100) m_DetailCombo.SetCurSel(1);

  if (m_Detail == 200) m_DetailCombo.SetCurSel(2);

  if (m_Detail == 64) m_DetailCombo.SetCurSel(2);

  if (m_Detail == 148) m_DetailCombo.SetCurSel(3);

  // what is special about 100 and 2
  //
  if (m_Detail == 130) m_DetailCombo.SetCurSel(3);
  if (m_Detail == 2) m_DetailCombo.SetCurSel(1);

  for (int i = 0; i < 17; i++) {
    char buffer[256];
    sprintf(buffer, "%d - gp2 defined object", i);
    m_IdCombo.AddString(buffer);
  }

  int numberOfInternalObjects = track->InternalObjectOffsets->size();

  for (int i = 0; i < numberOfInternalObjects; i++) {
    char buffer[256];
    InternalObject *t =
      (InternalObject *)track->InternalObjectOffsets->elementAt(i);

    sprintf(buffer, "%d - %s", i + 17, t->getName());
    m_IdCombo.AddString(buffer);
  }

  // TODO: Add extra initialization here
  m_IdCombo.SetCurSel(m_Id1);

  m_Unk2Spin.SetRange(0, 30000);
  m_Unk1Spin.SetRange(0, 30000);
  // m_Id2Spin.SetRange(0,5000);
  m_HeightSpin.SetRange(-30000, 30000);
  m_DfcSpin.SetRange(-30000, 30000);
  // m_AngleZSpin.SetRange(0,65535);
  // m_AngleXSpin.SetRange(0,65535);
  m_Id2Spin.SetRange(0, 5000);

  return TRUE;// return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CObjectEdit::OnOK()
{
  // TODO: Add extra validation here
  int idx = m_ObjectCombo.GetCurSel();
  TrackObjectDefinition *tobj =
    (TrackObjectDefinition *)track->TrackObjectDefintions->elementAt(idx);

  locatorStart = tobj->getLocator();

  m_Id1 = m_IdCombo.GetCurSel();

  track->showJamPreviews = IsDlgButtonChecked(IDC_JAM_PREVIEW);

  CDialog::OnOK();
}

void CObjectEdit::OnSelchangeDetailcombo()
{
  int idx = m_DetailCombo.GetCurSel();

  switch (idx) {
  case 0:
    m_Detail = 4;
    break;
  case 1:
    m_Detail = 100;
    break;
  case 2:
    m_Detail = 64;
    break;
  case 3:
    m_Detail = 2;
    break;
  case 4:
    m_Detail = 148;
    break;
  }

  SetDlgItemInt(IDC_OBJECT_DETAIL, m_Detail);
}

void MakObject256SquareImage(CBitmap &jamBitmap, CBitmap &currentImg, int nWidth, int nHeight)
{
  CDC memdcA, memdcB;
  CBitmap *poldbmps, *poldbmpd;
  // Create a compatible memory DC
  memdcA.CreateCompatibleDC(NULL);
  memdcB.CreateCompatibleDC(NULL);

  // Select the bitmap into the DC
  poldbmps = memdcA.SelectObject(&jamBitmap);
  poldbmpd = memdcB.SelectObject(&currentImg);

  // memdcB.BitBlt( 0, 0, 256, 256, &memdcA, 0,0, SRCCOPY );
  memdcB.StretchBlt(0, 0, 128, 128, &memdcA, 0, 0, nWidth, nHeight, SRCCOPY);

  memdcA.SelectObject(&poldbmps);
  memdcB.SelectObject(&poldbmpd);
}

BOOL LoadJamTexture(GPTrack *mytrack, CBitmap &output, int idx)
{
  JAM *jam = mytrack->GetJAM(idx);
  if (jam == NULL) {
    output.LoadBitmap(IDB_MISSING_JAM);
    return FALSE;
  }
  int sidx = jam->getImageIndex(idx);
  jam->LoadSingleBMPImage(output, jam->getImageWidth(sidx), jam->getImageHeight(sidx), sidx);
  return TRUE;
}

void CObjectEdit::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
  if (!IsDlgButtonChecked(IDC_JAM_PREVIEW)) return;
  // TODO: Add your message handler code here and/or call default
  if (nIDCtl == IDC_PREVIEW && m_Id1 == 5) {
    CBitmap bmp, *poldbmp;
    int jamId = m_Id2 + 256;
    LoadJamTexture(track, bmp, jamId);

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

    CString msg;
    msg.Format("Jam Id:%d", jamId);
    SetDlgItemText(IDC_JAMID, msg);
  }

  CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CObjectEdit::OnClickId2Spin(NMHDR *pNMHDR, LRESULT *pResult)
{
  if (GetDlgItem(IDC_PREVIEW))
    GetDlgItem(IDC_PREVIEW)->InvalidateRect(NULL, FALSE);

  *pResult = 0;
}

void CObjectEdit::OnChangeId2()
{
  // UpdateData(TRUE);
  // m_Id2 = GetDlgItemInt(IDC_ID_2);
  if (GetDlgItem(IDC_PREVIEW)) {
    UpdateData(TRUE);
    GetDlgItem(IDC_PREVIEW)->InvalidateRect(NULL, TRUE);
  }
}

void CObjectEdit::OnSelchangeId1()
{
  UpdateData(TRUE);
  m_Id1 = m_IdCombo.GetCurSel();
  // m_Id2 = GetDlgItemInt(IDC_ID_2);
  if (GetDlgItem(IDC_PREVIEW))
    GetDlgItem(IDC_PREVIEW)->InvalidateRect(NULL, TRUE);

  GetDlgItem(IDC_BUTTON2)->EnableWindow(m_Id1 >= 17);
  GetDlgItem(IDC_BUTTON3)->EnableWindow(m_Id1 >= 17);
}

void CObjectEdit::OnButton2()
{
  m_Id1 = m_IdCombo.GetCurSel();

  if (m_Id1 > 17) {
    int internalObjectId = m_Id1 - 17;

    int numberOfInternalObjects = track->InternalObjectOffsets->size();
    if (internalObjectId >= 0 && internalObjectId < numberOfInternalObjects) {
      InternalObject *t =
        (InternalObject *)track->InternalObjectOffsets->elementAt(
          internalObjectId);
      // t->Expand(theApp.getMainTree(),t->getTreeNode());
      ((CTrackEditorDoc *)track->GetDocument())->setCurrentObject(t);
      theApp.OnInternalView(track->GetDocument());
    }
  }
}

void CObjectEdit::OnButton3()
{
  m_Id1 = m_IdCombo.GetCurSel();

  if (m_Id1 > 17) {
    int internalObjectId = m_Id1 - 17;

    int numberOfInternalObjects = track->InternalObjectOffsets->size();
    if (internalObjectId >= 0 && internalObjectId < numberOfInternalObjects) {
      InternalObject *t =
        (InternalObject *)track->InternalObjectOffsets->elementAt(
          internalObjectId);
      t->Expand(theApp.getMainTree(), t->getTreeNode());
      theApp.getMainTree()->getTree()->Expand(
        theApp.getMainTree()->getTree()->GetParentItem(t->getTreeNode()),
        TVE_EXPAND);
      theApp.getMainTree()->getTree()->SelectItem(t->getTreeNode());
    }
  }
}

void CObjectEdit::OnHelp()
{
  CString url = "object.htm";
  url += "#Input_objects";
  theApp.OnHelpContentString(url);
}
