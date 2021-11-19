#include "pch.h"
#include "RecordManage.h"
#include "DBModel.h"
#include <vector>
#include"FileOp.h"
#include <iostream>
using namespace std;


int RecordManage::insert(CString& dbName)
{
	//输入
	CString message;
	message = "insert into ...values...";
	//处理(创建.trd文件）
	//创建数据库实体
	CDBModel db(dbName);
	//为该数据库创建文件夹
	CString folderPath = CString("dbms_root/data") + CString("/") + dbName;
	vector<CString> init;
	init.push_back(message + dbName);
	CString logPath = folderPath + CString("/") + dbName + CString(".txt");
	/*FileOp f;
	f.WriteLog(CString("created database:") + dbName);*/
	bool crtDBFilelog = FileOp::WriteRecord(logPath, init);
	//输出
	return 0;
}
