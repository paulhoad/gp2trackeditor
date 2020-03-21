// SceneryEditor.cpp : implementation file
//

#include "stdafx.h"
#include "trackeditor.h"
#include "SceneryEditor.h"
#include "TrackSection.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSceneryEditor dialog


CSceneryEditor::CSceneryEditor(GPTrack *mytrack,BOOL pitlane,int StartingPosition,int StartingCmd,CWnd* pParent /*=NULL*/)
	: CDialog(CSceneryEditor::IDD, pParent),track(mytrack),pitlane(pitlane)
{
	//{{AFX_DATA_INIT(CSceneryEditor)
	m_RibbonsRight = 0;
	m_NumCmds = _T("");
	m_EditingCmds = _T("");
	m_ShowHidden = FALSE;
	//}}AFX_DATA_INIT

	m_Index = StartingPosition;
	currentCmd = NULL;
	edited = FALSE;
	Editing = StartingCmd;

	if (!pitlane)
	  currentSection = (TrackSection *)track->TrackSections->elementAt(m_Index);
	else
	  currentSection = (TrackSection *)track->PitlaneSections->elementAt(m_Index);
	
	bluePen = new CPen(PS_SOLID,1,0x0000FF);
	redBrush = new CBrush(0x0000FF);
}

CSceneryEditor::~CSceneryEditor()
{
	delete bluePen;
	delete redBrush;
}

int  IntToRibbon(int val)
{
	if (val==0) return 32;
	if (val==1) return 1;
	if (val==2) return 2;
	if (val==3) return 4;
	if (val==4) return 8;
	if (val==5) return 16;
    return 0;
}

int  IntToRibbon2(int val)
{
	if (val==5) return 2;
	if (val==0) return 4;
	if (val==1) return 8;
	if (val==2) return 16;
	if (val==3) return 32;
	if (val==4) return 1;
    return 0;
}




void CSceneryEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSceneryEditor)
	DDX_Text(pDX, IDC_RIBBONS_RIGHT, m_RibbonsRight);
	DDX_Text(pDX, IDC_NUM_SCENERY_CMDS, m_NumCmds);
	DDX_Text(pDX, IDC_EDITINGCOMMAND, m_EditingCmds);
	
	DDX_Check(pDX, IDC_CHECK1, m_ShowHidden);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSceneryEditor, CDialog)
	//{{AFX_MSG_MAP(CSceneryEditor)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_BN_CLICKED(IDC_PREV, OnPrev)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_EN_CHANGE(IDC_X1, OnChangeX1)
	ON_EN_CHANGE(IDC_X2, OnChangeX2)
	ON_EN_CHANGE(IDC_X3, OnChangeX3)
	ON_EN_CHANGE(IDC_X4, OnChangeX4)
	ON_EN_CHANGE(IDC_X5, OnChangeX5)
	ON_EN_CHANGE(IDC_X6, OnChangeX6)
	ON_EN_CHANGE(IDC_Z1, OnChangeZ1)
	ON_EN_CHANGE(IDC_Z2, OnChangeZ2)
	ON_EN_CHANGE(IDC_Z3, OnChangeZ3)
	ON_EN_CHANGE(IDC_Z4, OnChangeZ4)
	ON_EN_CHANGE(IDC_Z5, OnChangeZ5)
	ON_EN_CHANGE(IDC_Z6, OnChangeZ6)
	ON_EN_CHANGE(IDC_RIBBONS_RIGHT, OnChangeRibbonsRight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSceneryEditor message handlers

void CSceneryEditor::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDCtl == IDC_DRAW)
	{
	    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
        CRect rcItem(lpDrawItemStruct->rcItem);
        
        // Save dc state
        int nSavedDC = pDC->SaveDC();
		
		pDC->FillRect(rcItem, &CBrush(::GetSysColor(COLOR_WINDOW)));
		pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));

		DrawScenery(pDC,rcItem);

		// Restore dc
        pDC->RestoreDC( nSavedDC );
	}
	else if (nIDCtl == IDC_HEIGHTDRAW)
	{
		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
        CRect rcItem(lpDrawItemStruct->rcItem);
        
        // Save dc state
        int nSavedDC = pDC->SaveDC();
		
		pDC->FillRect(rcItem, &CBrush(::GetSysColor(COLOR_WINDOW)));
		pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));

		DrawProfile(pDC,rcItem);

		// Restore dc
        pDC->RestoreDC( nSavedDC );
	}
	else if (nIDCtl == IDC_RIBBONSTATE)
	{
		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
        CRect rcItem(lpDrawItemStruct->rcItem);
        
        // Save dc state
        int nSavedDC = pDC->SaveDC();
		
		pDC->FillRect(rcItem, &CBrush(::GetSysColor(COLOR_WINDOW)));
		pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));

		DrawRibbonState(pDC,rcItem);

		// Restore dc
        pDC->RestoreDC( nSavedDC );
	}
	
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

