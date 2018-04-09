// New_EZManageDlg.h : header file
//

#if !defined(AFX_NEW_EZMANAGEDLG_H__853E4E5D_8138_4C23_B9CA_E520DE0E8EAC__INCLUDED_)
#define AFX_NEW_EZMANAGEDLG_H__853E4E5D_8138_4C23_B9CA_E520DE0E8EAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PageFlash.h"
#include "PageDisk.h"
#include "DirTreeCtrl.h"
#include "MyOleDropTarget.h"
#include "rommanager.h"
#include <afxtempl.h>
#include "ScrollLink.h"
#include "Label.h"
#include "TextProgressCtrl.h"
#include ".\usb\EzFlashBase.h"
#include "GameNameInfo.h"
#include "GB2Big5.h"
/////////////////////////////////////////////////////////////////////////////
// CNew_EZManageDlg dialog
#define BIT0(n)		((n)&0x1)
#define BIT1(n)		((n)&0x2?1:0)
#define BIT2(n)		((n)&0x4?1:0)
#define BIT3(n)		((n)&0x8?1:0)
#define BIT4(n)		((n)&0x10?1:0)
#define BIT5(n)		((n)&0x20?1:0)
#define BIT6(n)		((n)&0x40?1:0)
#define BIT7(n)		((n)&0x80?1:0)
#define BIT8(n)		((n)&0x100?1:0)
#define BIT9(n)		((n)&0x200?1:0)
#define BIT10(n)	((n)&0x400?1:0)
#define BIT11(n)	((n)&0x800?1:0)
#define BIT12(n)	((n)&0x1000?1:0)
#define BIT13(n)	((n)&0x2000?1:0)
#define BIT14(n)	((n)&0x4000?1:0)
#define BIT15(n)	((n)&0x8000?1:0)


#define SETFORMATETC(fe,cf,asp,td,med,li)\
	((fe).cfFormat=cf,\
	 (fe).dwAspect=asp,\
	 (fe).ptd=td,\
	 (fe).tymed=med,\
	 (fe).lindex=li)

typedef struct _Code{
	DWORD	m_dwAddress;		//地址4 Byte
	BYTE	m_register;			//寄存器件1Byte
	BYTE	reserved1 ;
	WORD	reserved2 ;
}CODE,*PCODE;
typedef struct _XCode{
	UINT	sign;				//标志必须为XCDE	4Byte
	BYTE	gameName[12];		//游戏名			12Byte
	DWORD	structSize;			//整个结构的大小	4Byte
	WORD	CodeSize;			//XCode个数			2Byte
	WORD	dataSize;			//数据个数			2Byte
	CODE	*pCode;
}XCODE,*PXCODE;


typedef struct _GOLD_DATA{
	UINT	sign;				//标志为GOLD 4 Byte
	BYTE	m_descriptor[16];	//描述符号16 Byte
	BYTE    m_setup[16];
	DWORD	m_dwAddress;		//地址4 Byte
	DWORD	m_dwData;			//数据4 Byte
	BYTE	m_dataType;			//数据类型1 Byte
	BYTE	m_bUse;				//使用/未使用1 Byte
	WORD	m_loopCounter;		//循环自加2 Byte
}GOLD_DATA , *PGOLD_DATA;

typedef struct _GOLDDATA{
		DWORD  Address;
		DWORD  Data;
		DWORD  DataType;
		DWORD  LoopCounter;   
}GOLDDATA,*PGOLDDATA;

typedef struct _GoldCodeItem{
		char 		ItemName[14];
		WORD   		ItemSize;
		GOLDDATA	*pGlodData;
}GOLDCODEITEM,*PGOLDCODEITEM;

typedef struct _GoldCheat{
		char Totalname[12];
		WORD TotalSize;
		WORD SumofItem;
		GOLDCODEITEM *pItem;
}GOLDCHEAT , *PGOLDCHEAT;

