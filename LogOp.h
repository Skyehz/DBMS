#pragma once
#include"LogModel.h"
#include"ParseSQL.h"
#include"vector"
using namespace std;
class LogOp
{

	//���ݵ�ǰ���ݿ������
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

	void InitBackUp(CString& dbName);
	void CopyDirectory(CString source, CString target); //��ָ��·���ļ��и��Ƶ�Ŀ��·���ļ���
};