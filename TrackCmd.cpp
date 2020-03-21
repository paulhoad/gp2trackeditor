
#include "stdafx.h"
#include "TrackEditor.h"
#include "GPTrack.h"
#include "TrackCmd.h"
#include "CmdEditor.h"
#include "TextureChooser.h"
#include "SceneryEditor.h"
#include "SceneryOnOff.h"
#include "TrackSection.h"
#include "KerbEditor.h"
#include "ObjectPos.h"

#include "DataChangeObserver.h"

typedef struct 
{
	int   id;
	char *Desc;
	int   numArgs;
	char *Arguments;
	char *format;
	int   icon;
	char* argsDesc[14];
	char * helpStr;
}t_Cmds;

static t_Cmds CommandDesc[] =
{
	{0,"TrackSection",0,"W","",IDB_TRACK,{"",},"No Help For This Command"},

	{0x80,"Object Position",1,"BW","",IDB_OBJECT,{"Offset Into Sector","Object Definition Offset"},"No Help For This Command"},
	{0x81,"Display Distance In Front",1,"W","",IDB_EYES,{"Offset Into Sector","View Distance; typical values: 80-255"},"No Help For This Command"},
	{0x82,"Display Distance Behind",1,"W","",IDB_EYES,{"Offset Into Sector","View Distance; typical values: 80-255"},"No Help For This Command"},
	{0x83,"Obsolete (probaly old f1gp commands)",0,"","",IDB_UNK,{"unk; typical values: 3"},"No Help For This Command"},
	{0x84,"Obsolete (probaly old f1gp commands)",0,"","",IDB_UNK,{"unk; typical values: 3"},"No Help For This Command"},	
	{0x85,"Track Width Change",2,"BWWW","",IDB_TRACK_CHANGE,{"Offset Into Sector","Transition Length; typical values 4 .. 40","New Width; typical values: 950 .. 1800"},"No Help For This Command"},
	{0x86,"Connect PitLane Start",0,"W","",IDB_PIT,{""},"No Help For This Command"},
	{0x87,"Connect PitLane End",0,"W","",IDB_PIT,{""},"No Help For This Command"},
	{0x88,"Pit Lane Cmd; Left Pits",1,"W","",IDB_PIT,{""},"No Help For This Command"},
	{0x89,"Pit Lane Cmd; Right Pits",1,"W","",IDB_PIT,{""},"No Help For This Command"},


	{0x8a,"Track Markings Type A",5,"W","",IDB_TRACK_MARKINGS,{"Offset Into Sector","Marking Type; typical values: 3 (gives yellow color) 8 (light grey) 8968 14088 49160","Num Dotted Lines; (determins length of marking); typical values: 2 3 4 5 6 8 13","Horizontal Movement; (inch; range is about +/-1500)","Angle Of Line; typical values: -620 0 620","Marking Type 2; typical values: 257 513 771 1281"},"Many thanks to babbel for finding these a more thorough description\n The Yellow dotted lines before/after pit entry/exit\nColors 3=Yellow\n 1,2 light Gray\n"},
	{0x8b,"Track Markings Type B",5,"W","",IDB_TRACK_MARKINGS,{"Offset Into Sector","Marking Type; typical values: 3 (gives yellow color) 8 (light grey) 8968 14088 49160","Num Dotted Lines; (determins length of marking); typical values: 2 3 4 5 6 8 13","Horizontal Movement; (inch; range is about +/-1500)","Angle Of Line; typical values: -620 0 620","Marking Type 2; typical values: 257 513 771 1281"},"No Help For This Command"},
	{0x8c,"Unknown",1,"W","",IDB_UNK,{""},"No Help For This Command"},
	{0x8d,"Unknown",1,"W","",IDB_UNK,{""},"No Help For This Command"},
	{0x8e,"Left Kerbs Begin/Length",2,"","",IDB_KERB,{"Unused","Offset Into Sector","Length"},"No Help For This Command"},
	{0x8f,"Right Kerbs Begin/Length",2,"","",IDB_KERB,{"Unused","Offset Into Sector","Length"},"No Help For This Command"},




	{0x90,"?Possible CC line command? (backward ref)",1,"","",IDB_UNK,{""},"No Help For This Command"},
	{0x91,"?Possible CC line command? (backward ref)",1,"","",IDB_UNK,{""},"No Help For This Command"},
	{0x92,"Unknown",1,"W","",IDB_UNK,{""},"No Help For This Command"},
	{0x93,"Unknown",1,"W","",IDB_UNK,{""},"No Help For This Command"},
	{0x94,"Track Grip/Bumps?",1,"W","",IDB_GRIP,{"Possibly ignore","Grip Value?"},"No Help For This Command"},
	{0x95,"Track Grip/Bumps?/Slide Marks?",1,"W","",IDB_GRIP,{"Possibly ignore","Grip Value?"},"No Help For This Command"},
	{0x96,"PitLane Speed Limiter On",0,"W","",IDB_SPEED,{"Offset Into Sector"},"No Help For This Command"},
	{0x97,"PitLane Speed Limiter Off",0,"W","",IDB_SPEED,{"Offset Into Sector"},"No Help For This Command"},
	{0x98,"Left Fence Height Change",1,"W","",IDB_WALLCHANGE,{"Unk?","New Fence Height; typical values: 1 2 3 4"},"No Help For This Command"},
	{0x99,"Right Fence Height Change",1,"W","",IDB_WALLCHANGE,{"Unk?","New Fence Height; typical values: 1 2 3 4"},"No Help For This Command"},
	{0x9a,"Left Fence Height Change?",2,"W","",IDB_WALLCHANGE,{"?Offset Info Sector","New Height; typical values: 3","Transision Length; typical values: 256 (see t74 in barcelona)"},"No Help For This Command"},
	{0x9b,"Pit Lane Begin Offset",0,"W","",IDB_PIT,{"value must be <= the length to 0x9f"},"No Help For This Command"},
	{0x9c,"Not USED UNDERDEVELOPMENT",0,"","",IDB_UNK,{""},"No Help For This Command"},  //NEW
	{0x9d,"Not USED UNDERDEVELOPMENT",0,"","",IDB_UNK,{""},"No Help For This Command"},  //NEW
	

	{0x9e,"Pit Lane End Length",0,"","",IDB_PIT,{"value must be <= the remaining length"},"No Help For This Command"},
	{0x9f,"Pit Lane Fences Begin",0,"","",IDB_PIT,{""},"No Help For This Command"},
	{0xA0,"Pit Lane Fences End",0,"","",IDB_PIT,{""},"No Help For This Command"},
	{0xA1,"Pit Lane Entry, Join Right Pit Lane Fence",0,"","",IDB_PIT,{""},"No Help For This Command"},
	{0xA2,"Pit Lane Entry, Join Left Pit Lane Fence",0,"","",IDB_PIT,{""},"No Help For This Command"},
	{0xA3,"Pit Lane Exit, Join Right Pit Lane Fence",0,"","",IDB_PIT,{""},"No Help For This Command"},
	{0xA4,"Pit Lane Exit, Join Left Pit Lane Fence",0,"","",IDB_PIT,{""},"No Help For This Command"},
	{0xA5,"Not USED UNDERDEVELOPMENT (seen in f1gp)",0,"","",IDB_UNK,{""},"No Help For This Command"},
	
	{0xA6,"Unknown",2,"WWW","",IDB_UNK,{""},"No Help For This Command"},
	{0xA7,"Unknown",2,"WWW","",IDB_UNK,{""},"No Help For This Command"},
	{0xA8,"Trigger Flag Men Waving at race end!",0,"WWW","",IDB_CHECKED_FLAG,{""},"No Help For This Command"},

	{0xA9,"Not USED UNDERDEVELOPMENT",1,"","",IDB_UNK,{""},"No Help For This Command"},

	{0xAA,"Unknown",3,"WWW","",IDB_UNK,{""},"No Help For This Command"},
	{0xAB,"Unknown",2,"WWW","",IDB_UNK,{""},"No Help For This Command"},
	{0xAC,"Unknown",4,"WWW","",IDB_UNK,{""},"No Help For This Command"},	
	{0xAD,"Track Banking",2,"WWW","",IDB_BANK_RIGHT,{"Unused","Length","Height +ve left -ve right"},"No Help For This Command"},
	{0xAF,"Pair of Swivel Arms",2,"WWW","",IDB_SCENERY,{"Offset into section","Distance to Left","Distance to Right"},"No Help For This Command"},
	

	{0xB0,"Turn Ribbons Off",1,"","",IDB_SCENERY,{"Offset Info Sector","Location CodeType B (for switched ribbon)"},"No Help For This Command"},
	{0xB1,"Unknown",1,"W","",IDB_UNK,{""},"No Help For This Command"},
	{0xB2,"Switch Track Side Off",1,"","",IDB_UNK,{"Offset Info Sector","(0,1=left,2=right,3=both) "},"Bit 1: turns off the left boundary line. Bit 2: turns off the right boundary line. This means that value 0 has no effect, 1 turns left line off, 2 turns right line off and 3 turns both lines off. "},
	{0xB3,"Unknown",1,"W","",IDB_UNK,{""},"No Help For This Command"},
	{0xB4,"Track Width Change Left",2,"WWW","",IDB_TRACK_CHANGE_LEFT,{"Unused","Length into sector","new width (avg~=1500)"},"No Help For This Command"},
	{0xB5,"Track Width Change Right",2,"WWW","",IDB_TRACK_CHANGE_RIGHT,{"Unused","Length into sector","new width (avg~=1500)"},"No Help For This Command"},
	{0xB6,"Pitlane Start Angle (adjustment)",1,"","",IDB_PIT,{"Unused","Pitlane Angle"},"No Help For This Command"},
	{0xB7,"Pitlane Start Height (adjustment)",1,"","",IDB_PIT,{"Unused","Pitlane Height"},"No Help For This Command"},
	{0xB8,"Scenery Ribbon Structure",13,"W","",IDB_SCENERY,{"unused","ribbons to the right","Ribbon 1 x","Ribbon 1 z","Ribbon 2 x","Ribbon 2 z","Ribbon 3 x","Ribbon 3 z","Ribbon 4 x","Ribbon 4 z","Ribbon 5 x","Ribbon 5 z","Ribbon 6 x","Ribbon 6 z"},"No Help For This Command"},
	{0xB9,"Turn On Ribbons",1,"WWW","",IDB_SCENERY,{"Offset Info Sector","Location CodeType B (for switched ribbon)"},"No Help For This Command"},
    {0xBA,"Bridge Scenery",2,"","",IDB_SCENERY,{"?Offset Info Sector","Location Code Type B","?Location Code Type B"},"No Help For This Command"},
	{0xBB,"Texture Mapping Light ",4,"","",IDB_OFFSET,{"start pos into sector?","Texture Location?","length?","texture?"},"No Help For This Command"},
    {0xBC,"Texture Mapping ",7,"WWWWWWW","",IDB_SIDE_UNK,{"start pos into sector","Texture Location","length","texture","nbitmaps","nrows","y offset","rotation"},"This command is used for markings on road side or armco coverings\n i.e. adverts,red/white,gravel and grass etc.\nTexture Location:\n4=right fence\n5=left fence\n8=right road side\n9=left road side"},
	{0xBD,"Light Source (Sun)",2,"","",IDB_LIGHT,{"Unk?","x","y"},"No Help For This Command"},
	{0xBE,"View-Distance To Banks InFront",2,"","",IDB_EYES,{"offset into sector","location code 32=left 16=right","new distance"},"No Help For This Command"},
	{0xBF,"View-Distance to Banks Behind",2,"","",IDB_EYES,{"offset into sector","location code 32=left 16=right","new distance"},"No Help For This Command"},

	{0xC0,"Single Swivel Arm Left",1,"","",IDB_SCENERY,{"distance into sector","distance to peek"},"No Help For This Command"},
	{0xC1,"Single Swivel Arm Right",1,"","",IDB_SCENERY,{"distance into sector","distance to peek"},"No Help For This Command"},
	{0xC2,"Unknown",2,"","",IDB_UNK,{""},"No Help For This Command"},
	{0xC3,"Unknown",2,"","",IDB_UNK,{""},"No Help For This Command"},
	{0xC4,"Unknown",3,"","",IDB_UNK,{""},"No Help For This Command"},
	{0xC5,"Define Far Sight",7,"","",IDB_EYES,{""},"No Help For This Command"},
	{0xC6,"Far Sight Area",1,"","",IDB_EYES,{""},"No Help For This Command"},
	{0xC7,"UNK Scenery bridging?",2,"","",IDB_UNK,{""},"No Help For This Command"},
	{0xC8,"Scenery Texture Definition",7,"W","",IDB_SCENERY,{"Unk","Texture Location","Length of Repeated Section","Texture id","nbitmaps","nrows","y offset","rotation"},"No Help For This Command"},
	{0xC9,"Set Colors In GP2-Palette",8,"W","",IDB_SCENERY,{"Unused","Palette Index Begin","Palette Index End","Hue Angle Begin","Hue Angle End","Saturation Begin","Saturation End","Intensity Value Begin","Intensity Value End"},"No Help For This Command"},
	{0xCA,"Type A Kerb Definition ",4,"WWWWW","",IDB_KERB,{"Always 0?","Distance to Height 1","Distance to Height 2","Height1","Height2"},"No Help For This Command"},
	{0xCB,"Type B Kerb Definition (click 0x4)",4,"WWWWW","",IDB_KERB,{"Always 0?","Distance to Height 1","Distance to Height 2","Height1","Height2"},"No Help For This Command"},
	{0xCC,"Horizon Adjustment",2,"","",IDB_SCENERY,{"Unused","Pitlane Angle"},"No Help For This Command"},
	{0xCD,"Scenery Ribbons Lighten or Darken (Adjust Shadow)",3,"","",IDB_SCENERY,{"offset into sector","location code","length","light/darken"},"1:wall left\n2:wall right\n16=bank left\n32=bank right\n128=ribbon on right\n1024=2nd ribbon\n2048=1st ribbon"},
	{0xCE,"Unknown/Monaco Tunnel adjust!",1,"","",IDB_UNK,{""},"No Help For This Command"},
	{0xCF,"Show pit objects through pitwall ",1,"","",IDB_UNK,{"offset into sector","distance effect seen for"},"No Help For This Command"},
	
    {0xD0,"Possible CC line command",2,"","",IDB_CC_LINE,{""},"No Help For This Command"},
	{0xD1,"Unknown",0,"","",IDB_UNK,{""},"No Help For This Command"},
	{0xD2,"UNK Track Grip?",1,"","",IDB_UNK,{"Unk?","Track Grip Level?"},"No Help For This Command"},
	{0xD3,"View into Pitlane Entrance",1,"","",IDB_EYES,{""},"No Help For This Command"},
	{0xD4,"View All Pit Lane From Entry",0,"","",IDB_EYES,{"Offset Into Sector"},"No Help For This Command"},
	{0xD5,"View into Pitlane Exit",1,"","",IDB_EYES,{""},"No Help For This Command"},
	{0xD6,"View All Pit Lane From Exit,",0,"","",IDB_EYES,{"Offset Into Sector"},"No Help For This Command"},
	{0xD7,"Unknown",0,"","",IDB_UNK,{""},"No Help For This Command"},
	{0xD8,"Unknown",0,"","",IDB_UNK,{""},"No Help For This Command"},
	{0xD9,"Turn Ribbons On/Off",1,"","",IDB_SCENERY,{"Unused","Ribbon Switch 0=all of -> 15 all on"},"No Help For This Command"},
	{0xDA,"Silly Sceney Command",1,"","",IDB_UNK,{"Offset Into Sector","Location Code Type B"},"No Help For This Command"},	
	{0xDB,"Switch pits/track drawing order",0,"","",IDB_UNK,{"Unused","offset into sector"},"No Help For This Command"},	
	{0xDC,"Unknown",1,"","",IDB_UNK,{""},"No Help For This Command"},
	{0xDD,"Weirdo Enabler",0,"","",IDB_UNK,{""},"No Help For This Command"},

	{0xDE,"Black Flag Area Left",3,"W","",IDB_BLACK_FLAG,{"Unused","Begin pos from start","End Pos from start","Limited Speed"},"No Help For This Command"},
	{0xDF,"Black Flag Area Right",3,"W","",IDB_BLACK_FLAG,{"Unused","Begin pos from start","End Pos from start","Limited Speed"},"No Help For This Command"},

	{0xE0,"Kerb Adjust Left (0xCA Kerb)",1,"","",IDB_KERB,{"Unused=0","New Height"},"No Help For This Command"},
	{0xE1,"Unknown",0,"W","",IDB_UNK,{""},"No Help For This Command"},
	{0xE2,"Unknown",0,"W","",IDB_UNK,{""},"No Help For This Command"},
	{0xE3,"Unknown",0,"W","",IDB_UNK,{""},"No Help For This Command"},
	{0xE4,"Unknown",0,"W","",IDB_UNK,{""},"No Help For This Command"},
	{0xE5,"Unknown",0,"W","",IDB_UNK,{""},"No Help For This Command"},
	//{0xE1,"Kerb Adjust Left (0xCB Kerb) (UNDERDEVELOPMENT DO NOT USE)",1,"","",IDB_KERB,{"Unknown","New Height"},"No Help For This Command"},
};

