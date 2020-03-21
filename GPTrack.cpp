// cpp : implementation file
//

#include "stdafx.h"
#include "TrackEditor.h"
#include "TrackEditorDoc.h"
#include "GPTrack.h"
#include "stdio.h"
#include "math.h"
#include "TrackObject.h"
#include "TrackSection.h"
#include "TrackObjectDefinition.h"
#include "CCLineSection.h"
#include "TrackCmd.h"
#include "InternalObject.h"
#include "JamFileEditor.h"
#include "Texture.h"
#include "Ruler.h"
#include <direct.h>
#include "IO_Name.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static long GetLengthofFile(const char *name);

#define getTrackDataWord ReadPCWord
#define getTrackDataDWord ReadPCDWord

#define getTrackDataByte ReadPCByte
#ifndef PI
#define PI 3.14159265359
#endif
#define TRACK_WIDTH 1.75
#define PIT_WIDTH 1.75

#define PIT_LANE_DEFAULT_WIDTH 1024



#define putTrackDataWord WritePCWord
/////////////////////////////////////////////////////////////////////////////
// GPTrack
static CString *createString(int size,char *beginStr)
{
	CString *buffer = new CString(beginStr);
	return buffer;
}

GPTrack::GPTrack(BOOL create):
//TrackFilename(NULL),
PitLaneEntryIndex(0),
PitLaneExitIndex(0),
regenerate(FALSE),
TrackXBeginPos(128),
TrackYBeginPos(128),
carsetup(NULL),
TrackSections(NULL),PitlaneSections(NULL),TrackObjectDefintions(NULL),CCLineSections(NULL),
TrackCameras(NULL),InternalObjectOffsets(NULL),JamFiles(NULL),InternalObjectNames(NULL),TrackRootNode(0)
{
   GP2BackupNumber = 1;
   GP2MaxBackupNumber = 20;
   memset(trackdata,0,65535);
   circuitName  = CString("Unnamed Circuit");
   countryName  = CString("Unknown Country");
   circuitYear  = CString("1998");
   authorName   = CString("Unknown");
   circuitDesc  = CString("");
   circuitCopyright = CString("");
   circuitEvent = CString("Formula 1");
   lapRecord    = CString("None Entered");
   lapRecordQualify    = CString("None Entered");
   circuitLaps  = 99;
   setBestSlotInfo(1);
   circuitTyreConsumption = 25000;
   CCDataLog = NULL;
   split1 = 0;
   split2 = 0;

   TrackFilename = CString("unnamed.dat");

   CCScaleFactor = 1024.0;

   UnderLayBitmapX.SetValue(0);
   UnderLayBitmapY.SetValue(0);
   UnderLayBitmapWidth.SetValue(100);
   UnderLayBitmapHeight.SetValue(100);

   

   if (create)
	   Create();

   DefaultLeftFloorBrush  = TextureIdToBrush(this,TXT_GRASS);
   DefaultRightFloorBrush = TextureIdToBrush(this,TXT_GRASS);
   DefaultRoadBrush       = TextureIdToBrush(this,TXT_ROAD);
   DefaultRibbon11Brush       = TextureIdToBrush(this,TXT_GRASS);
   DefaultRibbon12Brush       = TextureIdToBrush(this,TXT_GRASS);
   DefaultRibbon13Brush       = TextureIdToBrush(this,TXT_GRASS);
   DefaultRibbon14Brush       = TextureIdToBrush(this,TXT_GRASS);
   DefaultRibbon15Brush       = TextureIdToBrush(this,TXT_GRASS);
   DefaultRibbon16Brush       = TextureIdToBrush(this,TXT_GRASS);
   DefaultTyreWallBrush=122;
}

#define PROFILE(x,b) x =  pApp->GetProfileInt(strSection, #x,b) == 1 ? TRUE : FALSE;
#define WR_PROFILE(x) pApp->WriteProfileInt(strSection, #x,x);

#define PROFILE_SPECIAL(x,b) pApp->GetProfileInt(strSection, #x,b)
#define WR_PROFILE_SPECIAL(x,y) pApp->WriteProfileInt(strSection, #x,y);

extern void AddJamDefaults(GPTrack *);

void GPTrack::Create()
{
	setTrackName("New Track");
	TrackSections   = new Vector("TrackSections 1",__LINE__);
	PitlaneSections = new Vector("PitlaneSections",__LINE__);
	TrackObjectDefintions = new Vector("TrackObjectDefinitions",__LINE__);
	CCLineSections = new Vector("CCLineSection",__LINE__);
	TrackCameras   = new Vector("TrackCameras",__LINE__);
	InternalObjectOffsets = new Vector("InternalObjectOffsets",__LINE__);
	JamFiles       = new Vector("JamFiles",__LINE__);

	AddJamDefaults(this);
	
	CString strSection       = "Preferences";
	CWinApp* pApp = AfxGetApp();

	CCScaleFactor = 1024.0;

	PROFILE(showTrack,TRUE)
	PROFILE(showWalls,FALSE)
	PROFILE(showPitWalls,showWalls);
	PROFILE(showPitLane,TRUE)
	PROFILE(showObjects,FALSE)
	PROFILE(showCCLine,TRUE)

	PROFILE(showHiddenAsGray,TRUE)
	PROFILE(showTrackPie,FALSE)
	PROFILE(showCameras,FALSE)
	PROFILE(showBitmaps,TRUE)
	PROFILE(showGreenScreen,FALSE)
	PROFILE(showTrackSide,FALSE)
	PROFILE(showFinishLine,TRUE)
	PROFILE(showFilledObjects,FALSE)

	PROFILE(showRemovedWalls,FALSE)
	PROFILE(showRoadSigns,TRUE)
	PROFILE(showCommands,FALSE)
	PROFILE(showCenterLine,TRUE)
	PROFILE(showAxis,TRUE)

	PROFILE(showKerbs,TRUE)
	PROFILE(showGrip,FALSE)
	PROFILE(showBlackFlags,TRUE)
	PROFILE(showScenery,TRUE)
	PROFILE(showGridLines,FALSE)
	PROFILE(showTrackNumbers,FALSE)
	PROFILE(showUseTextures,FALSE)
	PROFILE(showPitCmds,FALSE)
	PROFILE(showTrackDistances,FALSE)
	PROFILE(showUnkVariousTable,FALSE)
	PROFILE(showCCNumbers,FALSE)
	PROFILE(showCurvedWallStraight,FALSE)
	PROFILE(showObjectPositionMarker,TRUE)
	PROFILE(maintainBitmapAspectRatio,TRUE)
	PROFILE(showGraphPaper,FALSE)
	PROFILE(showMarkings,FALSE)
	PROFILE(showAnnotations,FALSE)
	PROFILE(showDefaultSides,TRUE)
	PROFILE(DefaultTyreWallBrush,122)
	PROFILE(DefaultGravelBrush,164)
	PROFILE(DefaultTarmacBrush,33)
	PROFILE(showCCDataLog,TRUE)
	PROFILE(showSplits,FALSE)
	PROFILE(showTextures,FALSE);
	PROFILE(showUnderlayBitmap,FALSE);
	PROFILE(showPointNumbers,FALSE)
	PROFILE(showVertexNumbers,FALSE)
	PROFILE(showPolygonNumbers,FALSE)
	PROFILE(showLines,TRUE)
	PROFILE(showPoints,FALSE)
	PROFILE(showCulling,FALSE)
	PROFILE(showNormals,FALSE)
	PROFILE(showOutlines,FALSE)
	PROFILE(showExternalObjects,TRUE)
	PROFILE(showCameraViewDistances,TRUE)
	PROFILE(showTextureMappingCommands,FALSE)
	PROFILE(showJamPreviews,TRUE)
	PROFILE(showViewDistanceCommands,FALSE)
	PROFILE(showViewDistances,TRUE)
	PROFILE(showReadObjNames,TRUE)
	PROFILE(useSwivelAngles,TRUE)
	PROFILE(showSceneryArms,TRUE)
	//PROFILE(showToolbar,TRUE)

	GP2MaxBackupNumber = (int)PROFILE_SPECIAL(GP2MaxBackupNumber,20);
	GP2BackupNumber = (int)PROFILE_SPECIAL(GP2BackupNumber,1);
	


	UnderLayBitmapX.SetValue((int)PROFILE_SPECIAL(UnderLayBitmapX,0));
	UnderLayBitmapY.SetValue((int)PROFILE_SPECIAL(UnderLayBitmapY,0));
	UnderLayBitmapWidth.SetValue((int)PROFILE_SPECIAL(UnderLayBitmapWidth,300));
	UnderLayBitmapHeight.SetValue((int)PROFILE_SPECIAL(UnderLayBitmapHeight,300));
	gridSizeOption = (int)PROFILE_SPECIAL(gridSizeOption,ONE_MILE);

	

	InternalObjectNames = new Vector("IOObjs",__LINE__);
	
	DefaultLeftVergeId = -1;
	DefaultRightVergeId = -1;
	DefaultRoadId = 33;
	DefaultLeftFenceId = 24;
	DefaultRightFenceId = 24;
}

void GPTrack::ShowHideAll(BOOL state)
{
	showTrack = state;
	showWalls = state;
	showPitLane = state;
	showObjects = state;
	showCCLine = state;
	showHiddenAsGray = state;
	showTrackPie = state;
	showCameras	 = state;
	showBitmaps = state;
	showGreenScreen	 = state;
	showTrackSide = state;
	showFinishLine  = state;
	showFilledObjects = state;
	showRemovedWalls = state;
	showRoadSigns = state;
	showCenterLine = state;
	showCommands = state;
	showKerbs = state;
	showGrip = state;
	showBlackFlags = state;
	showScenery = state;
	showGridLines = state;
	showTrackNumbers = state;
	showUseTextures = state;
	showPitCmds = state;
	showTrackDistances = state;
	showUnkVariousTable = state;
	showCCNumbers = state;
	showCurvedWallStraight = state;
	showObjectPositionMarker = state;
	showMarkings = state;
	showAnnotations = state;
	showDefaultSides = state;
	showSplits = state;
	showPitWalls = state;
}




void GPTrack::NewTrack()
{
	Create();
	setTrackName("New Track");
	carsetup       = new CCCarsetup(this,0);
	carsetup->defaultValues();

	// Jam File
	JamFiles->addElement(new JamFileEditor(this,"gamesjams/dproad.jam"));


	TrackStartAngle.SetValue(0);
	TrackStartClimb.SetValue(0);
	TrackStartX.SetValue(9000);
	TrackStartZ.SetValue(0);
	TrackStartY.SetValue(9000);
	TrackBeginWidth.SetValue(1400);

	int size = 40;

	for(int i=0;i<size;i++)
	{
		TrackSection *t = new TrackSection(TrackSections,this,i,10,0,0,20,20,0,0,NULL,0,1400,1400,NULL,t_TrackSection);
		TrackSections->addElement(t);
	}
	for(i=0;i<7;i++)
	{
		TrackSection *t = new TrackSection(PitlaneSections,this,i,10,0,0,20,20,0,0,NULL,0,1400,1400,NULL,t_PitSection);
		PitlaneSections->addElement(t);
	}
	for(i=0;i<20;i++)
	{
		CCLineSection *t = new CCLineSection(this,10,0,0,0,0,0,0x50,0);
		CCLineSections->addElement(t);
	}
	int offset = 10;
	int arcsize = 4;
	int mid = size/2;
	regenerate=TRUE;

	for(i=0;i<size;i++)
	{
		TrackSection *t = (TrackSection*)TrackSections->elementAt(i);

		if (i>offset && i < offset+arcsize)
		{
		  // put arc in
		  t->setAngle(2);
		  t->setLength(30);
		}
		else if (i>(mid+offset) && i< (mid+offset+arcsize))
		{
		  // other corner
		  t->setAngle(2);
		  t->setLength(30);
		}
		else 
		{
		  t->setAngle(0);
		  t->setLength(10);
		}
	}


	// Add commands that are essential
	TrackSection *begin = (TrackSection*)TrackSections->elementAt(0);
	TrackCmd *Cmd1 = new TrackCmd(this,begin,0xC9,0,"LIGHT",0,0,0,0,0,0,0,0,0);
	TrackCmd *Cmd2 = new TrackCmd(this,begin,0xC9,0,"LIGHT",0,0,0,0,0,0,0,0,0);
	TrackCmd *Cmd3 = new TrackCmd(this,begin,0xC9,0,"LIGHT",0,0,0,0,0,0,0,0,0);
	TrackCmd *Cmd4 = new TrackCmd(this,begin,0xC9,0,"LIGHT",0,0,0,0,0,0,0,0,0);
	begin->addCommand(Cmd1);
	begin->addCommand(Cmd2);
	begin->addCommand(Cmd3);
	begin->addCommand(Cmd4);
	
	TrackCmd *Cmd5 = new TrackCmd(this,begin,0xC8,0,"LIGHT",0,0,0,0,0,0,0,0,0);
	TrackCmd *Cmd6 = new TrackCmd(this,begin,0xC8,0,"LIGHT",0,0,0,0,0,0,0,0,0);
	TrackCmd *Cmd7 = new TrackCmd(this,begin,0xC8,0,"LIGHT",0,0,0,0,0,0,0,0,0);
	TrackCmd *Cmd8 = new TrackCmd(this,begin,0xC8,0,"LIGHT",0,0,0,0,0,0,0,0,0);
	TrackCmd *Cmd9 = new TrackCmd(this,begin,0xC8,0,"LIGHT",0,0,0,0,0,0,0,0,0);
	TrackCmd *Cmd10 = new TrackCmd(this,begin,0xC8,0,"LIGHT",0,0,0,0,0,0,0,0,0);
	TrackCmd *Cmd11 = new TrackCmd(this,begin,0xC8,0,"LIGHT",0,0,0,0,0,0,0,0,0);
	TrackCmd *Cmd12 = new TrackCmd(this,begin,0xC8,0,"LIGHT",0,0,0,0,0,0,0,0,0);
	begin->addCommand(Cmd5);
	begin->addCommand(Cmd6);
	begin->addCommand(Cmd7);
	begin->addCommand(Cmd8);
	begin->addCommand(Cmd9);
	begin->addCommand(Cmd10);
	begin->addCommand(Cmd11);
	begin->addCommand(Cmd12);

	TrackCmd *Light = new TrackCmd(this,begin,0xBD,0,"LIGHT",0,40784,8281);
	TrackCmd *kerbA = new TrackCmd(this,begin,KERB_DEF_1,0,"KERB_DEF_1",0,1,250,1,15);
	TrackCmd *kerbB = new TrackCmd(this,begin,KERB_DEF_2,0,"KERB_DEF_2",0,1,300,1,10);
	begin->addCommand(Light);
	begin->addCommand(kerbA);
	begin->addCommand(kerbB);


	TrackSection *pitroaden = (TrackSection*)TrackSections->elementAt(TrackSections->size()-2);
	TrackCmd *pitentry = new TrackCmd(this,pitroaden,0x86,0,"PITENTRY");
	pitroaden->addCommand(pitentry);

	TrackSection *pitroadex = (TrackSection*)TrackSections->elementAt(5);
	TrackCmd *pitexit = new TrackCmd(this,pitroadex,0x87,0,"PITEXIT");
	pitroadex->addCommand(pitexit);
	createTrack(NULL);
	createCC();

	//possible bug
	CCLineToSetupGap = 32;
	UnknownNumber    = 3;

	// calculate some offsets for new track
	ListObjectOffset = WriteOffsets();
	// now write the track definitions
	TrackDataOffset = WriteTrackObjectDefinitions(ListObjectOffset);

	//RecreateData();
	
	valid = TRUE;
}

GPTrack::~GPTrack()
{
	//TRACE("GPTrack::Destructor Called\n");
	if (TrackSections ) delete TrackSections;
	if (PitlaneSections) delete PitlaneSections;
	if (TrackObjectDefintions) delete TrackObjectDefintions;
	if (CCLineSections) delete CCLineSections;
	if (TrackCameras) delete TrackCameras;
	if (InternalObjectOffsets) delete InternalObjectOffsets;
	if (JamFiles) delete JamFiles;
	if (carsetup) delete carsetup;
	if (InternalObjectNames) delete InternalObjectNames;

	CString strSection       = "Preferences";
	CWinApp* pApp = AfxGetApp();
	

	WR_PROFILE(showTrack);
	WR_PROFILE(showWalls)
	WR_PROFILE(showPitLane)
	WR_PROFILE(showObjects)
	WR_PROFILE(showCCLine)

	WR_PROFILE(showHiddenAsGray)
	WR_PROFILE(showTrackPie)
	WR_PROFILE(showCameras)
	WR_PROFILE(showBitmaps)
	WR_PROFILE(showGreenScreen)
	WR_PROFILE(showTrackSide)
	WR_PROFILE(showFinishLine)
	WR_PROFILE(showFilledObjects)
	WR_PROFILE(showRemovedWalls)
	WR_PROFILE(showRoadSigns)
	WR_PROFILE(showCommands)
	WR_PROFILE(showCenterLine)
	WR_PROFILE(showKerbs);
	WR_PROFILE(showGrip);
	WR_PROFILE(showBlackFlags)
	WR_PROFILE(showScenery)
	WR_PROFILE(showGridLines)
	WR_PROFILE(showTrackNumbers)
	WR_PROFILE(showUseTextures)
	WR_PROFILE(showPitCmds)
	WR_PROFILE(showTrackDistances)
	WR_PROFILE(showUnkVariousTable)
	WR_PROFILE(showCCNumbers)
	WR_PROFILE(showCurvedWallStraight)
	WR_PROFILE(showObjectPositionMarker)
	WR_PROFILE(maintainBitmapAspectRatio)
	WR_PROFILE(showGraphPaper)
	WR_PROFILE(showMarkings)
	WR_PROFILE(showAnnotations)
	WR_PROFILE(showDefaultSides)
	WR_PROFILE(showCCDataLog)
	WR_PROFILE(showSplits)
	WR_PROFILE(showTextures)
	WR_PROFILE(showPitWalls)
	WR_PROFILE(showUnderlayBitmap)
	WR_PROFILE(showAxis)
	WR_PROFILE(showPointNumbers)
	WR_PROFILE(showVertexNumbers)
	WR_PROFILE(showPolygonNumbers)
	WR_PROFILE(showLines)
	WR_PROFILE(showPoints)
	WR_PROFILE(showCulling)
	WR_PROFILE(showNormals)
	WR_PROFILE(showOutlines);
	WR_PROFILE(showExternalObjects);
	WR_PROFILE(showCameraViewDistances)
	WR_PROFILE(showTextureMappingCommands)
	WR_PROFILE(showJamPreviews)
	WR_PROFILE(showViewDistanceCommands)
	WR_PROFILE(showViewDistances)
	WR_PROFILE(useSwivelAngles)
	WR_PROFILE(showSceneryArms)
	//WR_PROFILE(showToolbar)

	WR_PROFILE_SPECIAL(GP2MaxBackupNumber,GP2MaxBackupNumber)
	WR_PROFILE_SPECIAL(GP2BackupNumber,GP2BackupNumber)
	


    WR_PROFILE_SPECIAL(gridSizeOption,gridSizeOption)
	WR_PROFILE_SPECIAL(DefaultTyreWallBrush,DefaultTyreWallBrush)
	WR_PROFILE_SPECIAL(DefaultGravelBrush,DefaultGravelBrush)
	WR_PROFILE_SPECIAL(DefaultTarmacBrush,DefaultTarmacBrush)


	WR_PROFILE_SPECIAL(UnderLayBitmapX,UnderLayBitmapX.GetValue())
	WR_PROFILE_SPECIAL(UnderLayBitmapY,UnderLayBitmapY.GetValue())
	WR_PROFILE_SPECIAL(UnderLayBitmapWidth,UnderLayBitmapWidth.GetValue())
	WR_PROFILE_SPECIAL(UnderLayBitmapHeight,UnderLayBitmapHeight.GetValue())

	TrackSections=NULL;
	PitlaneSections=NULL;
	TrackObjectDefintions=NULL;
	CCLineSections=NULL;
	TrackCameras=NULL;
	InternalObjectOffsets=NULL;
	JamFiles=NULL;
	InternalObjectNames= NULL;

	TrackRootNode=0;
}

BOOL GPTrack::isCircuitDefined()
{
	if (!circuitName.Compare("") || !circuitName.Compare("Unnamed Circuit")) return FALSE;
	return TRUE;
}
BOOL GPTrack::isCountryDefined()
{
	if (!countryName.Compare("") || !countryName.Compare("Unknown Country")) return FALSE;
	return TRUE;
}



void GPTrack::WriteInfo()
{
	char buffer[512];
	memset(buffer,0,512);

	int circuitLengthMeters =  (int)(LENGTH_TO_KM * totalLength());
	

	wsprintf(buffer,"#GP2INFO|Name|%s|Country|%s|Created|Created by Track Editor written by Paul Hoad see (License.txt about distributing this track)|Author|%s|Year|%s|Event|%s|Desc|%s|Laps|%d|Slot|%d|Tyre|%d|LengthMeters|%d|LapRecord|%s|LapRecordQualify|%s|Split1|0x%x|Split2|0x%x|#",circuitName,countryName,authorName,circuitYear,circuitEvent,circuitDesc,circuitLaps,circuitSlot,circuitTyreConsumption,circuitLengthMeters,lapRecord,lapRecordQualify,split1,split2);

	for(int i =0;i<(int)strlen(buffer);i++)
	{
		trackdata[i] = buffer[i];
	}
}

void GPTrack::WriteAnnotations()
{
	char buffer[2000];
	memset(buffer,0,2000);

	int count = 2000;

	trackdata[count++] = '#';
	trackdata[count++] = 'G';
	trackdata[count++] = 'P';
	trackdata[count++] = '2';
	trackdata[count++] = 'T';
	trackdata[count++] = 'I';

	 for(int i=0;i<TrackSections->size();i++)
	 {
		TrackSection *t = (TrackSection*)TrackSections->elementAt(i); 
		LPSTR text = (LPSTR)t->getAnnotation();
		if (text!=NULL)
		
		{
		  wsprintf(buffer,"t%d|%s|",i,text);
		  for(int j =0;j<(int)strlen(buffer);j++)
	      {
		   if (
			   count > 4000) 
		   {
			   AfxMessageBox("Too much annotation text (track)");
			   break;
		   }
		   trackdata[count++] = buffer[j];
	      }
		}
	 }
	 for(i=0;i<PitlaneSections->size();i++)
	 {
		TrackSection *t = (TrackSection*)PitlaneSections->elementAt(i);
		LPSTR text = (LPSTR)t->getAnnotation();
		if (text!=NULL)
		{
		  wsprintf(buffer,"p%d|%s|",i,text);
		  for(int j =0;j<(int)strlen(buffer);j++)
	      {
		   if (count > 4000) 
		   {
			   AfxMessageBox("Too much annotation text (pit)");
			   break;
		   }
		   trackdata[count++] = buffer[j];
	      }
		}
	}
	trackdata[count++] = '#';
	trackdata[count++] = '|';
	trackdata[count++] = '#';

	
}

