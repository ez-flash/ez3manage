// AmdEz3FlashBase.cpp: implementation of the CAmdEz3FlashBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AmdEz3FlashBase.h"
#include <winioctl.h>
#include "ezusbsys.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAmdEz3FlashBase::CAmdEz3FlashBase()
{

}

CAmdEz3FlashBase::~CAmdEz3FlashBase()
{

}

DWORD  CAmdEz3FlashBase::EXECFLH_ReadID(HANDLE hDev)
{
	WORD id1,id2,id3,id4;
	WriteDevice(hDev,0x555,0xaa);
	WriteDevice(hDev,0x2aa,0x55);
	WriteDevice(hDev,0x555,0x90);

	WriteDevice(hDev,0x1555,0xaa);
	WriteDevice(hDev,0x12aa,0x55);
	WriteDevice(hDev,0x1555,0x90);

	WriteDevice(hDev,0x2555,0xaa);
	WriteDevice(hDev,0x22aa,0x55);
	WriteDevice(hDev,0x2555,0x90);

	WriteDevice(hDev,0x3555,0xaa);
	WriteDevice(hDev,0x32aa,0x55);
	WriteDevice(hDev,0x3555,0x90);

	id1=ReadDevice(hDev,1);
	id2=ReadDevice(hDev,0x1001);
	id3=ReadDevice(hDev,0x2001);
	id4=ReadDevice(hDev,0x3001);
	if((id1!=0x227E)|(id2!=0x227E)|(id3!=0x227E)|(id4!=0x227E))
		return 0xFFFFFFFF ;
	id1=ReadDevice(hDev,0xE);
	id2=ReadDevice(hDev,0x100e);
	id3=ReadDevice(hDev,0x200e);
	id4=ReadDevice(hDev,0x300e);
	if((id1==0x2221)|(id2==0x2221)|(id3==0x2221)|(id4==0x2221))
		return 0x7E217E21 ;
	if((id1!=0x2202)|(id2!=0x2202)|(id3!=0x2202)|(id4!=0x2202))
		return 0xFFFFFFFF ;

	return 0x7E027E02;
}

void  CAmdEz3FlashBase::EXECFLH_ReadArray(HANDLE hDev)
{
	WriteDevice(hDev,0x0,0xF0);
	WriteDevice(hDev,0x1000,0xF0);
	WriteDevice(hDev,0x2000,0xF0);
	WriteDevice(hDev,0x3000,0xF0);

	WriteDevice(hDev,0xff0000,0xd2FF);
	WriteDevice(hDev,0x000000,0x15FF);
	WriteDevice(hDev,0x010000,0xd2FF);
	WriteDevice(hDev,0x020000,0x15FF);
	WriteDevice(hDev,0xd20000,0xe2FF);//串行
	WriteDevice(hDev,0xfe0000,0x15FF);
	m_uiMode = 0 ;

}

WORD  CAmdEz3FlashBase::EXECFLH_ReadStatus(HANDLE hDev)
{
	return 0x8080 ;
}

void  CAmdEz3FlashBase::EXECFLH_ClearStatus(HANDLE hDev)
{
	return ;
}

