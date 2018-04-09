//ini文件读写类
//共有3个函数可用.分别是
//iniopen(char FileName,int Itemcount)  打开或建立INI文件
//FileName:文件名     Itemcount:项个数(该个数必须大于文件中项目个数)
//char *iniread(char* Items)            读字段值
//Items:字段名  返回值:字段值
//iniwrite(char *Items,char *ItemValute)   写字段名和字段值 
//Items:字段名  ItemValute:字段值
//inimodify(char *Items,char *ItemValute)   修改字段名和字段值 
//Items:字段名  ItemValute:字段值
//方俊海  设计


class  CFileini
{

public:
	 
     int  iniopen(char  *FileName,int It);
	 char* iniread(char *Item);
	 iniwrite(char *Item,  char *ItemValue);
	 char*  inimodify(char *Item,char *ItemValue);
	 CFileini();
	 ~CFileini();

protected:
	char inifilename[512];
	 char **Items;
	 char **ItemValues;
	 int Itemcount;
	 int newcount;
	 FILE *fp; 
 

};