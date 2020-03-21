/*---------------------------------------------------------------------------------
        dibkernel.cpp, last modified 25th of November 1997
        René Dencker Eriksen (http://www.imada.ou.dk/~edr) and
        Hans-Martin Brændmose Jensen (http://www.mip.ou.dk/~hansm)
---------------------------------------------------------------------------------*/

#include "stdafx.h"//standard precompiled headers
#include "dibkernel.h"//dibkernel header

// default RGB color table for 8 bit color depth
const BYTE def256pal_r[256] = {
  0,
  128,
  0,
  128,
  0,
  128,
  0,
  192,
  192,
  166,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  192,
  224,
  0,
  32,
  64,
  96,
  128,
  160,
  255,
  160,
  128,
  255,
  0,
  255,
  0,
  255,
  0,
  255
};
const BYTE def256pal_g[256] = {
  0,
  0,
  128,
  128,
  0,
  0,
  128,
  192,
  220,
  202,
  32,
  32,
  32,
  32,
  32,
  32,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  96,
  96,
  96,
  96,
  96,
  96,
  96,
  96,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  160,
  160,
  160,
  160,
  160,
  160,
  160,
  160,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  224,
  224,
  224,
  224,
  224,
  224,
  224,
  224,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  32,
  32,
  32,
  32,
  32,
  32,
  32,
  32,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  96,
  96,
  96,
  96,
  96,
  96,
  96,
  96,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  160,
  160,
  160,
  160,
  160,
  160,
  160,
  160,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  224,
  224,
  224,
  224,
  224,
  224,
  224,
  224,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  32,
  32,
  32,
  32,
  32,
  32,
  32,
  32,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  96,
  96,
  96,
  96,
  96,
  96,
  96,
  96,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  160,
  160,
  160,
  160,
  160,
  160,
  160,
  160,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  224,
  224,
  224,
  224,
  224,
  224,
  224,
  224,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  32,
  32,
  32,
  32,
  32,
  32,
  32,
  32,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  96,
  96,
  96,
  96,
  96,
  96,
  96,
  96,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  160,
  160,
  160,
  160,
  160,
  160,
  160,
  160,
  192,
  192,
  192,
  192,
  192,
  192,
  251,
  160,
  128,
  0,
  255,
  255,
  0,
  0,
  255,
  255
};
const BYTE def256pal_b[256] = {
  0,
  0,
  0,
  0,
  128,
  128,
  128,
  192,
  192,
  240,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  64,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  128,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  192,
  240,
  164,
  128,
  0,
  0,
  0,
  255,
  255,
  255,
  255
};

IMPLEMENT_SERIAL(CDib, CObject, 0);

CPoint
  CFloatPoint::Round()
{
  return CPoint(round2(x), round(y));
}

CPoint
  CFloatPoint::Int()
{
  return CPoint((int)x, (int)y);
}

int round2(const double f)
{
  if (f < 0) {
    return (int)(f - 0.5);
  } else {
    return (int)(f + 0.5);
  }
}

CDib::CDib()// default constructor
{
  m_pntOrigo = CPoint(0, 0);// default origo
  m_lpBMIH = NULL;
  m_lpImage = NULL;
  m_hPalette = NULL;
  m_pImageMatrix = NULL;
  Empty();
}

CDib::CDib(CDib& dib)// copy constructor
{
  //	m_pntOrigo = CPoint(0,0);	//default origo
  m_lpBMIH = NULL;
  m_lpImage = NULL;
  m_hPalette = NULL;
  m_pImageMatrix = NULL;
  Empty();
  *this = dib;
}

CDib::CDib(CSize size, int nBitCount, BOOL bInitialize)
{
  m_pntOrigo = CPoint(0, 0);// default origo
  m_hPalette = NULL;
  m_lpBMIH = NULL;
  m_lpImage = NULL;
  m_pImageMatrix = NULL;
  Empty();
  ComputePaletteSize(nBitCount);
  m_lpBMIH =
    (LPBITMAPINFOHEADER) new char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries];
  m_lpBMIH->biSize = sizeof(BITMAPINFOHEADER);
  m_lpBMIH->biWidth = size.cx;
  m_lpBMIH->biHeight = size.cy;
  m_lpBMIH->biPlanes = 1;
  m_lpBMIH->biBitCount = (unsigned short)nBitCount;
  m_lpBMIH->biCompression = BI_RGB;
  m_lpBMIH->biSizeImage = 0;
  m_lpBMIH->biXPelsPerMeter = 0;
  m_lpBMIH->biYPelsPerMeter = 0;
  m_lpBMIH->biClrUsed = m_nColorTableEntries;
  m_lpBMIH->biClrImportant = m_nColorTableEntries;
  ComputeMetrics();
  m_lpImage = (LPBYTE) new char[m_dwSizeImage];
  if (bInitialize) {
    // make default palette from def256pal_x constants
    switch (nBitCount) {
    case 1:
      SetPaletteEntry(0, 0);// black pal. entry
      SetPaletteEntry(1, RGB(255, 255, 255));// white pal. entry
      break;
    case 8:
      for (int i = 0; i <= 255; i++) {
        SetPaletteEntry(i,
          RGB(def256pal_r[i], def256pal_g[i], def256pal_b[i]));
      }
      break;
    case 24:
      break;
    }

    memset(m_lpImage, 0, m_dwSizeImage);// initialize image bits to zero
  }
  MakePalette();
}

CDib::CDib(const CDib& dib, CRect rect)
{// copy area of interest from dib

  int headerSize =
    sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * dib.m_nColorTableEntries;
  // copy header
  m_lpBMIH = (LPBITMAPINFOHEADER) new char[headerSize];
  memcpy(m_lpBMIH, dib.m_lpBMIH, headerSize);
  // correct size
  m_lpBMIH->biWidth = rect.Width();
  m_lpBMIH->biHeight = rect.Height();
  m_lpBMIH->biSizeImage = 0;
  // compute member variables
  ComputeMetrics();
  ComputePaletteSize(dib.m_lpBMIH->biBitCount);
  MakePalette();
  m_pntOrigo = CPoint(0, 0);// default origo
  // copy bits
  m_lpImage = (LPBYTE) new char[m_dwSizeImage];
  for (int y = rect.top; y <= rect.bottom; y++) {
    for (int x = rect.left; x <= rect.right; x++) {
      SetPixel(CPoint(x - rect.left, y - rect.top), dib.GetPixel(CPoint(x, y)));
    }
  }
  m_pImageMatrix = NULL;
}

