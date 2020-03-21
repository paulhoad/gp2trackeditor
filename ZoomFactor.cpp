// ZoomFactor.cpp : implementation of the CZoomFactor class
//
///////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "ZoomFactor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


///////////////////////////////////////////////////////////////////
// Construction and destruction

CZoomFactor::CZoomFactor (const UINT max_in, const UINT max_out)
{
	// Assign the numerator and denominator
	m_MaxIn = (max_in > 0) ? max_in : 1;
	m_MaxOut = (max_out > 0) ? max_out : 1;

	// Set the initial zoom
	m_Numerator = 1;
	m_Denominator = 1;

	// Calculate the scaling factor
	m_Factor = CalcFactor(m_Numerator, m_Denominator);

	// The factor represents the numerator and denominator
	m_bSpecific = FALSE;
}

CZoomFactor::~CZoomFactor ()
{
}


///////////////////////////////////////////////////////////////////
// Set the limits for zooming

// Set both limits at once
BOOL CZoomFactor::SetLimits (const UINT max_in, const UINT max_out)
{
	// Make sure they're valid values
	if (0 == max_in || 0 == max_out)
		return FALSE;

	// Assign member variables
	m_MaxIn = max_in;
	m_MaxOut = max_out;

	// Make sure we're not past the new limits already
	if (!CanMagnify())
		MagnifyTo(m_MaxIn);
	if (!CanShrink())
		ShrinkTo(m_MaxOut);

	return TRUE;
}

BOOL CZoomFactor::SetMagnifyLimit (const UINT max_in)
{
	// Make sure it's a valid value
	if (0 == max_in)
		return FALSE;

	// Assign member variable
	m_MaxIn = max_in;

	// Make sure we're not past the new limit already
	if (!CanMagnify())
		MagnifyTo(m_MaxIn);

	return TRUE;
}

BOOL CZoomFactor::SetShrinkLimit (const UINT max_out)
{
	// Make sure it's a valid value
	if (0 == max_out)
		return FALSE;

	// Assign member variable
	m_MaxOut = max_out;

	// Make sure we're not past the new limit already
	if (!CanShrink())
		ShrinkTo(m_MaxOut);

	return TRUE;
}


///////////////////////////////////////////////////////////////////
// Zoom in and out

// Can we zoom in?
BOOL CZoomFactor::CanMagnify ()
{
	return ((m_Factor + 0.001f) < (float)m_MaxIn);
}

// Can we zoom out?
BOOL CZoomFactor::CanShrink ()
{
	return ((m_Factor - 0.001f) > (1.0f / (float)m_MaxOut));
}

// Zoom in by one notch
BOOL CZoomFactor::Magnify ()
{
	if (!m_bSpecific)
	{
		if (1 == m_Denominator && m_Numerator < m_MaxIn)
			m_Numerator++;
		else if (m_Denominator > 1)
			m_Denominator--;
		else
			return FALSE;
	}
	else
	{
		if (m_Factor < 1.0f)	// Denominator > Numerator
		{
			ASSERT(m_Factor != 0.0f);
			m_Denominator = (int)(1.0f / m_Factor);
			if (JustAboveIntegerZoom(m_Factor, 0.0001f))
				// Reduce the denominator by another whole integer as it was
				// so close to being an integer anyway.
				m_Denominator = max(1, min(m_MaxOut, m_Denominator - 1));
			m_Numerator = 1;
		}
		else
		{
			m_Numerator = (int)m_Factor + 1;
			if (JustBelowIntegerZoom(m_Factor, 0.0001f))
				// Increase the numerator by another whole integer as it was
				// so close to being an integer anyway.
				m_Numerator = min(m_MaxIn, max(1, m_Numerator + 1));
			m_Denominator = 1;
		}
	}

	// No longer using a specific factor, if we were before
	m_bSpecific = FALSE;
	m_Factor = CalcFactor(m_Numerator, m_Denominator);

	return TRUE;
}

// Zoom out by one notch
BOOL CZoomFactor::Shrink ()
{
	if (!m_bSpecific)
	{
		if (1 == m_Numerator && m_Denominator < m_MaxOut)	// 1:x
			m_Denominator++;
		else if (m_Numerator > 1)	// x:1
			m_Numerator--;
		else
			return FALSE;
	}
	else
	{
		if (m_Factor < 1.0f)	// Denominator > Numerator
		{
			ASSERT(m_Factor != 0.0f);
			m_Denominator = (int)(1.0f / m_Factor) + 1;
			if (JustBelowIntegerZoom(m_Factor, 0.0001f))
				// Increase the denominator by another whole integer as it was
				// so close to being an integer anyway.  Think of original m_Factor == 6.9999
				m_Denominator = max(1, min(m_MaxOut, m_Denominator + 1));
			m_Numerator = 1;
		}
		else
		{
			m_Numerator = (int)m_Factor;
			if (JustAboveIntegerZoom(m_Factor, 0.0001f))
				// Increase the numerator by another whole integer as it was
				// so close to being an integer anyway.
				m_Numerator = min(m_MaxIn, max(1, m_Numerator - 1));
			m_Denominator = 1;
		}
	}

	// No longer using a specific factor, if we were before
	m_bSpecific = FALSE;
	m_Factor = CalcFactor(m_Numerator, m_Denominator);

	return TRUE;
}

