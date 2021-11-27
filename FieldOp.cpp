#include "pch.h"
#include "FieldOp.h"
#include"FileOp.h"

//MFC�ж�Ŀ¼���Ƿ����ָ���ļ��� ���ļ�����
#include <shlwapi.h>
#pragma comment(lib,"Shlwapi.lib") 


FieldOp::FieldOp(CString dbName, CString tableName)
{
	this->tdfPath = CString("./dbms_root/data") + CString("/") + dbName + CString("/") + tableName + CString(".tdf");
	this->trdPath = CString("./dbms_root/data") + CString("/") + dbName + CString("/") + tableName + CString(".trd");
	this->dbName = dbName;
	this->tbName = tableName;
}

FieldOp::~FieldOp(void)
{
}

bool FieldOp::AddFields(vector<FieldModel> fields)
{
	if (!fields.empty())
	{
		for (int i = 0;i<fields.size();i++)
		{
			AddOneField(fields[i]);
		}
		return true;
	}
	else
		return false;

}

bool FieldOp::AddOneField(FieldModel &newField)
{

	//�жϱ���tdf�ļ��Ƿ����
	if (!IsTableExist(dbName, tbName))
		return false;
	//�鿴����tdf�ֶ��Ƿ����

	//�����ֶ�ʵ��

	FieldModel field(newField.GetId(), newField.GetName(), newField.GetType(), newField.GetParam(), newField.GetIntegrities());
	//�����ֶ���Ϣ
	if (!addFieldInfo(dbName, tbName, field))
	{
		return false;
	}
	return true;
}



//bool FieldOp::addField(CString& dbName, CString& tbName, CString& fieldName, int fieldOrder, int fieldType, int fieldParam, int fieldIntegrities)
//{
//
//	//�жϱ���tdf�ļ��Ƿ����
//	if (!IsTableExist(dbName, tbName))
//		return false;
//	//�鿴����tdf�ֶ��Ƿ����
//
//	//�����ֶ�ʵ��
//	FieldModel field(fieldOrder, fieldName, fieldType, fieldParam, fieldIntegrities);
//	//�����ֶ���Ϣ
//	if (!addFieldInfo(dbName, tbName, field))
//	{
//		return false;
//	}
//	return true;
//
//
//}

bool FieldOp::addFieldInfo(CString& dbName, CString& tableName, FieldModel field)
{
	CString filePath = CString("./dbms_root/data") + CString("/") + dbName +
		CString("/") +  tableName + CString(".tdf");
	CString Corder, CType, Cparam, Cintegrities;
	Corder.Format(_T("%d"), field.GetId());
	CType.Format(_T("%d"), field.GetType());
	Cparam.Format(_T("%d"), field.GetParam());
	Cintegrities.Format(_T("%d"), field.GetIntegrities());

	CString str = Corder + CString(" ") + field.GetName() + CString(" ") + CType +
		CString(" ") + Cparam + CString(" ") + field.mtime + CString(" ") + Cintegrities;
	return(FileOp::AddAnLine(filePath, str));
}

//�޸��ֶ���Ϣ
bool FieldOp::ModifyField(FieldModel &newField)
{
	vector<CString> list = FileOp::ReadAll(tdfPath);
	if (list.empty())
		return false;
	else
	{
		for (vector<CString>::iterator ite = list.begin() + 1; ite != list.end(); ++ite)
		{
			vector<CString> vfield = FileOp::StrSplit(*ite, CString(" "));
			if (vfield[0] == FileOp::IntegerToString(newField.GetId()))
			{
				newField.mtime = FileOp::GetCurrTime();
				CString str = FileOp::IntegerToString(newField.GetId()) + CString(" ")
					+ newField.GetName() + CString(" ")
					+ FileOp::IntegerToString(newField.GetType()) + CString(" ")
					+ FileOp::IntegerToString(newField.GetParam()) + CString(" ")
					+ newField.GetMtime() + CString(" ")
					+ FileOp::IntegerToString(newField.GetIntegrities());
				*ite = str;
				break;
			}
		}
		return FileOp::WriteRecord(tdfPath, list);
	}


	return false;
}

