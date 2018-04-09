#if !defined(AFX_DLGOSSELECT_H__05A5F50F_5B75_41FA_9AE4_0A62C32466DE__INCLUDED_)
#define AFX_DLGOSSELECT_H__05A5F50F_5B75_41FA_9AE4_0A62C32466DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOSSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOSSelect dialog

class CDlgOSSelect : public CDialog
{
// Construction
public:
	CDlgOSSelect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgOSSelect)
	enum { IDD = IDD_DIALOG_LOADER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	BOOL GetLoader();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOSSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL	bNds;
	// Generated message map functions
	//{{AFX_MSG(CDlgOSSelect)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOSSELECT_H__05A5F50F_5B75_41FA_9AE4_0A62C32466DE__INCLUDED_)
