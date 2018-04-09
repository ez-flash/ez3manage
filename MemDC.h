#if !defined(AFX_MEMDC_H__CA1D3541_7235_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_MEMDC_H__CA1D3541_7235_11D1_ABBA_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MemDC.h : header file
//

//////////////////////////////////////////////////
// CMemDC - memory DC
//
// Author: Keith Rule
// Email:  keithr@europa.com
// Copyright 1996-1997, Keith Rule
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// History - 10/3/97 Fixed scrolling bug.
//					 Added print support.
//			 25 feb 98 - fixed minor assertion bug
//
// This class implements a memory Device Context

///////////////////////////////////////////////////////////////////////////////
// Modified by:  Yury Goltsman
// email:   ygprg@go.to
// page:    http://go.to/ygprg
//
// Modifications provided without warranty of any kind.
//
// version : 1.0
// Added optional parameter : rectangle of dc area
// Added preliminary copying image from orig. dc to bitmap
// (somewhy without this it doesn't work in some places)

class CMemDC : public CDC
{
public:

	// constructor sets up the memory DC
	CMemDC(CDC* pDC, LPRECT pRect = NULL, BOOL fCopyOld = FALSE, BOOL fUsePalette = FALSE) : CDC()
	{
		ASSERT(pDC != NULL);

		m_pDC = pDC;
		m_pOldBitmap = NULL;
		m_bMemDC = !pDC->IsPrinting();
		m_fUsePalette = fUsePalette;

		if (m_bMemDC) // Create a Memory DC
		{
			if(pRect)
				m_rect = *pRect;
			else
				pDC->GetClipBox(&m_rect);
			CreateCompatibleDC(pDC);
			m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
			m_pOldBitmap = SelectObject(&m_bitmap);
			SetWindowOrg(m_rect.left, m_rect.top);
			if(fCopyOld)
				BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
				       m_pDC, m_rect.left, m_rect.top, SRCCOPY);
		}
		else  // Make a copy of the relevent parts of the current DC for printing
		{
			m_bPrinting = pDC->m_bPrinting;
			m_hDC = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}
	}
	
	// Destructor copies the contents of the mem DC to the original DC
	~CMemDC()
	{
		if (m_bMemDC) 
		{	 
			CPalette pal, *pOldPal=NULL;
			if(m_fUsePalette && CreatePalette(pal))
			{
				pOldPal = m_pDC->SelectPalette(&pal, FALSE);
				m_pDC->RealizePalette();
			}
			// Copy the offscreen bitmap onto the screen.
			m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
			              this, m_rect.left, m_rect.top, SRCCOPY);

			if(pOldPal)
				m_pDC->SelectPalette(pOldPal, TRUE);
			//Swap back the original bitmap.
			SelectObject(m_pOldBitmap);
		} 
		else 
		{
			// All we need to do is replace the DC with an illegal value,
			// this keeps us from accidently deleting the handles associated with
			// the CDC that was passed to the constructor.
			m_hDC = m_hAttribDC = NULL;
		}
	}

	BOOL CreatePalette(CPalette& pal, CDC* pDC = NULL)
	{
		if(!pDC) pDC = this;
		if(!(pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE))
			return FALSE;

		//Get the DIBSection's color table
		const int nColors = 236;
		RGBQUAD rgb[nColors];
		::GetDIBColorTable(pDC->m_hDC, 0, nColors, rgb);

		//Create a palette from the color table
		LPLOGPALETTE lpPal = (LPLOGPALETTE) new BYTE[sizeof(LOGPALETTE) + (nColors*sizeof(PALETTEENTRY))];
		lpPal->palVersion = 0x300;       
		lpPal->palNumEntries = nColors;

		for (int i = 0; i < nColors; i++)       
		{
			lpPal->palPalEntry[i].peRed = rgb[i].rgbRed;
			lpPal->palPalEntry[i].peGreen = rgb[i].rgbGreen;
			lpPal->palPalEntry[i].peBlue = rgb[i].rgbBlue;
			lpPal->palPalEntry[i].peFlags = 0;
		}
		VERIFY(pal.CreatePalette(lpPal));
  
		//Clean up
		delete lpPal;
		return TRUE;
	}

	// Allow usage as a pointer
	CMemDC* operator->() {return this;}
		
	// Allow usage as a pointer
	operator CMemDC*() {return this;}

private:
	CBitmap  m_bitmap;      // Offscreen bitmap
	CBitmap* m_pOldBitmap;  // bitmap originally found in CMemDC
	CDC*     m_pDC;         // Saves CDC passed in constructor
	CRect    m_rect;        // Rectangle of drawing area.
	BOOL     m_bMemDC;      // TRUE if CDC really is a Memory DC.
	BOOL     m_fUsePalette; // if necessary create and select palette
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMDC_H__CA1D3541_7235_11D1_ABBA_00A0243D1382__INCLUDED_)
