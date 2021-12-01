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
#define INTEGRITY_ERROR_TYPE 301 //错误类型
#define INTEGRITY_ERROR_DATETYPE 351//日期错误
#define INTEGRITY_NOT_PK 306 //不是主键错误
#define INTEGRITY_NULL_VALUE 304 //非空键值填入空值错误
#define INTEGRITY_NOT_UNIQUE 305 //不是唯一错误
#define INTEGRITY_TOO_LONG 302//超出varchar约束
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
	//alter操作
	bool insertOp(vector<CString> init);		//insert操作
	vector<CDataModel> selectOp(CString str);		//select操作
	bool deleteOp(vector<CString> init); //delete record 
	bool updateOp(vector<CString> init);
	bool altOp(CString& str, CDataModel& record, vector<FieldModel>& fieldmodellist, vector<CDataModel>& recordlist);
	vector<CDataModel> whereOp(CString& str, vector<CDataModel> recordlist, vector<FieldModel>& fieldmodellist);
	int IntegrityVerify(CString& val, FieldModel& field);
};



