#if !defined( \
  AFX_VERTEXEDITOR_H__30D2AE43_B7BB_11D3_A455_0050DA3CA74F__INCLUDED_)
#define AFX_VERTEXEDITOR_H__30D2AE43_B7BB_11D3_A455_0050DA3CA74F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif// _MSC_VER > 1000
// VertexEditor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVertexEditor dialog

class CVertexEditor : public CDialog
{
  // Construction
public:
  CVertexEditor(CWnd* pParent = NULL);// standard constructor

  // Dialog Data
  //{{AFX_DATA(CVertexEditor)
  enum {
    IDD = IDD_VERTEXVALUE
  };
  int m_From;
  int m_To;
  //}}AFX_DATA

  ObjectData PtPt;

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CVertexEditor)
protected:
  virtual void
    DoDataExchange(CDataExchange* pDX);// DDX/DDV support
    //}}AFX_VIRTUAL

  // Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CVertexEditor)
  virtual BOOL
    OnInitDialog();
  virtual void
    OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before
// the previous line.

#endif// !defined(AFX_VERTEXEDITOR_H__30D2AE43_B7BB_11D3_A455_0050DA3CA74F__INCLUDED_)
