#if !defined(AFX_PROGRESSCTRLX_H__F26E8924_D1C1_11D3_B261_00104BB13A66__INCLUDED_)
#define AFX_PROGRESSCTRLX_H__F26E8924_D1C1_11D3_B261_00104BB13A66__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

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

#ifndef UIBITS_API
	#ifdef UIBITS_DLL
		#define  UIBITS_API __declspec(dllexport)
	#else
		#define  UIBITS_API __declspec(dllimport)
	#endif
#endif

#include "Progress.h"

/////////////////////////////////////////////////////////////////////////////
// class CProgressCtrlX

class /*UIBITS_API*/ CProgressCtrlX : public CProgressCtrl, public CProgress
{
// Construction
public:
	CProgressCtrlX();

// Attributes
public:
	void SetTooltipFormat (LPCTSTR lpszToolTipText, DWORD fFormat = PBS_SHOW_TEXTONLY);
	LPCTSTR GetTooltipText() { return m_sToolTipText;}
	void HideTooltip() {SetTooltipFormat(0);}

	int SetPos(int nPos) {int res = CProgress::SetPos(nPos); Invalidate(); return res;}
	int GetPos() {return CProgress::GetPos();}
	
	int SetStep(int nStep){int res = CProgress::SetStep(nStep); Invalidate(); return res;}

	void SetRange(int nLower, int nUpper) {CProgress::SetRange(nLower, nUpper); Invalidate();}
	void GetRange(int& nLower, int& nUpper) {CProgress::GetRange(nLower, nUpper);}

	int OffsetPos(int nIncrement) {int res = CProgress::OffsetPos(nIncrement); Invalidate(); return res;}

	void SetFont(CFont* pFont) {CProgress::SetFont(pFont); Invalidate();}
	CFont* GetFont() {return CProgress::GetFont();}

	virtual DWORD GetBarStyle() {return CProgressCtrl::GetStyle();}
	virtual void ModifyBarStyle(DWORD dwRemove, DWORD dwAdd) {CProgressCtrl::ModifyStyle(dwRemove, dwAdd); Invalidate();}
// Operations
public:
	int StepIt() {int res = CProgress::StepIt(); Invalidate(); return res;}
	
	void RunSnake(int nMsPerStep) {ASSERT(GetSnake()); StopSnake(); SetTimer(SnakeTimer, nMsPerStep, NULL);}
	void StopSnake() {KillTimer(SnakeTimer);}

	void RunAnimation(int nMsPerStep, int nStep) {m_nAnimStep = nStep; StopAnimation(); SetTimer(AnimationTimer, nMsPerStep, NULL);}
	void StopAnimation() {KillTimer(AnimationTimer);}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressCtrlX)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CProgressCtrlX(){}

	// Generated message map functions
protected:
	enum {SnakeTimer = 11, AnimationTimer = 12};
	CToolTipCtrl m_wndToolTip;
	CString m_sToolTipText;
	DWORD m_fToolTipFormat;
	int m_nAnimStep;
	virtual void OnPosChanged(int nNewPos){ UpdateTooltipText(nNewPos);}
	void UpdateTooltipText(int nPos);

	//{{AFX_MSG(CProgressCtrlX)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg LRESULT OnSetBarColor(WPARAM, LPARAM);
	afx_msg LRESULT OnSetBkColor(WPARAM, LPARAM);
	afx_msg LRESULT OnGetPos(WPARAM, LPARAM);
	afx_msg LRESULT OnSetPos(WPARAM, LPARAM);
	afx_msg LRESULT OnDeltaPos(WPARAM, LPARAM);
	afx_msg LRESULT OnStepIt(WPARAM, LPARAM);
	afx_msg LRESULT OnSetStep(WPARAM, LPARAM);
	afx_msg LRESULT OnGetRange(WPARAM, LPARAM);
	afx_msg LRESULT OnSetRange(WPARAM, LPARAM);
	afx_msg LRESULT OnSetRange32(WPARAM, LPARAM);
	afx_msg LRESULT OnSetText(WPARAM, LPARAM);
	afx_msg LRESULT OnGetText(WPARAM, LPARAM);
	afx_msg LRESULT OnGetTextLength(WPARAM, LPARAM);
	afx_msg LRESULT OnGetFont(WPARAM, LPARAM);
	afx_msg LRESULT OnSetFont(WPARAM, LPARAM);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSCTRLX_H__F26E8924_D1C1_11D3_B261_00104BB13A66__INCLUDED_)
