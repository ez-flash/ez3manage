// ScrollLink.cpp : implementation file
//

#include "stdafx.h"
#include "ScrollLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Constant definitions

#define CLR_RED RGB(255, 0, 0) // Red color
#define CLR_BLUE RGB(0, 0, 255) // Blue color
#define CLR_BLACK RGB(0, 0, 0) // Black color
#define CLR_GRAY RGB(178, 178, 178) // Gray color
#define CLR_WHITE RGB(255, 255, 255) // White color

#define SCROLLRATE 25 // Frequency of timer
#define SCROLLSPEED 1 // Amount of pixels to scroll
#define SCROLLALLOWANCE 50 // Amount of pixels between 2 scroll text

#define ID_SCROLLTIMER (WM_USER + 1001) // ID for scroll timer

/////////////////////////////////////////////////////////////////////////////
// CScrollLink

CScrollLink::CScrollLink()
{
	// Empty array
	m_oTextFrames.RemoveAll();
	m_iTextFramesScrolling.RemoveAll();

	// Scroll rate is the value used in SetTimer(), which scrolls your hyperlinks to
	// the left by x pixels, where x is the scroll speed value. Scroll allowance is the
	// gap in pixels between 2 scrolling hyperlinks
	m_uiScrollRate = SCROLLRATE;
	m_uiScrollSpeed = SCROLLSPEED;
	m_uiScrollAllowance = SCROLLALLOWANCE;

	// The text of the hyperlink will be black if there is no url for it. If not, it will
	// be blue. The color of the hyperlink is set to red when mouse it is hovered and the
	// background color of the control is set to the same as the button color.
	m_clrText = CLR_BLACK;
	m_clrHover = CLR_RED;
	m_clrBackground = GetSysColor(COLOR_BTNFACE);

	// Set flag
	m_bOnHover = FALSE;
}

CScrollLink::~CScrollLink()
{
	// Delete CTextFrame objects
	DeleteTextFrames();
}


BEGIN_MESSAGE_MAP(CScrollLink, CStatic)
	//{{AFX_MSG_MAP(CScrollLink)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Create CFont objects for text

void CScrollLink::CreateTextFonts(void)
{
	// Get window font
	CFont* pFont = GetFont();
	
	// Create LOGFONT structure
	LOGFONT lfLogFont;

	// Get LOGFONT structure of current font
	pFont->GetLogFont(&lfLogFont);

	// Set font to be bold
	lfLogFont.lfWeight = FW_BLACK;

	// Create normal font that is bold (when not hovered)
	m_oNormalFont.CreateFontIndirect(&lfLogFont);
	
	// Set underline attribute
	lfLogFont.lfUnderline = TRUE;//设置下划线的属性

	// Create current font with underline attribute (when hovered)
	m_oUnderlinedFont.CreateFontIndirect(&lfLogFont);
}

/////////////////////////////////////////////////////////////////////////////
// Delete CTextFrame objects for scroll text