BOOL CSceneryEditor::PreCreateWindow(CREATESTRUCT& cs) 
{
	return CDialog::PreCreateWindow(cs);
}

static void drawPoint(CDC *g,short x1,short y1)
{
	int size = 2;

	g->Ellipse(x1-size,y1-size,x1+size,y1+size);
	g->MoveTo(x1,y1-size);
	g->LineTo(x1,y1+size);
	g->MoveTo(x1-size,y1);
	g->LineTo(x1+size,y1);

	

}

void CSceneryEditor::UpdateDialogBoxes()
{
	int B8size = currentSection->numCommands(0xb8);
	m_NumCmds.Format("%d",B8size);
	SetDlgItemText(IDC_NUM_SCENERY_CMDS,m_NumCmds);
	m_EditingCmds.Format("%d",Editing);
	SetDlgItemText(IDC_EDITINGCOMMAND,m_EditingCmds);


		TrackCmd *scenery = currentSection->NthCommandOccurance(0xb8,Editing);
		if (scenery)
		{
			currentCmd = scenery;
			
			short u2 = scenery->getNthArg(2);
	         x[0] = scenery->getNthArg(3);
             z[0] = scenery->getNthArg(4);
			 x[1] = scenery->getNthArg(5);
             z[1] = scenery->getNthArg(6);
			 x[2] = scenery->getNthArg(7);
             z[2] = scenery->getNthArg(8);
			 x[3] = scenery->getNthArg(9);
             z[3] = scenery->getNthArg(10);
			 x[4] = scenery->getNthArg(11);
             z[4] = scenery->getNthArg(12);
			 x[5] = scenery->getNthArg(13);
			 z[5] = scenery->getNthArg(14);

			 SetDlgItemInt(IDC_RIBBONS_RIGHT,u2);

			 SetDlgItemInt(IDC_X1,x[0]);
			 SetDlgItemInt(IDC_X2,x[1]);
			 SetDlgItemInt(IDC_X3,x[2]);
			 SetDlgItemInt(IDC_X4,x[3]);
			 SetDlgItemInt(IDC_X5,x[4]);
			 SetDlgItemInt(IDC_X6,x[5]);

			 SetDlgItemInt(IDC_Z1,z[0]);
			 SetDlgItemInt(IDC_Z2,z[1]);
			 SetDlgItemInt(IDC_Z3,z[2]);
			 SetDlgItemInt(IDC_Z4,z[3]);
			 SetDlgItemInt(IDC_Z5,z[4]);
			 SetDlgItemInt(IDC_Z6,z[5]);
	}
	GetDlgItem(IDC_DRAW)->InvalidateRect(NULL);
	GetDlgItem(IDC_HEIGHTDRAW)->InvalidateRect(NULL);
}

