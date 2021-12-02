#pragma once
#include"FileOp.h"

class LogModel
{
public:
	int id;//日志id
	CString sqlStr;//存入的sql语句
	CString mtime;//日志存入时间
	
	LogModel(int id=1,CString sqlStr=L"null");
	//LogModel(CString str);
	CString toString();

	int GetId() { return id; };
	CString GetSql() { return sqlStr; };
	CString GetMtime() { return mtime; };

	void SetId(int m_id) { id = m_id; };
	void SetSql(CString m_log) { sqlStr = m_log; };
	void SetMtime(CString m_mtime) { mtime = m_mtime; };
};

