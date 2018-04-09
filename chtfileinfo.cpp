// ChtFileInfo.cpp: implementation of the CChtFileInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChtFileInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChtFileInfo::CChtFileInfo(CString str)
{
	pData = NULL;
	m_strChtFileName = str;
	m_LineCounter=0;
	m_GoldList.RemoveAll();
	m_pData = NULL;
	CFile file;
	int counter=0;
	if(m_strChtFileName.GetLength()<=0)
		return ;
	if(!file.Open(m_strChtFileName,CFile::modeRead))
		return ;
	m_length = file.GetLength();
	m_pData = new BYTE[m_length];
	if(m_length >0)
	{
		//pData = new BYTE[m_length ];
		//memset(pData,0,file.GetLength());
		file.Read(m_pData ,m_length);
		file.Close();
	}
	m_LineCounter= GetLineCounter();
}

CChtFileInfo::~CChtFileInfo()
{	
	if(pData)
	{
		delete []pData;
		pData = NULL;
	}
	if(m_pData)
	{
		delete []m_pData;
		m_pData = NULL;
	}
	m_LineCounter=0;
}

void CChtFileInfo::DeleteBuffer()
{
	if(m_pData)
	{
		delete []m_pData;
		m_pData = NULL;
	}
}
void CChtFileInfo::SetChtFileName(CString str)
{
	m_strChtFileName = str;
	m_pData = NULL;
	CFile file;
	int counter=0;
	if(m_strChtFileName.GetLength()<=0)
		return ;
	if(!file.Open(m_strChtFileName,CFile::modeRead))
		return ;
	m_length = file.GetLength();

	if(m_length>0)
	{
		m_pData = new BYTE[m_length];
		//memset(pData,0,file.GetLength());
		file.Read(m_pData ,m_length);
		file.Close();
	}
	m_LineCounter= GetLineCounter();
}
 
void CChtFileInfo::ClearData()
{
	if(m_pData)
	{
		delete []m_pData;
		m_pData = NULL;
	}
}
int CChtFileInfo::GetLineCounter()
{
	CFile file;
	BYTE *pTemp;
	int counter=0;
	if(m_strChtFileName.GetLength()<=0)
		return -1;
	//if(!file.Open(m_strChtFileName,CFile::modeRead))
	//	return -1;
	//if(file.GetLength()>0)
	{
	//	pData = new BYTE[file.GetLength()];
	//	memset(pData,0,file.GetLength());
	//	file.Read(pData,file.GetLength());
		pTemp = m_pData;
		for(DWORD i=0;i<=m_length;i++)
		{
			if((*pTemp == '[') && (i==0))
			{
				counter++;
			}
			else if((*pTemp == 0x0A) && (*(pTemp+1) == '['))
			{
				counter ++;
			}
			pTemp++;
		}
	}
	//file.Close();
	if(pData)
	{
		delete pData;
		pData = NULL;
	}
	return counter-1;
}

CString CChtFileInfo::GetLastLine()
{
	CString str = _T("");
	int Line_counter = m_LineCounter;
	CFile file;
	BYTE *pStart = NULL;
	int counter=0;
	DWORD offset=0;
	
	//if(!file.Open(m_strChtFileName,CFile::modeRead))
	//	return -1;
	//DWORD file_len = file.GetLength();
	//if(file_len>0)
	{
	//	pData = new BYTE[file_len];
	//	memset(pData,0,file.GetLength());
	//	file.Read(pData,file.GetLength());
		pStart = m_pData;//file.GetLength
		for(DWORD i=0;i<=m_length;i++)
		{
			if(*pStart++ == '[')
			{
				counter ++;
				if(counter == (Line_counter+1))
					break;
			}
			offset++;
		}
	}
	//file.Close();
	//现在pTemp指向该行的'['
	int len=m_length-offset;;
	pStart--;
	if(len>0)
		memcpy(str.GetBuffer(len),pStart,len);
	if(pData)
	{
		delete pData;
		pData = NULL;
	}
	//file.Close();
	str.ReleaseBuffer(len);
	return str;
}
CString CChtFileInfo::ReadLine(int index)//读'['和'['之间的内容
{
	if(index<0 || index>m_LineCounter)
		return _T("");
	//CFile file;
	BYTE *pStart = NULL;
	int counter=0;
	CString str=_T("");
	//if(!file.Open(m_strChtFileName,CFile::modeRead))
	//	return -1;
	//if(file.GetLength()>0)
	{
	//	pData = new BYTE[file.GetLength()];
	//	memset(pData,0,file.GetLength());
	//	file.Read(pData,file.GetLength());
		pStart = m_pData;
		for(DWORD i=0;i<=m_length;i++)
			if(*pStart++ == '[')
			{
				if(index == counter)
					break;
				counter ++;
			}
	}
	//file.Close();
	//现在pTemp指向该行的'['
	int len=0;
	pStart--;
	BYTE *pEnd = pStart;
	do{
		len++;
		pEnd ++;
	}while(*pEnd != '[');
	if(len>0)
		memcpy(str.GetBuffer(len),pStart,len);
	if(pData)
	{
		delete pData;
		pData = NULL;
	}
	str.ReleaseBuffer(len);
	//file.Close();
	//str.Remove(0x0D);
	//str.Remove(0x0A);
	return str;
}


