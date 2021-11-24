#pragma once
class FieldOp
{
	FieldOp();
	FieldOp(CString& tdfPath);
	CString tdfpath;
	
	//对表字段的增删改查
	bool addFiled(CString& dbName,CString& tableName,CString& fieldName,int fieldOrder,int fieldType,int fieldParam,int fieldIntegrities=-1);//添加字段

};

