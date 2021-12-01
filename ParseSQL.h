#pragma once
#include <vector>
#include<map>
#include"DataModel.h"
#include"FieldModel.h"
using namespace std;


#define ALTER_CHANGE_FIELDNAME 1
#define ALTER_CHANGE_FIELDTYPE 2
#define ALTER_ADD_CONSTRANIT_PK 3
#define ALTER_ADD_CONSTRANIT_NOTNULL 4
#define ALTER_ADD_CONSTRANIT_UNIQUE 5
#define ALTER_ADD_CONSTRANIT_DEFAULT 5
#define ALTER_CHANGE_FIELDNAME 1
#define ALTER_CHANGE_FIELDTYPE 2
#define ALTER_ADD_CONSTRANIT_PK 3
#define ALTER_ADD_CONSTRANIT_NOTNULL 4
#define ALTER_ADD_CONSTRANIT_UNIQUE 5
#define ALTER_ADD_CONSTRANIT_DEFAULT 6
#define INTEGRITY_ERROR_TYPE 301 //��������
#define INTEGRITY_ERROR_DATETYPE 351//���ڴ���
#define INTEGRITY_NOT_PK 306 //������������
#define INTEGRITY_NULL_VALUE 304 //�ǿռ�ֵ�����ֵ����
#define INTEGRITY_NOT_UNIQUE 305 //����Ψһ����
#define INTEGRITY_TOO_LONG 302//����varcharԼ��
#define YES 1

class ParseSQL
{
public:

	void setDB(CString& dbmName);
	vector<CDataModel> getSql(CString& statement);
	CString dbmName;
	bool alterOp(vector<CString> init);
	vector<CString> getDistinctList(vector<CDataModel>& whereRes, CString& field);
	vector<vector<CDataModel>> getGroupByRes(vector<CDataModel>& whereRes, CString& field);
	vector<CDataModel> getHavingRes(vector<vector<CDataModel>>& groupRes, vector<CDataModel>& res, CString& gField, CString& condition);
	vector<CDataModel> getOrderBy(vector<CDataModel>& list, CString& field, int type);
	vector<CString> getSumRes(vector<CString> &distinctList, vector<vector<CDataModel>>& groupRes, CString& field);
	vector<CString> getCountRes(vector<CString>& distinctList, vector<vector<CDataModel>>& groupRes, CString& field);
	vector<CString> getAvgRes(vector<CString> &distinctList, vector<vector<CDataModel>>& groupRes, CString& field);
	//alter����
	bool insertOp(vector<CString> init);		//insert����
	vector<CDataModel> selectOp(CString str);		//select����
	bool deleteOp(vector<CString> init); //delete record 
	bool updateOp(vector<CString> init);
	bool altOp(CString& str, CDataModel& record, vector<FieldModel>& fieldmodellist, vector<CDataModel>& recordlist);
	vector<CDataModel> whereOp(CString& str, vector<CDataModel> recordlist, vector<FieldModel>& fieldmodellist);
	int IntegrityVerify(CString& val, FieldModel& field);
};



