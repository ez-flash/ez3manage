// FileSys.cpp: implementation of the CFileSys class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileSys.h"
#include "ECC_Check.h"
//#include "resource.h"

//欢迎使用yafei的字库转换程序
//创建于2003年3月9日
//字库大小512字节
const BYTE fathead[] = {
0xeb,0x3c,0x90,0x4d,0x53,0x44,0x4f,0x53,
0x35,0x2e,0x30,0x00,0x02,0x04,0x20,0x00,
0x02,0x00,0x02,0x00,0x00,0xf8,0x00,0x01,
0x3f,0x00,0xff,0x00,0x20,0x00,0x00,0x00,
0x00,0x00,0x04,0x00,0x80,0x00,0x29,0x44,
0x7a,0x80,0x88,0x4e,0x4f,0x20,0x4e,0x41,
0x4d,0x45,0x20,0x20,0x20,0x20,0x46,0x41,
0x54,0x31,0x36,0x20,0x20,0x20,0x33,0xc9,
0x8e,0xd1,0xbc,0xf0,0x7b,0x8e,0xd9,0xb8,
0x00,0x20,0x8e,0xc0,0xfc,0xbd,0x00,0x7c,
0x38,0x4e,0x24,0x7d,0x24,0x8b,0xc1,0x99,
0xe8,0x3c,0x01,0x72,0x1c,0x83,0xeb,0x3a,
0x66,0xa1,0x1c,0x7c,0x26,0x66,0x3b,0x07,
0x26,0x8a,0x57,0xfc,0x75,0x06,0x80,0xca,
0x02,0x88,0x56,0x02,0x80,0xc3,0x10,0x73,
0xeb,0x33,0xc9,0x8a,0x46,0x10,0x98,0xf7,
0x66,0x16,0x03,0x46,0x1c,0x13,0x56,0x1e,
0x03,0x46,0x0e,0x13,0xd1,0x8b,0x76,0x11,
0x60,0x89,0x46,0xfc,0x89,0x56,0xfe,0xb8,
0x20,0x00,0xf7,0xe6,0x8b,0x5e,0x0b,0x03,
0xc3,0x48,0xf7,0xf3,0x01,0x46,0xfc,0x11,
0x4e,0xfe,0x61,0xbf,0x00,0x00,0xe8,0xe6,
0x00,0x72,0x39,0x26,0x38,0x2d,0x74,0x17,
0x60,0xb1,0x0b,0xbe,0xa1,0x7d,0xf3,0xa6,
0x61,0x74,0x32,0x4e,0x74,0x09,0x83,0xc7,
0x20,0x3b,0xfb,0x72,0xe6,0xeb,0xdc,0xa0,
0xfb,0x7d,0xb4,0x7d,0x8b,0xf0,0xac,0x98,
0x40,0x74,0x0c,0x48,0x74,0x13,0xb4,0x0e,
0xbb,0x07,0x00,0xcd,0x10,0xeb,0xef,0xa0,
0xfd,0x7d,0xeb,0xe6,0xa0,0xfc,0x7d,0xeb,
0xe1,0xcd,0x16,0xcd,0x19,0x26,0x8b,0x55,
0x1a,0x52,0xb0,0x01,0xbb,0x00,0x00,0xe8,
0x3b,0x00,0x72,0xe8,0x5b,0x8a,0x56,0x24,
0xbe,0x0b,0x7c,0x8b,0xfc,0xc7,0x46,0xf0,
0x3d,0x7d,0xc7,0x46,0xf4,0x29,0x7d,0x8c,
0xd9,0x89,0x4e,0xf2,0x89,0x4e,0xf6,0xc6,
0x06,0x96,0x7d,0xcb,0xea,0x03,0x00,0x00,
0x20,0x0f,0xb6,0xc8,0x66,0x8b,0x46,0xf8,
0x66,0x03,0x46,0x1c,0x66,0x8b,0xd0,0x66,
0xc1,0xea,0x10,0xeb,0x5e,0x0f,0xb6,0xc8,
0x4a,0x4a,0x8a,0x46,0x0d,0x32,0xe4,0xf7,
0xe2,0x03,0x46,0xfc,0x13,0x56,0xfe,0xeb,
0x4a,0x52,0x50,0x06,0x53,0x6a,0x01,0x6a,
0x10,0x91,0x8b,0x46,0x18,0x96,0x92,0x33,
0xd2,0xf7,0xf6,0x91,0xf7,0xf6,0x42,0x87,
0xca,0xf7,0x76,0x1a,0x8a,0xf2,0x8a,0xe8,
0xc0,0xcc,0x02,0x0a,0xcc,0xb8,0x01,0x02,
0x80,0x7e,0x02,0x0e,0x75,0x04,0xb4,0x42,
0x8b,0xf4,0x8a,0x56,0x24,0xcd,0x13,0x61,
0x61,0x72,0x0b,0x40,0x75,0x01,0x42,0x03,
0x5e,0x0b,0x49,0x75,0x06,0xf8,0xc3,0x41,
0xbb,0x00,0x00,0x60,0x66,0x6a,0x00,0xeb,
0xb0,0x4e,0x54,0x4c,0x44,0x52,0x20,0x20,
0x20,0x20,0x20,0x20,0x0d,0x0a,0x52,0x65,
0x6d,0x6f,0x76,0x65,0x20,0x64,0x69,0x73,
0x6b,0x73,0x20,0x6f,0x72,0x20,0x6f,0x74,
0x68,0x65,0x72,0x20,0x6d,0x65,0x64,0x69,
0x61,0x2e,0xff,0x0d,0x0a,0x44,0x69,0x73,
0x6b,0x20,0x65,0x72,0x72,0x6f,0x72,0xff,
0x0d,0x0a,0x50,0x72,0x65,0x73,0x73,0x20,
0x61,0x6e,0x79,0x20,0x6b,0x65,0x79,0x20,
0x74,0x6f,0x20,0x72,0x65,0x73,0x74,0x61,
0x72,0x74,0x0d,0x0a,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xac,0xcb,0xd8,0x55,0xaa,
}; 
//结束

//////////////////////////////////////////////////////////////////////
// CDiskCache Construction/Destruction
//////////////////////////////////////////////////////////////////////
int	haswrite = 0;
int  zerocluster = -1;

CDiskCache::CDiskCache(PVOID MemoryDisk) : BytesPerSector(512)
{
//	ASSERT(MemoryDisk);
	MemDisk = MemoryDisk;	 
	m_pEZActual = NULL;
	m_pEZActual = new CEzFlashBase;
	PageCache = NULL;
	SectorCache = NULL;
	BlcokCache = NULL;
	BlcokCacheWithSpare = NULL ;
	CurrentCachePage=-1;
	m_iCurrentBlock=-1 ;
	m_pos = 0;
	m_readpos=0;
	m_type = OPNONE;
	m_hWnd = NULL;
}

CDiskCache::~CDiskCache()
{
	if(m_pEZActual)
	{
		delete m_pEZActual;
		m_pEZActual = NULL;
	}
	if(BlcokCache)
		delete BlcokCache ;
	if(BlcokCacheWithSpare)
		delete BlcokCacheWithSpare ;
	if(PageCache)
	{
		delete PageCache;
		PageCache = NULL;
	}
	m_pos = 0;
	m_readpos=0;
}

void CDiskCache::CartInit()
{
	if(!m_pEZActual)
		m_pEZActual = new CEzFlashBase;
	m_pEZActual->OpenCartDevice(hDev);
	m_pEZActual->SetMapMode(0x8000);
	m_pEZActual->CartSetROMPage(hDev,0);
	m_pEZActual->CartOpenFlashOP(hDev);
	m_pEZActual->CartOpen(hDev);
	m_pEZActual->NAND_ENABLE(hDev,1);
	m_pEZActual->NAND_Reset(hDev);
	DWORD Nand_ID = m_pEZActual->NAND_ReadNANDID(hDev);	
	m_pEZActual->NAND_Reset(hDev);
	m_iCurrentBlock = -1 ;
	BlcokCache = new BYTE[2048*64] ;
	BlcokCacheWithSpare = new BYTE[2112*64] ;
	PageCache = new BYTE[2112*8];
	m_pos = 0;
	m_readpos =0;
}

void CDiskCache::ClearCartInit()
{
	if(m_pEZActual)
	{
		m_pEZActual->NAND_ENABLE(hDev,0);
		m_pEZActual->CartClose(hDev);
		m_pEZActual->CartCloseFlashOP(hDev);
		m_pEZActual->CloseCartDevice(hDev);
		delete m_pEZActual;
		m_pEZActual = NULL;
		if(BlcokCache) delete BlcokCache ;
		BlcokCache = NULL ;
		if(BlcokCacheWithSpare) delete BlcokCacheWithSpare ;
		BlcokCacheWithSpare = NULL ;
	}
	m_pos = 0;
	m_readpos=0;
	CurrentCachePage=-1;
	m_iCurrentBlock=-1 ;
}
BYTE* CDiskCache::GetSectorData(DWORD StartSector)
{
	DWORD block = StartSector / 256 ;
	DWORD page = StartSector/4 ;
	DWORD pos = StartSector%4 ;
	int cluster = -3 ;
	if(page == CurrentCachePage)
	{
		SectorCache = PageCache + pos*512 ;
	}
/*
	else if(block == m_iCurrentBlock)
	{
		if((StartSector)&&(StartSector>=FirstDataSector))
		{
			cluster = (WORD)((StartSector - FirstDataSector) / Bpb.SecPerClus + 2);;
		}
		if(cluster!=zerocluster)
			memcpy(PageCache,BlcokCache+(page*4-block*256)*512,2048);
		else
			memset(PageCache,0,2112);
		SectorCache = PageCache + pos*512 ;
	}
*/
	else
	{
		if((StartSector)&&(StartSector>=FirstDataSector))
		{
			cluster = (WORD)((StartSector - FirstDataSector) / Bpb.SecPerClus + 2);;
		}
		if(cluster!=zerocluster)
		{
			m_pEZActual->NAND_Reset(hDev);
			m_pEZActual->NAND_ReadFlash(hDev,StartSector * BytesPerSector,(BYTE *)PageCache,2112);
			m_readpos++;
			if(m_hWnd && (m_type==READFILE))
				::PostMessage(m_hWnd,WM_NAND_READ_STEP_MESSAGE,m_readpos,0);
		}
		else
			memset(PageCache,0,2112);
		SectorCache = PageCache;
	}
	zerocluster= -1;
	CurrentCachePage = page ;	
	return SectorCache;
}

