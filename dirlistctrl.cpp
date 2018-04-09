// DirListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "DirListCtrl.h"

#include <algorithm>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//
// Globals and forward declarations
//

// forward declarations
TCHAR* FormatWithCommas (unsigned long ulSize);
TCHAR* FormatAttr (DWORD dwFileAttributes);
TCHAR* GetTypeName (CString strFPath);
int GetIconIndex (CString strFPath, DWORD dwFileAttr);

//
// FileInfoStruc implementation
//

FileInfoStruc::FileInfoStruc (const CFileFind& ff)
{
	// The following cast allows me to call the accessor function 
	//    GetFileAttributes() defined for CMyFileFind.
	//    The cast is safe because CMyFileFind does not add any data 
	//    members to CFileFind, and does not override/add any virtual functions.
	dwFileAttributes = 
		reinterpret_cast <const CMyFileFind*> (&ff)->GetFileAttributes();
	ff.GetCreationTime(&ftCreationTime);
	nFileSize = ff.GetLength();
	CString strFName = ff.GetFileName();
	cFileName = new TCHAR[strFName.GetLength() + 1];
	lstrcpy(cFileName, (LPCTSTR) strFName);
}


// Predicate used to compare two FileInfoStruc* in sort<>()
bool LessFISPtr(FileInfoStruc *pfis1, FileInfoStruc *pfis2);

/////////////////////////////////////////////////////////////////////////////
//
// CDirListCtrl Implementation
//

// Constants for the ListView control layout
CDirListCtrl::ColInfoStruc CDirListCtrl::s_aCIS[] =
{
	{ _T("Filename"),		LVCFMT_LEFT,	120 },
	{ _T("Size"),			LVCFMT_RIGHT,	60  },
	{ _T("Type"),			LVCFMT_LEFT,	120 },
	{ _T("Created"),		LVCFMT_LEFT,	60  },
	{ _T("Attributes"),	LVCFMT_LEFT,	60  },
};


CDirListCtrl::CDirListCtrl (): 
	m_strDirectory("."), m_hWndBuddy(NULL)
{
}

CDirListCtrl::~CDirListCtrl ()
{
}


BEGIN_MESSAGE_MAP(CDirListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CDirListCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetDispInfo)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_SETDIRECTORY, OnSetDirectory)
	ON_MESSAGE(WM_USER_GETDIRECTORY, OnGetDirectory)
	ON_MESSAGE(WM_USER_SETBUDDY, OnSetBuddy)
	ON_MESSAGE(WM_USER_GETBUDDY, OnGetBuddy)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CDirListCtrl message handlers
//
//

void CDirListCtrl::PreSubclassWindow () 
{
	// Set list view style to report, no sort
	//    I mention it here for reference, but ModifyStyle() will not work if 
	//    LVS_OWNERDATA is not already set in the dialog template.
	// Note that the LVS_ style LVS_OWNERDATA has to be set in the dialog template.
	ASSERT ((GetStyle() & LVS_OWNERDATA) != 0);
	ModifyStyle (
		LVS_SORTASCENDING | LVS_SORTDESCENDING | LVS_ICON | LVS_SMALLICON | LVS_LIST, 
		LVS_REPORT);

	int nHdrCnt = GetHeaderCtrl()->GetItemCount();
	ASSERT(nHdrCnt == 0);

	// Insert the columns
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	
	nHdrCnt = sizeof(s_aCIS) / sizeof(ColInfoStruc);
	for(int i = 0; i < nHdrCnt; i++)
	{
		lvc.iSubItem	= i;
		lvc.pszText		= s_aCIS[i].pszColHdr;
		lvc.fmt			= s_aCIS[i].nColFmt;
		lvc.cx			= s_aCIS[i].nColWidth;
		InsertColumn(i, &lvc);
	}

	// Retrieve the system image list
	SHFILEINFO sfi;
	memset(&sfi, 0, sizeof(sfi));
	HIMAGELIST hil = reinterpret_cast<HIMAGELIST> (
		SHGetFileInfo (
			"C:\\", 
			0, 
			&sfi, 
			sizeof(sfi), 
			SHGFI_SYSICONINDEX | SHGFI_SMALLICON)
	);
	if (hil)
	{
		m_SmallImgList.Attach (hil);
		SetImageList (&m_SmallImgList, LVSIL_SMALL);
	}

	// perform default processing
	CListCtrl::PreSubclassWindow ();
}


