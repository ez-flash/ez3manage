// ezflashbase.cpp: implementation of the CEzFlashBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ezflashbase.h"
#include <winioctl.h>
#include "ezusbsys.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma warning (disable:4244)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEzFlashBase::CEzFlashBase()
{
	ol = new OVERLAPPED;
	ol->Offset = 0;
	ol->OffsetHigh = 0;
	hEve = CreateEvent(NULL,false,false,NULL);
	ol->hEvent = hEve;
	m_wEz3OPMode = 0x8000;
	m_uiMode = 0 ;
	m_pByte = new BYTE[32768];
	m_wNandMode = 0 ;
//fx2
	pipe[0][0]=3;pipe[0][1]=8;pipe[0][2]=1;pipe[0][3]=10;
	pipe[1][0]=0;pipe[1][1]=3;pipe[1][2]=2;pipe[1][3]=1;
	pipe[2][0]=2;pipe[2][1]=1;pipe[2][2]=0;pipe[2][3]=3;
	m_readsize =0;
	m_NANDSize = 0 ;
}

CEzFlashBase::~CEzFlashBase()
{
	delete []m_pByte ;
	m_pByte = NULL ;
	if(ol)
	{
		if(ol->hEvent)
		{
			CloseHandle(ol->hEvent);
		}
		delete ol ;
	}
}

//发送控制代码到设备驱动程序，引起相应的设备执行相应的操作。
DWORD  CEzFlashBase::Bulk(HANDLE HDev, 
						 unsigned long ControlCode, 
						 unsigned long pipeNum,
						 LPVOID buf, 
						 unsigned long ByteCount)
{
	DWORD err =0 ;
	BULK_TRANSFER_CONTROL btc;
	unsigned long junk;
	btc.pipeNum = pipeNum;
	//发送控制代码到设备驱动程序，引起相应的设备执行相应的操作。
	if(!DeviceIoControl(HDev,			//设备的句柄
						ControlCode,	//操作控制代码
						&btc,			//输入数据缓存
						sizeof(BULK_TRANSFER_CONTROL),//
						buf,			//输出数据缓存
						ByteCount,		//输出数据缓存大小
						&junk,			//
						ol
						))
	{
		err = GetLastError();
		return err ;
	}

	if(err = WaitForSingleObject(hEve,4000)!=WAIT_OBJECT_0)
	{
		return err ;
	}
	return 0 ;
}

void CEzFlashBase::WriteDevice(HANDLE &hDev,UINT address,WORD data)
{
	ctrlbuf[0] = Write_Operation;
	ctrlbuf[1] = (address)&0xFF;
	ctrlbuf[2] = (address>>8)&0xFF;
	ctrlbuf[3] = (address>>16)&0xFF;
	*(WORD *)&ctrlbuf[4] = data;

	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[0],ctrlbuf,6);

}

void CEzFlashBase::WriteClockDevice(HANDLE &hDev,UINT address,WORD data)
{
	ctrlbuf[0] = WriteClock_Operation;
	ctrlbuf[1] = (address)&0xFF;
	ctrlbuf[2] = (address>>8)&0xFF;
	ctrlbuf[3] = (address>>16)&0xFF;
	ctrlbuf[4] = data&0xFF;
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[0],ctrlbuf,5);
}

WORD CEzFlashBase::ReadDevice(HANDLE &hDev,UINT address)
{
	ctrlbuf[0] = Read_Operation;
	ctrlbuf[1] = (address)&0xFF;
	ctrlbuf[2] = (address>>8)&0xFF;
	ctrlbuf[3] = (address>>16)&0xFF;
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[0],ctrlbuf,4);
	Bulk(hDev,IOCTL_EZUSB_BULK_READ,pUsepipe[3],ctrlbuf,m_readsize);
	return(*(WORD *)ctrlbuf);

}

//---------------------------------------------------------------------------
void CEzFlashBase::CartOpen(HANDLE hDev)
{
	ctrlbuf[0] = Open_Port;
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[0],ctrlbuf,1);
}
//---------------------------------------------------------------------------
void CEzFlashBase::CartClose(HANDLE hDev)
{
	ctrlbuf[0] = Close_Port;
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[0],ctrlbuf,1);
}

bool CEzFlashBase::OpenCartDevice(HANDLE &hDev)
{
	hDev = CreateFile("\\\\.\\Ez3wt-0",
					  GENERIC_READ || GENERIC_WRITE,
					  FILE_SHARE_READ || FILE_SHARE_WRITE,
					  NULL,
					  OPEN_EXISTING,
					  FILE_FLAG_OVERLAPPED,
					  NULL
					  );
	if(hDev == INVALID_HANDLE_VALUE)
	{
		hDev = CreateFile("\\\\.\\Ezw-0",
						  GENERIC_READ || GENERIC_WRITE,
						  FILE_SHARE_READ || FILE_SHARE_WRITE,
						  NULL,
						  OPEN_EXISTING,
						  FILE_FLAG_OVERLAPPED,
						  NULL
						  );
		//对于2131的卡
		if(hDev == INVALID_HANDLE_VALUE)
		{
			hDev = CreateFile("\\\\.\\ez321000",
					  GENERIC_READ || GENERIC_WRITE,
					  FILE_SHARE_READ || FILE_SHARE_WRITE,
					  NULL,
					  OPEN_EXISTING,
					  FILE_FLAG_OVERLAPPED,
					  NULL
					  );
			if(hDev == INVALID_HANDLE_VALUE)
				return false;
			else
			{
				//对于3210的卡
				pUsepipe = pipe[2] ;
				m_readsize = 16;
				return true;
			}
		}
		else
		{
			pUsepipe = pipe[0] ;
			m_readsize = 64;
			return true;
		}
	}

	pUsepipe = pipe[1] ;
	m_readsize = 64;
	return true ;
}

