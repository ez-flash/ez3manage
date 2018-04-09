#if !defined(AFX_PAGEDISK_H__DB150644_A6D3_4BAB_8D38_C840AAC8BFB5__INCLUDED_)
#define AFX_PAGEDISK_H__DB150644_A6D3_4BAB_8D38_C840AAC8BFB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageDisk.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CPageDisk dialog
typedef struct{
	char	f_name[32];
	DWORD	f_size;
	BYTE	f_type;
	BYTE	f_attribute;
	SYSTEMTIME filetime;
}FILE_ATTRIBUTE;
class CPageDisk : public CDialog
{
private:
	CStringArray *m_pArray;

	void ChangeListCtrlView(NM_LISTVIEW *pNM);
// Construction
public:
	CString m_strNandDiskPath;
	CString m_strReOldName;
	CPageDisk(CWnd* pParent = NULL);   // standard constructor

	BOOL CheckSameName(CString str);
	CStringArray *GetSelectFileName(BOOL bFolder);
	void RemoveSlectItem(CString str);
// Dialog Data
	//{{AFX_DATA(CPageDisk)
	enum { IDD = IDD_DIALOG_DISK };
	CListCtrl	m_DiskListCtrl;
	//}}AFX_DATA
	void InitDisk();
	void QuitDisk();
	void InitDiskList();
	void AddItemToListCtrl(FILE_ATTRIBUTE file_attribute);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageDisk)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList		m_ImageListctrl;
	// Generated message map functions
	//{{AFX_MSG(CPageDisk)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonNandRename();
	afx_msg void OnButtonNandRead();
	afx_msg void OnButtonNandDelete();
	afx_msg void OnButtonNandBackdir();
	afx_msg void OnButtonNandCreatedir();
	afx_msg void OnButtonNandDeletedir();
	afx_msg void OnButtonNandFormat();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeleditList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonNandRefresh();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnDropFiles(HDROP hDropInfo);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEDISK_H__DB150644_A6D3_4BAB_8D38_C840AAC8BFB5__INCLUDED_)
