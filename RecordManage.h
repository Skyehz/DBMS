#pragma once
#include<vector>
using namespace std;
class RecordManage
{
public:
	CString name;//数据库名称
	CString tableName;//记录用户输入表名
	CString field;//字段名
	CString value;//字段值对应的键值
	int insert(CString& dbName);
};

