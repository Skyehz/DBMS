#include "pch.h"
#include "LogModel.h"

#include<atlconv.h>

LogModel::LogModel(int id, CString sqlStr)
	:id(id),sqlStr(sqlStr)
{
	mtime = FileOp::GetCurrTime();
}

LogModel::LogModel(CString str)
{
	vector<CString> log = FileOp::StrSplit(str, L" ");
	USES_CONVERSION;
	int m_id = atoi(T2A(log.at(0)));

	this->id = m_id;
	this->sqlStr = log.at(1);
	this->mtime = log.at(2);

}

CString LogModel::toString()
{
	CString Cid;
	Cid.Format(_T("%d"), this->GetId());
	CString str = Cid + CString(" ") + this->GetSql() + CString(" ") + this->GetMtime();
	return str;
}
