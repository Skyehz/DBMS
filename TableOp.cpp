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

	CString tablePath =folderPath+CString("/") + tableName + CString(".tb");//���������ļ�
	vector<CString> init;
	init.push_back(CString("dbmName:") + dbmName+CString("\n"));
	bool crtTableFile = FileOp::WriteRecord(tablePath, init);
	this->table->name = tableName;
	init.push_back(CString("TableName:") + tableName + CString("\n"));
	crtTableFile=FileOp::WriteRecord(tablePath, init);
	this->table->dbmName = dbmName;


	//�����ݿ��ļ����´��������ļ� ����.tdf
	CString tdfPath = folderPath + CString("/") + tableName + CString(".tdf");
	this->table->tdfpath = tdfPath;
	init.push_back(CString("#0") + tableName);
	bool crtTabletdf = FileOp::WriteRecord(tdfPath, init);
	//�����ݿ��ļ����´������������ļ� ����.tic
	CString ticPath = folderPath + CString("/") + tableName + CString(".tic");
	this->table->ticpath = ticPath;
	init.push_back(CString("#0") + tableName);
	bool crtTabletic = FileOp::WriteRecord(ticPath, init);
	//�����ݿ��ļ����´��������ļ� ����.trd
	CString trdPath = folderPath + CString("/") + tableName + CString(".trd");
	this->table->trdpath = trdPath;
	init.push_back(CString("#0") + tableName);
	bool crtTabletrd = FileOp::WriteRecord(trdPath, init);
	//�����ݿ��ļ����´��������ļ� ����.tid
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

	//���޸ĺ����Ϣ����д���ļ�
	CString folderPath = CString("dbms_root/data") + CString("/") + dbmName;

	CString tablePath = folderPath + CString("/") + tableName + CString(".tb");//���������ļ�
	vector<CString> init;
	init.push_back(CString("#dbmName:") + dbmName + CString("\n"));
	bool crtTableFile = FileOp::WriteRecord(tablePath, init);

	init.push_back(CString("TableName:") + tableName + CString("\n"));
	crtTableFile = FileOp::WriteRecord(tablePath, init);


	//�����ݿ��ļ����´��������ļ� ����.tdf
	CString tdfPath = folderPath + CString("/") + tableName + CString(".tdf");
	this->table->tdfpath = tdfPath;
	init.push_back(CString("#0") + tableName);
	bool crtTabletdf = FileOp::WriteRecord(tdfPath, init);
	//�����ݿ��ļ����´������������ļ� ����.tic
	CString ticPath = folderPath + CString("/") + tableName + CString(".tic");
	this->table->ticpath = ticPath;
	init.push_back(CString("#0") + tableName);
	bool crtTabletic = FileOp::WriteRecord(ticPath, init);
	//�����ݿ��ļ����´��������ļ� ����.trd
	CString trdPath = folderPath + CString("/") + tableName + CString(".trd");
	this->table->trdpath = trdPath;
	init.push_back(CString("#0") + tableName);
	bool crtTabletrd = FileOp::WriteRecord(trdPath, init);
	//�����ݿ��ļ����´��������ļ� ����.tid
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