typedef struct {
	UINT		uiIndex ;		//第几号
	UINT		uiWhere ;		//从rom中还是准备写入的文件文件 , 0 rom , 1 from file , '_Ldr'表示loader 特殊,便于计算大小
	SAVERTYPE	uiSaverMode ;	//如果是准备写入文件，这个是文件的保存方式
	DWORD 		uiSaverSize ;	//源文件的存档大小 ， 可修改
	int			flGameSize ;	//游戏大小
	char		strGameWhere[256] ;	//当为要的烧录文件时，为其文件地址
	char		strGameEnglishName[32];
	char		strGameChinaName[48];
	char		strGameName[12];
	char		strIpsPatchFileName[256];
	BOOL		bReset;				//是否有软复位
	BOOL		bBoma;				//是否有Boma
	BOOL		bCheat;				//是否有金手指
	BOOL		bNdsRom;			//是否是NdsROM
	BOOL		bHomebrew;
	BOOL		bSaveNow;			//是否有即时存档
	BYTE		SavePatch;			//是否有存档补丁
	BYTE*		pGameData ;
	BYTE	   *pXCode;
	char		password[8];		//密码
	PGOLDCHEAT	pGlodCheat; 
	WORD		CheatSize;
	BYTE		bZip;				//==1表示压缩,==0表示没有压缩
}GameRomInfo ,	*PGameRomInfo;

typedef CArray<PXCODE,PXCODE> XCODE_ARRAY ;
typedef CArray<PGOLD_DATA,PGOLD_DATA> GOLD_DATA_ARRAY;

typedef struct
{
	unsigned __int32	start_code;		// B instruction
	unsigned char		logo[0xA0-0x04];	// logo data
	char				title[0xC];		// game title name
	unsigned __int32	game_code;		//
	unsigned __int16		maker_code;		//
	unsigned char		fixed;			// 0x96
	unsigned char		unit_code;		// 0x00
	unsigned char		device_type;	// 0x80
	unsigned char		unused[7];		//
	unsigned char		game_version;	// 0x00
	unsigned char		complement;		// 800000A0..800000BC
	unsigned __int16		checksum;		// 0x0000
} ROMHEADER ;

typedef struct
{
	char				title[0xc];
	unsigned __int32	game_code ;
	unsigned __int16	maker_code;
	unsigned __int8		uinit_code;
	unsigned __int8		device_code;
	unsigned __int8		card_size ;//2^(20+s) = xx Mb bit 为单位
	char				card_info[0xA];
	unsigned __int8		flags ;
	//arm9 code
	unsigned __int32	arm9_source ;
	unsigned __int32	arm9_exec ;
	unsigned __int32	arm9_copyto ;
	unsigned __int32	arm9_size ;
	//arm7 code
	unsigned __int32	arm7_source ;
	unsigned __int32	arm7_exec ;
	unsigned __int32	arm7_copyto ;
	unsigned __int32	arm7_size ;
	//filename table
	unsigned __int32	nametable_offset ;
	unsigned __int32	nametable_size ;
	unsigned __int32	fat_offset ;
	unsigned __int32	fat_size ;
	//overlay
	unsigned __int32	overlay9_source ;
	unsigned __int32	overlay9_size ;
	unsigned __int32	overlay7_source ;
	unsigned __int32	overlay7_size ;
	//unknown
	unsigned __int32	unk1 ;
	unsigned __int32	unk2 ;
	unsigned __int32	icon ;
	unsigned __int16	secure_crc16 ;
	unsigned __int16	rom_timeout ;
	unsigned __int32	arm9_unk ;
	unsigned __int32	arm7_unk ;
	char				unk3c[0x8];
	unsigned __int32	rom_size ;
	unsigned __int32	header_size ;
	char				zero[56] ;
	char				GBAlogo[156];
	unsigned __int16	logo_crc16;
	unsigned __int16	header_crc16 ;
	char				reserved[160] ;
}NDSHEADER ;

