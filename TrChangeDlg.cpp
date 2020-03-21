// TrChangeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEditor.h"
#include "TrChangeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void ValToCombo(int val);
int ValsToIndex(BOOL r8, BOOL r40, BOOL r80);

/////////////////////////////////////////////////////////////////////////////
// CTrChangeDlg dialog

CTrChangeDlg::CTrChangeDlg(CWnd *pParent /*=NULL*/)
  : CDialog(CTrChangeDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CTrChangeDlg)
  m_feature1 = 0;
  m_feature2 = 0;
  m_left_bank = 0;
  m_length = 0;
  m_right_bank = 0;
  m_angle = 0.0;
  m_height = 0.0;
  m_startangle = 0.0;
  m_endangle = 0.0;
  m_arc = 0.0;
  m_gradient = 0.0;
  m_altitude = 0.0;
  m_Sign1 = FALSE;
  m_Sign2 = FALSE;
  m_Sign3 = FALSE;
  m_Sign4 = FALSE;
  m_Sign5 = FALSE;
  m_Sign6 = FALSE;
  m_Sign7 = FALSE;
  m_Sign8 = FALSE;
  m_Kerb1 = FALSE;
  m_Kerb2 = FALSE;
  m_Kerb3 = FALSE;
  m_Kerb4 = FALSE;
  m_Kerb6 = FALSE;
  m_Kerb5 = FALSE;
  m_Kerb7 = FALSE;
  m_Kerb8 = FALSE;
  m_Index = 0;
  m_Annotation = _T("");
  //}}AFX_DATA_INIT

  currentSection = NULL;
}

void CTrChangeDlg::DoDataExchange(CDataExchange *pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CTrChangeDlg)
  DDX_Control(pDX, IDC_SIGN_COMBO, m_SignCombo);
  DDX_Text(pDX, IDC_FEATURE_1, m_feature1);
  DDV_MinMaxInt(pDX, m_feature1, 0, 255);
  DDX_Text(pDX, IDC_FEATURE_2, m_feature2);
  DDV_MinMaxInt(pDX, m_feature2, 0, 255);
  DDX_Text(pDX, IDC_TR_LEFT_BANK, m_left_bank);
  DDV_MinMaxInt(pDX, m_left_bank, 0, 255);
  DDX_Text(pDX, IDC_TR_LEN, m_length);
  DDV_MinMaxInt(pDX, m_length, 0, 255);
  DDX_Text(pDX, IDC_TR_RIGHT_BANK, m_right_bank);
  DDV_MinMaxInt(pDX, m_right_bank, 0, 255);
  DDX_Text(pDX, IDC_TR_ANGLE, m_angle);
  DDX_Text(pDX, IDC_TR_HEIGHT, m_height);
  DDX_Text(pDX, IDC_STARTANGLE, m_startangle);
  DDX_Text(pDX, IDC_ENDANGLE, m_endangle);
  DDX_Text(pDX, IDC_ARC, m_arc);
  DDX_Text(pDX, IDC_GRADIENT, m_gradient);
  DDX_Text(pDX, IDC_ALTITUDE, m_altitude);
  DDX_Check(pDX, IDC_CHECK1, m_Sign1);
  DDX_Check(pDX, IDC_CHECK2, m_Sign2);
  DDX_Check(pDX, IDC_CHECK3, m_Sign3);
  DDX_Check(pDX, IDC_CHECK4, m_Sign4);
  DDX_Check(pDX, IDC_CHECK5, m_Sign5);
  DDX_Check(pDX, IDC_CHECK6, m_Sign6);
  DDX_Check(pDX, IDC_CHECK7, m_Sign7);
  DDX_Check(pDX, IDC_CHECK8, m_Sign8);
  DDX_Check(pDX, IDC_KERB_01, m_Kerb1);
  DDX_Check(pDX, IDC_KERB_02, m_Kerb2);
  DDX_Check(pDX, IDC_RIGHT_KERB, m_Kerb3);
  DDX_Check(pDX, IDC_LEFT_KERB, m_Kerb4);
  DDX_Check(pDX, IDC_CHECK_WALL_LEFT, m_Kerb6);
  DDX_Check(pDX, IDC_CHECK_WALL_RIGHT, m_Kerb5);
  DDX_Check(pDX, IDC_KERB_40, m_Kerb7);
  DDX_Check(pDX, IDC_KERB_80, m_Kerb8);
  DDX_Text(pDX, IDC_INDEX, m_Index);
  DDX_Text(pDX, IDC_ANNOTATION, m_Annotation);
  //}}AFX_DATA_MAP

  ValToKerbs(m_feature2);
  ValToSign(m_feature1);
}

