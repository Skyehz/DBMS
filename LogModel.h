#pragma once
#include"FileOp.h"

class LogModel
{
public:
	int id;//��־id
	CString sqlStr;//�����sql���
	CString mtime;//��־����ʱ��
	
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

