#pragma once
#include<vector>
#include <string>
using namespace std;
class FileOp
{
public:
	static CString GetCurrTime();		//获取当前系统时间
	static CString setOneline(CString& str);
	static vector<CString> StrSplit(CString str, CString split);
	static CString IntegerToString(int number);
	static int StringToInteger(CString cstring);
	static CString BoolToString(bool bl);
	static bool StringToBool(CString str);
	static CString GetTypeCString(int type);
	static int GetTypeInt(CString type);
	static CString semicolon(CString& str);//处理分号
	static CString getbrakets(CString& str);//获得括号中的内容
	static CString getbeforebrakets(CString& str);//获得括号(前的内容
	static bool paren(CString& str);//判断括号匹配S



	static vector<CString> ReadAll(CString& fileName);	//分条读取全部文件的内容
	static bool WriteRecord(CString& fileName, vector<CString>& str);	//将多条记录一次性写入文件
	static bool WriteLog(CString &info, CString &fileName);		//写日志文件
	static bool AddAnLine(CString& fileName, CString& str);	//在文件末尾加入一条记录
	static bool DeleteFolder(CString& folderName);	//删除文件夹
	static bool ModifyFileName(CString& oldname, CString& newname);
	static CString FileOp::ReadFirstLine(CString& fileName);//读取第一条文件的内容
	static bool FileOp::SaveCounter(CString& filePath, int counter);
};

