// GB2Big5.cpp: implementation of the CGB2Big5 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GB2Big5.h"
#include "gb_10.h"
#include "big5_10.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGB2Big5::CGB2Big5()
{

}

CGB2Big5::~CGB2Big5()
{

}

void CGB2Big5::Initial()
{
	//locate the resource of "GBTOBIG" 定位资源 "GBTOBIG"
	hRes=FindResource(NULL,"GBTOBIG",RT_RCDATA);//IDR_GBTOBIG
	hGBTable=LoadResource(0,hRes);
	pGBTable=(LPWORD)LockResource(hGBTable);
	if(pGBTable==NULL)
		AfxMessageBox("fail to load GB2BIG Resource");

	//locate the resource of "BIGTOGB" 定位资源 "BIGTOGB"
	hRes=FindResource(NULL,"BIGTOGB",RT_RCDATA);
	hBIGTable=LoadResource(0,hRes);
	pBIGTable=(LPWORD)LockResource(hBIGTable);
	if(pBIGTable==NULL)
		AfxMessageBox("fail to load Big2GB Resource");
}

void CGB2Big5::Close()
{
	UnlockResource(hGBTable); 
	FreeResource(hGBTable);   	
	UnlockResource(hBIGTable); 
	FreeResource(hBIGTable);   	
}


CString CGB2Big5::Gb2Big5(LPCTSTR str)
{
	CString strRet ;
	BYTE* SourceSTR = (BYTE* )str;
	int len = strlen(str);
	int i=0;
	if(len)
	{	
		BYTE* DestSTR=new BYTE[len+1];
		do
		{
			// is English 是英文字符
			if(SourceSTR[i]<0xA1||(SourceSTR[i+1]<0xA1)) 
			{
				DestSTR[i]=SourceSTR[i];
				i++;
			}
			else if(SourceSTR[i]>0xA1&&SourceSTR[i]<0xB0) //是GB2312的汉字码
			{
				DestSTR[i+1]=HIBYTE(pGBTable[(SourceSTR[i]-0xA1)*0x5E+SourceSTR[i+1]-0xA1]);
				DestSTR[i]=LOBYTE(pGBTable[(SourceSTR[i]-0xA1)*0x5E+SourceSTR[i+1]-0xA1]);
				i+=2;
			}
			else
			{
				DestSTR[i+1]=HIBYTE(pGBTable[(SourceSTR[i]-0xA7)*0x5E+SourceSTR[i+1]-0xA1]);
				DestSTR[i]=LOBYTE(pGBTable[(SourceSTR[i]-0xA7)*0x5E+SourceSTR[i+1]-0xA1]);
				i+=2;
			}
		}while(i<len);
        DestSTR[i]='\0';
		strRet = DestSTR ;
		delete DestSTR;
	}
	return strRet ;
}

CString CGB2Big5::Big52Gb(LPCTSTR str)
{
	CString strRet ;
	BYTE* SourceSTR = (BYTE* )str;
	int len = strlen(str);
	int i=0;
	if(len)
	{	
		BYTE* DestSTR=new BYTE[len+1];
		do
		{
			// is English 是英文字符
			if(SourceSTR[i]<0xA1||(SourceSTR[i+1]<0x40)) 
			{
				DestSTR[i]=SourceSTR[i];
				i++;
			}
			else //是BIG5的汉字码
			{
				DestSTR[i+1]=HIBYTE(pBIGTable[(SourceSTR[i]-0xA1)*0xBF+SourceSTR[i+1]-0x40]);
				DestSTR[i]=LOBYTE(pBIGTable[(SourceSTR[i]-0xA1)*0xBF+SourceSTR[i+1]-0x40]);
				i+=2;
			}
		}while(i<len);
		DestSTR[i]='\0';
		strRet = DestSTR ;
		delete DestSTR;
	}
	return strRet ;
}

void CGB2Big5::GetChinese(LPCTSTR str, BYTE *pBB,int &size)
{
	int len = strlen(str);
	int i=0;
	BYTE *pp = (BYTE*)str ;
	if(len)
	{
		do
		{
			if(pp[i]<0xA1) 
			{
				i++;
			}
			else //是的汉字码
			{
				pBB[size*2+1]=pp[i+1];
				pBB[size*2]=pp[i];
				i+=2;
				size ++ ;
			}
		}while(i<len);
	}
}

