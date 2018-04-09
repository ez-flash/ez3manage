#if !defined(AFX_PAGEFLASH_H__97B11D49_A0FB_4B37_A9D5_B674982367F2__INCLUDED_)
#define AFX_PAGEFLASH_H__97B11D49_A0FB_4B37_A9D5_B674982367F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageFlash.h : header file
//
#include "resource.h"
#include "EZListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CPageFlash dialog

class CPageFlash : public CDialog
{
private:
// Construction
public:
	CPageFlash(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CPageFlash)
	enum { IDD = IDD_DIALOG_FLASH };
	CEZListCtrl	m_ListCtrl;
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageFlash)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageFlash)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonAddRom();
	afx_msg void OnButtonDeleteRom();
	afx_msg void OnButtonBackRom();
	afx_msg void OnButtonReflashRom();
	afx_msg void OnButtonFormarRom();
	afx_msg void OnButtonWriteRom();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEFLASH_H__97B11D49_A0FB_4B37_A9D5_B674982367F2__INCLUDED_)
