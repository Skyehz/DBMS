#include "pch.h"
#include "FieldOp.h"
#include"FileOp.h"
#include"FieldModel.h"


//FieldOp::FieldOp() {
//	this->tdfpath = CString("./dbms_root/data/db1/db1.tdf");
//}
//FieldOp::FieldOp(CString& tdfpath) {
//	this->tdfpath = tdfpath;
//}

bool FieldOp::addFiled(CString& dbName, CString& tableName, CString& fieldName, int fieldOrder, int fieldType, int fieldParam, int fieldIntegrities)
{

	//判断表定义文件是否存在
	if (!IsTableExist(dbName, tableName))
		return false;
	//判断字段名是否有效
	
	//创建字段实体
	FieldModel field(fieldOrder, fieldName, fieldType, fieldParam, fieldIntegrities);
	//更新字段信息
	addFieldInfo(field, tdfpath);

	return true;
}
//判断表定义文件是否存在
bool IsTableExist(CString& dbName,CString& tableName) {
	
	

	return true;
}
//判断字段名是否有效
bool IsFieldNameValid(CString& filedName) {


}
//更新字段信息
bool addFieldInfo(FieldModel field,CString tdfPath) {

	CString Corder, CType, Cparam, Cintegrities;
	Corder.Format(_T("%d"), field.GetOrder());
	CType.Format(_T("%d"), field.GetType());
	Cparam.Format(_T("%d"), field.GetParam());
	Cintegrities.Format(_T("%d"), field.GetIntegrities());

	CString str = Corder + CString("#") + field.GetName() + CString("#") + CType +
		CString("#") + Cparam + field.mtime + CString("#") + Cintegrities;
	return(FileOp::AddAnLine(tdfPath,str));
}

