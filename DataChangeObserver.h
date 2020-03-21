
#ifndef _INCLUDED_DATACHANGE_OBSERVER_H_
#define _INCLUDED_DATACHANGE_OBSERVER_H_

#include "Observer.h"
#include "DataValue.h"
#include "DataEditor.h"

typedef enum {
  t_INT,
  t_STRING,
  t_DOUBLE
} t_DataType;

class DataChangeObserver : public Observer
  , public CObject
{
public:
  DataChangeObserver(GPTrack *track, DataValue *ptr, t_DataType typ, LPSTR _name, LPSTR _descr)
    : ptr(ptr), Observer(track, t_DataChange, NULL), type(typ)
  {
    name = new CString(_name);
    descr = new CString(_descr);
  }

  virtual ~DataChangeObserver()
  {
    delete name;
    delete descr;
  }

  BOOL
    ChangeDataValue(LPCSTR text)
  {
    switch (type) {
    case t_INT: {
      int data = atoi(text);
      ptr->SetValue(data);
      if (getTrack()) {
        getTrack()->createTrack(getTrack()->GetDocument());
        getTrack()->createCC();
      }
      return TRUE;

    } break;
    default:
      AfxMessageBox("DataChangeObserver Data Type not supported");
      break;
    }

    return FALSE;
  }

  t_DataType type;
  DataValue *ptr;

  CString *
    GetName()
  {
    return name;
  }

  CString *
    GetDescription()
  {
    return descr;
  }

  virtual int
    Open()
  {
    CDataEditor *dlg = new CDataEditor();

    dlg->m_Desc = *descr;
    dlg->m_DataName = *name;
    dlg->m_NewValue = ptr->GetValue();

    int result = dlg->DoModal();
    if (result == IDOK) {
      ptr->SetValue(dlg->m_NewValue);
    }
    if (dlg) delete dlg;
    return 1;
  }

private:
  CString *name;
  CString *descr;
};

#endif