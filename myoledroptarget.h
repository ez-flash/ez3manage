// MyOleDropTarget.h: interface for the CMyOleDropTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYOLEDROPTARGET_H__757971F0_CD1B_4793_A9A5_A2B24C8D430B__INCLUDED_)
#define AFX_MYOLEDROPTARGET_H__757971F0_CD1B_4793_A9A5_A2B24C8D430B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxole.h>

class CMyOleDropTarget  : public COleDropTarget
{
public:
	CMyOleDropTarget();
	virtual ~CMyOleDropTarget();
public:
    virtual DROPEFFECT OnDragEnter( CWnd* pWnd, COleDataObject*
pDataObject, DWORD dwKeyState, CPoint point );
    
    virtual DROPEFFECT OnDragOver( CWnd* pWnd, COleDataObject*
pDataObject, DWORD dwKeyState, CPoint point );
    
	virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
		DROPEFFECT dropEffect, CPoint point);

};

#endif // !defined(AFX_MYOLEDROPTARGET_H__757971F0_CD1B_4793_A9A5_A2B24C8D430B__INCLUDED_)
