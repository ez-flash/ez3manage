// DlgOSSelect.cpp : implementation file
//

#include "stdafx.h"
#include "new_ezmanage.h"
#include "DlgOSSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOSSelect dialog


CDlgOSSelect::CDlgOSSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOSSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOSSelect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}


void CDlgOSSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOSSelect)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOSSelect, CDialog)
	//{{AFX_MSG_MAP(CDlgOSSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOSSelect message handlers

BOOL CDlgOSSelect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	((CButton *)GetDlgItem(IDC_RADIO_GBA_OS))->SetCheck(TRUE);
	bNds = FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOSSelect::OnOK() 
{
	// TODO: Add extra validation here
	
	if(((CButton *)GetDlgItem(IDC_RADIO_GBA_OS))->GetCheck() == TRUE)
		bNds = FALSE;
	else if(((CButton *)GetDlgItem(IDC_RADIO_NDSOS))->GetCheck() == TRUE)
		bNds =  TRUE;
	
	CDialog::OnOK();
}

BOOL CDlgOSSelect::GetLoader()
{
	return bNds;
}