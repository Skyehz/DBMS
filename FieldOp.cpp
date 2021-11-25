#include "pch.h"
#include "FieldOp.h"
#include"FileOp.h"
#include"FieldModel.h"

//MFC判断目录下是否包含指定文件。 含文件名。
#include <shlwapi.h>
#pragma comment(lib,"Shlwapi.lib") 


//FieldOp::FieldOp() {
//
//	this->tdfpath = CString("./dbms_root/data/db1/");
//}
//FieldOp::FieldOp(CString& tdfpath) {
//	//包含文件名的路径
//	this->tdfpath = tdfpath;
//}

bool FieldOp::addFiled(CString& dbName, CString& tableName, CString& fieldName, int fieldOrder, int fieldType, int fieldParam, int fieldIntegrities)
{

	//判断表定义tdf文件是否存在
	if (!IsTableExist(dbName, tableName))
		return false;
	//查看表内tdf字段是否存在

	//创建字段实体
	FieldModel field(fieldOrder, fieldName, fieldType, fieldParam, fieldIntegrities);
	//增加字段信息
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



	//判断表定义文件tdf是否存在
	if (!IsTableExist(dbName, tableName))
		return false;
	//判断字段是否存在
	int number = IsFiledExist(dbName, tableName, fieldName);
	if (number==-1)
		return false;
	//更新字段信息，写入文件
		//修改原字段信息
	vector<CString>	fields = FileOp::ReadAll(filePath);
	fields.at(number) = new_field;
	CString filePath = CString("./dbms_root /data") + CString(" / ") + dbName + CString(" / ") + tableName + CString(".tb");
	FileOp::WriteRecord(filePath, fields);

	//更新索引中存在的字段信息（暂定）

	//查看是否有记录（元组，表内数据）（暂定）
	//如果有，更新记录

	return true;
}
bool FieldOp::dropFiled(CString& dbName, CString& tableName, CString& fieldName)
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
	vector<CString>	fields = FileOp::ReadAll(filePath);

	vector<CString>::iterator it=fields.begin();
	it = it + number;
	fields.erase(it);

	//查询是否有表内记录（暂定）
	//如果有，更新记录
	
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




//判断表定义文件是否存在
bool FieldOp::IsTableExist(CString& dbName,CString& tableName) {
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
	CString filePath = CString("./dbms_root /data") + CString(" / ") + dbName + CString(" / ") + tableName + CString(".tb");
	//查询文件，一行一行查询文件名，返回信息
		//文件op  vector<CString> ReadAll(CString& fileName);
		//遍历向量，分割，查看是否有该字段
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
