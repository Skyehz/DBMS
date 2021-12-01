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
	//把整个数据库进行复制
	CString AimFilePath = CString("./backUp/data/") + dbName;
	CString dbPath = CString("./dbms_root/data/") + dbName;

	CopyDirectory(dbPath,AimFilePath);//需考虑文件的替换问题

	//拷贝根目录的.db文件
	CString dbFilePath = dbPath + CString("/../../") + dbName + CString(".db");
	CString dbFileTargetPath = AimFilePath + CString("/../../") + dbName + CString(".db");
	CopyFile(dbFilePath, dbFileTargetPath, FALSE);
	//拷贝data目录的.sys文件
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

//将备份复制回数据库，然后运行log文件
bool LogOp::Restore()
{
	
	//恢复备份
	if (!RestoreDB())
		return false;

	////使用sql类执行log命令
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
	//替换数据库
	CString AimFilePath = CString("./backUp/data/") + dbName;
	CString dbPath = CString("./dbms_root/data/") + dbName;
	CopyDirectory(AimFilePath, dbPath);

	//拷贝根目录的.db文件
	CString dbFilePath = dbPath + CString("/../../") + dbName + CString(".db");
	CString dbFileTargetPath = AimFilePath + CString("/../../") + dbName + CString(".db");
	//拷贝data目录的.sys文件
	CString sysFilePath = dbPath + CString("/../") + CString("dbms.sys");
	CString sysFileTargetPath = AimFilePath + CString("/../") + CString("dbms.sys");
	CopyFile(sysFileTargetPath, sysFilePath, FALSE);

	return true;
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

	//CreateDirectory(target, NULL); //获取目标拷贝的路径 拷贝至哪里
	CFileFind finder;
	CString path = source + _T("\\*.*"); //需要拷贝的文件的路径
   // AfxMessageBox(path);  //调试用
	bool bWorking = finder.FindFile(path);  //是否找到了需要拷贝的文件的路径
	while (bWorking) {
		bWorking = finder.FindNextFile();
		//AfxMessageBox(finder.GetFileName());  //调试用
		if (finder.IsDirectory() && !finder.IsDots()) { //是不是有效的文件夹
			CopyDirectory(finder.GetFilePath(), target + _T("\\") + finder.GetFileName()); //递归查找文件夹
		}
		else { 
			//判断文件类型，如果是log文件就不复制

			CString FileName=finder.GetFileName();
			CString LogName = dbName + CString(".log");
			if (FileName == LogName)
				continue;
			//CopyFile(A, B, FALSE); 表示将文件A拷贝到B，如果B已经存在则覆盖（第三参数为TRUE时表示不覆盖）
			CopyFile(finder.GetFilePath(), target + _T("\\") + finder.GetFileName(), FALSE);  //拷贝文件夹下的所有文件
		}
	}



}
