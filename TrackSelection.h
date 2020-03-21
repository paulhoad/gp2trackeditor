// TrackSelection.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrackSelection dialog

class CTrackSelection : public CDialog
{
  // Construction
public:
  CTrackSelection(CWnd* pParent = NULL);// standard constructor

  // Dialog Data
  //{{AFX_DATA(CTrackSelection)
  enum {
    IDD = IDD_TRACK_SELECTION
  };
  CListBox m_AllTracks;
  CListBox m_SelectedTracks;
  CString m_CurrentDir;
  //}}AFX_DATA

  void
    UpdateLists();
  void
    UpdateAllLists();

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTrackSelection)
protected:
  virtual void
    DoDataExchange(CDataExchange* pDX);// DDX/DDV support
    //}}AFX_VIRTUAL

  // Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CTrackSelection)
  afx_msg void
    OnTrackAdd();
  afx_msg void
    OnTrackRemove();
  afx_msg void
    OnLocate();
  virtual BOOL
    OnInitDialog();
  afx_msg void
    OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void
    OnDblclkTrackAllList();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