CString CChtFileInfo::GetName()
{
	CString strGameName;
	CString strLastLine = GetLastLine();
	int iStart	= strLastLine.Find(_T("Name="),0);
	int iEnd	= strLastLine.Find(_T("System="),0);
	strGameName = strLastLine.Mid(iStart,iEnd-iStart);
	strGameName.Remove(0x0A);
	strGameName.Remove(0x0D);
	strGameName.TrimLeft(_T("Name="));
	return strGameName;
}

CString CChtFileInfo::GetSystem()
{
	return _T("GBA");
}

CString CChtFileInfo::GetText()
{
	CString strText;
	CString strLastLine = GetLastLine();
	int iStart	= strLastLine.Find(_T("Text="),0);
	int iEnd = strLastLine.GetLength();
	strText = strLastLine.Mid(iStart,iEnd-iStart);
	strText .TrimLeft(_T("Text="));

	//strGameName.Remove(0x0A);
	//strGameName.Remove(0x0D);
	return strText;
}

CString CChtFileInfo::Getdescriptor(int index)
{
	CString str;
	if(index<0 || index > m_LineCounter)
		return _T("");
	str = ReadLine(index);
	int iStart	= str.Find('[');
	int iEnd	= str.Find(']');
	CString strFunction = str.Mid(iStart+1,iEnd-iStart-1);
	return strFunction; 
}

