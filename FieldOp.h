#pragma once
class FieldOp
{
	FieldOp();
	FieldOp(CString& tdfPath);
	CString tdfpath;
	
	//�Ա��ֶε���ɾ�Ĳ�
	bool addFiled(CString& dbName,CString& tableName,CString& fieldName,int fieldOrder,int fieldType,int fieldParam,int fieldIntegrities=-1);//����ֶ�

};

