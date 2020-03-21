
#ifndef _INCLUDED_TOOL_H_
#define _INCLUDED_TOOL_H_
#include "stdafx.h"
//#include "TrackEditor.h"
#include "TrackEditorDoc.h"

#define TOOL_ZOOM_ID    500
#define TOOL_POINTER_ID 501
#define TOOL_TRACK_ID   502
#define TOOL_OBJECT_ID  503
#define TOOL_PITLANE_ID 504
#define TOOL_WALL_ID    505
#define TOOL_CC_ID      506
#define TOOL_RULER_ID   507
#define TOOL_LINE_ID    508
#define TOOL_PITLINE_ID 509
#define TOOL_HAND_ID    510
#define TOOL_CCLINE_ID  511
#define TOOL_CAMERA_ID  512

#ifndef MK_ALT 
#include "oleidl.h"
#endif

class Tool
{
   public:
	int ToolId;

	Tool(int id):
	ToolId(id)
	{
		m_dragging = FALSE;	
	}

	BOOL isA(int id)
	{
		if(ToolId == id) return TRUE;
		else return FALSE;
	}

	BOOL isTrackTool()  {return 	isA(TOOL_TRACK_ID);	}
	BOOL isPitTool()    {return 	isA(TOOL_PITLANE_ID);	}
	BOOL isObjectTool() {return isA(TOOL_OBJECT_ID);	}
	BOOL isCCTool()     {return 	isA(TOOL_CC_ID);	}
	BOOL isRulerTool()	{return 	isA(TOOL_RULER_ID);	}
	BOOL isPointerTool() {return    isA(TOOL_POINTER_ID);}
	BOOL isLineTool() {return    isA(TOOL_LINE_ID);}
	BOOL isPitLineTool() {return    isA(TOOL_PITLINE_ID);}
	BOOL isHandTool() {return    isA(TOOL_HAND_ID);}
	BOOL isCCLineTool()     {return 	isA(TOOL_CCLINE_ID);	}
	BOOL isCameraTool()     {return 	isA(TOOL_CAMERA_ID);	}

	

	virtual void OnLButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point)
	{}

	virtual void OnMouseMove(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point)
	{}

	virtual void OnLButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point)
	{}

	virtual void OnRButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point)
	{}

	virtual void OnRButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point)
	{}

	virtual void OnMButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point)
	{}

	virtual void OnMButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point)
	{}

	virtual void OnLButtonDblClk(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point) 
	{}

	virtual void OnKeyDown(CView *view,CTrackEditorDoc *pDoc,UINT nChar, UINT nRepCnt, UINT nFlags)
	{}

	virtual void  InsertObject(CView *view,CTrackEditorDoc *pDoc)
	{}

	virtual void  InsertSection(CView *view,CTrackEditorDoc *pDoc)
	{}
	
	virtual void  RemoveSection(CView *view,CTrackEditorDoc *pDoc)
	{}
	
	virtual void  RemoveObject(CView *view,CTrackEditorDoc *pDoc)
	{}

	BOOL m_dragging;
	CPoint m_dragStart;
};

class CameraTool: public Tool
{
    public:

		CameraTool():
		  Tool(TOOL_CAMERA_ID)
	{
			  m_dragging = FALSE;
		changingAngle = FALSE;
    }