BEGIN_MESSAGE_MAP(CTrChangeDlg, CDialog)
//{{AFX_MSG_MAP(CTrChangeDlg)
ON_BN_CLICKED(IDC_KERB_01, OnKerb)
ON_BN_CLICKED(IDC_CHECK1, OnSign)
ON_EN_CHANGE(IDC_FEATURE_1, OnChangeFeature1)
ON_EN_CHANGE(IDC_FEATURE_2, OnChangeFeature2)
ON_BN_CLICKED(IDFOUND, OnFound)
ON_BN_CLICKED(IDC_SIGNCODES, OnSigncodes)
ON_BN_CLICKED(IDC_NEXT, OnNext)
ON_BN_CLICKED(IDC_PREV, OnPrev)
ON_CBN_SELCHANGE(IDC_SIGN_COMBO, OnSelchangeSignCombo)
ON_BN_CLICKED(IDC_HELP, OnHelp)
ON_BN_CLICKED(IDC_KERB_02, OnKerb)
ON_BN_CLICKED(IDC_KERB_40, OnKerb)
ON_BN_CLICKED(IDC_KERB_80, OnKerb)
ON_BN_CLICKED(IDC_LEFT_KERB, OnKerb)
ON_BN_CLICKED(IDC_RIGHT_KERB, OnKerb)
ON_BN_CLICKED(IDC_CHECK_WALL_LEFT, OnKerb)
ON_BN_CLICKED(IDC_CHECK_WALL_RIGHT, OnKerb)
ON_BN_CLICKED(IDC_CHECK10, OnSign)
ON_BN_CLICKED(IDC_CHECK2, OnSign)
ON_BN_CLICKED(IDC_CHECK3, OnSign)
ON_BN_CLICKED(IDC_CHECK4, OnSign)
ON_BN_CLICKED(IDC_CHECK5, OnSign)
ON_BN_CLICKED(IDC_CHECK6, OnSign)
ON_BN_CLICKED(IDC_CHECK7, OnSign)
ON_BN_CLICKED(IDC_CHECK8, OnSign)
ON_EN_UPDATE(IDC_FEATURE_1, OnChangeFeature1)
ON_EN_UPDATE(IDC_FEATURE_2, OnChangeFeature2)
ON_BN_CLICKED(ID_HELP, OnHelp)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrChangeDlg message handlers

void CTrChangeDlg::OnKerb()
{
  // TODO: Add your control notification handler code here
  int val = 0;

  m_Kerb1 = (BOOL)IsDlgButtonChecked(IDC_KERB_01);
  m_Kerb2 = (BOOL)IsDlgButtonChecked(IDC_KERB_02);
  m_Kerb3 = (BOOL)IsDlgButtonChecked(IDC_RIGHT_KERB);
  m_Kerb4 = (BOOL)IsDlgButtonChecked(IDC_LEFT_KERB);
  m_Kerb5 = (BOOL)IsDlgButtonChecked(IDC_CHECK_WALL_RIGHT);
  m_Kerb6 = (BOOL)IsDlgButtonChecked(IDC_CHECK_WALL_LEFT);
  m_Kerb7 = (BOOL)IsDlgButtonChecked(IDC_KERB_40);
  m_Kerb8 = (BOOL)IsDlgButtonChecked(IDC_KERB_80);

  if (m_Kerb1) val = val | 0x01;
  if (m_Kerb2) val = val | 0x02;
  if (m_Kerb3) val = val | 0x04;
  if (m_Kerb4) val = val | 0x08;
  if (m_Kerb5) val = val | 0x10;
  if (m_Kerb6) val = val | 0x20;
  if (m_Kerb7) val = val | 0x40;
  if (m_Kerb8) val = val | 0x80;

  m_feature2 = val;

  SetDlgItemInt(IDC_FEATURE_2, val);
}

