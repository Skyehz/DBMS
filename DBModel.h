#pragma once

class CDBModel
{
public:
	int id;				//数据库编号
	CString name;		//数据库名称
	int type;			//数据库类型（用户1/系统0）
	CString filepath;	//数据库存放路径 （文件夹的路径）
	CString crtime;		//数据库创建时间

	CDBModel(int id = 1, CString name = L"NewDatabase", int type = 1, CString filename = CString("data/dbms.sys"));
	CDBModel(CString& str);
	~CDBModel(void);

	//Get操作
	int GetId() { return id; };
	CString GetName() { return name; };
	int GetType() { return type; };
	//int GetTableNum() { return m_iTableNum; };
	CString GetCreateTime() { return crtime; };
	CString GetDBPath() { return L"data/" + name; };

	//Set操作
	void SetId(int m_id) { id = id; };
	void SetName(CString m_name) { name = m_name; };
	void SetType(int m_type) { type = m_type; };
	//void SetTableNum(int m_tableNum) { m_iTableNum = m_tableNum; };
	void SetCreatTime(CString m_crtime) { crtime = m_crtime; };
};