//Flush cache data onto disk.
BOOL CDiskCache::Flush()
{
	
	CECC_Check ck;
	BYTE tmpBuf[2112] ;
	register int i ;
	int temp=0;
	if(m_iCurrentBlock != -1)
	{
		if(m_iCurrentBlock ==0x2C)
			temp++;
		if(haswrite)
			return TRUE;
		memset(BlcokCacheWithSpare,0,2112*64);
		m_pEZActual->NAND_Reset(hDev);
		if(m_pEZActual->NAND_BlockErase(hDev,m_iCurrentBlock*64*2048)==FALSE)
		{
			if(m_pEZActual->NAND_BlockErase(hDev,m_iCurrentBlock*64*2048)==FALSE)
				return FALSE;
		}
		for(i=0;i<64;i++)
		{
			memcpy(tmpBuf,BlcokCache+(i<<11) ,2048);
			ck.make_ecc_512(&tmpBuf[2048+8],tmpBuf);
			ck.make_ecc_512(&tmpBuf[2048+8+16],tmpBuf+512);
			ck.make_ecc_512(&tmpBuf[2048+8+32],tmpBuf+1024);
			ck.make_ecc_512(&tmpBuf[2048+8+48],tmpBuf+1536);
			memcpy(BlcokCacheWithSpare+(i*2112),tmpBuf,2112);
		}

		m_pEZActual->NAND_Reset(hDev);//分两次写入。
		m_pEZActual->NAND_WriteFlash(hDev,m_iCurrentBlock*64*2048 , BlcokCacheWithSpare,2112*32);
		m_pEZActual->NAND_WriteFlash(hDev,m_iCurrentBlock*64*2048+32*2048,BlcokCacheWithSpare+32*2112,2112*32);
		haswrite = 1 ;
		m_pos++;
		if(m_hWnd && (m_type==CREATEFILE))
			::PostMessage(m_hWnd,WM_NAND_WRITE_STEP_MESSAGE,m_pos,0);
	}
	
	return TRUE;
}

//Write data into spe cial sector. Size can more than bytes per sector.
int CDiskCache::SetSectorData(DWORD StartSector, BYTE* Buffer, DWORD Size)
{
	//memcpy(&((BYTE*)MemDisk)[StartSector * BytesPerSector], Buffer, Size);
	int  blcAddress = StartSector/256 ;
	if(m_iCurrentBlock != blcAddress)
	{
		Flush();
		m_pEZActual->NAND_Reset(hDev);
		m_pEZActual->NAND_ReadFlash(hDev,blcAddress*64*2048,BlcokCacheWithSpare,2112*32);
		m_pEZActual->NAND_ReadFlash(hDev,blcAddress*64*2048+32*2048,BlcokCacheWithSpare+32*2112,2112*32);
		for(int i=0;i<64;i++)
		{
			memcpy(BlcokCache+i*2048, BlcokCacheWithSpare+i*2112, 2048) ;
		}
	}	
	m_iCurrentBlock = blcAddress ;
	memcpy(&BlcokCache[(StartSector-blcAddress*256)*512], Buffer, Size) ;
	haswrite = 0;
	return 0;
}

BOOL CDiskCache::FormatCart(CProgressCtrl *m_pProgressCtrl,BOOL bQuick,CString strVolume)
{
	HANDLE hDev;
	DWORD loop=0;
	DWORD dwID;
	CECC_Check ecc ;
	CDWordArray dw_ErrorArry;
//	CString strVolume;
//	((m_pProgressCtrl->GetParent())->GetDlgItem(IDC_NAND_DISK_NAMEEDIT))->GetWindowText(strVolume);
	if(m_pEZActual)
	{
		delete m_pEZActual;
		m_pEZActual = NULL;
	}
	m_pEZActual = new CEzFlashBase;
	m_pEZActual->OpenCartDevice(hDev);
	m_pEZActual->CartOpenFlashOP(hDev);
	m_pEZActual->CartOpen(hDev);
	m_pEZActual->NAND_ENABLE(hDev,1);
	m_pEZActual->NAND_Reset(hDev);
	dwID = m_pEZActual->NAND_ReadNANDID(hDev);
	m_pEZActual->NAND_Reset(hDev);
	loop = m_pEZActual->GetNandSize();
	switch(loop)
	{
	case 0:
		loop = 0x8000000;
		break;
	case 2:
		loop = 0x8000000*2;
		break;
	case 4:
		loop = 0x8000000*4;
		break;
	case 8:
		loop = 0x8000000*8;	
		break;
	}
	/*
	if(bQuick)
	{
		//快速格式化:格式化0--543扇区
		m_pProgressCtrl->SetRange(0,3+1);
		for(loop=0;loop<3;loop++)
		{
			m_pEZActual->NAND_BlockErase(hDev,loop*64*2048);
			m_pProgressCtrl->SetPos(loop+1);
		}
	}
	else
	*/
	{
		/*
		m_pProgressCtrl->SetRange(0,1024+100);
		for(loop=0;loop<1024;loop++)
		{
			m_pEZActual->NAND_BlockErase(hDev,loop*64*2048);
			m_pProgressCtrl->SetPos(loop+1);
		}
		*/
		dw_ErrorArry.RemoveAll();
		m_pProgressCtrl->SetRange(0,loop/0x20000+10);
		m_pProgressCtrl->SetPos(0);;
		for(DWORD j=0;j<loop;j+=0x20000)
		{
			m_pEZActual->NAND_Reset(hDev);
			if(m_pEZActual->NAND_BlockErase(hDev,j)==FALSE)
			{
				if(m_pEZActual->NAND_BlockErase(hDev,j)==FALSE)
				{
					dw_ErrorArry.Add(j);
					continue;
				}
			}
			m_pProgressCtrl->SetPos(j/0x20000);;
		}

	}
	//写如初始化数据
	m_pEZActual->NAND_Reset(hDev);
	BYTE *bpb=new BYTE[2112];
	WORD *pFat=new WORD[1056];
	BYTE *pDir=new BYTE[2112];
	BYTE *pTmp = new BYTE[2112*8] ;

	BYTE *pfat8 = (BYTE*)pFat ;
	memset(pTmp,0,8*2112);
	memset(bpb,0,2112);
	memcpy(bpb,fathead,0x200) ;
	switch(m_pEZActual->GetNandSize())
	{
	case 2:
		bpb[13]=8;
		*((DWORD*)(&bpb[32])) = 0x80000 ;
		dw_NandSize = 128*1024*2;
		break;
	case 4:
		bpb[13]=16;
		*((DWORD*)(&bpb[32])) = 0x100000 ;
		dw_NandSize = 128*1024*4;
		break;
	case 8:
		bpb[13]=32;
		*((DWORD*)(&bpb[32])) = 0x200000 ;
		dw_NandSize = 128*1024*8;
		break;
	default:
		bpb[13]=4;
		*(DWORD*)(bpb+32) = 0x3ffff; //总共的扇区数
		dw_NandSize = 128*1024*1;
		break;
	}
	memset(pFat,0x00,2112);
	memset(pDir,0xFF,2112);

	//*(DWORD*)(bpb+32) = 0x3ffff; //总共的扇区数
	ecc.make_ecc_512(&bpb[2048+8],bpb);
	ecc.make_ecc_512(&bpb[2048+8+16],bpb+512);
	ecc.make_ecc_512(&bpb[2048+8+32],bpb+1024);
	ecc.make_ecc_512(&bpb[2048+8+48],bpb+1536);
	memcpy(pTmp,bpb,2112);
	m_pEZActual->NAND_WriteFlash(hDev,0,pTmp,2112*8);
//	for(int counter=0;counter<8;counter)
//	{
//		m_pEZActual->NAND_WriteFlash(hDev,counter*2048,&pTmp[counter*2112],2112);
//	}

	pFat[0]=0xFFF8 ;
	pFat[1]=0xFFFF ;
	/*
	//计算坏块位置，标注坏块位置
	for(int j=0;j<dw_ErrorArry.GetSize();j++)
	{
		DWORD dw_BadClusNum = 0x20000/(bpb[13]*512);
		DWORD dw_temp = dw_ErrorArry.GetAt(j);
		DWORD dw_ClusOff = dw_temp/(bpb[13]*512)+2;
		for(int k=0;k<dw_BadClusNum;k++)
			pFat[dw_ClusOff+k] = 0xFFF7;
	}
	*/
	//
	ecc.make_ecc_512(&pfat8[2048+8],pfat8);
	ecc.make_ecc_512(&pfat8[2048+8+16],pfat8+512);
	ecc.make_ecc_512(&pfat8[2048+8+32],pfat8+1024);
	ecc.make_ecc_512(&pfat8[2048+8+48],pfat8+1536);	
	//m_pEZActual->NAND_WriteFlash(hDev,8*2048,(unsigned char *)pFat,2112);
	int j;
	DWORD dw_BadClusNum;
	DWORD dw_temp;
	DWORD dw_ClusOff;
	DWORD dw_Counter=0;
	dw_BadClusNum = 0x20000/(bpb[13]*512);
	DWORD dw_DataOff = Bpb.RsvdSecCnt + (Bpb.NumFATs * Bpb.FATSz) + RootDirSectors;
	dw_DataOff = dw_DataOff*512;
	BOOL bLeft=FALSE;
	int leftCounter=0;
	int k=0;
	for(int i=8;i<135;i++)
	{
		if(i!=8)
		{
			memset(pFat,0x00,2112);
			if(bLeft)
			{
				bLeft = FALSE;
				for(k=0;k<leftCounter;k++)
					pFat[k] = 0xFFF7;
			}
			dw_Counter++;
			for(int j=0;j<dw_ErrorArry.GetSize();j++)
			{
				dw_temp = dw_ErrorArry.GetAt(j);
				dw_temp = dw_temp-dw_DataOff;
				dw_ClusOff = 2+dw_temp/(bpb[13]*512);
				if(dw_ClusOff>(dw_Counter*1024))
				{
					dw_ClusOff = dw_ClusOff-dw_Counter*1024;
					if(dw_ClusOff<1024)
					{
						for(k=0;k<dw_BadClusNum;k++)
						{
							if((dw_ClusOff+k)<1024)
								pFat[dw_ClusOff+k] = 0xFFF7;
							else
							{
								bLeft = TRUE;
								leftCounter = dw_BadClusNum-k;
								break;
							}
						}
					}
				}
				
			}
			ecc.make_ecc_512(&pfat8[2048+8],pfat8);
			ecc.make_ecc_512(&pfat8[2048+8+16],pfat8+512);
			ecc.make_ecc_512(&pfat8[2048+8+32],pfat8+1024);
			ecc.make_ecc_512(&pfat8[2048+8+48],pfat8+1536);	
		}
		else
		{
			for(int j=0;j<dw_ErrorArry.GetSize();j++)
			{
				
				dw_temp = dw_ErrorArry.GetAt(j);
				dw_temp = dw_temp-dw_DataOff;
				dw_ClusOff = 2+dw_temp/(bpb[13]*512);
				if(dw_ClusOff<1024)
				{
					for(int k=0;k<dw_BadClusNum;k++)
						pFat[dw_ClusOff+k] = 0xFFF7;
				}
			}
			//pFat[5]=0xFFF7 ;
			//pFat[7]=0xFFF7 ;
		}
		memcpy(pTmp+(i%8)*2112,pFat,2112);
		if((i%8)==7)
			m_pEZActual->NAND_WriteFlash(hDev,(i-7)*2048,(unsigned char *)pTmp,2112*8);
	}
	_DIR dir;
	dir.Attr=0x08;
	memcpy(dir.Name,strVolume.GetBuffer(11),11);
	strVolume.ReleaseBuffer(11);
	memcpy(pDir,&dir,32);
	ecc.make_ecc_512(&pDir[2048+8],pfat8);
	ecc.make_ecc_512(&pDir[2048+8+16],pfat8+512);
	ecc.make_ecc_512(&pDir[2048+8+32],pfat8+1024);
	ecc.make_ecc_512(&pDir[2048+8+48],pfat8+1536);	
	memcpy(pTmp,pDir,2112);
	m_pEZActual->NAND_WriteFlash(hDev,136*2048,pTmp,2112*8);

	m_pEZActual->NAND_ENABLE(hDev,0);
	m_pEZActual->CartClose(hDev);
	m_pEZActual->CartCloseFlashOP(hDev);
	m_pEZActual->CloseCartDevice(hDev);
	delete []m_pEZActual;
	m_pEZActual = NULL;
	delete []bpb;
	delete []pFat;
	delete []pDir;
	return TRUE;
}

