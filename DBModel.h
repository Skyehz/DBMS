#pragma once

class CDBModel
{
public:
	int id;				//数据库编号
	CString name;		//数据库名称
	int type;			//数据库类型（用户1/系统0）
	CString filepath;	//数据库存放路径
	CString crtime;		//数据库创建时间

	CDBModel(int id = 1, CString name = L"NewDatabase", int type = 1, CString filename = CString("data/dbms.sys"));
	CDBModel(CString& str);
	~CDBModel(void);

};

