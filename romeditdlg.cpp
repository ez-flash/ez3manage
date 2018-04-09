// RomEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "new_ezmanage.h"
#include "RomEditDlg.h"
#ifdef _DEBUG

#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRomEditDlg dialog

//[RomEdit]
extern CString m_gStrMsgRomEditInfo;			//ROM信息编辑
extern CString m_gStrMsbBaseInfo;
extern CString m_gStrMsgRomName;				//ROM 名
extern CString m_gStrMsgRomSize;				//ROM大小
extern CString m_gStrMsgSoftReset;				//软复位
extern CString m_gStrMsgInstantSave;
extern CString m_gStrMsgCompress;				//压缩
extern CString m_gStrMsgSaveType;				//存档方式
extern CString m_gStrMsgSaveSize;				//存档大小
extern CString m_gStrMsgAddPatch;				//打存档补丁
extern CString m_gStrMsgChectInfo;				//金手指信息
extern CString m_gStrMsgOK;					//确定
extern CString m_gStrMsgCancle;
CRomEditDlg::CRomEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRomEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRomEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pInfo = NULL;
	m_bZip = FALSE;
	m_bZipButState = FALSE;
	m_bXCodeState = FALSE;
}


void CRomEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRomEditDlg)
	DDX_Control(pDX, IDC_COMBO_SAVEPATCH, m_CbSavepatchType);
	DDX_Control(pDX, IDC_CHEAT_TREE, m_CheatTree);
	DDX_Control(pDX, IDC_COMBO_SAVESIZE, m_CbSaveSize);
	DDX_Control(pDX, IDC_COMBO_SAVETYPE, m_CbSaveType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRomEditDlg, CDialog)
	//{{AFX_MSG_MAP(CRomEditDlg)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_CHTFILE, OnButtonOpenChtfile)
	ON_BN_CLICKED(IDC_CHECK_NOR_RESET, OnCheckNorReset)
	ON_CBN_SELCHANGE(IDC_COMBO_SAVETYPE, OnSelchangeComboSavetype)
	ON_CBN_SELCHANGE(IDC_COMBO_SAVESIZE, OnSelchangeComboSavesize)
	ON_CBN_SELCHANGE(IDC_COMBO_SAVEPATCH, OnSelchangeComboSavepatch)
	ON_BN_CLICKED(IDC_CHECK_ZIP, OnCheckZip)
	ON_BN_CLICKED(IDC_CHECK_NOR_SAVENOW, OnCheckNorSavenow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRomEditDlg message handlers

void CRomEditDlg::SetGameInfo(GameRomInfo *pInfo)
{
	m_pInfo = pInfo;
}

GameRomInfo *CRomEditDlg::GetGameInfo()
{
	return m_pInfo;
}

BOOL CRomEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetWindowText(m_gStrMsgRomEditInfo);
	CButton *pBut; 
	pBut = (CButton *)GetDlgItem(IDC_STATIC_BASEINFO);
	pBut->SetWindowText(m_gStrMsbBaseInfo);
	
	pBut = (CButton *)GetDlgItem(IDC_STATIC_NOR_ROMNAME);
	pBut->SetWindowText(m_gStrMsgRomName);

	pBut = (CButton *)GetDlgItem(IDC_STATIC_NORROMSIZE);
	pBut->SetWindowText(m_gStrMsgRomSize);

	pBut = (CButton *)GetDlgItem(IDC_CHECK_NOR_RESET);
	pBut->SetWindowText(m_gStrMsgSoftReset);

	pBut = (CButton *)GetDlgItem(IDC_CHECK_NOR_SAVENOW);
	pBut->SetWindowText(m_gStrMsgInstantSave);
	
	pBut = (CButton *)GetDlgItem(IDC_CHECK_ZIP);
	pBut->SetWindowText(m_gStrMsgCompress);

	pBut = (CButton *)GetDlgItem(IDC_STATIC_NOR_SAVETYPE);
	pBut->SetWindowText(m_gStrMsgSaveType);

	pBut = (CButton *)GetDlgItem(IDC_STATIC_NOR_SAVESIZE);
	pBut->SetWindowText(m_gStrMsgSaveSize);

	pBut = (CButton *)GetDlgItem(IDC_STATIC_NOR_SAVEPATCH);
	pBut->SetWindowText(m_gStrMsgAddPatch);

	pBut = (CButton *)GetDlgItem(IDC_STATIC_CHEATINFO);
	pBut->SetWindowText(m_gStrMsgChectInfo);

	pBut = (CButton *)GetDlgItem(IDOK);
	pBut->SetWindowText(m_gStrMsgOK);

	pBut = (CButton *)GetDlgItem(IDCANCEL);
	pBut->SetWindowText(m_gStrMsgCancle);
	
	CString str;
	if(m_pInfo)
	{
		//显示ROM 名字
		pBut = (CButton *)GetDlgItem(IDC_STATIC_DISPROMNAME);
		if(pBut)
			pBut->SetWindowText(m_pInfo->strGameName);
		//显示ROM大小
		pBut = (CButton *)GetDlgItem(IDC_STATIC_DISPROMSIZE);
		if(pBut)
		{
			str.Format("%d%s",m_pInfo->flGameSize/1024,"K Bits");
			pBut->SetWindowText(str);
		}
		//显示软复位状态
		pBut = (CButton *)GetDlgItem(IDC_CHECK_NOR_RESET);
		if(pBut)
		{
			if(m_pInfo->pXCode && m_pInfo->bReset)
			{
				pBut->SetCheck(TRUE);
				GetDlgItem(IDC_BUTTON_OPEN_CHTFILE)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_OPEN_CHTFILE)->EnableWindow(TRUE);
			}
			else
			{
				pBut->SetCheck(FALSE);
				GetDlgItem(IDC_BUTTON_OPEN_CHTFILE)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_OPEN_CHTFILE)->EnableWindow(TRUE);
			}
		}
		//显示即时存档的状态
		pBut = (CButton *)GetDlgItem(IDC_CHECK_NOR_SAVENOW);
		if(pBut)
		{
			if(m_pInfo->pXCode && m_pInfo->bSaveNow)
			{
				pBut->SetCheck(TRUE);
				GetDlgItem(IDC_BUTTON_OPEN_CHTFILE)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_OPEN_CHTFILE)->EnableWindow(TRUE);
			}
			else
			{
				pBut->SetCheck(FALSE);
				GetDlgItem(IDC_BUTTON_OPEN_CHTFILE)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_OPEN_CHTFILE)->EnableWindow(TRUE);
			}
		}
	
		
		CButton *pBut = (CButton *)GetDlgItem(IDC_CHECK_ZIP);
		if(pBut)
		{
			pBut->EnableWindow(m_bZipButState);
		}
		pBut = (CButton *)GetDlgItem(IDC_CHECK_NOR_RESET);
		if(pBut)
		{
			pBut->EnableWindow(m_bXCodeState);
		}
		
		//显示存档类型
		switch(m_pInfo->uiSaverMode)
		{
			case UNKNOW:
				m_CbSaveType.SetCurSel(3);
				break;
			case SRAM_TYPE:
				m_CbSaveType.SetCurSel(0);
				break;
			case EEPROM_TYPE:
				m_CbSaveType.SetCurSel(1);
				break;
			case FLASH_TYPE:
				m_CbSaveType.SetCurSel(2);
				break;
		}
		//显示存档大小
		int saveSize=m_pInfo->uiSaverSize*8/1024;
		switch(saveSize)
		{
			case 0:
				m_CbSaveSize.SetCurSel(0);
				break;	
			case 64:
				m_CbSaveSize.SetCurSel(1);
				break;
			case 128:
				m_CbSaveSize.SetCurSel(2);
				break;
			case 256:
				m_CbSaveSize.SetCurSel(3);
				break;
			case 512:
				m_CbSaveSize.SetCurSel(4);
				break;
			case 1024:
				m_CbSaveSize.SetCurSel(5);
				break;
		}
		switch(m_pInfo->SavePatch)
		{
		case 0:
			m_CbSavepatchType.SetCurSel(1);
			break;
		case 1:
			m_CbSavepatchType.SetCurSel(2);
			break;
		default:
			m_CbSavepatchType.SetCurSel(0);
			break;
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRomEditDlg::OnButtonOpenChtfile() 
{
	// TODO: Add your control notification handler code here
	CString str,strCheatFileName,strCheatFilePathName;
	
	CString strFilter = _T("All Surport files(*.cht)|*.cht|\
						Cheat File(*.cht)|*.cht");
						/*All Files (*.*)|*.*||");*/
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
		strCheatFilePathName = openDlg.GetPathName();
		strCheatFileName = openDlg.GetFileName();

		CWnd *pWnd = GetDlgItem(IDC_EDIT_OPEN_CHTFILE);
		if(pWnd)
			pWnd->SetWindowText(strCheatFilePathName);
		
		pWnd = GetParent();
		CNew_EZManageDlg *pDlg = (CNew_EZManageDlg *)pWnd;
		DiskPlayCheatFile(strCheatFilePathName);
		WORD size=0;
		PGOLDCHEAT pGoldCheat = GetGoldDataFormTree(size);
		if(!m_pInfo->pXCode)
		{
			pDlg->DeleteGoldCheatPointer(m_pInfo->pGlodCheat,m_pInfo->CheatSize);
			return;
		}
		else
		{
			if(m_pInfo->pGlodCheat)
			{
				pDlg->DeleteGoldCheatPointer(m_pInfo->pGlodCheat,m_pInfo->CheatSize);
			}
			m_pInfo->pGlodCheat=pGoldCheat;
			m_pInfo->CheatSize = size;
		}

	}
	else
		return;
}