void GPTrack::ReadAnnotations()
{
	int count = 2000;
	if (trackdata[count]=='#' && trackdata[count+1]=='G' && trackdata[count+2]=='P')
	{
	  //found track info
	}
	else
	{
		return;
	}

	count=2006;
	static char cat[256];
	static char val[256];
	int last = 4000;

	
	while(count<last)
	{
		int pos=0;
		//get catagory
		while(count<last && pos < 256)
		{
		  if (trackdata[count] == '|')break;
		  cat[pos++]=trackdata[count];
		  cat[pos]='\0';
		  count++;
		}
		count++;
		pos=0;
		// get class
		while(count<last && pos < 256)
		{
		  if (trackdata[count] == '|')break;
		  val[pos++]=trackdata[count];
		  val[pos]='\0';
		  count++;
		}
		count++;

		if (TrackSections!=NULL && PitlaneSections!=NULL)
		{
		 if (cat[0]=='t')
		 {
			int sec;
			sscanf(cat,"t%d",&sec);

			if (sec >=0 && sec < TrackSections->size())
			{
				TrackSection *t = (TrackSection *)TrackSections->elementAt(sec);
				t->setAnnotation(val);
			}
		 }
		 else if (cat[0]=='p')
		 {
			int sec;
			sscanf(cat,"p%d",&sec);

			if (sec >=0 && sec < PitlaneSections->size())
			{
				TrackSection *t = (TrackSection*)PitlaneSections->elementAt(sec);
				t->setAnnotation(val);
			}
		 }		
		 else if (cat[0]=='#')
		 {
			break;
		 }
		}
		else
		{
			AfxMessageBox("No Track Sections Yet!");
		}
		//memset(val,0,256);
	}
}

BOOL GPTrack::isF1GP()
{
		return ((CTrackEditorDoc*)document)->isF1GP();
}


void GPTrack::ReadInfo(BOOLEAN complainNoGPInfo)
{
	if (isF1GP()) {
		AfxMessageBox("GP2INFO not present in F1GP file");
		return;
	}
	if (trackdata[0]=='#' && trackdata[1]=='G' && trackdata[2]=='P')
	{
	  //found track info
	}
	else
	{
		//AfxMessageBox("DON'T PANIC but....\n\nGP2INFO Missing for this file! (Your file has no GP2INFO attached) Track Editor will ask you for this info before saving it to disk",MB_OK);
		GuessTrackIdenty(complainNoGPInfo);
		return;
	}

	int count=9;
	char cat[256];
	char val[256];
	int last = 2000;
	

	while(count<last)
	{
		int pos=0;
		//get catagory
		while(count<last)
		{
		  if (trackdata[count] == '|')break;
		  cat[pos++]=trackdata[count];
		  cat[pos]='\0';
		  count++;
		}
		count++;
		pos=0;
		// get class
		while(count<last)
		{
		  if (trackdata[count] == '|')break;
		  val[pos++]=trackdata[count];
		  val[pos]='\0';
		  count++;
		}
		count++;
		char Mess[1024];

		wsprintf(Mess,"Info:=%s =%s",cat,val);

		if (!strcmp(cat,"Name"))
			setCircuitName(val);
		if (!strcmp(cat,"Country"))
			setCircuitCountry(val);
		if (!strcmp(cat,"Author"))
			setCircuitAuthor(val);
		if (!strcmp(cat,"Year"))
			setCircuitYears(val);
		if (!strcmp(cat,"Event"))
			setCircuitEvent(val);
		if (!strcmp(cat,"Desc"))
			setCircuitDesc(val);
		if (!strcmp(cat,"Created"))
			setCircuitCopyright(val);
	    if (!strcmp(cat,"Laps"))
			setCircuitLaps(atoi(val));
		if (!strcmp(cat,"Slot"))
			setBestSlotInfo(atoi(val));
		if (!strcmp(cat,"Tyre"))
			setTyreConsumption(atoi(val));
		if (!strcmp(cat,"LapRecord"))
			setLapRecord(val);
		if (!strcmp(cat,"LapRecordQualify"))
			setLapRecordQualify(val);
		if (!strcmp(cat,"Split1")){
			sscanf(val,"%x",&split1);
		}
		if (!strcmp(cat,"Split2")){
			sscanf(val,"%x",&split2);
		}

		

		//AfxMessageBox(Mess,MB_OK);
		if (trackdata[count]=='#')break;

		memset(val,0,256);
	}
}


void GPTrack::ReadTrackFile(CDocument *pDoc,const char * filename)
{
	FILE *fp;

	fp = fopen(filename,"rb");
	if (!fp)
		AfxMessageBox("Failed to open TrackFile for reading",MB_OK);

	int count = 0;

	count = GetLengthofFile(filename);
	setTrackName(filename);

	if (trackdata!=NULL && fp!=NULL)
	{
      fread(trackdata,count,1,fp);
	}
	else
	{
		
	    AfxMessageBox("Failed to malloc Track Data",MB_OK);

		valid = FALSE;
		if (fp)
	       fclose(fp);
		return;
	}

	fileLength = count;

	ReadInfo();
	

	if (fp)
	  fclose(fp);

	valid = TRUE;

	
	ParseTrack(pDoc);
	ReadObjNames();
	//ReadAnnotations();
	//if (showReadObjNames) ReadObjNames();
}

int GPTrack::ReadTrackInfoFile(const char * filename)
{
	FILE *fp;

	fp = fopen(filename,"rb");
	if (!fp)
	{
		AfxMessageBox("Failed to open TrackFile for reading",MB_OK);
		return 0;
	}

	int count = 0;

	count = GetLengthofFile(filename);
	setTrackName(filename);

	if (trackdata!=NULL && fp!=NULL)
	{
      fread(trackdata,count,1,fp);
	}
	else
	{	
	    AfxMessageBox("Failed to malloc Track Data",MB_OK);

		valid = FALSE;
		if (fp)
	       fclose(fp);
		return 0;
	}

	fileLength = count;

	if (fileLength < 65535)
	{
	    ReadInfo(FALSE);
		valid = TRUE;
	}
	else
	{
		AfxMessageBox("Dat files exist that are not track files");
		valid = FALSE;
	}

	

	if (fp)
	  fclose(fp);

	return 1;
}




int GPTrack::WriteTrackObjectDefinitions(int offset)
{
	for(int i=0;i<TrackObjectDefintions->size();i++)
    {
       TrackObjectDefinition *t = (TrackObjectDefinition*)TrackObjectDefintions->elementAt(i);

	   t->write(this,offset);
	   offset+=16;
	}
	return offset;
}

int GPTrack::WritePitLane(int offset)
{
	for(int i=0;i<PitlaneSections->size();i++)
    {
       TrackSection *t = (TrackSection*)PitlaneSections->elementAt(i);

	   Vector *trackCommands = t->getCommands();

	   for(int j=0;j<trackCommands->size();j++)
	   {
		   TrackCmd *tcmd = (TrackCmd*)trackCommands->elementAt(j);
		   offset = tcmd->write(this,offset);
	   }
	   offset = t->write(this,offset);
	}
	WritePCWord(offset,0xFFFF);	offset+=2;
	return offset;
}

int GPTrack::WriteCCLine(int offset)
{
	WritePCByte(offset,(int)CCLineStartX.GetValue());offset+=1;
	WritePCByte(offset,CCLineStartXHigh.GetValue());offset+=1;

	//WritePCWord(offset,(int)CCLineStartYData.GetValue());offset+=2;
	WritePCWord(offset,(int)CCLineStartY.GetValue());offset+=2;

	WritePCWord(offset,CCUnknown1.GetValue());offset+=2;
	WritePCWord(offset,CCUnknown2.GetValue());offset+=2;
	WritePCWord(offset,CCUnknown3.GetValue());offset+=2;
	WritePCWord(offset,CCUnknown4.GetValue());offset+=2;

	for(int i=0;i<CCLineSections->size();i++)
    {
       CCLineSection *t = (CCLineSection*)CCLineSections->elementAt(i);
	   offset = t->write(this,offset);
	}
	
	WritePCWord(offset,0x0000);offset+=2;
	return offset;
}

int GPTrack::WriteCameras(int offset)
{	
	for(int i=0;i<TrackCameras->size();i++)
    {
	    Camera *cam = (Camera*)TrackCameras->elementAt(i);
        cam->write(this,offset);
		offset+=32;
    }
	WritePCWord(offset,0xFFFF);
	offset+=2;
	return offset;
}

int GPTrack::WriteSetup(int offset)
{
	offset = carsetup->write(this,offset);
	return offset;
}



void GPTrack::WriteTrackFile(const char * outfilename)
{
	FILE *fp;

	fp        = fopen(outfilename,"wb");
	if (!fp)
	{
		AfxMessageBox("Failed to open TrackFile for writing",MB_OK);
		return;
	}

	if (isF1GP())
	{

	}
	else {
		WriteInfo();
		WriteObjNames(fp);
		//WriteAnnotations();	
	}	
	CalculateCheckSum();
	WriteSum(fileLength-4,TRUE,FALSE);

	if (trackdata!=NULL && fp!=NULL)
	{
	  fwrite(trackdata,fileLength,1,fp);
	}
	else
	{
		
	    AfxMessageBox("Failed to malloc Track Data",MB_OK);

		valid = FALSE;
		if (fp)
	       fclose(fp);
		return;
	}

	if (fp)
	  fclose(fp);

}

// recreate a new trackdata structure made up purely from components
void GPTrack::RecreateData()
{
	if (isF1GP())
	{
		AfxMessageBox("Track being written as F1GP file");
		// write F1GP horizon
		for(int i =0;i<4096;i++)
		{
			// do nothing
		}
		//ListObjectOffset  = WriteOffsets();
		WriteOffsets();
		TrackDataOffset   = WriteTrackObjectDefinitions(ListObjectOffset);
		int CCStartOffset = WriteTrack(TrackDataOffset);
	}
	else {
		WriteInfo();
		ListObjectOffset  = WriteOffsets();
		TrackDataOffset   = WriteTrackObjectDefinitions(ListObjectOffset);
		int CCStartOffset = WriteTrack(TrackDataOffset);
		int EndOfCCLine   = WriteCCLine(CCStartOffset);
		// EndOfCCLine == SeupOffset
		// comment this out to get to work
		// gap increased by 32
		if (CCLineToSetupGap > 4)
		{
		int offsetA;
		for(int i=0;i<CCLineToSetupGap-3;i++)
		{
			WritePCByte(EndOfCCLine+i,0x00);
			offsetA = EndOfCCLine+i; 
		}
		//write out identifier
		WritePCByte(offsetA,'P');offsetA++;
		WritePCByte(offsetA,'D');offsetA++;
		WritePCByte(offsetA,'H');offsetA++;
		}

		SetupOffset           =  EndOfCCLine+CCLineToSetupGap;	// error
		int EndOfSetup        = WriteSetup(SetupOffset);
		// this should be uncommented
		// EndOfSetup ==  TrackPitLaneStart
		TrackPitLaneStart     =  EndOfSetup; //error
		int CameraOffset      = WritePitLane(TrackPitLaneStart);
		int EndOfCameras      = WriteCameras(CameraOffset);	
		int EndOfVariousData  = WriteVariousData(EndOfCameras);
		int offset            = WriteJamFiles(EndOfVariousData);
		// comment this out to get to work
		CheckSumOffset = offset;//error
		WriteOffsets();
		fileLength=CheckSumOffset+4;
	}
}



void GPTrack::ParseTrack(CDocument *pDoc)
{
	setValid(TRUE);
	ReadOffsets(pDoc);

	ReadTrackObjectDefinitions(ListObjectOffset,TrackDataOffset);
	int EndOfTrack   = ReadTrack(TrackDataOffset);
	int EndOfCCLine;

	if (!isF1GP())
	{
		int EndOfPitLane = ReadPitLane(TrackPitLaneStart);
		EndOfCCLine      = ReadCCLine(EndOfTrack);


		CameraOffset     = EndOfPitLane;
		int EndOfCameras = ReadCameras(CameraOffset);
		int EndOfSetup   = ReadSetup(SetupOffset);
		int EndOfVariousData = ReadVariousData(EndOfCameras);
		ReadJamFiles(EndOfVariousData);
	}
	else {
		//EndOfTrack+=36;
		EndOfTrack = 0x3c4b;
		EndOfCCLine  = ReadPitLane(EndOfTrack);
	}
		createTrack(NULL);

		BOOL resized = FindTopLeft();
		if (resized)
			createTrack(NULL);

	if (!isF1GP())
	{
		createCC();

		CCLineToSetupGap = (SetupOffset - EndOfCCLine);// 32 counter act commands
	}

	//ReadAnnotations();
}

int  GPTrack::ReadVariousData(int start)
{
	VariousDataCount=0;
	while(1)
	{
	 int unknown  = getTrackDataWord(start);			  
	 start+=2;
	 VariousData[VariousDataCount++].SetValue(unknown);
	 if (unknown == 0xFFFF)break;
	}
	return start;
}

int  GPTrack::WriteVariousData(int start)
{
	for(int i=0;i<VariousDataCount;i++)
	{
	  WritePCWord(start,VariousData[i].GetValue());
	  start+=2;
	}
	return start;
}

extern CPoint GetTrackPoint(double currrad,double angle,double length);

void drawVariousPt(Display *g,TrackSection *t,double inDistance,int val,int index)
{
         double nextypos;
		 double nextxpos;
		 double angle   = t->getAngle();
		 double stAngle = t->getStartAngle();
		 double x1 = t->getStartX();
		 double y1 = t->getStartY();
		 char  buffer[10];
		 wsprintf(buffer,"(%d)%d",index,val);

		 if (angle != 0)
		 {
		  CPoint trackPt = GetTrackPoint(stAngle,angle,inDistance);
          nextypos = trackPt.x;
          nextxpos = trackPt.y;
		  angle = DegToRad(angle*t->getLength())*(inDistance/t->getLength());
		 }
		 else
		 {
		  nextypos = inDistance*sin(stAngle);
          nextxpos = inDistance*cos(stAngle);
		  angle = stAngle;
		 }
		 
		 double xr = x1+nextxpos;
		 double yr = y1+nextypos;

		 g->drawLine(xr-2,yr,xr+2,yr);
		 g->drawLine(xr,yr-2,xr,yr+2);
		 g->drawLine(x1,y1,xr,yr);
         g->setColor(0);	
		 g->drawText(xr,yr,buffer);
}


void GPTrack::drawVarious(Display *g)
{
	int count = 0;
	for(int i=50;i<VariousDataCount-1;i+=2)
	{
	  int distance = VariousData[i].GetValue();
	  int val = VariousData[i+1].GetValue();
	  
	  
      TrackSection *varInSection = SectorForLength(distance);

	  double distance_to_section = LengthToTrackSector(varInSection);

	  double sectionCamDist = distance-distance_to_section;						   
	  
	  //draw various value
	  drawVariousPt(g,varInSection,sectionCamDist,val,count);
	  count++;
	}
}


int  GPTrack::ReadJamFiles(int start)
{
	JamFileWritingStartOffset = start;
	// read past the last FF
	int data = ReadPCWord(start);
	start+=2;

	// read until valid data
	JamZeroCount=2;
	while(data == 0)
	{
		data = ReadPCWord(start);
		if (data!=0) break;
		start+=2;
		JamZeroCount+=2;
	}

	JamFileUnknown.SetValue(ReadPCWord(start));
	start+=2;

	int AlwaysZero = ReadPCWord(start);
	start+=2;

	JamFileWritingOffset = start;

	int NumberOfJamFiles = ReadPCWord(start);
	start+=2;


	for(int i=0;i<NumberOfJamFiles;i++)
	{
	  char *jamfile = ReadNullTerminatedString(start);
	  if (jamfile)
	  {
	   start+=strlen(jamfile)+1;
	   char *ext = strrchr(jamfile,'.');
	   if (ext && !strcmp(ext,".jam"))
	   {
	    JamFileEditor *jam = new JamFileEditor(this,jamfile);
	    JamFiles->addElement(jam);
	   }
	   free(jamfile);
	  }
	}
	//
	return 0;
}

int GPTrack::WriteJamFiles(int start)
{
	// blank spaces
	for(int i=0;i<JamZeroCount;i++)
	{
	   WritePCByte(start,0x00);start++;
	}
	WritePCWord(start,JamFileUnknown.GetValue());
	start+=2;
	// zero probably previous high bit of DWORD
	WritePCWord(start,0x00);
	start+=2;
	int size = numberOfJams(JamFiles);
	// must write the unknown ?
	WritePCWord(start,size);
	start+=2;

	for(i=0;i<JamFiles->size();i++)
	{
	  JamFileEditor *jam = (JamFileEditor*)JamFiles->elementAt(i);
	  if (jam->Internal) continue;
	  char *jamfile = jam->getJamFileName();
	  int len = strlen(jamfile)+1;
	  WriteNullTermintedString(start,jamfile,len);
	  start+=len;
	}
	//CheckSumOffset = start;
	return start;
}

void GPTrack::WriteNullTermintedString(int start,char *file,int len)
{
	char jamchar;

	for(int i=0;i<len;i++)
	{
	   jamchar = file[i];
	   WritePCByte(start,jamchar);
	   start++;
	}
}

char * GPTrack::ReadNullTerminatedString(int start)
{
	char *buffer;
	char jamchar = (char)1;

	buffer = (char*)malloc(257*sizeof(char));
	memset(buffer,0,256);

	int count=0;
	while(jamchar != 0)
	{
	   jamchar = ReadPCByte(start);
	   start++;
	   buffer[count++] = jamchar;
	   if (count >=255) 
	   {
		   break;
	   }
	}
	return buffer;
}


typedef struct{
		int offset;
		int index;
		int size;
} t_item;

int compareOffset( const void *arg1, const void *arg2 )
{
   /* Compare all of both offsets: */
   t_item *a = (t_item*)arg1;
   t_item *b = (t_item*)arg2;
   return a->offset - b->offset;
}
int compareIndex( const void *arg1, const void *arg2 )
{
   /* Compare all of both index: */
   t_item *a = (t_item*)arg1;
   t_item *b = (t_item*)arg2;
   return a->index - b->index;
}


void GPTrack::ReadOffsets(CDocument *pDoc)
{
	int offsetstart=0x1000;

    LargeNumber1.SetValue(ReadPCDWord(offsetstart));
    offsetstart+=4;

	if (LOWORD(LargeNumber1.GetValue()) == 0x1010)
	{
		if (AfxMessageBox("Is this a F1GP File?",MB_YESNO)==IDYES)
		{
			((CTrackEditorDoc*)pDoc)->F1GP = true;
		}
	}

    LargeNumber2.SetValue(ReadPCDWord(offsetstart));
    offsetstart+=4;
	if (isF1GP())
	{
		CheckSumOffset      = ReadPCWord(offsetstart)+0x1010;
		offsetstart+=2;
		ListObjectOffset    = ReadPCWord(offsetstart)+0x1010;
		offsetstart+=2;
		TrackDataOffset     = ReadPCWord(offsetstart)+0x1010;
		offsetstart+=2;
		NumberOfObjects     = ReadPCWord(offsetstart);
		offsetstart+=2;
	}
	else {
		CheckSumOffset      = ReadPCDWord(offsetstart)+0x1020;
		offsetstart+=4;
		ListObjectOffset    = ReadPCDWord(offsetstart)+0x1020;
		offsetstart+=4;
		TrackDataOffset     = ReadPCDWord(offsetstart)+0x1020;
		offsetstart+=4;
		SetupOffset         = ReadPCDWord(offsetstart)+0x1020;
		offsetstart+=4;
		TrackPitLaneStart   = ReadPCDWord(offsetstart)+0x1020;
		offsetstart+=4;
		NumberOfObjects     = ReadPCDWord(offsetstart);
		offsetstart+=4;

	
		t_item objoff[100];

		for(int i=0;i<NumberOfObjects;i++)
		{
			int ObjectOffset = ReadPCDWord(offsetstart);
			offsetstart+=4;
			objoff[i].offset=ObjectOffset;			
			objoff[i].index=i;			
			objoff[i].size=-1;	
		}
		qsort( (void *)objoff, (size_t)NumberOfObjects, sizeof( t_item ), compareOffset );
		for(i=0;i<NumberOfObjects;i++)
		{
			if (i != NumberOfObjects-1)
			{
			 objoff[i].size = objoff[i+1].offset - objoff[i].offset;
			 }
			else
			{
			 objoff[i].size = (ListObjectOffset-0x1020) - objoff[i].offset;
			}
		}
		qsort( (void *)objoff, (size_t)NumberOfObjects, sizeof( t_item ), compareIndex );
		for(i=0;i<NumberOfObjects;i++)
		{
			int ObjectOffset = objoff[i].offset;
			int nextOffset   = ObjectOffset+objoff[i].size;
			InternalObject *obj = new InternalObject(this,pDoc,ObjectOffset+0x1020,nextOffset+0x1020);
			obj->ReadObject(this);
			InternalObjectOffsets->addElement(obj);
		}
	}
}




int GPTrack::WriteOffsets()
{
	int offsetstart=0x1000;
	WritePCDWord(offsetstart,LargeNumber1.GetValue());offsetstart+=4;
	WritePCDWord(offsetstart,LargeNumber2.GetValue());offsetstart+=4;

	if (isF1GP())
	{
		WritePCWord(offsetstart,CheckSumOffset-0x1010);offsetstart+=2;	//second pass	
		WritePCWord(offsetstart,ListObjectOffset-0x1010);offsetstart+=2; //second pass
		WritePCWord(offsetstart,TrackDataOffset-0x1010);offsetstart+=2; //second pass		
		WritePCWord(offsetstart,NumberOfObjects);offsetstart+=2;
	}
	else {
		WritePCDWord(offsetstart,CheckSumOffset-0x1020);offsetstart+=4;	//second pass	
		WritePCDWord(offsetstart,ListObjectOffset-0x1020);offsetstart+=4; //second pass
		WritePCDWord(offsetstart,TrackDataOffset-0x1020);offsetstart+=4; //second pass
		WritePCDWord(offsetstart,SetupOffset-0x1020);offsetstart+=4; //second pass
		WritePCDWord(offsetstart,TrackPitLaneStart-0x1020);offsetstart+=4; //second pass

		NumberOfObjects	= InternalObjectOffsets->size(); //first pass

		WritePCDWord(offsetstart,NumberOfObjects);offsetstart+=4;

		for(int i=0;i<NumberOfObjects;i++)
		{
		  InternalObject *obj = (InternalObject*)InternalObjectOffsets->elementAt(i);
		  
		  WritePCDWord(offsetstart,(obj->getOffset())-0x1020);
		  offsetstart+=4;
		}
		
		int nInternals = InternalObjectOffsets->size();
		for(i=0;i<nInternals;i++)
		{
		  InternalObject *obj = (InternalObject*)InternalObjectOffsets->elementAt(i);
		  offsetstart = obj->write(this,0);
		}
	}

	return  offsetstart;
}

int GPTrack::WriteTrack(int offset)
{
	WritePCWord(offset,TrackStartAngle.GetValue());offset+=2;
	WritePCWord(offset,TrackStartClimb.GetValue());offset+=2;
	WritePCWord(offset,TrackStartY.GetValue());offset+=2;
	WritePCWord(offset,TrackStartZ.GetValue());offset+=2;
	WritePCWord(offset,TrackStartX.GetValue());offset+=2;
	WritePCWord(offset,TrackBeginWidth.GetValue());offset+=2;

	WritePCWord(offset,TrackUnknown1.GetValue());offset+=2; // unknown
	WritePCWord(offset,C5Code.GetValue());offset+=2; // c5code
	WritePCByte(offset,BeginTrackWidthRight.GetValue());offset+=1;
	WritePCByte(offset,BeginTrackWidthLeft.GetValue());offset+=1;

	
    WritePCWord(offset,UnknownNumber.GetValue());offset+=2;

    for(int j=0;j<UnknownNumberCount;j++)
	{
		WritePCWord(offset,UnknownNumbers[j].GetValue());offset+=2;
	}

	int numSections =TrackSections->size(); 
	for(int i=0;i<numSections;i++)
    {
       TrackSection *t = (TrackSection*)TrackSections->elementAt(i);

	   Vector *trackCommands = t->getCommands();

	   int numCmds = trackCommands->size();
	   for(int j=0;j<numCmds;j++)
	   {
		   TrackCmd *tcmd = (TrackCmd*)trackCommands->elementAt(j);
		   offset = tcmd->write(this,offset);
	   }
	   if (t->index!=-99)
	   {
	    offset = t->write(this,offset);
	   }
	}
	WritePCWord(offset,0xFFFF);offset+=2;
	return offset;
}