bool CEzFlashBase::CloseCartDevice(HANDLE &hDev)
{
	BOOL ret ;
	ret = CloseHandle(hDev);
	return ret?true:false;
}

void CEzFlashBase::CartOpenFlashOP(HANDLE hDev)
{
	WriteDevice(hDev,0xff0000,0xd2FF);
	WriteDevice(hDev,0x000000,0x15FF);
	WriteDevice(hDev,0x010000,0xd2FF);
	WriteDevice(hDev,0x020000,0x15FF);
	WriteDevice(hDev,0xe20000,0x15FF);
	WriteDevice(hDev,0xfe0000,0x15FF);

}
//---------------------------------------------------------------------------
void CEzFlashBase::CartCloseFlashOP(HANDLE hDev)
{
	WriteDevice(hDev,0xff0000,0xd2FF);
	WriteDevice(hDev,0x000000,0x15FF);
	WriteDevice(hDev,0x010000,0xd2FF);
	WriteDevice(hDev,0x020000,0x15FF);
	WriteDevice(hDev,0xe20000,0xd2FF);
	WriteDevice(hDev,0xfe0000,0x15FF);
}

//---------------------------------------------------------------------------
void CEzFlashBase::CartRAMRead(HANDLE hDev, WORD StartAddr, BYTE* pbuf, unsigned long ByteCount)
{
	ctrlbuf[0] = RAM_Read;
	*(WORD *)&ctrlbuf[1] = StartAddr;
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[0],ctrlbuf,3);
	Sleep(5);
	Bulk(hDev,IOCTL_EZUSB_BULK_READ,pUsepipe[1],pbuf,ByteCount);
}
//---------------------------------------------------------------------------
void CEzFlashBase::CartRAMWrite(HANDLE hDev, WORD StartAddr, BYTE* pbuf, unsigned long ByteCount)
{
	ctrlbuf[0] = RAM_Write;
	*(WORD *)&ctrlbuf[1] = StartAddr;
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[0],ctrlbuf,3);
	Sleep(5);
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[2],pbuf,ByteCount);
}
//---------------------------------------------------------------------------
void CEzFlashBase::CartSetRAMPage(HANDLE hDev,DWORD Offset)
{
/*	ctrlbuf[0] = Set_SRAM_Page;
	ctrlbuf[1] = (BYTE)Offset&0xFF;
	ctrlbuf[2] = (BYTE)(Offset>>8)&0xFF;
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,3,ctrlbuf,3);
	*/
	WriteDevice(hDev,0xff0000,0xd2FF);
	WriteDevice(hDev,0x000000,0x15FF);
	WriteDevice(hDev,0x010000,0xd2FF);
	WriteDevice(hDev,0x020000,0x15FF);
	WriteDevice(hDev,0xe00000,(WORD)Offset);
	WriteDevice(hDev,0xfe0000,0x15FF);
}

//////////////////////nand flash的操作///////////////////
#define EZ3_NAND_COMMAND 0xFFFFF1
#define EZ3_NAND_ADDRESS 0xFFFFF0
#define EZ3_NAND_DATA	 0xFFE000

DWORD  CEzFlashBase::NAND_ReadNANDID(HANDLE hDev)
{
	BYTE retID[4];
	WriteDevice(hDev,EZ3_NAND_COMMAND,0x90);//
	WriteDevice(hDev,EZ3_NAND_ADDRESS,0x01);//
	if(pUsepipe==pipe[0])
	{
		ReadDevice(hDev,EZ3_NAND_DATA);
		retID[0] = ctrlbuf[0];
		retID[1] = ctrlbuf[2];
		retID[2] = ctrlbuf[4];
		retID[3] = ctrlbuf[6];
	}
	else
	{
		retID[0]=(0xff&ReadDevice(hDev,EZ3_NAND_DATA));
		retID[1]=(0xFF&ReadDevice(hDev,EZ3_NAND_DATA));
		retID[2]=(0xFF&ReadDevice(hDev,EZ3_NAND_DATA));
		retID[3]=(0xFF&ReadDevice(hDev,EZ3_NAND_DATA));
	}
	m_NANDSize = 0 ;
	if((*(DWORD*)retID==0x1580daec)||(*(DWORD*)retID==0x1580daad))
		m_NANDSize = 2 ;
	else if((*(DWORD*)retID==0x1580dcec)||(*(DWORD*)retID==0x1580dcad))
		m_NANDSize = 4 ;
	else if((*(DWORD*)retID==0x15C1dcec)||(*(DWORD*)retID==0x15C1dcad)||(*(DWORD*)retID==0x9551d3ec))
		m_NANDSize = 8 ;
	return *(DWORD*)retID ;
}

WORD CEzFlashBase::NAND_ReadNANDStatus(HANDLE hDev)
{
	WORD dd ;
	WriteDevice(hDev,EZ3_NAND_COMMAND,0x70);//
	dd = ReadDevice(hDev,EZ3_NAND_DATA);
	return dd ;
}

void CEzFlashBase::NAND_Reset(HANDLE hDev)
{
	WriteDevice(hDev,EZ3_NAND_COMMAND,0xFF);//
}