BOOL CRomEditDlg::DiskPlayCheatFile(CString strFileName)
{
	BOOL Ret=TRUE;
	CString strTemp;
	PCHT_DATA pCht_data;

	CChtFileInfo m_ChtFile(strFileName);
	m_ChtFile.GetData();
	POSITION pos = m_ChtFile.m_GoldList.GetHeadPosition();
	int rowcounter= 0;
	
	strTemp.Empty();
	//填充金手指数据到树
	m_CheatTree.DeleteAllItems();
	dw_CheatCounter=0;
	for(int i=0;i<m_ChtFile.m_GoldList.GetCount();i++)
	{
		pCht_data = m_ChtFile.m_GoldList.GetNext(pos);
		if(pCht_data->strFunction!=strTemp && pCht_data->strFunction!=_T(""))
		{
			//添加到根目录
			AddItemToCheatTree(pCht_data,TRUE);
			strTemp = pCht_data->strFunction;
		}
		else
		{
			//添加到当前子目录
			AddItemToCheatTree(pCht_data,FALSE);
		}
	}
	m_ChtFile.RemoveAllItem();
	return Ret;
}

void CRomEditDlg::AddItemToCheatTree(PCHT_DATA pCht_data,BOOL bRoot)
{
	TV_INSERTSTRUCT     curTreeItem;
	CString strFunction = pCht_data->strFunction;
	if(bRoot)
	{
		//插入Function
		curTreeItem.hParent = 0;
		curTreeItem.hInsertAfter = TVI_LAST;
		curTreeItem.item.pszText = strFunction.GetBuffer(strFunction.GetLength());
		strFunction.ReleaseBuffer(strFunction.GetLength());
		curTreeItem.item.mask = TVIF_TEXT;
		
		m_hRootItem = m_CheatTree.InsertItem(&curTreeItem);
//		m_CheatTree.SetItemBold(m_hRootItem);
//		m_CheatTree.SetItemColor(m_hRootItem,RGB(255,0,0));

		//插入Setup
		strFunction  = pCht_data->strSetup;
		curTreeItem.hParent = m_hRootItem;
		curTreeItem.hInsertAfter = TVI_LAST;
		curTreeItem.item.pszText = strFunction.GetBuffer(strFunction.GetLength());
		strFunction.ReleaseBuffer(strFunction.GetLength());
		curTreeItem.item.mask = TVIF_TEXT;
		m_hItem1 = m_CheatTree.InsertItem(&curTreeItem);
		//m_CheatTree.SetItemBold(m_hRootItem);
		//m_CheatTree.SetItemColor(m_hRootItem,RGB(0,0,0));
		//加入数据
		strFunction.Format("Address=0x%x,Data=0x%x,DataType=0x%1x,LoopCounter=0x%x",pCht_data->dw_Address,pCht_data->dw_Data,pCht_data->m_dataType,pCht_data->loopcounter);
		curTreeItem.hParent = m_hItem1;
		curTreeItem.hInsertAfter = TVI_LAST;
		curTreeItem.item.pszText = strFunction.GetBuffer(strFunction.GetLength());
		strFunction.ReleaseBuffer(strFunction.GetLength());
		curTreeItem.item.mask = TVIF_TEXT;
		m_CheatTree.InsertItem(&curTreeItem);

		dw_CheatCounter++;
	}
	else if(strFunction!=_T("") && bRoot == FALSE)
	{	
		strFunction  = pCht_data->strSetup;
		curTreeItem.hParent = m_hRootItem;
		curTreeItem.hInsertAfter = TVI_LAST;
		curTreeItem.item.pszText = strFunction.GetBuffer(strFunction.GetLength());
		strFunction.ReleaseBuffer(strFunction.GetLength());
		curTreeItem.item.mask = TVIF_TEXT;
		m_hItem1 = m_CheatTree.InsertItem(&curTreeItem);
		
		strFunction.Format("Address=0x%x,Data=0x%x,DataType=0x%x,LoopCounter=0x%x",pCht_data->dw_Address,pCht_data->dw_Data,pCht_data->m_dataType,pCht_data->loopcounter);
		curTreeItem.hParent = m_hItem1;
		curTreeItem.hInsertAfter = TVI_LAST;
		curTreeItem.item.pszText = strFunction.GetBuffer(strFunction.GetLength());
		strFunction.ReleaseBuffer(strFunction.GetLength());
		curTreeItem.item.mask = TVIF_TEXT;
		m_hItem2 = m_CheatTree.InsertItem(&curTreeItem);
	}		
	else if(strFunction==_T("") && bRoot == FALSE)
	{
		strFunction.Format("Address=0x%x,Data=0x%x,DataType=0x%x,LoopCounter=0x%x",pCht_data->dw_Address,pCht_data->dw_Data,pCht_data->m_dataType,pCht_data->loopcounter);
		curTreeItem.hParent = m_hItem1;
		curTreeItem.hInsertAfter = TVI_LAST;
		curTreeItem.item.pszText = strFunction.GetBuffer(strFunction.GetLength());
		strFunction.ReleaseBuffer(strFunction.GetLength());
		curTreeItem.item.mask = TVIF_TEXT;
		m_CheatTree.InsertItem(&curTreeItem);
	}
}