void CTrChangeDlg::OnSign()
{
  // TODO: Add your control notification handler code here
  int val = 0;

  m_Sign1 = (BOOL)IsDlgButtonChecked(IDC_CHECK1);
  m_Sign2 = (BOOL)IsDlgButtonChecked(IDC_CHECK2);
  m_Sign3 = (BOOL)IsDlgButtonChecked(IDC_CHECK3);
  m_Sign4 = (BOOL)IsDlgButtonChecked(IDC_CHECK4);
  m_Sign5 = (BOOL)IsDlgButtonChecked(IDC_CHECK5);
  m_Sign6 = (BOOL)IsDlgButtonChecked(IDC_CHECK6);
  m_Sign7 = (BOOL)IsDlgButtonChecked(IDC_CHECK7);
  m_Sign8 = (BOOL)IsDlgButtonChecked(IDC_CHECK8);

  if (m_Sign1) val = val | 0x01;
  if (m_Sign2) val = val | 0x02;
  if (m_Sign3) val = val | 0x04;
  if (m_Sign4) val = val | 0x08;
  if (m_Sign5) val = val | 0x10;
  if (m_Sign6) val = val | 0x20;
  if (m_Sign7) val = val | 0x40;
  if (m_Sign8) val = val | 0x80;

  m_feature1 = val;

  SetDlgItemInt(IDC_FEATURE_1, val);
  ValToCombo(val);
}

void CTrChangeDlg::ValToKerbs(int val)
{
  CheckDlgButton(IDC_KERB_01, (val & 0x01) ? TRUE : FALSE);
  CheckDlgButton(IDC_KERB_02, (val & 0x02) ? TRUE : FALSE);
  CheckDlgButton(IDC_RIGHT_KERB, (val & 0x04) ? TRUE : FALSE);
  CheckDlgButton(IDC_LEFT_KERB, (val & 0x08) ? TRUE : FALSE);
  CheckDlgButton(IDC_CHECK_WALL_RIGHT, (val & 0x10) ? TRUE : FALSE);
  CheckDlgButton(IDC_CHECK_WALL_LEFT, (val & 0x20) ? TRUE : FALSE);
  CheckDlgButton(IDC_KERB_40, (val & 0x40) ? TRUE : FALSE);
  CheckDlgButton(IDC_KERB_80, (val & 0x80) ? TRUE : FALSE);
}

void CTrChangeDlg::ValToCombo(int val)
{
  BOOL rs8 = (val & 0x8);
  BOOL rs40 = (val & 0x40);
  BOOL rs80 = (val & 0x80);

  int selectionIndex = ValsToIndex(rs8, rs40, rs80);

  CComboBox *cmb = (CComboBox *)GetDlgItem(IDC_SIGN_COMBO);
  if (cmb != NULL) {
    cmb->SetCurSel(selectionIndex);
  }
}

void CTrChangeDlg::ValToSign(int val)
{
  CheckDlgButton(IDC_CHECK1, (val & 0x01) ? TRUE : FALSE);
  CheckDlgButton(IDC_CHECK2, (val & 0x02) ? TRUE : FALSE);
  CheckDlgButton(IDC_CHECK3, (val & 0x04) ? TRUE : FALSE);
  CheckDlgButton(IDC_CHECK4, (val & 0x08) ? TRUE : FALSE);
  CheckDlgButton(IDC_CHECK5, (val & 0x10) ? TRUE : FALSE);
  CheckDlgButton(IDC_CHECK6, (val & 0x20) ? TRUE : FALSE);
  CheckDlgButton(IDC_CHECK7, (val & 0x40) ? TRUE : FALSE);
  CheckDlgButton(IDC_CHECK8, (val & 0x80) ? TRUE : FALSE);
  ValToCombo(val);
}

