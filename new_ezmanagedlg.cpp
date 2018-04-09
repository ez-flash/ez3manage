// New_EZManageDlg.cpp : implementation file
//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "stdafx.h"
#include "New_EZManage.h"
#include "New_EZManageDlg.h"
#include "unzip.h"
#include "unrar.h"
#include <math.h>
#include "GoldenCheat.h"
#include "XCodeFile.h"
#include ".\usb\AmdEz3FlashBase.h"
#include ".\usb\FujiEZ3Flash1g.h"
#include "EfaCheat.h"
#include ".\fat16\FileSys.h"
#include "DlgNandFormat.h"
#include "RomEditDlg.h"
#include "Lz77.h"
#include "Rescvt.h"
#include "bmp.h"
#include "SkinPlusPlus.h"
#include <afxinet.h>
#include "Wininet.h"
#include "DlgOSSelect.h"
#include "Winbase.h"
#include "Windows.h"
#include "Winver.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
/************************************************************************************************/
///声明几个全局变量，方便使用，方便处理
CRITICAL_SECTION	g_statusCritical ;
BOOL				g_IsEzWriterConnected = FALSE ;
BOOL				g_IscartConnected = FALSE ;
EZ3CARTTYPE			g_CartType = NON_EZ3Cart ;
BOOL				g_Quit = FALSE;
BOOL				b_SysBusy = FALSE;
DWORD				g_dwNandFreeSpace = 0;
DWORD				g_dwNandSize=0;
UINT ZipNDS ;
CFileSys fs(NULL);
/************************************************************************************************/
//软件所用到的字符串
//1.菜单
CString m_gMenuSysConfig;				//系统设置
CString m_gMenuInstallLoader;			//安装EZ3操作系统
CString m_gMenuInstallNdsLoader;
CString m_gMenuSynClock;				//同步系统时钟;
CString m_gMenuDownLoadFiler;			//升级数据文件;
CString m_gMenuPassMe2;					//兼容EZPassMe2
CString m_gMenuLanguage;				//语言选择;
//2.本地文件树
CString m_gStrLocalDisk;				//本地磁盘
CString m_gStrPath;						//磁盘路径
//3.Tab
CString m_gStrConfig;					//烧录配置
CString m_gStrCapacityInfo;				//容量信息
CString m_gStrWriteInfo;				//烧录信息

CString m_gStrNorFlashPage;				//256M Bits EZ Flash 管理
CString m_gStrNorIndex;					//序号
CString m_gStrNorRomName;				//ROM名
CString	m_gStrNorRomSize;				//ROM大小
CString	m_gStrNorRomEdit;				//ROM信息编辑
CString m_gStrNorRomState;				//ROM状态
CString m_gStrAddRom;					//添加Rom
CString m_gStrDelRom;					//删除Rom
CString m_gStrBackupRom;				//备份Rom
CString m_gStrRefresh;					//刷新Rom
CString m_gStrFormatRom;				//格式化Rom
CString m_gStrWriteRom;					//烧录Rom

CString m_gStrdiskPage;					//1G Bits EZ-Disk 管理
CString m_gStrDiskName;					//名称
CString m_gStrDiskSize;					//大小
CString m_gStrDiskType;					//类型
CString m_gStrDiskModifyData;			//修改日期
CString m_gStrAttribute;				//属性
CString m_gStrDiskReName;				//重命名
CString m_gStrDiskRead;					//读文件
CString m_gStrDiskDelFile;				//删除文件
CString m_gStrDiskPreFolder;			//回上级目录
CString m_gStrDiskCreateFolder;			//创建文件夹
CString m_gStrDiskDelFolder;			//删除文件夹
CString m_gStrDiskFormatDisk;			//格式化磁盘
CString m_gStrDiskrefresh;				//刷新磁盘
//[DiskFormat]
CString m_gStrMsgFormatDisk;			//格式化EZ磁盘
CString m_gStrMsgCapacity;				//容量
CString m_gStrMsgFileSystem;			//文件系统
CString m_gStrMsgAllocationUint;		//分配单元大小
CString m_gStrMsgVolume;				//卷标
CString m_gStrMsgFormatItem;			//格式化选项
CString m_gStrMsgQuickFormat;			//格式化选项
CString m_gStrBeginFormat;				//开始格式化
CString m_gStrCancleFormat;				//取消
//[RomEdit]
CString m_gStrMsgRomEditInfo;			//ROM信息编辑
CString m_gStrMsbBaseInfo;
CString m_gStrMsgRomName;				//ROM 名
CString m_gStrMsgRomSize;				//ROM大小
CString m_gStrMsgSoftReset;				//软复位
CString m_gStrMsgInstantSave;
CString m_gStrMsgCompress;				//压缩
CString m_gStrMsgSaveType;				//存档方式
CString m_gStrMsgSaveSize;				//存档大小
CString m_gStrMsgAddPatch;				//打存档补丁
CString m_gStrMsgChectInfo;				//金手指信息
CString m_gStrMsgOK;					//确定
CString m_gStrMsgCancle;
//[MsgInfo]
CString m_gStrMsgReading	;					//正在读EZ-Disk...
CString m_gStrMsgReadEnd;//=读EZ-Disk结束
CString m_gStrMsgCation;//=提示
CString m_gStrMsgNoWrited;//=未烧录
CString m_gStrMsgWrited;//=已烧录
CString m_gStrMsgUse;//=Use
CString m_gStrMsgLeft;//=剩余
CString m_gStrMsgReserve;//=保留
CString m_gStrMsgFolder;;//=文件夹
CString m_gStrMsgNoCart;//=没有找到EZ3卡
CString m_gStrMsgEZDisk;//=EZ磁盘
CString m_gStrMsgInstallOS;//=正在安装EZ3操作系统...
CString m_gStrMsgInstallOSUp;
CString m_gStrMsgInstallOSEnd;//=EZ3操作系统安装完毕
CString m_gStrMsgNoEnough;//=没有足够的Flash ROM空间
CString m_gStrMsgIncalideFile;//=无效的ROM文件,不能添加到Flash区
CString m_gStrMsgSelectFolder;//=请选择本地文件路径
CString m_gStrMsgSlectBackRom;//=请选择备份的ROM
CString m_gStrMsgErrorBackRom;//=错误的备份ROM
CString m_gStrMsgBackRomEnd;//=备份ROM结束
CString m_gStrMsgFormatNorCation;//=格式化256M Bits Flash将丢失所有数据
CString m_gStrMsgFormatingNor;//=正在格式化256Mfalsh区...
CString m_gStrMsgFormatNorEnd;//=格式化256Mfalsh结束
CString m_gStrMsgErasing;//=正在擦除
CString m_gStrMsgWriting;//=正在写...
CString m_gStrMsgWriteEnd;//=烧录结束
CString m_gStrMsgCreatingFolder;//=正在创建目录...
CString m_gStrMsgCreateFolderOK;//=创建目录成功
CString m_gStrMsgDeletingFolder;//=正在删除目录...
CString m_gStrMsgDelSaveCation;//=saver文件夹不能改名
CString m_gStrMsgEnterFileName;//=请输入合法的文件名
CString m_gStrMsgSameCation;//=重名,不能修改文件名
CString m_gStrMsgDelFolderEnd;//=删除目录操作结束
CString m_gStrMsgReadFileEnd;//=读文件结束
CString m_gStrMsgSelectFile;//=请选择待删除文件
CString m_gStrMsgDeletingFile;//=正在删除文件...
CString m_gStrMsgIsDelFile;//=确认删除文件
CString m_gStrMsgDleFileOK;//=删除文件结束
CString m_gStrMsgCompressing;//=正在压缩数据...
CString m_gStrMsgFormatcation;//=EZ磁盘没有格式化,请按确定格式化
CString m_gStrMsgCurrent;//=当前目录
CString m_gStrMsgRootDir;//=当前目录是根目录
CString m_gStrMsgMore128MInfo;
CString m_gStrMsgLanguageChange;
CString m_gStrMsgOSChange;
CString m_gStrMsgDownLoad;//=正在下载数据文件;
CString m_gStrMsgDownLoadEnd;//=正在下载数据文件结束,请重新安装EZ3操作系统;
/************************************************************************************************/
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNew_EZManageDlg dialog

CNew_EZManageDlg::CNew_EZManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNew_EZManageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNew_EZManageDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pGameInfoList = new GAMEINFOLIST;
	InitializeCriticalSection(&g_statusCritical);
	m_pezVirtualBase = NULL;
	pTWThread = NULL;
	m_bTimesNand = TRUE;
	m_bTimeNor = TRUE;
	m_bWriteIns = FALSE;
	m_FirstRun = TRUE;
	m_bUpdtaLoader = FALSE;
	m_bNdsLoader = FALSE;
	m_strFilePathName.Empty();
		
	LPTSTR pStr = new char[1024];
	GetWindowsDirectory (pStr,1024);
	m_strWindowsDir = pStr;

	GetCurrentDirectory(256,pStr);
	m_strWorkPath = pStr;

	
	m_pGameNameInfo = new CGameNameInfo;
	m_pGameNameInfo->Init(m_strWorkPath+ _T("\\langdata\\romname.lst"));
	m_bb.Initial();
	
	m_backstrFileName.Empty();
	m_strBackfileSaver.Empty();
	delete []pStr;
}

void CNew_EZManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNew_EZManageDlg)
	DDX_Control(pDX, IDC_STATIC_WRITEPOS, m_StaticWritePos);
	DDX_Control(pDX, IDC_PROGRESS_POS, m_WriteProgress);
	DDX_Control(pDX, IDC_PROGRESS_ROMUSE_INFO, m_RomProgress);
	DDX_Control(pDX, IDC_STATIC_HYPERLINKSCROLLER, m_ctrlHyperlinkScroller);
	DDX_Control(pDX, IDC_TREE1, m_DirTree);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNew_EZManageDlg, CDialog)
	//{{AFX_MSG_MAP(CNew_EZManageDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON_SETPATH, OnButtonSetpath)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE1, OnBegindragTree1)
	ON_WM_DESTROY()
	ON_COMMAND(IDD_ADD_LOADER, OnAddLoader)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(IDD_ADD_LOADER, OnUpdateAddLoader)
	ON_COMMAND(IDD_SYNC_CLOCK, OnSyncClock)
	ON_UPDATE_COMMAND_UI(IDD_SYNC_CLOCK, OnUpdateSyncClock)
	ON_COMMAND(IDD_UPDATA_FILE, OnUpdataFile)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_UPDATE_COMMAND_UI(IDD_UPDATA_FILE, OnUpdateUpdataFile)
	ON_COMMAND(IDD_ADD_NDSLOADER, OnAddNdsloader)
	ON_COMMAND(IDD_PASSME2, OnPassme2)
	ON_UPDATE_COMMAND_UI(IDD_PASSME2, OnUpdatePassme2)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ADDROM, OnAddRom)
	ON_MESSAGE(WM_DELROM, OnDelRom)		
	ON_MESSAGE(WM_BACKROM,OnBackRom)		
	ON_MESSAGE(WM_FRESH,OnFreshRom)		
	ON_MESSAGE(WM_FORMATROM,OnFormatRom)	
	ON_MESSAGE(WM_WRITEROM,OnWriteRom)		

	ON_MESSAGE(WM_CREATEFLODER,OnCreateFolder)
	ON_MESSAGE(WM_REMOVEFOLDER,OnRemoveFolder)
	ON_MESSAGE(WM_DELETEFILE,OnDeleteFile)
	ON_MESSAGE(WM_READFILE,OnReadFile)
	ON_MESSAGE(WM_PREFOLDER,OnPreFolder)
	ON_MESSAGE(WM_FORMATNAND,OnFormatNand)
	ON_MESSAGE(WM_BAKCUPFOLDER,OnBackupFolder)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNew_EZManageDlg message handlers

BOOL CNew_EZManageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	DropTarget.Register(this);

	
	m_Tab.InsertItem(0,m_gStrNorFlashPage);
	m_Tab.InsertItem(1,m_gStrdiskPage);
	
	m_PageFlash.Create(IDD_DIALOG_FLASH,&m_Tab);
	m_PageDisk.Create(IDD_DIALOG_DISK,&m_Tab);
	
	m_PageFlash.ShowWindow(SW_SHOW);
	m_PageDisk.ShowWindow(SW_HIDE);
	   
	m_Tab.Invalidate(TRUE);
	m_Tab.EnableWindow(FALSE);
    
	char *pstr = new char[1024];
	::GetPrivateProfileString("PathInfo","Path","C:\\",pstr,256,m_strWindowsDir+("\\temp\\conf.ini"));
	m_DirTree.DisplayTree(pstr, TRUE );
	m_DirTree.SetSelPath(pstr);
	//m_DirTree.ModifyStyle(0,TVS_TRACKSELECT);
	CWnd *pWnd = GetDlgItem(IDC_EDIT_PATH);
	if(pWnd)
		pWnd->SetWindowText(pstr);
	m_DirTree.Expand(m_DirTree.GetRootItem(),TVE_EXPAND);

	delete []pstr;

	m_strDriverVersion = GetDriverVersionInfo();
	m_strDriverVersion .Remove('0');
	SetWindowText("EZManager 2.22 (Driver Version"+m_strDriverVersion+_T(")"));
	m_pThreadConnection = AfxBeginThread(TThreadConnection,this);
	InitMenu();
	InitInterface();
	SetTimer(IDENTIFY_CART_TIMER,1000,NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

UINT CNew_EZManageDlg::TThreadConnection( LPVOID pParam )
{
	CNew_EZManageDlg*  pView = (CNew_EZManageDlg*)pParam ;
	return pView->TThreadConnectionProc(pView);
}

UINT CNew_EZManageDlg::TThreadConnectionProc(CNew_EZManageDlg *pThis)
{
		char *p = NULL ;
	char *pTemp;
	CString strObject ;
	CString strEEE ;
	CString strNewLocation;
	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;
	CInternetSession session(NULL,0);
	DWORD dwHttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT;
	CString message ;
	char cChar[256];
	::GetPrivateProfileString("Language","Version","",cChar,256,m_strWindowsDir+("\\temp\\conf.ini"));
	CString strLanguage;
	strLanguage = cChar;
	try
	{
		CString strServerName = _T("www.ezflash.cn");
		
		if(strLanguage ==_T("简体中文"))
		{
			strObject = _T("/ez3c.txt");
		//	strEEE = _T("网络通知会在下次启动时读取");
		}
		//else if(m_strLang ==_T("羉砰いゅ"))
		//{
		//	strObject = _T("/notice-tc.txt");
		//	strEEE = _T("呼蹈硄穦Ω币笆弄");
		//}
		else
		{
			strObject = _T("/ez3e.txt");
			strEEE = _T("The Notice will be Read Next time start");
		}
		int nPort = 80;
		pServer = session.GetHttpConnection(strServerName,0,80,0,0);//INTERNET_DEFAULT_HTTP_PORT,NULL,NULL);
		pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,
			strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
		const TCHAR szHeaders[] =_T("GET www.ezflash.cn/ez3c.txt");
		pFile->AddRequestHeaders(szHeaders);
		BOOL bRet = pFile->SendRequest();
		DWORD dwRet;
		pFile->QueryInfoStatusCode(dwRet);
		pFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH,strNewLocation);//得到文件的长度
		DWORD dwLength =strtoul(strNewLocation.GetBuffer(strNewLocation.GetLength()),&pTemp,10);
		strNewLocation.ReleaseBuffer(strNewLocation.GetLength());

		p = new char[dwLength+2];
		memset(p,0,dwLength+2);
		pFile->Read(p,dwLength);
		message = p ;
		m_ctrlHyperlinkScroller.AddScrollText(message,"www.ezflash.cn");
		m_ctrlHyperlinkScroller.StartScrolling();

//		EnterCriticalSection(&g_msgCritical);
//		m_strMsgArray.Add(message);
//		LeaveCriticalSection(&g_msgCritical);
		//PostMessage(WM_OUTLISTMSG);
		if(p) delete p ; p = NULL ;
	}
	catch (CInternetException* pEx)
	{
		// catch errors from WinINet

		TCHAR szErr[1024];
		pEx->GetErrorMessage(szErr, 1024);
		pEx->Delete();
		message = szErr ;
		message += strEEE ;
//		EnterCriticalSection(&g_msgCritical);
//		m_strMsgArray.Add(message);
//		LeaveCriticalSection(&g_msgCritical);
//		PostMessage(WM_OUTLISTMSG);
	}
	if(pFile)
	{
		pFile->Close();
		delete pFile;
	}
	if(pServer)
	{
		pServer->Close();
		delete pServer;
	}
	session.Close();
	return 0 ;
}

void CNew_EZManageDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNew_EZManageDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNew_EZManageDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CNew_EZManageDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_PageFlash.ShowWindow(SW_HIDE);
	m_PageDisk.ShowWindow(SW_HIDE);
	
	switch(m_Tab.GetCurSel())
	{
	case 0:
		if(m_bTimeNor)
		{
			ReadEZ3NorFlash();
			m_bTimeNor = FALSE;
		}
		m_PageFlash.ShowWindow(SW_SHOW);
		ShowRomUseInfo(FALSE);
		break;
	case 1:
		m_PageDisk.ShowWindow(SW_SHOW);
		if(m_bTimesNand)
		{
			ReadNandFlash();
			m_bTimesNand = FALSE;
		}
		ShowNandUseInfo(NAND_TOTAL_SIZE,g_dwNandFreeSpace);
		break;
	
	}
	*pResult = 0;
}
void CNew_EZManageDlg::ReadNandFlash()
{
	m_Tab.EnableWindow(FALSE);
	m_WriteProgress.SetWindowText(m_gStrMsgReading);
	m_PageDisk.InitDisk();
	m_Tab.EnableWindow(TRUE);
	ShowNandUseInfo(NAND_TOTAL_SIZE,g_dwNandFreeSpace);
	m_WriteProgress.SetWindowText(m_gStrMsgReadEnd);
}
void CNew_EZManageDlg::OnButtonSetpath() 
{
	// TODO: Add your control notification handler code here
	CString sFolderPath;
	BROWSEINFO bi;
	char Buffer[MAX_PATH];
	//初始化入口参数bi开始
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;//此参数如为NULL则不能显示对话框
	bi.lpszTitle = m_gStrMsgSelectFolder;
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	//bi.iImage=IDI_ICON2;
	//初始化入口参数bi结束
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//调用显示选择对话框
	if(pIDList)
	{
		SHGetPathFromIDList(pIDList, Buffer);
		//取得文件夹路径到Buffer里
		sFolderPath = Buffer;//将路径保存在一个CString对象里
	}	
	LPMALLOC lpMalloc;
	if(FAILED(SHGetMalloc(&lpMalloc))) return;
	//释放内存
	lpMalloc->Free(pIDList);
	lpMalloc->Release();
	if(!pIDList)
		return;
	m_DirTree.DisplayTree( sFolderPath, TRUE );
	m_DirTree.SetSelPath(sFolderPath);
	
	CWnd *pWnd = GetDlgItem(IDC_EDIT_PATH);
	if(pWnd)
		pWnd->SetWindowText(sFolderPath);
	//m_DirTree.Expand(NULL,TVE_EXPAND);
	m_DirTree.Expand(m_DirTree.GetRootItem(),TVE_EXPAND);

	::WritePrivateProfileString("PathInfo","Path",sFolderPath,m_strWindowsDir+("\\temp\\conf.ini"));
}

void CNew_EZManageDlg::OnBegindragTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString m_strPath;
	m_strPath = m_DirTree.GetFullPath( pNMTreeView->itemNew.hItem );
	if(m_strPath.Find('.') == -1)
		return ;

	int nTextSize = m_strPath.GetLength();
	HGLOBAL hText = ::GlobalAlloc(GMEM_SHARE,nTextSize);
	LPSTR pText = (LPSTR)::GlobalLock(hText);
	//strcpy(pText,str);
	memcpy(pText,m_strPath.GetBuffer(nTextSize),nTextSize);
	m_strPath.ReleaseBuffer(nTextSize);
	::GlobalUnlock(hText);

	COleDataSource *pSource = new COleDataSource;
	pSource->CacheGlobalData(CF_TEXT,hText);
	DROPEFFECT  ret = pSource->DoDragDrop();
	delete pSource;
	*pResult = 0;
	
}

