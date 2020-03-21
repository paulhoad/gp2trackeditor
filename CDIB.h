///////////////////////////////////////////////////////////
// CDIB.H: Header file for the DIB class.
///////////////////////////////////////////////////////////

#ifndef __CDIB_H
#define __CDIB_H

class CTextureMap : public CObject
{
protected:
  LPBITMAPFILEHEADER m_pBmFileHeader;
  LPBITMAPINFO m_pBmInfo;
  LPBITMAPINFOHEADER m_pBmInfoHeader;
  RGBQUAD* m_pRGBTable;
  BYTE* m_pDibBits;
  UINT m_numColors;

public:
  CTextureMap(char* fileName);
  ~CTextureMap();

  void
    CreateTextureMapBitmap(void* bits);

  DWORD
  GetDibSizeImage();
  UINT
    GetDibWidth();
  UINT
    GetDibHeight();
  UINT
    GetDibNumColors();
  LPBITMAPINFOHEADER
  GetDibInfoHeaderPtr();
  LPBITMAPINFO
  GetDibInfoPtr();
  LPRGBQUAD
  GetDibRGBTablePtr();
  BYTE*
    GetDibBitsPtr();

protected:
  void
    LoadBitmapFile(const char* fileName);
};

#endif
