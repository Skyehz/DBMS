#pragma once
#include<vector>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
using namespace std;
class DataOp
{
public:
	CString name;//数据库名称
	CString tableName;//记录用户输入表名
	CString field;//字段名
	CString value;//字段值对应的键值
	CString message;
	void getMessage();
	void getSql(CString& tableName);
};

