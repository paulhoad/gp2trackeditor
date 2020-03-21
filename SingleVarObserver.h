
#include "DataChangeObserver.h"

#include "PaletteChooser.h"
#include "VertexEditor.h"


class SingleVariableObserver: public DataChangeObserver
{
public:
	SingleVariableObserver(GPTrack *track,DataValue *val,LPSTR _name,LPSTR _descr):
		DataChangeObserver(track,val,t_INT,_name,_descr)
	{
	}

	virtual ~SingleVariableObserver()
	{
	}

	BOOL isSingleVariable() { return TRUE; }
};

class SingleVertexObserver: public DataChangeObserver
{
public:
	SingleVertexObserver(GPTrack *track,DataValue *val,LPSTR _name,LPSTR _descr):
		DataChangeObserver(track,val,t_INT,_name,_descr)
	{
	}

	virtual ~SingleVertexObserver()
	{
	}

	int Open()
	{
		CVertexEditor *dlg = new CVertexEditor();
		dlg->PtPt.SetValue(ptr->GetValue());
		int result = dlg->DoModal();
		if (result == IDOK)
		{
			ptr->SetValue(dlg->PtPt.GetValue());
		}
		if (dlg) delete dlg;
		return 0;
	}

	BOOL isSingleVariable() { return TRUE; }
};




class SingleColorObserver: public DataChangeObserver
{
public:
	SingleColorObserver(GPTrack *track,DataValue *val,LPSTR _name,LPSTR _descr):
		DataChangeObserver(track,val,t_INT,_name,_descr)
	{
	}

	virtual ~SingleColorObserver()
	{
	}

	int Open()
	{
		CPaletteChooser *dlg = new CPaletteChooser();
		dlg->setSelectedColor(ptr->GetValue());
		int result = dlg->DoModal();
		if (result == IDOK)
		{
			ptr->SetValue(dlg->getSelectedColor());
		}
		if (dlg) delete dlg;
		return 0;
	}

	BOOL isSingleVariable() { return TRUE; }
};