void CRomEditDlg::OnCheckNorReset() 
{
	// TODO: Add your control notification handler code here
	//BOOL bCheck = ((CButton *)(GetDlgItem(IDC_CHECK_NOR_RESET)))->GetCheck();
	//GetDlgItem(IDC_BUTTON_OPEN_CHTFILE)->EnableWindow(bCheck);
	//GetDlgItem(IDC_EDIT_OPEN_CHTFILE)->EnableWindow(bCheck);			
	m_pInfo->bReset = TRUE;
	m_pInfo->bCheat = TRUE;	
	m_pInfo->bSaveNow = FALSE;
}



void CRomEditDlg::OnSelchangeComboSavetype() 
{
	// TODO: Add your control notification handler code here
		//显示存档类型
	int i=m_CbSaveType.GetCurSel();
	switch(i)
	{
		case 3:
			m_pInfo->uiSaverMode = UNKNOW;
			break;
		case 0:
			m_pInfo->uiSaverMode = SRAM_TYPE;
			break;
		case 1:
			m_pInfo->uiSaverMode = EEPROM_TYPE;
			break;
		case 2:
			m_pInfo->uiSaverMode = FLASH_TYPE;
			break;
	}
}

void CRomEditDlg::OnSelchangeComboSavesize() 
{
	// TODO: Add your control notification handler code here
		int i=m_CbSaveType.GetCurSel();
		switch(i)
		{
			case 0:
				m_pInfo->uiSaverSize = 0;
				break;	
			case 1:
				m_pInfo->uiSaverSize = 0x2000;
				break;
			case 2:
				m_pInfo->uiSaverSize = 0x4000;
				break;
			case 3:
				m_pInfo->uiSaverSize = 0x8000;
				break;
			case 4:
				m_pInfo->uiSaverSize = 0x10000;
				break;
			case 5:
				m_pInfo->uiSaverSize = 0x20000;
				break;
		}
}


