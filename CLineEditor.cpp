// CLineEditor.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEditor.h"
#include "CLineEditor.h"
#include "CCLineSection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCLineEditor dialog


CCLineEditor::CCLineEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CCLineEditor::IDD, pParent),currentSection(NULL)
{
	//{{AFX_DATA_INIT(CCLineEditor)
	m_ccrad = 0;
	m_cclen = 0;
	m_cccmd = 0;
	m_ccidx = 0;
	m_ccang = 0;
	m_ccarg1 = 0;
	m_ccarg4 = 0;
	m_ccarg2 = 0;
	m_truerad = 0;
	m_LeftCurve = FALSE;
	m_RadiusCardinal = 0;
	m_ccarg3 = 0;
	//}}AFX_DATA_INIT

	radBoxhasFocus = FALSE;
	radiusBoxhasFocus = FALSE;
}


void CCLineEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCLineEditor)
	DDX_Text(pDX, IDC_CCRAD, m_ccrad);
	DDX_Text(pDX, IDC_CCLEN, m_cclen);
	DDX_Text(pDX, IDC_CCCMD, m_cccmd);
	DDX_Text(pDX, IDC_CCIDX, m_ccidx);
	DDX_Text(pDX, IDC_CCARG1, m_ccarg1);
	DDX_Text(pDX, IDC_CCARG4, m_ccarg4);
	DDX_Text(pDX, IDC_CCARG2, m_ccarg2);
	DDX_Text(pDX, IDC_TRUERAD, m_truerad);
	DDX_Text(pDX, IDC_RADIUSCARD, m_RadiusCardinal);
	DDX_Text(pDX, IDC_CCARG3, m_ccarg3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCLineEditor, CDialog)
	//{{AFX_MSG_MAP(CCLineEditor)
	ON_BN_CLICKED(IDC_PREV, OnPrev)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_EN_CHANGE(IDC_CCARG3, OnChangeArgs)
	ON_EN_CHANGE(IDC_RADIUSCARD, OnChangeRadiuscard)
	ON_BN_CLICKED(IDC_RIGHT, OnRight)
	ON_BN_CLICKED(IDC_STRAIGHT, OnStraight)
	ON_BN_CLICKED(IDC_LEFT, OnLeft)
	ON_EN_SETFOCUS(IDC_CCRAD, OnSetfocusCcrad)
	ON_EN_KILLFOCUS(IDC_CCRAD, OnKillfocusCcrad)
	ON_EN_CHANGE(IDC_CCARG4, OnChangeArgs)
	ON_EN_CHANGE(IDC_CCCMD, OnChangeArgs)
	ON_EN_CHANGE(IDC_CCRAD, OnChangeArgs)
	ON_EN_CHANGE(IDC_CCLEN, OnChangeArgs)
	ON_EN_UPDATE(IDC_CCARG1, OnChangeArgs)
	ON_EN_UPDATE(IDC_CCARG2, OnChangeArgs)
	ON_BN_CLICKED(IDC_CURVE_LEFT, OnCurveCheck)
	ON_BN_CLICKED(ID_HELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//ON_EN_CHANGE(IDC_RADIUSCARD, OnChangeRadiuscard)
//ON_EN_UPDATE(IDC_RADIUSCARD, OnChangeRadiuscard)


void CCLineEditor::OnCurveCheck()
{
	if  (m_ccarg2 < 0) 
	{
		m_ccarg2 = -(m_ccarg2+1);
		m_ccrad = 0xFFFF - m_ccrad;

	}
	else  
	{
		m_ccarg2 = -m_ccarg2-1;
		m_ccrad = 0xFFFF - m_ccrad;
	}
	UpdateData(FALSE);
	OnChangeArgs();
}

/////////////////////////////////////////////////////////////////////////////
// CCLineEditor message handlers

void CCLineEditor::OnPrev() 
{
	if (currentSection!=NULL)
	{
		UpdateData(TRUE);
		getSection(currentSection);
		CCLineSection *nextSection = (CCLineSection*)currentSection->track->CCLineSections->previous(currentSection);
		setSection(nextSection);
		UpdateData(FALSE);
		OnChangeArgs();
	}
}

void CCLineEditor::OnNext() 
{
	if (currentSection!=NULL)
	{
		UpdateData(TRUE);
		getSection(currentSection);
		currentSection->track->createTrack(currentSection->track->GetDocument());
		CCLineSection *nextSection = (CCLineSection*)currentSection->track->CCLineSections->next(currentSection);
		setSection(nextSection);		
		UpdateData(FALSE);
		OnChangeArgs();
	}
}


BOOL CCLineEditor::EditCCSections(CCLineSection *t)
{
	if (t)
	{
	  setSection(t);
	}

	if (currentSection == NULL) return FALSE;

	int result = DoModal();

	if (t && result==IDOK)
	{	  
	  getSection(currentSection);
	  currentSection->track->createCC();
	  return TRUE;
	}
	return FALSE;
}

void CCLineEditor::OnChangeArgs() 
{
	OnMyChangeArgs(TRUE);
}

void CCLineEditor::OnMyChangeArgs(BOOL update) 
{
	if (radBoxhasFocus && update) return;
	if (currentSection == NULL) return;

	if (update)
	{
	 m_ccrad  = GetDlgItemInt(IDC_CCRAD);
	 m_ccarg2 = GetDlgItemInt(IDC_CCARG2);
	}
	m_cclen  = GetDlgItemInt(IDC_CCLEN);

	double rad       = m_ccrad;
	int    direction = m_ccarg2;
	double length    = m_cclen;

	double rad2=0;
	m_truerad = RadiusTo(currentSection->track,rad,direction,m_ccarg1,m_cccmd,m_ccarg3,m_ccarg4,rad2);

	char buff[256];
	sprintf(buff,"%f",m_truerad);
	SetDlgItemText(IDC_TRUERAD,buff);

	if (m_truerad != 0)
	{
	 double anglerad = ((length)/m_truerad);
	 sprintf(buff,"%f",RadToDeg(anglerad));
	 SetDlgItemText(IDC_TRUEANGLE,buff);	 
	}
	else
	{
	 SetDlgItemText(IDC_TRUEANGLE,"0");
	}

	int m_Direction = IDC_STRAIGHT;

	if (m_truerad != 0)
	{
	 if (m_truerad <= 0) 
	 {
		m_Direction = IDC_LEFT;
	 }
	 else
	 {
		m_Direction = IDC_RIGHT;
	 }
	}
	CheckRadioButton(IDC_LEFT,IDC_RIGHT,m_Direction);

	if (update)
	{
	 
	 double rad2=0;
     m_RadiusCardinal = RadiusVal(currentSection->track,rad,direction,m_ccarg1,m_cccmd,m_ccarg3,m_ccarg4,rad2);
	 sprintf(buff,"%d",m_RadiusCardinal);
	 radiusBoxhasFocus = TRUE;
	 SetDlgItemText(IDC_RADIUSCARD,buff);
	 radiusBoxhasFocus = FALSE;
	}


	if (((CEdit*)GetDlgItem(IDC_CCARG3)))
       ((CEdit*)GetDlgItem(IDC_CCARG3))->SetReadOnly(m_cccmd==0x50);
	if (((CEdit*)GetDlgItem(IDC_CCARG4)))
	   ((CEdit*)GetDlgItem(IDC_CCARG4))->SetReadOnly(m_cccmd==0x50);
}

BOOL CCLineEditor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	OnChangeArgs();

	if (m_cccmd == 0x50)
	{
	   ((CEdit*)GetDlgItem(IDC_CCARG3))->SetReadOnly();
	   ((CEdit*)GetDlgItem(IDC_CCARG4))->SetReadOnly();
	}


	//m_SignSpin.SetRange(-100,100);
	//m_LengthSpin.SetRange(0,1000);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCLineEditor::OnChangeRadiuscard() 
{
   OnMyChangeRadiuscard(TRUE);
}

void CCLineEditor::OnMyChangeRadiuscard(BOOL update) 
{	
	if (radiusBoxhasFocus) return;
	CString data;
	GetDlgItemText(IDC_RADIUSCARD,data);

	if (data.GetLength() == 0) return;
	// TODO: Add your control notification handler code here
	int radiusAbs = GetDlgItemInt(IDC_RADIUSCARD);

	
	

	if (IsDlgButtonChecked(IDC_LEFT))
	{
		radiusAbs=-radiusAbs;
	}
	else if (IsDlgButtonChecked(IDC_RIGHT))
	{
		//do nothing
	}
	else
	{
		//radiusAbs = 0;
	}
	
	//RadiusFrom(radiusAbs);
	double radius=radiusAbs;
	int direction=0;
	RadiusFrom(radius,direction);

	TRACE("%f %d\n",radius,direction);

    m_ccrad  = (int)radiusAbs;
	m_ccarg2 = direction;

	radBoxhasFocus = TRUE;
	SetDlgItemInt(IDC_CCRAD,m_ccrad);
	SetDlgItemInt(IDC_CCARG2,m_ccarg2);
	radBoxhasFocus = FALSE;
	
	OnMyChangeArgs(FALSE);
}


void CCLineEditor::OnLeft() 
{
	// TODO: Add your control notification handler code here
	//OnChangeRadiuscard();
	if (m_ccarg2>=0)
	{
	 m_ccarg2 = (-m_ccarg2)-1;
	 m_ccrad = 0xFFFF - m_ccrad;
	}
	//SetDlgItemInt(IDC_CCARG2,m_ccarg2);
	UpdateData(FALSE);
	OnChangeArgs();
}

void CCLineEditor::OnStraight() 
{
	// TODO: Add your control notification handler code here
	m_ccrad = 0;
	m_ccarg2 = 0;
	UpdateData(FALSE);
	OnChangeArgs();
}

void CCLineEditor::OnRight() 
{
	// TODO: Add your control notification handler code here
	if (m_ccarg2<0)
	{
		m_ccarg2 = (-m_ccarg2)-1;
		m_ccrad = 0xFFFF - m_ccrad;
	}
	//SetDlgItemInt(IDC_CCARG2,m_ccarg2);
	UpdateData(FALSE);
	OnChangeArgs();
}

void CCLineEditor::OnSetfocusCcrad() 
{
	//radBoxhasFocus = TRUE;
}

void CCLineEditor::OnKillfocusCcrad() 
{
	//radBoxhasFocus = FALSE;
}

void CCLineEditor::OnHelp() 
{
	CString url = "tutu.htm";
	url += "#cclineedit";
	theApp.OnHelpContentString(url);
}