void CDiskCache::SetMessageHandle(HWND hWnd)
{
	m_hWnd = hWnd;
}


//////////////////////////////////////////////////////////////////////
// CFileSys Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileSys::CFileSys(PVOID MemoryDisk) : CDiskCache(MemoryDisk)
{
	FatCache = 0;
	dw_FreeSpace = 0;
	dw_NandSize = 0;
}

CFileSys::~CFileSys()
{
	if(FatCache)
		delete FatCache;
	FatCache = NULL ;
}

//////////////////////////////////////////////////////////////////////
// Public interface
//////////////////////////////////////////////////////////////////////
DWORD CountofClusters;	

BOOL CFileSys::Initialize()
{
	BYTE* BootSec;
	DWORD DataSec;
	BYTE* Cache;
	WORD* pFat;
	DWORD Sector;

	// Initialize BPB.
	BootSec = GetSectorData(0);
	Bpb.BytsPerSec = *((WORD*)&BootSec[11]);
	if(Bpb.BytsPerSec !=0x200)
		return FALSE;
	Bpb.SecPerClus = BootSec[13];
	switch(Bpb.SecPerClus)
	{
	case 8:
		dw_NandSize = 128*1024*2;
		break;
	case 16:
		dw_NandSize = 128*1024*4;
		break;
	case 32:
		dw_NandSize = 128*1024*8;
		break;
	default:
		dw_NandSize = 128*1024*1;
	}
extern DWORD				g_dwNandSize;
	g_dwNandSize = GetNandSize();
	Bpb.RsvdSecCnt = *((WORD*)&BootSec[14]);//保留区域中保留扇区的数量
	Bpb.NumFATs = BootSec[16];
	Bpb.RootEntCnt = *((WORD*)&BootSec[17]);
	if((Bpb.TotSec = *((WORD*)&BootSec[19])) == 0)
		Bpb.TotSec = *((DWORD*)&BootSec[32]);
	if((Bpb.FATSz = *((WORD*)&BootSec[22])) == 0)
		Bpb.FATSz = *((DWORD*)&BootSec[36]);

	//计算根路径占用的扇区数
	RootDirSectors = ((Bpb.RootEntCnt * 32) + (Bpb.BytsPerSec - 1)) / Bpb.BytsPerSec;
	//数据区域扇区数
	DataSec = Bpb.TotSec - (Bpb.RsvdSecCnt + (Bpb.NumFATs * Bpb.FATSz) + RootDirSectors);
	CountofClusters = DataSec / Bpb.SecPerClus;

	/*FAT12 < 4085, FAT32 >= 65525*/
	if(CountofClusters < 4085 || CountofClusters >= 65525)
		return FALSE;

	FirstFatSecNum = Bpb.RsvdSecCnt;
 	FirstRootDirSecNum = Bpb.RsvdSecCnt + (Bpb.NumFATs * Bpb.FATSz);
	FirstDataSector = Bpb.RsvdSecCnt + (Bpb.NumFATs * Bpb.FATSz) + RootDirSectors;
	BYTE *pTest;
	if(!FatCache)
	{
		FatCache = new WORD[Bpb.BytsPerSec * Bpb.FATSz / sizeof(WORD)];
		pTest = (BYTE *)FatCache;
		memset(pTest,0,0x20000);
	//	CFile file;
	//	file.Open("C:\\0001\\数据.bin",CFile::modeCreate|CFile::modeWrite);
	//	file.Write((BYTE *)pTest,0x20000);
	//	file.Close();
		
	}
	if(FatCache == NULL)
		return FALSE;
	WORD *p = FatCache;
	//保存FAT区数据到FatCache指针中
	for(Sector = FirstFatSecNum, pFat = FatCache; Sector <(FirstFatSecNum+Bpb.FATSz); Sector++, pFat+=Bpb.BytsPerSec/sizeof(WORD))
	{
		Cache = GetSectorData(Sector);
		if(Cache == NULL)
		{
			delete[] FatCache;
			return FALSE;
		}

		memcpy(pFat, Cache, Bpb.BytsPerSec);
	}

	dw_FreeSpace=0;

	//CFile file;
	//file.Open("C:\\0001\\数据Debug.bin",CFile::modeCreate|CFile::modeWrite);
	//file.Write(pTest,0x20000);
	//file.Close();
	for(DWORD i=0;i<0x10000;i++)
	{
		if(p[i] == 0x0)
			dw_FreeSpace++;
	}
	memset(&fat_get, 0, sizeof(fat_get));
	memset(&handles, 0, sizeof(handles));
	return TRUE;
}

DWORD CFileSys::GetFreeNandSpace()
{
	switch(Bpb.SecPerClus)
	{
	case 8:
		return (dw_FreeSpace*4 );
		break;
	case 16:
		return (dw_FreeSpace*8 );
		break;
	case 32:
		return (dw_FreeSpace*16 );
		break;
	default:
		return (dw_FreeSpace*2 );
	}
}

DWORD CFileSys::GetNandSize()
{
	return dw_NandSize;
}
void CFileSys::Uninitialize()
{
	DWORD i,j;

	// close all opened files
	for(i=0; i<16; i++)
	{
		if(handles[i].valid)
		{
			fat_close(i);
		}
	}

	// flush the fat cache to disk.
	for(i = 0; i < Bpb.NumFATs; i++)
	{
		for(j=0;j<Bpb.FATSz;j++)
			SetSectorData(FirstFatSecNum+i*Bpb.FATSz + j, ((BYTE*)FatCache)+j*Bpb.BytsPerSec , Bpb.BytsPerSec );
	}
	Flush();
}

void CFileSys::Callback()
{

}

//////////////////////////////////////////////////////////////////////
// Private functions
//////////////////////////////////////////////////////////////////////

