#include "pch.h"
#include "DataOp.h"
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



//计数器
bool CDataOp::SaveRcdCounter(CString& trdFileName, int counter)
{
	return FileOp::SaveCounter(trdFileName, counter);
}


//获取id
int CDataOp::GetRcdCounter(CString& trdFileName, vector<FieldModel>& fieldList)
{
	vector<CDataModel> dataList = ReadDataList(fieldList);
	if (dataList.empty()) {
		return 0;
	}
	else {
		return dataList.back().GetId() + 1;
	}

}


//从文件新增一条记录,写入文件
bool CDataOp::WriteAnRecord(CDataModel& record, CString& trdFileName, vector<FieldModel>& fieldList)
{
	CString str = FileOp::IntegerToString(record.GetId());
	//map<CString,CString> values = record.GetValues();
	for (vector<FieldModel>::iterator ite = fieldList.begin(); ite != fieldList.end(); ++ite)
	{
		CString str1 = record.GetValue(ite->GetName());
		if (str1.Find(CString("\"")) != -1)str1 = str1.Mid(1, str1.GetLength() - 2);
		if (str1.Find(CString("'")) != -1)str1 = str1.Mid(1, str1.GetLength() - 2);
		str += CString("#") + str1;
	}
	//MessageBox(NULL,str,L"",MB_OK);
	return FileOp::AddAnLine(trdFileName, str);
}
int CDataOp::AddRecord(CDataModel& record, vector<FieldModel>& fieldList)
{
	int count = GetRcdCounter(this->trdPath, fieldList);
	record.SetId(count);
	map<CString, CString> value = record.GetValues();
	map<CString, CString>::iterator it;
	bool r = true;
	for (it = value.begin(); it != value.end(); ++it) {
		for (int i = 0; i < fieldList.size(); i++)
		{
			if (it->first == fieldList[i].GetName()) {
				if (IntegrityVerify1(it->second, fieldList[i]) != 1)r = false;
			}
		}
	}
	if (r) {
		if (!WriteAnRecord(record, trdPath, fieldList))
			return ADD_ERROR;
	}
	if (SaveRcdCounter(this->trdPath, count))
		return SAVE_COUNTER_ERROR;

	return TRUE;

}