PGOLDCHEAT CRomEditDlg::GetGoldDataFormTree(WORD &w_CheatSize)
{
	PGOLDCHEAT pGoldCheat = NULL;
	pGoldCheat = new GOLDCHEAT[dw_CheatCounter];
	w_CheatSize = dw_CheatCounter;
	GOLDCHEAT *pTempGoldCheat = NULL;
	GOLDCODEITEM *pTempItem=NULL;
	GOLDDATA *pGoldData=NULL;
	GOLDDATA *pTempGoldData=NULL;	
	HTREEITEM hCurrent = m_CheatTree.GetNextItem(NULL,TVGN_CHILD);
	TVITEM item;
	TCHAR szText[1024];
	int sub1=0;
	int sub2=0;
	int sub3=0;
	int index1=0;
	int index2=0;
	int index3=0;
	while(hCurrent != NULL)
	{
		//得到能源
		item.hItem = hCurrent;
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.pszText = szText;
		item.cchTextMax = 1024;
		memset(item.pszText,0,1024);
		m_CheatTree.GetItem(&item);
		pTempGoldCheat = &pGoldCheat[index1];
		memcpy(pTempGoldCheat->Totalname,item.pszText,12);
		//能源下面有四项
		pTempGoldCheat->SumofItem = GetTreeItemCounter(hCurrent);
		pTempGoldCheat->pItem = new GOLDCODEITEM[pTempGoldCheat->SumofItem];
		HTREEITEM hSubItem1 = m_CheatTree.GetNextItem(hCurrent,TVGN_CHILD);
		index2 = 0;
		while(hSubItem1!=NULL)
		{
			item.hItem = hSubItem1;
			item.mask = TVIF_TEXT | TVIF_HANDLE;
			item.pszText = szText;
			item.cchTextMax = 1024;
			memset(item.pszText,0,1024);
			m_CheatTree.GetItem(&item);
			pTempItem = &(pTempGoldCheat->pItem[index2]);
			memcpy(pTempItem->ItemName,item.pszText,14);
			pTempItem->ItemSize = GetTreeItemCounter(hSubItem1);
			pTempItem->pGlodData =new GOLDDATA[pTempItem->ItemSize];
			HTREEITEM hSubItem2 = m_CheatTree.GetNextItem(hSubItem1,TVGN_CHILD);
			index3 = 0;
			while(hSubItem2!=NULL)
			{
				item.hItem = hSubItem2;
				item.mask = TVIF_TEXT | TVIF_HANDLE;
				item.pszText = szText;
				item.cchTextMax = 1024;
				memset(item.pszText,0,1024);
				m_CheatTree.GetItem(&item);
				pTempGoldData = &pTempItem->pGlodData[index3];

				CString str = item.pszText;
				GOLD_DATA *p = GetGoldData(str);
				pTempGoldData->Address = p->m_dwAddress;
				pTempGoldData->Data = p->m_dwData;
				pTempGoldData->DataType = p->m_dataType;
				pTempGoldData->LoopCounter = p->m_loopCounter;
				delete p;

				hSubItem2 = m_CheatTree.GetNextItem(hSubItem2, TVGN_NEXT);
				index3++;
			}
			hSubItem1 = m_CheatTree.GetNextItem(hSubItem1, TVGN_NEXT);
			index2++;
		}
		hCurrent = m_CheatTree.GetNextItem(hCurrent, TVGN_NEXT);
		index1++;
	}
	
	return pGoldCheat;
}