inline WORD CFileSys::SectorNum2ClusterNum(DWORD SectorNum)
{
	return (WORD)((SectorNum - FirstDataSector) / Bpb.SecPerClus + 2);
}

inline DWORD CFileSys::ClusterNum2SectorNum(WORD ClusterNum)
{
	return FirstDataSector + (ClusterNum - 2) * Bpb.SecPerClus;
}

//return next cluster num,
//0xffff indicate no next cluster.
//Note! : this function will dirty cache!
inline WORD CFileSys::GetNextClusterNum(WORD ClusterNum)
{
	return FatCache[ClusterNum];
}

// alloc a free cluster. policy is searching from prev cluster number, if no free cluster till end of fat, then search from head of fat.
// return a cluster number. 0xffff indicate faild, disk overflow.
// argument 0 : no prev cluster.
WORD CFileSys::AllocCluster(WORD PrevClusterNum)
{
	static WORD LastAllocClusterNum=0;
	WORD i;

	if(LastAllocClusterNum == 0)
		LastAllocClusterNum = PrevClusterNum;

	for(i = LastAllocClusterNum; i < CountofClusters/*Bpb.BytsPerSec * Bpb.FATSz / sizeof(WORD)*/; i++)
	{
		if(FatCache[i] == 0)
		{
			FatCache[i] = 0xffff;	// flag with 0xffff, this is the last cluster.
			
			LastAllocClusterNum = i;
			
			//chain this cluster to prev one.
			if(PrevClusterNum != 0)
				FatCache[PrevClusterNum] = LastAllocClusterNum;
			zerocluster = LastAllocClusterNum;
			return LastAllocClusterNum;
		}
	}

	// we have to search from head of fat
	for(i = 2; i < CountofClusters/*Bpb.BytsPerSec * Bpb.FATSz / sizeof(WORD)*/; i++)
	{
		if(FatCache[i] == 0)
		{
			FatCache[i] = 0xffff;	// flag with 0xffff, this is the last cluster.
			
			LastAllocClusterNum = i;
			
			//chain this cluster to prev one.	
			if(PrevClusterNum != 0)
				FatCache[PrevClusterNum] = LastAllocClusterNum;
			
			zerocluster = LastAllocClusterNum;
			return LastAllocClusterNum;
		}
	}
	
	return 0xffff;
}

// free cluster chain.
// argument 0 : no prev cluster.
void CFileSys::FreeCluster(WORD StartClusterNum)
{
	WORD Cluster;
	WORD NextCluster;

	Cluster = StartClusterNum;

	while((Cluster != 0xffff)&&(Cluster != 0x0000))
	{
		NextCluster = FatCache[Cluster];
		FatCache[Cluster] = 0x0000;
		Cluster = NextCluster;
	}
}

// return the first sector number of dir content .
// 0xffffffff indicate failed.
/****************************************************************************************/
/*
	为新的文件或目录分配DIR
*/
/****************************************************************************************/
int CFileSys::AllocDir(DWORD ParentDirSectorNum, struct _DIR* new_dir, struct _FILE * fp)
{
	BYTE* Cache;
	struct _DIR *dir;
	DWORD i;
	WORD PrevCluster;
	WORD Cluster;
	DWORD DirSectorNum = ParentDirSectorNum;

	//if(dirname == NULL)
		//return 1;

	if(ParentDirSectorNum == FirstRootDirSecNum)
	{//在根目录下
		for(i=0; i<Bpb.RootEntCnt * sizeof(struct _DIR) / Bpb.BytsPerSec; i++)
		{
			Cache = GetSectorData(DirSectorNum);
			if(Cache == NULL)
				return 2;
			
			for(dir = (struct _DIR *)Cache; (BYTE*)dir < Cache + Bpb.BytsPerSec; dir++)
			{
				if(dir->Name[0] == '\0' || dir->Name[0] == ERCHAR ||(dir->Name[0]==0xFF&&dir->Name[1]==0xFF))
				{
					memcpy(dir, new_dir, sizeof(struct _DIR));					
					SetSectorData(DirSectorNum, Cache , Bpb.BytsPerSec);

					if(fp)
					{
						fp->DirSectorNum = DirSectorNum;
						fp->DirIndex = ((BYTE*)dir - Cache) / sizeof(struct _DIR);
						memcpy(&fp->dir, new_dir, sizeof(struct _DIR));
					}
					Flush();
					return 0;
				}
			}
			DirSectorNum++;
		}

		// root dir have no room.
		return 3; 
	}
	
	else
	{
		Cluster = SectorNum2ClusterNum(DirSectorNum);
		
		while(Cluster != 0xffff)
		{
			for(i=0; i< Bpb.SecPerClus; i++)
			{
				Cache = GetSectorData(DirSectorNum);
				if(Cache == NULL)
					return 2;
				
				for(dir = (struct _DIR *)Cache; (BYTE*)dir < Cache + Bpb.BytsPerSec; dir++)
				{
					if(dir->Name[0] == '\0' || dir->Name[0] == ERCHAR||(dir->Name[0]==0xFF&&dir->Name[1]==0xFF))
					{
						memcpy(dir, new_dir, sizeof(struct _DIR));
						SetSectorData(DirSectorNum, Cache , Bpb.BytsPerSec);
						
						if(fp)
						{
							fp->DirSectorNum = DirSectorNum;
							fp->DirIndex = ((BYTE*)dir - Cache) / sizeof(struct _DIR);
							memcpy(&fp->dir, new_dir, sizeof(struct _DIR));
						}
						Flush();
						return 0;
					}
				}
				DirSectorNum++;
			}
			
			PrevCluster = Cluster;
			Cluster = GetNextClusterNum(Cluster);
			DirSectorNum = ClusterNum2SectorNum(Cluster);
		}
		
		//
		// we have to extend this parent dir room.
		//
		Cluster = AllocCluster(PrevCluster);
		if(Cluster == 0xffff)
			return 4;
		
		DirSectorNum = ClusterNum2SectorNum(Cluster);
		
		Cache = GetSectorData(DirSectorNum);
		if(Cache == NULL)
			return 2;
		
		dir = (struct _DIR *)Cache;
		
		memcpy(dir, new_dir, sizeof(struct _DIR));
		SetSectorData(DirSectorNum, Cache , Bpb.BytsPerSec);
		Flush();
		
		if(fp)
		{
			fp->DirSectorNum = DirSectorNum;
			fp->DirIndex = ((BYTE*)dir - Cache) / sizeof(struct _DIR);
			memcpy(&fp->dir, new_dir, sizeof(struct _DIR));
		}
		
		return 0;
	}

	return 5;
}

 int CFileSys::DeleteDir(struct _FILE *file)
{
	BYTE* Cache;
	struct _DIR *dir;

	Cache = GetSectorData(file->DirSectorNum);
	if(Cache == NULL)
		return 1;

	dir = (struct _DIR *)Cache;
	dir += file->DirIndex;

	dir->Name[0] = ERCHAR;
	SetSectorData(file->DirSectorNum, Cache , Bpb.BytsPerSec);
	Flush();

	return 0;
}

// NULL indicate failed.
//Valid format is full path:	\[8.3\]*DIR_Name
//limits:
// length < 80 && !(special char)
char* CFileSys::get_valid_format(const char *fullpath)
{
	static char static_path[512];
	char* p=static_path;
	char path[80];
	char* ppath = path;
	int dir_len_count; //count dir len.
	int i;

	if(fullpath == NULL || strlen(fullpath) >=80 || *fullpath != '\\')
		return NULL;

	if(strlen(fullpath) > 1 && fullpath[strlen(fullpath)-1] =='\\')
		return NULL;

	strcpy(path, fullpath);
	strupr(path);
	memset(p, 0, 512);

	for(;;)
	{
		switch(*ppath)
		{
		case 0x00:
			{
				if(dir_len_count != 0) // prev is not '\\'
				{
					for(i=0; i<(11 - dir_len_count); i++)
					{
						*p = 0x20;
						p++;
					}
				}
			}
			return static_path;

		case '\\':
			{
				if(p != static_path) // this is not the first '\\'
				{
					if(dir_len_count == 0)// more than one '\\'
						return NULL;

					for(i=0; i<(11 - dir_len_count); i++)
					{
						*p = 0x20;
						p++;
					}
				}
				
				*p = '\\';
				p++;
				
				ppath++;
				dir_len_count =0;
				continue;
			}
			break;

		case '.':
			{
				if(dir_len_count > 8 || dir_len_count ==0) // '\\.' check
					return NULL;

				if(ppath[1] == '.' || ppath[1] == '\\') // more than one '.' or '.\\' check
					return NULL;

				for(i=0; i<(8 - dir_len_count); i++)
				{
					*p = 0x20;
					p++;
				}

				dir_len_count =8;
				ppath++;
				continue;
			}
			break;

		case 0x22:
		case 0x2A:
		case 0x2B:
		case 0x2C:
		case 0x2F:
		case 0x3A:
		case 0x3B:
		case 0x3C:
		case 0x3D:
		case 0x3E:
		case 0x3F:
		case 0x5B:
		case 0x5D:
		case 0x7C:
			return NULL;

		default:
			{
				if((unsigned char)(*ppath) < 0x20)
					return NULL;				
			}
			break;
		}

		*p = *ppath;
		dir_len_count ++;

		if(dir_len_count > 11)
			return NULL;

		p++;
		ppath++;
	}

	return static_path;
}