void CSceneryEditor::DrawScenery(CDC* pDC ,RECT &rect)
{
	 int top    = rect.top;
	 int bottom = rect.bottom-4;
	 int left   = rect.left;
	 int right  = rect.right;
	 int midx   = ((left+right)/2);
	 int midy   = ((top+bottom)/2);



	 

	 pDC->SelectStockObject(BLACK_PEN);

	 if (currentSection && currentSection->containsCommand(0xb8))
	 {
	  pDC->MoveTo(left,bottom);
	  pDC->LineTo(right,bottom);
	  pDC->MoveTo(midx,bottom);
	  pDC->LineTo(midx,top);
	  BOOL found=FALSE;

	  TrackSection *prev = currentSection->track->GetPreviousSection(currentSection);
	  int leftWall  =  prev->getLeftToBank()*0x40;
	  int rightWall =  prev->getRightToBank()*0x40;

	  double trackLeft  =  currentSection->getWidthLeft();
	  double trackRight =  currentSection->getWidthRight();



	  		if (currentCmd)
			{
			TrackCmd *scenery = currentCmd;


			

			short u2 = scenery->getNthArg(2);
	         x[0] = scenery->getNthArg(3);
             z[0] = scenery->getNthArg(4);
			 x[1] = scenery->getNthArg(5);
             z[1] = scenery->getNthArg(6);
			 x[2] = scenery->getNthArg(7);
             z[2] = scenery->getNthArg(8);
			 x[3] = scenery->getNthArg(9);
             z[3] = scenery->getNthArg(10);
			 x[4] = scenery->getNthArg(11);
             z[4] = scenery->getNthArg(12);
			 x[5] = scenery->getNthArg(13);
			 z[5] = scenery->getNthArg(14);


			 
			 edited = FALSE;
	     
			double minX = -10000;
			double detX = 20000;
			double detZ = 18000;
			double minZ = -2000;

			double scaleX=0;
			double scaleZ=0;
			
			if (detX!=0)
			   scaleX = (right-left)/(detX);

			if (detZ!=0)
			   scaleZ = (top-bottom)/(detZ);

			short ldx=0;
			short ldy=0;

			for(int i=0;i<6;i++)
			{
			  

			  short dx1 = (short)(((x[i]-minX)*scaleX));
			  short dy1 = (short)(bottom + (z[i]-minZ)*scaleZ);			

			  pDC->SelectObject(redBrush);
			  //BOOL drawIt = (m_ShowHidden || track->isRibbonOn(currentSection->getIndex(),IntToRibbon2(i)));
					

			  drawPoint(pDC,dx1,dy1);
			  pDC->SelectStockObject(BLACK_PEN);

			  if (i==((6-u2)-1))
			  {
				  double wL = (leftWall + trackLeft)/8;
				  double wR = (rightWall + trackRight)/8;
				  //double alt = currentSection->getTrueAltitude();
				  double alt = 0;
				  
				  dx1 = (short)(((-wL-minX)*scaleX));
			      dy1 = (short)(bottom + (alt-minZ)*scaleZ);
				  pDC->MoveTo(ldx,ldy);
				  pDC->LineTo(dx1,dy1);
				  pDC->LineTo(dx1,dy1-10);
				  pDC->MoveTo(dx1,dy1);
				  dx1 = (short)(((wR-minX)*scaleX));
			      dy1 = (short)(bottom + (alt-minZ)*scaleZ);
				  pDC->LineTo(dx1,dy1);
				  pDC->LineTo(dx1,dy1-10);
				  pDC->MoveTo(dx1,dy1);
				  dx1 = (short)(((x[i]-minX)*scaleX));
			      dy1 = (short)(bottom + (z[i]-minZ)*scaleZ);
				  pDC->LineTo(dx1,dy1);
			  }
			  else if (i!=0)
			  {
				  pDC->MoveTo(ldx,ldy);
				  pDC->LineTo(dx1,dy1);
			  }	

			  ldx = dx1;
			  ldy = dy1;
			}
		}
	  //}
	 }
	 else
	 {
	  pDC->TextOut(midx,midy,"No Scenery Cmds");
	 }

}

