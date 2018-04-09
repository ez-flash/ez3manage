// ChtFileInfo.h: interface for the CChtFileInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHTFILEINFO_H__265F96C1_3F3C_4659_8BAC_63D73A3A48EE__INCLUDED_)
#define AFX_CHTFILEINFO_H__265F96C1_3F3C_4659_8BAC_63D73A3A48EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>

typedef struct _CHT_DATA{
	CString		strFunction;
	CString		strSetup;		//金手指描述符
	DWORD		dw_Address;		//地址
	DWORD		dw_Data;		//数据
	BYTE		m_dataType;		//数据类型
	DWORD		loopcounter;	//是否循环
	BOOL		m_bUsePre;		//该变量表示该数据是否同前一个数据是一组的	
	_CHT_DATA	*pChtData;	
}CHT_DATA, *PCHT_DATA;
typedef CArray<PCHT_DATA,PCHT_DATA> CHT_DATA_ARRAY;

/*class CChtFileInfo负责cht文件的读取*/
class CChtFileInfo  
{
public:
	CChtFileInfo(CString str=_T(""));
	virtual ~CChtFileInfo();
	CList<PCHT_DATA,PCHT_DATA> m_GoldList;
	void DeleteBuffer();
	void ClearData();
private:
	CString m_strChtFileName;
	CString strName;
	CString strSystem;
	CString	strText;
	int			m_LineCounter;
	int			CounterEqualSign(CString strData);
	int			CountersemicolonSign(CString strData);
	void		Parse_Data(CHT_DATA_ARRAY &cht_data_array,CString strData);
	CHT_DATA	*Parse_Data1(CString strData);//没有分号的情况
	void		Parse_Data2(CHT_DATA_ARRAY &cht_data_array,CString strData);//有分号的情况
	CString		GetFunction(CString str);
public:
	void SetChtFileName(CString str);
	CString Getdescriptor(int index=0);//返回金手指的描述符号
	CString GetSetUp(int index=0);
	CString GetName();
	CString GetSystem();
	CString GetText();
	void	GetData();
	CList<PCHT_DATA,PCHT_DATA> *ReturnList();//该list指针指向包含所有cht文件中的金手指数据的链表
public:
	CString ReadLine(int index=0);//读'['和'['之间的内容
	int GetLineCounter();		//返回行数
	CString GetLastLine();
	BYTE	*pData;
	BYTE	*m_pData;
	DWORD	m_length;
	void RemoveAllItem();
};

#endif // !defined(AFX_CHTFILEINFO_H__265F96C1_3F3C_4659_8BAC_63D73A3A48EE__INCLUDED_)
