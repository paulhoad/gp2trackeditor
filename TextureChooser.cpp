// TextureChooser.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "TextureChooser.h"
#include "MyListCtrl.h"

#include "JamPlus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextureChooser dialog
void LoadTextureListBox(CMyListCtrl *list);

CTextureChooser::CTextureChooser(GPTrack *mytrack, CWnd *pParent /*=NULL*/)
  : CDialog(CTextureChooser::IDD, pParent), track(mytrack)
{
  //{{AFX_DATA_INIT(CTextureChooser)
  m_YOffset = _T("");
  //}}AFX_DATA_INIT

  SelectedItem = -1;
}

void CTextureChooser::DoDataExchange(CDataExchange *pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CTextureChooser)
  DDX_Control(pDX, IDC_EDIT4, m_Resolution);
  DDX_Control(pDX, IDC_EDIT3, m_Repeats);
  DDX_Control(pDX, IDC_EDIT2, m_Length);
  DDX_Control(pDX, IDC_EDIT1, m_Offset);
  DDX_Control(pDX, IDC_COMBO2, m_Rotation);
  DDX_Control(pDX, IDC_LIST1, m_List);
  DDX_Control(pDX, IDC_COMBO1, m_TextureCombo);
  DDX_Text(pDX, IDC_EDIT5, m_YOffset);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTextureChooser, CDialog)
//{{AFX_MSG_MAP(CTextureChooser)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
ON_WM_DRAWITEM()
ON_NOTIFY(LVN_KEYDOWN, IDC_LIST1, OnKeydownList1)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextureChooser message handlers

extern void
  LoadJamListBox(GPTrack *track, CListCtrl *list);

BOOL CTextureChooser::OnInitDialog()
{
  CDialog::OnInitDialog();

  CheckDlgButton(IDC_JAM_PREVIEW,
    track->showJamPreviews ? BST_CHECKED : BST_UNCHECKED);

  LoadJamListBox(track, &m_List);

  m_TextureCombo.AddString("Unk(0)");
  m_TextureCombo.AddString("Unk(1)");
  m_TextureCombo.AddString("A Right Kerb Outside(2)");
  m_TextureCombo.AddString("A Left  Kerb Outside(3)");
  m_TextureCombo.AddString("Right Fence(4)");
  m_TextureCombo.AddString("Left Fence(5)");

  m_TextureCombo.AddString("Unk(6)");
  m_TextureCombo.AddString("Unk(7)");

  m_TextureCombo.AddString("Right Verge(8)");
  m_TextureCombo.AddString("Left Verge(9)");

  m_TextureCombo.AddString("Road Surface(10)");
  m_TextureCombo.AddString("Ribbon1(11)");
  m_TextureCombo.AddString("Ribbon2(12)");
  m_TextureCombo.AddString("Ribbon3(13)");
  m_TextureCombo.AddString("Ribbon4(14)");
  m_TextureCombo.AddString("Unk(15)");
  m_TextureCombo.AddString("A Right Kerb Inside(16)");
  m_TextureCombo.AddString("A Left Kerb Inside(17)");
  m_TextureCombo.AddString("Bank Right(19)");
  m_TextureCombo.AddString("Bank Left(18)");
  m_TextureCombo.AddString("Pitlane On/Off Ramp(20)");
  m_TextureCombo.AddString("B Right Kerb Outside(25)");
  m_TextureCombo.AddString("B Left  Kerb Outside(26)");
  m_TextureCombo.AddString("B Right Kerb Inside(27)");
  m_TextureCombo.AddString("B Left Kerb Inside(28)");

  TRACE("SelectedLocation=%d", SelectedLocation);
  m_TextureCombo.SetCurSel(SelectedLocation);

  m_Rotation.AddString("0");
  m_Rotation.AddString("90");
  m_Rotation.AddString("180");
  m_Rotation.AddString("270");
  m_Rotation.AddString("360 (mirror)");
  m_Rotation.AddString("450 (mirror)");
  m_Rotation.AddString("540 (mirror)");
  m_Rotation.AddString("630 (mirror)");

  m_Rotation.SetCurSel(SelectedRotation);

  SetDlgItemInt(IDC_EDIT1, SelectedOffset);
  SetDlgItemInt(IDC_EDIT2, SelectedLength);
  SetDlgItemInt(IDC_EDIT3, SelectedRepeats);
  SetDlgItemInt(IDC_EDIT4, SelectedResolution);
  SetDlgItemInt(IDC_EDIT5, SelectedYOffset);

  BOOL found = FALSE;

  for (int i = 0; i < m_List.GetItemCount(); i++) {
    JAMExtraData *extraData = (JAMExtraData *)m_List.GetItemData(i);
    int texture = extraData->id;

    if (texture == SelectedTexture) {
      setSelected(i);
      SelectedItem = i;
      m_List.EnsureVisible(i, FALSE);
      found = TRUE;
      break;
    }
  }

  SetDlgItemInt(IDC_EDIT6, SelectedTexture);

  return TRUE;// return TRUE unless you set the focus to a control
              // EXCEPTION: OCX Property Pages should return FALSE
}

