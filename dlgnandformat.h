#if !defined(AFX_DLGNANDFORMAT_H__F436F073_1A21_4B02_81C0_E50DB8737913__INCLUDED_)
#define AFX_DLGNANDFORMAT_H__F436F073_1A21_4B02_81C0_E50DB8737913__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNandFormat.h : header file
//
#include "New_EZManageDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgNandFormat dialog

class CDlgNandFormat : public CDialog
{
public:
	HANDLE				m_EventHandle;
// Construction
public:
	CDlgNandFormat(CWnd* pParent = NULL);   // standard constructor
	BOOL m_bFormat;
	CNew_EZManageDlg *m_pDlg;

	CWinThread		*pTWThreadFormatNand;
	UINT TWThreadFormatNand();
	static UINT TWThreadFormatNand( LPVOID pParam );
// Dialog Data
	//{{AFX_DATA(CDlgNandFormat)
	enum { IDD = IDD_DIALOG_FORMAT_NAND };
	CProgressCtrl	m_NandFormatCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNandFormat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNandFormat)
	afx_msg void OnBeginFormat();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNANDFORMAT_H__F436F073_1A21_4B02_81C0_E50DB8737913__INCLUDED_)
