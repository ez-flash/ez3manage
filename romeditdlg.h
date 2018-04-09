#if !defined(AFX_ROMEDITDLG_H__E143818A_98F2_417F_9029_0CC4F1C7D177__INCLUDED_)
#define AFX_ROMEDITDLG_H__E143818A_98F2_417F_9029_0CC4F1C7D177__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RomEditDlg.h : header file
//

#include "New_EZManageDlg.h"
#include "ChtFileInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CRomEditDlg dialog

class CRomEditDlg : public CDialog
{
private:
	GameRomInfo *m_pInfo;
	DWORD dw_CheatCounter;	
	HTREEITEM m_hRootItem;
	HTREEITEM m_hItem1;
	HTREEITEM m_hItem2;
	BOOL m_bZip;
	BOOL m_bZipButState;
	BOOL m_bXCodeState;
	BOOL DiskPlayCheatFile(CString strFileName);
	void AddItemToCheatTree(PCHT_DATA pCht_data,BOOL bRoot);
	DWORD GetTreeItemCounter(HTREEITEM hItem);
	GOLD_DATA *GetGoldData(CString str);
	
// Construction
public:
	CRomEditDlg(CWnd* pParent = NULL);   // standard constructor

	void SetGameInfo(GameRomInfo *pInfo);
	GameRomInfo *GetGameInfo();
	PGOLDCHEAT GetGoldDataFormTree(WORD &w_CheatSize);
	BOOL GetZipState();
	void SetZipState(BOOL bState);
	void SetXCodeState(BOOL bState);
// Dialog Data
	//{{AFX_DATA(CRomEditDlg)
	enum { IDD = IDD_DIALOG_ROM_EDIT };
	CComboBox	m_CbSavepatchType;
	CTreeCtrl	m_CheatTree;
	CComboBox	m_CbSaveSize;
	CComboBox	m_CbSaveType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRomEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRomEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonOpenChtfile();
	afx_msg void OnCheckNorReset();
	afx_msg void OnSelchangeComboSavetype();
	afx_msg void OnSelchangeComboSavesize();
	afx_msg void OnSelchangeComboSavepatch();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCheckZip();
	afx_msg void OnCheckNorSavenow();
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROMEDITDLG_H__E143818A_98F2_417F_9029_0CC4F1C7D177__INCLUDED_)