void CNew_EZManageDlg::AddDropDataToNorList(COleDataObject* pDataObject)
{
	CString strCheck;
	STGMEDIUM stg;
	FORMATETC fmt;
	CString strFilePathName;
	if(!pDataObject->IsDataAvailable(CF_TEXT))
			return ;
	SETFORMATETC(fmt,CF_TEXT,DVASPECT_CONTENT,NULL,TYMED_HGLOBAL,-1);
	VERIFY(pDataObject->GetData(CF_TEXT,&stg,&fmt));
	HGLOBAL hText = stg.hGlobal;
	int memsize = GlobalSize(stg.hGlobal);
	LPSTR pStr = (LPSTR)::GlobalLock(hText);
	strFilePathName = pStr;
	strFilePathName.ReleaseBuffer(memsize);
	::GlobalUnlock(hText);
	GlobalFree(hText);

	strCheck = strFilePathName.Right(4);
	strCheck.MakeUpper();
	if(strCheck!=".GBA" && strCheck!=".ZIP" && strCheck!=".BIN" && strCheck!=".RAR" &&  strCheck!=".NDS")
	{
		MessageBox(m_gStrMsgIncalideFile,m_gStrMsgCation,MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	GameRomInfo *pInfo = GetRomInfo(strFilePathName);
	if(pInfo)
	{
		if(!CheckRom(pInfo->flGameSize) || pInfo->bNdsRom)
		{
			if(pInfo->pGameData)
			{
				delete []pInfo->pGameData;
				pInfo->pGameData = NULL;
			}
			if(pInfo->pGlodCheat)
			{
				DeleteGoldCheatPointer(pInfo->pGlodCheat,pInfo->CheatSize);
			}
			delete pInfo;
			if(pInfo->bNdsRom)
			{
				MessageBox(m_gStrMsgIncalideFile,m_gStrMsgCation,MB_ICONEXCLAMATION | MB_OK);
				return;
			}
			else
			{
				MessageBox(m_gStrMsgNoEnough,m_gStrMsgCation,MB_ICONEXCLAMATION | MB_OK);
				return ;
			}
		}
		pGameInfoList->Add(pInfo);
		AddItemToNorList(pInfo);
		ShowRomUseInfo();
	}
}
void CNew_EZManageDlg::AddItemToNorList(GameRomInfo *pInfo,int index)
{
	CString str;
	if(index == -1)
		index = pGameInfoList->GetSize()-1;
	m_PageFlash.m_ListCtrl.SetTextColor(RGB(255,0,0));
	str.Format("%d",index);
	int counter = m_PageFlash.m_ListCtrl.GetItemCount();
	m_PageFlash.m_ListCtrl.InsertItem(counter,str);
	if(strlen(pInfo->strGameChinaName)>0)
		m_PageFlash.m_ListCtrl.SetItem(counter, 1, LVIF_TEXT, pInfo->strGameChinaName, 0, NULL, NULL, NULL);
	else
		m_PageFlash.m_ListCtrl.SetItem(counter, 1, LVIF_TEXT, pInfo->strGameName, 0, NULL, NULL, NULL);
	str.Format("%d%s",pInfo->flGameSize/1024,"K Bit");
	m_PageFlash.m_ListCtrl.SetItem(counter, 2, LVIF_TEXT, str, 0, NULL, NULL, NULL);
	m_PageFlash.m_ListCtrl.SetItem(counter, 3, LVIF_TEXT, "...", 0, NULL, NULL, NULL);
	if(pInfo->uiWhere ==1)
	{
		m_PageFlash.m_ListCtrl.SetItem(counter, 4, LVIF_TEXT, m_gStrMsgNoWrited, 0, NULL, NULL, NULL);
	}
	else
	{
		m_PageFlash.m_ListCtrl.SetItem(counter, 4, LVIF_TEXT, m_gStrMsgWrited, 0, NULL, NULL, NULL);
	}
}
BOOL CNew_EZManageDlg::AddFileToDiskList(COleDataObject* pDataObject)
{
	CString strPath;
	STGMEDIUM stg;
	FORMATETC fmt;
	SETFORMATETC(fmt,CF_HDROP,DVASPECT_CONTENT,NULL,TYMED_HGLOBAL,-1);
	pDataObject->GetData(CF_HDROP,&stg,&fmt);
	HDROP hDropInfo  = (HDROP)stg.hGlobal;
	m_strArray.RemoveAll();
	int testcounter=0;
	//取得拖放的文件总数
	unsigned int nFiles=DragQueryFile(hDropInfo,0xFFFFFFFF,NULL,0);
	for(int i=0;i<nFiles;i++)
	{
		unsigned int nLen=DragQueryFile(hDropInfo,i,NULL,0)+1;//取得文件名长度
		char *psBuffer=new char[nLen];
		unsigned int sLen=DragQueryFile(hDropInfo,i,psBuffer,nLen);//取得文件名到psBuffer中,sLen为实际拷贝的字符数	
		m_strFilePathName = psBuffer;
		delete [] psBuffer;
		if(GetFileAttributes(m_strFilePathName) == FILE_ATTRIBUTE_DIRECTORY)
			continue;
		int ret;
		strPath = m_PageDisk.m_strNandDiskPath;
		CString strCopyFileName,strTarget,str;
		GameRomInfo *pInfo;
		CRomManager rm;
		CFile file;
		bChangeName = FALSE;
		PGOLDCHEAT pGoldCheat=NULL;
		WORD w_CheatSize=0;
			
		GOLD_DATA_ARRAY *pGoldArray=NULL;
		pInfo = GetRomInfo(m_strFilePathName);
		if(pInfo!=NULL && pInfo->bHomebrew)
		{
			//删除pInfo
			if(pInfo->pGameData)
			{
				delete []pInfo->pGameData;
				pInfo->pGameData = NULL;
			}
			if(pInfo->pXCode)
			{
				if(pInfo->uiWhere ==0)
				{
					if(((XCODE *)pInfo->pXCode)->pCode)
						delete []((XCODE *)pInfo->pXCode)->pCode;
					delete pInfo->pXCode;
				}
				else
				{
					delete []pInfo->pXCode;
					pInfo->pXCode=NULL;
				}
			}
			if(pInfo->pGlodCheat)
				DeleteGoldCheatPointer(pInfo->pGlodCheat,pInfo->CheatSize);
			if(pInfo)
			{
				delete pInfo;
				pInfo = NULL;
			}
			MessageBox("Can not add Nds HomeBrew",m_gStrMsgCation,MB_ICONEXCLAMATION | MB_OK);
			continue;
		}
		//预留编辑的信息
		if(pInfo != NULL && pInfo->bNdsRom==FALSE)
		{
			CRomEditDlg dlg;
			dlg.SetGameInfo(pInfo);
			dlg.SetZipState(TRUE);
			if(pInfo->pXCode)
				dlg.SetXCodeState(TRUE);
			else
				dlg.SetXCodeState(FALSE);
			ret = dlg.DoModal();
			if(ret == IDOK)
			{  
				pInfo->pGameData[0xb5] = 0 ;
				pInfo->pGameData[0xb6] = 0 ;
				pInfo->pGameData[0xb7] = 0 ;
				
				if(pInfo->bReset )
				{
					FillGoldData(pInfo,rm);
				}
				strCopyFileName = pInfo->strGameName;
				int len = strlen(pInfo->strGameName);
				if(len>12)
					len =12;
				strCopyFileName.ReleaseBuffer(len);
				strCopyFileName.Remove('-');
				strCopyFileName.Remove('.');
				strCopyFileName.Remove('_');
				strCopyFileName.Remove('+');
				strCopyFileName.Remove(' ');
				if(strCopyFileName.GetLength()>8)
					strCopyFileName = strCopyFileName.Left(8);
				/*****************************************************************************/
				//添加即时存档
				//判断是否 有相同的文件,如果有,修改文件名
				int cou=0;
				CString strTemp= strCopyFileName;
				do
				{
					if(cou!=0)
						strTemp =GetNewFileName(strTemp,cou);
					cou++;
				}while(HasSameNameInArray(strTemp,&m_strArray) || HasSameNameInList(strTemp+".gba"));

				strCopyFileName = strTemp;
				if(pInfo->bSaveNow && pInfo->pXCode)
				{
					KillTimer(IDENTIFY_CART_TIMER);
					fs.CartInit();
					if(!fs.Initialize())
					{
						fs.ClearCartInit();
						//格式化磁盘
						if(IDYES == MessageBox(m_gStrMsgFormatcation,NULL,MB_YESNO))
						{
						}
						return FALSE;
					}
					CString str;
					str =strCopyFileName;
					str = m_PageDisk.m_strNandDiskPath+str;
					str.Left(str.GetLength()-4);
					str+=".inv";
					int NandAdd = fs.fat_Instancesave_creat(str.GetBuffer(str.GetLength()),ATTR_ARCHIVE);
					if(NandAdd <= 0)
					{
						return FALSE;
					}
					str.ReleaseBuffer(str.GetLength());
					fs.Uninitialize();
					fs.ClearCartInit();
					FillGoldDataAndSaveNow(pInfo,rm,NandAdd);
					m_bWriteIns = TRUE;
					SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
				}
							
				
				m_strFilePathName = m_strWindowsDir+_T("\\temp\\")+strCopyFileName+_T(".gba");		
				FillNandGameInfo(pInfo);	
				/*****************************************************************************/
				file.Open(m_strFilePathName,CFile::modeReadWrite|CFile::modeCreate);
				file.Write(pInfo->pGameData,pInfo->flGameSize);
				file.Close();
				//删除pInfo
				if(pInfo->pGameData)
				{
					delete []pInfo->pGameData;
					pInfo->pGameData = NULL;
				}
				if(pInfo->pXCode)
				{
					if(pInfo->uiWhere ==0)
					{
						if(((XCODE *)pInfo->pXCode)->pCode)
							delete []((XCODE *)pInfo->pXCode)->pCode;
						delete pInfo->pXCode;
					}
					else
					{
						delete []pInfo->pXCode;
						pInfo->pXCode=NULL;
					}
				}
				if(pInfo->pGlodCheat)
					DeleteGoldCheatPointer(pInfo->pGlodCheat,pInfo->CheatSize);
				if(pInfo)
				{
					delete pInfo;
					pInfo = NULL;
				}
				if(dlg.GetZipState())
				{
					strTarget = m_strFilePathName;
					strTarget  = strTarget.Left(strTarget.GetLength()-4);
					strTarget +=".lz7";
					DWORD Time1 = GetTickCount();
					Compress_LZ77_DosCommand(m_strFilePathName,strTarget);
					DWORD Time2 = GetTickCount();
					DWORD Time3 = (Time2-Time1);///1000;
					CString str ;
					str.Format("%s%d%s","打开文件花了",Time3,"毫秒");
					DeleteFile(m_strFilePathName);
					m_strFilePathName=strTarget;
				}
				bChangeName = TRUE;
			}
			else//取消写
			{
				//删除pInfo
				if(pInfo->pGameData)
				{
					delete []pInfo->pGameData;
					pInfo->pGameData = NULL;
				}
				if(pInfo->pXCode)
				{
					if(pInfo->uiWhere ==0)
					{
						if(((XCODE *)pInfo->pXCode)->pCode)
							delete []((XCODE *)pInfo->pXCode)->pCode;
						delete pInfo->pXCode;
					}
					else
					{
						delete []pInfo->pXCode;
						pInfo->pXCode=NULL;
					}
				}
				if(pInfo->pGlodCheat)
					DeleteGoldCheatPointer(pInfo->pGlodCheat,pInfo->CheatSize);
				if(pInfo)
				{
					delete pInfo;
					pInfo = NULL;
				}
				return FALSE;
			}
		}
		else if(pInfo != NULL && pInfo->bNdsRom==TRUE)
		{
			strCopyFileName = pInfo->strGameName;
			int len = strlen(pInfo->strGameName);
			if(len>12)
				len =12;
			strCopyFileName.ReleaseBuffer(len);
			strCopyFileName.Remove('-');
			strCopyFileName.Remove('.');
			strCopyFileName.Remove('_');
			strCopyFileName.Remove('+');
			strCopyFileName.Remove(' ');
			if(strCopyFileName.GetLength()>8)
				strCopyFileName = strCopyFileName.Left(8);
			int cou=0;
			CString strTemp= strCopyFileName+_T(".nds");
			do
			{
				if(cou!=0)
					strTemp =GetNewFileName(strTemp,cou);
				cou++;
			}while(HasSameNameInList(strTemp));
			strCopyFileName = strTemp.Left(strTemp.GetLength()-4);
			m_strFilePathName = m_strWindowsDir+_T("\\temp\\")+strCopyFileName+_T(".nds");		
			FillNandGameInfo(pInfo);	
			/*****************************************************************************/
			file.Open(m_strFilePathName,CFile::modeReadWrite|CFile::modeCreate);
			file.Write(pInfo->pGameData,pInfo->flGameSize);
			file.Close();
			//删除pInfo
			if(pInfo->pGameData)
			{
				delete []pInfo->pGameData;
				pInfo->pGameData = NULL;
			}
			if(pInfo)
			{
				delete pInfo;
				pInfo = NULL;
			}
			bChangeName = TRUE;
			}
		else
		{
			if(m_strFilePathName.Right(4) == _T(".txt") || m_strFilePathName.Right(4) == _T(".sav"))
			{
			}
			else if(m_strFilePathName.Right(4) == _T(".htm"))
			{
				CString s = m_strWorkPath;   
				CString strTargetName = m_strFilePathName.Right( m_strFilePathName.GetLength()-m_strFilePathName.ReverseFind('\\')-1);
				strTargetName = m_strWindowsDir +"\\temp\\"+strTargetName;
				CopyFile(m_strFilePathName,strTargetName,FALSE);
				Html2Txt(s.GetBuffer(s.GetLength()),strTargetName.GetBuffer(strTargetName.GetLength()));
				DeleteFile(strTargetName);
				s.ReleaseBuffer(s.GetLength());
				strTargetName.ReleaseBuffer(strTargetName.GetLength());
				str.ReleaseBuffer(str.GetLength());
				m_strFilePathName = strTargetName;
				m_strFilePathName.Replace(_T(".htm"),_T(".txt"));  
				bChangeName = TRUE;
			}
			else if(m_strFilePathName.Right(4) == _T(".doc"))
			{
				CString s;// = GetDocument()->GetSystemPath();   
				CString strTargetName = m_strFilePathName.Right( m_strFilePathName.GetLength()-m_strFilePathName.ReverseFind('\\')-1);
				strTargetName = m_strWindowsDir +"\\temp\\"+strTargetName;
				CopyFile(m_strFilePathName,strTargetName,FALSE);
				s = m_strWindowsDir +"\\temp\\";
				Doc2Txt(s.GetBuffer(s.GetLength()),strTargetName.GetBuffer(strTargetName.GetLength()));
				DeleteFile(strTargetName);
				s.ReleaseBuffer(s.GetLength());
				strTargetName.ReleaseBuffer(strTargetName.GetLength());
				str.ReleaseBuffer(str.GetLength());
				m_strFilePathName = strTargetName;
				m_strFilePathName.Replace(_T(".doc"),_T(".txt"));  
				bChangeName = TRUE;
			}
			else if(m_strFilePathName.Right(5) == _T(".html"))
			{
				CString s = m_strWorkPath;   
				CString strTargetName = m_strFilePathName.Right( m_strFilePathName.GetLength()-m_strFilePathName.ReverseFind('\\')-1);
				strTargetName = m_strWindowsDir +"\\temp\\"+strTargetName;
				CopyFile(m_strFilePathName,strTargetName,FALSE);
				Html2Txt(s.GetBuffer(s.GetLength()),strTargetName.GetBuffer(strTargetName.GetLength()));
				DeleteFile(strTargetName);
				s.ReleaseBuffer(s.GetLength());
				strTargetName.ReleaseBuffer(strTargetName.GetLength());
				str.ReleaseBuffer(str.GetLength());
				m_strFilePathName = strTargetName;
				m_strFilePathName.Replace(_T(".html"),_T(".txt"));  
				bChangeName = TRUE;
			}
			else if(m_strFilePathName.Right(4) == _T(".bmp") || m_strFilePathName.Right(4) == _T(".BMP")|| m_strFilePathName.Right(4) == _T(".jpg") ||  m_strFilePathName.Right(4) == _T(".JPG"))
			{
				//修改bmp文件
				if(m_strFilePathName.Right(4) == _T(".bmp") || m_strFilePathName.Right(4) == _T(".BMP"))
				{
					str = ModifyBmpFile(m_strFilePathName);
					m_strFilePathName = str;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
				}
				else
				{
					str = m_strFilePathName.Left(m_strFilePathName.GetLength()-4) + _T(".bmp");
					CString strSysPath = m_strWorkPath + _T("\\sysbin\\pictrans.exe ");
					CString strCmd;
					strCmd = strSysPath;
					strCmd +=  m_strFilePathName + _T(" /convert=") +str;
					ExecCommand(strCmd.GetBuffer(strCmd.GetLength()));
					strCmd.ReleaseBuffer(strCmd.GetLength());
					m_strFilePathName = ModifyBmpFile(str);
					DeleteFile(str);
					bChangeName = TRUE;
				}	
			}
			else
			{
			}
		}
		testcounter++;
		m_strArray.Add(m_strFilePathName);
	}
	/********************************lichuan debug begin*********************/
	/*
	CString strTest1,strTest2;
	for(int loop1=0;loop1<m_strArray.GetSize();loop1++)
	{
		strTest1 = m_strArray.GetAt(loop1);
		for(int loop2=0;loop2<m_strArray.GetSize();loop2++)
		{
			if(loop1==loop2)
				continue;
			strTest2 = m_strArray.GetAt(loop2);
			if(strTest1 == strTest2)
				AfxMessageBox("Error");
		}
	}
	*/
	/********************************lichuan debug end***********************/
	DragFinish(hDropInfo);
	pThreadWriteMoreNand = AfxBeginThread(TWThreadWriteMoreNand,
								this);
	if(pThreadWriteMoreNand == NULL)
		AfxMessageBox("execute write thread failed");
	return TRUE;
}
BOOL CNew_EZManageDlg::AddDropDataToDiskList(COleDataObject* pDataObject)
{
	CString strPath = m_PageDisk.m_strNandDiskPath;
	if(strPath.GetLength()<=0)
	{
		return FALSE;
	}
	STGMEDIUM stg;
	FORMATETC fmt;
	if(!pDataObject->IsDataAvailable(CF_TEXT))
			return FALSE;
	
	SETFORMATETC(fmt,CF_TEXT,DVASPECT_CONTENT,NULL,TYMED_HGLOBAL,-1);
	VERIFY(pDataObject->GetData(CF_TEXT,&stg,&fmt));
	HGLOBAL hText = stg.hGlobal;
	int memsize = GlobalSize(stg.hGlobal);
	LPSTR pStr = (LPSTR)::GlobalLock(hText);
	m_strFilePathName = pStr;
	m_strFilePathName.ReleaseBuffer(memsize);
	::GlobalUnlock(hText);
	GlobalFree(hText);
	if(GetFileAttributes(m_strFilePathName) == FILE_ATTRIBUTE_DIRECTORY)
	{
		return FALSE;
	}
	int ret;
	strPath = m_PageDisk.m_strNandDiskPath;
	CString strCopyFileName,strTarget,str;
	GameRomInfo *pInfo;
	CRomManager rm;
	CFile file;
	bChangeName = FALSE;
	PGOLDCHEAT pGoldCheat=NULL;
	WORD w_CheatSize=0;
		
	GOLD_DATA_ARRAY *pGoldArray=NULL;
	pInfo = GetRomInfo(m_strFilePathName);
	if(pInfo!=NULL && pInfo->bHomebrew)
	{
		//删除pInfo
		if(pInfo->pGameData)
		{
			delete []pInfo->pGameData;
			pInfo->pGameData = NULL;
		}
		if(pInfo->pXCode)
		{
			if(pInfo->uiWhere ==0)
			{
				if(((XCODE *)pInfo->pXCode)->pCode)
					delete []((XCODE *)pInfo->pXCode)->pCode;
				delete pInfo->pXCode;
			}
			else
			{
				delete []pInfo->pXCode;
				pInfo->pXCode=NULL;
			}
		}
		if(pInfo->pGlodCheat)
			DeleteGoldCheatPointer(pInfo->pGlodCheat,pInfo->CheatSize);
		if(pInfo)
		{
			delete pInfo;
			pInfo = NULL;
		}
		MessageBox("Can not add Nds HomeBrew",m_gStrMsgCation,MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}
	//预留编辑的信息
	if(pInfo != NULL && pInfo->bNdsRom==FALSE)
	{
		CRomEditDlg dlg;
		dlg.SetGameInfo(pInfo);
		dlg.SetZipState(TRUE);
		if(pInfo->pXCode)
			dlg.SetXCodeState(TRUE);
		else
			dlg.SetXCodeState(FALSE);
		ret = dlg.DoModal();
		if(ret == IDOK)
		{  
			pInfo->pGameData[0xb5] = 0 ;
			pInfo->pGameData[0xb6] = 0 ;
			pInfo->pGameData[0xb7] = 0 ;
			
			if(pInfo->bReset )
			{
				FillGoldData(pInfo,rm);
			}
			strCopyFileName = pInfo->strGameName;
			int len = strlen(pInfo->strGameName);
			if(len>12)
				len =12;
			strCopyFileName.ReleaseBuffer(len);
			strCopyFileName.Remove('-');
			strCopyFileName.Remove('.');
			strCopyFileName.Remove('_');
			strCopyFileName.Remove('+');
			strCopyFileName.Remove(' ');
			if(strCopyFileName.GetLength()>8)
				strCopyFileName = strCopyFileName.Left(8);
			/*****************************************************************************/
			//添加即时存档
			//判断是否 有相同的文件,如果有,修改文件名
			int cou=0;
			CString strTemp= strCopyFileName+_T(".gba");
			do
			{
				if(cou!=0)
					strTemp =GetNewFileName(strTemp,cou);
				cou++;
			}while(HasSameNameInList(strTemp));
			strCopyFileName = strTemp.Left(strTemp.GetLength()-4);
			if(pInfo->bSaveNow && pInfo->pXCode)
			{
				KillTimer(IDENTIFY_CART_TIMER);
				fs.CartInit();
				if(!fs.Initialize())
				{
					fs.ClearCartInit();
					//格式化磁盘
					if(IDYES == MessageBox(m_gStrMsgFormatcation,NULL,MB_YESNO))
					{
					}
					return FALSE;
				}
				CString str;
				str =strCopyFileName;
				str = m_PageDisk.m_strNandDiskPath+str;
				str.Left(str.GetLength()-4);
				str+=".inv";
				int NandAdd = fs.fat_Instancesave_creat(str.GetBuffer(str.GetLength()),ATTR_ARCHIVE);
				if(NandAdd <= 0)
				{
					return FALSE;
				}
				str.ReleaseBuffer(str.GetLength());
				fs.Uninitialize();
				fs.ClearCartInit();
				FillGoldDataAndSaveNow(pInfo,rm,NandAdd);
				m_bWriteIns = TRUE;
				SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
			}
						
			
			m_strFilePathName = m_strWindowsDir+_T("\\temp\\")+strCopyFileName+_T(".gba");		
			FillNandGameInfo(pInfo);	
			/*****************************************************************************/
			file.Open(m_strFilePathName,CFile::modeReadWrite|CFile::modeCreate);
			file.Write(pInfo->pGameData,pInfo->flGameSize);
			file.Close();
			//删除pInfo
			if(pInfo->pGameData)
			{
				delete []pInfo->pGameData;
				pInfo->pGameData = NULL;
			}
			if(pInfo->pXCode)
			{
				if(pInfo->uiWhere ==0)
				{
					if(((XCODE *)pInfo->pXCode)->pCode)
						delete []((XCODE *)pInfo->pXCode)->pCode;
					delete pInfo->pXCode;
				}
				else
				{
					delete []pInfo->pXCode;
					pInfo->pXCode=NULL;
				}
			}
			if(pInfo->pGlodCheat)
				DeleteGoldCheatPointer(pInfo->pGlodCheat,pInfo->CheatSize);
			if(pInfo)
			{
				delete pInfo;
				pInfo = NULL;
			}
			if(dlg.GetZipState())
			{
				strTarget = m_strFilePathName;
				strTarget  = strTarget.Left(strTarget.GetLength()-4);
				strTarget +=".lz7";
				DWORD Time1 = GetTickCount();
				Compress_LZ77_DosCommand(m_strFilePathName,strTarget);
				DWORD Time2 = GetTickCount();
				DWORD Time3 = (Time2-Time1);///1000;
				CString str ;
				str.Format("%s%d%s","打开文件花了",Time3,"毫秒");
				DeleteFile(m_strFilePathName);
				m_strFilePathName=strTarget;
			}
			bChangeName = TRUE;
		}
		else//取消写
		{
			//删除pInfo
			if(pInfo->pGameData)
			{
				delete []pInfo->pGameData;
				pInfo->pGameData = NULL;
			}
			if(pInfo->pXCode)
			{
				if(pInfo->uiWhere ==0)
				{
					if(((XCODE *)pInfo->pXCode)->pCode)
						delete []((XCODE *)pInfo->pXCode)->pCode;
					delete pInfo->pXCode;
				}
				else
				{
					delete []pInfo->pXCode;
					pInfo->pXCode=NULL;
				}
			}
			if(pInfo->pGlodCheat)
				DeleteGoldCheatPointer(pInfo->pGlodCheat,pInfo->CheatSize);
			if(pInfo)
			{
				delete pInfo;
				pInfo = NULL;
			}
			return FALSE;
		}
	}
	else if(pInfo != NULL && pInfo->bNdsRom==TRUE)
	{
			strCopyFileName = pInfo->strGameName;
			int len = strlen(pInfo->strGameName);
			if(len>12)
				len =12;
			strCopyFileName.ReleaseBuffer(len);
			strCopyFileName.Remove('-');
			strCopyFileName.Remove('.');
			strCopyFileName.Remove('_');
			strCopyFileName.Remove('+');
			strCopyFileName.Remove(' ');
			if(strCopyFileName.GetLength()>8)
				strCopyFileName = strCopyFileName.Left(8);
			int cou=0;
			CString strTemp= strCopyFileName+_T(".nds");
			do
			{
				if(cou!=0)
					strTemp =GetNewFileName(strTemp,cou);
				cou++;
			}while(HasSameNameInList(strTemp));
			strCopyFileName = strTemp.Left(strTemp.GetLength()-4);

			m_strFilePathName = m_strWindowsDir+_T("\\temp\\")+strCopyFileName+_T(".nds");		
			FillNandGameInfo(pInfo);	
			/*****************************************************************************/
			file.Open(m_strFilePathName,CFile::modeReadWrite|CFile::modeCreate);
			file.Write(pInfo->pGameData,pInfo->flGameSize);
			file.Close();
			//删除pInfo
			if(pInfo->pGameData)
			{
				delete []pInfo->pGameData;
				pInfo->pGameData = NULL;
			}
			if(pInfo)
			{
				delete pInfo;
				pInfo = NULL;
			}
			bChangeName = TRUE;
	}
	else
	{
		if(m_strFilePathName.Right(4) == _T(".txt") || m_strFilePathName.Right(4) == _T(".sav"))
		{
		}
		else if(m_strFilePathName.Right(4) == _T(".htm"))
		{
			CString s = m_strWorkPath;   
			CString strTargetName = m_strFilePathName.Right( m_strFilePathName.GetLength()-m_strFilePathName.ReverseFind('\\')-1);
			strTargetName = m_strWindowsDir +"\\temp\\"+strTargetName;
			CopyFile(m_strFilePathName,strTargetName,FALSE);
			Html2Txt(s.GetBuffer(s.GetLength()),strTargetName.GetBuffer(strTargetName.GetLength()));
			DeleteFile(strTargetName);
			s.ReleaseBuffer(s.GetLength());
			strTargetName.ReleaseBuffer(strTargetName.GetLength());
			str.ReleaseBuffer(str.GetLength());
			m_strFilePathName = strTargetName;
			m_strFilePathName.Replace(_T(".htm"),_T(".txt"));  
			bChangeName = TRUE;
		}
		else if(m_strFilePathName.Right(4) == _T(".doc"))
		{
			CString s;// = GetDocument()->GetSystemPath();   
			CString strTargetName = m_strFilePathName.Right( m_strFilePathName.GetLength()-m_strFilePathName.ReverseFind('\\')-1);
			strTargetName = m_strWindowsDir +"\\temp\\"+strTargetName;
			CopyFile(m_strFilePathName,strTargetName,FALSE);
			s = m_strWindowsDir +"\\temp\\";
			Doc2Txt(s.GetBuffer(s.GetLength()),strTargetName.GetBuffer(strTargetName.GetLength()));
			DeleteFile(strTargetName);
			s.ReleaseBuffer(s.GetLength());
			strTargetName.ReleaseBuffer(strTargetName.GetLength());
			str.ReleaseBuffer(str.GetLength());
			m_strFilePathName = strTargetName;
			m_strFilePathName.Replace(_T(".doc"),_T(".txt"));  
			bChangeName = TRUE;
		}
		else if(m_strFilePathName.Right(5) == _T(".html"))
		{
			CString s = m_strWorkPath;   
			CString strTargetName = m_strFilePathName.Right( m_strFilePathName.GetLength()-m_strFilePathName.ReverseFind('\\')-1);
			strTargetName = m_strWindowsDir +"\\temp\\"+strTargetName;
			CopyFile(m_strFilePathName,strTargetName,FALSE);
			Html2Txt(s.GetBuffer(s.GetLength()),strTargetName.GetBuffer(strTargetName.GetLength()));
			DeleteFile(strTargetName);
			s.ReleaseBuffer(s.GetLength());
			strTargetName.ReleaseBuffer(strTargetName.GetLength());
			str.ReleaseBuffer(str.GetLength());
			m_strFilePathName = strTargetName;
			m_strFilePathName.Replace(_T(".html"),_T(".txt"));  
			bChangeName = TRUE;
		}
		else if(m_strFilePathName.Right(4) == _T(".bmp") || m_strFilePathName.Right(4) == _T(".BMP")|| m_strFilePathName.Right(4) == _T(".jpg") ||  m_strFilePathName.Right(4) == _T(".JPG"))
		{
			//修改bmp文件
			if(m_strFilePathName.Right(4) == _T(".bmp") || m_strFilePathName.Right(4) == _T(".BMP"))
			{
				str = ModifyBmpFile(m_strFilePathName);
				m_strFilePathName = str;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
			}
			else
			{
				str = m_strFilePathName.Left(m_strFilePathName.GetLength()-4) + _T(".bmp");
				CString strSysPath = m_strWorkPath + _T("\\sysbin\\pictrans.exe ");
				CString strCmd;
				strCmd = strSysPath;
				strCmd +=  m_strFilePathName + _T(" /convert=") +str;
				ExecCommand(strCmd.GetBuffer(strCmd.GetLength()));
				strCmd.ReleaseBuffer(strCmd.GetLength());
				m_strFilePathName = ModifyBmpFile(str);
				DeleteFile(str);
				bChangeName = TRUE;
			}	
		}
		else
		{
		}
	}
	//开始调用写Nand Flash线程
	pThreadNandWrite = AfxBeginThread(TRThreadNandWrite,
								this);
	return TRUE;
}

UINT CNew_EZManageDlg::TWThreadWriteMoreNand( LPVOID pParam )
{
	CNew_EZManageDlg  *pView = (CNew_EZManageDlg*)pParam ;
	return pView->TWThreadWriteMoreNand();
}

UINT CNew_EZManageDlg::TWThreadWriteMoreNand()
{
	if(m_strArray.GetSize()<=0)
		return -1;
	/***************************************************************************************************/
	CString str;
	DWORD f_hSize,len,lenstr;
	CString strFileName,strErr;

	KillTimer(IDENTIFY_CART_TIMER);
	m_Tab.EnableWindow(FALSE);
	
	for(int i=0;i<m_strArray.GetSize();i++)
	{
		m_strFilePathName = m_strArray.GetAt(i);
		f_hSize=0;
		len = m_strFilePathName.ReverseFind('\\');
		lenstr = m_strFilePathName.GetLength();
		strFileName = m_strFilePathName.Right(lenstr-len-1);
		BYTE *pFileData=NULL;
		DWORD filelen=0;
		CString strNandFilePathName = m_PageDisk.m_strNandDiskPath + strFileName;	
		int itemcounter = m_PageDisk.m_DiskListCtrl.GetItemCount();

		if(m_strFilePathName.Find('.')>0)
		{
			HANDLE handle = CreateFile(m_strFilePathName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
			BY_HANDLE_FILE_INFORMATION f_info;
			SYSTEMTIME f_time;
			GetFileInformationByHandle(handle,&f_info);//读文件信息
			if(!CheckNandDisk(f_info.nFileSizeLow))
			{
				CloseHandle(handle);
				SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
				m_Tab.EnableWindow(TRUE);
				ShowNandUseInfo(NAND_TOTAL_SIZE,g_dwNandFreeSpace);
				return 0;
			}
			ShowNandUseInfo(NAND_TOTAL_SIZE,g_dwNandFreeSpace);
			pFileData = new BYTE[f_info.nFileSizeLow];
			ReadFile(handle,pFileData,f_info.nFileSizeLow,&filelen,NULL);
			CloseHandle(handle);
			if(m_strFilePathName.Find(_T(".lz7"),0)>0)
				DeleteFile(m_strFilePathName);
			DWORD range = ((filelen*8)/(1024*1024))+1;
			m_WriteProgress.SetRange(0,range+2);
			m_WriteProgress.SetPos(0);
			FileTimeToSystemTime(&f_info.ftLastWriteTime,&f_time);
			TIME_ZONE_INFORMATION time_zone_information;
			GetTimeZoneInformation(&time_zone_information);
			SystemTimeToTzSpecificLocalTime(&time_zone_information,&f_time,&f_time);
			strFileName.MakeLower();
			if(strFileName.GetLength()>12)
			{
				str = strFileName.Right(4);
				strFileName = strFileName.Left(strFileName.GetLength()-4);
				strFileName.Remove('.');
				strFileName.Remove(' ');
				if(strFileName>8)
					strFileName = strFileName.Left(8);
				strFileName += str;
			}		 
			if(strFileName.Right(4) == _T(".gba") || strFileName.Right(4) == _T(".lz7"))
			{
				m_PageDisk.m_DiskListCtrl.InsertItem(itemcounter,strFileName,1);
				CString strTemp = strFileName.Left(strFileName.GetLength()-4);
				if(m_bWriteIns)
				{
					strTemp+=".inv";
					m_PageDisk.m_DiskListCtrl.InsertItem(itemcounter+1,strTemp,2);
					m_PageDisk.m_DiskListCtrl.SetItem(itemcounter+1, 1, LVIF_TEXT, "512 KBytes", 0, NULL, NULL, NULL);
					CString strTime;
					strTime.Format("%d-%d-%d %d:%d",f_time.wYear,f_time.wMonth,f_time.wDay,f_time.wHour,f_time.wMinute);
					m_PageDisk.m_DiskListCtrl.SetItem(itemcounter+1, 3, LVIF_TEXT, strTime, 0, NULL, NULL, NULL);
					m_bWriteIns = FALSE;
				}
			}
			else /*if(strFileName.Right(4) == _T(".txt") || strFileName.Right(4)==_T(".bmp") )*/
			{
				m_PageDisk.m_DiskListCtrl.InsertItem(itemcounter,strFileName,2);
			}
			m_PageDisk.m_DiskListCtrl.SetItem(itemcounter, 2, LVIF_TEXT, "N", 0, NULL, NULL, NULL);
			CString strSize;
			strSize.Format("%d%s",(f_info.nFileSizeLow+1023)/1024," KBytes");
			m_PageDisk.m_DiskListCtrl.SetItem(itemcounter, 1, LVIF_TEXT, strSize, 0, NULL, NULL, NULL);
			CString strTime;
			strTime.Format("%d-%d-%d %d:%d",f_time.wYear,f_time.wMonth,f_time.wDay,f_time.wHour,f_time.wMinute);
			m_PageDisk.m_DiskListCtrl.SetItem(itemcounter, 3, LVIF_TEXT, strTime, 0, NULL, NULL, NULL);
			strNandFilePathName = m_PageDisk.m_strNandDiskPath + strFileName;

			/********************************************************/
			fs.CartInit();
			if(!fs.Initialize())
			{
				fs.ClearCartInit();
				if(IDYES == MessageBox(m_gStrMsgFormatcation,NULL,MB_YESNO))
				{
				}
				return -1;
			}
			PostMessage(WM_NAND_WRITE_BEGIN_MESSAGE,0,0);
			fs.SetOPType(CREATEFILE);
			fs.SetMessageHandle(this->m_hWnd);
			int h = fs.fat_creat(strNandFilePathName.GetBuffer(strNandFilePathName.GetLength()),ATTR_ARCHIVE);
			if(h <0)
			{
				h = fs.fat_open(strNandFilePathName.GetBuffer(strNandFilePathName.GetLength()));
				if(h<0)
				{
					strErr.Format("%s:%s","不能在EZ-Disk创建文件: ",strNandFilePathName);
				}
			}
			strNandFilePathName.ReleaseBuffer(strNandFilePathName.GetLength());
			fs.fat_write(h,(char*)pFileData,f_info.nFileSizeLow);
			fs.fat_close(h);
			fs.Uninitialize();
			if(pFileData)
			{
				delete []pFileData;
				pFileData = NULL;
			}
			fs.ClearCartInit();
			/**********************************************/
		}
		fs.SetOPType(OPNONE);
		PostMessage(WM_NAND_WRITE_END_MESSAGE,0,0);
		DeleteFile(m_strFilePathName);
		ShowNandUseInfo(NAND_TOTAL_SIZE,g_dwNandFreeSpace);
		
	}
	m_strArray.RemoveAll();
	SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
	m_Tab.EnableWindow(TRUE);
	return 1;
	/***************************************************************************************************/
	return 0;
}
UINT CNew_EZManageDlg::TRThreadNandWrite( LPVOID pParam )
{
	CNew_EZManageDlg  *pView = (CNew_EZManageDlg*)pParam ;
	return pView->TRThreadProcessNand(pView);
}
UINT CNew_EZManageDlg::TRThreadProcessNand(CNew_EZManageDlg *pThis)
{
	CString str;
	KillTimer(IDENTIFY_CART_TIMER);
	m_Tab.EnableWindow(FALSE);
	CString strFileName;
	DWORD f_hSize=0;
	DWORD len = m_strFilePathName.ReverseFind('\\');
	DWORD lenstr = m_strFilePathName.GetLength();
	strFileName = m_strFilePathName.Right(lenstr-len-1);
	BYTE *pFileData=NULL;
	DWORD filelen=0;
	CString strNandFilePathName = m_PageDisk.m_strNandDiskPath + strFileName;	
		int itemcounter = m_PageDisk.m_DiskListCtrl.GetItemCount();

	if(m_strFilePathName.Find('.')>0)
	{
		HANDLE handle = CreateFile(m_strFilePathName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
		BY_HANDLE_FILE_INFORMATION f_info;
		SYSTEMTIME f_time;
		GetFileInformationByHandle(handle,&f_info);//读文件信息

		if(!CheckNandDisk(f_info.nFileSizeLow))
		{
			CloseHandle(handle);
			SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
			m_Tab.EnableWindow(TRUE);
			ShowNandUseInfo(NAND_TOTAL_SIZE,g_dwNandFreeSpace);
			return -1;
		}
		ShowNandUseInfo(NAND_TOTAL_SIZE,g_dwNandFreeSpace);
		pFileData = new BYTE[f_info.nFileSizeLow];
		ReadFile(handle,pFileData,f_info.nFileSizeLow,&filelen,NULL);
		CloseHandle(handle);
		if(m_strFilePathName.Find(_T(".lz7"),0)>0)
			DeleteFile(m_strFilePathName);
//////+++++++++++++++++++++++++++++++++++++++++++++++++++++++
//yafei: 加入的nds的测试码
		/*
		CString strnds = m_strFilePathName.Right(4);
		strnds.MakeUpper();
		if(strnds == _T(".NDS"))
		{
			CRomManager tt;
			tt.NDSPatch1(pFileData,filelen,NULL);
		}
		*/
//////+++++++++++++++++++++++++++++++++++++++++++++++++++++++
		DWORD range = ((filelen*8)/(1024*1024))+1;
		m_WriteProgress.SetRange(0,range+2);
		m_WriteProgress.SetPos(0);
		FileTimeToSystemTime(&f_info.ftLastWriteTime,&f_time);
		TIME_ZONE_INFORMATION time_zone_information;
		GetTimeZoneInformation(&time_zone_information);
		SystemTimeToTzSpecificLocalTime(&time_zone_information,&f_time,&f_time);
		strFileName.MakeLower();
		if(strFileName.GetLength()>12)
		{
			str = strFileName.Right(4);
			strFileName = strFileName.Left(strFileName.GetLength()-4);
			strFileName.Remove('.');
			strFileName.Remove(' ');
			if(strFileName>8)
				strFileName = strFileName.Left(8);
			strFileName += str;
		}		 
		if(strFileName.Right(4) == _T(".gba") || strFileName.Right(4) == _T(".lz7"))
		{
			m_PageDisk.m_DiskListCtrl.InsertItem(itemcounter,strFileName,1);
			CString strTemp = strFileName.Left(strFileName.GetLength()-4);
			if(m_bWriteIns)
			{
				strTemp+=".inv";
				m_PageDisk.m_DiskListCtrl.InsertItem(itemcounter+1,strTemp,2);
				m_PageDisk.m_DiskListCtrl.SetItem(itemcounter+1, 1, LVIF_TEXT, "512 KBytes", 0, NULL, NULL, NULL);
				CString strTime;
				strTime.Format("%d-%d-%d %d:%d",f_time.wYear,f_time.wMonth,f_time.wDay,f_time.wHour,f_time.wMinute);
				m_PageDisk.m_DiskListCtrl.SetItem(itemcounter+1, 3, LVIF_TEXT, strTime, 0, NULL, NULL, NULL);
				m_bWriteIns = FALSE;
			}
		}
		else /*if(strFileName.Right(4) == _T(".txt") || strFileName.Right(4)==_T(".bmp") )*/
		{
			m_PageDisk.m_DiskListCtrl.InsertItem(itemcounter,strFileName,2);
		}
		m_PageDisk.m_DiskListCtrl.SetItem(itemcounter, 2, LVIF_TEXT, "N", 0, NULL, NULL, NULL);
		CString strSize;
		strSize.Format("%d%s",(f_info.nFileSizeLow+1023)/1024," KBytes");
		m_PageDisk.m_DiskListCtrl.SetItem(itemcounter, 1, LVIF_TEXT, strSize, 0, NULL, NULL, NULL);
		CString strTime;
		strTime.Format("%d-%d-%d %d:%d",f_time.wYear,f_time.wMonth,f_time.wDay,f_time.wHour,f_time.wMinute);
		m_PageDisk.m_DiskListCtrl.SetItem(itemcounter, 3, LVIF_TEXT, strTime, 0, NULL, NULL, NULL);
		strNandFilePathName = m_PageDisk.m_strNandDiskPath + strFileName;
		fs.CartInit();
		if(!fs.Initialize())
		{
			fs.ClearCartInit();
			if(IDYES == MessageBox(m_gStrMsgFormatcation,NULL,MB_YESNO))
			{
			}
			return -1;
		}
		PostMessage(WM_NAND_WRITE_BEGIN_MESSAGE,0,0);
		fs.SetOPType(CREATEFILE);
		fs.SetMessageHandle(this->m_hWnd);
		int h = fs.fat_creat(strNandFilePathName.GetBuffer(strNandFilePathName.GetLength()),ATTR_ARCHIVE);
		if(h <0)
			h = fs.fat_open(strNandFilePathName.GetBuffer(strNandFilePathName.GetLength()));
		strNandFilePathName.ReleaseBuffer(strNandFilePathName.GetLength());
		/////+++++++++++++++++++++++++++++++++++++++++++++
		//yafei 临时修改		
		fs.fat_write(h,(char*)pFileData,filelen);
		//fs.fat_write(h,(char*)pFileData,f_info.nFileSizeLow);
		/////+++++++++++++++++++++++++++++++++++++++++++++
		fs.fat_close(h);
		fs.Uninitialize();
		if(pFileData)
		{
			delete []pFileData;
			pFileData = NULL;
		}
		fs.ClearCartInit();
		PostMessage(WM_NAND_WRITE_END_MESSAGE,0,0);
	}
	if(bChangeName)
		DeleteFile(m_strFilePathName);
	ShowNandUseInfo(NAND_TOTAL_SIZE,g_dwNandFreeSpace);
	SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
	m_Tab.EnableWindow(TRUE);
	return 1;
}
GameRomInfo *CNew_EZManageDlg::GetRomInfo(CString strRomName)
{
	ROMHEADER	rhHeader;
	NDSHEADER   ndshd ;
	GameRomInfo *pInfo=NULL;
	CString		strCheck;
	DWORD		length,bufsize;
	int			index=0;
	BYTE		*m_pZipData=NULL;    
	BYTE		*pH=NULL;
	CFile		file;
	int			iSize=0;
	CRomManager rmg ;
	BYTE romReadHeader[0x100];

	//前面已经检查了ROM文件的合法性,这里就不检查了
	
	strCheck = strRomName.Right(4);
	strCheck .MakeUpper();
	if(strCheck == _T(".ZIP") || strCheck == _T(".RAR"))
	{
		//压缩文件
		if(strCheck == _T(".ZIP"))
			m_pZipData = (unsigned char *)GetZipData(strRomName,length);
		else
			m_pZipData = (unsigned char *)ExtractArchive(strRomName.GetBuffer(strRomName.GetLength()),length);
		if(!m_pZipData)
			return NULL;
		pInfo = new GameRomInfo;
		memset(pInfo , 0 , sizeof(GameRomInfo));
		if(ZipNDS)
		{
				memcpy(&ndshd,m_pZipData,sizeof(NDSHEADER));
				pH =(BYTE*) &ndshd ;
				//读ROM 名
				if(strlen(ndshd.title)>0)
				{
					//memcpy(pInfo->strGameName ,rhHeader.title,12);
					memset(pInfo->strGameName,0,12);
					sprintf(pInfo->strGameName,"%s",ndshd.title);
					memcpy(pInfo->strGameEnglishName ,pH+0xa0,16);
				}
				else
				{
					memcpy(pInfo->strGameName ,strRomName.GetBuffer(12),12);
					strRomName.ReleaseBuffer(12);
				}
				pInfo->bNdsRom = TRUE ;
		}
		else
		{
			memcpy(&rhHeader,m_pZipData,sizeof(ROMHEADER));
			pH =(BYTE*) &rhHeader ;
			//读ROM 名
			if(strlen(rhHeader.title)>0)
			{
				memset(pInfo->strGameName,0,12);
				pInfo->strGameName[12] = '\0';
				sprintf(pInfo->strGameName,"%s",rhHeader.title);
				memcpy(pInfo->strGameEnglishName ,pH+0xa0,16);
			}
			else
			{
				CString str = strRomName;
				str = str.Right(str.GetLength()-str.ReverseFind('\\')-1);
				int len = str.GetLength();
				if(len>12)
					len = 12;
				memset(pInfo->strGameName,0,12);
				memcpy(pInfo->strGameName ,str.GetBuffer(len),len);
				str.ReleaseBuffer(len);
			}
			pInfo->bNdsRom = FALSE;
		}
		
		index = pGameInfoList->GetSize();
		pInfo->uiIndex = index;
		pInfo->uiWhere = 1 ;//表示没有写入
		memcpy(pInfo->strGameWhere,strRomName.GetBuffer(strRomName.GetLength()),strRomName.GetLength());
		strRomName.ReleaseBuffer(strRomName.GetLength());

		
		//得到文件的大小
		pInfo->flGameSize  = length;
		bufsize = (UINT)ceil((double)pInfo->flGameSize / 0x40000)*0x40000 ;
		if(bufsize>length)
		{
			BYTE *pTemp = new BYTE[bufsize];
			memset(pTemp,0xFF,bufsize);
			memcpy(pTemp,m_pZipData,length);
			pInfo->pGameData = pTemp;
			pInfo->flGameSize = bufsize;
			delete []m_pZipData;
			m_pZipData=NULL;
		}
		else
		{
			pInfo->pGameData =m_pZipData;
		}
	}
	else if(strRomName.Right(4)  == _T(".NDS")||strRomName.Right(4)  == _T(".nds")||strRomName.Right(3)  == _T(".DS")||strRomName.Right(3)  == _T(".d"))
	{
		file.Open(strRomName,CFile::modeRead);
		file.Read(&ndshd,sizeof(NDSHEADER));//读文件的头 192 Bytes;
		pH =(BYTE*) &ndshd ;
		if( (pH[2]==0 && pH[3]==0xEA) || ( *(DWORD *)(&pH[0xAC])==0x53534150))
		{
			file.Close();
			/////////////
			//非压缩文件
			file.Open(strRomName,CFile::modeRead);
			file.Read(&rhHeader,sizeof(ROMHEADER));//读文件的头 192 Bytes;
			memcpy(&romReadHeader,&rhHeader,0x100);
			if(!rmg.HeaderValid(romReadHeader))
			{
				delete []m_pZipData;
				return NULL;
			}

			pH =(BYTE*) &rhHeader ;
			pInfo = new GameRomInfo;
			memset(pInfo , 0 , sizeof(GameRomInfo));
			index = pGameInfoList->GetSize();//游戏的索引(个数)从0开始
			pInfo->uiIndex = index;
			pInfo->uiWhere = 1 ;//表示没有写入
			//写入文件的全路径名字
			memcpy(pInfo->strGameWhere,strRomName.GetBuffer(strRomName.GetLength()),strRomName.GetLength());
			strRomName.ReleaseBuffer(strRomName.GetLength());
			//读ROM 名
			if(strlen(rhHeader.title)>0)
			{
				memcpy(pInfo->strGameName ,rhHeader.title,12);
				memcpy(pInfo->strGameEnglishName ,pH+0xa0,16);
			}
			else
			{
				CString str = strRomName;
				str = str.Right(str.GetLength()-str.ReverseFind('\\')-1);
				int len = str.GetLength();
				if(len>12)
					len = 12;
				memset(pInfo->strGameName,0,12);
				memcpy(pInfo->strGameName ,str.GetBuffer(len),len);
				str.ReleaseBuffer(len);
			}
			//得到文件的大小
			pInfo->bHomebrew = TRUE;
			pInfo->flGameSize = file.GetLength();
			bufsize = (UINT)ceil((double)pInfo->flGameSize / 0x40000)*0x40000 ;
			pInfo->pGameData = new BYTE[bufsize] ;
			pInfo->flGameSize = bufsize;
			memset(pInfo->pGameData,0xFF,bufsize);
			file.SeekToBegin();
			file.Read(pInfo->pGameData,bufsize);
			file.Close();
			/////////////
		}
		else
		{
			pInfo = new GameRomInfo;			
			memset(pInfo , 0 , sizeof(GameRomInfo));
			index = pGameInfoList->GetSize();//游戏的索引(个数)从0开始
			pInfo->uiIndex = index;
			pInfo->uiWhere = 1 ;//表示没有写入
			//写入文件的全路径名字
			memcpy(pInfo->strGameWhere,strRomName.GetBuffer(strRomName.GetLength()),strRomName.GetLength());
			strRomName.ReleaseBuffer(strRomName.GetLength());
			//读ROM 名
			if(strlen(ndshd.title)>0)
			{
				memcpy(pInfo->strGameName ,ndshd.title,12);
				memcpy(pInfo->strGameEnglishName ,pH+0xa0,16);
			}
			else
			{
				memcpy(pInfo->strGameName ,strRomName.GetBuffer(12),12);
				strRomName.ReleaseBuffer(12);
			}
			//得到文件的大小
			pInfo->flGameSize = file.GetLength();
			pInfo->bNdsRom = TRUE ;
			bufsize = (UINT)ceil((double)pInfo->flGameSize / 32768)*32768 ;
			pInfo->pGameData = new BYTE[bufsize] ;
			file.SeekToBegin();
			file.Read(pInfo->pGameData,bufsize);
			file.Close();
		}
	}
	else
	{
		//非压缩文件
		file.Open(strRomName,CFile::modeRead);
		file.Read(&rhHeader,sizeof(ROMHEADER));//读文件的头 192 Bytes;
		memcpy(&romReadHeader,&rhHeader,0x100);
		if(!rmg.HeaderValid(romReadHeader))
		{
			delete []m_pZipData;
			return NULL;
		}

		pH =(BYTE*) &rhHeader ;
		pInfo = new GameRomInfo;
		memset(pInfo , 0 , sizeof(GameRomInfo));
		index = pGameInfoList->GetSize();//游戏的索引(个数)从0开始
		pInfo->uiIndex = index;
		pInfo->uiWhere = 1 ;//表示没有写入
		//写入文件的全路径名字
		memcpy(pInfo->strGameWhere,strRomName.GetBuffer(strRomName.GetLength()),strRomName.GetLength());
		strRomName.ReleaseBuffer(strRomName.GetLength());
		//读ROM 名
		if(strlen(rhHeader.title)>0)
		{
			memcpy(pInfo->strGameName ,rhHeader.title,12);
			memcpy(pInfo->strGameEnglishName ,pH+0xa0,16);
		}
		else
		{
			CString str = strRomName;
			str = str.Right(str.GetLength()-str.ReverseFind('\\')-1);
			int len = str.GetLength();
			if(len>12)
				len = 12;
			memset(pInfo->strGameName,0,12);
			memcpy(pInfo->strGameName ,str.GetBuffer(len),len);
			str.ReleaseBuffer(len);
		}
		//得到文件的大小
		pInfo->flGameSize = file.GetLength();
		bufsize = (UINT)ceil((double)pInfo->flGameSize / 0x40000)*0x40000 ;
		pInfo->pGameData = new BYTE[bufsize] ;
		pInfo->flGameSize = bufsize;
		memset(pInfo->pGameData,0xFF,bufsize);
		file.SeekToBegin();
		file.Read(pInfo->pGameData,bufsize);
		file.Close();
	}
//	memcpy(&(pInfo->pGameData[4]),Nintendo_logo,Nintendo_logo_LEN);
//	pInfo->pGameData[0xb2]=0x96;
	for(int i =0 ;i<16;i++)
	{
		if(pInfo->strGameEnglishName[i]==0)
			pInfo->strGameEnglishName[i] = 0x20;
	}
	char cChar[256];
	::GetPrivateProfileString("Language","Version","",cChar,256,m_strWindowsDir+("\\temp\\conf.ini"));
	CString strLanguage;
	strLanguage = cChar;
	CString strttmmpp = pInfo->strGameEnglishName ;
	strttmmpp.Remove(' ');
	CString strChinaGameName;
	CString strTemp = pInfo->strGameName;

	if(strLanguage == _T("简体中文"))//中文界面
	{
		CString strChinaGameName = m_pGameNameInfo->FindGameChinaName(strttmmpp);
		if(strChinaGameName.GetLength()>0)
		{
			memset(pInfo->strGameChinaName,0,48);
			memcpy(pInfo->strGameChinaName,strChinaGameName.GetBuffer(strChinaGameName.GetLength()),strChinaGameName.GetLength());
			strChinaGameName.ReleaseBuffer(strChinaGameName .GetLength());
		}
		else
		{
			memset(pInfo->strGameChinaName,0,48);
			memcpy(pInfo->strGameChinaName,pInfo->strGameName,12);

		}
	}
	else if(strLanguage == _T("羉砰いゅ"))//中文界面
	{
		
		CString strTT ;
		strChinaGameName = m_pGameNameInfo->FindGameChinaName(strttmmpp);
		strTT = strChinaGameName ;
		strChinaGameName = m_bb.Gb2Big5(strTT);
		if(strChinaGameName.GetLength()>0)
		{
			memset(pInfo->strGameChinaName,0,48);
			memcpy(pInfo->strGameChinaName,strChinaGameName.GetBuffer(strChinaGameName.GetLength()),strChinaGameName.GetLength());
			strChinaGameName.ReleaseBuffer(strChinaGameName .GetLength());
		}
		else
		{
			memset(pInfo->strGameChinaName,0,48);
			memcpy(pInfo->strGameChinaName,pInfo->strGameName,12);

		}
		
	}
	else
	{
		strChinaGameName = m_pGameNameInfo->FindGameEnglishName(strttmmpp);
		
		memset(pInfo->strGameChinaName,0,48);
		memcpy(pInfo->strGameChinaName,strChinaGameName.GetBuffer(strChinaGameName.GetLength()),strChinaGameName.GetLength());
		strChinaGameName.ReleaseBuffer(strChinaGameName .GetLength());
	}
	XCODE *pXCode = FindXCode(pInfo->pGameData,pInfo->strGameName,pInfo->flGameSize);
	if(pXCode)
	{
		//if(cartType !=EZ2Cart)
		{
			pInfo->bCheat = TRUE;
			pInfo->bReset = TRUE;
		}
	
		iSize = sizeof(XCODE)-4+pXCode->CodeSize*sizeof(CODE);
		pInfo->pXCode = new BYTE[iSize];
		memset(pInfo->pXCode ,0,iSize);
		memcpy(pInfo->pXCode,(XCODE *)pXCode,sizeof(XCODE)-4);
		memcpy(pInfo->pXCode+sizeof(XCODE)-4,(CODE *)pXCode->pCode,pXCode->CodeSize*sizeof(CODE));
		if(pXCode->pCode)
		{
			delete []pXCode->pCode;
			pXCode->pCode=NULL;
			delete pXCode;
			pXCode=NULL;
		}
	}
	SAVERTYPE type;
	DWORD SaveSize =GetSaverTypeAndSize(&pInfo->pGameData,(DWORD *)&pInfo->flGameSize,&type);
	pInfo->uiSaverSize = SaveSize;
	pInfo->uiSaverMode = type;
	TrimRom(&pInfo->pGameData,&(pInfo->flGameSize));
	if((pInfo->uiSaverMode==EEPROM_TYPE)||(pInfo->uiSaverMode==SRAM_TYPE))
		pInfo->SavePatch = 0;//EZ-TYOE
	else if(pInfo->uiSaverSize == 0x20000)
	{
		pInfo->SavePatch = 0;//EZ-TYPE
	}
	if(pInfo->uiSaverMode==UNKNOW)
	{
		pInfo->uiSaverSize  = 0x8000;
	}
	//下面是检查ROM大小

	return pInfo;
}

/****************************************************/
/*Zip解压函数
参数:入strZipFileName:待解压的Zip文件;
返回:游戏数指针
/****************************************************/
char *CNew_EZManageDlg::GetZipData(CString strZipFileName,unsigned long &length)
{
	int result;
	int rt;
	unz_file_info m_file_info;
	char *pszFileName = new char[256];
	uLong fileNameBufferSize = 256;
	char *extraField = new char[256];
	uLong extraFieldBufferSize = 256;
	char *szComment= new char[256];
	uLong commentBufferSize = 256;
	CString strTemp;
	char *pGameData =NULL;
	ZipNDS = 0 ;
	unzFile temp = unzOpen(strZipFileName.GetBuffer(strZipFileName.GetLength()));
	strZipFileName.ReleaseBuffer(strZipFileName.GetLength());
	unzOpenCurrentFile(temp);
	unzGetCurrentFileInfo(temp,&m_file_info,pszFileName,fileNameBufferSize,extraField,extraFieldBufferSize,szComment,commentBufferSize);
	strTemp = pszFileName;
	strTemp.MakeUpper();
	CString strFileName = strTemp;
	while(strTemp.Find(".GBA")<0 &&strTemp.Find(".BIN")<0&&strTemp.Right(4)!=_T(".NDS")&&strTemp.Right(3)!=_T(".DS"))//没有找到GBA文件
	{
		rt = unzGoToNextFile(temp);
		if(rt == UNZ_END_OF_LIST_OF_FILE )
			goto out;
		unzOpenCurrentFile(temp);
		unzGetCurrentFileInfo(temp,&m_file_info,pszFileName,fileNameBufferSize,extraField,extraFieldBufferSize,szComment,commentBufferSize);
		if(strTemp == pszFileName)
			goto out;
		strTemp = pszFileName;
		strTemp.MakeUpper();
		
	}
	if(strTemp.Right(4)==_T(".NDS")||strTemp.Right(3)==_T(".DS"))
		ZipNDS = 1;
	else
		ZipNDS = 0;

	pGameData = new char[m_file_info.uncompressed_size];
	length = m_file_info.uncompressed_size;
	memset(pGameData,0,m_file_info.uncompressed_size);
	result = unzReadCurrentFile(temp,pGameData,m_file_info.uncompressed_size);
	unzClose(temp);
out:
	if(pszFileName)
		delete []pszFileName;
	if(extraField )
		delete []extraField;
	if(szComment)
		delete []szComment;
	if(result<0)
	{
		delete []pGameData;
		return NULL;
	}
	return pGameData;
}

BYTE* CNew_EZManageDlg::ExtractArchive(char *ArcName,DWORD &dw_Len)
{
	enum { EXTRACT, TEST, PRINT };
	CString strTempFileName = m_strWorkPath+_T("\\temp\\temp.gba");
		CString str;
	BYTE *pData=NULL;
	int Mode = EXTRACT;
	HANDLE hArcData;
	int RHCode,PFCode;
	char CmtBuf[16384];
	struct RARHeaderData HeaderData;
	struct RAROpenArchiveDataEx OpenArchiveData;
	dw_Len=0;
	memset(&OpenArchiveData,0,sizeof(OpenArchiveData));
	OpenArchiveData.ArcName=ArcName;
	OpenArchiveData.CmtBuf=CmtBuf;
	OpenArchiveData.CmtBufSize=sizeof(CmtBuf);
	OpenArchiveData.OpenMode=RAR_OM_EXTRACT;
	hArcData=RAROpenArchiveEx(&OpenArchiveData);

	if (OpenArchiveData.OpenResult!=0)
	{
		return NULL;
	}

  HeaderData.CmtBuf=NULL;

  while ((RHCode=RARReadHeader(hArcData,&HeaderData))==0)
  {
	str = HeaderData.FileName;
	str.MakeUpper();
	if((str.Right(4) != _T(".GBA"))&&(str.Right(4) != _T(".NDS"))&&(str.Right(4) != _T(".BIN"))&& (str.Right(3) != _T(".DS")))//RAR文件
		continue;
	ZipNDS = 0 ;
	if((str.Right(4) == _T(".NDS"))||(str.Right(3) == _T(".DS")))
		ZipNDS = 1 ;
    PFCode=RARProcessFile(hArcData,(Mode==EXTRACT) ? RAR_EXTRACT:RAR_TEST,
                          NULL,strTempFileName.GetBuffer(strTempFileName.GetLength()));
	strTempFileName.ReleaseBuffer(strTempFileName.GetLength());
    if (PFCode==0)
		break;
  }

  if (RHCode==ERAR_BAD_DATA)
    printf("\nFile header broken");
  CFile file;
  if(file.Open(strTempFileName,CFile::modeRead))
  {
	dw_Len = file.GetLength();
	pData = new BYTE[dw_Len];
	file.Read(pData,dw_Len);
	file.Close();
	DeleteFile(strTempFileName);
  }
  RARCloseArchive(hArcData);
  return pData;
}

XCODE *CNew_EZManageDlg::FindXCode(BYTE *pGameData,char romname[12],DWORD GameSize)
{
	XCODE *pXCode = NULL ;
	XCODE *m_pxCode= NULL;
	CString str;
	CGoldenCheat testXCode;
	str =m_strWorkPath+ _T("\\Sysbin\\ez_flash.bin");
	BOOL bOK = FALSE;
//	BYTE temp[13];
	CXCodeFile xcodefile;
	xcodefile.Init(str);
	BYTE *p = NULL;
	for(int i=0;i<xcodefile.m_xCodeArray.GetSize();i++)
	{
		m_pxCode = xcodefile.m_xCodeArray.GetAt(i);
		if(memcmp(romname,m_pxCode->gameName,12)==0)
		{
			//找到相同的名字的XCode,再校验.正确的话,返回该XCode,否则在找
			int iSzie = sizeof(XCODE)-4+m_pxCode->CodeSize*sizeof(CODE);
			p = new BYTE[iSzie];
			memcpy(p,(XCODE *)m_pxCode,sizeof(XCODE)-4);
			memcpy(p+sizeof(XCODE)-4,(CODE *)m_pxCode->pCode,m_pxCode->CodeSize*sizeof(CODE));
			if(testXCode.IsXCodeValid(p,pGameData,GameSize))
			{
				if(p)
					delete p;
				bOK = TRUE;
				break;
			}
			else
			{
				if(p)
					delete p;
			}
		}	
	}
	if(m_pxCode && (bOK == TRUE))
	{
		pXCode = new XCODE;
		pXCode->pCode = NULL;
		memcpy(pXCode,m_pxCode,sizeof(XCODE)-4);
		if(pXCode->CodeSize>0)
			pXCode->pCode = new CODE[pXCode->CodeSize];
		memcpy(pXCode->pCode,m_pxCode->pCode,pXCode->CodeSize*sizeof(CODE));
	}
	xcodefile.Quit();

	return pXCode;
}

DWORD CNew_EZManageDlg::GetSaverTypeAndSize(BYTE **buf,DWORD *length,SAVERTYPE *type)
{
	BYTE * find = NULL ;
	*type = UNKNOW ;
	DWORD rvalue = 0;

	find = FindMotif(*buf, *length, (BYTE*)"SRAM_", 5);
	if(find)
	{
		*type = SRAM_TYPE ; //表示存档类型为sram
		rvalue = 32768;
	}
	find = FindMotif(*buf, *length, (BYTE*)"EEPROM_V", 8);
	if(find)
	{
		*type = EEPROM_TYPE ; //表示存档类型为sram
		rvalue = 8192 ;
	}
	find = FindMotif(*buf, *length, (BYTE*)"FLASH_V", 7);
	if(find) 
	{
		*type = FLASH_TYPE ;
		rvalue = 65536;
	}
	find = FindMotif(*buf, *length, (BYTE*)"FLASH512_V", 9);
	if(find) 
	{
		*type = FLASH_TYPE ;
		rvalue = 65536;
	}
	find = FindMotif(*buf, *length, (BYTE*)"FLASH1M_", 8);
	if(find)
	{
		*type = FLASH_TYPE ;
		rvalue = 65536*2;
	}

	char name[16] ;
	memset(name,0,16);
	//here 处理特别的游戏
	memcpy(name,&(*buf)[0xa0],12);
	memset(name,0,16);
	memcpy(name,&(*buf)[0xAC],4);
	if((!strcmp(_T("GMBA"),name))||(!strcmp(_T("PCEA"),name))||
		(!strcmp(_T("FCA"),name))||(!strcmp(_T("PNES"),name)))
	{
		*type = SRAM_TYPE ; //表示存档类型为sram
		rvalue = 32768;
	}

	return rvalue ;
}
BYTE * CNew_EZManageDlg::FindMotif(BYTE *Buffer, DWORD BufferSize, BYTE *Motif, DWORD MotifSize)
{
	//Arithmatic from Dadycool
	BYTE* find = Buffer;
	while(find)
	{
		find = (BYTE *)memchr(Buffer,Motif[0],BufferSize-MotifSize);
		if (find)
		{
			if (memcmp(find,Motif,MotifSize) == 0)
			{
				break;
			}
		}
		BufferSize -= find - Buffer + 1;
		Buffer = find + 1;
	}
	return find;
}
void CNew_EZManageDlg::TrimRom(BYTE **pBuf, int *iSize)
{
	int  addr,i;
	bool OverDump;

	if(*iSize > 0x20000)
	{
		if((*iSize % 0x20000) != 0)
			addr = *iSize - (*iSize % 0x20000);
		else
			addr = *iSize - 0x20000;

		while(addr > 0x20000)
		{
			OverDump = false;
			for(i=addr; i<*iSize; i++) 
				if(((*pBuf)[i] != 0x00)&&((*pBuf)[i] != 0xFF))
					break;
			if(i == *iSize)
			{
				OverDump = true;
			}

			if(!OverDump)
			{
				break;
			}
			*iSize = addr;
			addr -= 0x20000;
		}
	}
}

void CNew_EZManageDlg::DeleteGoldCheatPointer(GOLDCHEAT *pGoldCheat,WORD size)
{
	if(!pGoldCheat)
		return;
	GOLDCHEAT *pTempGlodCheat=NULL;
	PGOLDCODEITEM pItem=NULL;
	for(int i=0;i<size;i++)
	{
		pTempGlodCheat = &pGoldCheat[i];

		pItem = pTempGlodCheat->pItem;
		for(int i=0;i<pTempGlodCheat->SumofItem;i++)
		{
			if(pItem)
			{
				if(pItem->pGlodData)
				{
					delete []pItem->pGlodData;
					pItem->pGlodData = NULL;
				}
			}
			pItem++;
		}
		delete []pTempGlodCheat->pItem;
	}
	delete []pGoldCheat;
}

void CNew_EZManageDlg::ReMoveAllGameList()
{
	UINT i ;
	CString strTemp;
	BYTE *pGameData=NULL;
	BYTE *pXCode =NULL;
	PGOLDCHEAT	pGlodCheat = NULL;
	UINT uiCount = pGameInfoList->GetSize();
	
	for(i = 0 ; i < uiCount ; i++)
	{
		pGameData = pGameInfoList->GetAt(i)->pGameData ;
		if(pGameData)
		{
			delete []pGameData;
			pGameData = NULL;
		}
		pXCode = pGameInfoList->GetAt(i)->pXCode;
		if(pXCode)
		{
			if(pGameInfoList->GetAt(i)->uiWhere ==0)
			{
				if(((XCODE *)pXCode)->pCode)
					delete []((XCODE *)pXCode)->pCode;
				delete pXCode;
			}
			else
			{
				delete []pXCode;
				pXCode=NULL;
			}
		}
		pGlodCheat = pGameInfoList->GetAt(i)->pGlodCheat;
		if(pGlodCheat)
		{
			DeleteGoldCheatPointer(pGlodCheat,pGameInfoList->GetAt(i)->CheatSize);
		}
		delete pGameInfoList->GetAt(i);
	}
	pGameInfoList->RemoveAll();
	
}

void CNew_EZManageDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer(IDENTIFY_CART_TIMER);
	ReMoveAllGameList();
	DeleteCriticalSection(&g_statusCritical);
	if(m_pezVirtualBase)
	{
		delete m_pezVirtualBase;
		m_pezVirtualBase = NULL;
	}
	if(m_pGameNameInfo)
	{
	//	m_pGameNameInfo->Quit();
		delete m_pGameNameInfo;
	}

	m_bb.Close();

}

void CNew_EZManageDlg::OnAddLoader() 
{
	// TODO: Add your command handler code here
	m_bNdsLoader = FALSE;
	WriteLoader();
}

void CNew_EZManageDlg::WriteLoader()
{
	pTWThread = AfxBeginThread(TWThreadWriteLoader,
								this);	
}

UINT CNew_EZManageDlg::TWThreadWriteLoader( LPVOID pParam )
{
	CNew_EZManageDlg  *pDlg = (CNew_EZManageDlg*)pParam ;
	return pDlg->TWThreadWriteLoader();
}
UINT CNew_EZManageDlg::TWThreadWriteLoader()
{
//	if(IDYES != MessageBox("检测到新的EZ3 OS 版本,是否更新",NULL,MB_YESNO))
//		return -1;
	EZ3CARTTYPE	ttp;
	HANDLE hDev;
	DWORD iLength, sector, dwi,dw_Range,dw_pos;
	CFile file;
	BYTE *pBuf;
	EnterCriticalSection(&g_statusCritical);
	ttp = g_CartType ;
	LeaveCriticalSection(&g_statusCritical);
	if(ttp ==NON_EZ3Cart)
		return 0;
	KillTimer(IDENTIFY_CART_TIMER);
	CString strPath;
	if(m_bNdsLoader)
		strPath=m_strWorkPath+ _T("\\Sysbin\\loader_NDS.bin");
	else
		strPath=m_strWorkPath+ _T("\\Sysbin\\loader_GBA.bin");
	file.Open(strPath,CFile::modeRead);
	iLength = file.GetLength();
	sector = (iLength+0xFFFF)/0x10000 ;
	iLength = (iLength+32767)/32768*32768 ;
	pBuf = new BYTE[iLength] ;
	dw_Range = sector + iLength/32768;
	m_WriteProgress.SetRange(0,dw_Range);
	file.Read(pBuf,iLength);
	file.Close();
	if(m_bUpdtaLoader)
	{
		if(m_bNdsLoader)
			m_WriteProgress.SetWindowText(m_gStrMsgInstallOS+" NDS OS 1.04");
		else
			m_WriteProgress.SetWindowText(m_gStrMsgInstallOS+" "+GetLoaderVersionFromCard(*(WORD *)(pBuf+0x1C8)));
	}
	else
	{
		if(m_bNdsLoader)
			m_WriteProgress.SetWindowText(m_gStrMsgInstallOSUp+" NDS OS 1.04");
		else
			m_WriteProgress.SetWindowText(m_gStrMsgInstallOSUp+" "+GetLoaderVersionFromCard(*(WORD *)(pBuf+0x1C8)));
	}
	m_LoaderVersion = *(WORD *)(pBuf+0x1C8);
	
	m_Tab.EnableWindow(FALSE);
	m_pezVirtualBase->OpenCartDevice(hDev);
	m_pezVirtualBase->CartOpenFlashOP(hDev);
	m_pezVirtualBase->CartOpen(hDev);
	m_pezVirtualBase->LDRFLH_ReadArray(hDev);
	dw_pos=0;
	for(dwi=0; dwi<sector; dwi++)
	{
		m_pezVirtualBase->LDRFLH_BlockErase(hDev,dwi);
		m_WriteProgress.SetPos(dw_pos++);
	}
	
	GAMEINFOLIST *pList = pGameInfoList ;
	GameRomInfo *pInfo = NULL;
	BYTE *pChinaNameBuf = &pBuf[0x28000];
	char *sign= "EZ3-Name";
	memcpy(pChinaNameBuf,sign,8);
	WORD loop = pList->GetSize();
	*((WORD *)(&pChinaNameBuf[9])) = loop;
	for(int i=0;i<loop;i++)
	{
		pInfo = pList->GetAt(i);
		if(pInfo->strGameChinaName[0]!='0')
			memcpy(&pChinaNameBuf[(i+1)*48],pInfo->strGameChinaName,48);
		else
			memcpy(&pChinaNameBuf[(i+1)*48],pInfo->strGameName,48);
	}
	dwi=m_pezVirtualBase->LDRFLH_ReadStatus(hDev);
	m_pezVirtualBase->LDRFLH_ClearStatus(hDev);
	m_pezVirtualBase->LDRFLH_ReadArray(hDev);
	for(dwi = 0 ; dwi< iLength ; dwi+= 32768 )
	{
		//title.Format("写入%x 字节",dwi);
		m_pezVirtualBase->LDRFLH_ReadArray(hDev);
		Sleep(5);
		m_pezVirtualBase->LDRFLH_Write(hDev,dwi/2,pBuf+dwi , ((dwi+32768)>(4*iLength))?(dwi%32768):32768);
		m_WriteProgress.SetPos(dw_pos++);
		//m_ezFlash._Write(hDev,0x400000+dwi/2,pBuf+dwi , ((dwi+32768)>(4*iLength))?(dwi%32768):32768);	
	}
	m_pezVirtualBase->CartClose(hDev);
	m_pezVirtualBase->CartCloseFlashOP(hDev);
	m_pezVirtualBase->CloseCartDevice(hDev);
	m_Tab.EnableWindow(TRUE);
	m_WriteProgress.SetWindowText(m_gStrMsgInstallOSEnd);
	m_WriteProgress.SetPos(0);
	m_bUpdtaLoader  = FALSE;
	if(m_bNdsLoader)
		SetWindowText(_T("EZManager 2.22 (NDS OS 1.04,Driver Version ")+m_strDriverVersion+_T(" )"));
	else
		SetWindowText("EZManager 2.22 (" +GetLoaderVersionFromCard(m_LoaderVersion)+_T("Driver Version ")+m_strDriverVersion+_T(" )"));
	m_bNdsLoader = FALSE;
	SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
	return 0;
}
void CNew_EZManageDlg::ShowRomUseInfo(BOOL bCart)
{
	CString str;
	DWORD dw_Use = 0;
	switch(g_dwNandSize)
	{
	case 0x20000:
		m_RomProgress.SetRange(0,128);
	case 0x40000:
		m_RomProgress.SetRange(0,256);
		break;
	case 0x80000:
		m_RomProgress.SetRange(0,512);
		break;
	case 0x100000:
		m_RomProgress.SetRange(0,1024);
		break;
	}
	for(int i=0;i<pGameInfoList->GetSize();i++)
	{
		dw_Use += pGameInfoList->GetAt(i)->flGameSize;
	}
	dw_Use = (dw_Use*8)/(1024*1024);
	if(dw_Use<=256)
	{
		str.Format("%s%s%d%s%s%d%s","256MBits FLASH: ",m_gStrMsgUse,dw_Use,"MBits,",m_gStrMsgLeft,256-dw_Use,"MBits.");
		m_RomProgress.SetWindowText(str);
		m_RomProgress.SetPos(dw_Use);
	}
	if(bCart)
	{
		m_RomProgress.SetWindowText(m_gStrMsgNoCart);
		m_RomProgress.SetPos(dw_Use);
	}
}
void CNew_EZManageDlg::ShowNandUseInfo(int TotalSize,int NouseSize,BOOL bCart)
{
extern DWORD g_dwNandSize;
	CString str;
	CString strTemp;
	int dw_reserve = 8*1024;
	TotalSize = g_dwNandSize;
	strTemp.Format("%dMB %s",TotalSize/1024,m_gStrMsgEZDisk);
	str = strTemp;
	switch(TotalSize)
	{
	case 0x20000:
		m_RomProgress.SetRange(0,128);
	case 0x40000:
		m_RomProgress.SetRange(0,256);
		break;
	case 0x80000:
		m_RomProgress.SetRange(0,512);
		break;
	case 0x100000:
		m_RomProgress.SetRange(0,1024);
		break;
	}
	if((NouseSize-dw_reserve)>0)
		strTemp.Format("%s:%dMB ",m_gStrMsgLeft,(NouseSize-dw_reserve)/1024);
	else
		strTemp.Format("%s:%dMB ",m_gStrMsgLeft,0);
	str+=strTemp;
	strTemp.Format("%s:%dMB ",m_gStrMsgReserve,dw_reserve/1024);
	str+=strTemp;
	str+=m_gStrMsgCurrent+m_PageDisk.m_strNandDiskPath;
	m_RomProgress.SetWindowText(str);
	m_RomProgress.SetPos( (TotalSize-(NouseSize-dw_reserve))/1024);
	//m_RomProgress.SetPos();
	if(bCart)
	{
		m_RomProgress.SetWindowText(m_gStrMsgNoCart);
		m_RomProgress.SetPos(0);
	}
}
BOOL CNew_EZManageDlg::CheckRom(DWORD dwLastGameLength)
{
	int length=0;
	int iSize = pGameInfoList->GetSize();
	GameRomInfo *pInfo;
	for(int i=0;i<iSize;i++)
	{
		pInfo = pGameInfoList->GetAt(i);
		length += pInfo->flGameSize;
	}
	length = length +dwLastGameLength;
	if(length<=ROM_SIZE)
		return TRUE;
	else
		return FALSE;
}

LRESULT CNew_EZManageDlg::OnAddRom(WPARAM wparam, LPARAM lparam)
{

	POSITION	pos;
	CString str,strGbaPathName,strGbaFileName;
	CWnd *pWnd = GetParent();
	CNew_EZManageDlg *pDlg = (CNew_EZManageDlg *)pWnd;

	CString strFilter = _T("All Surport files(*.gba;*.bin;*.zip)|*.gba;*.bin;*.zip|\
						GameBoy Advanced Roms(*.gba;*.bin)|*.gba;*.bin|\
						Zipped Roms(*.zip)|*.zip|\
						All Files (*.*)|*.*||");
	CFileDialog openDlg(TRUE , //for open
						NULL , //default filename extension
						NULL , //initial filename 
						OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT|OFN_HIDEREADONLY|OFN_EXPLORER ,
						strFilter,
						this
						) ;
	int code = openDlg.DoModal();
	if(code == IDOK)
	{
		strGbaPathName = openDlg.GetPathName();
		strGbaFileName = openDlg.GetFileName();
		pos = openDlg.GetStartPosition() ;
		while(pos)
		{
			str.Empty();
			str = openDlg.GetNextPathName(pos);
			str.MakeUpper();
			GameRomInfo *pInfo = GetRomInfo(str);
			if(pInfo)
			{
				if(!CheckRom(pInfo->flGameSize))
				{
					if(pInfo->pGameData)
					{
						delete []pInfo->pGameData;
						pInfo->pGameData = NULL;
					}
					if(pInfo->pGlodCheat)
					{
						pDlg->DeleteGoldCheatPointer(pInfo->pGlodCheat,pInfo->CheatSize);
					}
					delete pInfo;
					MessageBox(m_gStrMsgNoEnough,m_gStrMsgCation,MB_ICONEXCLAMATION | MB_OK);
					return 1;
				}
			}
			pGameInfoList->Add(pInfo);
			AddItemToNorList(pInfo);
			ShowRomUseInfo();
			
		}	
	}
	return 1;
}
LRESULT CNew_EZManageDlg::OnDelRom(WPARAM wparam, LPARAM lparam)
{
	if(pGameInfoList->GetSize() ==0)
		return -1;
	GameRomInfo *pInfo= pGameInfoList->GetAt(pGameInfoList->GetSize()-1);
	if(pInfo->pGameData)
	{
		delete []pInfo->pGameData;
		pInfo->pGameData=NULL;
	}
	BYTE *pXCode =pInfo->pXCode;
	if(pXCode)
	{
		if(pInfo->uiWhere ==0)
		{
			if(((XCODE *)pXCode)->pCode)
				delete []((XCODE *)pXCode)->pCode;
			delete pXCode;
		}
		else
		{
			delete []pXCode;
			pXCode=NULL;
		}
	}
	PGOLDCHEAT	pGlodCheat= pInfo->pGlodCheat;
	if(pGlodCheat)
	{
		DeleteGoldCheatPointer(pGlodCheat,pInfo->CheatSize);
	}
	if(pInfo)
	{
		delete pInfo;
		pInfo = NULL;
	}
	pGameInfoList->RemoveAt(pGameInfoList->GetSize()-1);
	ShowRomUseInfo();
	return 1;
}
LRESULT CNew_EZManageDlg::OnBackRom(WPARAM wparam, LPARAM lparam)
{
	int  nItem = -1;
	UINT i;
	UINT uSelectedCount  = m_PageFlash.m_ListCtrl.GetSelectedCount();
	if (uSelectedCount > 0)
	{
	   for (i=0;i < uSelectedCount;i++)
	   {
		  nItem = m_PageFlash.m_ListCtrl.GetNextItem(nItem, LVNI_SELECTED);
	   }
	}
	else
	{
		MessageBox(m_gStrMsgSlectBackRom,m_gStrMsgCation,MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}
	GameRomInfo *pInfo ;
	int iGameCount =0;

	int index=nItem;
	if(pGameInfoList->GetSize()<=0)
	{
		return -1;
	}
	CString strBackfile ;
	pInfo = pGameInfoList->GetAt(index);
	if(pInfo->uiWhere)
	{
		MessageBox(m_gStrMsgErrorBackRom,NULL,MB_ICONWARNING);
		return -1;
	}
	iGameCount = pGameInfoList->GetSize();
	if((index>iGameCount) || (index)<0)
	{
		SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
		m_WriteProgress.SetWindowText(m_gStrMsgBackRomEnd);
		return -1;
	}

	CString strFile = pInfo->strGameName;
	strFile +=_T("_Back");
	CString strFilter = _T("GameBoy Advanced Roms(*.gba;*.bin)|*.gba;*.bin|\
	All Files (*.*)|*.*||");
	CFileDialog openDlg(FALSE , //for open
						".gba" , //default filename extension
						strFile,
						OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT|OFN_HIDEREADONLY|OFN_EXPLORER ,
						strFilter,
						this
						) ;
	if(openDlg.DoModal() != IDOK)
	{
		return -1;
	}
	m_backstrFileName = openDlg.GetPathName();

	pTWThread = AfxBeginThread(TWThreadBackupRom,
								this);
	return 1;
}
UINT CNew_EZManageDlg::TWThreadBackupRom( LPVOID pParam )
{
	CNew_EZManageDlg  *pDlg = (CNew_EZManageDlg*)pParam ;
	return pDlg->TWThreadBackupRom();
}
UINT CNew_EZManageDlg::TWThreadBackupRom()
{
	int  nItem = -1;
	UINT i;
	UINT uSelectedCount  = m_PageFlash.m_ListCtrl.GetSelectedCount();
	if (uSelectedCount > 0)
	{
	   for (i=0;i < uSelectedCount;i++)
	   {
		  nItem = m_PageFlash.m_ListCtrl.GetNextItem(nItem, LVNI_SELECTED);
	   }
	}
	
	HANDLE hDev;
	CFile backfile;
	GameRomInfo *pInfo ;
	int iGameCount =0;
	DWORD StartAddr,uipage=0;
	DWORD dwRomSize=0;
	BYTE *tempbuf;

	int index=nItem;
	if(pGameInfoList->GetSize()<=0)
	{
		return -1;
	}
	CString strBackfile ;
	pInfo = pGameInfoList->GetAt(index);
	if(pInfo->uiWhere)
	{
		MessageBox(m_gStrMsgErrorBackRom,NULL,MB_ICONWARNING);
		return -1;
	}
	iGameCount = pGameInfoList->GetSize();
	if((index>iGameCount) || (index)<0)
	{
		SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
		m_WriteProgress.SetWindowText(m_gStrMsgBackRomEnd);
		return -1;
	}

	
	KillTimer(IDENTIFY_CART_TIMER);
	CMenu *pMenu = AfxGetMainWnd()->GetMenu();
	pMenu->EnableMenuItem( IDD_ADD_LOADER, MF_GRAYED);
	pMenu->EnableMenuItem( IDD_SYNC_CLOCK, MF_GRAYED);
	pMenu->EnableMenuItem( IDD_UPDATA_FILE, MF_GRAYED);

	m_pezVirtualBase->OpenCartDevice(hDev);
	m_pezVirtualBase->SetMapMode(0x0);
	m_pezVirtualBase->CartSetROMPage(hDev,0);
	m_pezVirtualBase->CartOpenFlashOP(hDev);
	m_pezVirtualBase->CartOpen(hDev);
	m_pezVirtualBase->NAND_ENABLE(hDev,0);
	m_pezVirtualBase->EXECFLH_ReadArray(hDev);
	m_WriteProgress.SetWindowText("正在备份ROM...");
	m_Tab.EnableWindow(FALSE);
	tempbuf = new BYTE[pInfo->flGameSize];
	StartAddr  = 0;
	for(int ii=0;ii<index;ii++)
	{
		StartAddr += pGameInfoList->GetAt(ii)->flGameSize;
	}
	ii = pInfo->flGameSize/32768;
	m_WriteProgress.SetRange(0,ii);
	for(ii=0;ii<(pInfo->flGameSize/32768);ii++)
	{
		m_pezVirtualBase->EXECFLH_Read(hDev,(StartAddr+ii*32768)>>1,&tempbuf[ii*32768],32768);
		m_WriteProgress.SetPos(ii);
	}
	m_pezVirtualBase->CartCloseFlashOP(hDev);
	m_pezVirtualBase->CartClose(hDev);
	m_pezVirtualBase->CloseCartDevice(hDev);
	backfile.Open(m_backstrFileName,CFile::modeCreate|CFile::modeReadWrite);
	backfile.Write(tempbuf,pInfo->flGameSize);
	delete []tempbuf;

	SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
	pMenu->EnableMenuItem( IDD_ADD_LOADER, MF_BYCOMMAND);
	pMenu->EnableMenuItem( IDD_SYNC_CLOCK, MF_BYCOMMAND);
	pMenu->EnableMenuItem( IDD_UPDATA_FILE, MF_BYCOMMAND);
	m_Tab.EnableWindow(TRUE);
	m_WriteProgress.SetWindowText(m_gStrMsgBackRomEnd);
	m_WriteProgress.SetPos(0);	
	m_backstrFileName.Empty();
	return 1;
}
LRESULT CNew_EZManageDlg::OnFreshRom(WPARAM wparam, LPARAM lparam)
{
	ReMoveAllGameList();
	m_PageFlash.m_ListCtrl.DeleteAllItems();
	ReadEZ3NorFlash();
	return 1;
}
LRESULT CNew_EZManageDlg::OnFormatRom(WPARAM wparam, LPARAM lparam)
{
	if(IDNO == MessageBox(m_gStrMsgFormatNorCation,m_gStrMsgCation,MB_ICONEXCLAMATION | MB_YESNO))
	{
		return 1;
	}
	pTWThread = AfxBeginThread(TWThreadFormatRom,
								this);
	return 1;
}


UINT CNew_EZManageDlg::TWThreadFormatRom( LPVOID pParam )
{
	CNew_EZManageDlg  *pDlg = (CNew_EZManageDlg*)pParam ;
	return pDlg->TWThreadFormatRom();
}
UINT CNew_EZManageDlg::TWThreadFormatRom()
{
	KillTimer(IDENTIFY_CART_TIMER);
	CMenu *pMenu = AfxGetMainWnd()->GetMenu();
	pMenu->EnableMenuItem( IDD_ADD_LOADER, MF_GRAYED);
	pMenu->EnableMenuItem( IDD_SYNC_CLOCK, MF_GRAYED);
	pMenu->EnableMenuItem( IDD_UPDATA_FILE, MF_GRAYED);

	HANDLE hDev;
	m_Tab.EnableWindow(FALSE);
	m_WriteProgress.SetWindowText(m_gStrMsgFormatingNor);
	m_pezVirtualBase->OpenCartDevice(hDev);
	m_pezVirtualBase->SetMapMode(0x0);
	m_pezVirtualBase->CartSetROMPage(hDev,0);
	m_pezVirtualBase->CartOpenFlashOP(hDev);
	m_pezVirtualBase->CartOpen(hDev);
	m_pezVirtualBase->NAND_ENABLE(hDev,0);
	m_pezVirtualBase->EXECFLH_ReadArray(hDev);
	m_pezVirtualBase->EXECFLH_ClearStatus(hDev);
	m_pezVirtualBase->EXECFLH_BlockErase(hDev,0);
	m_pezVirtualBase->EXECFLH_BlockErase(hDev,1);
	m_pezVirtualBase->CartClose(hDev);
	m_pezVirtualBase->CartCloseFlashOP(hDev);
	m_pezVirtualBase->CloseCartDevice(hDev);
	ReMoveAllGameList();
	m_PageFlash.m_ListCtrl.DeleteAllItems();
	ReadEZ3NorFlash();
	m_WriteProgress.SetWindowText(m_gStrMsgFormatNorEnd);
	m_Tab.EnableWindow(TRUE);
	SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
	pMenu->EnableMenuItem( IDD_ADD_LOADER, MF_BYCOMMAND);
	pMenu->EnableMenuItem( IDD_SYNC_CLOCK, MF_BYCOMMAND);
	pMenu->EnableMenuItem( IDD_UPDATA_FILE, MF_BYCOMMAND);

	return 1;
}
LRESULT CNew_EZManageDlg::OnWriteRom(WPARAM wparam, LPARAM lparam)	
{
	pTWThread = AfxBeginThread(TWThreadWrite,
								this);
	return 1;
}
UINT CNew_EZManageDlg::TWThreadWrite( LPVOID pParam )
{
	CNew_EZManageDlg  *pDlg = (CNew_EZManageDlg*)pParam ;
	return pDlg->TWThreadWriteRom();
}
UINT CNew_EZManageDlg::TWThreadWriteRom()
{
	EZ3CARTTYPE	ttp;
	//GAMEINFOLIST *pList = pGameInfoList;
	GameRomInfo *pInfo = NULL , *pLsInfo = NULL ;
	DWORD StartAddress = 0 , RomSize = 0 , dwOff = 0 , SaverSize = 0 ;
	HANDLE hDev ;
	CStringArray nameArray ;
	DWORD loadsize=0;
	int iGame  =0;
	BYTE *pTemp  = NULL;
	CString m_strWritemLeft;

	int j;
	CRomManager rm ;
	CString str;
	int status;

	EnterCriticalSection(&g_statusCritical);
	ttp = g_CartType ;
	LeaveCriticalSection(&g_statusCritical);
	
	if(ttp ==NON_EZ3Cart)
		return 0;
	m_Tab.EnableWindow(FALSE);
	RomSize = NOR_FLASH_SIZE;
	iGame  = pGameInfoList->GetSize();
	if(iGame==0 || pGameInfoList->GetAt(iGame-1)->uiWhere==0)
	{
		m_Tab.EnableWindow(TRUE);
		return 0;
	}
	KillTimer(IDENTIFY_CART_TIMER);
	CMenu *pMenu = AfxGetMainWnd()->GetMenu();
	pMenu->EnableMenuItem( IDD_ADD_LOADER, MF_GRAYED);
	pMenu->EnableMenuItem( IDD_SYNC_CLOCK, MF_GRAYED);
	pMenu->EnableMenuItem( IDD_UPDATA_FILE, MF_GRAYED);

	DWORD dw_AllWriteSize=0;
	for(int k=0;k<pGameInfoList->GetSize();k++)
	{
		if(pGameInfoList->GetAt(k)->uiWhere==1)
		{
			dw_AllWriteSize += pGameInfoList->GetAt(k)->flGameSize;
		}
	}
	DWORD dw_Range = dw_AllWriteSize/0x40000+dw_AllWriteSize/0x8000;
	m_WriteProgress.SetRange(0,dw_Range);
	DWORD dw_Pos=0;
	DWORD dw_percent=0;
	//m_strWritemLeft.Format("%s","准备写");
	//m_StaticWritePos.SetWindowText(m_strWritemLeft);
	m_WriteProgress.SetWindowText(m_gStrMsgWriting);
	WriteChinaNameBuffer(pGameInfoList);

	pLsInfo = pGameInfoList->GetAt(0);
	for(int i=0;i<iGame;i++)
	{
		pInfo = pGameInfoList->GetAt(i);
		if(pInfo->uiWhere==0)
		{
			StartAddress += pInfo->flGameSize ;
			SaverSize += pInfo->uiSaverSize ;
			pLsInfo = pInfo ;
			continue ;
		}
		if(pInfo->uiWhere == 1)
		{
			if(pInfo->bCheat && pInfo->bReset)
				FillGoldData(pInfo,rm);
			else
			{
				
				if(pInfo->bSaveNow && pInfo->pXCode)
				{
					
					fs.CartInit();
					if(!fs.Initialize())
					{
						fs.ClearCartInit();
						//格式化磁盘
						if(IDYES == MessageBox(m_gStrMsgFormatcation,NULL,MB_YESNO))
						{
						}
						return -1;
					}
					CString str;
					str =pInfo->strGameName;
					str.Remove(' ');
					str.Remove('-');
					str.Remove('_');
					str.Remove('+');
					str.Remove('!');
					str.Remove('.');
					str = str.Left(8);
					str = "\\saver\\"+str;
					str.Left(str.GetLength()-4);
					str+=".inv";
					int NandAdd = fs.fat_Instancesave_creat(str.GetBuffer(str.GetLength()),ATTR_ARCHIVE);
					if(NandAdd <= 0)
					{
						//fs.Uninitialize();
						fs.ClearCartInit();
						continue;
					}
					str.ReleaseBuffer(str.GetLength());
					fs.Uninitialize();
					fs.ClearCartInit();
					FillGoldDataAndSaveNow(pInfo,rm,NandAdd);					
				}
				else				
				{
					pInfo->pGameData[0xb5] = 0 ;
					pInfo->pGameData[0xb6] = 0 ;
					pInfo->pGameData[0xb7] = 0 ;
				}
			}
			if(pInfo->SavePatch == 0)
			{
				rm.SaverPatch(&(pInfo->pGameData),
							(ULONG*)&(pInfo->flGameSize),
							EZ2Cart,
							16,
							&(pInfo->uiSaverMode),
							0);
				rm.SpecialRomPatch(&(pInfo->pGameData),
									(ULONG*)&(pInfo->flGameSize),
									EZ2Cart,
									0);
			}
			else if(pInfo->SavePatch ==1)
			{
				rm.ApplyIPSPatch(pInfo->strIpsPatchFileName,
								(pInfo->pGameData),
								pInfo->flGameSize);
			}
			*((WORD *)(pInfo->pGameData+0xb8)) = pInfo->flGameSize >> 15 ;
			*((BYTE *)(pInfo->pGameData+0xbA)) = 0xFF&(pInfo->uiSaverSize>>12);
			pInfo->pGameData[0xbc] = pInfo->uiSaverMode + (pInfo->uiSaverMode<<4);
			if(pInfo->SavePatch!=2)
			{
				pInfo->pGameData[0xbc] &=0x0F;
				pInfo->pGameData[0xbc] |=0x10;
			}
			pInfo->pGameData[0xbe] = 0xCE ; //游戏链标志
			if(i==iGame-1)
				pInfo->pGameData[0xbe] = 0xCf ;
			rm.FillComplementCheck(pInfo->pGameData);
			m_pezVirtualBase->OpenCartDevice(hDev);
			m_pezVirtualBase->SetMapMode(0x0);
			m_pezVirtualBase->CartSetROMPage(hDev,0);
			m_pezVirtualBase->CartOpenFlashOP(hDev);
			m_pezVirtualBase->CartOpen(hDev);
			m_pezVirtualBase->NAND_ENABLE(hDev,0);
			m_pezVirtualBase->EXECFLH_ReadArray(hDev);
			m_pezVirtualBase->EXECFLH_ClearStatus(hDev);
			while(m_pezVirtualBase->EXECFLH_ReadStatus(hDev)!=0x8080){};
			m_pezVirtualBase->EXECFLH_ClearStatus(hDev);
			m_pezVirtualBase->EXECFLH_ReadArray(hDev);
			m_pezVirtualBase->EXECFLH_ClearStatus(hDev);
			int dd = (StartAddress+pInfo->flGameSize+0x3FFFF)/0x40000 ;
			dd = dd*0x40000 ;
			float progress = 0 ;
			for(j=StartAddress;j<dd;j+=0x40000)
			{
				status = 0x22 ;
				if((j==StartAddress)&&(j%0x40000))//忽略掉第一次erase ;
				{
					progress += (float)100*0x40000/pInfo->flGameSize ;
					continue ;
				}
				m_pezVirtualBase->EXECFLH_BlockErase(hDev,j>>17);
				dw_percent = (dw_Pos*100)/dw_Range;
				m_strWritemLeft.Format("%s  %d%%",m_gStrMsgErasing,dw_percent);
				dw_Pos++;
				m_WriteProgress.SetWindowText(m_strWritemLeft);
				m_WriteProgress.SetPos(dw_Pos);
			}
			if( j<0x2000000)
				m_pezVirtualBase->EXECFLH_BlockErase(hDev,j>>17);
			status = 0x0 ;
			while(m_pezVirtualBase->EXECFLH_ReadStatus(hDev)!=0x8080){}
			m_pezVirtualBase->NAND_ENABLE(hDev,0);
			m_pezVirtualBase->EXECFLH_ClearStatus(hDev);
			m_pezVirtualBase->EXECFLH_ReadArray(hDev);
			//写游戏
			progress = (float)100*32768/pInfo->flGameSize ;
			for(j=StartAddress;j<StartAddress+pInfo->flGameSize;j+=32768)
			{
				status = 0x33 ;
				if(j<0x40000 && (i!=0))
					j=0x40000 ; //跳过头文件
				m_pezVirtualBase->EXECFLH_ReadArray(hDev);
				m_pezVirtualBase->EXECFLH_Write(hDev,j>>1,pInfo->pGameData+j-StartAddress,32768);
				dw_percent = (dw_Pos*100)/dw_Range;
				if(dw_percent>=100)
					dw_percent=100;
				m_strWritemLeft.Format("%s%s",m_gStrMsgWriting,pInfo->strGameChinaName,dw_percent);
				str.Format("  %d%%",dw_percent);
				m_strWritemLeft+=str;
				dw_Pos++;
				m_WriteProgress.SetWindowText(m_strWritemLeft);
				m_WriteProgress.SetPos(dw_Pos);
			}
			status = 0x0 ;
			delete []pInfo->pGameData ;
			pInfo->pGameData = NULL ;
			StartAddress += pInfo->flGameSize ;
			SaverSize += pInfo->uiSaverSize ;
			m_PageFlash.m_ListCtrl.SetItemText(i,4,m_gStrMsgWrited);
			m_PageFlash.m_ListCtrl.Invalidate(TRUE);
			m_pezVirtualBase->EXECFLH_ReadArray(hDev);
			m_pezVirtualBase->CartClose(hDev);
			m_pezVirtualBase->CartCloseFlashOP(hDev);
			m_pezVirtualBase->CloseCartDevice(hDev);
		}
	}
	
	SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
	pMenu->EnableMenuItem( IDD_ADD_LOADER, MF_BYCOMMAND   );
	pMenu->EnableMenuItem( IDD_SYNC_CLOCK, MF_BYCOMMAND   );
	pMenu->EnableMenuItem( IDD_UPDATA_FILE, MF_BYCOMMAND   );

	m_WriteProgress.SetWindowText(m_gStrMsgWriteEnd);
	m_WriteProgress.SetPos(0);
	m_Tab.EnableWindow(TRUE);
	return 0;
}
DWORD CNew_EZManageDlg::FillGoldDataAndSaveNow(GameRomInfo *pInfo,CRomManager rm,int NandAdd)
{
	if(!pInfo)
		return 0;
#ifdef _DEBUG
	CFile file;
	file.Open(_T("D:\\0001\\goldgame_EZ3Begin.gba"),CFile::modeCreate|CFile::modeReadWrite);
	file.Write(pInfo->pGameData,pInfo->flGameSize);
	file.Close();
#endif //_DEBUG
	CEfaCheat efacheat;
	DWORD Totalsize=0;
	PGOLDCHEAT pTempGlodCheat,pGlodCheat;
	PGOLDCODEITEM pItem;
	DWORD TotalSize=0;
	BYTE *pCheatList = NULL;
	//计算金手指数据总共的大小
	for(int i=0;i<pInfo->CheatSize;i++)
	{
		pTempGlodCheat = &pInfo->pGlodCheat[i];
		pItem = pTempGlodCheat->pItem;
		Totalsize+=16;
		for(int k=0;k<pTempGlodCheat->SumofItem;k++)
		{
			if(pItem)
			{
				Totalsize+=16;
				Totalsize+=pItem->ItemSize*sizeof(GOLDDATA);
			}
			pItem++;
		}
	}
	PXCODE		pxcode = (PXCODE)pInfo->pXCode ;
	pxcode->dataSize = pInfo->CheatSize;
	PCODE		pcode = (PCODE)(pInfo->pXCode+24) ;
	DWORD BlockSize = 24+pxcode->CodeSize*sizeof(CODE)+pxcode->CodeSize*INS_SIZE+Totalsize;
	BlockSize = ((BlockSize+15)/16)*16;
	DWORD dw_off = FoundFreeSpaceFormRom(&(pInfo->pGameData),pInfo->flGameSize,BlockSize);
	if(dw_off ==-1)
	{
		BYTE *pTempBuffer = new BYTE[pInfo->flGameSize+((BlockSize+0x40000)/0x40000)*0x40000];
		memcpy(pTempBuffer,pInfo->pGameData,pInfo->flGameSize);
		delete []pInfo->pGameData;
		pInfo->pGameData = pTempBuffer;
		dw_off = pInfo->flGameSize;
		pInfo->flGameSize +=((BlockSize+0x40000)/0x40000)*0x40000;
	}
	BYTE *pOff = pInfo->pGameData+dw_off;

	DWORD xCodeSize = 24+pxcode->CodeSize*sizeof(CODE);
	memcpy(pOff,pInfo->pXCode,xCodeSize);
	DWORD ProSize = 0;
	DWORD Itemoff=0;
	char Totalname[12];
	memset(Totalname,0,12);
	char ItemName[14];
	memset(ItemName,0,14);
	
	for(i=0;i<pInfo->CheatSize;i++)
	{
		pGlodCheat = &pInfo->pGlodCheat[i];
		memcpy(pOff+xCodeSize+ProSize+Itemoff,pGlodCheat,16);
		Itemoff+=16;
		
		for(int k=0;k<pGlodCheat->SumofItem;k++)
		{
			memcpy(pOff+xCodeSize+ProSize+Itemoff,(BYTE *)&pGlodCheat->pItem[k],16);
			Itemoff+=16;
			GOLDDATA *pGoldData=NULL;
			for(int j=0;j<pGlodCheat->pItem->ItemSize;j++)
			{
				pGoldData = &((&pGlodCheat->pItem[k])->pGlodData[j]);
				memcpy(pOff+xCodeSize+ProSize+Itemoff,(BYTE *)pGoldData,sizeof(GOLDDATA));
				Itemoff+=sizeof(GOLDDATA);
			}
		}
			
	}
	if(pInfo->CheatSize>0)
	{
		pCheatList = new BYTE[Itemoff];
		memset(pCheatList,0,Itemoff);
		memcpy(pCheatList,pOff+xCodeSize,Itemoff);
	}
	efacheat.InstanceAddNewGame(pInfo->pGameData,pInfo->pGameData+dw_off,pInfo->pXCode,(BYTE *)pCheatList,Itemoff,NandAdd);
	if(pCheatList)
	{
		delete []pCheatList;
		pCheatList = NULL;
	}
	dw_off = dw_off>>4;
	memcpy((BYTE *)(&pInfo->pGameData[0xb5]),(BYTE *)(&dw_off),3);
	rm.FillComplementCheck(pInfo->pGameData);
	//把游戏数据写成文件,做测试
#ifdef _DEBUG
	file.Open(_T("D:\\0001\\goldgame_EZ3After.gba"),CFile::modeCreate|CFile::modeReadWrite);
	file.Write(pInfo->pGameData,pInfo->flGameSize);
	file.Close();
#endif //_DEBUG
	return dw_off;

}
DWORD CNew_EZManageDlg::FillGoldData(GameRomInfo *pInfo,CRomManager rm)
{
		//把游戏数据写成文件,做测试
	if(!pInfo)
		return 0;
#ifdef _DEBUG
	CFile file;
	file.Open(_T("D:\\0001\\goldgame_EZ3Begin.gba"),CFile::modeCreate|CFile::modeReadWrite);
	file.Write(pInfo->pGameData,pInfo->flGameSize);
	file.Close();
#endif //_DEBUG
	CEfaCheat efacheat;
	DWORD Totalsize=0;
	PGOLDCHEAT pTempGlodCheat,pGlodCheat;
	PGOLDCODEITEM pItem;
	DWORD TotalSize=0;
	BYTE *pCheatList = NULL;
	//计算金手指数据总共的大小
	for(int i=0;i<pInfo->CheatSize;i++)
	{
		pTempGlodCheat = &pInfo->pGlodCheat[i];
		pItem = pTempGlodCheat->pItem;
		Totalsize+=16;
		for(int k=0;k<pTempGlodCheat->SumofItem;k++)
		{
			if(pItem)
			{
				Totalsize+=16;
				Totalsize+=pItem->ItemSize*sizeof(GOLDDATA);
			}
			pItem++;
		}
	}
	PXCODE		pxcode = (PXCODE)pInfo->pXCode ;
	pxcode->dataSize = pInfo->CheatSize;
	PCODE		pcode = (PCODE)(pInfo->pXCode+24) ;
	DWORD BlockSize = 24+pxcode->CodeSize*sizeof(CODE)+pxcode->CodeSize*0x1e0+Totalsize;
	BlockSize = ((BlockSize+15)/16)*16;
	DWORD dw_off = FoundFreeSpaceFormRom(&(pInfo->pGameData),pInfo->flGameSize,BlockSize);
	if(dw_off ==-1)
	{
		BYTE *pTempBuffer = new BYTE[pInfo->flGameSize+((BlockSize+0x40000)/0x40000)*0x40000];
		memcpy(pTempBuffer,pInfo->pGameData,pInfo->flGameSize);
		delete []pInfo->pGameData;
		pInfo->pGameData = pTempBuffer;
		dw_off = pInfo->flGameSize;
		pInfo->flGameSize +=((BlockSize+0x40000)/0x40000)*0x40000;
	}
	BYTE *pOff = pInfo->pGameData+dw_off;

	DWORD xCodeSize = 24+pxcode->CodeSize*sizeof(CODE);
	memcpy(pOff,pInfo->pXCode,xCodeSize);
	DWORD ProSize = 0;
	DWORD Itemoff=0;
	char Totalname[12];
	memset(Totalname,0,12);
	char ItemName[14];
	memset(ItemName,0,14);
	
	for(i=0;i<pInfo->CheatSize;i++)
	{
		pGlodCheat = &pInfo->pGlodCheat[i];
		memcpy(pOff+xCodeSize+ProSize+Itemoff,pGlodCheat,16);
		Itemoff+=16;
		
		for(int k=0;k<pGlodCheat->SumofItem;k++)
		{
			memcpy(pOff+xCodeSize+ProSize+Itemoff,(BYTE *)&pGlodCheat->pItem[k],16);
			Itemoff+=16;
			GOLDDATA *pGoldData=NULL;
			for(int j=0;j<pGlodCheat->pItem->ItemSize;j++)
			{
				pGoldData = &((&pGlodCheat->pItem[k])->pGlodData[j]);
				memcpy(pOff+xCodeSize+ProSize+Itemoff,(BYTE *)pGoldData,sizeof(GOLDDATA));
				Itemoff+=sizeof(GOLDDATA);
			}
		}
			
	}
	if(pInfo->CheatSize>0)
	{
		pCheatList = new BYTE[Itemoff];
		memset(pCheatList,0,Itemoff);
		memcpy(pCheatList,pOff+xCodeSize,Itemoff);
	}
	efacheat.CheatAddNewGame(pInfo->pGameData,pInfo->pGameData+dw_off,pInfo->pXCode,(BYTE *)pCheatList,Itemoff);
	if(pCheatList)
	{
		delete []pCheatList;
		pCheatList = NULL;
	}
	dw_off = dw_off>>4;
	memcpy((BYTE *)(&pInfo->pGameData[0xb5]),(BYTE *)(&dw_off),3);
	rm.FillComplementCheck(pInfo->pGameData);
#ifdef _DEBUG
	file.Open(_T("D:\\0001\\goldgame_EZ3After.gba"),CFile::modeCreate|CFile::modeReadWrite);
	file.Write(pInfo->pGameData,pInfo->flGameSize);
	file.Close();
#endif //_DEBUG
	return dw_off;
}

DWORD CNew_EZManageDlg::FoundFreeSpaceFormRom(BYTE **pGameDataBegin,int &GameSize,DWORD Blocksize)
{
	BYTE *pNewGameData = NULL;
	if(!pGameDataBegin || Blocksize==0)
		return NULL;
	int dw_off=-1;
	Blocksize = ((Blocksize/16)+1)*16;
	BYTE *p=*pGameDataBegin+GameSize-Blocksize;
	BYTE *p2 = new BYTE[Blocksize];
	BYTE *p3 = new BYTE[Blocksize];
	memset(p3,0x00,Blocksize);
	memset(p2,0xFF,Blocksize);
	for(dw_off=GameSize-Blocksize;dw_off>0;dw_off-=Blocksize)
	{
		//判断p后的数据是不是全是0xFF
		if(memcmp(p,p2,Blocksize)==0 || memcmp(p,p3,Blocksize)==0)
		{
			delete []p2;
			delete []p3;
			return dw_off;
		}
		else
			p = p-Blocksize;
	}
	//下面是没有找到空间的处理
	if(dw_off<0)
	{
		pNewGameData = new BYTE[GameSize+0x20000];
		memset(pNewGameData,0xFF,GameSize+0x20000);
		memcpy(pNewGameData,*pGameDataBegin,GameSize);
		dw_off = GameSize;
		GameSize +=0x20000;
		delete [](*pGameDataBegin);
		*pGameDataBegin = pNewGameData;
	}
	delete []p2;
	delete []p3;
	return dw_off;
	/*
	if(!pGameDataBegin || Blocksize==0)
		return NULL;
	DWORD dw_off;
	dw_off = GameSize-Blocksize;
	BYTE *p=pGameDataBegin+GameSize-Blocksize;
	BYTE *p2 = new BYTE[Blocksize];
	BYTE *p3 = new BYTE[Blocksize];
	memset(p3,0x00,Blocksize);
	memset(p2,0xFF,Blocksize);
	for(int i=0;i<(GameSize-Blocksize);i+=0x10)
	{
		//判断p后的数据是不是全是0xFF
		if(memcmp(p,p2,Blocksize)==0 || memcmp(p,p3,Blocksize)==0)
		{
			delete []p2;
			delete []p3;
			return dw_off-i;
		}
		else
		{
			dw_off-=0x10;	
			p=p-0x10;
		}
	}
	delete []p2;
	delete []p3;
	return -1;
	*/
}

void CNew_EZManageDlg::WriteChinaNameBuffer(GAMEINFOLIST *pList)
{
	if(!pList)
		return;
	if(pList->GetSize()<=0)
		return;
	HANDLE hDev;
	GameRomInfo *pInfo = NULL;
	BYTE *pBuf = new BYTE[0x10000];
	memset(pBuf,0,0x10000);
	BYTE *pChinaNameBuf = &pBuf[0x8000];
	WORD loop = pList->GetSize();

	
	m_pezVirtualBase->OpenCartDevice(hDev);
	m_pezVirtualBase->SetMapMode(0x8000);
	m_pezVirtualBase->CartSetROMPage(hDev,0);
	m_pezVirtualBase->CartOpenFlashOP(hDev);
	m_pezVirtualBase->CartOpen(hDev);
	m_pezVirtualBase->LDRFLH_ReadArray(hDev);
	m_pezVirtualBase->LDRFLH_Read(hDev,0x10000,pBuf,0x10000);
	memset(pChinaNameBuf,0,48);
	char *sign= "EZ3-Name";
	memcpy(pChinaNameBuf,sign,8);
	*((WORD *)(&pChinaNameBuf[9])) = loop;
	for(int i=0;i<loop;i++)
	{
		pInfo = pList->GetAt(i);
		if(pInfo->strGameChinaName[0]!=0)
			memcpy(&pChinaNameBuf[(i+1)*48],pInfo->strGameChinaName,48);
		else
			memcpy(&pChinaNameBuf[(i+1)*48],pInfo->strGameName,48);
	}
	m_pezVirtualBase->LDRFLH_ClearStatus(hDev);
	m_pezVirtualBase->LDRFLH_ReadArray(hDev);
	m_pezVirtualBase->LDRFLH_BlockErase(hDev,2);
	m_pezVirtualBase->LDRFLH_ClearStatus(hDev);
	m_pezVirtualBase->LDRFLH_ReadArray(hDev);
	m_pezVirtualBase->LDRFLH_Write(hDev,0x10000,pBuf,0x10000);
	m_pezVirtualBase->CartClose(hDev);
	m_pezVirtualBase->CartCloseFlashOP(hDev);
	m_pezVirtualBase->CloseCartDevice(hDev);
	delete []pBuf;

}
void CNew_EZManageDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case IDENTIFY_CART_TIMER:
		AnalyzeCart();
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CNew_EZManageDlg::AnalyzeCart()
{
	BOOL fWriterConnected = FALSE , fCartInsert = FALSE ;
	EZ3CARTTYPE cartType = NON_EZ3Cart;
	HANDLE hDev;
	DWORD Nand_Id=0,Nor_Id=0,Ldr_Id=0;

	EnterCriticalSection(&g_statusCritical);
	fWriterConnected = g_IsEzWriterConnected ;
	fCartInsert = g_IscartConnected;
	cartType = g_CartType;
	LeaveCriticalSection(&g_statusCritical);

	if(fWriterConnected&&fCartInsert)//如果有写卡器同时有卡
	{
		if(cartType == NON_EZ3Cart)
		{
			EnterCriticalSection(&g_statusCritical);
			g_IsEzWriterConnected = FALSE ;
			g_IscartConnected = FALSE;
			g_CartType = NON_EZ3Cart ;
			LeaveCriticalSection(&g_statusCritical);
			m_Tab.EnableWindow(FALSE);
		}	
		else
		{
			m_pezVirtualBase->OpenCartDevice(hDev);
			if(hDev == INVALID_HANDLE_VALUE)
			{
				EnterCriticalSection(&g_statusCritical);
				g_IsEzWriterConnected = FALSE ;
				g_IscartConnected = FALSE;
				g_CartType = NON_EZ3Cart ;
				LeaveCriticalSection(&g_statusCritical);
				if(pGameInfoList->GetSize()>0)
				{
					ReMoveAllGameList();
					m_PageFlash.m_ListCtrl.DeleteAllItems();
					ShowRomUseInfo(TRUE);
					m_Tab.EnableWindow(FALSE);
				}
				return;
			}
			m_pezVirtualBase->NAND_ENABLE(hDev,1);
			m_pezVirtualBase->NAND_Reset(hDev);				
			Nand_Id = m_pezVirtualBase->NAND_ReadNANDID(hDev);	
			
			m_pezVirtualBase->SetMapMode(0x0);
			m_pezVirtualBase->CartSetROMPage(hDev,0);
			m_pezVirtualBase->CartOpenFlashOP(hDev);
			m_pezVirtualBase->CartOpen(hDev);
			m_pezVirtualBase->LDRFLH_ReadArray(hDev);
			Nor_Id = m_pezVirtualBase->EXECFLH_ReadID(hDev);
	
			m_pezVirtualBase->SetMapMode(0x8000);
			m_pezVirtualBase->CartSetROMPage(hDev,0);
			m_pezVirtualBase->LDRFLH_ReadArray(hDev);
			Ldr_Id = m_pezVirtualBase->LDRFLH_ReadID(hDev);
			m_pezVirtualBase->CartClose(hDev);
			m_pezVirtualBase->CartCloseFlashOP(hDev);
			m_pezVirtualBase->CloseCartDevice(hDev);		
			if(/*(Nand_Id == NAND_ID) &&(Nor_Id==NOR_ID) &&*/(Ldr_Id==LDR_ID1 || Ldr_Id==LDR_ID2)||Ldr_Id==LDR_ID3||Ldr_Id==LDR_ID4)
			{
				EnterCriticalSection(&g_statusCritical);
				g_IsEzWriterConnected = TRUE ;
				g_IscartConnected = TRUE;
				g_CartType = EZ3_Cart ;
				LeaveCriticalSection(&g_statusCritical);
				fs.SetOPType(OPNONE);
				m_WriteProgress.SetWindowText(" ");
			}
			else
			{
				EnterCriticalSection(&g_statusCritical);
				g_IsEzWriterConnected = FALSE ;
				g_IscartConnected = FALSE;
				g_CartType = NON_EZ3Cart ;
				LeaveCriticalSection(&g_statusCritical);
//				switch(m_Tab.GetCurSel())
//				{
//					case 0:
						if(pGameInfoList->GetSize()>0)
						{
							ReMoveAllGameList();
							m_PageFlash.m_ListCtrl.DeleteAllItems();
							ShowRomUseInfo(TRUE);
						}
//						break;
//					case 1:
						m_PageDisk.QuitDisk();
						ShowRomUseInfo(TRUE);
						m_bTimesNand = TRUE;
						m_bTimeNor = TRUE;
//						break;
//				}
				m_Tab.EnableWindow(FALSE);
				return;

			}
		}
	}
	else
	{
		if(m_pezVirtualBase ==NULL)
		{
			m_pezVirtualBase = new CEzFlashBase;
			m_pezVirtualBase->OpenCartDevice(hDev);
			if(hDev == INVALID_HANDLE_VALUE)
			{
				return ;
			}
			m_pezVirtualBase->SetMapMode(0x0);
			m_pezVirtualBase->CartSetROMPage(hDev,0);
			m_pezVirtualBase->CartOpenFlashOP(hDev);
			m_pezVirtualBase->CartOpen(hDev);
			m_pezVirtualBase->LDRFLH_ReadArray(hDev);
			Nor_Id = m_pezVirtualBase->TryID(hDev);
			m_pezVirtualBase->CartClose(hDev);
			m_pezVirtualBase->CartCloseFlashOP(hDev);
			m_pezVirtualBase->CloseCartDevice(hDev); 
			delete m_pezVirtualBase;
			m_pezVirtualBase = NULL;
			switch(Nor_Id)
			{
			case AMD_ID:
				m_pezVirtualBase = new CAmdEz3FlashBase;
				break;
			case FUJI_ID:
				m_pezVirtualBase = new CFujiEZ3Flash1g;
				break;
			default:
				m_pezVirtualBase = new CAmdEz3FlashBase;
			}
		}
		m_pezVirtualBase->OpenCartDevice(hDev);
		if(hDev == INVALID_HANDLE_VALUE)
		{
			if(pGameInfoList->GetSize()>0)
			{
				ReMoveAllGameList();
				m_PageFlash.m_ListCtrl.DeleteAllItems();
				ShowRomUseInfo(TRUE);
				m_Tab.EnableWindow(FALSE);
			}
			return;
		}
		m_pezVirtualBase->CartOpenFlashOP(hDev);
		m_pezVirtualBase->CartOpen(hDev);
		m_pezVirtualBase->CartSetROMPage(hDev,0x8000);
		m_pezVirtualBase->NAND_ENABLE(hDev,1);
		m_pezVirtualBase->NAND_Reset(hDev);
		Nand_Id = m_pezVirtualBase->NAND_ReadNANDID(hDev);

		if( (Nand_Id==0x15c1dcec) || (Nand_Id==0x15c1dcad)||(Nand_Id==0x9551d3ec))
		{
			g_dwNandSize = 128*1024*8;
			m_gStrdiskPage.Replace('1','8');
		}
		if((Nand_Id == 0x1580DCec)||(Nand_Id == 0x1580DCAD))
		{
			g_dwNandSize = 128*1024*4;
			m_gStrdiskPage.Replace('1','4');
		}
		if((Nand_Id == 0x1580DAEC)||(Nand_Id == 0x1580DAad))
		{
			g_dwNandSize = 128*1024*2;
			m_gStrdiskPage.Replace('1','2');
		}
		else if(Nand_Id == 0x1580F1AD || Nand_Id == 0x1580F1EC )
		{
			g_dwNandSize = 128*1024*1;
		}
		//修改Nand容量显示
		TCITEM tcItem;
		tcItem.mask = TCIF_TEXT;
		tcItem.pszText =m_gStrdiskPage.GetBuffer(m_gStrdiskPage.GetLength());
		m_Tab.SetItem(1,&tcItem);
		m_gStrdiskPage.ReleaseBuffer(m_gStrdiskPage.GetLength());

		m_pezVirtualBase->SetMapMode(0x0);
		m_pezVirtualBase->CartSetROMPage(hDev,0);
		m_pezVirtualBase->LDRFLH_ReadArray(hDev);
		Nor_Id = m_pezVirtualBase->EXECFLH_ReadID(hDev);
		m_pezVirtualBase->SetMapMode(0x8000);
		m_pezVirtualBase->CartSetROMPage(hDev,0);
		m_pezVirtualBase->WriteDevice(hDev,0xff0000,0xd2FF);
		m_pezVirtualBase->WriteDevice(hDev,0x000000,0x15FF);
		m_pezVirtualBase->WriteDevice(hDev,0x010000,0xd2FF);
		m_pezVirtualBase->WriteDevice(hDev,0x020000,0x15FF);
		m_pezVirtualBase->WriteDevice(hDev,0xe20000,0x55FF);
		m_pezVirtualBase->WriteDevice(hDev,0xfe0000,0x15FF);
		m_pezVirtualBase->LDRFLH_ReadArray(hDev);
		Ldr_Id = m_pezVirtualBase->LDRFLH_ReadID(hDev);
		//如果是第一次运行，检测是否有loader,如果没有Loader提示写Loader,
		BYTE *pLoaderBuffer = new BYTE[0x8000];
		//if(m_FirstRun)
		{
			m_pezVirtualBase->LDRFLH_ReadArray(hDev);
			m_pezVirtualBase->LDRFLH_Read(hDev,0,pLoaderBuffer,0x8000);		
		}
		m_pezVirtualBase->CartClose(hDev);
		m_pezVirtualBase->CartCloseFlashOP(hDev);
		m_pezVirtualBase->CloseCartDevice(hDev);
		if(Ldr_Id==LDR_ID1 || Ldr_Id==LDR_ID2 || Ldr_Id==LDR_ID3 ||Ldr_Id==LDR_ID4)
		{
			EnterCriticalSection(&g_statusCritical);
			g_IsEzWriterConnected = TRUE ;
			g_IscartConnected = TRUE;
			g_CartType = EZ3_Cart ;
			LeaveCriticalSection(&g_statusCritical);
			switch(m_Tab.GetCurSel())
			{
				case 0:
					ReadEZ3NorFlash();
					m_bTimeNor = FALSE;
					break;
				case 1:
					ReadNandFlash();
					m_bTimesNand = FALSE;
					break;
			}
			//如果没有Loader提示写Loader
			if((*(DWORD *)pLoaderBuffer)!= 0xea00002e)
			{
				//写Loader
				m_FirstRun = FALSE;
				if(IDYES == MessageBox(m_gStrMsgOSChange,NULL,MB_YESNO))
				{
					CDlgOSSelect DlgOSSelect;
					if(DlgOSSelect.DoModal() == IDOK)
					{
						m_bNdsLoader = DlgOSSelect.GetLoader();
						WriteLoader();
					}
					return;	
				}
			}
			else if(!bLoaderVersion(pLoaderBuffer))
			{
				//如果版本不相同//写Loader
				m_FirstRun = FALSE;
				//是否写Loader提示
				if(IDYES == MessageBox(m_gStrMsgOSChange,NULL,MB_YESNO))
				{
					m_bUpdtaLoader  = TRUE;
					WriteLoader();
					return;
				}
			}
			
		}
		/*
		else
		{
			EnterCriticalSection(&g_statusCritical);
			g_IsEzWriterConnected = FALSE ;
			g_IscartConnected = FALSE;
			g_CartType = NON_EZ3Cart ;
			LeaveCriticalSection(&g_statusCritical);
			m_Tab.EnableWindow(FALSE);
			return;
		}
		*/
	}
}

void CNew_EZManageDlg::ReadEZ3NorFlash()
{
	GameRomInfo *pInfo = NULL ;
	BYTE romReadHeader[0x100];
	HANDLE hDev ;
	EZ3CARTTYPE ttp ;
	CStringArray strArray ;
	XCODE_ARRAY xcodeArray;
	GOLD_DATA_ARRAY golddArray;
	DWORD StartAddr = 0 , dwRomSize = 0 ;
	CRomManager rmg ;
	int i=0;

	EnterCriticalSection(&g_statusCritical);
	ttp = g_CartType ;
	LeaveCriticalSection(&g_statusCritical);

	m_Tab.EnableWindow(TRUE);
	ReMoveAllGameList();
	KillTimer(IDENTIFY_CART_TIMER);
	m_pezVirtualBase->OpenCartDevice(hDev);
	m_pezVirtualBase->SetMapMode(0x0);
	m_pezVirtualBase->CartSetROMPage(hDev,0);
	m_pezVirtualBase->CartOpen(hDev);
	m_pezVirtualBase->EXECFLH_ReadArray(hDev);
	m_pezVirtualBase->EXECFLH_Read(hDev,0,romReadHeader,0x100);
	if(romReadHeader[0xbe]==0xCE || romReadHeader[0xbe]==0xCF)//后面还有游戏
	{
		if(rmg.HeaderValid(romReadHeader))
		{
			pInfo = new GameRomInfo;
			SetROMInfo(pInfo,
						pGameInfoList->GetSize(),
						0,
						(SAVERTYPE)romReadHeader[0xbc],
						(romReadHeader[0xba]<<12),
						(*(WORD *)&romReadHeader[0xb8])<<15,
						"",
						"",
						"",
						&romReadHeader[0xA0],
						BIT7(romReadHeader[0xBB]),
						BIT2(romReadHeader[0xB5]),
						BIT7(romReadHeader[0xBB]),
						romReadHeader[0xB5]&0x3,
					   	0,
						0,
						(PGOLDCHEAT)0);
			pGameInfoList->Add(pInfo);
		}
		StartAddr = pInfo->flGameSize ;
		while(romReadHeader[0xbe]==0xCE || romReadHeader[0xbe]==0xCF)
		{
			memset(&romReadHeader,0,0x100);
			m_pezVirtualBase->EXECFLH_ReadArray(hDev);
			m_pezVirtualBase->EXECFLH_Read(hDev,StartAddr>>1,romReadHeader,0x100);
			if(rmg.HeaderValid(romReadHeader))
			{
					pInfo = new GameRomInfo ;
					SetROMInfo(pInfo,
								pGameInfoList->GetSize(),
								0,
								(SAVERTYPE)romReadHeader[0xbc],
								(ttp == EZ1OLDWHITE)?(romReadHeader[0xba]<<14):(romReadHeader[0xba]<<12),
								(*(WORD *)&romReadHeader[0xb8])<<15,
								"",
								"",
								"",
								&romReadHeader[0xA0],
								BIT7(romReadHeader[0xBB]),
								BIT2(romReadHeader[0xB5]),
								BIT7(romReadHeader[0xBB]),
								romReadHeader[0xB5]&0x3,
								0,
								0,
								(PGOLDCHEAT)0);
					pGameInfoList->Add(pInfo);
					StartAddr += pInfo->flGameSize ;
					if(StartAddr >=0x2000000)
						break;
			}
		}
	}
	m_pezVirtualBase->CartCloseFlashOP(hDev);
	m_pezVirtualBase->CartClose(hDev);
	m_pezVirtualBase->CloseCartDevice(hDev);
	BYTE *pBlockBuf = GetChinaNameBuffer();
	if(pBlockBuf)
	{
		BYTE *pChinaNameBuf = &pBlockBuf[0x8000];
//		char s[8];
		char *sign = "EZ3-Name";
		if(memcmp(pChinaNameBuf,sign,8) == 0)
		{
			for(i=0;i<pGameInfoList->GetSize();i++)
			{
				pInfo = pGameInfoList->GetAt(i);
				memcpy(pInfo->strGameChinaName,&pChinaNameBuf[(i+1)*48],48);
			}
		}
		delete []pBlockBuf;
	}
	for(i=0;i<pGameInfoList->GetSize();i++)
	{
		AddItemToNorList(pGameInfoList->GetAt(i),i);
	}
	ShowRomUseInfo();
	SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
}

BOOL CNew_EZManageDlg::SetROMInfo(GameRomInfo *pInfo ,
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
								)
{
	pInfo->uiIndex  = uiIndex ;		//
	pInfo->uiWhere = uiWhere;		//
	pInfo->uiSaverMode = uiSaverMode ;	//
	pInfo->uiSaverSize = uiSaverSize ;	//
	pInfo->flGameSize =flGameSize ;	//
	memset(pInfo->strGameWhere,0,256);
	memset(pInfo->strGameEnglishName,0,32);
	memset(pInfo->strGameChinaName,0,48);
	memset(pInfo->strGameName,0,12);
	memset(pInfo->strIpsPatchFileName,0,256);
	sprintf(pInfo->strGameWhere,strGameWhere); 
	sprintf(pInfo->strGameEnglishName , strGameEnglishName );
	sprintf(pInfo->strGameChinaName,strGameChinaName);
	memcpy(pInfo->strGameName,strGameName,12);
	pInfo->bReset = bReset;
	pInfo->bBoma = bBoma;
	pInfo->bCheat = bCheat ;
	pInfo->SavePatch = bSavePatch ;
	pInfo->pGameData = pGameData ;
	pInfo->pXCode = pXCode;
	pInfo->pGlodCheat= pGoldData ;
	return TRUE ;
}

/**********************************************************************/
/*	中文名字是这样放的
	在32M的Loader flash,从0开始偏移160K Byte处开始,大小是0x10
	K Word.每个中文名占48 个Byte.第一个48 Byte:标识是EZ3-Name,接着一个WORD
	表示个数.以后每48Byte表示一个游戏的中文名或英文名

*/
/**********************************************************************/
BYTE *CNew_EZManageDlg::GetChinaNameBuffer()
{

	HANDLE hDev;
	BYTE *pChinaNameBuf = new BYTE[0x10000];
	//中文名字从pChinaNameBuf开始,偏移0x8000;
	m_pezVirtualBase->OpenCartDevice(hDev);
	m_pezVirtualBase->SetMapMode(0x8000);
	m_pezVirtualBase->CartSetROMPage(hDev,0);
	m_pezVirtualBase->CartOpenFlashOP(hDev);
	m_pezVirtualBase->CartOpen(hDev);
	m_pezVirtualBase->LDRFLH_ReadArray(hDev);
	m_pezVirtualBase->LDRFLH_Read(hDev,0x10000,pChinaNameBuf,0x10000);
	m_pezVirtualBase->CartClose(hDev);
	m_pezVirtualBase->CartCloseFlashOP(hDev);
	m_pezVirtualBase->CloseCartDevice(hDev);
	return pChinaNameBuf;
}

void CNew_EZManageDlg::OnUpdateAddLoader(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_Tab.IsWindowEnabled())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}
LRESULT CNew_EZManageDlg::OnFormatNand(WPARAM wparam, LPARAM lparam)	
{
	CDlgNandFormat DlgNandFormat;
	int Ret = DlgNandFormat.DoModal();
	if(DlgNandFormat.m_bFormat== TRUE)
	{
	}
	return 1;
}
LRESULT CNew_EZManageDlg::OnPreFolder(WPARAM wparam, LPARAM lparam)	
{
	if(m_PageDisk.m_strNandDiskPath == "\\")
	{
		AfxMessageBox(m_gStrMsgRootDir);
		return -1;
	}
	if(m_PageDisk.m_strNandDiskPath.Right(1) == "\\")
	{
		m_PageDisk.m_strNandDiskPath = m_PageDisk.m_strNandDiskPath.Left(m_PageDisk.m_strNandDiskPath.GetLength()-1);
		int len = m_PageDisk.m_strNandDiskPath.ReverseFind('\\');
		m_PageDisk.m_strNandDiskPath = m_PageDisk.m_strNandDiskPath.Left(len);
	}
	typedef CArray<FILE_ATTRIBUTE,FILE_ATTRIBUTE> FILELIST;
	char name[256];
	CString strItemTextName,strItemText;
	CString strPath,str;
	FILE_ATTRIBUTE file_arribute;
	FILELIST file_list;
	char *pStr;
	_STAT stat;
	CStringArray arr;
	KillTimer(IDENTIFY_CART_TIMER);
	m_Tab.EnableWindow(FALSE);
	strPath = m_PageDisk.m_strNandDiskPath;
	if(strPath.GetLength()<=0)
		strPath = "\\";
	m_PageDisk.m_strNandDiskPath +="\\";
	m_PageDisk.m_DiskListCtrl.DeleteAllItems();
	ShowNandUseInfo(NAND_TOTAL_SIZE,g_dwNandFreeSpace);
	fs.CartInit();
	if(!fs.Initialize())
	{
		SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
		m_Tab.EnableWindow(TRUE);
		return -1;
	}
	int ret = fs.fat_getfirst(strPath,name);
	while(!ret)
	{
		str = name;
		str.Insert(0,"\\");
		if(strPath != "\\")
			str = strPath+_T("\\")+name;
		pStr = str.GetBuffer(str.GetLength());
		fs.fat_get_stat(pStr,&stat);
		str.ReleaseBuffer(str.GetLength());
		//
		//设置文件名字
		memcpy(file_arribute.f_name,name,32);
		//设置文件大小
		file_arribute.f_size = stat.FileSize;
		//设置文件类型
		//file_arribute.f_type = stat.	
		//设置文件日期
		file_arribute.filetime.wYear = ((stat.WrtDate&0xFE00)>>9)+1980;
		file_arribute.filetime.wMonth = (stat.WrtDate&0x1E0)>>5;
		file_arribute.filetime.wDay = (stat.WrtDate & 0x7E);
		file_arribute.filetime.wHour = (stat.WrtTime & 0xF800)>>11;
		file_arribute.filetime.wMinute =(stat.WrtTime & 0x7E0)>>5;
		//设置文件属性
		file_arribute.f_attribute = stat.Attr;
		//把读出的Nand flash的数据添加到数组
		file_list.Add(file_arribute);
		//pMainView->AddItemToListCtrl(file_arribute);
		//
		ret = fs.fat_getnext(name);
	}
	for(int i=0;i<file_list.GetSize();i++)
	{
		if(file_list.GetAt(i).f_attribute == 0x10)//目录
		{
			m_PageDisk.AddItemToListCtrl(file_list.GetAt(i));
		}
	}
	for(i=0;i<file_list.GetSize();i++)
	{
		if(file_list.GetAt(i).f_attribute == 0x20)//文件
		{
			m_PageDisk.AddItemToListCtrl(file_list.GetAt(i));
		}
	}
	fs.ClearCartInit();

	SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
	m_Tab.EnableWindow(TRUE);
	return 1;
}
LRESULT CNew_EZManageDlg::OnCreateFolder(WPARAM wparam, LPARAM lparam)	
{
	pThreadNandCreateDir = AfxBeginThread(TRThreadNandCreateDir,
								this);
	
	return 1;
}

UINT CNew_EZManageDlg::TRThreadNandCreateDir( LPVOID pParam )
{
	CNew_EZManageDlg  *pView = (CNew_EZManageDlg*)pParam ;
	return pView->TRThreadProcessNandCreateDir(pView);
}
UINT CNew_EZManageDlg::TRThreadProcessNandCreateDir(CNew_EZManageDlg *pThis)
{
	
	CString  strFoldName,str;
	UINT index =::GetPrivateProfileInt("Folder Index","index",0,m_strWindowsDir+("\\temp\\conf.ini"));
	strFoldName.Format("%s%d","folder",index);
	while(m_PageDisk.CheckSameName(strFoldName))
	{
		index++;
		strFoldName.Format("%s%d","folder",index);
		str.Format("%d",index);
		::WritePrivateProfileString("folder Index","index",str,m_strWindowsDir+("\\temp\\conf.ini"));
	}
	m_Tab.EnableWindow(FALSE);	
	KillTimer(IDENTIFY_CART_TIMER);
	CString strPath = m_PageDisk.m_strNandDiskPath +strFoldName;
	char *pDir = strPath.GetBuffer(strPath.GetLength());
	strPath.ReleaseBuffer(strPath.GetLength());
	m_WriteProgress.SetRange(0,10);
	m_WriteProgress.SetWindowText(m_gStrMsgCreatingFolder);
	m_WriteProgress.SetPos(0);
	fs.CartInit();
	m_WriteProgress.SetPos(2);
	if(!fs.Initialize())
	{
		fs.ClearCartInit();
		//格式化磁盘
		if(IDYES == MessageBox(m_gStrMsgFormatcation,NULL,MB_YESNO))
		{
			CDlgNandFormat DlgNandFormat;
			DlgNandFormat.DoModal();
		}
	}
	m_WriteProgress.SetPos(4);
	fs.SetOPType(OPNONE);
	fs.fat_mkdir(pDir);
	m_WriteProgress.SetPos(6);
	fs.Uninitialize();
	m_WriteProgress.SetPos(8);
	fs.ClearCartInit();
	m_WriteProgress.SetPos(10);

	FILE_ATTRIBUTE file_attribute;
	memset(file_attribute.f_name,0,32);
	memcpy(file_attribute.f_name,strFoldName,strFoldName.GetLength());
	file_attribute.f_attribute = 0x10;
	GetLocalTime (&file_attribute.filetime);
	m_PageDisk.AddItemToListCtrl(file_attribute);

	SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
	m_Tab.EnableWindow(TRUE);	
	str.Format("%d",index);
	::WritePrivateProfileString("folder Index","index",str,m_strWindowsDir+("\\temp\\conf.ini"));
	m_WriteProgress.SetPos(0);
	m_WriteProgress.SetWindowText(m_gStrMsgCreateFolderOK);
	return 1;
}

LRESULT CNew_EZManageDlg::OnRemoveFolder(WPARAM wparam, LPARAM lparam)	
{
	CStringArray *pArray = m_PageDisk.GetSelectFileName(TRUE);
	if(pArray->GetSize()<=0)
		return 0;
	if(MessageBox(m_gStrMsgIsDelFile+"?",m_gStrMsgIsDelFile,MB_YESNO) == IDNO)
	{
		return -1;
	}
	pThreadNandDelDir = AfxBeginThread(TRThreadNandDelDir,
								this);
	return 1;
}
UINT CNew_EZManageDlg::TRThreadNandDelDir( LPVOID pParam )
{
	CNew_EZManageDlg  *pView = (CNew_EZManageDlg*)pParam ;
	return pView->TRThreadProcessNandDelDir(pView);
}
UINT CNew_EZManageDlg::TRThreadProcessNandDelDir(CNew_EZManageDlg *pThis)
{
	CString strMsg,strItemText;
	CStringArray *pArray = m_PageDisk.GetSelectFileName(TRUE);
	if(pArray->GetSize()<=0)
		return 0;
	char name[256];
	m_Tab.EnableWindow(FALSE);	
	KillTimer(IDENTIFY_CART_TIMER);
	int iSize = pArray->GetSize();
	m_WriteProgress.SetRange(0,iSize);
	m_WriteProgress.SetPos(0);
	strMsg = m_gStrMsgDeletingFolder;

	m_WriteProgress.SetWindowText(strMsg);
	fs.CartInit();
	if(!fs.Initialize())
	{
		fs.ClearCartInit();
		//格式化磁盘
		if(IDYES == MessageBox(m_gStrMsgFormatcation,NULL,MB_YESNO))
		{
			CDlgNandFormat DlgNandFormat;
			DlgNandFormat.DoModal();
		}
	}
	CString strPath;
	int ret ;
	int Counter;
	CString str;
	_STAT stat;
	BOOL  bDel=TRUE;
	for(UINT i=0;i<iSize;i++)
	{
		strItemText = pArray->GetAt(i);
		strPath = m_PageDisk.m_strNandDiskPath+strItemText;
		ret = fs.fat_getfirst(strPath,name);
		Counter=0;
		while(!ret)
		{
			str = name;
			str.Insert(0,"\\");
			fs.fat_get_stat((LPCTSTR)str,&stat);
			if(str == "\\." || str=="\\..")
			{
			}
			else
			{
				Counter++;
			}
			ret = fs.fat_getnext(name);
		}
		if(Counter>0)
		{
			bDel = FALSE;
			AfxMessageBox("Current directory is not empty,can not delete!");
		}
	}	
	if(bDel)
	{
		for(i=0;i<iSize;i++)
		{
			fs.SetOPType(DELDIR);
			strItemText = pArray->GetAt(i);
			strMsg += strItemText +("...");
			fs.fat_rmdir(m_PageDisk.m_strNandDiskPath+strItemText);
			strMsg = m_gStrMsgDeletingFolder;
			m_WriteProgress.SetPos(i);
			m_WriteProgress.SetWindowText(strMsg);
			m_PageDisk.RemoveSlectItem(strItemText);
		}
	}
	fs.Uninitialize();
	fs.ClearCartInit();
	m_WriteProgress.SetPos(0);
	m_WriteProgress.SetWindowText(m_gStrMsgDelFolderEnd);
	SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
	m_Tab.EnableWindow(TRUE);	
	return 1;
}

LRESULT CNew_EZManageDlg::OnBackupFolder(WPARAM wparam, LPARAM lparam)
{
	pThreadNandCreateDir = AfxBeginThread(TRThreadNandBackupSave,
								this);
	return 1;
}

UINT CNew_EZManageDlg::TRThreadNandBackupSave( LPVOID pParam )
{
	CNew_EZManageDlg  *pView = (CNew_EZManageDlg*)pParam ;
	return pView->TRThreadProcessBackupSave(pView);
}

UINT CNew_EZManageDlg::TRThreadProcessBackupSave(CNew_EZManageDlg *pThis)
{
	KillTimer(IDENTIFY_CART_TIMER);
	m_Tab.EnableWindow(FALSE);
	//读出\save下所有的save文件
	typedef CArray<FILE_ATTRIBUTE,FILE_ATTRIBUTE> FILELIST;	
	FILELIST file_list;
	_STAT stat;
	char name[16] ;
	char *pStr;
	CString str;
	fs.CartInit();
	CStringArray strArray;
	strArray.RemoveAll();
	if(!fs.Initialize())
	{
		return 0;
	}
	g_dwNandFreeSpace = fs.GetFreeNandSpace();
	int ret = fs.fat_getfirst("\\saver",name);
	while(!ret)
	{
		str = name;
		str.Insert(0,"\\");
		pStr = str.GetBuffer(str.GetLength());
		fs.fat_get_stat(pStr,&stat);
		str.ReleaseBuffer(str.GetLength());		
		if(str == "\\." || str=="\\..")
		{
		}
		else
		{
			//str.Remove('\\');
			strArray.Add(str);
		}
		ret = fs.fat_getnext(name);
	}
//	fs.Uninitialize();
//	fs.ClearCartInit();
	//开始备份文件
	
	
	char *pFileData;
	CString strPathName;
//	fs.CartInit();
//	if(!fs.Initialize())
//	{
//		return 0;
//	}
	PostMessage(WM_NAND_WRITE_BEGIN_MESSAGE,5,0);
	for(int i=0;i<strArray.GetSize();i++)
	{
		strPathName = strArray.GetAt(i);
		strPathName ="\\saver"+strPathName;
		pStr = strPathName.GetBuffer(strPathName.GetLength());
		fs.fat_get_stat(pStr,&stat);
		DWORD dw_FileSize = stat.FileSize;
		if(stat.Attr != 0x20)
		{
			fs.Uninitialize();
			fs.ClearCartInit();
			continue;
		}
		int h = fs.fat_open(pStr);
		pFileData = new char[dw_FileSize];
		memset(pFileData,0,dw_FileSize);
		fs.SetOPType(READFILE);
		fs.SetMessageHandle(this->m_hWnd);
		m_WriteProgress.SetRange(0,dw_FileSize/2048+1);
		fs.fat_read(h,(char*)pFileData,dw_FileSize);
		fs.fat_close(h);
		strPathName.ReleaseBuffer(strPathName.GetLength());
		//写成文件
		CFile file;
		CString tests = m_strWorkPath+strPathName;
		file.Open(m_strWorkPath+strPathName,CFile::modeCreate|CFile::modeReadWrite);
		file.Write(pFileData,dw_FileSize);
		file.Close();
		delete []pFileData;
		m_WriteProgress.SetWindowText(m_gStrMsgReadFileEnd);
		m_WriteProgress.SetPos(0);
	}
	fs.Uninitialize();
	fs.ClearCartInit();

	SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
	m_Tab.EnableWindow(TRUE);
	m_strBackfileSaver.Empty();

	return 1;
}

LRESULT CNew_EZManageDlg::OnReadFile(WPARAM wparam, LPARAM lparam)
{
	if(m_PageDisk.m_DiskListCtrl.GetSelectedCount()<=0)
	{
		MessageBox("请选择读文件",NULL,MB_ICONWARNING);
		return -1;
	}
	int nItem = m_PageDisk.m_DiskListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString strItemText = m_PageDisk.m_DiskListCtrl.GetItemText(nItem,0);
	CFileDialog backdlg(FALSE,
						" ",
						strItemText,
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						"EZ3 Saver files(*.*)|*.*||",
						this);
	if(IDOK == backdlg.DoModal())
	{
		m_strBackfileSaver = backdlg.GetPathName();
	}
	else
	{
		return 0;
	}
	pThreadNandCreateDir = AfxBeginThread(TRThreadNandReadFile,
								this);
	return 1;
}
UINT CNew_EZManageDlg::TRThreadNandReadFile( LPVOID pParam )
{
	CNew_EZManageDlg  *pView = (CNew_EZManageDlg*)pParam ;
	return pView->TRThreadProcessNandReadFile(pView);
}

UINT CNew_EZManageDlg::TRThreadProcessNandReadFile(CNew_EZManageDlg *pThis)
{
	KillTimer(IDENTIFY_CART_TIMER);
	m_Tab.EnableWindow(FALSE);
	PostMessage(WM_NAND_WRITE_BEGIN_MESSAGE,5,0);
	_STAT stat;
	char *pStr;
	char *pFileData;
	CString strPathName;
	int nItem = m_PageDisk.m_DiskListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString strItemText = m_PageDisk.m_DiskListCtrl.GetItemText(nItem,0);
	fs.CartInit();
	if(!fs.Initialize())
	{
		fs.ClearCartInit();
		//格式化磁盘
		if(IDYES == MessageBox(m_gStrMsgFormatcation,NULL,MB_YESNO))
		{
	//		CDlgNandFormat DlgNandFormat;
	//		DlgNandFormat.DoModal();
		}
		return 0;
	}
	CString strPath = m_PageDisk.m_strNandDiskPath;
	strPathName = strPath+strItemText;
	pStr = strPathName.GetBuffer(strPathName.GetLength());
	fs.fat_get_stat(pStr,&stat);
	DWORD dw_FileSize = stat.FileSize;
	if(stat.Attr != 0x20)
	{
		return -1;
	}
	int h = fs.fat_open(pStr);
	pFileData = new char[dw_FileSize];
	memset(pFileData,0,dw_FileSize);
	fs.SetOPType(READFILE);
	fs.SetMessageHandle(this->m_hWnd);
	m_WriteProgress.SetRange(0,dw_FileSize/2048+1);
	fs.fat_read(h,(char*)pFileData,dw_FileSize);
	fs.fat_close(h);
	fs.Uninitialize();
	fs.ClearCartInit();
	strPathName.ReleaseBuffer(strPathName.GetLength());
	//写成文件
	CFile file;
	file.Open(m_strBackfileSaver,CFile::modeCreate|CFile::modeReadWrite);
	file.Write(pFileData,dw_FileSize);
	file.Close();
	delete []pFileData;
	m_WriteProgress.SetWindowText(m_gStrMsgReadFileEnd);
	m_WriteProgress.SetPos(0);
	SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
	m_Tab.EnableWindow(TRUE);
	m_strBackfileSaver.Empty();
	return 1;
}
LRESULT CNew_EZManageDlg::OnDeleteFile(WPARAM wparam, LPARAM lparam)
{
	CString strPath,str,strSelectFileName,strTemp;
	if(m_PageDisk.m_DiskListCtrl.GetSelectedCount()<=0)
	{
		MessageBox(m_gStrMsgSelectFile,NULL,MB_ICONWARNING);
		return -1;
	}
	CStringArray *pArray = m_PageDisk.GetSelectFileName(FALSE);
	if(pArray == NULL || pArray->GetSize()<=0)
	{
		return -1;
	}
	strPath = m_PageDisk.m_strNandDiskPath;
	strSelectFileName = pArray->GetAt(0);
	if(MessageBox(m_gStrMsgIsDelFile+str+"?",m_gStrMsgIsDelFile,MB_YESNO) == IDNO)
	{
		return -1;
	}
	pThreadNandDelFile = AfxBeginThread(TRThreadNandDelFile,
								this);
	return 1;
}
UINT CNew_EZManageDlg::TRThreadNandDelFile( LPVOID pParam )
{
	CNew_EZManageDlg  *pView = (CNew_EZManageDlg*)pParam ;
	return pView->TRThreadProcessNandDelFile(pView);
}
UINT CNew_EZManageDlg::TRThreadProcessNandDelFile(CNew_EZManageDlg *pThis)
{
	CString strPath,str,strSelectFileName,strTemp;
	KillTimer(IDENTIFY_CART_TIMER);
	m_Tab.EnableWindow(FALSE);
	CStringArray *pArray = m_PageDisk.GetSelectFileName(FALSE);
	if(pArray == NULL || pArray->GetSize()<=0)
	{
		SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
		m_Tab.EnableWindow(TRUE);
		return 0;
	}
	strPath = m_PageDisk.m_strNandDiskPath;
	m_WriteProgress.SetRange(0,10);
	m_WriteProgress.SetWindowText(m_gStrMsgDeletingFile);
	strSelectFileName = pArray->GetAt(0);
	//str = strPath +str;
	
	fs.CartInit();
	if(!fs.Initialize())
	{
		fs.ClearCartInit();
		//格式化磁盘
		if(IDYES == MessageBox(m_gStrMsgFormatcation,NULL,MB_YESNO))
		{
			CDlgNandFormat DlgNandFormat;
			DlgNandFormat.DoModal();
		}
	}
	for(UINT i=0;i<pArray->GetSize();i++)
	{
		strSelectFileName = pArray->GetAt(i);
		strTemp = m_gStrMsgDeletingFile +strSelectFileName;
		m_WriteProgress.SetWindowText(strTemp);
		str = strPath+strSelectFileName;
		PostMessage(WM_NAND_WRITE_BEGIN_MESSAGE,2,0);
		fs.SetOPType(DELFILE);
		_STAT stat;
		fs.fat_get_stat(str.GetBuffer(str.GetLength()),&stat);
		str.ReleaseBuffer(str.GetLength());
		g_dwNandFreeSpace = fs.GetFreeNandSpace();
		g_dwNandFreeSpace +=stat.FileSize/1024;
		ShowNandUseInfo(NAND_TOTAL_SIZE,g_dwNandFreeSpace);
		m_WriteProgress.SetPos(2);
		fs.fat_remove(str);
		m_WriteProgress.SetPos(6);
	
		m_WriteProgress.SetPos(9);
		PostMessage(WM_NAND_WRITE_END_MESSAGE,0,0);
		m_PageDisk.RemoveSlectItem(strSelectFileName);
	}
	fs.Uninitialize();
	fs.ClearCartInit();
	m_WriteProgress.SetWindowText(m_gStrMsgDleFileOK);
	SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
	m_Tab.EnableWindow(TRUE);
	return 1;
}
BOOL CNew_EZManageDlg::Compress_LZ77_DosCommand(CString strSourceFile,CString strTargetFile)
{
	if(strSourceFile.IsEmpty() || strTargetFile.IsEmpty())
		return FALSE;
	BYTE *pbuf = NULL;
	BYTE *pWrite = NULL;
	CFile fl;
	DWORD len;
	short block;
	int first,compsize,totalsize=0;
	DWORD  pos[256]; //压缩后的位置。
	BYTE *pcomp = new BYTE[0x40000];
	CLz77 zz;
	CString CommandLine ;
	PROCESS_INFORMATION pcInfo ;
	STARTUPINFO			startInfo ;
	BOOL fSuc = NULL ;
	CString strPath  ;
	DWORD result = 0;
	CFile file;
	CString strTemp,str,strMsg;
	int dw_RangeLow,dw_RangeHigh;
//	DWORD dw_percent ;

	strMsg.Format("%s  %d%%",m_gStrMsgCompressing,0);

	//读文件数据,调整为16的倍数
	fl.Open(strSourceFile,CFile::modeRead);
	len = fl.GetLength();
	block = (len + 0x1FFFF )/0x20000 ;
	len = block*0x20000 ;
	pbuf = new BYTE[len] ;
	memset(pbuf,0,len);
	fl.Read(pbuf,fl.GetLength());
	fl.Close();
	//压缩数据
	first = (4+4+4*block+15)/16 * 16 ;
	pWrite = new BYTE[len + first];
	compsize = 0 ;
	totalsize = first ;
	m_WriteProgress.SetRange(0,block);
	m_WriteProgress.GetRange(dw_RangeLow,dw_RangeHigh);
	
	for(int i=0;i<block;i++)//做每一块的循环。
	{
		//compsize = zz.Encode(pbuf+0x20000*i,0x20000,pcomp,0x20000);
		//写文件
		DWORD dw_percent = (i*100)/dw_RangeHigh;
		if(dw_percent>=98)
		{
			dw_percent = 98;
		}
		strMsg.Format("%s  %d%%",m_gStrMsgCompressing,dw_percent);
		m_WriteProgress.SetWindowText(strMsg);
		m_WriteProgress.SetPos(i);
		strTemp.Format("%s%d","\\temp\\tempfile",i);
		strTemp+=".bin";
		strTemp =m_strWindowsDir +strTemp;
		fl.Open(strTemp,CFile::modeCreate|CFile::modeReadWrite);
		fl.Write(pbuf+0x20000*i,0x20000);
		fl.Close();

		//调用DOS命令
		
		CString strSysPath = m_strWorkPath ;
		strSysPath += _T("\\sysbin\\GBACrusherCL.exe");
		if(strSysPath.Find(' ')>0)
			strSysPath = '"'+strSysPath+'"';
		strSysPath += _T(" -l ");
		if(strTemp.Find(' ')>0)
			strTemp = '"'+strTemp+'"';
		CommandLine  = strSysPath + strTemp;
		//CommandLine = GetSystemPath() + _T("\\sysbin\\GBACrusherCL.exe -l ")+ strTemp;

		str = strTemp;
		//str.Remove('"');
//		str.Insert(str.GetLength()-4,"_LZ");
		str.Replace(".bin","_ELZ.bin");
		//if(str.Find(' ')>0)
		//	str = '"'+str+'"';
		CommandLine =  CommandLine+ _T(" -O ")+str;
		memset(&pcInfo,0,sizeof(pcInfo));
		memset(&startInfo,0,sizeof(startInfo));
		startInfo.cb = sizeof(startInfo);
		startInfo.dwFlags =STARTF_USESHOWWINDOW;
		startInfo.wShowWindow = SW_HIDE ;
//		AfxMessageBox(CommandLine);
//		return 0;
		fSuc = CreateProcess(NULL,CommandLine.GetBuffer(CommandLine.GetLength()),NULL,NULL,FALSE,
						CREATE_NO_WINDOW|CREATE_SUSPENDED,
						NULL,
						NULL,
						&startInfo,&pcInfo);
		CommandLine.ReleaseBuffer(-1);
		if(fSuc)
		{
			ResumeThread(pcInfo.hThread);
			//等待线呈结束
			WaitForSingleObject(pcInfo.hThread,INFINITE);
			GetExitCodeProcess(pcInfo.hProcess,&result);
		}
		else
			return 0x55aaaa55 ;
		//AfxMessageBox(CommandLine);
		if(str.GetAt(0) == '"')
			str = str.Mid(1,str.GetLength()-2);
		fl.Open(str,CFile::modeRead);
		compsize = fl.GetLength();
		DWORD len = compsize;
		compsize = (compsize +15)/16*16 ;
		BYTE *p = new BYTE[compsize];
		memset(p,0,compsize);
		fl.Read(p,compsize);
		fl.Close();

		if(compsize>=0x20000)
		{
			memcpy(totalsize + pWrite,pbuf+0x20000*i,0x20000);
			totalsize += 0x20000 ;
			compsize = 0x20000 ;
		}
		else
		{
			memcpy(totalsize + pWrite,p,len);
			delete []p;
			totalsize += compsize ;
		}
		pos[i] = compsize ;
		
		strTemp.Remove('"');
		DeleteFile(strTemp);
		str.Remove('"');
		DeleteFile(str);
	}
	memcpy(pWrite+8,pos,first-8);
	*(DWORD*)(pWrite)='lz77';
	*(DWORD*)(pWrite+4)=block;
	fl.Open(strTargetFile,CFile::modeCreate|CFile::modeWrite);
	fl.Write(pWrite,totalsize);
	fl.Close();
	delete []pbuf ;
	delete []pWrite;
	delete []pcomp;
//	m_TextZipProgressCtrl.ShowWindow(SW_HIDE);
	str.Format("%s  %d%%","Writing EZ-Disk...",0);
//	m_TextWriteProgressCtrl.SetWindowText(str);
//	m_TextWriteProgressCtrl.ShowWindow(SW_SHOW);
	return TRUE;
}

CString CNew_EZManageDlg::ModifyBmpFile(CString strFileName)
{
	unsigned short  *pix;
	int i=0,width, height;
	FILE *fpt;
	unsigned short  w, h, bpp;
	char head[3] = "BM";
	CString str;
	str = strFileName.Left(strFileName.GetLength()-4);
	str +=_T("back.bmp");

	if(read_bmp(strFileName.GetBuffer(strFileName.GetLength()), (void**)&pix, 16, &width, &height, 0, 0) != ERR_BMP_OK)
			return 1;
	strFileName.ReleaseBuffer(strFileName.GetLength());

	fpt = fopen(str.GetBuffer(str.GetLength()), "wb");
	str.ReleaseBuffer(str.GetLength());
	if(fpt == NULL)
		return 1;
	w = width;
	h = height;
	bpp = 16;

	fwrite(&head, 2, 1, fpt);
	fwrite(&bpp, 2, 1, fpt);
	fwrite(&w, 2, 1, fpt);
	fwrite(&h, 2, 1, fpt);
	fwrite(pix, 2, width*height, fpt);
	fclose(fpt);
	return str;
}

int CNew_EZManageDlg::ExecCommand(char* bstrCmd)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // Create the process
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.wShowWindow = SW_HIDE;
    si.dwFlags = STARTF_USESHOWWINDOW;
    BOOL b = CreateProcess( NULL,
                            bstrCmd,
                            NULL,
                            NULL,
                            FALSE,
                            NORMAL_PRIORITY_CLASS,
                            NULL,
                            NULL,
                            &si,
                            &pi);
    if (b)
        WaitForSingleObject(pi.hProcess, INFINITE); 
    return 0;
}

BOOL CNew_EZManageDlg::CheckNandDisk(DWORD size)
{
//extern DWORD g_dwNandFreeSpace;
	if( (g_dwNandFreeSpace-8*1024)<size/1024)
	{
		AfxMessageBox("没有足够的Nand空间");
		return FALSE;
	}
	else
	{
		g_dwNandFreeSpace -=size/1024;
		return TRUE;
	}
}
CString CNew_EZManageDlg::GetNewFileName(CString strName,int index)
{
	CString str,strTemp;
	str.Empty();
	strTemp.Empty();
	if(strName.Find('.')>0)
	{
		strTemp = strName.Right(4);
		strName = strName.Left(strName.GetLength()-4);
	}
	
	if(strName.GetLength()>=8)
	{
		strName = strName.Left(7);
	}
	str.Format("%d",index);
	strName = strName+str+strTemp;
	return strName;
}
BOOL CNew_EZManageDlg::HasSameNameInArray(CString strName,CStringArray *pArray)
{
	CString str;
	int pos;
	for(int i=0;i<pArray->GetSize();i++)
	{
		str = pArray->GetAt(i);
		//str=str.Right(strName.GetLength()+4);
		pos = str.ReverseFind('\\');
		str = str.Right(str.GetLength()-pos-1);
		if(str.Find('.')>0)
			str = str.Left(str.GetLength()-4);
		if(strName == str)
			return TRUE;
	}
	return FALSE;
}
BOOL CNew_EZManageDlg::HasSameNameInList(CString strName)
{
	CString str;
	strName.MakeLower();
	int loop=0;
	loop = m_PageDisk.m_DiskListCtrl.GetItemCount();
	for(int i=0;i<loop;i++)
	{
		str = m_PageDisk.m_DiskListCtrl.GetItemText(i,0);
		if(str == strName)
		{
			return TRUE;
		}
	}
	return FALSE;
}

LRESULT CNew_EZManageDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	int iType = (int)wParam;
	CString str;
	if(message == WM_NAND_WRITE_BEGIN_MESSAGE)
	{
	//_WriteProgress.ShowWindow(SW_HIDE);
//		m_TextRamProgressCtrl.ShowWindow(SW_HIDE);
		switch(iType)
		{
		case 0:
			str.Format("%s  %d%%",m_gStrMsgWriting,0);
			m_WriteProgress.SetWindowText(str);
			m_WriteProgress.SetPos(0);
			break;
		case 1:
	//m_WriteProgress.SetWindowText("正在删除Ez磁盘文件...");
			break;
		case 2:
			m_WriteProgress.SetWindowText("删除文件");	
			break;
		case 3:
			m_WriteProgress.SetWindowText("正在删除Ez磁盘目录...");
			break;
		case 4:
			m_WriteProgress.SetWindowText("正在创建Ez磁盘目录...");
			break;
		case 5:
			m_WriteProgress.SetWindowText("正在读Ez磁盘文件...");
			break;
		}
		m_WriteProgress.ShowWindow(SW_SHOW);
		m_WriteProgress.SetPos(0);
	}	
	if(message == WM_NAND_WRITE_END_MESSAGE)
	{
		m_WriteProgress.SetPos(0);
		m_WriteProgress.SetWindowText("写EZ-Disk结束");
	}
	if(message == WM_NAND_WRITE_STEP_MESSAGE)
	{	
			int pos = (int ) wParam;
			int dw_RangeLow,dw_RangeHigh;
			BOOL bSign = FALSE;
			m_WriteProgress.GetRange(dw_RangeLow,dw_RangeHigh);
			DWORD dw_percent = (pos*100)/dw_RangeHigh;
			if(dw_percent>=98)
			{
				dw_percent = 98;
				bSign = TRUE;
				pos = (int)(dw_percent*dw_RangeHigh)/100;
			}
			else
			{	
				bSign = FALSE;
			}
			str.Format("%s  %d%%",m_gStrMsgWriting,dw_percent);
			m_WriteProgress.SetWindowText(str);
			m_WriteProgress.SetPos(pos);
	}
	if(message == WM_NAND_READ_STEP_MESSAGE)
	{
		int pos = (int ) wParam;
		int dw_RangeLow,dw_RangeHigh;
		BOOL bSign = FALSE;
		m_WriteProgress.GetRange(dw_RangeLow,dw_RangeHigh);
		DWORD dw_percent = (pos*100)/dw_RangeHigh;
		if(dw_percent>=98)
		{
			dw_percent = 98;
			bSign = TRUE;
			pos = (int)(dw_percent*dw_RangeHigh)/100;
		}
		else
		{	
			bSign = FALSE;
		}
		CString str;
		str.Format("%s  %d%%","Reading EZ-Disk...",dw_percent);
		m_WriteProgress.SetWindowText(str);
		m_WriteProgress.SetPos(pos);
	}
	//处理语言菜单的更新
	UINT i=0;
	HMENU  hmenu;
	CMenu *pRootMenu = GetMenu();
	CMenu *pSubRoot= NULL,*pSub = NULL;
	BOOL b = pRootMenu->ModifyMenu(0,MF_BYPOSITION,0,m_gMenuSysConfig);

	if(!pSubRoot)
	{
		pSubRoot = pRootMenu->GetSubMenu(0);
		
	}
	if(!pSub)
	{
		pSub = pSubRoot->GetSubMenu(5);
		pSubRoot->ModifyMenu(5,MF_ENABLED|MF_POPUP|MF_BYPOSITION,(UINT)pSub->m_hMenu,m_gMenuLanguage);//语言选择

	}
	if(message == WM_COMMAND)
	{
		hmenu = (HMENU)lParam ;
		int index = (int)wParam ;
		if(index>=IDD_CHINESE)
		{
			if(pSub)
			{
				pSub->GetMenuString(index,str,MF_BYCOMMAND);
				if(str.GetLength()>0)
				{
					for(i=0;i<pSub->GetMenuItemCount();i++)
					{
						pSub->CheckMenuItem(i,MF_BYPOSITION|MF_UNCHECKED);
					}
					pSub->CheckMenuItem(index,MF_CHECKED);
					pSub->GetMenuString(index,str,MF_BYCOMMAND);
					char cChar[256];
					::GetPrivateProfileString("Language","Version","",cChar,256,m_strWindowsDir+("\\temp\\conf.ini"));
					if(str != cChar)
					{
						::WritePrivateProfileString("Language","Version",str,m_strWindowsDir+("\\temp\\conf.ini"));
						MessageBox(m_gStrMsgLanguageChange,m_gStrMsgCation,MB_ICONEXCLAMATION | MB_OK);
					}
				}
			}
		}
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CNew_EZManageDlg::OnSyncClock() 
{
	// TODO: Add your command handler code here
	KillTimer(IDENTIFY_CART_TIMER);
	m_Tab.EnableWindow(FALSE);
	BYTE clock ;
	BYTE Year;
	BYTE Month;
	BYTE Data;
	BYTE Week;
	BYTE Hour;
	BYTE Minitue;
	BYTE Second;
	SYSTEMTIME HostSystemtime;
	CTimeSpan  VerifyTime;

	CTime m_cTime   = CTime::GetCurrentTime();
	m_cTime.GetAsSystemTime(HostSystemtime);
	Year = (BYTE)HostSystemtime.wYear-2000;
	Month = (BYTE)HostSystemtime.wMonth;
	Data = (BYTE)HostSystemtime.wDay;
	Week = (BYTE)HostSystemtime.wDayOfWeek;
	Hour = (BYTE)HostSystemtime.wHour;
	Minitue = (BYTE)HostSystemtime.wMinute;
	Second = (BYTE)HostSystemtime.wSecond;
	VerifyTime = 0;
	
	HANDLE hDev;
	m_pezVirtualBase->OpenCartDevice(hDev);//打开卡设备
	if(hDev == INVALID_HANDLE_VALUE)//打开设备
	{
		m_Tab.EnableWindow(TRUE);
		SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
		return ;
	}
	m_pezVirtualBase->CartOpen(hDev);	//红灯
	m_pezVirtualBase->CartCloseFlashOP(hDev);
	clock = m_pezVirtualBase->ReadStatus(hDev);
	if(clock && 0x80)
	{
		m_pezVirtualBase->ClockReset(hDev);
	}
	m_pezVirtualBase->WriteStatus(hDev,0x40);

	m_pezVirtualBase->WriteClock(hDev,Year,Month,Data,Week,Hour,Minitue,Second);
	Year=0;Month=0;Data=0;Week=0;Hour=0;Minitue=0;Second=0;
	m_pezVirtualBase->CartClose(hDev);
	m_pezVirtualBase->CloseCartDevice(hDev);
		
	m_Tab.EnableWindow(TRUE);
	SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
}

void CNew_EZManageDlg::OnUpdateSyncClock(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_Tab.IsWindowEnabled())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CNew_EZManageDlg::InitMenu()
{
	CMenu *pRootMenu = GetMenu() ;
	CMenu *pSubRoot,*pSub;
	char	*pstr = new char[256]; 
	CString str;
	m_MenustringArray.RemoveAll();
	CNew_EZManageApp *pApp = (CNew_EZManageApp *)AfxGetApp();
	CStringArray *pStringArray = &pApp->m_stringArray;
	for(UINT i=0;i<pStringArray->GetSize();i++)
	{
		::GetPrivateProfileString("Language","LanguageVersion","",pstr,256,m_strWorkPath+_T("\\langdata\\")+pStringArray->GetAt(i));
		m_MenustringArray.Add(pstr);
	}
	pSubRoot = pRootMenu->GetSubMenu(0);
	pSub = pSubRoot->GetSubMenu(5);
	::GetPrivateProfileString("Language","Version","",pstr,256,m_strWindowsDir+("\\temp\\conf.ini"));
	m_strLanguage = pstr;
	for(i=0;i<m_MenustringArray.GetSize();i++)
	{
		str = m_MenustringArray.GetAt(i);
		if(i==0)
		{
			pSub->RemoveMenu(0,MF_BYPOSITION);
		}
		if(m_strLanguage == str)
			pSub->AppendMenu(MF_ENABLED|MF_STRING|MF_BYCOMMAND|MF_MOUSESELECT|MF_CHECKED,IDD_CHINESE+i,str);
		else
			pSub->AppendMenu(MF_ENABLED|MF_STRING|MF_BYCOMMAND|MF_MOUSESELECT,IDD_CHINESE+i,str);
	}
	
//	BOOL b = pRootMenu->ModifyMenu(0,MF_BYPOSITION,0,m_gMenuSysConfig);
//	DWORD err= GetLastError();
	pRootMenu->ModifyMenu(IDD_ADD_LOADER,MF_BYCOMMAND|MF_ENABLED,IDD_ADD_LOADER,m_gMenuInstallLoader);//写入游戏ROM
	pRootMenu->ModifyMenu(IDD_ADD_NDSLOADER,MF_BYCOMMAND|MF_ENABLED,IDD_ADD_NDSLOADER,m_gMenuInstallNdsLoader);//写入游戏ROM
	pRootMenu->ModifyMenu(IDD_SYNC_CLOCK,MF_BYCOMMAND|MF_ENABLED,IDD_SYNC_CLOCK,m_gMenuSynClock);//写入游戏ROM
	pRootMenu->ModifyMenu(IDD_UPDATA_FILE,MF_BYCOMMAND|MF_ENABLED,IDD_UPDATA_FILE,m_gMenuDownLoadFiler);//写入游戏ROM
	pRootMenu->ModifyMenu(IDD_PASSME2,MF_BYCOMMAND|MF_ENABLED,IDD_PASSME2,m_gMenuPassMe2);//写入游戏ROM
	::GetPrivateProfileString("EZPassMe2","use","",pstr,256,m_strWindowsDir+("\\temp\\conf.ini"));
	str = pstr;
	if(str == "use")
	{
		//pRootMenu->ModifyMenu(IDD_PASSME2,MF_BYCOMMAND|MF_ENABLED|MF_CHECKED,IDD_PASSME2,m_gMenuPassMe2);//写入游戏ROM
		pRootMenu->CheckMenuItem(IDD_PASSME2,MF_CHECKED);
		m_bUsePassMe2 = TRUE;
	}
	else
	{
		//pRootMenu->ModifyMenu(IDD_PASSME2,MF_BYCOMMAND|MF_ENABLED|MF_UNCHECKED,IDD_PASSME2,m_gMenuPassMe2);//写入游戏ROM
		pRootMenu->CheckMenuItem(IDD_PASSME2,MF_UNCHECKED);
		m_bUsePassMe2 = TRUE;
		m_bUsePassMe2 = FALSE;
	}
	delete pstr;
	//DrawMenuBar ();
}

void CNew_EZManageDlg::InitInterface()
{
	CButton *pButton;
	//配置本地磁盘树
	pButton = (CButton *)GetDlgItem(IDC_STATIC_LOCALDISK);
	pButton->SetWindowText(m_gStrLocalDisk);
	pButton = (CButton *)GetDlgItem(IDC_STATIC_PATH);
	pButton->SetWindowText(m_gStrPath);
	//配置Tab
	pButton = (CButton *)GetDlgItem(IDC_STATIC_WRITECON);
	pButton->SetWindowText(m_gStrConfig);

	pButton = (CButton *)GetDlgItem(IDC_STATIC_CAPACITYINFO);
	pButton->SetWindowText(m_gStrCapacityInfo);


	pButton = (CButton *)GetDlgItem(IDC_STATIC_WRITEPOS);
	pButton->SetWindowText(m_gStrWriteInfo );

}

void CNew_EZManageDlg::FillNandGameInfo(GameRomInfo *pInfo)
{
	CRomManager rm ;
	DWORD len;
	if(pInfo->bNdsRom)
	{
		len = pInfo->flGameSize;	
		if( (pInfo->pGameData[2]==0 && pInfo->pGameData[3]==0xEA) || ( *(DWORD *)(&pInfo->pGameData[0xAC])==0x53534150))
		{
		}
		else
		{
			rm.NDSPatch1(pInfo->pGameData,len,NULL,0,16);
		}
		*((WORD *)(pInfo->pGameData+0xb8))= pInfo->flGameSize >> 15 ;
		*(BYTE *)(pInfo->pGameData+0xba) = 0xFF&(pInfo->uiSaverSize>>12);
		pInfo->flGameSize = len;
	}
	else
	{
		//填游戏的大小
		*((WORD *)(pInfo->pGameData+0xb8))= pInfo->flGameSize >> 15 ;
		//添存档的大小
		*(BYTE *)(pInfo->pGameData+0xba) = 0xFF&(pInfo->uiSaverSize>>12);
		rm.FillComplementCheck(pInfo->pGameData);
		int SaverSize=pInfo->uiSaverSize;
		if(pInfo->SavePatch == 0)
		{//EZPatch
			rm.SaverPatch(&(pInfo->pGameData),
						(ULONG*)&(pInfo->flGameSize),
						EZ2Cart,
						16,
						&(pInfo->uiSaverMode),
						0);
			rm.SpecialRomPatch(&(pInfo->pGameData),
								(ULONG*)&(pInfo->flGameSize),
								EZ2Cart,
								0);
			//pInfo->pGameData[0xb5] |= 0 ;
			//if((pInfo->flGameSize)%0x8000)
			//{
			//	rm.InflateROM(&pInfo->pGameData,(ULONG*)&pInfo->flGameSize);
			//}
		}
		else if(pInfo->SavePatch ==1)
		{
			rm.ApplyIPSPatch(pInfo->strIpsPatchFileName,
							(pInfo->pGameData),
							pInfo->flGameSize);
			//pInfo->pGameData[0xb5] |= 1 ;
		}
	//	else
			//pInfo->pGameData[0xb5] |= 2 ;
		
		if(pInfo->bReset && pInfo->pXCode)
			pInfo->pGameData[0xbb]=0x80;
		pInfo->pGameData[0xbb]=0x80;
		pInfo->pGameData[0xbc] = pInfo->uiSaverMode ;
		//pInfo->pGameData[0xb5] |= (pInfo->bBoma)?0x4:0;
		rm.FillComplementCheck(pInfo->pGameData);
	}
}

void CNew_EZManageDlg::OnUpdataFile() 
{
	// TODO: Add your command handler code here
	BOOL bRet ;
	DWORD dwRet;
	CString strServerName = _T("www.ezflash.cn");
	CString strObject,strXCodeFileName,strFileName,strInfo;
	int nPort = 80;
	CHttpConnection* pServer = NULL;
	CInternetSession session0(NULL,0);
	CHttpFile* pFile = NULL;
	DWORD dwHttpRequestFlags =INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT;
	CString strNewLocation;
	char *pTemp=NULL;
	char *p=NULL;
	CFile file;
	DWORD dwOldLength=0;
	DWORD dwNewLength=0;
	strXCodeFileName =m_strWorkPath+ _T("\\Sysbin\\ez_flash.bin");
	m_WriteProgress.SetWindowText(m_gStrMsgDownLoad);
	m_WriteProgress.SetRange(0,20);
	m_WriteProgress.SetPos(0);
	//升级ez_flash.bin
	try
	{
		m_WriteProgress.SetWindowText(m_gStrMsgDownLoad+_T("...ez_flash.bin..."));
		strObject = _T("/zip/ez_flash.bin");
		//2.连接一个Http服务器
		pServer = session0.GetHttpConnection(strServerName,0,80,NULL,NULL);
		m_WriteProgress.SetPos(2);
		if(pServer==NULL)
		{
			m_WriteProgress.SetPos(0);
			return ;
		}
		//3.打开一个HTTP请求
		pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,
			strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
		const TCHAR szHeaders[] =_T("GET www.ezflash.cn/zip/ez_flash.bin");
		m_WriteProgress.SetPos(5);
		//4.发送一个HTTP请求
		pFile->AddRequestHeaders(szHeaders);
		bRet = pFile->SendRequest();
		pFile->QueryInfoStatusCode(dwRet);
		pFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH,strNewLocation);//得到文件的长度
		dwNewLength =strtoul(strNewLocation.GetBuffer(strNewLocation.GetLength()),&pTemp,10);
		strNewLocation.ReleaseBuffer(strNewLocation.GetLength());
		file.Open(strXCodeFileName,CFile::modeRead);
		dwOldLength = file.GetLength();
		file.Close();
		m_WriteProgress.SetPos(6);
		if(dwNewLength>dwOldLength)
		{
			p = new char[dwNewLength];
			memset(p,0,dwNewLength);
			pFile->Read(p,dwNewLength);
			CFile file;
			//CString strFileName =_T("C:\\test.zip");
			//如果是只读属性,改变其属性
			DWORD dw = GetFileAttributes(strFileName);
			if(dw&&FILE_ATTRIBUTE_READONLY== 0x1)
				SetFileAttributes(strFileName,FILE_ATTRIBUTE_NORMAL);
			DeleteFile(strXCodeFileName);
			file.Open(strXCodeFileName,CFile::modeCreate|CFile::modeWrite);
			file.Write(p,dwNewLength);
			file.Close();
			delete p;
			p = NULL;
			m_WriteProgress.SetPos(7);
			//bUpdata = TRUE;
		}
		if(pFile)
		{
			pFile->Close();
			delete pFile;
		}
		if(pServer)
		{
			pServer->Close();
			delete pServer;
		}
		strXCodeFileName =m_strWorkPath+ _T("\\Sysbin\\loader.bin");
		m_WriteProgress.SetWindowText(m_gStrMsgDownLoad+_T(" loader.bin..."));
		strObject = _T("/zip/loader.bin");
		//2.连接一个Http服务器
		m_WriteProgress.SetPos(8);
		pServer = session0.GetHttpConnection(strServerName,0,80,NULL,NULL);
		m_WriteProgress.SetPos(9);
		if(pServer==NULL)
		{
			m_WriteProgress.SetPos(0);
			return ;
		}
		//3.打开一个HTTP请求
		pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,
			strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
		const TCHAR szHeaders1[] =_T("GET www.ezflash.cn/zip/loader.bin");
		m_WriteProgress.SetPos(10);
		if(!pFile)
		{
			m_WriteProgress.SetPos(0);
			return ;
		}
		//4.发送一个HTTP请求
		pFile->AddRequestHeaders(szHeaders1);
		bRet = pFile->SendRequest();
		pFile->QueryInfoStatusCode(dwRet);
		pFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH,strNewLocation);//得到文件的长度
		dwNewLength =strtoul(strNewLocation.GetBuffer(strNewLocation.GetLength()),&pTemp,10);
		strNewLocation.ReleaseBuffer(strNewLocation.GetLength());
		file.Open(strXCodeFileName,CFile::modeRead);
		dwOldLength = file.GetLength();
		file.Close();
		m_WriteProgress.SetPos(16);
		//if(dwNewLength>dwOldLength)
		{
			p = new char[dwNewLength];
			memset(p,0,dwNewLength);
			pFile->Read(p,dwNewLength);
			CFile file;
			//CString strFileName =_T("C:\\test.zip");
			//如果是只读属性,改变其属性
			DWORD dw = GetFileAttributes(strFileName);
			if(dw&&FILE_ATTRIBUTE_READONLY== 0x1)
				SetFileAttributes(strFileName,FILE_ATTRIBUTE_NORMAL);
			DeleteFile(strXCodeFileName);
			file.Open(strXCodeFileName,CFile::modeCreate|CFile::modeWrite);
			file.Write(p,dwNewLength);
			file.Close();
			delete p;
			p = NULL;
			m_WriteProgress.SetPos(18);
			//bUpdata = TRUE;
		}
		m_WriteProgress.SetPos(0);
		m_WriteProgress.SetWindowText(m_gStrMsgDownLoadEnd);
	}
	catch (CInternetException* pEx)
	{
		TCHAR szErr[1024];
		pEx->GetErrorMessage(szErr, 1024);
		pEx->Delete();
		return ;
	}
	if(pFile)
	{
		pFile->Close();
		delete pFile;
	}
	if(pServer)
	{
		pServer->Close();
		delete pServer;
	}
//	AfxMessageBox("结束");
	//关于更新loader的处理
	strXCodeFileName =m_strWorkPath+ _T("\\Sysbin\\loader.bin");
	file.Open(strXCodeFileName,CFile::modeRead);
	DWORD iLength = file.GetLength();
	BYTE *pBuf = new BYTE[iLength] ;
	file.Read(pBuf,iLength);
	file.Close();
	WORD tempVersion = *(WORD *)(pBuf+0x1C8); 
	delete []pBuf;
	if(tempVersion!=m_LoaderVersion)
	{	
		if(IDYES == MessageBox(m_gStrMsgOSChange,NULL,MB_YESNO))
		{
			m_bUpdtaLoader  = TRUE;
			WriteLoader();
		}
	}
}

void CNew_EZManageDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CNew_EZManageDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	
}

void CNew_EZManageDlg::OnUpdateUpdataFile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_Tab.IsWindowEnabled())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