void CFileSys::unformat_name(char * filename, const unsigned char dirname[11])
{
	int i;
	int j;

	memset(filename, 0, 13);

	for(i=0; i<8; i++)
	{
		if(dirname[i] != 0x20)
			filename[i] = dirname[i];
		else
			break;
	}
	
	if(dirname[8] != 0x20)
	{
		filename[i] = '.';
		j = i + 1;
		
		for(i=8; i<11; i++,j++)
		{
			if(dirname[i] != 0x20)
				filename[j] = dirname[i];
			else
				break;
		}
	}
}
/****************************************************************************************/
/*
	传入的参数:	1.	DWORD Sector				扇区号,
				2.	const char dirname[11]:		文件名或目录名字
				3.	struct _FILE *file			文件信息
	返回数值:	如果找到文件,返回文件的第一个扇区
				没有文件,返回-1;
*/
/****************************************************************************************/
DWORD CFileSys::SectorSearch(DWORD Sector, const char dirname[11], struct _FILE *file)
{
	BYTE* Cache;
	struct _DIR *dir;
	unsigned int j;

	Cache = GetSectorData(Sector);
	if(Cache == NULL)
		return 0xffffffff;
	
	dir = (struct _DIR *)Cache;
	//对比Root区
	for(j=0; (dir->Name[0] != '\0')&&(dir->Name[0]!=0xFF||dir->Name[1]!=0xFF)&& (j< Bpb.BytsPerSec / sizeof(struct _DIR)); j++)
	{
		if(memcmp((const char*)dir->Name, dirname, 11) == 0 && (!(dir->Attr & ATTR_VOLUME_ID)))// not volumn id or long name entry.
		{
			if(file != NULL)
			{
				memset(file, 0, sizeof(struct _FILE));
				file->DirSectorNum = Sector;
				file->DirIndex = j;
				memcpy(&file->dir, dir, sizeof(struct _DIR));
			}
			
			return ClusterNum2SectorNum(dir->FstClusLO);
		}
		
		dir++;
	}

	return 0xffffffff;
}

int CFileSys::SectorGet(DWORD Sector, struct _FatGet *fat_get)
{
	BYTE* Cache;
	struct _DIR *dir;
	unsigned int j ; 
	/*process for long name*/
	//unsigned int loop;
	//unsigned short uni[128];
	
	Cache = GetSectorData(Sector);
	if(Cache == NULL)
		return 1;

	dir = (struct _DIR *)Cache;
	dir += fat_get->DirIndex;
	//yafei modify , blow is wrong in process
	for(j = fat_get->DirIndex ; (j< Bpb.BytsPerSec / sizeof(struct _DIR)); j++)
	{
		if((dir->Name[0] != '\0')&&(dir->Name[0]!=0xFF||dir->Name[1]!=0xFF)&&(dir->Name[0] != ERCHAR) && (!(dir->Attr & ATTR_VOLUME_ID)))// not volumn id or long name entry.
		{
			fat_get->DirSectorNum = Sector;
			fat_get->DirIndex = fat_get->DirIndex +1;
			unformat_name(fat_get->filename, dir->Name);

			if(fat_get->DirIndex == Bpb.BytsPerSec / sizeof(struct _DIR))
			{
				fat_get->DirIndex = 0;
				fat_get->DirSectorNum ++ ;
			}
			return 0;
		}
/*
		//add by yafei  , long filename process
		else if(dir->Name[0] != ERCHAR) 
		{//长文件名处理
			if(dir->Name[0]&0x40)
			{//第一个，为正确的
				loop = (dir->Name[0] & 0x3F);
				uni[loop*13] = 0 ;
				fat_get->DirSectorNum = Sector;
				fat_get->DirIndex = fat_get->DirIndex + 1;
				while(loop --)
				{
					memcpy(&uni[loop*13],(void*)((UINT)dir+1),10);
					memcpy(&uni[loop*13+5],(void*)((UINT)dir+14),12);
					memcpy(&uni[loop*13+11],(void*)((UINT)dir+28),4);
					dir++ ;
					j++ ;
				}
				Unicode2ansi((char*)uni,fat_get->filename);
			}
		}
*/
		dir++;
		fat_get->DirIndex = fat_get->DirIndex +1;
	}
	if(fat_get->DirIndex == Bpb.BytsPerSec / sizeof(struct _DIR))
	{
		fat_get->DirIndex = 0;
		fat_get->DirSectorNum ++ ;
	}
/* 
	for(j=fat_get->DirIndex; (dir->Name[0] != '\0') && (j< Bpb.BytsPerSec / sizeof(struct _DIR)); j++)
	{
		if(dir->Name[0] != ERCHAR && (!(dir->Attr & ATTR_VOLUME_ID)))// not volumn id or long name entry.
		{
			fat_get->DirSectorNum = Sector;
			fat_get->DirIndex = j + 1;
			unformat_name(fat_get->filename, dir->Name);

			return 0;
		}

		dir++;
	}
*/
	return 2;
}

// return the first sector number of dir/file content.
// 0xffffffff indicate failed.
DWORD CFileSys::Search(DWORD Sector, const char dirname[11], struct _FILE *file)
{	
	unsigned int i;
	WORD Cluster;
	DWORD FirstSectorOfFile;

	//if(dirname == NULL)
		//return 0xffffffff;
	
	if(Sector == FirstRootDirSecNum)
	{
		for(i=0; i<Bpb.RootEntCnt * sizeof(struct _DIR) / Bpb.BytsPerSec; i++)
		{
			FirstSectorOfFile = SectorSearch(Sector++, dirname, file);
			if(FirstSectorOfFile != 0xffffffff)
				return FirstSectorOfFile;
		}
	}

	else
	{
		Cluster = SectorNum2ClusterNum(Sector);

		// because arg Sector is the first sector of parent dir content,
		// so it is the first sector of the cluster.
		/*
		i = (Sector - FirstDataSector) % Bpb.SecPerClus;
		
		if(i != 0)
		{
			for(; i< Bpb.SecPerClus; i++)
			{
				FirstSectorOfFile = SectorSearch(Sector++, dirname, file);
				if(FirstSectorOfFile != 0xffffffff)
					return FirstSectorOfFile;
			}

			Cluster = GetNextClusterNum(Cluster);
			Sector = ClusterNum2SectorNum(Cluster);
		}*/
		
		while(Cluster != 0xffff)
		{
			for(i=0; i< Bpb.SecPerClus; i++)
			{
				FirstSectorOfFile = SectorSearch(Sector++, dirname, file);
				if(FirstSectorOfFile != 0xffffffff)
					return FirstSectorOfFile;
			}

			Cluster = GetNextClusterNum(Cluster);
			Sector = ClusterNum2SectorNum(Cluster);
		}
	}

	return 0xffffffff;
}

// return the first sector number of dir/file content.
// 0xffffffff indicate failed.
// if path is root dir, file arg is ignore.
DWORD CFileSys::Locate(const char *path, struct _FILE *file)
{
	DWORD Sector = FirstRootDirSecNum;
	char *p;

	p = get_valid_format(path);
	if(p == NULL)
		return 0xffffffff;

	// locate first sub dir.
	p++;

	// locate next sub dir(s).
	for(;;)
	{
		if(*p == NULL)
			return Sector;

		Sector = Search(Sector, p, file);
		if(Sector == 0xffffffff)
			return 0xffffffff;

		p+=12;
	}

	// we never go here.
	return 0xffffffff;
}

void CFileSys::datetime(struct _FatDateTime *fatdatetime)
{
	SYSTEMTIME systm;
	struct _FatDate* pdate = &fatdatetime->Date.fatdate;
	struct _FatTime *ptm = &fatdatetime->Time.fattime;

	GetLocalTime (&systm);

	pdate->Day = systm.wDay;
	pdate->Month = systm.wMonth;
	pdate->Year = systm.wYear - 1980;

	ptm->Hours = systm.wHour;
	ptm->Minutes = systm.wMinute;
	ptm->Second_2s = systm.wSecond / 2;

	fatdatetime->TimeTenth = (systm.wSecond % 2) * 100 + systm.wMilliseconds / 10;
}




///////////////////////////////////////////////////////////////////
// fat apis
///////////////////////////////////////////////////////////////////

