// ObjectEdit.h : header file
//
#ifndef _INCLUDE_OBJECT_EDIT_H_
#define _INCLUDE_OBJECT_EDIT_H_

/////////////////////////////////////////////////////////////////////////////
// CObjectEdit dialog

class CObjectEdit : public CDialog
{
  // Construction
public:
  GPTrack* track;

  CObjectEdit(GPTrack* mytrack, CWnd* pParent = NULL);// standard constructor

  int locatorStart;

  // Dialog Data
  //{{AFX_DATA(CObjectEdit)
  enum {
    IDD = IDD_OBJECT_EDIT
  };
  CSpinButtonCtrl m_Unk2Spin;
  CSpinButtonCtrl m_Unk1Spin;
  CSpinButtonCtrl m_HeightSpin;
  CSpinButtonCtrl m_DfcSpin;
  CSpinButtonCtrl m_AngleZSpin;
  CSpinButtonCtrl m_AngleXSpin;
  CSpinButtonCtrl m_Id2Spin;
  CComboBox m_IdCombo;
  CComboBox m_DetailCombo;
  CComboBox m_ObjectCombo;
  int m_Id2;
  int m_AngleX;
  int m_AngleY;
  int m_Detail;
  int m_Height;
  int m_Unk1;
  int m_Unk2;
  int m_Y;
  CString m_Name;
  CString m_InternalExternal;
  int m_InternalNumber;
  //}}AFX_DATA

  int m_Id1;

  void
    setSection(TrackObjectDefinition* t)
  {
    m_Id1 = t->getId1();

    m_Id2 = t->getId2();
    m_Height = t->getHeight();
    m_AngleX = t->getAngleX();
    m_AngleY = t->getAngleY();
    m_Detail = t->getDetail();
    m_Unk1 = t->getUnk1();
    m_Unk2 = t->getUnk2();
    m_Y = t->getDistance();
    m_Name = t->getName();

    locatorStart = t->getLocator();

    if (m_Id1 > 17) {
      m_InternalExternal = "Internal Object";
      m_InternalNumber = m_Id1 - 17;
    } else {
      m_InternalExternal = "External Object";
      m_InternalNumber = 0;
    }
  }

  void
    getSection(TrackObjectDefinition* t)
  {
    t->id = m_Id1;

    t->id2 = m_Id2;
    t->height = m_Height;
    t->angleX = m_AngleX;
    t->angleY = m_AngleY;
    t->detail = m_Detail;
    t->Unk1 = m_Unk1;
    t->Unk2 = m_Unk2;
    t->y = m_Y;
    t->locator = locatorStart;
  }

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CObjectEdit)
protected:
  virtual void
    DoDataExchange(CDataExchange* pDX);// DDX/DDV support
    //}}AFX_VIRTUAL

  // Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CObjectEdit)
  virtual BOOL
    OnInitDialog();
  virtual void
    OnOK();
  afx_msg void
    OnSelchangeDetailcombo();
  afx_msg void
    OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
  afx_msg void
    OnClickId2Spin(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void
    OnChangeId2();
  afx_msg void
    OnSelchangeId1();
  afx_msg void
    OnButton2();
  afx_msg void
    OnButton3();
  afx_msg void
    OnHelp();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

#endif
