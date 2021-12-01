#pragma once
#include"LogModel.h"
#include"vector"
using namespace std;
class LogOp
{
	//将所有文件备份
	//将sql语句存入日志
	//通过备份文件和sql语句进行恢复

	//需要一个LogModel 日记id（方便）回滚，存入日志时间，sql操作

private:
	CString logFilePath;
	CString dbName;
public:
	LogOp(CString dbName);
	~LogOp(void);
public:
	bool BackupDB();

	bool WriteLogs(vector<LogModel> logs);
	bool WriteOneLog(LogModel& newLog);
	

	bool Restore();
	bool RestoreDB();
	vector<LogModel> ReadLogs();

	bool IsLogExist(CString& dbName);
};

