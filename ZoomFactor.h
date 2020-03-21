// ZoomFactor.h : interface of the CZoomFactor class
//

#ifndef _ZOOM_FACTOR_H_
#define _ZOOM_FACTOR_H_


//////////////////////////////////////////////////////////////////////////
// Class:    CZoomFactor
// Created:  28 May 98 by M.Ross
//
// Overview:
//    A class for representing a zoom ratio with limits between 
//  m_MaxIn:1 and 1:m_MaxOut.  Generally, the ratio is set at
//  m_Numerator:m_Denominator
//

class CZoomFactor
{
// Construction/destruction and serialization
public:
	CZoomFactor (const UINT max_in = 1, const UINT max_out = 1);
	~CZoomFactor ();

// Attributes
protected:
	// These two will stand for nothing if the m_bSpecific variable is flagged.
	UINT m_Numerator;
	UINT m_Denominator;

	// The limits of the zoom/shrink
	UINT m_MaxIn;	// Maximum numerator value
	UINT m_MaxOut;	// Maximum denominator value

	// The factor by which things must be multiplied for display, == m_Numerator / m_Denominator
	float m_Factor;

	// If the zoom factor has been set to a specific float value, then don't use the
	// m_Numerator:m_Denominator ratio
	BOOL m_bSpecific;

// Operations
public:
	// Get the factor by which positions must be multiplied for display
	float GetFactor () { return m_Factor; }

	// Is the current zoom ratio specifically-set e.g. by zoom-to-area?
	BOOL IsSpecific () { return m_bSpecific; }

	// Zoom in or out by one notch
	BOOL Magnify ();
	BOOL Shrink ();
	// Zoom or shrink to a specific integer factor
	BOOL MagnifyTo (const UINT numerator);
	BOOL ShrinkTo (const UINT denominator);
	// Zoom or shrink to a specific floating point factor
	BOOL SetFactor (const float factor);
	// Area of interest zooming - return the required scroll size
	CSize MagnifyArea (const CSize& sizWindow, const CSize& sizAOI, const CSize& sizDoc);
	CSize ShrinkArea (const CSize& sizWindow, const CSize& sizAOI, const CSize& sizDoc);

	// Can we zoom in or out?
	BOOL CanMagnify ();
	BOOL CanShrink ();

	// Set the limits for zooming
	BOOL SetLimits (const UINT max_in, const UINT max_out);
	BOOL SetMagnifyLimit (const UINT max_in);
	BOOL SetShrinkLimit (const UINT max_out);

// Implementation
protected:
	// Calculate the factor for multiplying, given numerator and denominator
	float CalcFactor (const UINT numer, const UINT denom)
		{ return ((float)numer / (float)denom); }

	// Is the given zoom factor roughly equivalent to an integer ratio? e.g. 1:6.001 or 1.002:1
	BOOL JustAboveIntegerZoom (const float factor, const float tolerance);
	BOOL JustBelowIntegerZoom (const float factor, const float tolerance);
};


#endif	// _ZOOM_FACTOR_H_