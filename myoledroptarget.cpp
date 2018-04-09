// MyOleDropTarget.cpp: implementation of the CMyOleDropTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "new_ezmanage.h"
#include "MyOleDropTarget.h"
#include "New_EZManageDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyOleDropTarget::CMyOleDropTarget()
{

}

CMyOleDropTarget::~CMyOleDropTarget()
{

}

DROPEFFECT CMyOleDropTarget::OnDragEnter( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
    TRACE( "DRAG Over\n" );
	DROPEFFECT Ret=DROPEFFECT_NONE;
	CRect rect;
	CNew_EZManageDlg *pDlg = (CNew_EZManageDlg *)pWnd;
	
	switch(pDlg->m_Tab.GetCurSel())
	{
	case 0:
		pDlg->m_PageFlash.m_ListCtrl.GetClientRect(&rect);
		pDlg->m_PageFlash.m_ListCtrl.MapWindowPoints(pDlg,&rect);
		if(rect.PtInRect(point))
			Ret = DROPEFFECT_MOVE;
		break;
	case 1:
		pDlg->m_PageDisk.m_DiskListCtrl.GetClientRect(&rect);
		pDlg->m_PageDisk.m_DiskListCtrl.MapWindowPoints(pDlg,&rect);
		if(rect.PtInRect(point))
			Ret = DROPEFFECT_MOVE;
		break;
	
	}
    
    return Ret;
}

DROPEFFECT CMyOleDropTarget::OnDragOver( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
	 TRACE( "DRAG Over\n" );
	DROPEFFECT Ret=DROPEFFECT_NONE;
	CRect rect;
	CNew_EZManageDlg *pDlg = (CNew_EZManageDlg *)pWnd;
	
	switch(pDlg->m_Tab.GetCurSel())
	{
	case 0:
		pDlg->m_PageFlash.m_ListCtrl.GetClientRect(&rect);
		pDlg->m_PageFlash.m_ListCtrl.MapWindowPoints(pDlg,&rect);
		if(rect.PtInRect(point))
			Ret = DROPEFFECT_MOVE;
		break;
	case 1:
		pDlg->m_PageDisk.m_DiskListCtrl.GetClientRect(&rect);
		pDlg->m_PageDisk.m_DiskListCtrl.MapWindowPoints(pDlg,&rect);
		if(rect.PtInRect(point))
			Ret = DROPEFFECT_MOVE;
		break;
	
	}
    if(Ret == DROPEFFECT_NONE)
	{
		return Ret;
	}
    return Ret;
}
BOOL CMyOleDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
		DROPEFFECT dropEffect, CPoint point)
{
	TRACE( "DRAG Drop\n" );
	BOOL Ret=FALSE;
	CRect rect;
	CNew_EZManageDlg *pDlg = (CNew_EZManageDlg *)pWnd;
	if(!pDlg->m_Tab.IsWindowEnabled())
		return FALSE;
	switch(pDlg->m_Tab.GetCurSel())
	{
	case 0:
		pDlg->m_PageFlash.m_ListCtrl.GetClientRect(&rect);
		pDlg->m_PageFlash.m_ListCtrl.MapWindowPoints(pDlg,&rect);
		if(rect.PtInRect(point))
		{
			Ret = TRUE;
			if(pDlg->m_Tab.IsWindowEnabled())
				pDlg->AddDropDataToNorList(pDataObject);
		}
		break;
	case 1:
		pDlg->m_PageDisk.m_DiskListCtrl.GetClientRect(&rect);
		pDlg->m_PageDisk.m_DiskListCtrl.MapWindowPoints(pDlg,&rect);
		if(rect.PtInRect(point))
		{
			Ret = TRUE;
			if(pDataObject->IsDataAvailable(CF_TEXT))
				pDlg->AddDropDataToDiskList(pDataObject);
			if(pDataObject->IsDataAvailable(CF_HDROP))
				pDlg->AddFileToDiskList(pDataObject);
			//if(!pDataObject->IsDataAvailable(CF_HDROP))
			//	return FALSE;
		}
		break;
	}
    
    return Ret;
};