int ValsToIndex(BOOL r8, BOOL r40, BOOL r80)
{
  if (!r8 && !r40 && !r80) return 0;
  if (r8 && !r40 && !r80) return 1;
  if (!r8 && r40 && !r80) return 2;
  if (!r8 && !r40 && r80) return 3;
  if (r8 && r40 && !r80) return 4;
  if (!r8 && r40 && r80) return 5;
  return 0;
}

void CTrChangeDlg::OnChangeFeature1()
{
  // TODO: Add your control notification handler code here
  int val;
  val = GetDlgItemInt(IDC_FEATURE_1);

  ValToSign(val);
}

void CTrChangeDlg::OnChangeFeature2()
{
  // TODO: Add your control notification handler code here
  int val;
  val = GetDlgItemInt(IDC_FEATURE_2);

  ValToKerbs(val);
}

void CTrChangeDlg::OnFound()
{
  // TODO: Add your control notification handler code here
  AfxMessageBox(
    "Have you Found something!, Please mail the Author if you think you may "
    "have some information about what codes do",
    MB_OK);
}

#define TC_300_AR_100 "Left Side <300 arrow (right) 100>  Code=68"
#define TC_AR_100 "Left Side <arrow (right) 100>  Code=128/132"

/*
 96 - 100m            (left)
112 - 200m            (left)
128 - 300m            (left)
144 - 90 degree right (left)
160 - 45 degree right (left)

176 - 100m            (right)
192 - 200m            (right)
208 - 300m            (right)
224 - 90 degree left  (right)
240 - 45 degree left  (right)
*/

void CTrChangeDlg::OnSigncodes()
{
  // TODO: Add your control notification handler code here
  char buffer[1024];

  wsprintf(buffer, "Code we know about\n%s\n%s\n", TC_300_AR_100, TC_AR_100);
  AfxMessageBox(buffer);
}

BOOL CTrChangeDlg::EditTrackSections(TrackSection *t)
{
  if (t) {
    setSection(t);
  }

  int result = DoModal();

  if (currentSection && result == IDOK) {
    getSection(currentSection);
    currentSection->track->createTrack(currentSection->track->GetDocument());
    return TRUE;
  }
  return FALSE;
}

void CTrChangeDlg::OnNext()
{
  if (currentSection != NULL) {
    UpdateData(TRUE);
    getSection(currentSection);
    currentSection->track->createTrack(currentSection->track->GetDocument());
    TrackSection *newSection =
      currentSection->track->GetNextSection(currentSection);
    setSection(newSection);
    UpdateData(FALSE);
  }
}

void CTrChangeDlg::OnPrev()
{
  // TODO: Add your control notification handler code here
  if (currentSection != NULL) {
    UpdateData(TRUE);
    getSection(currentSection);
    currentSection->track->createTrack(currentSection->track->GetDocument());
    TrackSection *newSection =
      currentSection->track->GetPreviousSection(currentSection);
    setSection(newSection);
    UpdateData(FALSE);
  }
}