void CChtFileInfo::GetData()
{
	CString strDes,str,strData0;
	int n1;
	CHT_DATA_ARRAY cht_data_array;
	CHT_DATA *pcht_data;
	CHT_DATA *pTemp;
	CHT_DATA *pCht_dataPointer;
	int EqualSignNum = 0;
	int n=0;
	CString strTemp;
	int k=0;
	int counter=0;
	for(int index=0;index<m_LineCounter;index++)
	{
		strDes = ReadLine(index);
		//如果有多个等号
		EqualSignNum=CounterEqualSign(strDes);
		if(EqualSignNum>1)
		{	
			n = strDes.Find(0x0A,0);
			strDes = strDes.Right(strDes.GetLength()-n-1);
			k=0;
			counter=0;
			for(int i=0;i<EqualSignNum;i++)
			{
				//n = strDes.Find(0x0A,0);
				//str = strDes.Left(n-1);
				n = strDes.Find('=',0);
				n = strDes.Find('=',n+1);
				if(n>0)
				{
					str = strDes.Left(n-1);
					n1 = str.ReverseFind(0x0A);
					str = str.Left(n1);
					str.Remove(0x0D);
					str.Remove(0x0A);
				}
				else
				{
					strDes.Remove(0x0D);
					strDes.Remove(0x0A);
					str = strDes;
				}
				if(str.Find(_T(";"))>0)
				{
					//有等号,有分号的情况
					Parse_Data2(cht_data_array,str);
					if(cht_data_array.GetSize()>0)
					{
						pcht_data = cht_data_array.GetAt(0);
						pcht_data->strSetup= GetFunction(str); 
						strData0 = str ;
						pcht_data->strFunction = Getdescriptor(index);
						if(k==0)
							pcht_data->m_bUsePre = FALSE;
						else
							pcht_data->m_bUsePre = TRUE;
						strTemp = pcht_data->strSetup;
						pcht_data->strSetup.Format("%d-%s",counter,strTemp);
						str = pcht_data->strSetup;
						counter++;
					}
					pCht_dataPointer = pcht_data;
					for(k =1;k<cht_data_array.GetSize();k++)
					{
						pTemp = cht_data_array.GetAt(k);
						pTemp->strSetup= GetFunction(strData0); 
						pTemp->strFunction = Getdescriptor(index);
						pTemp->m_bUsePre = TRUE;
						strTemp = pTemp->strSetup;
						pTemp->strSetup = str;
						pTemp->pChtData=NULL;
						pCht_dataPointer->pChtData = pTemp;
						pCht_dataPointer = pTemp;
					}
					m_GoldList.AddTail(pcht_data);					
					cht_data_array.RemoveAll();
					strDes = strDes.Right(strDes.GetLength()-n1-1);
				}
				else
				{
					pcht_data = Parse_Data1(str);
					pcht_data->strSetup= GetFunction(str); 
					pcht_data->strFunction = Getdescriptor(index);
					if(i==0)
						pcht_data->m_bUsePre = FALSE;
					else
						pcht_data->m_bUsePre = TRUE;
					strTemp = pcht_data->strSetup;
	
					pcht_data->strSetup.Format("%d-%s",i+1,strTemp);
					m_GoldList.AddTail(pcht_data);
					strDes = strDes.Right(strDes.GetLength()-n1-1);
				}
			}
		}
		else
		{
			//如果有分号
			if(strDes.Find(_T(";"))>0)
			{
					Parse_Data2(cht_data_array,strDes);
					CString strSetup = GetFunction(strDes); 
					CString strFunction =Getdescriptor(index);;
					for(int i=0;i<cht_data_array.GetSize();i++)
					{
						pcht_data = cht_data_array.GetAt(i);
						if(i==0)
						{
							pcht_data->strSetup = strSetup;
							pcht_data->strFunction = strFunction;
						}
						pcht_data->m_bUsePre = FALSE;
						m_GoldList.AddTail(pcht_data);
					}
					cht_data_array.RemoveAll();
					
			}
			else
			{
				pcht_data = Parse_Data1(strDes);
				pcht_data->strSetup= GetFunction(strDes); 
				pcht_data->strFunction = Getdescriptor(index);
				pcht_data->m_bUsePre = FALSE;
				m_GoldList.AddTail(pcht_data);
			}
		}
	}
}

CString CChtFileInfo::GetFunction(CString str)
{
	str.Remove(0x0D);
	str.Remove(0x0A);
	int n = str.Find('=');
	int n1 = str.Find(']');
	CString strReturn;
	if(n1<0)
		strReturn = str.Left(n);
	else
		strReturn =  str.Mid(n1+1,n-n1-1);
	return strReturn;
}
void CChtFileInfo::Parse_Data(CHT_DATA_ARRAY &cht_data_array,CString strData)
{
	cht_data_array.RemoveAll();
}

