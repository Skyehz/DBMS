#pragma once
#include <vector>
#include<map>
#include"DataModel.h"
using namespace std;

#define ALTER_CHANGE_FIELDNAME 1
#define ALTER_CHANGE_FIELDTYPE 2
#define ALTER_ADD_CONSTRANIT_PK 3
#define ALTER_ADD_CONSTRANIT_NOTNULL 4
#define ALTER_ADD_CONSTRANIT_UNIQUE 5
#define ALTER_ADD_CONSTRANIT_DEFAULT 5

class ParseSQL
{
public:

	void setDB(CString& dbmName);
	void getSql(CString& statement);
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
	bool altOp(CString& str, CDataModel& record);
	vector<CDataModel> whereOp(CString& str, vector<CDataModel> recordlist);
};



