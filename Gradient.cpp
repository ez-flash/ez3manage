///////////////////////////////////////////////////////////////////////////////
// class CGradient
//
// Author:  Yury Goltsman
// email:   ygprg@go.to
// page:    http://go.to/ygprg
// Copyright © 2000, Yury Goltsman
//
// This code provided "AS IS," without warranty of any kind.
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// version : 1.1
// Gradient shift added
// Linear Gradient stretch factor
// Per color gradient stretch factor
//
// version : 1.0
// This code is part of CProgressCtrlX and
//   specially oriented for gradient drawing
// Added palette support for 256 colors mode
// Fully rewritten to be more extensible
//

#include "stdafx.h"
#include "Gradient.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGradient::CGradient()
{
	m_fCreatePalette = FALSE;
	m_fDirection = LTR;
	m_flStretchGrad = 1;
}

CGradient::~CGradient()
{

}

void CGradient::DrawLinearGradient(CDC *pDC, CRect rcGrad, int nClipStart, int nClipEnd, int nShift)
{
	BOOL f256Color = pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE;
	if(!f256Color && (pDC->GetDeviceCaps(BITSPIXEL)*pDC->GetDeviceCaps(PLANES) < 8))
	{
		// for 16 colors no gradient
		ASSERT(m_ardwGradColors.GetSize() > 0);
		pDC->FillSolidRect(&ConvertToReal(rcGrad, nClipStart, nClipEnd), m_ardwGradColors[0]);
		return; 
	}

	int nGradWidth = 0;
	if(m_fDirection == TTB || m_fDirection == BTT)
		nGradWidth = rcGrad.Height();	// vert
	else
		nGradWidth = rcGrad.Width();	// horz

	if(nClipEnd == -1) nClipEnd = nGradWidth;
	ASSERT(nGradWidth >= nClipEnd);

	ASSERT(m_flStretchGrad >= 1);
	nGradWidth = int(nGradWidth * m_flStretchGrad);

	CGradArray arElements;
	CalcShiftedGradient(arElements, nShift, nGradWidth, nClipStart, nClipEnd, f256Color ? 236 : -1);
	int nSteps = arElements.GetSize();
	
	int nBandStart = nClipStart;
	int nBandEnd = nClipStart;

	// Start filling
	CPalette *pOldPal = NULL;
	if (f256Color && m_fCreatePalette && GetPalette().GetSafeHandle())
	{
		pOldPal = pDC->SelectPalette(&GetPalette(), FALSE);
		pDC->RealizePalette();
	}

	for (int i = 0; i < nSteps; i++, nBandStart = nBandEnd) 
	{
		nBandEnd += arElements[i].length;

		COLORREF nColor = arElements[i].color;
		if(f256Color)
		{
			if (pOldPal) // in background draw dithered
				nColor |= 0x02000000; // (PALETTERGB) without it will be dithering

			CBrush br(nColor);
			// CDC::FillSolidRect is faster, but it does not handle 8-bit color depth
			pDC->FillRect(&ConvertToReal(rcGrad, nBandStart, nBandEnd), &br);
			br.DeleteObject();
		}
		else
			pDC->FillSolidRect(&ConvertToReal(rcGrad, nBandStart, nBandEnd), nColor);
	}
	if(pOldPal)
		pDC->SelectPalette(pOldPal, TRUE);
}

CRect CGradient::ConvertToReal(CRect rcDraw, int nBandStart, int nBandEnd)
{
	BOOL fReverse = (m_fDirection == TTB || m_fDirection == RTL);
	BOOL fVert = (m_fDirection == TTB || m_fDirection == BTT);

	CRect rc(rcDraw);
	if(fVert)
	{
		if(nBandEnd == -1) nBandEnd = rcDraw.Height();
		rc.top = rcDraw.top + 
		         (fReverse ? nBandStart : (rcDraw.Height() - nBandEnd));
		rc.bottom = rc.top + (nBandEnd - nBandStart);
	}
	else
	{
		if(nBandEnd == -1) nBandEnd = rcDraw.Width();
		rc.left = rcDraw.left + 
		          (fReverse ? (rcDraw.Width() - nBandEnd) : nBandStart);
		rc.right = rc.left + (nBandEnd - nBandStart);
	}
	return rc;
}

