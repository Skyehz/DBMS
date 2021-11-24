#pragma once
class FieldOp
{
public:
	//FieldOp();
	//FieldOp(CString& tdfPath);
	//CString tdfpath;
	//
	//对表字段的增删改查 
	bool addFiled(CString& dbName,CString& tableName,CString& fieldName,int fieldOrder,int fieldType,int fieldParam,int fieldIntegrities=-1);//添加字段
																																			 //增加字段信息
	bool addFieldInfo(CString& dbName, CString& tableName,FieldModel field);//增加字段信息
	
	bool modifyFiled(CString& dbName, CString& tableName, CString& fieldName,
		CString& new_fieldName, int new_fieldOrder, int new_fieldType, int new_fieldParam, int new_fieldIntegrities = -1);
	//修改表内字段信息
	
	bool dropFiled(CString& dbName, CString& tableName,CString& fieldName);



	bool IsTableExist(CString& dbName, CString& tableName);//判断表定义文件是否存在
	bool IsFiledExist(CString& dbName, CString& tableName, CString& fieldName);//查看表内tdf字段是否存在
	CString queryFiled(CString& dbName, CString& tableName, CString& fieldName);//查询表内字段信息，返回字符串
	
};

