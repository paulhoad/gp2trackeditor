// CLineEditor.h : header file
//
#ifndef _INCLUDE_CC_EDITOR_H_
#define _INCLUDE_CC_EDITOR_H_

#include "CCLineSection.h"

/////////////////////////////////////////////////////////////////////////////
// CCLineEditor dialog

class CCLineEditor : public CDialog
{
  // Construction
public:
  CCLineEditor(CWnd* pParent = NULL);// standard constructor

  BOOL radBoxhasFocus;
  BOOL radiusBoxhasFocus;

  void
    setSection(CCLineSection* t, int idx = -1)
  {
    m_ccrad = (int)t->getRadius();
    // m_ccang = t->getAngle();
    m_cclen = (int)t->getLength();
    m_ccarg1 = (int)t->getArg1();
    m_ccarg2 = (int)t->getArg2();
    m_ccarg3 = (int)t->getArg3();
    m_ccarg4 = (int)t->getArg4();
    m_cccmd = t->getCmd();
    m_ccidx = t->track->CCLineSections->findElementIndex(t);
    m_LeftCurve = ((m_ccarg2 < 0) ? TRUE : FALSE);
    currentSection = t;
  }

  void
    OnCurveCheck();

  void
    getSection(CCLineSection* t)
  {
    t->radius = m_ccrad;
    t->length = (double)m_cclen;
    t->setArg1(m_ccarg1);
    t->setArg2(m_ccarg2);
    t->setArg3(m_ccarg3);
    t->setArg4(m_ccarg4);
  }

  BOOL
    EditCCSections(CCLineSection* t);

  CCLineSection* currentSection;

  // Dialog Data
  //{{AFX_DATA(CCLineEditor)
  enum {
    IDD = CCLineChange
  };
  int m_ccrad;
  int m_cclen;
  int m_cccmd;
  int m_ccidx;
  int m_ccang;
  int m_ccarg1;
  int m_ccarg4;
  short m_ccarg2;
  double m_truerad;
  double m_trueang;
  BOOL m_LeftCurve;
  int m_RadiusCardinal;
  short m_ccarg3;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CCLineEditor)
protected:
  virtual void
    DoDataExchange(CDataExchange* pDX);// DDX/DDV support
  //}}AFX_VIRTUAL

  void
    OnMyChangeArgs(BOOL update = TRUE);
  void
    OnMyChangeRadiuscard(BOOL update = TRUE);

  // Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CCLineEditor)
  afx_msg void
    OnPrev();
  afx_msg void
    OnNext();
  virtual BOOL
    OnInitDialog();
  afx_msg void
    OnChangeArgs();
  afx_msg void
    OnClickSpinrad(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void
    OnChangeRadiuscard();
  afx_msg void
    OnRight();
  afx_msg void
    OnStraight();
  afx_msg void
    OnLeft();
  afx_msg void
    OnSetfocusCcrad();
  afx_msg void
    OnKillfocusCcrad();
  afx_msg void
    OnHelp();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

#endif