static int cmddescs = sizeof(CommandDesc)/sizeof(CommandDesc[0]);

static void InsertString(CListCtrl *list,int i,int index,LPSTR name,int ImageId)
{
	int mask = LVIF_TEXT;

	if (index == 0) 
	{
		 list->InsertItem(mask,0,name,0,0,ImageId,0);
	}
	else
	{
		 list->SetItem(0,index,mask,name,ImageId,0,0,0);
	}
}



void LoadCmdListBox(CListCtrl *list)
{
	char buffer[256];

	int colName = list->InsertColumn(0,"Code",LVCFMT_LEFT,80,0);
	int colData = list->InsertColumn(1,"Command",LVCFMT_LEFT,150,1);
	int colDesc = list->InsertColumn(2,"Args",LVCFMT_LEFT,100,2);

    

	for(int i=0;i<cmddescs;i++)
	{
	  t_Cmds cmdd = CommandDesc[i];

	  if (cmdd.id != 0)
	  {
	   wsprintf(buffer,"0x%x (%d)",cmdd.id,cmdd.id);

	   InsertString(list,i,0,buffer,TO_ID(getCmdIcon(cmdd.id)));
	   list->SetItemData(0,cmdd.id);

	   wsprintf(buffer,"%s",cmdd.Desc);
	   InsertString(list,i,1,buffer,TO_ID(getCmdIcon(cmdd.id)));

	   wsprintf(buffer,"%d",cmdd.numArgs);
	   InsertString(list,i,2,buffer,TO_ID(getCmdIcon(cmdd.id)));
	  }
	}
}

