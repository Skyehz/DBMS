#pragma once
#include "DataModel.h"
#include "FieldModel.h"
#include "FileOp.h"
#include "FieldOp.h"
#define ADD_ERROR 315
#define SAVE_COUNTER_ERROR 318
#define INTEGRITY_ERROR_TYPE 301 //错误类型
#define INTEGRITY_ERROR_DATETYPE 351//日期错误
#define INTEGRITY_NOT_PK 306 //不是主键错误
#define INTEGRITY_NULL_VALUE 304 //非空键值填入空值错误
#define INTEGRITY_NOT_UNIQUE 305 //不是唯一错误
#define INTEGRITY_TOO_LONG 302//超出varchar约束
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

	static bool WriteAnRecord(CDataModel& record, CString& trdFileName, vector<FieldModel>& fieldList);//从文件新增一条记录,写入文件
	int AddRecord(CDataModel& record, vector<FieldModel>& fieldList);
	vector<CDataModel> ReadDataList(vector<FieldModel>& fieldList);
	static bool SaveRcdCounter(CString& trdFileName, int counter);//保存计数器
	int GetRcdCounter(CString& trdFileName, vector<FieldModel>& fieldList);//得到计数器
	bool ModifyRecord(CDataModel& record);//从文件修改指定的记录
	static bool DeleteAnRecord(int id, CString& trdFileName);//从文件删除一条记录
	int DeleteRecord(int id);
	static bool DeleteFieldRecord(CString& trdFileName, int index);//按字段名称删除关于该字段的记录
	int IntegrityVerify(CDataModel& record, FieldModel& field);//验证是否符合字段要求
	int IntegrityVerify1(CString& val, FieldModel& field);//完整性约束
	bool OnlyHaveNumber(CString& str);
	bool OnlyHaveNumberAndDot(CString& str);
	int IsUnique(CString& value, CString& fieldName);//是否唯一
	vector<CDataModel> QueryFieldRecord(CString& tbName, vector<CString> fieldList);//按字段查询记录
};