typedef CArray<PGameRomInfo,PGameRomInfo> GAMEINFOLIST;
#define Nintendo_logo_LEN 156
const unsigned char Nintendo_logo[]={
	0x24,0xff,0xae,0x51,0x69,0x9a,0xa2,0x21,
	0x3d,0x84,0x82,0x0a,0x84,0xe4,0x09,0xad,
	0x11,0x24,0x8b,0x98,0xc0,0x81,0x7f,0x21,
	0xa3,0x52,0xbe,0x19,0x93,0x09,0xce,0x20,
	0x10,0x46,0x4a,0x4a,0xf8,0x27,0x31,0xec,
	0x58,0xc7,0xe8,0x33,0x82,0xe3,0xce,0xbf,
	0x85,0xf4,0xdf,0x94,0xce,0x4b,0x09,0xc1,
	0x94,0x56,0x8a,0xc0,0x13,0x72,0xa7,0xfc,
	0x9f,0x84,0x4d,0x73,0xa3,0xca,0x9a,0x61,
	0x58,0x97,0xa3,0x27,0xfc,0x03,0x98,0x76,
	0x23,0x1d,0xc7,0x61,0x03,0x04,0xae,0x56,
	0xbf,0x38,0x84,0x00,0x40,0xa7,0x0e,0xfd,
	0xff,0x52,0xfe,0x03,0x6f,0x95,0x30,0xf1,
	0x97,0xfb,0xc0,0x85,0x60,0xd6,0x80,0x25,
	0xa9,0x63,0xbe,0x03,0x01,0x4e,0x38,0xe2,
	0xf9,0xa2,0x34,0xff,0xbb,0x3e,0x03,0x44,
	0x78,0x00,0x90,0xcb,0x88,0x11,0x3a,0x94,
	0x65,0xc0,0x7c,0x63,0x87,0xf0,0x3c,0xaf,
	0xd6,0x25,0xe4,0x8b,0x38,0x0a,0xac,0x72,
	0x21,0xd4,0xf8,0x07
   };
typedef enum {
	NON_EZ3Cart = -1 ,
	EZ3_Cart,
}EZ3CARTTYPE;


#define ROM_SIZE 0x2000000 

#define WM_ADDROM		WM_USER + 0x2000
#define WM_DELROM		WM_USER + 0x2001
#define WM_BACKROM		WM_USER + 0x2002
#define WM_FRESH		WM_USER + 0x2003
#define WM_FORMATROM	WM_USER + 0x2004
#define WM_WRITEROM		WM_USER + 0x2005

#define WM_CREATEFLODER WM_USER + 0x2006
#define WM_REMOVEFOLDER WM_USER + 0x2007
#define WM_DELETEFILE   WM_USER + 0x2008
#define WM_READFILE		WM_USER + 0x2009
#define WM_PREFOLDER	WM_USER + 0x200A
#define WM_FORMATNAND	WM_USER + 0x200B
#define WM_BAKCUPFOLDER WM_USER + 0x200C

#define IDENTIFY_CART_TIMER		0x4000000

#define NAND_ID		0x80EC15F1
#define NOR_ID		0x2b2a2b2a
#define LDR_ID1		0x0021001C
#define LDR_ID2		0x0039001C
#define LDR_ID3		0x002b001C
#define LDR_ID4		0x0020001C


#define NOR_FLASH_SIZE 0x2000000
#define NAND_TOTAL_SIZE		128*1024

#define INS_SIZE	0x6c0
class CNew_EZManageDlg : public CDialog
{
private:
	BOOL			m_bUsePassMe2;
	BOOL			m_bNdsLoader;
	BOOL			m_bTimesNand;
	BOOL			m_bTimeNor;
	BOOL			m_bWriteIns;
	BOOL			m_FirstRun;
	BOOL			m_bUpdtaLoader;
	CString			m_strFilePathName;
	CString			m_strWindowsDir;
	CStringArray	m_MenustringArray;
	CString			m_strLanguage;
	CGameNameInfo *m_pGameNameInfo;
	CString			m_backstrFileName;
	CString			m_strBackfileSaver;
	WORD			m_LoaderVersion;
	CGB2Big5  m_bb ;