CDib& CDib::operator=(const CDib& dib)
{
  ASSERT(this != &dib);// beware of dib=dib

  if (dib.m_lpBMIH) {
    Empty();// clear left operand (dib we are coping to)

    int headerSize =
      sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * dib.m_nColorTableEntries;
    // copy header
    m_lpBMIH = (LPBITMAPINFOHEADER) new char[headerSize];
    memcpy(m_lpBMIH, dib.m_lpBMIH, headerSize);
    // copy member variables
    ComputeMetrics();
    ComputePaletteSize(dib.m_lpBMIH->biBitCount);
    MakePalette();
    m_pntOrigo = dib.m_pntOrigo;
    // copy bits
    m_lpImage = (LPBYTE) new char[dib.m_dwSizeImage];
    memcpy(m_lpImage, dib.m_lpImage, dib.m_dwSizeImage);
    m_pImageMatrix = NULL;
  }
  return *this;
}

CDib::~CDib()
{
  Empty();// clean up memory
}

CSize CDib::GetDimensions() const
{
  if (m_lpBMIH == NULL) return CSize(0, 0);
  return CSize((int)m_lpBMIH->biWidth, (int)m_lpBMIH->biHeight);
}

BOOL CDib::Draw(CDC* pDC, CRect rcDest, CPoint pntSrc)
/*-----------------------------------------------------------------------------
pDC:	Device context pointer to do output to.
rcDest:	Rectangle on DC to do output to.
pntSrc:	Coordinate of the lower-left corner of the DIB to output into rcDest.
-----------------------------------------------------------------------------*/
{
  /* Check for valid DIB handle */
  if ((m_lpBMIH == NULL) /*|| (m_hPalette == NULL)*/) return FALSE;

  BOOL bSuccess = FALSE;// Success/fail flag
  HPALETTE hOldPal = NULL;// Previous palette (our DIB's palette is
    // m_hPalette)
  CSize dibsize = GetDimensions();// Get DIB's dimensions

  // Select as background since we have already realized in forground if needed
  hOldPal = ::SelectPalette(pDC->GetSafeHdc(), m_hPalette, TRUE);
  if (pDC->IsPrinting())// printer DC
  {
    // get size of printer page (in pixels)
    int cxPage = pDC->GetDeviceCaps(HORZRES);
    int cyPage = pDC->GetDeviceCaps(VERTRES);
    // get printer pixels per inch
    int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
    int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);

    // Best Fit case -- create a rectangle which preserves
    // the DIB's aspect ratio, and fills the page horizontally.
    //
    // The formula in the "->bottom" field below calculates the Y
    // position of the printed bitmap, based on the size of the
    // bitmap, the width of the page, and the relative size of
    // a printed pixel (cyInch / cxInch).
    rcDest.top = rcDest.left = 0;
    rcDest.bottom = (int)(((double)dibsize.cy * cxPage * cyInch) / ((double)dibsize.cx * cxInch));
    rcDest.right = cxPage;
  }
  /* Make sure to use the stretching mode best for color pictures */
  ::SetStretchBltMode(pDC->GetSafeHdc(), COLORONCOLOR);
  /* Determine whether to call StretchDIBits or SetDIBitsToDevice */
  if (dibsize == rcDest.Size()) {
    bSuccess = ::SetDIBitsToDevice(
      pDC->GetSafeHdc(),// handle of device context
      rcDest.left,// x-coordinate of upper-left corner of dest. rect.
      rcDest.top,// y-coordinate of upper-left corner of dest. rect.
      rcDest.Width(),// source rectangle width
      rcDest.Height(),// source rectangle height
      pntSrc.x,// x-coordinate of lower-left corner of source rect.
      pntSrc.y,// y-coordinate of lower-left corner of source rect.
      0,// first scan line in array
      rcDest.Height(),// number of scan lines
      m_lpImage,// address of array with DIB bits
      (LPBITMAPINFO)m_lpBMIH,// address of structure with bitmap info.
      DIB_RGB_COLORS);// RGB or palette indices
  } else {
    bSuccess = ::StretchDIBits(
      pDC->GetSafeHdc(),// handle of device context
      rcDest.left,// x-coordinate of upper-left corner of dest. rect.
      rcDest.top,// y-coordinate of upper-left corner of dest. rect.
      rcDest.Width(),// width of destination rectangle
      rcDest.Height(),// height of destination rectangle
      pntSrc.x,// x-coordinate of upper-left corner of source rect.
      pntSrc.y,// y-coordinate of upper-left corner of source rect.
      m_lpBMIH->biWidth,// width of source rectangle
      m_lpBMIH->biHeight,// height of source rectangle
      m_lpImage,// address of bitmap bits
      (LPBITMAPINFO)m_lpBMIH,// address of bitmap data
      DIB_RGB_COLORS,// usage
      SRCCOPY);// raster operation code
  }
  /* Reselect old palette */
  if (hOldPal != NULL) {
    ::SelectPalette(pDC->GetSafeHdc(), hOldPal, TRUE);
  }
  return bSuccess;
}

BOOL CDib::MakePalette()
{// makes a logical palette (m_hPalette) from the DIB's color table
  // this palette will be selected and realized prior to drawing the DIB

  if (m_nColorTableEntries == 0) return FALSE;
  if (m_hPalette != NULL) ::DeleteObject(m_hPalette);
  LPLOGPALETTE pLogPal =
    (LPLOGPALETTE) new char[2 * sizeof(WORD) + m_nColorTableEntries * sizeof(PALETTEENTRY)];
  pLogPal->palVersion = 0x300;
  pLogPal->palNumEntries = m_nColorTableEntries;
  LPRGBQUAD pDibQuad = (LPRGBQUAD)m_lpvColorTable;
  for (int i = 0; i < m_nColorTableEntries; i++) {
    pLogPal->palPalEntry[i].peRed = pDibQuad->rgbRed;
    pLogPal->palPalEntry[i].peGreen = pDibQuad->rgbGreen;
    pLogPal->palPalEntry[i].peBlue = pDibQuad->rgbBlue;
    pLogPal->palPalEntry[i].peFlags = 0;
    pDibQuad++;
  }
  m_hPalette = ::CreatePalette(pLogPal);
  delete pLogPal;
  return TRUE;
}

