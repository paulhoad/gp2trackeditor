// TextureChooser.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextureChooser dialog
LPCSTR getTextureDescription(int id);
int getTextureIcon(int id);

class CTextureChooser : public CDialog
{
// Construction
public:
	CTextureChooser(GPTrack*track,CWnd* pParent = NULL);   // standard constructor

	GPTrack *track;

	int SelectedItem;
// Dialog Data
	//{{AFX_DATA(CTextureChooser)
	enum { IDD = IDD_TEXTURE_CHOOSER };
	CEdit	m_Resolution;
	CEdit	m_Repeats;
	CEdit	m_Length;
	CEdit	m_Offset;
	CComboBox	m_Rotation;
	CMyListCtrl	m_List;
	CComboBox	m_TextureCombo;
	CString	m_YOffset;
	//}}AFX_DATA

	int SelectedTexture;
	int SelectedLocation;
	int SelectedRotation;
	int SelectedLength;
	int SelectedOffset;
	int SelectedRepeats;
	int SelectedResolution;
	int SelectedYOffset;


	int getRotation(int Rotation)
	{
		switch(Rotation)
		{
		   /*
		   case 0: return 0;
		   case 1: return 16;
		   case 2: return 32;
		   case 3: return 48;
		   case 4: return 64;
		   case 5: return 80;
		   case 6: return 96;
		   case 7: return 112;
		   */
		   case 0: return 3;
		   case 1: return 19;
		   case 2: return 35;
		   case 3: return 51;
		   case 4: return 67;
		   case 5: return 83;
		   case 6: return 99;
		   case 7: return 115;
		   
		}
		return 0;
	}


	void setInfo(int offset,int loc,int length,int textureid,int rep,int res,int yoff,int rot)
	{
		SelectedOffset = offset;
		SelectedLength = length;

		/*
		if (rot >=0 && rot <=15) SelectedRotation =0;
		if (rot >15 && rot <=31) SelectedRotation =1;
		if (rot >31 && rot <=47) SelectedRotation =2;
		if (rot >47 && rot <=63) SelectedRotation =3;
		if (rot >63 && rot <=79) SelectedRotation =4;
		if (rot >79 && rot <=95) SelectedRotation =5;
		if (rot >95 && rot <=111) SelectedRotation =6;
		if (rot >111 && rot <=127) SelectedRotation =7;
		*/
		if (rot >=0 && rot <=18)SelectedRotation =0;
		if (rot >18 && rot <=34) SelectedRotation =1;
		if (rot >34 && rot <=50) SelectedRotation =2;
		if (rot >50 && rot <=66) SelectedRotation =3;
		if (rot >66 && rot <=82) SelectedRotation =4;
		if (rot >82 && rot <=98) SelectedRotation =5;
		if (rot >98 && rot <=114) SelectedRotation =6;
		if (rot >114 && rot <=130) SelectedRotation =7;
		

		SelectedLocation = loc;
		/*
		switch(loc)
		{ 
			case 4: SelectedLocation = 1; break;
			case 5: SelectedLocation = 0;break;			
			case 8: SelectedLocation = 2; break;
			case 9: SelectedLocation = 3; break;
			case 10: SelectedLocation = 4; break;
			case 11: SelectedLocation = 5; break;
			case 12: SelectedLocation = 6; break;
			case 13: SelectedLocation = 7; break;
			case 14: SelectedLocation = 8; break;
			case 15: SelectedLocation = 9; break;
			case 16: SelectedLocation = 10; break;
			case 17: SelectedLocation = 11; break;
			case 18: SelectedLocation = 12; break;
			case 19: SelectedLocation = 13; break;
		}
		*/
		//TRACE("setInfo=%d %d\n",loc,SelectedLocation);

		SelectedRepeats = rep;
		SelectedResolution = res;
		SelectedYOffset = yoff;
		SelectedTexture = textureid;
	}

	int getTextureId()
	{
		return 	SelectedTexture;
	}

	int getTextureLocationNumber(int loca)
	{
		TRACE("Location %d\n",loca);
		/*
		switch(loca)
		{
		   case 1: return 4;
		   case 0: return 5;
		   case 3: return 6;
		   case 2: return 7;
		   case 4: return 10;
		   case 5: return 11;
		   case 6: return 12;
		   case 7: return 13;
		   case 8: return 14;		 
		}
		return 1;
		*/
		return loca;
	}

	void setSelected(int row)
	{
	  m_List.SetItemState( row,LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED | LVIS_FOCUSED);
	}
    // Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextureChooser)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTextureChooser)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnKeydownList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