static void
  InsertString(CMyListCtrl *list, int row, int column, LPSTR name, BOOL update)
{
  LV_ITEM NameVal;

  NameVal.mask = LVIF_TEXT;
  NameVal.iItem = row;
  NameVal.iSubItem = column;
  NameVal.pszText = _T(name);
  NameVal.cchTextMax = 128;

  if (column == 0 && update == FALSE)
    list->InsertItem(&NameVal);
  else
    list->SetItem(&NameVal);
}

typedef struct
{
  int id;
  char *Desc;
  char *Jam;
  int icon_id;
} t_Texture;

static t_Texture TextureDesc[] = {
  { 0x01, "Drk Tarmac Gray", "-", IDB_SIDE_TARMAC },
  { 0x03, "Gray", "-", IDB_SIDE_TARMAC },
  { 0x06, "ScrubLand", "scrublnd.jam", IDB_SIDE_TARMAC },
  { 0x0A, "Gray", "-", IDB_SIDE_TARMAC },
  { 0x0B, "Gray", "-", IDB_SIDE_TARMAC },
  { 0x0C, "Gray", "-", IDB_SIDE_TARMAC },
  { 0x0D, "Gray", "-", IDB_SIDE_TARMAC },

  { 0x0E, "Starting Lights Red", "-", IDB_UNK },
  { 0x0F, "Starting Lights Green ", "-", IDB_UNK },

  { 0x14, "Pit Crew ", "-", IDB_UNK },
  { 0x15, "Pit Crew ", "-", IDB_UNK },

  { 0x17, "Wall", "pavxxx.jam", IDB_UNK },
  { 0x18, "Armco Barrier", "pavxxx.jam", IDB_UNK },

  { 0x1A, "Advert (CAMPARI)", "xxx_ad1.jam", IDB_UNK },
  { 0x1B, "Advert (MOBIL1)", "xxx_ad1.jam", IDB_UNK },
  { 0x1C, "Advert (FOSTERS)", "xxx_ad1.jam", IDB_UNK },
  { 0x1D, "Advert (RENAULT)", "xxx_ad1.jam", IDB_UNK },

  { 0x21, "Road", "Unk", IDB_SIDE_TARMAC },
  { 0x22, "Grass", "Unk", IDB_SIDE_GRASS },

  { 0x23, "Crowd", "crowd4.jam", IDB_CROWDICON },

  { 0x45, "Concrete Wall", "monaco22.jam", IDB_SIDE_TARMAC },

  { 0x4F, "Cross Walk", "monaco22.jam", IDB_SIDE_TARMAC },

  { 0x68, "Tyre Barrier", "-", IDB_SIDE_BLUE_WHITE_BARRIER },

  { 0x60, "Concrete Wall", "monaco22.jam", IDB_SIDE_TARMAC },
  { 0x6B, "Concrete Wall", "monaco22.jam", IDB_SIDE_TARMAC },

  { 0x76, "Yellow/White Barrier", "roadsgn.jam", IDB_SIDE_YELLOW_WHITE_BARRIER },
  { 0x77, "Blue/White Barrier", "roadsgn.jam", IDB_SIDE_BLUE_WHITE_BARRIER },
  { 0x78, "Red/White Barrier", "roadsgn.jam", IDB_SIDE_RED_WHITE_BARRIER },
  { 0x79, "Blue/White Barrier", "roadsgn.jam", IDB_SIDE_BLUE_WHITE_BARRIER },
  { 0x7A, "Red/White Barrier", "roadsgn.jam", IDB_SIDE_RED_WHITE_BARRIER },

  { 0xA0, "Tyre Barrier", "pavxxx.jam", IDB_UNK },
  { 0xA1, "Kerb 1", "pavxxx.jam", IDB_UNK },
  { 0xA2, "Kerb 2", "pavxxx.jam", IDB_UNK },
  { 0xA4, "Gravel", "pavxxx.jam", IDB_SIDE_SAND },

  { 0xAD, "Advert?", "adverts1.jam", IDB_UNK },
  { 0xAE, "Advert?", "adverts1.jam", IDB_UNK },
  { 0xAF, "Advert?", "adverts1.jam", IDB_UNK },
  { 0xB0, "Advert?", "adverts1.jam", IDB_UNK },
  { 0xB1, "Advert?", "adverts1.jam", IDB_UNK },
  { 0xB2, "Advert?", "adverts1.jam", IDB_UNK },
  { 0xB3, "Advert?", "adverts1.jam", IDB_UNK },
  { 0xB4, "Advert?", "adverts1.jam", IDB_UNK },
  { 0xB5, "Advert?", "adverts1.jam", IDB_UNK },
  { 0xB6, "Advert?", "adverts1.jam", IDB_UNK },
  { 0xB7, "Advert?", "adverts1.jam", IDB_UNK },
  { 0xB8, "Advert?", "adverts2.jam", IDB_UNK },
  { 0xB9, "Advert?", "adverts2.jam", IDB_UNK },
  { 0x1A, "Advert?", "xxx_ad1.jam", IDB_UNK },
  { 0x1B, "Advert?", "xxx_ad1.jam", IDB_UNK },
  { 0x1C, "Advert?", "xxx_ad1.jam", IDB_UNK },
  { 0x1D, "Advert?", "xxx_ad1.jam", IDB_UNK },
  { 0x1E, "Advert?", "xxx_ad1.jam", IDB_UNK },
};