void CEzFlashBase::NAND_ReadFlashBase(HANDLE hDev,DWORD address,BYTE *pbuf,DWORD ByteCount)
{
	DWORD a1,a2;
	BYTE add1 = 0 , add2 = 0 , add3 = 0 , add4 = 0 , add5=0;
	a1 = (DWORD)(address%2048)&0xFFFF; //低位
	a2 = (DWORD)(address/2048); //高位
	add1 =a1&0xFF ;
	add2 =(a1>>8)&0xFF;
	add3 =a2&0xFF ;
	add4 =(a2>>8)&0xFF;
	add5 =(a2>>16)&0xFF ;

	ctrlbuf[0] = NandFlash_Read;
	ctrlbuf[1] = 1+m_NANDSize ;//1:with spare 0 withou spare, bit 1 , 2 Gbit , bit 2 , 4gbit
	ctrlbuf[2] = add1;
	ctrlbuf[3] = add2;
	ctrlbuf[4] = add3;
	ctrlbuf[5] = add4;
	ctrlbuf[6] = add5;
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[0],ctrlbuf,7);
	Sleep(5);
	if(m_readsize == 64)
	{
		//2131的卡
		Bulk(hDev,IOCTL_EZUSB_BULK_READ,pUsepipe[1],pbuf,ByteCount);
	}
	else
	{
		unsigned long size;
		int i=7;
		int k=i/2;
		int j=i%2;
		if(ByteCount%128!=0)
		{
			size = (ByteCount/128+1)*128;
		}
		else
		{
			size = ByteCount;
		}
		BYTE *pTemp = new BYTE[size];
		Bulk(hDev,IOCTL_EZUSB_BULK_READ,pUsepipe[1],pTemp,size);
		memcpy(pbuf,pTemp,ByteCount);
		delete []pTemp;
	}

}

void CEzFlashBase::NAND_ReadFlash16(HANDLE hDev,DWORD address,BYTE *pbuf,DWORD ByteCount)
{
	WORD a1,a2;
	BYTE add1 = 0 , add2 = 0 , add3 = 0 , add4 = 0 ;
	a1 = (WORD)(address%2048)&0xFFFF; //低位
	a2 = (WORD)(address/2048); //高位
	add1 =a1&0xFF ;
	add2 =(a1>>8)&0xFF;
	add3 =a2&0xFF ;
	add4 =(a2>>8)&0xFF;

	ctrlbuf[0] = NandFlash_Read16;
	ctrlbuf[1] = 1 ;//1:with spare 0 withou spare
	ctrlbuf[2] = add1;
	ctrlbuf[3] = add2;
	ctrlbuf[4] = add3;
	ctrlbuf[5] = add4;
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,3,ctrlbuf,6);
	Bulk(hDev,IOCTL_EZUSB_BULK_READ,8,pbuf,ByteCount);

}
void CEzFlashBase::NAND_ReadFlash(HANDLE hDev,DWORD address,BYTE *pbuf,DWORD ByteCount)
{
	if(m_readsize == 64)
		NAND_ReadFlashBase(hDev,address,pbuf,ByteCount);
	else
	{
		BYTE *ptemp = new BYTE[2112+64];
		for(int i=0;i<ByteCount/2112;i++)
		{
			NAND_ReadFlashBase(hDev,address+i*2048,ptemp,2112+64);
			memcpy(&pbuf[i*2112],ptemp,2112);
		}
		delete []ptemp;
		
	}
}

void CEzFlashBase::NAND_WriteFlash(HANDLE hDev,DWORD address,BYTE *pbuf,DWORD ByteCount)
{
	/*
	if(m_readsize == 64)
		NAND_WriteFlashBase(hDev,address,pbuf,ByteCount);
	else
	{
		BYTE *ptemp = new BYTE[2112+64];
		for(int i=0;i<ByteCount/2112;i++)
		{
			memcpy(ptemp,&pbuf[i*2112],2112);
			NAND_WriteFlashBase(hDev,i*2048+address,ptemp,2112+64);
		}
		delete []ptemp;
	}
	*/
	NAND_WriteFlashBase(hDev,address,pbuf,ByteCount);
}

void CEzFlashBase::NAND_WriteFlashBase(HANDLE hDev,DWORD address,BYTE *pbuf,DWORD ByteCount)
{
	DWORD a1,a2;
	BYTE add1 = 0 , add2 = 0 , add3 = 0 , add4 = 0 ,add5=0;
	a1 = (WORD)(address%2048)&0xFFFF; //低位
	a2 = (DWORD)(address/2048)/*&0xFFFF*/; //高位

	add1 =a1&0xFF ;
	add2 =(a1>>8)&0xFF;
	add3 =a2&0xFF ;
	add4 =(a2>>8)&0xFF;
	add5 = (a2>>16)&0xFF;
	ctrlbuf[0] = NandFlash_Write;
	ctrlbuf[1] = 1+m_NANDSize ;//1:with spare 0 withou spare
	ctrlbuf[2] = add1;
	ctrlbuf[3] = add2;
	ctrlbuf[4] = add3;
	ctrlbuf[5] = add4;
	ctrlbuf[6] = add5;

	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[0],ctrlbuf,7);
	Sleep(5);
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[2],pbuf,ByteCount);
}
BOOL CEzFlashBase::NAND_BlockErase(HANDLE hDev,DWORD address)
{
	
	DWORD a1,a2;
	BYTE add1 = 0 , add2 = 0 , add3 = 0 , add4 = 0 , add5 =0 ;
	a1 = (WORD)(address%2048)&0xFFFF; //低位
	a2 = (DWORD)(address/2048)/*&0xFFFF*/; //高位
	add1 =a1&0xFF ;
	add2 =(a1>>8)&0xFF;
	add3 =a2&0xFF ;
	add4 =(a2>>8)&0xFF;
	add5 =(a2>>16)&0xFF ;
	WriteDevice(hDev,EZ3_NAND_COMMAND,0xFF);//命令-reset
	WriteDevice(hDev,EZ3_NAND_COMMAND,0x60);//命令-reset
	WriteDevice(hDev,EZ3_NAND_ADDRESS,add3);//命令-写地址
	WriteDevice(hDev,EZ3_NAND_ADDRESS,add4);//命令-写地址
	if(m_NANDSize)
		WriteDevice(hDev,EZ3_NAND_ADDRESS,add5);//命令-写地址
	WriteDevice(hDev,EZ3_NAND_COMMAND,0xD0);//命令-reset

	WriteDevice(hDev,CLOCK_DEVICE,1) ;
//	do{
//		a1 = ReadDevice(hDev,CLOCK_DATA);
//	}while(!(a1&0x8));
	WriteDevice(hDev,CLOCK_DEVICE,0) ;

	int loop=0;
	do
	{
		if(loop!=10)
		{
			Sleep(5);
			loop++;
		}
		else
			return FALSE;
		add1 = (BYTE)NAND_ReadNANDStatus(hDev);
	}
	while((add1&0xC1)!=0xc0);
	return TRUE;
}

