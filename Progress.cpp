///////////////////////////////////////////////////////////////////////////////
// class CProgress
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
// version : 1.4
// Gradient Animation added
//
// version : 1.3
// Separated to window independent classes CProgress and CGradient (base)
// Added palette support for 256 colors mode (to CGradient)
// Added shaped style drawing 
//
//
// Old history of CProgressCtrlX:
//
// version : 1.2
// Added support for "Any angle text rotation" 
//   (define PBT_ANY_ANGLE and set font with appropriated angle)
// Added text alignment
// Added possibility to format text (via virtual function FormatText())
//   e.g. you can show percent as decimal fraction instead of integer
//
// version : 1.1
// Added multi-color gradient
// Added filling with brush for background and bar(overrides color settings)
// Added borders attribute
// Added vertical text support
// Added snake mode
// Added reverse mode
// Added dual color for text
// Added text formatting
// Added tied mode for text and rubber bar mode
// Added support for vertical oriented control(PBS_VERTICAL)
// 
// version : 1.0
//

#include "stdafx.h"
#include "Progress.h"

#include "MemDC.h"
#include "DrawGdiX.h"
#include "resource.h"

#ifdef PBT_ANY_ANGLE
	#include <math.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgress

CProgress::CProgress()
	: m_rcBorders(0,0,0,0)
{
	// Init colors
	m_clrBk = ::GetSysColor(COLOR_3DFACE);
	m_clrTextOnBar = ::GetSysColor(COLOR_CAPTIONTEXT);
	m_clrTextOnBk = ::GetSysColor(COLOR_BTNTEXT);
	
	// set gradient colors
	COLORREF clrStart, clrEnd;
	clrStart = clrEnd = ::GetSysColor(COLOR_ACTIVECAPTION);
#if(WINVER >= 0x0500)
	BOOL fGradientCaption = FALSE;
	if(SystemParametersInfo(SPI_GETGRADIENTCAPTIONS, 0, &fGradientCaption, 0) &&
	   fGradientCaption)
		clrEnd = ::GetSysColor(COLOR_GRADIENTACTIVECAPTION);
#endif /* WINVER >= 0x0500 */
	SetGradientColors(clrStart, clrEnd);
	m_pbrBk = m_pbrBar = NULL;

	m_nStep = 10;
	m_nTail = 0;
	m_nTailSize = 40;
	m_nLower = 0;
	m_nUpper = 100;
	m_nPos = 0;
	m_nAnimPos = 0;

	m_pFont = NULL;
	m_dwStyle = PBS_CENTER|PBS_VCENTER;
}