void  CAmdEz3FlashBase::EXECFLH_BlockErase(HANDLE hDev,DWORD BlockNum)
{//同之前的一样的
	int i = 0 ;
	BYTE ret[64] ;
	WORD status =0;
	EXECFLH_ReadArray(hDev);
	if(BlockNum ==0)
	{
		for(i=0;i<8;i++)
		{
			WriteDevice(hDev,0x555,0xaa) ;
			WriteDevice(hDev,0x2AA,0x55) ;
			WriteDevice(hDev,0x555,0x80) ;
			WriteDevice(hDev,0x555,0xaa) ;
			WriteDevice(hDev,0x2AA,0x55) ;
			WriteDevice(hDev,i*0x4000,0x30);

			WriteDevice(hDev,0x1000+0x555,0xaa) ;
			WriteDevice(hDev,0x1000+0x2AA,0x55) ;
			WriteDevice(hDev,0x1000+0x555,0x80) ;
			WriteDevice(hDev,0x1000+0x555,0xaa) ;
			WriteDevice(hDev,0x1000+0x2AA,0x55) ;
			WriteDevice(hDev,i*0x4000+0x1000,0x30);

			WriteDevice(hDev,0x2000+0x555,0xaa) ;
			WriteDevice(hDev,0x2000+0x2AA,0x55) ;
			WriteDevice(hDev,0x2000+0x555,0x80) ;
			WriteDevice(hDev,0x2000+0x555,0xaa) ;
			WriteDevice(hDev,0x2000+0x2AA,0x55) ;
			WriteDevice(hDev,i*0x4000+0x2000,0x30);

			WriteDevice(hDev,0x3000+0x555,0xaa) ;
			WriteDevice(hDev,0x3000+0x2AA,0x55) ;
			WriteDevice(hDev,0x3000+0x555,0x80) ;
			WriteDevice(hDev,0x3000+0x555,0xaa) ;
			WriteDevice(hDev,0x3000+0x2AA,0x55) ;
			WriteDevice(hDev,i*0x4000+0x3000,0x30);
			do
			{
				
				EXECFLH_Read(hDev,i*0x4000,ret,64) ;
			}
			while(ret[0]!=ret[2]);
			do
			{
				
				EXECFLH_Read(hDev,i*0x4000+0x1000,ret,64) ;
			}
			while(ret[0]!=ret[2]);
			do
			{
				
				EXECFLH_Read(hDev,i*0x4000+0x2000,ret,64) ;
			}
			while(ret[0]!=ret[2]);
			do
			{
				EXECFLH_Read(hDev,i*0x4000+0x3000,ret,64) ;
			}
			while(ret[0]!=ret[2]);
		}
	}
	else if(BlockNum ==254)
	{
		for(i=0;i<8;i++)
		{
			WriteDevice(hDev,0x555,0xaa) ;
			WriteDevice(hDev,0x2AA,0x55) ;
			WriteDevice(hDev,0x555,0x80) ;
			WriteDevice(hDev,0x555,0xaa) ;
			WriteDevice(hDev,0x2AA,0x55) ;
			WriteDevice(hDev,0xFE0000+i*0x4000,0x30);

			WriteDevice(hDev,0x1000+0x555,0xaa) ;
			WriteDevice(hDev,0x1000+0x2AA,0x55) ;
			WriteDevice(hDev,0x1000+0x555,0x80) ;
			WriteDevice(hDev,0x1000+0x555,0xaa) ;
			WriteDevice(hDev,0x1000+0x2AA,0x55) ;
			WriteDevice(hDev,0xFE0000+i*0x4000+0x1000,0x30);

			WriteDevice(hDev,0x2000+0x555,0xaa) ;
			WriteDevice(hDev,0x2000+0x2AA,0x55) ;
			WriteDevice(hDev,0x2000+0x555,0x80) ;
			WriteDevice(hDev,0x2000+0x555,0xaa) ;
			WriteDevice(hDev,0x2000+0x2AA,0x55) ;
			WriteDevice(hDev,0xFE0000+i*0x4000+0x2000,0x30);

			WriteDevice(hDev,0x3000+0x555,0xaa) ;
			WriteDevice(hDev,0x3000+0x2AA,0x55) ;
			WriteDevice(hDev,0x3000+0x555,0x80) ;
			WriteDevice(hDev,0x3000+0x555,0xaa) ;
			WriteDevice(hDev,0x3000+0x2AA,0x55) ;
			WriteDevice(hDev,0xFE0000+i*0x4000+0x3000,0x30);
			do
			{
				LDRFLH_Read(hDev,0xFE0000+i*0x4000,ret,64) ;
			}
			while(ret[0]!=ret[2]);
			do
			{
				LDRFLH_Read(hDev,0xFE0000+i*0x4000+0x2000,ret,64) ;
			}
			while(ret[0]!=ret[2]);
			do
			{
				LDRFLH_Read(hDev,0xFE0000+i*0x4000+0x3000,ret,64) ;
			}
			while(ret[0]!=ret[2]);
		}
	}
	else
	{
		WriteDevice(hDev,0x555,0xAA) ;
		WriteDevice(hDev,0x2AA,0x55) ;
		WriteDevice(hDev,0x555,0x80) ;
		WriteDevice(hDev,0x555,0xAA) ;
		WriteDevice(hDev,0x2AA,0x55) ;
		WriteDevice(hDev,(BlockNum<<16),0x30) ;

		WriteDevice(hDev,0x1000+0x555,0xAA) ;
		WriteDevice(hDev,0x1000+0x2AA,0x55) ;
		WriteDevice(hDev,0x1000+0x555,0x80) ;
		WriteDevice(hDev,0x1000+0x555,0xAA) ;
		WriteDevice(hDev,0x1000+0x2AA,0x55) ;
		WriteDevice(hDev,0x1000+(BlockNum<<16),0x30) ;

		WriteDevice(hDev,0x2000+0x555,0xAA) ;
		WriteDevice(hDev,0x2000+0x2AA,0x55) ;
		WriteDevice(hDev,0x2000+0x555,0x80) ;
		WriteDevice(hDev,0x2000+0x555,0xAA) ;
		WriteDevice(hDev,0x2000+0x2AA,0x55) ;
		WriteDevice(hDev,0x2000+(BlockNum<<16),0x30) ;

		WriteDevice(hDev,0x3000+0x555,0xAA) ;
		WriteDevice(hDev,0x3000+0x2AA,0x55) ;
		WriteDevice(hDev,0x3000+0x555,0x80) ;
		WriteDevice(hDev,0x3000+0x555,0xAA) ;
		WriteDevice(hDev,0x3000+0x2AA,0x55) ;
		WriteDevice(hDev,0x3000+(BlockNum<<16),0x30) ;
		do
		{
//			EXECFLH_ReadID(hDev);
			EXECFLH_Read(hDev,(BlockNum<<16),ret,64) ;
		}
		while(ret[0]!=ret[2]);
		do
		{
			EXECFLH_Read(hDev,(BlockNum<<16)+0x1000,ret,64) ;
		}
		while(ret[0]!=ret[2]);
		do
		{
			EXECFLH_Read(hDev,(BlockNum<<16)+0x2000,ret,64) ;
		}
		while(ret[0]!=ret[2]);
		do
		{
			EXECFLH_Read(hDev,(BlockNum<<16)+0x3000,ret,64) ;
		}
		while(ret[0]!=ret[2]);
	}
}

