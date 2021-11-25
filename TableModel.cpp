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

TableModel::TableModel() {
	id = 0;
	dbmspath = "dbms_root/data/db1";
	col_num = 0;
	record_num = 0;
	name = "t1";
	CString str_time;
	CTime tm; tm = CTime::GetCurrentTime();
	str_time = tm.Format("%Y/%m/%d %X");
	dbmName = "db1";

	CreateTime = str_time;
	EditTime = str_time;

}

TableModel::~TableModel() {
}