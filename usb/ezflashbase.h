// ezflashbase.h: interface for the CEzFlashBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EZFLASHBASE_H__C8EDFF2B_2CFC_40A7_90CE_2CCDF1D8C3D3__INCLUDED_)
#define AFX_EZFLASHBASE_H__C8EDFF2B_2CFC_40A7_90CE_2CCDF1D8C3D3__INCLUDED_

#include "ezusbsys.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define AMD_ID  0x7E027E02
#define FUJI_ID 0x227E2221

#define ROM_Read 1
#define ROM_Write 2
#define RAM_Read 3
#define RAM_Write 4
#define Open_Port 5
#define Close_Port 6

#define Write_Operation	25 
#define Read_Operation 26
#define NandFlash_Read 27
#define NandFlash_Write 28
#define LDRFlash_Write 29
#define ROM_Write_UMAC 30
#define NandFlash_Read16 31
#define WriteClock_Operation 32
#define WriteI2C 33
#define ReadI2C 34
#define InitI2C 35
#define ROM_Write_FUJIMCP 36

#define Ram_Write_Operation	98 
#define Ram_Read_Operation 99
//clock
#define CLOCK_DEVICE	0x64
#define CLOCK_CONTROL	0x63
#define CLOCK_DATA		0x62
#define	CLOCK_CSOUT		0x4
#define	CLOCK_SIOOUT	0x2
#define	CLOCK_SCKOUT	0x1
#define CLOCK_SCK		0x1
#define CLOCK_SIO		0x2
#define CLOCK_CS 		0x4		
#define DEVICE_ENABLE	0x1


class CEzFlashBase  
{
public:
	CEzFlashBase();
	virtual ~CEzFlashBase();
protected :
	DWORD  Bulk(	HANDLE HDev, unsigned long ControlCode, 
					unsigned long pipeNum,
					LPVOID buf, unsigned long ByteCount);
private:
	DWORD Try_AmdId(HANDLE hDev);
	DWORD Try_FujiId(HANDLE hDev);
public:
	DWORD  TryID(HANDLE hDev);
	//NAND flash 控制命令
	void NAND_Reset(HANDLE hDev);
	DWORD  NAND_ReadNANDID(HANDLE hDev);
	WORD   NAND_ReadNANDStatus(HANDLE hDev);
	void   NAND_ReadFlash(HANDLE hDev,DWORD address,BYTE *pbuf,DWORD ByteCount);
	void   NAND_ReadFlashBase(HANDLE hDev,DWORD address,BYTE *pbuf,DWORD ByteCount);