void CSceneryEditor::OnNext() 
{
	if (currentSection)
	{
	   int B8size = currentSection->numCommands(0xb8);

	   if (currentSection && (Editing >= B8size))
	   {
	      currentSection = track->GetNextSection(currentSection);
		  Editing = 1;
	   }
	   else {
		   Editing++;
	   }

	m_Index = track->GetTrackSectionIndex(currentSection);

	SetDlgItemInt(IDC_INDEX,m_Index);

	if (currentSection && currentSection->containsCommand(0xb8))
	{
	 GetDlgItem(IDC_DRAW)->InvalidateRect(NULL);
	 GetDlgItem(IDC_HEIGHTDRAW)->InvalidateRect(NULL);
	}

	}
	UpdateDialogBoxes();
}

void CSceneryEditor::OnPrev() 
{
	if (currentSection)
	{
	 int B8size = currentSection->numCommands(0xb8);

	 if (currentSection && (Editing <= 1))
	 {
	      currentSection = track->GetPreviousSection(currentSection);
		  Editing = currentSection->numCommands(0xb8);
	 }
	 else {
		   Editing--;
	 }
		

	
	 m_Index = track->GetTrackSectionIndex(currentSection);

	 SetDlgItemInt(IDC_INDEX,m_Index);
	 if (currentSection && currentSection->containsCommand(0xb8))
	 {
	  GetDlgItem(IDC_DRAW)->InvalidateRect(NULL);
	  GetDlgItem(IDC_HEIGHTDRAW)->InvalidateRect(NULL);
	 }
	}
	UpdateDialogBoxes();
	
}

BOOL CSceneryEditor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (currentSection)
	{
	 m_Index = track->GetTrackSectionIndex(currentSection);

	 SetDlgItemInt(IDC_INDEX,m_Index);
	 UpdateDialogBoxes();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSceneryEditor::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	CWnd *wnd= GetDlgItem(IDC_DRAW);

	int midy = cy/2;

	

	if (wnd!=NULL)
	{
	  RECT rect;
	  wnd->GetClientRect(&rect);

	  int height2 = midy-50;
	  int height1 = midy-120;
	  int width   = cx-30;


	  wnd->SetWindowPos(NULL,10,midy-10-height1,width,height1,SWP_NOZORDER);
	  wnd = GetDlgItem(IDC_HEIGHTDRAW); if (wnd) wnd->SetWindowPos(NULL,10,midy+10,width,height2,SWP_NOZORDER);

	  wnd = GetDlgItem(IDC_PROFILE)    ; if (wnd) wnd->SetWindowPos(NULL,15,midy-5,0,0,SWP_NOSIZE | SWP_NOZORDER);
	  
	  wnd = GetDlgItem(IDOK)    ; if (wnd) wnd->SetWindowPos(NULL,15,cy-30,0,0,SWP_NOSIZE | SWP_NOZORDER);
	  wnd = GetDlgItem(IDCANCEL); if (wnd) wnd->SetWindowPos(NULL,15+80+20,cy-30,0,0,SWP_NOSIZE | SWP_NOZORDER);
	  wnd = GetDlgItem(IDC_PREV); if (wnd) wnd->SetWindowPos(NULL,cx-15-80-80-20,cy-30,0,0,SWP_NOSIZE | SWP_NOZORDER);
	  wnd = GetDlgItem(IDC_NEXT); if (wnd) wnd->SetWindowPos(NULL,cx-15-80,cy-30,0,0,SWP_NOSIZE | SWP_NOZORDER);
	}

	
}