CString getCmdDescription(int id)
{
	char buffer[256];

		for(int i=0;i<cmddescs;i++)
		{
			t_Cmds cmdd = CommandDesc[i];

			if (cmdd.id == id) 
			{
				wsprintf(buffer,"%s",cmdd.Desc);
				return CString(buffer);
			}
		}
		return CString("Unknown");
}

CString getCmdHelp(int id)
{
	char buffer[256];

	for(int i=0;i<cmddescs;i++)
	{
			t_Cmds cmdd = CommandDesc[i];

			if (cmdd.id == id) 
			{
				wsprintf(buffer,"%s",cmdd.helpStr);
				return CString(buffer);
			}
	}
	return CString("Unknown");
}

int getCmdNumArgs(int id)
{
	for(int i=0;i<cmddescs;i++)
		{
			t_Cmds cmdd = CommandDesc[i];

			if (cmdd.id == id) 
			{
				
				return cmdd.numArgs;
			}
	}
	return 0;
}

int TrackCmd::getCmdIcon()
	{
		for(int i=0;i<cmddescs;i++)
		{
			t_Cmds cmdd = CommandDesc[i];

			if (cmdd.id == cmd) 
			{
				return cmdd.icon;
			}
		}
		return IDB_UNK;
	}

int getCmdIcon(int id)
{
		for(int i=0;i<cmddescs;i++)
		{
			t_Cmds cmdd = CommandDesc[i];

			if (cmdd.id == id) 
			{
				return cmdd.icon;
			}
		}
		return IDB_UNK;
}