BOOL CDib::Read(
  CFile* pFile)// CFile ptr passed by the documents serialize function
{
  // 1. read file header to get size of info hdr + color table
  // 2. read info hdr (to get image size) and color table
  // 3. read image

  Empty();
  int nCount, nSize;
  BITMAPFILEHEADER bmfh;
  TRY
  {
    nCount = pFile->Read((LPVOID)&bmfh, sizeof(BITMAPFILEHEADER));
    if (nCount != sizeof(BITMAPFILEHEADER)) {
      AfxMessageBox("Error reading header");
      return FALSE;
    }
    if (bmfh.bfType != 0x4d42) {// 'BM'
      AfxMessageBox("Invalid bitmap file");
      return FALSE;
    }
    nSize = bmfh.bfOffBits - sizeof(BITMAPFILEHEADER);
    m_lpBMIH = (LPBITMAPINFOHEADER) new char[nSize];
    nCount = pFile->Read(m_lpBMIH, nSize);// info hdr & color table
    // check if Windows 3.x or Windows NT BMP
    if (m_lpBMIH->biSize != 40) {
      AfxMessageBox("Is not a Windows 3.x or Windows NT BMP (OS/2 BMP?)");
      return FALSE;
    }
    // check if compressed
    if (!((m_lpBMIH->biCompression == BI_RGB) || (m_lpBMIH->biCompression == BI_RLE8) || (m_lpBMIH->biCompression == BI_RLE4))) {
      AfxMessageBox("Compressed bitmaps not supported");
      return FALSE;
    }
    ComputeMetrics();// sets m_dwSizeImage etc.
    m_lpImage = (LPBYTE) new char[m_dwSizeImage];
    nCount = pFile->Read(m_lpImage, m_dwSizeImage);// image only
  }
  CATCH(CException, e)
  {
    AfxMessageBox("Error reading file (exception thrown)");
    return FALSE;
  }
  END_CATCH
  ComputePaletteSize(m_lpBMIH->biBitCount);
  MakePalette();
  m_FileInfo.FileName = pFile->GetFileName();
  m_FileInfo.FilePath = pFile->GetFilePath();
  return TRUE;
}

BOOL CDib::Read(CString filename)
{
  TRY
  {
    CFile f(filename, CFile::modeRead);
    if (Read(&f)) {
      TRACE("DIB read from file %s\n", filename);
      return TRUE;
    }
  }
  CATCH(CFileException, e)
  {
    CString msg;
    msg.Format("%s could not be opened (OS error information =  %d)", filename, e->m_cause);
    AfxMessageBox(msg);
  }
  END_CATCH
  return FALSE;
}

BOOL CDib::Write(
  CFile* pFile)// CFile ptr passed by the documents serialize function
{
  BITMAPFILEHEADER bmfh;
  bmfh.bfType = 0x4d42;// 'BM'
  int nSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries;
  bmfh.bfSize = nSize + sizeof(BITMAPFILEHEADER);
  bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
  bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries;
  TRY
  {
    pFile->Write((LPVOID)&bmfh, sizeof(BITMAPFILEHEADER));
    pFile->Write((LPVOID)m_lpBMIH, nSize);
    pFile->Write((LPVOID)m_lpImage, m_dwSizeImage);
  }
  CATCH(CException, e)
  {
    AfxMessageBox("Write error");
    return FALSE;
  }
  END_CATCH
  m_FileInfo.FileName = pFile->GetFileName();
  m_FileInfo.FilePath = pFile->GetFilePath();
  return TRUE;
}

BOOL CDib::Write(CString filename, BOOL overwrite)
{
  CString msg;
  TRY
  {
    CFileStatus status;
    if (CFile::GetStatus(filename, status)) {// 0 if file does not exist
      if (overwrite) {
        TRACE("Warning! Overwriting:");
      } else {
        msg.Format("Replace existing file %s?", filename);
        if (AfxMessageBox(msg, MB_YESNO) == IDNO) {
          return FALSE;
        }
      }
    }
    CFile f(filename, CFile::modeCreate | CFile::modeWrite);
    if (Write(&f)) {
      TRACE("DIB wrote to file %s\n", filename);
      return TRUE;
    }
  }
  CATCH(CFileException, e)
  {
    msg.Format("%s could not be opened (OS error information =  %d)", filename, e->m_cause);
    AfxMessageBox(msg);
  }
  END_CATCH
  return FALSE;
}

void CDib::Serialize(CArchive& ar)
{
  CObject::Serialize(ar);// always call base class Serialize
  ar.Flush();
  if (ar.IsStoring()) {
    Write(ar.GetFile());
  } else {
    Read(ar.GetFile());
  }
}

int CDib::GetSizeImage()
{
  return m_dwSizeImage;
}

int CDib::GetSizeHeader()
{
  return sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries;
}

DWORD
CDib::GetPixel(CPoint pnt) const
{
  CPoint pntReal = pnt + m_pntOrigo;
  register BYTE* adr;

  if ((pntReal.x >= 0) && (pntReal.x < m_lpBMIH->biWidth) && (pntReal.y >= 0) && (pntReal.y < m_lpBMIH->biHeight))
    switch (m_lpBMIH->biBitCount) {
    case 1:
      adr = m_lpImage + (m_lpBMIH->biHeight - pntReal.y - 1) * m_nByteWidth + (pntReal.x >> 3);
      return (((*adr) & (0x80 >> (pntReal.x & 0x7))) != 0);
    case 8:
      adr = m_lpImage + (m_lpBMIH->biHeight - pntReal.y - 1) * m_nByteWidth + pntReal.x;
      return (*adr) & 0xff;
    case 24:
      adr = m_lpImage + (m_lpBMIH->biHeight - pntReal.y - 1) * m_nByteWidth + pntReal.x + pntReal.x + pntReal.x;
      return ((*(DWORD*)adr) & 0xffffff);
    default:
      AfxMessageBox(
        "GetPixel: only implemented for 1,8 and 24 bit per pixels");
      return DIBERROR;
    }
  return DIBERROR;
}