int CFileSys::fat_mkdir( const char *dirname)
{
	struct _DIR dir;
	DWORD SectorNum,sec;
	char path[512];
	char name[11];
	char *p;
	struct _FatDateTime tm;
	struct _FILE file;	
	BYTE* Cache;
	struct _DIR *pdir;
	WORD NewCluster;	
	char dot[11] = {'.', 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20};
	char dotdot[11] = {'.', '.', 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20};


	// is path format correct ?
	p = get_valid_format(dirname);
	if(p == NULL)
		return 1;

	//if exist this dir ?
	if(Locate(dirname, NULL) != 0xffffffff)
		return 4;

	//separate path into parent and name
	strncpy(name, &p[strlen(p)-11], 11);

	strcpy(path, dirname);
	p = strrchr(path, '\\');
	if(p == path) // it is root dir.
		*(p+1) = '\0';
	else
		*p = '\0';

	//locate parent path
	SectorNum = Locate(path, NULL);
	if(SectorNum == 0xffffffff)
		return 2;

	//fill dir attributes
	memset(&dir, 0, sizeof(dir));
	memcpy(dir.Name, name, 11);
	dir.Attr = ATTR_DIRECTORY;
	datetime(&tm);
	dir.CrtDate = dir.LstAccDate = dir.WrtDate = tm.Date.Date;
	dir.CrtTime = dir.WrtTime = tm.Time.Time;
	dir.CrtTimeTenth = tm.TimeTenth;

	//alloc one dir
	if(AllocDir(SectorNum, &dir, &file) != 0)
		return 3;

	//alloc a cluster
	NewCluster = AllocCluster(0);
	if(NewCluster == 0xffff)
		return 4;

	//flush to disk
	Cache = GetSectorData(file.DirSectorNum);
	if(Cache == NULL)
		return 6;

	pdir = (struct _DIR *)Cache;
	pdir += file.DirIndex;

	pdir->FstClusLO = NewCluster;

	SetSectorData(file.DirSectorNum, Cache , Bpb.BytsPerSec);
	Flush();
	//create . and .. dir items.
	zerocluster = NewCluster ;
	sec = ClusterNum2SectorNum(NewCluster) ;
	Cache = GetSectorData(sec);
	memset(Cache,0,Bpb.BytsPerSec*Bpb.SecPerClus);
	if(Cache == NULL)
		return 6;

	pdir = (struct _DIR *)Cache;
	memset(pdir, 0, sizeof(struct _DIR));
	memcpy(pdir->Name, dot, 11);
	pdir->Attr = ATTR_DIRECTORY;
	datetime(&tm);
	pdir->CrtDate = pdir->LstAccDate = pdir->WrtDate = tm.Date.Date;
	pdir->CrtTime = pdir->WrtTime = tm.Time.Time;
	pdir->CrtTimeTenth = tm.TimeTenth;
	pdir->FstClusLO = NewCluster;

	pdir++;
	memset(pdir, 0, sizeof(struct _DIR));
	memcpy(pdir->Name, dotdot, 11);
	pdir->Attr = ATTR_DIRECTORY;
	datetime(&tm);
	pdir->CrtDate = pdir->LstAccDate = pdir->WrtDate = tm.Date.Date;
	pdir->CrtTime = pdir->WrtTime = tm.Time.Time;
	pdir->CrtTimeTenth = tm.TimeTenth;
	pdir->FstClusLO = SectorNum2ClusterNum(SectorNum);

	SetSectorData(sec, Cache , Bpb.BytsPerSec*Bpb.SecPerClus);
	Flush();

	return 0;
}

int CFileSys::fat_rmdir( const char *dirname)
{
	DWORD SectorNum;
	struct _FILE file;
	char filename[13];
	
	//is dir have no sub dir or file ?
	if(fat_getfirst(dirname, filename) == 0)
	{
		if(filename[0] == '.'  )
		{
			fat_getnext(filename);
			if(filename[0] == '..')
			{
				if(fat_getnext(filename) == 0)				
				{
					return 3;
				}	
			}
		}
		else
			return 3;
	}
	//locate
	SectorNum = Locate(dirname, &file);
	if(SectorNum == 0xffffffff)
		return 4;

	// is it a dir ?
	if(!(file.dir.Attr & ATTR_DIRECTORY))
		return 6;

	if(DeleteDir(&file) != 0)
		return 5;

	FreeCluster(file.dir.FstClusLO);

	return 0;
}

int CFileSys::fat_getfirst(const char *path, char* filename)
{
	DWORD Sector;
	unsigned int i;
	WORD Cluster;

	//if exist this dir ?
	Sector = Locate(path, NULL);
	if(Sector == 0xffffffff)
		return 1;

	if(Sector == FirstRootDirSecNum)
	{
		fat_get.IsRootDir = 1;
		fat_get.DirIndex = 1 ;

		for(i=0; i<Bpb.RootEntCnt * sizeof(struct _DIR) / Bpb.BytsPerSec; i++)
		{
			if(SectorGet(Sector++, &fat_get) == 0)
			{
				strcpy(filename, fat_get.filename);
				return 0;
			}
		}
	}

	else
	{
		fat_get.IsRootDir = 0;
		fat_get.DirIndex = 0;

		Cluster = SectorNum2ClusterNum(Sector);

		// because the sector is the first sector of parent dir,
		// so it is the first sector of cluster.
		/*
		i = (Sector - FirstDataSector) % Bpb.SecPerClus;
		
		if(i != 0)
		{
			for(; i< Bpb.SecPerClus; i++)
			{
				if(SectorGet(Sector++, &fat_get) == 0)
				{
					strcpy(filename, fat_get.filename);
					return 0;
				}
			}

			Cluster = GetNextClusterNum(Cluster);
			Sector = ClusterNum2SectorNum(Cluster);
		}*/
		
		while(Cluster != 0xffff)
		{
			for(i=0; i< Bpb.SecPerClus; i++)
			{
				if(SectorGet(Sector++, &fat_get) == 0)
				{
					strcpy(filename, fat_get.filename);
					return 0;
				}
			}

			Cluster = GetNextClusterNum(Cluster);
			Sector = ClusterNum2SectorNum(Cluster);
		}
	}

	return 2;
}

int CFileSys::fat_getnext(char* filename)
{
	DWORD Sector;
	unsigned int i;
	WORD Cluster;

	Sector = fat_get.DirSectorNum;

	if(fat_get.IsRootDir)
	{
		i=(Sector - FirstRootDirSecNum) ;

		for(; i<Bpb.RootEntCnt * sizeof(struct _DIR) / Bpb.BytsPerSec; i++)
		{
			if(SectorGet(Sector++, &fat_get) == 0)
			{
				strcpy(filename, fat_get.filename);
				return 0;
			}
		}
	}

	else
	{
		Cluster = SectorNum2ClusterNum(Sector);

		i = (Sector - FirstDataSector) % Bpb.SecPerClus;

		if(i != 0)
		{
			for(; i< Bpb.SecPerClus; i++)
			{
				if(SectorGet(Sector++, &fat_get) == 0)
				{
					strcpy(filename, fat_get.filename);
					return 0;
				}
			}

			Cluster = GetNextClusterNum(Cluster);
			Sector = ClusterNum2SectorNum(Cluster);
		}

		while(Cluster != 0xffff)
		{
			for(i=0; i< Bpb.SecPerClus; i++)
			{
				if(SectorGet(Sector++, &fat_get) == 0)
				{
					strcpy(filename, fat_get.filename);
					return 0;
				}
			}

			Cluster = GetNextClusterNum(Cluster);
			Sector = ClusterNum2SectorNum(Cluster);
		}
	}

	return 2;
}

int CFileSys::fat_close(int handle)
{
	struct _FILE *fp;
	struct _FatDateTime tm;
	BYTE* Cache;
	struct _DIR *dir;

	if(handle <0 || handle >= sizeof(handles)/sizeof(struct _FILE))
		return -1;

	fp = &handles[handle];

	datetime(&tm);
	fp->dir.LstAccDate = fp->dir.WrtDate = tm.Date.Date;
	fp->dir.WrtTime = tm.Time.Time;

	Cache = GetSectorData(fp->DirSectorNum);
	if(Cache == NULL)
		return -2;

	dir = (struct _DIR *)Cache;
	dir += fp->DirIndex;

	memcpy(dir, &fp->dir, sizeof(struct _DIR));
	SetSectorData(fp->DirSectorNum, Cache , Bpb.BytsPerSec);
	Flush();

	handles[handle].valid = 0;
	return 0;
}
/********************************************************************************/
/*
	fat_creat创建文件, 入口参数	1.cosnst char* filename:	文件名
								2.BYTE attribute:			文件属性
					   返回值	
								-2:							文件名不正确
								-3:							文件存在
								-4:							
*/
/********************************************************************************/
int CFileSys::fat_creat(const char* filename, BYTE attribute)
{
	struct _DIR dir;
	char path[512];
	char name[11];
	char *p;
	DWORD ParentDirSectorNum;
	struct _FatDateTime tm;
	struct _FILE file;
	struct _DIR *pdir;
	WORD NewCluster;
	BYTE* Cache;

	// is path format correct ?
	p = get_valid_format(filename);
	if(p == NULL)
		return -2;

	//if exist this file ?
	if(Locate(filename, NULL) != 0xffffffff)
		return -3;

	//separate path into parent and name
	strncpy(name, &p[strlen(p)-11], 11);

	strcpy(path, filename);
	p = strrchr(path, '\\');
	if(p==path)
		*(p+1) = 0 ;
	else
		*p = '\0';

	//locate parent path
	ParentDirSectorNum = Locate(path, NULL);
	if(ParentDirSectorNum == 0xffffffff)
		return -4;

	//fill dir attributes
	memset(&dir, 0, sizeof(dir));
	memcpy(dir.Name, name, 11);
	dir.Attr = attribute;
	datetime(&tm);
	dir.CrtDate = dir.LstAccDate = dir.WrtDate = tm.Date.Date;
	dir.CrtTime = dir.WrtTime = tm.Time.Time;
	dir.CrtTimeTenth = tm.TimeTenth;
	dir.FileSize = 0;

	//alloc one dir
	if(AllocDir(ParentDirSectorNum, &dir, &file) != 0)
		return -5;

	//alloc a cluster
	NewCluster = AllocCluster(0);
	if(NewCluster == 0xffff)
		return -6;

	//flush to disk
	Cache = GetSectorData(file.DirSectorNum);
	if(Cache == NULL)
		return -7;

	pdir = (struct _DIR *)Cache;
	pdir += file.DirIndex;

	pdir->FstClusLO = NewCluster;
	SetSectorData(file.DirSectorNum, Cache , Bpb.BytsPerSec);
	Flush();

	return fat_open(filename);
}

