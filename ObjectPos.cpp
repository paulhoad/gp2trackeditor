// ObjectPos.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "ObjectPos.h"
#include "ObjectEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CObjectPos dialog


CObjectPos::CObjectPos(GPTrack *track,CWnd* pParent /*=NULL*/)
	: CDialog(CObjectPos::IDD, pParent),track(track)
{
	//{{AFX_DATA_INIT(CObjectPos)
	m_Distance = 0;
	m_Desc = _T("");
	//}}AFX_DATA_INIT
}


void CObjectPos::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectPos)
	DDX_Control(pDX, IDC_SPIN1, m_PositionSpin);
	DDX_Control(pDX, IDC_LOCATOR, m_ObjectCombo);
	DDX_Text(pDX, IDC_DISTANCE, m_Distance);
	DDX_Text(pDX, IDC_DESC, m_Desc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CObjectPos, CDialog)
	//{{AFX_MSG_MAP(CObjectPos)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_NEWDESC, OnNewdesc)
	ON_CBN_SELCHANGE(IDC_LOCATOR, OnSelchangeLocator)
	ON_BN_CLICKED(IDC_OBJHELP, OnObjhelp)
	ON_CBN_EDITUPDATE(IDC_LOCATOR, OnEditupdateLocator)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectPos message handlers

void CObjectPos::OnEdit() 
{
	// TODO: Add your control notification handler code here
	CObjectEdit *trkdlg = new CObjectEdit(track,this);
	int idx = m_ObjectCombo.GetCurSel();
	TrackObjectDefinition *tobj = (TrackObjectDefinition*)track->TrackObjectDefintions->elementAt(idx);
		   


	TrackObjectDefinition *t = track->getObjectDefinition(tobj->getLocator());

	if (trkdlg && t)
	  trkdlg->setSection(t);

	int result = trkdlg->DoModal();

    if (result!=IDCANCEL && trkdlg && t)
	{	  
	  trkdlg->getSection(t);
	  //t->write(track,t->originaloffset,track->ListObjectOffset);
	  //t->write(track,t->originaloffset);
	  track->regenerate=TRUE;
	}

}

BOOL CObjectPos::OnInitDialog() 
{
	CDialog::OnInitDialog();
	char buffer[7];
	
	// TODO: Add extra initialization here
	for(int i=0;i<track->TrackObjectDefintions->size();i++)
	{
	  TrackObjectDefinition *tobj = (TrackObjectDefinition*)track->TrackObjectDefintions->elementAt(i);
		   
	  wsprintf(buffer,"%d",tobj->getLocator());
	  m_ObjectCombo.AddString(buffer);
	}
	wsprintf(buffer,"%d",locatorStart);
	m_ObjectCombo.SelectString(-1,buffer);

	m_PositionSpin.SetRange(-5000,5000);

	OnSelchangeLocator();

	//m_BalloonToolTip.AddTool(GetDlgItem(IDC_LOCATOR),"Choose the object you require!");
	//m_BalloonToolTip.AddTool(GetDlgItem(IDC_DISTANCE),"This is the distance into the track sector");

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CObjectPos::OnOK() 
{
	// TODO: Add extra validation here
	int idx = m_ObjectCombo.GetCurSel();
	TrackObjectDefinition *tobj = (TrackObjectDefinition*)track->TrackObjectDefintions->elementAt(idx);
		   
	locatorStart = tobj->getLocator();	

	
	
	CDialog::OnOK();
}

void CObjectPos::OnNewdesc() 
{
	// TODO: Add your control notification handler code here
	TrackObjectDefinition *t = track->AddNewTrackObjectDefinition();

	CObjectEdit *trkdlg = new CObjectEdit(track,this);
	
	if (trkdlg && t)
	  trkdlg->setSection(t);

	int res = trkdlg->DoModal();

	if (trkdlg && t && res!=IDCANCEL)
	{	  
	  trkdlg->getSection(t);
	  track->regenerate=TRUE;
	  CString buffer;

	  m_ObjectCombo.ResetContent();
	  for(int i=0;i<track->TrackObjectDefintions->size();i++)
	  {
	    TrackObjectDefinition *tobj = (TrackObjectDefinition*)track->TrackObjectDefintions->elementAt(i);
	    buffer.Format("%d",tobj->getLocator());
	    m_ObjectCombo.AddString(buffer);
	  }
	  buffer.Format("%d",t->getLocator());
	  m_ObjectCombo.SelectString(-1,buffer);
	  OnSelchangeLocator();
	}

	if (trkdlg) delete trkdlg;

}

void CObjectPos::OnSelchangeLocator() 
{
	int idx = m_ObjectCombo.GetCurSel();
	TrackObjectDefinition *tobj = (TrackObjectDefinition*)track->TrackObjectDefintions->elementAt(idx);
	if (tobj)
	{
		 m_Desc.Format("Internal Object %d = %s",tobj->id, tobj->getName());
		 SetDlgItemText(IDC_DESC,m_Desc);
	}
	//UpdateData(FALSE);
}

BOOL CObjectPos::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::PreTranslateMessage(pMsg);
}

void CObjectPos::OnObjhelp() 
{
	CString url = "object.htm";
	
	url+="#Input_objects";
	theApp.OnHelpContentString(url);
}

void CObjectPos::OnEditupdateLocator() 
{
	// TODO: Add your control notification handler code here
	
}
