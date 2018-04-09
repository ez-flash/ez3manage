#if !defined(AFX_PROGRESS_H__F26E8924_D1C1_11D3_B261_00104BB13A66__INCLUDED_)
#define AFX_PROGRESS_H__F26E8924_D1C1_11D3_B261_00104BB13A66__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

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

#ifndef UIBITS_API
	#ifdef UIBITS_DLL
		#define  UIBITS_API __declspec(dllexport)
	#else
		#define  UIBITS_API __declspec(dllimport)
	#endif
#endif

#include "Gradient.h"

// To set text alignment styles use ModifyBarStyle()
#define PBS_LEFT                 0x0010L
#define PBS_RIGHT                0x0020L
#define PBS_CENTER               0x0030L
#define PBS_TOP                  0x0040L
#define PBS_BOTTOM               0x0080L
#define PBS_VCENTER              0x00C0L

// To set text format use "SetTextFormat" and "HideText"
#define PBS_SHOW_PERCENT         0x0100L
#define PBS_SHOW_POSITION        0x0200L
#define PBS_SHOW_TEXTONLY        0x0300L
#define PBS_TEXTMASK             0x0300L

// To set this attributes use ModifyBarStyle() or appropriated functions
#define PBS_TIED_TEXT            0x1000L
#define PBS_RUBBER_BAR           0x2000L
#define PBS_REVERSE              0x4000L
#define PBS_SNAKE                0x8000L

/////////////////////////////////////////////////////////////////////////////
// class CProgress

class /*UIBITS_API*/ CProgress : public CGradient
{
// Construction
public:
	CProgress();

// Attributes
public:	
	void SetBarBrush(CBrush* pbrBar) { m_pbrBar = pbrBar; }
	CBrush* GetBarBrush() { return m_pbrBar; }

	void SetBkColor(COLORREF clrBk) { m_clrBk = clrBk; }
	COLORREF GetBkColor() { return m_clrBk; }

	void SetBkBrush(CBrush* pbrBk) { m_pbrBk = pbrBk; }
	CBrush* GetBkBrush() { return m_pbrBk; }

	void SetTextColor(COLORREF clrTextOnBar, COLORREF clrTextOnBk = -1) { m_clrTextOnBar = m_clrTextOnBk = clrTextOnBar; if(clrTextOnBk != -1) m_clrTextOnBk = clrTextOnBk;}
	COLORREF GetTextColor() { return m_clrTextOnBar; }
	COLORREF GetTextColorOnBk() { return m_clrTextOnBk; }

	void SetShowPercent(BOOL fShowPercent = TRUE) { SetTextFormat(fShowPercent ? "%d%%" : NULL, PBS_SHOW_PERCENT); }
	BOOL GetShowPercent() { return GetBarStyle()&PBS_SHOW_PERCENT; }

	void SetTextAlign(DWORD dwStyle) {ModifyBarStyle(PBS_CENTER|PBS_VCENTER, dwStyle&(PBS_CENTER|PBS_VCENTER));}
	DWORD GetTextAlign() {return GetBarStyle()&(PBS_CENTER|PBS_VCENTER); }

	void SetTextFormat(LPCTSTR szFormat, DWORD ffFormat = PBS_SHOW_TEXTONLY);
	void HideText() {SetTextFormat(0);}

	void SetText(LPCTSTR lpszText) {m_sText = lpszText;}
	CString GetText() {return m_sText;}

	void SetTiedText(BOOL fTiedText = TRUE) { ModifyBarStyle(fTiedText ? 0 : PBS_TIED_TEXT, fTiedText ? PBS_TIED_TEXT : 0); }
	BOOL GetTiedText() { return GetBarStyle()&PBS_TIED_TEXT; }

	void SetRubberBar(BOOL fRubberBar = TRUE) { ModifyBarStyle(fRubberBar ? 0 : PBS_RUBBER_BAR, fRubberBar ? PBS_RUBBER_BAR : 0); }
	BOOL GetRubberBar() { return GetBarStyle()&PBS_RUBBER_BAR; }

