#pragma once
#include "FieldModel.h"
#include"vector"
using namespace std;

class FieldOp
{
private:
	CString tdfPath;
	CString trdPath;
	CString dbName;
	CString tbName;
public:
	FieldOp(CString dbName, CString tableName);
	~FieldOp(void);
public:
	//�Ա��ֶε���ɾ�Ĳ� 				
	bool AddFields(vector<FieldModel> fields);	//sql��Ӷ����ֶ�
	bool AddOneField(FieldModel& newField);						//���һ���ֶ�															 //�����ֶ���Ϣ
	bool addFieldInfo(CString& dbName, CString& tableName, FieldModel field);//�����ֶ���Ϣ

	bool ModifyField(FieldModel& field,int type);	//�޸��ֶ���Ϣ
	bool ModifyFieldSQL(vector<CString> condition, int type);	//��sql�е��õ��޸��ֶ�

	bool dropField(CString& dbName, CString& tableName, CString& fieldName);
	int DeleteField(CString& fieldName);

	CString queryField(CString& dbName, CString& tableName, CString& fieldName);//��ѯ�����ֶ���Ϣ�������ַ���
	FieldModel queryFieldModel(CString& dbName, CString& tableName, CString& fieldName);
	vector<CString> queryFields(CString& dbName, CString& tableName);// ��ѯ���������ֶ���
	vector<FieldModel> queryFieldsModel(CString& dbName, CString& tableName);

	bool IsTableExist(CString& dbName, CString& tableName);//�жϱ����ļ��Ƿ����
	int  IsFiledExist(CString& dbName, CString& tableName, CString& fieldName);//�鿴����tdf�ֶ��Ƿ����
};