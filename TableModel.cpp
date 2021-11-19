#include "pch.h"
#include "TableModel.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>


#include <vector>
using namespace std;
//TableModel::TableModel(int id, CString name, CString ticpath, CString trdpath, CString tidpath, CString tdfpath, CString CreateTime, CString EditTime, int col_num, int record_num, int db_id)
//{
//	auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
//}

TableModel::TableModel(CString& str, CString& dbmName) {
	id = 0;
	name = str;
	dbmspath = "dbms_root/data/db1";
	col_num = 0;
	record_num = 0;
	dbmName = dbmName;
	/*	auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		//תΪstring->CString
		std::stringstream ss;
		ss << std::put_time(std::localtime(&t), "%Y/%m%/d %H:%M:%S");
		std::string str_time = ss.str();*/
	string str_time = "2021/11/15 22:46";
	CreateTime = str_time.c_str();
	string edit_time = "2021/11/15 23:50";
	EditTime = edit_time.c_str();

}

TableModel::~TableModel() {
}