BOOL CNew_EZManageDlg::bLoaderVersion(BYTE *pLoad)
{
	WORD *pdw1,*pdw2;
	CString strPath =m_strWorkPath+ _T("\\Sysbin\\loader_GBA.bin");
	BYTE *pFile;
	CFile file;
	if(!file.Open(strPath,CFile::modeRead))
		return FALSE;
	pFile = new BYTE[file.GetLength()];
	file.Read(pFile,file.GetLength());
	file.Close();
	pdw1 = (WORD *)(pLoad+0x1CA); 
	if(*pdw1 != 0x5AA5)
	{
		//有可能是Nds的Loader
		if(pLoad[0xAC]=='P' && pLoad[0xAD]=='A' && pLoad[0xAE]=='S' && pLoad[0xAF]=='S')
			return TRUE;
		return FALSE;
	}
	else
	{
		pdw2 = (WORD *)(pLoad+0x1C8); 
		pdw1 = (WORD *)(pFile+0x1C8); 
		m_LoaderVersion = *pdw2;
		SetWindowText("EZManager 2.22 (" +GetLoaderVersionFromCard(m_LoaderVersion)+_T(" Driver Version ")+m_strDriverVersion+_T(" )"));
		if(*pdw1 != *pdw2)
			return FALSE;
	}
	delete []pFile;
	
	return TRUE;
}