//获取全部记录
vector<CDataModel> CDataOp::ReadDataList(vector<FieldModel>& fieldList)
{
	vector<CDataModel> res;
	vector<CString> strList = FileOp::ReadAll(trdPath);

	if (!strList.empty())
	{
		for (vector<CString>::iterator ite = strList.begin(); ite != strList.end(); ++ite)
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


//从文件修改指定的记录
bool CDataOp::ModifyRecord(CDataModel& record)
{
	vector<CString> list = FileOp::ReadAll(trdPath);
	//vector<CDataModel> list=this->ReadDataList(this->tbName)
	FieldOp fieldop(dbName, tbName);
	vector<FieldModel> fieldList = fieldop.queryFieldsModel(dbName, tbName);
	if (list.empty())
		return FALSE;
	else
	{
		for (vector<CString>::iterator ite = list.begin(); ite != list.end(); ++ite)
		{
			vector<CString> temp = FileOp::StrSplit(*ite, _T("#"));
			if (temp[0] == FileOp::IntegerToString(record.GetId()))
			{
				//*ite = record.toString();
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
	for (vector<CString>::iterator ite = list.begin(); ite != list.end(); ++ite)
	{
		vector<CString> temp = FileOp::StrSplit(*ite, _T("#"));
		if (temp[0] == FileOp::IntegerToString(id))
		{
			list.erase(ite); break;
		}
	}
	return FileOp::WriteRecord(trdFileName, list);
}
int CDataOp::DeleteRecord(int id)
{
	DeleteAnRecord(id, trdPath);
	return 307;
}

//按照字段名称删除关于该字段的记录
bool CDataOp::DeleteFieldRecord(CString& trdFileName, int index)
{
	vector<CString> list = FileOp::ReadAll(trdFileName);
	if (list.empty())
		return false;
	else
	{
		for (vector<CString>::iterator ite = list.begin(); ite != list.end(); ++ite)
		{
			int count = 0;
			CString start, end;
			bool can01 = true, can02 = true;
			for (size_t i = 0; i < ite->GetLength(); i++)
			{
				if (ite->GetAt(i) == '#') ++count;
				if (count == index && can01)
				{
					can01 = false;
					start = ite->Left(i);
				}

				if (count == index + 1 && can02)
				{
					can02 = false;
					end = ite->Right(ite->GetLength() - i);
				}
			}
			*ite = start + end;
		}

		return FileOp::WriteRecord(trdFileName, list);
	}
}
//验证是否合法
bool CDataOp::OnlyHaveNumber(CString& str)
{
	bool isOK = TRUE;
	if (str != str.SpanIncluding(_T("1234567890")))
		isOK = FALSE;
	return isOK;
}
bool CDataOp::OnlyHaveNumberAndDot(CString& str)
{
	bool isOK = TRUE;
	if (str != str.SpanIncluding(_T("1234567890.")))
		isOK = FALSE;
	return isOK;
}


//判断是否唯一
int CDataOp::IsUnique(CString& value, CString& fieldName)
{
	FieldOp* fieldOp = new FieldOp(dbName, tbName);
	vector<CDataModel> list = CDataOp::ReadDataList(fieldOp->queryFieldsModel(dbName, tbName));
	for (vector<CDataModel>::iterator ite = list.begin(); ite != list.end(); ++ite)
	{
		if (ite->GetValue(fieldName) == value)
		{
			return FALSE;
		}
	}
	return TRUE;
}


//类型判断
int CDataOp::IntegrityVerify1(CString& val, FieldModel& field)
{


	//完整性
	switch (field.GetType())
	{
	case 1://integer
	{
		if (OnlyHaveNumber(val))
		{
			//(判断是否符合范围大小)
			int iVal = FileOp::StringToInteger(val);
			//return 数字范围错误
		}
		else
		{
			return INTEGRITY_ERROR_TYPE;
		}
		break;
	}
	case 2://布尔
	{
		if (val != "true" && val != "false")
			return INTEGRITY_ERROR_TYPE;
		break;
	}
	case 3://double类型
	{
		if (OnlyHaveNumberAndDot(val))
		{
			//判断double范围是否符合（待）
		}
		else
			return INTEGRITY_ERROR_TYPE;

		break;
	}
	case 4://varchar*类型
	{

		if (val.GetLength() > field.GetParam())
		{
			return INTEGRITY_TOO_LONG;
		}
		break;
	}
	case 5://datatime类型
	{
		if (val != val.SpanIncluding(_T("1234567890-")) || val.GetLength() != 10)
			return INTEGRITY_ERROR_TYPE;
		else
		{
			//2014-09-02
			for (int i = 0; i < 10; i++)
			{
				if (i != 4 && i != 7)
				{
					if (val[i] > '9' || val[i] < '0')
						return INTEGRITY_ERROR_DATETYPE;
				}
			}

			if (val[4] != '-' || val[7] != '-')
				return INTEGRITY_ERROR_DATETYPE;
			CString date = val;
			vector<CString> ymd = FileOp::StrSplit(date, CString("-"));
			int m = FileOp::StringToInteger(ymd[1]);
			int d = FileOp::StringToInteger(ymd[2]);
			if (m > 12 || (m == 2 && d > 28))
				return INTEGRITY_ERROR_DATETYPE;

			if (((m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12) && d > 31) ||
				((m == 2 || m == 4 || m == 6 || m == 9 || m == 11) && d > 30))
				return INTEGRITY_ERROR_DATETYPE;
		}
		break;
	}
	default:
		break;
	}

	//验证主键
	if (field.IsPrimaryKey() == YES)
	{

		if (val == CString("") || this->IsUnique(val, field.GetName()) == FALSE)
		{
			return INTEGRITY_NOT_PK;
		}
	}
	else
	{
		//验证非空
		if (field.IsEmpty() == FALSE)
		{
			if (val == CString(""))
			{
				return INTEGRITY_NULL_VALUE;
			}
		}
		//验证唯一
		if (field.IsUniqueKey() == TRUE)
		{
			if (this->IsUnique(val, field.GetName()) == FALSE)
			{
				return INTEGRITY_NOT_UNIQUE;
			}
		}
	}


	return TRUE;
}

vector<CDataModel> CDataOp::QueryFieldRecord(CString& tbName, vector<CString> fieldList)
{
	FieldOp* fieldOp = new FieldOp(dbName, tbName);
	vector<FieldModel> listFile = fieldOp->queryFieldsModel(dbName, tbName);
	int index = 0;
	for (int i = 0; i < listFile.size(); i++)
	{
		for (int j = 0; j < fieldList.size(); j++)
		{
			if (listFile[i].GetName() == fieldList[j])
			{
				index = i;
				break;
			}
		}
	}
	vector<CString> list = FileOp::ReadAll(tbName);
	for (vector<CString>::iterator ite = list.begin(); ite != list.end(); ++ite)
	{
		int count = 0;
		CString start, end;
		bool can01 = true, can02 = true;
		for (size_t i = 0; i < ite->GetLength(); i++)
		{
			if (ite->GetAt(i) == '#') ++count;
			if (count == index && can01)
			{
				can01 = false;
				start = ite->Left(i);
			}

			if (count == index + 1 && can02)
			{
				can02 = false;
				end = ite->Right(ite->GetLength() - i);
			}
		}
		*ite = start + end;
	}
	return vector<CDataModel>();
}
int CDataOp::IntegrityVerify(CDataModel& record, FieldModel& field)
{
	CString val = record.GetValue(field.GetName());

	return this->IntegrityVerify1(val, field);
}