void CScrollLink::DeleteTextFrames(void)
{
	// Loop through every object pointers in array
	for (int i = 0; i < m_oTextFrames.GetSize(); i ++)
	{
		// Check if pointer is valid
		if (m_oTextFrames.GetAt(i) != NULL)
		{
			// Free memory of object
			delete m_oTextFrames.GetAt(i);

			// Assign null pointer value
			m_oTextFrames.SetAt(i, NULL);
		}
	}

	// Empty array
	m_oTextFrames.RemoveAll();
	m_iTextFramesScrolling.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
// Calculate window and text area coordinates

void CScrollLink::CalculateWindowAndTextArea(void)
{
	// Get window coordinates of control
	GetWindowRect(&m_oWindowArea);

	// Convert coordinates with respect to screen
	ScreenToClient(&m_oWindowArea);

	// Duplicate window area for calculation purposes
	m_oTextArea = m_oWindowArea;

	// Deflate area by 2 pixels
	m_oTextArea.DeflateRect(2, 2);
}

/////////////////////////////////////////////////////////////////////////////
// Initialize size of text and its starting position

void CScrollLink::PrepareTextFramesForScrolling(void)
{
	// Create a CClientDC object
	CClientDC oClientDC(this);

	// Select our created font into device context
	CFont* pOldFont = oClientDC.SelectObject(&m_oNormalFont);

	// Loop through every object pointers in array
	for (int i = 0; i < m_oTextFrames.GetSize(); i ++)
	{
		// Get pointer to current looping object
		CTextFrame* pTextFrame = (CTextFrame*)m_oTextFrames.GetAt(i);

		// Empty array
		pTextFrame->m_iXOffSets.RemoveAll();

		// Calculate and set size of text to be drawn based on current device context
		pTextFrame->m_szTextSize = oClientDC.GetTextExtent(pTextFrame->m_strText);
	}

	// Restore original font
	oClientDC.SelectObject(pOldFont);

	// Check if there are at least 1 text frame in array
	if (m_oTextFrames.GetSize() > 0)
	{
		// Set starting position in pixels for first frame to start ball rolling
		((CTextFrame*)m_oTextFrames.GetAt(0))->m_iXOffSets.Add(m_oTextArea.right);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Set new position for current scrolling texts based on scroll speed

void CScrollLink::IncrementScrollingTextFramesPosition(void)
{
	// Make sure there are text to scroll
	if (m_oTextFrames.GetSize() > 0)
	{
		// Loop through the indexes of current scrolling text
		for (int i = 0; i < m_iTextFramesScrolling.GetSize(); i ++)
		{
			// Get index of scrolling text frame object
			int iIndex = m_iTextFramesScrolling.GetAt(i);

			// Get pointer to the text frame object
			CTextFrame* pTextFrame = (CTextFrame*)m_oTextFrames.GetAt(iIndex);

			// Check for the number of same element if array until current position, excluding
			int iXOffSetIndex = NumOfSameElement(m_iTextFramesScrolling, i);

			// Get and calcualte new x offset value
			int iNewXOffSet = pTextFrame->m_iXOffSets.GetAt(iXOffSetIndex) - SCROLLSPEED;

			// Increment position of scrolling text
			pTextFrame->m_iXOffSets.SetAt(iXOffSetIndex, iNewXOffSet);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Set the clipping region so that text not within it will not be drawn 

void CScrollLink::SetClippingRegion(CDC* pDC /* Pointer to CDC object */)
{
	// Create CRgn object
	CRgn oClipRgn;

	// Create clip region for text area
	oClipRgn.CreateRectRgnIndirect(&m_oTextArea);

	// Set clip region
	pDC->SelectClipRgn(&oClipRgn);
}

/////////////////////////////////////////////////////////////////////////////
// Draw scrolling text into text area

void CScrollLink::DrawScrollingText(CDC* pDC /* Pointer to CDC object */)
{
	// Make sure pointer is valid
	if (pDC != NULL)
	{
		// Loop through the indexes of text frame objects to draw
		for (int i = 0; i < m_iTextFramesScrolling.GetSize(); i ++)
		{
			// Get index of scrolling text frame object
			int iIndex = m_iTextFramesScrolling.GetAt(i);

			// Get pointer to the text frame object
			CTextFrame* pTextFrame = (CTextFrame*)m_oTextFrames.GetAt(iIndex);

			// Check for the number of same element if array until current position, excluding
			int iXOffSetIndex = NumOfSameElement(m_iTextFramesScrolling, i);

			// Calculate a rectangular coordinates to draw scroll text
			CRect oDrawingArea(pTextFrame->m_iXOffSets.GetAt(iXOffSetIndex), m_oTextArea.top, m_oTextArea.right, m_oTextArea.bottom);

			// Check if text has no associate url, then draw it as black
			if (pTextFrame->m_strURL.IsEmpty() != FALSE)
			{
				// Set black color text
				pDC->SetTextColor(CLR_BLACK);
			}

			// Text has an associate url, so draw as blue
			else
			{
				// Set blue color text
				pDC->SetTextColor(CLR_BLUE);
			}

			// Draw scroll text
			pDC->DrawText(pTextFrame->m_strText, &oDrawingArea, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);

			// Calculate a rectangular coordinates to invalidate previous drawn text
			CRect oInvalidateArea(pTextFrame->m_iXOffSets.GetAt(iXOffSetIndex) + pTextFrame->m_szTextSize.cx, m_oTextArea.top,
								  pTextFrame->m_iXOffSets.GetAt(iXOffSetIndex) + pTextFrame->m_szTextSize.cx + SCROLLSPEED, m_oTextArea.bottom);

			// Repaint last drawn area
			InvalidateRect(&oInvalidateArea);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Add index of new scrolling text if allowance satisfied

void CScrollLink::AddOnNewScrollingText(void)
{
	// Make sure there are text to scroll and there must be at least
	// one text scrolling
	if (m_oTextFrames.GetSize() > 0 && m_iTextFramesScrolling.GetSize() > 0)
	{
		// Get index of last scrolling text frame object index
		int iLastScrolling = m_iTextFramesScrolling.GetSize() - 1;

		// Get index of last scrolling text frame object
		int iIndex = m_iTextFramesScrolling.GetAt(iLastScrolling);
	
		// Get pointer to the text frame object
		CTextFrame* pTextFrame = (CTextFrame*)m_oTextFrames.GetAt(iIndex);

		// Check if there is enough room to scroll another text behind it
		if ((pTextFrame->m_iXOffSets.GetAt(pTextFrame->m_iXOffSets.GetSize() - 1) + pTextFrame->m_szTextSize.cx + SCROLLALLOWANCE) < m_oTextArea.right)
		{
			// Set new index for next scrolling text frame object in array
			iIndex ++;

			// Check if index is not within array size
			if (iIndex >= m_oTextFrames.GetSize())
			{
				// Reset index to start
				iIndex = 0;
			}

			// Get pointer to the next text frame object
			CTextFrame* pTextFrame = (CTextFrame*)m_oTextFrames.GetAt(iIndex);

			// Set starting position in pixels for scrolling
			pTextFrame->m_iXOffSets.Add(m_oTextArea.right);

			// Add new text frame index into array
			m_iTextFramesScrolling.Add(iIndex);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Remove index of finished scrolling text if it runs out of view

void CScrollLink::RemoveFinishedScrollingText(void)
{
	// Make sure there are text to scroll and indexes of text frame objects are there
	if (m_oTextFrames.GetSize() > 0 && m_iTextFramesScrolling.GetSize() > 0)
	{
		// Get index of first scrolling text frame object
		int iIndex = m_iTextFramesScrolling.GetAt(0);

		// Get pointer to the text frame object
		CTextFrame* pTextFrame = (CTextFrame*)m_oTextFrames.GetAt(iIndex);

		// Check if the first text frame has finished scrolling
		if ((pTextFrame->m_iXOffSets.GetAt(0) + pTextFrame->m_szTextSize.cx) <= m_oTextArea.left)
		{
			// Remove x offset from array
			pTextFrame->m_iXOffSets.RemoveAt(0);

			// Remove its index from array
			m_iTextFramesScrolling.RemoveAt(0);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Check the number of same element previously in array

int CScrollLink::NumOfSameElement(CArray<int, int>& iArray /* Reference to array */,
								  int iIndexToCheck /* Index of array item to be check against */)
{
	// Counter to keep track of number of same element
	int iCount = 0;

	// Get value of array content at index position
	int iValue = iArray.GetAt(iIndexToCheck);

	// Loop through every element in array until indexed position (excluding)
	for (int i = 0; i < iIndexToCheck; i ++)
	{
		// Check if element match the value to check
		if (iArray.GetAt(i) == iValue)
		{
			// Increment counter
			iCount ++;
		}
	}

	// Return counter value
	return iCount;
}

/////////////////////////////////////////////////////////////////////////////
// Get index of CTextFrame object in array that is being hovered

int CScrollLink::IsTextBeingHovered(CPoint oCursorPos /* CPoint object containing coordinates of cursor */,
									int* ipXOffSet /* X coordinate of the text frame that is being hovered */)
{
	// Make sure pointer is valid
	if (ipXOffSet != NULL)
	{
		// Loop through every text frame indexes in array
		for (int i = 0; i < m_iTextFramesScrolling.GetSize(); i ++)
		{
			// Get index of scrolling text frame object
			int iIndex = m_iTextFramesScrolling.GetAt(i);

			// Get pointer to the text frame object
			CTextFrame* pTextFrame = (CTextFrame*)m_oTextFrames.GetAt(iIndex);

			for (int x = 0; x < pTextFrame->m_iXOffSets.GetSize(); x ++)
			{
				// Calculate left and right x coordinate of text
				int iLeftXOffSet = pTextFrame->m_iXOffSets.GetAt(x);
				int iRightXOffSet = pTextFrame->m_iXOffSets.GetAt(x) + pTextFrame->m_szTextSize.cx;

				// Calculate starting y coordinate of scroll text
				int iYPos = (m_oTextArea.Height() - pTextFrame->m_szTextSize.cy) / 2;

				// Check if text is left partially out of area
				if (iLeftXOffSet < m_oTextArea.left)
				{
					// Set left x offset value for later formation of rect coordinates
					iLeftXOffSet = m_oTextArea.left;
				}

				// Check if text is right partially out of area
				if (iRightXOffSet > m_oTextArea.right)
				{
					// Set right x offset value for later formation of rect coordinates
					iRightXOffSet = m_oTextArea.right;
				}

				// Calculate a rectangular coordinates to represent the bounding rect around scroll text
				CRect oScrollTextArea(iLeftXOffSet, iYPos, iRightXOffSet, iYPos + pTextFrame->m_szTextSize.cy);

				// Check if cursor over text
				if (oScrollTextArea.PtInRect(oCursorPos) != FALSE)
				{
					// Set x coordinate of text frame
					*ipXOffSet = pTextFrame->m_iXOffSets.GetAt(x);

					// Return index of text frame object in array
					return iIndex;
				}
			}
		}
	}

	// Cursor not on scroll text
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// Start scrolling text

BOOL CScrollLink::StartScrolling(BOOL bRestart /* Flag to indicate whether to restart scrolling */)
{
	// Make sure there are text to scroll
	if (m_oTextFrames.GetSize() > 0)
	{
		// Stop timer if any
		KillTimer(ID_SCROLLTIMER);

		// Check if scrolling is to be restarted
		if (bRestart == TRUE)
		{
			// Empty array of indexes of current scrolling text frames
			m_iTextFramesScrolling.RemoveAll();

			// Store index of the first text frame to be scrolling
			m_iTextFramesScrolling.Add(0);

			// Initialize size of text and its starting position
			PrepareTextFramesForScrolling();

			// Clear any previous scroll text
			Invalidate();
		}

		// Scrolling is to be continued
		else
		{
		}

		// Start timer
		SetTimer(ID_SCROLLTIMER, SCROLLRATE, NULL);

		// Indicate success
		return TRUE;
	}

	// Indicate failure
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Add scrolling text and its url

BOOL CScrollLink::AddScrollText(CString strText /* Text to scroll */,
								CString strURL /* URL behind scrolled text */)
{
	// Allocate memory for CTextFrame object
	CTextFrame* pTextFrame = new CTextFrame(strText, strURL);

	// Check if memory allocation successful
	if (pTextFrame != NULL)
	{
		// Store object into array
		m_oTextFrames.Add(pTextFrame);

		// Assign null pointer value
		pTextFrame = NULL;

		// Indicate success
		return TRUE;
	}

	// Indicate failure
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Remove all previously added scroll text

BOOL CScrollLink::RemoveAllScrollText(void)
{
	// Remove all scroll text
	DeleteTextFrames();

	// Repaint
	Invalidate();

	// Indicate success
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CScrollLink message handlers

void CScrollLink::PreSubclassWindow() 
{
	// Create default font for text
	CreateTextFonts();

	// Calculate window and text area coordinates
	CalculateWindowAndTextArea();

	CStatic::PreSubclassWindow();
}

BOOL CScrollLink::OnEraseBkgnd(CDC* pDC) 
{
	// Paint background with color
	pDC->FillSolidRect(&m_oWindowArea, m_clrBackground);
	
	// Background erased
	return TRUE;
}

void CScrollLink::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Duplicate window area for calculation purposes
	CRect oInnerArea = m_oWindowArea;
	
	// Deflate rect area by 1 pixel
	oInnerArea.DeflateRect(1, 1);

	// Draw 2 3d borders
	dc.Draw3dRect(&oInnerArea, GetSysColor(COLOR_BTNHILIGHT), GetSysColor(COLOR_BTNSHADOW));
	dc.Draw3dRect(&m_oWindowArea, GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNHILIGHT));

	// Do not call CStatic::OnPaint() for painting messages
}

void CScrollLink::OnTimer(UINT nIDEvent) 
{
	// Set new position for current scrolling texts based on scroll speed
	IncrementScrollingTextFramesPosition();

	// Create a CClientDC object
	CClientDC oClientDC(this);

	// Set the clipping region so that text not within it will not be drawn 
	SetClippingRegion(&oClientDC);
	
	// Select font for text to be drawn
	CFont* pOldFont = oClientDC.SelectObject(&m_oNormalFont);

	// Set background mode to opaque
    oClientDC.SetBkMode(OPAQUE);

	// Set text background color to match control background
	oClientDC.SetBkColor(m_clrBackground);

	// Draw scrolling text into text area
	DrawScrollingText(&oClientDC);

	// Restore original font
	oClientDC.SelectObject(pOldFont);

	// Remove index of finished scrolling text if it runs out of view
	RemoveFinishedScrollingText();

	// Add index of new scrolling text if allowance satisfied
	AddOnNewScrollingText();

	CStatic::OnTimer(nIDEvent);
}

void CScrollLink::OnMouseMove(UINT nFlags, CPoint point) 
{
	// X coordinate of hovered text frame
	int iXOffSet = 0;

	// Device context for client area
	CClientDC oClientDC(this);

	// Get index of text frame object that is being hovered
	m_iHoveredTextFrame = IsTextBeingHovered(point, &iXOffSet);

	// Check if mouse is not hovering text
	if (m_bOnHover == FALSE)
	{
		// Check if text is being hovered
		if (m_iHoveredTextFrame != -1)
		{
			// Stop timer
			KillTimer(ID_SCROLLTIMER);

			// Mouse is now hovering text
			m_bOnHover = TRUE;

			// Create TRACKMOUSEEVENT structure
			TRACKMOUSEEVENT tmeMouseEvent;		

			// Initialize members of structure
			tmeMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
			tmeMouseEvent.dwFlags = TME_LEAVE;
			tmeMouseEvent.hwndTrack = m_hWnd;
	
			// Track mouse leave event
			_TrackMouseEvent(&tmeMouseEvent);

			// Get pointer to the text frame object
			CTextFrame* pTextFrame = (CTextFrame*)m_oTextFrames.GetAt(m_iHoveredTextFrame);

			// Check if this text has a hyperlink
			if (pTextFrame->m_strURL.IsEmpty() == FALSE)
			{
				// Set the clipping region so that text not within it will not be drawn 
				SetClippingRegion(&oClientDC);
	
				// Select font for text to be drawn
				CFont* pOldFont = oClientDC.SelectObject(&m_oUnderlinedFont);

				// Set background mode to opaque
				oClientDC.SetBkMode(OPAQUE);

				// Set text color
				oClientDC.SetTextColor(m_clrHover);

				// Set text background color to match control background
				oClientDC.SetBkColor(m_clrBackground);

				// Calculate a rectangular coordinates to draw scroll text
				CRect oDrawingArea(iXOffSet, m_oTextArea.top, m_oTextArea.right, m_oTextArea.bottom);

				// Draw scroll text
				oClientDC.DrawText(pTextFrame->m_strText, &oDrawingArea, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);

				// Restore original font
				oClientDC.SelectObject(pOldFont);
			}
		}
	}
	
	// Mouse move when text is hovered
	else
	{
		// Check if text is not being hovered
		if (m_iHoveredTextFrame == -1)
		{
			// Mouse not hovering
			OnMouseLeave(0, 0);
		}
	}
	
	CStatic::OnMouseMove(nFlags, point);
}

LRESULT CScrollLink::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	// Mouse not hovering text
	m_bOnHover = FALSE;

	// Continue scrolling text
	SetTimer(ID_SCROLLTIMER, m_uiScrollRate, NULL);	

	return NULL;
}

void CScrollLink::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	
	// Calculate window and text area coordinates
	CalculateWindowAndTextArea();
}

BOOL CScrollLink::DestroyWindow() 
{
	// Kill timer
	KillTimer(ID_SCROLLTIMER);
	
	return CStatic::DestroyWindow();
}

void CScrollLink::OnClicked() 
{
	// Check if mouse hovering text and index of text being hovered is valid
	if (m_bOnHover != FALSE && m_iHoveredTextFrame != -1)
	{
		// Get pointer to the text frame object
		CTextFrame* pTextFrame = (CTextFrame*)m_oTextFrames.GetAt(m_iHoveredTextFrame);

		// Make sure url is not empty
		if (pTextFrame->m_strURL.IsEmpty() != TRUE)
		{
			// Open browser and go to url
			ShellExecute(NULL, _T("open"), pTextFrame->m_strURL, NULL, NULL, SW_SHOW);
		}
	}	
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextFrame::CTextFrame()
{
	// Initialize member data
	m_szTextSize.cx = 0;
	m_szTextSize.cy = 0;
	m_strText = _T("");
	m_strURL = _T("");

	// Empty array
	m_iXOffSets.RemoveAll();
}

CTextFrame::CTextFrame(CString strText /* Text of scrol */,
					   CString strURL /* URL behind text */)
{
	// Initialize member data
	m_szTextSize.cx = 0;
	m_szTextSize.cy = 0;
	m_strText = strText;
	m_strURL = strURL;

	// Empty array
	m_iXOffSets.RemoveAll();
}

CTextFrame::~CTextFrame()
{
	// Empty array
	m_iXOffSets.RemoveAll();
}