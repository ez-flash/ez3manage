// DlgLanguageSelect.cpp : implementation file
//

#include "stdafx.h"
#include "new_ezmanage.h"
#include "DlgLanguageSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLanguageSelect dialog


CDlgLanguageSelect::CDlgLanguageSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLanguageSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLanguageSelect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgLanguageSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLanguageSelect)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLanguageSelect, CDialog)
	//{{AFX_MSG_MAP(CDlgLanguageSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLanguageSelect message handlers

BOOL CDlgLanguageSelect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	((CButton *)GetDlgItem(IDC_RADIO_CHINESE))->SetCheck(TRUE);
	strLanguage = "简体中文";
 	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
CString CDlgLanguageSelect::GetSelectLanguage()
{
	return strLanguage;
}
void CDlgLanguageSelect::OnOK() 
{
	// TODO: Add extra validation here
	if(((CButton *)GetDlgItem(IDC_RADIO_CHINESE))->GetCheck() == TRUE)
		strLanguage = "简体中文";
	else if(((CButton *)GetDlgItem(IDC_RADIO_ENGLISH))->GetCheck() == TRUE)
		strLanguage =  "English";
	
	CDialog::OnOK();
}