void CGradient::CalcShiftedGradient(CGradArray& arElements, int nShift, int nGradWidth, 
                                  int nClipStart, int nClipEnd, UINT nMaxColors)
{
	// normalize shift
	if(nGradWidth == 0)
		nShift = 0;
	else
	{
		while(nShift < 0) nShift += nGradWidth;
		nShift %= nGradWidth;
	}

	if(nShift == 0 || m_ardwGradColors.GetSize() < 2)
	{
		CalcMultiGradient(arElements, nGradWidth, nClipStart, nClipEnd, nMaxColors);
		return;
	}

	CGradArray arElementsOrig;
	CalcMultiGradient(arElementsOrig, nGradWidth, 0, -1, nMaxColors);
	int nCount = arElementsOrig.GetSize();
	if(nCount < 2)
	{
		// no gradient
		arElements.Add(CGradElement(m_ardwGradColors[0], nClipEnd - nClipStart));
		return;
	}
	
	// do shift
	CGradArray arElementsShift;
	int nLength = 0;
	for(int i = nCount-1; i >= 0; i--)
	{
		CGradElement& el = arElementsOrig.ElementAt(i);
		if(nLength + el.length > nShift)
		{
			// Separate
			arElementsShift.InsertAt(0, CGradElement(el.color, nShift - nLength));
			el.length -= nShift - nLength;
			break;
		}
		else
		{
			nLength += el.length;
			arElementsShift.InsertAt(0, el);
			arElementsOrig.RemoveAt(i);
			if(nLength == nShift)
				break;
		}
	}
	// combine shifted
	arElementsShift.Append(arElementsOrig);

	// do clip
	if(nClipEnd == -1) nClipEnd = nGradWidth;
	nCount = arElementsShift.GetSize();
	nLength = 0;
	for(i = 0; i < nCount; i++)
	{
		CGradElement& el = arElementsShift.ElementAt(i);
		if(nLength + el.length <= nClipStart)
		{
			nLength += el.length;
			continue;	// skip before clip start
		}
		int nStart = max(nLength, nClipStart);
		nLength += el.length;
		int nEnd = min(nLength, nClipEnd);
		arElements.Add(CGradElement(el.color, nEnd - nStart));
		if(nLength >= nClipEnd)
			break; // skip after clip end
	}
}

void CGradient::CalcMultiGradient(CGradArray& arElements, int nGradWidth, 
                                  int nClipStart, int nClipEnd, UINT nMaxColors)
{
	if(nClipEnd == -1) nClipEnd = nGradWidth;
	int nSteps = m_ardwGradColors.GetSize()-1;
	if(nSteps < 0)
	{
		ASSERT(0); // at least 1 color should be added
		return;
	}
	if(nSteps == 0)
	{
		arElements.Add(CGradElement(m_ardwGradColors[0], nClipEnd - nClipStart));
		return;
	}
	
	double flBandGradStart = 0;

	nMaxColors /= nSteps;
	for (int i = 0; i < nSteps; i++) 
	{
		int nBandGradStart = int((double)nGradWidth*flBandGradStart/100);
		flBandGradStart += m_arflGradStretch[i];
		int nBandGradEnd = int((double)nGradWidth*flBandGradStart/100);

		if(i == nSteps-1)	//last step (because of problems with float)
			nBandGradEnd = nGradWidth;

		if(nBandGradEnd < nClipStart)
			continue; // skip - band before cliping rect
		
		int nBandClipStart = nBandGradStart;
		int nBandClipEnd = nBandGradEnd;
		if(nBandClipStart < nClipStart)
			nBandClipStart = nClipStart;
		if(nBandClipEnd > nClipEnd)
			nBandClipEnd = nClipEnd;

		CalcGradient(arElements, m_ardwGradColors[i], m_ardwGradColors[i+1], 
		             nBandGradEnd - nBandGradStart, 
		             nBandClipStart - nBandGradStart, 
		             nBandClipEnd - nBandGradStart, 
		             nMaxColors);

		if(nBandClipEnd == nClipEnd)
			break; // stop filling - next band is out of clipping rect
	}
}

void CGradient::CalcGradient(CGradArray& arElements, COLORREF clrStart, COLORREF clrEnd, 
                             int nGradWidth, int nClipStart, int nClipEnd, 
                             UINT nMaxColors)
{
	if(nClipEnd == -1) nClipEnd = nGradWidth;
	// Split colors to RGB chanels, find chanel with maximum difference 
	// between the start and end colors. This distance will determine 
	// number of steps of gradient
	int r = (GetRValue(clrEnd) - GetRValue(clrStart));
	int g = (GetGValue(clrEnd) - GetGValue(clrStart));
	int b = (GetBValue(clrEnd) - GetBValue(clrStart));
	UINT nSteps = max(abs(r), max(abs(g), abs(b)));
	nSteps = min(nSteps, nMaxColors);
	// if number of pixels in gradient less than number of steps - 
	// use it as numberof steps
	UINT nPixels = nGradWidth;
	nSteps = min(nPixels, nSteps);
	if(nSteps == 0) nSteps = 1;

	float rStep = (float)r/nSteps;
	float gStep = (float)g/nSteps;
	float bStep = (float)b/nSteps;

	r = GetRValue(clrStart);
	g = GetGValue(clrStart);
	b = GetBValue(clrStart);

	float nWidthPerStep = (float)nGradWidth / nSteps;
	CBrush br;
	// Start filling
	for (UINT i = 0; i < nSteps; i++) 
	{
		int nFillStart = (int)(nWidthPerStep * i);
		int nFillEnd = (int)(nWidthPerStep * (i+1));
		if(i == nSteps-1)	//last step (because of problems with float)
			nFillEnd = nGradWidth;

		if(nFillEnd < nClipStart)
			continue; // skip - band before cliping rect
		
		// clip it
		if(nFillStart < nClipStart)
			nFillStart = nClipStart;
		if(nFillEnd > nClipEnd)
			nFillEnd = nClipEnd;

		COLORREF clrFill = RGB(r + (int)(i * rStep),
		                       g + (int)(i * gStep),
		                       b + (int)(i * bStep));
		// add band
		arElements.Add(CGradElement(clrFill, nFillEnd - nFillStart));

		if(nFillEnd >= nClipEnd)
			break; // stop filling if we reach current position
	}
}

