#pragma once
#include<vector> 
#include"DBModel.h"

using namespace std;

class CDBOp
{
	
public:
	CDBOp();
	CString sysPath;

	int InitDatabase();//��ʼ�����ݿ�
	int CreateDatabase(CString& dbName); //����һ�����ݿ������������ݿ�
	int DropDatabase(CString& dbname); //ɾ��ָ�����ݿ�
	vector<CDBModel> GetDatabaseList();//��ȡ���ݿ��б�
};