void CAmdEz3FlashBase::EXECFLH_Read(HANDLE hDev, DWORD StartAddr, BYTE* pbuf, DWORD ByteCount)
{
	ctrlbuf[0] = ROM_Read;
	*(DWORD *)&ctrlbuf[1] = StartAddr;
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[0],ctrlbuf,4);
	Sleep(5);
	Bulk(hDev,IOCTL_EZUSB_BULK_READ,pUsepipe[1],pbuf,ByteCount);
}

void CAmdEz3FlashBase::EXECFLH_Write(HANDLE hDev, DWORD StartAddr, BYTE* pbuf, DWORD ByteCount)
{
	BYTE *pBlock1,*pBlock2,*pBlock3,*pBlock4;
	if(m_uiMode==0)
	{
		WriteDevice(hDev,0xff0000,0xd2FF);
		WriteDevice(hDev,0x000000,0x15FF);
		WriteDevice(hDev,0x010000,0xd2FF);
		WriteDevice(hDev,0x020000,0x15FF);
		WriteDevice(hDev,0x150000,0xe2FF);//并行模式
		WriteDevice(hDev,0xfe0000,0x15FF);
		m_uiMode = 1 ;

	}
	ASSERT(ByteCount==32768) ;
	pBlock1 = pbuf ;
	pBlock2 = &(pbuf[0x2000]) ;
	pBlock3 = &(pbuf[0x4000]);
	pBlock4 = &(pbuf[0x6000]);
	for(int i = 0 ;i<4096;i++)
	{
		m_pByte[i*8+0]=pBlock1[i*2] ;
		m_pByte[i*8+1]=pBlock1[i*2+1] ;
		m_pByte[i*8+2]=pBlock2[i*2+0] ;
		m_pByte[i*8+3]=pBlock2[i*2+1] ;
		m_pByte[i*8+4]=pBlock3[i*2+0] ;
		m_pByte[i*8+5]=pBlock3[i*2+1] ;
		m_pByte[i*8+6]=pBlock4[i*2+0] ;
		m_pByte[i*8+7]=pBlock4[i*2+1] ;
	}

	ctrlbuf[0] = ROM_Write_UMAC;
	*(DWORD *)&ctrlbuf[1] = StartAddr;
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[0],ctrlbuf,5);
	Sleep(5);
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[2],m_pByte,ByteCount);
}