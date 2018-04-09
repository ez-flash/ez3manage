// FileSys.h: interface for the CFileSys class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILESYS_H__53C0DAD3_7405_402E_9357_662BCB1072A7__INCLUDED_)
#define AFX_FILESYS_H__53C0DAD3_7405_402E_9357_662BCB1072A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\usb\EzFlashBase.h"

struct _BPB
{
	BYTE NumFATs;	//The count of FAT data structures on the volume.

	BYTE SecPerClus;	//Number of sectors per allocation unit.

	WORD BytsPerSec;	//Count of bytes per sector.

	WORD RsvdSecCnt;	//Number of reserved sectors in the Reserved region of the volume starting at the first sector of the volume.

	WORD RootEntCnt;	//this field contains the count of 32-byte directory entries in the root directory.

	DWORD TotSec;	//This field is the total count of sectors on the volume.

	DWORD FATSz;	//This field is the FAT12/FAT16 16-bit count of sectors occupied by ONE FAT.
};

#define ERCHAR	0xE5

struct _DIR
{
	BYTE Name[11];
	BYTE Attr;
	BYTE NTRes;
	BYTE CrtTimeTenth;
	WORD CrtTime;
	WORD CrtDate;
	WORD LstAccDate;
	WORD FstClusHI;
	WORD WrtTime;
	WORD WrtDate;
	WORD FstClusLO;
	DWORD FileSize;
};

#define ATTR_READ_ONLY	0x01
#define ATTR_HIDDEN		0x02
#define ATTR_SYSTEM		0x04
#define ATTR_VOLUME_ID	0x08
#define ATTR_DIRECTORY	0x10
#define ATTR_ARCHIVE	0x20

//The sector number of the first sector of that cluster.
//FirstSectorofCluster = ((N – 2) * BPB_SecPerClus) + FirstDataSector;
#define FirstSectorofCluster(N)	(((N – 2) * Bpb.SecPerClus) + FirstDataSector)


//
//FAT16 Apis
//

struct _STAT
{
	BYTE Attr;
	BYTE CrtTimeTenth;
	WORD CrtTime;
	WORD CrtDate;
	WORD LstAccDate;
	WORD WrtTime;
	WORD WrtDate;
	DWORD FileSize;
};

struct _FILE
{
	int valid; // 1 valid, 0 free.

	DWORD DirSectorNum;
	int DirIndex;

	DWORD StartSectorNum;
	DWORD CurrentSectorNum;
	DWORD SectorOffset;

	struct _DIR dir;

	unsigned long offset;
};

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

struct _FatDate
{
	WORD Day : 5;
	WORD Month : 4;
	WORD Year : 7;
};

struct _FatTime
{
	WORD Second_2s : 5;
	WORD Minutes : 6;
	WORD Hours : 5;
};

struct _FatDateTime
{
	union
	{
		struct _FatDate fatdate;
		WORD Date;
	}Date;

	union
	{
		struct _FatTime fattime;
		WORD Time;
	}Time;

	BYTE TimeTenth;
};

struct _FatGet
{
	DWORD DirSectorNum;
	int DirIndex;

	int IsRootDir;

	char filename[13];
};
#define WM_NAND_WRITE_BEGIN_MESSAGE	WM_USER+10
#define WM_NAND_WRITE_END_MESSAGE	WM_USER+11
#define WM_NAND_WRITE_STEP_MESSAGE  WM_USER+12
#define WM_NAND_READ_STEP_MESSAGE   WM_USER+13
typedef enum 
{
	OPNONE=-1,
	CREATEDIR=2,	//创建目录
	DELDIR=3,		//删除目录
	CREATEFILE=4,	//创建文件
	DELFILE=5,    //删除文件
	READFILE=6,
}FILE_OP_TYPE;//拖动源头的枚举结构
class CDiskCache
{
public:
	CDiskCache(PVOID MemoryDisk);
	virtual ~CDiskCache();

	BYTE* GetSectorData(DWORD StartSector);
	BOOL Flush();
	int SetSectorData(DWORD StartSector, BYTE* Buffer, DWORD Size);
	
