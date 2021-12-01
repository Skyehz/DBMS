#pragma once
#include "DataModel.h"
#include "FieldModel.h"
#include "FileOp.h"
#include "FieldOp.h"
#define ADD_ERROR 315
#define SAVE_COUNTER_ERROR 318
#define INTEGRITY_ERROR_TYPE 301 //��������
#define INTEGRITY_ERROR_DATETYPE 351//���ڴ���
#define INTEGRITY_NOT_PK 306 //������������
#define INTEGRITY_NULL_VALUE 304 //�ǿռ�ֵ�����ֵ����
#define INTEGRITY_NOT_UNIQUE 305 //����Ψһ����
#define INTEGRITY_TOO_LONG 302//����varcharԼ��
#define YES 1

class CDataOp
{
private:
	CString trdPath;
	CString tdfPath;
	CString dbName;
	CString tbName;
public:
	CDataOp(CString dbname, CString tableName);
	~CDataOp(void);

	static bool WriteAnRecord(CDataModel& record, CString& trdFileName, vector<FieldModel>& fieldList);//���ļ�����һ����¼,д���ļ�
	int AddRecord(CDataModel& record, vector<FieldModel>& fieldList);
	vector<CDataModel> ReadDataList(vector<FieldModel>& fieldList);
	static bool SaveRcdCounter(CString& trdFileName, int counter);//���������
	int GetRcdCounter(CString& trdFileName, vector<FieldModel>& fieldList);//�õ�������
	bool ModifyRecord(CDataModel& record);//���ļ��޸�ָ���ļ�¼
	static bool DeleteAnRecord(int id, CString& trdFileName);//���ļ�ɾ��һ����¼
	int DeleteRecord(int id);
	static bool DeleteFieldRecord(CString& trdFileName, int index);//���ֶ�����ɾ�����ڸ��ֶεļ�¼
	int IntegrityVerify(CDataModel& record, FieldModel& field);//��֤�Ƿ�����ֶ�Ҫ��
	int IntegrityVerify1(CString& val, FieldModel& field);//������Լ��
	bool OnlyHaveNumber(CString& str);
	bool OnlyHaveNumberAndDot(CString& str);
	int IsUnique(CString& value, CString& fieldName);//�Ƿ�Ψһ
	vector<CDataModel> QueryFieldRecord(CString& tbName, vector<CString> fieldList);//���ֶβ�ѯ��¼
};