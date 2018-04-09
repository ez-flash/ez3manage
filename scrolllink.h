/////////////////////////////////////////////////////////////////////////////
//	Class :		CScrollLink
//	Version : 1.0
//
//	Author :	Weiye Chen
//  Email :		weiye@asiamail.com
//
//	Disclaimer
//	----------
//	THIS SOFTWARE AND THE ACCOMPANYING FILES ARE DISTRIBUTED "AS IS" AND WITHOUT
//	ANY WARRANTIES WHETHER EXPRESSED OR IMPLIED. NO REPONSIBILITIES FOR POSSIBLE
//	DAMAGES OR EVEN FUNCTIONALITY CAN BE TAKEN. THE USER MUST ASSUME THE ENTIRE
//	RISK OF USING THIS SOFTWARE.
//
//	Terms of use
//	------------
//	THIS SOFTWARE IS FREE.

#if !defined(AFX_SCROLLLINK_H__10BD0543_310C_11D6_887D_00002126253D__INCLUDED_)
#define AFX_SCROLLLINK_H__10BD0543_310C_11D6_887D_00002126253D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScrollLink.h : header file
//
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CScrollLink window

class CScrollLink : public CStatic
{
// Construction
public:

	// Constructor
	CScrollLink();

// Attributes
public:

	CObArray m_oTextFrames; // Array of CTextFrame objects

	COLORREF m_clrText; // Text color
	COLORREF m_clrHover; // On mouse hover color
	COLORREF m_clrBackground; // Control's background color

// Operations
public:

	// Start scrolling text
	BOOL StartScrolling(BOOL bRestart = TRUE);

	// Add scrolling text and its url
	BOOL AddScrollText(CString, CString);

	// Remove all previously added scroll text
	BOOL RemoveAllScrollText(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScrollLink)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:

	// Destructor
	virtual ~CScrollLink();

	// Generated message map functions
protected:
	//{{AFX_MSG(CScrollLink)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClicked();
	//}}AFX_MSG

	// Handler for WM_MOUSELEAVE
	LRESULT OnMouseLeave(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()

// Operations
private:

	// Create CFont objects for text
	void CreateTextFonts(void);

	// Delete CTextFrame objects
	void DeleteTextFrames(void);

	// Calculate window and text area coordinates
	void CalculateWindowAndTextArea(void);

	// Initialize size of text and its starting position
	void PrepareTextFramesForScrolling(void);

	// Set new position for current scrolling texts based on scroll speed
	void IncrementScrollingTextFramesPosition(void);

	// Set the clipping region so that text not within it will not be drawn 
	void SetClippingRegion(CDC*);

	// Draw scrolling text into text area
	void DrawScrollingText(CDC*);

	// Add index of new scrolling text if allowance satisfied
	void AddOnNewScrollingText(void);

	// Remove index of finished scrolling text if it runs out of view
	void RemoveFinishedScrollingText(void);

	// Check the number of same element previously in array
	int NumOfSameElement(CArray<int, int>&, int);
	
	// Get index of CTextFrame object in array that is being hovered
	int IsTextBeingHovered(CPoint, int*);
	
// Attributes
private:

	UINT m_uiScrollRate; // Timer value for scrolling
	UINT m_uiScrollSpeed; // Amount of pixels to scroll
	UINT m_uiScrollAllowance; // Amout of pixels between 2 text scroll

	CRect m_oTextArea; // Coordinates of text area
	CRect m_oWindowArea; // Coordinates of window area

	CFont m_oNormalFont; // Pointer to current font of control
	CFont m_oUnderlinedFont; // Pointer to current font of control with underlined attribute
	
	/* --- For internal user --- */

	int m_iHoveredTextFrame; // Index of current text frame that is being hovered

	BOOL m_bOnHover; // Flag to indicate mouse is hovering over text

	CArray<int, int> m_iTextFramesScrolling; // Indexes of current scrolling text frame objects
};

/////////////////////////////////////////////////////////////////////////////

class CTextFrame : public CObject
{
// Operations
public:

	// Constructors
	CTextFrame();
	CTextFrame(CString ,CString);

	// Destructor
	virtual ~CTextFrame();

// Attrbutes
public:

	CSize m_szTextSize; // Size of text

	CString m_strURL; // URL behind text;
	CString m_strText; // Text to scroll

	CArray<int, int> m_iXOffSets; // X coordinates of text frames currently showing
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCROLLLINK_H__10BD0543_310C_11D6_887D_00002126253D__INCLUDED_)
