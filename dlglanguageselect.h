#if !defined(AFX_DLGLANGUAGESELECT_H__6C336D40_367E_4279_9E9C_4541F411AFD4__INCLUDED_)
#define AFX_DLGLANGUAGESELECT_H__6C336D40_367E_4279_9E9C_4541F411AFD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLanguageSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLanguageSelect dialog

class CDlgLanguageSelect : public CDialog
{
private:
	CString strLanguage;
// Construction
public:
	CDlgLanguageSelect(CWnd* pParent = NULL);   // standard constructor
	CString  GetSelectLanguage();
// Dialog Data
	//{{AFX_DATA(CDlgLanguageSelect)
	enum { IDD = IDD_DIALOG_SELECT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLanguageSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLanguageSelect)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLANGUAGESELECT_H__6C336D40_367E_4279_9E9C_4541F411AFD4__INCLUDED_)