DWORD
CDib::SetPixel(CPoint pnt, DWORD color)
{
  CPoint pntReal = pnt + m_pntOrigo;
  register unsigned int pixnum;
  register BYTE* adr;

  if ((pntReal.x >= 0) && (pntReal.x < m_lpBMIH->biWidth) && (pntReal.y >= 0) && (pntReal.y < m_lpBMIH->biHeight))
    switch (m_lpBMIH->biBitCount) {
    case 1:
      adr = m_lpImage + (m_lpBMIH->biHeight - pntReal.y - 1) * m_nByteWidth + (pntReal.x >> 3);
      pixnum = pntReal.x & 0x7;
      color ? (*adr |= (0x80 >> pixnum)) : (*adr &= (0xff7f >> pixnum));
      break;
    case 8:
      adr = m_lpImage + (m_lpBMIH->biHeight - pntReal.y - 1) * m_nByteWidth + pntReal.x;
      *adr = (BYTE)color;
      break;
    case 24:
      adr = m_lpImage + (m_lpBMIH->biHeight - pntReal.y - 1) * m_nByteWidth + pntReal.x + pntReal.x + pntReal.x;
      *((DWORD*)adr) = ((*((DWORD*)adr)) & 0xff000000) | (color & 0x00ffffff);
      break;
    default:
      AfxMessageBox(
        "SetPixel: only implemented for 1,8 and 24 bit per pixels");
      return DIBERROR;
    }
  return DIBERROR;
}

void CDib::Empty()
{
  // this is supposed to clean up whatever is in the DIB
  if (m_lpBMIH != NULL) {
    delete[] m_lpBMIH;
  }
  if (m_lpImage != NULL) {
    delete[] m_lpImage;
  }
  if (m_hPalette != NULL) ::DeleteObject(m_hPalette);
  if (m_pImageMatrix != NULL) {
    delete[] m_pImageMatrix;
  }
  m_lpBMIH = NULL;
  m_lpImage = NULL;
  m_hPalette = NULL;
  m_pImageMatrix = NULL;
  m_lpvColorTable = NULL;
  m_nColorTableEntries = 0;
  m_dwSizeImage = 0;
}

void CDib::SetOrigo(CPoint pntOrigo)
{
  m_pntOrigo = pntOrigo;
}

BOOL CDib::SetPaletteEntry(BYTE bEntry, DWORD dwValue)
{
  switch (m_lpBMIH->biBitCount) {
  case 1:
    if (bEntry > 1) {
      AfxMessageBox("SetPaletteEntry: palette entry out of range");
      ASSERT(FALSE);
      return FALSE;
    }
    *((DWORD*)((BYTE*)m_lpvColorTable + bEntry * sizeof(RGBQUAD))) = dwValue;
    return TRUE;
  case 8:
    *((DWORD*)((BYTE*)m_lpvColorTable + bEntry * sizeof(RGBQUAD))) = dwValue;
    return TRUE;
  case 24:
    AfxMessageBox(
      "SetPaletteEntry: image with 24 bits per pixel has no palette");
    ASSERT(FALSE);
    return FALSE;
  default:
    AfxMessageBox("CDib only supports 1, 8 and 24 bits per pixel");
    ASSERT(FALSE);
    return FALSE;
  }
  // MakePalette() ?????
}

DWORD
CDib::GetPaletteEntry(BYTE bEntry)
{
  switch (m_lpBMIH->biBitCount) {
  case 1:
    if (bEntry > 1) {
      AfxMessageBox("GetPaletteEntry: palette entry out of range");
      ASSERT(FALSE);
      return DIBERROR;
    }
    return *((DWORD*)((BYTE*)m_lpvColorTable + bEntry * sizeof(RGBQUAD)));
  case 8:
    return *((DWORD*)((BYTE*)m_lpvColorTable + bEntry * sizeof(RGBQUAD)));
  case 24:
    AfxMessageBox(
      "GetPaletteEntry: image with 24 bits per pixel has no palette");
    ASSERT(FALSE);
    return DIBERROR;
  default:
    AfxMessageBox("CDib only supports 1, 8 and 24 bits per pixel");
    ASSERT(FALSE);
    return DIBERROR;
  }
}

BOOL CDib::CopyPalette(const CDib& srcdib)
{// copy palette from srcdib

  ASSERT(m_nColorTableEntries == srcdib.m_nColorTableEntries);// colpal not same size
  switch (m_lpBMIH->biBitCount) {
  case 1:
  case 8:
    memcpy(m_lpvColorTable, srcdib.m_lpvColorTable, m_nColorTableEntries * sizeof(RGBQUAD));
    //			MakePalette() ????
    return TRUE;
  case 24:
    AfxMessageBox("CopyPalette: image with 24 bits per pixel has no palette");
    ASSERT(FALSE);
    return FALSE;
  default:
    AfxMessageBox("CDib only supports 1, 8 and 24 bits per pixel");
    ASSERT(FALSE);
    return FALSE;
  }
}