void CEzFlashBase::NAND_ENABLE(HANDLE hDev,BOOL fEnable)
{
	m_wNandMode &= 0xFFFE ;
	m_wNandMode |= (fEnable?5:0) ;
	if(fEnable)
		m_wNandMode |= 5 ;
	else
		m_wNandMode = 0 ;

	WriteDevice(hDev,0xff0000,0xd2FF);
	WriteDevice(hDev,0x000000,0x15FF);
	WriteDevice(hDev,0x010000,0xd2FF);
	WriteDevice(hDev,0x020000,0x15FF);
	WriteDevice(hDev,0xA00000,m_wNandMode);
	WriteDevice(hDev,0xfe0000,0x15FF);
}

void CEzFlashBase::NAND_SetReadMode(HANDLE hDev,BYTE mode)
{
	m_wNandMode &= 0xFFFD ;
	m_wNandMode |= ((mode>8)?6:0) ;
	WriteDevice(hDev,0xff0000,0xd2FF);
	WriteDevice(hDev,0x000000,0x15FF);
	WriteDevice(hDev,0x010000,0xd2FF);
	WriteDevice(hDev,0x020000,0x15FF);
	WriteDevice(hDev,0xA00000,m_wNandMode);
	WriteDevice(hDev,0xfe0000,0x15FF);
}

void CEzFlashBase::CartSetROMPage(HANDLE hDev,DWORD Offset)
{
	WriteDevice(hDev,0xff0000,0xd2FF);
	WriteDevice(hDev,0x000000,0x15FF);
	WriteDevice(hDev,0x010000,0xd2FF);
	WriteDevice(hDev,0x020000,0x15FF);
	WriteDevice(hDev,0xc40000,(WORD)(Offset|m_wEz3OPMode));
	WriteDevice(hDev,0xfe0000,0x15FF);

}

void CEzFlashBase::CartSetEZControl(HANDLE hDev,WORD control)
{
	WriteDevice(hDev,0xff0000,0xd2FF);
	WriteDevice(hDev,0x000000,0x15FF);
	WriteDevice(hDev,0x010000,0xd2FF);
	WriteDevice(hDev,0x020000,0x15FF);
	WriteDevice(hDev,0xF00000,control);
	WriteDevice(hDev,0xfe0000,0x15FF);
}

void CEzFlashBase::SetMapMode(WORD wMode)
{
	m_wEz3OPMode = wMode ;
}

DWORD  CEzFlashBase::LDRFLH_ReadID(HANDLE hDev)
{
	WORD id1,id2;
	WriteDevice(hDev,0x0,0x90);
	id1=ReadDevice(hDev,0);
	id2=ReadDevice(hDev,1);
	return id1+(id2<<16);
}

void  CEzFlashBase::LDRFLH_ReadArray(HANDLE hDev)
{
	WriteDevice(hDev,0x0,0xFF);
}

WORD  CEzFlashBase::LDRFLH_ReadStatus(HANDLE hDev)
{
	WORD sts ;
	WriteDevice(hDev,0x0,0x70);
	sts=ReadDevice(hDev,0);
	return sts ;
}

void  CEzFlashBase::LDRFLH_ClearStatus(HANDLE hDev)
{
	WriteDevice(hDev,0x0,0x50);
}

void  CEzFlashBase::LDRFLH_BlockErase(HANDLE hDev,DWORD dwBlock)
{
	int i = 0 ;
	WORD status =0;
	if(dwBlock ==0)
	{
		for(i=0;i<8;i++)
		{
			WriteDevice(hDev,0x1000*i,0xFF);
			Sleep(5);
			WriteDevice(hDev,0x1000*i,0x20);
			WriteDevice(hDev,0x1000*i,0xd0);
			do
			{
				status = ReadDevice(hDev,0x1000*i);
			}
			while(status!=0x80);
		}
	}
	else
	{
		WriteDevice(hDev,0x8000*dwBlock,0xFF);
		Sleep(5);
		WriteDevice(hDev,0x8000*dwBlock,0x20);
		WriteDevice(hDev,0x8000*dwBlock,0xd0);
		do
		{
			status = ReadDevice(hDev,0x8000*dwBlock);
		}
		while(status!=0x80);
	}

}

void CEzFlashBase::LDRFLH_Read(HANDLE hDev, DWORD StartAddr, BYTE* pbuf, DWORD ByteCount)
{
	ctrlbuf[0] = ROM_Read;
	*(DWORD *)&ctrlbuf[1] = StartAddr;
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[0],ctrlbuf,4);
	Sleep(5);
	Bulk(hDev,IOCTL_EZUSB_BULK_READ,pUsepipe[1],pbuf,ByteCount);
}


void CEzFlashBase::LDRFLH_Write(HANDLE hDev, DWORD StartAddr, BYTE* pbuf, DWORD ByteCount)
{
	ctrlbuf[0] = LDRFlash_Write;
	*(DWORD *)&ctrlbuf[1] = StartAddr;
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[0],ctrlbuf,5);
	Sleep(5);
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[2],pbuf,ByteCount);
}

DWORD  CEzFlashBase::EXECFLH_ReadID(HANDLE hDev)
{	
	return 0;
}

void  CEzFlashBase::EXECFLH_ReadArray(HANDLE hDev)
{
	return;
}

WORD  CEzFlashBase::EXECFLH_ReadStatus(HANDLE hDev)
{
	return 0x8080 ;
}