	void SetReverse(BOOL fReverse = TRUE) { ModifyBarStyle(fReverse ? 0 : PBS_REVERSE, fReverse ? PBS_REVERSE : 0); }
	BOOL GetReverse() { return GetBarStyle()&PBS_REVERSE; }

	void SetSnake(BOOL fSnake = TRUE) { ModifyBarStyle(fSnake ? 0 : PBS_SNAKE|PBS_RUBBER_BAR, fSnake ? PBS_SNAKE|PBS_RUBBER_BAR : 0); }
	BOOL GetSnake() { return GetBarStyle()&PBS_SNAKE; }

	void SetSnakeTail(int nTailSize) { m_nTailSize = nTailSize; }
	int  GetSnakeTail() { return m_nTailSize; }

	void SetBorders(const CRect& rcBorders) { m_rcBorders = rcBorders; }
	const CRect& GetBorders() { return m_rcBorders; }

	BOOL SetShape(HRGN hRgn);

	void SetRange(int nLower, int nUpper);
	void GetRange(int& nLower, int& nUpper) {nLower = m_nLower; nUpper = m_nUpper;}

	int GetPos() {return m_nPos;}
	int SetPos(int nPos);

	int OffsetPos(int nIncrement);

	int SetStep(int nStep);

	void SetFont(CFont* pFont) {m_pFont = pFont;}
	CFont* GetFont() {return m_pFont;}

	virtual DWORD GetBarStyle() {return m_dwStyle;}
	virtual void ModifyBarStyle(DWORD dwRemove, DWORD dwAdd) {m_dwStyle &= ~dwRemove; m_dwStyle |= dwAdd;}

	void Animate(int nStep) {m_nAnimPos += nStep; m_nAnimPos %= int((m_nUpper - m_nLower)*GetStretchGradient());}
	void ResetAnimation() {m_nAnimPos = 0;}

// Operations
public:
	int StepIt();
	void Draw(CDC* pDC, CRect rcDraw, BOOL fSkipDCCache = FALSE);
	
// Implementation
public:
	virtual ~CProgress(){}

protected:
	struct CDrawInfo
	{
		CDC *pDC;
		DWORD dwStyle;
		CRect rcClient;
		int nCurPos;
		int nLower;
		int nUpper;
	};
	
	virtual void DrawText(const CDrawInfo& info, const CRect &rcMax, const CRect &rcGrad);
	virtual void DrawClippedText(const CDrawInfo& info, const CRect& rcClip, CString& sText, const CPoint& ptWndOrg);
	CRect ConvertToReal(const CDrawInfo& info, const CRect& rcVirt);
	virtual BOOL SetSnakePos(int nNewPos, BOOL fIncrement = FALSE);
	virtual CString FormatText(const CDrawInfo& info, CString sFormat, float nValue);
	virtual float CalcPos(const CDrawInfo& info);
	void UpdatePos(int nNewPos, BOOL fForse = FALSE);
	virtual BOOL OnPosChanging(int nOldPos, int nNewPos) {return TRUE;}
	virtual void OnPosChanged(int nNewPos) {}

	// color atributes
	CBrush* m_pbrBar; 
	COLORREF m_clrBk;
	CBrush* m_pbrBk;
	COLORREF m_clrTextOnBar;
	COLORREF m_clrTextOnBk;

	// snake/progress attributes
	int m_nTail;
	int m_nTailSize;
	int m_nStep;
	int m_nLower;
	int m_nUpper;

	CRect m_rcBorders;
	CString m_sText;
	CFont* m_pFont;
	DWORD m_dwStyle;
	CRgn m_rgnShape;
private:
	int m_nPos;
	int m_nAnimPos;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESS_H__F26E8924_D1C1_11D3_B261_00104BB13A66__INCLUDED_)