// Magnify to a specific integer ratio
BOOL CZoomFactor::MagnifyTo (const UINT numerator)
{
	if (numerator < 1 || numerator > m_MaxIn)
		return FALSE;

	m_Numerator = numerator;
	m_Denominator = 1;
	m_bSpecific = FALSE;
	m_Factor = CalcFactor(m_Numerator, m_Denominator);

	return TRUE;
}

// Shrink to a specific integer ratio
BOOL CZoomFactor::ShrinkTo (const UINT denominator)
{
	if (denominator < 1 || denominator > m_MaxOut)
		return FALSE;

	m_Numerator = 1;
	m_Denominator = denominator;
	m_bSpecific = FALSE;
	m_Factor = CalcFactor(m_Numerator, m_Denominator);

	return TRUE;
}

// Zoom or shrink to a specific floating point factor
BOOL CZoomFactor::SetFactor (const float factor)
{
	// Make sure it's not outside the valid range
	if (factor > (float)m_MaxIn ||
		factor < (1.0f / (float)m_MaxOut))
		return FALSE;

	// Accept new value and set member variables
	m_Factor = factor;
	m_bSpecific = TRUE;

	return TRUE;
}

// Area of interest zooming in.  Return the required scroll size
CSize CZoomFactor::MagnifyArea (const CSize& sizWindow, const CSize& sizAOI, const CSize& sizDoc)
{
	// Make sure the parameters are ok
	if (sizDoc.cx < 1 || sizDoc.cy < 1 || sizWindow.cx < 1 || sizWindow.cy < 1
		|| sizAOI.cx < 1 || sizAOI.cy < 1)
		return CSize(-1, -1);

	// Calculate the desired zoom factor
	float fScale = min(float(sizWindow.cx) / float(sizAOI.cx),
					   float(sizWindow.cy) / float(sizAOI.cy));
	m_Factor *= (0.0f < fScale) ? fScale : 1.0f;

	// Limit zooming to the minimum and maximum extents
	if (m_Factor < (1.0f / (float)m_MaxOut))
		m_Factor = (1.0f / (float)m_MaxOut);
	if (m_Factor > (float)m_MaxIn)
		m_Factor = (float)m_MaxIn;

	// Flag zoom ratio as being set at a specific level
	// i.e. not an integer zoom
	m_bSpecific = TRUE;

	// Calculate and return the required scroll size (one of the dimensions should
	// be equal to the size of the window in that dimension
	return CSize(int(sizDoc.cx * m_Factor), int(sizDoc.cy * m_Factor));
}

// Area of interest zooming out.  Return the required scroll size
CSize CZoomFactor::ShrinkArea (const CSize& sizWindow, const CSize& sizAOI, const CSize& sizDoc)
{
	// Make sure the parameters are ok
	if (sizDoc.cx < 1 || sizDoc.cy < 1 || sizWindow.cx < 1 || sizWindow.cy < 1
		|| sizAOI.cx < 1 || sizAOI.cy < 1)
		return CSize(-1, -1);

	// Calculate the desired zoom factor
	float fScale = min(float(sizWindow.cx) / float(sizAOI.cx),
					   float(sizWindow.cy) / float(sizAOI.cy));
	m_Factor /= (0.0f < fScale) ? fScale : 1.0f;

	// Limit zooming to the minimum and maximum extents
	if (m_Factor < (1.0f / (float)m_MaxOut))
		m_Factor = (1.0f / (float)m_MaxOut);
	if (m_Factor > (float)m_MaxIn)
		m_Factor = (float)m_MaxIn;

	// Flag zoom ratio as being set at a specific level
	// i.e. not an integer zoom
	m_bSpecific = TRUE;

	// Calculate and return the required scroll size (one of the dimensions should
	// be equal to the size of the window in that dimension
	return CSize(int(sizDoc.cx * m_Factor), int(sizDoc.cy * m_Factor));
}


///////////////////////////////////////////////////////////////////
// Internal worker functions

// Is the given zoom factor roughly equivalent to an integer ratio? e.g. 1:6.001 or 3.002:1
BOOL CZoomFactor::JustAboveIntegerZoom (const float factor, const float tolerance)
{
	// Make sure the function input is not going to cause problems
	if (0.0f == factor)
		return TRUE;

	float fac = (factor < 1.0f) ? (1.0f / factor) : factor;

	return ((int)fac != (int)(fac - tolerance));
}

// Is the given zoom factor roughly equivalent to an integer ratio? e.g. 1:6.001 or 3.002:1
BOOL CZoomFactor::JustBelowIntegerZoom (const float factor, const float tolerance)
{
	// Make sure the function input is not going to cause problems
	if (0.0f == factor)
		return TRUE;

	float fac = (factor < 1.0f) ? (1.0f / factor) : factor;

	return ((int)fac != (int)(fac + tolerance));
}