int CFileSys::fat_Instancesave_creat(const char* filename, BYTE attribute)
{
	struct _DIR dir;
	char path[512];
	char name[11];
	WORD zero[256] ;
	char *p;
	DWORD ParentDirSectorNum;
	struct _FatDateTime tm;
	struct _FILE file;
	struct _DIR *pdir;
	WORD NewCluster;
	BYTE* Cache;
	int handle ;

	// is path format correct ?
	p = get_valid_format(filename);
	if(p == NULL)
		return -2;

	//if exist this file ?
	if(Locate(filename, NULL) != 0xffffffff)
		return -3;

	//separate path into parent and name
	strncpy(name, &p[strlen(p)-11], 11);

	strcpy(path, filename);
	p = strrchr(path, '\\');
	if(p==path)
		*(p+1) = 0 ;
	else
		*p = '\0';

	//locate parent path
	ParentDirSectorNum = Locate(path, NULL);
	if(ParentDirSectorNum == 0xffffffff)
		return -4;

	//fill dir attributes
	memset(&dir, 0, sizeof(dir));
	memcpy(dir.Name, name, 11);
	dir.Attr = attribute;
	datetime(&tm);
	dir.CrtDate = dir.LstAccDate = dir.WrtDate = tm.Date.Date;
	dir.CrtTime = dir.WrtTime = tm.Time.Time;
	dir.CrtTimeTenth = tm.TimeTenth;
	dir.FileSize = 512*1024 ;

	//alloc one dir
	if(AllocDir(ParentDirSectorNum, &dir, &file) != 0)
		return -5;

	//alloc a cluster
	int k=0,nand=0,s=0;
	nand = m_pEZActual->GetNandSize() ;
	memset(zero,0,512);
	zero[0] = 0xFFFF ;
	s = 512 ;
	if(nand)
		s = s/nand ;
	while(1)
	{
		NewCluster = AllocAllianedCluster(k);
		k = NewCluster ;
		if(NewCluster == 0xffff)
			return -6;
		if(memcmp(&FatCache[NewCluster],zero,s))
			continue;
		else
			break;
	}
	//flush to disk
	Cache = GetSectorData(file.DirSectorNum);
	if(Cache == NULL)
		return -7;

	pdir = (struct _DIR *)Cache;
	pdir += file.DirIndex;

	pdir->FstClusLO = NewCluster;
	SetSectorData(file.DirSectorNum, Cache , Bpb.BytsPerSec);
	Flush();

	handle = fat_open(filename);
	if(handle<0) return handle ;
	s = 256 ;
	if(nand)
		s = s/nand ;
	for(int i=NewCluster;i<NewCluster+s;i++)
	{
		FatCache[i]=i+1;
	}
	FatCache[i-1] = 0xFFFF ;
	fat_close(handle);
	return ClusterNum2SectorNum(NewCluster)*512 ;
}

WORD CFileSys::AllocAllianedCluster(WORD PrevClusterNum)
{
	static WORD LastAllocClusterNum=0;
	WORD i;

	if(LastAllocClusterNum == 0)
		LastAllocClusterNum = PrevClusterNum;

	for(i = LastAllocClusterNum; i < CountofClusters/*Bpb.BytsPerSec * Bpb.FATSz / sizeof(WORD)*/; i++)
	{
		if(FatCache[i] == 0)
		{
			if(ClusterNum2SectorNum(i)%256)
				continue ;
			FatCache[i] = 0xffff;	// flag with 0xffff, this is the last cluster.
			
			LastAllocClusterNum = i;
			
			//chain this cluster to prev one.
			if(PrevClusterNum != 0)
				FatCache[PrevClusterNum] = LastAllocClusterNum;
			zerocluster = LastAllocClusterNum;
			return LastAllocClusterNum;
		}
	}

	// we have to search from head of fat
	for(i = 2; i < CountofClusters/*Bpb.BytsPerSec * Bpb.FATSz / sizeof(WORD)*/; i++)
	{
		if(FatCache[i] == 0)
		{
			if(!(ClusterNum2SectorNum(i)%64))
				continue ;
			FatCache[i] = 0xffff;	// flag with 0xffff, this is the last cluster.
			
			LastAllocClusterNum = i;
			
			//chain this cluster to prev one.	
			if(PrevClusterNum != 0)
				FatCache[PrevClusterNum] = LastAllocClusterNum;
			
			zerocluster = LastAllocClusterNum;
			return LastAllocClusterNum;
		}
	}
	
	return 0xffff;
}

long CFileSys::fat_lseek(int handle, long offset, int origin)
{
	struct _FILE *fp;
	WORD Cluster;
	unsigned int len;
	int i;

	if(handle <0 || handle >= sizeof(handles)/sizeof(struct _FILE))
		return 0;

	fp = &handles[handle];

	switch(origin)
	{
	case SEEK_SET:
		{
			if(offset < 0)
				return -1;

			fp->offset = offset;
		}
		break;

	case SEEK_CUR:
		{
			if(fp->offset + offset <0 )
				return -1;

			fp->offset += offset;
		}
		break;

	case SEEK_END:
		{
			if(fp->offset + offset <0 )
				return -1;

			fp->offset = fp->dir.FileSize + offset;
		}
		break;

	default:
		return -2;
	}

	// re-locate CurrentSectorNum, SectorOffset
	Cluster = fp->dir.FstClusLO;
	fp->CurrentSectorNum = ClusterNum2SectorNum(Cluster);
	len = 0;

	while(Cluster != 0xffff)
	{
		for(i=0; i< Bpb.SecPerClus; i++)
		{
			len += Bpb.BytsPerSec;

			if(len >= fp->offset)
			{
				fp->SectorOffset = fp->offset % Bpb.BytsPerSec;
				return fp->offset;
			}
			
			fp->CurrentSectorNum ++;
		}

		Cluster = GetNextClusterNum(Cluster);
		fp->CurrentSectorNum = ClusterNum2SectorNum(Cluster);
	}

	return handles[handle].offset;
}

int CFileSys::fat_open(const char* filename)
{
	int i;
	struct _FILE * fp = NULL;
	DWORD FirstSectorNum;

	for(i=0; i<16; i++)
	{
		if(!handles[i].valid)
		{
			fp = &handles[i];
			break;
		}
	}

	if(fp == NULL)
		return -1;

	FirstSectorNum = Locate(filename, fp);
	if(FirstSectorNum == 0xffffffff)
		return -2;

	fp->StartSectorNum = FirstSectorNum;
	fp->CurrentSectorNum = fp->StartSectorNum;
	fp->SectorOffset = 0;
	fp->offset = 0;
	fp->valid = 1;
	return i;
}

unsigned int CFileSys::fat_read(int handle, void* buffer, unsigned int bytes)
{
	BYTE* Cache;
	unsigned int read_bytes =0;
	unsigned int max_copy_bytes_in_sector;
	struct _FILE *fp;
	WORD Cluster;
	int i;

	if(handle <0 || handle >= sizeof(handles)/sizeof(struct _FILE))
		return 0;

	fp = &handles[handle];
	bytes = (fp->dir.FileSize - fp->offset) > bytes ? bytes : (fp->dir.FileSize - fp->offset);

	Cluster = SectorNum2ClusterNum(fp->CurrentSectorNum);

	i = (fp->CurrentSectorNum - FirstDataSector) % Bpb.SecPerClus;

	if(i != 0)
	{
		for(; i< Bpb.SecPerClus; i++)
		{
			Cache = GetSectorData(fp->CurrentSectorNum);
			if(Cache == NULL)
				return 0;

			Cache += fp->SectorOffset;
			max_copy_bytes_in_sector = (Bpb.BytsPerSec - fp->SectorOffset) > (bytes - read_bytes) ? (bytes - read_bytes) : (Bpb.BytsPerSec - fp->SectorOffset);
			memcpy(buffer, Cache, max_copy_bytes_in_sector);

			read_bytes += max_copy_bytes_in_sector;
			fp->SectorOffset += max_copy_bytes_in_sector;
			fp->offset += max_copy_bytes_in_sector;
			buffer = (char*)buffer + max_copy_bytes_in_sector;

			if(fp->SectorOffset == Bpb.BytsPerSec)
			{
				if(i == Bpb.SecPerClus -1)
				{
					Cluster = GetNextClusterNum(Cluster);
					if(Cluster != 0xffff)
						fp->CurrentSectorNum = ClusterNum2SectorNum(Cluster);
				}
				else
					fp->CurrentSectorNum ++;

				fp->SectorOffset = 0;
			}

			if(read_bytes == bytes)
			{
				return bytes;
			}
		}
	}
	
	while(Cluster != 0xffff)
	{
		for(i=0; i< Bpb.SecPerClus; i++)
		{
			Cache = GetSectorData(fp->CurrentSectorNum);
			if(Cache == NULL)
				return 0;

			Cache += fp->SectorOffset;
			max_copy_bytes_in_sector = (Bpb.BytsPerSec - fp->SectorOffset) > (bytes - read_bytes) ? (bytes - read_bytes) : (Bpb.BytsPerSec - fp->SectorOffset);
			memcpy(buffer, Cache, max_copy_bytes_in_sector);
			
			read_bytes += max_copy_bytes_in_sector;
			fp->SectorOffset += max_copy_bytes_in_sector;
			fp->offset += max_copy_bytes_in_sector;
			buffer = (char*)buffer + max_copy_bytes_in_sector;

			if(fp->SectorOffset == Bpb.BytsPerSec)
			{				
				if(i == Bpb.SecPerClus -1)
				{
					Cluster = GetNextClusterNum(Cluster);
					if(Cluster != 0xffff)
						fp->CurrentSectorNum = ClusterNum2SectorNum(Cluster);
				}
				else
					fp->CurrentSectorNum ++;

				fp->SectorOffset = 0;
			}

			if(read_bytes == bytes)
			{
				return bytes;
			}
		}		
	}

	return 0;
}

