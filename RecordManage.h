#pragma once
#include<vector>
using namespace std;
class RecordManage
{
public:
	CString name;//���ݿ�����
	CString tableName;//��¼�û��������
	CString field;//�ֶ���
	CString value;//�ֶ�ֵ��Ӧ�ļ�ֵ
	int insert(CString& dbName);
};

