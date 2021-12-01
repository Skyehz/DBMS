#include "pch.h"
#include "FieldOp.h"
#include"FileOp.h"
#include "ParseSQL.h"
//MFC�ж�Ŀ¼���Ƿ����ָ���ļ��� ���ļ�����
#include <shlwapi.h>
#include "DataOp.h"
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
	vector<CString> nowFields = FileOp::ReadAll(tdfPath);
	if (nowFields.empty()) {
		FieldModel fm;
		fm.SetId(0);
		fm.SetName(CString("#"));
		AddOneField(fm);
	}
	if (!fields.empty())
	{
		for (int i = 0; i < fields.size(); i++)
		{
			AddOneField(fields[i]);
		}
		return true;
	}
	else
		return false;

}

bool FieldOp::AddOneField(FieldModel& newField)
{

	//�жϱ���tdf�ļ��Ƿ����
	if (!IsTableExist(dbName, tbName))
		return false;
	//�鿴����tdf�ֶ��Ƿ����

	//�����ֶ�ʵ��

	/*FieldModel field(newField.GetId(), newField.GetName(), newField.GetType(), newField.GetParam(), newField.GetIntegrities());*/
	//�����ֶ���Ϣ
	if (!addFieldInfo(dbName, tbName, newField))
	{
		return false;
	}
	return true;
}


bool FieldOp::addFieldInfo(CString& dbName, CString& tableName, FieldModel field)
{
	CString filePath = CString("./dbms_root/data") + CString("/") + dbName +
		CString("/") + tableName + CString(".tdf");

	CString str = field.toString();
	return (FileOp::AddAnLine(filePath, str));
}

//�޸��ֶ���Ϣ
bool FieldOp::ModifyField(FieldModel& newField, int type)
{
	//�жϱ����ļ�tdf�Ƿ����
	if (!IsTableExist(dbName, tbName))
		return false;
	//�������޸��ֶ���,���ж��ֶ��Ƿ����
	if (type != 1) {
		int number = IsFiledExist(dbName, tbName, newField.GetName());
		if (number == -1)
			return false;
	}
	vector<CString> list = FileOp::ReadAll(tdfPath);
	if (list.empty())
		return false;
	else
	{
		for (vector<CString>::iterator ite = list.begin(); ite != list.end(); ++ite)
		{
			vector<CString> vfield = FileOp::StrSplit(*ite, CString("#"));
			if (vfield[0] == FileOp::IntegerToString(newField.GetId()))
			{
				newField.mtime = FileOp::GetCurrTime();
				*ite = newField.toString();
				break;
			}
		}
		return FileOp::WriteRecord(tdfPath, list);
	}


	return false;
}