void  CEzFlashBase::EXECFLH_ClearStatus(HANDLE hDev)
{
	return ;
}

void  CEzFlashBase::EXECFLH_BlockErase(HANDLE hDev,DWORD BlockNum)
{//同之前的一样的
	return;
}

void CEzFlashBase::EXECFLH_Read(HANDLE hDev, DWORD StartAddr, BYTE* pbuf, DWORD ByteCount)
{
	return;
}

void CEzFlashBase::EXECFLH_Write(HANDLE hDev, DWORD StartAddr, BYTE* pbuf, DWORD ByteCount)
{
	return;
}

void CEzFlashBase::_Read(HANDLE hDev, DWORD StartAddr, BYTE* pbuf, DWORD ByteCount)
{
	ctrlbuf[0] = ROM_Read;
	*(DWORD *)&ctrlbuf[1] = StartAddr;
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[0],ctrlbuf,4);
	Sleep(5);
	Bulk(hDev,IOCTL_EZUSB_BULK_READ,pUsepipe[1],pbuf,ByteCount);
}

void   CEzFlashBase::_Write(HANDLE hDev, DWORD StartAddr, BYTE* pbuf, DWORD ByteCount)
{
	ctrlbuf[0] = ROM_Write;
	*(DWORD *)&ctrlbuf[1] = StartAddr;
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[0],ctrlbuf,5);
	Sleep(10);
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[2],pbuf,ByteCount);
}

void CEzFlashBase::WriteI2C_op(HANDLE &hDev,BYTE address,BYTE length,BYTE*data)
{//length <60
	ctrlbuf[0] = WriteI2C;
	ctrlbuf[1] = address;
	ctrlbuf[2] = length;
	for(int i=0;i<length;i++)
		ctrlbuf[4+i]=data[i];
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[0],ctrlbuf,length+4);
}

WORD CEzFlashBase::ReadI2C_op(HANDLE &hDev,BYTE address,BYTE length,BYTE*data)
{
	ctrlbuf[0] = ReadI2C;
	ctrlbuf[1] = address;
	ctrlbuf[2] = length;
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[0],ctrlbuf,3);
	Bulk(hDev,IOCTL_EZUSB_BULK_READ,pUsepipe[3],ctrlbuf,64);
	for(int i=0;i<length;i++)
		data[i] = ctrlbuf[i] ;
	return 0 ;
}

void CEzFlashBase::InitI2C_op(HANDLE &hDev)
{//length <60
	ctrlbuf[0] = InitI2C;
	Bulk(hDev,IOCTL_EZUSB_BULK_WRITE,pUsepipe[0],ctrlbuf,1);
}

/*************************时钟处理函数*******************************/
BYTE CEzFlashBase::ReadStatus(HANDLE &hDev )
{
	BYTE bb ;
	//首先允许时钟读写
	WriteClockDevice(hDev,CLOCK_DEVICE,DEVICE_ENABLE) ;
	//允许 CS-OUT-EN、SIO-OUT-EN、SCK-OUT-EN
	WriteClockDevice(hDev,CLOCK_CONTROL,CLOCK_CSOUT+CLOCK_SIOOUT+CLOCK_SCKOUT) ;
	//写命令字 SIO-OUT-EN = 1 
	//开始前的准备
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_SIO+CLOCK_SCK) ; //CS = 0 ;SCK = 1 ;SIO = 1;
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_CS+CLOCK_SIO+CLOCK_SCK) ; //CS = 1 ;SCK = 1 ;SIO = 1;
	//开始准备数据0110 0111 , 命令
	WriteBit(hDev,0);
	WriteBit(hDev,1);
	WriteBit(hDev,1);
	WriteBit(hDev,0);
	WriteBit(hDev,0);
	WriteBit(hDev,0);
	WriteBit(hDev,1);
	WriteBit(hDev,1);
	//开始读数据  SIO-OUT-EN = 0
	WriteClockDevice(hDev,CLOCK_CONTROL,CLOCK_SCKOUT+CLOCK_CSOUT);
	bb = ReadByte(hDev);
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_SIO+CLOCK_SCK) ; //CS = 0 ;SCK = 1 ;SIO = 1;
	//禁止 CS-OUT-EN、SIO-OUT-EN、SCK-OUT-EN
	WriteClockDevice(hDev,CLOCK_CONTROL,0) ;
	//禁止时钟芯片 
	WriteClockDevice(hDev,CLOCK_DEVICE,0) ;
	return bb ;
}


void CEzFlashBase::WriteStatus(HANDLE &hDev,BYTE status)
{
	//首先允许时钟读写
	WriteClockDevice(hDev,CLOCK_DEVICE,DEVICE_ENABLE) ;
	//允许 CS-OUT-EN、SIO-OUT-EN、SCK-OUT-EN
	WriteClockDevice(hDev,CLOCK_CONTROL,CLOCK_CSOUT+CLOCK_SIOOUT+CLOCK_SCKOUT) ;
	//写命令字 SIO-OUT-EN = 1 
	//开始前的准备
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_SIO+CLOCK_SCK) ; //CS = 0 ;SCK = 1 ;SIO = 1;
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_CS+CLOCK_SIO+CLOCK_SCK) ; //CS = 1 ;SCK = 1 ;SIO = 1;
	//开始准备数据0110 0111 , 命令
	WriteBit(hDev,0);
	WriteBit(hDev,1);
	WriteBit(hDev,1);
	WriteBit(hDev,0);
	WriteBit(hDev,0);
	WriteBit(hDev,0);
	WriteBit(hDev,1);
	WriteBit(hDev,0);
	//开始写数据  SIO-OUT-EN = 1
	
	WriteByte(hDev,status);

	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_SIO+CLOCK_SCK) ; //CS = 0 ;SCK = 1 ;SIO = 1;
	//禁止 CS-OUT-EN、SIO-OUT-EN、SCK-OUT-EN
	WriteClockDevice(hDev,CLOCK_CONTROL,0) ;
	//禁止时钟芯片 
	WriteClockDevice(hDev,CLOCK_DEVICE,0) ;
	return ;
}

