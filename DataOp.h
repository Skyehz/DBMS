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
	CString name;//���ݿ�����
	CString tableName;//��¼�û��������
	CString field;//�ֶ���
	CString value;//�ֶ�ֵ��Ӧ�ļ�ֵ
	CString message;
	void getMessage();
	void getSql(CString& tableName);
};

