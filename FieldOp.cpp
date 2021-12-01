#include "pch.h"
#include "FieldOp.h"
#include"FileOp.h"
#include "ParseSQL.h"
//MFC判断目录下是否包含指定文件。 含文件名。
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

	//判断表定义tdf文件是否存在
	if (!IsTableExist(dbName, tbName))
		return false;
	//查看表内tdf字段是否存在

	//创建字段实体

	/*FieldModel field(newField.GetId(), newField.GetName(), newField.GetType(), newField.GetParam(), newField.GetIntegrities());*/
	//增加字段信息
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

//修改字段信息
bool FieldOp::ModifyField(FieldModel& newField, int type)
{
	//判断表定义文件tdf是否存在
	if (!IsTableExist(dbName, tbName))
		return false;
	//若不是修改字段名,则判断字段是否存在
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
			//修改字段名
			if (type== ALTER_CHANGE_FIELDNAME && vfield[1] == condition[0])	
			{
				FieldModel newField(FileOp::StringToInteger(vfield[0]), //id
					condition[1], //name
					FileOp::GetTypeInt(FileOp::getbeforebrakets(condition[2])),	//类型
					FileOp::StringToInteger(FileOp::getbrakets(condition[2])),	//长度
					FileOp::StringToInteger(vfield[5]), //完整性
					FileOp::StringToInteger(vfield[6]),	//主键
					FileOp::StringToInteger(vfield[7]),	//唯一值
					vfield[8], //默认值
					vfield[9], //注释
					FileOp::StringToInteger(vfield[10]));	//非空
				return ModifyField(newField, type);
			}
			//修改字段类型
			else if (type == ALTER_CHANGE_FIELDTYPE && vfield[1] == condition[0]) {
				FieldModel newField(FileOp::StringToInteger(vfield[0]), //id
					vfield[1], //name
					FileOp::GetTypeInt(FileOp::getbeforebrakets(condition[1])),	//类型
					FileOp::StringToInteger(FileOp::getbrakets(condition[1])),	//长度
					FileOp::StringToInteger(vfield[5]), //完整性
					FileOp::StringToInteger(vfield[6]),	//主键
					FileOp::StringToInteger(vfield[7]),	//唯一值
					vfield[8], //默认值
					vfield[9], //注释
					FileOp::StringToInteger(vfield[10]));	//非空
				return ModifyField(newField, type);
			}
			//设置为非空
			else if (type == ALTER_ADD_CONSTRANIT_NOTNULL && vfield[1] == condition[0]) {
				FieldModel newField(FileOp::StringToInteger(vfield[0]), //id
					vfield[1], //name
					FileOp::StringToInteger(vfield[2]),	//类型
					FileOp::StringToInteger(vfield[3]),	//长度
					FileOp::StringToInteger(vfield[5]), //完整性
					FileOp::StringToInteger(vfield[6]),	//主键
					FileOp::StringToInteger(vfield[7]),	//唯一值
					vfield[8], //默认值
					vfield[9], //注释
					1);	//非空
				return ModifyField(newField, type);
			}
			//设置为主键
			else if (type == ALTER_ADD_CONSTRANIT_PK ) {
				vector<CString> fieldNames = FileOp::StrSplit(condition[1], CString(","));
				for (int j = 0; j < fieldNames.size(); j++) {
					if (vfield[1] == fieldNames[j]) {
						FieldModel newField(FileOp::StringToInteger(vfield[0]), //id
							vfield[1], //name
							FileOp::StringToInteger(vfield[2]),	//类型
							FileOp::StringToInteger(vfield[3]),	//长度
							FileOp::StringToInteger(vfield[5]), //完整性
							1,	//主键
							FileOp::StringToInteger(vfield[7]),	//唯一值
							vfield[8], //默认值
							vfield[9], //注释
							FileOp::StringToInteger(vfield[10]));	//非空
						ModifyField(newField, type);
						break;
					}
				}
			}
			//设置为唯一值
			else if (type == ALTER_ADD_CONSTRANIT_UNIQUE) {
				vector<CString> fieldNames = FileOp::StrSplit(condition[1], CString(","));
				for (int j = 0; j < fieldNames.size(); j++) {
					if (vfield[1] == fieldNames[j]) {
						FieldModel newField(FileOp::StringToInteger(vfield[0]), //id
							vfield[1], //name
							FileOp::StringToInteger(vfield[2]),	//类型
							FileOp::StringToInteger(vfield[3]),	//长度
							FileOp::StringToInteger(vfield[5]), //完整性
							FileOp::StringToInteger(vfield[6]),	//主键
							1,	//唯一值
							vfield[8], //默认值
							vfield[9], //注释
							FileOp::StringToInteger(vfield[10]));	//非空
						ModifyField(newField, type);
						break;
					}
				}
			}
			//设置默认值
			else if (type == ALTER_ADD_CONSTRANIT_DEFAULT && vfield[1] == condition[0]) {
				FieldModel newField(FileOp::StringToInteger(vfield[0]), //id
					vfield[1], //name
					FileOp::StringToInteger(vfield[2]),	//类型
					FileOp::StringToInteger(vfield[3]),	//长度
					FileOp::StringToInteger(vfield[5]), //完整性
					FileOp::StringToInteger(vfield[6]),	//主键
					FileOp::StringToInteger(vfield[7]),	//唯一值
					condition[2], //默认值
					vfield[9], //注释
					FileOp::StringToInteger(vfield[10]));	//非空
				return ModifyField(newField, type);
			}

		}
		return flag;
	}

	return false;
}



bool FieldOp::dropField(CString& dbName, CString& tableName, CString& fieldName)
{
	//判断表定义文件tdf是否存在
	if (!IsTableExist(dbName, tableName))
		return false;
	//判断字段是否存在
	int number = IsFiledExist(dbName, tableName, fieldName);
	if (number == -1)
		return false;
	//判断是否有索引在使用当前字段（暂定）
	//如果有索引则删除当前字段(暂定)

	//删除字段信息
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






	//查询是否有表内记录（暂定）
	//如果有，更新记录

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




//判断表定义文件是否存在
bool FieldOp::IsTableExist(CString& dbName, CString& tableName) {
	//判断表是否存在
	//通过调用文件op查看是否存在表
	CString folderPath = CString("dbms_root/data") + CString("/") + dbName;
	CString tablePath = folderPath + CString("/") + tableName + CString(".tdf");
	return PathFileExists(tablePath);
}


//判断字段是否存在
int  FieldOp::IsFiledExist(CString& dbName, CString& tableName, CString& fieldName)
{

	//文件路径
	CString filePath = CString("./dbms_root/data") + CString("/") + dbName + CString("/") + tableName + CString(".tdf");
	//查询文件，一行一行查询文件名，返回信息
		//文件op  vector<CString> ReadAll(CString& fileName);
		//遍历向量，分割，查看是否有该字段
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