BOOL CDib::ConvertDepth8to1(BYTE cThreshold)
{// Converts an 8 bits per pixel image to 1 bit per pixel

  if ((!(m_lpBMIH)) || (m_lpBMIH->biBitCount != 8)) {
    AfxMessageBox("Cannot convert depth, source is not 8 bits per pixel");
    return FALSE;
  }
  LPBITMAPINFOHEADER lpBMIH1 =
    (LPBITMAPINFOHEADER) new char[sizeof(BITMAPINFOHEADER) + 2 * sizeof(RGBQUAD)];
  memcpy(lpBMIH1, m_lpBMIH, sizeof(BITMAPINFOHEADER));
  int nWidth = m_lpBMIH->biWidth;
  int nHeight = m_lpBMIH->biHeight;
  int bytewidth1 = (nWidth + 31) / 32 * 4;
  LPBYTE pImg8 = m_lpImage;
  LPBYTE pImg1 = (LPBYTE) new char[bytewidth1 * nHeight];
  LPBYTE lpImage1 = pImg1;
  int aligndw = (bytewidth1 - (nWidth + 7) / 8) + ((nWidth & 0x7) ? 1 : 0);// double word alignment
  int y = 0;
  while (y < nHeight) {
    int x = 0;
    BYTE cBit = 0x80;
    while (x < nWidth) {
      *pImg1 =
        ((*pImg8) > cThreshold) ? (cBit | (*pImg1)) : ((~cBit) & (*pImg1));
      if (!(cBit >>= 1)) {
        cBit = 0x80;
        pImg1++;
      }
      pImg8++;
      x++;
    }
    pImg1 += aligndw;
    if (nWidth != m_nByteWidth) {// if zero padded
      pImg8 += (m_nByteWidth - nWidth);
    }
    y++;
  }
  // clear 8 bpp image
  Empty();
  // set dib object to new 1 bpp picture
  m_lpBMIH = lpBMIH1;
  m_lpBMIH->biBitCount = 1;
  m_lpBMIH->biClrUsed = 0;
  ComputePaletteSize(m_lpBMIH->biBitCount);
  m_lpBMIH->biClrImportant = m_nColorTableEntries;
  m_lpBMIH->biSizeImage = 0;
  ComputeMetrics();
  m_lpBMIH->biSizeImage = m_dwSizeImage;
  m_lpImage = lpImage1;
  SetPaletteEntry(0, 0);// black pal. entry
  SetPaletteEntry(1, RGB(255, 255, 255));// white pal. entry
  MakePalette();
  return TRUE;
}

int CDib::FindThreshold(int width)
{// Calls SmoothHist until there is only one minimum found (not including
  // the ends)

  CDWordArray Hist;
  Hist.SetSize(m_nColorTableEntries);// array[0 - (m_nColorTableEntries-1)]
  BYTE** pImage = GetImageMatrix();// access image as matrix

  for (int y = 0; y < m_lpBMIH->biHeight; y++) {
    for (int x = 0; x < m_lpBMIH->biWidth; x++) {
      Hist[pImage[y][x]]++;
    }
  }
  int threshold;
  threshold = FindSingleMin(Hist);
  while (threshold < 0) {
    SmoothHist(Hist, width);
    threshold = FindSingleMin(Hist);
  }
  return threshold;
}

BOOL CDib::Rotate(double theta, BOOL centerOrigo, BOOL enlarge)
{// rotate DIB by theta radians. If enlarge is TRUE the size of the rotated
  // DIB will be enlarged. The point of rotation will be m_pntOrigo;

  if (m_lpBMIH->biBitCount != 8) {
    AfxMessageBox("Implemented for 8 bpp only!");
    return FALSE;
  }

  CSize dim = GetDimensions();
  SetOrigo(centerOrigo);
  CRect newrect = CalcNewCanvas(dim, theta, enlarge);
  CDib newdib =
    CDib(CSize(newrect.Size() + CSize(1, 1)), m_lpBMIH->biBitCount, FALSE);
  newdib.CopyPalette(*this);

  int newpix;
  BYTE** pImage = GetImageMatrix();
  BYTE** pNewImage = newdib.GetImageMatrix();
  for (int x = newrect.left; x <= newrect.right; x++) {
    for (int y = newrect.top; y <= newrect.bottom; y++) {
      CFloatPoint fpntFrom = FromPoint(CPoint(x, y), theta);
      CPoint pntFrom = fpntFrom.Int();
      if ((pntFrom.x >= 0) && (pntFrom.x < dim.cx) && (pntFrom.y >= 0) && (pntFrom.y < dim.cy)) {
        double a = fpntFrom.x - floor(fpntFrom.x);
        double b = fpntFrom.y - floor(fpntFrom.y);
        if ((pntFrom.x == (dim.cx - 1)) || (pntFrom.y == (dim.cy - 1))) {
          newpix =
            (int)((1.0 - a) * (1.0 - b) * (double)(pImage[pntFrom.y][pntFrom.x]) + (1.0 - a) * b * GetBorderPixel(pImage, pntFrom.x, pntFrom.y + 1) + a * (1.0 - b) * GetBorderPixel(pImage, pntFrom.x + 1, pntFrom.y) + a * b * GetBorderPixel(pImage, pntFrom.x + 1, pntFrom.y + 1));
        } else {
          newpix =
            (int)((1.0 - a) * (1.0 - b) * (double)(pImage[pntFrom.y][pntFrom.x]) + (1.0 - a) * b * (double)(pImage[pntFrom.y + 1][pntFrom.x]) + a * (1.0 - b) * (double)(pImage[pntFrom.y][pntFrom.x + 1]) + a * b * (double)(pImage[pntFrom.y + 1][pntFrom.x + 1]));
        }
        pNewImage[y - newrect.top][x - newrect.left] = (BYTE)newpix;
      }
    }
  }
  Empty();
  *this = newdib;
  return TRUE;
}

BYTE**
  CDib::GetImageMatrix()
{
  if (m_lpBMIH->biBitCount != 8) {
    AfxMessageBox("Cannot create matrix - not 8 bits per pixel");
    return NULL;
  } else if (m_pImageMatrix == NULL) {// check if matrix already exists
    m_pImageMatrix = (BYTE**)new BYTE*[m_lpBMIH->biHeight];
    for (int y = (m_lpBMIH->biHeight - 1); y >= 0; y--) {
      m_pImageMatrix[(m_lpBMIH->biHeight - 1) - y] =
        (BYTE*)(m_lpImage + y * m_nByteWidth);
    }
  }
  return m_pImageMatrix;
}

