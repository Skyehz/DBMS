#pragma once
#include<vector>
using namespace std;
class FileOp
{
public:
	static vector<CString> ReadAll(CString& fileName);	//分条读取全部文件的内容
	static bool WriteRecord(CString& fileName, vector<CString>& str);	//将多条记录一次性写入文件


	bool WriteLog(CString& oper);	//写日志文件
};