// WM_USER_GETBUDDY handler.  Returns the HWND of the Buddy control
LRESULT CDirListCtrl::OnGetBuddy (WPARAM, LPARAM)
{
	return (LRESULT) m_hWndBuddy;
}


// WM_USER_SETBUDDY handler.  Sets the HWND for the Buddy control
LRESULT CDirListCtrl::OnSetBuddy (WPARAM wp, LPARAM)
{
	m_hWndBuddy = reinterpret_cast<HWND> (wp);
	return 0;
}


// WM_USER_GETDIRECTORY handler.
//    Copies at most lp TCHARs from m_strDirectory to (LPTSTR) wp.
LRESULT CDirListCtrl::OnGetDirectory (WPARAM wp, LPARAM lp)
{
	lstrcpyn((LPTSTR) wp, (LPCTSTR) m_strDirectory, lp);
	return 0;
}


// WM_USER_SETDIRECTORY handler.
//    Sets the directory to (LPTSTR) wp.  Fills the list view.
LRESULT CDirListCtrl::OnSetDirectory (WPARAM wp, LPARAM lp)
{
	CString strNewDir;

	// If wp == NULL ==> Refresh
	if (!wp)
		strNewDir = m_strDirectory;
	else 
		strNewDir = reinterpret_cast<LPTSTR> (wp);

	int nLen = strNewDir.GetLength();

	if (nLen == 0)
		return FALSE;

	if (strNewDir[nLen-1] == TCHAR('\\'))
		strNewDir = strNewDir.Left(nLen - 1);

	CFileFind ff;

	if (!ff.FindFile (strNewDir + "\\*.*"))
		return FALSE;

	CWaitCursor crsWait;

	DWORD tickInit = GetTickCount ();	// get current tick.

	FreeVec();									// Clear m_vecFiles
	DeleteAllItems();							// and the list view
	TRACE ("FreeOld required: %d milliseconds.\n", 
		GetTickCount() - tickInit);

	m_strDirectory = strNewDir;			// Update cached directory name

	// Get the files from strNewDir into m_vecFiles
	BOOL bCont;
	do
	{
		bCont = ff.FindNextFile ();
		if (ff.IsDots())
			continue;

		if (m_vecFiles.capacity() == m_vecFiles.size())
			m_vecFiles.reserve(m_vecFiles.capacity() + 256);

		m_vecFiles.push_back(new FileInfoStruc (ff));
	}
	while (bCont);

	SetItemCountEx (m_vecFiles.size ());
	TRACE ("FreeOld/GetNew required: %d milliseconds.\n",
		GetTickCount() - tickInit);

	// Sort m_vecFiles
	sort(m_vecFiles.begin(), m_vecFiles.end(), LessFISPtr);
	TRACE ("FreeOld/GetNew/SortNew required: %d milliseconds.\n",
		GetTickCount() - tickInit);

	// Set the text in the buddy control to reflect the current directory
	if (m_hWndBuddy != NULL && m_hWndBuddy != INVALID_HANDLE_VALUE)
		::SetWindowText (m_hWndBuddy, m_strDirectory);

	return TRUE;
}


