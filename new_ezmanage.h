// New_EZManage.h : main header file for the NEW_EZMANAGE application
//

#if !defined(AFX_NEW_EZMANAGE_H__F4D98B37_56AA_4ED6_A7AA_A45A3AD90885__INCLUDED_)
#define AFX_NEW_EZMANAGE_H__F4D98B37_56AA_4ED6_A7AA_A45A3AD90885__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


/////////////////////////////////////////////////////////////////////////////
// CNew_EZManageApp:
// See New_EZManage.cpp for the implementation of this class
//

class CNew_EZManageApp : public CWinApp
{
private:
	CString m_strLanguage;
	OSVERSIONINFO	osvi;
	BOOL bOsVersionInfoEx;

public:
	CStringArray	m_stringArray;

	CNew_EZManageApp();
	void InitLanguageFile(CString m_strWorkPath);
	void InitAllString(CString m_strWorkPath);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNew_EZManageApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNew_EZManageApp)
	afx_msg void OnAddLoader();
	afx_msg void OnUpdateAddLoader(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEW_EZMANAGE_H__F4D98B37_56AA_4ED6_A7AA_A45A3AD90885__INCLUDED_)