bool FieldOp::ModifyFieldSQL(vector<CString> condition, int type) {
	vector<CString> fieldlist = FileOp::ReadAll(tdfPath);
	bool flag = false;
	if (fieldlist.empty())
		return false;
	else
	{
		for (vector<CString>::iterator ite = fieldlist.begin(); ite != fieldlist.end(); ++ite)
		{
			vector<CString> vfield = FileOp::StrSplit(*ite, CString("#"));
			//�޸��ֶ���
			if (type== ALTER_CHANGE_FIELDNAME && vfield[1] == condition[0])	
			{
				FieldModel newField(FileOp::StringToInteger(vfield[0]), //id
					condition[1], //name
					FileOp::GetTypeInt(FileOp::getbeforebrakets(condition[2])),	//����
					FileOp::StringToInteger(FileOp::getbrakets(condition[2])),	//����
					FileOp::StringToInteger(vfield[5]), //������
					FileOp::StringToInteger(vfield[6]),	//����
					FileOp::StringToInteger(vfield[7]),	//Ψһֵ
					vfield[8], //Ĭ��ֵ
					vfield[9], //ע��
					FileOp::StringToInteger(vfield[10]));	//�ǿ�
				return ModifyField(newField, type);
			}
			//�޸��ֶ�����
			else if (type == ALTER_CHANGE_FIELDTYPE && vfield[1] == condition[0]) {
				FieldModel newField(FileOp::StringToInteger(vfield[0]), //id
					vfield[1], //name
					FileOp::GetTypeInt(FileOp::getbeforebrakets(condition[1])),	//����
					FileOp::StringToInteger(FileOp::getbrakets(condition[1])),	//����
					FileOp::StringToInteger(vfield[5]), //������
					FileOp::StringToInteger(vfield[6]),	//����
					FileOp::StringToInteger(vfield[7]),	//Ψһֵ
					vfield[8], //Ĭ��ֵ
					vfield[9], //ע��
					FileOp::StringToInteger(vfield[10]));	//�ǿ�
				return ModifyField(newField, type);
			}
			//����Ϊ�ǿ�
			else if (type == ALTER_ADD_CONSTRANIT_NOTNULL && vfield[1] == condition[0]) {
				FieldModel newField(FileOp::StringToInteger(vfield[0]), //id
					vfield[1], //name
					FileOp::StringToInteger(vfield[2]),	//����
					FileOp::StringToInteger(vfield[3]),	//����
					FileOp::StringToInteger(vfield[5]), //������
					FileOp::StringToInteger(vfield[6]),	//����
					FileOp::StringToInteger(vfield[7]),	//Ψһֵ
					vfield[8], //Ĭ��ֵ
					vfield[9], //ע��
					1);	//�ǿ�
				return ModifyField(newField, type);
			}
			//����Ϊ����
			else if (type == ALTER_ADD_CONSTRANIT_PK ) {
				vector<CString> fieldNames = FileOp::StrSplit(condition[1], CString(","));
				for (int j = 0; j < fieldNames.size(); j++) {
					if (vfield[1] == fieldNames[j]) {
						FieldModel newField(FileOp::StringToInteger(vfield[0]), //id
							vfield[1], //name
							FileOp::StringToInteger(vfield[2]),	//����
							FileOp::StringToInteger(vfield[3]),	//����
							FileOp::StringToInteger(vfield[5]), //������
							1,	//����
							FileOp::StringToInteger(vfield[7]),	//Ψһֵ
							vfield[8], //Ĭ��ֵ
							vfield[9], //ע��
							FileOp::StringToInteger(vfield[10]));	//�ǿ�
						ModifyField(newField, type);
						break;
					}
				}
			}
			//����ΪΨһֵ
			else if (type == ALTER_ADD_CONSTRANIT_UNIQUE) {
				vector<CString> fieldNames = FileOp::StrSplit(condition[1], CString(","));
				for (int j = 0; j < fieldNames.size(); j++) {
					if (vfield[1] == fieldNames[j]) {
						FieldModel newField(FileOp::StringToInteger(vfield[0]), //id
							vfield[1], //name
							FileOp::StringToInteger(vfield[2]),	//����
							FileOp::StringToInteger(vfield[3]),	//����
							FileOp::StringToInteger(vfield[5]), //������
							FileOp::StringToInteger(vfield[6]),	//����
							1,	//Ψһֵ
							vfield[8], //Ĭ��ֵ
							vfield[9], //ע��
							FileOp::StringToInteger(vfield[10]));	//�ǿ�
						ModifyField(newField, type);
						break;
					}
				}
			}
			//����Ĭ��ֵ
			else if (type == ALTER_ADD_CONSTRANIT_DEFAULT && vfield[1] == condition[0]) {
				FieldModel newField(FileOp::StringToInteger(vfield[0]), //id
					vfield[1], //name
					FileOp::StringToInteger(vfield[2]),	//����
					FileOp::StringToInteger(vfield[3]),	//����
					FileOp::StringToInteger(vfield[5]), //������
					FileOp::StringToInteger(vfield[6]),	//����
					FileOp::StringToInteger(vfield[7]),	//Ψһֵ
					condition[2], //Ĭ��ֵ
					vfield[9], //ע��
					FileOp::StringToInteger(vfield[10]));	//�ǿ�
				return ModifyField(newField, type);
			}

		}
		return flag;
	}

	return false;
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
	CString filePath = CString("./dbms_root/data") + CString("/") + dbName + CString("/") + tableName + CString(".tdf");
	vector<CString>	fields = FileOp::ReadAll(filePath);

	FieldModel fieldModel(fields.at(number));
	return fieldModel;
}

vector<CString> FieldOp::queryFields(CString& dbName, CString& tableName)
{
	CString filePath = CString("./dbms_root/data") + CString("/") + dbName + CString("/") + tableName + CString(".tdf");
	vector<CString>	fields = FileOp::ReadAll(filePath);
	vector<CString> nameFields;
	for (int i = 0; i < fields.size(); i++)
	{
		CString field = fields.at(i);
		vector<CString> field_vct = FileOp::StrSplit(field, L"#");
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
	CString tablePath = folderPath + CString("/") + tableName + CString(".tdf");
	return PathFileExists(tablePath);
}


//�ж��ֶ��Ƿ����
int  FieldOp::IsFiledExist(CString& dbName, CString& tableName, CString& fieldName)
{

	//�ļ�·��
	CString filePath = CString("./dbms_root/data") + CString("/") + dbName + CString("/") + tableName + CString(".tdf");
	//��ѯ�ļ���һ��һ�в�ѯ�ļ�����������Ϣ
		//�ļ�op  vector<CString> ReadAll(CString& fileName);
		//�����������ָ�鿴�Ƿ��и��ֶ�
	vector<CString>	fields = FileOp::ReadAll(filePath);
	for (int i = 0; i < fields.size(); i++)
	{
		CString field = fields.at(i);
		vector<CString> field_vct = FileOp::StrSplit(field, L"#");
		CString field_str = field_vct.at(1);
		if (field_str == fieldName)
		{
			return i;
		}
	}
	return -1;
}
int FieldOp::DeleteField(CString& fieldName)
{
	vector<FieldModel> list = FieldOp::queryFieldsModel(dbName, tbName);
	int index = 0;

	for (int i = 0; i < list.size(); i++)
	{
		if (list[i].GetName() == fieldName)
		{
			index = i+1;
			break;
		}
	}

	vector<CString> flist = FileOp::ReadAll(trdPath);
	if (!flist.empty()) {
		if (!CDataOp::DeleteFieldRecord(trdPath, index))
			return FALSE;
	}
	if (!FieldOp::dropField(dbName, tbName, fieldName))
		return FALSE;
	else
		return TRUE;
}