void CDirListCtrl::OnGetDispInfo (NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_ITEM &lvItem = reinterpret_cast<LV_DISPINFO*>(pNMHDR)->item;

	if (lvItem.mask & LVIF_TEXT)
	{
		switch (lvItem.iSubItem)
		{
		case 0:
			// fill in the filename
			lvItem.pszText = GetFileName (lvItem.iItem);
			break;
		case 1:
			// fill in the file size if item is not directory
			if (!(GetFileAttributes(lvItem.iItem) & FILE_ATTRIBUTE_DIRECTORY))
				lvItem.pszText = FormatWithCommas(GetFileSize(lvItem.iItem));
			break;
		case 2:
			lvItem.pszText = GetTypeName (m_strDirectory + _T("\\") + GetFileName(lvItem.iItem));
			break;
		case 3:
			// fill in creation date
			{
				CTime ctm;
				static CString strDate;
				ctm = GetFileCreationTime (lvItem.iItem);
				strDate = ctm.Format("%x");
				lvItem.pszText = const_cast<LPTSTR> ((LPCTSTR) strDate);
				break;
			}
		case 4:
			lvItem.pszText = FormatAttr(GetFileAttributes(lvItem.iItem));
			break;
		default:
			break;
		}
	}

	// fill in icon index
	if (lvItem.mask & LVIF_IMAGE)
	{
		lvItem.iImage = GetIconIndex (
			m_strDirectory + _T("\\") + GetFileName(lvItem.iItem), 
			GetFileAttributes(lvItem.iItem)
		);
	}

	*pResult = 0;
}


// Double clicks on directories ==> Change to that directory
void CDirListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	UINT fFlags;
	int iItem = HitTest(point, &fFlags);

	if (fFlags & (LVHT_ONITEMLABEL | LVHT_ONITEMICON))
	{
		CString strRelDir = GetItemText(iItem, 0);
		SendMessage(WM_USER_SETDIRECTORY, reinterpret_cast<WPARAM> ((LPCTSTR) ConcatDirectory(strRelDir)));
	}
	
	CListCtrl::OnLButtonDblClk(nFlags, point);
}


// Enter when a directory item has focus ==> Change to that directory
void CDirListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// if user presses Enter change to the directory with focus
	if (nChar == VK_RETURN)
	{
		int nFocused = GetNextItem(-1, LVNI_FOCUSED);
		if (nFocused != -1)
		{
			CString strRelDir = GetItemText(nFocused, 0);
			SendMessage(WM_USER_SETDIRECTORY, reinterpret_cast<WPARAM> ((LPCTSTR) ConcatDirectory(strRelDir)));
		}
	}
	else if (nChar == VK_BACK)
	{
		SendMessage(WM_USER_SETDIRECTORY, reinterpret_cast<WPARAM> ((LPCTSTR) ConcatDirectory("..")));
	}
	else if (nChar == VK_TAB)
   {
      if (GetAsyncKeyState(VK_SHIFT) < 0)
         GetParent()->PostMessage (WM_NEXTDLGCTL, 1, 0L);
      else
         GetParent()->PostMessage (WM_NEXTDLGCTL, 0, 0L);

		return;
	}
	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CDirListCtrl::OnDestroy() 
{
	// Clear m_vecFiles
	FreeVec();
	// Detachimage list so the system image list won't be destroyed
	m_SmallImgList.Detach();
	// Call default processing
	CListCtrl::OnDestroy();
}


// Declare we want all keys, so we can handle <<Enter>>
UINT CDirListCtrl::OnGetDlgCode() 
{
	return DLGC_WANTALLKEYS;
}


//////////////////////////////////////////////////////////////////////
//
// Helper functions
//
//


void CDirListCtrl::FreeVec()
{
	for (iterator_fisp it = m_vecFiles.begin(); it != m_vecFiles.end(); it++)
	{
		delete (*it);
	}
	m_vecFiles.clear();
}


CString CDirListCtrl::ConcatDirectory (CString strRelDir)
{
	if (strRelDir == CString(_T("..")))
	{
		int nLastBS = m_strDirectory.ReverseFind(TCHAR('\\'));
		return m_strDirectory.Left(nLastBS);
	}
	else
	{
		return m_strDirectory + _T("\\") + strRelDir;
	}
}


