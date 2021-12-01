#include "pch.h"
#include "LogOp.h"

#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")

LogOp::LogOp(CString dbName)
{

	this->dbName = dbName;
	this->logFilePath = CString("./dbms_root/data/") + dbName + CString("/") +
		dbName + CString(".log");
	InitBackUp(dbName);
}

LogOp::~LogOp(void)
{
}

bool LogOp::BackupDB()
{	
	//���������ݿ���и���
	CString AimFilePath = CString("./backUp/data/") + dbName;
	CString dbPath = CString("./dbms_root/data/") + dbName;

	CopyDirectory(dbPath,AimFilePath);//�迼���ļ����滻����

	//������Ŀ¼��.db�ļ�
	CString dbFilePath = dbPath + CString("/../../") + dbName + CString(".db");
	CString dbFileTargetPath = AimFilePath + CString("/../../") + dbName + CString(".db");
	CopyFile(dbFilePath, dbFileTargetPath, FALSE);
	//����dataĿ¼��.sys�ļ�
	CString sysFilePath = dbPath + CString("/../") + CString("dbms.sys");
	CString sysFileTargetPath = AimFilePath + CString("/../") + CString("dbms.sys");
	CopyFile(sysFilePath, sysFileTargetPath, FALSE);

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

	////ʹ��sql��ִ��log����
	//vector<LogModel> logModels = ReadLogs();
	//ParseSQL parseSql;
	//parseSql.setDB(dbName);
	//for (int  i = 0; i < logModels.size(); i++)
	//{
	//	LogModel logModel = logModels.at(i);
	//	CString sql = logModel.GetSql();
	//	parseSql.getSql(sql);
	//}
	return true;
}

bool LogOp::RestoreDB()
{
	//�滻���ݿ�
	CString AimFilePath = CString("./backUp/data/") + dbName;
	CString dbPath = CString("./dbms_root/data/") + dbName;
	CopyDirectory(AimFilePath, dbPath);

	//������Ŀ¼��.db�ļ�
	CString dbFilePath = dbPath + CString("/../../") + dbName + CString(".db");
	CString dbFileTargetPath = AimFilePath + CString("/../../") + dbName + CString(".db");
	//����dataĿ¼��.sys�ļ�
	CString sysFilePath = dbPath + CString("/../") + CString("dbms.sys");
	CString sysFileTargetPath = AimFilePath + CString("/../") + CString("dbms.sys");
	CopyFile(sysFileTargetPath, sysFilePath, FALSE);

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

void LogOp::InitBackUp(CString& dbName)
{
	CString  path = CString("./backUp");
	if (!PathIsDirectory(path))
		CreateDirectory(path, NULL);
	path += "/data";
	if (!PathIsDirectory(path))
		CreateDirectory(path, NULL);

}

void LogOp::CopyDirectory(CString source, CString target)
{
	if (!PathIsDirectory(target))
		CreateDirectory(target, NULL);

	//CreateDirectory(target, NULL); //��ȡĿ�꿽����·�� ����������
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

			CString FileName=finder.GetFileName();
			CString LogName = dbName + CString(".log");
			if (FileName == LogName)
				continue;
			//CopyFile(A, B, FALSE); ��ʾ���ļ�A������B�����B�Ѿ������򸲸ǣ���������ΪTRUEʱ��ʾ�����ǣ�
			CopyFile(finder.GetFilePath(), target + _T("\\") + finder.GetFileName(), FALSE);  //�����ļ����µ������ļ�
		}
	}



}