	BOOL FormatCart(CProgressCtrl *m_pProgressCtrl,BOOL bQuick,CString strVolume);


	virtual void CartInit();
	virtual void ClearCartInit();
	void SetMessageHandle(HWND hWnd);
	CEzFlashBase *m_pEZActual;
private:
	PVOID MemDisk;
	const int BytesPerSector;
	HANDLE hDev;
	BYTE *PageCache;
	BYTE *SectorCache;
	BYTE *BlcokCache; 
	BYTE *BlcokCacheWithSpare;  //带spare的块缓冲
	DWORD CurrentCachePage;
	int  m_iCurrentBlock ;
	HWND m_hWnd; 
	int	 m_pos;
	int  m_readpos;
protected:
	FILE_OP_TYPE m_type;

	struct _BPB Bpb;
	DWORD RootDirSectors;	// Numbers of sectors occupied by Root Directory.
	DWORD FirstDataSector;	//The first sector of cluster 2 (the data region of the disk).
	DWORD FirstFatSecNum;	//Start sector of the first fat data structure.
	DWORD FirstRootDirSecNum;	//Start sector of root directory.
	DWORD	dw_NandSize;

};

class CFileSys : public CDiskCache //, CPeriEq
{
public:	
	CFileSys(PVOID MemoryDisk);
	virtual ~CFileSys();
	
	BOOL Initialize();
	void Uninitialize();
	void Callback();	
	void SetOPType(FILE_OP_TYPE type);
public:	
	DWORD GetFreeNandSpace();//返回的单位是KByte
	DWORD GetNandSize();
	int fat_mkdir( const char *dirname );
	int fat_rmdir( const char *dirname );
	int fat_getfirst(const char *path, char* filename);
	int fat_getnext(char* filename);
	
	int fat_close(int handle);
	int fat_creat(const char* filename, BYTE attribute);
	long fat_lseek(int handle, long offset, int origin);
	int fat_open(const char* filename);
	unsigned int fat_read(int handle, void* buffer, unsigned int bytes);
	unsigned int fat_write(int handle, const char* buffer, unsigned int bytes);
	
	int fat_remove( const char *filename);
	int fat_get_stat( const char *filename, struct _STAT* stat);
	int fat_set_stat( const char *filename, struct _STAT* stat);
	int fat_rename( const char *oldname, const char *newname );
	//add by yafei
	int CFileSys::fat_Format(int size);
	void Unicode2ansi(char*pfrom ,char*pto);
	void ansi2Unicode(char*pfrom ,char*pto) ;
	WORD AllocAllianedCluster(WORD PrevClusterNum);
	int  fat_Instancesave_creat(const char* filename, BYTE attribute);
private:
	//helper functions
	inline WORD SectorNum2ClusterNum(DWORD SectorNum);	
	inline DWORD ClusterNum2SectorNum(WORD ClusterNum);	
	inline WORD GetNextClusterNum(WORD ClusterNum);	

	WORD AllocCluster(WORD PrevClusterNum);
	void FreeCluster(WORD StartClusterNum);

	int AllocDir(DWORD ParentDirSectorNum, struct _DIR* new_dir, struct _FILE * fp);
	int DeleteDir(struct _FILE *file);
	
	DWORD SectorSearch(DWORD Sector, const char dirname[11], struct _FILE *file);
	int SectorGet(DWORD Sector, struct _FatGet *fat_get);
	DWORD Search(DWORD Sector, const char dirname[11], struct _FILE *file);
	DWORD Locate(const char *path, struct _FILE *file);

	void datetime(struct _FatDateTime *fatdatetime);
	char* get_valid_format(const char *fullpath);
	void unformat_name(char * filename, const unsigned char dirname[11]);

	
private:
	
	WORD* FatCache;
	DWORD dw_FreeSpace;
	struct _FatGet fat_get;
	struct _FILE handles[16];
};

#endif // !defined(AFX_FILESYS_H__53C0DAD3_7405_402E_9357_662BCB1072A7__INCLUDED_)