bool LessFISPtr(FileInfoStruc *pfis1, FileInfoStruc *pfis2)
{
	ASSERT(pfis1 && pfis2);
	DWORD dw1 = pfis1->dwFileAttributes;
	DWORD dw2 = pfis2->dwFileAttributes;

	if ((dw1 & FILE_ATTRIBUTE_DIRECTORY) && !(dw2 & FILE_ATTRIBUTE_DIRECTORY))
		return true;
	else if ((dw2 & FILE_ATTRIBUTE_DIRECTORY) && !(dw1 & FILE_ATTRIBUTE_DIRECTORY))
		return false;

	if (lstrcmp(pfis1->cFileName, pfis2->cFileName) == -1)
		return true;
	else
		return false;
}

int GetIconIndex (CString strFPath, DWORD dwFileAttr)
{
	static int _nDirIconIdx = -1;
	SHFILEINFO sfi;
	memset(&sfi, 0, sizeof(sfi));

	if (dwFileAttr & FILE_ATTRIBUTE_DIRECTORY)
	{
		if (_nDirIconIdx == -1)
		{
			SHGetFileInfo (
				strFPath, 
				FILE_ATTRIBUTE_DIRECTORY, 
				&sfi, 
				sizeof(sfi), 
				SHGFI_SMALLICON | SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES
			);
			// Cache the index for the directory icon
			_nDirIconIdx = sfi.iIcon;
		}

		return _nDirIconIdx;
	}
	else
	{
		SHGetFileInfo (
			strFPath, 
			FILE_ATTRIBUTE_NORMAL, 
			&sfi, 
			sizeof(sfi), 
			SHGFI_SMALLICON | SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES
		);

		return sfi.iIcon;
	}

	return -1;
}


TCHAR* FormatWithCommas (unsigned long ulSize)
{
	static TCHAR lpBuff[100];

	TCHAR *psz = lpBuff + 99;
	*psz = TCHAR('\0');

	int nCnt = 0;
	while (1)
	{
		*(--psz) = (ulSize % 10) + TCHAR('0');
		ulSize /= 10;
		if (ulSize == 0)
			break;

		nCnt++;
		if (nCnt % 3 == 0)
			*(--psz) = TCHAR(',');
	}

	return psz;
}


TCHAR* FormatAttr (DWORD dwFileAttributes)
{
	static TCHAR lpBuff[10];
	static DWORD adwFA[5] = { FILE_ATTRIBUTE_READONLY,	FILE_ATTRIBUTE_HIDDEN,	FILE_ATTRIBUTE_ARCHIVE,	FILE_ATTRIBUTE_SYSTEM,	FILE_ATTRIBUTE_COMPRESSED };
	static TCHAR achFA[5] = { TCHAR('R'),					TCHAR('H'),					TCHAR('A'),					TCHAR('S'),					TCHAR('C')};
	int nCnt = 0;

	for (int i = 0; i < 5; i++)
		if (dwFileAttributes & adwFA[i])
			lpBuff[nCnt++] = achFA[i];

	lpBuff[nCnt] = (TCHAR) 0;

	return lpBuff;
}


TCHAR* GetTypeName (CString strFPath)
{
	SHFILEINFO sfi;
	memset(&sfi, 0, sizeof(sfi));

	static TCHAR lpBuff[MAX_PATH];
	lpBuff[0] = TCHAR ('\0');

	SHGetFileInfo (
		strFPath, 
		0, 
		&sfi, 
		sizeof(sfi), 
		SHGFI_TYPENAME
	);

	lstrcpy(lpBuff, sfi.szTypeName);
	if (lpBuff[0] == TCHAR('\0'))
	{
		int nDotIdx = strFPath.ReverseFind (TCHAR('.'));
		int nBSIdx = strFPath.ReverseFind (TCHAR('\\'));
		if (nDotIdx > nBSIdx)
		{
			strFPath = strFPath.Mid(nDotIdx+1);
			strFPath.MakeUpper();
			lstrcpy (lpBuff, strFPath + TCHAR (' '));
		}

		lstrcat (lpBuff, _T("File"));
	}

	return lpBuff;
}

