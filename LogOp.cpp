#include "pch.h"
#include "LogOp.h"

LogOp::LogOp(CString dbName)
{

	this->dbName = dbName;
	this->logFilePath = CString("./dbms_root/data/") + dbName + CString("/") +
		dbName + CString(".log");
}

LogOp::~LogOp(void)
{
}

bool LogOp::BackupDB()
{	
	//把整个数据库进行复制
	CreateDirectory(L"./backUp", NULL);


	return false;
}

bool LogOp::WriteLogs(vector<LogModel> logs)
{
	if (!logs.empty()) {
		for (int i = 0; i < logs.size(); i++)
		{
			WriteOneLog(logs[i]);
		}
		return true;
	}
	else
		return false;

}

bool LogOp::WriteOneLog(LogModel& newLog)
{
	return false;

	if (!IsLogExist(dbName))
		return false;
	
	CString str = newLog.toString();
	return(FileOp::AddAnLine(logFilePath, str));
}

bool LogOp::Restore()
{
	
	//恢复备份
	if (!RestoreDB())
		return false;

	
	vector<LogModel> logModels = ReadLogs();
	//执行log命令


	return false;
}

bool LogOp::RestoreDB()
{
	//解压文件替换数据库
	return false;
}

vector<LogModel> LogOp::ReadLogs()
{
	//读取log文件
	vector<CString>	logs = FileOp::ReadAll(this->logFilePath);
	vector<LogModel> logModels;
	for (int i = 0; i < logs.size(); i++)
	{
		CString log = logs.at(i);
		LogModel logModel(log);
		logModels.push_back(logModel);
	}
	return logModels;
}

bool LogOp::IsLogExist(CString& dbName)
{
	CString logPath = CString("./dbms_root/data/") + dbName + CString("/") +
		dbName + CString(".log");
	return PathFileExists(logPath);
}