void CEzFlashBase::ClockEndtst(HANDLE &hDev)
{
	//首先允许时钟读写
	WriteClockDevice(hDev,CLOCK_DEVICE,DEVICE_ENABLE) ;
	//允许 CS-OUT-EN、SIO-OUT-EN、SCK-OUT-EN
	WriteClockDevice(hDev,CLOCK_CONTROL,CLOCK_CSOUT+CLOCK_SIOOUT+CLOCK_SCKOUT) ;
	//写命令字 SIO-OUT-EN = 1 
	//开始前的准备
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_SIO+CLOCK_SCK) ; //CS = 0 ;SCK = 1 ;SIO = 1;
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_CS+CLOCK_SIO+CLOCK_SCK) ; //CS = 1 ;SCK = 1 ;SIO = 1;
	//开始准备数据0110 0111 , 命令
	WriteBit(hDev,0);
	WriteBit(hDev,1);
	WriteBit(hDev,1);
	WriteBit(hDev,0);
	WriteBit(hDev,1);
	WriteBit(hDev,1);
	WriteBit(hDev,1);
	WriteBit(hDev,1);
	//开始读数据  SIO-OUT-EN = 0
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_SIO+CLOCK_SCK) ; //CS = 0 ;SCK = 1 ;SIO = 1;
	//禁止 CS-OUT-EN、SIO-OUT-EN、SCK-OUT-EN
	WriteClockDevice(hDev,CLOCK_CONTROL,0) ;
	//禁止时钟芯片 
	WriteClockDevice(hDev,CLOCK_DEVICE,0) ;
}


void CEzFlashBase::ClockReset(HANDLE &hDev )
{
	//首先允许时钟读写
	WriteClockDevice(hDev,CLOCK_DEVICE,DEVICE_ENABLE) ;
	//允许 CS-OUT-EN、SIO-OUT-EN、SCK-OUT-EN
	WriteClockDevice(hDev,CLOCK_CONTROL,CLOCK_CSOUT+CLOCK_SIOOUT+CLOCK_SCKOUT) ;
	//写命令字 SIO-OUT-EN = 1 
	//开始前的准备
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_SIO+CLOCK_SCK) ; //CS = 0 ;SCK = 1 ;SIO = 1;
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_CS+CLOCK_SIO+CLOCK_SCK) ; //CS = 1 ;SCK = 1 ;SIO = 1;
	//开始准备数据0110 0111 , 命令
	WriteBit(hDev,0);
	WriteBit(hDev,1);
	WriteBit(hDev,1);
	WriteBit(hDev,0);
	WriteBit(hDev,0);
	WriteBit(hDev,0);
	WriteBit(hDev,0);
	WriteBit(hDev,1);
	//开始读数据  SIO-OUT-EN = 0
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_SIO+CLOCK_SCK) ; //CS = 0 ;SCK = 1 ;SIO = 1;
	//禁止 CS-OUT-EN、SIO-OUT-EN、SCK-OUT-EN
	WriteClockDevice(hDev,CLOCK_CONTROL,0) ;
	//禁止时钟芯片 
	WriteClockDevice(hDev,CLOCK_DEVICE,0) ;
}
void CEzFlashBase::Set24Hour(HANDLE hDev)
{
	BYTE bb = 0x40 ;
	//首先允许时钟读写
	WriteClockDevice(hDev,CLOCK_DEVICE,DEVICE_ENABLE) ;
	//允许 CS-OUT-EN、SIO-OUT-EN、SCK-OUT-EN
	WriteClockDevice(hDev,CLOCK_CONTROL,CLOCK_CSOUT+CLOCK_SIOOUT+CLOCK_SCKOUT) ;
	//写命令字 SIO-OUT-EN = 1 
	//开始前的准备
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_SIO+CLOCK_SCK) ; //CS = 0 ;SCK = 1 ;SIO = 1;
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_CS+CLOCK_SIO+CLOCK_SCK) ; //CS = 1 ;SCK = 1 ;SIO = 1;
	//开始准备数据0110 0111 , 命令
	WriteBit(hDev,0);
	WriteBit(hDev,1);
	WriteBit(hDev,1);
	WriteBit(hDev,0);
	WriteBit(hDev,0);
	WriteBit(hDev,0);
	WriteBit(hDev,1);
	WriteBit(hDev,0);
	//开始写数据  SIO-OUT-EN = 1
	//	
	WriteByte(hDev,bb);

	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_SIO+CLOCK_SCK) ; //CS = 0 ;SCK = 1 ;SIO = 1;
	//禁止 CS-OUT-EN、SIO-OUT-EN、SCK-OUT-EN
	WriteClockDevice(hDev,CLOCK_CONTROL,0) ;
	//禁止时钟芯片 
	WriteClockDevice(hDev,CLOCK_DEVICE,0) ;
	return ;
}

