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
	//���������ݿ���и���
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
	
	//�ָ�����
	if (!RestoreDB())
		return false;

	
	vector<LogModel> logModels = ReadLogs();
	//ִ��log����


	return false;
}

bool LogOp::RestoreDB()
{
	//��ѹ�ļ��滻���ݿ�
	return false;
}

vector<LogModel> LogOp::ReadLogs()
{
	//��ȡlog�ļ�
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