	BOOL bChangeName ;
	CStringArray m_strArray;
	CString		 m_strDriverVersion;
	void WriteEZPassMe2Function();
	void InitMenu();
	void InitInterface();
	CString GetNewFileName(CString strName,int index);
	void AnalyzeCart();
	void ReadEZ3NorFlash();
	BOOL SetROMInfo(GameRomInfo *pInfo ,
								UINT		uiIndex ,		//第几号
								UINT		uiWhere ,		//从rom中还是准备写入的文件文件 , 0 rom , 1 from file , '_Ldr'表示loader 特殊,便于计算大小
								SAVERTYPE	uiSaverMode ,	//如果是准备写入文件，这个是文件的保存方式
								DWORD 		uiSaverSize ,	//源文件的存档大小 ， 可修改
								UINT		flGameSize ,	//游戏大小
								LPCTSTR		strGameWhere ,	//当为要的烧录文件时，为其文件地址
								LPCTSTR		strGameEnglishName,
								LPCTSTR		strGameChinaName,
								BYTE*		strGameName,
								BOOL		bReset,				//是否有软复位
								BOOL		bBoma,				//是否有Boma
								BOOL		bCheat,				//是否有金手指
								BYTE		bSavePatch,		//是否有存档补丁
								BYTE*		pGameData ,
								BYTE	   *pXCode,
								PGOLDCHEAT	pGoldData
								);
	BYTE *GetChinaNameBuffer();

	CWinThread		*pTWThread;
	static UINT TWThreadWrite( LPVOID pParam );
	UINT TWThreadWriteRom();
	static UINT TWThreadFormatRom( LPVOID pParam );
	UINT TWThreadFormatRom();
	static UINT TWThreadBackupRom( LPVOID pParam );
	UINT TWThreadBackupRom();
	static UINT TWThreadWriteLoader( LPVOID pParam );
	UINT TWThreadWriteLoader();
	
	//创建Nand的目录的函数和变量
	CWinThread		*pThreadNandCreateDir;
	static UINT TRThreadNandCreateDir(LPVOID pParam);
	UINT TRThreadProcessNandCreateDir(CNew_EZManageDlg *pThis);
	//删除Nand目录的函数和变量
	CWinThread		*pThreadNandDelDir;
	static UINT TRThreadNandDelDir(LPVOID pParam);
	UINT TRThreadProcessNandDelDir(CNew_EZManageDlg *pThis);
	//写Nand的函数和变量
	CWinThread		*pThreadNandWrite;
	static UINT TRThreadNandWrite(LPVOID pParam);
	UINT TRThreadProcessNand(CNew_EZManageDlg *pThis);
	
	CWinThread      *pThreadWriteMoreNand;
	UINT			TWThreadWriteMoreNand();
	static UINT TWThreadWriteMoreNand(LPVOID pParam);

	//删除Nand文件的函数和变量
	CWinThread		*pThreadNandDelFile;
	static UINT TRThreadNandDelFile(LPVOID pParam);
	UINT TRThreadProcessNandDelFile(CNew_EZManageDlg *pThis);
	//读EZ-Disk的函数和变量
	CWinThread		*pThreadNandReadFile;
	static UINT TRThreadNandReadFile(LPVOID pParam);
	UINT TRThreadProcessNandReadFile(CNew_EZManageDlg *pThis);
	//读EZ3-存档目录所有内容
	static UINT TRThreadNandBackupSave(LPVOID pParam);
	UINT TRThreadProcessBackupSave(CNew_EZManageDlg *pThis);
	

	void WriteChinaNameBuffer(GAMEINFOLIST *pList);
	DWORD FillGoldData(GameRomInfo *pInfo,CRomManager rm);
	DWORD FillGoldDataAndSaveNow(GameRomInfo *pInfo,CRomManager rm,int NandAdd);
	DWORD FoundFreeSpaceFormRom(BYTE **pGameDataBegin,int &GameSize,DWORD Blocksize);
	//动态升级，在线显示数据
	CWinThread		*m_pThreadConnection;
	static UINT TThreadConnection( LPVOID pParam );
	UINT TThreadConnectionProc(CNew_EZManageDlg *pThis);