CString TrackCmd::getCmdDescription()
	{
		char buffer[256];

		for(int i=0;i<cmddescs;i++)
		{
			t_Cmds cmdd = CommandDesc[i];

			if (cmdd.id == cmd) 
			{
				wsprintf(buffer,"%s",cmdd.Desc);
				return CString(buffer);
			}
		}
		return CString("Unknown");
	}

CString TrackCmd::getCmdNthArgDescription(int arg)
	{
		char buffer[256];

		for(int i=0;i<cmddescs;i++)
		{
			t_Cmds cmdd = CommandDesc[i];

			if (cmdd.id == cmd) 
			{
				 int sizeofarray = sizeof(cmdd.argsDesc)/sizeof(cmdd.argsDesc[0]);
				 if ((arg-1) >= sizeofarray)
				 {
					 return CString("No Description Found");
				 }
				 char *str = cmdd.argsDesc[arg-1];
				 wsprintf(buffer,"Unk(%d)",arg);

				 if (str)
				 {
				   if(!strcmp(str,""))
				   {
				     wsprintf(buffer,"Unk(%d)",arg);
				   }
				   else
				   {
				     wsprintf(buffer,"%s",str);
				   }
				 }
				 else
				   wsprintf(buffer,"Unk(%d)",arg);
				
				return CString(buffer);
			}
		}
		return CString("Unk");
	}