unsigned int CFileSys::fat_write(int handle, const char* buffer, unsigned int bytes)
{
	BYTE* Cache;
	unsigned int write_bytes =0;
	unsigned int max_write_bytes_in_sector;
	struct _FILE *fp;
	WORD Cluster;
	WORD PrevCluster;
	int i;

	if(handle <0 || handle >= sizeof(handles)/sizeof(struct _FILE))
		return 0;

	fp = &handles[handle];

	Cluster = SectorNum2ClusterNum(fp->CurrentSectorNum);
	PrevCluster = Cluster;

	i = (fp->CurrentSectorNum - FirstDataSector) % Bpb.SecPerClus;

	if(i != 0)
	{
		for(; i< Bpb.SecPerClus; i++)
		{
			Cache = GetSectorData(fp->CurrentSectorNum);
			if(Cache == NULL)
				return 0;

			Cache += fp->SectorOffset;
			max_write_bytes_in_sector = (Bpb.BytsPerSec - fp->SectorOffset) > (bytes - write_bytes) ? (bytes - write_bytes) : (Bpb.BytsPerSec - fp->SectorOffset);
			memcpy(Cache, buffer, max_write_bytes_in_sector);
			SetSectorData(fp->CurrentSectorNum, Cache , Bpb.BytsPerSec);

			write_bytes += max_write_bytes_in_sector;
			fp->SectorOffset += max_write_bytes_in_sector;
			fp->offset += max_write_bytes_in_sector;
			buffer = (char*)buffer + max_write_bytes_in_sector;
			//modify by yafei 2004
			if(fp->offset>=fp->dir.FileSize)
				fp->dir.FileSize = fp->offset ;

			//below is replace by up
			//fp->dir.FileSize +=  max_write_bytes_in_sector;
			if(fp->SectorOffset == Bpb.BytsPerSec)
			{
				if(i == Bpb.SecPerClus -1)
				{
					PrevCluster = Cluster;
					Cluster = GetNextClusterNum(Cluster);
					if(Cluster != 0xffff)
						fp->CurrentSectorNum = ClusterNum2SectorNum(Cluster);
					else
					{
						Cluster = AllocCluster(PrevCluster);
						if(Cluster == 0xffff)
						{
							Flush();
							return 0;
						}
						fp->CurrentSectorNum = ClusterNum2SectorNum(Cluster);
					}
				}
				else
					fp->CurrentSectorNum ++;

				fp->SectorOffset = 0;
			}

			if(write_bytes == bytes)
			{
				Flush();
				return bytes;
			}
		}
	}

	for(;;)
	{
		for(i=0; i< Bpb.SecPerClus; i++)
		{
			Cache = GetSectorData(fp->CurrentSectorNum);
			if(Cache == NULL)
				return 0;

			Cache += fp->SectorOffset;
			max_write_bytes_in_sector = (Bpb.BytsPerSec - fp->SectorOffset) > (bytes - write_bytes) ? (bytes - write_bytes) : (Bpb.BytsPerSec - fp->SectorOffset);
			memcpy(Cache, buffer, max_write_bytes_in_sector);
			SetSectorData(fp->CurrentSectorNum, Cache , Bpb.BytsPerSec);

			write_bytes += max_write_bytes_in_sector;
			fp->SectorOffset += max_write_bytes_in_sector;
			fp->offset += max_write_bytes_in_sector;
			buffer = (char*)buffer + max_write_bytes_in_sector;
			//modify by yafei 2004
			if(fp->offset>=fp->dir.FileSize)
				fp->dir.FileSize = fp->offset ;

			//below is replace by up
			//fp->dir.FileSize +=  max_write_bytes_in_sector;

			if(fp->SectorOffset == Bpb.BytsPerSec)
			{
				if(i == Bpb.SecPerClus -1)
				{
					PrevCluster = Cluster;
					Cluster = GetNextClusterNum(Cluster);
					if(Cluster != 0xffff)
						fp->CurrentSectorNum = ClusterNum2SectorNum(Cluster);
					else
					{
						Cluster = AllocCluster(PrevCluster);
						if(Cluster == 0xffff)
						{
							Flush();
							return 0;
						}
						
						fp->CurrentSectorNum = ClusterNum2SectorNum(Cluster);
					}
				}
				else
					fp->CurrentSectorNum ++;

				fp->SectorOffset = 0;
			}

			if(write_bytes == bytes)
			{
				Flush();
				return bytes;
			}
		}
	}

	// we can not reach here.
	return 0;
}

int CFileSys::fat_remove( const char *filename)
{
	DWORD SectorNum;
	struct _FILE file;
	//locate
	SectorNum = Locate(filename, &file);
	if(SectorNum == 0xffffffff)
		return 4;

	// is it a dir ?
	if(file.dir.Attr & ATTR_DIRECTORY)
		return 6;

	if(DeleteDir(&file) != 0)
		return 5;

	FreeCluster(file.dir.FstClusLO);
	return 0;
}

int CFileSys::fat_get_stat( const char *filename, struct _STAT* stat)
{
	DWORD SectorNum;
	struct _FILE file;

	//locate
	SectorNum = Locate(filename, &file);
	if(SectorNum == 0xffffffff)
		return 1;

	stat->Attr = file.dir.Attr;
	stat->CrtDate = file.dir.CrtDate;
	stat->CrtTime = file.dir.CrtTime;
	stat->CrtTimeTenth = file.dir.CrtTimeTenth;
	stat->FileSize = file.dir.FileSize;
	stat->LstAccDate = file.dir.LstAccDate;
	stat->WrtDate = file.dir.WrtDate;
	stat->WrtTime = file.dir.WrtTime;

	return 0;
}

int CFileSys::fat_set_stat( const char *filename, struct _STAT* stat)
{
	DWORD SectorNum;
	struct _FILE file;
	BYTE* Cache;
	struct _DIR *dir;

	//locate
	SectorNum = Locate(filename, &file);
	if(SectorNum == 0xffffffff)
		return 1;

	file.dir.Attr = stat->Attr;
	file.dir.CrtDate = stat->CrtDate;
	file.dir.CrtTime = stat->CrtTime;
	file.dir.CrtTimeTenth = stat->CrtTimeTenth;
	file.dir.FileSize = stat->FileSize;
	file.dir.LstAccDate = stat->LstAccDate;
	file.dir.WrtDate = stat->WrtDate;
	file.dir.WrtTime = stat->WrtTime;

	Cache = GetSectorData(file.DirSectorNum);
	if(Cache == NULL)
		return 2;

	dir = (struct _DIR *)Cache;
	dir += file.DirIndex;

	memcpy(dir, &file.dir, sizeof(struct _DIR));
	SetSectorData(file.DirSectorNum, Cache , Bpb.BytsPerSec);
	Flush();

	return 0;
}


int CFileSys::fat_rename( const char *oldname, const char *newname )
{
	struct _DIR dir;
	char path[512];
	char newpath[512];
	char name[11];
	char new_name[11];
	char *p;
	DWORD ParentDirSectorNum;
	struct _FILE old_file;

	//
	//check oldname file
	//

	// is path format correct ?
	p = get_valid_format(oldname);
	if(p == NULL)
		return -2;

	//if exist this file ?
	if(Locate(oldname, &old_file) == 0xffffffff)
		return -3;

	//separate path into parent and name
	strncpy(name, &p[strlen(p)-11], 11);

	strcpy(path, oldname);
	p = strrchr(path, '\\');
	*p = '\0';


	//
	//check newname file
	//

	if(strchr(newname, '\\') != NULL)
		return -2;

	sprintf(newpath, "%s\\%s", path, newname);

	// is path format correct ?
	p = get_valid_format(newpath);
	if(p == NULL)
		return -2;

	//if exist this file ?
	if(Locate(newpath, NULL) != 0xffffffff)
		return -3;

	//separate path into parent and name
	strncpy(new_name, &p[strlen(p)-11], 11);



	//locate parent path
	if(path[0]==0)
		ParentDirSectorNum = Locate("\\", NULL);
	else
		ParentDirSectorNum = Locate(path, NULL);
	if(ParentDirSectorNum == 0xffffffff)
		return -4;

	//fill dir attributes
	memcpy(&dir, &old_file.dir, sizeof(struct _DIR));
	memcpy(dir.Name, new_name, 11);

	//alloc one dir
	if(AllocDir(ParentDirSectorNum, &dir, NULL) != 0)
		return -5;

	//delete old one
	if(DeleteDir(&old_file) != 0)
		return -6;

	return 0;
}

int CFileSys::fat_Format(int size)
{
	CECC_Check ecc ;
	BYTE *bpb = new BYTE[2112] ;
	WORD *pFat = new WORD[1056] ;
	BYTE *pfat8 = (BYTE*)pFat ;
	memset(bpb,0,2112);
	memcpy(bpb,fathead,0x200) ;
	memset(pFat,0,2112);
	*(DWORD*)(bpb+32) = 0x3ffff; //总共的扇区数
	ecc.make_ecc_512(&bpb[2048+8],bpb);
	ecc.make_ecc_512(&bpb[2048+8+16],bpb+512);
	ecc.make_ecc_512(&bpb[2048+8+32],bpb+1024);
	ecc.make_ecc_512(&bpb[2048+8+48],bpb+1536);


	pFat[0]=0xFFF8 ;
	pFat[1]=0xFFFF ;
	ecc.make_ecc_512(&pfat8[2048+8],bpb);
	ecc.make_ecc_512(&pfat8[2048+8+16],bpb+512);
	ecc.make_ecc_512(&pfat8[2048+8+32],bpb+1024);
	ecc.make_ecc_512(&pfat8[2048+8+48],bpb+1536);	
	//写入芯片即可


	return 0 ;
}

void CFileSys::SetOPType(FILE_OP_TYPE type)
{
	m_type=type;
}
