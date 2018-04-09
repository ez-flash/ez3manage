// FujiEZ3Flash1g.h: interface for the CFujiEZ3Flash1g class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUJIEZ3FLASH1G_H__B145F554_B1E0_463A_B959_D7AA3339DB06__INCLUDED_)
#define AFX_FUJIEZ3FLASH1G_H__B145F554_B1E0_463A_B959_D7AA3339DB06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ezflashbase.h"

class CFujiEZ3Flash1g : public CEzFlashBase  
{
public:
	CFujiEZ3Flash1g();
	virtual ~CFujiEZ3Flash1g();

	DWORD  EXECFLH_ReadID(HANDLE hDev);
	void   EXECFLH_ReadArray(HANDLE hDev);
	WORD   EXECFLH_ReadStatus(HANDLE hDev);
	void   EXECFLH_ClearStatus(HANDLE hDev);
	void   EXECFLH_BlockErase(HANDLE hDev,DWORD BlockNum);
	void   EXECFLH_Read(HANDLE hDev, DWORD StartAddr, BYTE* pbuf, DWORD ByteCount);
	void   EXECFLH_Write(HANDLE hDev, DWORD StartAddr, BYTE* pbuf, DWORD ByteCount);

};

#endif // !defined(AFX_FUJIEZ3FLASH1G_H__B145F554_B1E0_463A_B959_D7AA3339DB06__INCLUDED_)
