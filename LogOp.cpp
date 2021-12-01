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
	//把整个数据库进行复制
	CString AimFilePath = CString("./backUp/") + dbName;
	CreateDirectory(AimFilePath, NULL);
	CopyDirectory(dbPath,AimFilePath);//需考虑文件的替换问题
	
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

//将备份复制回数据库，然后运行log文件
bool LogOp::Restore()
{
	
	//恢复备份
	if (!RestoreDB())
		return false;

	//使用sql类执行log命令
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
	//替换数据库
	CString AimFilePath = CString("./backUp/") + dbName;
	CopyDirectory(AimFilePath, dbPath);
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

void LogOp::CopyDirectory(CString source, CString target)
{
	
	CreateDirectory(target, NULL); //获取目标拷贝的路径 拷贝至哪里
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
			CString FilePath = finder.GetFilePath();
			if (FilePath == this->logFilePath)
				continue;
			
			CopyFile(FilePath, target + _T("\\") + finder.GetFileName(), FALSE);  //拷贝文件夹下的所有文件
		}
	}
}
