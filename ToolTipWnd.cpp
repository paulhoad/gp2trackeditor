// ToolTipWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ToolTipWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolTipWnd

CToolTipWnd::CToolTipWnd()
{

	lpWndCls	= AfxRegisterWndClass(0);
	
	//Defaults
	m_bMouseIn	= FALSE;
	m_bStuck	= FALSE;
	
	m_iWidth = 100;
	m_iHeight = 60;

	m_clrBkColor = RGB(249,254,188); //light yellow
	m_clrFrameColor = RGB(0,0,255);  //blue
	m_clrTextColor = RGB(0,0,0);	 //black

	m_iFontHeight = 14;
	m_strFontName = "Arial";

	pCurrwnd = NULL;
}

CToolTipWnd::~CToolTipWnd()
{

	BTOOLINFO *stToolInfo;
	CWnd *pWnd;
	for(POSITION pos = m_ToolPtr.GetStartPosition(); pos != NULL;)
	{
		m_ToolPtr.GetNextAssoc(pos, (void *&)pWnd, (void*&) stToolInfo);
		delete stToolInfo;
	}
	m_ToolPtr.RemoveAll();

}


BEGIN_MESSAGE_MAP(CToolTipWnd, CWnd)
	//{{AFX_MSG_MAP(CToolTipWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CToolTipWnd message handlers
BOOL CToolTipWnd::Create(CWnd* pParentWnd) 
{

	BOOL bRet = CWnd::CreateEx(NULL, lpWndCls, NULL,
		WS_POPUP, 0, 0, m_iWidth, m_iHeight,
		pParentWnd->GetSafeHwnd(), NULL, NULL);

	m_hParentWnd = pParentWnd->GetSafeHwnd();
	
	if(bRet)
		SetOwner(pParentWnd);

	return bRet;

}

void CToolTipWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectCl;
	GetClientRect(&rectCl);

	CRgn rgnComb;	
	rgnComb.CreateRectRgn(rectCl.left+10,rectCl.top,rectCl.right,rectCl.bottom);

	int iRetComb = rgnComb.CombineRgn(&rgnTri, &rgn, RGN_OR);
	if(iRetComb==ERROR)
	{
		AfxMessageBox("ERROR in Combining Region");
		return;
	}

	CBrush pBrush;
	pBrush.CreateSolidBrush(m_clrFrameColor);
	
	CBrush pBrush1;
	pBrush1.CreateSolidBrush(m_clrBkColor);
	
	dc.FillRgn( &rgnComb, &pBrush1);
	dc.FrameRgn(&rgnComb, &pBrush, 2, 1);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(m_clrTextColor);
	
	CFont *pFont = dc.SelectObject(&m_fontText);
	//dc.Rectangle(&m_RectText);

	CSize czTextWidth = dc.GetTextExtent(m_strText);
	
	if( czTextWidth.cx < m_RectText.Width())
		dc.DrawText(m_strText, m_RectText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	else
		dc.DrawText(m_strText, m_RectText, DT_CENTER | DT_WORDBREAK);
	
	

	dc.SelectObject(pFont);
	
}


int CToolTipWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectCl;
	GetClientRect(&rectCl);

	int x=0, y=0;
	CRect rectTemp;

	rectTemp = rectCl;
	rectTemp.left = rectTemp.left + 10;
	
	x = (int)( (float)((float)rectTemp.Width() / 2.0) / 1.41421);
	y = (int)( (float)((float)rectTemp.Height() / 2.0) / 1.41421);

	m_RectText.top = ( (rectTemp.Height() / 2) - y);
	m_RectText.left = ( (rectTemp.Width() / 2) - x) + 10;
	m_RectText.right = ( (rectTemp.Width() / 2) + x) + 10;
	m_RectText.bottom = ( (rectTemp.Height() / 2) + y);

	rgn.m_hObject = NULL;
	rgnTri.m_hObject = NULL;
	rgnComb.m_hObject = NULL;

	BOOL bRegRet = rgn.CreateEllipticRgn(rectCl.left+10,rectCl.top,rectCl.right,rectCl.bottom);
		
	CPoint ptTri[3];
	ptTri[0].x = rectCl.left;
	ptTri[0].y = (rectCl.bottom / 2) - 10;

	ptTri[1].x = rectCl.left + 15;
	ptTri[1].y = (rectCl.bottom / 2) - 5;

	ptTri[2].x = rectCl.left + 15;
	ptTri[2].y = (rectCl.bottom / 2) + 5;
	
	ptTri[3].x = rectCl.left;
	ptTri[3].y = (rectCl.bottom / 2) - 10;

	BOOL bRegTriRet = rgnTri.CreatePolygonRgn(ptTri, 3, ALTERNATE);

	rgnComb.CreateRectRgn(rectCl.left+10,rectCl.top,rectCl.right,rectCl.bottom);
	int iRetComb = rgnComb.CombineRgn(&rgnTri, &rgn, RGN_OR);

	if(iRetComb == ERROR)
	{
		AfxMessageBox("ERROR in Combining Region");
		return -1;
	}

	int bRgnWnd = ::SetWindowRgn(m_hWnd,rgnComb.operator HRGN( ), TRUE);	

	m_fontText.CreateFont(m_iFontHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,m_strFontName);	
	
	return 0;
}

