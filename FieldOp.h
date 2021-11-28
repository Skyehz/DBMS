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
	//FieldOp();
	//FieldOp(CString& tdfPath);
	//CString tdfpath;
	//bool addField(CString& dbName, CString& tableName, CString& fieldName, int fieldOrder, int fieldType, int fieldParam, int fieldIntegrities = -1);//����ֶ�
				
	//�Ա��ֶε���ɾ�Ĳ� 				
	bool AddFields(vector<FieldModel> fields);	//sql��Ӷ����ֶ�
	bool AddOneField(FieldModel& newField);						//���һ���ֶ�															 //�����ֶ���Ϣ
	bool addFieldInfo(CString& dbName, CString& tableName, FieldModel field);//�����ֶ���Ϣ
	

	/*bool  modifyField(CString& dbName, CString& tableName, CString& fieldName,
		CString& new_fieldName, int new_fieldOrder, int new_fieldType, int new_fieldParam, int new_fieldIntegrities = -1);*/
	//�޸ı����ֶ���Ϣ

	bool ModifyField(FieldModel& field);	//�ڿ��ӻ�����ֱ���޸�
	bool modifyField(CString& fieldName, int new_fieldType, int new_fieldParam);	//sql����޸�

	bool dropField(CString& dbName, CString& tableName, CString& fieldName);

	CString queryField(CString& dbName, CString& tableName, CString& fieldName);//��ѯ�����ֶ���Ϣ�������ַ���
	FieldModel queryFieldModel(CString& dbName, CString& tableName, CString& fieldName);
	vector<CString> queryFields(CString& dbName, CString& tableName);// ��ѯ���������ֶ���
	vector<FieldModel> queryFieldsModel(CString& dbName, CString& tableName);

	bool IsTableExist(CString& dbName, CString& tableName);//�жϱ����ļ��Ƿ����
	int  IsFiledExist(CString& dbName, CString& tableName, CString& fieldName);//�鿴����tdf�ֶ��Ƿ����
};