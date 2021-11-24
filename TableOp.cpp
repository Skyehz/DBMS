#include "pch.h"
#include "TableOp.h"
#include"FileOp.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <string>
using namespace std;


int TableOp::CreateTable(CString& tableName, CString& dbmName) {
	TableModel table(tableName,dbmName);
	CString folderPath = CString("dbms_root/data") + CString("/") + dbmName;

	CString tablePath =folderPath+CString("/") + tableName + CString(".tb");//表描述性文件
	vector<CString> init;
	init.push_back(CString("dbmName:") + dbmName+CString("\n"));
	bool crtTableFile = FileOp::WriteRecord(tablePath, init);

	init.push_back(CString("TableName:") + tableName);
	crtTableFile=FileOp::WriteRecord(tablePath, init);


	//在数据库文件夹下创建表定义文件 表名.tdf
	CString tdfPath = folderPath + CString("/") + tableName + CString(".tdf");
	table.tdfpath = tdfPath;
	init.push_back(CString("#0") + tableName);
	bool crtTabletdf = FileOp::WriteRecord(tdfPath, init);
	//在数据库文件夹下创建表完整性文件 表名.tic
	CString ticPath = folderPath + CString("/") + tableName + CString(".tic");
	table.ticpath = ticPath;
	init.push_back(CString("#0") + tableName);
	bool crtTabletic = FileOp::WriteRecord(ticPath, init);
	//在数据库文件夹下创建表定义文件 表名.trd
	CString trdPath = folderPath + CString("/") + tableName + CString(".trd");
	table.trdpath = trdPath;
	init.push_back(CString("#0") + tableName);
	bool crtTabletrd = FileOp::WriteRecord(trdPath, init);
	//在数据库文件夹下创建表定义文件 表名.tid
	CString tidPath = folderPath + CString("/") + tableName + CString(".tid");
	table.tidpath = tidPath;
	init.push_back(CString("#0") + tableName);
	bool crtTabletid = FileOp::WriteRecord(tidPath, init);

	init.push_back(tdfPath);
	bool crtTableFiletb = FileOp::WriteRecord(tablePath, init);
	init.push_back(ticPath);
	crtTableFiletb = FileOp::WriteRecord(tablePath, init);
	init.push_back(trdPath);
	crtTableFiletb = FileOp::WriteRecord(tablePath, init);
	init.push_back(tidPath);
	crtTableFiletb = FileOp::WriteRecord(tablePath, init);

	init.push_back(CString("createtime:") + table.CreateTime);
	crtTableFiletb = FileOp::WriteRecord(tablePath, init);
	init.push_back(CString("edittime:") + table.EditTime);
	crtTableFiletb = FileOp::WriteRecord(tablePath, init);

	CString coln;
	coln.Format(L"%d", table.col_num);
	init.push_back(CString("column number:") + coln);
	crtTableFiletb = FileOp::WriteRecord(tablePath, init);
	CString rcdn;
	rcdn.Format(L"%d", table.record_num);
	init.push_back(CString("record number:") +rcdn);
	crtTableFiletb = FileOp::WriteRecord(tablePath, init);

	return 0;
}

int DropTable(CString& tableName, CString& dbmName) {
	CString folderPath = CString("dbms_root/data") + CString("/") + dbmName;
	CString tablePath = folderPath + CString("/") + tableName + CString(".tb");
	CString tdfPath = folderPath + CString("/") + tableName + CString(".tdf");
	CString ticPath = folderPath + CString("/") + tableName + CString(".tic");
	CString trdPath = folderPath + CString("/") + tableName + CString(".trd");
	CString tidPath = folderPath + CString("/") + tableName + CString(".tid");
	DeleteFile(tablePath);
	DeleteFile(tdfPath);
	DeleteFile(ticPath);
	DeleteFile(trdPath);
	DeleteFile(tidPath);

	return 0;

}
int AlterTable(CString& tableName, CString& dbmName, int col_num, int record_num) {
	return 0;
}