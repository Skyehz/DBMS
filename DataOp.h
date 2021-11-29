#pragma once
#include "DataModel.h"
#include "FieldModel.h"
#include "FileOp.h"
#include "FieldOp.h"
#define ADD_ERROR 315
#define SAVE_COUNTER_ERROR 318
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
	static vector<CDataModel> ReadDataList(CString& trdFileName, vector<FieldModel>& fieldList);//��ȡȫ����¼
	static bool WriteAnRecord(CDataModel& record, CString& trdFileName, vector<FieldModel>& fieldList);//���ļ�����һ����¼,д���ļ�
	int AddRecord(CDataModel& record, vector<FieldModel>& fieldList);
	static bool SaveRcdCounter(CString& trdFileName, int counter);//���������
	static int GetRcdCounter(CString& trdFileName);//�õ�������
	bool ModifyRecord(CDataModel& record);//���ļ��޸�ָ���ļ�¼
	static bool DeleteAnRecord(int id, CString& trdFileName);//���ļ�ɾ��һ����¼
	int DeleteRecord(int id);


};

