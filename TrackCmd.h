

#ifndef _INCLUDED_TRACKCMD_H_
#define _INCLUDED_TRACKCMD_H_

#define GP_CMD_BYTE 1
#define GP_CMD_WORD 2
#define GP_CMD_DWORD 3

#define KERB_DEF_1 0xCA
#define KERB_DEF_2 0xCB

static char *ArgType[] = {{"Unk"},{"Byte"},{"Word"},{"DWord"}};

#include "DataValue.h"

class TrackArg: public CObject
{
    public:
	LPCSTR name;

	TrackArg(int val,int type,LPCSTR name):
	value(val),argtype(type),name(name)
	{
	}

	int getValue()
	{
		return value.GetValue();
	}

	void setValue(int val)
	{
		 value.SetValue(val);
	}

	int getType()
	{
		return argtype;
	}

	DataValue  value;
	int      argtype;
};


int getCmdIcon(int id);

class TrackCmd : public Observer, public CObject
{
    public:

    int cmd;
	int offset;
	//GPTrack *track;
	LPCSTR name;
	TrackSection *owner;

	TrackCmd(GPTrack *mytrack,TrackSection *t,int cmd,int offset,LPCSTR name,int arg1=-1,int arg2=-1,int arg3=-1,int arg4=-1,int arg5=-1,int arg6=-1,int arg7=-1,int arg8=-1,int arg9=-1,int arg10=-1):
	cmd(cmd),offset(offset),name(name),Observer(mytrack,t_Command,(Observer*)t),owner(t)
	{
		args = new Vector("Track Cmds",__LINE__);
		setTreeNode(NULL);

		if (arg1 == -1) return;

		// add arguments from constructor
		if (arg1 != -1) {TrackArg *arg = new TrackArg(arg1,GP_CMD_WORD,"Unk");args->addElement(arg); }
		if (arg2 != -1) {TrackArg *arg = new TrackArg(arg2,GP_CMD_WORD,"Unk");args->addElement(arg); }
		if (arg3 != -1) {TrackArg *arg = new TrackArg(arg3,GP_CMD_WORD,"Unk");args->addElement(arg); }
		if (arg4 != -1) {TrackArg *arg = new TrackArg(arg4,GP_CMD_WORD,"Unk");args->addElement(arg); }
		if (arg5 != -1) {TrackArg *arg = new TrackArg(arg5,GP_CMD_WORD,"Unk");args->addElement(arg); }
		if (arg6 != -1) {TrackArg *arg = new TrackArg(arg6,GP_CMD_WORD,"Unk");args->addElement(arg); }
		if (arg7 != -1) {TrackArg *arg = new TrackArg(arg7,GP_CMD_WORD,"Unk");args->addElement(arg); }
		if (arg8 != -1) {TrackArg *arg = new TrackArg(arg8,GP_CMD_WORD,"Unk");args->addElement(arg); }
		if (arg9 != -1) {TrackArg *arg = new TrackArg(arg9,GP_CMD_WORD,"Unk");args->addElement(arg); }
		if (arg10 != -1) {TrackArg *arg = new TrackArg(arg10,GP_CMD_WORD,"Unk");args->addElement(arg); }
		
		
	}

	TrackCmd(TrackCmd *cmd);	 

	Observer *Copy();

	int Open();
	int OpenTextureEditor();
	int OpenCmdEditor();
	int OpenSceneryEditor();
	int OpenSceneryOnOffEditor();
	int OpenKerbEditor();
	int OpenObjectEditor();



	BOOL isMultiVariable() { return TRUE;}

	void LoadPropertyTable(CTrackPropertySheet *table);
	
	virtual ~TrackCmd()
	{
		if (args) 
			delete args;
		args=NULL;
	}