void CEzFlashBase::ReadClock(HANDLE &hDev,
							 BYTE &hour,
							 BYTE &minitue,
							 BYTE &second)
{
	 BYTE bhour;
	 BYTE bminitue;
	 BYTE bsecond;

	//首先允许时钟读写
	WriteClockDevice(hDev,CLOCK_DEVICE,DEVICE_ENABLE) ;
	//允许 CS-OUT-EN、SIO-OUT-EN、SCK-OUT-EN
	WriteClockDevice(hDev,CLOCK_CONTROL,CLOCK_CSOUT+CLOCK_SIOOUT+CLOCK_SCKOUT) ;
	//写命令字 SIO-OUT-EN = 1 
	//开始前的准备
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_SIO+CLOCK_SCK) ; //CS = 0 ;SCK = 1 ;SIO = 1;
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_CS+CLOCK_SIO+CLOCK_SCK) ; //CS = 1 ;SCK = 1 ;SIO = 1;
	//开始准备数据0110 0111 , 命令
	WriteBit(hDev,0);
	WriteBit(hDev,1);
	WriteBit(hDev,1);
	WriteBit(hDev,0);
	WriteBit(hDev,0);
	WriteBit(hDev,1);
	WriteBit(hDev,1);
	WriteBit(hDev,1);
	//开始读数据  SIO-OUT-EN = 0
	WriteClockDevice(hDev,CLOCK_CONTROL,CLOCK_SCKOUT+CLOCK_CSOUT);
	//
	bhour = ReadByte(hDev);
	bminitue = ReadByte(hDev);
	bsecond = ReadByte(hDev);

	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_SIO+CLOCK_SCK) ; //CS = 0 ;SCK = 1 ;SIO = 1;
	//禁止 CS-OUT-EN、SIO-OUT-EN、SCK-OUT-EN
	WriteClockDevice(hDev,CLOCK_CONTROL,0) ;
	//禁止时钟芯片 
	WriteClockDevice(hDev,CLOCK_DEVICE,0) ;
	hour = Clock_FromTime(bhour);
	minitue = Clock_FromTime(bminitue);
	second = Clock_FromTime(bsecond);
	return ;
}

void CEzFlashBase::WriteClock(HANDLE &hDev,
							 BYTE hour,
							 BYTE minitue,
							 BYTE second)
{
	 BYTE bhour;
	 BYTE bminitue;
	 BYTE bsecond;

	bhour = Clock_ToTime(hour);
	bminitue = Clock_ToTime(minitue);
	bsecond = Clock_ToTime(second);

	//首先允许时钟读写
	WriteClockDevice(hDev,CLOCK_DEVICE,DEVICE_ENABLE) ;
	//允许 CS-OUT-EN、SIO-OUT-EN、SCK-OUT-EN
	WriteClockDevice(hDev,CLOCK_CONTROL,CLOCK_CSOUT+CLOCK_SIOOUT+CLOCK_SCKOUT) ;
	//写命令字 SIO-OUT-EN = 1 
	//开始前的准备
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_SIO+CLOCK_SCK) ; //CS = 0 ;SCK = 1 ;SIO = 1;
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_CS+CLOCK_SIO+CLOCK_SCK) ; //CS = 1 ;SCK = 1 ;SIO = 1;
	//开始准备数据0110 0111 , 命令
	WriteBit(hDev,0);
	WriteBit(hDev,1);
	WriteBit(hDev,1);
	WriteBit(hDev,0);
	WriteBit(hDev,0);
	WriteBit(hDev,1);
	WriteBit(hDev,1);
	WriteBit(hDev,0);
	//开始写数据  SIO-OUT-EN = 1
	//
	
	WriteByte(hDev,bhour);
	WriteByte(hDev,bminitue);
	WriteByte(hDev,bsecond);

	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_SIO+CLOCK_SCK) ; //CS = 0 ;SCK = 1 ;SIO = 1;
	//禁止 CS-OUT-EN、SIO-OUT-EN、SCK-OUT-EN
	WriteClockDevice(hDev,CLOCK_CONTROL,0) ;
	//禁止时钟芯片 
	WriteClockDevice(hDev,CLOCK_DEVICE,0) ;
	return ;
}

void CEzFlashBase::ReadClock(HANDLE &hDev,
							BYTE &year,
							BYTE &month,
							BYTE &date,
							BYTE &week ,
							BYTE &hour,
							BYTE &minute,
							BYTE &second)
{
	 BYTE byear,bmonth,bdate,bweek,bhour,bminute,bsecond;

	//首先允许时钟读写
	WriteClockDevice(hDev,CLOCK_DEVICE,DEVICE_ENABLE) ;
	//允许 CS-OUT-EN、SIO-OUT-EN、SCK-OUT-EN
	WriteClockDevice(hDev,CLOCK_CONTROL,CLOCK_CSOUT+CLOCK_SIOOUT+CLOCK_SCKOUT) ;
	//写命令字 SIO-OUT-EN = 1 
	//开始前的准备
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_SIO+CLOCK_SCK) ; //CS = 0 ;SCK = 1 ;SIO = 1;
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_CS+CLOCK_SIO+CLOCK_SCK) ; //CS = 1 ;SCK = 1 ;SIO = 1;
	//开始准备数据0110 0111 , 命令
	WriteBit(hDev,0);
	WriteBit(hDev,1);
	WriteBit(hDev,1);
	WriteBit(hDev,0);
	WriteBit(hDev,0);
	WriteBit(hDev,1);
	WriteBit(hDev,0);
	WriteBit(hDev,1);
	//开始读数据  SIO-OUT-EN = 0
	WriteClockDevice(hDev,CLOCK_CONTROL,CLOCK_CSOUT+CLOCK_SCKOUT) ;
	//
	byear = ReadByte(hDev);
	bmonth = ReadByte(hDev);
	bdate = ReadByte(hDev);
	bweek = ReadByte(hDev);
	bhour = ReadByte(hDev);
	bminute = ReadByte(hDev);
	bsecond = ReadByte(hDev);

	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_SIO+CLOCK_SCK) ; //CS = 0 ;SCK = 1 ;SIO = 1;
	//禁止 CS-OUT-EN、SIO-OUT-EN、SCK-OUT-EN
	WriteClockDevice(hDev,CLOCK_CONTROL,0) ;
	//禁止时钟芯片 
	WriteClockDevice(hDev,CLOCK_DEVICE,0) ;
	year = Clock_FromTime(byear);
	month = Clock_FromTime(bmonth);
	date = Clock_FromTime(bdate);
	week = Clock_FromTime(bweek);
	hour = Clock_FromTime(bhour);
	minute = Clock_FromTime(bminute);
	second = Clock_FromTime(bsecond);
}