void CGradient::SetGradientColorsX(int nCount, COLORREF clrFirst, COLORREF clrNext, ...)
{ 
	ASSERT(nCount > 1);
	m_ardwGradColors.SetSize(nCount); 
	
	m_ardwGradColors.SetAt(0, clrFirst); 
	m_ardwGradColors.SetAt(1, clrNext);  

	if(nCount > 2)
	{
		va_list pArgs;
		va_start(pArgs, clrNext);
		for(int i = 2; i < nCount; i++)
			m_ardwGradColors.SetAt(i, va_arg(pArgs, COLORREF));
		va_end( pArgs );
	}	
	
	nCount--;
	m_arflGradStretch.SetSize(nCount);
	for(int i = 0; i < nCount; i++)
		m_arflGradStretch[i] = (double)100 / nCount;
	
	// remove all dependent objects
	m_Pal.DeleteObject();
}

void CGradient::GetGradientColors(COLORREF& clrStart, COLORREF& clrEnd) 
{ 
	if(m_ardwGradColors.GetSize() > 0)
	{
		clrStart = m_ardwGradColors[0]; 
		clrEnd = m_ardwGradColors[m_ardwGradColors.GetSize() > 1 ? 1 : 0];
	}
	else
		clrStart = clrEnd = CLR_NONE;
}

void CGradient::AddColor(COLORREF clr) 
{
	m_ardwGradColors.Add(clr); 
	if(m_ardwGradColors.GetSize() > 1) 
	{
		double flNew = (double)100/m_arflGradStretch.GetSize();
		m_arflGradStretch.Add(flNew); 
		NormalizeColorsStretch();
	}
}

void CGradient::CreatePalette()
{
	if(!m_fCreatePalette || m_Pal.GetSafeHandle())
		return;

	int nNumColors = 236;
	CGradArray arElements;
	CalcMultiGradient(arElements, nNumColors, 0, nNumColors, nNumColors);
	ASSERT(nNumColors >= arElements.GetSize());
	nNumColors = arElements.GetSize();
	if(!nNumColors)
		return;

	LPLOGPALETTE lpPal = (LPLOGPALETTE)new BYTE[sizeof(LOGPALETTE) +
												sizeof(PALETTEENTRY) *
												nNumColors];
	
	if (!lpPal)
		return;

	lpPal->palVersion = 0x300;
	lpPal->palNumEntries = nNumColors;
	
	for (int i = 0; i < nNumColors; i++)
	{
		COLORREF nColor = arElements[i].color;
		
		lpPal->palPalEntry[i].peRed = GetRValue(nColor);
		lpPal->palPalEntry[i].peGreen = GetGValue(nColor);
		lpPal->palPalEntry[i].peBlue = GetBValue(nColor);
		lpPal->palPalEntry[i].peFlags = 0;
	}
	
	m_Pal.CreatePalette(lpPal);
	
	delete [](PBYTE)lpPal;
}

void CGradient::SetStretchGradient(float flStretchFactor)
{
	ASSERT(flStretchFactor >= 1);
	if(flStretchFactor < 1)
		flStretchFactor = 1;
	m_flStretchGrad = flStretchFactor;
}

void CGradient::NormalizeColorsStretch()
{
	int nCount = m_arflGradStretch.GetSize();
	ASSERT(nCount == m_ardwGradColors.GetSize()-1);
	double flFull = 0;
	for(int i = 0; i < nCount; i++)
		flFull += m_arflGradStretch[i];

	for(i = 0; i < nCount; i++)
		m_arflGradStretch[i] = m_arflGradStretch[i] * 100 / flFull;
}

void CGradient::SetColorsStretch(double flFirst, ...)
{
	int nCount = m_ardwGradColors.GetSize()-1;
	if(nCount < 1)
	{
		ASSERT(0); // before you use this function add all necessary colors
		return;
	}	

	m_arflGradStretch.SetSize(nCount);
	m_arflGradStretch.SetAt(0, flFirst);
	va_list pArgs;
	va_start(pArgs, flFirst);
	for(int i = 1; i < nCount; i++)
		m_arflGradStretch.SetAt(i, va_arg(pArgs, double));
	va_end( pArgs );

	NormalizeColorsStretch(); // normalize
}