void CProgress::Draw(CDC* pDC, CRect rcDraw, BOOL fSkipDCCache) 
{
	if(m_rgnShape.GetSafeHandle())
	{
		pDC->SelectClipRgn(&m_rgnShape);
		pDC->OffsetClipRgn(rcDraw.left, rcDraw.top);
	}
	
	CDrawInfo info;
	info.rcClient = rcDraw;

	// retrieve current position and range
	info.nCurPos = GetPos();
	GetRange(info.nLower, info.nUpper);
	
	// Draw to memory DC
	BOOL fPrinting = pDC->m_bPrinting;
	if(fSkipDCCache) pDC->m_bPrinting = TRUE; // CMemDC will skip caching
	CMemDC memDC(pDC, rcDraw, !m_pbrBk && m_clrBk == CLR_NONE);
	pDC->m_bPrinting = fPrinting;
	info.pDC = &memDC;

	// fill background 
	if(m_pbrBk)
		info.pDC->FillRect(&info.rcClient, m_pbrBk);
	else if(m_clrBk != CLR_NONE)
		info.pDC->FillSolidRect(&info.rcClient, m_clrBk);

	// apply borders
	info.rcClient.DeflateRect(m_rcBorders);
		
	// if current pos is out of range return
	if (info.nCurPos < info.nLower || info.nCurPos > info.nUpper)
		return;

	info.dwStyle = GetBarStyle();
	BOOL fVert = info.dwStyle&PBS_VERTICAL;
	BOOL fSnake = info.dwStyle&PBS_SNAKE;
	BOOL fRubberBar = info.dwStyle&PBS_RUBBER_BAR;
	BOOL fRev = info.dwStyle&PBS_REVERSE;

	// calculate visible gradient width
	CRect rcBar(0,0,0,0);
	CRect rcMax(0,0,0,0);
	rcMax.right = fVert ? info.rcClient.Height() : info.rcClient.Width();
	rcBar.right = (int)((float)(info.nCurPos-info.nLower) * rcMax.right / (info.nUpper-info.nLower));
	if(fSnake)
		rcBar.left = (int)((float)(m_nTail-info.nLower) * rcMax.right / (info.nUpper-info.nLower));
	
	// draw bar
	if(m_pbrBar)
		info.pDC->FillRect(&ConvertToReal(info, rcBar), m_pbrBar);
	else
	{
		CRect rcMaxT(fRubberBar ? rcBar : rcMax);
		SetDirection(fVert ? (fRev ? TTB : BTT) : (fRev ? RTL : LTR));
		DrawLinearGradient(info.pDC, ConvertToReal(info, rcMaxT), rcBar.left-rcMaxT.left, rcBar.right-rcMaxT.left, rcMaxT.Width()*m_nAnimPos/(info.nUpper-info.nLower));
//		static i = 0;
//		DrawLinearGradient(info.pDC, ConvertToReal(info, rcMaxT), 0,-1, i+=m_nStep);
	}
	// Draw text
	DrawText(info, rcMax, rcBar);
	// Do not call CProgressCtrl::OnPaint() for painting messages
}

void CProgress::DrawText(const CDrawInfo& info, const CRect &rcMax, const CRect &rcBar)
{
	if(!(info.dwStyle&PBS_TEXTMASK))
		return;
	BOOL fVert = info.dwStyle&PBS_VERTICAL;
	CDC *pDC = info.pDC;

	CString sText = FormatText(info, GetText(), CalcPos(info));
	if (sText.IsEmpty())
		return;

	CFont* pFont = GetFont();
	CSelFont sf(pDC);
	if(pFont)
		sf.Select(pFont);
	CSelTextColor tc(pDC, m_clrTextOnBar);
	CSelBkMode bm(pDC, TRANSPARENT);
	CSelTextAlign ta(pDC, TA_BOTTOM|TA_CENTER);
	CPoint ptWOrg = pDC->GetWindowOrg();
	
	LONG grad = 0;
	if(pFont)
	{
		LOGFONT lf;
		pFont->GetLogFont(&lf);
		grad = lf.lfEscapement/10;
	}
	CSize sizText = pDC->GetTextExtent(sText);
	int cx = 0, cy = 0, dx = 0, dy = 0;
#ifdef PBT_ANY_ANGLE 
	// required "math.h"
	double pi = 3.1415926535;
	double rad = grad*pi/180;
	double nSin = sin(rad);
	double nCos = cos(rad);
	dx = (int)(sizText.cy*nSin);
	dy = (int)(sizText.cy*nCos);
	cx = (int)(fabs(sizText.cy*nSin) + fabs(sizText.cx*nCos));
	cy = (int)(fabs(sizText.cy*nCos) + fabs(sizText.cx*nSin));
#else //!PBT_ANY_ANGLE
	if(grad == 0)         {	cx = sizText.cx; cy = sizText.cy; dx = 0; dy = sizText.cy;}
	else if(grad == 90)   {	cx = sizText.cy; cy = sizText.cx; dx = sizText.cy; dy = 0;}
	else if(grad == 180)  {	cx = sizText.cx; cy = sizText.cy; dx = 0; dy = -sizText.cy;}
	else if(grad == 270)  {	cx = sizText.cy; cy = sizText.cx; dx = -sizText.cy; dy = 0;}
	else ASSERT(0); // angle not supported
#endif //PBT_ANY_ANGLE
	
	CPoint ptVpOrgOld = pDC->GetViewportOrg();
	if(info.dwStyle&PBS_TIED_TEXT)
	{
		CRect rcFill(ConvertToReal(info, rcBar));
		if((fVert ? cy : cx) <= rcBar.Width())
		{
			// align text
			if(info.dwStyle&PBS_LEFT)
				dx += -(rcFill.Width()-cx); 
			if(info.dwStyle&PBS_RIGHT)
				dx += (rcFill.Width()-cx); 
			if(info.dwStyle&PBS_TOP)
				dy += -(rcFill.Height()-cy); 
			if(info.dwStyle&PBS_BOTTOM)
				dy += (rcFill.Height()-cy); 

			pDC->SetViewportOrg(rcFill.left + (rcFill.Width() + dx)/2, 
													rcFill.top + (rcFill.Height() + dy)/2);
			DrawClippedText(info, rcBar, sText, ptWOrg);
		}
	}
	else
	{
		// align text
		if(info.dwStyle&PBS_LEFT)
			dx += -(info.rcClient.Width()-cx); 
		if(info.dwStyle&PBS_RIGHT)
			dx += (info.rcClient.Width()-cx); 
		if(info.dwStyle&PBS_TOP)
			dy += -(info.rcClient.Height()-cy); 
		if(info.dwStyle&PBS_BOTTOM)
			dy += (info.rcClient.Height()-cy); 

		pDC->SetViewportOrg(info.rcClient.left + (info.rcClient.Width() + dx)/2, 
		                    info.rcClient.top + (info.rcClient.Height() + dy)/2);
		
		// draw text on background
		tc.Select(m_clrTextOnBk);
		DrawClippedText(info, rcMax, sText, ptWOrg);

		if(m_clrTextOnBar != m_clrTextOnBk)
		{
			// if different colors on bar and background - draw text on gradient
			tc.Select(m_clrTextOnBar);
			DrawClippedText(info, rcBar, sText, ptWOrg);
		}
	}
	pDC->SetViewportOrg(ptVpOrgOld);
}

