// TableColumn.cpp : implementation file
//

#include "stdafx.h"
#include "TrackEditor.h"
#include "TableColumn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTableColumn

CTableColumn::CTableColumn(LPCSTR ctitle, int width)
  : title(title), width(width)
{
  title = (LPSTR)malloc((strlen(ctitle) + 1));
  strcpy(title, ctitle);
}

CTableColumn::~CTableColumn() { free(title); }

/////////////////////////////////////////////////////////////////////////////
// CTableColumn message handlers