void GPTrack::ExportTrackCommands(LPCSTR filename)
{
  FILE *fp = fopen(filename,"w");

  for(int i=0;i<TrackSections->size();i++)
  {
       TrackSection *t = (TrackSection*)TrackSections->elementAt(i);

	   fprintf(fp,"t%d(%d)\t",i,t->getLength());

	   Vector *trackCommands = t->getCommands();

	   for(int j=0;j<trackCommands->size();j++)
	   {
		   TrackCmd *tcmd = (TrackCmd*)trackCommands->elementAt(j);
		   if (j>0) fprintf(fp,"\t");

		   fprintf(fp,"0x%x",tcmd->getCmd());
		   for(int k=0;k<tcmd->getCmdNumArgs();k++)
		   {   
			   fprintf(fp,"\t");
			   fprintf(fp,"%d",tcmd->getNthArg(k+1));
		   }
		   fprintf(fp,"\n");
	   }
	   fprintf(fp,"\n");
  }
  for( i=0;i<PitlaneSections->size();i++)
  {
       TrackSection *t = (TrackSection*)PitlaneSections->elementAt(i);

	   fprintf(fp,"p%d(%d)\t",i,t->getLength());

	   Vector *trackCommands = t->getCommands();

	   for(int j=0;j<trackCommands->size();j++)
	   {
		   TrackCmd *tcmd = (TrackCmd*)trackCommands->elementAt(j);
		   if (j>0) fprintf(fp,"\t");

		   fprintf(fp,"0x%x",tcmd->getCmd());
		   for(int k=0;k<tcmd->getCmdNumArgs();k++)
		   {   
			   fprintf(fp,"\t");
			   fprintf(fp,"%d",tcmd->getNthArg(k+1));
		   }
		   fprintf(fp,"\n");
	   }
	   fprintf(fp,"\n");
  }
  

  fclose(fp);
}



int GPTrack::DebugTrack(FILE *fp,int offset)
{
	fprintf(fp,"TrackStartAngle:%d\n",TrackStartAngle);
	fprintf(fp,"TrackStartClimb:%d\n",TrackStartClimb);
	fprintf(fp,"TrackStartX:%d\n",TrackStartX);
	fprintf(fp,"TrackStartZ:%d\n",TrackStartZ);
	fprintf(fp,"TrackStartY:%d\n",TrackStartY);
	fprintf(fp,"TrackBeginWidth:%d\n",TrackBeginWidth);
	fprintf(fp,"TrackUnknown1:%d\n",TrackUnknown1);
	fprintf(fp,"C5Code:%d\n",C5Code);
	fprintf(fp,"TrackWidthRight:%d\n",BeginTrackWidthRight);
	fprintf(fp,"TrackWidthLeft:%d\n",BeginTrackWidthLeft);

	fprintf(fp,"UnknownNumber:%d\n",UnknownNumber);
	fprintf(fp,"UnknownNumberCount:%d\n",UnknownNumberCount);

    for(int j=0;j<UnknownNumberCount;j++)
	{
		fprintf(fp,"0x%x UnknownNumber%d:%d\n",offset,j,UnknownNumbers[j]);
	}

	for(int i=0;i<TrackSections->size();i++)
    {
       TrackSection *t = (TrackSection*)TrackSections->elementAt(i);

	   Vector *trackCommands = t->getCommands();

	   for(int j=0;j<trackCommands->size();j++)
	   {
		   TrackCmd *tcmd = (TrackCmd*)trackCommands->elementAt(j);
		   offset = tcmd->debug(fp,this,offset);
		   
	   }
	   offset = t->debug(fp,this,offset);
	}
	fprintf(fp,"End of Track 0xFFFF");


	for(i=0;i<CCLineSections->size();i++)
	{
	   CCLineSection *t = (CCLineSection*)CCLineSections->elementAt(i);
	   
	   offset = t->debug(fp,this,offset);
	}
	fprintf(fp,"End of CCLine 0xFFFF");
	return offset;
}


int GPTrack::ReadTrack(int offset)
{
	int gripLevel = 0;

    TrackStartAngle.SetValue(getTrackDataWord(offset));
    offset+=2;
    TrackStartClimb.SetValue(HeightTo(getTrackDataWord(offset)));
    offset+=2;
    TrackStartY.SetValue((short)getTrackDataWord(offset));
    offset+=2;
    TrackStartZ.SetValue(getTrackDataWord(offset));
    offset+=2;
    TrackStartX.SetValue((short)getTrackDataWord(offset));
    offset+=2;
    TrackBeginWidth.SetValue(getTrackDataWord(offset));
    
	TrackWidthLeft=TrackWidthRight=TrackBeginWidth.GetValue();

    offset+=2;
    TrackUnknown1 = getTrackDataWord(offset);
    offset+=2;

    int C5CodeL  = getTrackDataByte(offset);
    int C5CodeH  = getTrackDataByte(offset+1);
    C5Code   = getTrackDataWord(offset);
    offset+=2;

    BeginTrackWidthRight.SetValue(getTrackDataByte(offset));
    offset+=1;
    BeginTrackWidthLeft.SetValue(getTrackDataByte(offset));
    offset+=1;

    UnknownNumber.SetValue(getTrackDataWord(offset));
    offset+=2;
    UnknownNumberCount=UnknownNumber.GetValue();

    if      (UnknownNumber.GetValue() == 3) UnknownNumberCount = 4;
    else if (UnknownNumber.GetValue() == 4) 
	{
		UnknownNumberCount = 6;
	}
	else
	{
		AfxMessageBox("Unknown Track Begin Number",MB_OK);
	}

    for(int i=0;i<UnknownNumberCount;i++)
    {
        UnknownNumbers[i].SetValue(getTrackDataWord(offset));
        offset+=2;
    }

	if (UnknownNumber.GetValue() == 4)
	{
		int data;
		//eat any extra space!
		while((data = getTrackDataWord(offset))<0xFF)
		{
			UnknownNumbers[UnknownNumberCount].SetValue(getTrackDataWord(offset));
			UnknownNumberCount++;
			offset+=2;
		}
	}

            
   	Vector *SectionCmds  = new Vector("ReadTrack SectionCmds",__LINE__);

	int count = 1;
    while(count)
    {
               int TrackCmdValue  = getTrackDataByte(offset);
               offset+=1;
               int TrackCmd       = getTrackDataByte(offset);
               offset+=1;

			   //TRACE("Cmd = %x %x\n",TrackCmdValue,TrackCmd);

               switch(TrackCmd)
               {
                  case 0xa1:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // entry from pit road
                  case 0xa2:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // entry from pit road
                  case 0xa3:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // exit from pit road
                  case 0xa4:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // exit from pit road
                  case 0xa7:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // unknown
                  case 0xa8:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
                  case 0xa6:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // unknown
                  case 0xaa:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,3);break;
                  case 0xab:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break;
                  case 0xac:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,4);break;
                  case 0xaf:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // triangle
				  case 0xad:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // triangle

                  case 0xb4:  
					  {
						  //offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // trackWidthChangeLeft
						  TrackWidthLeft   = GetTrackWidthLeft(SectionCmds,TrackCmdValue,offset,TRUE,0xb4);
						  offset+=4;
					  }break;
                  case 0xb5:  
					  {
						  //offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);// trackWidthChangeRight
						  TrackWidthRight   = GetTrackWidthLeft(SectionCmds,TrackCmdValue,offset,FALSE,0xb5);
						  offset+=4;
					  }break;
                  case 0xb0:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
				  case 0xb1:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
				  case 0xb2:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
				  case 0xb3:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xb9:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // polygon
				  case 0xb6:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
				  case 0xb7:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xb8:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,13);break; // trees
                  case 0xba:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // unknown
                  case 0xbb:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,4);break; // unknown
                  case 0xbd:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // sun
                  case 0xbe:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // unknown
                  case 0xbf:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // unknown
                  case 0xbc:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,7);break; // texture

                  case 0xca:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,4);break; // kerb
                  case 0xcb:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,4);break; // kerb
				  case 0xcc:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // horizon
                  case 0xc0:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xc3:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // unknown
                  case 0xc4:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,3);break; // unknown
                  case 0xc1:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xc2:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // unknown
                  case 0xc5:
                      {
                       if (C5CodeH==13)
                         offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,6); // unknown                       
                       if (C5CodeH==31)
                         offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,7); // unknown                       
                       if (C5CodeH==29)
                         offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,6); // unknown                       
                       if (C5CodeH==159)
                         offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,7); // unknown
           
                      }
                      break;
                  case 0xc6:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xc9:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,8);break; // lights
                  case 0xc8:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,7);break; // lights
                  case 0xcd:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,3);break; // unknown
                  case 0xc7:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // unknown
                  case 0xce:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xcf:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown

                   
                  case 0xd0:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // unknown
                  case 0xd1:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
                  case 0xd2:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xd3:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xd4:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // show pitlane exit
                  case 0xd5:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xd6:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // show pitlane entry
                  case 0xd7:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
                  case 0xd8:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
                  case 0xd9:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xda:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xdc:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xdb:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
				  case 0xdd:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
                  case 0xde:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,3);break; // black flag left
                  case 0xdf:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,3);break; // black flag

                  case 0xe0:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // kirb height
				  //case 0xe1:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // kirb height
				  case 0xe1:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
				  case 0xe2:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
				  case 0xe3:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
				  case 0xe4:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
				  case 0xe5:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown

                  case 0x80:  
                      {
                          TrackObject *obj = GetTrackObject(offset,TrackCmdValue,SectionCmds);
                          offset+=2;
                          //TrackObjects->addElement(obj);
                      }
                      break; // object
                  case 0x81:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // distantce 
                  case 0x82:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // distantce
                  case 0x83:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
                  case 0x84:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
                  case 0x85:  
					  { 						   
                           int TrackWidth   = GetTrackWidth(SectionCmds,TrackCmdValue,offset);
						   TrackWidthLeft   = TrackWidth;
						   TrackWidthRight  = TrackWidth;
						   offset+=4;
				      }	
					  break;
				  case 0x86:  
                      {
                          offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);
                          PitLaneEntryIndex = TrackSections->size(); 
                      } break; // pitlane entry
				  case 0x87:  
					  {
						  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);
						  PitLaneExitIndex = TrackSections->size();
					  }break; // pitlane exit
                  
                  case 0x8a:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,5);break; // track marking
                  case 0x8b:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,5);break; // track marking
				  case 0x8c:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
				  case 0x8d:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0x8e:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // kerb left
                  case 0x8f:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // kerb right
                  
                  
                  case 0x00:  
                      {
                      offset = GetTrackSection(offset,TrackCmdValue,NULL,TrackWidthLeft,TrackWidthRight,SectionCmds,gripLevel);
                      //TrackObjects = new Vector("New TrackObjects",__LINE__);
					  SectionCmds  = new Vector("New SectionCmds",__LINE__);
                      }
                      break; // track

                  case 0xff:   
					  {
						  if (TrackCmdValue != 0xff)
							  AfxMessageBox("Not true end of message");
						  if (SectionCmds) delete SectionCmds;
						  count = 0;
					  }
					  break;

                  case 0x90:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0x91:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0x92:  
					  {
						  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1); // track grid
					  }	break;
				  case 0x93:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0x94:  
					  {
						  //offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // track grid
						  gripLevel = GetTrackGrip(SectionCmds,TrackCmdValue,offset,0x94);
						  offset+=2;
					  }	break;
                  case 0x95:  
					  {
						  //offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);// unknown
						  gripLevel = GetTrackGrip(SectionCmds,TrackCmdValue,offset,0x95);
						  offset+=2;
					  }	break;
                  case 0x98:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // changing fence
                  case 0x99:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // changing fence right
                  case 0x9a:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // changing fence left
				  case 0x9b:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // changing fence left

				  case 0x9c:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
				  case 0x9d:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
                  
				  //case 0x9c:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break;
				  case 0xA5:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break;
				  case 0xA9:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break;
				  //case 0xDB:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break;


           

                  default: 
                      {
						  char buffer[256];

						  wsprintf(buffer,"Unknown Code %d",TrackCmd);
						  AfxMessageBox(buffer,MB_OK);
                          count = 0;
                          break;
                      }

               }
            }
			return offset;
        }

        int GPTrack::GetNWordUnknownCommand(Vector *SectionCmds,int cmd,int arg1,int offset,int num)
        {
		   TrackCmd *trackcmd = new TrackCmd(this,NULL,cmd,offset-2,"Unknown TrackCmd");
		   trackcmd->AddWordArgument(arg1,"Unknown Arg");

           for(int i=0;i<num;i++)
           {
            int Unknown  = getTrackDataWord(offset);
            offset+=2;
			trackcmd->AddWordArgument(Unknown,"Unknown Arg");
           }

		   SectionCmds->addElement(trackcmd);
           return offset;
        }

		int GPTrack::GetTrackWidth(Vector *SectionCmds,int arg1,int offset)
		{
		   TrackCmd *trackcmd = new TrackCmd(this,NULL,0x85,offset-2,"Track Width");
		   
		   int Position  = getTrackDataWord(offset); offset+=2;
           int TrackWidth    = getTrackDataWord(offset); offset+=2;
		   trackcmd->AddWordArgument(arg1,"Unknown");
           trackcmd->AddWordArgument(Position,"Position");
		   trackcmd->AddWordArgument(TrackWidth,"Track Width");
		   SectionCmds->addElement(trackcmd);

           return TrackWidth;
		}
		int GPTrack::GetTrackGrip(Vector *SectionCmds,int arg1,int offset,int cmd)
		{
		   TrackCmd *trackcmd = new TrackCmd(this,NULL,cmd,offset-2,"Track Grip");
		   
		   //int Position      = getTrackDataWord(offset); offset+=2;
           int GripLevel     = getTrackDataWord(offset); offset+=2;
		   trackcmd->AddWordArgument(arg1,"Unknown");
           trackcmd->AddWordArgument(GripLevel,"Grip Level");
		   SectionCmds->addElement(trackcmd);

           return GripLevel;
		}

		int GPTrack::GetTrackWidthLeft(Vector *SectionCmds,int arg1,int offset,BOOL left,int cmd)
		{
		   TrackCmd *trackcmd = new TrackCmd(this,NULL,cmd,offset-2,"Track Width Left/Right");
		   
		   int Position      = getTrackDataWord(offset); offset+=2;
           int newTrackWidth    = getTrackDataWord(offset); offset+=2;
		   trackcmd->AddWordArgument(arg1,"Unknown");
           trackcmd->AddWordArgument(Position,"Position");
		   trackcmd->AddWordArgument(newTrackWidth,"Track Width");
		   SectionCmds->addElement(trackcmd);

           return newTrackWidth;
		}

         TrackObject* GPTrack::GetTrackObject(int offset,int len,Vector *SectionCmds)
        {
            int code  = getTrackDataWord(offset);
            int length = len;
            TrackObject *obj = new TrackObject(this,NULL,length,code,offset-2);

			
			obj->AddWordArgument(len,"Length");
		    obj->AddWordArgument(code,"Code");
		    SectionCmds->addElement(obj);
            return obj;
        }

         int GPTrack::GetTrackSection(int offset,int length,Vector *objects,double widthLeft,double widthRight,Vector *sectionCmds,int gripLevel)
        {
		   int trackDataOffset = offset-2;
           int angle  = getTrackDataWord(offset);
           offset+=2;
           int climb  = getTrackDataWord(offset);
           offset+=2;

           int RS  = getTrackDataByte(offset);
           offset+=1;
		   int CB  = getTrackDataByte(offset);
           offset+=1;

           int wr  = getTrackDataByte(offset);
           offset+=1;
           int wl  = getTrackDataByte(offset);
           offset+=1;

		   int idx = TrackSections->size();

           TrackSection *ts = new TrackSection(TrackSections,this,idx,length,angle,climb,wl,wr,RS,CB,objects,trackDataOffset,widthLeft,widthRight,sectionCmds,t_TrackSection);
		   ts->setGripLevel(gripLevel);
           
           TrackSections->addElement(ts);
           return offset;
        }

		 

		 int GPTrack::ReadPitLane(int offset)
        {
			int gripLevel=0;
            int count = 10000;
			TrackWidthLeft = PIT_LANE_DEFAULT_WIDTH;
			TrackWidthRight = PIT_LANE_DEFAULT_WIDTH;

			//if (offset%4){
			//	offset += (4-offset%4);
			//}

            Vector *SectionCmds    = new Vector("ReadPitLane New SectionCmds",__LINE__);
            
            while(count > 0)
            {
                count--;
               int TrackCmdValue  = getTrackDataByte(offset);
               offset+=1;
               int TrackCmd       = getTrackDataByte(offset);
               offset+=1;

               switch(TrackCmd)
               {
                  case 0xa0:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // end of pitlane box
                  case 0xa1:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // entry from pit road
                  case 0xa2:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // entry from pit road
                  case 0xa3:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // exit from pit road
                  case 0xa4:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // exit from pit road
                  case 0xa7:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // unknown
                  case 0xa8:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
                  case 0xa6:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // unknown
                  case 0xa9:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown pitlane
                  case 0xaa:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,3);break;
                  case 0xab:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break;
                  case 0xac:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,4);break;
				  case 0xad:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // triangle
                  case 0xaf:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // triangle

                  case 0xb4:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // trackWidthChangeLeft
                  case 0xb5:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // trackWidthChangeRight
				  case 0xb6:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
				  case 0xb7:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xb0:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
				  case 0xb1:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
				  case 0xb2:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
				  case 0xb3:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xb9:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // polygon
                  case 0xb8:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,13);break; // trees
                  case 0xba:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // unknown
                  case 0xbb:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,4);break; // unknown
                  case 0xbd:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // sun
                  case 0xbe:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // unknown
                  case 0xbf:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // unknown
                  case 0xbc:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,7);break; // texture

                  case 0xca:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,4);break; // kerb
				  case 0xcc:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // horizon
                  case 0xcb:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,4);break; // kerb
                  case 0xc0:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xc3:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // unknown
                  case 0xc4:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,3);break; // unknown
                  case 0xc1:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xc2:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // unknown
                  
                  case 0xc6:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xc9:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,8);break; // lights
                  case 0xc8:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,7);break; // lights
                  case 0xcd:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,3);break; // unknown
                  case 0xc7:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // unknown
                  case 0xce:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xcf:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown

                   
                  case 0xd0:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // unknown
                  case 0xd1:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
                  case 0xd2:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xd3:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xd4:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // show pitlane exit
                  case 0xd5:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xd6:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // show pitlane entry
                  case 0xd7:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
                  case 0xd8:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
                  case 0xd9:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xda:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xdc:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0xdb:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
				  case 0xdd:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
                  case 0xde:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,3);break; // black flag left
                  case 0xdf:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,3);break; // black flag

                  case 0xe0:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // kirb height
				  case 0xe1:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
				  case 0xe2:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
				  case 0xe3:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
				  case 0xe4:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
				  case 0xe5:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown

                  case 0x80:  
                      {
                          TrackObject *obj = GetTrackObject(offset,TrackCmdValue,SectionCmds);
                          offset+=2;
                      }
                      break; // object

                  case 0x81:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // distantce 
                  case 0x82:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // distantce
                  case 0x83:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
                  case 0x84:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
                  case 0x85:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // track width
                  case 0x86:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // pitlane entry
				  case 0x87:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // pitlane exit
                  case 0x88:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // pitlane unknown line?
                  case 0x89:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // pitlane unknown line? needed
                  case 0x8a:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,5);break; // track marking
                  case 0x8b:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,5);break; // track marking
				  case 0x8c:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
				  case 0x8d:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown                  
                  case 0x8e:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // kerb left
                  case 0x8f:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // kerb right
                  
                  
                  case 0x00:  
                      {
                      offset = GetPitLaneSection(offset,TrackCmdValue,NULL,TrackWidthLeft,TrackWidthRight,SectionCmds,gripLevel);
                      SectionCmds    = new Vector("ReadPitLane New SectionCmds",__LINE__);
                      }
                      break; // track
                  case 0xff:   
					  {
					   if (SectionCmds) delete SectionCmds;
					   count = 0;break;
					  }

                  case 0x90:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0x91:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0x92:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // track grid
					
				  case 0x93:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown

                  case 0x94:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // track grid
                  case 0x95:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // unknown
                  case 0x96:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // pitlane unknown
                  case 0x97:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // pitlane unknown
                  case 0x98:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // changing fence
                  case 0x99:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break; // changing fence right
                  case 0x9a:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,2);break; // changing fence left
                  case 0x9b:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // changing fence left

				  case 0x9c:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
				  case 0x9d:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // unknown
				  
                  case 0x9e:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // pitlane unknown
                  case 0x9f:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break; // pitlane unknown

           
				  //case 0x9c:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break;
				  case 0xA5:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break;
				  //case 0xA9:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,1);break;
				  //case 0xDB:  offset = GetNWordUnknownCommand(SectionCmds,TrackCmd,TrackCmdValue,offset,0);break;

                  default: 
                      {
						 CString msg;
						 msg.Format("Unknown Pitlane Command 0x%x, please report",TrackCmd);
						 AfxMessageBox(msg);
						 if (SectionCmds) delete SectionCmds;
                         count = 0;
                         break;
                      }

               }
            }
            return offset;
        }

		int GPTrack::GetPitLaneSection(int offset,int length,Vector *objects,double widthLeft,double widthRight,Vector *sectionCmds,int gripLevel)
        {
		   int trackDataOffset = offset-2;
           int angle  = getTrackDataWord(offset);
           offset+=2;
           int climb  = getTrackDataWord(offset);
           offset+=2;

           int RS  = getTrackDataByte(offset);
           offset+=1;
		   int CB  = getTrackDataByte(offset);
           offset+=1;

           int wr  = getTrackDataByte(offset);
           offset+=1;
           int wl  = getTrackDataByte(offset);
           offset+=1;

		   int idx = PitlaneSections->size();

           TrackSection *ts = new TrackSection(PitlaneSections,this,idx,length,angle,climb,wl,wr,RS,CB,objects,trackDataOffset,widthLeft,widthRight,sectionCmds,t_PitSection);
		   ts->setGripLevel(gripLevel);
           
           PitlaneSections->addElement(ts);
           return offset;
        }

		void GPTrack::ReadTrackObjectDefinitions(int start,int end)
        {
			int NumberOfObjectDefinition = (end - start)/16;
			int offset = start;

			for(int i=0;i<NumberOfObjectDefinition;i++)
			{
			   GetTrackObjectDefinition(offset);
			   offset+=16;
			}
		}

		int GPTrack::ReadSetup(int offset)
		{

			carsetup = new CCCarsetup(this,offset);

			offset = carsetup->read(this,offset);
			return offset;
		}

		int GPTrack::ReadCameras(int offset)
		{
			int NumberOfCameras = 100;


			for(int i=0;i<NumberOfCameras;i++)
			{
			   Camera *cam = GetCamera(offset,i);
			   if (cam == NULL) 
			   {
				   // if we are breaking out
				   offset+=2;
				   break;
			   }
			   offset+=32;
			   TrackCameras->addElement(cam);
			}
			return offset;
		}

		Camera *GPTrack::GetCamera(int offset,int num)
		{
			Camera *cam = new Camera(this,offset);
			cam->setNumber(num);

			offset = cam->read(this,offset);

			if (cam->getDistance() == 0xFFFF) 
			{
				delete cam;
				return NULL;
			}

			return cam;
		}

