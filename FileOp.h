#pragma once
#include<vector>
using namespace std;
class FileOp
{
public:
	static CString GetCurrTime();		//获取当前系统时间
	static vector<CString> ReadAll(CString& fileName);	//分条读取全部文件的内容
	static bool WriteRecord(CString& fileName, vector<CString>& str);	//将多条记录一次性写入文件
	static bool WriteLog(CString &info, CString &fileName);		//写日志文件
	static bool AddAnLine(CString& fileName, CString& str);	//在文件末尾加入一条记录
};

