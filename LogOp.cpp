#include "pch.h"
#include "LogOp.h"

LogOp::LogOp(CString dbName)
{

	this->dbName = dbName;
	this->dbPath = CString("./dbms_root/data/") + dbName;
	this->logFilePath = CString("./dbms_root/data/") + dbName + CString("/") +
		dbName + CString(".log");
}

LogOp::~LogOp(void)
{
}

bool LogOp::BackupDB()
{	
	//���������ݿ���и���
	CString AimFilePath = CString("./backUp/") + dbName;
	CreateDirectory(AimFilePath, NULL);
	CopyDirectory(dbPath,AimFilePath);//�迼���ļ����滻����
	
	return true;
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

//�����ݸ��ƻ����ݿ⣬Ȼ������log�ļ�
bool LogOp::Restore()
{
	
	//�ָ�����
	if (!RestoreDB())
		return false;

	//ʹ��sql��ִ��log����
	vector<LogModel> logModels = ReadLogs();
	ParseSQL parseSql;
	parseSql.setDB(dbName);
	for (int  i = 0; i < logModels.size(); i++)
	{
		LogModel logModel = logModels.at(i);
		CString sql = logModel.GetSql();
		parseSql.getSql(sql);
	}
	return true;
}

bool LogOp::RestoreDB()
{
	//�滻���ݿ�
	CString AimFilePath = CString("./backUp/") + dbName;
	CopyDirectory(AimFilePath, dbPath);
	return true;
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

void LogOp::CopyDirectory(CString source, CString target)
{
	
	CreateDirectory(target, NULL); //��ȡĿ�꿽����·�� ����������
	CFileFind finder;
	CString path = source + _T("\\*.*"); //��Ҫ�������ļ���·��
   // AfxMessageBox(path);  //������
	bool bWorking = finder.FindFile(path);  //�Ƿ��ҵ�����Ҫ�������ļ���·��
	while (bWorking) {
		bWorking = finder.FindNextFile();
		//AfxMessageBox(finder.GetFileName());  //������
		if (finder.IsDirectory() && !finder.IsDots()) { //�ǲ�����Ч���ļ���
			CopyDirectory(finder.GetFilePath(), target + _T("\\") + finder.GetFileName()); //�ݹ�����ļ���
		}
		else { 
			//�ж��ļ����ͣ������log�ļ��Ͳ�����
			CString FilePath = finder.GetFilePath();
			if (FilePath == this->logFilePath)
				continue;
			
			CopyFile(FilePath, target + _T("\\") + finder.GetFileName(), FALSE);  //�����ļ����µ������ļ�
		}
	}
}