#define SET_ARG(x) 	if (args >= (x)) setNthArg((x),dlg->m_Arg##x);

#define CHECK_ARG(x) if (args>= (x)) {dlg->m_Arg##x = getNthArg(##x);dlg->m_Desc##x = getCmdNthArgDescription(##x);}

int TrackCmd::Open()
{
	switch(getCmd())
	{
	   case 0xBC:
		   return OpenTextureEditor();
		   break;
	   case 0xC8:
		   return OpenTextureEditor();
		   break;
	   case 0xB8:
		   return OpenSceneryEditor();
	   case 0xca:
		   return OpenKerbEditor();
	   case 0xcb:
		   return OpenKerbEditor();
	   case 0xB9:
       case 0xD9:
	   case 0xB0:
		   return OpenSceneryOnOffEditor();
	   case 0x80:
		   return OpenObjectEditor();
	   default:
		   return OpenCmdEditor();
		   break;
	}
	return 0;
}

int TrackCmd::OpenSceneryOnOffEditor()
{
	if (owner->isTrackSection())
	{
	int index = getTrack()->GetTrackSectionIndex(owner);
    CSceneryOnOff *dlg = new CSceneryOnOff(getTrack(),getNthArg(1),getNthArg(2));

	if (getCmd() == 0xB0) dlg->m_OnOff="Off";
	else dlg->m_OnOff="On";

	int result = dlg->DoModal();

	 if (result == IDOK)
	 {
	  setNthArg(2,dlg->val);
	  setNthArg(1,dlg->m_Offset);
	  track->regenerate = TRUE;
	 }

	if (dlg) delete dlg;
	}
	else
	{
		OpenCmdEditor();
	}
	return 0;
}