BOOL CDib::CopyToClipboard()
{// Before calling CopyToClipboard, an application must open the clipboard
  //  by using the OpenClipboard function.

  if (m_lpBMIH == NULL) return FALSE;

  // Clean clipboard of contents, and copy the DIB.
  HGLOBAL h = ::GlobalAlloc(GHND, GetSizeHeader() + GetSizeImage());
  if (h != NULL) {
    void* lp = ::GlobalLock(h);
    // copy header and palette
    memcpy(lp, m_lpBMIH, GetSizeHeader());
    // copy image bits
    memcpy((LPBYTE)lp + GetSizeHeader(), m_lpImage, GetSizeImage());
    ::GlobalUnlock(h);
    EmptyClipboard();
    SetClipboardData(CF_DIB, h);
    CloseClipboard();
    return TRUE;
  }
  return FALSE;
}

BOOL CDib::PasteFromClipboard()
{// Before calling PasteFromClipboard, an application must open the clipboard
  //  by using the OpenClipboard function.

  HGLOBAL hNewDIB = (HGLOBAL)::GetClipboardData(CF_DIB);
  if (hNewDIB != NULL) {
    Empty();// free the old DIB
    LPBITMAPINFOHEADER lpNewBMIH = m_lpBMIH =
      (LPBITMAPINFOHEADER)::GlobalLock(hNewDIB);
    ComputePaletteSize(lpNewBMIH->biBitCount);
    int headersize =
      sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries;

    m_lpBMIH = (LPBITMAPINFOHEADER) new char[headersize];
    // copy header and palette
    memcpy(m_lpBMIH, lpNewBMIH, headersize);
    ComputeMetrics();
    MakePalette();

    m_lpImage = (LPBYTE) new char[m_dwSizeImage];
    // copy image bits
    memcpy(m_lpImage, (LPBYTE)lpNewBMIH + headersize, m_dwSizeImage);

    ::GlobalUnlock(hNewDIB);
    m_pntOrigo = CPoint(0, 0);// default origo

    //		EmptyClipboard();	//free handles to data in the clipboard
    CloseClipboard();// enable other windows to access the clipboard
    return TRUE;
  }
  return FALSE;
}

BOOL CDib::DibLoaded()
{
  return (m_lpBMIH != NULL);
}

void CDib::Scale(double factorX, double factorY)
{
  if (m_lpBMIH->biBitCount != 8) {
    AfxMessageBox("Implemented for 8 bpp only!");
    return;
  }
  if ((factorX < 0) || (factorY < 0)) {
    AfxMessageBox("Cannot scale image with negative factor");
    ASSERT(FALSE);
    return;
  }
  int nNewWidth = (int)ceil(m_lpBMIH->biWidth * factorX);
  int nNewHeight = (int)ceil(m_lpBMIH->biHeight * factorY);
  int addOffset = (4 - (nNewWidth % 4)) % 4;
  int nSizeImage = (nNewWidth + addOffset) * nNewHeight;
  LPBYTE lpNewImage = (LPBYTE)malloc(nSizeImage);
  double a, b;
  CFloatPoint fpntFrom;
  WORD wFromX, wFromY;
  BYTE** ImageMatrix = GetImageMatrix();
  int nYValue, nXValue, YIndexInNewIm;
  // calculate new image pixels excluding borders to the right and at bottom
  for (nYValue = 0; nYValue < (nNewHeight - (int)ceil(factorY));
       nYValue++) {// -factorY = do not calculate at border
    YIndexInNewIm = (nNewHeight - 1) * (nNewWidth + addOffset) - (nYValue * (nNewWidth + addOffset));
    for (nXValue = 0; nXValue < (nNewWidth - (int)ceil(factorX));
         nXValue++) {// -factorX = do not calculate at border
      fpntFrom.x = nXValue / factorX;
      fpntFrom.y = nYValue / factorY;
      // Finds interpolated value (bilinear)
      a = fpntFrom.x - floor(fpntFrom.x);
      b = fpntFrom.y - floor(fpntFrom.y);
      wFromX = int(fpntFrom.x);
      wFromY = int(fpntFrom.y);
      lpNewImage[YIndexInNewIm + nXValue] =
        (BYTE)((1.0 - a) * (1.0 - b) * (double)ImageMatrix[wFromY][wFromX] + (1.0 - a) * b * (double)ImageMatrix[wFromY + 1][wFromX] + a * (1.0 - b) * (double)ImageMatrix[wFromY][wFromX + 1] + a * b * (double)ImageMatrix[wFromY + 1][wFromX + 1]);
    }
  }
  // Calculate right border in new image (only interpolates in y-direction)
  fpntFrom.x = double(m_lpBMIH->biWidth - 1);
  wFromX = int(fpntFrom.x);
  a = fpntFrom.x - floor(fpntFrom.x);
  for (nYValue = 0; nYValue < (nNewHeight - (int)ceil(factorY));
       nYValue++) {// -factorY = do not calculate lower left corner
    YIndexInNewIm = (nNewHeight - 1) * (nNewWidth + addOffset) - (nYValue * (nNewWidth + addOffset));
    for (nXValue = (nNewWidth - (int)ceil(factorX)); nXValue < (nNewWidth);
         nXValue++) {// -factorX = do not calculate at border
      fpntFrom.y = nYValue / factorY;
      // Finds interpolated value (bilinear)
      b = fpntFrom.y - floor(fpntFrom.y);
      wFromX = (int)floor(fpntFrom.x);
      wFromY = (int)floor(fpntFrom.y);
      lpNewImage[YIndexInNewIm + nXValue] = (BYTE)(
        (1.0 - a) * (1.0 - b) * (double)ImageMatrix[wFromY][wFromX] + (1.0 - a) * b * (double)ImageMatrix[wFromY + 1][wFromX] + a * (1.0 - b) * (double)ImageMatrix[wFromY][wFromX]// indexing out of image -
        // uses pntFrom-value
        + a * b * (double)ImageMatrix[wFromY][wFromX]);// indexing out of image
        // - uses pntFrom-value
    }
  }
  // Calculate lower border in new image (only interpolates in x-direction)
  fpntFrom.y = double(m_lpBMIH->biHeight - 1);
  wFromY = int(fpntFrom.y);
  b = fpntFrom.y - floor(fpntFrom.y);
  for (nYValue = (nNewHeight - (int)ceil(factorY)); nYValue < nNewHeight;
       nYValue++) {
    YIndexInNewIm = (nNewHeight - 1) * (nNewWidth + addOffset) - (nYValue * (nNewWidth + addOffset));
    for (nXValue = 0; nXValue < (nNewWidth - (int)ceil(factorX));
         nXValue++) {// -factorX = do not calculate lower left corner
      fpntFrom.x = nXValue / factorX;
      // Finds interpolated value (bilinear)
      a = fpntFrom.x - floor(fpntFrom.x);
      wFromX = (int)floor(fpntFrom.x);
      lpNewImage[YIndexInNewIm + nXValue] = (BYTE)(
        (1.0 - a) * (1.0 - b) * (double)ImageMatrix[wFromY][wFromX] + (1.0 - a) * b * (double)ImageMatrix[wFromY][wFromX]// indexing out of image -
        // uses pntFrom-value
        + a * (1.0 - b) * (double)ImageMatrix[wFromY][wFromX + 1] + a * b * (double)ImageMatrix[wFromY][wFromX]);// indexing out of image -
        // uses pntFrom-value
    }
  }
  // still missing lower right rectangle, simply copies old corner pixel
  for (nYValue = (nNewHeight - (int)ceil(factorY)); nYValue < nNewHeight;
       nYValue++) {
    YIndexInNewIm = (nNewHeight - 1) * (nNewWidth + addOffset) - (nYValue * (nNewWidth + addOffset));
    for (nXValue = (nNewWidth - (int)ceil(factorX)); nXValue < nNewWidth;
         nXValue++) {
      lpNewImage[YIndexInNewIm + nXValue] =
        (BYTE)ImageMatrix[m_lpBMIH->biHeight - 1][m_lpBMIH->biWidth - 1];
    }
  }
  // copy scaled image to CDib object
  LPBYTE temp = m_lpImage;
  m_lpImage = lpNewImage;
  free(temp);
  m_lpBMIH->biWidth = nNewWidth;
  m_lpBMIH->biHeight = nNewHeight;
  m_lpBMIH->biSizeImage =
    0;// So that ComputeMetrics() computes m_dwSizeImage variable
  ComputeMetrics();
  if (m_pImageMatrix != NULL) {// An old pointer is no longer valid
    delete[] m_pImageMatrix;
  }
  m_pImageMatrix = NULL;
}

