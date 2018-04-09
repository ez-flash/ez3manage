// PageFlash.cpp : implementation file
//

#include "stdafx.h"
#include "New_EZManage.h"
#include "PageFlash.h"
#include "New_EZManageDlg.h"
#include <afxole.h>
#include "RomEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageFlash dialog

//3.Tab
extern CString m_gStrConfig;					//烧录配置
extern CString m_gStrNorFlashPage;				//256M Bits EZ Flash 管理
extern CString m_gStrNorIndex;					//序号
extern CString m_gStrNorRomName;				//ROM名
extern CString	m_gStrNorRomSize;				//ROM大小
extern CString	m_gStrNorRomEdit;				//ROM信息编辑
extern CString m_gStrNorRomState;				//ROM状态
extern CString m_gStrAddRom;					//添加Rom
extern CString m_gStrDelRom;					//删除Rom
extern CString m_gStrBackupRom;					//备份Rom
extern CString m_gStrRefresh;					//刷新Rom
extern CString m_gStrFormatRom;					//格式化Rom
extern CString m_gStrWriteRom;					//烧录Rom
CPageFlash::CPageFlash(CWnd* pParent /*=NULL*/)
	: CDialog(CPageFlash::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageFlash)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPageFlash::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageFlash)
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageFlash, CDialog)
	//{{AFX_MSG_MAP(CPageFlash)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_ADD_ROM, OnButtonAddRom)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_ROM, OnButtonDeleteRom)
	ON_BN_CLICKED(IDC_BUTTON_BACK_ROM, OnButtonBackRom)
	ON_BN_CLICKED(IDC_BUTTON_REFLASH_ROM, OnButtonReflashRom)
	ON_BN_CLICKED(IDC_BUTTON_FORMAR_ROM, OnButtonFormarRom)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_ROM, OnButtonWriteRom)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageFlash message handlers

