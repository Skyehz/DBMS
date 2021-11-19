#include "pch.h"
#include "DBOp.h"
#include"FileOp.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <string>
using namespace std;

int CDBOp::InitDatabase() {
	//构建数据库dbms__root 文件夹
	CString folderPath = CString("./dbms_root");
	bool crtFolder = CreateDirectory(folderPath, NULL);

	return 0;
}

int CDBOp::CreateDatabase(CString& dbName) {
	//创建数据库实体
	CDBModel db(dbName);

	//为该数据库创建文件夹
	CString folderPath = CString("./dbms_root/") + CString("/") + dbName;
	bool crtFolder = CreateDirectory(folderPath, NULL);


	//在该文件夹下创建数据库描述文件： 数据库名.db文件
	CString dbPath = CString("./dbms_root/") + CString("/") + dbName + CString(".db");
	vector<CString> init;
	init.push_back(CString("0#") + dbName);
	bool crtDBFile = FileOp::WriteRecord(dbPath, init);

	//在该文件夹下创建表描述文件： 数据库名.tb文件
	CString tbPath = folderPath + CString("/") + dbName + CString(".tb");
	init.push_back(CString("0#") + dbName);
	bool crtDBFiletb = FileOp::WriteRecord(tbPath, init);

	//在该文件夹下创建日志文件： 数据库名.log文件
	init.push_back(CString("0#") + dbName);
	CString logPath = folderPath + CString("/") + dbName + CString(".log");
	/*FileOp f;
	f.WriteLog(CString("created database:") + dbName);*/
	bool crtDBFilelog = FileOp::WriteRecord(logPath, init);

	return 0;
}