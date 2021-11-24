#include "pch.h"
#include "DBOp.h"
#include "FileOp.h"
#include "DBModel.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <string>
using namespace std;

CDBOp::CDBOp() {
	this->sysPath = CString("./dbms_root/data/dbms.sys");
	//InitDatabase();
}

//向sys文件中添加一条数据库信息
bool WriteAnDB(CDBModel db,CString sysPath) {
	CString Cid, Ctype;
	Cid.Format(_T("%d"), db.GetId());
	Cid.Format(_T("%d"), db.GetType());
	CString str = Cid + CString("#") + db.GetName() + CString("#") + Ctype + CString("#")  + CString("#") + db.GetCreateTime();
	return(FileOp::AddAnLine(sysPath, str));
}

//创建文件夹并创建.db文件
bool InitDBFile(CString& dbName)
{
	//为该数据库创建文件夹
	CString folderPath = CString("./dbms_root/") + CString("data/") + dbName;
	bool crtFolder = CreateDirectory(folderPath, NULL);

	//在该文件夹下创建数据库描述文件： 数据库名.db文件
	CString dbPath = CString("./dbms_root/") + dbName + CString(".db");
	vector<CString> init;
	init.push_back(CString("0#") + dbName);
	bool crtDBFile = FileOp::WriteRecord(dbPath, init);

	return crtFolder && crtDBFile;
}

//创建.tb文件
bool InitTBFile(CString& dbName) {
	CString tbPath = CString("./dbms_root/data/") + dbName + CString("/") + dbName + CString(".tb");
	vector<CString> init;
	init.push_back(CString("0#") + dbName);
	bool crtTBFile = FileOp::WriteRecord(tbPath, init);

	return crtTBFile;
}

//创建.log日志文件
bool InitLogFile(CString& dbName) {
	CString logPath = CString("./dbms_root/data/") + dbName + CString("/") + dbName + CString(".log");
	CString info = CString("created database:") + dbName + CString("  ") + FileOp::GetCurrTime();
	bool crtLogFile = FileOp::WriteLog(info, logPath);

	return crtLogFile;
}

int CDBOp::InitDatabase() {
	//构建数据库dbms__root 文件夹
	CString folderPath = CString("./dbms_root");
	bool crtFolder = CreateDirectory(folderPath, NULL);
	//构建data文件夹
	CString folderDataPath = CString("./dbms_root/data");
	bool crtFolderData = CreateDirectory(folderDataPath, NULL);
	return 0;
}

//接受数据库名，创建数据库
int CDBOp::CreateDatabase(CString& dbName) {
	//创建数据库实体
	CDBModel db(0,dbName,1);

	//向dbms.sys文件中添加数据库信息
	if (!WriteAnDB(db, sysPath))
		return false;
	//初始化.db文件并创建文件夹
	if (!InitDBFile(dbName))
		return false;
	//在该文件夹下创建表描述文件： 数据库名.tb文件
	if (!InitTBFile(dbName))
		return false;
	//在该文件夹下创建日志文件： 数据库名.log文件
	if(!InitLogFile(dbName))
		return false;


	return 0;
}

int DeleteDBRecord(CString& sysFileName, CString& dbname)
{
	vector<CString> list = FileOp::ReadAll(sysFileName);
	if (list.empty())
		return false;
	else
	{
		vector<CString>::iterator ite = list.begin();
		++ite;
		for (; ite != list.end(); ++ite)
		{
			vector<CString> temp = FileOp::StrSplit(*ite, CString("#"));
			//如果找到指定的记录
			if (temp[1] == dbname)
			{
				list.erase(ite);
				break;
			}
		}
		if (FileOp::WriteRecord(sysFileName, list))
			return true;
		else
			return false;
	}

}



//删除指定的数据库
int CDBOp::DropDatabase(CString& dbname)
{
	if (!DeleteDBRecord(this->sysPath, dbname) || !DeleteFile(CString("./dbms_root/") + dbname + CString(".db")) || !FileOp::DeleteFolder(CString("./dbms_root/data/") + dbname))
	{
		return false;
	}
	else
	{
		/*CSystemLogic sysLogic;
		sysLogic.WriteLog(CString("deleted database:") + dbname);*/
		return true;
	}
	return 0;
}

//获取当前数据库列表
vector<CDBModel> CDBOp::GetDatabaseList()
{
	vector<CDBModel> res;
	vector<CString> strList = FileOp::ReadAll(CString("./dbms_root/data/dbms.sys"));
	//MessageBox(NULL,CUtil::IntegerToString(strList.size()),L"",MB_OK);
	if (!strList.empty())
	{
		vector<CString>::iterator ite = strList.begin();
		++ite;
		for (; ite != strList.end(); ++ite)
		{
			res.push_back(CDBModel(*ite));
		}
	}
	return res;
}



