#pragma once
#include<vector> 
#include"DBModel.h"

using namespace std;

class CDBOp
{

public:
	int InitDatabase();//��ʼ�����ݿ�
	int CreateDatabase(CString& dbName); //����һ�����ݿ������������ݿ�

};

