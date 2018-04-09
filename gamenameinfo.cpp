// GameNameInfo.cpp: implementation of the CGameNameInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameNameInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameNameInfo::CGameNameInfo()
{
	pFile = NULL;
	m_RomNameStringArray.RemoveAll();
	m_ChinaNameStringArray.RemoveAll();
	m_EnglishNameStringArray.RemoveAll();
}

CGameNameInfo::~CGameNameInfo()
{
	
	if(pFile)
		delete []pFile;
	
	m_RomNameStringArray.RemoveAll();
	m_ChinaNameStringArray.RemoveAll();
	m_EnglishNameStringArray.RemoveAll();	
}


CString CGameNameInfo::ReadLine(DWORD dwBegin,DWORD &dwEnd)
{
	CString strTemp;
	BYTE *p = pFile;
	return strTemp;
}

void CGameNameInfo::ParseLine(CString Instr, CString &strGameName, CString &strChinaGameName)
{
	int n=Instr.Find('|');
	if(n<0)
	{
		strGameName.Empty();
		strChinaGameName.Empty();
		return;
	}
	strGameName = Instr.Left(n);
	strChinaGameName=Instr.Right(Instr.GetLength()-n-1);
	strChinaGameName.Remove('|');
}

void CGameNameInfo::ParseLine(CString Instr, 
							CString &strGameName, 
							CString &strChinaGameName,
							CString &strEnglishName)
{
	int n=Instr.Find('|');
	if(n<0)
	{
		strGameName.Empty();
		strChinaGameName.Empty();
		return;
	}
	strGameName = Instr.Left(n);
	Instr.Delete(0,n);
//	Instr.TrimLeft(strGameName);
	Instr.TrimLeft('|');
	Instr.TrimLeft(' ');
	Instr.TrimLeft(0x9);
	n=Instr.Find('|');
	strChinaGameName = Instr.Left(n);
	strChinaGameName.TrimRight(' ');
	strChinaGameName.TrimRight(0x9);
	Instr.TrimLeft('|');
	Instr.TrimLeft(' ');
	Instr.TrimLeft(0x9);
	strEnglishName=Instr.Right(Instr.GetLength()-n-1);
	strEnglishName.Remove('|');
	strGameName.Remove(' ');
	strGameName.Remove(0x9);

}


CString CGameNameInfo::FindGameChinaName(CString strGameName)
{
	CString strGameChinaName;
	CString strTemp;
	int index=0;
	int SavIndex = -1;
	for(index=0;index<m_RomNameStringArray.GetSize();index++)
	{
		strTemp = m_RomNameStringArray.GetAt(index);
		if(strTemp  == strGameName)
		{
			SavIndex = index;
			break;
		}
	}
	if(SavIndex>=0)
		strGameChinaName = m_ChinaNameStringArray.GetAt(index);
	else
		strGameChinaName.Empty();
	
	return strGameChinaName;
}

CString CGameNameInfo::FindGameEnglishName(CString strGameName)
{
	CString strGameChinaName;
	CString strTemp;
	int index=0;
	int SavIndex = -1;
	for(index=0;index<m_RomNameStringArray.GetSize();index++)
	{
		strTemp = m_RomNameStringArray.GetAt(index);
		if(strTemp  == strGameName)
		{
			SavIndex = index;
			break;
		}
	}
	if(SavIndex>=0)
		strGameChinaName = m_EnglishNameStringArray.GetAt(index);
	else
		strGameChinaName.Empty();
	
	return strGameChinaName;

}

CString CGameNameInfo::FindGameName(CString strGameChinaName)
{
	CString strGameName;
	CString strTemp;
	int index=0;
	int SavIndex = -1;
	for(index=0;index<m_ChinaNameStringArray.GetSize();index++)
	{
		strTemp = m_ChinaNameStringArray.GetAt(index);
		if(strTemp  == strGameChinaName)
		{
			SavIndex = index;
			break;
		}
	}
	if(SavIndex>=0)
		strGameName = m_RomNameStringArray.GetAt(index);
	else
		strGameName .Empty();
	return strGameName;
}

BOOL CGameNameInfo::OpenFile(DWORD &dwLength)
{
	if(!m_file.Open(m_strFileName,CFile::modeRead))
		return FALSE;
	DWORD len = m_file.GetLength();
	if(len <=0)
		return FALSE;
	pFile = new BYTE[len];
	m_file.Read(pFile,len);
	dwLength = m_file.GetLength();
	m_file.Close();
	return TRUE;
}

void CGameNameInfo::SetFileName(CString strFileName)
{
	m_strFileName = strFileName;
}

CString  CGameNameInfo::Parse_Buffer(BYTE *pBuffer,DWORD &dwPosition,DWORD dwLength)
{
	CString str = _T("");
	if(dwPosition>dwLength)
		return str;
	int iStart	=0;
	int iEnd	=0;
	BYTE *pStart = pBuffer+dwPosition;
	BYTE *p = pBuffer+dwPosition;
	while(*p !=0x0D)
	{
		p++;
		iStart++;
		dwPosition++;
	}
	memcpy(str.GetBuffer(iStart),pStart,iStart);
	str.ReleaseBuffer(iStart);
	str.Remove(0x0D);
	str.Remove(0x0A);
//	str.Remove(0x20);
	return str;
}

void CGameNameInfo::Init(CString strFileName)
{
	DWORD dwLength =0;
	DWORD dwPosition=0;
	CString strTemp;
	CString strGameName;
	CString strChinaGameName;
	CString strEnglishName ;
	m_strFileName=strFileName;
	OpenFile(dwLength);
	do
	{
		strTemp = Parse_Buffer(pFile,dwPosition,dwLength);
	
		ParseLine(strTemp,strGameName,strChinaGameName,strEnglishName);
		m_RomNameStringArray.Add(strGameName);
		m_ChinaNameStringArray.Add(strChinaGameName);
		m_EnglishNameStringArray.Add(strEnglishName);
		while(*(pFile+dwPosition)== 0x0D ||*(pFile+dwPosition) == 0x0A)
		{
			dwPosition++;
		}
	}while (dwPosition<dwLength);
	int i=0;
	i++;
}