int GPTrack::ReadCCLine(int offset)
        {
	        int Data1l  = getTrackDataByte(offset);offset+=1;
			int Data1h  = getTrackDataByte(offset);offset+=1;
			short Data2  = (short)getTrackDataWord(offset);offset+=2;
			CCUnknown1.SetValue(getTrackDataWord(offset));offset+=2;
			CCUnknown2.SetValue(getTrackDataWord(offset));offset+=2;
			CCUnknown3.SetValue(getTrackDataWord(offset));offset+=2;
			CCUnknown4.SetValue(getTrackDataWord(offset));offset+=2;

			CCLineStartY.SetValue(Data2);
			
			CCLineStartX.SetValue(Data1l);
			CCLineStartXHigh.SetValue(Data1h);
			//CCLineStartYData.SetValue(Data2);
			
			int count = 10000;
            int totalLength=0;

            while(count > 0)
            {
			   int trackoffset = offset;
                count--;
               int TrackCmdValue  = getTrackDataByte(offset);
               offset+=1;
               int TrackCmd       = getTrackDataByte(offset);
               offset+=1;

               switch(TrackCmd)
               {
                  case 0x50:
                      {
						  // unk
						  int DataA   = getTrackDataWord(offset); offset+=2;
						  // direction
						  int DataB1   = getTrackDataWord(offset); offset+=2;
						  // radius
                          int Radius  = getTrackDataWord(offset); offset+=2;

						  //int v = (HIWORD(DataB1)*0x10000000)+(LOWORD(DataB1)*0x10000)+((HIWORD(Radius)*0x100))+(LOWORD(Radius));

						  //Radius = v;
						  short DataB = (short)DataB1;

						 
						  

						  //TRACE("%d %d %d %d %d\n",TrackCmdValue,DataA,Radius,DataB,(Radius+(65536*DataB)));


						  //if (DataB < 0 || DataB > 0x8000)Radius = 0xFFFF - Radius;

                          CCLineSection *ccline = new CCLineSection(this,TrackCmdValue,Radius,DataA,DataB,0,0,0x50,trackoffset);
						  ccline->arg2 = (Radius+(65536*DataB));
						  
                          totalLength+=TrackCmdValue;
                          CCLineSections->addElement(ccline);
                      } break;
                  case 0x70:
                      {
						  int DataA   = getTrackDataWord(offset); offset+=2;
						  short DataB   = getTrackDataWord(offset); offset+=2;
                          int Radius  = getTrackDataWord(offset); offset+=2;

						  //TRACE("%d %d %d %d %d\n",TrackCmdValue,DataA,Radius,DataB,(Radius+(65536*DataB)));

						  int DataC  = getTrackDataWord(offset);offset+=2;
                          int DataD  = getTrackDataWord(offset);offset+=2;
                          CCLineSection *ccline = new CCLineSection(this,TrackCmdValue,Radius,DataA,DataB,DataC,DataD,0x70,trackoffset);
                          totalLength+=TrackCmdValue;
                          CCLineSections->addElement(ccline);
                      } break;
                  case 0x00:
                      {
						  count =0;break;
                      }
                  default:
                      {
                         
                      }
               }
            }
            return offset;
        }

int GPTrack::PutCCSection(CCLineSection *t)
		 {
	           AfxMessageBox("PutCCSection out of date");
			   int TrackCmdValue  = (int)t->getLength();
               int TrackCmd       = t->getCmd();
			   int offset         = t->getOffset();

			   WritePCByte(offset,TrackCmdValue);offset+=1;
			   WritePCByte(offset,TrackCmd);offset+=1;

               switch(TrackCmd)
               {
                  case 0x50:
                      {
						  
						  WritePCWord(offset,(int)t->getArg1());offset+=2;
						  WritePCWord(offset,(int)t->getArg2());offset+=2;

						  int Direction = (int)t->getArg2();
						  int Radius    = (int)t->getRadius();

						  if (Direction < 0 || Direction > 0x8000)
						  {
						  	  //Radius = 0xFFFF - Radius;
						  }

						  WritePCWord(offset,Radius);offset+=2;
                
                      } break;
                  case 0x70:
                      {
						  WritePCWord(offset,(int)t->getArg1());offset+=2;
						  WritePCWord(offset,(int)t->getArg2());offset+=2;
						  WritePCWord(offset,(int)t->getRadius());offset+=2;
						  WritePCWord(offset,(int)t->getArg3());offset+=2;
						  WritePCWord(offset,(int)t->getArg4());offset+=2;
                      } break;
		 }
	return 1;
}

		void GPTrack::GetTrackObjectDefinition(int start)
        {
		TrackObjectDefinition *tobj = new TrackObjectDefinition(this,start);
		tobj->read(this,start,ListObjectOffset);

		TrackObjectDefintions->addElement(tobj);
		}


		TrackObjectDefinition * GPTrack::getObjectDefinition(int code)
		{
			// check the divide by 4 location first
			int possibleLocation = code/16;

			TrackObjectDefinition *tPossible = (	TrackObjectDefinition*) TrackObjectDefintions->elementAt(possibleLocation);
			if (tPossible && tPossible->getLocator() == code) 
			{
				return tPossible;
			}


            int size = TrackObjectDefintions->size();
			for(int i=0;i<size;i++)
			{
			   TrackObjectDefinition *t = (	TrackObjectDefinition*) TrackObjectDefintions->elementAt(i);
			   if (t->getLocator() == code) return t;
			}
			return NULL;
		}

         double AngleToDeg(double angle)
        {
            double newangle = 0;

            if (angle > 0x8000)
                newangle = -(0xFFFF - angle) -1;                
            else
                newangle = angle;



            double degangle = (((double)newangle)*180.0)/0x8000;
            return degangle;
        }

		  double ConvertDeg(double angle)
        {
            double degangle = (((double)angle)*180.0)/0xFFFF;
            return degangle;
        }

          double AngleTo(double angle)
        {
            double newangle = 0;

            if (angle > 0x8000)
                newangle = -(0xFFFF - angle) -1;                
            else
                newangle = angle;

            return newangle;
        }

		

		int AngleFrom(double angle)
        {
			int angledata = (int)((angle*0x8000)/180);
            return angledata;
        }

        double HeightTo(double height)
        {
            double newheight = 0;

            if (height > 0x8000)
                newheight = -(0xFFFF - height) -1;                
            else
                newheight = height;

            return newheight;
        }

        double DegToRad(double angle)
        {
            return (double)(angle)*(double)(PI/180.0);
        }

          double RadToDeg(double angle)
        {
            return (double)(angle)*(double)(180.0/PI);
        }

 
        static  int Minutes(double angle)
        {
           double angleDMS = angle;
           double deg = (int) angleDMS;
           double minsfact = angleDMS - deg;
           double mins = 60*minsfact;

           int minI = (int)mins;
           return minI;
        }

        static  int Degrees(double angle)
        {
           double angleDMS = angle;
           int deg = (int) angleDMS;
           return deg;
        }

        static  double DMStoAngle(int degrees,int minutes)
        {
            double angle = ((double)degrees)+((double)minutes/60.0);
            return angle;
        }

        double getLeftSideX(double angle,double length)
        {
            return length*cos(angle-PI/2);
        }

          double getLeftSideY(double angle,double length)
        {
            return length*sin(angle-PI/2);
        }

          double getRightSideX(double angle,double length)
        {
            return length*cos(angle+PI/2);
        }

          double getRightSideY(double angle,double length)
        {
            return  length*sin(angle+PI/2);
        }





/* |/////////////////////////////oOo\\\\\\\\\\\\\\\\\\\\\\\\\\\| */
/* |/////////////////////////////oOo\\\\\\\\\\\\\\\\\\\\\\\\\\\| */
int GPTrack::ReadPCDWord(int offset)
{
        unsigned char data[4];
        int  PCword;

		data[0] = trackdata[offset];
		data[1] = trackdata[offset+1];
		data[2] = trackdata[offset+2];
		data[3] = trackdata[offset+3];

        PCword = data[3]*256*256*256 +
                 data[2]*256*256 +
                 data[1]*256 +
                 data[0];
        return PCword;
}
/* |/////////////////////////////oOo\\\\\\\\\\\\\\\\\\\\\\\\\\\| */
/* |/////////////////////////////oOo\\\\\\\\\\\\\\\\\\\\\\\\\\\| */
int GPTrack::ReadPCWord(int offset)
{
        unsigned char data[2];
        int  PCword;

		data[0] = trackdata[offset];
		data[1] = trackdata[offset+1];
        PCword = data[1]*256 +
                 data[0];
        return 0x0000FFFF & PCword;
}

void GPTrack::WritePCDWord(int offset,int value)
{
	    trackdata[offset]   = (value &  0x000000FF);
		trackdata[offset+1] = ((value & 0x0000FF00)>>8);
		trackdata[offset+2] = ((value & 0x00FF0000)>>16);
		trackdata[offset+3] = ((value & 0xFF000000)>>24);
}


void GPTrack::WritePCWord(int offset,int value)
{
	    trackdata[offset]   = (value & 0x00FF);
		trackdata[offset+1] = ((value &0xFF00)>>8);
}

void GPTrack::WritePCByte(int offset,int value)
{
	    trackdata[offset]   = value;
}

/* |/////////////////////////////oOo\\\\\\\\\\\\\\\\\\\\\\\\\\\| */
/* |/////////////////////////////oOo\\\\\\\\\\\\\\\\\\\\\\\\\\\| */
int GPTrack::ReadPCByte(int offset)
{
	    int PCWord = 0;
	    PCWord = trackdata[offset];
        return 0x000000FF & PCWord;
}

/* |/////////////////////////////oOo\\\\\\\\\\\\\\\\\\\\\\\\\\\| */
/* |/////////////////////////////oOo\\\\\\\\\\\\\\\\\\\\\\\\\\\| */
static long GetLengthofFile(const char *name)
{
  FILE *fp;
  long i=0;
  unsigned char da[1];

  fp = fopen(name,"rb");
  if (fp == NULL) return -1;
  while(fread(&da,sizeof(unsigned char),1,fp)>0){
     i++;
  }
  fclose(fp);
  return i;
}

#define WIDTH_TO_CCWIDTH2(x) ((((double)(x))*1.0)/1024.0)

void GPTrack::createCC()
{
     if (TrackSections)
	 {
		   TrackSection *t1 = (TrackSection*)TrackSections->elementAt(0);

	       double xpos = t1->getStartX();
	       double ypos = t1->getStartY();
		   //double currentangle  = ((360*TrackStartAngle.GetValue())/65535)-90;
		   double currentangle  = ((360*TrackStartAngle.GetValue())/65535);
		   double currrad = DegToRad(currentangle);
		   //double currrad = 0.0;

		   
		   double radXStart = CCLineStartX.GetValue();
		   double radYStart = -WIDTH_TO_CCWIDTH2(CCLineStartY.GetValue());

		   xpos += radXStart * cos(currrad);
		   ypos += radXStart * sin(currrad);

		   double radCur = (PI/2)-currrad;
		   xpos += radYStart * cos(radCur);
		   ypos += radYStart * sin(radCur);

		   int size = CCLineSections->size();
		   for(int i=0;i<size;i++)
           {
             CCLineSection *t = (CCLineSection*)CCLineSections->elementAt(i);
		     double nextxpos,nextypos;

			 double  wider = (double)t->getArg1();
		     double  direction = (double)t->getArg2();
			 double  ndirection = (double)t->getArg3();

		     if (direction > 0x8000) direction = -(0x10000 - direction);
			 if (ndirection > 0x8000) ndirection = -(0x10000 - ndirection);

		     double rad     = t->getRadius();
			 double length  = t->getLength();
				 if (wider){
				   //length+=(wider/300.0);
				 }
			 double anglelen;


			 double sectorStartAngle = currrad;
             double sectorEndAngle   = currrad;

			 double rad2=0;
			 rad = RadiusTo(this,rad,direction,(double)t->getArg1(),t->getCmd(),ndirection,t->getArg4(),rad2);
			 ////rad = ((double)t->arg2)/1024.0;

			 double arg1 = (double)t->getArg1();
			 if (arg1 != 0)
			 {				
				double offset = ((1.0/128.0) *(double)arg1);
				ypos = ypos+offset*sin(currrad);
                xpos = xpos+offset*cos(currrad);
			 }
			 
			 
				 if (rad ==0)
				 {
				  nextypos = length*sin(currrad);
				  nextxpos = length*cos(currrad);
				  anglelen = 0;
				 }
				 else {
					 if (t->getCmd()==0x50)
					 {
						 double theta = length/rad;
						 double angle1 = currrad+theta/2.0;
						 double len   = 2.0*rad*sin(theta/2.0);
						 nextypos =  len*sin(angle1);
				         nextxpos =  len*cos(angle1);  
						 currrad += theta;
						 sectorEndAngle = currrad;
					 }
					 else {
						  /*
						  double nx,ny;
						  nx = 0;
						  ny = 0;
						  double theta = atan2(length,fabs(rad));
						  
						  double cx = xpos+getRightSideX(currrad,rad2);
						  double cy = ypos+getRightSideY(currrad,rad2);
						  double angle1 = currrad+theta; 
						  ny =  cy+(rad*sin((currrad+theta)));
						  nx =  cx+(rad*cos((currrad+theta)));
						  */
						  

						  
						  double radBegin = rad2;
						  double radDelta = (rad-rad2)/((double)length);
						  double nx,ny;
						  nx = 0;
						  ny = 0;
						  for(int k=0;k<length;k++)
						  {
							  /*
							  double theta = 1.0/radBegin;
							  double angle1 = currrad+theta/2.0;
						      double len   = 2.0*radBegin*sin(theta/2.0);
						      nextypos =  len*sin(angle1);
				              nextxpos =  len*cos(angle1);
							  nx += nextxpos;
							  ny += nextypos;
						      currrad += theta;
							  radBegin+=radDelta;
							  */
						     
		  					  double anglerad = ((1.0)/radBegin);				 
							  double inrad = anglerad/2.0;
							  double len   = 2.0*radBegin*sin(inrad);
							  double angle1 = currrad+inrad; 
							  nextypos =  (len*sin((angle1)));
							  nextxpos =  (len*cos((angle1)));
							  nx += nextxpos;
							  ny += nextypos;
							  currrad += anglerad;
							  radBegin+=radDelta;
							  
						  }
						  sectorEndAngle = currrad;
						  
						 
						 nextxpos = nx;
						 nextypos = ny;						 
					 }
				}
			
             t->setStartX(xpos);
             t->setStartY(ypos);

			 if (t->getCmd()==0x50)
			 {
               t->setEndX(nextxpos);
               t->setEndY(nextypos);
               t->setStartAngle(sectorStartAngle);
               t->setEndAngle(sectorEndAngle);
			   xpos =(xpos+nextxpos);
	           ypos =(ypos+nextypos);
			 }
			 else {
			   t->setEndX(nextxpos);
               t->setEndY(nextypos);
               t->setStartAngle(sectorStartAngle);
               t->setEndAngle(sectorEndAngle);
			   xpos =(xpos+nextxpos);
	           ypos =(ypos+nextypos);
				/*
				double cx = xpos+getRightSideX(sectorStartAngle,rad);
				double cy = ypos+getRightSideY(sectorStartAngle,rad);
			    double sA = sectorStartAngle-(PI/2);
				double eB = sectorEndAngle-(PI/2);
				double de = (eB-sA);
				double eA = sA + 2.0*(de);
				//double eA = sA + (de);

				double sX = cx + (rad*cos(sA));
				double sY = cy + (rad*sin(sA));

				double eX = cx + (rad2*cos(eA));
				double eY = cy + (rad2*sin(eA));
				double count = 20.0;
				double deltaAngle = (eA - sA);
				double dA = deltaAngle/count;
				double deltaRadius = (rad2-rad);
				double dr = deltaRadius/count;
				double otX,otY;

				for(int j=0;j<=count;j++)
				{
					double ndA = (double)j*dA;
					double ndr = (double)j*dr;
					double tX = cx + ((rad+ndr)*cos(sA+ndA));
					double tY = cy + ((rad+ndr)*sin(sA+ndA));
					otX = tX;
					otY = tY;
				}
			   t->setStartAngle(sectorStartAngle);
			   t->setEndAngle(sectorEndAngle);
			   t->setEndX(otX-xpos);
               t->setEndY(otY-ypos);
			   //currrad = sectorEndAngle;
			   double anglerad = ((length)/rad);	
			   currrad = currrad+anglerad;	
			   xpos = otX;
			   ypos = otY;
			   */
			 }

		     
		}
   }
}

int TextureIdToBrush(GPTrack *track,int texture)
{
	switch(texture)
	{
			case TXT_SAND:
				return (track->showUseTextures ? SAND_BITMAP_BRUSH : SAND_BRUSH);
				break;
			case TXT_ROAD:
				return (track->showUseTextures ? ROAD_BITMAP_BRUSH : ROAD_BRUSH);
				break;
			case TXT_DARKROAD:
			case 22:
				return (track->showUseTextures ? DRK_ROAD_BITMAP_BRUSH : DRK_ROAD_BRUSH);
			case TXT_CROWD:
			case TXT_CROWD1:
			case TXT_CROWD2:
			case TXT_CROWD3:
				return (track->showUseTextures ? CROWD_BITMAP_BRUSH : RED_BRUSH);
				break;
			case 0x22:
				return (track->showUseTextures ? GRASS_BITMAP_BRUSH : GRASS_BRUSH);
				break;
			default:
				return (track->showUseTextures ? RED_BRUSH : RED_BRUSH);
				break;
	}
}


void GPTrack::CheckDefaultSetting()
{
	TrackSection *firstSection = (TrackSection*)TrackSections->elementAt(0);

	Vector *cmds = firstSection->getCommands();

	for(int i=0;i<cmds->size();i++)
	{
		TrackCmd *cmd = (TrackCmd*)cmds->elementAt(i);

		switch(cmd->getCmd())
		{
		    case 0xc8:
			{
				int location   = cmd->getNthArg(2);
				int textureid  = cmd->getNthArg(4);

				switch(location)
				{
				   case LOC_FLOOR_RIGHT: 
					   DefaultLeftFloorBrush  = TextureIdToBrush(this,textureid);
					   DefaultLeftVergeId     = textureid;
					   break;
                   case LOC_FLOOR_LEFT:					   
					   DefaultRightFloorBrush = TextureIdToBrush(this,textureid);
					   DefaultRightVergeId     = textureid;
					   break;
				   case LOC_ROAD:					   
					   DefaultRoadBrush       = TextureIdToBrush(this,textureid);
					   DefaultRoadId     = textureid;
					   break;
				   case LOC_RIBBON11:					   
					   DefaultRibbon11Brush       = TextureIdToBrush(this,textureid);break;
				   case LOC_RIBBON12:					   
					   DefaultRibbon12Brush       = TextureIdToBrush(this,textureid);break;
				   case LOC_RIBBON13:					   
					   DefaultRibbon13Brush       = TextureIdToBrush(this,textureid);break;
				   case LOC_RIBBON14:					   
					   DefaultRibbon14Brush       = TextureIdToBrush(this,textureid);break;
				   case LOC_RIBBON15:					   
					   DefaultRibbon15Brush       = TextureIdToBrush(this,textureid);break;
				   case LOC_RIBBON16:					   
					   DefaultRibbon16Brush       = TextureIdToBrush(this,textureid);break;
				}
			}break;
		}
	}
}

#define Deg2Rad(x) ((double)(x)*PI/180.0)

static double ToMeters(double d)
{
	return d*4.8768;
}