void CSceneryEditor::DrawProfile(CDC* pDC ,RECT &rect)
{
	 int top    = rect.top;
	 int bottom = rect.bottom-4;
	 int left   = rect.left;
	 int right  = rect.right;
	 int midx   = ((left+right)/2);
	 int midy   = ((top+bottom)/2);
	 int size = 0;

	 if (!pitlane)
	    size = track->TrackSections->size();
	 else
		size = track->PitlaneSections->size();

	 int psize = track->PitlaneSections->size();

	 double total  = track->totalLength();
	 if (pitlane)
	   total = track->totalPitLength();
	 
	 double maxAlt = 10;
	 double minAlt = 0;
	 for(int i=0;i<size;i++)
	 {
	   TrackSection *t = NULL;

	   if (!pitlane)
	     t = (TrackSection*)track->TrackSections->elementAt(i);
	   else
		 t = (TrackSection*)track->PitlaneSections->elementAt(i);
	   maxAlt = max(maxAlt,t->getTrueAltitude());
	   minAlt = min(minAlt,t->getTrueAltitude());
	 }

	 //for(i=0;i<psize;i++)
	 //{
	 //  TrackSection *t = (TrackSection*)track->PitlaneSections->elementAt(i);
	 //  maxAlt = max(maxAlt,t->getTrueAltitude());
	 //  minAlt = min(minAlt,t->getTrueAltitude());
	 //}

	 double MaxMinAlt = max(fabs(minAlt),fabs(maxAlt));
	 double totaly = (1.1*(MaxMinAlt*2));

	 double singleUnit = ((double)(right-left))/total;
	 double singleUnity = ((double)(bottom-top))/totaly;

	 

	 pDC->SelectStockObject(BLACK_PEN);
	 pDC->SelectStockObject(LTGRAY_BRUSH);

	 double xpos = left;
	 double ypos = midy;

	 pDC->MoveTo(left,midy);
	 pDC->LineTo(right,midy);

	 
	 double pitentryx = left;
	 double pitentryy = midy;
	 double pitexitx  = left;
	 double pitexity  = midy;
	 

	 POINT pts[4];

	 for(i=0;i<size;i++)
	 {
		 pDC->SelectStockObject(BLACK_PEN);
		 pDC->SelectStockObject(LTGRAY_BRUSH);
		 TrackSection *t = NULL;

		 if (!pitlane)
			 t = (TrackSection*)track->TrackSections->elementAt(i);
		 else
		     t = (TrackSection*)track->PitlaneSections->elementAt(i);

		 double len = t->getLength();

		 double x1 = len*singleUnit;
		 //drawing is upside down
		 double y1 = -1.0*(singleUnity*t->getTrueAltitude());

		 /*
		 if (i == track->PitLaneEntryIndex)
		 {
			 pitentryx = xpos;
			 pitentryy = ypos;
		 }
		 if (i == track->PitLaneExitIndex)
		 {
			 pitexitx = xpos;
			 pitexity = ypos;
		 }
		 */

		 if (t->containsCommand(0x86)){
			pitexitx = xpos;
		 }

         pts[0].x = (int) xpos;
		 pts[0].y = (int) midy;		 
		 pts[1].x = (int) xpos;
		 pts[1].y = (int) ypos;

		 if (t == currentSection)
		 {
			 pDC->SelectObject(redBrush);
			 pDC->MoveTo((int)xpos,top);
			 pDC->LineTo((int)xpos,bottom);
		 }

		 if ((i%10)==0)
		 {
			 char buff[10];

			 wsprintf(buff,"%d",i);
			 pDC->MoveTo((int)xpos,bottom-10);
			 pDC->LineTo((int)xpos,bottom-30);
			 pDC->TextOut((int)xpos-5,bottom-10,buff);
		 }
		 else
		 {
			 pDC->MoveTo((int)xpos,bottom-10);
			 pDC->LineTo((int)xpos,bottom-20);
		 }

		 pDC->MoveTo((int)xpos,(int)(ypos));
		 xpos+=x1;
		 ypos=midy+y1;

		 pts[2].x = (int) xpos;
		 pts[2].y = (int) ypos;

		 pDC->LineTo((int)xpos,(int)ypos);

		 //drawPoint(pDC,(int)xpos,(int)ypos);
		 pDC->LineTo((int)xpos,midy);

		 pts[3].x = (int) xpos;
		 pts[3].y = (int) midy;
		 pDC->Polygon(pts,4);

		 if (t->getAngle()!=0)
		 {
			 pDC->SelectObject(bluePen);

		 }
		 else
		 {
			 pDC->SelectStockObject(BLACK_PEN);

		 }


		 pDC->MoveTo(pts[0].x,bottom-15);
		 pDC->LineTo((int)xpos,bottom-15);
		 pDC->SelectStockObject(BLACK_PEN);
		


	 }


	 
	 xpos = pitentryx;
	 

	 if (!pitlane)
	 {
	 CPen pen(PS_SOLID,1,RGB(0,0,0xFF));
	 for(i=0;i<psize;i++)
	 {
		 pDC->SelectObject(pen);
		 pDC->SelectStockObject(NULL_BRUSH);
		 TrackSection *t = (TrackSection*)track->PitlaneSections->elementAt(i);

		 double len = t->getLength();

		 double x1 = len*singleUnit;
		 //drawing is upside down
		 double y1 = -1*(singleUnity*t->getTrueAltitude());

		 pts[0].x = (int) xpos;
		 pts[0].y = (int) midy;		 
		 pts[1].x = (int) xpos;
		 pts[1].y = (int) ypos;

		 if (t == currentSection)
		 {
			 pDC->SelectObject(redBrush);
			 pDC->MoveTo((int)xpos,top);
			 pDC->LineTo((int)xpos,bottom);
		 }

		 pDC->MoveTo((int)xpos,(int)(ypos));
		 xpos+=x1; 
		 ypos=midy+y1;

		 pts[2].x = (int) xpos;
		 pts[2].y = (int) ypos;

		 pDC->LineTo((int)xpos,(int)ypos);

		 //drawPoint(pDC,(int)xpos,(int)ypos);
		 pDC->LineTo((int)xpos,midy);

		 pts[3].x = (int) xpos;
		 pts[3].y = (int) midy;
		 pDC->Polygon(pts,4);

		 if (xpos > right)
		 {
			 xpos -= (right-left);
		 }
	 }
	 }
}




