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

	//�жϱ����ļ��Ƿ����
	if (!IsTableExist(dbName, tableName))
		return false;
	//�ж��ֶ����Ƿ���Ч
	
	//�����ֶ�ʵ��
	FieldModel field(fieldOrder, fieldName, fieldType, fieldParam, fieldIntegrities);
	//�����ֶ���Ϣ
	addFieldInfo(field, tdfpath);

	return true;
}
//�жϱ����ļ��Ƿ����
bool IsTableExist(CString& dbName,CString& tableName) {
	
	

	return true;
}
//�ж��ֶ����Ƿ���Ч
bool IsFieldNameValid(CString& filedName) {


}
//�����ֶ���Ϣ
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

