/*---------------------------------------------------------------------------------
	dibkernel.h, last modified 25th of November 1997
	René Dencker Eriksen (http://www.imada.ou.dk/~edr) and
	Hans-Martin Brændmose Jensen (http://www.mip.ou.dk/~hansm)
---------------------------------------------------------------------------------*/

#ifndef _CDIBKERNEL_H
#define _CDIBKERNEL_H

#include <math.h>						//cos & sin

#define DIBERROR 0xffffffff				//define constants
#define BLACK    0x0					//for 1 bit pictures only
#define WHITE    0x1					//for 1 bit pictures only
// only use the following color definitions after a call to
// DecreaseGrayTones(byColors) with byColor>=6
#define YELLOW	 255
#define PINK	 254
#define RED		 253
#define CYAN	 252
#define GREEN	 251
#define BLUE	 250

#define pi (2.0*asin(1.0))

typedef class {
public: //attributes
	double x,y;
public: //methods
	CPoint Round();
	CPoint Int();
} CFloatPoint;

int round(const double f);

class CDib : public CObject
{
	DECLARE_SERIAL(CDib)				//we want to use serialize

public: //attributes
	struct strFileInfo{
		CString FilePath;				//path+filename+extension
		CString FileName;				//     filename+extension
	}m_FileInfo;
	LPBITMAPINFOHEADER m_lpBMIH;		//ptr to BITMAPINFOHEADER structure
private: //attributes
	BYTE** m_pImageMatrix;				//image matrix pointer
	LPVOID  m_lpvColorTable;			//ptr to DIB palette
	CPoint m_pntOrigo;					//GetPixel and SetPixel origo
	LPBYTE  m_lpImage;					//address of DIB bits
	int m_nByteWidth;					//width of image (incl. zeropadding)
	DWORD m_dwSizeImage;				//size of image (incl. zeropadding)
	int m_nColorTableEntries;			//number of palette entries
	HPALETTE m_hPalette;				//system palette handle
	CFloatPoint m_fpntRotOrigo;			//rotation origo
public: //methods
	CDib();								//default constructor
	CDib(CSize size, int nBitCount,BOOL bInitialize=TRUE); //constr. for empty DIB
	CDib(const CDib& dib,CRect rect);	//copies rect from given DIB to new DIB
	~CDib();							//destructor
	CDib& operator=(const CDib& dib);	//overloading of assignment operator
	CDib(CDib& dib);					//copy constructor
	BOOL DibLoaded();					//check if DIB is empty or not
	int GetSizeImage();					//image bits in bytes (incl. zeropad.)
	int GetSizeHeader();				//size of bitmapinfoheader+colortable
	CSize GetDimensions() const;		//dimensions of image in pixels
	BOOL CDib::Draw(CDC* pDC,CRect rcDest, CPoint pntSrc=0); 
										//draw DIB, ment to be called from OnDraw
	BOOL Read(CFile* pFile);			//reads BMP (DIB) from file (serialize)
	BOOL Read(CString filename);		//loads bitmap from file "filename"
	BOOL Write(CFile* pFile);			//writes BMP (DIB) to file (serialize)
	BOOL Write(CString filename, BOOL overwrite=FALSE); //saves bitmap to file "filename"
	void Serialize(CArchive& ar);		//serializes the CDib object
	void Empty();						//clear image and deallocate memory
	DWORD GetPixel(CPoint pnt) const;	//get pixel value
	DWORD SetPixel(CPoint pnt, DWORD color); //set pixel value
	void SetOrigo(CPoint pntOrigo);		//set origo for GetPixel and SetPixel
	DWORD GetPaletteEntry(BYTE bEntry);	//retrieves a palette entry
	BOOL SetPaletteEntry(BYTE bEntry, DWORD dwValue); //sets a palette entry
	BOOL CopyPalette(const CDib& srcdib); //cpy palette from srcdib
	BYTE** GetImageMatrix();			//access image as matrix[y][x] (no security)
	BOOL CopyToClipboard();				//cpy to clipbrd - use OpenClipboard() first
	BOOL PasteFromClipboard();			//pst from clpbrd - use OpenClipboard() first
	int FindThreshold(int width=7);		//finds thresh for CD8to1() (width must be odd)
	BOOL ConvertDepth8to1(BYTE cThreshold); //converts 8bpp bmp to 1bpp bmp
	BOOL Rotate(double theta, BOOL centerOrigo=TRUE, BOOL enlarge=TRUE);
										//rotate by theta (rad.) ccw (B/W only)
	//always call GetImageMatrix() after Rotate() to get correct pointer
	void Scale(double factorX, double factorY); //scales image with x- and y factor
	//always call GetImageMatrix() after Scale() to get correct pointer
	BOOL DecreaseGrayTones(BYTE byColors=6); // insert some colors in the end of the palette
	BOOL Blur(BYTE byBlurFactor); // blurs the image, higher BlurFactor means more blur

	//to convert RGB-value to DWORD use the RGB(BYTE r,BYTE g,BYTE b) macro
	//to get Red, Green, Blue component use GetBValue(rgb), GetGValue(rgb),
	//  GetRValue(rgb) or PALETTEINDEX(i) macros
private: //methods
	BOOL MakePalette();									//dib functions
	void ComputePaletteSize(int nBitCount);
	void ComputeMetrics();
	void SmoothHist(CDWordArray& Hist, int width);		//threshold functions
	int FindSingleMin(const CDWordArray& Hist);
	CFloatPoint FromPoint(CPoint pnt, double theta);	//rotate functions
	CFloatPoint ToPoint(CPoint pnt, double theta);
	CRect CalcNewCanvas(const CSize& dim, double theta, BOOL enlarge);
	void SetOrigo(BOOL centerOrigo);
	double GetBorderPixel(BYTE** pImage, int x, int y);
};

#endif //_CDIBKERNEL_H