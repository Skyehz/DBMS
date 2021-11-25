#include "pch.h"
#include "TableOp.h"
#include"FileOp.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <string>
using namespace std;

TableOp::TableOp(){
	this->table  = new TableModel();

}

int TableOp::CreateTable(CString& tableName, CString& dbmName) {
	CString folderPath = CString("dbms_root/data") + CString("/") + dbmName;

	CString tablePath =folderPath+CString("/") + tableName + CString(".tb");//表描述性文件
	vector<CString> init;
	init.push_back(CString("dbmName:") + dbmName+CString("\n"));
	bool crtTableFile = FileOp::WriteRecord(tablePath, init);
	this->table->name = tableName;
	init.push_back(CString("TableName:") + tableName + CString("\n"));
	crtTableFile=FileOp::WriteRecord(tablePath, init);
	this->table->dbmName = dbmName;


	//在数据库文件夹下创建表定义文件 表名.tdf
	CString tdfPath = folderPath + CString("/") + tableName + CString(".tdf");
	this->table->tdfpath = tdfPath;
	init.push_back(CString("#0") + tableName);
	bool crtTabletdf = FileOp::WriteRecord(tdfPath, init);
	//在数据库文件夹下创建表完整性文件 表名.tic
	CString ticPath = folderPath + CString("/") + tableName + CString(".tic");
	this->table->ticpath = ticPath;
	init.push_back(CString("#0") + tableName);
	bool crtTabletic = FileOp::WriteRecord(ticPath, init);
	//在数据库文件夹下创建表定义文件 表名.trd
	CString trdPath = folderPath + CString("/") + tableName + CString(".trd");
	this->table->trdpath = trdPath;
	init.push_back(CString("#0") + tableName);
	bool crtTabletrd = FileOp::WriteRecord(trdPath, init);
	//在数据库文件夹下创建表定义文件 表名.tid
	CString tidPath = folderPath + CString("/") + tableName + CString(".tid");
	this->table->tidpath = tidPath;
	init.push_back(CString("#0") + tableName);
	bool crtTabletid = FileOp::WriteRecord(tidPath, init);

	init.push_back(tdfPath + CString("\n"));
	bool crtTableFiletb = FileOp::WriteRecord(tablePath, init);
	init.push_back(ticPath + CString("\n"));
	crtTableFiletb = FileOp::WriteRecord(tablePath, init);
	init.push_back(trdPath + CString("\n"));
	crtTableFiletb = FileOp::WriteRecord(tablePath, init);
	init.push_back(tidPath + CString("\n"));
	crtTableFiletb = FileOp::WriteRecord(tablePath, init);

	init.push_back(CString("createtime:") + this->table->CreateTime + CString("\n"));
	crtTableFiletb = FileOp::WriteRecord(tablePath, init);
	init.push_back(CString("edittime:") + this->table->EditTime+CString("\n"));
	crtTableFiletb = FileOp::WriteRecord(tablePath, init);

	CString coln;
	coln.Format(L"%d", this->table->col_num);
	init.push_back(CString("column number:") + coln + CString("\n"));
	crtTableFiletb = FileOp::WriteRecord(tablePath, init);
	CString rcdn;
	rcdn.Format(L"%d", this->table->record_num);
	init.push_back(CString("record number:") +rcdn + CString("\n"));
	crtTableFiletb = FileOp::WriteRecord(tablePath, init);


	return 0;
}

int TableOp::DropTable(CString& tableName, CString& dbmName) {
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
int TableOp::AlterTable(CString& tableName, CString& dbmName, int col_num, int record_num) {
	DropTable(this->table->name, this->table->dbmName);
	this->table->name = tableName;
	this->table->dbmName = dbmName;
	this->table->col_num = col_num;
	this->table->record_num = record_num;


	CString str_time;
	CTime tm; tm = CTime::GetCurrentTime();
	str_time = tm.Format("%Y/%m/%d %X");
	this->table->EditTime = str_time;

	//将修改后的信息重新写入文件
	CString folderPath = CString("dbms_root/data") + CString("/") + dbmName;

	CString tablePath = folderPath + CString("/") + tableName + CString(".tb");//表描述性文件
	vector<CString> init;
	init.push_back(CString("#dbmName:") + dbmName + CString("\n"));
	bool crtTableFile = FileOp::WriteRecord(tablePath, init);

	init.push_back(CString("TableName:") + tableName + CString("\n"));
	crtTableFile = FileOp::WriteRecord(tablePath, init);


	//在数据库文件夹下创建表定义文件 表名.tdf
	CString tdfPath = folderPath + CString("/") + tableName + CString(".tdf");
	this->table->tdfpath = tdfPath;
	init.push_back(CString("#0") + tableName);
	bool crtTabletdf = FileOp::WriteRecord(tdfPath, init);
	//在数据库文件夹下创建表完整性文件 表名.tic
	CString ticPath = folderPath + CString("/") + tableName + CString(".tic");
	this->table->ticpath = ticPath;
	init.push_back(CString("#0") + tableName);
	bool crtTabletic = FileOp::WriteRecord(ticPath, init);
	//在数据库文件夹下创建表定义文件 表名.trd
	CString trdPath = folderPath + CString("/") + tableName + CString(".trd");
	this->table->trdpath = trdPath;
	init.push_back(CString("#0") + tableName);
	bool crtTabletrd = FileOp::WriteRecord(trdPath, init);
	//在数据库文件夹下创建表定义文件 表名.tid
	CString tidPath = folderPath + CString("/") + tableName + CString(".tid");
	this->table->tidpath = tidPath;
	init.push_back(CString("#0") + tableName);
	bool crtTabletid = FileOp::WriteRecord(tidPath, init);

	init.push_back(tdfPath + CString("\n"));
	bool crtTableFiletb = FileOp::WriteRecord(tablePath, init);
	init.push_back(ticPath + CString("\n"));
	crtTableFiletb = FileOp::WriteRecord(tablePath, init);
	init.push_back(trdPath + CString("\n"));
	crtTableFiletb = FileOp::WriteRecord(tablePath, init);
	init.push_back(tidPath + CString("\n"));
	crtTableFiletb = FileOp::WriteRecord(tablePath, init);

	init.push_back(CString("createtime:") + this->table->CreateTime + CString("\n"));
	crtTableFiletb = FileOp::WriteRecord(tablePath, init);
	init.push_back(CString("edittime:") + this->table->EditTime + CString("\n"));
	crtTableFiletb = FileOp::WriteRecord(tablePath, init);

	CString coln;
	coln.Format(L"%d", this->table->col_num);
	init.push_back(CString("column number:") + coln + CString("\n"));
	crtTableFiletb = FileOp::WriteRecord(tablePath, init);
	CString rcdn;
	rcdn.Format(L"%d", this->table->record_num);
	init.push_back(CString("record number:") + rcdn + CString("\n"));
	crtTableFiletb = FileOp::WriteRecord(tablePath, init);



	return 0;
}