void GPTrack::createTrack(CDocument *pDoc)
{
	   CheckDefaultSetting();

	   double currentangle  = ((360*TrackStartAngle.GetValue())/65535);
       int height = getTrackHeight();

       double xpos = TrackXBeginPos;
       double ypos = TrackYBeginPos;
	   
	   double xoff = TrackStartX.GetValue()/128; 
	   double yoff = TrackStartY.GetValue()/128; 
	   xpos = xpos+xoff;
	   ypos = ypos+yoff;

	   double zpos = 0;
       double nextxpos = xpos;
       double nextypos = ypos;
	   double nextzpos = zpos;
	   int altitude = 0;
	   double trueAltitudeLevel = 0;

       int length;
	   //double GradientAtBeginning = 0;
	   
	   double GradientAtBeginning = TrackStartClimb.GetValue();

       double trackWidth = TRACK_WIDTH;
	   int widthLeft  = (int)TrackBeginWidth.GetValue();
	   int widthRight = (int)TrackBeginWidth.GetValue();

	   int viewAhead = 64;
	   int viewBehind = 64;

	   double GradientAtBeginningIso = TrackStartClimb.GetValue();
	   GradientAtBeginningIso=(45.0*GradientAtBeginningIso)/8192.0; 
	   double heightIso = 0;
	   double adIso=0;

	   int size = TrackSections->size();
	   //double cumaltitude;
       for(int i=0;i<size;i++)
       {
           TrackSection *t = (TrackSection*)TrackSections->elementAt(i);
		   t->setIndex(i);
		   
		   double angle = t->getAngle();
		   height += t->getHeight();
           length  = t->getLength();

		   
		   heightIso = t->getHeight();
		
		   // you say height delta but which delta between segment or the true height value!
		   if(heightIso!=0) { 
				double m=(45.0*heightIso*length)/8192.0; 
				double den = m/sin(Deg2Rad(m/2.0))/sin(Deg2Rad(m/2.0+GradientAtBeginningIso)); 
				adIso+=(2.0*length*(180.0/PI))/den;						
				GradientAtBeginningIso+=m; 
		   } 
		   else {
				adIso+=length*sin(Deg2Rad(GradientAtBeginningIso)); 
		   } 
		   t->heightIsoDelta = ToMeters(adIso);
		

#if 1
		   double md = t->getHeight()*length;
		   double ad = (GradientAtBeginning + md/2.0)*length;
		   GradientAtBeginning+=md;

		   t->setAltitude(GradientAtBeginning);
		   trueAltitudeLevel+=ad;
		   t->setTrueAltitude(trueAltitudeLevel/100.0);
		   t->setTrueAltitudeDelta(ad/100.0);
		
#else
		    t->setAltitude((int)trueAltitudeLevel);
		   
			double ad=0;
		    if(height!=0) { 
				double m=45.0*height*1.0/8192.0; 
				ad=2.0*length*180.0/PI/m*sin(Deg2Rad(m/2.0))*
								     sin(Deg2Rad(m/2.0+GradientAtBeginning)); 
				GradientAtBeginning+=m; 
			} 
			else {
				ad=length*sin(Deg2Rad(GradientAtBeginning)); 
			} 
			//trueAltitudeLevel += ad/100;
			trueAltitudeLevel+=ad;
		    t->setTrueAltitude(trueAltitudeLevel/100.0);
			t->setTrueAltitudeDelta(ad/100.0);
#endif
			




	

		   double anglelen = length*angle;
		   double anglerad = DegToRad(anglelen);
		   double currrad  = DegToRad(currentangle);

		   double sectorStartAngle = currrad;
		   double sectorEndAngle   = currrad;
		   double sectorRadius     = 0;

           if (angle!=0)
           {
            double beta = anglelen/2.0;
            double inrad        = DegToRad(beta);
            sectorRadius        = (length/anglerad);
            double chordrad     = DegToRad((anglelen/2.0));
            double len = 2*(sectorRadius*sin(inrad));

            nextypos =  len*sin((currrad+inrad));
            nextxpos =  len*cos((currrad+inrad));
            
            sectorEndAngle = currrad+inrad;
            currentangle += anglelen;
           }
           else
           {
            nextypos = length*sin(currrad);
            nextxpos = length*cos(currrad);
            
           }

		   if (t->getHeight()!=0)
		   {
		    nextzpos = t->getTrueAltitude()/128;
		   }
		   else
		   {
			nextzpos = t->getTrueAltitude()/128;		    
		   }

           t->setStartX(xpos);
           t->setStartY(ypos);
           t->setEndX(nextxpos);
           t->setEndY(nextypos);
           t->setStartAngle(sectorStartAngle);
           t->setEndAngle(sectorEndAngle);
		   t->setRadius(sectorRadius);
		   t->setStartZ(zpos);
           t->setEndZ(nextzpos);


		   if (t->containsCommand(0x85))
		   {
			   Vector *widthChange =   t->getCommands(0x85);
               int csize =widthChange->size();
			   for(int i=0;i<csize;i++)
			   {
				   TrackCmd *cmd = (TrackCmd*)widthChange->elementAt(i);
				   widthLeft = widthRight = cmd->getNthArg(3);
			   }
			   delete widthChange;
		   }
		   if (t->containsCommand(0xb4))
		   {
			   Vector *widthChange =   t->getCommands(0xb4);
               int csize =widthChange->size();
			   for(int i=0;i<csize;i++)
			   {
				   TrackCmd *cmd = (TrackCmd*)widthChange->elementAt(i);
				   widthLeft = cmd->getNthArg(3);
			   }
			   delete widthChange;
		   }
		   if (t->containsCommand(0xb5))
		   {
			   Vector *widthChange =   t->getCommands(0xb5);
			   int csize =widthChange->size();
			   for(int i=0;i<csize;i++)
			   {
				   TrackCmd *cmd = (TrackCmd*)widthChange->elementAt(i);
				   widthRight = cmd->getNthArg(3);
			   }
			   delete widthChange;
		   }

		   if (t->containsCommand(0x81))
		   {
			   Vector *viewChange =   t->getCommands(0x81);
			   int csize =viewChange->size(); 
			   for(int i=0;i<csize;i++)
			   {
				   TrackCmd *cmd = (TrackCmd*)viewChange->elementAt(i);
				   viewAhead = cmd->getNthArg(2);
			   }
			   delete viewChange;
		   }
		   if (t->containsCommand(0x82))
		   {
			   Vector *viewChange =   t->getCommands(0x82);
			   int csize =viewChange->size();
			   for(int i=0;i<csize;i++)
			   {
				   TrackCmd *cmd = (TrackCmd*)viewChange->elementAt(i);
				   viewBehind = cmd->getNthArg(2);
			   }
			   delete viewChange;
		   }


		   t->setWidthLeft(widthLeft);
		   t->setWidthRight(widthRight);

		   t->LookAhead = viewAhead;
		   t->LookBehind = viewBehind;
           

		   xpos =(xpos+nextxpos);
	       ypos =(ypos+nextypos);
		   zpos =nextzpos;

        }

	   TrackSection *PitEntrance = FindTrackCommand(0x86);
	   if (PitEntrance!=NULL)
			PitLaneEntryIndex = GetTrackSectionIndex(PitEntrance);
	   else
		    PitLaneEntryIndex = TrackSections->size();

	   createPitlane();

	   if (pDoc!=NULL)
		   pDoc->UpdateAllViews(NULL);
	   

}

BOOL GPTrack::PitLaneEntranceToLeft()
{
   TrackSection *pitEntrance = GetTrackSection(PitLaneEntryIndex);

   TrackSection *pitEntrancePlusOne = GetNextSection(pitEntrance);
   TrackSection *pitEntrancePlusTwo = GetNextSection(pitEntrancePlusOne);

   BOOL a1First = pitEntrancePlusOne->containsCommand(0xa1);
   BOOL a2First = pitEntrancePlusOne->containsCommand(0xa2);
   BOOL a1Second = pitEntrancePlusTwo->containsCommand(0xa1);
   BOOL a2Second = pitEntrancePlusTwo->containsCommand(0xa2);

   if (a1First && a2Second)
   {
	  return FALSE;
   }
   else if (a1Second && a2First)
   {
	   return TRUE;
   }
   else
   {
	   AfxMessageBox("Error with pitlane entrance commands cannot find 0xa1 or 0xa2 commands");
	   return FALSE;
   }
}

BOOL GPTrack::PitLaneExitToLeft()
{
   TrackSection *pitExit = GetTrackSection(PitLaneExitIndex);

   TrackSection *pitExitMinusOne = GetPreviousSection(pitExit);
   TrackSection *pitExitMinusTwo = GetPreviousSection(pitExitMinusOne);

   BOOL a3Second  = pitExitMinusOne->containsCommand(0xa3);
   BOOL a4Second  = pitExitMinusOne->containsCommand(0xa4);
   BOOL a3First = pitExitMinusTwo->containsCommand(0xa3);
   BOOL a4First = pitExitMinusTwo->containsCommand(0xa4);

   if (a4First && a3Second)
   {
	  return FALSE;
   }
   else if (a4Second && a3First)
   {
	   return TRUE;
   }
   else
   {
	   AfxMessageBox("Error with pitlane exit commands cannot find 0xa3 or 0xa4 commands");
	   return FALSE;
   }
}



TrackSection * GPTrack::GetTrackSection(int idx)
{
	TrackSection *section = (TrackSection*)TrackSections->elementAt(idx);
	return section;
}

TrackSection * GPTrack::GetTrackSection(Vector *vec,int idx)
{
	if (vec!=NULL)
	{
	  TrackSection *section = (TrackSection*)vec->elementAt(idx);
	  return section;
	}
	return NULL;
}

int GPTrack::GetTrackSectionIndex(TrackSection *t)
{
	Vector *vec = t->getVector();
	if (vec)
	{
	 int size = vec->size();
	 for(int i=0;i<size;i++)
	 {
	   TrackSection *section = (TrackSection*)vec->elementAt(i);
	   if (t == section) return i;
	 }
	}
	return -1;
}

CCLineSection * GPTrack::GetCCSection(int idx)
{
	Vector *vec = CCLineSections;
	if (vec!=NULL)
	{
	  CCLineSection *section = (CCLineSection*)vec->elementAt(idx);
	  return section;
	}
	return NULL;
}
	
int GPTrack::GetCCSectionIndex(CCLineSection *t)
{
	Vector *vec = t->getVector();
	if (vec)
	{
	 int size = vec->size();
	 for(int i=0;i<size;i++)
	 {
	   CCLineSection *section = (CCLineSection*)vec->elementAt(i);
	   if (t == section) return i;
	 }
	}
	return -1;
}

TrackSection * GPTrack::GetNextSection(TrackSection *t)
{
	Vector *vec = t->getVector();

	if (vec) return (TrackSection*)vec->next(t);

	return NULL;
	/*
	Vector *vec = t->getVector();
	if (vec)
	{
	  int idx = GetTrackSectionIndex(t);

	  if (idx == -1) return NULL;

	  if (idx >= vec->size()-1) idx = -1;

	  return GetTrackSection(vec,idx+1);
	}
	return NULL;
	*/
}

CCLineSection * GPTrack::GetNextSection(CCLineSection *t)
{
	Vector *vec = t->getVector();
	if (vec)
	{
	  int idx = GetCCSectionIndex(t);

	  if (idx == -1) return NULL;

	  if (idx >= vec->size()-1) idx = -1;

	  return GetCCSection(idx+1);
	}
	return NULL;
}

Camera  * GPTrack::GetPreviousCamera(Camera *t)
{
	Vector *vec = TrackCameras;
	if (vec)
	{
	 int idx = vec->findElementIndex(t);

	 if (idx == -1) return NULL;

	 if (idx == 0) idx = vec->size();

	 return (Camera*)TrackCameras->elementAt(idx-1);
	}
	return NULL;
}

CCLineSection * GPTrack::GetPreviousSection(CCLineSection *t)
{
	Vector *vec = t->getVector();
	if (vec)
	{
	 int idx = GetCCSectionIndex(t);

	 if (idx == -1) return NULL;

	 if (idx == 0) idx = vec->size();

	 return GetCCSection(idx-1);
	}
	return NULL;
}

TrackSection * GPTrack::GetPreviousSection(TrackSection *t)
{
	Vector *vec = t->getVector();
	if (vec)
	{
		return (TrackSection *)vec->previous(t);
	}
	/*
	Vector *vec = t->getVector();
	if (vec)
	{
	 int idx = GetTrackSectionIndex(t);

	 if (idx == -1) return NULL;

	 if (idx == 0) idx = vec->size();

	 return GetTrackSection(vec,idx-1);
	}
	return NULL;
	*/
	return NULL;
}


TrackSection * GPTrack::FindTrackCommand(int cmd,int pos)
{
	for(int i=pos;i<TrackSections->size();i++)
	{
		TrackSection *t = GetTrackSection(i);

		if (t->containsCommand(cmd)) return t;
	}
	return NULL;
}



void GPTrack::createPitlane()
{
    if (TrackSections == NULL || TrackSections->size() == 0) return;

    TrackSection *pitlaneStart = (TrackSection*)TrackSections->elementAt(PitLaneEntryIndex);

	if (pitlaneStart==NULL)return;
	
    double currentangle = RadToDeg(pitlaneStart->getStartAngle());
    double xpos = pitlaneStart->getStartX();
    double ypos = pitlaneStart->getStartY();
	double height=0;
	double length;
	double nextypos;
	double nextxpos;
	double zpos = pitlaneStart->getStartZ();
	double nextzpos = zpos;
	//double trackWidth = PIT_WIDTH;
	double GradientAtBeginning = 0;
	double trueAltitudeLevel = 0;

	int size = PitlaneSections->size();
    for(int i=0;i<size;i++)
    {
       TrackSection *t = (TrackSection*)PitlaneSections->elementAt(i);
	   t->setIndex(i);

       double angle = t->getAngle();
           
       height += t->getHeight();
       length  = t->getLength();

	   double md = t->getHeight()*length;
		   double ad = (GradientAtBeginning + md/2.0)*length;
		   GradientAtBeginning+=md;

		   t->setAltitude((int)GradientAtBeginning);
		   t->setTrueAltitudeDelta(ad/100.0);
		   trueAltitudeLevel += ad;
		   t->setTrueAltitude(trueAltitudeLevel/100.0);

           double anglelen = length*angle;
           double anglerad = DegToRad(anglelen);
           double currrad  = DegToRad(currentangle);

           double sectorStartAngle = currrad;
           double sectorEndAngle   = currrad;
           double sectorRadius     = 0;

           if (angle!=0)
           {
            double beta = anglelen/2.0;
            double inrad        = DegToRad(beta);
            sectorRadius       = (length/anglerad);
            double chordrad     = DegToRad((anglelen/2.0));
            //double len = 2*(sectorRadius*sin(chordrad));
			double len = 2*(sectorRadius*sin(inrad));

            nextypos =  len*sin((currrad+inrad));
            nextxpos =  len*cos((currrad+inrad));
            
            sectorEndAngle = currrad+inrad;
            currentangle += anglelen;
           }
           else
           {
            nextypos = length*sin(currrad);
            nextxpos = length*cos(currrad);
            
           }

		   //if (t->getHeight()!=0)
		   //{
		   // nextzpos = zpos + length/t->getHeight();
		   //}
		   //else
		   //{
			//nextzpos = zpos;		    
		   //}

		   if (t->getHeight()!=0)
		   {
		    nextzpos = t->getTrueAltitude()/128;
		   }
		   else
		   {
			nextzpos = t->getTrueAltitude()/128;		    
		   }

           t->setStartX(xpos);
           t->setStartY(ypos);
           t->setEndX(nextxpos);
           t->setEndY(nextypos);
           t->setStartAngle(sectorStartAngle);
           t->setEndAngle(sectorEndAngle);
		   t->setRadius(sectorRadius);
		   t->setStartZ(zpos);
           t->setEndZ(nextzpos);
           

		   xpos = xpos+nextxpos;
           ypos = ypos+nextypos;
		   zpos = nextzpos;
       }
}

void GPTrack::drawGrid(Display *g)
{
	

	int maxWidth  = 65535/256;
	int maxHeight = 65535/256;
	double xpos = TrackXBeginPos;
    double ypos = TrackYBeginPos;

	g->setColor(GRID_PEN);

	if (showGridLines)
	{	
	// edge
	g->drawLine(xpos-maxWidth,ypos-maxHeight,xpos+maxWidth,ypos-maxWidth);
	g->drawLine(xpos+maxWidth,ypos-maxHeight,xpos+maxWidth,ypos+maxWidth);
	g->drawLine(xpos+maxWidth,ypos+maxHeight,xpos-maxWidth,ypos+maxWidth);
	g->drawLine(xpos-maxWidth,ypos+maxHeight,xpos-maxWidth,ypos-maxWidth);

	// cross
	g->drawLine(xpos,ypos,xpos+maxWidth,ypos+maxWidth);
	g->drawLine(xpos,ypos,xpos-maxWidth,ypos-maxWidth);
	g->drawLine(xpos,ypos,xpos+maxWidth,ypos-maxWidth);
	g->drawLine(xpos,ypos,xpos-maxWidth,ypos+maxWidth);
	}


	//grid
	if (showGraphPaper)
	{
	int x =  (int)(xpos-maxWidth);
	int y =  (int)(ypos-maxHeight);
	int width = 2*maxWidth;
	int height = 2*maxHeight;
	int gridx;

	// edge
	g->drawLine(xpos-maxWidth,ypos-maxHeight,xpos+maxWidth,ypos-maxWidth);
	g->drawLine(xpos+maxWidth,ypos-maxHeight,xpos+maxWidth,ypos+maxWidth);
	g->drawLine(xpos+maxWidth,ypos+maxHeight,xpos-maxWidth,ypos+maxWidth);
	g->drawLine(xpos-maxWidth,ypos+maxHeight,xpos-maxWidth,ypos-maxWidth);



	switch(gridSizeOption)
	{
	   case TEN_METERS:          gridx = (int)(10.0/LENGTH_TO_KM);break;
	   case FIFTY_METERS:        gridx = (int)(50.0/LENGTH_TO_KM);break;
	   case ONE_HUNDRED_METERS:  gridx = (int)(100.0/LENGTH_TO_KM);break;
	   case FIVE_HUNDRED_METERS: gridx = (int)(500.0/LENGTH_TO_KM);break;
	   case ONE_KM:              gridx = (int)(1000.0/LENGTH_TO_KM);break;
	   case HALF_MILE:           gridx = (int)(800.0/LENGTH_TO_KM);break;
	   default:
	   case ONE_MILE:            gridx = (int)(1600.0/LENGTH_TO_KM);break;
	   
	}



	int gridy=gridx;

	for(int i=x;i<x+width;i+=gridx)
	{
	   g->drawLine(i,y,i,y+height);	
	}

	for( i=y;i<y+height;i+=gridy)
	{
		g->drawLine(x,i,x+width,i);	
	}
	}

}

void drawMarking(Display *g,TrackSection *t,int cmd)
{
	    double xpos     = t->getStartX();
		double ypos     = t->getStartY();
		double nextxpos = t->getEndX();
		double nextypos = t->getEndY();
		

	    //char buffer[10];
		double txpos = (xpos+(xpos+nextxpos))/2;
	    double typos = (ypos+(ypos+nextypos))/2;

		//wsprintf(buffer,"0x%x",cmd);

		//g->drawText(txpos,typos,buffer);

		g->drawBitmap(IDB_TRACK_MARKINGS,txpos,typos);
	  
}

void drawCmdText(Display *g,TrackSection *t,int cmd)
{
	    double xpos     = t->getStartX();
		double ypos     = t->getStartY();
		double nextxpos = t->getEndX();
		double nextypos = t->getEndY();
		

	    char buffer[10];
		double txpos = (xpos+(xpos+nextxpos))/2;
	    double typos = (ypos+(ypos+nextypos))/2;

		wsprintf(buffer,"0x%x",cmd);

		g->drawText(txpos,typos,buffer);
	  
}




#define CONTAIN_PITCMD(x) if(t->containsCommand(x)) drawCmdText(g,t,x);
#define CONTAIN_MARKING_CMD(x) if(t->containsCommand(x)) drawMarking(g,t,x);

void GPTrack::drawPitCmds(Display *g)
{
	int size = TrackSections->size();
	for(int i=0;i<size;i++)
    {
        TrackSection *t = (TrackSection*)TrackSections->elementAt(i);

		CONTAIN_PITCMD(0xa1);		
		CONTAIN_PITCMD(0xa2);		
		CONTAIN_PITCMD(0xa3);		
		CONTAIN_PITCMD(0xa4);		

		CONTAIN_PITCMD(0x86);		
		CONTAIN_PITCMD(0x87);		
	}


	size = PitlaneSections->size();
	for(i=0;i<size;i++)
	{
		TrackSection *t = (TrackSection*)PitlaneSections->elementAt(i);

		CONTAIN_PITCMD(0x9f);		
		CONTAIN_PITCMD(0x9e);		
		CONTAIN_PITCMD(0x88);		
		CONTAIN_PITCMD(0x89);		
	}
}

void GPTrack::drawTrackMarkings(Display *g)
{
	int size = TrackSections->size();
	for(int i=0;i<size;i++)
    {
        TrackSection *t = (TrackSection*)TrackSections->elementAt(i);

		CONTAIN_MARKING_CMD(0x8a);		
		CONTAIN_MARKING_CMD(0x8b);				
	}


	size = PitlaneSections->size();
	for(i=0;i<size;i++)
	{
		TrackSection *t = (TrackSection*)PitlaneSections->elementAt(i);

		CONTAIN_MARKING_CMD(0x8a);		
		CONTAIN_MARKING_CMD(0x8b);		
	}
}


void GPTrack::drawTrack(Display *g,BOOL drawGray)
{
	   double trackWidth = TRACK_WIDTH;
	   TrackSection *tn;

	   int size = TrackSections->size();

	   // default sides
	   VectorIter *iter =TrackSections->firstIter(); 
	   for(int i=0;i<size;i++)
       {
           //TrackSection *t = (TrackSection*)TrackSections->elementAt(i);
		   TrackSection *t = (TrackSection*)iter->ptr;
		   
		   if (i<size-1){
		    tn = t->next();
		   }
		   else{
			tn = (TrackSection*)TrackSections->elementAt(0);
		   }

		   if (showScenery)
			 t->drawScenery(g);

           if (showDefaultSides)
		   {
			 TrackSection *prev = GetPreviousSection(t);
			 t->drawSides(g,t->getAngle(),this,t,tn,prev);
		   }
		   iter = TrackSections->nextIter(iter);
	   }

	   if (showTrack)
	   {
	    // road
		VectorIter *iter =TrackSections->firstIter(); 
	    for(i=0;i<size;i++)
        {
		   //TrackSection *t = (TrackSection*)TrackSections->elementAt(i);	
		   TrackSection *t = (TrackSection*)iter->ptr;
		   t->drawTrack(g,t->getAngle(),this,drawGray);
		   iter = TrackSections->nextIter(iter);
	    }
	   }

	   if (showTrackSide)
	   {
	    // gravel traps etc...
		VectorIter *iter =TrackSections->firstIter(); 
	    for(i=0;i<size;i++)
        {
		   //TrackSection *t = (TrackSection*)TrackSections->elementAt(i);
		   TrackSection *t = (TrackSection*)iter->ptr;
		   t->drawTextureSides(g,this);
		   iter = TrackSections->nextIter(iter);
	    }
	   }

	   {
	   VectorIter *iter =TrackSections->firstIter(); 
	   for(i=0;i<size;i++)
       {
           //TrackSection *t = (TrackSection*)TrackSections->elementAt(i);
		   TrackSection *t = (TrackSection*)iter->ptr;

		   if (i<size-1){
		    tn = t->next();
		   }
		   else{
			tn = (TrackSection*)TrackSections->elementAt(0);
		   }
		   //if (i<size-1)
		   //{
		   // tn = (TrackSection*)TrackSections->elementAt(i+1);
		   //}
		   //else
		   //{
		//	tn = (TrackSection*)TrackSections->elementAt(0);
		   //}

		   //if (showTrack)
           //  t->drawTrack(g,t->getAngle(),this,drawGray);

		   if (showWalls)
		   {
			 TrackSection *prev = GetPreviousSection(t);
             t->drawWalls(g,t->getAngle(),this,prev,t);			 
		   }
		   iter = TrackSections->nextIter(iter);
	   }
	   }

	   {
	   VectorIter *iter =TrackSections->firstIter();
	   for(i=0;i<size;i++)
       {
           //TrackSection *t = (TrackSection*)TrackSections->elementAt(i);
		   TrackSection *t = (TrackSection*)iter->ptr;

		   if (showObjects)
		     t->drawObjects(g,t->getStartX(),t->getStartY(),t->getStartAngle(),showBitmaps);

		   if (showOutlines)
			 DrawObjectOutline(g,t);

		   if (showRoadSigns)
			 t->drawRoadSign(g);

		   if (showCommands)
			 t->drawCommands(g);

		   if (showTextureMappingCommands)
			 t->drawTextureCommands(g);

		   if (showViewDistanceCommands)
			 t->drawViewDistanceCommands(g);

		   if (i==0 && showFinishLine)
			 t->drawStartingLine(g);

		   if (showGrip)
			 t->drawGrip(g,t->getAngle(),this);

		   if (showPitCmds)
			 drawPitCmds(g);

		   if (showMarkings)
			 drawTrackMarkings(g);

		   if (showUnkVariousTable)
			 drawVarious(g);	
		   iter = TrackSections->nextIter(iter);
        }  
	   }
}

double GPTrack::LengthToTrackSector(TrackSection *in)
{
	double totalLength=0;

	int size = TrackSections->size();
	for(int i=0;i<size;i++)
    {
        TrackSection *t = (TrackSection*)TrackSections->elementAt(i);
		if (t == in)
			return totalLength;

		if (t->getAngle() ==0) totalLength+=t->getLength();
        else totalLength+= abs(t->getLength());

	}
	return totalLength;
}

double GPTrack::LengthToTrackSectorEnd(TrackSection *in)
{
	double totalLength=0;

	int size = TrackSections->size();
	for(int i=0;i<size;i++)
    {
        TrackSection *t = (TrackSection*)TrackSections->elementAt(i);
		

		if (t->getAngle() ==0) totalLength+=t->getLength();
        else totalLength+= abs(t->getLength());

		if (t == in)
			return totalLength;

	}
	return totalLength;
}

double GPTrack::LengthToPitSectorEnd(TrackSection *in)
{
	double totalLength=0;

	int size = PitlaneSections->size();
	for(int i=0;i<size;i++)
    {
        TrackSection *t = (TrackSection*)PitlaneSections->elementAt(i);
		

		if (t->getAngle() ==0) totalLength+=t->getLength();
        else totalLength+= abs(t->getLength());

		if (t == in)
			return totalLength;

	}
	return totalLength;
}