	void WriteLoader();
	BOOL Compress_LZ77_DosCommand(CString strSourceFile,CString strTargetFile);
	CString ModifyBmpFile(CString strFileName);
	int ExecCommand(char* bstrCmd);
	BOOL CheckNandDisk(DWORD size);
	void FillNandGameInfo(GameRomInfo *pInfo);
	BOOL bLoaderVersion(BYTE *pLoad);
	CString GetLoaderVersionFromCard(WORD Version);
	CString GetDriverVersionInfo();
public:
	char *GetZipData(CString strZipFileName,unsigned long &length);
	BYTE* ExtractArchive(char *ArcName,DWORD &dw_Len);
	GameRomInfo *GetRomInfo(CString strRomName);
	XCODE *FindXCode(BYTE *pGameData,char romname[12],DWORD GameSize);
	DWORD GetSaverTypeAndSize(BYTE **buf,DWORD *length,SAVERTYPE *type);
	BYTE * FindMotif(BYTE *Buffer, DWORD BufferSize, BYTE *Motif, DWORD MotifSize);
	void TrimRom(BYTE **pBuf, int *iSize);

	void AddItemToNorList(GameRomInfo *pInfo,int index=-1);
	BOOL CheckRom(DWORD dwLastGameLength);
	BOOL HasSameNameInList(CString strName);
	BOOL HasSameNameInArray(CString strName,CStringArray *pArray);

// Construction
public:
	CEzFlashBase	*m_pezVirtualBase ;
	CPageFlash m_PageFlash;
	CPageDisk  m_PageDisk;
	CMyOleDropTarget DropTarget;
	CString m_strWorkPath;
	GAMEINFOLIST *pGameInfoList;

	CNew_EZManageDlg(CWnd* pParent = NULL);	// standard constructor
		
	void ReadNandFlash();
	void AddDropDataToNorList(COleDataObject* pDataObject);
	BOOL AddDropDataToDiskList(COleDataObject* pDataObject);
	BOOL AddFileToDiskList(COleDataObject* pDataObject);

	
	void ReMoveAllGameList();
	void DeleteGoldCheatPointer(GOLDCHEAT *pGoldCheat,WORD size);

	void ShowRomUseInfo(BOOL bCart = FALSE);
	void ShowNandUseInfo(int TotalSize,int NouseSize,BOOL bCart=FALSE);

// Dialog Data
	//{{AFX_DATA(CNew_EZManageDlg)
	enum { IDD = IDD_NEW_EZMANAGE_DIALOG };
	CStatic	m_StaticWritePos;
	CTextProgressCtrl	m_WriteProgress;
	CTextProgressCtrl	m_RomProgress;
	CScrollLink	m_ctrlHyperlinkScroller;
	CDirTreeCtrl	m_DirTree;
	CTabCtrl	m_Tab;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNew_EZManageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CNew_EZManageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSetpath();
	afx_msg void OnBegindragTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnAddLoader();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateAddLoader(CCmdUI* pCmdUI);
	afx_msg void OnSyncClock();
	afx_msg void OnUpdateSyncClock(CCmdUI* pCmdUI);
	afx_msg void OnUpdataFile();
	virtual void OnOK();
	afx_msg void OnOk();
	afx_msg void OnUpdateUpdataFile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddNdsloader(CCmdUI* pCmdUI);
	afx_msg void OnAddNdsloader();
	afx_msg void OnPassme2();
	afx_msg void OnUpdatePassme2(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnAddRom(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnDelRom(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnBackRom(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnFreshRom(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnFormatRom(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnWriteRom(WPARAM wparam, LPARAM lparam);	

	afx_msg LRESULT OnCreateFolder(WPARAM wparam, LPARAM lparam);	
	afx_msg LRESULT OnRemoveFolder(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnDeleteFile(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnReadFile(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnPreFolder(WPARAM wparam, LPARAM lparam);		
	afx_msg LRESULT OnFormatNand(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnBackupFolder(WPARAM wparam, LPARAM lparam);
	
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEW_EZMANAGEDLG_H__853E4E5D_8138_4C23_B9CA_E520DE0E8EAC__INCLUDED_)
