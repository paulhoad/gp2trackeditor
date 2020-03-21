// TableRowElement.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEditor.h"
#include "TableRowElement.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTableRowElement

CTableRowElement::CTableRowElement(CString *name,int icon,WORD extraData):
name(name),icon(icon),extraData(extraData)
{
}

CTableRowElement::~CTableRowElement()
{
	if (name!=NULL) 
		delete name;
}


BEGIN_MESSAGE_MAP(CTableRowElement, CWnd)
	//{{AFX_MSG_MAP(CTableRowElement)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTableRowElement message handlers
