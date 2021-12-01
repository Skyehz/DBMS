#pragma once
#include"LogModel.h"
#include"vector"
using namespace std;
class LogOp
{
	//�������ļ�����
	//��sql��������־
	//ͨ�������ļ���sql�����лָ�

	//��Ҫһ��LogModel �ռ�id�����㣩�ع���������־ʱ�䣬sql����

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