DWORD CRomEditDlg::GetTreeItemCounter(HTREEITEM hItem)
{
	DWORD counter=0;
	HTREEITEM hCurrent = m_CheatTree.GetNextItem(hItem,TVGN_CHILD);
	while(hCurrent!=NULL)
	{
		counter++;
		hCurrent = m_CheatTree.GetNextItem(hCurrent, TVGN_NEXT);
	}
	return counter;
}

GOLD_DATA *CRomEditDlg::GetGoldData(CString str)
{
	GOLD_DATA *pGoldData = new GOLD_DATA;
	CString str1,str2,str3,str4;
	int index = 0;
	str1 = str.Left(str.Find(','));
	str = str.Right(str.GetLength()-str.Find(',')-1);
	str1 = str1.Right(str1.GetLength()-str1.ReverseFind('=')-1);

	str2 = str.Left(str.Find(','));
	str = str.Right(str.GetLength()-str.Find(',')-1);
	str2 = str2.Right(str2.GetLength()-str2.ReverseFind('=')-1);

	str3 = str.Left(str.Find(','));
	str = str.Right(str.GetLength()-str.Find(',')-1);
	str3 = str3.Right(str3.GetLength()-str3.ReverseFind('=')-1);

	str4 = str;
	str = str.Right(str.GetLength()-str.Find(',')-1);
	str4 = str4.Right(str4.GetLength()-str4.ReverseFind('=')-1);
	
	char *ctemp=NULL;
	pGoldData->m_dwAddress = strtoul(str1,&ctemp,16);
	pGoldData->m_dwData = strtoul(str2,&ctemp,16);
	pGoldData->m_dataType = strtoul(str3,&ctemp,16);
	pGoldData->m_loopCounter = strtoul(str4,&ctemp,16);

	return pGoldData;
}

