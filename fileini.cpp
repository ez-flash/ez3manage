#include "StdAfx.h"
#include "fileini.h"
#include "stdio.h"


CFileini::CFileini()
{
	Itemcount=0;
	newcount=0;
	Items=NULL;
	ItemValues=NULL;
	fp=NULL;
}

int CFileini::iniopen(char *FileName,int It)
{
	int filelen=strlen(FileName);
	if(filelen>512)filelen=512;
	strncpy(inifilename,FileName,filelen);
	inifilename[filelen]=0;
	
	
	fp=fopen(FileName,"a+");
	int i=0;
	if(fp==NULL)return i;
	i=1;
	int fileeof=0,strsize=0;
	
	char temp[200];
	char Itemtemp[100];
	char ItemValuetemp[100];
	
	
	ItemValues=(char**)malloc(It*sizeof(char**));
	Items=(char**)malloc(It*sizeof(char**));


	for(int e=0;e<It;e++)
	{
	ItemValues[e]=NULL;
	Items[e]=NULL;
	}
	
	
	char *ptemp=NULL;
	while(Itemcount<It)
	{
		fileeof=fscanf(fp, "%s\n",temp);
		if(fileeof==EOF)
		{
			i=2;
			break;
		}
		
		char *temp1=strstr(temp,"=");
		int rect=temp1-temp;
		if(rect<0)continue;
		strncpy(Itemtemp,temp,rect);
		Itemtemp[rect]=0;
		
		rect++;
		int tv=0;
		
		for(int i=rect;i<(int)strlen(temp)+1;i++)
		{
			ItemValuetemp[tv++]=temp[i];
		}
		ItemValuetemp[tv]=0;
		
		
		strsize=strlen(Itemtemp)+1;
		ptemp=(char*)malloc(strsize);
		strcpy(ptemp,Itemtemp);
		Items[Itemcount]=ptemp;
		
		
		strsize=strlen(ItemValuetemp)+1;
		ptemp=(char*)malloc(strsize);
		strcpy(ptemp,ItemValuetemp);
		ItemValues[Itemcount]=ptemp;
		
		Itemcount++;
		}
	return i;
}



char*  CFileini::iniread(char *Item)
{
    int i=0;
	
	if(Itemcount==0)return NULL;
	
	while(strstr(Items[i],Item)==NULL)    
	{
		i++;
		if(i==Itemcount)return NULL;
	}
	
	return  ItemValues[i];
	
}


CFileini::iniwrite(char *Item,char *ItemValue)
{
	
		
	int strsize=strlen(Item)+1;
	Items[Itemcount]=(char*)malloc(strsize);
	strcpy(Items[Itemcount],Item);
	
	
	strsize=strlen(ItemValue)+1;
	ItemValues[Itemcount]=(char*)malloc(strsize);
	strcpy(ItemValues[Itemcount],ItemValue);
	
	Itemcount++;
	newcount++;

//		fprintf(fp,"%s=%s\n",Item,ItemValue);

}


CFileini::~CFileini()
{
	
	if(fp!=NULL)fclose(fp);
	DeleteFile(inifilename);
	fp=fopen(inifilename,"a+");
	
	fseek(fp,0,SEEK_SET);
	for(int j=0;j<Itemcount;j++)
	{
	fprintf(fp,"%s=%s\n",Items[j],ItemValues[j]);
	}

	
	
	for(int i=0;i<Itemcount;i++)
	{
		if(Items[i]!=NULL){free(Items[i]);Items[i]=NULL;}
		if(ItemValues[i]!=NULL){free(ItemValues[i]);ItemValues[i]=NULL;}
	}
	if(Items!=NULL){free(Items);Items=NULL;}
	if(ItemValues!=NULL){free(ItemValues);ItemValues=NULL;}
	if(fp!=NULL)fclose(fp);
		
}



char*  CFileini::inimodify(char *Item,char *ItemValue)
{
    int i=0;
	
	
	
	if(Itemcount==0)return NULL;
	
	while(strstr(Items[i],Item)==NULL)    
	{
		i++;
		if(i==Itemcount)return NULL;
	}
	
	free(ItemValues[i]);
	int len=strlen(ItemValue)+1;
	ItemValues[i]=(char*)malloc(len);
	strcpy(ItemValues[i],ItemValue);

	return  ItemValues[i];
	
}