TrackSection * GPTrack::SectorForLength(double len)
{
	double totalLength=0;
	int size=TrackSections->size();

	for(int i=0;i<size;i++)
    {
        TrackSection *t = (TrackSection*)TrackSections->elementAt(i);
		
		if (t->getAngle() ==0) totalLength+= (t->getLength());
		//else totalLength+= abs(t->getLength()*t->getAngle());
		else totalLength+= abs(t->getLength());

		if (totalLength > len) return t;
	}
	return (TrackSection*)TrackSections->elementAt(size-1);
}


// Draw Track Side Cameras
void GPTrack::drawCameras(Display *g)
{
	   if (!showCameras) return;

	   int size = TrackCameras->size();
	   for(int i=0;i<size;i++)
       {
		   Camera *t = (Camera*)TrackCameras->elementAt(i);
		   t->DrawCamera(g,this);
	   }
 
}

void GPTrack::drawBlackFlags(Display *g)
{
	   if (!showBlackFlags) return;
	   TrackSection *first = (TrackSection*)TrackSections->elementAt(0);

	   Vector *rightBlackFlags =  first->getCommands(0xdf);
	   Vector *leftBlackFlags =   first->getCommands(0xde);

	   if (rightBlackFlags)
	   {
		int rightFlagsSize =rightBlackFlags->size(); 
	    for(int i=0;i<rightFlagsSize;i++)
        {
		   TrackCmd *t = (TrackCmd*)rightBlackFlags->elementAt(i);
		   if (t)
		   {
			int numArgs = t->getCmdNumArgs();
			if (numArgs > 1)
			{
		     double begin = t->getNthArg(2);

		     //find which sector this lies in
		     TrackSection *flagInSection = SectorForLength(begin);
		     double distance_to_section = LengthToTrackSector(flagInSection);
		     double sectionFlagDist = begin-distance_to_section;						   		   
		     flagInSection->drawFlag(g,TRUE);
			}
			else
			{
				//AfxMessageBox("No enough args");
			}
		   }
	    }
		delete rightBlackFlags;
	   }
	   if (leftBlackFlags)
	   {
		int leftFlagSize = leftBlackFlags->size();
	    for(int i=0;i<leftFlagSize;i++)
        {
		   TrackCmd *t = (TrackCmd*)leftBlackFlags->elementAt(i);
		   if (t)
		   {
			int numArgs = t->getCmdNumArgs();
			if (numArgs > 1)
			{		     
		    double begin = t->getNthArg(2);

		    //find which sector this lies in
		    TrackSection *flagInSection = SectorForLength(begin);
		    double distance_to_section = LengthToTrackSector(flagInSection);
		    double sectionFlagDist = begin-distance_to_section;						   		   
		    flagInSection->drawFlag(g,FALSE);
			}
			else
			{
				//AfxMessageBox("Black flag not enough args");
			}
		   }
	    }
		delete leftBlackFlags;
	   }
 
}



void GPTrack::drawCCLine(Display *g)
{
	   double trackWidth = TRACK_WIDTH;


	   TrackSection *t = (TrackSection*)TrackSections->elementAt(0);
	   double xpos = TRACK_START_POSX;
	   double ypos = TRACK_START_POSY;
	   if (t)
	   {
	    xpos = t->getStartX();
	    ypos = t->getStartY();
	   }
	   double currangle = 0;

	   xpos += CCLineStartX.GetValue();
	   ypos -= CCLineStartY.GetValue();

	   g->setColor(2);

	   int size = CCLineSections->size(); 
	   for(int i=0;i<size;i++)
       {
           CCLineSection *t = (CCLineSection*)CCLineSections->elementAt(i);
		   xpos = t->getStartX();
	       ypos = t->getStartY();

		   
		   if (t->cmd == 0x50) g->setColor(2);
		   else                g->setColor(3);

		   if (showCCLine)
             t->drawLine(g,&xpos,&ypos,&currangle,ypos);

		   if (showCCNumbers && showCCLine)
	       {
		     char buffer[10];
		     double txpos = (xpos);
	         double typos = (ypos);

		     wsprintf(buffer,"%d",i);
			 //g->setColor(0);
		     g->drawText(txpos,typos,buffer);
		   }
	   }  
}


void GPTrack::drawPitlane(Display *g)
{
    if (TrackSections == NULL || TrackSections->size() == 0) return;

    TrackSection *pitlaneStart = (TrackSection*)TrackSections->elementAt(PitLaneEntryIndex);

	double currentangle = 0;
	double xpos = TRACK_START_POSX;
	double ypos = TRACK_START_POSY;

	if (pitlaneStart)
	{
      currentangle = RadToDeg(pitlaneStart->getStartAngle());
      xpos = pitlaneStart->getStartX();
      ypos = pitlaneStart->getStartY();
	}
	double height=0;
	double length;
	double nextypos;
	double nextxpos;
	double trackWidth = PIT_WIDTH;

	int size = PitlaneSections->size();
    for(int i=0;i<size;i++)
    {
       TrackSection *t = (TrackSection*)PitlaneSections->elementAt(i);

       double angle = t->getAngle();
           
       height += t->getHeight();
       length  = t->getLength();

           double anglelen = length*angle;
           double anglerad = DegToRad(anglelen);
           double currrad  = DegToRad(currentangle);

           double sectorStartAngle = currrad;
           double sectorEndAngle   = currrad;
           double sectorRadius     = 0;

           if (angle!=0)
           {
            double beta = anglelen/2.0;
            double inrad        = DegToRad(beta);
            sectorRadius       = (length/anglerad);
            double chordrad     = DegToRad((anglelen/2.0));
            double len = 2*(sectorRadius*sin(inrad));

            nextypos =  len*sin((currrad+inrad));
            nextxpos =  len*cos((currrad+inrad));
            
            sectorEndAngle = currrad+inrad;
            currentangle += anglelen;
           }
           else
           {
            nextypos = length*sin(currrad);
            nextxpos = length*cos(currrad);
            
           }

           t->setStartX(xpos);
           t->setStartY(ypos);
           t->setEndX(nextxpos);
           t->setEndY(nextypos);
           t->setStartAngle(sectorStartAngle);
           t->setEndAngle(sectorEndAngle);
		   t->setRadius(sectorRadius);

		   if (showPitLane)
             t->drawTrack(g,angle,this);			  

		   if (showObjects && showPitLane)
		     t->drawObjects(g,xpos,ypos,currrad,showBitmaps);

		   if (showOutlines)
			 DrawObjectOutline(g,t);

		   //if (showRoadSigns)
		   // t->drawRoadSign(g);

	


		   TrackSection *tn;

		   if (i<PitlaneSections->size()-1)
		   {
		    tn = (TrackSection*)PitlaneSections->elementAt(i+1);
		   }
		   else
		   {
			tn = (TrackSection*)PitlaneSections->elementAt(i);
		   }

		   if (showPitWalls)
		   {
			 TrackSection *prev = GetPreviousSection(t);
		     t->drawWalls(g,t->getAngle(),this,prev,t);
		   }
		   
			 
           xpos = xpos+nextxpos;
           ypos = ypos+nextypos;
       }

	for( i=0;i<size;i++)
    {
       TrackSection *t = (TrackSection*)PitlaneSections->elementAt(i);

	   	   if (showCommands)
			 t->drawCommands(g);
		   if (showTextureMappingCommands)
			 t->drawTextureCommands(g);

		   if (showViewDistanceCommands)
			 t->drawViewDistanceCommands(g);


	}

	if (showWalls && showPitWalls)
	{
	  drawPitToTrackConnections(g);
	}


}

void GPTrack::drawPitToTrackConnections(Display *g)
{
	CPen *pen = new CPen(PS_SOLID,2,0xFF00FF);
	g->getGraphics()->SelectObject(pen);
    g->drawLine(pitLeftWallBeginX,pitLeftWallBeginY,trackWalltoPitLeftWallBeginX,trackWalltoPitLeftWallBeginY);
	g->drawLine(pitRightWallBeginX,pitRightWallBeginY,trackWalltoPitRightWallBeginX,trackWalltoPitRightWallBeginY);

	g->drawLine(pitLeftWallEndX,pitLeftWallEndY,trackWalltoPitLeftWallEndX,trackWalltoPitLeftWallEndY);
	g->drawLine(pitRightWallEndX,pitRightWallEndY,trackWalltoPitRightWallEndX,trackWalltoPitRightWallEndY);
	g->setColor(0);
	delete (pen);
}


void GPTrack::clearTrackSelections()
{
	int size = TrackSections->size();
	for(int i=0;i<size;i++)
    {
        TrackSection *t = (TrackSection*)TrackSections->elementAt(i);
		t->setSelected(FALSE);
	}
}

void GPTrack::clearPitSelections()
{
	int size = PitlaneSections->size();
	for(int i=0;i<size;i++)
    {
        TrackSection *t = (TrackSection*)PitlaneSections->elementAt(i);
		t->setSelected(FALSE);
	}
}


void GPTrack::setTrackSelection(TrackSection *t)
{
	clearTrackSelections();
	t->setSelected(TRUE);
}

void GPTrack::setPitSelection(TrackSection *t)
{
	clearPitSelections();
	t->setSelected(TRUE);
}

void GPTrack::setCCSelection(CCLineSection *t)
{
	clearCCSelection();
	t->setSelected(true);
}


void GPTrack::selectedSection(CPoint pt)
{
	       int size = TrackSections->size();
	       clearTrackSelections();
		
		   for(int i=0;i<size;i++)
           {
             TrackSection *t = (TrackSection*)TrackSections->elementAt(i);

			 if (t->contains(pt))
			 {
				 t->setSelected(TRUE);
				 break;
			 }
			 
		   }
	   }

void GPTrack::clearCCSelection()
{
	       int size = CCLineSections->size();
	       for(int i=0;i<size;i++)
           {
             CCLineSection *t = (CCLineSection*)CCLineSections->elementAt(i);
			 t->setSelected(FALSE);
		   }
}

void GPTrack::selectedCCSection(CPoint pt)
	   {
	       clearCCSelection();

		   CCLineSection *selectedSection = NULL;
		   double minLength = 10;
		   int size = CCLineSections->size();
		   for(int i=0;i<size;i++)
           {
             CCLineSection *t = (CCLineSection*)CCLineSections->elementAt(i);

			 
			 double xpos = t->getStartX();
			 double ypos = t->getStartY();
			 
			 
			 double dx = pt.x - xpos;
			 double dy = pt.y - ypos;

			 double length = sqrt(dx*dx+dy*dy);

			 if (length < minLength)
			 {
				selectedSection = t;
				minLength = length;
			 }
		   }

		   if (selectedSection!=NULL)
		   {
		     selectedSection->setSelected(TRUE);
			 CTrackEditorDoc *pDoc = (CTrackEditorDoc*)GetDocument();
			 if (pDoc)
			 {
			  pDoc->getTree()->Expand(pDoc->getTree()->GetParentItem(selectedSection->getTreeNode()),TVE_EXPAND);
	          pDoc->getTree()->SelectItem(selectedSection->getTreeNode());
			 }
		   }
		   
	   }


void GPTrack::selectedPitSection(CPoint pt)
{
	int size = PitlaneSections->size();
	for(int i=0;i<size;i++)
    {
       TrackSection *t = (TrackSection*)PitlaneSections->elementAt(i);
	   t->setSelected(FALSE);
	}
		
	for(i=0;i<size;i++)
    {
		TrackSection *t = (TrackSection*)PitlaneSections->elementAt(i);

		if (t!=NULL && t->contains(pt))
		{
			t->setSelected(TRUE);
			break;
		}
	}
}

TrackSection * GPTrack::getTrackSelection()
{
	int size = TrackSections->size();
	for(int i=0;i<size;i++)
    {
	  TrackSection *t = (TrackSection*)TrackSections->elementAt(i);

	  if (t->isSelected()) return t;
	}
	return NULL;
}

CCLineSection * GPTrack::getCCSelection()
{
	int size = CCLineSections->size();
	for(int i=0;i<size;i++)
    {
	  CCLineSection *t = (CCLineSection*)CCLineSections->elementAt(i);

	  if (t->isSelected()) return t;
	}
	return NULL;
}


TrackSection * GPTrack::getPitLaneSelection()
{
	int size = PitlaneSections->size();
	for(int i=0;i<size;i++)
    {
	  TrackSection *t = (TrackSection*)PitlaneSections->elementAt(i);

	  if (t->isSelected()) return t;
	}
	return NULL;
}


int GPTrack::getTrackSelectionIndex()
{
	int size = TrackSections->size();
	for(int i=0;i<size;i++)
    {
	  TrackSection *t = (TrackSection*)TrackSections->elementAt(i);

	  if (t->isSelected()) return i;
	}
	return -1;
}
			
int GPTrack::getCCSelectionIndex()
{
	int size = CCLineSections->size();
	for(int i=0;i<size;i++)
    {
	  CCLineSection *t = (CCLineSection*)CCLineSections->elementAt(i);

	  if (t->isSelected()) return i;
	}
	return -1;
}

int GPTrack::getPitSelectionIndex()
{
	int size = PitlaneSections->size();
	for(int i=0;i<size;i++)
    {
	  TrackSection *t = (TrackSection*)PitlaneSections->elementAt(i);

	  if (t->isSelected()) return i;
	}
	return -1;
}

int GPTrack::getObjectSelectionIndex()
{
	int size = TrackObjectDefintions->size();
	for(int i=0;i<size;i++)
    {
	  TrackObjectDefinition *t = (TrackObjectDefinition*)TrackObjectDefintions->elementAt(i);

	  if (t->isSelected()) return i;
	}
	return -1;
}

typedef struct 
{
	char *circuit;
	char *country;
	char *author;
	char *years;
	char *event;
	char *copyright;
	char *desc;
	int laps;
	int MagicNumber;
	int bestSlot;
	int tyreConsumption;
} t_TrackDesc;


static t_TrackDesc AllTracks[] = 
{
	{"Nurburgring","Germany","Instant Access","1996","","","",99,317080380,1,25000},
    {"Interlagos","Brazil","Microprose","1996","","","",71,12747392,1,25000},
	{"T1 Circuit Aida","Pacific GP","Microprose","1996","","","",83,105641373,1,25000},
	{"Imola","San Marino","Microprose","1996","","","",61,-3150168,1,25000},
	{"Melbourne","Austraila","Instant Access","1996","","","",99,-381277325,1,25000},
	{"Paul Richard","France","Instant Access","1996","","","",99,786565684,1,25000},
    {"Monte-Carlo","Monaco","Microprose","1996","","","",78,46867275,1,25000},
	{"Silverstone","England","Microprose","1996","","","",60,629112540,1,25000},
	{"Suzuka","Japan","Microprose","1996","","","",53,-550844285,1,25000},
	{"Monza","Italy","Microprose","1996","","","",53,-482661254,1,25000},
	{"Spa-Francorchamps","Belgium","Microprose","1996","","","",44,49590866,1,25000},	
	{"Estoril","Portual","Microprose","1996","","","",71,-46537200,1,25000},
	{"Jerez","Europe GP","Microprose","1996","","","",69,458945425,1,25000},
	{"Adelaide","Australia","Microprose","1996","","","",81,-406537992,1,25000},
	{"Barcelona","Spain","Microprose","1996","","","",65,237939922,1,25000},
	{"Gilles Villeneuve","Canada","Microprose","1996","","","",69,252711705,1,25000},
	{"Magny Cours","France","Microprose","1996","","","",72,146684314,1,25000},
	{"Hockenheim","Germany","Microprose","1996","","","",45,-508819110,1,25000},
	{"Brands Hatch","England","Instant Access","1996","","","",99,-121054442,1,25000},
	{"Hungaroring","Hungry","Microprose","1996","","","",99,87614174,1,25000},
    // new Tracks
	{"Monza","Italy","John Doe","1968","","","",99,96162195,1,25000},
};

static trackDescrs = sizeof(AllTracks)/sizeof(AllTracks[0]);

void GPTrack::GuessTrackIdenty(BOOL complain)
{
	CalculateCheckSum();

	unsigned short low1  = firstCheckSum & 0x00FF;
	unsigned short high1 = (firstCheckSum  & 0xFF00) >> 8;
	unsigned short low2  = secondCheckSum & 0x00FF;
	unsigned short high2 = (secondCheckSum  & 0xFF00) >> 8;

	long val = firstCheckSum * secondCheckSum;

	BOOLEAN foundTrack=FALSE;
	for(int i=0;i<trackDescrs;i++)
	{
		if (val == AllTracks[i].MagicNumber)
		{
		   setCircuitName(AllTracks[i].circuit);
		   setCircuitCountry(AllTracks[i].country);
		   setCircuitAuthor(AllTracks[i].author);
		   setCircuitYears(AllTracks[i].years);
		   circuitLaps = AllTracks[i].laps;
		   circuitSlot = AllTracks[i].bestSlot;
		   circuitTyreConsumption = AllTracks[i].tyreConsumption;
		   foundTrack = TRUE;
		   break;
		}
	}
	if (!foundTrack && complain)
	{
		char mess[256];
		wsprintf(mess,"The track you have loaded has no GP2INFO added and is not regocnised by Track Editor as a registered track and its name could not be found this track has the Magic Number %ld",val);
		AfxMessageBox(mess,MB_OK);
	}
}

void GPTrack::clearSelectedCameras()
{
	for(int i=0;i<TrackCameras->size();i++)
    {
		Camera *t = (Camera*)TrackCameras->elementAt(i);
		t->setSelected(FALSE);
	}
}

void GPTrack::setCameraSelection(Camera *cam)
{
	clearSelectedCameras();
	for(int i=0;i<TrackCameras->size();i++)
    {
		Camera *t = (Camera*)TrackCameras->elementAt(i);

		if (cam == t){
			cam->setSelected(TRUE);
		}
	}
}

Camera* GPTrack::selectedCamera(CPoint pt)
{
	double minDistance = 10000;

	Camera *CurrentSelectedCamera=NULL;

	for(int i=0;i<TrackCameras->size();i++)
    {
		Camera *t = (Camera*)TrackCameras->elementAt(i);
		t->setSelected(FALSE);
	}

	for(i=0;i<TrackCameras->size();i++)
    {
		Camera *t = (Camera*)TrackCameras->elementAt(i);

	    double dist = t->DistanceFromPt(pt);

		if (dist < minDistance)
		{
			  CurrentSelectedCamera = t;
			  minDistance = dist;
		}
	}

	

	if (CurrentSelectedCamera==NULL) AfxMessageBox("Select Camera Failed");
	else CurrentSelectedCamera->setSelected(TRUE);
	return CurrentSelectedCamera;
}

Camera* GPTrack::GetSelectedCamera()
{
	for(int i=0;i<TrackCameras->size();i++)
    {
		Camera *t = (Camera*)TrackCameras->elementAt(i);
		if (t->getSelected()) return t;
	}
	return NULL;
}


TrackObject * GPTrack::selectedObject(CPoint pt)
{
	double minDistance = 10000;
	TrackObject *CurrentSelectedObject=NULL;

	for(int i=0;i<TrackSections->size();i++)
    {
      TrackSection *t = (TrackSection*)TrackSections->elementAt(i);
	  Vector *SelObjects  = t->getObjects();
	  for(int j=0;j<SelObjects->size();j++)
	  {
		  TrackObject *selobj = (TrackObject*)SelObjects->elementAt(j);
		  selobj->setSelected(FALSE);
	  }
	  // could be dangerous
	  if (SelObjects)
		  delete(SelObjects);
	}
		
	for(i=0;i<TrackSections->size();i++)
    {
       TrackSection *t = (TrackSection*)TrackSections->elementAt(i);
	   Vector *SelObjects  = t->getObjects();
	   for(int j=0;j<SelObjects->size();j++)
	   {
		  TrackObject *selobj = (TrackObject*)SelObjects->elementAt(j);
		  selobj->setTrackSection(t);
		  double dist = selobj->DistanceFromPt(pt);

		  if (dist < minDistance)
		  {
			  CurrentSelectedObject = selobj;
			  minDistance = dist;
		  }
	   }
	   // could be dangerous
	   if (SelObjects)
	   	 delete(SelObjects);
	}

	if (CurrentSelectedObject!=NULL)
	          CurrentSelectedObject->setSelected(TRUE);

	return CurrentSelectedObject;
}

TrackObject * GPTrack::GetSelectedObject()
{
	int size = TrackSections->size();
	for(int i=0;i<size;i++)
    {
      TrackSection *t = (TrackSection*)TrackSections->elementAt(i);
	  if (t == NULL) continue;
	  Vector *SelObjects  = t->getObjects();
	  if (SelObjects!=NULL)
	  {
	   int objsize = SelObjects->size();
	   for(int j=0;j<objsize;j++)
	   {
		  TrackObject *selobj = (TrackObject*)SelObjects->elementAt(j);
		  if (selobj!=NULL)
		  {
		    if (selobj->isSelected()) return selobj;
		  }
	   }
	   // could be dangerous
	   if (SelObjects)
	   	  delete(SelObjects);
	  }
	}	
	return NULL;
}


BOOL GPTrack::FindTopLeft()
{
	CSize totalSize;
	BOOL resized = FALSE;

	totalSize.cx = -128;
	totalSize.cy = -128;

	int size = TrackSections->size();

	for(int i=0;i<size;i++)
    {
             TrackSection *t = (TrackSection*)TrackSections->elementAt(i);
			 totalSize.cx = (long)min(totalSize.cx,t->getStartX());
			 totalSize.cy = (long)min(totalSize.cy,t->getStartY());
			 totalSize.cx = (long)min(totalSize.cx,t->getEndX());
			 totalSize.cy = (long)min(totalSize.cy,t->getEndY());
	}

	//TRACE("%d %d\n",totalSize.cx,totalSize.cy);

	if (totalSize.cx < 0)
	{
		 TrackXBeginPos+=abs(totalSize.cx)+20;
		 resized = TRUE;
	}

	if (totalSize.cy < 0)
	{
		 TrackYBeginPos+=abs(totalSize.cy)+20;
		 resized = TRUE;
	}
	return resized;	
}


int GPTrack::totalLength()
{
	double totalLength=0;
	int size = TrackSections->size();

	for(int i=0;i<size;i++)
    {
             TrackSection *t = (TrackSection*)TrackSections->elementAt(i);

			 double len = fabs(t->getTrueLength());

			 totalLength += len;
					
	}
	return (int)totalLength;
}


int GPTrack::totalPitLength()
{
	double totalLength=0;

	int size = PitlaneSections->size();
	for(int i=0;i<size;i++)
    {
             TrackSection *t = (TrackSection*)PitlaneSections->elementAt(i);
			 double len = fabs(t->getTrueLength());
			 totalLength += len;
					
	}
	return (int)totalLength;
}

int GPTrack::MaxStraightLength()
{
	double totalLength=0;

	for(int i=0;i<TrackSections->size();i++)
    {
             TrackSection *t = (TrackSection*)TrackSections->elementAt(i);	 
			 double ang = t->getAngle();
			 double len = t->getLength();

			 if (ang==0){
				 totalLength = max(totalLength,len);
			 }
	}
	return (int)totalLength;
}

int GPTrack::MinStraightLength()
{
	double totalLength=10E8;

	for(int i=0;i<TrackSections->size();i++)
    {
             TrackSection *t = (TrackSection*)TrackSections->elementAt(i);	 
			 double ang = t->getAngle();
			 double len = t->getLength();

			 if (ang==0){
				 totalLength = min(totalLength,len);
			 }
	}
	return (int)totalLength;
}

int GPTrack::MaxCurveLength()
{
	double totalLength=0;

	for(int i=0;i<TrackSections->size();i++)
    {
             TrackSection *t = (TrackSection*)TrackSections->elementAt(i);	 
			 double ang = t->getAngle();
			 double len = t->getLength();

			 if (ang!=0){
				 totalLength = max(totalLength,fabs(ang*len));
			 }
	}
	return (int)totalLength;
}

