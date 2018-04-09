///////////////////////////////////////////////////////////////////////////////
// class CProgressCtrlX
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
// Most code separated to window independent base classes 
//    (maximum compatibility with previous version)
// Added support for ToolTips
// Added self running snake
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
#include "ProgressCtrlX.h"

#include "MemDC.h"
#include "DrawGdiX.h"

#ifdef PBT_ANY_ANGLE
	#include <math.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressCtrlX

CProgressCtrlX::CProgressCtrlX()
{
	m_fToolTipFormat = 0;
}

BEGIN_MESSAGE_MAP(CProgressCtrlX, CProgressCtrl)
	//{{AFX_MSG_MAP(CProgressCtrlX)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_MESSAGE(PBM_SETBARCOLOR, OnSetBarColor)
	ON_MESSAGE(PBM_SETBKCOLOR, OnSetBkColor)
	ON_MESSAGE(PBM_GETPOS, OnGetPos)
	ON_MESSAGE(PBM_SETPOS, OnSetPos)
	ON_MESSAGE(PBM_DELTAPOS, OnDeltaPos)
	ON_MESSAGE(PBM_STEPIT, OnStepIt)
	ON_MESSAGE(PBM_SETSTEP, OnSetStep)
	ON_MESSAGE(PBM_GETRANGE, OnGetRange)
	ON_MESSAGE(PBM_SETRANGE, OnSetRange)
	ON_MESSAGE(PBM_SETRANGE32, OnSetRange32)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_MESSAGE(WM_GETTEXT, OnGetText)
	ON_MESSAGE(WM_GETTEXTLENGTH, OnGetTextLength)
	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressCtrlX message handlers

BOOL CProgressCtrlX::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE; // erase in OnPaint()
}

void CProgressCtrlX::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(&rc);

	CPalette *pOldPal = NULL;
	if (GetCreatePalette() && GetPalette().GetSafeHandle())
	{
		pOldPal = dc.SelectPalette(&GetPalette(), FALSE);
		dc.RealizePalette();
	}

	Draw(&dc, rc);

	if(pOldPal)
		dc.SelectPalette(pOldPal, TRUE);
	// Do not call CProgressCtrl::OnPaint() for painting messages
}

LRESULT CProgressCtrlX::OnSetBarColor(WPARAM clrEnd, LPARAM clrStart)
{
	SetGradientColors(clrStart, clrEnd ? clrEnd : clrStart);

	return CLR_DEFAULT;
}

LRESULT CProgressCtrlX::OnSetBkColor(WPARAM, LPARAM clrBk)
{
	SetBkColor(clrBk);
	return CLR_DEFAULT;
}

LRESULT CProgressCtrlX::OnSetStep(WPARAM nStepInc, LPARAM)
{
	int res = CProgress::SetStep(nStepInc);
	Invalidate();
	return res;
}

LRESULT CProgressCtrlX::OnGetPos(WPARAM, LPARAM)
{
	return CProgress::GetPos();
}

LRESULT CProgressCtrlX::OnSetPos(WPARAM newPos, LPARAM)
{
	int res = CProgress::SetPos(newPos);
	Invalidate();
	return res;
}

LRESULT CProgressCtrlX::OnGetRange(WPARAM wParam, LPARAM lParam)
{
	int Low, Hi;
	CProgress::GetRange(Low, Hi);
	if(lParam)
	{
		PPBRANGE ppBRange = (PPBRANGE)lParam;
		ppBRange->iHigh = Hi;
		ppBRange->iLow = Low;
	}
	return wParam ? Low : Hi;
}

LRESULT CProgressCtrlX::OnSetRange(WPARAM, LPARAM lParam)
{
	return OnSetRange32(LOWORD(lParam), HIWORD(lParam));
}

LRESULT CProgressCtrlX::OnSetRange32(WPARAM low, LPARAM hi)
{
	int oldLow, oldHi;
	CProgress::GetRange(oldLow, oldHi);
	CProgress::SetRange(low, hi);
	Invalidate();
	return MAKELRESULT(oldLow, oldHi);
}

LRESULT CProgressCtrlX::OnDeltaPos(WPARAM nIncrement, LPARAM)
{
	int res = CProgress::OffsetPos(nIncrement);
	Invalidate();
	return res;
}

LRESULT CProgressCtrlX::OnStepIt(WPARAM, LPARAM)
{
	int res = CProgress::StepIt();
	Invalidate();
	return res;
}

LRESULT CProgressCtrlX::OnSetText(WPARAM, LPARAM lParam)
{
	CProgress::SetText((LPCTSTR)lParam);
	Invalidate();
	return TRUE;
}

LRESULT CProgressCtrlX::OnGetFont(WPARAM, LPARAM)
{
	CFont* pFont = CProgress::GetFont();
	if(!pFont)
		return NULL;
	return (LRESULT)pFont->GetSafeHandle();
}

LRESULT CProgressCtrlX::OnSetFont(WPARAM hFont, LPARAM lParam)
{
	CProgress::SetFont(CFont::FromHandle((HFONT)hFont));
	if(LOWORD(lParam))
		RedrawWindow();
	return 0;
}

LRESULT CProgressCtrlX::OnGetText(WPARAM nCount, LPARAM pBuf)
{
	CString sText = CProgress::GetText();
	_tcsncpy((TCHAR* )pBuf, sText, nCount);
	return min(nCount, (UINT)sText.GetLength());
}

LRESULT CProgressCtrlX::OnGetTextLength(WPARAM, LPARAM)
{
	return CProgress::GetText().GetLength();
}

void CProgressCtrlX::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == SnakeTimer)
		StepIt();
	else if(nIDEvent == AnimationTimer)
	{
		Animate(m_nAnimStep);
		Invalidate();
	}

	CProgressCtrl::OnTimer(nIDEvent);
}

BOOL CProgressCtrlX::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_wndToolTip.GetSafeHwnd () != NULL)
	{
		if (pMsg->message == WM_MOUSEMOVE)
		{
			m_wndToolTip.RelayEvent(pMsg);
		}
	}
	
	return CProgressCtrl::PreTranslateMessage(pMsg);
}

void CProgressCtrlX::SetTooltipFormat (LPCTSTR lpszToolTipText, DWORD fFormat)
{
	ASSERT (GetSafeHwnd () != NULL);

	m_sToolTipText = lpszToolTipText;
	m_fToolTipFormat = (fFormat & PBS_TEXTMASK);
	if (lpszToolTipText == NULL || lpszToolTipText[0] == 0 || !m_fToolTipFormat)
	{
		if (m_wndToolTip.GetSafeHwnd () != NULL)
			m_wndToolTip.Activate (FALSE);
	}
	else
	{
		if (m_wndToolTip.GetSafeHwnd () == NULL)
		{
			m_wndToolTip.Create (this, TTS_ALWAYSTIP|TTS_NOPREFIX);
			m_wndToolTip.AddTool (this, "");
			UpdateTooltipText(CProgress::GetPos());
		}

		ASSERT (m_wndToolTip.GetSafeHwnd () != NULL);
		m_wndToolTip.Activate (TRUE);
	}
}

void CProgressCtrlX::UpdateTooltipText(int nPos)
{
	if (m_wndToolTip.GetSafeHwnd () == NULL)
		return;
	CDrawInfo info;
	// retrieve current position and range
	info.nCurPos = nPos;
	CProgress::GetRange(info.nLower, info.nUpper);
	info.dwStyle = m_fToolTipFormat;
	CString sText = FormatText(info, GetTooltipText(), CalcPos(info));
	if (!sText.IsEmpty())
		m_wndToolTip.UpdateTipText(sText, this);
}