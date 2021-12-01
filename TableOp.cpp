#include "pch.h"
#include "TableOp.h"

#include"FileOp.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <string>
using namespace std;


TableOp::TableOp() {
}

vector<TableModel> TableOp::GetTableMess(CString& dbmName) {
	CString dbmPath = CString("dbms_root/data/") + dbmName + CString("/") + dbmName + CString(".tb");
	vector<CString> tables = FileOp::ReadAll(dbmPath);
	vector<TableModel> tablelist;
	int num = tables.size();
	for (int i = 1; i < num; i++)
	{
		tablelist.push_back(TableModel(tables[i]));

	}
	return tablelist;


}

int TableOp::CreateTable(CString& tableName, CString& dbmName) {
	vector<TableModel> tablelist = GetTableMess(dbmName);
	vector<TableModel>::iterator it = tablelist.begin();
	int repeat = 0;
	for (; it != tablelist.end(); ++it)
	{
		if (it->name == tableName) {
			MessageBox(NULL, CString("该表已存在！"), CString("错误"), MB_OK | MB_ICONWARNING);
			repeat = 1;
			break;
		}

	}
	if (repeat != 1) {

		int id = 1;
		if (tablelist.size() != 0)id = tablelist[tablelist.size() - 1].id + 1;
		CString folderPath = CString("./dbms_root/data/") + dbmName;

		CString tablePath = folderPath + CString("/") + dbmName + CString(".tb");//表描述性文件


//在数据库文件夹下创建表定义文件 表名.tdf
		CString tdfPath = folderPath + CString("/") + tableName + CString(".tdf");
		CFile file1(tdfPath, CFile::modeCreate);
		file1.Close();
		//在数据库文件夹下创建表完整性文件 表名.tic
		CString ticPath = folderPath + CString("/") + tableName + CString(".tic");
		CFile file2(ticPath, CFile::modeCreate);
		file2.Close();
		//在数据库文件夹下创建表定义文件 表名.trd
		CString trdPath = folderPath + CString("/") + tableName + CString(".trd");
		CFile file3(trdPath, CFile::modeCreate);
		file3.Close();
		//在数据库文件夹下创建表定义文件 表名.tid
		CString tidPath = folderPath + CString("/") + tableName + CString(".tid");
		CFile file4(tidPath, CFile::modeCreate);
		file4.Close();

		CString str_time;
		str_time = FileOp::GetCurrTime();

		CString coln;
		coln.Format(L"%d", 0);
		CString rcdn;
		rcdn.Format(L"%d", 0);

		CString message;
		CString str_id;
		str_id.Format(_T("%d"), id);
		message = str_id + _T("#") + tableName + _T("#") + tablePath + _T("#") + str_time + _T("#") + str_time + _T("#") + coln + _T("#") + rcdn + _T("#") + dbmName;
		FileOp::AddAnLine(tablePath, message);

	}


	return 0;
}

int TableOp::DropTable(CString& tableName, CString& dbmName) {
	vector<TableModel> tablelist = GetTableMess(dbmName);
	vector<TableModel>::iterator it = tablelist.begin();
	int del = 0;
	CString dbmPath = CString("dbms_root/data/") + dbmName + CString("/") + dbmName + CString(".tb");;
	vector<CString> db = FileOp::ReadAll(dbmPath);
	for (; it != tablelist.end(); ++it)
	{
		if (it->name == tableName) {
			CString folderPath = CString("dbms_root/data") + CString("/") + dbmName;
			CString tdfPath = folderPath + CString("/") + tableName + CString(".tdf");
			CString ticPath = folderPath + CString("/") + tableName + CString(".tic");
			CString trdPath = folderPath + CString("/") + tableName + CString(".trd");
			CString tidPath = folderPath + CString("/") + tableName + CString(".tid");
			DeleteFile(tdfPath);
			DeleteFile(ticPath);
			DeleteFile(trdPath);
			DeleteFile(tidPath);
			del = 1;
			break;
		}

	}
	if (del == 0) {
		MessageBox(NULL, CString("该表不存在！删除失败！"), CString("错误"), MB_OK | MB_ICONWARNING);
	}
	else {
		vector<CString>::iterator it1 = db.begin();
		for (; it1 != db.end(); ++it1) {
			vector<CString> temp = FileOp::StrSplit(*it1, CString("#"));
			if (temp[1] == tableName) {
				db.erase(it1);
				break;
			}
		}
		FileOp::WriteRecord(dbmPath, db);
		MessageBox(NULL, CString("删除成功！"), CString("消息"), MB_OK | MB_ICONWARNING);

	}
	return 0;

}
//重命名表
int TableOp::AlterTable(CString& currentName, CString& tableName, CString& dbmName) {
	vector<TableModel> tablelist = GetTableMess(dbmName);
	vector<TableModel>::iterator it = tablelist.begin();
	int repeat = 0;
	for (; it != tablelist.end(); ++it)
	{
		if (it->name == tableName) {
			MessageBox(NULL, CString("该表已存在！"), CString("错误"), MB_OK | MB_ICONWARNING);
			repeat = 1;
			return 0;
		}

	}
	if (repeat != 1) {
		//修改tb文件
		CString dbmPath = CString("dbms_root/data/") + dbmName + CString("/") + dbmName + CString(".tb");;
		vector<CString> tablelist = FileOp::ReadAll(dbmPath);
		CString message;
		CString etime = FileOp::GetCurrTime();
		int num = tablelist.size();
		for (int i = 0; i < num; i++) {
			vector<CString> temp = FileOp::StrSplit(tablelist[i], CString("#"));
			if (temp[1] == currentName) {
				temp[1] = tableName;
				temp[4] = FileOp::GetCurrTime();
				tablelist[i] = temp[0] + CString("#") + temp[1] + CString("#") + temp[2] + CString("#") + temp[3] + CString("#") + temp[4] + CString("#") + temp[5] + CString("#") + temp[6] + CString("#") + temp[7];
				break;
			}
		}

		FileOp::WriteRecord(dbmPath, tablelist);


		CString folderPath = CString("dbms_root/data/") + dbmName;
		CString tdfPath = folderPath + CString("/") + currentName + CString(".tdf");
		CString ticPath = folderPath + CString("/") + currentName + CString(".tic");
		CString trdPath = folderPath + CString("/") + currentName + CString(".trd");
		CString tidPath = folderPath + CString("/") + currentName + CString(".tid");

		CString n_tdfPath = folderPath + CString("/") + tableName + CString(".tdf");
		CString n_ticPath = folderPath + CString("/") + tableName + CString(".tic");
		CString n_trdPath = folderPath + CString("/") + tableName + CString(".trd");
		CString n_tidPath = folderPath + CString("/") + tableName + CString(".tid");
		//文件重命名
		CFile::Rename(tdfPath, n_tdfPath);
		CFile::Rename(ticPath, n_ticPath);
		CFile::Rename(trdPath, n_trdPath);
		CFile::Rename(tidPath, n_tidPath);

		MessageBox(NULL, CString("重命名成功！"), CString("消息"), MB_OK | MB_ICONWARNING);
		return 1;
	}
}