void CGB2Big5::GetDianZheng(BYTE *neima, BYTE *dianzhen,UINT type)
{
	int  wei,qu,location;
	int	 pos =0 , bit = 0;
	if(type ==2 )//繁体 
	{
		while((*neima != 0xFF)&&(*neima != 0x00))
		{
			qu = *(neima++)-0xA1; 
			wei =*(neima++);
			if(wei<0x7f)
				wei = wei -0x40;
			else if(wei >0xa0) 
				wei = wei - 0x62 ;
			location = (qu*157+wei)*13 ;
			for(bit=0;bit<13;bit++)
				dianzhen[pos*13+bit] = big5_10[location+bit] ;	
			pos++ ;
		}
	}
	else//简体
	{
		while((*neima != 0xFF)&&(*neima != 0x00))
		{
			qu = (*(neima++)-0x20)&0x7F; 
			wei =(*(neima++)-0x20)&0x7F; 
			location = ((qu-1)*94+wei)*13 ;
			for(bit=0;bit<13;bit++)
				dianzhen[pos*13+bit] = gb_10[location+bit] ;	
			pos++ ;
		}
	}
}

void CGB2Big5::AddLdrSpecialName(BYTE *pLdr, CStringArray &strArray,UINT type)
{
//字库:0x400 字库内码索引 , 不用的地方全部写0xFF(0x6000-0x6400)
//点阵:0x1A00 字库点阵索引, 不用的地方全部写0xFF(0x6400-0x7A00)
//游戏名称链表：0x1A00 -0x2000 .(中文或者繁体中文)特殊位0x18， 1字节长度 后面长度是游戏名。
	CString str;
	int strLen , iTemp ;
	BYTE strTemp[256] , *pFind = NULL ;
	WORD wUni ,*pWT;
	BYTE *pUnicode = pLdr + ZIKUBASE + 0x0 ;
	int uiUnicode = 0x400 , uiUniIndex = 0;
	BYTE *pDotCode = pLdr + ZIKUBASE +0x400 ;
	int uiDotcode = 0x1600 , uiDotIndex = 0 ;
	BYTE *pNameList = pLdr + ZIKUBASE + 0x1A00 ;
	int uiNameList = 0x600 , uiNameIndex = 0;
	int size = strArray.GetSize();
	if(size < 1)
		return ;
	memset(pUnicode,0xFF,0x2000);
	for(int i =0 ;i<size ;i++)
	{
		str = strArray.GetAt(i);
		//首先要将特殊列表填入文件名列表
		strLen = str.GetLength();
		pNameList[uiNameIndex] = 0x18 ;
		pNameList[uiNameIndex+1] = strLen ;
		uiNameIndex+=2;
		memcpy(pNameList+uiNameIndex,str.GetBuffer(strLen),strLen);
		uiNameIndex+=strLen ;
		str.ReleaseBuffer(strLen);
		//将其中的中文字符加入字库
		memset(strTemp,0,256);
		iTemp = 0 ;
		GetChinese(str,strTemp,iTemp);
		pWT = (WORD*)strTemp ;
		for(int j=0;j<iTemp;j++)
		{
			wUni = pWT[j] ;
			pFind = FindMotif(pUnicode,0x400,(BYTE*)&wUni,2);
			if(pFind)
				continue ;
			if(uiUniIndex>0x362) //限制为字库大小
				break;
			pUnicode[uiUniIndex] = strTemp[j*2] ;
			pUnicode[uiUniIndex+1] = strTemp[j*2+1] ;
			uiUniIndex += 2 ;
		}
	}
	//字库计算完毕，
	GetDianZheng(pUnicode,pDotCode,type);
}

BYTE * CGB2Big5::FindMotif(BYTE *Buffer, DWORD BufferSize, BYTE *Motif, DWORD MotifSize)
{
	WORD * pWord = (WORD *)Motif ;
	WORD * pBuf = (WORD *)Buffer ;
	//Arithmatic from Dadycool
	for(int i=0;i<0x200;i++)
	{
		if(pBuf[i] == *pWord)
			break;
	}
	if(i==0x200)
		return NULL ;
	else
		return Buffer+i*2 ;
/*	BYTE* find = Buffer;
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
	*/
}