static int texturedescs = sizeof(TextureDesc) / sizeof(TextureDesc[0]);

LPCSTR
getTextureDescription(int id)
{
  for (int i = 0; i < texturedescs; i++) {
    t_Texture texture = TextureDesc[i];

    if (texture.id == id) {
      static char buffer[256];
      wsprintf(buffer, "0x%x %s", texture.id, texture.Desc);
      return buffer;
    }
  }
  return "Unknown";
}

LPCSTR
getSceneryLocation(int id)
{
  static char buffer[256];

  switch (id) {
  case 104:
    wsprintf(buffer, "Verges Green Walls");
    break;
  case 112:
    wsprintf(buffer, "Scrubland");
    break;
  case 176:
    wsprintf(buffer, "Some Walls");
    break;
  case 186:
    wsprintf(buffer, "Track");
    break;
  case 194:
    wsprintf(buffer, "Trees and Bushes");
    break;
  default:
    wsprintf(buffer, "Unknown Scenery Location (%d) (please report!)", id);
    break;
  }

  return buffer;
}

LPCSTR
getTextureLocation(int id)
{
  static char buffer[256];

  switch (id) {
  case 2:
    wsprintf(buffer, "Type A Right Kerb Outside Part(%d)", id);
    break;
  case 3:
    wsprintf(buffer, "Type A Left Kerb Outside Part(%d)", id);
    break;
  case 4:
    wsprintf(buffer, "Right Fence(%d)", id);
    break;
  case 5:
    wsprintf(buffer, "Left Fence(%d)", id);
    break;
  case 8:
    wsprintf(buffer, "Right Verge(%d)", id);
    break;
  case 9:
    wsprintf(buffer, "Left Verge(%d)", id);
    break;
  case 10:
    wsprintf(buffer, "Road Surface(%d)", id);
    break;
  case 11:
    wsprintf(buffer, "Ribbon1(%d)", id);
    break;
  case 12:
    wsprintf(buffer, "Ribbon2(%d)", id);
    break;
  case 13:
    wsprintf(buffer, "Ribbon3(%d)", id);
    break;
  case 14:
    wsprintf(buffer, "Ribbon4(%d)", id);
    break;
  case 16:
    wsprintf(buffer, "Type A Right Kerb Inside Part(%d)", id);
    break;
  case 17:
    wsprintf(buffer, "Type A Left Kerb Inside Part(%d)", id);
    break;
  case 19:
    wsprintf(buffer, "Right Bank(%d)", id);
    break;
  case 18:
    wsprintf(buffer, "Left Bank(%d)", id);
    break;
  case 20:
    wsprintf(buffer, "Pitlane On/Off Ramp(%d)", id);
    break;
  case 25:
    wsprintf(buffer, "Type B Right Kerb Outside Part(%d)", id);
    break;
  case 26:
    wsprintf(buffer, "Type B Left Kerb Outside Part(%d)", id);
    break;
  case 27:
    wsprintf(buffer, "Type B Right Kerb Inside Part(%d)", id);
    break;
  case 28:
    wsprintf(buffer, "Type B Left Kerb Inside Part(%d)", id);
    break;
  default:
    wsprintf(buffer, "Unk(%d)", id);
    break;
  }

  return buffer;
}

