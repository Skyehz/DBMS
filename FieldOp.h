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
	//bool addField(CString& dbName, CString& tableName, CString& fieldName, int fieldOrder, int fieldType, int fieldParam, int fieldIntegrities = -1);//添加字段
				
	//对表字段的增删改查 				
	bool AddFields(vector<FieldModel> fields);	//sql添加多条字段
	bool AddOneField(FieldModel& newField);						//添加一条字段															 //增加字段信息
	bool addFieldInfo(CString& dbName, CString& tableName, FieldModel field);//增加字段信息
	

	/*bool  modifyField(CString& dbName, CString& tableName, CString& fieldName,
		CString& new_fieldName, int new_fieldOrder, int new_fieldType, int new_fieldParam, int new_fieldIntegrities = -1);*/
	//修改表内字段信息

	bool ModifyField(FieldModel& field);	//在可视化界面直接修改
	bool modifyField(CString& fieldName, int new_fieldType, int new_fieldParam);	//sql语句修改

	bool dropField(CString& dbName, CString& tableName, CString& fieldName);

	CString queryField(CString& dbName, CString& tableName, CString& fieldName);//查询表内字段信息，返回字符串
	FieldModel queryFieldModel(CString& dbName, CString& tableName, CString& fieldName);
	vector<CString> queryFields(CString& dbName, CString& tableName);// 查询表内所有字段名
	vector<FieldModel> queryFieldsModel(CString& dbName, CString& tableName);

	bool IsTableExist(CString& dbName, CString& tableName);//判断表定义文件是否存在
	int  IsFiledExist(CString& dbName, CString& tableName, CString& fieldName);//查看表内tdf字段是否存在
};