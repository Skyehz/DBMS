#pragma once
#include<vector> 
#include"DBModel.h"

using namespace std;

class CDBOp
{
	
public:
	CDBOp();
	CString sysPath;

	int InitDatabase();//初始化数据库
	int CreateDatabase(CString& dbName); //接受一个数据库名，创建数据库
	vector<CDBModel> GetDatabaseList();//获取数据库列表
};

