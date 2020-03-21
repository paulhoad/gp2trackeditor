// TableColumn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTableColumn window
#ifndef _INCLUDED_COLUMN_H_
#define _INCLUDED_COLUMN_H_

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CTableColumn : public CObject
{
  // Construction
public:
  CTableColumn(LPCSTR title, int width);

  ~CTableColumn();

  // Attributes
public:
  int width;
  LPSTR title;
  int icon;

  int getWidth()
  {
    return width;
  }

  LPCSTR
  getTitle()
  {
    return title;
  }

  // Operations
public:
  // Implementation
public:
};

/////////////////////////////////////////////////////////////////////////////

#endif