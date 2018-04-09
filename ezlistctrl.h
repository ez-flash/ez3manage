#if !defined(AFX_EZLISTCTRL_H__51D5758B_2FBE_4F9D_A1C5_14806CB511A9__INCLUDED_)
#define AFX_EZLISTCTRL_H__51D5758B_2FBE_4F9D_A1C5_14806CB511A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EZListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEZListCtrl window

class CEZListCtrl : public CListCtrl
{
// Construction
public:
	CEZListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEZListCtrl)
	//}}AFX_VIRTUAL

// Implementation
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

public:
	virtual ~CEZListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEZListCtrl)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EZLISTCTRL_H__51D5758B_2FBE_4F9D_A1C5_14806CB511A9__INCLUDED_)