int TrackCmd::OpenSceneryEditor()
{
	if (owner->isTrackSection() || owner->isPitSection())
	{
	BOOL pitLane = owner->isPitSection();
	int index = getTrack()->GetTrackSectionIndex(owner);
	int editing = ((TrackSection*)owner)->GetNthOccurance(this);
    CSceneryEditor *dlg = new CSceneryEditor(getTrack(),pitLane,index,editing);

	dlg->DoModal();

	if (dlg) delete dlg;
	}
	else
	{
		OpenCmdEditor();
	}
	return 0;
}

int TrackCmd::OpenObjectEditor()
{
        CObjectPos *dlg = new CObjectPos(track);

		dlg->setSection((TrackObject*)this);
		
		int result=0;
		

		if (dlg)
		{
		  result = dlg->DoModal();
		}

		if (result!=IDCANCEL)
		{
		 dlg->getSection((TrackObject*)this);
		 track->regenerate = TRUE;
		}
		return result;
}

int TrackCmd::OpenTextureEditor()
{
	CTextureChooser *tc = new CTextureChooser(track);
	tc->setInfo(getNthArg(1),getNthArg(2),getNthArg(3),getNthArg(4),getNthArg(5),getNthArg(6),getNthArg(7),getNthArg(8));
	int result = tc->DoModal();

	if (result!=IDCANCEL)
	{
		TRACE("Setting %d",tc->getTextureLocationNumber(tc->SelectedLocation));
		setNthArg(1,tc->SelectedOffset);
		setNthArg(2,tc->getTextureLocationNumber(tc->SelectedLocation));
		setNthArg(3,tc->SelectedLength);
		setNthArg(4,tc->SelectedTexture);
		setNthArg(5,tc->SelectedRepeats);
		setNthArg(6,tc->SelectedResolution);
		setNthArg(7,tc->SelectedYOffset);
		setNthArg(8,tc->getRotation(tc->SelectedRotation));
		track->regenerate = TRUE;
	}
	
	delete tc;
	return result;
}