BOOL CSceneryEditor::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nID == IDC_DRAW)
	{
	   if (nCode == WM_LBUTTONDOWN) 
	   {
		   AfxMessageBox("Draw LButton Down");
	   }
	}
	
	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CSceneryEditor::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{

	return CDialog::OnChildNotify(message, wParam, lParam, pLResult);
}

LRESULT CSceneryEditor::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CSceneryEditor::OnApply() 
{
	// TODO: Add your control notification handler code here
	
}

#define UPDATE_SCX(a) if (currentCmd!=NULL) currentCmd->setNthArg((2*a)+1,GetDlgItemInt(IDC_X##a));edited=TRUE;GetDlgItem(IDC_DRAW)->InvalidateRect(NULL);
#define UPDATE_SCZ(a) if (currentCmd!=NULL) currentCmd->setNthArg((2*a)+2,GetDlgItemInt(IDC_Z##a));edited=TRUE;GetDlgItem(IDC_DRAW)->InvalidateRect(NULL);

void CSceneryEditor::OnChangeX1() 
{
	UPDATE_SCX(1)
}

void CSceneryEditor::OnChangeX2() 
{
	UPDATE_SCX(2)	
}

void CSceneryEditor::OnChangeX3() 
{
	UPDATE_SCX(3)	
}

void CSceneryEditor::OnChangeX4() 
{
	UPDATE_SCX(4)	
}

void CSceneryEditor::OnChangeX5() 
{
	UPDATE_SCX(5)	
}

void CSceneryEditor::OnChangeX6() 
{
	UPDATE_SCX(6)	
}

void CSceneryEditor::OnChangeZ1() 
{
	UPDATE_SCZ(1)	
}

void CSceneryEditor::OnChangeZ2() 
{
	UPDATE_SCZ(2)	
}

void CSceneryEditor::OnChangeZ3() 
{
	UPDATE_SCZ(3)	
}

void CSceneryEditor::OnChangeZ4() 
{
	UPDATE_SCZ(4)	
}

void CSceneryEditor::OnChangeZ5() 
{
	UPDATE_SCZ(5)		
}

void CSceneryEditor::OnChangeZ6() 
{
	UPDATE_SCZ(6)		
}