int GPTrack::MinCurveLength()
{
	double totalLength=10E8;

	for(int i=0;i<TrackSections->size();i++)
    {
             TrackSection *t = (TrackSection*)TrackSections->elementAt(i);	 
			 double ang = t->getAngle();
			 double len = t->getLength();

			 if (ang!=0){
				 totalLength = min(totalLength,fabs(ang*len));
			 }
	}
	return (int)totalLength;
}


void GPTrack::RemoveAllObjects()
{
	for(int i=0;i<TrackSections->size();i++)
    {
             TrackSection *t = (TrackSection*)TrackSections->elementAt(i);
			 
			 Vector *objs = t->getObjects();

			 for(int j=0;j<objs->size();j++)
			 {
				 TrackObject *obj = (TrackObject*)objs->elementAt(j);
				 t->RemoveObject(obj);
			 }

			 // could be dangerous
			 if (objs)
				 delete objs;
	}
}

void GPTrack::ZeroAllArgs(int cmdval)
{
	for(int i=0;i<TrackSections->size();i++)
    {
             TrackSection *t = (TrackSection*)TrackSections->elementAt(i);
			 
			 Vector *objs = t->getCommandIterator();
			 Vector *cmds = t->getCommands();

			 for(int j=0;j<objs->size();j++)
			 {
				 TrackCmd *cmd = (TrackCmd*)objs->elementAt(j);
				 if (cmdval != -1)
				 {
					 if (cmdval == cmd->cmd)
					 {
					   cmd->ZeroCommandArgs();
					   if (cmdval == 0xb8)
					   {
						   cmd->setNthArg(2,2);
					   }
					 }
				 }
				 else
				 {
			           cmd->ZeroCommandArgs();
				 }
			 }
			 
			 delete objs;
	}
}

void GPTrack::DefaultAllArgs(int cmdval)
{
	for(int i=0;i<TrackSections->size();i++)
    {
             TrackSection *t = (TrackSection*)TrackSections->elementAt(i);
			 
			 Vector *objs = t->getCommandIterator();
			 Vector *cmds = t->getCommands();

			 for(int j=0;j<objs->size();j++)
			 {
				 TrackCmd *cmd = (TrackCmd*)objs->elementAt(j);
				 if (cmdval != -1)
				 {
					 if (cmdval == cmd->cmd)
					 {
					   
					   if (cmdval == 0xb8)
					   {
						   cmd->setNthArg(2,2);
						   cmd->setNthArg(3,-3000);
						   cmd->setNthArg(5,-1500);
						   cmd->setNthArg(7,-1000);
						   cmd->setNthArg(9,1000);
						   cmd->setNthArg(11,1500);
						   cmd->setNthArg(13,3000);
						   cmd->setNthArg(4,1000);
						   cmd->setNthArg(6,500);
						   cmd->setNthArg(8,100);
						   cmd->setNthArg(10,100);
						   cmd->setNthArg(12,500);
						   cmd->setNthArg(14,1000);
					   }
					   else if (cmdval == 0xaf)
					   {
						   cmd->setNthArg(2,16387);
						   cmd->setNthArg(3,16384);
					   }
					   else
					   {
					       cmd->ZeroCommandArgs();
					   }
					 }
				 }
				 else
				 {
			           cmd->ZeroCommandArgs();
				 }
			 }
			 
			 delete objs;
	}
}

void GPTrack::RemoveAllCmds(int cmdval,int startingAt,int finishinAt)
{
	for(int i=startingAt;i<TrackSections->size();i++)
    {
             TrackSection *t = (TrackSection*)TrackSections->elementAt(i);
			 
			 Vector *objs = t->getCommandIterator();
			 Vector *cmds = t->getCommands();

			 for(int j=0;j<objs->size();j++)
			 {
				 TrackCmd *cmd = (TrackCmd*)objs->elementAt(j);
				 if (cmdval != -1)
				 {
					 if (cmdval == cmd->cmd)
					 {
					 cmds->removeElement(cmd);
					 }
				 }
				 else
				 {
			        cmds->removeElement(cmd);
				 }
			 }
			 
			 delete objs;
	}
}

void GPTrack::RemoveAllPitObjects()
{
	for(int i=0;i<PitlaneSections->size();i++)
    {
             TrackSection *t = (TrackSection*)PitlaneSections->elementAt(i);
			 
			 Vector *objs = t->getObjects();

			 for(int j=0;j<objs->size();j++)
			 {
				 TrackObject *obj = (TrackObject*)objs->elementAt(j);
				 t->RemoveObject(obj);
			 }

			 // could be dangerous
			 if (objs)
				 delete objs;
	}
}


TrackObjectDefinition * GPTrack::AddNewTrackObjectDefinition()
{
	int size = TrackObjectDefintions->size();
	TrackObjectDefinition *last = (TrackObjectDefinition*)TrackObjectDefintions->elementAt(size-1);
	int obdefoffset = last->getLocator();
	// make room
	obdefoffset+=16;

	TrackObjectDefinition *objdef = new TrackObjectDefinition(this,0);
	objdef->setLocator(obdefoffset);
	TrackObjectDefintions->addElement(objdef);

	//AfxMessageBox("Track Object Definitions cannot be saved yet!",MB_OK | MB_ICONWARNING);
	return objdef;
}

void GPTrack::RemoveAllPitCmds()
{
	for(int i=0;i<PitlaneSections->size();i++)
    {
             TrackSection *t = (TrackSection*)PitlaneSections->elementAt(i);
			 
			 Vector *objs = t->getCommands();

			 for(int j=0;j<objs->size();j++)
			 {
				 TrackCmd *obj = (TrackCmd*)objs->elementAt(j);
				 t->RemoveCommand(obj);
			 }
	}
}

void GPTrack::RemoveCommand(TrackCmd *cmd)
{
	for(int i=0;i<TrackSections->size();i++)
    {
             TrackSection *t = (TrackSection*)TrackSections->elementAt(i);
			 
			 Vector *objs = t->getCommands();

			 for(int j=0;j<objs->size();j++)
			 {
				 TrackCmd *obj = (TrackCmd*)objs->elementAt(j);
				 if (obj == cmd) t->RemoveCommand(obj);
			 }
	}
	for(i=0;i<PitlaneSections->size();i++)
    {
             TrackSection *t = (TrackSection*)PitlaneSections->elementAt(i);
			 
			 Vector *objs = t->getCommands();

			 for(int j=0;j<objs->size();j++)
			 {
				 TrackCmd *obj = (TrackCmd*)objs->elementAt(j);
				 if (obj == cmd) t->RemoveCommand(obj);
			 }
	}
}


void GPTrack::ImportNewInternalObject(CDocument *pDoc,LPCSTR filename)
{
	 // importing a new object!
	 InternalObject *last = (InternalObject*)InternalObjectOffsets->lastElement();

	 int lastOffset = ListObjectOffset;
	 
	 InternalObject *obj = new InternalObject(this,pDoc,lastOffset,lastOffset);

	 //if (obj->ImportFromFile(filename))
	 if (obj->ImportObject(filename))
	 {
		 int size = obj->getSize();
		 InternalObjectOffsets->addElement(obj);


		 for(int i=0;i<InternalObjectOffsets->size();i++)
		 {
			InternalObject *elm = (InternalObject*)InternalObjectOffsets->elementAt(i);
			elm->setOffset(elm->getOffset()+4);
		 }

		 ListObjectOffset = obj->getOffset()+size;
		 NumberOfObjects = InternalObjectOffsets->size();

		 //adjust objects pointers.....
		 obj->AdjustPointers();

		 //adjust LargerNumber1
		 int largeNumber1 = LargeNumber1.GetValue();
		 LargeNumber1.SetValue(largeNumber1-4);

		 // move starting light!
		 int jamunk = JamFileUnknown.GetValue();
		 JamFileUnknown.SetValue(jamunk+4);
	 }
	 else
	 {
		 delete obj;
	 }
}


Model3D *GPTrack::createSceneryModel(Display *g)
{
	Model3D *allSegments = NULL;

	for(int i=0;i<TrackSections->size();i++)
	{
		TrackSection *t = (TrackSection*)TrackSections->elementAt(i);

		Model3D * scenerySegs = t->drawScenery3D(g);
		if (scenerySegs)
		{
			if (allSegments == NULL)
			{
			  allSegments = new Model3D();
			}
		    allSegments->addPrimitive(scenerySegs);
		}

	}
	return allSegments;
}


void GPTrack::ScaleTrack(float scale)
{
	for(int i=0;i<TrackSections->size();i++)
	{
		TrackSection *t = (TrackSection*)TrackSections->elementAt(i);

		double oldLength = t->getLength();
		double newLength = oldLength;
		
		double oldAngle = t->getAngle();

		if (oldAngle!=0)
		{
		    //double secorRad = 180.0/(PI*oldAngle);

			//double angleLen  = oldLength * oldAngle;
			//double anglerad  = DegToRad(angleLen);
			//double secorRad  = oldLength/anglerad;
			//double nsecorRad = 	secorRad*scale;
			//double newAngle = 	180.0 /(nsecorRad*PI);

			//double newAngleRad  = (oldLength/nsecorRad);
			//double newAngle     = RadToDeg(newAngleRad);
			//newLength           = nsecorRad*newAngleRad;

			//t->setAngle((int)newAngle);
			//newLength = 180.0/(PI*nsecorRad);
			//newAngle  = angleLen/newLength;
			//t->setAngle((int)newAngle);
			newLength =  oldLength;
		}
		else
		{
			 newLength = (oldLength*scale);
		}
		if (newLength<1) newLength=1;
		t->setLength((int)newLength);
	}

	createTrack(GetDocument());
}

BOOL GPTrack::isPitLaneLeftSide()
{
	int val =  LOBYTE(C5Code.GetValue());

	if (val & 10) return TRUE;
	return FALSE;
}

void GPTrack::ExportCleverNames()
{
	CString idToNames[256];

	FILE *fp = fopen("clevernames.txt","w");

	if (fp == NULL) 
	{
		AfxMessageBox("Cannot open clevername.txt");
		return;
	}

	for(int i=0;i<256;i++)
	{
		idToNames[i] = CString("Unknown");
	}

	int maxId =0;

	for(i=0;i<TrackObjectDefintions->size();i++)
	{
		TrackObjectDefinition *obj = (TrackObjectDefinition*)TrackObjectDefintions->elementAt(i);

		int id1 = obj->id;
		maxId = max(id1,maxId);

		idToNames[id1] = CString(obj->getName());
	}

	for(i=0;i<maxId;i++)
	{
		fprintf(fp,"%d - 0 - %s\n",i,idToNames[i]);
	}

	fclose(fp);
}

BOOL GPTrack::isRibbonOn(int ts,int myribbon)
{
	 BOOL ribbon=FALSE;
	 int size = TrackSections->size();

	 for(int i=0;i<ts;i++)
	 {
		 TrackSection *t = (TrackSection*)TrackSections->elementAt(i);

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
				   //int myribbon = IntToRibbon(j);
				   if (value & myribbon)
					   ribbon = TRUE;
			   } break;
			   case 0xB0:
			   {
				   int value = cmd->getNthArg(2);
				   //int myribbon = IntToRibbon(j);
				   if (value & myribbon)
					   ribbon = FALSE;
			   } break;
			 }
		 }
	 }
	 return ribbon;
}


void GPTrack::DrawSplits(Display *display)
{
	double total  = totalLength();

	double split1 = (total/100.0)*33.5;
	double split2 = (total/100.0)*67.5;

	TrackSection *SplitSector1 = SectorForLength(split1);
	TrackSection *SplitSector2 = SectorForLength(split2);

	double distance_to_section = LengthToTrackSector(SplitSector1);
    double dist1 = split1-distance_to_section;
	       distance_to_section = LengthToTrackSector(SplitSector2);
    double dist2 = split2-distance_to_section;
		     
	CString spTxt1;
	CString spTxt2;

	spTxt1.Format("?Split 1 in Sector %d dist=%f ",SplitSector1->getIndex(),split1);
	spTxt2.Format("?Split 2 in Sector %d dist=%f ",SplitSector2->getIndex(),split2);
	

	SplitSector1->HighlightSector(display,this,dist1,(LPSTR)(LPCSTR)spTxt1);
	SplitSector2->HighlightSector(display,this,dist2,(LPSTR)(LPCSTR)spTxt2);
}

int GPTrack::WriteObjNames(FILE *fp)
{
	CString buffer;

	if (InternalObjectNames->size())
	{
	 buffer+="#GP2OBJINFO|";
	 for(int i=0;i<InternalObjectNames->size();i++)
	 {
		IO_name *name = (IO_name*)InternalObjectNames->elementAt(i);
		CString msg;
		int id = InternalObjectOffsets->findElementIndex(name->obj);
		msg.Format("%d|%s|",id,name->name);
		buffer+=msg;
	 };
	 buffer+="#";
	}
	else
	{
	 buffer="                                                    ";
	}

	 for(int i =0;i<buffer.GetLength();i++)
	 {
		trackdata[512+i] = buffer[i];
	 }
	return 0;
}

void GPTrack::ReadObjNames()
{
	if (trackdata[512]=='#' && trackdata[513]=='G' && trackdata[514]=='P')
	{
	  //found track info
	}
	else 
		return;


	// jump past header
	int count = 512+12;

	// number and name
	char cat[2000];
	char val[2000];
	int last = 2048;

	InternalObjectNames->removeAllElements();
	memset(cat,0,256);
	memset(val,0,256);

	BOOL finished = FALSE;

	while(!finished)
	{
		if (count >= last) break;

		int pos=0;
		//get catagory
		while(count<last)
		{
		  char a =trackdata[count];
		  if ( a== '|') break;
		  if (a == '#'){
					    finished = TRUE;
		                continue;
		               }
		  cat[pos++]=trackdata[count];
		  cat[pos]='\0';
		  count++;
		}
		count++;
		pos=0;
		// get class
		while(count<last)
		{
		  char a =trackdata[count];
		  if (a == '|')break;
		  if (a == '#'){
					    finished = TRUE;
		                continue;
		               }
		  val[pos++]=trackdata[count];
		  val[pos]='\0';
		  count++;
		}
		count++;
		char Mess[1024];

		wsprintf(Mess,"ObjName:=%s =%s",cat,val);
		//AfxMessageBox(Mess);

		if (cat && strcmp(cat,"") && strlen(cat)>0)
		{
		 int objId = atoi(cat);

		 if (InternalObjectOffsets && objId >= 0 && objId <= InternalObjectOffsets->size())
		 {
		  InternalObject *obj = (InternalObject*)InternalObjectOffsets->elementAt(objId);
		  obj->setObjectName(strdup(val));
		  obj->MagicNumber = 0;
		  InternalObjectNames->addElement(new IO_name(obj,val));
		 }
		}

		if (trackdata[count]=='#')break;

		memset(val,0,256);
	}
}

int GPTrack::totalCommands()
{
	return 	(totalTrackCommands()+totalPitCommands());
}
int GPTrack::totalTrackCommands()
{
	int numCommands = 0;
	for(int i=0;i<TrackSections->size();i++)
	{
		// add one for track section
		numCommands++;
		TrackSection *t = (TrackSection*)TrackSections->elementAt(i);
		Vector *cmds = t->getCommands();
		numCommands+=cmds->size();
	}
	return numCommands;
}
int GPTrack::totalPitCommands()
{
	int numCommands = 0;
	for(int i=0;i<PitlaneSections->size();i++)
	{
		// add one for track section
		numCommands++;
		TrackSection *t = (TrackSection*)PitlaneSections->elementAt(i);
		Vector *cmds = t->getCommands();
		numCommands+=cmds->size();
	}
	return numCommands;
}

void  GPTrack::Insert112CmdSection()
{
	MakeBackupNow();
	regenerate = TRUE;
	CCLineSection *t = getCCSelection();

	if (t)
	{
	     CCLineSection *ts = new CCLineSection(this,10,0,0,0,0,0,0x70,0);
           
         CCLineSections->insertAfterElement(t,ts);
	     createCC();
	}
	else
	{
		AfxMessageBox("No current CC line selection");
	}
}

void GPTrack::DrawObjectOutline(Display *g,TrackSection *t)
{
	     double xpos     = t->getStartX();
         double ypos     = t->getStartY();
         double nextxpos = t->getEndX();
         double nextypos = t->getEndY();
         double sectorStartAngle   = t->getStartAngle();
		 double zpos     = t->getStartZ();

         //if (showObjects)
		 {
		 Vector *TrackSideObjects = t->getObjects();
		 int objsize = TrackSideObjects->size();
         for(int i=0;i<objsize;i++)
         {
			 TrackObject *ob = (TrackObject*)TrackSideObjects->elementAt(i);
		     if (t!=NULL)
		     {
               double trackWidth = t->getWidthLeft();
			   double nextypos;
		       double nextxpos;
		       double remainder = ob->getDistance();
		       double length    = t->getLength();
			   double angle     = t->getAngle();
			   double stAngle   = sectorStartAngle;

		       if (angle != 0)
		       {
		         CPoint trackPt = GetTrackPoint(stAngle,angle,remainder);
                 nextypos = trackPt.x;
                 nextxpos = trackPt.y;
	             double deltaAngle = DegToRad(angle*length)*(remainder/length);
	             angle=stAngle+deltaAngle;
		       }
		       else
		       {
		         nextypos = ob->getDistance()*sin(stAngle);
                 nextxpos = ob->getDistance()*cos(stAngle);
		         angle = stAngle;
		        }
		 

				double xr = xpos+nextxpos;
				double yr = ypos+nextypos;
				double obposx;
				double obposy;

				TrackObjectDefinition *obj = ob->getObjectDef();

		
				if (obj!=NULL)
				{
					double distance = obj->getDistance();
					if (distance > 0)
					{
					double di = WIDTH_TO_TRACKWIDTH_OBJECTS(fabs(distance));
					//double di = (fabs(distance))/150.0;
					obposx = xpos+nextxpos+getRightSideX(angle,di);
					obposy = ypos+nextypos+getRightSideY(angle,di);
					}
					else
					{
					double di = WIDTH_TO_TRACKWIDTH_OBJECTS(fabs(distance));
					//double di = (fabs(distance))/150.0;
					obposx = xpos+nextxpos+getLeftSideX(angle,di);
					obposy = ypos+nextypos+getLeftSideY(angle,di);						
					}
		 
					double glxpos =  obposx;
					double glypos =  obposy;
					double glzpos =  zpos;

					//CDC *pDC = CDC::FromHandle(hdc);
					obj->Draw2D(g,glxpos,glypos,glzpos,angle,showAxis);
				}
			}
		  }
		  if (TrackSideObjects) delete TrackSideObjects;
		 }
}

void GPTrack::MakeBackupNow()
{
	if (isF1GP()) return;

	CString msg;
	char buff[1024];

	if (!((CTrackEditorApp*)AfxGetApp())->AutoBackup) return;

	GetTempPath(1024,buff);

	msg.Format("%s\\GP2TE",buff);

	_mkdir(msg);

	msg.Format("%s\\GP2TE\\TE%4.4d.dat",buff,GP2BackupNumber);

	RecreateData();	
	WriteTrackFile(msg);
	GP2BackupNumber++;

	if (GP2BackupNumber >= GP2MaxBackupNumber) GP2BackupNumber = 1;
}

#define KDIFF 0.01
#define KWIDTH 150

#define KERB_1_WIDTH 100
#define KERB_2_WIDTH 500
#define KERB_1_HEIGHT 0.05
#define KERB_2_HEIGHT 0.1

#define GET_GL_PT_X(x) (((x))/SC)
#define GET_GL_PT_Y(y) (((y))/SC)
#define GET_GL_PT_Z(z) (((z))/SC)

#define GL_POLYGON 1

#include "fstream.h"

class glPolygonItem
{
	public:
		glPolygonItem()
		{
			r = 1.0;
			g = 0.0;
			b = 1.0;
			pts = new Vector("Pts",__LINE__);
		}
		~glPolygonItem()
		{
			pts->removeAllData();
		};
	Vector *pts;
	double r,g,b;
};

static glPolygonItem * currentPolygon;
static double red,green,blue;

void vmlBegin(int item)
{
	currentPolygon = new glPolygonItem();
}

void vmlEnd(ofstream &file)
{
	file << "Shape" << endl;
	file << "{" << endl; 
	file << "\tappearance Appearance " << endl;
	file << "{" << endl; 		
	file << "material Material { diffuseColor " ;
	file << red << " " << green << " " << blue << " }" << endl;
		//material Material { diffuseColor 1 1 0 } 		
	file << "}" << endl; 
	file << "\tgeometry IndexedFaceSet { " << endl;
	file << "\tcoord Coordinate { point [ " ;
	for(int i=0;i<currentPolygon->pts->size();i++)
	{
		if (i > 0) file << ", ";
		Point3D *pt = (Point3D *)currentPolygon->pts->elementAt(i);
		file << pt->x << " " << pt->y << " " << pt->z;
	}
	file << "] }" << endl;
		//coord Coordinate { point [ 1 0 1, -1 0 1, -1 0 -1, 1 0 -1, 0 2 0 ] } 
	//file << "coordIndex [ 0 1 2 3 0]" << endl;
	file << "coordIndex [ ";
	for(i=0;i<currentPolygon->pts->size();i++)
	{
		file << " " << i ;
	}
	file << " 0]" << endl;
	//file << "coordIndex [ 0 1 2 3 0]" << endl;
		//coordIndex [ 0 1 2 3 0 -1, 1 0 4 -1, 2 1 4 -1, 3 2 4 -1, 0 3 4 -1 ] 
	file << "}" << endl;
	file << "}" << endl;
	delete currentPolygon;
	currentPolygon = NULL;
}

 void vmlColor3f(double x,double y,double z)
{
	if (currentPolygon){
		currentPolygon->r =x;
		currentPolygon->g =y;
		currentPolygon->b =z;
		
	}
	red = x;
		green = y;
		blue = z;
}

 void vmlVertex3d(double x,double y,double z)
{
	if (currentPolygon){
	   currentPolygon->pts->addElement(new Point3D(x,y,z));
	}
}

 void vmlTexCoord2f(double u,double v)
{
}



