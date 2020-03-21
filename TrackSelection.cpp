// TrackSelection.cpp : implementation file
//

#include "stdafx.h"
#include "stdlib.h"
#include "io.h"
#include "TrackEditor.h"
#include "TrackSelection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void OpenSeasonSetFile(LPCSTR filename);

/////////////////////////////////////////////////////////////////////////////
// CTrackSelection dialog

static CString GP2Location;


CTrackSelection::CTrackSelection(CWnd* pParent /*=NULL*/)
	: CDialog(CTrackSelection::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrackSelection)
	m_CurrentDir = _T("");
	//}}AFX_DATA_INIT
}


void CTrackSelection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrackSelection)
	DDX_Control(pDX, IDC_TRACK_ALL_LIST, m_AllTracks);
	DDX_Control(pDX, IDC_TRACK_SELECTED_LIST, m_SelectedTracks);
	DDX_Text(pDX, IDC_CURRENTDIR, m_CurrentDir);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrackSelection, CDialog)
	//{{AFX_MSG_MAP(CTrackSelection)
	ON_BN_CLICKED(IDC_TRACK_ADD, OnTrackAdd)
	ON_BN_CLICKED(IDC_TRACK_REMOVE, OnTrackRemove)
	ON_BN_CLICKED(IDC_LOCATE, OnLocate)
	ON_WM_LBUTTONDBLCLK()
	ON_LBN_DBLCLK(IDC_TRACK_ALL_LIST, OnDblclkTrackAllList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackSelection message handlers

void CTrackSelection::OnTrackAdd() 
{
	BOOL AddSeasonSetFile=FALSE;
	// TODO: Add your control notification handler code here
	CListBox *list    = (CListBox*)GetDlgItem(IDC_TRACK_SELECTED_LIST);
	CListBox *alllist = (CListBox*)GetDlgItem(IDC_TRACK_ALL_LIST);

	int inStallTo   = LB_ERR;
	int inStallFrom = LB_ERR;

	if (list)
	   inStallTo   = list->GetCurSel();
	
	if (alllist)
	{
	   inStallFrom = alllist->GetCurSel();

	   if (inStallFrom != LB_ERR)
	   {
		 if (inStallTo == LB_ERR)
		 {
			 AddSeasonSetFile=TRUE;
		 }
		 else 
		 {
	       LPCSTR newFile      = (LPCSTR)alllist->GetItemData(inStallFrom);
		   // if newFile extension equals .set then set file
		   AddSeasonSetFile=FALSE;
		 }
		 
	   }
	}

	BOOL error = FALSE;

	if (inStallTo == LB_ERR && !AddSeasonSetFile)
	{
		AfxMessageBox("You Must Select a Destination Location");
		error = TRUE;
	}

	if (inStallFrom == LB_ERR)
	{
		AfxMessageBox("You Must Select a Source Track to install");
		error = TRUE;
	}

	if (error) 
	{
		AfxMessageBox("Track Installation Aborted");
		return;
	}

		
	//lets back up the original track
	//if orig does not exist then create a copy of it
	LPCSTR originalFile = (LPCSTR)list->GetItemData(inStallTo);
	LPCSTR newFile      = (LPCSTR)alllist->GetItemData(inStallFrom);

	if (originalFile==NULL)
		AfxMessageBox("No To File");

	if (newFile==NULL)
		AfxMessageBox("No From File");


	if (AddSeasonSetFile)
	{
		OpenSeasonSetFile(newFile);
		UpdateLists();
		return;
	}



	//char buffer[256];
	//wsprintf(buffer,"Copying track %d to track %d please confirm?",inStallFrom,inStallTo);
    //int result = AfxMessageBox(buffer,MB_YESNO);
	

	if (originalFile && newFile)
	{
	 char buffer[256];
	 wsprintf(buffer,"Copying track %s to track %s please confirm?",newFile,originalFile);
	 int result = AfxMessageBox(buffer,MB_YESNO);
	 if (result == IDNO) return;

	 char originalFileOrig[256];
	 char originalFileCopy[256];

	 int ch = '.';

	 strcpy(originalFileCopy,originalFile);
	 char *dotidx = strrchr(originalFileCopy,ch);

	 int posidx = (dotidx-originalFileCopy);
	 originalFileCopy[posidx] = '\0';

	 wsprintf(originalFileOrig,"%s.org",originalFileCopy);
	
	
	 //wsprintf(buffer,"Copying track %s to track %s please confirm?",originalFile,originalFileOrig);
	 //result = AfxMessageBox(buffer,MB_YESNO);
	 //if (result == IDNO) return;

	 if (CopyFile(originalFile,originalFileOrig,TRUE)==FALSE)
	 {
		//result = AfxMessageBox("A Backup Copy of the original file already exists to you want to overwrite the backup",MB_YESNO);
		//if (result == IDYES)
		//{
			CopyFile(originalFile,originalFileOrig,FALSE);
		//}
		//else
		//	return;
	 }
	 //copy track file made by item to new trackfile location
	 //result = AfxMessageBox("About to Install Track Are you still sure?",MB_YESNO);
	 //if (result == IDNO) return;

	 CopyFile(newFile,originalFile,FALSE);	

	 UpdateLists();
	}
	else
	{
		AfxMessageBox("Cannot find file names");
	}
}

void CTrackSelection::OnTrackRemove() 
{
	// TODO: Add your control notification handler code here
}

#include "Shlobj.h"

void CTrackSelection::OnLocate() 
{
	char fileName[256];

	strcpy(fileName,"*.*");
	
	BROWSEINFO browser;
	char buffer[MAX_PATH];

	browser.hwndOwner = m_hWnd;
	browser.pszDisplayName = buffer;
	browser.lpszTitle = "Select GP2 Location";
	browser.lpfn = NULL;
	browser.pidlRoot = NULL;
	browser.ulFlags = BIF_RETURNONLYFSDIRS;

	LPITEMIDLIST files;


	files = SHBrowseForFolder(&browser);

   
	if (files !=NULL)
	{
		char filename[256];

		if (SHGetPathFromIDList(files,filename))
		{
		 CString strSection       = "Preferences";
         CString strStringItem    = "GP2Location";
         CWinApp* pApp = AfxGetApp();

		 pApp->WriteProfileString(strSection, strStringItem, CString(filename));

		 SetDlgItemText(IDC_GP2LOCATION,filename);
		}
		else
		{
	     AfxMessageBox("Failed to Locate GP2 directory");
		}
   	}
	else
	{
		AfxMessageBox("Failed to find GP2 directory");
	}
	UpdateLists();
}

BOOL CTrackSelection::OnInitDialog() 
{
	CDialog::OnInitDialog();

	UpdateLists();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTrackSelection::UpdateLists()
{
	CString strSection       = "Preferences";
    CString strStringItem    = "GP2Location";
	CWinApp* pApp = AfxGetApp();

	CString filename = pApp->GetProfileString(strSection, strStringItem,"c:\\gp2\\");

	SetDlgItemText(IDC_GP2LOCATION,filename);
	
	// TODO: Add extra initialization here
	CListBox *list = (CListBox*)GetDlgItem(IDC_TRACK_SELECTED_LIST);
	

	if (list && filename)
	{
		list->ResetContent();
	    for(int i=1;i<17;i++)
	    {
		  char buffer[256];
		  LPSTR fileName = (LPSTR)malloc(256*sizeof(char));
		  wsprintf(fileName,"%s\\Circuits\\f1ct%2.2d.dat",filename,i);

		  //CString *file = new CString(buffer);
		  GPTrack *myTrack = new GPTrack(FALSE);
		  if (myTrack->ReadTrackInfoFile(fileName))
		  {
		   wsprintf(buffer,"[%d]%s, %s, %s, %d",i,(LPCTSTR)myTrack->getCircuitName(),(LPCTSTR)myTrack->getCircuitCountry(),(LPCTSTR)myTrack->getCircuitYears(),myTrack->getCircuitLaps());
		   int idx = list->AddString(buffer);
		   list->SetItemData(idx,(DWORD)(LPCSTR)fileName);
		  }
		  else
		  {
		   AfxMessageBox("I have some problems reading files");
		   delete myTrack;
		   break;
		  }
		  //if (fileName) free(fileName);
		  delete myTrack;
		}

	}
	UpdateAllLists();
}

void CTrackSelection::UpdateAllLists()
{
	CListBox *alllist = (CListBox*)GetDlgItem(IDC_TRACK_ALL_LIST);

	char pwd[256];
	GetCurrentDirectory(256,pwd);

	SetDlgItemText(IDC_CURRENTDIR,pwd);
	

	if (alllist)
	{
		alllist->ResetContent();

	    HANDLE 	hFile;
		WIN32_FIND_DATA track_file;

		//TODO
		//Find All directories first add them in
		//TODO 
		//Find All drives
		alllist->Dir(0x00008010,"*");
		alllist->Dir(0x0000C000,"");
		
     
        if ((hFile = FindFirstFile("*.dat",&track_file)) != INVALID_HANDLE_VALUE)
        {
		    char buffer[256];
			LPSTR fileName = (LPSTR)malloc(256*sizeof(char));
            wsprintf(fileName,"%s", track_file.cFileName);

			
			GPTrack *myTrack = new GPTrack(FALSE);
		    if (myTrack->ReadTrackInfoFile(fileName))
			{
		    wsprintf(buffer,"%s, %s, %s, %d, (%s)",(LPCTSTR)myTrack->getCircuitName(),(LPCTSTR)myTrack->getCircuitCountry(),(LPCTSTR)myTrack->getCircuitYears(),myTrack->getCircuitLaps(),fileName);
		    int fromidx = alllist->AddString(buffer);			
		    alllist->SetItemData(fromidx,(DWORD)(LPCSTR)fileName);
			}
			//if (fileName) free(fileName);
		    delete myTrack;

           
            while( FindNextFile( hFile, &track_file ))
            {
				LPSTR fileName = (LPSTR)malloc(256*sizeof(char));
				wsprintf(fileName,"%s", track_file.cFileName);
				GPTrack *myTrack = new GPTrack(FALSE);
		        if (myTrack->ReadTrackInfoFile(fileName))
				{
				  if (myTrack->isValid())
				  {
		           wsprintf(buffer,"%s, %s, %s, %d, (%s)",(LPCTSTR)myTrack->getCircuitName(),(LPCTSTR)myTrack->getCircuitCountry(),(LPCSTR)myTrack->getCircuitYears(),myTrack->getCircuitLaps(),fileName);
		           int fromidx = alllist->AddString(buffer);
				   alllist->SetItemData(fromidx,(DWORD)(LPCSTR)fileName);
				  }
				}
				//if (fileName) free(fileName);
		        delete myTrack;
            }
	   }
       FindClose( hFile );



	   // Get Season Sets

	   if ((hFile = FindFirstFile("*.set",&track_file))!= INVALID_HANDLE_VALUE)
       {
		    int fromidx = alllist->AddString(track_file.cFileName);			
		    alllist->SetItemData(fromidx,(DWORD)(LPCSTR)strdup(track_file.cFileName));
			
            while( FindNextFile( hFile, &track_file ))
            {
				int fromidx = alllist->AddString(track_file.cFileName);
				alllist->SetItemData(fromidx,(DWORD)(LPCSTR)strdup(track_file.cFileName));
				
            }
	   }
       FindClose( hFile );
	   
	   
	}	
}

void CTrackSelection::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CTrackSelection::OnDblclkTrackAllList() 
{
	// TODO: Add your control notification handler code here
	CListBox *alllist = (CListBox*)GetDlgItem(IDC_TRACK_ALL_LIST);

	int inStallFrom = LB_ERR;

	char currentPwd[256];
	GetCurrentDirectory(256,currentPwd);

	
	if (alllist)
	{
	   inStallFrom = alllist->GetCurSel();

	   CString buffer;

	   alllist->GetText(inStallFrom,buffer);

	   

	   if (buffer[0] == '[')
	   {
		   // this is a drive or directroy
		   
		   if (buffer[1] == '-')
		   {
			   CString drive = buffer.Mid(2,buffer.GetLength()-4);
			   drive += ":\\";
			   SetCurrentDirectory(drive);
			   UpdateAllLists();
		   }
		   else
		   {
			   CString newPwd = CString(currentPwd);
			   CString newBuffer = buffer.Mid(1,buffer.GetLength()-2);

			   if (newPwd[newPwd.GetLength()-1]!='\\')
			   {
			     newPwd += "\\" +newBuffer;
			   }
			   else
			   {
				 newPwd += newBuffer;
			   }

			   //AfxMessageBox(newPwd);

			   SetCurrentDirectory(newPwd);

			   UpdateAllLists();
		   }
	   }
	}
}

char * getline(FILE *fp)
{
	char *buffer = (char*)malloc(256*sizeof(char));

	fscanf(fp,"%[^\n]\n",buffer);
	return buffer;
}

char * getCatagory(LPCSTR str)
{
	static char catagory[25];

	sscanf(str,"%[^:]",catagory);

	return catagory;
}

char * getValue(LPCSTR str)
{
	char * ptr = strchr(str,':');

	
	return ptr+1;
}


class SeasonFile: public CObject
{
   public:

	   SeasonFile(LPCSTR file,int pos):
		filePosition(pos)
	   {
			 fileName = strdup(file);
	   }

	   virtual ~SeasonFile()
	   {
		   free(fileName);
	   }

	   char *fileName;
	   int   filePosition;
}; 

void OpenSeasonSetFile(LPCSTR filename)
{
	FILE *fp;


	if (filename == NULL) 
	{
		AfxMessageBox("Bad Season Set File");
		return;
	}

	fp = fopen(filename,"r");

	if (fp == NULL)
	{
		char buffer[256];
		wsprintf(buffer,"Cannot open Season Set File (%s)",filename);
		AfxMessageBox(buffer);
		return;
	}

	Vector *TrackFiles = new Vector("TrackSet File",__LINE__);

	char *SeasonSetYear=NULL;
	char *SeasonSetEvent=NULL;

	while(!feof(fp))
	{
		char *line = getline(fp);

		if (line == NULL) break;

		if (line[0] == '/' && line[1] == '/')
		{
			// comment
			continue;
		}

		if (line[0] == '#') 
		{
			//read comment
			LPSTR cat = getCatagory(line);

			if (!strcmp(cat,"#YEAR"))
			{
				SeasonSetYear = getValue(line);
			}
			if (!strcmp(cat,"#EVENT"))
			{
				SeasonSetEvent = getValue(line);
			}
			if (!strcmp(cat,"#End"))
			{
				break;
			}
			continue;
		}
		if (!strcmp(line,"end") || !strcmp(line,"End"))
		{
			break;
		}

		char fileName[256];
		int    pos = 0;

		sscanf(line,"%[^|]|%d",fileName,&pos);

		SeasonFile * nextFile = new SeasonFile(fileName,pos);

		TrackFiles->addElement(nextFile);
	}

	CString strSection       = "Preferences";
    CString strStringItem    = "GP2Location";
	CWinApp* pApp = AfxGetApp();

	CString circuitsDir = pApp->GetProfileString(strSection, strStringItem,"c:\\gp2\\");

	char currentPwd[256];
	GetCurrentDirectory(256,currentPwd);

	CString mess;
	if (SeasonSetEvent!=NULL && SeasonSetYear!=NULL)
	{
      mess = CString("Do you want to install ");
	  mess += CString(SeasonSetEvent);
	  mess += ",";
	  mess += CString(SeasonSetYear);
	  mess += " Season Set?";
	}
	else
	{
		mess = CString("Do you want to install Unnamed Season Set?");
	}

	int result = AfxMessageBox(mess,MB_YESNO);

	if (result == IDYES)
	{
	 for(int i=0;i<TrackFiles->size();i++)
	 {
	   char buffer[256];
	   char originalFile[256];
	   SeasonFile * nextFile = (SeasonFile*)TrackFiles->elementAt(i);
	  
	   if (nextFile)
	   {
	    //wsprintf(buffer,"Installing %s to  F1ct%2.2d.dat",nextFile->fileName,nextFile->filePosition);
	    //AfxMessageBox(buffer);
		char message[1024];

		wsprintf(buffer,"%s\\Circuits\\F1ct%2.2d.dat",(LPCTSTR)circuitsDir,nextFile->filePosition);
		wsprintf(originalFile,"%s\\%s",currentPwd,nextFile->fileName);


		if (!CopyFile(originalFile,buffer,FALSE))
		{
			wsprintf(message,"Failed to copy file %s to %s",originalFile,buffer);
		    AfxMessageBox(message);
		} 
	   }	   
	 }
	}
	fclose(fp);


	delete TrackFiles;
}
