// TrChangeDlg.h : header file
//

#ifndef _INCLUDED_TRCHANGE_DLG_H_
#define _INCLUDED_TRCHANGE_DLG_H_

#include "GPTrack.h"
#include "TrackSection.h"

/////////////////////////////////////////////////////////////////////////////
// CTrChangeDlg dialog

class CTrChangeDlg : public CDialog
{
  // Construction
public:
  CTrChangeDlg(CWnd *pParent = NULL);// standard constructor

  void
    setSection(TrackSection *t);

  void
    ValToKerbs(int val);
  void
    ValToSign(int val);
  void
    ValToCombo(int val);

  void
    getSection(TrackSection *t);

  BOOL
    EditTrackSections(TrackSection *t);

  TrackSection *currentSection;

  // Dialog Data
  //{{AFX_DATA(CTrChangeDlg)
  enum {
    IDD = IDD_TR_DLG
  };
  CComboBox m_SignCombo;
  int m_feature1;
  int m_feature2;
  int m_left_bank;
  int m_length;
  int m_right_bank;
  double m_angle;
  double m_height;
  double m_startangle;
  double m_endangle;
  double m_arc;
  double m_gradient;
  double m_altitude;
  BOOL m_Sign1;
  BOOL m_Sign2;
  BOOL m_Sign3;
  BOOL m_Sign4;
  BOOL m_Sign5;
  BOOL m_Sign6;
  BOOL m_Sign7;
  BOOL m_Sign8;
  BOOL m_Kerb1;
  BOOL m_Kerb2;
  BOOL m_Kerb3;
  BOOL m_Kerb4;
  BOOL m_Kerb6;
  BOOL m_Kerb5;
  BOOL m_Kerb7;
  BOOL m_Kerb8;
  int m_Index;
  CString m_Annotation;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTrChangeDlg)
protected:
  virtual void
    DoDataExchange(CDataExchange *pDX);// DDX/DDV support
    //}}AFX_VIRTUAL

  // Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CTrChangeDlg)
  afx_msg void
    OnKerb();
  afx_msg void
    OnSign();
  afx_msg void
    OnChangeFeature1();
  afx_msg void
    OnChangeFeature2();
  afx_msg void
    OnFound();
  afx_msg void
    OnSigncodes();
  afx_msg void
    OnNext();
  afx_msg void
    OnPrev();
  virtual BOOL
    OnInitDialog();
  afx_msg void
    OnSelchangeSignCombo();
  afx_msg void
    OnHelp();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
#endif
