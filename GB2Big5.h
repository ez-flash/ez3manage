// GB2Big5.h: interface for the CGB2Big5 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GB2BIG5_H__303EB0FE_8A1C_4200_B79D_CDA6F69D94CC__INCLUDED_)
#define AFX_GB2BIG5_H__303EB0FE_8A1C_4200_B79D_CDA6F69D94CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ZIKUBASE 0xE000

class CGB2Big5     
{
public:
	BYTE *FindMotif(BYTE *Buffer, DWORD BufferSize, BYTE *Motif, DWORD MotifSize);
	void AddLdrSpecialName(BYTE *pLdr , CStringArray &strArray,UINT type);
	void GetDianZheng(BYTE* neima , BYTE* dianzhen,UINT type);
	void GetChinese(LPCTSTR str,BYTE* pBB,int &size);
	CString Big52Gb(LPCTSTR str);
	CString Gb2Big5(LPCTSTR str);
	void Close();
	void Initial();
	CGB2Big5();
	virtual ~CGB2Big5();
protected:
	HRSRC hRes;
	HGLOBAL hGBTable;
	HGLOBAL hBIGTable;
	LPWORD pGBTable;
	LPWORD pBIGTable;
};

#endif // !defined(AFX_GB2BIG5_H__303EB0FE_8A1C_4200_B79D_CDA6F69D94CC__INCLUDED_)