	void   NAND_ReadFlash16(HANDLE hDev,DWORD address,BYTE *pbuf,DWORD ByteCount);
	void   NAND_WriteFlash(HANDLE hDev,DWORD address,BYTE *pbuf,DWORD ByteCount);
	void   NAND_WriteFlashBase(HANDLE hDev,DWORD address,BYTE *pbuf,DWORD ByteCount);
	BOOL   NAND_BlockErase(HANDLE hDev,DWORD block);
	void   NAND_ENABLE(HANDLE hDev,BOOL fEnable);
	void   NAND_SetReadMode(HANDLE hDev,BYTE mode);
	//LDRFLH 控制命令
	DWORD  LDRFLH_ReadID(HANDLE hDev);
	void   LDRFLH_ReadArray(HANDLE hDev);
	WORD   LDRFLH_ReadStatus(HANDLE hDev);
	void   LDRFLH_ClearStatus(HANDLE hDev);
	void   LDRFLH_BlockErase(HANDLE hDev,DWORD dwBlock);
	void   LDRFLH_Read(HANDLE hDev, DWORD StartAddr, BYTE* pbuf, DWORD ByteCount);
	void   LDRFLH_Write(HANDLE hDev, DWORD StartAddr, BYTE* pbuf, DWORD ByteCount);
	//EXECFLH 
	virtual DWORD  EXECFLH_ReadID(HANDLE hDev);
	virtual void   EXECFLH_ReadArray(HANDLE hDev); 
	virtual WORD   EXECFLH_ReadStatus(HANDLE hDev);
	virtual void   EXECFLH_ClearStatus(HANDLE hDev);
	virtual void   EXECFLH_BlockErase(HANDLE hDev,DWORD BlockNum);
	virtual void   EXECFLH_Read(HANDLE hDev, DWORD StartAddr, BYTE* pbuf, DWORD ByteCount);
	virtual void   EXECFLH_Write(HANDLE hDev, DWORD StartAddr, BYTE* pbuf, DWORD ByteCount);
	//ram 的直接读写命令
	void   _Read(HANDLE hDev, DWORD StartAddr, BYTE* pbuf, DWORD ByteCount);
	void   _Write(HANDLE hDev, DWORD StartAddr, BYTE* pbuf, DWORD ByteCount);
	//clock 命令
	BYTE Clock_ToTime(BYTE data );
	BYTE Clock_FromTime(BYTE time);
	void WriteByte(HANDLE &hDev,BYTE bdata);
	BYTE ReadByte(HANDLE &hDev);
	BYTE ReadBit(HANDLE &hDev);
	void WriteBit(HANDLE &hDev,BYTE bit);
	void WriteClock(HANDLE &hDev,
								BYTE year,
								BYTE month,
								BYTE date,
								BYTE week ,
								BYTE hour,
								BYTE minute,
								BYTE second);
	void ReadClock(HANDLE &hDev,
								BYTE &year,
								BYTE &month,
								BYTE &date,
								BYTE &week ,
								BYTE &hour,
								BYTE &minute,
								BYTE &second);
	void WriteClock(HANDLE &hDev,
								 BYTE hour,
								 BYTE minitue,
								 BYTE second);
	void ReadClock(HANDLE &hDev,
								 BYTE &hour,
								 BYTE &minitue,
								 BYTE &second);
	void Set24Hour(HANDLE hDev);
	void ClockReset(HANDLE &hDev );
	void ClockEndtst(HANDLE &hDev);
	void WriteStatus(HANDLE &hDev,BYTE status);
	BYTE ReadStatus(HANDLE &hDev );
	///I2c operation
	void WriteI2C_op(HANDLE &hDev,BYTE address,BYTE length,BYTE*data);
	WORD ReadI2C_op(HANDLE &hDev,BYTE address,BYTE length,BYTE*data);
	void InitI2C_op(HANDLE &hDev);

	BYTE			m_readsize;
public:
	BYTE GetNandSize();
	void SetMapMode(WORD wMode);
	bool OpenCartDevice(HANDLE &hDev);
	//打开卡设备
	bool CloseCartDevice(HANDLE &hDev);
	//关闭设备
	void WriteClockDevice(HANDLE &hDev,UINT address,WORD data);
	void WriteDevice(HANDLE &hDev,UINT address,WORD data);
	WORD ReadDevice(HANDLE &hDev,UINT address);

	void CartOpen(HANDLE hDev);
	void CartClose(HANDLE hDev);
	void CartOpenFlashOP(HANDLE hDev);
	void CartCloseFlashOP(HANDLE hDev);
	//
	void CartSetROMPage(HANDLE hDev,DWORD Offset);
	void CartSetEZControl(HANDLE hDev,WORD control);
	//---------------------------------------------------------------------------
	void CartRAMRead(HANDLE hDev, WORD StartAddr, BYTE* pbuf, unsigned long ByteCount);
	//---------------------------------------------------------------------------
	void CartRAMWrite(HANDLE hDev, WORD StartAddr, BYTE* pbuf, unsigned long ByteCount);
	void CartSetRAMPage(HANDLE hDev,DWORD Offset);

protected:
	BYTE			ctrlbuf[64];
	OVERLAPPED		*ol;
	HANDLE			hEve;
	WORD			m_wEz3OPMode ;
	UINT			m_uiMode;
	BYTE*			m_pByte;
	WORD			m_wNandMode;
	//used for usb2.0 fx2 chip 
	BYTE*			pUsepipe ;
	BYTE			pipe[3][4] ;
	BYTE			m_NANDSize ;	
	
};

#endif // !defined(AFX_EZFLASHBASE_H__C8EDFF2B_2CFC_40A7_90CE_2CCDF1D8C3D3__INCLUDED_)
