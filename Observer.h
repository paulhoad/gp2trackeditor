
#ifndef _INCLUDED_OBSERVER_H_
#define _INCLUDED_OBSERVER_H_



class  CTrackPropertySheet;
class  GPTrack;
class  CTrackTree;

typedef enum ObserverType { t_Unkown, t_Camera, t_Command, t_Object	,t_TrackSection, t_PitSection, t_ObjectDefinition,t_CCSetup,t_InternalObject,t_DataChange,t_TrackWidth,t_JamFile,t_CCSection, t_ObjectData};

class Observer
{
	ObserverType obtype;
	

    public:
		Observer     *parent;

		Observer(GPTrack *gptrack,ObserverType type, Observer *parent,HTREEITEM node=NULL):
		treeNode(node),track(gptrack),obtype(type),parent(parent)
	    {
			parent = NULL;
		}

		Observer(Observer *ob):
		treeNode(NULL),track(ob->track),obtype(ob->obtype)
		{
			parent = ob->parent;
		}

		virtual ~Observer()
		{
		}

	GPTrack *track;

	GPTrack * getTrack()
	{
		return track;
	}

	BOOL isUnknown() {return ((obtype == t_Unkown) ? TRUE : FALSE);}
	BOOL isCamera() {return ((obtype == t_Camera) ? TRUE : FALSE);}
	BOOL isCommand() {return ((obtype == t_Command) ? TRUE : FALSE);}
	BOOL isObject() {return ((obtype == t_Object) ? TRUE : FALSE);}
	BOOL isTrackSection() {return ((obtype == t_TrackSection) ? TRUE : FALSE);}
	BOOL isPitSection() {return ((obtype == t_PitSection) ? TRUE : FALSE);}
	BOOL isJamFile() {return ((obtype == t_JamFile) ? TRUE : FALSE);}
	BOOL isInternalObject() {return ((obtype == t_InternalObject) ? TRUE : FALSE);}
	


    virtual BOOL isSingleVariable() { return FALSE; }
	virtual BOOL isMultiVariable() { return FALSE;}

	virtual void LoadPropertyTable(CTrackPropertySheet *table)
	{
	}

	virtual int Open()
	{
		//AfxMessageBox("No Observer for this Tree Item",MB_OK);
		return 0;
	}

	virtual void Expand(CTrackTree *tracktree,HTREEITEM node)
	{
	}

	virtual void Selected()
	{
	}

	virtual BOOL ChangeDataValue(LPCSTR dataValue)
	{
		AfxMessageBox("Cannot Change this data value");
		return FALSE;
	}

	
	virtual void Delete()
	{
		AfxMessageBox("Cannot call Delete on this Object",MB_OK);
	}

	virtual void InsertNew()
	{
		AfxMessageBox("Cannot call Insert on this Object",MB_OK);
	}

	virtual void InsertNew(Observer *obj)
	{
		AfxMessageBox("Cannot call Insert on this Object",MB_OK);
	}

	virtual Observer *Copy()
	{
		return NULL;
	}

	virtual LPCSTR toString()
	{
		if (isCommand()) return "Command";
		if (isObject()) return "Object";
		if (isTrackSection()) return "Track Section";
		if (isPitSection()) return "Pit Section";
		return "Unknown Observer Type";
	}

	

	HTREEITEM getTreeNode()
	{
		return treeNode;
	}

	void setTreeNode(HTREEITEM node)
	{
		treeNode = node;
	}

private:
	HTREEITEM treeNode;

};

#endif