float CProgress::CalcPos(const CDrawInfo& info)
{
	DWORD dwTextFormat = info.dwStyle&PBS_TEXTMASK;
	if(dwTextFormat == PBS_SHOW_PERCENT)
		// get current position in percents
		return ((float)(info.nCurPos-info.nLower) * 100 / (info.nUpper-info.nLower));
	if(dwTextFormat == PBS_SHOW_POSITION)
		// get absolute value of current position
		return (float)info.nCurPos;
	return (float)0;
}

CString CProgress::FormatText(const CDrawInfo& info, CString sFormat, float nValue)
{
	if(sFormat.IsEmpty())
	{
		// get default format
		DWORD dwTextFormat = info.dwStyle&PBS_TEXTMASK;
		if(dwTextFormat == PBS_SHOW_PERCENT)
			sFormat = "%d%%";
		else if(dwTextFormat == PBS_SHOW_POSITION)
			sFormat = "%d";
		else
			return "";
	}

	CString sText;
	sText.Format(sFormat, (int)nValue);
	return sText;
}

void CProgress::DrawClippedText(const CDrawInfo& info, const CRect& rcClip, CString& sText, const CPoint& ptWndOrg)
{
	CDC *pDC = info.pDC;
	CRgn rgn;
	CRect rc = ConvertToReal(info, rcClip);
	if(rc.IsRectEmpty())
		return;
	rc.OffsetRect(-ptWndOrg);
	rgn.CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);
	pDC->SelectClipRgn(&rgn);
	pDC->TextOut (0, 0, sText);
	rgn.DeleteObject();
}

void CProgress::SetRange(int nLower, int nUpper)
{
	ASSERT(nLower < nUpper);

	if(nLower < nUpper)
	{
		m_nLower = nLower; 
		m_nUpper = nUpper;
	}
	else
	{
		m_nLower = nUpper; 
		m_nUpper = nLower;
	}
	UpdatePos(m_nPos);
}

