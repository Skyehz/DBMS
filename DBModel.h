#pragma once

class CDBModel
{
public:
	int id;				//���ݿ���
	CString name;		//���ݿ�����
	int type;			//���ݿ����ͣ��û�1/ϵͳ0��
	CString filepath;	//���ݿ���·�� ���ļ��е�·����
	CString crtime;		//���ݿⴴ��ʱ��

	CDBModel(int id = 1, CString name = L"NewDatabase", int type = 1, CString filename = CString("data/dbms.sys"));
	CDBModel(CString& str);
	~CDBModel(void);

	//Get����
	int GetId() { return id; };
	CString GetName() { return name; };
	int GetType() { return type; };
	//int GetTableNum() { return m_iTableNum; };
	CString GetCreateTime() { return crtime; };
	CString GetDBPath() { return L"data/" + name; };

	//Set����
	void SetId(int m_id) { id = id; };
	void SetName(CString m_name) { name = m_name; };
	void SetType(int m_type) { type = m_type; };
	//void SetTableNum(int m_tableNum) { m_iTableNum = m_tableNum; };
	void SetCreatTime(CString m_crtime) { crtime = m_crtime; };
};