CHT_DATA *CChtFileInfo::Parse_Data1(CString strData)//没有分号的情况
{
	CHT_DATA *pCht_data  =new CHT_DATA;
	pCht_data->pChtData = NULL;
	int n;
	CString strtemp;
	char *ctemp = NULL;
	
	int left = strData.Find('=',0);
	strData = strData.Right(strData.GetLength()-left-1);
	n = strData.Find(",");
	strtemp = strData.Left(n);
	pCht_data->dw_Address = strtoul(strtemp,&ctemp,16);
	if((pCht_data->dw_Address & 0x03000000)==0 || (pCht_data->dw_Address  & 0x02000000)==0)
	{
		if((pCht_data->dw_Address  & 0x00040000) !=0)
			pCht_data->dw_Address   +=0x03000000;
		else
			pCht_data->dw_Address   +=0x02000000;
	}
	strtemp = strtemp +_T(",");
	strData = strData.Right(strData.GetLength()-strtemp.GetLength());

	//计算逗号的个数
	char *ptemp = strData.GetBuffer(strData.GetLength());
	int comma_number=0;
	for(int loop=0;loop<strData.GetLength();loop++)
	{
		if(*(ptemp+loop)==',')
			comma_number++;
	}
	strData.ReleaseBuffer(strData.GetLength());
	if(comma_number==0)//1
	{
		pCht_data->dw_Data = strtoul(strData,&ctemp,16);
		pCht_data->m_dataType =1;
		pCht_data->loopcounter=1;
	}
	else if(comma_number==1)
	{
		//处理低位
		n = strData.Find(",");
		strtemp = strData.Left(n);
		pCht_data->dw_Data= strtoul(strtemp,&ctemp,16);
		//处理高位
		DWORD dwHigh;
		strtemp = strtemp +_T(",");
		strData= strData.Right(strData.GetLength()-strtemp.GetLength());
		dwHigh = strtoul(strData,&ctemp,16);
		dwHigh = dwHigh <<8;
		pCht_data->dw_Data =pCht_data->dw_Data +dwHigh ;
		pCht_data->m_dataType=2;
		pCht_data->loopcounter=1;
	}
	else if(comma_number==2)
	{
		n = strData.Find(",");
		strtemp = strData.Left(n);
		pCht_data->dw_Data = strtoul(strtemp,&ctemp,16);
		//处理高位
		DWORD dwHigh,dwHigh1;
		strtemp = strtemp +_T(",");
		strData= strData.Right(strData.GetLength()-strtemp.GetLength());
		n = strData.Find(",");
		strtemp = strData.Left(n);
		dwHigh = strtoul(strtemp,&ctemp,16);
		dwHigh = dwHigh <<8;
        strtemp = strtemp +_T(",");
        strData = strData.Right(strData.GetLength()-strtemp.GetLength());

		dwHigh1 = strtoul(strData,&ctemp,16);
        dwHigh1 = dwHigh1 <<16;

		pCht_data->dw_Data =pCht_data->dw_Data +dwHigh +dwHigh1 ;
		pCht_data->m_dataType =4;
		pCht_data->loopcounter=1;
	}
	else if(comma_number==3)
	{
		//处理低位
		n = strData.Find(",");
		strtemp = strData.Left(n);
		pCht_data->dw_Data = strtoul(strtemp,&ctemp,16);
		//处理高位
		DWORD dwHigh0;
		//dwHigh0 = strtoul(strtemp,&ctemp,16);
		strtemp = strtemp +_T(",");
		strData = strData.Right(strData.GetLength()-strtemp.GetLength());
		n = strData.Find(",");
		strtemp = strData.Left(n);
		dwHigh0  = strtoul(strtemp,&ctemp,16);
		dwHigh0 = dwHigh0 <<8;
		pCht_data->dw_Data =pCht_data->dw_Data +dwHigh0;
		//
		DWORD dwHigh1;
		//dwHigh1 = strtoul(strtemp,&ctemp,16);

		n = strData.Find(",");
		strtemp = strtemp +_T(",");
		strData = strData.Right(strData.GetLength()-strtemp.GetLength());
		n = strData.Find(",");
		strtemp = strData.Left(n);
		dwHigh1  = strtoul(strtemp,&ctemp,16);
		dwHigh1 = dwHigh1 <<16;
		pCht_data->dw_Data =pCht_data->dw_Data +dwHigh1;	

		//
		DWORD dwHigh2;
		strtemp = strtemp +_T(",");
		strData = strData.Right(strData.GetLength()-strtemp.GetLength());
		n = strData.Find(",");
		strtemp = strData.Left(n);
		dwHigh2  = strtoul(strtemp,&ctemp,16);
		dwHigh2 = dwHigh2 <<16;
		pCht_data->dw_Data =pCht_data->dw_Data +dwHigh2;	

		//dwHigh2 = strtoul(strData,&ctemp,16);
		//dwHigh2 = dwHigh2 <<24;
		//cht_dat.dw_Data =cht_dat.dw_Data +dwHigh2 ;
		pCht_data->m_dataType =4;
		pCht_data->loopcounter =1;
	}
	else if(comma_number>3)
	{
		if((comma_number+1)%2 ==0)
		{
			n = strData.Find(",");
			strtemp = strData.Left(n);
			pCht_data->dw_Data= strtoul(strtemp,&ctemp,16);
			//处理高位
			DWORD dwHigh;
			strtemp = strtemp +_T(",");
			strData= strData.Right(strData.GetLength()-strtemp.GetLength());
			dwHigh = strtoul(strData,&ctemp,16);
			dwHigh = dwHigh <<8;
			pCht_data->dw_Data =pCht_data->dw_Data +dwHigh ;
			pCht_data->m_dataType=2;
			pCht_data->loopcounter=(comma_number+1)/2;
		}
		else
		{
		
			n = strData.Find(",");
			strtemp = strData.Left(n);
			pCht_data->dw_Data= strtoul(strtemp,&ctemp,16);
			pCht_data->m_dataType=1;
			pCht_data->loopcounter= comma_number+1;
		}
	}	
	return pCht_data;
}

