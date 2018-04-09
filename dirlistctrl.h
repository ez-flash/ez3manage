#if !defined(AFX_DIRLISTCTRL_H__1A881E53_0496_11D3_A7D4_00C04F597B8A__INCLUDED_)
#define AFX_DIRLISTCTRL_H__1A881E53_0496_11D3_A7D4_00C04F597B8A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DirListCtrl.h : header file
//

#define WM_USER_SETDIRECTORY	(WM_USER + 0x0500)
#define WM_USER_GETDIRECTORY	(WM_USER + 0x0501)
#define WM_USER_SETBUDDY		(WM_USER + 0x0502)
#define WM_USER_GETBUDDY		(WM_USER + 0x0503)

#include <vector>
using namespace std;

// Structure mirroring fields of interest from WIN32_FIND_DATA
struct FileInfoStruc
{
	DWORD		dwFileAttributes;
	FILETIME	ftCreationTime; 
	DWORD		nFileSize; 
	TCHAR		*cFileName;

	FileInfoStruc(const CFileFind& ff);
	~FileInfoStruc()
	{ delete [] cFileName; }

protected:
	// CMyFileFind: allows direct access to dwFileAttributes of CFileFind
	class CMyFileFind: public CFileFind
	{
	public:
		DWORD GetFileAttributes () const
		{ return ((LPWIN32_FIND_DATA) m_pFoundInfo)->dwFileAttributes; }
	};
};

/////////////////////////////////////////////////////////////////////////////
// CDirListCtrl window

class CDirListCtrl : public CListCtrl
{
// Construction
public:
	CDirListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	// Supporting data-types
	typedef vector<FileInfoStruc*> vector_fisp;
	typedef vector<FileInfoStruc*>::iterator iterator_fisp;

	struct ColInfoStruc
	{
		LPTSTR	pszColHdr;					// Column header string
		int		nColFmt;						// Column format
		int		nColWidth;					// Column width
	};

// Data members
protected:
	CImageList		m_SmallImgList;		// System small image list
	CString			m_strDirectory;		// Directory being displayed
	vector_fisp		m_vecFiles;				// File info for the files in m_strDirectory
	HWND				m_hWndBuddy;			// Stores the HWND of the buddy Edit control
	static 
	ColInfoStruc	s_aCIS[];				// Column info for the report view

protected:
	void FreeVec();

	CString	ConcatDirectory(CString strRelDir);

public:
	virtual ~CDirListCtrl();

protected:
	// helper accessor functions
	inline DWORD GetFileAttributes (int nIdx) const
	{ return m_vecFiles[nIdx]->dwFileAttributes; }

	inline FILETIME GetFileCreationTime (int nIdx) const
	{ return m_vecFiles[nIdx]->ftCreationTime; }

	inline DWORD GetFileSize (int nIdx) const
	{ return m_vecFiles[nIdx]->nFileSize; }

	inline TCHAR* GetFileName (int nIdx) const
	{ return m_vecFiles[nIdx]->cFileName; }

	// Generated message map functions
protected:
	//{{AFX_MSG(CDirListCtrl)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnSetDirectory (WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnGetDirectory (WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnSetBuddy (WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnGetBuddy (WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRLISTCTRL_H__1A881E53_0496_11D3_A7D4_00C04F597B8A__INCLUDED_)