BOOL CDib::DecreaseGrayTones(BYTE byColors)
{
  // Inserts "byColors" (defaults 5) colors at the end of the palette. Colors
  // are from the def256pal constants in top of this file byColors max value is
  // 254, best results for byColors<=245
  if ((!(m_lpBMIH)) || (m_lpBMIH->biBitCount != 8)) {
    AfxMessageBox("Cannot decrease gray tones, source is not 8 bits per pixel");
    return FALSE;
  }
  DWORD rgb = GetPaletteEntry(250);
  if ((GetRValue(rgb) != 250) || (GetGValue(rgb) != 250) || (GetBValue(rgb) != 250)) {
    AfxMessageBox("Not a correct grayscale image");
    return FALSE;
  }
  float fDecreaseFactor = (float)((255 - byColors) / (float)255);
  BYTE byCount;
  for (byCount = 0; byCount <= (255 - byColors); byCount++) {
    SetPaletteEntry(byCount, RGB((BYTE)(byCount / fDecreaseFactor + 0.5), (BYTE)(byCount / fDecreaseFactor + 0.5), (BYTE)(byCount / fDecreaseFactor + 0.5)));
  }
  BYTE** DibMatrix = GetImageMatrix();
  for (WORD wYCount = 0; wYCount < m_lpBMIH->biHeight; wYCount++) {
    for (WORD wXCount = 0; wXCount < m_lpBMIH->biWidth; wXCount++) {
      DibMatrix[wYCount][wXCount] =
        (BYTE)((float)DibMatrix[wYCount][wXCount] * fDecreaseFactor);
    }
  }
  for (byCount = 255; byCount > (255 - byColors); byCount--) {
    SetPaletteEntry(byCount,
      RGB(def256pal_r[(byCount - 1)], def256pal_g[(byCount - 1)], def256pal_b[(byCount - 1)]));
  }
  return TRUE;
}

BOOL CDib::Blur(BYTE byBlurFactor)
{
  // blurs image with a mask of size (byBlurFactor*byBlurFactor)
  if ((!(m_lpBMIH)) || (m_lpBMIH->biBitCount != 8)) {
    AfxMessageBox("Cannot blur image, source is not 8 bits per pixel");
    return FALSE;
  }
  BYTE** DibMatrix = GetImageMatrix();
  int nLocalXC, nLocalYC;
  WORD wSumPixels, wNrOfPixels;
  DWORD dwPixVal;
  for (WORD wXCount = 0; wXCount < m_lpBMIH->biWidth; wXCount++) {
    for (WORD wYCount = 0; wYCount < m_lpBMIH->biHeight; wYCount++) {
      wSumPixels = 0;
      wNrOfPixels = 0;
      for (nLocalXC = (wXCount - byBlurFactor);
           nLocalXC <= (wXCount + byBlurFactor);
           nLocalXC++) {
        for (nLocalYC = (wYCount - byBlurFactor);
             nLocalYC <= (wYCount + byBlurFactor);
             nLocalYC++) {
          if ((dwPixVal = GetPixel(CPoint(nLocalXC, nLocalYC))) != DIBERROR) {
            wSumPixels += (WORD)dwPixVal;
            wNrOfPixels++;
          }
        }
      }
      DibMatrix[wYCount][wXCount] = (wSumPixels / wNrOfPixels);
    }
  }
  return TRUE;
}

//***********************************************
//         private helper functions
//***********************************************