void CEzFlashBase::WriteClock(HANDLE &hDev,
							BYTE year,
							BYTE month,
							BYTE date,
							BYTE week ,
							BYTE hour,
							BYTE minute,
							BYTE second)
{
	 BYTE byear,bmonth,bdate,bweek,bhour,bminute,bsecond;

	byear = Clock_ToTime(year);
	bmonth = Clock_ToTime(month);
	bdate = Clock_ToTime(date);
	bweek = Clock_ToTime(week);
	bhour = Clock_ToTime(hour);
	bminute = Clock_ToTime(minute);
	bsecond = Clock_ToTime(second);

	//首先允许时钟读写
	WriteClockDevice(hDev,CLOCK_DEVICE,DEVICE_ENABLE) ;
	//允许 CS-OUT-EN、SIO-OUT-EN、SCK-OUT-EN
	WriteClockDevice(hDev,CLOCK_CONTROL,CLOCK_CSOUT+CLOCK_SIOOUT+CLOCK_SCKOUT) ;
	//写命令字 SIO-OUT-EN = 1 
	//开始前的准备
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_SIO+CLOCK_SCK) ; //CS = 0 ;SCK = 1 ;SIO = 1;
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_CS+CLOCK_SIO+CLOCK_SCK) ; //CS = 1 ;SCK = 1 ;SIO = 1;
	//开始准备数据0110 0111 , 命令
	WriteBit(hDev,0);
	WriteBit(hDev,1);
	WriteBit(hDev,1);
	WriteBit(hDev,0);
	WriteBit(hDev,0);
	WriteBit(hDev,1);
	WriteBit(hDev,0);
	WriteBit(hDev,0);
	//
	WriteByte(hDev,byear);
	WriteByte(hDev,bmonth);
	WriteByte(hDev,bdate);
	WriteByte(hDev,bweek);
	WriteByte(hDev,bhour);
	WriteByte(hDev,bminute);
	WriteByte(hDev,bsecond);

	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_SIO+CLOCK_SCK) ; //CS = 0 ;SCK = 1 ;SIO = 1;
	//禁止 CS-OUT-EN、SIO-OUT-EN、SCK-OUT-EN
	WriteClockDevice(hDev,CLOCK_CONTROL,0) ;
	//禁止时钟芯片 
	WriteClockDevice(hDev,CLOCK_DEVICE,0) ;
}

void CEzFlashBase::WriteBit(HANDLE &hDev,BYTE bit)
{
	BYTE bb = bit?2:0 ;
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_CS+bb+0) ;
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_CS+bb+CLOCK_SCK) ;
}

BYTE CEzFlashBase::ReadBit(HANDLE &hDev)
{
	BYTE bb = 0 ;
	WORD wordread = 0 ;
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_CS+bb+0) ;
	WriteClockDevice(hDev,CLOCK_DATA,CLOCK_CS+bb+CLOCK_SCK) ;
	wordread = ReadDevice(hDev,CLOCK_DATA) ;
	bb = (wordread&2)?1:0 ;
	return bb ;
}

BYTE CEzFlashBase::ReadByte(HANDLE &hDev)
{
	int i = 0 ; 
	BYTE byReturn = 0 , bb =0 ;
	for( i=0;i<8;i++)
	{
		bb = ReadBit(hDev);	
		byReturn += bb<<i ;
	}
	return byReturn ;
}

void CEzFlashBase::WriteByte(HANDLE &hDev,BYTE bdata)
{
	for(int i=0;i<8;i++)
	{
		WriteBit(hDev,(bdata>>i)&0x1);	
	}
}

BYTE CEzFlashBase::Clock_FromTime(BYTE time)
{
	register BYTE bb , dd ;
	bb = time & 0xF ;
	dd = (time>>4)&0xF ;
	return bb+dd*10 ;
}

BYTE CEzFlashBase::Clock_ToTime(BYTE data )
{
	register BYTE bb , dd ;
	bb = data%10 ;
	dd = data/10 ;
	return (dd<<4)+bb ;
}

DWORD CEzFlashBase::Try_AmdId(HANDLE hDev)
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
	if((id1!=0x2202)|(id2!=0x2202)|(id3!=0x2202)|(id4!=0x2202))
		return 0xFFFFFFFF ;

	return 0x7E027E02;
}
DWORD CEzFlashBase::Try_FujiId(HANDLE hDev)
{
	WORD id1,id2;
	WriteDevice(hDev,0,0xFF);
	WriteDevice(hDev,0x555,0xaa);
	WriteDevice(hDev,0x2aa,0x55);
	WriteDevice(hDev,0x555,0x90);

	WriteDevice(hDev,0x1000,0xFF);
	WriteDevice(hDev,0x1555,0xaa);
	WriteDevice(hDev,0x12aa,0x55);
	WriteDevice(hDev,0x1555,0x90);

	id1=ReadDevice(hDev,1);
	id2=ReadDevice(hDev,0x1001);
	if((id1!=0x227E)|(id2!=0x227E))
		return 0xFFFFFFFF ;
	id1=ReadDevice(hDev,0xE);
	id2=ReadDevice(hDev,0x100e);
	if((id1!=0x2221)|(id2!=0x2221) && ((id1!=0x2218)|(id2!=0x2218)))
		return 0xFFFFFFFF ;
	return 0x227E2221 ;
}
DWORD  CEzFlashBase::TryID(HANDLE hDev)
{
	if(Try_AmdId(hDev) == AMD_ID)
		return AMD_ID;
	else if(Try_FujiId(hDev) == FUJI_ID)
		return FUJI_ID;
	else 
		return 0;
}

BYTE CEzFlashBase::GetNandSize()
{
	return m_NANDSize ;
}
