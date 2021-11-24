#include "pch.h"
#include "FieldOp.h"
#include"FileOp.h"
#include"FieldModel.h"

//MFC�ж�Ŀ¼���Ƿ����ָ���ļ��� ���ļ�����
#include <shlwapi.h>
#pragma comment(lib,"Shlwapi.lib") 


//FieldOp::FieldOp() {
//
//	this->tdfpath = CString("./dbms_root/data/db1/");
//}
//FieldOp::FieldOp(CString& tdfpath) {
//	//�����ļ�����·��
//	this->tdfpath = tdfpath;
//}

bool FieldOp::addFiled(CString& dbName, CString& tableName, CString& fieldName, int fieldOrder, int fieldType, int fieldParam, int fieldIntegrities)
{

	//�жϱ���tdf�ļ��Ƿ����
	if (!IsTableExist(dbName, tableName))
		return false;
	//�鿴����tdf�ֶ��Ƿ����

	//�����ֶ�ʵ��
	FieldModel field(fieldOrder, fieldName, fieldType, fieldParam, fieldIntegrities);
	//�����ֶ���Ϣ
	addFieldInfo(dbName,tableName,field);

	return true;
}
bool FieldOp::addFieldInfo(CString& dbName, CString& tableName,FieldModel field)
{	
	CString filePath = CString("./dbms_root /data") + CString(" / ") + dbName+ CString(" / ") +tableName+ CString(".tb");
	CString Corder, CType, Cparam, Cintegrities;
	Corder.Format(_T("%d"), field.GetOrder());
	CType.Format(_T("%d"), field.GetType());
	Cparam.Format(_T("%d"), field.GetParam());
	Cintegrities.Format(_T("%d"), field.GetIntegrities());

	CString str = Corder + CString("#") + field.GetName() + CString("#") + CType +
		CString("#") + Cparam + field.mtime + CString("#") + Cintegrities;
	return(FileOp::AddAnLine(filePath, str));
}
bool FieldOp::modifyFiled(CString& dbName, CString& tableName, CString& fieldName, CString& new_fieldName, int new_fieldOrder, int new_fieldType, int new_fieldParam, int new_fieldIntegrities)
{
	//�жϱ����ļ�tdf�Ƿ����
	
	//�ж��ֶ��Ƿ����

	//��ȡԭ�ֶ���Ϣ
		//��ȡ�ļ������ֶ���Ϣ

	//�����ֶ���Ϣ��д���ļ�
		//�޸�ԭ�ֶ���Ϣ

	//���������д��ڵ��ֶ���Ϣ���ݶ���

	//�鿴�Ƿ��м�¼��Ԫ�飬�������ݣ����ݶ���
	//����У����¼�¼

	return false;
}
bool FieldOp::dropFiled(CString& dbName, CString& tableName, CString& fieldName)
{
	//�жϱ����ļ�tdf�Ƿ����

	//�ж��ֶ��Ƿ����

	//�ж��Ƿ���������ʹ�õ�ǰ�ֶΣ��ݶ���
	//�����������ɾ����ǰ�ֶ�

	//ɾ���ֶ���Ϣ

	//��ѯ�Ƿ��б��ڼ�¼���ݶ���
	//����У����¼�¼
	
	return false;
}

//�жϱ����ļ��Ƿ����
bool FieldOp::IsTableExist(CString& dbName,CString& tableName) {
	//�жϱ��Ƿ����
	//ͨ�������ļ�op�鿴�Ƿ���ڱ�
	CString folderPath = CString("dbms_root/data") + CString("/") + dbName;
	CString tablePath = folderPath + CString("/") + tableName + CString(".tdf");
	return PathFileExists(tablePath);
}
//�ж��ֶ��Ƿ����
bool FieldOp::IsFiledExist(CString& dbName, CString& tableName, CString& fieldName)
{	
	
	//�ļ�·��
	CString filePath = CString("./dbms_root /data") + CString(" / ") + dbName + CString(" / ") + tableName + CString(".tb");
	//��ѯ�ļ���һ��һ�в�ѯ�ļ�����������Ϣ
		//�ļ�op  vector<CString> ReadAll(CString& fileName);
		//�����������ָ�鿴�Ƿ��и��ֶ�

	return false;
}
CString FieldOp::queryFiled(CString& dbName, CString& tableName, CString& fieldName)
{
	return CString();
}