CString CNew_EZManageDlg::GetLoaderVersionFromCard(WORD Version)
{
	CString s ;//= str;
	s.Format("EZ3 OS Version:%3x",Version/0x10);
	s.Insert(16,'.');
	return s;
}



void CNew_EZManageDlg::OnUpdateAddNdsloader(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	int i=0;i++;
}

void CNew_EZManageDlg::OnAddNdsloader() 
{
	// TODO: Add your command handler code here
	m_bNdsLoader = TRUE;
	WriteLoader();
}

void CNew_EZManageDlg::OnPassme2() 
{
	// TODO: Add your command handler code here
	CMenu *pRootMenu = GetMenu() ;
	if(m_bUsePassMe2)
	{
		::WritePrivateProfileString("EZPassMe2","use","no use",m_strWindowsDir+("\\temp\\conf.ini"));
		//pRootMenu->ModifyMenu(IDD_PASSME2,MF_BYCOMMAND|MF_ENABLED|MF_UNCHECKED,IDD_PASSME2,m_gMenuPassMe2);//写入游戏ROM
		pRootMenu->CheckMenuItem(IDD_PASSME2,MF_UNCHECKED);
		m_bUsePassMe2 = FALSE;
	}
	else
	{
		::WritePrivateProfileString("EZPassMe2","use","use",m_strWindowsDir+("\\temp\\conf.ini"));
		//pRootMenu->ModifyMenu(IDD_PASSME2,MF_BYCOMMAND|MF_ENABLED|MF_CHECKED,IDD_PASSME2,m_gMenuPassMe2);//写入游戏ROM
		pRootMenu->CheckMenuItem(IDD_PASSME2,MF_CHECKED);
		m_bUsePassMe2 = TRUE;
		//下面调用写EZPassMe2写sav文件的函数
		WriteEZPassMe2Function();
	}
}

