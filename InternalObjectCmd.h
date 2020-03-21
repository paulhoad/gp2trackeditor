
#include "TrackCmd.h"

#define INTERAL_CMD_TEXTURE_03 1
#define INTERAL_CMD_TEXTURE_83 2

class InternalObjectCmd: public Observer, public CObject
{
    public:
	  int cmdType;

	  InternalObjectCmd(GPTrack *mytrack,int type,int num):
	  Observer(mytrack,t_Command,NULL),
	  cmdType(type)
	  {
		  args = new Vector("Object Cmds",__LINE__);

		  for(int i=0;i<num;i++)
		  {
		  TrackArg *arg = new TrackArg(0,GP_CMD_WORD,"Unk");
		  args->addElement(arg);
		  }
	  }

	  int Open();

	  inline int getCmd()
	  {
		return cmdType;
	  }

	  inline int getCmdNumArgs()
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


	  int getNthArg(int idx)
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

	  void setNthArg(int idx,int val)
	  {
		TrackArg *targ = (TrackArg*)args->elementAt(idx-1);
		if (targ)
		   targ->setValue(val);
		else
		{
		  AfxMessageBox("setNthArg No Cmd Arg",MB_OK); 
		}
	  }

	  void LoadPropertyTable(CTrackPropertySheet *table);

	  BOOL isMultiVariable() { return TRUE;}


	  Vector *args;
};