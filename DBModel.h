#pragma once

class CDBModel
{
public:
	int id;				//���ݿ���
	CString name;		//���ݿ�����
	int type;			//���ݿ����ͣ��û�1/ϵͳ0��
	CString filepath;	//���ݿ���·��
	CString crtime;		//���ݿⴴ��ʱ��

	CDBModel(int id = 1, CString name = L"NewDatabase", int type = 1, CString filename = CString("data/dbms.sys"));
	CDBModel(CString& str);
	~CDBModel(void);

};