void CNew_EZManageDlg::WriteEZPassMe2Function()
{
	/*
	CEzFlashBase *m_pEz3Flash;
	HANDLE hDev;
	BYTE *pBuf = new BYTE[32768];
	memset(pBuf,0,32768);
	KillTimer(IDENTIFY_CART_TIMER);
	m_pEz3Flash = new CEzFlashBase ;
	m_pEz3Flash->OpenCartDevice(hDev);
	if((hDev == NULL)||(hDev==(void*)0xFFFFFFFF))
	{
		SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
		delete []pBuf;
		return ;
	}
	m_pEz3Flash->CartOpen(hDev);
	m_pEz3Flash->CartSetRAMPage(hDev,0);
	m_pEz3Flash->CartOpenFlashOP(hDev);
	m_pEz3Flash->CartRAMRead(hDev,0,pBuf,32768);
	m_pEz3Flash->CartCloseFlashOP(hDev);
	m_pEz3Flash->CartClose(hDev);
	m_pEz3Flash->CloseCartDevice(hDev);
	SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
	delete []pBuf;
	delete []m_pEz3Flash;
	*/
	
	CString strFilaname,strFile;
	BYTE *pBuf;
	DWORD len;
	CEzFlashBase *m_pEz3Flash;
	HANDLE hDev;
	
	CFile file;
	
	CFileDialog backdlg(TRUE,
						"sav",
						strFile,
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						"Saver files(*.sav)|*.sav||",
						this);
	if(IDOK == backdlg.DoModal())
	{
		strFilaname = backdlg.GetPathName();
		if(!file.Open(strFilaname,CFile::modeRead))
			return ;
		len = file.GetLength();
		pBuf = new BYTE[len];
		file.Read(pBuf,len);
		file.Close();
		KillTimer(IDENTIFY_CART_TIMER);
		m_pEz3Flash = new CEzFlashBase ;
		m_pEz3Flash->OpenCartDevice(hDev);
		if((hDev == NULL)||(hDev==(void*)0xFFFFFFFF))
		{
			SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
			delete []pBuf;
			delete []m_pEz3Flash;
			return ;
		}
		m_pEz3Flash->CartOpen(hDev);
		m_pEz3Flash->CartSetRAMPage(hDev,0);
		m_pEz3Flash->CartOpenFlashOP(hDev);
		m_pEz3Flash->CartRAMWrite(hDev,0,pBuf,len);
		//m_pEz3Flash->CartRAMWrite(hDev,0,psrc,65536);
//		memset(pdes,0,65536);
//		m_pEz3Flash->CartRAMRead(hDev,0,pdes,65536);
		m_pEz3Flash->CartCloseFlashOP(hDev);
		m_pEz3Flash->CartClose(hDev);
		m_pEz3Flash->CloseCartDevice(hDev);
		SetTimer(IDENTIFY_CART_TIMER,1000,NULL);
		delete []pBuf;
		delete []m_pEz3Flash;
	}
	
}
void CNew_EZManageDlg::OnUpdatePassme2(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

CString CNew_EZManageDlg::GetDriverVersionInfo()
{
	char ver[256];
	char pstr[256];
	CString strSys,strPath;
	VS_FIXEDFILEINFO  *pinfo ;  // buffer for version value pointer
	UINT uLen ;          // version length
	CFile file;

	GetSystemDirectory(strSys.GetBuffer(256),256);
	strSys.ReleaseBuffer();
	//strSys += "\\drivers\\ezwinit.sys" ;
	GetCurrentDirectory(256,pstr);
	strPath = pstr;
	if(file.Open(strSys + "\\drivers\\ezwrit3.sys",CFile::modeRead))
	{
		strSys += "\\drivers\\ezwrit3.sys" ;
		file.Close();
	}
	else if(file.Open(strSys + "\\drivers\\ezwrite2.sys",CFile::modeRead))
	{
		strSys += "\\drivers\\ezwrite2.sys" ;
		file.Close();
	}
	else
	{
		return _T("");
	}

	//判断windows的版本
	DWORD dwVersion = GetVersion();
	DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
	DWORD dwWindowsMinorVersion =  (DWORD)(HIBYTE(LOWORD(dwVersion)));
	DWORD dwBuild;
	DWORD size;
	if (dwVersion < 0x80000000)                // Windows NT/2000
	{
		dwBuild = (DWORD)(HIWORD(dwVersion));
		size = GetFileVersionInfoSize(strSys.GetBuffer(256),0);
		strSys.ReleaseBuffer(256);
	}
	else if (dwWindowsMajorVersion < 4)        // Win32s
		dwBuild = (DWORD)(HIWORD(dwVersion) & ~0x8000);
	else                  // Windows 95/98 -- No build number
	{
		strSys = strSys.Left(2);
		strSys += _T("\\WINDOWS\\SYSTEM32\\DRIVERS\\ezwinit.sys");
		size = GetFileVersionInfoSize(strSys.GetBuffer(256),0);
		strSys.ReleaseBuffer(256);
		DWORD 
		dwBuild =  0;
	}
	//
	size = GetFileVersionInfoSize(strSys.GetBuffer(256),0);
	strSys.ReleaseBuffer(256);
	
	BOOL ff;
	ff = GetFileVersionInfo(strSys.GetBuffer(256),0,256,ver);\
	strSys.ReleaseBuffer(256);
	if(ff!=0)
	{
		VerQueryValue(ver,"\\",(void**)&pinfo,&uLen) ;
		strSys.Format(":%d.%d%d%d",HIWORD(pinfo->dwFileVersionMS),
						LOWORD(pinfo->dwFileVersionMS),HIWORD(pinfo->dwFileVersionLS),LOWORD(pinfo->dwFileVersionLS));		
	}
	return strSys;
}