BOOL CPageFlash::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rc(0, 0, 0, 0);

	GetParent()->GetClientRect(&rc);
	((CTabCtrl*)GetParent())->AdjustRect(false, &rc); 

	MoveWindow(&rc);

	m_ListCtrl.InsertColumn(0, m_gStrNorIndex,   LVCFMT_LEFT, 50);
	m_ListCtrl.InsertColumn(1, m_gStrNorRomName,   LVCFMT_LEFT, 150);
	m_ListCtrl.InsertColumn(2, m_gStrNorRomSize,   LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(3, m_gStrNorRomEdit,   LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(4, m_gStrNorRomState,   LVCFMT_LEFT, 100);

	CButton *pBut = (CButton *)GetDlgItem(IDC_BUTTON_ADD_ROM);
	pBut->SetWindowText(m_gStrAddRom);

	pBut = (CButton *)GetDlgItem(IDC_BUTTON_DELETE_ROM);
	pBut->SetWindowText(m_gStrDelRom);

	pBut = (CButton *)GetDlgItem(IDC_BUTTON_BACK_ROM);
	pBut->SetWindowText(m_gStrBackupRom);

	pBut = (CButton *)GetDlgItem(IDC_BUTTON_REFLASH_ROM);
	pBut->SetWindowText(m_gStrRefresh);

	pBut = (CButton *)GetDlgItem(IDC_BUTTON_FORMAR_ROM);
	pBut->SetWindowText(m_gStrFormatRom);

	pBut = (CButton *)GetDlgItem(IDC_BUTTON_WRITE_ROM);
	pBut->SetWindowText(m_gStrWriteRom);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageFlash::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect r;
	GetClientRect(&r);
	int width = r.Width();
	int height = r.Height();
	
	CButton *pButton=NULL;
	if(m_ListCtrl)
	{
		m_ListCtrl.MoveWindow(0,0,width,height/10*9);
		m_ListCtrl.Invalidate(TRUE);
	}
	pButton = (CButton *)GetDlgItem(IDC_BUTTON_ADD_ROM);
	if(pButton)
	{
		pButton->MoveWindow(0,height/10*9+height/20,width/6,height/20);
		pButton->Invalidate(TRUE);
	}
	pButton = (CButton *)GetDlgItem(IDC_BUTTON_DELETE_ROM);
	if(pButton)
	{
		pButton->MoveWindow(width/6,height/10*9+height/20,width/6,height/20);
		pButton->Invalidate(TRUE);
	}	
	
	pButton = (CButton *)GetDlgItem(IDC_BUTTON_BACK_ROM);
	if(pButton)
	{
		pButton->MoveWindow(width/6*2,height/10*9+height/20,width/6,height/20);
		pButton->Invalidate(TRUE);
	}	
	pButton = (CButton *)GetDlgItem(IDC_BUTTON_REFLASH_ROM);
	if(pButton)
	{
		pButton->MoveWindow(width/6*3,height/10*9+height/20,width/6,height/20);
		pButton->Invalidate(TRUE);
	}
	pButton = (CButton *)GetDlgItem(IDC_BUTTON_FORMAR_ROM);
	if(pButton)
	{
		pButton->MoveWindow(width/6*4,height/10*9+height/20,width/6,height/20);
		pButton->Invalidate(TRUE);
	}		
	pButton = (CButton *)GetDlgItem(IDC_BUTTON_WRITE_ROM);
	if(pButton)
	{
		pButton->MoveWindow(width/6*5,height/10*9+height/20,width/6,height/20);
		pButton->Invalidate(TRUE);
	}
	
}


void CPageFlash::OnButtonAddRom() 
{
	// TODO: Add your control notification handler code here
	CNew_EZManageDlg *pDlg = (CNew_EZManageDlg *)GetTopLevelParent();
	pDlg->PostMessage(WM_ADDROM,0,0);
}



void CPageFlash::OnButtonDeleteRom() 
{
	// TODO: Add your control notification handler code here
	int i = m_ListCtrl.GetItemCount();
	BOOL b = m_ListCtrl.DeleteItem(i-1);
	m_ListCtrl.Invalidate(TRUE);
	CNew_EZManageDlg *pDlg = (CNew_EZManageDlg *)GetTopLevelParent();
	pDlg->PostMessage(WM_DELROM,0,0);
}

void CPageFlash::OnButtonBackRom() 
{
	// TODO: Add your control notification handler code here
	CNew_EZManageDlg *pDlg = (CNew_EZManageDlg *)GetTopLevelParent();
	pDlg->PostMessage(WM_BACKROM,0,0);
}

void CPageFlash::OnButtonReflashRom() 
{
	// TODO: Add your control notification handler code here
	CNew_EZManageDlg *pDlg = (CNew_EZManageDlg *)GetTopLevelParent();
	pDlg->PostMessage(WM_FRESH,0,0);
}

void CPageFlash::OnButtonFormarRom() 
{
	// TODO: Add your control notification handler code here
	CNew_EZManageDlg *pDlg = (CNew_EZManageDlg *)GetTopLevelParent();
	pDlg->PostMessage(WM_FORMATROM,0,0);
}

void CPageFlash::OnButtonWriteRom() 
{
	// TODO: Add your control notification handler code here
	CNew_EZManageDlg *pDlg = (CNew_EZManageDlg *)GetTopLevelParent();
	pDlg->PostMessage(WM_WRITEROM,0,0);
}

void CPageFlash::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CNew_EZManageDlg *pDlg = (CNew_EZManageDlg *)GetTopLevelParent();
	GAMEINFOLIST *pList = pDlg->pGameInfoList;
	PGameRomInfo pInfo;
	if(pList->GetSize()<pNMListView->iItem)
		return ;
	pInfo = pList->GetAt(pNMListView->iItem);
	if(pInfo->uiWhere)
	{
		CRomEditDlg dlg;
		dlg.SetZipState(FALSE);
		if(pInfo->pXCode)
			dlg.SetXCodeState(TRUE);
		else
			dlg.SetXCodeState(FALSE);
		dlg.SetGameInfo(pInfo);
		dlg.DoModal();
	}
	*pResult = 0;
}

void CPageFlash::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here

}
