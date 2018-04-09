// XCodeFile.h: interface for the CXCodeFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCODEFILE_H__7F21E762_124E_4827_8A4E_510E4237B377__INCLUDED_)
#define AFX_XCODEFILE_H__7F21E762_124E_4827_8A4E_510E4237B377__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 100
#include ".\mars\BlowFish.h"

class CXCodeFile  
{ 
public:
	CXCodeFile();
	virtual ~CXCodeFile();
public:
	CArray<PXCODE,PXCODE> m_xCodeArray;
	BOOL Init(CString strFileName);
	void Quit();
private:
	DWORD dwFileLength;
};

#endif // !defined(AFX_XCODEFILE_H__7F21E762_124E_4827_8A4E_510E4237B377__INCLUDED_)