	int write(GPTrack *track,int offset)
	{	
		int TrackArg1 = 0;
		int numArgs = args->size();
		if (numArgs>0)
		{
		TrackArg1 = getNthArg(1);
		}

		track->WritePCByte(offset,TrackArg1);offset+=1;
		track->WritePCByte(offset,cmd);offset+=1;

		for(int i=1;i<numArgs;i++)
		{
			TrackArg *targ = (TrackArg*)args->elementAt(i);

			int type  = targ->getType();
			//save some time
			//int value = getNthArg(i+1);
			int value = 0;
			if (targ)
				value =  targ->getValue();
			else
			{
				AfxMessageBox("getNthArg: No Cmd Arg",MB_OK);
				value = 0;
			}

			switch(type)
			{
			   case GP_CMD_BYTE:
				   track->WritePCByte(offset,value);offset+=1;
				   break;
			   case GP_CMD_WORD:
				   track->WritePCWord(offset,value);offset+=2;
				   break;
			   case GP_CMD_DWORD:
				   track->WritePCDWord(offset,value);offset+=4;
				   break;

			}
		}
		return offset;
	}
	
	
	void TrackCmd::ZeroCommandArgs()
	{
		for(int i=0;i<args->size();i++)
		{
			TrackArg *targ = (TrackArg*)args->elementAt(i);			
			targ->setValue(0);
		}
	}
	
	int TrackCmd::debug(FILE *fp,GPTrack *track,int offset)
	{
		int TrackArg1 = getNthArg(1);

		fprintf(fp,"0x%d Cmd:%d %s\n",offset,cmd,getCmdDescription());
		fprintf(fp,"\tArg(1)=%d\n",TrackArg1);
		
		for(int i=1;i<args->size();i++)
		{
			TrackArg *targ = (TrackArg*)args->elementAt(i);
			

			int type  = targ->getType();
			int value = getNthArg(i+1);

			fprintf(fp,"\tArg(%d)=%d %s %s\n",i+1,value,ArgType[type],getCmdNthArgDescription(i+1));

			switch(type)
			{
			   case GP_CMD_BYTE:
				   track->WritePCByte(offset,value);offset+=1;
				   break;
			   case GP_CMD_WORD:
				   track->WritePCWord(offset,value);offset+=2;
				   break;
			   case GP_CMD_DWORD:
				   track->WritePCDWord(offset,value);offset+=4;
				   break;

			}
		}
		return offset;
	}


	int getCmdIcon();
	CString getCmdDescription();
	CString getCmdNthArgDescription(int arg);

	int getCmd()
	{
		return cmd;
	}

	int getCmdNumArgs()
	{
		if (args)
		  return args->size();
		else
		{
		  AfxMessageBox("getCmdNumArgs size bads arguments");
		  return 0;
		}
	}

	void AddWordArgument(int arg,LPCSTR name)
	{
		TrackArg *targ = new TrackArg(arg,GP_CMD_WORD,name);
		args->addElement(targ);
	}


	virtual int getNthArg(int idx)
	{
		TrackArg *targ = (TrackArg*)args->elementAt(idx-1);
		if (targ)
		  return targ->getValue();
		else
		{
			AfxMessageBox("getNthArg: No Cmd Arg",MB_OK);
		  return 0;
		}
	}

	DataValue * getNthArgDataValue(int idx)
	{
	   TrackArg *targ = (TrackArg*)args->elementAt(idx-1);
	   if (targ)
		   return &(targ->value);
	   return NULL;
	}

	virtual void setNthArg(int idx,int val)
	{
		TrackArg *targ = (TrackArg*)args->elementAt(idx-1);
		if (targ)
		   targ->setValue(val);
		else
		{
		  AfxMessageBox("setNthArg No Cmd Arg",MB_OK); 
		}
	}


	int getNthArgType(int idx)
	{
		TrackArg *targ = (TrackArg*)args->elementAt(idx-1);
		if (targ)
		  return targ->getType();
		else
		{
		  AfxMessageBox("getNthArgType No Cmd Arg",MB_OK);
		  return 0;
		}
	}

	void Delete();


	
	



	Vector *args;
	//HTREEITEM treeNode;
};


#endif