void CDib::ComputePaletteSize(int nBitCount)
{
  if (m_lpBMIH == NULL || m_lpBMIH->biClrUsed == 0) {
    switch (nBitCount) {
    case 1:
      m_nColorTableEntries = 2;
      break;
    case 4:
      m_nColorTableEntries = 16;
      break;
    case 8:
      m_nColorTableEntries = 256;
      break;
    case 24:
      m_nColorTableEntries = 0;
      break;
    default:
      AfxMessageBox("CDib only supports 1, 8 and 24 bits per pixel");
      ASSERT(FALSE);
    }
  } else {
    m_nColorTableEntries = m_lpBMIH->biClrUsed;
  }
  ASSERT(m_nColorTableEntries <= 256);
}

void CDib::ComputeMetrics()
{
  m_dwSizeImage = m_lpBMIH->biSizeImage;
  if (m_dwSizeImage == 0) {
    DWORD dwBytes = ((DWORD)m_lpBMIH->biWidth * m_lpBMIH->biBitCount) / 32;
    if (((DWORD)m_lpBMIH->biWidth * m_lpBMIH->biBitCount) % 32) {
      dwBytes++;
    }
    dwBytes *= 4;
    m_dwSizeImage = dwBytes * m_lpBMIH->biHeight;// no compression
  }
  m_lpvColorTable = (LPBYTE)m_lpBMIH + sizeof(BITMAPINFOHEADER);
  m_nByteWidth = (((m_lpBMIH->biWidth) * (m_lpBMIH->biBitCount) + 31) / 32 * 4);
}

int CDib::FindSingleMin(const CDWordArray& Hist)
{// If only one minimum (not including the ends) is found its intensity
  // is returned, otherwise -1 is returned

  int color = 0, min;

  while ((color < m_nColorTableEntries - 1) && (Hist[color] <= Hist[color + 1])) {
    color++;
  }
  while ((color < m_nColorTableEntries - 1) && (Hist[color] >= Hist[color + 1])) {
    color++;
  }
  min = color;
  while ((color < m_nColorTableEntries - 1) && (Hist[color] <= Hist[color + 1])) {
    color++;
  }
  while ((color < m_nColorTableEntries - 1) && (Hist[color] >= Hist[color + 1])) {
    color++;
  }
  if (color < m_nColorTableEntries - 1) {// more than one minimum
    min = -1;
  }
  return min;
}

void CDib::SmoothHist(CDWordArray& Hist, int width)
{// Smoothes the histogram to remove local variations by changing each value
  // to the mean value of itself at its width-1 neighbours (width must be odd)

  CDWordArray NewHist;
  NewHist.SetSize(Hist.GetSize());
  int color, count, hw = (width - 1) / 2;
  DWORD sum;

  for (color = 0; color <= m_nColorTableEntries - 1; color++) {
    sum = 0;
    count = 0;
    for (int i = color - hw; i <= color + hw; i++) {
      if ((i >= 0) && (i < m_nColorTableEntries)) {
        sum += Hist[i];
        count++;
      }
    }
    NewHist[color] = sum / count;
  }
  Hist.Copy(NewHist);
}

CFloatPoint
  CDib::FromPoint(CPoint pnt, double theta)
{// finds point to get pixel in target-to-source rotation

  CFloatPoint fPnt;

  fPnt.x = (pnt.x - m_fpntRotOrigo.x) * cos(theta) - (pnt.y - m_fpntRotOrigo.y) * sin(theta) + m_fpntRotOrigo.x;
  fPnt.y = (pnt.x - m_fpntRotOrigo.x) * sin(theta) + (pnt.y - m_fpntRotOrigo.y) * cos(theta) + m_fpntRotOrigo.y;
  return fPnt;
}

CFloatPoint
  CDib::ToPoint(CPoint pnt, double theta)
{// finds where point maps to in source-to-target rotation

  CFloatPoint fPnt;

  fPnt.x = (pnt.x - m_fpntRotOrigo.x) * cos(theta) + (pnt.y - m_fpntRotOrigo.y) * sin(theta) + m_fpntRotOrigo.x;
  fPnt.y = (m_fpntRotOrigo.x - pnt.x) * sin(theta) + (pnt.y - m_fpntRotOrigo.y) * cos(theta) + m_fpntRotOrigo.y;
  return fPnt;
}

CRect CDib::CalcNewCanvas(const CSize& dim, double theta, BOOL enlarge)
{
  if (enlarge) {
    CFloatPoint ul = ToPoint(CPoint(0, 0), theta);
    CFloatPoint ur = ToPoint(CPoint(dim.cx - 1, 0), theta);
    CFloatPoint lr = ToPoint(dim - CPoint(1, 1), theta);
    CFloatPoint ll = ToPoint(CPoint(0, dim.cy - 1), theta);
    CPoint pntMin(round2(min(0, min(min(ul.x, ur.x), min(lr.x, ll.x)))),
      round2(min(0, min(min(ul.y, ur.y), min(lr.y, ll.y)))));
    CPoint pntMax(
      round2(max(dim.cx - 1, max(max(ul.x, ur.x), max(lr.x, ll.x)))),
      round2(max(dim.cy - 1, max(max(ul.y, ur.y), max(lr.y, ll.y)))));
    return CRect(pntMin, pntMax);
  } else {
    return CRect(CPoint(0, 0), dim - CPoint(1, 1));
  }
}

void CDib::SetOrigo(BOOL centerOrigo)
{
  if (centerOrigo) {
    CSize dim = GetDimensions();
    m_fpntRotOrigo.x = (double)(dim.cx - 1) / 2;
    m_fpntRotOrigo.y = (double)(dim.cy - 1) / 2;
  } else {
    m_fpntRotOrigo.x = (double)m_pntOrigo.x;
    m_fpntRotOrigo.y = (double)m_pntOrigo.y;
  }
}

double
  CDib::GetBorderPixel(BYTE** pImage, int x, int y)
{
  CSize dim = GetDimensions();
  if ((x == dim.cx) && (y == dim.cy)) {
    return (double)pImage[y - 1][x - 1];
  } else if (x == dim.cx) {
    return (double)pImage[y][x - 1];
  } else {
    return (double)pImage[y - 1][x];
  }
}