void CSceneryEditor::OnChangeRibbonsRight() 
{
	if (currentCmd!=NULL) 
		currentCmd->setNthArg(2,GetDlgItemInt(IDC_RIBBONS_RIGHT));
	edited=TRUE;
	GetDlgItem(IDC_DRAW)->InvalidateRect(NULL);
}





void CSceneryEditor::DrawRibbonState(CDC* pDC ,RECT &rect)
{
	 int top    = rect.top;
	 int bottom = rect.bottom-4;
	 int left   = rect.left;
	 int right  = rect.right;
	 int midx   = ((left+right)/2);
	 int midy   = ((top+bottom)/2);

	 int size = 0;
		 
	 if (pitlane)
		 size = track->PitlaneSections->size();
	 else
		 size = track->TrackSections->size();

	 CBrush *brush[6];

	 brush[0] = new CBrush(RGB(0xFF,0,0));
	 brush[1] = new CBrush(RGB(0,0xFF,0));	 
	 brush[2] = new CBrush(RGB(0xFF,0xFF,0));
	 brush[3] = new CBrush(RGB(0xFF,0,0xFF));
	 brush[4] = new CBrush(RGB(0,0,0xFF));
	 brush[5] = new CBrush(RGB(0,0xFF,0xFF));
	 


	 double total  = 0;
	 
	 
	 if (pitlane)
		 total = track->totalPitLength();
	 else
		 total = track->totalLength();
	 
	 pDC->SelectStockObject(BLACK_PEN);
	 pDC->SelectStockObject(LTGRAY_BRUSH);

	 

	 //POINT pts[4];

	 double singleUnit = ((double)(right-left))/total;

	 BOOL ribbon=FALSE;
	 for(int j=0;j<6;j++)
	 {
     double xpos = left;
	 double ypos = j*(bottom-top)/6;
	 double ypos1 = (j+1)*(bottom-top)/6;
	 for(int i=0;i<size;i++)
	 {
		 TrackSection *t=NULL;
		 if (!pitlane)
		    t = (TrackSection*)track->TrackSections->elementAt(i);
		 else
			t = (TrackSection*)track->PitlaneSections->elementAt(i);

		 Vector *cmds = t->getCommands();
		 for(int k=0;k<cmds->size();k++)
		 {
			 TrackCmd *cmd = (TrackCmd *)cmds->elementAt(k);
			 switch(cmd->getCmd() )
			 {
			   case 0xD9:
			   case 0xB9:
			   {
				   int value = cmd->getNthArg(2);
				   int myribbon = IntToRibbon(j);
				   if (value & myribbon)
					   ribbon = TRUE;
			   } break;
			   case 0xB0:
			   {
				   int value = cmd->getNthArg(2);
				   int myribbon = IntToRibbon(j);
				   if (value & myribbon)
					   ribbon = FALSE;
			   } break;
			 }
		 }

		 pDC->SelectStockObject(NULL_PEN);
		 if (ribbon) pDC->SelectObject(brush[j]);
		 else        pDC->SelectStockObject(LTGRAY_BRUSH);

		 double len = t->getLength();

		 double x1 = len*singleUnit;

		
		 pDC->Rectangle((int)xpos,(int)ypos,(int)(xpos+x1),(int)(ypos1));

		 /*
		 pDC->SelectStockObject(BLACK_PEN);
		 if ((i%10)==0)
		 {
			 char buff[10];

			 wsprintf(buff,"%d",i);
			 pDC->MoveTo((int)xpos,bottom-10);
			 pDC->LineTo((int)xpos,bottom-30);
			 pDC->TextOut((int)xpos-5,bottom-10,buff);
		 }
		 else
		 {
			 pDC->MoveTo((int)xpos,bottom-10);
			 pDC->LineTo((int)xpos,bottom-20);
		 }
		 */

		 xpos+=x1;
		 //ypos=midy+y1;

		 

		/*
		 pDC->MoveTo(pts[0].x,bottom-15);
		 pDC->LineTo((int)xpos,bottom-15);
		 pDC->SelectStockObject(BLACK_PEN);
		 */
	 }
	 }
}