bool FieldOp::modifyField(CString& dbName, CString& tableName, CString& fieldName, CString& new_fieldName, int new_fieldOrder, int new_fieldType, int new_fieldParam, int new_fieldIntegrities)
{
	FieldModel field(new_fieldOrder, new_fieldName, new_fieldType, new_fieldParam, new_fieldIntegrities);
	CString Corder, CType, Cparam, Cintegrities;
	Corder.Format(_T("%d"), field.GetId());
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
	if (number == -1)
		return false;
	//�����ֶ���Ϣ��д���ļ�
		//�޸�ԭ�ֶ���Ϣ
	CString filePath = CString("./dbms_root/data") + CString("/") + dbName + CString("/") + tableName + CString("/") + tableName + CString(".tdf");
	vector<CString>	fields = FileOp::ReadAll(filePath);
	fields.at(number) = new_field;
	FileOp::WriteRecord(filePath, fields);

	//���������д��ڵ��ֶ���Ϣ���ݶ���

	//�鿴�Ƿ��м�¼��Ԫ�飬�������ݣ����ݶ���
	//����У����¼�¼

	return true;
}
bool FieldOp::dropField(CString& dbName, CString& tableName, CString& fieldName)
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
	CString filePath = CString("./dbms_root/data") + CString("/") + dbName + CString("/") + tableName + CString(".tdf");
	vector<CString>	fields = FileOp::ReadAll(filePath);

	if (!fields.empty())
	{
		for (vector<CString>::iterator ite = fields.begin(); ite != fields.end(); ++ite)
		{
			FieldModel temp(*ite);
			if (temp.GetName() == fieldName)
			{
				fields.erase(ite);
				break;
			}
		}
		return FileOp::WriteRecord(filePath, fields);
	}
	else
		return false;




	

	//��ѯ�Ƿ��б��ڼ�¼���ݶ���
	//����У����¼�¼

}


CString FieldOp::queryField(CString& dbName, CString& tableName, CString& fieldName)
{
	int number = IsFiledExist(dbName, tableName, fieldName);
	if (number == -1)
	{
		return L"No Such Field In " + tableName + " at database " +
			dbName;
	}

	CString filePath = CString("./dbms_root/data") + CString("/") + dbName + CString("/") + tableName + CString("/") + tableName + CString(".tdf");
	vector<CString>	fields = FileOp::ReadAll(filePath);
	return fields.at(number);
}

FieldModel FieldOp::queryFieldModel(CString& dbName, CString& tableName, CString& fieldName)
{
	int number = IsFiledExist(dbName, tableName, fieldName);
	if (number == -1)
	{
		return FieldModel();
	}
	CString filePath = CString("./dbms_root/data") + CString("/") + dbName + CString("/") + tableName + CString("/") + tableName + CString(".tdf");
	vector<CString>	fields = FileOp::ReadAll(filePath);

	FieldModel fieldModel(fields.at(number));
	return fieldModel;
}

vector<CString> FieldOp::queryFields(CString& dbName, CString& tableName)
{
	CString filePath = CString("./dbms_root/data") + CString("/") + dbName + CString("/") + tableName + CString("/") + tableName + CString(".tdf");
	vector<CString>	fields = FileOp::ReadAll(filePath);
	vector<CString> nameFields;
	for (int i = 0; i < fields.size(); i++)
	{
		CString field = fields.at(i);
		vector<CString> field_vct = FileOp::StrSplit(field, L" ");
		CString field_str = field_vct.at(1);
		nameFields.push_back(field_str);
	}

	return nameFields;
}

vector<FieldModel> FieldOp::queryFieldsModel(CString& dbName, CString& tableName)
{
	CString filePath = CString("./dbms_root/data") + CString("/") + dbName + CString("/") + tableName + CString(".tdf");
	vector<CString>	fields = FileOp::ReadAll(filePath);
	vector<FieldModel> modelFields;
	for (int i = 0; i < fields.size(); i++)
	{
		CString field = fields.at(i);
		FieldModel fieldModel(field);
		modelFields.push_back(fieldModel);
	}
	return modelFields;
}




//�жϱ����ļ��Ƿ����
bool FieldOp::IsTableExist(CString& dbName, CString& tableName) {
	//�жϱ��Ƿ����
	//ͨ�������ļ�op�鿴�Ƿ���ڱ�
	CString folderPath = CString("dbms_root/data") + CString("/") + dbName;
	CString tablePath = folderPath + CString("/")  + tableName + CString(".tdf");
	return PathFileExists(tablePath);
}
//�ж��ֶ��Ƿ����
int  FieldOp::IsFiledExist(CString& dbName, CString& tableName, CString& fieldName)
{

	//�ļ�·��
	CString filePath = CString("./dbms_root/data") + CString("/") + dbName  + CString("/") + tableName + CString(".tdf");
	//��ѯ�ļ���һ��һ�в�ѯ�ļ�����������Ϣ
		//�ļ�op  vector<CString> ReadAll(CString& fileName);
		//�����������ָ�鿴�Ƿ��и��ֶ�
	vector<CString>	fields = FileOp::ReadAll(filePath);
	for (int i = 0; i < fields.size(); i++)
	{
		CString field = fields.at(i);
		vector<CString> field_vct = FileOp::StrSplit(field, L" ");
		CString field_str = field_vct.at(1);
		if (field_str == fieldName)
		{
			return i;
		}
	}
	return -1;
}