int getTextureIcon(int id)
{
  for (int i = 0; i < texturedescs; i++) {
    t_Texture texture = TextureDesc[i];

    if (texture.id == id) {
      return texture.icon_id;
    }
  }
  return IDB_SIDE_UNK;
}

void CTextureChooser::OnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
  NM_LISTVIEW *pNMListView = (NM_LISTVIEW *)pNMHDR;
  // TODO: Add your control notification handler code here
  SelectedItem = m_List.GetNextItem(-1, LVNI_SELECTED);
  GetDlgItem(IDC_PREVIEW)->InvalidateRect(NULL);

  JAMExtraData *extraData = NULL;

  if (SelectedItem >= 0)
    extraData = (JAMExtraData *)m_List.GetItemData(SelectedItem);

  if (extraData) {
    SelectedTexture = extraData->id;
    SetDlgItemInt(IDC_EDIT6, SelectedTexture);
  }

  /*
  if (pNMListView->uNewState)
  {
    SelectedItem = pNMListView->iItem;
    SelectedTexture = m_List.GetItemData(pNMListView->iItem);
  }
  */

  *pResult = 0;
}

void CTextureChooser::OnOK()
{
  // TODO: Add extra validation here
  int pos = m_TextureCombo.GetCurSel();

  SelectedLocation = pos;

  SelectedRotation = m_Rotation.GetCurSel();

  track->showJamPreviews = IsDlgButtonChecked(IDC_JAM_PREVIEW);

  SelectedOffset = GetDlgItemInt(IDC_EDIT1);
  SelectedLength = GetDlgItemInt(IDC_EDIT2);
  SelectedRepeats = GetDlgItemInt(IDC_EDIT3);
  SelectedResolution = GetDlgItemInt(IDC_EDIT4);
  SelectedYOffset = GetDlgItemInt(IDC_EDIT5);

  CDialog::OnOK();
}

void CTextureChooser::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
  if (!IsDlgButtonChecked(IDC_JAM_PREVIEW)) return;

  if (nIDCtl == IDC_PREVIEW) {
    CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rcItem(lpDrawItemStruct->rcItem);

    // Save dc state
    int nSavedDC = pDC->SaveDC();

    pDC->FillRect(rcItem, &CBrush(::GetSysColor(COLOR_WINDOW)));
    pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));

    JAMExtraData *extraData = NULL;

    if (SelectedItem >= 0)
      extraData = (JAMExtraData *)m_List.GetItemData(SelectedItem);

    if (extraData) {
      CBitmap bmp, *poldbmp;

      JAM *jam = extraData->jam;
      int idx = extraData->subidx;

      jam->LoadSingleBMPImage(bmp, jam->getImageWidth(idx), jam->getImageHeight(idx), idx);

      // char msg[256];
      // wsprintf(msg,"%d",extraData->id);
      // SetDlgItemText(IDC_INDEX,msg);
      // SetDlgItemText(IDC_FILENAME,extraData->filename);

      CDC memdc;

      // Create a compatible memory DC
      memdc.CreateCompatibleDC(pDC);

      // Select the bitmap into the DC
      poldbmp = memdc.SelectObject(&bmp);

      int nWidth = jam->getImageWidth(idx);
      int nHeight = jam->getImageHeight(idx);
      pDC->BitBlt(0, 0, nWidth, nHeight, &memdc, 0, 0, SRCCOPY);
      memdc.SelectObject(&poldbmp);
    } else {
      char buf[256];
      wsprintf(buf, "No Item Data at %d", SelectedItem);
      AfxMessageBox(buf);
    }
    // Restore dc
    pDC->RestoreDC(nSavedDC);
  }

  CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CTextureChooser::OnKeydownList1(NMHDR *pNMHDR, LRESULT *pResult)
{
  LV_KEYDOWN *pLVKeyDow = (LV_KEYDOWN *)pNMHDR;
  SelectedItem = m_List.GetNextItem(-1, LVNI_SELECTED);
  GetDlgItem(IDC_PREVIEW)->InvalidateRect(NULL);

  JAMExtraData *extraData = NULL;

  if (SelectedItem >= 0)
    extraData = (JAMExtraData *)m_List.GetItemData(SelectedItem);

  if (extraData) {
    SelectedTexture = extraData->id;
    SetDlgItemInt(IDC_EDIT6, SelectedTexture);
  }

  *pResult = 0;
}
