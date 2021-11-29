#include "pch.h"
#include "DataOp.h"
CDataOp::CDataOp(CString dbname, CString tableName)
{
	this->tdfPath = CString("dbms_root/data/") + dbname + CString("/") + tableName + CString(".tdf");
	this->trdPath = CString("dbms_root/data/") + dbname + CString("/") + tableName + CString(".trd");
	this->dbName = dbname;
	this->tbName = tableName;
}


CDataOp::~CDataOp(void)
{
}

//从文件新增一条记录,写入文件
bool CDataOp::WriteAnRecord(CDataModel& record, CString& trdFileName, vector<FieldModel>& fieldList)
{
	CString str = FileOp::IntegerToString(record.GetId());
	//map<CString,CString> values = record.GetValues();
	for (vector<FieldModel>::iterator ite = fieldList.begin(); ite != fieldList.end(); ++ite)
	{
		str += CString("#") + record.GetValue(ite->GetName());
	}
	//MessageBox(NULL,str,L"",MB_OK);
	return FileOp::AddAnLine(trdFileName, str);
}
bool CDataOp::SaveRcdCounter(CString& trdFileName, int counter)
{
	return FileOp::SaveCounter(trdFileName, counter);
}
int CDataOp::GetRcdCounter(CString& trdFileName)
{
	CString firstLine = FileOp::ReadFirstLine(trdFileName);
	CString res = FileOp::StrSplit(firstLine, CString("#"))[0];
	return FileOp::StringToInteger(res);
}
//获取全部记录
vector<CDataModel> CDataOp::ReadDataList(CString& trdFileName, vector<FieldModel>& fieldList)
{
	vector<CDataModel> res;
	vector<CString> strList = FileOp::ReadAll(trdFileName);

	if (!strList.empty())
	{
		for (vector<CString>::iterator ite = strList.begin() + 1; ite != strList.end(); ++ite)
		{
			vector<CString> tmpList = FileOp::StrSplit(*ite, _T("#"));
			CDataModel tmpRecord = CDataModel(FileOp::StringToInteger(tmpList[0]));
			vector<FieldModel>::iterator ite2 = fieldList.begin();
			for (int i = 1; i < tmpList.size(); i++)
			{
				tmpRecord.SetValue(ite2->GetName(), tmpList[i]);
				ite2++;
			}
			res.push_back(tmpRecord);
		}
	}
	return res;
}
int CDataOp::AddRecord(CDataModel& record, vector<FieldModel>& fieldList)
{
	
	int count = GetRcdCounter(this->trdPath);
	record.SetId(++count);

	if (!WriteAnRecord(record, trdPath, fieldList))
		return ADD_ERROR;
	if (SaveRcdCounter(this->trdPath, count))
		return SAVE_COUNTER_ERROR;


	return YES;

}
//从文件修改指定的记录
bool CDataOp::ModifyRecord(CDataModel& record)
{
	vector<CString> list = FileOp::ReadAll(trdPath);
	FieldOp fieldop(dbName,tbName);
	vector<FieldModel> fieldList = fieldop.queryFieldsModel(dbName,tbName);
	if (list.empty())
		return false;
	else
	{
		for (vector<CString>::iterator ite = list.begin(); ite != list.end(); ++ite)
		{
			vector<CString> temp = FileOp::StrSplit(*ite, _T("#"));
			if (temp[0] == FileOp::IntegerToString(record.GetId()))
			{
				CString str = FileOp::IntegerToString(record.GetId());
				for (vector<FieldModel>::iterator itei = fieldList.begin(); itei != fieldList.end(); ++itei)
				{
					str += CString("#") + record.GetValue(itei->GetName());
				}
				*ite = str;
				break;
			}
		}

		return FileOp::WriteRecord(trdPath, list);
	}

}
//从文件删除一条记录
bool CDataOp::DeleteAnRecord(int id, CString& trdFileName)
{
	vector<CString> list = FileOp::ReadAll(trdFileName);
	/*if (list.empty())
		return false;
	else
	{*/
		for (vector<CString>::iterator ite = list.begin() + 1; ite != list.end(); ++ite)
		{
			vector<CString> temp = FileOp::StrSplit(*ite, _T("#"));
			if (temp[0] == FileOp::IntegerToString(id))
			{
				list.erase(ite); break;
			}
		}
		return FileOp::WriteRecord(trdFileName, list);
	//}
}
int CDataOp::DeleteRecord(int id)
{
	DeleteAnRecord(id, trdPath);
	return 307;
}