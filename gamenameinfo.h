// GameNameInfo.h: interface for the CGameNameInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMENAMEINFO_H__DC60FCCB_DB62_45A1_A324_1993281214F7__INCLUDED_)
#define AFX_GAMENAMEINFO_H__DC60FCCB_DB62_45A1_A324_1993281214F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGameNameInfo  
{
public:
	void Init(CString strFileName);
	void SetFileName(CString strFileName);
	CGameNameInfo();
//	CGameNameInfo(CString strFileName);
	virtual ~CGameNameInfo();
	CString FindGameName(CString strGameChinaName);
	CString	FindGameEnglishName(CString strGameName);
	CString FindGameChinaName(CString strGameName);
	void ParseLine(CString Instr,CString &strGameName,CString &strChinaGameName);
	void ParseLine(CString Instr, 
					CString &strGameName, 
					CString &strChinaGameName,
					CString &strEnglishName);
private:
	BOOL OpenFile(DWORD &dwLength);
	CString m_strFileName;
	CString m_StringLine;
	CFile m_file;
	BYTE *pFile;
	CStringArray m_RomNameStringArray;
	CStringArray m_ChinaNameStringArray;
	CStringArray m_EnglishNameStringArray ;
	CString ReadLine(DWORD dwBegin,DWORD &dwEnd);
	CString Parse_Buffer(BYTE *pBuffer,DWORD &dwPosition,DWORD dwLength);
};

#endif // !defined(AFX_GAMENAMEINFO_H__DC60FCCB_DB62_45A1_A324_1993281214F7__INCLUDED_)
