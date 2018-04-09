// RomManager.h: interface for the CRomManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROMMANAGER_H__8B3E5F15_1FB5_47DF_8E34_6AC6B8E5DB42__INCLUDED_)
#define AFX_ROMMANAGER_H__8B3E5F15_1FB5_47DF_8E34_6AC6B8E5DB42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


typedef enum {
	UNKNOW =0,
	SRAM_TYPE=1,
	EEPROM_TYPE=2,
	FLASH_TYPE=3,
}SAVERTYPE ;
typedef enum {
	EZ1OLDWHITE=1,
	EZ1NEWCART=2,
	EZ2Cart=3,
	NON_EZCart = 4 ,
	NO_CART =5,
	EZLink = 6 ,
}EZCARTTYPE;

class  CRomManager  
{
public:
	BOOL NDSPatch1(BYTE*  &pGame,DWORD &romsize,BYTE *bSavePatch,WORD rompage=0 ,WORD rampage=0,bool b_ids=0,bool b_gba=0);
	BOOL QuerySpecial(BYTE * pdata,DWORD &offset);
	BYTE GetSaverSpacial(BYTE * pdata);	
	void ModifyNDSHeader(BYTE * pdata ,DWORD newArm9size,DWORD addon);
	WORD CRC16(unsigned char *ptr,unsigned int len);
	void fixCRC(BYTE * pdata);

	BOOL RemoveIntro(BYTE *pData,DWORD* pdwLen);

	DWORD GetSaverTypeAndSize(BYTE **buf,DWORD *length,SAVERTYPE *type);
	void InflateROM(BYTE **buf, DWORD *length);
	void FillComplementCheck(BYTE *buf);
	BOOL HeaderValid(BYTE* buf);
	DWORD SaverPatch(BYTE **buf,
					DWORD *length,
					EZCARTTYPE cartType,
					UINT saverOffset,
					SAVERTYPE *dwSaverType,
					DWORD dwReserved);
	DWORD SpecialRomPatch(BYTE **buf, 
						  DWORD *length,
						  EZCARTTYPE cartType,
						  DWORD dwReserved);
	void  Modify1MSaverRom(BYTE **buf, DWORD *length,UINT saverOffset);
	DWORD ApplyIPSPatch(LPCTSTR ips,BYTE* pROM,DWORD RomLen);
	DWORD ApplyIPSPatch(LPCTSTR ips , LPCTSTR ROM);
	DWORD AddROMBomaPatch(LPCTSTR Boma,LPCTSTR ROM,BYTE whattodo);
	void SetPCECharacter(int iIndex ,	//note max is 64
						  BOOL fCPU50Clock = 0,
						  BOOL fUSRom = 0,
						  BOOL fCPUhackDis = 0,
						  BOOL fFollowMem = 0,
						  DWORD dwFollowSprite = 0);
	void GetPCECharacter(int Index ,
						  BOOL &f50CPUClock,
						  BOOL &fUSRom,
						  BOOL &fCPUHACKDISABLE,
						  BOOL &fFollowMem,
						  DWORD &dwFollowSprite);
	void SetNESCharacter(int iIndex ,//note max is 64
						  BOOL fFFUhackEn=0,
						  BOOL fCPUhackDis=0,
						  BOOL fUsePALTimeing=0,
						  BOOL fFollowMem=0,
						  DWORD dwFollowSprite=0);
	void GetNESCharacter(int iIndex ,//note max is 64
						  BOOL &fFFUhackEn,
						  BOOL &fCPUhackDis,
						  BOOL &fUsePALTimeing,
						  BOOL &fFollowMem,
						  DWORD &dwFollowSprite);

	DWORD EnumNES2GBAFromFiles(BYTE *pBuf,//输出的buf 
								DWORD *pSize ,//输出的buf大小，如果小于需要的，这个大小是实际的需要大小
								LPCTSTR pFile1,...);//第一个文件是pocketnes.gba ,后面的文件是需要添加的文件
	DWORD EnumNES2GBAFromFiles(BYTE *pBuf,//输出的buf 
								DWORD *pSize ,//输出的buf大小，如果小于需要的，这个大小是实际的需要大小
								CStringArray &fileArray);//第一个文件是pocketnes.gba ,后面的文件是需要添加的文件
	DWORD EnumGBC2GBAFromFiles(BYTE *pBuf, //输出的buf 
								DWORD *pSize ,//输出的buf大小，如果小于需要的，这个大小是实际的需要大小
								LPCTSTR pFile1,...);//第一个文件是pocketnes.gba ,后面的文件是需要添加的文件
	DWORD EnumGBC2GBAFromFiles(BYTE *pBuf, //输出的buf 
								DWORD *pSize ,//输出的buf大小，如果小于需要的，这个大小是实际的需要大小
								CStringArray &fileArray);//第一个文件是pocketnes.gba ,后面的文件是需要添加的文件
	DWORD EnumPCE2GBAFromFiles(BYTE *pBuf,
								DWORD *pSize ,
								LPCTSTR pFile1,...);
	DWORD EnumPCE2GBAFromFiles(BYTE *pBuf,
								DWORD *pSize ,
								CStringArray &fileArray);

//	void TrimRom(BYTE** pBuf , int *iSize);
	CRomManager();
	virtual ~CRomManager();

private:
	//这里是nes需要的
	BYTE	*FindMotif(BYTE *Buffer, DWORD BufferSize, BYTE *Motif, DWORD MotifSize,DWORD skip1);
	BYTE	*FindMotif(BYTE *Buffer, DWORD BufferSize, BYTE *Motif, DWORD MotifSize);
	void	FormatPCEPreload(int iIndex, DWORD length , LPCTSTR name) ;
	void	FormatNESPreload(int iIndex , DWORD length, LPCTSTR name ) ;
	BYTE	m_bufPreload[64];
	BOOL	m_flagNES_CPUHackDisable[64];//bit0
	BOOL	m_flagNES_FFUHackEnable[64]; //bit1
	BOOL	m_flagNES_UsePALTimeing[64]; //bit2
	BOOL	m_flagNES_FollowMem[64];	  //bit5
	DWORD	m_dwNES_FollowSprite[64];
	//这里是pce游戏需要的
	BOOL	m_flagPCE_50CPUClock[64] ;//bit0
	BOOL	m_flagPCE_USRom[64]; //bit1
	BOOL	m_flagPCE_CPUHackDisable[64]; //bit2
	BOOL	m_flagPCE_FollowMem[64];	  //bit5
	DWORD	m_dwPCE_FollowSprite[64];
};

#endif // !defined(AFX_ROMMANAGER_H__8B3E5F15_1FB5_47DF_8E34_6AC6B8E5DB42__INCLUDED_)
