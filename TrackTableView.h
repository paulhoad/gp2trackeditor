// TrackTableView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrackTableView window

class CTrackTableView : public CListCtrl
{
  // Construction
  DECLARE_DYNCREATE(CTrackTableView)
public:
  CTrackTableView();

  // Attributes
public:
  // Operations
public:
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTrackTableView)
  //}}AFX_VIRTUAL

  // Implementation
public:
  virtual ~CTrackTableView();

  // Generated message map functions
protected:
  //{{AFX_MSG(CTrackTableView)
  // NOTE - the ClassWizard will add and remove member functions here.
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