void CRomEditDlg::OnSelchangeComboSavepatch() 
{
	// TODO: Add your control notification handler code here
		int i=m_CbSavepatchType.GetCurSel();
		switch(i) 
		{
			case 1:  
				m_pInfo->SavePatch = 0;
				break;
			case 2:
				m_pInfo->SavePatch = 1;
				break;
			default:
				m_pInfo->SavePatch = -1;
				break;
		}

}

void CRomEditDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CRomEditDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CRomEditDlg::GetZipState()
{
	return m_bZip;
}

void CRomEditDlg::SetZipState(BOOL bState)
{
	m_bZipButState = bState;
}

void CRomEditDlg::SetXCodeState(BOOL bState)
{
	m_bXCodeState = bState;
}
void CRomEditDlg::OnCheckZip() 
{
	// TODO: Add your control notification handler code here
	CButton *pBut = (CButton *)GetDlgItem(IDC_CHECK_ZIP);
	if(pBut->GetCheck())
		m_bZip = TRUE;
	else
		m_bZip = FALSE;
}

void CRomEditDlg::OnCheckNorSavenow() 
{
	// TODO: Add your control notification handler code here
	m_pInfo->bReset = FALSE;
	m_pInfo->bCheat = FALSE;	
	m_pInfo->bSaveNow = TRUE;
}

void CRomEditDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	
}