BOOL CTrChangeDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO: Add extra initialization here
  m_SignCombo.AddString("No Signs");
  m_SignCombo.AddString("300, 200, 100");
  m_SignCombo.AddString("Arrow");
  m_SignCombo.AddString("Arrow, 100");
  m_SignCombo.AddString("300 Arrow 100");
  m_SignCombo.AddString("Arrow, 200, 100");

  OnChangeFeature1();

  return TRUE;// return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CTrChangeDlg::setSection(TrackSection *t)
{
  // char buffer[10];

  m_angle = t->getAngle();
  m_length = t->getLength();
  m_height = t->getHeight();
  m_right_bank = t->getRightToBank();
  m_left_bank = t->getLeftToBank();
  m_feature1 = t->rs.GetValue();
  m_feature2 = t->cb.GetValue();
  m_Index = t->getIndex();
  m_startangle = RadToDeg(t->getStartAngle());
  m_endangle = RadToDeg(t->getEndAngle());

  LPCSTR text = t->getAnnotation();
  if (text) {
    m_Annotation = CString(text);
  } else {
    m_Annotation = CString("");
  }

  // wsprintf(buffer,"%2.3f",t->getAngle()*t->getLength());
  // m_arc         = CString(buffer);
  m_arc = t->getAngle() * t->getLength();

  // wsprintf(buffer,"%2.3f",t->getAltitudeDelta());
  // m_altitude    = CString(buffer);
  m_altitude = t->getAltitudeDelta();

  m_gradient = t->getHeight() * t->getLength();
  currentSection = t;
}

void CTrChangeDlg::getSection(TrackSection *t)
{
  t->angle = m_angle;
  t->length = m_length;
  t->setHeight((int)m_height);
  t->setRightToBank(m_right_bank);
  t->setLeftToBank(m_left_bank);
  t->rs = m_feature1;
  t->cb = m_feature2;
  t->track->regenerate = TRUE;
  t->setAnnotation((LPSTR)(LPCSTR)m_Annotation);
}

void CTrChangeDlg::OnSelchangeSignCombo()
{
  // TODO: Add your control notification handler code here
  int idex = m_SignCombo.GetCurSel();

  BOOL v8, v40, v80;

  switch (idex) {
  case 0:
    v8 = FALSE;
    v40 = FALSE;
    v80 = FALSE;
    break;
  case 1:
    v8 = TRUE;
    v40 = FALSE;
    v80 = FALSE;
    break;
  case 2:
    v8 = FALSE;
    v40 = TRUE;
    v80 = FALSE;
    break;
  case 3:
    v8 = FALSE;
    v40 = FALSE;
    v80 = TRUE;
    break;
  case 4:
    v8 = TRUE;
    v40 = TRUE;
    v80 = FALSE;
    break;
  case 5:
    v8 = FALSE;
    v40 = TRUE;
    v80 = TRUE;
    break;
  }

  CheckDlgButton(IDC_CHECK4, v8);
  CheckDlgButton(IDC_CHECK7, v40);
  CheckDlgButton(IDC_CHECK8, v80);

  int val = 0;

  m_Sign1 = (BOOL)IsDlgButtonChecked(IDC_CHECK1);
  m_Sign2 = (BOOL)IsDlgButtonChecked(IDC_CHECK2);
  m_Sign3 = (BOOL)IsDlgButtonChecked(IDC_CHECK3);
  m_Sign4 = (BOOL)IsDlgButtonChecked(IDC_CHECK4);
  m_Sign5 = (BOOL)IsDlgButtonChecked(IDC_CHECK5);
  m_Sign6 = (BOOL)IsDlgButtonChecked(IDC_CHECK6);
  m_Sign7 = (BOOL)IsDlgButtonChecked(IDC_CHECK7);
  m_Sign8 = (BOOL)IsDlgButtonChecked(IDC_CHECK8);

  if (m_Sign1) val = val | 0x01;
  if (m_Sign2) val = val | 0x02;
  if (m_Sign3) val = val | 0x04;
  if (m_Sign4) val = val | 0x08;
  if (m_Sign5) val = val | 0x10;
  if (m_Sign6) val = val | 0x20;
  if (m_Sign7) val = val | 0x40;
  if (m_Sign8) val = val | 0x80;

  m_feature1 = val;

  SetDlgItemInt(IDC_FEATURE_1, val);
}

void CTrChangeDlg::OnHelp()
{
  CString url = "tutu.htm";
  url += "#trackchangedlg";
  theApp.OnHelpContentString(url);
}