int TrackCmd::OpenKerbEditor()
{
	CKerbEditor *tc = new CKerbEditor(track);
	tc->m_X1 = getNthArg(2);
	tc->m_X2 = getNthArg(3);
	tc->m_Y1 = getNthArg(4);
	tc->m_Y2 = getNthArg(5);
	int result = tc->DoModal();

	if (result!=IDCANCEL)
	{
		//setNthArg(1,0);
		setNthArg(2,tc->m_X1);
		setNthArg(3,tc->m_X2);
		setNthArg(4,tc->m_Y1);
		setNthArg(5,tc->m_Y2);
		track->regenerate = TRUE;
	}
	
	delete tc;
	return result;
}

int TrackCmd::OpenCmdEditor()
{	
	CCmdEditor *dlg = new CCmdEditor(NULL,getCmdNumArgs());

	dlg->m_Cmd = getCmd();
	int args   = getCmdNumArgs();

	dlg->m_DescriptionText = getCmdDescription();

	
	CHECK_ARG(1);
	CHECK_ARG(2);
	CHECK_ARG(3);
	CHECK_ARG(4);
	CHECK_ARG(5);
	CHECK_ARG(6);
	CHECK_ARG(7);
	CHECK_ARG(8);
	CHECK_ARG(9);
	CHECK_ARG(10);
    CHECK_ARG(11);
    CHECK_ARG(12);
	CHECK_ARG(13);
	CHECK_ARG(14);

	int result;
	
	if (dlg)
	{ 
		result = dlg->DoModal();
		if (result != IDCANCEL)
		{
		SET_ARG(1)
		SET_ARG(2)
		SET_ARG(3)
		SET_ARG(4)
		SET_ARG(5)
		SET_ARG(6)
		SET_ARG(7)
		SET_ARG(8)
		SET_ARG(9)
		SET_ARG(10)
		SET_ARG(11)
		SET_ARG(12)
		SET_ARG(13)
	    SET_ARG(14)
		track->regenerate = TRUE;
		}
	}
	if (dlg) delete dlg;
	return result;
}

void TrackCmd::Delete()
{
	track->RemoveCommand(this);
}


void TrackCmd::LoadPropertyTable(CTrackPropertySheet *table)
{
	int numArgs   = getCmdNumArgs();

	CString cmdName  = getCmdDescription();

	for(int i=1;i<=numArgs;i++)
	{
		char buffer[20];

		int   data    = getNthArg(i);
		DataValue *dataPtr = getNthArgDataValue(i);
		CString desc = getCmdNthArgDescription(i);
		char argStr[15];

		wsprintf(argStr,"Arg %d",i);

		DataChangeObserver *observer = new DataChangeObserver(getTrack(),dataPtr,t_INT,argStr,(LPSTR)(LPCTSTR)desc);
		
		
		wsprintf(buffer,"Cmd 0x%x Arg (%d)",cmd,i);
		table->InsertTableString(i-1,0,(LPSTR)buffer,(LPARAM)observer);
		wsprintf(buffer,"%d",data);
		table->InsertTableString(i-1,1,(LPSTR)buffer,(LPARAM)observer);
		table->InsertTableString(i-1,2,(LPSTR)(LPCTSTR)desc,(LPARAM)observer);
		table->m_TrackProperty.SetItemData(i-1,(LPARAM)observer);
	}
}

Observer * TrackCmd::Copy()
{
	TrackCmd *copy = new TrackCmd(this);
	return (Observer*)copy;
}

TrackCmd::TrackCmd(TrackCmd *cmdIn):
	 Observer((Observer*)cmdIn),
	 name(cmdIn->name),
	 cmd(cmdIn->cmd),
	 owner(cmdIn->owner)
	{
	 //args = cmd->args->Copy();
	 args = new Vector("Track Cmds",__LINE__);
	 setTreeNode(NULL);

	 for(int i=1;i<=cmdIn->args->size();i++)
	 {
		 int arg1 = cmdIn->getNthArg(i);
		 TrackArg *arg = new TrackArg(arg1,GP_CMD_WORD,"Unk");
		 args->addElement(arg);
	 }
	}



