// EZListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "new_ezmanage.h"
#include "EZListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEZListCtrl
//[MsgInfo]
extern CString m_gStrMsgReading	;					//正在读EZ-Disk...
extern CString m_gStrMsgReadEnd;//=读EZ-Disk结束
extern CString m_gStrMsgCation;//=提示
extern CString m_gStrMsgNoWrited;//=未烧录
extern CString m_gStrMsgWrited;//=已烧录
extern CString m_gStrMsgUse;//=Use
extern CString m_gStrMsgLeft;//=剩余
extern CString m_gStrMsgReserve;//=保留
extern CString m_gStrMsgFolder;//文件夹
extern CString m_gStrMsgNoCart;//=没有找到EZ3卡
extern CString m_gStrMsgEZDisk;//=EZ磁盘
extern CString m_gStrMsgInstallOS;//=正在安装EZ3操作系统...
extern CString m_gStrMsgInstallOSUp;
extern CString m_gStrMsgInstallOSEnd;//=EZ3操作系统安装完毕
extern CString m_gStrMsgInstallOSUp;
extern CString m_gStrMsgNoEnough;//=没有足够的Flash ROM空间
extern CString m_gStrMsgIncalideFile;//=无效的ROM文件,不能添加到Flash区
extern CString m_gStrMsgSelectFolder;//=请选择本地文件路径
extern CString m_gStrMsgSlectBackRom;//=请选择备份的ROM
extern CString m_gStrMsgErrorBackRom;//=错误的备份ROM
extern CString m_gStrMsgBackRomEnd;//=备份ROM结束
extern CString m_gStrMsgFormatNorCation;//=格式化256M Bits Flash将丢失所有数据
extern CString m_gStrMsgFormatingNor;//=正在格式化256Mfalsh区...
extern CString m_gStrMsgFormatNorEnd;//=格式化256Mfalsh结束
extern CString m_gStrMsgErasing;//=正在擦除
extern CString m_gStrMsgWriting;//=正在写...
extern CString m_gStrMsgWriteEnd;//=烧录结束
extern CString m_gStrMsgCreatingFolder;//=正在创建目录...
extern CString m_gStrMsgCreateFolderOK;//=创建目录成功
extern CString m_gStrMsgDeletingFolder;//=正在删除目录...
extern CString m_gStrMsgDelSaveCation;//=saver文件夹不能改名
extern CString m_gStrMsgEnterFileName;//=请输入合法的文件名
extern CString m_gStrMsgSameCation;//=重名,不能修改文件名
extern CString m_gStrMsgDelFolderEnd;//=删除目录操作结束
extern CString m_gStrMsgReadFileEnd;//=读文件结束
extern CString m_gStrMsgSelectFile;//=请选择待删除文件
extern CString m_gStrMsgDeletingFile;//=正在删除文件...
extern CString m_gStrMsgIsDelFile;//=确认删除文件
extern CString m_gStrMsgDleFileOK;//=删除文件结束
extern CString m_gStrMsgCompressing;//=正在压缩数据...
extern CString m_gStrMsgFormatcation;//=EZ磁盘没有格式化,请按确定格式化
extern CString m_gStrMsgCurrent;//=当前目录
extern CString m_gStrMsgRootDir;//=当前目录是根目录
extern CString m_gStrMsgMore128MInfo;
extern CString m_gStrMsgLanguageChange;
extern CString m_gStrMsgOSChange;
extern CString m_gStrMsgDownLoad;//=正在下载数据文件;
extern CString m_gStrMsgDownLoadEnd;
CEZListCtrl::CEZListCtrl()
{
}

CEZListCtrl::~CEZListCtrl()
{
}


BEGIN_MESSAGE_MAP(CEZListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CEZListCtrl)
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEZListCtrl message handlers

void CEZListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: Add your message handler code here and/or call default
	CDC* pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
	int nItem = lpDrawItemStruct->itemID;
	
	CRect rcAllLabels;
	GetItemRect(nItem,rcAllLabels,LVIR_BOUNDS);

	pDC->SaveDC();
	
	BOOL bSelected = false;
	if(GetItemState(nItem, LVIS_SELECTED) == LVIS_SELECTED)
	{
		//pDC->SetTextColor(RGB(200,0,0));//::GetSysColor(COLOR_HIGHLIGHTTEXT));
		//pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));

		pDC->FillRect(rcAllLabels,&CBrush(RGB(10,36,106)));
		bSelected = true;
	}
	else
		pDC->FillRect(rcAllLabels,&CBrush(RGB(255,255,255)));
	TCHAR buffer[255];
	LV_COLUMN lvc;
	lvc.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.pszText = buffer;
	lvc.cchTextMax = 255;

	CRect rcItem;
	rcItem.left = rcAllLabels.left;
	rcItem.right = rcAllLabels.left;
	rcItem.top = rcAllLabels.top;
	rcItem.bottom = rcAllLabels.bottom;
	CRect rcLabel;

	for(int nColumn = 0; GetColumn(nColumn, &lvc); nColumn++)
	{
		rcItem.left=rcItem.right;
		rcItem.right += lvc.cx;

		rcLabel=rcItem;
		rcLabel.DeflateRect(1,1);
		
		GetItemText(nItem,nColumn,buffer,255);
		if(GetItemText(nItem,4) == m_gStrMsgNoWrited)
			pDC->SetTextColor(RGB(255,0,0));
		else
			pDC->SetTextColor(RGB(0,0,0));
		if(GetItemState(nItem, LVIS_SELECTED) == LVIS_SELECTED && (GetItemText(nItem,4) != m_gStrMsgNoWrited))
			pDC->SetTextColor(RGB(255,255,255));
		pDC->DrawText(buffer , rcLabel , DT_LEFT|DT_VCENTER);

		pDC->MoveTo(rcItem.left, rcItem.top);
		pDC->LineTo(rcItem.left, rcItem.bottom-1);
		pDC->LineTo(rcItem.right, rcItem.bottom-1);
		pDC->LineTo(rcItem.right, rcItem.top);
	}
	pDC->RestoreDC(-1);
}