void GPTrack::CreateVRMLFile(char *filename)
{
		ofstream file(filename);
		 
		file << "#VRML V2.0 utf8" << endl;	


		GPTrack *mytrack = this;
		int size = mytrack->TrackSections->size();
		int to = size;
		int from = 0;
	    //InitView(from,to);
		//double MidX,MidY,MidZ,
		double SC;

		//double MinX=10E8;
		//double MinY=10E8;
		//double MinZ=10E8;
		//double MaxX=-10E8;
		//double MaxY=-10E8;
		//double MaxZ=-10E8;

		double currentangle  = ((360*mytrack->TrackStartAngle.GetValue())/65535);
		double xpos1 = mytrack->TrackStartX.GetValue()/128; 
		double ypos1 = mytrack->TrackStartY.GetValue()/128; 

		for(int i=from;i<to;i++)
		{
		 TrackSection *t = (TrackSection*)mytrack->TrackSections->elementAt(i);

		 double xpos = t->getStartX();
		 double ypos = t->getStartY();
		 double zpos = t->getStartZ();

		 //MinX = min(MinX,xpos);
		 //MinY = min(MinY,ypos);
		 //MinZ = min(MinZ,zpos);
		 //MaxX = max(MaxX,xpos);
		 //MaxY = max(MaxY,ypos);
		 //MaxZ = max(MaxZ,zpos);
		}

		//MidX = (MaxX+MinX)/2;
		//MidY = (MaxY+MinY)/2;
		//MidZ = 0.0;

		SC = 1.0;

	    //if (mytrack->showScenery)
		//    DrawScenery(mytrack);
		//int size = mytrack->TrackSections->size();
		
		
		TrackSection *tFrom = (TrackSection*)mytrack->TrackSections->elementAt(from);
		if (to >= size) to = size-1;
		TrackSection *tTo   = (TrackSection*)mytrack->TrackSections->elementAt(to);
		TrackSection *nextT = mytrack->GetNextSection(tFrom);
		TrackSection *tEnd  = mytrack->GetNextSection(tTo);
		TrackSection *tTerm = tTo;

		if (mytrack->showTrack)
		{
		//for(int i=from;i<to;i++)
		//
		TrackSection *t = tFrom;
		while (t)
		{ 
		 //int idx = mytrack->TrackSections->findElementIndex(t);
		 //TRACE("T[%d]\n",idx);
		 //vmlDisable(vml_TEXTURE_2D);
		 //TrackSection *t = (TrackSection*)mytrack->TrackSections->elementAt(i);
		 nextT = mytrack->GetNextSection(t);

		 double xpos = t->getStartX();
	     double ypos = t->getStartY();
		 double zpos = -t->getStartZ();
	     double nextxpos = nextT->getStartX();
		 double nextypos = nextT->getStartY();
		 double nextzpos = -nextT->getStartZ();
	     double sectorStartAngle = t->getStartAngle();
	     double sectorEndAngle   = t->getEndAngle();
		 double sectorStartAngleN  = nextT->getStartAngle();
		 //double zpos=0;

	     TrackSection *prev = mytrack->GetPreviousSection(t);

	     double previousLeft = t->getWidthLeft();
	     double previousRight = t->getWidthRight();

	     if (prev!=NULL)
	     {
	      previousLeft = prev->getWidthLeft();
	      previousRight = prev->getWidthRight();
	     }

	
	     // track parts
	     double leftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft));
	     double leftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft));
	     double rightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight));
	     double rightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight));
		 double KleftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft-KWIDTH));
	     double KleftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft-KWIDTH));
	     double KrightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight-KWIDTH));
	     double KrightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight-KWIDTH));

		 double Kerb1leftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft+KERB_1_WIDTH));
	     double Kerb1leftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft+KERB_1_WIDTH));
	     double Kerb1rightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight+KERB_1_WIDTH));
	     double Kerb1rightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight+KERB_1_WIDTH));

		 double Kerb2leftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft+KERB_2_WIDTH));
	     double Kerb2leftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousLeft+KERB_2_WIDTH));
	     double Kerb2rightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight+KERB_2_WIDTH));
	     double Kerb2rightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(previousRight+KERB_2_WIDTH));



           
	     // track parts
	     double leftSideNX = nextxpos+getLeftSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()));
	     double leftSideNY = nextypos+getLeftSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()));   
	     double rightSideNX = nextxpos+getRightSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()));
	     double rightSideNY = nextypos+getRightSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()));
		 double KleftSideNX = nextxpos+getLeftSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()-KWIDTH));
	     double KleftSideNY = nextypos+getLeftSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()-KWIDTH));   
	     double KrightSideNX = nextxpos+getRightSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()-KWIDTH));
	     double KrightSideNY = nextypos+getRightSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()-KWIDTH));


		 double Kerb1leftSideNX = nextxpos+getLeftSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+KERB_1_WIDTH));
	     double Kerb1leftSideNY = nextypos+getLeftSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+KERB_1_WIDTH));   
	     double Kerb1rightSideNX = nextxpos+getRightSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+KERB_1_WIDTH));
	     double Kerb1rightSideNY = nextypos+getRightSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+KERB_1_WIDTH));
		 double Kerb2leftSideNX = nextxpos+getLeftSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+KERB_2_WIDTH));
	     double Kerb2leftSideNY = nextypos+getLeftSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+KERB_2_WIDTH));   
	     double Kerb2rightSideNX = nextxpos+getRightSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+KERB_2_WIDTH));
	     double Kerb2rightSideNY = nextypos+getRightSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+KERB_2_WIDTH));



		 


		   int leftWall   =  prev->getLeftToBank()*0x40;
	       int rightWall  =  prev->getRightToBank()*0x40;

	       int EleftWall  = t->getLeftToBank()*0x40;
	       int ErightWall = t->getRightToBank()*0x40;
		  
           // track parts
           double WleftSideX = xpos+getLeftSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+leftWall));
           double WleftSideY = ypos+getLeftSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+leftWall));
		   double WrightSideX = xpos+getRightSideX(sectorStartAngle,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+rightWall));
           double WrightSideY = ypos+getRightSideY(sectorStartAngle,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+rightWall));
 
           // track parts
           double WleftSideNX = nextxpos+getLeftSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+EleftWall));
           double WleftSideNY = nextypos+getLeftSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthLeft()+EleftWall));           
           double WrightSideNX = nextxpos+getRightSideX(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+ErightWall));
           double WrightSideNY = nextypos+getRightSideY(sectorStartAngleN,WIDTH_TO_TRACKWIDTH(t->getWidthRight()+ErightWall));


		 
		 if (mytrack->showTextures){ 
			 //glEnable(GL_TEXTURE_2D);
			 //RoadBits = TextureMap(GetDC(),RoadBmp,RoadBits,FALSE);
	     }
		 //road
		 //vmlColor3f(0.6f,0.6f,0.6f);
		 vmlBegin(GL_POLYGON); 
		 vmlTexCoord2f(0.0f,0.0f);
		 vmlVertex3d(GET_GL_PT_X(leftSideX),GET_GL_PT_Y(leftSideY),GET_GL_PT_Z(zpos));
		 vmlTexCoord2f(1.0f,0.0f);
		 vmlVertex3d(GET_GL_PT_X(leftSideNX),GET_GL_PT_Y(leftSideNY),GET_GL_PT_Z(nextzpos));		 		 
	  	 vmlTexCoord2f(1.0f,1.0f);
		 vmlVertex3d(GET_GL_PT_X(rightSideNX),GET_GL_PT_Y(rightSideNY),GET_GL_PT_Z(nextzpos));
		 vmlTexCoord2f(0.0f,1.0f);
		 vmlVertex3d(GET_GL_PT_X(rightSideX),GET_GL_PT_Y(rightSideY),GET_GL_PT_Z(zpos));
		 vmlEnd(file);

		 if (mytrack->showTextures){ 
			 //glEnable(GL_TEXTURE_2D);
			 //VergeLeftBits = TextureMap(GetDC(),VergeLeftBmp,VergeLeftBits,FALSE);
	     }

		 // left bank
		 vmlColor3f(0.0f,0.6f,0.0f);
		 vmlBegin(GL_POLYGON);
		 vmlTexCoord2f(0.0f,0.0f);
		 vmlVertex3d(GET_GL_PT_X(leftSideX),GET_GL_PT_Y(leftSideY),GET_GL_PT_Z(zpos));
		 vmlTexCoord2f(0.0f,1.0f);
		 vmlVertex3d(GET_GL_PT_X(WleftSideX),GET_GL_PT_Y(WleftSideY),GET_GL_PT_Z(zpos));
		 vmlTexCoord2f(1.0f,1.0f);
		 vmlVertex3d(GET_GL_PT_X(WleftSideNX),GET_GL_PT_Y(WleftSideNY),GET_GL_PT_Z(nextzpos));
		 vmlTexCoord2f(1.0f,0.0f);
		 vmlVertex3d(GET_GL_PT_X(leftSideNX),GET_GL_PT_Y(leftSideNY),GET_GL_PT_Z(nextzpos));		 		 	 
		 vmlEnd(file);

		 
		 if (mytrack->showTextures){ 
			 //glEnable(GL_TEXTURE_2D);
			 //VergeRightBits= TextureMap(GetDC(),VergeRightBmp,VergeRightBits,FALSE);
	     }

		 // right bank
		 vmlColor3f(0.0f,0.6f,0.0f);
		 vmlBegin(GL_POLYGON);
		 vmlTexCoord2f(0.0f,0.0f);
		 vmlVertex3d(GET_GL_PT_X(rightSideX),GET_GL_PT_Y(rightSideY),GET_GL_PT_Z(zpos));
		 vmlTexCoord2f(1.0f,0.0f);
		 vmlVertex3d(GET_GL_PT_X(rightSideNX),GET_GL_PT_Y(rightSideNY),GET_GL_PT_Z(nextzpos));		 
		 vmlTexCoord2f(1.0f,1.0f);
		 vmlVertex3d(GET_GL_PT_X(WrightSideNX),GET_GL_PT_Y(WrightSideNY),GET_GL_PT_Z(nextzpos));
		 

		 vmlTexCoord2f(0.0f,1.0f);
		 vmlVertex3d(GET_GL_PT_X(WrightSideX),GET_GL_PT_Y(WrightSideY),GET_GL_PT_Z(zpos));
		 
		 vmlEnd(file);

		 if (mytrack->showWalls)
		 {

		 if (mytrack->showTextures){ 
			 //glEnable(GL_TEXTURE_2D);
			 //FenceLeftBits = TextureMap(GetDC(),FenceLeftBmp,FenceLeftBits,FALSE);
	     }
		 double wall_height = -0.3;
		 // left wall
		 vmlColor3f(0.8f,0.8f,0.8f);
		 vmlBegin(GL_POLYGON);
		 vmlTexCoord2f(0.0f,1.0f);
		 vmlVertex3d(GET_GL_PT_X(WleftSideX),GET_GL_PT_Y(WleftSideY),GET_GL_PT_Z(zpos));
		 vmlTexCoord2f(0.0f,0.0f);
		 vmlVertex3d(GET_GL_PT_X(WleftSideX),GET_GL_PT_Y(WleftSideY),GET_GL_PT_Z(zpos+wall_height));
		 vmlTexCoord2f(1.0f,0.0f);
		 vmlVertex3d(GET_GL_PT_X(WleftSideNX),GET_GL_PT_Y(WleftSideNY),GET_GL_PT_Z(nextzpos+wall_height));
		 vmlTexCoord2f(1.0f,1.0f);
		 vmlVertex3d(GET_GL_PT_X(WleftSideNX),GET_GL_PT_Y(WleftSideNY),GET_GL_PT_Z(nextzpos));		 
		 
		 vmlEnd(file);

		 if (mytrack->showTextures){ 
			 //glEnable(GL_TEXTURE_2D);
			 //FenceRightBits = TextureMap(GetDC(),FenceRightBmp,FenceRightBits,FALSE);
	     }

		 // right wall
		 vmlColor3f(0.8f,0.8f,0.8f);
		 vmlBegin(GL_POLYGON);
		 vmlTexCoord2f(1.0f,1.0f);
		 vmlVertex3d(GET_GL_PT_X(WrightSideX),GET_GL_PT_Y(WrightSideY),GET_GL_PT_Z(zpos));
		 vmlTexCoord2f(0.0f,1.0f);
		 vmlVertex3d(GET_GL_PT_X(WrightSideNX),GET_GL_PT_Y(WrightSideNY),GET_GL_PT_Z(nextzpos));		 		 
		 vmlTexCoord2f(0.0f,0.0f);
		 vmlVertex3d(GET_GL_PT_X(WrightSideNX),GET_GL_PT_Y(WrightSideNY),GET_GL_PT_Z(nextzpos+wall_height));
		 vmlTexCoord2f(1.0f,0.0f);
		 vmlVertex3d(GET_GL_PT_X(WrightSideX),GET_GL_PT_Y(WrightSideY),GET_GL_PT_Z(zpos+wall_height));
		 vmlEnd(file);
		 }

		 

		 if (t->isLeftKerb())
		 {
		   if (mytrack->showTextures){ 
			 //glEnable(GL_TEXTURE_2D);
			 //Kerb1Bits = TextureMap(GetDC(),Kerb1Bmp,Kerb1Bits,FALSE);
	       }
		 // red kerb
		 vmlColor3f(1.0f,0.0f,0.0f);
		 vmlBegin(GL_POLYGON);		 
		 vmlTexCoord2f(0.0f,0.0f);
		 vmlVertex3d(GET_GL_PT_X(leftSideX),GET_GL_PT_Y(leftSideY),GET_GL_PT_Z(zpos));		 
		 vmlTexCoord2f(0.0f,1.0f);
		 vmlVertex3d(GET_GL_PT_X(Kerb1leftSideX),GET_GL_PT_Y(Kerb1leftSideY),GET_GL_PT_Z(zpos-KERB_1_HEIGHT));		 
		 vmlTexCoord2f(1.0f,1.0f);
		 vmlVertex3d(GET_GL_PT_X(Kerb1leftSideNX),GET_GL_PT_Y(Kerb1leftSideNY),GET_GL_PT_Z(nextzpos-KERB_1_HEIGHT));		 
		 vmlTexCoord2f(1.0f,0.0f);
		 vmlVertex3d(GET_GL_PT_X(leftSideNX),GET_GL_PT_Y(leftSideNY),GET_GL_PT_Z(nextzpos));		 		 	 
		 vmlEnd(file);

		 // blue kerb
		 vmlColor3f(0.0f,0.0f,1.0f);
		 vmlBegin(GL_POLYGON);	
		 vmlTexCoord2f(0.0f,0.0f);
		 vmlVertex3d(GET_GL_PT_X(Kerb1leftSideX),GET_GL_PT_Y(Kerb1leftSideY),GET_GL_PT_Z(zpos-KERB_1_HEIGHT));		 
		 vmlTexCoord2f(0.0f,1.0f);
		 vmlVertex3d(GET_GL_PT_X(Kerb2leftSideX),GET_GL_PT_Y(Kerb2leftSideY),GET_GL_PT_Z(zpos-KERB_2_HEIGHT));		 
		 vmlTexCoord2f(1.0f,1.0f);
		 vmlVertex3d(GET_GL_PT_X(Kerb2leftSideNX),GET_GL_PT_Y(Kerb2leftSideNY),GET_GL_PT_Z(nextzpos-KERB_2_HEIGHT));		 
		 vmlTexCoord2f(1.0f,0.0f);
		 vmlVertex3d(GET_GL_PT_X(Kerb1leftSideNX),GET_GL_PT_Y(Kerb1leftSideNY),GET_GL_PT_Z(nextzpos-KERB_1_HEIGHT));		 		 	 		 
		 vmlEnd(file);


		 }

		 if (t->isRightKerb())
		 {
		   if (mytrack->showTextures){ 
			 //glEnable(GL_TEXTURE_2D);
			 //Kerb1Bits = TextureMap(GetDC(),Kerb1Bmp,Kerb1Bits,FALSE);
	       }
		 vmlColor3f(1.0f,0.0f,0.0f);
		 vmlBegin(GL_POLYGON);		 
		 vmlTexCoord2f(0.0f,0.0f);
		 vmlVertex3d(GET_GL_PT_X(rightSideX),GET_GL_PT_Y(rightSideY),GET_GL_PT_Z(zpos));		 
		 vmlTexCoord2f(0.0f,1.0f);
		 vmlVertex3d(GET_GL_PT_X(Kerb1rightSideX),GET_GL_PT_Y(Kerb1rightSideY),GET_GL_PT_Z(zpos-KERB_1_HEIGHT));		 
		 vmlTexCoord2f(1.0f,1.0f);
		 vmlVertex3d(GET_GL_PT_X(Kerb1rightSideNX),GET_GL_PT_Y(Kerb1rightSideNY),GET_GL_PT_Z(nextzpos-KERB_1_HEIGHT));		 
		 vmlTexCoord2f(1.0f,0.0f);
		 vmlVertex3d(GET_GL_PT_X(rightSideNX),GET_GL_PT_Y(rightSideNY),GET_GL_PT_Z(nextzpos));		 		 	 
		 vmlEnd(file);

		 vmlColor3f(0.0f,0.0f,1.0f);
		 vmlBegin(GL_POLYGON);		 
		 vmlTexCoord2f(0.0f,0.0f);
		 vmlVertex3d(GET_GL_PT_X(Kerb1rightSideX),GET_GL_PT_Y(Kerb1rightSideY),GET_GL_PT_Z(zpos-KERB_1_HEIGHT));		 
		 vmlTexCoord2f(0.0f,1.0f);
		 vmlVertex3d(GET_GL_PT_X(Kerb2rightSideX),GET_GL_PT_Y(Kerb2rightSideY),GET_GL_PT_Z(zpos-KERB_2_HEIGHT));		 
		 vmlTexCoord2f(1.0f,1.0f);
		 vmlVertex3d(GET_GL_PT_X(Kerb2rightSideNX),GET_GL_PT_Y(Kerb2rightSideNY),GET_GL_PT_Z(nextzpos-KERB_2_HEIGHT));		 
		 vmlTexCoord2f(1.0f,0.0f);
		 vmlVertex3d(GET_GL_PT_X(Kerb1rightSideNX),GET_GL_PT_Y(Kerb1rightSideNY),GET_GL_PT_Z(nextzpos-KERB_1_HEIGHT));		 		 	 
		 vmlEnd(file);

		 }

		 //glDisable(GL_TEXTURE_2D);


		 

		 

		 

		 // white kerb
		 vmlColor3f(1.0f,1.0f,1.0f);
		 vmlBegin(GL_POLYGON);		 
		 vmlVertex3d(GET_GL_PT_X(leftSideX),GET_GL_PT_Y(leftSideY),GET_GL_PT_Z(zpos-KDIFF));		 
		 vmlVertex3d(GET_GL_PT_X(leftSideNX),GET_GL_PT_Y(leftSideNY),GET_GL_PT_Z(nextzpos-KDIFF));		 		 	 
		 vmlVertex3d(GET_GL_PT_X(KleftSideNX),GET_GL_PT_Y(KleftSideNY),GET_GL_PT_Z(nextzpos-KDIFF));		 
		 vmlVertex3d(GET_GL_PT_X(KleftSideX),GET_GL_PT_Y(KleftSideY),GET_GL_PT_Z(zpos-KDIFF));		 			 
		 vmlEnd(file);

		 // white kerb
		 vmlBegin(GL_POLYGON);
		 vmlVertex3d(GET_GL_PT_X(rightSideX),GET_GL_PT_Y(rightSideY),GET_GL_PT_Z(zpos-KDIFF));
		 vmlVertex3d(GET_GL_PT_X(KrightSideX),GET_GL_PT_Y(KrightSideY),GET_GL_PT_Z(zpos-KDIFF));
		 vmlVertex3d(GET_GL_PT_X(KrightSideNX),GET_GL_PT_Y(KrightSideNY),GET_GL_PT_Z(nextzpos-KDIFF));
		 vmlVertex3d(GET_GL_PT_X(rightSideNX),GET_GL_PT_Y(rightSideNY),GET_GL_PT_Z(nextzpos-KDIFF));		 
		 vmlEnd(file);

		 

		 vmlColor3f(0.0f,0.0f,0.0f);

		 if (mytrack->showObjects)
		 {
		 Vector *TrackSideObjects = t->getObjects();
		 int numObjects = TrackSideObjects->size();
         for(int i=0;i<numObjects;i++)
         {
			 TrackObject *ob = (TrackObject*)TrackSideObjects->elementAt(i);
		     if (t!=NULL)
		     {
               double trackWidth = t->getWidthLeft();
			   double nextypos;
		       double nextxpos;
		       double remainder = ob->getDistance();
		       double length    = t->getLength();
			   double angle     = t->getAngle();
			   double stAngle   = sectorStartAngle;

		       if (angle != 0)
		       {
		         CPoint trackPt = GetTrackPoint(stAngle,angle,remainder);
                 nextypos = trackPt.x;
                 nextxpos = trackPt.y;
	             double deltaAngle = DegToRad(angle*length)*(remainder/length);
	             angle=stAngle+deltaAngle;
		       }
		       else
		       {
		         nextypos = ob->getDistance()*sin(stAngle);
                 nextxpos = ob->getDistance()*cos(stAngle);
		         angle = stAngle;
		        }
		 

				double xr = xpos+nextxpos;
				double yr = ypos+nextypos;
				double obposx;
				double obposy;
				double obposz;

				TrackObjectDefinition *obj = ob->getObjectDef();

		
				if (obj!=NULL)
				{
					double distance = obj->getDistance();
					obposz = zpos;
					double di = WIDTH_TO_TRACKWIDTH_OBJECTS(fabs(distance));
					if (distance > 0)
					{
					obposx = xr+getRightSideX(angle,di);
					obposy = yr+getRightSideY(angle,di);
					}
					else
					{
					obposx = xr+getLeftSideX(angle,di);
					obposy = yr+getLeftSideY(angle,di);						
					}
		 
					double glxpos =  GET_GL_PT_X(obposx);
					double glypos =  GET_GL_PT_Y(obposy);
					double glzpos =  GET_GL_PT_Z(obposz);

					//CDC *pDC = CDC::FromHandle(hdc);
					//obj->DrawGL(pDC,glxpos,glypos,glzpos,angle,mytrack->showAxis);
					//obj->DrawVML(file,glxpos,glypos,glzpos,angle,mytrack->showAxis);
				}
			}
		  }
		  delete TrackSideObjects;
		 }		 
		 //if (mytrack->showTrackSide) drawTextureSides(t,mytrack);
		 // next track section
		 if (t == tTo) break;
		 t = nextT;
		}
		//if (mytrack->showTrackSide)
		//{
		// for(int i=to;i<from;i++)
        // {
		//   TrackSection *t = (TrackSection*)mytrack->TrackSections->elementAt(i);		   		   
		//   
	    // }
		//}
        } // if show track

		//if (mytrack->showPitLane) OpenGLDrawPitlane(hdc);

		

		if (mytrack->showCameras)
		 {
		   for(int i=0;i<mytrack->TrackCameras->size();i++)
           {
			  Camera* cam = (Camera*)mytrack->TrackCameras->elementAt(i);
		      CPosAngle *CameraPos    = DistanceToPoint(mytrack,(cam->getDistance()-16384));
			  double distance = cam->getPerpendicularDistance();        
			  double di = WIDTH_TO_TRACKWIDTH_OBJECTS(fabs(distance));
			  double leftSideX,leftSideY;
			  double xr = CameraPos->x;
			  double yr = CameraPos->y;
		      if (distance > 0)
		      {
			   
		       leftSideX = xr+getRightSideX(CameraPos->angle,di+WIDTH_TO_TRACKWIDTH_OBJECTS(0));
               leftSideY = yr+getRightSideY(CameraPos->angle,di+WIDTH_TO_TRACKWIDTH_OBJECTS(0));
		      }
		      else
		      {			  
			   leftSideX = xr+getLeftSideX(CameraPos->angle,di+WIDTH_TO_TRACKWIDTH_OBJECTS(0));
               leftSideY = yr+getLeftSideY(CameraPos->angle,di+WIDTH_TO_TRACKWIDTH_OBJECTS(0));			
		      }

			  //drawPoint(GET_GL_PT_X(leftSideX),GET_GL_PT_Y(leftSideY),GET_GL_PT_Z(CameraPos->z),FALSE);
		   }
		 }

		//vmlEndList();


		file << "NavigationInfo { type \"EXAMINE\" }" << endl;
		file << "Viewpoint { description \"start\" position 0 0 1000 } " << endl;
		file.close();
		}









BEGIN_MESSAGE_MAP(GPTrack, CWnd)
	//{{AFX_MSG_MAP(GPTrack)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// GPTrack message handlers
