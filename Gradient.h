#if !defined(AFX_GRADIENT_H__2FB4902E_E00C_4892_AA03_60779F349F58__INCLUDED_)
#define AFX_GRADIENT_H__2FB4902E_E00C_4892_AA03_60779F349F58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

#include <afxtempl.h>

#ifndef UIBITS_API
	#ifdef UIBITS_DLL
		#define  UIBITS_API __declspec(dllexport)
	#else
		#define  UIBITS_API __declspec(dllimport)
	#endif
#endif

struct CGradElement
{
	CGradElement(COLORREF color = 0, int length = 0){this->color = color; this->length = length;}
	COLORREF color;
	int length;
};

typedef CArray<CGradElement, CGradElement&>	CGradArray;

class /*UIBITS_API*/ CGradient  
{
// Construction
public:
	CGradient();
	virtual ~CGradient();

// Attributes
public:
// linear gradient attributes
	enum eDirection{LTR, RTL, TTB, BTT};
	void SetDirection(eDirection fDirection){if(m_fDirection != fDirection){m_fDirection = fDirection;}}
	eDirection GetDirection() {return m_fDirection;}
	void SetStretchGradient(float flStretchFactor = 1); // useful for animation
	float GetStretchGradient() {return m_flStretchGrad;}
// linear gradient operations
	virtual void DrawLinearGradient(CDC *pDC, CRect rcGrad, int nClipStart = 0, int nClipEnd = -1, int nShift = 0);

// attributes
	void SetGradientColors(COLORREF clrStart, COLORREF clrEnd) { SetGradientColorsX(2, clrStart, clrEnd);}
	void GetGradientColors(COLORREF& clrStart, COLORREF& clrEnd); 

	void SetGradientColorsX(int nCount, COLORREF clrFirst, COLORREF clrNext, ...);
	const CDWordArray& GetGradientColorsX() { return m_ardwGradColors; }
	void AddColor(COLORREF clr);
	void SetColorsStretch(double flFirst, ...); // in percent, num of arguments should be one less then num of colors

	void SetCreatePalette(BOOL fCreate = TRUE) {m_fCreatePalette = fCreate;}
	BOOL GetCreatePalette() { return m_fCreatePalette;}
	CPalette& GetPalette() {CreatePalette(); return m_Pal;}

// operations
	virtual void CalcShiftedGradient(CGradArray& arElements, int nShift, int nGradWidth, int nClipStart = 0, int nClipEnd = -1, UINT nMaxColors = (UINT)-1);
	virtual void CalcMultiGradient(CGradArray& arElements, int nGradWidth, int nClipStart = 0, int nClipEnd = -1, UINT nMaxColors = (UINT)-1);
	virtual void CalcGradient(CGradArray& arElements, COLORREF clrStart, COLORREF clrEnd, int nGradWidth, int nClipStart = 0, int nClipEnd = -1, UINT nMaxColors = (UINT)-1);
protected:
	void CreatePalette();
	void NormalizeColorsStretch();

	// color atributes
	CDWordArray m_ardwGradColors;
	BOOL m_fCreatePalette;
	CPalette m_Pal;
private:
	CArray<double, double&> m_arflGradStretch;
protected:
	// linear gradient
	eDirection m_fDirection;
	CRect ConvertToReal(CRect rcDraw, int nBandStart, int nBandEnd);
	float m_flStretchGrad;
};

#endif // !defined(AFX_GRADIENT_H__2FB4902E_E00C_4892_AA03_60779F349F58__INCLUDED_)