int	CChtFileInfo::CounterEqualSign(CString strData)
{
	int counter=0;
	char *p = strData.GetBuffer(strData.GetLength());
	for(int i=0;i<strData.GetLength();i++)
	{
		if(*p++ == '=')
			counter++;
	}
	strData.ReleaseBuffer(strData.GetLength());
	return counter;
}

int CChtFileInfo::CountersemicolonSign(CString strData)
{
	int counter=0;
	char *ptemp = strData.GetBuffer(strData.GetLength());
	for(int loop=0;loop<strData.GetLength();loop++)
	{
		if(*(ptemp+loop)==';')
				counter++;
	}
	strData.ReleaseBuffer(strData.GetLength());
	return counter;
}
void CChtFileInfo::Parse_Data2(CHT_DATA_ARRAY &cht_data_array,CString strData)//有分号的情况
{
	int n=0;
	CString strtemp;
	CHT_DATA *pcht_dat=NULL;
	char *ctemp=NULL;
	int semicolonSigncounter = CountersemicolonSign(strData);
	for(int i=0;i<=semicolonSigncounter;i++)
	{
		n=strData.Find(";");
		if(n !=-1)
			strtemp = strData.Left(n);
		else
			strtemp = strData;
		if(CountersemicolonSign(strtemp)<4)
		{
//			cht_dat = Parse_Data1(strtemp);
			cht_data_array.Add(Parse_Data1(strtemp));
			strData= strData.Right(strData.GetLength()-strtemp.GetLength()-1);
		}
		else
		{
			
			n = strData.Find(",");
			strtemp = strData.Left(n);
			DWORD m_dwAddress = strtoul(strtemp,&ctemp,16);
			//处理地址
			if((m_dwAddress & 0x03000000)==0 || (m_dwAddress & 0x02000000)==0)
			{
				if((m_dwAddress & 0x00040000) !=0)
					m_dwAddress  +=0x03000000;
				else
					m_dwAddress  +=0x02000000;
			}
			strData = strData.Right(strData.GetLength()-strtemp.GetLength()-1);
			for(int i=0;i<CounterEqualSign(strData)-1;i++)
			{
				pcht_dat= new CHT_DATA;
				pcht_dat = NULL;
				n = strData.Find(",");
				strtemp = strData.Left(n);
				strtemp.Remove(',');
				pcht_dat->dw_Address 	=m_dwAddress+i;
				pcht_dat->dw_Data= strtoul(strtemp,&ctemp,16);
				pcht_dat->m_dataType =1;
				pcht_dat->loopcounter =1;
				cht_data_array.Add(pcht_dat);
				strData= strData.Right(strData.GetLength()-strtemp.GetLength()-1);
			}
		}
	}
}

CList<PCHT_DATA,PCHT_DATA> *CChtFileInfo::ReturnList()
{
	return &m_GoldList;
}
void CChtFileInfo::RemoveAllItem()
{
	PCHT_DATA pCht_data;
	PCHT_DATA pTemp;
	POSITION  pos = m_GoldList.GetHeadPosition();
	for(int i=0;i<m_GoldList.GetCount();i++)
	{
		pCht_data =m_GoldList.GetAt(pos); 
		pTemp = pCht_data;
		while(pTemp->pChtData!=NULL)
		{
			pCht_data = pTemp->pChtData;
			delete pTemp;
			pTemp = pCht_data;
		}
		delete pTemp;
		pTemp=NULL;
		m_GoldList.GetNext(pos);
	}
	
}
