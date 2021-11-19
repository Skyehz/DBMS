#pragma once
#include<vector> 
#include"DBModel.h"

using namespace std;

class CDBOp
{

public:
	int InitDatabase();//初始化数据库
	int CreateDatabase(CString& dbName); //接受一个数据库名，创建数据库

};

