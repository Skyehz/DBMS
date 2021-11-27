#pragma once
class ParseSQL
{
public:
	void getSql(CString& statement);
	CString dbmName;
	void setDB(CString& dbmName);
};


