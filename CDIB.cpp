///////////////////////////////////////////////////////////
// CTextureMap.C: Implementation file for the DIB class.
///////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CDib.h"
#include "windowsx.h"

///////////////////////////////////////////////////////////
// CTextureMap::CTextureMap()
///////////////////////////////////////////////////////////
CTextureMap::CTextureMap(char* bits)
{
    // Load the bitmap and initialize
    // the class's data members.
    //LoadBitmapFile(fileName);
	CreateTextureMapBitmap(bits);
}



///////////////////////////////////////////////////////////
// CTextureMap::~CTextureMap()
///////////////////////////////////////////////////////////
CTextureMap::~CTextureMap()
{
    // Free the memory assigned to the bitmap.
    GlobalFreePtr(m_pBmInfo);
}

///////////////////////////////////////////////////////////
// CTextureMap::LoadBitmapFile()
//
// This function loads a DIB from disk into memory. It
// also initializes the various class data members.
///////////////////////////////////////////////////////////
void CTextureMap::LoadBitmapFile
    (const char* fileName)
{
    // Construct and open a file object.
    CFile file(fileName, CFile::modeRead);
    
    // Read the bitmap's file header into memory.
    BITMAPFILEHEADER bmFileHeader;
    file.Read((void*)&bmFileHeader, sizeof(bmFileHeader));

    // Check whether the file is really a bitmap.
    if (bmFileHeader.bfType != 0x4d42)
    {
        AfxMessageBox("Not a bitmap file");
        m_pBmFileHeader = 0;
        m_pBmInfo = 0;
        m_pBmInfoHeader = 0;
        m_pRGBTable = 0;
        m_pDibBits = 0;
        m_numColors = 0;
    }    
    // If the file checks out okay, continue loading.
    else
    {
        // Calculate the size of the DIB, which is the
        // file size minus the size of the file header.
        DWORD fileLength = file.GetLength();    
        DWORD dibSize = fileLength - sizeof(bmFileHeader);

        // Allocate enough memory to fit the bitmap.
        BYTE* pDib =
            (BYTE*)GlobalAllocPtr(GMEM_MOVEABLE, dibSize);
        
        // Read the bitmap into memory and close the file.
        file.Read((void*)pDib, dibSize);
        file.Close();
		CreateTextureMapBitmap(pDib);
	}

}

void CTextureMap::CreateTextureMapBitmap(void *bits)
{
	    m_pBmFileHeader = 0;
        m_pBmInfo = 0;
        m_pBmInfoHeader = 0;
        m_pRGBTable = 0;
        m_pDibBits = 0;
        m_numColors = 0;

	    BYTE *pDib = (BYTE*)bits;
        // Initialize pointers to the bitmap's BITMAPINFO
        // and BITMAPINFOHEADER structures.
        m_pBmInfo = (LPBITMAPINFO) pDib;
        m_pBmInfoHeader = (LPBITMAPINFOHEADER) pDib;
        
        // Calculate a pointer to the bitmap's color table.
        m_pRGBTable =
            (RGBQUAD*)(pDib + m_pBmInfoHeader->biSize);

        // Get the number of colors in the bitmap.
        int m_numColors = GetDibNumColors();

        // Calculate the bitmap image's size.
        m_pBmInfoHeader->biSizeImage =
            GetDibSizeImage();
        
        // Make sure the biClrUsed field
        // is initialized properly.
        if (m_pBmInfoHeader->biClrUsed == 0)
            m_pBmInfoHeader->biClrUsed = m_numColors;

        // Calculate a pointer to the bitmap's actual data.
        DWORD clrTableSize = m_numColors * sizeof(RGBQUAD);
        m_pDibBits =
            pDib + m_pBmInfoHeader->biSize + clrTableSize;
}

///////////////////////////////////////////////////////////
// CTextureMap::GetDibSizeImage()
//
// This function calculates and returns the size of the
// bitmap's image in bytes.
///////////////////////////////////////////////////////////
DWORD CTextureMap::GetDibSizeImage()
{
    // If the bitmap's biSizeImage field contains
    // invalid information, calculate the correct size.
    if (m_pBmInfoHeader->biSizeImage == 0)
    {
        // Get the width in bytes of a single row.
        DWORD byteWidth = (DWORD) GetDibWidth();
        
        // Get the height of the bitmap.
        DWORD height = (DWORD) GetDibHeight();
        
        // Multiply the byte width by the number of rows.
        DWORD imageSize = byteWidth * height;
        
        return imageSize;
    }
    // Otherwise, just return the size stored in
    // the BITMAPINFOHEADER structure.
    else
        return m_pBmInfoHeader->biSizeImage;
}

///////////////////////////////////////////////////////////
// CTextureMap::GetDibWidth()
//
// This function returns the width in bytes of a single
// row in the bitmap.
///////////////////////////////////////////////////////////
UINT CTextureMap::GetDibWidth()
{
    return (UINT) m_pBmInfoHeader->biWidth;
}
        
///////////////////////////////////////////////////////////
// CTextureMap::GetDibHeight()
//
// This function returns the bitmap's height in pixels.
///////////////////////////////////////////////////////////
UINT CTextureMap::GetDibHeight()
{
    return (UINT) m_pBmInfoHeader->biHeight;
}
        
///////////////////////////////////////////////////////////
// CTextureMap::GetDibNumColors()
//
// This function returns the number of colors in the
// bitmap.
///////////////////////////////////////////////////////////
UINT CTextureMap::GetDibNumColors()
{
    if ((m_pBmInfoHeader->biClrUsed == 0) &&
          (m_pBmInfoHeader->biBitCount < 9))
        return (1 << m_pBmInfoHeader->biBitCount);
    else
        return (int) m_pBmInfoHeader->biClrUsed;
}
    
///////////////////////////////////////////////////////////
// CTextureMap::GetDibInfoHeaderPtr()
//
// This function returns a pointer to the bitmap's
// BITMAPINFOHEADER structure.
///////////////////////////////////////////////////////////
LPBITMAPINFOHEADER CTextureMap::GetDibInfoHeaderPtr()
{
    return m_pBmInfoHeader;
}

///////////////////////////////////////////////////////////
// CTextureMap::GetDibInfoPtr()
//
// This function returns a pointer to the bitmap's
// BITMAPINFO structure.
///////////////////////////////////////////////////////////
LPBITMAPINFO CTextureMap::GetDibInfoPtr()
{
    return m_pBmInfo;
}

///////////////////////////////////////////////////////////
// CTextureMap::GetDibRGBTablePtr()
//
// This function returns a pointer to the bitmap's
// color table.
///////////////////////////////////////////////////////////
LPRGBQUAD CTextureMap::GetDibRGBTablePtr()
{
    return m_pRGBTable;
}

///////////////////////////////////////////////////////////
// CTextureMap::GetDibBitsPtr()
//
// This function returns a pointer to the bitmap's
// actual image data.
///////////////////////////////////////////////////////////
BYTE* CTextureMap::GetDibBitsPtr()
{
    return m_pDibBits;
}
