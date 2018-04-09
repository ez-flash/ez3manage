// AmdEz3FlashBase.h: interface for the CAmdEz3FlashBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AMDEZ3FLASHBASE_H__7D8F9B91_D617_4213_9E4E_0E9BC33FB3B1__INCLUDED_)
#define AFX_AMDEZ3FLASHBASE_H__7D8F9B91_D617_4213_9E4E_0E9BC33FB3B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "EzFlashBase.h"

class CAmdEz3FlashBase  :public CEzFlashBase
{
public:
	CAmdEz3FlashBase();
	virtual ~CAmdEz3FlashBase();
public:
	DWORD  EXECFLH_ReadID(HANDLE hDev);
	void   EXECFLH_ReadArray(HANDLE hDev); 
	WORD   EXECFLH_ReadStatus(HANDLE hDev);
	void   EXECFLH_ClearStatus(HANDLE hDev);
	void   EXECFLH_BlockErase(HANDLE hDev,DWORD BlockNum);
	void   EXECFLH_Read(HANDLE hDev, DWORD StartAddr, BYTE* pbuf, DWORD ByteCount);
	void   EXECFLH_Write(HANDLE hDev, DWORD StartAddr, BYTE* pbuf, DWORD ByteCount);
};

#endif // !defined(AFX_AMDEZ3FLASHBASE_H__7D8F9B91_D617_4213_9E4E_0E9BC33FB3B1__INCLUDED_)