	void OnLButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnMouseMove(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnLButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnRButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnRButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnLButtonDblClk(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	
	void InsertObject(CView *view,CTrackEditorDoc *pDoc);
	//void RemoveObject(CView *view,CTrackEditorDoc *pDoc);
	
	void OnKeyDown(CView *view,CTrackEditorDoc *pDoc,UINT nChar, UINT nRepCnt, UINT nFlags);
	
	BOOL changingAngle;
};



class ObjectTool: public Tool
{
    public:

		ObjectTool():
		  Tool(TOOL_OBJECT_ID)
	{
			  //m_dragging = FALSE;
		changingAngle = FALSE;
    }

	void OnLButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnMouseMove(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnLButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnRButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnRButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnLButtonDblClk(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);

	void OnKeyDown(CView *view,CTrackEditorDoc *pDoc,UINT nChar, UINT nRepCnt, UINT nFlags);
	
	void InsertObject(CView *view,CTrackEditorDoc *pDoc);
	void RemoveObject(CView *view,CTrackEditorDoc *pDoc);
	
	
	//BOOL dragging;
	BOOL changingAngle;

};

class WallTool: public Tool
{
    public:

		WallTool():
		  Tool(TOOL_WALL_ID)
	{
		m_dragging = FALSE;
		changingAngle = FALSE;
    }

	void OnLButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnMouseMove(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnLButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnRButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnRButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);


	void OnKeyDown(CView *view,CTrackEditorDoc *pDoc,UINT nChar, UINT nRepCnt, UINT nFlags);
	
	//BOOL dragging;
	BOOL changingAngle;

};

class PointerTool: public Tool
{
	public:

		PointerTool():
		  Tool(TOOL_POINTER_ID)
	{
			  //m_dragging = FALSE;
    }

    void OnMouseMove(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnLButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnLButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnLButtonDblClk(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnKeyDown(CView *view,CTrackEditorDoc *pDoc,UINT nChar, UINT nRepCnt, UINT nFlags);

	//BOOL dragging;
	CPoint BeginDragPoint;
	CPoint EndDragPoint;

	Vector *selections;
};

class TrackTool: public Tool
{
    public:

		TrackTool():
		  Tool(TOOL_TRACK_ID)
	{
		//dragging = FALSE;
		changingAngle = FALSE;

    }

	void OnLButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnMouseMove(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnLButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnRButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnRButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnLButtonDblClk(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnMButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnMButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);

	void OnKeyDown(CView *view,CTrackEditorDoc *pDoc,UINT nChar, UINT nRepCnt, UINT nFlags);
	
	void  InsertObject(CView *view,CTrackEditorDoc *pDoc);
	void  InsertSection(CView *view,CTrackEditorDoc *pDoc);
	void  RemoveSection(CView *view,CTrackEditorDoc *pDoc);

	//BOOL dragging;
	BOOL changingAngle;
};

class PitLaneTool: public Tool
{
    public:

		PitLaneTool():
		  Tool(TOOL_PITLANE_ID)
	{	  
	    //dragging = FALSE;
		changingAngle = FALSE;
	}

	void OnLButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnMouseMove(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnLButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnRButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnRButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnLButtonDblClk(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	
	void OnKeyDown(CView *view,CTrackEditorDoc *pDoc,UINT nChar, UINT nRepCnt, UINT nFlags);

	void  InsertSection(CView *view,CTrackEditorDoc *pDoc);
	void  RemoveSection(CView *view,CTrackEditorDoc *pDoc);
	void  InsertObject(CView *view,CTrackEditorDoc *pDoc);
	void  RemoveObject(CView *view,CTrackEditorDoc *pDoc);

	
	
	//BOOL dragging;
	BOOL changingAngle;
};




class ZoomTool: public Tool
{
    public:

	ZoomTool():
	 Tool(TOOL_ZOOM_ID)
	{}

	void OnLButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnRButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	
};

class CCTool: public Tool
{
    public:

		CCTool():
		  Tool(TOOL_CC_ID)
	{
		//dragging = FALSE;
		changingAngle = FALSE;

    }

	void OnLButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnMouseMove(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnLButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnRButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnRButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnLButtonDblClk(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);

	void OnKeyDown(CView *view,CTrackEditorDoc *pDoc,UINT nChar, UINT nRepCnt, UINT nFlags);

	void  InsertSection(CView *view,CTrackEditorDoc *pDoc);
	void  RemoveSection(CView *view,CTrackEditorDoc *pDoc);
	
	
	//BOOL dragging;
	BOOL changingAngle;
};

class RulerTool: public Tool
{
    public:

		RulerTool():
		  Tool(TOOL_RULER_ID)
	{
		//dragging = FALSE;
		changingAngle = FALSE;

    }

	void OnLButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnMouseMove(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnLButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);

	//BOOL dragging;
	BOOL changingAngle;
};

class LineTool: public Tool
{
    public:

	LineTool():
	Tool(TOOL_LINE_ID)
	{
		closestSector=NULL;
    }


	~LineTool()
	{
	
	}

	TrackSection *closestSector;

	void OnLButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnMouseMove(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnLButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
};

class PitLineTool: public Tool
{
    public:

	PitLineTool():
	Tool(TOOL_PITLINE_ID)
	{
		closestSector=NULL;
    }


	~PitLineTool()
	{
	
	}

	TrackSection *closestSector;

	void OnLButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnMouseMove(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnLButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
};

class HandTool: public Tool
{
    public:

	HandTool():
	Tool(TOOL_HAND_ID)
	{
		//dragg=FALSE;
    }


	~HandTool()
	{
	
	}

	void OnLButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnMouseMove(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnLButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);

	//BOOL dragg;
	CPoint BeginPoint;
};

class CCLineTool: public Tool
{
    public:

	CCLineTool():
	Tool(TOOL_CCLINE_ID)
	{
		closestSector=NULL;
    }


	~CCLineTool()
	{
	
	}

	CCLineSection *closestSector;

	void OnLButtonDown(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnMouseMove(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
	void OnLButtonUp(CView *view,CTrackEditorDoc *pDoc,UINT nFlags, CPoint point);
};



class LinePt: public CObject
{
    public:
	LinePt(int x,int y):
    x(x),y(y)
	{
	}

	virtual ~LinePt()
	{
	}

	int x;
	int y;
};


#endif