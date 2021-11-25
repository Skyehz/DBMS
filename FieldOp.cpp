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

	CString str = Corder + CString(" ") + field.GetName() + CString(" ") + CType +
		CString(" ") + Cparam + field.mtime + CString(" ") + Cintegrities;
	return(FileOp::AddAnLine(filePath, str));
}
bool FieldOp::modifyFiled(CString& dbName, CString& tableName, CString& fieldName, CString& new_fieldName, int new_fieldOrder, int new_fieldType, int new_fieldParam, int new_fieldIntegrities)
{
	FieldModel field(new_fieldOrder, new_fieldName, new_fieldType, new_fieldParam, new_fieldIntegrities);
	CString Corder, CType, Cparam, Cintegrities;
	Corder.Format(_T("%d"), field.GetOrder());
	CType.Format(_T("%d"), field.GetType());
	Cparam.Format(_T("%d"), field.GetParam());
	Cintegrities.Format(_T("%d"), field.GetIntegrities());

	CString new_field = Corder + CString(" ") + field.GetName() + CString(" ") + CType +
		CString(" ") + Cparam + field.mtime + CString(" ") + Cintegrities;



	//�жϱ����ļ�tdf�Ƿ����
	if (!IsTableExist(dbName, tableName))
		return false;
	//�ж��ֶ��Ƿ����
	int number = IsFiledExist(dbName, tableName, fieldName);
	if (number==-1)
		return false;
	//�����ֶ���Ϣ��д���ļ�
		//�޸�ԭ�ֶ���Ϣ
	vector<CString>	fields = FileOp::ReadAll(filePath);
	fields.at(number) = new_field;
	CString filePath = CString("./dbms_root /data") + CString(" / ") + dbName + CString(" / ") + tableName + CString(".tb");
	FileOp::WriteRecord(filePath, fields);

	//���������д��ڵ��ֶ���Ϣ���ݶ���

	//�鿴�Ƿ��м�¼��Ԫ�飬�������ݣ����ݶ���
	//����У����¼�¼

	return true;
}
bool FieldOp::dropFiled(CString& dbName, CString& tableName, CString& fieldName)
{
	//�жϱ����ļ�tdf�Ƿ����
	if (!IsTableExist(dbName, tableName))
		return false;
	//�ж��ֶ��Ƿ����
	int number = IsFiledExist(dbName, tableName, fieldName);
	if (number == -1)
		return false;
	//�ж��Ƿ���������ʹ�õ�ǰ�ֶΣ��ݶ���
	//�����������ɾ����ǰ�ֶ�(�ݶ�)

	//ɾ���ֶ���Ϣ
	vector<CString>	fields = FileOp::ReadAll(filePath);

	vector<CString>::iterator it=fields.begin();
	it = it + number;
	fields.erase(it);

	//��ѯ�Ƿ��б��ڼ�¼���ݶ���
	//����У����¼�¼
	
	return true;
}
//
CString FieldOp::queryFiled(CString& dbName, CString& tableName, CString& fieldName)
{
	int number = IsFiledExist(dbName, tableName, fieldName);
	if (number == -1)
	{
		return "No Such Field In " + tableName + " at database " + "
			dbName";
	}

	CString filePath = CString("./dbms_root /data") + CString(" / ") + dbName + CString(" / ") + tableName + CString(".tb");
	vector<CString>	fields = FileOp::ReadAll(filePath);
	return fields.at(number);
}

vector<CString> FieldOp::queryFields(CString& dbName, CString& tableName)
{
	CString filePath = CString("./dbms_root /data") + CString(" / ") + dbName + CString(" / ") + tableName + CString(".tb");
	vector<CString>	fields = FileOp::ReadAll(filePath);
	vector<CString> nameFields;
	for (int i = 0; i < fields.size; i++)
	{
		CString field = fields.at(i);
		vector<CString> field_vct = FileOp::StrSplit(field, L" ");
		CString field_str = field_vct.at(0);
		
		nameFields.push_back(field_str);
	}

	return nameFields;
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
int  FieldOp::IsFiledExist(CString& dbName, CString& tableName, CString& fieldName)
{	
	
	//�ļ�·��
	CString filePath = CString("./dbms_root /data") + CString(" / ") + dbName + CString(" / ") + tableName + CString(".tb");
	//��ѯ�ļ���һ��һ�в�ѯ�ļ�����������Ϣ
		//�ļ�op  vector<CString> ReadAll(CString& fileName);
		//�����������ָ�鿴�Ƿ��и��ֶ�
	vector<CString>	fields=FileOp::ReadAll(filePath);
	for (int i = 0; i < fields.size; i++)
	{
		CString field = fields.at(i);
		vector<CString> field_vct=FileOp::StrSplit(field, L" ");
		CString field_str = field_vct.at(0);
		if (field_str==fieldName)
		{
			return i;
		}
	}
	return -1;
}
