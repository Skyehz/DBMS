#include "pch.h"
#include "TableModel.h"

#include "FileOp.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>


#include <vector>
using namespace std;

TableModel::TableModel(CString& str) {
	vector<CString> s = FileOp::StrSplit(str, _T("#"));

	id = _ttoi(s[0]);
	name = s[1];
	dbmspath = s[2];
	CreateTime = s[3];
	EditTime = s[4];
	col_num = _ttoi(s[5]);
	record_num = _ttoi(s[6]);
	dbmName = s[7];

}

TableModel::~TableModel() {
}