int CProgress::SetPos(int nPos)
{
	int nOldPos = m_nPos;
	if(SetSnakePos(nPos))
		return nOldPos;

	UpdatePos(nPos);
	return nOldPos;
}

int CProgress::OffsetPos(int nIncrement)
{
	int nOldPos = m_nPos;
	if(SetSnakePos(nIncrement, TRUE))
		return nOldPos;

	UpdatePos(m_nPos + nIncrement);

	return nOldPos;
}

int CProgress::SetStep(int nStep)
{
	int nOldStep = m_nStep;
	m_nStep = nStep;
	return nOldStep;
}

int CProgress::StepIt()
{
	return OffsetPos(m_nStep);
}

/////////////////////////////////////////////////////////////////////////////
// CProgress implementation

BOOL CProgress::SetSnakePos(int nNewPos, BOOL fIncrement)
{
	DWORD dwStyle = GetBarStyle();
	if(!(dwStyle&PBS_SNAKE))
		return FALSE;
	
	int nLower, nUpper;
	GetRange(nLower, nUpper);
	if(fIncrement)
	{
		int nCurPos = GetPos();
		if(nCurPos == nUpper && nCurPos - m_nTail < m_nTailSize )
			nCurPos = m_nTail + m_nTailSize;
		nNewPos = nCurPos + abs(nNewPos);
	}
	if(nNewPos > nUpper+m_nTailSize)
	{
		nNewPos -= nUpper-nLower + m_nTailSize;
		if(nNewPos > nUpper + m_nTailSize)
		{
			ASSERT(0); // too far - reset
			nNewPos = nUpper + m_nTailSize;
		}
		if(dwStyle&PBS_REVERSE)
			ModifyBarStyle(PBS_REVERSE, 0);
		else
			ModifyBarStyle(0, PBS_REVERSE);
	}
//	else if(nNewPos >= nUpper)
//		Invalidate();
	
	m_nTail = nNewPos - m_nTailSize;
	if(m_nTail < nLower)
		m_nTail = nLower;

	UpdatePos(nNewPos);

	return TRUE;
}

void CProgress::SetTextFormat(LPCTSTR szFormat, DWORD ffFormat)
{
	if(!szFormat || !szFormat[0] || !ffFormat)
	{
		ModifyBarStyle(PBS_TEXTMASK, 0);
		SetText("");
	}
	else
	{
		ModifyBarStyle(PBS_TEXTMASK, ffFormat);
		SetText(szFormat);
	}
}

CRect CProgress::ConvertToReal(const CDrawInfo& info, const CRect& rcVirt)
{
	BOOL fReverse = info.dwStyle&PBS_REVERSE;
	BOOL fVert = info.dwStyle&PBS_VERTICAL;

	CRect rc(info.rcClient);
	if(fVert)
	{
		rc.top = info.rcClient.top + 
		         (fReverse ? rcVirt.left : (info.rcClient.Height() - rcVirt.right));
		rc.bottom = rc.top + rcVirt.Width();
	}
	else
	{
		rc.left = info.rcClient.left + 
		          (fReverse ? (info.rcClient.Width() - rcVirt.right) : rcVirt.left);
		rc.right = rc.left + rcVirt.Width();
	}
	return rc;
}

BOOL CProgress::SetShape(HRGN hRgn)
{	
	m_rgnShape.DeleteObject();
	if(!hRgn)
		return TRUE;
	return m_rgnShape.Attach(hRgn);
}

void CProgress::UpdatePos(int nNewPos, BOOL fForse)
{
	if(nNewPos > m_nUpper) nNewPos = m_nUpper;
	if(nNewPos < m_nLower) nNewPos = m_nLower;

	if(m_nPos == nNewPos)
		return;

	BOOL fChange = OnPosChanging(m_nPos, nNewPos);
	if(fForse || fChange)
	{
		m_nPos = nNewPos;
		OnPosChanged(nNewPos);
	}
}
