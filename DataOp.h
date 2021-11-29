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
	static vector<CDataModel> ReadDataList(CString& trdFileName, vector<FieldModel>& fieldList);//获取全部记录
	static bool WriteAnRecord(CDataModel& record, CString& trdFileName, vector<FieldModel>& fieldList);//从文件新增一条记录,写入文件
	int AddRecord(CDataModel& record, vector<FieldModel>& fieldList);
	static bool SaveRcdCounter(CString& trdFileName, int counter);//保存计数器
	static int GetRcdCounter(CString& trdFileName);//得到计数器
	bool ModifyRecord(CDataModel& record);//从文件修改指定的记录
	static bool DeleteAnRecord(int id, CString& trdFileName);//从文件删除一条记录
	int DeleteRecord(int id);


};

