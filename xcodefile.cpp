// XCodeFile.cpp: implementation of the CXCodeFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "New_EZManageDlg.h"
#include "XCodeFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXCodeFile::CXCodeFile()
{
	m_xCodeArray.RemoveAll();
	dwFileLength =0;
}

CXCodeFile::~CXCodeFile()
{
	XCODE *pXCode;
	for(int i=0;i<m_xCodeArray.GetSize();i++)
	{
		pXCode = m_xCodeArray.GetAt(i);
		if(pXCode->pCode)
			delete pXCode->pCode;
		delete pXCode;
	}
	m_xCodeArray.RemoveAll();
}

BOOL CXCodeFile::Init(CString strFileName)
{
	
	CFile file;
	if(!file.Open(strFileName,CFile::modeRead|CFile::typeBinary))
		return FALSE;
	DWORD offset = 0;
	file.Seek(0,CFile::begin);
	unsigned char *p = NULL;
	unsigned char *pData = NULL;
	dwFileLength = file.GetLength();
	if(dwFileLength>0)
		pData = new unsigned char[dwFileLength];
	file.Read(pData,dwFileLength);
	file.Close();
	CBlowFish oBlowFish((unsigned char*)"qd3452sanfafjk46adsfjkdsfjksadfjkaafgh125646346565673457", 56);//lichuan是密码
	int loop = dwFileLength / 8;
	//解密
	p = pData;
	for(int i=0;i<loop;i++)
	{
		oBlowFish.Decrypt(p,8);	
		p = p+8;
	}
	p = pData;
	//解密后的数据在指针pData中;
	//下面把数据写进数组中
	XCODE *pxCode = NULL;
	while (offset != dwFileLength)
	{
		pxCode = new XCODE;
		pxCode->pCode =NULL;
		memset(pxCode,0,sizeof(XCODE));
		memcpy(pxCode,(XCODE *)p,sizeof(XCODE)-4);
		if(pxCode->sign != 'XCDE')
		{
			delete pxCode;
			pxCode = NULL;
			return FALSE;
		}
		if(pxCode->CodeSize>0)
			pxCode->pCode = new CODE[pxCode->CodeSize];
		else
			pxCode->pCode  = NULL;
		p = p+sizeof(XCODE)-4;
		offset = offset +sizeof(XCODE)-4;
		memcpy(pxCode->pCode,(CODE *)p,sizeof(CODE)*pxCode->CodeSize);
		m_xCodeArray.Add(pxCode);
		p = p+sizeof(CODE)*pxCode->CodeSize;
		offset  = offset+sizeof(CODE)*pxCode->CodeSize;
	}
	if(pData)
		delete []pData;
	return TRUE;
}

void  CXCodeFile::Quit()
{
	XCODE *pXCode;
	for(int i=0;i<m_xCodeArray.GetSize();i++)
	{
		pXCode = m_xCodeArray.GetAt(i);
		if(pXCode->pCode)
			delete pXCode->pCode;
		delete pXCode;
	}
	m_xCodeArray.RemoveAll();
}