void CToolTipWnd::RelayEvent(LPMSG lpMsg)
{

	switch(lpMsg->message) 
	{
	case WM_KEYDOWN:
			if(IsWindowVisible())
			{		
				ShowWindow(SW_HIDE);
			}
			break;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
			if(IsWindowVisible())
			{
				ShowWindow(SW_HIDE);
			}
			break;

	case WM_MOUSEMOVE:
		{
			CWnd *pFocusWnd = AfxGetApp()->m_pMainWnd->GetFocus();
			if(pFocusWnd==NULL)
				break;
			CWnd* pWnd = CWnd::FromHandle(lpMsg->hwnd);

			HWND hWndTemp = ::GetParent(lpMsg->hwnd);
			
			CPoint pt;
			pt.x = lpMsg->pt.x;
			pt.y = lpMsg->pt.y;
			
			BTOOLINFO *stToolInfo;
			CWnd *pBToolWnd;
			
			for(POSITION pos = m_ToolPtr.GetStartPosition(); pos != NULL;)
			{
				m_ToolPtr.GetNextAssoc(pos, (void *&)pBToolWnd, (void*&) stToolInfo);
				
				if(!m_bMouseIn)
				{
					if(lpMsg->hwnd == stToolInfo->hwndTool)
					{
					
						if(m_bStuck && IsWindowVisible())
						{
							SetWindowPos(&wndTop,pt.x,pt.y,m_iWidth,m_iHeight,SWP_NOACTIVATE);
							ShowWindow(SW_SHOWNOACTIVATE);
						}

						m_bMouseIn = TRUE;
						m_clrTextColor = stToolInfo->clrToolTextClr; 
						m_strText = stToolInfo->strToolText; 
						
						SetWindowPos(&wndTop,pt.x,pt.y,m_iWidth,m_iHeight,SWP_NOACTIVATE);
						ShowWindow(SW_SHOWNOACTIVATE);
						
						pCurrwnd = stToolInfo->hwndTool;

						break;
					}
				}
				else
				{
					CRect rect;
					::GetWindowRect(pCurrwnd, &rect);
					if(m_bStuck && IsWindowVisible())
					{
						SetWindowPos(&wndTop,pt.x,pt.y,m_iWidth,m_iHeight,SWP_NOACTIVATE);
						ShowWindow(SW_SHOWNOACTIVATE);
					}
					
					
					CWnd* pWnd = CWnd::FromHandle(lpMsg->hwnd);
					CWnd *WndPt = pWnd->WindowFromPoint(lpMsg->pt);
					if(WndPt->GetSafeHwnd() != pCurrwnd)
					{
						m_bMouseIn = FALSE;
						ShowWindow(SW_HIDE);
					}
					

					break;
				}
			}
			
		}	
		break; //WM_MOUSEMOVE
	}
	 
}

void CToolTipWnd::AddTool(CWnd *pWnd, CString strText, COLORREF clrTextColor)
{

	BTOOLINFO *stToolInfo;

	if(!m_ToolPtr.Lookup( pWnd, ( void*& )  stToolInfo))
	{
		stToolInfo = new BTOOLINFO;
		stToolInfo->hwndTool = pWnd->GetSafeHwnd();
		stToolInfo->strToolText = strText;
		if(clrTextColor==NULL)
			stToolInfo->clrToolTextClr = m_clrTextColor;
		else
			stToolInfo->clrToolTextClr = clrTextColor;
	
		m_ToolPtr.SetAt(pWnd, stToolInfo);
	}

}
