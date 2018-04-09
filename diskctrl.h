#if !defined(AFX_DISKCTRL_H__C5BB193E_C51F_4B52_9040_E8902C49BA21__INCLUDED_)
#define AFX_DISKCTRL_H__C5BB193E_C51F_4B52_9040_E8902C49BA21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiskCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiskCtrl window

class CDiskCtrl : public CListCtrl
{
// Construction
public:
	CDiskCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiskCtrl)
	//}}AFX_VIRTUAL

// Implementation
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

public:
	virtual ~CDiskCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDiskCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISKCTRL_H__C5BB193E_C51F_4B52_9040_E8902C49BA21__INCLUDED_)
